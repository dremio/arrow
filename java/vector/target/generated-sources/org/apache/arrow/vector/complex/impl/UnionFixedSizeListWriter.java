

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
 * This class is generated using freemarker and the UnionFixedSizeListWriter.java template.
 */

@SuppressWarnings("unused")
public class UnionFixedSizeListWriter extends AbstractFieldWriter {

  protected FixedSizeListVector vector;
  protected PromotableWriter writer;
  private boolean inStruct = false;
  private String structName;
  private final int listSize;

  public UnionFixedSizeListWriter(FixedSizeListVector vector) {
    this(vector, NullableStructWriterFactory.getNullableStructWriterFactoryInstance());
  }

  public UnionFixedSizeListWriter(FixedSizeListVector vector, NullableStructWriterFactory nullableStructWriterFactory) {
    this.vector = vector;
    this.writer = new PromotableWriter(vector.getDataVector(), vector, nullableStructWriterFactory);
    this.listSize = vector.getListSize();
  }

  public UnionFixedSizeListWriter(FixedSizeListVector vector, AbstractFieldWriter parent) {
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
  public DecimalWriter decimal() {
    return this;
  }

  @Override
  public DecimalWriter decimal(String name, int scale, int precision) {
    return writer.decimal(name, scale, precision);
  }

  @Override
  public DecimalWriter decimal(String name) {
    return writer.decimal(name);
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
    return writer.decimal256(name);
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
    int start = vector.startNewValue(idx());
    writer.setPosition(start);
  }

  @Override
  public void endList() {
    setPosition(idx() + 1);
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
  public void write(DecimalHolder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.write(holder);
    writer.setPosition(writer.idx() + 1);
  }
 
  @Override
  public void write(Decimal256Holder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.write(holder);
    writer.setPosition(writer.idx() + 1);
  }


  @Override
  public void writeNull() {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeNull();
  }

  public void writeDecimal(long start, ArrowBuf buffer, ArrowType arrowType) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeDecimal(start, buffer, arrowType);
    writer.setPosition(writer.idx() + 1);
  }

  public void writeDecimal(BigDecimal value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeDecimal(value);
    writer.setPosition(writer.idx() + 1);
  }

  public void writeBigEndianBytesToDecimal(byte[] value, ArrowType arrowType) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeBigEndianBytesToDecimal(value, arrowType);
    writer.setPosition(writer.idx() + 1);
  }

  public void writeDecimal256(long start, ArrowBuf buffer, ArrowType arrowType) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeDecimal256(start, buffer, arrowType);
    writer.setPosition(writer.idx() + 1);
  }

  public void writeDecimal256(BigDecimal value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeDecimal256(value);
    writer.setPosition(writer.idx() + 1);
  }

  public void writeBigEndianBytesToDecimal256(byte[] value, ArrowType arrowType) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeBigEndianBytesToDecimal256(value, arrowType);
    writer.setPosition(writer.idx() + 1);
  }



  @Override
  public void writeTinyInt(byte value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeTinyInt(value);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(TinyIntHolder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeTinyInt(holder.value);
    writer.setPosition(writer.idx() + 1);
  }


  @Override
  public void writeUInt1(byte value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeUInt1(value);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(UInt1Holder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeUInt1(holder.value);
    writer.setPosition(writer.idx() + 1);
  }


  @Override
  public void writeUInt2(char value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeUInt2(value);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(UInt2Holder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeUInt2(holder.value);
    writer.setPosition(writer.idx() + 1);
  }


  @Override
  public void writeSmallInt(short value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeSmallInt(value);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(SmallIntHolder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeSmallInt(holder.value);
    writer.setPosition(writer.idx() + 1);
  }


  @Override
  public void writeFloat2(short value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeFloat2(value);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(Float2Holder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeFloat2(holder.value);
    writer.setPosition(writer.idx() + 1);
  }


  @Override
  public void writeInt(int value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeInt(value);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(IntHolder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeInt(holder.value);
    writer.setPosition(writer.idx() + 1);
  }


  @Override
  public void writeUInt4(int value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeUInt4(value);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(UInt4Holder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeUInt4(holder.value);
    writer.setPosition(writer.idx() + 1);
  }


  @Override
  public void writeFloat4(float value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeFloat4(value);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(Float4Holder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeFloat4(holder.value);
    writer.setPosition(writer.idx() + 1);
  }


  @Override
  public void writeDateDay(int value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeDateDay(value);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(DateDayHolder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeDateDay(holder.value);
    writer.setPosition(writer.idx() + 1);
  }


  @Override
  public void writeIntervalYear(int value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeIntervalYear(value);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(IntervalYearHolder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeIntervalYear(holder.value);
    writer.setPosition(writer.idx() + 1);
  }


  @Override
  public void writeTimeSec(int value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeTimeSec(value);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(TimeSecHolder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeTimeSec(holder.value);
    writer.setPosition(writer.idx() + 1);
  }


  @Override
  public void writeTimeMilli(int value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeTimeMilli(value);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(TimeMilliHolder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeTimeMilli(holder.value);
    writer.setPosition(writer.idx() + 1);
  }


  @Override
  public void writeBigInt(long value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeBigInt(value);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(BigIntHolder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeBigInt(holder.value);
    writer.setPosition(writer.idx() + 1);
  }


  @Override
  public void writeUInt8(long value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeUInt8(value);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(UInt8Holder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeUInt8(holder.value);
    writer.setPosition(writer.idx() + 1);
  }


  @Override
  public void writeFloat8(double value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeFloat8(value);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(Float8Holder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeFloat8(holder.value);
    writer.setPosition(writer.idx() + 1);
  }


  @Override
  public void writeDateMilli(long value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeDateMilli(value);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(DateMilliHolder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeDateMilli(holder.value);
    writer.setPosition(writer.idx() + 1);
  }



  @Override
  public void writeTimeStampSec(long value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeTimeStampSec(value);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(TimeStampSecHolder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeTimeStampSec(holder.value);
    writer.setPosition(writer.idx() + 1);
  }


  @Override
  public void writeTimeStampMilli(long value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeTimeStampMilli(value);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(TimeStampMilliHolder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeTimeStampMilli(holder.value);
    writer.setPosition(writer.idx() + 1);
  }


  @Override
  public void writeTimeStampMicro(long value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeTimeStampMicro(value);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(TimeStampMicroHolder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeTimeStampMicro(holder.value);
    writer.setPosition(writer.idx() + 1);
  }


  @Override
  public void writeTimeStampNano(long value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeTimeStampNano(value);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(TimeStampNanoHolder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeTimeStampNano(holder.value);
    writer.setPosition(writer.idx() + 1);
  }






  @Override
  public void writeTimeMicro(long value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeTimeMicro(value);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(TimeMicroHolder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeTimeMicro(holder.value);
    writer.setPosition(writer.idx() + 1);
  }


  @Override
  public void writeTimeNano(long value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeTimeNano(value);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(TimeNanoHolder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeTimeNano(holder.value);
    writer.setPosition(writer.idx() + 1);
  }


  @Override
  public void writeIntervalDay(int days, int milliseconds) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeIntervalDay(days, milliseconds);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(IntervalDayHolder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeIntervalDay(holder.days, holder.milliseconds);
    writer.setPosition(writer.idx() + 1);
  }


  @Override
  public void writeIntervalMonthDayNano(int months, int days, long nanoseconds) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeIntervalMonthDayNano(months, days, nanoseconds);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(IntervalMonthDayNanoHolder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeIntervalMonthDayNano(holder.months, holder.days, holder.nanoseconds);
    writer.setPosition(writer.idx() + 1);
  }




  @Override
  public void writeVarBinary(byte[] value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeVarBinary(value);
    writer.setPosition(writer.idx() + 1);
  }

  @Override
  public void writeVarBinary(byte[] value, int offset, int length) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeVarBinary(value, offset, length);
    writer.setPosition(writer.idx() + 1);
  }

  @Override
  public void writeVarBinary(ByteBuffer value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeVarBinary(value);
    writer.setPosition(writer.idx() + 1);
  }

  @Override
  public void writeVarBinary(ByteBuffer value, int offset, int length) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeVarBinary(value, offset, length);
    writer.setPosition(writer.idx() + 1);
  }

  @Override
  public void writeVarBinary(int start, int end, ArrowBuf buffer) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeVarBinary(start, end, buffer);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(VarBinaryHolder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeVarBinary(holder.start, holder.end, holder.buffer);
    writer.setPosition(writer.idx() + 1);
  }

  @Override
  public void writeVarChar(Text value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeVarChar(value);
    writer.setPosition(writer.idx() + 1);
  }

  @Override
  public void writeVarChar(String value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeVarChar(value);
    writer.setPosition(writer.idx() + 1);
  }

  @Override
  public void writeVarChar(int start, int end, ArrowBuf buffer) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeVarChar(start, end, buffer);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(VarCharHolder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeVarChar(holder.start, holder.end, holder.buffer);
    writer.setPosition(writer.idx() + 1);
  }

  @Override
  public void writeLargeVarChar(Text value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeLargeVarChar(value);
    writer.setPosition(writer.idx() + 1);
  }

  @Override
  public void writeLargeVarChar(String value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeLargeVarChar(value);
    writer.setPosition(writer.idx() + 1);
  }

  @Override
  public void writeLargeVarChar(long start, long end, ArrowBuf buffer) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeLargeVarChar(start, end, buffer);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(LargeVarCharHolder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeLargeVarChar(holder.start, holder.end, holder.buffer);
    writer.setPosition(writer.idx() + 1);
  }

  @Override
  public void writeLargeVarBinary(byte[] value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeLargeVarBinary(value);
    writer.setPosition(writer.idx() + 1);
  }

  @Override
  public void writeLargeVarBinary(byte[] value, int offset, int length) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeLargeVarBinary(value, offset, length);
    writer.setPosition(writer.idx() + 1);
  }

  @Override
  public void writeLargeVarBinary(ByteBuffer value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeLargeVarBinary(value);
    writer.setPosition(writer.idx() + 1);
  }

  @Override
  public void writeLargeVarBinary(ByteBuffer value, int offset, int length) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeLargeVarBinary(value, offset, length);
    writer.setPosition(writer.idx() + 1);
  }

  @Override
  public void writeLargeVarBinary(long start, long end, ArrowBuf buffer) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeLargeVarBinary(start, end, buffer);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(LargeVarBinaryHolder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeLargeVarBinary(holder.start, holder.end, holder.buffer);
    writer.setPosition(writer.idx() + 1);
  }


  @Override
  public void writeBit(int value) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeBit(value);
    writer.setPosition(writer.idx() + 1);
  }

  public void write(BitHolder holder) {
    if (writer.idx() >= (idx() + 1) * listSize) {
      throw new IllegalStateException(String.format("values at index %s is greater than listSize %s", idx(), listSize));
    }
    writer.writeBit(holder.value);
    writer.setPosition(writer.idx() + 1);
  }

}
