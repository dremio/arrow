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

#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "arrow/util/string.h"
#include "gtest/gtest.h"

namespace arrow::flight::sql::odbc {
namespace config {

namespace {

bool ContainsKeyIgnoreCase(const std::vector<std::string_view>& keys,
                           std::string_view key) {
  for (const auto& candidate : keys) {
    if (arrow::internal::AsciiEqualsCaseInsensitive(candidate, key)) {
      return true;
    }
  }
  return false;
}

std::set<std::string> ToSet(const std::vector<std::string>& keys) {
  return std::set<std::string>(keys.begin(), keys.end());
}

// Known keys that do not have a dedicated field in the Windows DSN dialog.
const std::vector<std::string_view> kKeysWithoutDialogField = {
    FlightSqlConnection::STRING_COLUMN_LENGTH, FlightSqlConnection::USE_WIDE_CHAR,
    FlightSqlConnection::CHUNK_BUFFER_CAPACITY};

std::vector<std::string> KnownKeysWithoutDialogField() {
  std::vector<std::string> keys;
  for (const auto& key : FlightSqlConnection::ALL_KEYS) {
    if (!ContainsKeyIgnoreCase(FlightSqlConnection::DSN_DIALOG_KEYS, key)) {
      keys.emplace_back(key);
    }
  }
  return keys;
}

void SetDialogKeys(Configuration& config) {
  config.Set(FlightSqlConnection::DSN, "test dsn");
  config.Set(FlightSqlConnection::DRIVER, "test driver");
  config.Set(FlightSqlConnection::HOST, "localhost");
  config.Set(FlightSqlConnection::PORT, "32010");
  config.Set(FlightSqlConnection::TOKEN, "token");
  config.Set(FlightSqlConnection::UID, "user");
  config.Set(FlightSqlConnection::USER_ID, "user");
  config.Set(FlightSqlConnection::PWD, "password");
  config.Set(FlightSqlConnection::USE_ENCRYPTION, "true");
  config.Set(FlightSqlConnection::TRUSTED_CERTS, "/path/to/cert");
  config.Set(FlightSqlConnection::USE_SYSTEM_TRUST_STORE, "false");
  config.Set(FlightSqlConnection::DISABLE_CERTIFICATE_VERIFICATION, "false");
}

void SetKeysWithoutDialogField(Configuration& config) {
  config.Set(FlightSqlConnection::STRING_COLUMN_LENGTH, "1024");
  config.Set(FlightSqlConnection::USE_WIDE_CHAR, "true");
  config.Set(FlightSqlConnection::CHUNK_BUFFER_CAPACITY, "10");
}

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

// Every key in ALL_KEYS must be classified. If you add a key, put it in DSN_DIALOG_KEYS
// when it gets its own field in the DSN dialog, or in kKeysWithoutDialogField when it
// should appear in the dialog's "Advanced properties" table.
TEST(DsnDialogKeysTest, EveryKnownKeyIsClassified) {
  for (const auto& key : FlightSqlConnection::ALL_KEYS) {
    const bool in_dialog =
        ContainsKeyIgnoreCase(FlightSqlConnection::DSN_DIALOG_KEYS, key);
    const bool in_advanced = ContainsKeyIgnoreCase(kKeysWithoutDialogField, key);
    EXPECT_NE(in_dialog, in_advanced)
        << key << " must be in exactly one of DSN_DIALOG_KEYS and the advanced keys";
  }
}

TEST(ConfigurationTest, TestHelpersSetEveryKnownKey) {
  Configuration config;
  SetDialogKeys(config);
  SetKeysWithoutDialogField(config);

  for (const auto& key : FlightSqlConnection::ALL_KEYS) {
    EXPECT_TRUE(config.IsSet(key)) << "Add a value for " << key << " to the test helpers";
  }
}

TEST(ConfigurationTest, GetCustomKeysEmptyWhenOnlyDialogKeysSet) {
  Configuration config;
  SetDialogKeys(config);

  for (const auto& key : FlightSqlConnection::DSN_DIALOG_KEYS) {
    EXPECT_TRUE(config.IsSet(key)) << "SetDialogKeys does not set " << key;
  }
  EXPECT_TRUE(config.GetCustomKeys().empty());
}

// Regression test for the DSN dialog silently dropping StringColumnLength, UseWideChar
// and ChunkBufferCapacity: they have no dedicated field, so they must be reported as
// custom keys for the "Advanced properties" table.
TEST(ConfigurationTest, GetCustomKeysIncludesKeysWithoutDialogField) {
  Configuration config;
  SetDialogKeys(config);
  SetKeysWithoutDialogField(config);

  const auto keys_without_dialog_field = KnownKeysWithoutDialogField();
  const std::set<std::string> expected(keys_without_dialog_field.begin(),
                                       keys_without_dialog_field.end());
  EXPECT_EQ(expected, ToSet(config.GetCustomKeys()));
  EXPECT_FALSE(expected.empty());
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
  SetDialogKeys(config);
  SetKeysWithoutDialogField(config);

  Configuration upper_case_config;
  for (const auto& [key, value] : config.GetProperties()) {
    upper_case_config.Set(arrow::internal::AsciiToUpper(key), value);
  }

  std::set<std::string> expected;
  for (const auto& key : KnownKeysWithoutDialogField()) {
    expected.insert(arrow::internal::AsciiToUpper(key));
  }
  EXPECT_EQ(expected, ToSet(upper_case_config.GetCustomKeys()));
}

}  // namespace config
}  // namespace arrow::flight::sql::odbc
