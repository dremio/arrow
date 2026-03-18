

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
 * A FieldWriter which delegates calls to another FieldWriter. The delegate FieldWriter can be promoted to a new type
 * when necessary. Classes that extend this class are responsible for handling promotion.
 *
 * This class is generated using freemarker and the AbstractPromotableFieldWriter.java template.
 *
 */
@SuppressWarnings("unused")
abstract class AbstractPromotableFieldWriter extends AbstractFieldWriter {
  /**
   * Retrieve the FieldWriter, promoting if it is not a FieldWriter of the specified type
   * @param type the type of the values we want to write
   * @return the corresponding field writer
   */
  protected FieldWriter getWriter(MinorType type) {
    return getWriter(type, null);
  }

  abstract protected FieldWriter getWriter(MinorType type, ArrowType arrowType);

  /**
   * @return the current FieldWriter
   */
  abstract protected FieldWriter getWriter();

  @Override
  public void start() {
    getWriter(MinorType.STRUCT).start();
  }

  @Override
  public void end() {
    getWriter(MinorType.STRUCT).end();
    setPosition(idx() + 1);
  }

  @Override
  public void startList() {
    getWriter(MinorType.LIST).startList();
  }

  @Override
  public void endList() {
    getWriter(MinorType.LIST).endList();
    setPosition(idx() + 1);
  }

  @Override
  public void startMap() {
    getWriter(MinorType.MAP).startMap();
  }

  @Override
  public void endMap() {
    getWriter(MinorType.MAP).endMap();
    setPosition(idx() + 1);
  }

  @Override
  public void startEntry() {
    getWriter(MinorType.MAP).startEntry();
  }

  @Override
  public MapWriter key() {
    return getWriter(MinorType.MAP).key();
  }

  @Override
  public MapWriter value() {
    return getWriter(MinorType.MAP).value();
  }

  @Override
  public void endEntry() {
    getWriter(MinorType.MAP).endEntry();
  }

  @Override
  public void write(TinyIntHolder holder) {
    getWriter(MinorType.TINYINT).write(holder);
  }

  public void writeTinyInt(byte value) {
    getWriter(MinorType.TINYINT).writeTinyInt(value);
  }


  @Override
  public void write(UInt1Holder holder) {
    getWriter(MinorType.UINT1).write(holder);
  }

  public void writeUInt1(byte value) {
    getWriter(MinorType.UINT1).writeUInt1(value);
  }


  @Override
  public void write(UInt2Holder holder) {
    getWriter(MinorType.UINT2).write(holder);
  }

  public void writeUInt2(char value) {
    getWriter(MinorType.UINT2).writeUInt2(value);
  }


  @Override
  public void write(SmallIntHolder holder) {
    getWriter(MinorType.SMALLINT).write(holder);
  }

  public void writeSmallInt(short value) {
    getWriter(MinorType.SMALLINT).writeSmallInt(value);
  }


  @Override
  public void write(Float2Holder holder) {
    getWriter(MinorType.FLOAT2).write(holder);
  }

  public void writeFloat2(short value) {
    getWriter(MinorType.FLOAT2).writeFloat2(value);
  }


  @Override
  public void write(IntHolder holder) {
    getWriter(MinorType.INT).write(holder);
  }

  public void writeInt(int value) {
    getWriter(MinorType.INT).writeInt(value);
  }


  @Override
  public void write(UInt4Holder holder) {
    getWriter(MinorType.UINT4).write(holder);
  }

  public void writeUInt4(int value) {
    getWriter(MinorType.UINT4).writeUInt4(value);
  }


  @Override
  public void write(Float4Holder holder) {
    getWriter(MinorType.FLOAT4).write(holder);
  }

  public void writeFloat4(float value) {
    getWriter(MinorType.FLOAT4).writeFloat4(value);
  }


  @Override
  public void write(DateDayHolder holder) {
    getWriter(MinorType.DATEDAY).write(holder);
  }

  public void writeDateDay(int value) {
    getWriter(MinorType.DATEDAY).writeDateDay(value);
  }


  @Override
  public void write(IntervalYearHolder holder) {
    getWriter(MinorType.INTERVALYEAR).write(holder);
  }

  public void writeIntervalYear(int value) {
    getWriter(MinorType.INTERVALYEAR).writeIntervalYear(value);
  }


  @Override
  public void write(TimeSecHolder holder) {
    getWriter(MinorType.TIMESEC).write(holder);
  }

  public void writeTimeSec(int value) {
    getWriter(MinorType.TIMESEC).writeTimeSec(value);
  }


  @Override
  public void write(TimeMilliHolder holder) {
    getWriter(MinorType.TIMEMILLI).write(holder);
  }

  public void writeTimeMilli(int value) {
    getWriter(MinorType.TIMEMILLI).writeTimeMilli(value);
  }


  @Override
  public void write(BigIntHolder holder) {
    getWriter(MinorType.BIGINT).write(holder);
  }

  public void writeBigInt(long value) {
    getWriter(MinorType.BIGINT).writeBigInt(value);
  }


  @Override
  public void write(UInt8Holder holder) {
    getWriter(MinorType.UINT8).write(holder);
  }

  public void writeUInt8(long value) {
    getWriter(MinorType.UINT8).writeUInt8(value);
  }


  @Override
  public void write(Float8Holder holder) {
    getWriter(MinorType.FLOAT8).write(holder);
  }

  public void writeFloat8(double value) {
    getWriter(MinorType.FLOAT8).writeFloat8(value);
  }


  @Override
  public void write(DateMilliHolder holder) {
    getWriter(MinorType.DATEMILLI).write(holder);
  }

  public void writeDateMilli(long value) {
    getWriter(MinorType.DATEMILLI).writeDateMilli(value);
  }


  @Override
  public void write(DurationHolder holder) {
    ArrowType.Duration arrowType = new ArrowType.Duration(holder.unit);
    getWriter(MinorType.DURATION, arrowType).write(holder);
  }

  /**
   * @deprecated
   * If you experience errors with using this version of the method, switch to the holder version.
   * The errors occur when using an untyped or unioned PromotableWriter, because this version of the
   * method does not have enough information to infer the ArrowType.
   * @see #write(DurationHolder)
   */
  @Deprecated
  @Override
  public void writeDuration(long value) {
    getWriter(MinorType.DURATION).writeDuration(value);
  }


  @Override
  public void write(TimeStampSecHolder holder) {
    getWriter(MinorType.TIMESTAMPSEC).write(holder);
  }

  public void writeTimeStampSec(long value) {
    getWriter(MinorType.TIMESTAMPSEC).writeTimeStampSec(value);
  }


  @Override
  public void write(TimeStampMilliHolder holder) {
    getWriter(MinorType.TIMESTAMPMILLI).write(holder);
  }

  public void writeTimeStampMilli(long value) {
    getWriter(MinorType.TIMESTAMPMILLI).writeTimeStampMilli(value);
  }


  @Override
  public void write(TimeStampMicroHolder holder) {
    getWriter(MinorType.TIMESTAMPMICRO).write(holder);
  }

  public void writeTimeStampMicro(long value) {
    getWriter(MinorType.TIMESTAMPMICRO).writeTimeStampMicro(value);
  }


  @Override
  public void write(TimeStampNanoHolder holder) {
    getWriter(MinorType.TIMESTAMPNANO).write(holder);
  }

  public void writeTimeStampNano(long value) {
    getWriter(MinorType.TIMESTAMPNANO).writeTimeStampNano(value);
  }


  @Override
  public void write(TimeStampSecTZHolder holder) {
    ArrowType.Timestamp arrowTypeWithoutTz = (ArrowType.Timestamp) MinorType.TIMESTAMPSEC.getType();
    // Take the holder.timezone similar to how PromotableWriter.java:write(DecimalHolder) takes the scale from the holder.
    ArrowType.Timestamp arrowType = new ArrowType.Timestamp(arrowTypeWithoutTz.getUnit(), holder.timezone);
    getWriter(MinorType.TIMESTAMPSECTZ, arrowType).write(holder);
  }

  /**
   * @deprecated
   * The holder version should be used instead otherwise the timezone will default to UTC.
   * @see #write(TimeStampSecTZHolder)
   */
  @Deprecated
  @Override
  public void writeTimeStampSecTZ(long value) {
    ArrowType.Timestamp arrowTypeWithoutTz = (ArrowType.Timestamp) MinorType.TIMESTAMPSEC.getType();
    // Assumes UTC if no timezone is provided
    ArrowType.Timestamp arrowType = new ArrowType.Timestamp(arrowTypeWithoutTz.getUnit(), "UTC");
    getWriter(MinorType.TIMESTAMPSECTZ, arrowType).writeTimeStampSecTZ(value);
  }


  @Override
  public void write(TimeStampMilliTZHolder holder) {
    ArrowType.Timestamp arrowTypeWithoutTz = (ArrowType.Timestamp) MinorType.TIMESTAMPMILLI.getType();
    // Take the holder.timezone similar to how PromotableWriter.java:write(DecimalHolder) takes the scale from the holder.
    ArrowType.Timestamp arrowType = new ArrowType.Timestamp(arrowTypeWithoutTz.getUnit(), holder.timezone);
    getWriter(MinorType.TIMESTAMPMILLITZ, arrowType).write(holder);
  }

  /**
   * @deprecated
   * The holder version should be used instead otherwise the timezone will default to UTC.
   * @see #write(TimeStampMilliTZHolder)
   */
  @Deprecated
  @Override
  public void writeTimeStampMilliTZ(long value) {
    ArrowType.Timestamp arrowTypeWithoutTz = (ArrowType.Timestamp) MinorType.TIMESTAMPMILLI.getType();
    // Assumes UTC if no timezone is provided
    ArrowType.Timestamp arrowType = new ArrowType.Timestamp(arrowTypeWithoutTz.getUnit(), "UTC");
    getWriter(MinorType.TIMESTAMPMILLITZ, arrowType).writeTimeStampMilliTZ(value);
  }


  @Override
  public void write(TimeStampMicroTZHolder holder) {
    ArrowType.Timestamp arrowTypeWithoutTz = (ArrowType.Timestamp) MinorType.TIMESTAMPMICRO.getType();
    // Take the holder.timezone similar to how PromotableWriter.java:write(DecimalHolder) takes the scale from the holder.
    ArrowType.Timestamp arrowType = new ArrowType.Timestamp(arrowTypeWithoutTz.getUnit(), holder.timezone);
    getWriter(MinorType.TIMESTAMPMICROTZ, arrowType).write(holder);
  }

  /**
   * @deprecated
   * The holder version should be used instead otherwise the timezone will default to UTC.
   * @see #write(TimeStampMicroTZHolder)
   */
  @Deprecated
  @Override
  public void writeTimeStampMicroTZ(long value) {
    ArrowType.Timestamp arrowTypeWithoutTz = (ArrowType.Timestamp) MinorType.TIMESTAMPMICRO.getType();
    // Assumes UTC if no timezone is provided
    ArrowType.Timestamp arrowType = new ArrowType.Timestamp(arrowTypeWithoutTz.getUnit(), "UTC");
    getWriter(MinorType.TIMESTAMPMICROTZ, arrowType).writeTimeStampMicroTZ(value);
  }


  @Override
  public void write(TimeStampNanoTZHolder holder) {
    ArrowType.Timestamp arrowTypeWithoutTz = (ArrowType.Timestamp) MinorType.TIMESTAMPNANO.getType();
    // Take the holder.timezone similar to how PromotableWriter.java:write(DecimalHolder) takes the scale from the holder.
    ArrowType.Timestamp arrowType = new ArrowType.Timestamp(arrowTypeWithoutTz.getUnit(), holder.timezone);
    getWriter(MinorType.TIMESTAMPNANOTZ, arrowType).write(holder);
  }

  /**
   * @deprecated
   * The holder version should be used instead otherwise the timezone will default to UTC.
   * @see #write(TimeStampNanoTZHolder)
   */
  @Deprecated
  @Override
  public void writeTimeStampNanoTZ(long value) {
    ArrowType.Timestamp arrowTypeWithoutTz = (ArrowType.Timestamp) MinorType.TIMESTAMPNANO.getType();
    // Assumes UTC if no timezone is provided
    ArrowType.Timestamp arrowType = new ArrowType.Timestamp(arrowTypeWithoutTz.getUnit(), "UTC");
    getWriter(MinorType.TIMESTAMPNANOTZ, arrowType).writeTimeStampNanoTZ(value);
  }


  @Override
  public void write(TimeMicroHolder holder) {
    getWriter(MinorType.TIMEMICRO).write(holder);
  }

  public void writeTimeMicro(long value) {
    getWriter(MinorType.TIMEMICRO).writeTimeMicro(value);
  }


  @Override
  public void write(TimeNanoHolder holder) {
    getWriter(MinorType.TIMENANO).write(holder);
  }

  public void writeTimeNano(long value) {
    getWriter(MinorType.TIMENANO).writeTimeNano(value);
  }


  @Override
  public void write(IntervalDayHolder holder) {
    getWriter(MinorType.INTERVALDAY).write(holder);
  }

  public void writeIntervalDay(int days, int milliseconds) {
    getWriter(MinorType.INTERVALDAY).writeIntervalDay(days, milliseconds);
  }


  @Override
  public void write(IntervalMonthDayNanoHolder holder) {
    getWriter(MinorType.INTERVALMONTHDAYNANO).write(holder);
  }

  public void writeIntervalMonthDayNano(int months, int days, long nanoseconds) {
    getWriter(MinorType.INTERVALMONTHDAYNANO).writeIntervalMonthDayNano(months, days, nanoseconds);
  }


  @Override
  public void write(Decimal256Holder holder) {
    getWriter(MinorType.DECIMAL256).write(holder);
  }

  public void writeDecimal256(long start, ArrowBuf buffer, ArrowType arrowType) {
    getWriter(MinorType.DECIMAL256).writeDecimal256(start, buffer, arrowType);
  }

  public void writeDecimal256(long start, ArrowBuf buffer) {
    getWriter(MinorType.DECIMAL256).writeDecimal256(start, buffer);
  }
  public void writeBigEndianBytesToDecimal256(byte[] value, ArrowType arrowType) {
    getWriter(MinorType.DECIMAL256).writeBigEndianBytesToDecimal256(value, arrowType);
  }

  public void writeBigEndianBytesToDecimal256(byte[] value) {
    getWriter(MinorType.DECIMAL256).writeBigEndianBytesToDecimal256(value);
  }


  @Override
  public void write(DecimalHolder holder) {
    getWriter(MinorType.DECIMAL).write(holder);
  }

  public void writeDecimal(int start, ArrowBuf buffer, ArrowType arrowType) {
    getWriter(MinorType.DECIMAL).writeDecimal(start, buffer, arrowType);
  }

  public void writeDecimal(int start, ArrowBuf buffer) {
    getWriter(MinorType.DECIMAL).writeDecimal(start, buffer);
  }

  public void writeBigEndianBytesToDecimal(byte[] value, ArrowType arrowType) {
    getWriter(MinorType.DECIMAL).writeBigEndianBytesToDecimal(value, arrowType);
  }

  public void writeBigEndianBytesToDecimal(byte[] value) {
    getWriter(MinorType.DECIMAL).writeBigEndianBytesToDecimal(value);
  }


  @Override
  public void write(FixedSizeBinaryHolder holder) {
    ArrowType.FixedSizeBinary arrowType = new ArrowType.FixedSizeBinary(holder.byteWidth);
    getWriter(MinorType.FIXEDSIZEBINARY, arrowType).write(holder);
  }

  /**
   * @deprecated
   * If you experience errors with using this version of the method, switch to the holder version.
   * The errors occur when using an untyped or unioned PromotableWriter, because this version of the
   * method does not have enough information to infer the ArrowType.
   * @see #write(FixedSizeBinaryHolder)
   */
  @Deprecated
  @Override
  public void writeFixedSizeBinary(ArrowBuf buffer) {
    getWriter(MinorType.FIXEDSIZEBINARY).writeFixedSizeBinary(buffer);
  }


  @Override
  public void write(VarBinaryHolder holder) {
    getWriter(MinorType.VARBINARY).write(holder);
  }

  public void writeVarBinary(int start, int end, ArrowBuf buffer) {
    getWriter(MinorType.VARBINARY).writeVarBinary(start, end, buffer);
  }

  @Override
  public void writeVarBinary(byte[] value) {
    getWriter(MinorType.VARBINARY).writeVarBinary(value);
  }

  @Override
  public void writeVarBinary(byte[] value, int offset, int length) {
    getWriter(MinorType.VARBINARY).writeVarBinary(value, offset, length);
  }

  @Override
  public void writeVarBinary(ByteBuffer value) {
    getWriter(MinorType.VARBINARY).writeVarBinary(value);
  }

  @Override
  public void writeVarBinary(ByteBuffer value, int offset, int length) {
    getWriter(MinorType.VARBINARY).writeVarBinary(value, offset, length);
  }

  @Override
  public void write(VarCharHolder holder) {
    getWriter(MinorType.VARCHAR).write(holder);
  }

  public void writeVarChar(int start, int end, ArrowBuf buffer) {
    getWriter(MinorType.VARCHAR).writeVarChar(start, end, buffer);
  }

  @Override
  public void writeVarChar(Text value) {
    getWriter(MinorType.VARCHAR).writeVarChar(value);
  }

  @Override
  public void writeVarChar(String value) {
    getWriter(MinorType.VARCHAR).writeVarChar(value);
  }

  @Override
  public void write(LargeVarCharHolder holder) {
    getWriter(MinorType.LARGEVARCHAR).write(holder);
  }

  public void writeLargeVarChar(long start, long end, ArrowBuf buffer) {
    getWriter(MinorType.LARGEVARCHAR).writeLargeVarChar(start, end, buffer);
  }

  @Override
  public void writeLargeVarChar(Text value) {
    getWriter(MinorType.LARGEVARCHAR).writeLargeVarChar(value);
  }

  @Override
  public void writeLargeVarChar(String value) {
    getWriter(MinorType.LARGEVARCHAR).writeLargeVarChar(value);
  }

  @Override
  public void write(LargeVarBinaryHolder holder) {
    getWriter(MinorType.LARGEVARBINARY).write(holder);
  }

  public void writeLargeVarBinary(long start, long end, ArrowBuf buffer) {
    getWriter(MinorType.LARGEVARBINARY).writeLargeVarBinary(start, end, buffer);
  }

  @Override
  public void writeLargeVarBinary(byte[] value) {
    getWriter(MinorType.LARGEVARBINARY).writeLargeVarBinary(value);
  }

  @Override
  public void writeLargeVarBinary(byte[] value, int offset, int length) {
    getWriter(MinorType.LARGEVARBINARY).writeLargeVarBinary(value, offset, length);
  }

  @Override
  public void writeLargeVarBinary(ByteBuffer value) {
    getWriter(MinorType.LARGEVARBINARY).writeLargeVarBinary(value);
  }

  @Override
  public void writeLargeVarBinary(ByteBuffer value, int offset, int length) {
    getWriter(MinorType.LARGEVARBINARY).writeLargeVarBinary(value, offset, length);
  }

  @Override
  public void write(BitHolder holder) {
    getWriter(MinorType.BIT).write(holder);
  }

  public void writeBit(int value) {
    getWriter(MinorType.BIT).writeBit(value);
  }


  public void writeNull() {
  }

  @Override
  public StructWriter struct() {
    return getWriter(MinorType.LIST).struct();
  }

  @Override
  public ListWriter list() {
    return getWriter(MinorType.LIST).list();
  }

  @Override
  public MapWriter map() {
    return getWriter(MinorType.LIST).map();
  }

  @Override
  public MapWriter map(boolean keysSorted) {
    return getWriter(MinorType.MAP, new ArrowType.Map(keysSorted));
  }

  @Override
  public StructWriter struct(String name) {
    return getWriter(MinorType.STRUCT).struct(name);
  }

  @Override
  public ListWriter list(String name) {
    return getWriter(MinorType.STRUCT).list(name);
  }

  @Override
  public MapWriter map(String name) {
    return getWriter(MinorType.STRUCT).map(name);
  }

  @Override
  public MapWriter map(String name, boolean keysSorted) {
    return getWriter(MinorType.STRUCT).map(name, keysSorted);
  }

  @Override
  public TinyIntWriter tinyInt(String name) {
    return getWriter(MinorType.STRUCT).tinyInt(name);
  }

  @Override
  public TinyIntWriter tinyInt() {
    return getWriter(MinorType.LIST).tinyInt();
  }


  @Override
  public UInt1Writer uInt1(String name) {
    return getWriter(MinorType.STRUCT).uInt1(name);
  }

  @Override
  public UInt1Writer uInt1() {
    return getWriter(MinorType.LIST).uInt1();
  }


  @Override
  public UInt2Writer uInt2(String name) {
    return getWriter(MinorType.STRUCT).uInt2(name);
  }

  @Override
  public UInt2Writer uInt2() {
    return getWriter(MinorType.LIST).uInt2();
  }


  @Override
  public SmallIntWriter smallInt(String name) {
    return getWriter(MinorType.STRUCT).smallInt(name);
  }

  @Override
  public SmallIntWriter smallInt() {
    return getWriter(MinorType.LIST).smallInt();
  }


  @Override
  public Float2Writer float2(String name) {
    return getWriter(MinorType.STRUCT).float2(name);
  }

  @Override
  public Float2Writer float2() {
    return getWriter(MinorType.LIST).float2();
  }


  @Override
  public IntWriter integer(String name) {
    return getWriter(MinorType.STRUCT).integer(name);
  }

  @Override
  public IntWriter integer() {
    return getWriter(MinorType.LIST).integer();
  }


  @Override
  public UInt4Writer uInt4(String name) {
    return getWriter(MinorType.STRUCT).uInt4(name);
  }

  @Override
  public UInt4Writer uInt4() {
    return getWriter(MinorType.LIST).uInt4();
  }


  @Override
  public Float4Writer float4(String name) {
    return getWriter(MinorType.STRUCT).float4(name);
  }

  @Override
  public Float4Writer float4() {
    return getWriter(MinorType.LIST).float4();
  }


  @Override
  public DateDayWriter dateDay(String name) {
    return getWriter(MinorType.STRUCT).dateDay(name);
  }

  @Override
  public DateDayWriter dateDay() {
    return getWriter(MinorType.LIST).dateDay();
  }


  @Override
  public IntervalYearWriter intervalYear(String name) {
    return getWriter(MinorType.STRUCT).intervalYear(name);
  }

  @Override
  public IntervalYearWriter intervalYear() {
    return getWriter(MinorType.LIST).intervalYear();
  }


  @Override
  public TimeSecWriter timeSec(String name) {
    return getWriter(MinorType.STRUCT).timeSec(name);
  }

  @Override
  public TimeSecWriter timeSec() {
    return getWriter(MinorType.LIST).timeSec();
  }


  @Override
  public TimeMilliWriter timeMilli(String name) {
    return getWriter(MinorType.STRUCT).timeMilli(name);
  }

  @Override
  public TimeMilliWriter timeMilli() {
    return getWriter(MinorType.LIST).timeMilli();
  }


  @Override
  public BigIntWriter bigInt(String name) {
    return getWriter(MinorType.STRUCT).bigInt(name);
  }

  @Override
  public BigIntWriter bigInt() {
    return getWriter(MinorType.LIST).bigInt();
  }


  @Override
  public UInt8Writer uInt8(String name) {
    return getWriter(MinorType.STRUCT).uInt8(name);
  }

  @Override
  public UInt8Writer uInt8() {
    return getWriter(MinorType.LIST).uInt8();
  }


  @Override
  public Float8Writer float8(String name) {
    return getWriter(MinorType.STRUCT).float8(name);
  }

  @Override
  public Float8Writer float8() {
    return getWriter(MinorType.LIST).float8();
  }


  @Override
  public DateMilliWriter dateMilli(String name) {
    return getWriter(MinorType.STRUCT).dateMilli(name);
  }

  @Override
  public DateMilliWriter dateMilli() {
    return getWriter(MinorType.LIST).dateMilli();
  }


  @Override
  public DurationWriter duration(String name, org.apache.arrow.vector.types.TimeUnit unit) {
    return getWriter(MinorType.STRUCT).duration(name, unit);
  }

  @Override
  public DurationWriter duration(String name) {
    return getWriter(MinorType.STRUCT).duration(name);
  }

  @Override
  public DurationWriter duration() {
    return getWriter(MinorType.LIST).duration();
  }


  @Override
  public TimeStampSecWriter timeStampSec(String name) {
    return getWriter(MinorType.STRUCT).timeStampSec(name);
  }

  @Override
  public TimeStampSecWriter timeStampSec() {
    return getWriter(MinorType.LIST).timeStampSec();
  }


  @Override
  public TimeStampMilliWriter timeStampMilli(String name) {
    return getWriter(MinorType.STRUCT).timeStampMilli(name);
  }

  @Override
  public TimeStampMilliWriter timeStampMilli() {
    return getWriter(MinorType.LIST).timeStampMilli();
  }


  @Override
  public TimeStampMicroWriter timeStampMicro(String name) {
    return getWriter(MinorType.STRUCT).timeStampMicro(name);
  }

  @Override
  public TimeStampMicroWriter timeStampMicro() {
    return getWriter(MinorType.LIST).timeStampMicro();
  }


  @Override
  public TimeStampNanoWriter timeStampNano(String name) {
    return getWriter(MinorType.STRUCT).timeStampNano(name);
  }

  @Override
  public TimeStampNanoWriter timeStampNano() {
    return getWriter(MinorType.LIST).timeStampNano();
  }


  @Override
  public TimeStampSecTZWriter timeStampSecTZ(String name, String timezone) {
    return getWriter(MinorType.STRUCT).timeStampSecTZ(name, timezone);
  }

  @Override
  public TimeStampSecTZWriter timeStampSecTZ(String name) {
    return getWriter(MinorType.STRUCT).timeStampSecTZ(name);
  }

  @Override
  public TimeStampSecTZWriter timeStampSecTZ() {
    return getWriter(MinorType.LIST).timeStampSecTZ();
  }


  @Override
  public TimeStampMilliTZWriter timeStampMilliTZ(String name, String timezone) {
    return getWriter(MinorType.STRUCT).timeStampMilliTZ(name, timezone);
  }

  @Override
  public TimeStampMilliTZWriter timeStampMilliTZ(String name) {
    return getWriter(MinorType.STRUCT).timeStampMilliTZ(name);
  }

  @Override
  public TimeStampMilliTZWriter timeStampMilliTZ() {
    return getWriter(MinorType.LIST).timeStampMilliTZ();
  }


  @Override
  public TimeStampMicroTZWriter timeStampMicroTZ(String name, String timezone) {
    return getWriter(MinorType.STRUCT).timeStampMicroTZ(name, timezone);
  }

  @Override
  public TimeStampMicroTZWriter timeStampMicroTZ(String name) {
    return getWriter(MinorType.STRUCT).timeStampMicroTZ(name);
  }

  @Override
  public TimeStampMicroTZWriter timeStampMicroTZ() {
    return getWriter(MinorType.LIST).timeStampMicroTZ();
  }


  @Override
  public TimeStampNanoTZWriter timeStampNanoTZ(String name, String timezone) {
    return getWriter(MinorType.STRUCT).timeStampNanoTZ(name, timezone);
  }

  @Override
  public TimeStampNanoTZWriter timeStampNanoTZ(String name) {
    return getWriter(MinorType.STRUCT).timeStampNanoTZ(name);
  }

  @Override
  public TimeStampNanoTZWriter timeStampNanoTZ() {
    return getWriter(MinorType.LIST).timeStampNanoTZ();
  }


  @Override
  public TimeMicroWriter timeMicro(String name) {
    return getWriter(MinorType.STRUCT).timeMicro(name);
  }

  @Override
  public TimeMicroWriter timeMicro() {
    return getWriter(MinorType.LIST).timeMicro();
  }


  @Override
  public TimeNanoWriter timeNano(String name) {
    return getWriter(MinorType.STRUCT).timeNano(name);
  }

  @Override
  public TimeNanoWriter timeNano() {
    return getWriter(MinorType.LIST).timeNano();
  }


  @Override
  public IntervalDayWriter intervalDay(String name) {
    return getWriter(MinorType.STRUCT).intervalDay(name);
  }

  @Override
  public IntervalDayWriter intervalDay() {
    return getWriter(MinorType.LIST).intervalDay();
  }


  @Override
  public IntervalMonthDayNanoWriter intervalMonthDayNano(String name) {
    return getWriter(MinorType.STRUCT).intervalMonthDayNano(name);
  }

  @Override
  public IntervalMonthDayNanoWriter intervalMonthDayNano() {
    return getWriter(MinorType.LIST).intervalMonthDayNano();
  }


  @Override
  public Decimal256Writer decimal256(String name, int scale, int precision) {
    return getWriter(MinorType.STRUCT).decimal256(name, scale, precision);
  }

  @Override
  public Decimal256Writer decimal256(String name) {
    return getWriter(MinorType.STRUCT).decimal256(name);
  }

  @Override
  public Decimal256Writer decimal256() {
    return getWriter(MinorType.LIST).decimal256();
  }


  @Override
  public DecimalWriter decimal(String name, int scale, int precision) {
    return getWriter(MinorType.STRUCT).decimal(name, scale, precision);
  }

  @Override
  public DecimalWriter decimal(String name) {
    return getWriter(MinorType.STRUCT).decimal(name);
  }

  @Override
  public DecimalWriter decimal() {
    return getWriter(MinorType.LIST).decimal();
  }


  @Override
  public FixedSizeBinaryWriter fixedSizeBinary(String name, int byteWidth) {
    return getWriter(MinorType.STRUCT).fixedSizeBinary(name, byteWidth);
  }

  @Override
  public FixedSizeBinaryWriter fixedSizeBinary(String name) {
    return getWriter(MinorType.STRUCT).fixedSizeBinary(name);
  }

  @Override
  public FixedSizeBinaryWriter fixedSizeBinary() {
    return getWriter(MinorType.LIST).fixedSizeBinary();
  }


  @Override
  public VarBinaryWriter varBinary(String name) {
    return getWriter(MinorType.STRUCT).varBinary(name);
  }

  @Override
  public VarBinaryWriter varBinary() {
    return getWriter(MinorType.LIST).varBinary();
  }


  @Override
  public VarCharWriter varChar(String name) {
    return getWriter(MinorType.STRUCT).varChar(name);
  }

  @Override
  public VarCharWriter varChar() {
    return getWriter(MinorType.LIST).varChar();
  }


  @Override
  public LargeVarCharWriter largeVarChar(String name) {
    return getWriter(MinorType.STRUCT).largeVarChar(name);
  }

  @Override
  public LargeVarCharWriter largeVarChar() {
    return getWriter(MinorType.LIST).largeVarChar();
  }


  @Override
  public LargeVarBinaryWriter largeVarBinary(String name) {
    return getWriter(MinorType.STRUCT).largeVarBinary(name);
  }

  @Override
  public LargeVarBinaryWriter largeVarBinary() {
    return getWriter(MinorType.LIST).largeVarBinary();
  }


  @Override
  public BitWriter bit(String name) {
    return getWriter(MinorType.STRUCT).bit(name);
  }

  @Override
  public BitWriter bit() {
    return getWriter(MinorType.LIST).bit();
  }


  public void copyReader(FieldReader reader) {
    getWriter().copyReader(reader);
  }

  public void copyReaderToField(String name, FieldReader reader) {
    getWriter().copyReaderToField(name, reader);
  }
}
