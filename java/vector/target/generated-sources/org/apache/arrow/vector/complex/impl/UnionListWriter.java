
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

import static org.apache.arrow.memory.util.LargeMemoryUtil.checkedCastToInt;

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
 * This class is generated using freemarker and the UnionListWriter.java template.
 */

@SuppressWarnings("unused")
public class UnionListWriter extends AbstractFieldWriter {

  protected ListVector vector;
  protected PromotableWriter writer;
  private boolean inStruct = false;
  private boolean listStarted = false;
  private String structName;
  private static final int OFFSET_WIDTH = 4;

  public UnionListWriter(ListVector vector) {
    this(vector, NullableStructWriterFactory.getNullableStructWriterFactoryInstance());
  }

  public UnionListWriter(ListVector vector, NullableStructWriterFactory nullableStructWriterFactory) {
    this.vector = vector;
    this.writer = new PromotableWriter(vector.getDataVector(), vector, nullableStructWriterFactory);
  }

  public UnionListWriter(ListVector vector, AbstractFieldWriter parent) {
    this(vector);
  }

  @Override
  public void allocate() {
    vector.allocateNew();
  }

  @Override
  public void clear() {
    vector.clear();
  }

  @Override
  public Field getField() {
    return vector.getField();
  }

  public void setValueCount(int count) {
    vector.setValueCount(count);
  }

  @Override
  public int getValueCapacity() {
    return vector.getValueCapacity();
  }

  @Override
  public void close() throws Exception {
    vector.close();
    writer.close();
  }

  @Override
  public void setPosition(int index) {
    super.setPosition(index);
  }

  @Override
  public TinyIntWriter tinyInt() {
    return this;
  }


  @Override
  public TinyIntWriter tinyInt(String name) {
    structName = name;
    return writer.tinyInt(name);
  }

  @Override
  public UInt1Writer uInt1() {
    return this;
  }


  @Override
  public UInt1Writer uInt1(String name) {
    structName = name;
    return writer.uInt1(name);
  }

  @Override
  public UInt2Writer uInt2() {
    return this;
  }


  @Override
  public UInt2Writer uInt2(String name) {
    structName = name;
    return writer.uInt2(name);
  }

  @Override
  public SmallIntWriter smallInt() {
    return this;
  }


  @Override
  public SmallIntWriter smallInt(String name) {
    structName = name;
    return writer.smallInt(name);
  }

  @Override
  public Float2Writer float2() {
    return this;
  }


  @Override
  public Float2Writer float2(String name) {
    structName = name;
    return writer.float2(name);
  }

  @Override
  public IntWriter integer() {
    return this;
  }


  @Override
  public IntWriter integer(String name) {
    structName = name;
    return writer.integer(name);
  }

  @Override
  public UInt4Writer uInt4() {
    return this;
  }


  @Override
  public UInt4Writer uInt4(String name) {
    structName = name;
    return writer.uInt4(name);
  }

  @Override
  public Float4Writer float4() {
    return this;
  }


  @Override
  public Float4Writer float4(String name) {
    structName = name;
    return writer.float4(name);
  }

  @Override
  public DateDayWriter dateDay() {
    return this;
  }


  @Override
  public DateDayWriter dateDay(String name) {
    structName = name;
    return writer.dateDay(name);
  }

  @Override
  public IntervalYearWriter intervalYear() {
    return this;
  }


  @Override
  public IntervalYearWriter intervalYear(String name) {
    structName = name;
    return writer.intervalYear(name);
  }

  @Override
  public TimeSecWriter timeSec() {
    return this;
  }


  @Override
  public TimeSecWriter timeSec(String name) {
    structName = name;
    return writer.timeSec(name);
  }

  @Override
  public TimeMilliWriter timeMilli() {
    return this;
  }


  @Override
  public TimeMilliWriter timeMilli(String name) {
    structName = name;
    return writer.timeMilli(name);
  }

  @Override
  public BigIntWriter bigInt() {
    return this;
  }


  @Override
  public BigIntWriter bigInt(String name) {
    structName = name;
    return writer.bigInt(name);
  }

  @Override
  public UInt8Writer uInt8() {
    return this;
  }


  @Override
  public UInt8Writer uInt8(String name) {
    structName = name;
    return writer.uInt8(name);
  }

  @Override
  public Float8Writer float8() {
    return this;
  }


  @Override
  public Float8Writer float8(String name) {
    structName = name;
    return writer.float8(name);
  }

  @Override
  public DateMilliWriter dateMilli() {
    return this;
  }


  @Override
  public DateMilliWriter dateMilli(String name) {
    structName = name;
    return writer.dateMilli(name);
  }

  @Override
  public DurationWriter duration() {
    return this;
  }

  @Override
  public DurationWriter duration(String name, org.apache.arrow.vector.types.TimeUnit unit) {
    return writer.duration(name, unit);
  }

  @Override
  public DurationWriter duration(String name) {
    structName = name;
    return writer.duration(name);
  }

  @Override
  public TimeStampSecWriter timeStampSec() {
    return this;
  }


  @Override
  public TimeStampSecWriter timeStampSec(String name) {
    structName = name;
    return writer.timeStampSec(name);
  }

  @Override
  public TimeStampMilliWriter timeStampMilli() {
    return this;
  }


  @Override
  public TimeStampMilliWriter timeStampMilli(String name) {
    structName = name;
    return writer.timeStampMilli(name);
  }

  @Override
  public TimeStampMicroWriter timeStampMicro() {
    return this;
  }


  @Override
  public TimeStampMicroWriter timeStampMicro(String name) {
    structName = name;
    return writer.timeStampMicro(name);
  }

  @Override
  public TimeStampNanoWriter timeStampNano() {
    return this;
  }


  @Override
  public TimeStampNanoWriter timeStampNano(String name) {
    structName = name;
    return writer.timeStampNano(name);
  }

  @Override
  public TimeStampSecTZWriter timeStampSecTZ() {
    return this;
  }

  @Override
  public TimeStampSecTZWriter timeStampSecTZ(String name, String timezone) {
    return writer.timeStampSecTZ(name, timezone);
  }

  @Override
  public TimeStampSecTZWriter timeStampSecTZ(String name) {
    structName = name;
    return writer.timeStampSecTZ(name);
  }

  @Override
  public TimeStampMilliTZWriter timeStampMilliTZ() {
    return this;
  }

  @Override
  public TimeStampMilliTZWriter timeStampMilliTZ(String name, String timezone) {
    return writer.timeStampMilliTZ(name, timezone);
  }

  @Override
  public TimeStampMilliTZWriter timeStampMilliTZ(String name) {
    structName = name;
    return writer.timeStampMilliTZ(name);
  }

  @Override
  public TimeStampMicroTZWriter timeStampMicroTZ() {
    return this;
  }

  @Override
  public TimeStampMicroTZWriter timeStampMicroTZ(String name, String timezone) {
    return writer.timeStampMicroTZ(name, timezone);
  }

  @Override
  public TimeStampMicroTZWriter timeStampMicroTZ(String name) {
    structName = name;
    return writer.timeStampMicroTZ(name);
  }

  @Override
  public TimeStampNanoTZWriter timeStampNanoTZ() {
    return this;
  }

  @Override
  public TimeStampNanoTZWriter timeStampNanoTZ(String name, String timezone) {
    return writer.timeStampNanoTZ(name, timezone);
  }

  @Override
  public TimeStampNanoTZWriter timeStampNanoTZ(String name) {
    structName = name;
    return writer.timeStampNanoTZ(name);
  }

  @Override
  public TimeMicroWriter timeMicro() {
    return this;
  }


  @Override
  public TimeMicroWriter timeMicro(String name) {
    structName = name;
    return writer.timeMicro(name);
  }

  @Override
  public TimeNanoWriter timeNano() {
    return this;
  }


  @Override
  public TimeNanoWriter timeNano(String name) {
    structName = name;
    return writer.timeNano(name);
  }

  @Override
  public IntervalDayWriter intervalDay() {
    return this;
  }


  @Override
  public IntervalDayWriter intervalDay(String name) {
    structName = name;
    return writer.intervalDay(name);
  }

  @Override
  public IntervalMonthDayNanoWriter intervalMonthDayNano() {
    return this;
  }


  @Override
  public IntervalMonthDayNanoWriter intervalMonthDayNano(String name) {
    structName = name;
    return writer.intervalMonthDayNano(name);
  }

  @Override
  public Decimal256Writer decimal256() {
    return this;
  }

  @Override
  public Decimal256Writer decimal256(String name, int scale, int precision) {
    return writer.decimal256(name, scale, precision);
  }

  @Override
  public Decimal256Writer decimal256(String name) {
    structName = name;
    return writer.decimal256(name);
  }

  @Override
  public DecimalWriter decimal() {
    return this;
  }

  @Override
  public DecimalWriter decimal(String name, int scale, int precision) {
    return writer.decimal(name, scale, precision);
  }

  @Override
  public DecimalWriter decimal(String name) {
    structName = name;
    return writer.decimal(name);
  }

  @Override
  public FixedSizeBinaryWriter fixedSizeBinary() {
    return this;
  }

  @Override
  public FixedSizeBinaryWriter fixedSizeBinary(String name, int byteWidth) {
    return writer.fixedSizeBinary(name, byteWidth);
  }

  @Override
  public FixedSizeBinaryWriter fixedSizeBinary(String name) {
    structName = name;
    return writer.fixedSizeBinary(name);
  }

  @Override
  public VarBinaryWriter varBinary() {
    return this;
  }


  @Override
  public VarBinaryWriter varBinary(String name) {
    structName = name;
    return writer.varBinary(name);
  }

  @Override
  public VarCharWriter varChar() {
    return this;
  }


  @Override
  public VarCharWriter varChar(String name) {
    structName = name;
    return writer.varChar(name);
  }

  @Override
  public LargeVarCharWriter largeVarChar() {
    return this;
  }


  @Override
  public LargeVarCharWriter largeVarChar(String name) {
    structName = name;
    return writer.largeVarChar(name);
  }

  @Override
  public LargeVarBinaryWriter largeVarBinary() {
    return this;
  }


  @Override
  public LargeVarBinaryWriter largeVarBinary(String name) {
    structName = name;
    return writer.largeVarBinary(name);
  }

  @Override
  public BitWriter bit() {
    return this;
  }


  @Override
  public BitWriter bit(String name) {
    structName = name;
    return writer.bit(name);
  }


  @Override
  public StructWriter struct() {
    inStruct = true;
    return this;
  }

  @Override
  public ListWriter list() {
    return writer;
  }

  @Override
  public ListWriter list(String name) {
    ListWriter listWriter = writer.list(name);
    return listWriter;
  }

  @Override
  public StructWriter struct(String name) {
    StructWriter structWriter = writer.struct(name);
    return structWriter;
  }

  @Override
  public MapWriter map() {
    return writer;
  }

  @Override
  public MapWriter map(String name) {
    MapWriter mapWriter = writer.map(name);
    return mapWriter;
  }

  @Override
  public MapWriter map(boolean keysSorted) {
    writer.map(keysSorted);
    return writer;
  }

  @Override
  public MapWriter map(String name, boolean keysSorted) {
    MapWriter mapWriter = writer.map(name, keysSorted);
    return mapWriter;
  }

  @Override
  public void startList() {
    vector.startNewValue(idx());
    writer.setPosition(vector.getOffsetBuffer().getInt((idx() + 1L) * OFFSET_WIDTH));
    listStarted = true;
  }

  @Override
  public void endList() {
    vector.getOffsetBuffer().setInt((idx() + 1L) * OFFSET_WIDTH, writer.idx());
    setPosition(idx() + 1);
    listStarted = false;
  }

  @Override
  public void start() {
    writer.start();
  }

  @Override
  public void end() {
    writer.end();
    inStruct = false;
  }

  @Override
  public void writeNull() {
    if (!listStarted){
      vector.setNull(idx());
    } else {
      writer.writeNull();
    }
  }

  @Override
  public void writeTinyInt(byte value) {
    writer.writeTinyInt(value);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(TinyIntHolder holder) {
    writer.writeTinyInt(holder.value);
    writer.setPosition(writer.idx()+1);
  }


  @Override
  public void writeUInt1(byte value) {
    writer.writeUInt1(value);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(UInt1Holder holder) {
    writer.writeUInt1(holder.value);
    writer.setPosition(writer.idx()+1);
  }


  @Override
  public void writeUInt2(char value) {
    writer.writeUInt2(value);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(UInt2Holder holder) {
    writer.writeUInt2(holder.value);
    writer.setPosition(writer.idx()+1);
  }


  @Override
  public void writeSmallInt(short value) {
    writer.writeSmallInt(value);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(SmallIntHolder holder) {
    writer.writeSmallInt(holder.value);
    writer.setPosition(writer.idx()+1);
  }


  @Override
  public void writeFloat2(short value) {
    writer.writeFloat2(value);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(Float2Holder holder) {
    writer.writeFloat2(holder.value);
    writer.setPosition(writer.idx()+1);
  }


  @Override
  public void writeInt(int value) {
    writer.writeInt(value);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(IntHolder holder) {
    writer.writeInt(holder.value);
    writer.setPosition(writer.idx()+1);
  }


  @Override
  public void writeUInt4(int value) {
    writer.writeUInt4(value);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(UInt4Holder holder) {
    writer.writeUInt4(holder.value);
    writer.setPosition(writer.idx()+1);
  }


  @Override
  public void writeFloat4(float value) {
    writer.writeFloat4(value);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(Float4Holder holder) {
    writer.writeFloat4(holder.value);
    writer.setPosition(writer.idx()+1);
  }


  @Override
  public void writeDateDay(int value) {
    writer.writeDateDay(value);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(DateDayHolder holder) {
    writer.writeDateDay(holder.value);
    writer.setPosition(writer.idx()+1);
  }


  @Override
  public void writeIntervalYear(int value) {
    writer.writeIntervalYear(value);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(IntervalYearHolder holder) {
    writer.writeIntervalYear(holder.value);
    writer.setPosition(writer.idx()+1);
  }


  @Override
  public void writeTimeSec(int value) {
    writer.writeTimeSec(value);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(TimeSecHolder holder) {
    writer.writeTimeSec(holder.value);
    writer.setPosition(writer.idx()+1);
  }


  @Override
  public void writeTimeMilli(int value) {
    writer.writeTimeMilli(value);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(TimeMilliHolder holder) {
    writer.writeTimeMilli(holder.value);
    writer.setPosition(writer.idx()+1);
  }


  @Override
  public void writeBigInt(long value) {
    writer.writeBigInt(value);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(BigIntHolder holder) {
    writer.writeBigInt(holder.value);
    writer.setPosition(writer.idx()+1);
  }


  @Override
  public void writeUInt8(long value) {
    writer.writeUInt8(value);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(UInt8Holder holder) {
    writer.writeUInt8(holder.value);
    writer.setPosition(writer.idx()+1);
  }


  @Override
  public void writeFloat8(double value) {
    writer.writeFloat8(value);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(Float8Holder holder) {
    writer.writeFloat8(holder.value);
    writer.setPosition(writer.idx()+1);
  }


  @Override
  public void writeDateMilli(long value) {
    writer.writeDateMilli(value);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(DateMilliHolder holder) {
    writer.writeDateMilli(holder.value);
    writer.setPosition(writer.idx()+1);
  }


  @Override
  public void writeDuration(long value) {
    writer.writeDuration(value);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(DurationHolder holder) {
    writer.write(holder);
    writer.setPosition(writer.idx()+1);
  }



  @Override
  public void writeTimeStampSec(long value) {
    writer.writeTimeStampSec(value);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(TimeStampSecHolder holder) {
    writer.writeTimeStampSec(holder.value);
    writer.setPosition(writer.idx()+1);
  }


  @Override
  public void writeTimeStampMilli(long value) {
    writer.writeTimeStampMilli(value);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(TimeStampMilliHolder holder) {
    writer.writeTimeStampMilli(holder.value);
    writer.setPosition(writer.idx()+1);
  }


  @Override
  public void writeTimeStampMicro(long value) {
    writer.writeTimeStampMicro(value);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(TimeStampMicroHolder holder) {
    writer.writeTimeStampMicro(holder.value);
    writer.setPosition(writer.idx()+1);
  }


  @Override
  public void writeTimeStampNano(long value) {
    writer.writeTimeStampNano(value);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(TimeStampNanoHolder holder) {
    writer.writeTimeStampNano(holder.value);
    writer.setPosition(writer.idx()+1);
  }


  @Override
  public void writeTimeStampSecTZ(long value) {
    writer.writeTimeStampSecTZ(value);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(TimeStampSecTZHolder holder) {
    writer.write(holder);
    writer.setPosition(writer.idx()+1);
  }



  @Override
  public void writeTimeStampMilliTZ(long value) {
    writer.writeTimeStampMilliTZ(value);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(TimeStampMilliTZHolder holder) {
    writer.write(holder);
    writer.setPosition(writer.idx()+1);
  }



  @Override
  public void writeTimeStampMicroTZ(long value) {
    writer.writeTimeStampMicroTZ(value);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(TimeStampMicroTZHolder holder) {
    writer.write(holder);
    writer.setPosition(writer.idx()+1);
  }



  @Override
  public void writeTimeStampNanoTZ(long value) {
    writer.writeTimeStampNanoTZ(value);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(TimeStampNanoTZHolder holder) {
    writer.write(holder);
    writer.setPosition(writer.idx()+1);
  }



  @Override
  public void writeTimeMicro(long value) {
    writer.writeTimeMicro(value);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(TimeMicroHolder holder) {
    writer.writeTimeMicro(holder.value);
    writer.setPosition(writer.idx()+1);
  }


  @Override
  public void writeTimeNano(long value) {
    writer.writeTimeNano(value);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(TimeNanoHolder holder) {
    writer.writeTimeNano(holder.value);
    writer.setPosition(writer.idx()+1);
  }


  @Override
  public void writeIntervalDay(int days, int milliseconds) {
    writer.writeIntervalDay(days, milliseconds);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(IntervalDayHolder holder) {
    writer.writeIntervalDay(holder.days, holder.milliseconds);
    writer.setPosition(writer.idx()+1);
  }


  @Override
  public void writeIntervalMonthDayNano(int months, int days, long nanoseconds) {
    writer.writeIntervalMonthDayNano(months, days, nanoseconds);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(IntervalMonthDayNanoHolder holder) {
    writer.writeIntervalMonthDayNano(holder.months, holder.days, holder.nanoseconds);
    writer.setPosition(writer.idx()+1);
  }


  @Override
  public void writeDecimal256(long start, ArrowBuf buffer) {
    writer.writeDecimal256(start, buffer);
    writer.setPosition(writer.idx()+1);
  }

  public void writeDecimal256(long start, ArrowBuf buffer, ArrowType arrowType) {
    writer.writeDecimal256(start, buffer, arrowType);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(Decimal256Holder holder) {
    writer.write(holder);
    writer.setPosition(writer.idx()+1);
  }

  public void writeDecimal256(BigDecimal value) {
    writer.writeDecimal256(value);
    writer.setPosition(writer.idx()+1);
  }

  public void writeBigEndianBytesToDecimal256(byte[] value, ArrowType arrowType){
    writer.writeBigEndianBytesToDecimal256(value, arrowType);
    writer.setPosition(writer.idx() + 1);
  }


  @Override
  public void writeDecimal(long start, ArrowBuf buffer) {
    writer.writeDecimal(start, buffer);
    writer.setPosition(writer.idx()+1);
  }

  public void writeDecimal(long start, ArrowBuf buffer, ArrowType arrowType) {
    writer.writeDecimal(start, buffer, arrowType);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(DecimalHolder holder) {
    writer.write(holder);
    writer.setPosition(writer.idx()+1);
  }

  public void writeDecimal(BigDecimal value) {
    writer.writeDecimal(value);
    writer.setPosition(writer.idx()+1);
  }

  public void writeBigEndianBytesToDecimal(byte[] value, ArrowType arrowType){
    writer.writeBigEndianBytesToDecimal(value, arrowType);
    writer.setPosition(writer.idx() + 1);
  }


  @Override
  public void writeFixedSizeBinary(ArrowBuf buffer) {
    writer.writeFixedSizeBinary(buffer);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(FixedSizeBinaryHolder holder) {
    writer.write(holder);
    writer.setPosition(writer.idx()+1);
  }



  @Override
  public void writeVarBinary(int start, int end, ArrowBuf buffer) {
    writer.writeVarBinary(start, end, buffer);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(VarBinaryHolder holder) {
    writer.writeVarBinary(holder.start, holder.end, holder.buffer);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void writeVarBinary(byte[] value) {
    writer.writeVarBinary(value);
    writer.setPosition(writer.idx() + 1);
  }

  @Override
  public void writeVarBinary(byte[] value, int offset, int length) {
    writer.writeVarBinary(value, offset, length);
    writer.setPosition(writer.idx() + 1);
  }

  @Override
  public void writeVarBinary(ByteBuffer value) {
    writer.writeVarBinary(value);
    writer.setPosition(writer.idx() + 1);
  }

  @Override
  public void writeVarBinary(ByteBuffer value, int offset, int length) {
    writer.writeVarBinary(value, offset, length);
    writer.setPosition(writer.idx() + 1);
  }

  @Override
  public void writeVarChar(int start, int end, ArrowBuf buffer) {
    writer.writeVarChar(start, end, buffer);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(VarCharHolder holder) {
    writer.writeVarChar(holder.start, holder.end, holder.buffer);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void writeVarChar(Text value) {
    writer.writeVarChar(value);
    writer.setPosition(writer.idx() + 1);
  }

  public void writeVarChar(String value) {
    writer.writeVarChar(value);
    writer.setPosition(writer.idx() + 1);
  }

  @Override
  public void writeLargeVarChar(long start, long end, ArrowBuf buffer) {
    writer.writeLargeVarChar(start, end, buffer);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(LargeVarCharHolder holder) {
    writer.writeLargeVarChar(holder.start, holder.end, holder.buffer);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void writeLargeVarChar(Text value) {
    writer.writeLargeVarChar(value);
    writer.setPosition(writer.idx() + 1);
  }

  public void writeLargeVarChar(String value) {
    writer.writeLargeVarChar(value);
    writer.setPosition(writer.idx() + 1);
  }

  @Override
  public void writeLargeVarBinary(long start, long end, ArrowBuf buffer) {
    writer.writeLargeVarBinary(start, end, buffer);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(LargeVarBinaryHolder holder) {
    writer.writeLargeVarBinary(holder.start, holder.end, holder.buffer);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void writeLargeVarBinary(byte[] value) {
    writer.writeLargeVarBinary(value);
    writer.setPosition(writer.idx() + 1);
  }

  @Override
  public void writeLargeVarBinary(byte[] value, int offset, int length) {
    writer.writeLargeVarBinary(value, offset, length);
    writer.setPosition(writer.idx() + 1);
  }

  @Override
  public void writeLargeVarBinary(ByteBuffer value) {
    writer.writeLargeVarBinary(value);
    writer.setPosition(writer.idx() + 1);
  }

  @Override
  public void writeLargeVarBinary(ByteBuffer value, int offset, int length) {
    writer.writeLargeVarBinary(value, offset, length);
    writer.setPosition(writer.idx() + 1);
  }

  @Override
  public void writeBit(int value) {
    writer.writeBit(value);
    writer.setPosition(writer.idx()+1);
  }

  @Override
  public void write(BitHolder holder) {
    writer.writeBit(holder.value);
    writer.setPosition(writer.idx()+1);
  }


}

