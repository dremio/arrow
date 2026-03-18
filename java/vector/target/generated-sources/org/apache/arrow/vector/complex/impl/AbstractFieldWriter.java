

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
 * This class is generated using freemarker and the AbstractFieldWriter.java template.
 * Note that changes to the AbstractFieldWriter template should also get reflected in the
 * AbstractPromotableFieldWriter, ComplexWriters, UnionFixedSizeListWriter, UnionListWriter
 * and UnionWriter templates and the PromotableWriter concrete code.
 */
@SuppressWarnings("unused")
abstract class AbstractFieldWriter extends AbstractBaseWriter implements FieldWriter {

  protected boolean addVectorAsNullable = true;

  /**
   * Set flag to control the FieldType.nullable property when a writer creates a new vector.
   * If true then vectors created will be nullable, this is the default behavior. If false then
   * vectors created will be non-nullable.
   *
   * @param nullable Whether or not to create nullable vectors (default behavior is true)
   */
  public void setAddVectorAsNullable(boolean nullable) {
    addVectorAsNullable = nullable;
  }

  @Override
  public void start() {
    throw new IllegalStateException(String.format("You tried to start when you are using a ValueWriter of type %s.", this.getClass().getSimpleName()));
  }

  @Override
  public void end() {
    throw new IllegalStateException(String.format("You tried to end when you are using a ValueWriter of type %s.", this.getClass().getSimpleName()));
  }

  @Override
  public void startList() {
    throw new IllegalStateException(String.format("You tried to start a list when you are using a ValueWriter of type %s.", this.getClass().getSimpleName()));
  }

  @Override
  public void endList() {
    throw new IllegalStateException(String.format("You tried to end a list when you are using a ValueWriter of type %s.", this.getClass().getSimpleName()));
  }

  @Override
  public void startMap() {
    throw new IllegalStateException(String.format("You tried to start a map when you are using a ValueWriter of type %s.", this.getClass().getSimpleName()));
  }

  @Override
  public void endMap() {
    throw new IllegalStateException(String.format("You tried to end a map when you are using a ValueWriter of type %s.", this.getClass().getSimpleName()));
  }

  @Override
  public void startEntry() {
    throw new IllegalStateException(String.format("You tried to start a map entry when you are using a ValueWriter of type %s.", this.getClass().getSimpleName()));
  }

  @Override
  public MapWriter key() {
    throw new IllegalStateException(String.format("You tried to start a map key when you are using a ValueWriter of type %s.", this.getClass().getSimpleName()));
  }

  @Override
  public MapWriter value() {
    throw new IllegalStateException(String.format("You tried to start a map value when you are using a ValueWriter of type %s.", this.getClass().getSimpleName()));
  }

  @Override
  public void endEntry() {
    throw new IllegalStateException(String.format("You tried to end a map entry when you are using a ValueWriter of type %s.", this.getClass().getSimpleName()));
  }

  @Override
  public void write(TinyIntHolder holder) {
    fail("TinyInt");
  }

  public void writeTinyInt(byte value) {
    fail("TinyInt");
  }




  @Override
  public void write(UInt1Holder holder) {
    fail("UInt1");
  }

  public void writeUInt1(byte value) {
    fail("UInt1");
  }




  @Override
  public void write(UInt2Holder holder) {
    fail("UInt2");
  }

  public void writeUInt2(char value) {
    fail("UInt2");
  }




  @Override
  public void write(SmallIntHolder holder) {
    fail("SmallInt");
  }

  public void writeSmallInt(short value) {
    fail("SmallInt");
  }




  @Override
  public void write(Float2Holder holder) {
    fail("Float2");
  }

  public void writeFloat2(short value) {
    fail("Float2");
  }




  @Override
  public void write(IntHolder holder) {
    fail("Int");
  }

  public void writeInt(int value) {
    fail("Int");
  }




  @Override
  public void write(UInt4Holder holder) {
    fail("UInt4");
  }

  public void writeUInt4(int value) {
    fail("UInt4");
  }




  @Override
  public void write(Float4Holder holder) {
    fail("Float4");
  }

  public void writeFloat4(float value) {
    fail("Float4");
  }




  @Override
  public void write(DateDayHolder holder) {
    fail("DateDay");
  }

  public void writeDateDay(int value) {
    fail("DateDay");
  }




  @Override
  public void write(IntervalYearHolder holder) {
    fail("IntervalYear");
  }

  public void writeIntervalYear(int value) {
    fail("IntervalYear");
  }




  @Override
  public void write(TimeSecHolder holder) {
    fail("TimeSec");
  }

  public void writeTimeSec(int value) {
    fail("TimeSec");
  }




  @Override
  public void write(TimeMilliHolder holder) {
    fail("TimeMilli");
  }

  public void writeTimeMilli(int value) {
    fail("TimeMilli");
  }




  @Override
  public void write(BigIntHolder holder) {
    fail("BigInt");
  }

  public void writeBigInt(long value) {
    fail("BigInt");
  }




  @Override
  public void write(UInt8Holder holder) {
    fail("UInt8");
  }

  public void writeUInt8(long value) {
    fail("UInt8");
  }




  @Override
  public void write(Float8Holder holder) {
    fail("Float8");
  }

  public void writeFloat8(double value) {
    fail("Float8");
  }




  @Override
  public void write(DateMilliHolder holder) {
    fail("DateMilli");
  }

  public void writeDateMilli(long value) {
    fail("DateMilli");
  }




  @Override
  public void write(DurationHolder holder) {
    fail("Duration");
  }

  public void writeDuration(long value) {
    fail("Duration");
  }




  @Override
  public void write(TimeStampSecHolder holder) {
    fail("TimeStampSec");
  }

  public void writeTimeStampSec(long value) {
    fail("TimeStampSec");
  }




  @Override
  public void write(TimeStampMilliHolder holder) {
    fail("TimeStampMilli");
  }

  public void writeTimeStampMilli(long value) {
    fail("TimeStampMilli");
  }




  @Override
  public void write(TimeStampMicroHolder holder) {
    fail("TimeStampMicro");
  }

  public void writeTimeStampMicro(long value) {
    fail("TimeStampMicro");
  }




  @Override
  public void write(TimeStampNanoHolder holder) {
    fail("TimeStampNano");
  }

  public void writeTimeStampNano(long value) {
    fail("TimeStampNano");
  }




  @Override
  public void write(TimeStampSecTZHolder holder) {
    fail("TimeStampSecTZ");
  }

  public void writeTimeStampSecTZ(long value) {
    fail("TimeStampSecTZ");
  }




  @Override
  public void write(TimeStampMilliTZHolder holder) {
    fail("TimeStampMilliTZ");
  }

  public void writeTimeStampMilliTZ(long value) {
    fail("TimeStampMilliTZ");
  }




  @Override
  public void write(TimeStampMicroTZHolder holder) {
    fail("TimeStampMicroTZ");
  }

  public void writeTimeStampMicroTZ(long value) {
    fail("TimeStampMicroTZ");
  }




  @Override
  public void write(TimeStampNanoTZHolder holder) {
    fail("TimeStampNanoTZ");
  }

  public void writeTimeStampNanoTZ(long value) {
    fail("TimeStampNanoTZ");
  }




  @Override
  public void write(TimeMicroHolder holder) {
    fail("TimeMicro");
  }

  public void writeTimeMicro(long value) {
    fail("TimeMicro");
  }




  @Override
  public void write(TimeNanoHolder holder) {
    fail("TimeNano");
  }

  public void writeTimeNano(long value) {
    fail("TimeNano");
  }




  @Override
  public void write(IntervalDayHolder holder) {
    fail("IntervalDay");
  }

  public void writeIntervalDay(int days, int milliseconds) {
    fail("IntervalDay");
  }




  @Override
  public void write(IntervalMonthDayNanoHolder holder) {
    fail("IntervalMonthDayNano");
  }

  public void writeIntervalMonthDayNano(int months, int days, long nanoseconds) {
    fail("IntervalMonthDayNano");
  }




  @Override
  public void write(Decimal256Holder holder) {
    fail("Decimal256");
  }

  public void writeDecimal256(long start, ArrowBuf buffer) {
    fail("Decimal256");
  }

  public void writeDecimal256(BigDecimal value) {
    fail("Decimal256");
  }

  public void writeDecimal256(long start, ArrowBuf buffer, ArrowType arrowType) {
    fail("Decimal256");
  }

  public void writeBigEndianBytesToDecimal256(byte[] value) {
    fail("Decimal256");
  }

  public void writeBigEndianBytesToDecimal256(byte[] value, ArrowType arrowType) {
    fail("Decimal256");
  }



  @Override
  public void write(DecimalHolder holder) {
    fail("Decimal");
  }

  public void writeDecimal(long start, ArrowBuf buffer) {
    fail("Decimal");
  }

  public void writeDecimal(BigDecimal value) {
    fail("Decimal");
  }

  public void writeDecimal(long start, ArrowBuf buffer, ArrowType arrowType) {
    fail("Decimal");
  }

  public void writeBigEndianBytesToDecimal(byte[] value) {
    fail("Decimal");
  }

  public void writeBigEndianBytesToDecimal(byte[] value, ArrowType arrowType) {
    fail("Decimal");
  }



  @Override
  public void write(FixedSizeBinaryHolder holder) {
    fail("FixedSizeBinary");
  }

  public void writeFixedSizeBinary(ArrowBuf buffer) {
    fail("FixedSizeBinary");
  }




  @Override
  public void write(VarBinaryHolder holder) {
    fail("VarBinary");
  }

  public void writeVarBinary(int start, int end, ArrowBuf buffer) {
    fail("VarBinary");
  }


  public void writeVarBinary(byte[] value) {
    fail("VarBinary");
  }

  public void writeVarBinary(byte[] value, int offset, int length) {
    fail("VarBinary");
  }

  public void writeVarBinary(ByteBuffer value) {
    fail("VarBinary");
  }

  public void writeVarBinary(ByteBuffer value, int offset, int length) {
    fail("VarBinary");
  }


  @Override
  public void write(VarCharHolder holder) {
    fail("VarChar");
  }

  public void writeVarChar(int start, int end, ArrowBuf buffer) {
    fail("VarChar");
  }



  public void writeVarChar(Text value) {
    fail("VarChar");
  }

  public void writeVarChar(String value) {
    fail("VarChar");
  }

  @Override
  public void write(LargeVarCharHolder holder) {
    fail("LargeVarChar");
  }

  public void writeLargeVarChar(long start, long end, ArrowBuf buffer) {
    fail("LargeVarChar");
  }



  public void writeLargeVarChar(Text value) {
    fail("LargeVarChar");
  }

  public void writeLargeVarChar(String value) {
    fail("LargeVarChar");
  }

  @Override
  public void write(LargeVarBinaryHolder holder) {
    fail("LargeVarBinary");
  }

  public void writeLargeVarBinary(long start, long end, ArrowBuf buffer) {
    fail("LargeVarBinary");
  }


  public void writeLargeVarBinary(byte[] value) {
    fail("LargeVarBinary");
  }

  public void writeLargeVarBinary(byte[] value, int offset, int length) {
    fail("LargeVarBinary");
  }

  public void writeLargeVarBinary(ByteBuffer value) {
    fail("LargeVarBinary");
  }

  public void writeLargeVarBinary(ByteBuffer value, int offset, int length) {
    fail("LargeVarBinary");
  }


  @Override
  public void write(BitHolder holder) {
    fail("Bit");
  }

  public void writeBit(int value) {
    fail("Bit");
  }





  public void writeNull() {
    fail("Bit");
  }

  /**
   * This implementation returns {@code false}.
   * <p>  
   *   Must be overridden by struct writers.
   * </p>  
   */
  @Override
  public boolean isEmptyStruct() {
    return false;
  }

  @Override
  public StructWriter struct() {
    fail("Struct");
    return null;
  }

  @Override
  public ListWriter list() {
    fail("List");
    return null;
  }

  @Override
  public MapWriter map() {
    fail("Map");
    return null;
  }

  @Override
  public StructWriter struct(String name) {
    fail("Struct");
    return null;
  }

  @Override
  public ListWriter list(String name) {
    fail("List");
    return null;
  }

  @Override
  public MapWriter map(String name) {
    fail("Map");
    return null;
  }

  @Override
  public MapWriter map(boolean keysSorted) {
    fail("Map");
    return null;
  }

  @Override
  public MapWriter map(String name, boolean keysSorted) {
    fail("Map");
    return null;
  }

  @Override
  public TinyIntWriter tinyInt(String name) {
    fail("TinyInt");
    return null;
  }

  @Override
  public TinyIntWriter tinyInt() {
    fail("TinyInt");
    return null;
  }


  @Override
  public UInt1Writer uInt1(String name) {
    fail("UInt1");
    return null;
  }

  @Override
  public UInt1Writer uInt1() {
    fail("UInt1");
    return null;
  }


  @Override
  public UInt2Writer uInt2(String name) {
    fail("UInt2");
    return null;
  }

  @Override
  public UInt2Writer uInt2() {
    fail("UInt2");
    return null;
  }


  @Override
  public SmallIntWriter smallInt(String name) {
    fail("SmallInt");
    return null;
  }

  @Override
  public SmallIntWriter smallInt() {
    fail("SmallInt");
    return null;
  }


  @Override
  public Float2Writer float2(String name) {
    fail("Float2");
    return null;
  }

  @Override
  public Float2Writer float2() {
    fail("Float2");
    return null;
  }


  @Override
  public IntWriter integer(String name) {
    fail("Int");
    return null;
  }

  @Override
  public IntWriter integer() {
    fail("Int");
    return null;
  }


  @Override
  public UInt4Writer uInt4(String name) {
    fail("UInt4");
    return null;
  }

  @Override
  public UInt4Writer uInt4() {
    fail("UInt4");
    return null;
  }


  @Override
  public Float4Writer float4(String name) {
    fail("Float4");
    return null;
  }

  @Override
  public Float4Writer float4() {
    fail("Float4");
    return null;
  }


  @Override
  public DateDayWriter dateDay(String name) {
    fail("DateDay");
    return null;
  }

  @Override
  public DateDayWriter dateDay() {
    fail("DateDay");
    return null;
  }


  @Override
  public IntervalYearWriter intervalYear(String name) {
    fail("IntervalYear");
    return null;
  }

  @Override
  public IntervalYearWriter intervalYear() {
    fail("IntervalYear");
    return null;
  }


  @Override
  public TimeSecWriter timeSec(String name) {
    fail("TimeSec");
    return null;
  }

  @Override
  public TimeSecWriter timeSec() {
    fail("TimeSec");
    return null;
  }


  @Override
  public TimeMilliWriter timeMilli(String name) {
    fail("TimeMilli");
    return null;
  }

  @Override
  public TimeMilliWriter timeMilli() {
    fail("TimeMilli");
    return null;
  }


  @Override
  public BigIntWriter bigInt(String name) {
    fail("BigInt");
    return null;
  }

  @Override
  public BigIntWriter bigInt() {
    fail("BigInt");
    return null;
  }


  @Override
  public UInt8Writer uInt8(String name) {
    fail("UInt8");
    return null;
  }

  @Override
  public UInt8Writer uInt8() {
    fail("UInt8");
    return null;
  }


  @Override
  public Float8Writer float8(String name) {
    fail("Float8");
    return null;
  }

  @Override
  public Float8Writer float8() {
    fail("Float8");
    return null;
  }


  @Override
  public DateMilliWriter dateMilli(String name) {
    fail("DateMilli");
    return null;
  }

  @Override
  public DateMilliWriter dateMilli() {
    fail("DateMilli");
    return null;
  }


  @Override
  public DurationWriter duration(String name, org.apache.arrow.vector.types.TimeUnit unit) {
    fail("Duration(" + "unit: " + unit + ", " + ")");
    return null;
  }

  @Override
  public DurationWriter duration(String name) {
    fail("Duration");
    return null;
  }

  @Override
  public DurationWriter duration() {
    fail("Duration");
    return null;
  }


  @Override
  public TimeStampSecWriter timeStampSec(String name) {
    fail("TimeStampSec");
    return null;
  }

  @Override
  public TimeStampSecWriter timeStampSec() {
    fail("TimeStampSec");
    return null;
  }


  @Override
  public TimeStampMilliWriter timeStampMilli(String name) {
    fail("TimeStampMilli");
    return null;
  }

  @Override
  public TimeStampMilliWriter timeStampMilli() {
    fail("TimeStampMilli");
    return null;
  }


  @Override
  public TimeStampMicroWriter timeStampMicro(String name) {
    fail("TimeStampMicro");
    return null;
  }

  @Override
  public TimeStampMicroWriter timeStampMicro() {
    fail("TimeStampMicro");
    return null;
  }


  @Override
  public TimeStampNanoWriter timeStampNano(String name) {
    fail("TimeStampNano");
    return null;
  }

  @Override
  public TimeStampNanoWriter timeStampNano() {
    fail("TimeStampNano");
    return null;
  }


  @Override
  public TimeStampSecTZWriter timeStampSecTZ(String name, String timezone) {
    fail("TimeStampSecTZ(" + "timezone: " + timezone + ", " + ")");
    return null;
  }

  @Override
  public TimeStampSecTZWriter timeStampSecTZ(String name) {
    fail("TimeStampSecTZ");
    return null;
  }

  @Override
  public TimeStampSecTZWriter timeStampSecTZ() {
    fail("TimeStampSecTZ");
    return null;
  }


  @Override
  public TimeStampMilliTZWriter timeStampMilliTZ(String name, String timezone) {
    fail("TimeStampMilliTZ(" + "timezone: " + timezone + ", " + ")");
    return null;
  }

  @Override
  public TimeStampMilliTZWriter timeStampMilliTZ(String name) {
    fail("TimeStampMilliTZ");
    return null;
  }

  @Override
  public TimeStampMilliTZWriter timeStampMilliTZ() {
    fail("TimeStampMilliTZ");
    return null;
  }


  @Override
  public TimeStampMicroTZWriter timeStampMicroTZ(String name, String timezone) {
    fail("TimeStampMicroTZ(" + "timezone: " + timezone + ", " + ")");
    return null;
  }

  @Override
  public TimeStampMicroTZWriter timeStampMicroTZ(String name) {
    fail("TimeStampMicroTZ");
    return null;
  }

  @Override
  public TimeStampMicroTZWriter timeStampMicroTZ() {
    fail("TimeStampMicroTZ");
    return null;
  }


  @Override
  public TimeStampNanoTZWriter timeStampNanoTZ(String name, String timezone) {
    fail("TimeStampNanoTZ(" + "timezone: " + timezone + ", " + ")");
    return null;
  }

  @Override
  public TimeStampNanoTZWriter timeStampNanoTZ(String name) {
    fail("TimeStampNanoTZ");
    return null;
  }

  @Override
  public TimeStampNanoTZWriter timeStampNanoTZ() {
    fail("TimeStampNanoTZ");
    return null;
  }


  @Override
  public TimeMicroWriter timeMicro(String name) {
    fail("TimeMicro");
    return null;
  }

  @Override
  public TimeMicroWriter timeMicro() {
    fail("TimeMicro");
    return null;
  }


  @Override
  public TimeNanoWriter timeNano(String name) {
    fail("TimeNano");
    return null;
  }

  @Override
  public TimeNanoWriter timeNano() {
    fail("TimeNano");
    return null;
  }


  @Override
  public IntervalDayWriter intervalDay(String name) {
    fail("IntervalDay");
    return null;
  }

  @Override
  public IntervalDayWriter intervalDay() {
    fail("IntervalDay");
    return null;
  }


  @Override
  public IntervalMonthDayNanoWriter intervalMonthDayNano(String name) {
    fail("IntervalMonthDayNano");
    return null;
  }

  @Override
  public IntervalMonthDayNanoWriter intervalMonthDayNano() {
    fail("IntervalMonthDayNano");
    return null;
  }


  @Override
  public Decimal256Writer decimal256(String name, int scale, int precision) {
    fail("Decimal256(" + "scale: " + scale + ", " + "precision: " + precision + ", " + ")");
    return null;
  }

  @Override
  public Decimal256Writer decimal256(String name) {
    fail("Decimal256");
    return null;
  }

  @Override
  public Decimal256Writer decimal256() {
    fail("Decimal256");
    return null;
  }


  @Override
  public DecimalWriter decimal(String name, int scale, int precision) {
    fail("Decimal(" + "scale: " + scale + ", " + "precision: " + precision + ", " + ")");
    return null;
  }

  @Override
  public DecimalWriter decimal(String name) {
    fail("Decimal");
    return null;
  }

  @Override
  public DecimalWriter decimal() {
    fail("Decimal");
    return null;
  }


  @Override
  public FixedSizeBinaryWriter fixedSizeBinary(String name, int byteWidth) {
    fail("FixedSizeBinary(" + "byteWidth: " + byteWidth + ", " + ")");
    return null;
  }

  @Override
  public FixedSizeBinaryWriter fixedSizeBinary(String name) {
    fail("FixedSizeBinary");
    return null;
  }

  @Override
  public FixedSizeBinaryWriter fixedSizeBinary() {
    fail("FixedSizeBinary");
    return null;
  }


  @Override
  public VarBinaryWriter varBinary(String name) {
    fail("VarBinary");
    return null;
  }

  @Override
  public VarBinaryWriter varBinary() {
    fail("VarBinary");
    return null;
  }


  @Override
  public VarCharWriter varChar(String name) {
    fail("VarChar");
    return null;
  }

  @Override
  public VarCharWriter varChar() {
    fail("VarChar");
    return null;
  }


  @Override
  public LargeVarCharWriter largeVarChar(String name) {
    fail("LargeVarChar");
    return null;
  }

  @Override
  public LargeVarCharWriter largeVarChar() {
    fail("LargeVarChar");
    return null;
  }


  @Override
  public LargeVarBinaryWriter largeVarBinary(String name) {
    fail("LargeVarBinary");
    return null;
  }

  @Override
  public LargeVarBinaryWriter largeVarBinary() {
    fail("LargeVarBinary");
    return null;
  }


  @Override
  public BitWriter bit(String name) {
    fail("Bit");
    return null;
  }

  @Override
  public BitWriter bit() {
    fail("Bit");
    return null;
  }


  public void copyReader(FieldReader reader) {
    fail("Copy FieldReader");
  }

  public void copyReaderToField(String name, FieldReader reader) {
    fail("Copy FieldReader to STring");
  }

  private void fail(String name) {
    throw new IllegalArgumentException(String.format("You tried to write a %s type when you are using a ValueWriter of type %s.", name, this.getClass().getSimpleName()));
  }
}
