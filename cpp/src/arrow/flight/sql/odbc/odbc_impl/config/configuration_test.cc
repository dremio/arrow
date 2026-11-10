// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

// flight_sql_connection.h needs to be included first due to conflicts with windows.h
#include "arrow/flight/sql/odbc/odbc_impl/flight_sql_connection.h"

#include "arrow/flight/sql/odbc/odbc_impl/config/configuration.h"

#include <algorithm>
#include <set>
#include <string>
#include <string_view>

#include <boost/algorithm/string/predicate.hpp>
#include "gtest/gtest.h"

namespace arrow::flight::sql::odbc {
namespace config {

namespace {

bool ContainsKeyIgnoreCase(const std::vector<std::string_view>& keys,
                           std::string_view key) {
  return std::any_of(keys.begin(), keys.end(), [&](std::string_view candidate) {
    return boost::iequals(candidate, key);
  });
}

std::set<std::string> ToSet(const std::vector<std::string>& keys) {
  return std::set<std::string>(keys.begin(), keys.end());
}

// Known keys that do not have a dedicated field in the Windows DSN dialog.
const std::vector<std::string_view> kKeysWithoutDialogField = {
    FlightSqlConnection::STRING_COLUMN_LENGTH, FlightSqlConnection::USE_WIDE_CHAR,
    FlightSqlConnection::CHUNK_BUFFER_CAPACITY};

}  // namespace

TEST(DsnDialogKeysTest, DialogKeysAreKnownKeys) {
  for (const auto& key : FlightSqlConnection::DSN_DIALOG_KEYS) {
    EXPECT_TRUE(ContainsKeyIgnoreCase(FlightSqlConnection::ALL_KEYS, key))
        << key << " is in DSN_DIALOG_KEYS but not in ALL_KEYS";
  }
}

TEST(DsnDialogKeysTest, KeysWithoutDialogFieldAreNotDialogKeys) {
  for (const auto& key : kKeysWithoutDialogField) {
    EXPECT_TRUE(ContainsKeyIgnoreCase(FlightSqlConnection::ALL_KEYS, key)) << key;
    EXPECT_FALSE(ContainsKeyIgnoreCase(FlightSqlConnection::DSN_DIALOG_KEYS, key))
        << key << " has no field in the DSN dialog and must not be in DSN_DIALOG_KEYS";
  }
}

// Fails when a key is added to ALL_KEYS without deciding whether it gets its own field in
// the DSN dialog. Add it to DSN_DIALOG_KEYS if it does, or to kKeysWithoutDialogField
// above if it should be listed in the dialog's "Advanced properties" table.
TEST(DsnDialogKeysTest, EveryKnownKeyIsClassified) {
  for (const auto& key : FlightSqlConnection::ALL_KEYS) {
    const bool in_dialog =
        ContainsKeyIgnoreCase(FlightSqlConnection::DSN_DIALOG_KEYS, key);
    const bool in_advanced = ContainsKeyIgnoreCase(kKeysWithoutDialogField, key);
    EXPECT_NE(in_dialog, in_advanced)
        << key << " must be in exactly one of DSN_DIALOG_KEYS and the advanced keys";
  }
}

TEST(ConfigurationTest, GetCustomKeysEmptyWhenOnlyDialogKeysSet) {
  Configuration config;
  config.Set(FlightSqlConnection::DSN, "test dsn");
  config.Set(FlightSqlConnection::DRIVER, "test driver");
  config.Set(FlightSqlConnection::HOST, "localhost");
  config.Set(FlightSqlConnection::PORT, "32010");
  config.Set(FlightSqlConnection::TOKEN, "token");
  config.Set(FlightSqlConnection::UID, "user");
  config.Set(FlightSqlConnection::PWD, "password");
  config.Set(FlightSqlConnection::USE_ENCRYPTION, "true");
  config.Set(FlightSqlConnection::TRUSTED_CERTS, "/path/to/cert");
  config.Set(FlightSqlConnection::USE_SYSTEM_TRUST_STORE, "false");
  config.Set(FlightSqlConnection::DISABLE_CERTIFICATE_VERIFICATION, "false");

  EXPECT_TRUE(config.GetCustomKeys().empty());
}

// Regression test for the DSN dialog silently dropping StringColumnLength, UseWideChar
// and ChunkBufferCapacity: they have no dedicated field, so they must be reported as
// custom keys for the "Advanced properties" table.
TEST(ConfigurationTest, GetCustomKeysIncludesKeysWithoutDialogField) {
  Configuration config;
  config.Set(FlightSqlConnection::HOST, "localhost");
  config.Set(FlightSqlConnection::PORT, "32010");
  config.Set(FlightSqlConnection::STRING_COLUMN_LENGTH, "1024");
  config.Set(FlightSqlConnection::USE_WIDE_CHAR, "true");
  config.Set(FlightSqlConnection::CHUNK_BUFFER_CAPACITY, "10");

  const std::set<std::string> expected = {
      std::string(FlightSqlConnection::STRING_COLUMN_LENGTH),
      std::string(FlightSqlConnection::USE_WIDE_CHAR),
      std::string(FlightSqlConnection::CHUNK_BUFFER_CAPACITY)};
  EXPECT_EQ(expected, ToSet(config.GetCustomKeys()));
}

TEST(ConfigurationTest, GetCustomKeysIncludesUnknownKeys) {
  Configuration config;
  config.Set(FlightSqlConnection::HOST, "localhost");
  config.Set(FlightSqlConnection::STRING_COLUMN_LENGTH, "1024");
  config.Set("MyCustomKey", "custom value");

  const std::set<std::string> expected = {
      std::string(FlightSqlConnection::STRING_COLUMN_LENGTH), "MyCustomKey"};
  EXPECT_EQ(expected, ToSet(config.GetCustomKeys()));
}

TEST(ConfigurationTest, GetCustomKeysExcludesDialogKeysCaseInsensitively) {
  Configuration config;
  config.Set("HOST", "localhost");
  config.Set("Port", "32010");
  config.Set("USEENCRYPTION", "true");
  config.Set("stringcolumnlength", "1024");

  const std::set<std::string> expected = {"stringcolumnlength"};
  EXPECT_EQ(expected, ToSet(config.GetCustomKeys()));
}

}  // namespace config
}  // namespace arrow::flight::sql::odbc
