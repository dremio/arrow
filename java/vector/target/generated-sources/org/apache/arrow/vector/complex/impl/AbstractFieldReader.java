

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
 * Source code generated using FreeMarker template AbstractFieldReader.java
 */
@SuppressWarnings("unused")
abstract class AbstractFieldReader extends AbstractBaseReader implements FieldReader{

  AbstractFieldReader(){
    super();
  }

  /**
   * Returns true if the current value of the reader is not null
   * @return whether the current value is set
   */
  public boolean isSet() {
    return true;
  }

  @Override
  public Field getField() {
    fail("getField");
    return null;
  }

  public Object readObject(int arrayIndex) {
    fail("readObject(int arrayIndex)");
    return null;
  }

  public Object readObject() {
    fail("readObject()");
    return null;
  }

  public BigDecimal readBigDecimal(int arrayIndex) {
    fail("readBigDecimal(int arrayIndex)");
    return null;
  }

  public BigDecimal readBigDecimal() {
    fail("readBigDecimal()");
    return null;
  }

  public Short readShort(int arrayIndex) {
    fail("readShort(int arrayIndex)");
    return null;
  }

  public Short readShort() {
    fail("readShort()");
    return null;
  }

  public Integer readInteger(int arrayIndex) {
    fail("readInteger(int arrayIndex)");
    return null;
  }

  public Integer readInteger() {
    fail("readInteger()");
    return null;
  }

  public Long readLong(int arrayIndex) {
    fail("readLong(int arrayIndex)");
    return null;
  }

  public Long readLong() {
    fail("readLong()");
    return null;
  }

  public Boolean readBoolean(int arrayIndex) {
    fail("readBoolean(int arrayIndex)");
    return null;
  }

  public Boolean readBoolean() {
    fail("readBoolean()");
    return null;
  }

  public LocalDateTime readLocalDateTime(int arrayIndex) {
    fail("readLocalDateTime(int arrayIndex)");
    return null;
  }

  public LocalDateTime readLocalDateTime() {
    fail("readLocalDateTime()");
    return null;
  }

  public Duration readDuration(int arrayIndex) {
    fail("readDuration(int arrayIndex)");
    return null;
  }

  public Duration readDuration() {
    fail("readDuration()");
    return null;
  }

  public Period readPeriod(int arrayIndex) {
    fail("readPeriod(int arrayIndex)");
    return null;
  }

  public Period readPeriod() {
    fail("readPeriod()");
    return null;
  }

  public Double readDouble(int arrayIndex) {
    fail("readDouble(int arrayIndex)");
    return null;
  }

  public Double readDouble() {
    fail("readDouble()");
    return null;
  }

  public Float readFloat(int arrayIndex) {
    fail("readFloat(int arrayIndex)");
    return null;
  }

  public Float readFloat() {
    fail("readFloat()");
    return null;
  }

  public Character readCharacter(int arrayIndex) {
    fail("readCharacter(int arrayIndex)");
    return null;
  }

  public Character readCharacter() {
    fail("readCharacter()");
    return null;
  }

  public Text readText(int arrayIndex) {
    fail("readText(int arrayIndex)");
    return null;
  }

  public Text readText() {
    fail("readText()");
    return null;
  }

  public String readString(int arrayIndex) {
    fail("readString(int arrayIndex)");
    return null;
  }

  public String readString() {
    fail("readString()");
    return null;
  }

  public Byte readByte(int arrayIndex) {
    fail("readByte(int arrayIndex)");
    return null;
  }

  public Byte readByte() {
    fail("readByte()");
    return null;
  }

  public byte[] readByteArray(int arrayIndex) {
    fail("readByteArray(int arrayIndex)");
    return null;
  }

  public byte[] readByteArray() {
    fail("readByteArray()");
    return null;
  }

  public PeriodDuration readPeriodDuration(int arrayIndex) {
    fail("readPeriodDuration(int arrayIndex)");
    return null;
  }

  public PeriodDuration readPeriodDuration() {
    fail("readPeriodDuration()");
    return null;
  }

  public void copyAsValue(StructWriter writer) {
    fail("CopyAsValue StructWriter");
  }

  public void copyAsField(String name, StructWriter writer) {
    fail("CopyAsField StructWriter");
  }

  public void copyAsField(String name, ListWriter writer) {
    fail("CopyAsFieldList");
  }

  public void copyAsField(String name, MapWriter writer) {
    fail("CopyAsFieldMap");
  }

  public void read(TinyIntHolder holder) {
    fail("TinyInt");
  }

  public void read(NullableTinyIntHolder holder) {
    fail("TinyInt");
  }

  public void read(int arrayIndex, TinyIntHolder holder) {
    fail("RepeatedTinyInt");
  }

  public void read(int arrayIndex, NullableTinyIntHolder holder) {
    fail("RepeatedTinyInt");
  }

  public void copyAsValue(TinyIntWriter writer) {
    fail("CopyAsValueTinyInt");
  }

  public void copyAsField(String name, TinyIntWriter writer) {
    fail("CopyAsFieldTinyInt");
  }

  public void read(UInt1Holder holder) {
    fail("UInt1");
  }

  public void read(NullableUInt1Holder holder) {
    fail("UInt1");
  }

  public void read(int arrayIndex, UInt1Holder holder) {
    fail("RepeatedUInt1");
  }

  public void read(int arrayIndex, NullableUInt1Holder holder) {
    fail("RepeatedUInt1");
  }

  public void copyAsValue(UInt1Writer writer) {
    fail("CopyAsValueUInt1");
  }

  public void copyAsField(String name, UInt1Writer writer) {
    fail("CopyAsFieldUInt1");
  }

  public void read(UInt2Holder holder) {
    fail("UInt2");
  }

  public void read(NullableUInt2Holder holder) {
    fail("UInt2");
  }

  public void read(int arrayIndex, UInt2Holder holder) {
    fail("RepeatedUInt2");
  }

  public void read(int arrayIndex, NullableUInt2Holder holder) {
    fail("RepeatedUInt2");
  }

  public void copyAsValue(UInt2Writer writer) {
    fail("CopyAsValueUInt2");
  }

  public void copyAsField(String name, UInt2Writer writer) {
    fail("CopyAsFieldUInt2");
  }

  public void read(SmallIntHolder holder) {
    fail("SmallInt");
  }

  public void read(NullableSmallIntHolder holder) {
    fail("SmallInt");
  }

  public void read(int arrayIndex, SmallIntHolder holder) {
    fail("RepeatedSmallInt");
  }

  public void read(int arrayIndex, NullableSmallIntHolder holder) {
    fail("RepeatedSmallInt");
  }

  public void copyAsValue(SmallIntWriter writer) {
    fail("CopyAsValueSmallInt");
  }

  public void copyAsField(String name, SmallIntWriter writer) {
    fail("CopyAsFieldSmallInt");
  }

  public void read(Float2Holder holder) {
    fail("Float2");
  }

  public void read(NullableFloat2Holder holder) {
    fail("Float2");
  }

  public void read(int arrayIndex, Float2Holder holder) {
    fail("RepeatedFloat2");
  }

  public void read(int arrayIndex, NullableFloat2Holder holder) {
    fail("RepeatedFloat2");
  }

  public void copyAsValue(Float2Writer writer) {
    fail("CopyAsValueFloat2");
  }

  public void copyAsField(String name, Float2Writer writer) {
    fail("CopyAsFieldFloat2");
  }

  public void read(IntHolder holder) {
    fail("Int");
  }

  public void read(NullableIntHolder holder) {
    fail("Int");
  }

  public void read(int arrayIndex, IntHolder holder) {
    fail("RepeatedInt");
  }

  public void read(int arrayIndex, NullableIntHolder holder) {
    fail("RepeatedInt");
  }

  public void copyAsValue(IntWriter writer) {
    fail("CopyAsValueInt");
  }

  public void copyAsField(String name, IntWriter writer) {
    fail("CopyAsFieldInt");
  }

  public void read(UInt4Holder holder) {
    fail("UInt4");
  }

  public void read(NullableUInt4Holder holder) {
    fail("UInt4");
  }

  public void read(int arrayIndex, UInt4Holder holder) {
    fail("RepeatedUInt4");
  }

  public void read(int arrayIndex, NullableUInt4Holder holder) {
    fail("RepeatedUInt4");
  }

  public void copyAsValue(UInt4Writer writer) {
    fail("CopyAsValueUInt4");
  }

  public void copyAsField(String name, UInt4Writer writer) {
    fail("CopyAsFieldUInt4");
  }

  public void read(Float4Holder holder) {
    fail("Float4");
  }

  public void read(NullableFloat4Holder holder) {
    fail("Float4");
  }

  public void read(int arrayIndex, Float4Holder holder) {
    fail("RepeatedFloat4");
  }

  public void read(int arrayIndex, NullableFloat4Holder holder) {
    fail("RepeatedFloat4");
  }

  public void copyAsValue(Float4Writer writer) {
    fail("CopyAsValueFloat4");
  }

  public void copyAsField(String name, Float4Writer writer) {
    fail("CopyAsFieldFloat4");
  }

  public void read(DateDayHolder holder) {
    fail("DateDay");
  }

  public void read(NullableDateDayHolder holder) {
    fail("DateDay");
  }

  public void read(int arrayIndex, DateDayHolder holder) {
    fail("RepeatedDateDay");
  }

  public void read(int arrayIndex, NullableDateDayHolder holder) {
    fail("RepeatedDateDay");
  }

  public void copyAsValue(DateDayWriter writer) {
    fail("CopyAsValueDateDay");
  }

  public void copyAsField(String name, DateDayWriter writer) {
    fail("CopyAsFieldDateDay");
  }

  public void read(IntervalYearHolder holder) {
    fail("IntervalYear");
  }

  public void read(NullableIntervalYearHolder holder) {
    fail("IntervalYear");
  }

  public void read(int arrayIndex, IntervalYearHolder holder) {
    fail("RepeatedIntervalYear");
  }

  public void read(int arrayIndex, NullableIntervalYearHolder holder) {
    fail("RepeatedIntervalYear");
  }

  public void copyAsValue(IntervalYearWriter writer) {
    fail("CopyAsValueIntervalYear");
  }

  public void copyAsField(String name, IntervalYearWriter writer) {
    fail("CopyAsFieldIntervalYear");
  }

  public void read(TimeSecHolder holder) {
    fail("TimeSec");
  }

  public void read(NullableTimeSecHolder holder) {
    fail("TimeSec");
  }

  public void read(int arrayIndex, TimeSecHolder holder) {
    fail("RepeatedTimeSec");
  }

  public void read(int arrayIndex, NullableTimeSecHolder holder) {
    fail("RepeatedTimeSec");
  }

  public void copyAsValue(TimeSecWriter writer) {
    fail("CopyAsValueTimeSec");
  }

  public void copyAsField(String name, TimeSecWriter writer) {
    fail("CopyAsFieldTimeSec");
  }

  public void read(TimeMilliHolder holder) {
    fail("TimeMilli");
  }

  public void read(NullableTimeMilliHolder holder) {
    fail("TimeMilli");
  }

  public void read(int arrayIndex, TimeMilliHolder holder) {
    fail("RepeatedTimeMilli");
  }

  public void read(int arrayIndex, NullableTimeMilliHolder holder) {
    fail("RepeatedTimeMilli");
  }

  public void copyAsValue(TimeMilliWriter writer) {
    fail("CopyAsValueTimeMilli");
  }

  public void copyAsField(String name, TimeMilliWriter writer) {
    fail("CopyAsFieldTimeMilli");
  }

  public void read(BigIntHolder holder) {
    fail("BigInt");
  }

  public void read(NullableBigIntHolder holder) {
    fail("BigInt");
  }

  public void read(int arrayIndex, BigIntHolder holder) {
    fail("RepeatedBigInt");
  }

  public void read(int arrayIndex, NullableBigIntHolder holder) {
    fail("RepeatedBigInt");
  }

  public void copyAsValue(BigIntWriter writer) {
    fail("CopyAsValueBigInt");
  }

  public void copyAsField(String name, BigIntWriter writer) {
    fail("CopyAsFieldBigInt");
  }

  public void read(UInt8Holder holder) {
    fail("UInt8");
  }

  public void read(NullableUInt8Holder holder) {
    fail("UInt8");
  }

  public void read(int arrayIndex, UInt8Holder holder) {
    fail("RepeatedUInt8");
  }

  public void read(int arrayIndex, NullableUInt8Holder holder) {
    fail("RepeatedUInt8");
  }

  public void copyAsValue(UInt8Writer writer) {
    fail("CopyAsValueUInt8");
  }

  public void copyAsField(String name, UInt8Writer writer) {
    fail("CopyAsFieldUInt8");
  }

  public void read(Float8Holder holder) {
    fail("Float8");
  }

  public void read(NullableFloat8Holder holder) {
    fail("Float8");
  }

  public void read(int arrayIndex, Float8Holder holder) {
    fail("RepeatedFloat8");
  }

  public void read(int arrayIndex, NullableFloat8Holder holder) {
    fail("RepeatedFloat8");
  }

  public void copyAsValue(Float8Writer writer) {
    fail("CopyAsValueFloat8");
  }

  public void copyAsField(String name, Float8Writer writer) {
    fail("CopyAsFieldFloat8");
  }

  public void read(DateMilliHolder holder) {
    fail("DateMilli");
  }

  public void read(NullableDateMilliHolder holder) {
    fail("DateMilli");
  }

  public void read(int arrayIndex, DateMilliHolder holder) {
    fail("RepeatedDateMilli");
  }

  public void read(int arrayIndex, NullableDateMilliHolder holder) {
    fail("RepeatedDateMilli");
  }

  public void copyAsValue(DateMilliWriter writer) {
    fail("CopyAsValueDateMilli");
  }

  public void copyAsField(String name, DateMilliWriter writer) {
    fail("CopyAsFieldDateMilli");
  }

  public void read(DurationHolder holder) {
    fail("Duration");
  }

  public void read(NullableDurationHolder holder) {
    fail("Duration");
  }

  public void read(int arrayIndex, DurationHolder holder) {
    fail("RepeatedDuration");
  }

  public void read(int arrayIndex, NullableDurationHolder holder) {
    fail("RepeatedDuration");
  }

  public void copyAsValue(DurationWriter writer) {
    fail("CopyAsValueDuration");
  }

  public void copyAsField(String name, DurationWriter writer) {
    fail("CopyAsFieldDuration");
  }

  public void read(TimeStampSecHolder holder) {
    fail("TimeStampSec");
  }

  public void read(NullableTimeStampSecHolder holder) {
    fail("TimeStampSec");
  }

  public void read(int arrayIndex, TimeStampSecHolder holder) {
    fail("RepeatedTimeStampSec");
  }

  public void read(int arrayIndex, NullableTimeStampSecHolder holder) {
    fail("RepeatedTimeStampSec");
  }

  public void copyAsValue(TimeStampSecWriter writer) {
    fail("CopyAsValueTimeStampSec");
  }

  public void copyAsField(String name, TimeStampSecWriter writer) {
    fail("CopyAsFieldTimeStampSec");
  }

  public void read(TimeStampMilliHolder holder) {
    fail("TimeStampMilli");
  }

  public void read(NullableTimeStampMilliHolder holder) {
    fail("TimeStampMilli");
  }

  public void read(int arrayIndex, TimeStampMilliHolder holder) {
    fail("RepeatedTimeStampMilli");
  }

  public void read(int arrayIndex, NullableTimeStampMilliHolder holder) {
    fail("RepeatedTimeStampMilli");
  }

  public void copyAsValue(TimeStampMilliWriter writer) {
    fail("CopyAsValueTimeStampMilli");
  }

  public void copyAsField(String name, TimeStampMilliWriter writer) {
    fail("CopyAsFieldTimeStampMilli");
  }

  public void read(TimeStampMicroHolder holder) {
    fail("TimeStampMicro");
  }

  public void read(NullableTimeStampMicroHolder holder) {
    fail("TimeStampMicro");
  }

  public void read(int arrayIndex, TimeStampMicroHolder holder) {
    fail("RepeatedTimeStampMicro");
  }

  public void read(int arrayIndex, NullableTimeStampMicroHolder holder) {
    fail("RepeatedTimeStampMicro");
  }

  public void copyAsValue(TimeStampMicroWriter writer) {
    fail("CopyAsValueTimeStampMicro");
  }

  public void copyAsField(String name, TimeStampMicroWriter writer) {
    fail("CopyAsFieldTimeStampMicro");
  }

  public void read(TimeStampNanoHolder holder) {
    fail("TimeStampNano");
  }

  public void read(NullableTimeStampNanoHolder holder) {
    fail("TimeStampNano");
  }

  public void read(int arrayIndex, TimeStampNanoHolder holder) {
    fail("RepeatedTimeStampNano");
  }

  public void read(int arrayIndex, NullableTimeStampNanoHolder holder) {
    fail("RepeatedTimeStampNano");
  }

  public void copyAsValue(TimeStampNanoWriter writer) {
    fail("CopyAsValueTimeStampNano");
  }

  public void copyAsField(String name, TimeStampNanoWriter writer) {
    fail("CopyAsFieldTimeStampNano");
  }

  public void read(TimeStampSecTZHolder holder) {
    fail("TimeStampSecTZ");
  }

  public void read(NullableTimeStampSecTZHolder holder) {
    fail("TimeStampSecTZ");
  }

  public void read(int arrayIndex, TimeStampSecTZHolder holder) {
    fail("RepeatedTimeStampSecTZ");
  }

  public void read(int arrayIndex, NullableTimeStampSecTZHolder holder) {
    fail("RepeatedTimeStampSecTZ");
  }

  public void copyAsValue(TimeStampSecTZWriter writer) {
    fail("CopyAsValueTimeStampSecTZ");
  }

  public void copyAsField(String name, TimeStampSecTZWriter writer) {
    fail("CopyAsFieldTimeStampSecTZ");
  }

  public void read(TimeStampMilliTZHolder holder) {
    fail("TimeStampMilliTZ");
  }

  public void read(NullableTimeStampMilliTZHolder holder) {
    fail("TimeStampMilliTZ");
  }

  public void read(int arrayIndex, TimeStampMilliTZHolder holder) {
    fail("RepeatedTimeStampMilliTZ");
  }

  public void read(int arrayIndex, NullableTimeStampMilliTZHolder holder) {
    fail("RepeatedTimeStampMilliTZ");
  }

  public void copyAsValue(TimeStampMilliTZWriter writer) {
    fail("CopyAsValueTimeStampMilliTZ");
  }

  public void copyAsField(String name, TimeStampMilliTZWriter writer) {
    fail("CopyAsFieldTimeStampMilliTZ");
  }

  public void read(TimeStampMicroTZHolder holder) {
    fail("TimeStampMicroTZ");
  }

  public void read(NullableTimeStampMicroTZHolder holder) {
    fail("TimeStampMicroTZ");
  }

  public void read(int arrayIndex, TimeStampMicroTZHolder holder) {
    fail("RepeatedTimeStampMicroTZ");
  }

  public void read(int arrayIndex, NullableTimeStampMicroTZHolder holder) {
    fail("RepeatedTimeStampMicroTZ");
  }

  public void copyAsValue(TimeStampMicroTZWriter writer) {
    fail("CopyAsValueTimeStampMicroTZ");
  }

  public void copyAsField(String name, TimeStampMicroTZWriter writer) {
    fail("CopyAsFieldTimeStampMicroTZ");
  }

  public void read(TimeStampNanoTZHolder holder) {
    fail("TimeStampNanoTZ");
  }

  public void read(NullableTimeStampNanoTZHolder holder) {
    fail("TimeStampNanoTZ");
  }

  public void read(int arrayIndex, TimeStampNanoTZHolder holder) {
    fail("RepeatedTimeStampNanoTZ");
  }

  public void read(int arrayIndex, NullableTimeStampNanoTZHolder holder) {
    fail("RepeatedTimeStampNanoTZ");
  }

  public void copyAsValue(TimeStampNanoTZWriter writer) {
    fail("CopyAsValueTimeStampNanoTZ");
  }

  public void copyAsField(String name, TimeStampNanoTZWriter writer) {
    fail("CopyAsFieldTimeStampNanoTZ");
  }

  public void read(TimeMicroHolder holder) {
    fail("TimeMicro");
  }

  public void read(NullableTimeMicroHolder holder) {
    fail("TimeMicro");
  }

  public void read(int arrayIndex, TimeMicroHolder holder) {
    fail("RepeatedTimeMicro");
  }

  public void read(int arrayIndex, NullableTimeMicroHolder holder) {
    fail("RepeatedTimeMicro");
  }

  public void copyAsValue(TimeMicroWriter writer) {
    fail("CopyAsValueTimeMicro");
  }

  public void copyAsField(String name, TimeMicroWriter writer) {
    fail("CopyAsFieldTimeMicro");
  }

  public void read(TimeNanoHolder holder) {
    fail("TimeNano");
  }

  public void read(NullableTimeNanoHolder holder) {
    fail("TimeNano");
  }

  public void read(int arrayIndex, TimeNanoHolder holder) {
    fail("RepeatedTimeNano");
  }

  public void read(int arrayIndex, NullableTimeNanoHolder holder) {
    fail("RepeatedTimeNano");
  }

  public void copyAsValue(TimeNanoWriter writer) {
    fail("CopyAsValueTimeNano");
  }

  public void copyAsField(String name, TimeNanoWriter writer) {
    fail("CopyAsFieldTimeNano");
  }

  public void read(IntervalDayHolder holder) {
    fail("IntervalDay");
  }

  public void read(NullableIntervalDayHolder holder) {
    fail("IntervalDay");
  }

  public void read(int arrayIndex, IntervalDayHolder holder) {
    fail("RepeatedIntervalDay");
  }

  public void read(int arrayIndex, NullableIntervalDayHolder holder) {
    fail("RepeatedIntervalDay");
  }

  public void copyAsValue(IntervalDayWriter writer) {
    fail("CopyAsValueIntervalDay");
  }

  public void copyAsField(String name, IntervalDayWriter writer) {
    fail("CopyAsFieldIntervalDay");
  }

  public void read(IntervalMonthDayNanoHolder holder) {
    fail("IntervalMonthDayNano");
  }

  public void read(NullableIntervalMonthDayNanoHolder holder) {
    fail("IntervalMonthDayNano");
  }

  public void read(int arrayIndex, IntervalMonthDayNanoHolder holder) {
    fail("RepeatedIntervalMonthDayNano");
  }

  public void read(int arrayIndex, NullableIntervalMonthDayNanoHolder holder) {
    fail("RepeatedIntervalMonthDayNano");
  }

  public void copyAsValue(IntervalMonthDayNanoWriter writer) {
    fail("CopyAsValueIntervalMonthDayNano");
  }

  public void copyAsField(String name, IntervalMonthDayNanoWriter writer) {
    fail("CopyAsFieldIntervalMonthDayNano");
  }

  public void read(Decimal256Holder holder) {
    fail("Decimal256");
  }

  public void read(NullableDecimal256Holder holder) {
    fail("Decimal256");
  }

  public void read(int arrayIndex, Decimal256Holder holder) {
    fail("RepeatedDecimal256");
  }

  public void read(int arrayIndex, NullableDecimal256Holder holder) {
    fail("RepeatedDecimal256");
  }

  public void copyAsValue(Decimal256Writer writer) {
    fail("CopyAsValueDecimal256");
  }

  public void copyAsField(String name, Decimal256Writer writer) {
    fail("CopyAsFieldDecimal256");
  }

  public void read(DecimalHolder holder) {
    fail("Decimal");
  }

  public void read(NullableDecimalHolder holder) {
    fail("Decimal");
  }

  public void read(int arrayIndex, DecimalHolder holder) {
    fail("RepeatedDecimal");
  }

  public void read(int arrayIndex, NullableDecimalHolder holder) {
    fail("RepeatedDecimal");
  }

  public void copyAsValue(DecimalWriter writer) {
    fail("CopyAsValueDecimal");
  }

  public void copyAsField(String name, DecimalWriter writer) {
    fail("CopyAsFieldDecimal");
  }

  public void read(FixedSizeBinaryHolder holder) {
    fail("FixedSizeBinary");
  }

  public void read(NullableFixedSizeBinaryHolder holder) {
    fail("FixedSizeBinary");
  }

  public void read(int arrayIndex, FixedSizeBinaryHolder holder) {
    fail("RepeatedFixedSizeBinary");
  }

  public void read(int arrayIndex, NullableFixedSizeBinaryHolder holder) {
    fail("RepeatedFixedSizeBinary");
  }

  public void copyAsValue(FixedSizeBinaryWriter writer) {
    fail("CopyAsValueFixedSizeBinary");
  }

  public void copyAsField(String name, FixedSizeBinaryWriter writer) {
    fail("CopyAsFieldFixedSizeBinary");
  }

  public void read(VarBinaryHolder holder) {
    fail("VarBinary");
  }

  public void read(NullableVarBinaryHolder holder) {
    fail("VarBinary");
  }

  public void read(int arrayIndex, VarBinaryHolder holder) {
    fail("RepeatedVarBinary");
  }

  public void read(int arrayIndex, NullableVarBinaryHolder holder) {
    fail("RepeatedVarBinary");
  }

  public void copyAsValue(VarBinaryWriter writer) {
    fail("CopyAsValueVarBinary");
  }

  public void copyAsField(String name, VarBinaryWriter writer) {
    fail("CopyAsFieldVarBinary");
  }

  public void read(VarCharHolder holder) {
    fail("VarChar");
  }

  public void read(NullableVarCharHolder holder) {
    fail("VarChar");
  }

  public void read(int arrayIndex, VarCharHolder holder) {
    fail("RepeatedVarChar");
  }

  public void read(int arrayIndex, NullableVarCharHolder holder) {
    fail("RepeatedVarChar");
  }

  public void copyAsValue(VarCharWriter writer) {
    fail("CopyAsValueVarChar");
  }

  public void copyAsField(String name, VarCharWriter writer) {
    fail("CopyAsFieldVarChar");
  }

  public void read(LargeVarCharHolder holder) {
    fail("LargeVarChar");
  }

  public void read(NullableLargeVarCharHolder holder) {
    fail("LargeVarChar");
  }

  public void read(int arrayIndex, LargeVarCharHolder holder) {
    fail("RepeatedLargeVarChar");
  }

  public void read(int arrayIndex, NullableLargeVarCharHolder holder) {
    fail("RepeatedLargeVarChar");
  }

  public void copyAsValue(LargeVarCharWriter writer) {
    fail("CopyAsValueLargeVarChar");
  }

  public void copyAsField(String name, LargeVarCharWriter writer) {
    fail("CopyAsFieldLargeVarChar");
  }

  public void read(LargeVarBinaryHolder holder) {
    fail("LargeVarBinary");
  }

  public void read(NullableLargeVarBinaryHolder holder) {
    fail("LargeVarBinary");
  }

  public void read(int arrayIndex, LargeVarBinaryHolder holder) {
    fail("RepeatedLargeVarBinary");
  }

  public void read(int arrayIndex, NullableLargeVarBinaryHolder holder) {
    fail("RepeatedLargeVarBinary");
  }

  public void copyAsValue(LargeVarBinaryWriter writer) {
    fail("CopyAsValueLargeVarBinary");
  }

  public void copyAsField(String name, LargeVarBinaryWriter writer) {
    fail("CopyAsFieldLargeVarBinary");
  }

  public void read(BitHolder holder) {
    fail("Bit");
  }

  public void read(NullableBitHolder holder) {
    fail("Bit");
  }

  public void read(int arrayIndex, BitHolder holder) {
    fail("RepeatedBit");
  }

  public void read(int arrayIndex, NullableBitHolder holder) {
    fail("RepeatedBit");
  }

  public void copyAsValue(BitWriter writer) {
    fail("CopyAsValueBit");
  }

  public void copyAsField(String name, BitWriter writer) {
    fail("CopyAsFieldBit");
  }

  public FieldReader reader(String name) {
    fail("reader(String name)");
    return null;
  }

  public FieldReader reader() {
    fail("reader()");
    return null;
  }

  public int size() {
    fail("size()");
    return -1;
  }

  private void fail(String name) {
    throw new IllegalArgumentException(String.format("You tried to read a [%s] type when you are using a field reader of type [%s].", name, this.getClass().getSimpleName()));
  }
}



