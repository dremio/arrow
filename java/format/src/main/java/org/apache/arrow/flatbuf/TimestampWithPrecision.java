/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.apache.arrow.flatbuf;

import com.google.flatbuffers.BaseVector;
import com.google.flatbuffers.Constants;
import com.google.flatbuffers.FlatBufferBuilder;
import com.google.flatbuffers.Table;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public final class TimestampWithPrecision extends Table {
  public static void ValidateVersion() { Constants.FLATBUFFERS_24_3_25(); }
  public static TimestampWithPrecision getRootAsTimestampWithPrecision(ByteBuffer _bb) {
    return getRootAsTimestampWithPrecision(_bb, new TimestampWithPrecision());
  }
  public static TimestampWithPrecision getRootAsTimestampWithPrecision(ByteBuffer _bb, TimestampWithPrecision obj) {
    _bb.order(ByteOrder.LITTLE_ENDIAN);
    return obj.__init(_bb.getInt(_bb.position()) + _bb.position(), _bb);
  }

  public TimestampWithPrecision __init(int _i, ByteBuffer _bb) {
    this.bb_pos = _i;
    this.bb = _bb;
    return this;
  }
  public TimestampWithPrecision __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }
  
  public int precision() {
    int o = this.__offset(4);
    return o != 0 ? this.bb.getInt(o + this.bb_pos) : 0;
  }
  public String timezone() {
    int o = this.__offset(6);
    return o != 0 ? this.__string(o + this.bb_pos) : null;
  }
  public ByteBuffer timezoneAsByteBuffer() {
    return this.__vector_as_bytebuffer(6, 1);
  }
  public ByteBuffer timezoneInByteBuffer(ByteBuffer _bb) { return __vector_in_bytebuffer(_bb, 6, 1); }

  public static int createTimestampWithPrecision(FlatBufferBuilder builder, int precision, int timezone) {
    builder.startTable(2);
    TimestampWithPrecision.addTimezone(builder, timezone);
    TimestampWithPrecision.addPrecision(builder, precision);
    return endTimestampWithPrecision(builder);
  }
  public static void startTimestampWithPrecision(FlatBufferBuilder builder) {
    builder.startTable(2);
  }
  public static void addPrecision(FlatBufferBuilder builder, int precision) {
    builder.addInt(0, precision, 0);
  }
  public static void addTimezone(FlatBufferBuilder builder, int timezoneOffset) {
    builder.addOffset(1, timezoneOffset, 0);
  }
  public static int endTimestampWithPrecision(FlatBufferBuilder builder) {
    int o = builder.endTable();
    return o;
  }

  public static final class Vector extends BaseVector {
    public Vector __assign(int _vector, int _element_size, ByteBuffer _bb) { __reset(_vector, _element_size, _bb); return this; }

    public TimestampWithPrecision get(int j) { return get( new TimestampWithPrecision(), j); }
    public TimestampWithPrecision get(TimestampWithPrecision obj, int j) {  return obj.__assign(__indirect(__element(j), bb), bb); }
  }
}
