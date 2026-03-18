

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



/**
 * Source code generated using FreeMarker template NullReader.java
 */
@SuppressWarnings("unused")
public class NullReader extends AbstractBaseReader implements FieldReader{
  
  public static final NullReader INSTANCE = new NullReader();
  public static final NullReader EMPTY_LIST_INSTANCE = new NullReader(MinorType.NULL);
  public static final NullReader EMPTY_STRUCT_INSTANCE = new NullReader(MinorType.STRUCT);
  private MinorType type;
  
  private NullReader(){
    super();
    type = MinorType.NULL;
  }

  private NullReader(MinorType type){
    super();
    this.type = type;
  }

  @Override
  public MinorType getMinorType() {
    return type;
  }

  @Override
  public Field getField() {
    return new Field("", FieldType.nullable(new Null()), null);
  }

  public void copyAsValue(StructWriter writer) {}

  public void copyAsValue(ListWriter writer) {}

  public void copyAsValue(UnionWriter writer) {}

  public void read(TinyIntHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableTinyIntHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, TinyIntHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(TinyIntWriter writer){}
  public void copyAsField(String name, TinyIntWriter writer){}

  public void read(int arrayIndex, NullableTinyIntHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(UInt1Holder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableUInt1Holder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, UInt1Holder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(UInt1Writer writer){}
  public void copyAsField(String name, UInt1Writer writer){}

  public void read(int arrayIndex, NullableUInt1Holder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(UInt2Holder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableUInt2Holder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, UInt2Holder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(UInt2Writer writer){}
  public void copyAsField(String name, UInt2Writer writer){}

  public void read(int arrayIndex, NullableUInt2Holder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(SmallIntHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableSmallIntHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, SmallIntHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(SmallIntWriter writer){}
  public void copyAsField(String name, SmallIntWriter writer){}

  public void read(int arrayIndex, NullableSmallIntHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(Float2Holder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableFloat2Holder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, Float2Holder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(Float2Writer writer){}
  public void copyAsField(String name, Float2Writer writer){}

  public void read(int arrayIndex, NullableFloat2Holder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(IntHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableIntHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, IntHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(IntWriter writer){}
  public void copyAsField(String name, IntWriter writer){}

  public void read(int arrayIndex, NullableIntHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(UInt4Holder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableUInt4Holder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, UInt4Holder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(UInt4Writer writer){}
  public void copyAsField(String name, UInt4Writer writer){}

  public void read(int arrayIndex, NullableUInt4Holder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(Float4Holder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableFloat4Holder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, Float4Holder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(Float4Writer writer){}
  public void copyAsField(String name, Float4Writer writer){}

  public void read(int arrayIndex, NullableFloat4Holder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(DateDayHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableDateDayHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, DateDayHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(DateDayWriter writer){}
  public void copyAsField(String name, DateDayWriter writer){}

  public void read(int arrayIndex, NullableDateDayHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(IntervalYearHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableIntervalYearHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, IntervalYearHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(IntervalYearWriter writer){}
  public void copyAsField(String name, IntervalYearWriter writer){}

  public void read(int arrayIndex, NullableIntervalYearHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(TimeSecHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableTimeSecHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, TimeSecHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(TimeSecWriter writer){}
  public void copyAsField(String name, TimeSecWriter writer){}

  public void read(int arrayIndex, NullableTimeSecHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(TimeMilliHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableTimeMilliHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, TimeMilliHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(TimeMilliWriter writer){}
  public void copyAsField(String name, TimeMilliWriter writer){}

  public void read(int arrayIndex, NullableTimeMilliHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(BigIntHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableBigIntHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, BigIntHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(BigIntWriter writer){}
  public void copyAsField(String name, BigIntWriter writer){}

  public void read(int arrayIndex, NullableBigIntHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(UInt8Holder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableUInt8Holder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, UInt8Holder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(UInt8Writer writer){}
  public void copyAsField(String name, UInt8Writer writer){}

  public void read(int arrayIndex, NullableUInt8Holder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(Float8Holder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableFloat8Holder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, Float8Holder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(Float8Writer writer){}
  public void copyAsField(String name, Float8Writer writer){}

  public void read(int arrayIndex, NullableFloat8Holder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(DateMilliHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableDateMilliHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, DateMilliHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(DateMilliWriter writer){}
  public void copyAsField(String name, DateMilliWriter writer){}

  public void read(int arrayIndex, NullableDateMilliHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(DurationHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableDurationHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, DurationHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(DurationWriter writer){}
  public void copyAsField(String name, DurationWriter writer){}

  public void read(int arrayIndex, NullableDurationHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(TimeStampSecHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableTimeStampSecHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, TimeStampSecHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(TimeStampSecWriter writer){}
  public void copyAsField(String name, TimeStampSecWriter writer){}

  public void read(int arrayIndex, NullableTimeStampSecHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(TimeStampMilliHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableTimeStampMilliHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, TimeStampMilliHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(TimeStampMilliWriter writer){}
  public void copyAsField(String name, TimeStampMilliWriter writer){}

  public void read(int arrayIndex, NullableTimeStampMilliHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(TimeStampMicroHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableTimeStampMicroHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, TimeStampMicroHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(TimeStampMicroWriter writer){}
  public void copyAsField(String name, TimeStampMicroWriter writer){}

  public void read(int arrayIndex, NullableTimeStampMicroHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(TimeStampNanoHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableTimeStampNanoHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, TimeStampNanoHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(TimeStampNanoWriter writer){}
  public void copyAsField(String name, TimeStampNanoWriter writer){}

  public void read(int arrayIndex, NullableTimeStampNanoHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(TimeStampSecTZHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableTimeStampSecTZHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, TimeStampSecTZHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(TimeStampSecTZWriter writer){}
  public void copyAsField(String name, TimeStampSecTZWriter writer){}

  public void read(int arrayIndex, NullableTimeStampSecTZHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(TimeStampMilliTZHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableTimeStampMilliTZHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, TimeStampMilliTZHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(TimeStampMilliTZWriter writer){}
  public void copyAsField(String name, TimeStampMilliTZWriter writer){}

  public void read(int arrayIndex, NullableTimeStampMilliTZHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(TimeStampMicroTZHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableTimeStampMicroTZHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, TimeStampMicroTZHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(TimeStampMicroTZWriter writer){}
  public void copyAsField(String name, TimeStampMicroTZWriter writer){}

  public void read(int arrayIndex, NullableTimeStampMicroTZHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(TimeStampNanoTZHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableTimeStampNanoTZHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, TimeStampNanoTZHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(TimeStampNanoTZWriter writer){}
  public void copyAsField(String name, TimeStampNanoTZWriter writer){}

  public void read(int arrayIndex, NullableTimeStampNanoTZHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(TimeMicroHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableTimeMicroHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, TimeMicroHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(TimeMicroWriter writer){}
  public void copyAsField(String name, TimeMicroWriter writer){}

  public void read(int arrayIndex, NullableTimeMicroHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(TimeNanoHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableTimeNanoHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, TimeNanoHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(TimeNanoWriter writer){}
  public void copyAsField(String name, TimeNanoWriter writer){}

  public void read(int arrayIndex, NullableTimeNanoHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(IntervalDayHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableIntervalDayHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, IntervalDayHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(IntervalDayWriter writer){}
  public void copyAsField(String name, IntervalDayWriter writer){}

  public void read(int arrayIndex, NullableIntervalDayHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(IntervalMonthDayNanoHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableIntervalMonthDayNanoHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, IntervalMonthDayNanoHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(IntervalMonthDayNanoWriter writer){}
  public void copyAsField(String name, IntervalMonthDayNanoWriter writer){}

  public void read(int arrayIndex, NullableIntervalMonthDayNanoHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(Decimal256Holder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableDecimal256Holder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, Decimal256Holder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(Decimal256Writer writer){}
  public void copyAsField(String name, Decimal256Writer writer){}

  public void read(int arrayIndex, NullableDecimal256Holder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(DecimalHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableDecimalHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, DecimalHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(DecimalWriter writer){}
  public void copyAsField(String name, DecimalWriter writer){}

  public void read(int arrayIndex, NullableDecimalHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(FixedSizeBinaryHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableFixedSizeBinaryHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, FixedSizeBinaryHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(FixedSizeBinaryWriter writer){}
  public void copyAsField(String name, FixedSizeBinaryWriter writer){}

  public void read(int arrayIndex, NullableFixedSizeBinaryHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(VarBinaryHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableVarBinaryHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, VarBinaryHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(VarBinaryWriter writer){}
  public void copyAsField(String name, VarBinaryWriter writer){}

  public void read(int arrayIndex, NullableVarBinaryHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(VarCharHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableVarCharHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, VarCharHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(VarCharWriter writer){}
  public void copyAsField(String name, VarCharWriter writer){}

  public void read(int arrayIndex, NullableVarCharHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(LargeVarCharHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableLargeVarCharHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, LargeVarCharHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(LargeVarCharWriter writer){}
  public void copyAsField(String name, LargeVarCharWriter writer){}

  public void read(int arrayIndex, NullableLargeVarCharHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(LargeVarBinaryHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableLargeVarBinaryHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, LargeVarBinaryHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(LargeVarBinaryWriter writer){}
  public void copyAsField(String name, LargeVarBinaryWriter writer){}

  public void read(int arrayIndex, NullableLargeVarBinaryHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  public void read(BitHolder holder){
    throw new UnsupportedOperationException("NullReader cannot write into non-nullable holder");
  }

  public void read(NullableBitHolder holder){
    holder.isSet = 0;
  }

  public void read(int arrayIndex, BitHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public void copyAsValue(BitWriter writer){}
  public void copyAsField(String name, BitWriter writer){}

  public void read(int arrayIndex, NullableBitHolder holder){
    throw new ArrayIndexOutOfBoundsException();
  }
  
  public int size(){
    return 0;
  }
  
  public boolean isSet(){
    return false;
  }
  
  public boolean next(){
    return false;
  }
  
  public RepeatedStructReader struct(){
    return this;
  }
  
  public RepeatedListReader list(){
    return this;
  }
  
  public StructReader struct(String name){
    return this;
  }
  
  public ListReader list(String name){
    return this;
  }
  
  public FieldReader reader(String name){
    return this;
  }
  
  public FieldReader reader(){
    return this;
  }
  
  private void fail(String name){
    throw new IllegalArgumentException(String.format("You tried to read a %s type when you are using a ValueReader of type %s.", name, this.getClass().getSimpleName()));
  }
  
  
  public Object readObject(int arrayIndex){
    return null;
  }
  
  public Object readObject(){
    return null;
  }
  
  public BigDecimal readBigDecimal(int arrayIndex){
    return null;
  }
  
  public BigDecimal readBigDecimal(){
    return null;
  }
  
  public Short readShort(int arrayIndex){
    return null;
  }
  
  public Short readShort(){
    return null;
  }
  
  public Integer readInteger(int arrayIndex){
    return null;
  }
  
  public Integer readInteger(){
    return null;
  }
  
  public Long readLong(int arrayIndex){
    return null;
  }
  
  public Long readLong(){
    return null;
  }
  
  public Boolean readBoolean(int arrayIndex){
    return null;
  }
  
  public Boolean readBoolean(){
    return null;
  }
  
  public LocalDateTime readLocalDateTime(int arrayIndex){
    return null;
  }
  
  public LocalDateTime readLocalDateTime(){
    return null;
  }
  
  public Duration readDuration(int arrayIndex){
    return null;
  }
  
  public Duration readDuration(){
    return null;
  }
  
  public Period readPeriod(int arrayIndex){
    return null;
  }
  
  public Period readPeriod(){
    return null;
  }
  
  public Double readDouble(int arrayIndex){
    return null;
  }
  
  public Double readDouble(){
    return null;
  }
  
  public Float readFloat(int arrayIndex){
    return null;
  }
  
  public Float readFloat(){
    return null;
  }
  
  public Character readCharacter(int arrayIndex){
    return null;
  }
  
  public Character readCharacter(){
    return null;
  }
  
  public Text readText(int arrayIndex){
    return null;
  }
  
  public Text readText(){
    return null;
  }
  
  public String readString(int arrayIndex){
    return null;
  }
  
  public String readString(){
    return null;
  }
  
  public Byte readByte(int arrayIndex){
    return null;
  }
  
  public Byte readByte(){
    return null;
  }
  
  public byte[] readByteArray(int arrayIndex){
    return null;
  }
  
  public byte[] readByteArray(){
    return null;
  }
  
  public PeriodDuration readPeriodDuration(int arrayIndex){
    return null;
  }
  
  public PeriodDuration readPeriodDuration(){
    return null;
  }
  
}



