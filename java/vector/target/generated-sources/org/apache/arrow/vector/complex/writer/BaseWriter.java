

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
package org.apache.arrow.vector.complex.writer;


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
 * File generated from BaseWriter.java using FreeMarker.
 */
@SuppressWarnings("unused")
public interface BaseWriter extends AutoCloseable, Positionable {
  int getValueCapacity();
  void writeNull();

  public interface StructWriter extends BaseWriter {

    Field getField();

    /**
     * Whether this writer is a struct writer and is empty (has no children).
     *
     * <p>
     *   Intended only for use in determining whether to add dummy vector to
     *   avoid empty (zero-column) schema, as in JsonReader.
     * </p>
     * @return whether the struct is empty
     */
    boolean isEmptyStruct();

    TinyIntWriter tinyInt(String name);
    UInt1Writer uInt1(String name);
    UInt2Writer uInt2(String name);
    SmallIntWriter smallInt(String name);
    Float2Writer float2(String name);
    IntWriter integer(String name);
    UInt4Writer uInt4(String name);
    Float4Writer float4(String name);
    DateDayWriter dateDay(String name);
    IntervalYearWriter intervalYear(String name);
    TimeSecWriter timeSec(String name);
    TimeMilliWriter timeMilli(String name);
    BigIntWriter bigInt(String name);
    UInt8Writer uInt8(String name);
    Float8Writer float8(String name);
    DateMilliWriter dateMilli(String name);
    DurationWriter duration(String name, org.apache.arrow.vector.types.TimeUnit unit);
    DurationWriter duration(String name);
    TimeStampSecWriter timeStampSec(String name);
    TimeStampMilliWriter timeStampMilli(String name);
    TimeStampMicroWriter timeStampMicro(String name);
    TimeStampNanoWriter timeStampNano(String name);
    TimeStampSecTZWriter timeStampSecTZ(String name, String timezone);
    TimeStampSecTZWriter timeStampSecTZ(String name);
    TimeStampMilliTZWriter timeStampMilliTZ(String name, String timezone);
    TimeStampMilliTZWriter timeStampMilliTZ(String name);
    TimeStampMicroTZWriter timeStampMicroTZ(String name, String timezone);
    TimeStampMicroTZWriter timeStampMicroTZ(String name);
    TimeStampNanoTZWriter timeStampNanoTZ(String name, String timezone);
    TimeStampNanoTZWriter timeStampNanoTZ(String name);
    TimeMicroWriter timeMicro(String name);
    TimeNanoWriter timeNano(String name);
    IntervalDayWriter intervalDay(String name);
    IntervalMonthDayNanoWriter intervalMonthDayNano(String name);
    Decimal256Writer decimal256(String name, int scale, int precision);
    Decimal256Writer decimal256(String name);
    DecimalWriter decimal(String name, int scale, int precision);
    DecimalWriter decimal(String name);
    FixedSizeBinaryWriter fixedSizeBinary(String name, int byteWidth);
    FixedSizeBinaryWriter fixedSizeBinary(String name);
    VarBinaryWriter varBinary(String name);
    VarCharWriter varChar(String name);
    LargeVarCharWriter largeVarChar(String name);
    LargeVarBinaryWriter largeVarBinary(String name);
    BitWriter bit(String name);

    void copyReaderToField(String name, FieldReader reader);
    StructWriter struct(String name);
    ListWriter list(String name);
    MapWriter map(String name);
    MapWriter map(String name, boolean keysSorted);
    void start();
    void end();
  }

  public interface ListWriter extends BaseWriter {
    void startList();
    void endList();
    StructWriter struct();
    ListWriter list();
    MapWriter map();
    MapWriter map(boolean keysSorted);
    void copyReader(FieldReader reader);

    TinyIntWriter tinyInt();
    UInt1Writer uInt1();
    UInt2Writer uInt2();
    SmallIntWriter smallInt();
    Float2Writer float2();
    IntWriter integer();
    UInt4Writer uInt4();
    Float4Writer float4();
    DateDayWriter dateDay();
    IntervalYearWriter intervalYear();
    TimeSecWriter timeSec();
    TimeMilliWriter timeMilli();
    BigIntWriter bigInt();
    UInt8Writer uInt8();
    Float8Writer float8();
    DateMilliWriter dateMilli();
    DurationWriter duration();
    TimeStampSecWriter timeStampSec();
    TimeStampMilliWriter timeStampMilli();
    TimeStampMicroWriter timeStampMicro();
    TimeStampNanoWriter timeStampNano();
    TimeStampSecTZWriter timeStampSecTZ();
    TimeStampMilliTZWriter timeStampMilliTZ();
    TimeStampMicroTZWriter timeStampMicroTZ();
    TimeStampNanoTZWriter timeStampNanoTZ();
    TimeMicroWriter timeMicro();
    TimeNanoWriter timeNano();
    IntervalDayWriter intervalDay();
    IntervalMonthDayNanoWriter intervalMonthDayNano();
    Decimal256Writer decimal256();
    DecimalWriter decimal();
    FixedSizeBinaryWriter fixedSizeBinary();
    VarBinaryWriter varBinary();
    VarCharWriter varChar();
    LargeVarCharWriter largeVarChar();
    LargeVarBinaryWriter largeVarBinary();
    BitWriter bit();
  }

  public interface MapWriter extends ListWriter {
    void startMap();
    void endMap();

    void startEntry();
    void endEntry();

    MapWriter key();
    MapWriter value();
  }

  public interface ScalarWriter extends
   TinyIntWriter,  UInt1Writer,  UInt2Writer,  SmallIntWriter,  Float2Writer,  IntWriter,  UInt4Writer,  Float4Writer,  DateDayWriter,  IntervalYearWriter,  TimeSecWriter,  TimeMilliWriter,  BigIntWriter,  UInt8Writer,  Float8Writer,  DateMilliWriter,  DurationWriter,  TimeStampSecWriter,  TimeStampMilliWriter,  TimeStampMicroWriter,  TimeStampNanoWriter,  TimeStampSecTZWriter,  TimeStampMilliTZWriter,  TimeStampMicroTZWriter,  TimeStampNanoTZWriter,  TimeMicroWriter,  TimeNanoWriter,  IntervalDayWriter,  IntervalMonthDayNanoWriter,  Decimal256Writer,  DecimalWriter,  FixedSizeBinaryWriter,  VarBinaryWriter,  VarCharWriter,  LargeVarCharWriter,  LargeVarBinaryWriter,  BitWriter,  BaseWriter {}

  public interface ComplexWriter {
    void allocate();
    void clear();
    void copyReader(FieldReader reader);
    StructWriter rootAsStruct();
    ListWriter rootAsList();
    MapWriter rootAsMap(boolean keysSorted);

    void setPosition(int index);
    void setValueCount(int count);
    void reset();
  }

  public interface StructOrListWriter {
    void start();
    void end();
    StructOrListWriter struct(String name);
    /**
     * @deprecated use {@link #listOfStruct()} instead.
     */
    StructOrListWriter listoftstruct(String name);
    StructOrListWriter listOfStruct(String name);
    StructOrListWriter list(String name);
    boolean isStructWriter();
    boolean isListWriter();
    VarCharWriter varChar(String name);
    IntWriter integer(String name);
    BigIntWriter bigInt(String name);
    Float4Writer float4(String name);
    Float8Writer float8(String name);
    BitWriter bit(String name);
    VarBinaryWriter binary(String name);
  }
}
