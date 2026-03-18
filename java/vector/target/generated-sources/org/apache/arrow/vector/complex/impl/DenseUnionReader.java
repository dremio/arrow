

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
 * Source code generated using FreeMarker template DenseUnionReader.java
 */
@SuppressWarnings("unused")
public class DenseUnionReader extends AbstractFieldReader {

  private BaseReader[] readers = new BaseReader[Byte.MAX_VALUE + 1];
  public DenseUnionVector data;

  public DenseUnionReader(DenseUnionVector data) {
    this.data = data;
  }

  public MinorType getMinorType() {
    byte typeId = data.getTypeId(idx());
    return data.getVectorByType(typeId).getMinorType();
  }

  public byte getTypeId() {
    return data.getTypeId(idx());
  }

  @Override
  public Field getField() {
    return data.getField();
  }

  public boolean isSet(){
    return !data.isNull(idx());
  }

  public void read(DenseUnionHolder holder) {
    holder.reader = this;
    holder.isSet = this.isSet() ? 1 : 0;
    holder.typeId = getTypeId();
  }

  public void read(int index, UnionHolder holder) {
    byte typeId = data.getTypeId(index);
    getList(typeId).read(index, holder);
  }

  private FieldReader getReaderForIndex(int index) {
    byte typeId = data.getTypeId(index);
    MinorType minorType = data.getVectorByType(typeId).getMinorType();
    FieldReader reader = (FieldReader) readers[typeId];
    if (reader != null) {
      return reader;
    }
    switch (minorType) {
      case NULL:
        reader = NullReader.INSTANCE;
        break;
      case STRUCT:
        reader = (FieldReader) getStruct(typeId);
        break;
      case LIST:
        reader = (FieldReader) getList(typeId);
        break;
      case TINYINT:
      reader = (FieldReader) getTinyInt(typeId);
      break;
      case UINT1:
      reader = (FieldReader) getUInt1(typeId);
      break;
      case UINT2:
      reader = (FieldReader) getUInt2(typeId);
      break;
      case SMALLINT:
      reader = (FieldReader) getSmallInt(typeId);
      break;
      case FLOAT2:
      reader = (FieldReader) getFloat2(typeId);
      break;
      case INT:
      reader = (FieldReader) getInt(typeId);
      break;
      case UINT4:
      reader = (FieldReader) getUInt4(typeId);
      break;
      case FLOAT4:
      reader = (FieldReader) getFloat4(typeId);
      break;
      case DATEDAY:
      reader = (FieldReader) getDateDay(typeId);
      break;
      case INTERVALYEAR:
      reader = (FieldReader) getIntervalYear(typeId);
      break;
      case TIMESEC:
      reader = (FieldReader) getTimeSec(typeId);
      break;
      case TIMEMILLI:
      reader = (FieldReader) getTimeMilli(typeId);
      break;
      case BIGINT:
      reader = (FieldReader) getBigInt(typeId);
      break;
      case UINT8:
      reader = (FieldReader) getUInt8(typeId);
      break;
      case FLOAT8:
      reader = (FieldReader) getFloat8(typeId);
      break;
      case DATEMILLI:
      reader = (FieldReader) getDateMilli(typeId);
      break;
      case TIMESTAMPSEC:
      reader = (FieldReader) getTimeStampSec(typeId);
      break;
      case TIMESTAMPMILLI:
      reader = (FieldReader) getTimeStampMilli(typeId);
      break;
      case TIMESTAMPMICRO:
      reader = (FieldReader) getTimeStampMicro(typeId);
      break;
      case TIMESTAMPNANO:
      reader = (FieldReader) getTimeStampNano(typeId);
      break;
      case TIMEMICRO:
      reader = (FieldReader) getTimeMicro(typeId);
      break;
      case TIMENANO:
      reader = (FieldReader) getTimeNano(typeId);
      break;
      case INTERVALDAY:
      reader = (FieldReader) getIntervalDay(typeId);
      break;
      case INTERVALMONTHDAYNANO:
      reader = (FieldReader) getIntervalMonthDayNano(typeId);
      break;
      case DECIMAL256:
      reader = (FieldReader) getDecimal256(typeId);
      break;
      case DECIMAL:
      reader = (FieldReader) getDecimal(typeId);
      break;
      case VARBINARY:
      reader = (FieldReader) getVarBinary(typeId);
      break;
      case VARCHAR:
      reader = (FieldReader) getVarChar(typeId);
      break;
      case LARGEVARCHAR:
      reader = (FieldReader) getLargeVarChar(typeId);
      break;
      case LARGEVARBINARY:
      reader = (FieldReader) getLargeVarBinary(typeId);
      break;
      case BIT:
      reader = (FieldReader) getBit(typeId);
      break;
      default:
        throw new UnsupportedOperationException("Unsupported type: " + MinorType.values()[typeId]);
    }
    return reader;
  }

  private SingleStructReaderImpl structReader;

  private StructReader getStruct(byte typeId) {
    StructReader structReader = (StructReader) readers[typeId];
    if (structReader == null) {
      structReader = (SingleStructReaderImpl) data.getVectorByType(typeId).getReader();
      structReader.setPosition(idx());
      readers[typeId] = structReader;
    }
    return structReader;
  }

  private UnionListReader listReader;

  private FieldReader getList(byte typeId) {
    UnionListReader listReader = (UnionListReader) readers[typeId];
    if (listReader == null) {
      listReader = new UnionListReader((ListVector) data.getVectorByType(typeId));
      listReader.setPosition(idx());
      readers[typeId] = listReader;
    }
    return listReader;
  }

  private UnionMapReader mapReader;

  private FieldReader getMap(byte typeId) {
    UnionMapReader mapReader = (UnionMapReader) readers[typeId];
    if (mapReader == null) {
      mapReader = new UnionMapReader((MapVector) data.getVectorByType(typeId));
      mapReader.setPosition(idx());
      readers[typeId] = mapReader;
    }
    return mapReader;
  }

  @Override
  public java.util.Iterator<String> iterator() {
    throw new UnsupportedOperationException();
  }

  @Override
  public void copyAsValue(UnionWriter writer) {
    writer.data.copyFrom(idx(), writer.idx(), data);
  }


  @Override
  public Object readObject() {
    return getReaderForIndex(idx()).readObject();
  }


  @Override
  public BigDecimal readBigDecimal() {
    return getReaderForIndex(idx()).readBigDecimal();
  }


  @Override
  public Short readShort() {
    return getReaderForIndex(idx()).readShort();
  }


  @Override
  public Integer readInteger() {
    return getReaderForIndex(idx()).readInteger();
  }


  @Override
  public Long readLong() {
    return getReaderForIndex(idx()).readLong();
  }


  @Override
  public Boolean readBoolean() {
    return getReaderForIndex(idx()).readBoolean();
  }


  @Override
  public LocalDateTime readLocalDateTime() {
    return getReaderForIndex(idx()).readLocalDateTime();
  }


  @Override
  public Duration readDuration() {
    return getReaderForIndex(idx()).readDuration();
  }


  @Override
  public Period readPeriod() {
    return getReaderForIndex(idx()).readPeriod();
  }


  @Override
  public Double readDouble() {
    return getReaderForIndex(idx()).readDouble();
  }


  @Override
  public Float readFloat() {
    return getReaderForIndex(idx()).readFloat();
  }


  @Override
  public Character readCharacter() {
    return getReaderForIndex(idx()).readCharacter();
  }


  @Override
  public Text readText() {
    return getReaderForIndex(idx()).readText();
  }


  @Override
  public Byte readByte() {
    return getReaderForIndex(idx()).readByte();
  }


  @Override
  public byte[] readByteArray() {
    return getReaderForIndex(idx()).readByteArray();
  }


  @Override
  public PeriodDuration readPeriodDuration() {
    return getReaderForIndex(idx()).readPeriodDuration();
  }


  public int size() {
    return getReaderForIndex(idx()).size();
  }


  private TinyIntReaderImpl getTinyInt(byte typeId) {
    TinyIntReaderImpl reader = (TinyIntReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new TinyIntReaderImpl((TinyIntVector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableTinyIntHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(TinyIntWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private UInt1ReaderImpl getUInt1(byte typeId) {
    UInt1ReaderImpl reader = (UInt1ReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new UInt1ReaderImpl((UInt1Vector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableUInt1Holder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(UInt1Writer writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private UInt2ReaderImpl getUInt2(byte typeId) {
    UInt2ReaderImpl reader = (UInt2ReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new UInt2ReaderImpl((UInt2Vector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableUInt2Holder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(UInt2Writer writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private SmallIntReaderImpl getSmallInt(byte typeId) {
    SmallIntReaderImpl reader = (SmallIntReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new SmallIntReaderImpl((SmallIntVector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableSmallIntHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(SmallIntWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private Float2ReaderImpl getFloat2(byte typeId) {
    Float2ReaderImpl reader = (Float2ReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new Float2ReaderImpl((Float2Vector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableFloat2Holder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(Float2Writer writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private IntReaderImpl getInt(byte typeId) {
    IntReaderImpl reader = (IntReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new IntReaderImpl((IntVector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableIntHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(IntWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private UInt4ReaderImpl getUInt4(byte typeId) {
    UInt4ReaderImpl reader = (UInt4ReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new UInt4ReaderImpl((UInt4Vector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableUInt4Holder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(UInt4Writer writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private Float4ReaderImpl getFloat4(byte typeId) {
    Float4ReaderImpl reader = (Float4ReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new Float4ReaderImpl((Float4Vector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableFloat4Holder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(Float4Writer writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private DateDayReaderImpl getDateDay(byte typeId) {
    DateDayReaderImpl reader = (DateDayReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new DateDayReaderImpl((DateDayVector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableDateDayHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(DateDayWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private IntervalYearReaderImpl getIntervalYear(byte typeId) {
    IntervalYearReaderImpl reader = (IntervalYearReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new IntervalYearReaderImpl((IntervalYearVector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableIntervalYearHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(IntervalYearWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private TimeSecReaderImpl getTimeSec(byte typeId) {
    TimeSecReaderImpl reader = (TimeSecReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new TimeSecReaderImpl((TimeSecVector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableTimeSecHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(TimeSecWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private TimeMilliReaderImpl getTimeMilli(byte typeId) {
    TimeMilliReaderImpl reader = (TimeMilliReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new TimeMilliReaderImpl((TimeMilliVector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableTimeMilliHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(TimeMilliWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private BigIntReaderImpl getBigInt(byte typeId) {
    BigIntReaderImpl reader = (BigIntReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new BigIntReaderImpl((BigIntVector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableBigIntHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(BigIntWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private UInt8ReaderImpl getUInt8(byte typeId) {
    UInt8ReaderImpl reader = (UInt8ReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new UInt8ReaderImpl((UInt8Vector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableUInt8Holder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(UInt8Writer writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private Float8ReaderImpl getFloat8(byte typeId) {
    Float8ReaderImpl reader = (Float8ReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new Float8ReaderImpl((Float8Vector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableFloat8Holder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(Float8Writer writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private DateMilliReaderImpl getDateMilli(byte typeId) {
    DateMilliReaderImpl reader = (DateMilliReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new DateMilliReaderImpl((DateMilliVector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableDateMilliHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(DateMilliWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private TimeStampSecReaderImpl getTimeStampSec(byte typeId) {
    TimeStampSecReaderImpl reader = (TimeStampSecReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new TimeStampSecReaderImpl((TimeStampSecVector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableTimeStampSecHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(TimeStampSecWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private TimeStampMilliReaderImpl getTimeStampMilli(byte typeId) {
    TimeStampMilliReaderImpl reader = (TimeStampMilliReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new TimeStampMilliReaderImpl((TimeStampMilliVector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableTimeStampMilliHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(TimeStampMilliWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private TimeStampMicroReaderImpl getTimeStampMicro(byte typeId) {
    TimeStampMicroReaderImpl reader = (TimeStampMicroReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new TimeStampMicroReaderImpl((TimeStampMicroVector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableTimeStampMicroHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(TimeStampMicroWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private TimeStampNanoReaderImpl getTimeStampNano(byte typeId) {
    TimeStampNanoReaderImpl reader = (TimeStampNanoReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new TimeStampNanoReaderImpl((TimeStampNanoVector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableTimeStampNanoHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(TimeStampNanoWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private TimeMicroReaderImpl getTimeMicro(byte typeId) {
    TimeMicroReaderImpl reader = (TimeMicroReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new TimeMicroReaderImpl((TimeMicroVector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableTimeMicroHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(TimeMicroWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private TimeNanoReaderImpl getTimeNano(byte typeId) {
    TimeNanoReaderImpl reader = (TimeNanoReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new TimeNanoReaderImpl((TimeNanoVector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableTimeNanoHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(TimeNanoWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private IntervalDayReaderImpl getIntervalDay(byte typeId) {
    IntervalDayReaderImpl reader = (IntervalDayReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new IntervalDayReaderImpl((IntervalDayVector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableIntervalDayHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(IntervalDayWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private IntervalMonthDayNanoReaderImpl getIntervalMonthDayNano(byte typeId) {
    IntervalMonthDayNanoReaderImpl reader = (IntervalMonthDayNanoReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new IntervalMonthDayNanoReaderImpl((IntervalMonthDayNanoVector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableIntervalMonthDayNanoHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(IntervalMonthDayNanoWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private Decimal256ReaderImpl getDecimal256(byte typeId) {
    Decimal256ReaderImpl reader = (Decimal256ReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new Decimal256ReaderImpl((Decimal256Vector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableDecimal256Holder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(Decimal256Writer writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private DecimalReaderImpl getDecimal(byte typeId) {
    DecimalReaderImpl reader = (DecimalReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new DecimalReaderImpl((DecimalVector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableDecimalHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(DecimalWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private VarBinaryReaderImpl getVarBinary(byte typeId) {
    VarBinaryReaderImpl reader = (VarBinaryReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new VarBinaryReaderImpl((VarBinaryVector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableVarBinaryHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(VarBinaryWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private VarCharReaderImpl getVarChar(byte typeId) {
    VarCharReaderImpl reader = (VarCharReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new VarCharReaderImpl((VarCharVector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableVarCharHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(VarCharWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private LargeVarCharReaderImpl getLargeVarChar(byte typeId) {
    LargeVarCharReaderImpl reader = (LargeVarCharReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new LargeVarCharReaderImpl((LargeVarCharVector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableLargeVarCharHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(LargeVarCharWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private LargeVarBinaryReaderImpl getLargeVarBinary(byte typeId) {
    LargeVarBinaryReaderImpl reader = (LargeVarBinaryReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new LargeVarBinaryReaderImpl((LargeVarBinaryVector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableLargeVarBinaryHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(LargeVarBinaryWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private BitReaderImpl getBit(byte typeId) {
    BitReaderImpl reader = (BitReaderImpl) readers[typeId];
    if (reader == null) {
      reader = new BitReaderImpl((BitVector) data.getVectorByType(typeId));
      reader.setPosition(idx());
      readers[typeId] = reader;
    }
    return reader;
  }

  public void read(NullableBitHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(BitWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  @Override
  public void copyAsValue(ListWriter writer) {
    ComplexCopier.copy(this, (FieldWriter) writer);
  }

  @Override
  public void setPosition(int index) {
    super.setPosition(index);
    byte typeId = data.getTypeId(index);
    if (readers[typeId] != null) {
      int offset = data.getOffset(index);
      readers[typeId].setPosition(offset);
    }
  }

  public FieldReader reader(byte typeId, String name){
    return getStruct(typeId).reader(name);
  }

  public FieldReader reader(byte typeId) {
    return getList(typeId).reader();
  }

  public boolean next() {
    return getReaderForIndex(idx()).next();
  }
}
