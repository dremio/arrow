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

#pragma once

#include <mutex>
#include <vector>

#include "arrow/flight/client.h"
#include "arrow/flight/sql/client.h"
#include "arrow/flight/sql/odbc/odbc_impl/blocking_queue.h"

namespace arrow::flight::sql::odbc {

class FlightStreamChunkBuffer {
  BlockingQueue<
      std::pair<arrow::Result<FlightStreamChunk>, std::shared_ptr<FlightSqlClient>>>
      queue_;

  // The readers backing each endpoint's DoGet stream. Retained (rather than kept
  // local to the constructor) so that Cancel() can interrupt a producer thread
  // that is currently blocked inside FlightStreamReader::Next() waiting on the
  // network, instead of only stopping the queue.
  std::mutex readers_mutex_;
  std::vector<std::shared_ptr<FlightStreamReader>> stream_readers_;

 public:
  FlightStreamChunkBuffer(FlightSqlClient& flight_sql_client,
                          const FlightClientOptions& client_options,
                          const FlightCallOptions& call_options,
                          const std::shared_ptr<FlightInfo>& flight_info,
                          size_t queue_capacity = 5);

  ~FlightStreamChunkBuffer();

  void Close();

  // Cancels the underlying DoGet stream(s) so any producer thread blocked on a
  // network read returns promptly, then closes the queue. Safe to call from a
  // different thread than the one fetching rows.
  void Cancel();

  bool GetNext(FlightStreamChunk* chunk);
};

}  // namespace arrow::flight::sql::odbc
