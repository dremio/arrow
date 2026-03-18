

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
 * Source code generated using FreeMarker template UnionReader.java
 */
@SuppressWarnings("unused")
public class UnionReader extends AbstractFieldReader {

  private static final int NUM_SUPPORTED_TYPES = 46;

  private BaseReader[] readers = new BaseReader[NUM_SUPPORTED_TYPES];
  public UnionVector data;
  
  public UnionReader(UnionVector data) {
    this.data = data;
  }

  public MinorType getMinorType() {
    return TYPES[data.getTypeValue(idx())];
  }

  private static MinorType[] TYPES = new MinorType[NUM_SUPPORTED_TYPES];

  static {
    for (MinorType minorType : MinorType.values()) {
      TYPES[minorType.ordinal()] = minorType;
    }
  }

  @Override
  public Field getField() {
    return data.getField();
  }

  public boolean isSet(){
    return !data.isNull(idx());
  }

  public void read(UnionHolder holder) {
    holder.reader = this;
    holder.isSet = this.isSet() ? 1 : 0;
  }

  public void read(int index, UnionHolder holder) {
    getList().read(index, holder);
  }

  private FieldReader getReaderForIndex(int index) {
    int typeValue = data.getTypeValue(index);
    FieldReader reader = (FieldReader) readers[typeValue];
    if (reader != null) {
      return reader;
    }
    switch (MinorType.values()[typeValue]) {
    case NULL:
      return NullReader.INSTANCE;
    case STRUCT:
      return (FieldReader) getStruct();
    case LIST:
      return (FieldReader) getList();
    case MAP:
      return (FieldReader) getMap();
    case TINYINT:
      return (FieldReader) getTinyInt();
    case UINT1:
      return (FieldReader) getUInt1();
    case UINT2:
      return (FieldReader) getUInt2();
    case SMALLINT:
      return (FieldReader) getSmallInt();
    case FLOAT2:
      return (FieldReader) getFloat2();
    case INT:
      return (FieldReader) getInt();
    case UINT4:
      return (FieldReader) getUInt4();
    case FLOAT4:
      return (FieldReader) getFloat4();
    case DATEDAY:
      return (FieldReader) getDateDay();
    case INTERVALYEAR:
      return (FieldReader) getIntervalYear();
    case TIMESEC:
      return (FieldReader) getTimeSec();
    case TIMEMILLI:
      return (FieldReader) getTimeMilli();
    case BIGINT:
      return (FieldReader) getBigInt();
    case UINT8:
      return (FieldReader) getUInt8();
    case FLOAT8:
      return (FieldReader) getFloat8();
    case DATEMILLI:
      return (FieldReader) getDateMilli();
    case DURATION:
      return (FieldReader) getDuration();
    case TIMESTAMPSEC:
      return (FieldReader) getTimeStampSec();
    case TIMESTAMPMILLI:
      return (FieldReader) getTimeStampMilli();
    case TIMESTAMPMICRO:
      return (FieldReader) getTimeStampMicro();
    case TIMESTAMPNANO:
      return (FieldReader) getTimeStampNano();
    case TIMESTAMPSECTZ:
      return (FieldReader) getTimeStampSecTZ();
    case TIMESTAMPMILLITZ:
      return (FieldReader) getTimeStampMilliTZ();
    case TIMESTAMPMICROTZ:
      return (FieldReader) getTimeStampMicroTZ();
    case TIMESTAMPNANOTZ:
      return (FieldReader) getTimeStampNanoTZ();
    case TIMEMICRO:
      return (FieldReader) getTimeMicro();
    case TIMENANO:
      return (FieldReader) getTimeNano();
    case INTERVALDAY:
      return (FieldReader) getIntervalDay();
    case INTERVALMONTHDAYNANO:
      return (FieldReader) getIntervalMonthDayNano();
    case DECIMAL256:
      return (FieldReader) getDecimal256();
    case DECIMAL:
      return (FieldReader) getDecimal();
    case FIXEDSIZEBINARY:
      return (FieldReader) getFixedSizeBinary();
    case VARBINARY:
      return (FieldReader) getVarBinary();
    case VARCHAR:
      return (FieldReader) getVarChar();
    case LARGEVARCHAR:
      return (FieldReader) getLargeVarChar();
    case LARGEVARBINARY:
      return (FieldReader) getLargeVarBinary();
    case BIT:
      return (FieldReader) getBit();
    default:
      throw new UnsupportedOperationException("Unsupported type: " + MinorType.values()[typeValue]);
    }
  }

  private SingleStructReaderImpl structReader;

  private StructReader getStruct() {
    if (structReader == null) {
      structReader = (SingleStructReaderImpl) data.getStruct().getReader();
      structReader.setPosition(idx());
      readers[MinorType.STRUCT.ordinal()] = structReader;
    }
    return structReader;
  }

  private UnionListReader listReader;

  private FieldReader getList() {
    if (listReader == null) {
      listReader = new UnionListReader(data.getList());
      listReader.setPosition(idx());
      readers[MinorType.LIST.ordinal()] = listReader;
    }
    return listReader;
  }

  private UnionMapReader mapReader;

  private FieldReader getMap() {
    if (mapReader == null) {
      mapReader = new UnionMapReader(data.getMap());
      mapReader.setPosition(idx());
      readers[MinorType.MAP.ordinal()] = mapReader;
    }
    return mapReader;
  }

  @Override
  public java.util.Iterator<String> iterator() {
    return getStruct().iterator();
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


  private TinyIntReaderImpl tinyIntReader;

  private TinyIntReaderImpl getTinyInt() {
    if (tinyIntReader == null) {
      tinyIntReader = new TinyIntReaderImpl(data.getTinyIntVector());
      tinyIntReader.setPosition(idx());
      readers[MinorType.TINYINT.ordinal()] = tinyIntReader;
    }
    return tinyIntReader;
  }

  public void read(NullableTinyIntHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(TinyIntWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private UInt1ReaderImpl uInt1Reader;

  private UInt1ReaderImpl getUInt1() {
    if (uInt1Reader == null) {
      uInt1Reader = new UInt1ReaderImpl(data.getUInt1Vector());
      uInt1Reader.setPosition(idx());
      readers[MinorType.UINT1.ordinal()] = uInt1Reader;
    }
    return uInt1Reader;
  }

  public void read(NullableUInt1Holder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(UInt1Writer writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private UInt2ReaderImpl uInt2Reader;

  private UInt2ReaderImpl getUInt2() {
    if (uInt2Reader == null) {
      uInt2Reader = new UInt2ReaderImpl(data.getUInt2Vector());
      uInt2Reader.setPosition(idx());
      readers[MinorType.UINT2.ordinal()] = uInt2Reader;
    }
    return uInt2Reader;
  }

  public void read(NullableUInt2Holder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(UInt2Writer writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private SmallIntReaderImpl smallIntReader;

  private SmallIntReaderImpl getSmallInt() {
    if (smallIntReader == null) {
      smallIntReader = new SmallIntReaderImpl(data.getSmallIntVector());
      smallIntReader.setPosition(idx());
      readers[MinorType.SMALLINT.ordinal()] = smallIntReader;
    }
    return smallIntReader;
  }

  public void read(NullableSmallIntHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(SmallIntWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private Float2ReaderImpl float2Reader;

  private Float2ReaderImpl getFloat2() {
    if (float2Reader == null) {
      float2Reader = new Float2ReaderImpl(data.getFloat2Vector());
      float2Reader.setPosition(idx());
      readers[MinorType.FLOAT2.ordinal()] = float2Reader;
    }
    return float2Reader;
  }

  public void read(NullableFloat2Holder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(Float2Writer writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private IntReaderImpl intReader;

  private IntReaderImpl getInt() {
    if (intReader == null) {
      intReader = new IntReaderImpl(data.getIntVector());
      intReader.setPosition(idx());
      readers[MinorType.INT.ordinal()] = intReader;
    }
    return intReader;
  }

  public void read(NullableIntHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(IntWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private UInt4ReaderImpl uInt4Reader;

  private UInt4ReaderImpl getUInt4() {
    if (uInt4Reader == null) {
      uInt4Reader = new UInt4ReaderImpl(data.getUInt4Vector());
      uInt4Reader.setPosition(idx());
      readers[MinorType.UINT4.ordinal()] = uInt4Reader;
    }
    return uInt4Reader;
  }

  public void read(NullableUInt4Holder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(UInt4Writer writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private Float4ReaderImpl float4Reader;

  private Float4ReaderImpl getFloat4() {
    if (float4Reader == null) {
      float4Reader = new Float4ReaderImpl(data.getFloat4Vector());
      float4Reader.setPosition(idx());
      readers[MinorType.FLOAT4.ordinal()] = float4Reader;
    }
    return float4Reader;
  }

  public void read(NullableFloat4Holder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(Float4Writer writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private DateDayReaderImpl dateDayReader;

  private DateDayReaderImpl getDateDay() {
    if (dateDayReader == null) {
      dateDayReader = new DateDayReaderImpl(data.getDateDayVector());
      dateDayReader.setPosition(idx());
      readers[MinorType.DATEDAY.ordinal()] = dateDayReader;
    }
    return dateDayReader;
  }

  public void read(NullableDateDayHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(DateDayWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private IntervalYearReaderImpl intervalYearReader;

  private IntervalYearReaderImpl getIntervalYear() {
    if (intervalYearReader == null) {
      intervalYearReader = new IntervalYearReaderImpl(data.getIntervalYearVector());
      intervalYearReader.setPosition(idx());
      readers[MinorType.INTERVALYEAR.ordinal()] = intervalYearReader;
    }
    return intervalYearReader;
  }

  public void read(NullableIntervalYearHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(IntervalYearWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private TimeSecReaderImpl timeSecReader;

  private TimeSecReaderImpl getTimeSec() {
    if (timeSecReader == null) {
      timeSecReader = new TimeSecReaderImpl(data.getTimeSecVector());
      timeSecReader.setPosition(idx());
      readers[MinorType.TIMESEC.ordinal()] = timeSecReader;
    }
    return timeSecReader;
  }

  public void read(NullableTimeSecHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(TimeSecWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private TimeMilliReaderImpl timeMilliReader;

  private TimeMilliReaderImpl getTimeMilli() {
    if (timeMilliReader == null) {
      timeMilliReader = new TimeMilliReaderImpl(data.getTimeMilliVector());
      timeMilliReader.setPosition(idx());
      readers[MinorType.TIMEMILLI.ordinal()] = timeMilliReader;
    }
    return timeMilliReader;
  }

  public void read(NullableTimeMilliHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(TimeMilliWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private BigIntReaderImpl bigIntReader;

  private BigIntReaderImpl getBigInt() {
    if (bigIntReader == null) {
      bigIntReader = new BigIntReaderImpl(data.getBigIntVector());
      bigIntReader.setPosition(idx());
      readers[MinorType.BIGINT.ordinal()] = bigIntReader;
    }
    return bigIntReader;
  }

  public void read(NullableBigIntHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(BigIntWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private UInt8ReaderImpl uInt8Reader;

  private UInt8ReaderImpl getUInt8() {
    if (uInt8Reader == null) {
      uInt8Reader = new UInt8ReaderImpl(data.getUInt8Vector());
      uInt8Reader.setPosition(idx());
      readers[MinorType.UINT8.ordinal()] = uInt8Reader;
    }
    return uInt8Reader;
  }

  public void read(NullableUInt8Holder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(UInt8Writer writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private Float8ReaderImpl float8Reader;

  private Float8ReaderImpl getFloat8() {
    if (float8Reader == null) {
      float8Reader = new Float8ReaderImpl(data.getFloat8Vector());
      float8Reader.setPosition(idx());
      readers[MinorType.FLOAT8.ordinal()] = float8Reader;
    }
    return float8Reader;
  }

  public void read(NullableFloat8Holder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(Float8Writer writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private DateMilliReaderImpl dateMilliReader;

  private DateMilliReaderImpl getDateMilli() {
    if (dateMilliReader == null) {
      dateMilliReader = new DateMilliReaderImpl(data.getDateMilliVector());
      dateMilliReader.setPosition(idx());
      readers[MinorType.DATEMILLI.ordinal()] = dateMilliReader;
    }
    return dateMilliReader;
  }

  public void read(NullableDateMilliHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(DateMilliWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private DurationReaderImpl durationReader;

  private DurationReaderImpl getDuration() {
    if (durationReader == null) {
      durationReader = new DurationReaderImpl(data.getDurationVector());
      durationReader.setPosition(idx());
      readers[MinorType.DURATION.ordinal()] = durationReader;
    }
    return durationReader;
  }

  public void read(NullableDurationHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(DurationWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private TimeStampSecReaderImpl timeStampSecReader;

  private TimeStampSecReaderImpl getTimeStampSec() {
    if (timeStampSecReader == null) {
      timeStampSecReader = new TimeStampSecReaderImpl(data.getTimeStampSecVector());
      timeStampSecReader.setPosition(idx());
      readers[MinorType.TIMESTAMPSEC.ordinal()] = timeStampSecReader;
    }
    return timeStampSecReader;
  }

  public void read(NullableTimeStampSecHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(TimeStampSecWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private TimeStampMilliReaderImpl timeStampMilliReader;

  private TimeStampMilliReaderImpl getTimeStampMilli() {
    if (timeStampMilliReader == null) {
      timeStampMilliReader = new TimeStampMilliReaderImpl(data.getTimeStampMilliVector());
      timeStampMilliReader.setPosition(idx());
      readers[MinorType.TIMESTAMPMILLI.ordinal()] = timeStampMilliReader;
    }
    return timeStampMilliReader;
  }

  public void read(NullableTimeStampMilliHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(TimeStampMilliWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private TimeStampMicroReaderImpl timeStampMicroReader;

  private TimeStampMicroReaderImpl getTimeStampMicro() {
    if (timeStampMicroReader == null) {
      timeStampMicroReader = new TimeStampMicroReaderImpl(data.getTimeStampMicroVector());
      timeStampMicroReader.setPosition(idx());
      readers[MinorType.TIMESTAMPMICRO.ordinal()] = timeStampMicroReader;
    }
    return timeStampMicroReader;
  }

  public void read(NullableTimeStampMicroHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(TimeStampMicroWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private TimeStampNanoReaderImpl timeStampNanoReader;

  private TimeStampNanoReaderImpl getTimeStampNano() {
    if (timeStampNanoReader == null) {
      timeStampNanoReader = new TimeStampNanoReaderImpl(data.getTimeStampNanoVector());
      timeStampNanoReader.setPosition(idx());
      readers[MinorType.TIMESTAMPNANO.ordinal()] = timeStampNanoReader;
    }
    return timeStampNanoReader;
  }

  public void read(NullableTimeStampNanoHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(TimeStampNanoWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private TimeStampSecTZReaderImpl timeStampSecTZReader;

  private TimeStampSecTZReaderImpl getTimeStampSecTZ() {
    if (timeStampSecTZReader == null) {
      timeStampSecTZReader = new TimeStampSecTZReaderImpl(data.getTimeStampSecTZVector());
      timeStampSecTZReader.setPosition(idx());
      readers[MinorType.TIMESTAMPSECTZ.ordinal()] = timeStampSecTZReader;
    }
    return timeStampSecTZReader;
  }

  public void read(NullableTimeStampSecTZHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(TimeStampSecTZWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private TimeStampMilliTZReaderImpl timeStampMilliTZReader;

  private TimeStampMilliTZReaderImpl getTimeStampMilliTZ() {
    if (timeStampMilliTZReader == null) {
      timeStampMilliTZReader = new TimeStampMilliTZReaderImpl(data.getTimeStampMilliTZVector());
      timeStampMilliTZReader.setPosition(idx());
      readers[MinorType.TIMESTAMPMILLITZ.ordinal()] = timeStampMilliTZReader;
    }
    return timeStampMilliTZReader;
  }

  public void read(NullableTimeStampMilliTZHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(TimeStampMilliTZWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private TimeStampMicroTZReaderImpl timeStampMicroTZReader;

  private TimeStampMicroTZReaderImpl getTimeStampMicroTZ() {
    if (timeStampMicroTZReader == null) {
      timeStampMicroTZReader = new TimeStampMicroTZReaderImpl(data.getTimeStampMicroTZVector());
      timeStampMicroTZReader.setPosition(idx());
      readers[MinorType.TIMESTAMPMICROTZ.ordinal()] = timeStampMicroTZReader;
    }
    return timeStampMicroTZReader;
  }

  public void read(NullableTimeStampMicroTZHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(TimeStampMicroTZWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private TimeStampNanoTZReaderImpl timeStampNanoTZReader;

  private TimeStampNanoTZReaderImpl getTimeStampNanoTZ() {
    if (timeStampNanoTZReader == null) {
      timeStampNanoTZReader = new TimeStampNanoTZReaderImpl(data.getTimeStampNanoTZVector());
      timeStampNanoTZReader.setPosition(idx());
      readers[MinorType.TIMESTAMPNANOTZ.ordinal()] = timeStampNanoTZReader;
    }
    return timeStampNanoTZReader;
  }

  public void read(NullableTimeStampNanoTZHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(TimeStampNanoTZWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private TimeMicroReaderImpl timeMicroReader;

  private TimeMicroReaderImpl getTimeMicro() {
    if (timeMicroReader == null) {
      timeMicroReader = new TimeMicroReaderImpl(data.getTimeMicroVector());
      timeMicroReader.setPosition(idx());
      readers[MinorType.TIMEMICRO.ordinal()] = timeMicroReader;
    }
    return timeMicroReader;
  }

  public void read(NullableTimeMicroHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(TimeMicroWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private TimeNanoReaderImpl timeNanoReader;

  private TimeNanoReaderImpl getTimeNano() {
    if (timeNanoReader == null) {
      timeNanoReader = new TimeNanoReaderImpl(data.getTimeNanoVector());
      timeNanoReader.setPosition(idx());
      readers[MinorType.TIMENANO.ordinal()] = timeNanoReader;
    }
    return timeNanoReader;
  }

  public void read(NullableTimeNanoHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(TimeNanoWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private IntervalDayReaderImpl intervalDayReader;

  private IntervalDayReaderImpl getIntervalDay() {
    if (intervalDayReader == null) {
      intervalDayReader = new IntervalDayReaderImpl(data.getIntervalDayVector());
      intervalDayReader.setPosition(idx());
      readers[MinorType.INTERVALDAY.ordinal()] = intervalDayReader;
    }
    return intervalDayReader;
  }

  public void read(NullableIntervalDayHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(IntervalDayWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private IntervalMonthDayNanoReaderImpl intervalMonthDayNanoReader;

  private IntervalMonthDayNanoReaderImpl getIntervalMonthDayNano() {
    if (intervalMonthDayNanoReader == null) {
      intervalMonthDayNanoReader = new IntervalMonthDayNanoReaderImpl(data.getIntervalMonthDayNanoVector());
      intervalMonthDayNanoReader.setPosition(idx());
      readers[MinorType.INTERVALMONTHDAYNANO.ordinal()] = intervalMonthDayNanoReader;
    }
    return intervalMonthDayNanoReader;
  }

  public void read(NullableIntervalMonthDayNanoHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(IntervalMonthDayNanoWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private Decimal256ReaderImpl decimal256Reader;

  private Decimal256ReaderImpl getDecimal256() {
    if (decimal256Reader == null) {
      decimal256Reader = new Decimal256ReaderImpl(data.getDecimal256Vector());
      decimal256Reader.setPosition(idx());
      readers[MinorType.DECIMAL256.ordinal()] = decimal256Reader;
    }
    return decimal256Reader;
  }

  public void read(NullableDecimal256Holder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(Decimal256Writer writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private DecimalReaderImpl decimalReader;

  private DecimalReaderImpl getDecimal() {
    if (decimalReader == null) {
      decimalReader = new DecimalReaderImpl(data.getDecimalVector());
      decimalReader.setPosition(idx());
      readers[MinorType.DECIMAL.ordinal()] = decimalReader;
    }
    return decimalReader;
  }

  public void read(NullableDecimalHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(DecimalWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private FixedSizeBinaryReaderImpl fixedSizeBinaryReader;

  private FixedSizeBinaryReaderImpl getFixedSizeBinary() {
    if (fixedSizeBinaryReader == null) {
      fixedSizeBinaryReader = new FixedSizeBinaryReaderImpl(data.getFixedSizeBinaryVector());
      fixedSizeBinaryReader.setPosition(idx());
      readers[MinorType.FIXEDSIZEBINARY.ordinal()] = fixedSizeBinaryReader;
    }
    return fixedSizeBinaryReader;
  }

  public void read(NullableFixedSizeBinaryHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(FixedSizeBinaryWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private VarBinaryReaderImpl varBinaryReader;

  private VarBinaryReaderImpl getVarBinary() {
    if (varBinaryReader == null) {
      varBinaryReader = new VarBinaryReaderImpl(data.getVarBinaryVector());
      varBinaryReader.setPosition(idx());
      readers[MinorType.VARBINARY.ordinal()] = varBinaryReader;
    }
    return varBinaryReader;
  }

  public void read(NullableVarBinaryHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(VarBinaryWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private VarCharReaderImpl varCharReader;

  private VarCharReaderImpl getVarChar() {
    if (varCharReader == null) {
      varCharReader = new VarCharReaderImpl(data.getVarCharVector());
      varCharReader.setPosition(idx());
      readers[MinorType.VARCHAR.ordinal()] = varCharReader;
    }
    return varCharReader;
  }

  public void read(NullableVarCharHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(VarCharWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private LargeVarCharReaderImpl largeVarCharReader;

  private LargeVarCharReaderImpl getLargeVarChar() {
    if (largeVarCharReader == null) {
      largeVarCharReader = new LargeVarCharReaderImpl(data.getLargeVarCharVector());
      largeVarCharReader.setPosition(idx());
      readers[MinorType.LARGEVARCHAR.ordinal()] = largeVarCharReader;
    }
    return largeVarCharReader;
  }

  public void read(NullableLargeVarCharHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(LargeVarCharWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private LargeVarBinaryReaderImpl largeVarBinaryReader;

  private LargeVarBinaryReaderImpl getLargeVarBinary() {
    if (largeVarBinaryReader == null) {
      largeVarBinaryReader = new LargeVarBinaryReaderImpl(data.getLargeVarBinaryVector());
      largeVarBinaryReader.setPosition(idx());
      readers[MinorType.LARGEVARBINARY.ordinal()] = largeVarBinaryReader;
    }
    return largeVarBinaryReader;
  }

  public void read(NullableLargeVarBinaryHolder holder){
    getReaderForIndex(idx()).read(holder);
  }

  public void copyAsValue(LargeVarBinaryWriter writer){
    getReaderForIndex(idx()).copyAsValue(writer);
  }

  private BitReaderImpl bitReader;

  private BitReaderImpl getBit() {
    if (bitReader == null) {
      bitReader = new BitReaderImpl(data.getBitVector());
      bitReader.setPosition(idx());
      readers[MinorType.BIT.ordinal()] = bitReader;
    }
    return bitReader;
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
    for (BaseReader reader : readers) {
      if (reader != null) {
        reader.setPosition(index);
      }
    }
  }

  public FieldReader reader(String name){
    return getStruct().reader(name);
  }

  public FieldReader reader() {
    return getList().reader();
  }

  public boolean next() {
    return getReaderForIndex(idx()).next();
  }
}
