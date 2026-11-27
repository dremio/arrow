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

#include "arrow/flight/sql/odbc/tests/odbc_test_suite.h"

#include "arrow/flight/sql/odbc/odbc_impl/platform.h"
#include "arrow/flight/sql/odbc/odbc_impl/util.h"

#include <sql.h>
#include <sqltypes.h>
#include <sqlucode.h>

// -AL- review this file, ask Claude to list all new tests that have been added
// in this branch, so I know which tests to pay attention to.
#include <algorithm>

#include <gtest/gtest.h>

namespace arrow::flight::sql::odbc {

template <typename T>
class ConnectionTest : public T {};

using TestTypes =
    ::testing::Types<FlightSQLODBCMockTestBase, FlightSQLODBCRemoteTestBase>;
TYPED_TEST_SUITE(ConnectionTest, TestTypes);

template <typename T>
class ConnectionHandleTest : public T {};

class ConnectionRemoteTest : public FlightSQLOdbcEnvConnHandleRemoteTestBase {};
using TestTypesHandle = ::testing::Types<FlightSQLOdbcEnvConnHandleMockTestBase,
                                         FlightSQLOdbcEnvConnHandleRemoteTestBase>;
TYPED_TEST_SUITE(ConnectionHandleTest, TestTypesHandle);

TEST(ODBCHandles, TestSQLAllocAndFreeEnv) {
  // Allocate an environment handle
  SQLHENV env;
  ASSERT_EQ(SQL_SUCCESS, SQLAllocEnv(&env));

  // Check for valid handle
  ASSERT_NE(nullptr, env);

  // Free environment handle
  ASSERT_EQ(SQL_SUCCESS, SQLFreeEnv(env));
}

TEST(ODBCHandles, TestSQLAllocAndFreeHandleConnect) {
  SQLHENV env;
  SQLHDBC conn;

  // Allocate an environment handle
  ASSERT_EQ(SQL_SUCCESS, SQLAllocEnv(&env));

  // Allocate a connection using alloc handle
  ASSERT_EQ(SQL_SUCCESS, SQLAllocHandle(SQL_HANDLE_DBC, env, &conn));

  // Check for valid handle
  ASSERT_NE(nullptr, conn);

  // Free the created connection using free handle
  ASSERT_EQ(SQL_SUCCESS, SQLFreeHandle(SQL_HANDLE_DBC, conn));

  // Free environment handle
  ASSERT_EQ(SQL_SUCCESS, SQLFreeHandle(SQL_HANDLE_ENV, env));
}

TEST(ODBCHandles, TestSQLAllocAndFreeConnect) {
  SQLHENV env;
  SQLHDBC conn;

  // Allocate an environment handle
  ASSERT_EQ(SQL_SUCCESS, SQLAllocEnv(&env));

  // Allocate a connection using alloc handle
  ASSERT_EQ(SQL_SUCCESS, SQLAllocConnect(env, &conn));

  // Check for valid handle
  ASSERT_NE(nullptr, conn);

  // Free the created connection using free connect
  ASSERT_EQ(SQL_SUCCESS, SQLFreeConnect(conn));

  // Free environment handle
  ASSERT_EQ(SQL_SUCCESS, SQLFreeEnv(env));
}

TEST(ODBCHandles, TestFreeNullHandles) {
  SQLHENV env = NULL;
  SQLHDBC conn = NULL;
  SQLHSTMT stmt = NULL;

  // Verifies attempt to free invalid handle does not cause segfault
  // Attempt to free null statement handle
  ASSERT_EQ(SQL_INVALID_HANDLE, SQLFreeHandle(SQL_HANDLE_STMT, stmt));

  // Attempt to free null connection handle
  ASSERT_EQ(SQL_INVALID_HANDLE, SQLFreeHandle(SQL_HANDLE_DBC, conn));

  // Attempt to free null environment handle
  ASSERT_EQ(SQL_INVALID_HANDLE, SQLFreeHandle(SQL_HANDLE_ENV, env));
}

TEST(SQLGetEnvAttr, TestSQLGetEnvAttrODBCVersion) {
  SQLHENV env;
  SQLINTEGER version;

  // Allocate an environment handle
  ASSERT_EQ(SQL_SUCCESS, SQLAllocEnv(&env));

  ASSERT_EQ(SQL_SUCCESS, SQLGetEnvAttr(env, SQL_ATTR_ODBC_VERSION, &version, 0, nullptr));

  ASSERT_EQ(SQL_OV_ODBC2, version);

  // Free environment handle
  ASSERT_EQ(SQL_SUCCESS, SQLFreeEnv(env));
}

TEST(SQLSetEnvAttr, TestSQLSetEnvAttrODBCVersionValid) {
  // Allocate an environment handle
  SQLHENV env;
  ASSERT_EQ(SQL_SUCCESS, SQLAllocEnv(&env));

  // Attempt to set to unsupported version
  ASSERT_EQ(SQL_SUCCESS, SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION,
                                       reinterpret_cast<void*>(SQL_OV_ODBC2), 0));

  // Free environment handle
  ASSERT_EQ(SQL_SUCCESS, SQLFreeEnv(env));
}

TEST(SQLSetEnvAttr, TestSQLSetEnvAttrODBCVersionInvalid) {
  // Allocate an environment handle
  SQLHENV env;
  ASSERT_EQ(SQL_SUCCESS, SQLAllocEnv(&env));

  // Attempt to set to unsupported version
  ASSERT_EQ(SQL_ERROR,
            SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, reinterpret_cast<void*>(1), 0));

  // Free environment handle
  ASSERT_EQ(SQL_SUCCESS, SQLFreeEnv(env));
}

TYPED_TEST(ConnectionTest, TestSQLGetEnvAttrOutputNTS) {
  SQLINTEGER output_nts;

  ASSERT_EQ(SQL_SUCCESS,
            SQLGetEnvAttr(this->env, SQL_ATTR_OUTPUT_NTS, &output_nts, 0, nullptr));

  ASSERT_EQ(SQL_TRUE, output_nts);
}

TYPED_TEST(ConnectionTest, DISABLED_TestSQLGetEnvAttrGetLength) {
  // Test is disabled because call to SQLGetEnvAttr is handled by the driver manager on
  // Windows. Windows driver manager ignores the length pointer.
  // This test case can be potentially used on macOS/Linux
  SQLINTEGER length;
  ASSERT_EQ(SQL_SUCCESS,
            SQLGetEnvAttr(this->env, SQL_ATTR_ODBC_VERSION, nullptr, 0, &length));

  EXPECT_EQ(sizeof(SQLINTEGER), length);
}

TYPED_TEST(ConnectionTest, DISABLED_TestSQLGetEnvAttrNullValuePointer) {
  // Test is disabled because call to SQLGetEnvAttr is handled by the driver manager on
  // Windows. The Windows driver manager doesn't error out when null pointer is passed.
  // This test case can be potentially used on macOS/Linux
  ASSERT_EQ(SQL_ERROR,
            SQLGetEnvAttr(this->env, SQL_ATTR_ODBC_VERSION, nullptr, 0, nullptr));
}

TEST(SQLSetEnvAttr, TestSQLSetEnvAttrOutputNTSValid) {
  // Allocate an environment handle
  SQLHENV env;
  ASSERT_EQ(SQL_SUCCESS, SQLAllocEnv(&env));

  // Attempt to set to output nts to supported version
  ASSERT_EQ(SQL_SUCCESS, SQLSetEnvAttr(env, SQL_ATTR_OUTPUT_NTS,
                                       reinterpret_cast<void*>(SQL_TRUE), 0));

  // Free environment handle
  ASSERT_EQ(SQL_SUCCESS, SQLFreeEnv(env));
}

TEST(SQLSetEnvAttr, TestSQLSetEnvAttrOutputNTSInvalid) {
  // Allocate an environment handle
  SQLHENV env;
  ASSERT_EQ(SQL_SUCCESS, SQLAllocEnv(&env));

  // Attempt to set to output nts to unsupported false
  ASSERT_EQ(SQL_ERROR, SQLSetEnvAttr(env, SQL_ATTR_OUTPUT_NTS,
                                     reinterpret_cast<void*>(SQL_FALSE), 0));

  // Free environment handle
  ASSERT_EQ(SQL_SUCCESS, SQLFreeEnv(env));
}

TEST(SQLSetEnvAttr, TestSQLSetEnvAttrNullValuePointer) {
  // Allocate an environment handle
  SQLHENV env;
  ASSERT_EQ(SQL_SUCCESS, SQLAllocEnv(&env));

  // Attempt to set using bad data pointer
  ASSERT_EQ(SQL_ERROR, SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, nullptr, 0));

  // Free environment handle
  ASSERT_EQ(SQL_SUCCESS, SQLFreeEnv(env));
}

TYPED_TEST(ConnectionHandleTest, TestSQLDriverConnect) {
  // Connect string
  std::string connect_str = this->GetConnectionString();
  ASSERT_OK_AND_ASSIGN(std::wstring wconnect_str,
                       arrow::util::UTF8ToWideString(connect_str));
  std::vector<SQLWCHAR> connect_str0(wconnect_str.begin(), wconnect_str.end());

  SQLWCHAR out_str[kOdbcBufferSize] = {};
  SQLSMALLINT out_str_len;

  // Connecting to ODBC server.
  ASSERT_EQ(SQL_SUCCESS,
            SQLDriverConnect(this->conn, NULL, &connect_str0[0],
                             static_cast<SQLSMALLINT>(connect_str0.size()), out_str,
                             kOdbcBufferSize, &out_str_len, SQL_DRIVER_NOPROMPT))
      << GetOdbcErrorMessage(SQL_HANDLE_DBC, this->conn);

  // Check that out_str has same content as connect_str
  std::string out_connection_string = ODBC::SqlWcharToString(out_str, out_str_len);
  Connection::ConnPropertyMap out_properties;
  Connection::ConnPropertyMap in_properties;
  ODBC::ODBCConnection::GetPropertiesFromConnString(out_connection_string,
                                                    out_properties);
  ODBC::ODBCConnection::GetPropertiesFromConnString(connect_str, in_properties);
  ASSERT_TRUE(CompareConnPropertyMap(out_properties, in_properties));

  // Disconnect from ODBC
  ASSERT_EQ(SQL_SUCCESS, SQLDisconnect(this->conn))
      << GetOdbcErrorMessage(SQL_HANDLE_DBC, this->conn);
}

TYPED_TEST(ConnectionHandleTest, TestSQLDriverConnectDsn) {
  // Connect string
  std::string connect_str = this->GetConnectionString();

  // Write connection string content into a DSN,
  // must succeed before continuing
  ASSERT_TRUE(WriteDSN(connect_str));

  std::string dsn(kTestDsn);
  ASSERT_OK_AND_ASSIGN(std::wstring wdsn, arrow::util::UTF8ToWideString(dsn));

  // Update connection string to use DSN to connect
  connect_str = std::string("DSN=") + std::string(kTestDsn) +
                std::string(";driver={Apache Arrow Flight SQL ODBC Driver};");
  ASSERT_OK_AND_ASSIGN(std::wstring wconnect_str,
                       arrow::util::UTF8ToWideString(connect_str));
  std::vector<SQLWCHAR> connect_str0(wconnect_str.begin(), wconnect_str.end());

  SQLWCHAR out_str[kOdbcBufferSize] = {};
  SQLSMALLINT out_str_len;

  // Connecting to ODBC server.
  ASSERT_EQ(SQL_SUCCESS,
            SQLDriverConnect(this->conn, NULL, &connect_str0[0],
                             static_cast<SQLSMALLINT>(connect_str0.size()), out_str,
                             kOdbcBufferSize, &out_str_len, SQL_DRIVER_NOPROMPT))
      << GetOdbcErrorMessage(SQL_HANDLE_DBC, this->conn);

  // Remove DSN
  ASSERT_TRUE(UnregisterDsn(wdsn));

  // Disconnect from ODBC
  ASSERT_EQ(SQL_SUCCESS, SQLDisconnect(this->conn))
      << GetOdbcErrorMessage(SQL_HANDLE_DBC, this->conn);
}

// StringColumnLength in the connection string must override the length reported for
// string columns.
TYPED_TEST(ConnectionHandleTest, TestSQLDriverConnectStringColumnLength) {
  constexpr SQLULEN kStringColumnLength = 100;
  std::string connect_str =
      this->GetConnectionString() + ";StringColumnLength=" +
      std::to_string(kStringColumnLength);
  ASSERT_OK_AND_ASSIGN(std::wstring wconnect_str,
                       arrow::util::UTF8ToWideString(connect_str));
  std::vector<SQLWCHAR> connect_str0(wconnect_str.begin(), wconnect_str.end());

  SQLWCHAR out_str[kOdbcBufferSize] = {};
  SQLSMALLINT out_str_len;
  ASSERT_EQ(SQL_SUCCESS,
            SQLDriverConnect(this->conn, NULL, &connect_str0[0],
                             static_cast<SQLSMALLINT>(connect_str0.size()), out_str,
                             kOdbcBufferSize, &out_str_len, SQL_DRIVER_NOPROMPT))
      << GetOdbcErrorMessage(SQL_HANDLE_DBC, this->conn);

  SQLHSTMT statement;
  ASSERT_EQ(SQL_SUCCESS, SQLAllocHandle(SQL_HANDLE_STMT, this->conn, &statement));

  ASSIGN_SQLWCHAR_ARR_AND_LEN(
      sql_query, L"SELECT 'abc' AS short_str, CAST('abc' AS VARCHAR(50)) AS varchar_str");
  ASSERT_EQ(SQL_SUCCESS, SQLExecDirect(statement, sql_query, sql_query_len))
      << GetOdbcErrorMessage(SQL_HANDLE_STMT, statement);

  for (SQLUSMALLINT column = 1; column <= 2; ++column) {
    SQLWCHAR column_name[kOdbcBufferSize];
    SQLSMALLINT name_length = 0;
    SQLSMALLINT data_type = 0;
    SQLULEN column_size = 0;
    SQLSMALLINT decimal_digits = 0;
    SQLSMALLINT nullable = 0;
    ASSERT_EQ(SQL_SUCCESS,
              SQLDescribeCol(statement, column, column_name,
                             kOdbcBufferSize, &name_length, &data_type, &column_size,
                             &decimal_digits, &nullable))
        << GetOdbcErrorMessage(SQL_HANDLE_STMT, statement);
    EXPECT_EQ(kStringColumnLength, column_size) << "column " << column;

    SQLLEN length = 0;
    ASSERT_EQ(SQL_SUCCESS, SQLColAttribute(statement, column, SQL_DESC_LENGTH, 0, 0,
                                           nullptr, &length));
    EXPECT_EQ(static_cast<SQLLEN>(kStringColumnLength), length) << "column " << column;
  }

  ASSERT_EQ(SQL_SUCCESS, SQLFreeHandle(SQL_HANDLE_STMT, statement));
  ASSERT_EQ(SQL_SUCCESS, SQLDisconnect(this->conn))
      << GetOdbcErrorMessage(SQL_HANDLE_DBC, this->conn);
}

TYPED_TEST(ConnectionHandleTest, TestSQLConnect) {
  // Connect string
  std::string connect_str = this->GetConnectionString();

  // Write connection string content into a DSN,
  // must succeed before continuing
  std::string uid(""), pwd("");
  ASSERT_TRUE(WriteDSN(connect_str));

  std::string dsn(kTestDsn);
  ASSERT_OK_AND_ASSIGN(std::wstring wdsn, arrow::util::UTF8ToWideString(dsn));
  ASSERT_OK_AND_ASSIGN(std::wstring wuid, arrow::util::UTF8ToWideString(uid));
  ASSERT_OK_AND_ASSIGN(std::wstring wpwd, arrow::util::UTF8ToWideString(pwd));
  std::vector<SQLWCHAR> dsn0(wdsn.begin(), wdsn.end());
  std::vector<SQLWCHAR> uid0(wuid.begin(), wuid.end());
  std::vector<SQLWCHAR> pwd0(wpwd.begin(), wpwd.end());

  // Connecting to ODBC server. Empty uid and pwd should be ignored.
  ASSERT_EQ(SQL_SUCCESS,
            SQLConnect(this->conn, dsn0.data(), static_cast<SQLSMALLINT>(dsn0.size()),
                       uid0.data(), static_cast<SQLSMALLINT>(uid0.size()), pwd0.data(),
                       static_cast<SQLSMALLINT>(pwd0.size())))
      << GetOdbcErrorMessage(SQL_HANDLE_DBC, this->conn);

  // Remove DSN
  ASSERT_TRUE(UnregisterDsn(wdsn));

  // Disconnect from ODBC
  ASSERT_EQ(SQL_SUCCESS, SQLDisconnect(this->conn))
      << GetOdbcErrorMessage(SQL_HANDLE_DBC, this->conn);
}

TEST_F(ConnectionRemoteTest, TestSQLConnectInputUidPwd) {
  // Connect string
  std::string connect_str = GetConnectionString();

  // Retrieve valid uid and pwd, assumes TEST_CONNECT_STR contains uid and pwd
  Connection::ConnPropertyMap properties;
  ODBC::ODBCConnection::GetPropertiesFromConnString(connect_str, properties);
  std::string uid_key("uid");
  std::string pwd_key("pwd");
  std::string uid = properties[uid_key];
  std::string pwd = properties[pwd_key];

  // Write connection string content without uid and pwd into a DSN,
  // must succeed before continuing
  properties.erase(uid_key);
  properties.erase(pwd_key);
  ASSERT_TRUE(WriteDSN(properties));

  std::string dsn(kTestDsn);
  ASSERT_OK_AND_ASSIGN(std::wstring wdsn, arrow::util::UTF8ToWideString(dsn));
  ASSERT_OK_AND_ASSIGN(std::wstring wuid, arrow::util::UTF8ToWideString(uid));
  ASSERT_OK_AND_ASSIGN(std::wstring wpwd, arrow::util::UTF8ToWideString(pwd));
  std::vector<SQLWCHAR> dsn0(wdsn.begin(), wdsn.end());
  std::vector<SQLWCHAR> uid0(wuid.begin(), wuid.end());
  std::vector<SQLWCHAR> pwd0(wpwd.begin(), wpwd.end());

  // Connecting to ODBC server.
  ASSERT_EQ(SQL_SUCCESS,
            SQLConnect(this->conn, dsn0.data(), static_cast<SQLSMALLINT>(dsn0.size()),
                       uid0.data(), static_cast<SQLSMALLINT>(uid0.size()), pwd0.data(),
                       static_cast<SQLSMALLINT>(pwd0.size())))
      << GetOdbcErrorMessage(SQL_HANDLE_DBC, this->conn);

  // Remove DSN
  ASSERT_TRUE(UnregisterDsn(wdsn));

  // Disconnect from ODBC
  ASSERT_EQ(SQL_SUCCESS, SQLDisconnect(this->conn))
      << GetOdbcErrorMessage(SQL_HANDLE_DBC, this->conn);
}

TEST_F(ConnectionRemoteTest, TestSQLConnectInvalidUid) {
  // Connect string
  std::string connect_str = GetConnectionString();

  // Retrieve valid uid and pwd, assumes TEST_CONNECT_STR contains uid and pwd
  Connection::ConnPropertyMap properties;
  ODBC::ODBCConnection::GetPropertiesFromConnString(connect_str, properties);
  std::string uid = properties["uid"];
  std::string pwd = properties["pwd"];

  // Append invalid uid to connection string
  connect_str += "uid=non_existent_id;";

  // Write connection string content into a DSN,
  // must succeed before continuing
  ASSERT_TRUE(WriteDSN(connect_str));

  std::string dsn(kTestDsn);
  ASSERT_OK_AND_ASSIGN(std::wstring wdsn, arrow::util::UTF8ToWideString(dsn));
  ASSERT_OK_AND_ASSIGN(std::wstring wuid, arrow::util::UTF8ToWideString(uid));
  ASSERT_OK_AND_ASSIGN(std::wstring wpwd, arrow::util::UTF8ToWideString(pwd));
  std::vector<SQLWCHAR> dsn0(wdsn.begin(), wdsn.end());
  std::vector<SQLWCHAR> uid0(wuid.begin(), wuid.end());
  std::vector<SQLWCHAR> pwd0(wpwd.begin(), wpwd.end());

  // Connecting to ODBC server.
  // UID specified in DSN will take precedence,
  // so connection still fails despite passing valid uid in SQLConnect call
  ASSERT_EQ(SQL_ERROR,
            SQLConnect(this->conn, dsn0.data(), static_cast<SQLSMALLINT>(dsn0.size()),
                       uid0.data(), static_cast<SQLSMALLINT>(uid0.size()), pwd0.data(),
                       static_cast<SQLSMALLINT>(pwd0.size())));

  VerifyOdbcErrorState(SQL_HANDLE_DBC, this->conn, kErrorState28000);

  // Remove DSN
  ASSERT_TRUE(UnregisterDsn(wdsn));
}

TEST_F(ConnectionRemoteTest, TestSQLConnectDSNPrecedence) {
  // Connect string
  std::string connect_str = GetConnectionString();

  // Write connection string content into a DSN,
  // must succeed before continuing

  // Pass incorrect uid and password to SQLConnect, they will be ignored.
  // Assumes TEST_CONNECT_STR contains uid and pwd
  std::string uid("non_existent_id"), pwd("non_existent_password");
  ASSERT_TRUE(WriteDSN(connect_str));

  std::string dsn(kTestDsn);
  ASSERT_OK_AND_ASSIGN(std::wstring wdsn, arrow::util::UTF8ToWideString(dsn));
  ASSERT_OK_AND_ASSIGN(std::wstring wuid, arrow::util::UTF8ToWideString(uid));
  ASSERT_OK_AND_ASSIGN(std::wstring wpwd, arrow::util::UTF8ToWideString(pwd));
  std::vector<SQLWCHAR> dsn0(wdsn.begin(), wdsn.end());
  std::vector<SQLWCHAR> uid0(wuid.begin(), wuid.end());
  std::vector<SQLWCHAR> pwd0(wpwd.begin(), wpwd.end());

  // Connecting to ODBC server.
  ASSERT_EQ(SQL_SUCCESS,
            SQLConnect(this->conn, dsn0.data(), static_cast<SQLSMALLINT>(dsn0.size()),
                       uid0.data(), static_cast<SQLSMALLINT>(uid0.size()), pwd0.data(),
                       static_cast<SQLSMALLINT>(pwd0.size())))
      << GetOdbcErrorMessage(SQL_HANDLE_DBC, this->conn);

  // Remove DSN
  ASSERT_TRUE(UnregisterDsn(wdsn));

  // Disconnect from ODBC
  ASSERT_EQ(SQL_SUCCESS, SQLDisconnect(this->conn))
      << GetOdbcErrorMessage(SQL_HANDLE_DBC, this->conn);
}

TEST_F(ConnectionRemoteTest, TestSQLDriverConnectInvalidUid) {
  // Invalid connect string
  std::string connect_str = GetInvalidConnectionString();

  ASSERT_OK_AND_ASSIGN(std::wstring wconnect_str,
                       arrow::util::UTF8ToWideString(connect_str));
  std::vector<SQLWCHAR> connect_str0(wconnect_str.begin(), wconnect_str.end());

  SQLWCHAR out_str[kOdbcBufferSize] = {0};
  SQLSMALLINT out_str_len;

  // Connecting to ODBC server.
  ASSERT_EQ(SQL_ERROR,
            SQLDriverConnect(this->conn, NULL, &connect_str0[0],
                             static_cast<SQLSMALLINT>(connect_str0.size()), out_str,
                             kOdbcBufferSize, &out_str_len, SQL_DRIVER_NOPROMPT));

  VerifyOdbcErrorState(SQL_HANDLE_DBC, this->conn, kErrorState28000);

  std::string out_connection_string = ODBC::SqlWcharToString(out_str, out_str_len);
  ASSERT_TRUE(out_connection_string.empty());
}

TYPED_TEST(ConnectionHandleTest, TestSQLDisconnectWithoutConnection) {
  // Attempt to disconnect without a connection, expect to fail
  ASSERT_EQ(SQL_ERROR, SQLDisconnect(this->conn));

  // Expect ODBC driver manager to return error state
  VerifyOdbcErrorState(SQL_HANDLE_DBC, this->conn, kErrorState08003);
}

TYPED_TEST(ConnectionTest, TestSQLAllocFreeStmt) {
  SQLHSTMT statement;

  // Allocate a statement using alloc statement
  ASSERT_EQ(SQL_SUCCESS, SQLAllocStmt(this->conn, &statement));

  // Close statement handle
  ASSERT_EQ(SQL_SUCCESS, SQLFreeStmt(statement, SQL_CLOSE));

  // Free statement handle
  ASSERT_EQ(SQL_SUCCESS, SQLFreeStmt(statement, SQL_DROP));
}

TYPED_TEST(ConnectionHandleTest, TestCloseConnectionWithOpenStatement) {
  SQLHSTMT statement;

  // Connect string
  std::string connect_str = this->GetConnectionString();
  ASSERT_OK_AND_ASSIGN(std::wstring wconnect_str,
                       arrow::util::UTF8ToWideString(connect_str));
  std::vector<SQLWCHAR> connect_str0(wconnect_str.begin(), wconnect_str.end());

  SQLWCHAR out_str[kOdbcBufferSize] = {};
  SQLSMALLINT out_str_len;

  // Connecting to ODBC server.
  ASSERT_EQ(SQL_SUCCESS,
            SQLDriverConnect(this->conn, NULL, &connect_str0[0],
                             static_cast<SQLSMALLINT>(connect_str0.size()), out_str,
                             kOdbcBufferSize, &out_str_len, SQL_DRIVER_NOPROMPT))
      << GetOdbcErrorMessage(SQL_HANDLE_DBC, this->conn);

  // Allocate a statement using alloc statement
  ASSERT_EQ(SQL_SUCCESS, SQLAllocStmt(this->conn, &statement));

  // Disconnect from ODBC without closing the statement first
  ASSERT_EQ(SQL_SUCCESS, SQLDisconnect(this->conn));
}

TYPED_TEST(ConnectionTest, TestSQLAllocFreeDesc) {
  SQLHDESC descriptor;

  // Allocate a descriptor using alloc handle
  ASSERT_EQ(SQL_SUCCESS, SQLAllocHandle(SQL_HANDLE_DESC, this->conn, &descriptor));

  // Free descriptor handle
  ASSERT_EQ(SQL_SUCCESS, SQLFreeHandle(SQL_HANDLE_DESC, descriptor));
}

TYPED_TEST(ConnectionTest, TestSQLSetStmtAttrDescriptor) {
  SQLHDESC apd_descriptor, ard_descriptor;

  // Allocate an APD descriptor using alloc handle
  ASSERT_EQ(SQL_SUCCESS, SQLAllocHandle(SQL_HANDLE_DESC, this->conn, &apd_descriptor));

  // Allocate an ARD descriptor using alloc handle
  ASSERT_EQ(SQL_SUCCESS, SQLAllocHandle(SQL_HANDLE_DESC, this->conn, &ard_descriptor));

  // Save implicitly allocated internal APD and ARD descriptor pointers
  SQLPOINTER internal_apd, internal_ard = nullptr;

  EXPECT_EQ(SQL_SUCCESS, SQLGetStmtAttr(this->stmt, SQL_ATTR_APP_PARAM_DESC,
                                        &internal_apd, sizeof(internal_apd), 0));

  EXPECT_EQ(SQL_SUCCESS, SQLGetStmtAttr(this->stmt, SQL_ATTR_APP_ROW_DESC, &internal_ard,
                                        sizeof(internal_ard), 0));
  EXPECT_NE(internal_apd, internal_ard);

  // Set APD descriptor to explicitly allocated handle
  EXPECT_EQ(SQL_SUCCESS, SQLSetStmtAttr(this->stmt, SQL_ATTR_APP_PARAM_DESC,
                                        reinterpret_cast<SQLPOINTER>(apd_descriptor), 0));

  // Set ARD descriptor to explicitly allocated handle
  EXPECT_EQ(SQL_SUCCESS, SQLSetStmtAttr(this->stmt, SQL_ATTR_APP_ROW_DESC,
                                        reinterpret_cast<SQLPOINTER>(ard_descriptor), 0));

  // Verify APD and ARD descriptors are set to explicitly allocated pointers
  SQLPOINTER value = nullptr;
  EXPECT_EQ(SQL_SUCCESS, SQLGetStmtAttr(this->stmt, SQL_ATTR_APP_PARAM_DESC, &value,
                                        sizeof(value), 0));

  EXPECT_EQ(apd_descriptor, value);

  EXPECT_EQ(SQL_SUCCESS,
            SQLGetStmtAttr(this->stmt, SQL_ATTR_APP_ROW_DESC, &value, sizeof(value), 0));

  EXPECT_EQ(ard_descriptor, value);

  // Assign replacement descriptors. Freeing the old descriptors must not revert these
  // replacements, which proves switching descriptors detached the old handles. Null
  // descriptor restoration is covered through the direct driver API because some driver
  // managers intercept it.
  SQLHDESC replacement_apd, replacement_ard;
  ASSERT_EQ(SQL_SUCCESS, SQLAllocHandle(SQL_HANDLE_DESC, this->conn, &replacement_apd));
  ASSERT_EQ(SQL_SUCCESS, SQLAllocHandle(SQL_HANDLE_DESC, this->conn, &replacement_ard));
  ASSERT_EQ(SQL_SUCCESS,
            SQLSetStmtAttr(this->stmt, SQL_ATTR_APP_PARAM_DESC, replacement_apd, 0));
  ASSERT_EQ(SQL_SUCCESS,
            SQLSetStmtAttr(this->stmt, SQL_ATTR_APP_ROW_DESC, replacement_ard, 0));

  ASSERT_EQ(SQL_SUCCESS, SQLFreeHandle(SQL_HANDLE_DESC, apd_descriptor));
  ASSERT_EQ(SQL_SUCCESS, SQLFreeHandle(SQL_HANDLE_DESC, ard_descriptor));
  EXPECT_EQ(SQL_SUCCESS, SQLGetStmtAttr(this->stmt, SQL_ATTR_APP_PARAM_DESC, &value,
                                        sizeof(value), 0));
  EXPECT_EQ(replacement_apd, value);
  EXPECT_EQ(SQL_SUCCESS,
            SQLGetStmtAttr(this->stmt, SQL_ATTR_APP_ROW_DESC, &value, sizeof(value), 0));
  EXPECT_EQ(replacement_ard, value);

  ASSERT_EQ(SQL_SUCCESS, SQLFreeHandle(SQL_HANDLE_DESC, replacement_apd));
  ASSERT_EQ(SQL_SUCCESS, SQLFreeHandle(SQL_HANDLE_DESC, replacement_ard));

  // Freeing the active explicit descriptors restores the implicit descriptors.
  value = nullptr;

  EXPECT_EQ(SQL_SUCCESS, SQLGetStmtAttr(this->stmt, SQL_ATTR_APP_PARAM_DESC, &value,
                                        sizeof(value), 0));

  EXPECT_EQ(internal_apd, value);

  EXPECT_EQ(SQL_SUCCESS,
            SQLGetStmtAttr(this->stmt, SQL_ATTR_APP_ROW_DESC, &value, sizeof(value), 0));

  EXPECT_EQ(internal_ard, value);
}

TYPED_TEST(ConnectionTest, TestExplicitDescriptorsOutliveStatement) {
  SQLHSTMT statement;
  SQLHDESC apd_descriptor, ard_descriptor;
  ASSERT_EQ(SQL_SUCCESS, SQLAllocHandle(SQL_HANDLE_STMT, this->conn, &statement));
  ASSERT_EQ(SQL_SUCCESS, SQLAllocHandle(SQL_HANDLE_DESC, this->conn, &apd_descriptor));
  ASSERT_EQ(SQL_SUCCESS, SQLAllocHandle(SQL_HANDLE_DESC, this->conn, &ard_descriptor));

  ASSERT_EQ(SQL_SUCCESS,
            SQLSetStmtAttr(statement, SQL_ATTR_APP_PARAM_DESC, apd_descriptor, 0));
  ASSERT_EQ(SQL_SUCCESS,
            SQLSetStmtAttr(statement, SQL_ATTR_APP_ROW_DESC, ard_descriptor, 0));

  ASSERT_EQ(SQL_SUCCESS, SQLFreeHandle(SQL_HANDLE_STMT, statement));
  EXPECT_EQ(SQL_SUCCESS, SQLFreeHandle(SQL_HANDLE_DESC, apd_descriptor));
  EXPECT_EQ(SQL_SUCCESS, SQLFreeHandle(SQL_HANDLE_DESC, ard_descriptor));
}

// StringColumnLength, UseWideChar and ChunkBufferCapacity are known keys without a field
// in the Windows DSN dialog. Configuration::LoadDsn used to drop them, so they were lost
// whenever a DSN was loaded and then saved again.
TEST(DsnConfiguration, LoadDsnReadsKeysWithoutDialogField) {
  using config::Configuration;

  Connection::ConnPropertyMap properties = {
      {std::string(FlightSqlConnection::DRIVER), "Apache Arrow Flight SQL ODBC Driver"},
      {std::string(FlightSqlConnection::HOST), "localhost"},
      {std::string(FlightSqlConnection::PORT), "32010"},
      {std::string(FlightSqlConnection::STRING_COLUMN_LENGTH), "2048"},
      {std::string(FlightSqlConnection::USE_WIDE_CHAR), "true"},
      {std::string(FlightSqlConnection::CHUNK_BUFFER_CAPACITY), "7"},
      {"MyCustomKey", "custom value"},
  };
  ASSERT_TRUE(WriteDSN(properties));

  const std::string dsn(kTestDsn);
  ASSERT_OK_AND_ASSIGN(std::wstring wdsn, arrow::util::UTF8ToWideString(dsn));

  Configuration loaded;
  loaded.LoadDsn(dsn);

  // Check before unregistering the DSN so a failed assertion does not leak it.
  EXPECT_EQ("localhost", loaded.Get(FlightSqlConnection::HOST));
  EXPECT_EQ("32010", loaded.Get(FlightSqlConnection::PORT));
  EXPECT_EQ("2048", loaded.Get(FlightSqlConnection::STRING_COLUMN_LENGTH));
  EXPECT_EQ("true", loaded.Get(FlightSqlConnection::USE_WIDE_CHAR));
  EXPECT_EQ("7", loaded.Get(FlightSqlConnection::CHUNK_BUFFER_CAPACITY));
  EXPECT_EQ("custom value", loaded.Get("MyCustomKey"));

  // The Windows DSN dialog lists exactly these keys in its "Advanced properties" table.
  // GetCustomKeys() returns them in the property map's case-insensitive alphabetical
  // order.
  const std::vector<std::string> expected_custom_keys = {
      std::string(FlightSqlConnection::CHUNK_BUFFER_CAPACITY), "MyCustomKey",
      std::string(FlightSqlConnection::STRING_COLUMN_LENGTH),
      std::string(FlightSqlConnection::USE_WIDE_CHAR)};
  EXPECT_EQ(expected_custom_keys, loaded.GetCustomKeys());

  ASSERT_TRUE(UnregisterDsn(wdsn));
}

// Every key in ALL_KEYS must survive LoadDsn, so a key added to ALL_KEYS without also
// being read by LoadDsn fails here. DSN is set from the argument and DRIVER only names
// the driver the DSN is registered with, so neither is loaded from the DSN's values.
TEST(DsnConfiguration, LoadDsnReadsAllKnownKeys) {
  using config::Configuration;

  // A realistic value for every known key. The encryption values differ from the defaults
  // LoadDsn falls back to, and UID and "User ID" differ from each other, so a key that is
  // not read from the DSN fails below.
  Connection::ConnPropertyMap properties = {
      {std::string(FlightSqlConnection::DRIVER), "Apache Arrow Flight SQL ODBC Driver"},
      {std::string(FlightSqlConnection::HOST), "localhost"},
      {std::string(FlightSqlConnection::PORT), "32010"},
      {std::string(FlightSqlConnection::TOKEN), "token"},
      {std::string(FlightSqlConnection::UID), "user"},
      {std::string(FlightSqlConnection::USER_ID), "ole db user"},
      {std::string(FlightSqlConnection::PWD), "password"},
      {std::string(FlightSqlConnection::USE_ENCRYPTION), "false"},
      {std::string(FlightSqlConnection::TRUSTED_CERTS), "/path/to/cert"},
      {std::string(FlightSqlConnection::USE_SYSTEM_TRUST_STORE), "false"},
      {std::string(FlightSqlConnection::DISABLE_CERTIFICATE_VERIFICATION), "true"},
      {std::string(FlightSqlConnection::STRING_COLUMN_LENGTH), "2048"},
      {std::string(FlightSqlConnection::USE_WIDE_CHAR), "true"},
      {std::string(FlightSqlConnection::CHUNK_BUFFER_CAPACITY), "7"},
  };
  // Fails when a key is added to ALL_KEYS without a value above.
  for (const auto& key : FlightSqlConnection::ALL_KEYS) {
    if (key == FlightSqlConnection::DSN) {
      continue;
    }
    EXPECT_EQ(1U, properties.count(key)) << "Add a value for " << key << " to this test";
  }
  ASSERT_TRUE(WriteDSN(properties));

  const std::string dsn(kTestDsn);
  ASSERT_OK_AND_ASSIGN(std::wstring wdsn, arrow::util::UTF8ToWideString(dsn));

  Configuration loaded;
  loaded.LoadDsn(dsn);

  // Check before unregistering the DSN so a failed assertion does not leak it.
  EXPECT_EQ(dsn, loaded.Get(FlightSqlConnection::DSN));
  for (const auto& [key, value] : properties) {
    if (key == FlightSqlConnection::DRIVER) {
      continue;
    }
    EXPECT_EQ(value, loaded.Get(key)) << key << " was not loaded from the DSN";
  }

  // The Windows DSN dialog lists exactly the known keys without a field of their own.
  std::vector<std::string> expected_custom_keys;
  for (const auto& key : FlightSqlConnection::ALL_KEYS) {
    const auto& dialog_keys = FlightSqlConnection::DSN_DIALOG_KEYS;
    if (std::find(dialog_keys.begin(), dialog_keys.end(), key) == dialog_keys.end()) {
      expected_custom_keys.emplace_back(key);
    }
  }
  auto custom_keys = loaded.GetCustomKeys();
  std::sort(expected_custom_keys.begin(), expected_custom_keys.end());
  std::sort(custom_keys.begin(), custom_keys.end());
  EXPECT_EQ(expected_custom_keys, custom_keys);

  ASSERT_TRUE(UnregisterDsn(wdsn));
}

// -AL- TODO: review the test
// -AL- see if #include <algorithm> can be removed
// Reproduces what the Windows DSN dialog does when a user opens an existing DSN and clicks
// OK (ConfigDSNW with ODBC_CONFIG_DSN): LoadDsn, then SaveParameters, which clears the
// configuration and rebuilds it from the dialog fields plus the "Advanced properties"
// rows returned by GetCustomKeys, then UnregisterDsn and RegisterDsn. Keys without a
// dialog field, such as StringColumnLength, used to be lost at this point.
TEST(DsnConfiguration, DsnSurvivesDialogRoundTrip) {
  using config::Configuration;

  const std::string driver = "Apache Arrow Flight SQL ODBC Driver";
  Connection::ConnPropertyMap properties = {
      {std::string(FlightSqlConnection::DRIVER), driver},
      {std::string(FlightSqlConnection::HOST), "localhost"},
      {std::string(FlightSqlConnection::PORT), "32010"},
      {std::string(FlightSqlConnection::UID), "user"},
      {std::string(FlightSqlConnection::PWD), "password"},
      {std::string(FlightSqlConnection::USE_ENCRYPTION), "false"},
      {std::string(FlightSqlConnection::TRUSTED_CERTS), "/path/to/cert"},
      {std::string(FlightSqlConnection::USE_SYSTEM_TRUST_STORE), "false"},
      {std::string(FlightSqlConnection::DISABLE_CERTIFICATE_VERIFICATION), "true"},
      {std::string(FlightSqlConnection::STRING_COLUMN_LENGTH), "2048"},
      {std::string(FlightSqlConnection::USE_WIDE_CHAR), "true"},
      {std::string(FlightSqlConnection::CHUNK_BUFFER_CAPACITY), "7"},
      {"MyCustomKey", "custom value"},
  };
  ASSERT_TRUE(WriteDSN(properties));

  const std::string dsn(kTestDsn);
  ASSERT_OK_AND_ASSIGN(std::wstring wdsn, arrow::util::UTF8ToWideString(dsn));
  CONVERT_SQLWCHAR_STR(wdriver, driver);

  // The dialog opens with the DSN loaded.
  Configuration loaded;
  loaded.LoadDsn(dsn);

  // Clicking OK: SaveParameters writes the dialog's own fields (basic authentication, so
  // no token) and then every row of the "Advanced properties" table.
  Configuration saved;
  for (const auto& key :
       {FlightSqlConnection::DSN, FlightSqlConnection::HOST, FlightSqlConnection::PORT,
        FlightSqlConnection::UID, FlightSqlConnection::PWD,
        FlightSqlConnection::USE_ENCRYPTION, FlightSqlConnection::TRUSTED_CERTS,
        FlightSqlConnection::USE_SYSTEM_TRUST_STORE,
        FlightSqlConnection::DISABLE_CERTIFICATE_VERIFICATION}) {
    saved.Set(key, loaded.Get(key));
  }
  for (const auto& key : loaded.GetCustomKeys()) {
    saved.Set(key, loaded.Get(key));
  }

  // ConfigDSNW then re-registers the DSN.
  ASSERT_TRUE(UnregisterDsn(wdsn));
  ASSERT_TRUE(RegisterDsn(saved, reinterpret_cast<LPCWSTR>(wdriver.c_str())));

  Configuration reloaded;
  reloaded.LoadDsn(dsn);

  // Check before unregistering the DSN so a failed assertion does not leak it.
  for (const auto& [key, value] : properties) {
    if (key == FlightSqlConnection::DRIVER) {
      continue;
    }
    EXPECT_EQ(value, reloaded.Get(key)) << key << " was lost by the dialog round trip";
  }

  ASSERT_TRUE(UnregisterDsn(wdsn));
}

}  // namespace arrow::flight::sql::odbc
