

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
package org.apache.arrow.vector.complex.impl;


import static org.apache.arrow.util.Preconditions.checkArgument;
import static org.apache.arrow.util.Preconditions.checkState;

import com.google.flatbuffers.FlatBufferBuilder;

import org.apache.arrow.memory.*;
import org.apache.arrow.util.Preconditions;
import org.apache.arrow.vector.types.Types;
import org.apache.arrow.vector.types.Types.*;
import org.apache.arrow.vector.types.pojo.*;
import org.apache.arrow.vector.types.pojo.ArrowType.*;
import org.apache.arrow.vector.types.*;
import org.apache.arrow.vector.*;
import org.apache.arrow.vector.holders.*;
import org.apache.arrow.vector.util.*;
import org.apache.arrow.vector.complex.*;
import org.apache.arrow.vector.complex.reader.*;
import org.apache.arrow.vector.complex.impl.*;
import org.apache.arrow.vector.complex.writer.*;
import org.apache.arrow.vector.complex.writer.BaseWriter.StructWriter;
import org.apache.arrow.vector.complex.writer.BaseWriter.ListWriter;
import org.apache.arrow.vector.complex.writer.BaseWriter.MapWriter;
import org.apache.arrow.vector.util.JsonStringArrayList;

import java.util.Arrays;
import java.util.Random;
import java.util.List;

import java.io.Closeable;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.ByteBuffer;

import java.math.BigDecimal;
import java.math.BigInteger;
import java.time.Duration;
import java.time.LocalDateTime;
import java.time.Period;
import java.time.ZonedDateTime;



/*
 * This class is generated using freemarker and the UnionMapWriter.java template.
 */

/**
 * <p>Writer for MapVectors. This extends UnionListWriter to simplify writing map entries to a list
 * of struct elements, with "key" and "value" fields. The procedure for writing a map begin with
 * {@link #startMap()} followed by {@link #startEntry()}. An entry is written by using the
 * {@link #key()} writer to write the key, then the {@link #value()} writer to write a value. After
 * writing the value, call {@link #endEntry()} to complete the entry. Each map can have 1 or more
 * entries. When done writing entries, call {@link #endMap()} to complete the map.
 *
 * <p>NOTE: the MapVector can have NULL values by not writing to position. If a map is started with
 * {@link #startMap()}, then it must have a key written. The value of a map entry can be NULL by
 * not using the {@link #value()} writer.
 *
 * <p>Example to write the following map to position 5 of a vector
 * <pre>{@code
 *   // {
 *   //   1 -> 3,
 *   //   2 -> 4,
 *   //   3 -> NULL
 *   // }
 *
 *   UnionMapWriter writer = ...
 *
 *   writer.setPosition(5);
 *   writer.startMap();
 *   writer.startEntry();
 *   writer.key().integer().writeInt(1);
 *   writer.value().integer().writeInt(3);
 *   writer.endEntry();
 *   writer.startEntry();
 *   writer.key().integer().writeInt(2);
 *   writer.value().integer().writeInt(4);
 *   writer.endEntry();
 *   writer.startEntry();
 *   writer.key().integer().writeInt(3);
 *   writer.endEntry();
 *   writer.endMap();
 * </pre>
 * </p>
 */
@SuppressWarnings("unused")
public class UnionMapWriter extends UnionListWriter {

  /**
   * Current mode for writing map entries, set by calling {@link #key()} or {@link #value()}
   * and reset with a call to {@link #endEntry()}. With KEY mode, a struct writer with field
   * named "key" is returned. With VALUE mode, a struct writer with field named "value" is
   * returned. In OFF mode, the writer will behave like a standard UnionListWriter
   */
  private enum MapWriteMode {
    OFF,
    KEY,
    VALUE,
  }

  private MapWriteMode mode = MapWriteMode.OFF;
  private StructWriter entryWriter;

  public UnionMapWriter(MapVector vector) {
    super(vector);
    entryWriter = struct();
  }

  /** Start writing a map that consists of 1 or more entries. */
  public void startMap() {
    startList();
  }

  /** Complete the map. */
  public void endMap() {
    endList();
  }

  /**
   * Start a map entry that should be followed by calls to {@link #key()} and {@link #value()}
   * writers. Call {@link #endEntry()} to complete the entry.
   */
  public void startEntry() {
    writer.setAddVectorAsNullable(false);
    entryWriter.start();
  }

  /** Complete the map entry. */
  public void endEntry() {
    entryWriter.end();
    mode = MapWriteMode.OFF;
    writer.setAddVectorAsNullable(true);
  }

  /** Return the key writer that is used to write to the "key" field. */
  public UnionMapWriter key() {
    writer.setAddVectorAsNullable(false);
    mode = MapWriteMode.KEY;
    return this;
  }

  /** Return the value writer that is used to write to the "value" field. */
  public UnionMapWriter value() {
    writer.setAddVectorAsNullable(true);
    mode = MapWriteMode.VALUE;
    return this;
  }

  @Override
  public TinyIntWriter tinyInt() {
    switch (mode) {
      case KEY:
        return entryWriter.tinyInt(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.tinyInt(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public UInt1Writer uInt1() {
    switch (mode) {
      case KEY:
        return entryWriter.uInt1(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.uInt1(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public UInt2Writer uInt2() {
    switch (mode) {
      case KEY:
        return entryWriter.uInt2(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.uInt2(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public SmallIntWriter smallInt() {
    switch (mode) {
      case KEY:
        return entryWriter.smallInt(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.smallInt(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public Float2Writer float2() {
    switch (mode) {
      case KEY:
        return entryWriter.float2(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.float2(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public IntWriter integer() {
    switch (mode) {
      case KEY:
        return entryWriter.integer(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.integer(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public UInt4Writer uInt4() {
    switch (mode) {
      case KEY:
        return entryWriter.uInt4(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.uInt4(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public Float4Writer float4() {
    switch (mode) {
      case KEY:
        return entryWriter.float4(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.float4(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public DateDayWriter dateDay() {
    switch (mode) {
      case KEY:
        return entryWriter.dateDay(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.dateDay(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public IntervalYearWriter intervalYear() {
    switch (mode) {
      case KEY:
        return entryWriter.intervalYear(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.intervalYear(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public TimeSecWriter timeSec() {
    switch (mode) {
      case KEY:
        return entryWriter.timeSec(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.timeSec(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public TimeMilliWriter timeMilli() {
    switch (mode) {
      case KEY:
        return entryWriter.timeMilli(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.timeMilli(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public BigIntWriter bigInt() {
    switch (mode) {
      case KEY:
        return entryWriter.bigInt(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.bigInt(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public UInt8Writer uInt8() {
    switch (mode) {
      case KEY:
        return entryWriter.uInt8(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.uInt8(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public Float8Writer float8() {
    switch (mode) {
      case KEY:
        return entryWriter.float8(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.float8(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public DateMilliWriter dateMilli() {
    switch (mode) {
      case KEY:
        return entryWriter.dateMilli(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.dateMilli(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public TimeStampSecWriter timeStampSec() {
    switch (mode) {
      case KEY:
        return entryWriter.timeStampSec(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.timeStampSec(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public TimeStampMilliWriter timeStampMilli() {
    switch (mode) {
      case KEY:
        return entryWriter.timeStampMilli(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.timeStampMilli(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public TimeStampMicroWriter timeStampMicro() {
    switch (mode) {
      case KEY:
        return entryWriter.timeStampMicro(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.timeStampMicro(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public TimeStampNanoWriter timeStampNano() {
    switch (mode) {
      case KEY:
        return entryWriter.timeStampNano(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.timeStampNano(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public TimeMicroWriter timeMicro() {
    switch (mode) {
      case KEY:
        return entryWriter.timeMicro(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.timeMicro(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public TimeNanoWriter timeNano() {
    switch (mode) {
      case KEY:
        return entryWriter.timeNano(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.timeNano(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public IntervalDayWriter intervalDay() {
    switch (mode) {
      case KEY:
        return entryWriter.intervalDay(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.intervalDay(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public IntervalMonthDayNanoWriter intervalMonthDayNano() {
    switch (mode) {
      case KEY:
        return entryWriter.intervalMonthDayNano(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.intervalMonthDayNano(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public VarBinaryWriter varBinary() {
    switch (mode) {
      case KEY:
        return entryWriter.varBinary(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.varBinary(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public VarCharWriter varChar() {
    switch (mode) {
      case KEY:
        return entryWriter.varChar(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.varChar(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public LargeVarCharWriter largeVarChar() {
    switch (mode) {
      case KEY:
        return entryWriter.largeVarChar(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.largeVarChar(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public LargeVarBinaryWriter largeVarBinary() {
    switch (mode) {
      case KEY:
        return entryWriter.largeVarBinary(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.largeVarBinary(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public BitWriter bit() {
    switch (mode) {
      case KEY:
        return entryWriter.bit(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.bit(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public DecimalWriter decimal() {
    switch (mode) {
      case KEY:
        return entryWriter.decimal(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.decimal(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }

  @Override
  public Decimal256Writer decimal256() {
    switch (mode) {
      case KEY:
        return entryWriter.decimal256(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.decimal256(MapVector.VALUE_NAME);
      default:
        return this;
    }
  }


  @Override
  public StructWriter struct() {
    switch (mode) {
      case KEY:
        return entryWriter.struct(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.struct(MapVector.VALUE_NAME);
      default:
        return super.struct();
    }
  }

  @Override
  public ListWriter list() {
    switch (mode) {
      case KEY:
        return entryWriter.list(MapVector.KEY_NAME);
      case VALUE:
        return entryWriter.list(MapVector.VALUE_NAME);
      default:
        return super.list();
    }
  }

  @Override
  public MapWriter map(boolean keysSorted) {
    switch (mode) {
      case KEY:
        return entryWriter.map(MapVector.KEY_NAME, keysSorted);
      case VALUE:
        return entryWriter.map(MapVector.VALUE_NAME, keysSorted);
      default:
        return super.map();
    }
  }
}
