

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


        import org.apache.arrow.vector.complex.writer.BaseWriter;
        import org.apache.arrow.vector.types.Types.MinorType;

/*
 * This class is generated using freemarker and the DenseUnionWriter.java template.
 */
@SuppressWarnings("unused")
public class DenseUnionWriter extends AbstractFieldWriter implements FieldWriter {

  DenseUnionVector data;

  private BaseWriter[] writers = new BaseWriter[Byte.MAX_VALUE + 1];
  private final NullableStructWriterFactory nullableStructWriterFactory;

  public DenseUnionWriter(DenseUnionVector vector) {
    this(vector, NullableStructWriterFactory.getNullableStructWriterFactoryInstance());
  }

  public DenseUnionWriter(DenseUnionVector vector, NullableStructWriterFactory nullableStructWriterFactory) {
    data = vector;
    this.nullableStructWriterFactory = nullableStructWriterFactory;
  }

  @Override
  public void setPosition(int index) {
    super.setPosition(index);
    for (BaseWriter writer : writers) {
      writer.setPosition(index);
    }
  }

  @Override
  public void start() {
    byte typeId = data.getTypeId(idx());
    getStructWriter((byte) idx()).start();
  }

  @Override
  public void end() {
    byte typeId = data.getTypeId(idx());
    getStructWriter(typeId).end();
  }

  @Override
  public void startList() {
    byte typeId = data.getTypeId(idx());
    getListWriter(typeId).startList();
  }

  @Override
  public void endList() {
    byte typeId = data.getTypeId(idx());
    getListWriter(typeId).endList();
  }

  private StructWriter getStructWriter(byte typeId) {
    StructWriter structWriter = (StructWriter) writers[typeId];
    if (structWriter == null) {
      structWriter = nullableStructWriterFactory.build((StructVector) data.getVectorByType(typeId));
      writers[typeId] = structWriter;
    }
    return structWriter;
  }

  public StructWriter asStruct(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getStructWriter(typeId);
  }

  private ListWriter getListWriter(byte typeId) {
    ListWriter listWriter = (ListWriter) writers[typeId];
    if (listWriter == null) {
      listWriter = new UnionListWriter((ListVector) data.getVectorByType(typeId), nullableStructWriterFactory);
      writers[typeId] = listWriter;
    }
    return listWriter;
  }

  public ListWriter asList(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getListWriter(typeId);
  }

  private MapWriter getMapWriter(byte typeId) {
    MapWriter mapWriter = (MapWriter) writers[typeId];
    if (mapWriter == null) {
      mapWriter = new UnionMapWriter((MapVector) data.getVectorByType(typeId));
      writers[typeId] = mapWriter;
    }
    return mapWriter;
  }

  public MapWriter asMap(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getMapWriter(typeId);
  }

  BaseWriter getWriter(byte typeId) {
    MinorType minorType = data.getVectorByType(typeId).getMinorType();
    switch (minorType) {
      case STRUCT:
        return getStructWriter(typeId);
      case LIST:
        return getListWriter(typeId);
      case MAP:
        return getMapWriter(typeId);
      case TINYINT:
      return getTinyIntWriter(typeId);
      case UINT1:
      return getUInt1Writer(typeId);
      case UINT2:
      return getUInt2Writer(typeId);
      case SMALLINT:
      return getSmallIntWriter(typeId);
      case FLOAT2:
      return getFloat2Writer(typeId);
      case INT:
      return getIntWriter(typeId);
      case UINT4:
      return getUInt4Writer(typeId);
      case FLOAT4:
      return getFloat4Writer(typeId);
      case DATEDAY:
      return getDateDayWriter(typeId);
      case INTERVALYEAR:
      return getIntervalYearWriter(typeId);
      case TIMESEC:
      return getTimeSecWriter(typeId);
      case TIMEMILLI:
      return getTimeMilliWriter(typeId);
      case BIGINT:
      return getBigIntWriter(typeId);
      case UINT8:
      return getUInt8Writer(typeId);
      case FLOAT8:
      return getFloat8Writer(typeId);
      case DATEMILLI:
      return getDateMilliWriter(typeId);
      case TIMESTAMPSEC:
      return getTimeStampSecWriter(typeId);
      case TIMESTAMPMILLI:
      return getTimeStampMilliWriter(typeId);
      case TIMESTAMPMICRO:
      return getTimeStampMicroWriter(typeId);
      case TIMESTAMPNANO:
      return getTimeStampNanoWriter(typeId);
      case TIMEMICRO:
      return getTimeMicroWriter(typeId);
      case TIMENANO:
      return getTimeNanoWriter(typeId);
      case INTERVALDAY:
      return getIntervalDayWriter(typeId);
      case INTERVALMONTHDAYNANO:
      return getIntervalMonthDayNanoWriter(typeId);
      case DECIMAL256:
      return getDecimal256Writer(typeId);
      case DECIMAL:
      return getDecimalWriter(typeId);
      case VARBINARY:
      return getVarBinaryWriter(typeId);
      case VARCHAR:
      return getVarCharWriter(typeId);
      case LARGEVARCHAR:
      return getLargeVarCharWriter(typeId);
      case LARGEVARBINARY:
      return getLargeVarBinaryWriter(typeId);
      case BIT:
      return getBitWriter(typeId);
      default:
        throw new UnsupportedOperationException("Unknown type: " + minorType);
    }
  }

  private TinyIntWriter getTinyIntWriter(byte typeId) {
    TinyIntWriter writer = (TinyIntWriter) writers[typeId];
    if (writer == null) {
      writer = new TinyIntWriterImpl((TinyIntVector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public TinyIntWriter asTinyInt(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getTinyIntWriter(typeId);
  }

  @Override
  public void write(TinyIntHolder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeTinyInt(byte value, byte typeId) {
    data.setTypeId(idx(), typeId);
    getTinyIntWriter(typeId).setPosition(data.getOffset(idx()));
    getTinyIntWriter(typeId).writeTinyInt(value);
  }

  private UInt1Writer getUInt1Writer(byte typeId) {
    UInt1Writer writer = (UInt1Writer) writers[typeId];
    if (writer == null) {
      writer = new UInt1WriterImpl((UInt1Vector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public UInt1Writer asUInt1(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getUInt1Writer(typeId);
  }

  @Override
  public void write(UInt1Holder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeUInt1(byte value, byte typeId) {
    data.setTypeId(idx(), typeId);
    getUInt1Writer(typeId).setPosition(data.getOffset(idx()));
    getUInt1Writer(typeId).writeUInt1(value);
  }

  private UInt2Writer getUInt2Writer(byte typeId) {
    UInt2Writer writer = (UInt2Writer) writers[typeId];
    if (writer == null) {
      writer = new UInt2WriterImpl((UInt2Vector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public UInt2Writer asUInt2(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getUInt2Writer(typeId);
  }

  @Override
  public void write(UInt2Holder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeUInt2(char value, byte typeId) {
    data.setTypeId(idx(), typeId);
    getUInt2Writer(typeId).setPosition(data.getOffset(idx()));
    getUInt2Writer(typeId).writeUInt2(value);
  }

  private SmallIntWriter getSmallIntWriter(byte typeId) {
    SmallIntWriter writer = (SmallIntWriter) writers[typeId];
    if (writer == null) {
      writer = new SmallIntWriterImpl((SmallIntVector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public SmallIntWriter asSmallInt(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getSmallIntWriter(typeId);
  }

  @Override
  public void write(SmallIntHolder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeSmallInt(short value, byte typeId) {
    data.setTypeId(idx(), typeId);
    getSmallIntWriter(typeId).setPosition(data.getOffset(idx()));
    getSmallIntWriter(typeId).writeSmallInt(value);
  }

  private Float2Writer getFloat2Writer(byte typeId) {
    Float2Writer writer = (Float2Writer) writers[typeId];
    if (writer == null) {
      writer = new Float2WriterImpl((Float2Vector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public Float2Writer asFloat2(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getFloat2Writer(typeId);
  }

  @Override
  public void write(Float2Holder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeFloat2(short value, byte typeId) {
    data.setTypeId(idx(), typeId);
    getFloat2Writer(typeId).setPosition(data.getOffset(idx()));
    getFloat2Writer(typeId).writeFloat2(value);
  }

  private IntWriter getIntWriter(byte typeId) {
    IntWriter writer = (IntWriter) writers[typeId];
    if (writer == null) {
      writer = new IntWriterImpl((IntVector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public IntWriter asInt(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getIntWriter(typeId);
  }

  @Override
  public void write(IntHolder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeInt(int value, byte typeId) {
    data.setTypeId(idx(), typeId);
    getIntWriter(typeId).setPosition(data.getOffset(idx()));
    getIntWriter(typeId).writeInt(value);
  }

  private UInt4Writer getUInt4Writer(byte typeId) {
    UInt4Writer writer = (UInt4Writer) writers[typeId];
    if (writer == null) {
      writer = new UInt4WriterImpl((UInt4Vector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public UInt4Writer asUInt4(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getUInt4Writer(typeId);
  }

  @Override
  public void write(UInt4Holder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeUInt4(int value, byte typeId) {
    data.setTypeId(idx(), typeId);
    getUInt4Writer(typeId).setPosition(data.getOffset(idx()));
    getUInt4Writer(typeId).writeUInt4(value);
  }

  private Float4Writer getFloat4Writer(byte typeId) {
    Float4Writer writer = (Float4Writer) writers[typeId];
    if (writer == null) {
      writer = new Float4WriterImpl((Float4Vector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public Float4Writer asFloat4(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getFloat4Writer(typeId);
  }

  @Override
  public void write(Float4Holder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeFloat4(float value, byte typeId) {
    data.setTypeId(idx(), typeId);
    getFloat4Writer(typeId).setPosition(data.getOffset(idx()));
    getFloat4Writer(typeId).writeFloat4(value);
  }

  private DateDayWriter getDateDayWriter(byte typeId) {
    DateDayWriter writer = (DateDayWriter) writers[typeId];
    if (writer == null) {
      writer = new DateDayWriterImpl((DateDayVector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public DateDayWriter asDateDay(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getDateDayWriter(typeId);
  }

  @Override
  public void write(DateDayHolder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeDateDay(int value, byte typeId) {
    data.setTypeId(idx(), typeId);
    getDateDayWriter(typeId).setPosition(data.getOffset(idx()));
    getDateDayWriter(typeId).writeDateDay(value);
  }

  private IntervalYearWriter getIntervalYearWriter(byte typeId) {
    IntervalYearWriter writer = (IntervalYearWriter) writers[typeId];
    if (writer == null) {
      writer = new IntervalYearWriterImpl((IntervalYearVector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public IntervalYearWriter asIntervalYear(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getIntervalYearWriter(typeId);
  }

  @Override
  public void write(IntervalYearHolder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeIntervalYear(int value, byte typeId) {
    data.setTypeId(idx(), typeId);
    getIntervalYearWriter(typeId).setPosition(data.getOffset(idx()));
    getIntervalYearWriter(typeId).writeIntervalYear(value);
  }

  private TimeSecWriter getTimeSecWriter(byte typeId) {
    TimeSecWriter writer = (TimeSecWriter) writers[typeId];
    if (writer == null) {
      writer = new TimeSecWriterImpl((TimeSecVector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public TimeSecWriter asTimeSec(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getTimeSecWriter(typeId);
  }

  @Override
  public void write(TimeSecHolder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeTimeSec(int value, byte typeId) {
    data.setTypeId(idx(), typeId);
    getTimeSecWriter(typeId).setPosition(data.getOffset(idx()));
    getTimeSecWriter(typeId).writeTimeSec(value);
  }

  private TimeMilliWriter getTimeMilliWriter(byte typeId) {
    TimeMilliWriter writer = (TimeMilliWriter) writers[typeId];
    if (writer == null) {
      writer = new TimeMilliWriterImpl((TimeMilliVector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public TimeMilliWriter asTimeMilli(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getTimeMilliWriter(typeId);
  }

  @Override
  public void write(TimeMilliHolder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeTimeMilli(int value, byte typeId) {
    data.setTypeId(idx(), typeId);
    getTimeMilliWriter(typeId).setPosition(data.getOffset(idx()));
    getTimeMilliWriter(typeId).writeTimeMilli(value);
  }

  private BigIntWriter getBigIntWriter(byte typeId) {
    BigIntWriter writer = (BigIntWriter) writers[typeId];
    if (writer == null) {
      writer = new BigIntWriterImpl((BigIntVector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public BigIntWriter asBigInt(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getBigIntWriter(typeId);
  }

  @Override
  public void write(BigIntHolder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeBigInt(long value, byte typeId) {
    data.setTypeId(idx(), typeId);
    getBigIntWriter(typeId).setPosition(data.getOffset(idx()));
    getBigIntWriter(typeId).writeBigInt(value);
  }

  private UInt8Writer getUInt8Writer(byte typeId) {
    UInt8Writer writer = (UInt8Writer) writers[typeId];
    if (writer == null) {
      writer = new UInt8WriterImpl((UInt8Vector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public UInt8Writer asUInt8(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getUInt8Writer(typeId);
  }

  @Override
  public void write(UInt8Holder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeUInt8(long value, byte typeId) {
    data.setTypeId(idx(), typeId);
    getUInt8Writer(typeId).setPosition(data.getOffset(idx()));
    getUInt8Writer(typeId).writeUInt8(value);
  }

  private Float8Writer getFloat8Writer(byte typeId) {
    Float8Writer writer = (Float8Writer) writers[typeId];
    if (writer == null) {
      writer = new Float8WriterImpl((Float8Vector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public Float8Writer asFloat8(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getFloat8Writer(typeId);
  }

  @Override
  public void write(Float8Holder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeFloat8(double value, byte typeId) {
    data.setTypeId(idx(), typeId);
    getFloat8Writer(typeId).setPosition(data.getOffset(idx()));
    getFloat8Writer(typeId).writeFloat8(value);
  }

  private DateMilliWriter getDateMilliWriter(byte typeId) {
    DateMilliWriter writer = (DateMilliWriter) writers[typeId];
    if (writer == null) {
      writer = new DateMilliWriterImpl((DateMilliVector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public DateMilliWriter asDateMilli(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getDateMilliWriter(typeId);
  }

  @Override
  public void write(DateMilliHolder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeDateMilli(long value, byte typeId) {
    data.setTypeId(idx(), typeId);
    getDateMilliWriter(typeId).setPosition(data.getOffset(idx()));
    getDateMilliWriter(typeId).writeDateMilli(value);
  }

  private TimeStampSecWriter getTimeStampSecWriter(byte typeId) {
    TimeStampSecWriter writer = (TimeStampSecWriter) writers[typeId];
    if (writer == null) {
      writer = new TimeStampSecWriterImpl((TimeStampSecVector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public TimeStampSecWriter asTimeStampSec(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getTimeStampSecWriter(typeId);
  }

  @Override
  public void write(TimeStampSecHolder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeTimeStampSec(long value, byte typeId) {
    data.setTypeId(idx(), typeId);
    getTimeStampSecWriter(typeId).setPosition(data.getOffset(idx()));
    getTimeStampSecWriter(typeId).writeTimeStampSec(value);
  }

  private TimeStampMilliWriter getTimeStampMilliWriter(byte typeId) {
    TimeStampMilliWriter writer = (TimeStampMilliWriter) writers[typeId];
    if (writer == null) {
      writer = new TimeStampMilliWriterImpl((TimeStampMilliVector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public TimeStampMilliWriter asTimeStampMilli(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getTimeStampMilliWriter(typeId);
  }

  @Override
  public void write(TimeStampMilliHolder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeTimeStampMilli(long value, byte typeId) {
    data.setTypeId(idx(), typeId);
    getTimeStampMilliWriter(typeId).setPosition(data.getOffset(idx()));
    getTimeStampMilliWriter(typeId).writeTimeStampMilli(value);
  }

  private TimeStampMicroWriter getTimeStampMicroWriter(byte typeId) {
    TimeStampMicroWriter writer = (TimeStampMicroWriter) writers[typeId];
    if (writer == null) {
      writer = new TimeStampMicroWriterImpl((TimeStampMicroVector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public TimeStampMicroWriter asTimeStampMicro(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getTimeStampMicroWriter(typeId);
  }

  @Override
  public void write(TimeStampMicroHolder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeTimeStampMicro(long value, byte typeId) {
    data.setTypeId(idx(), typeId);
    getTimeStampMicroWriter(typeId).setPosition(data.getOffset(idx()));
    getTimeStampMicroWriter(typeId).writeTimeStampMicro(value);
  }

  private TimeStampNanoWriter getTimeStampNanoWriter(byte typeId) {
    TimeStampNanoWriter writer = (TimeStampNanoWriter) writers[typeId];
    if (writer == null) {
      writer = new TimeStampNanoWriterImpl((TimeStampNanoVector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public TimeStampNanoWriter asTimeStampNano(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getTimeStampNanoWriter(typeId);
  }

  @Override
  public void write(TimeStampNanoHolder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeTimeStampNano(long value, byte typeId) {
    data.setTypeId(idx(), typeId);
    getTimeStampNanoWriter(typeId).setPosition(data.getOffset(idx()));
    getTimeStampNanoWriter(typeId).writeTimeStampNano(value);
  }

  private TimeMicroWriter getTimeMicroWriter(byte typeId) {
    TimeMicroWriter writer = (TimeMicroWriter) writers[typeId];
    if (writer == null) {
      writer = new TimeMicroWriterImpl((TimeMicroVector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public TimeMicroWriter asTimeMicro(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getTimeMicroWriter(typeId);
  }

  @Override
  public void write(TimeMicroHolder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeTimeMicro(long value, byte typeId) {
    data.setTypeId(idx(), typeId);
    getTimeMicroWriter(typeId).setPosition(data.getOffset(idx()));
    getTimeMicroWriter(typeId).writeTimeMicro(value);
  }

  private TimeNanoWriter getTimeNanoWriter(byte typeId) {
    TimeNanoWriter writer = (TimeNanoWriter) writers[typeId];
    if (writer == null) {
      writer = new TimeNanoWriterImpl((TimeNanoVector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public TimeNanoWriter asTimeNano(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getTimeNanoWriter(typeId);
  }

  @Override
  public void write(TimeNanoHolder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeTimeNano(long value, byte typeId) {
    data.setTypeId(idx(), typeId);
    getTimeNanoWriter(typeId).setPosition(data.getOffset(idx()));
    getTimeNanoWriter(typeId).writeTimeNano(value);
  }

  private IntervalDayWriter getIntervalDayWriter(byte typeId) {
    IntervalDayWriter writer = (IntervalDayWriter) writers[typeId];
    if (writer == null) {
      writer = new IntervalDayWriterImpl((IntervalDayVector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public IntervalDayWriter asIntervalDay(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getIntervalDayWriter(typeId);
  }

  @Override
  public void write(IntervalDayHolder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeIntervalDay(int days, int milliseconds, byte typeId) {
    data.setTypeId(idx(), typeId);
    getIntervalDayWriter(typeId).setPosition(data.getOffset(idx()));
    getIntervalDayWriter(typeId).writeIntervalDay(days, milliseconds);
  }

  private IntervalMonthDayNanoWriter getIntervalMonthDayNanoWriter(byte typeId) {
    IntervalMonthDayNanoWriter writer = (IntervalMonthDayNanoWriter) writers[typeId];
    if (writer == null) {
      writer = new IntervalMonthDayNanoWriterImpl((IntervalMonthDayNanoVector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public IntervalMonthDayNanoWriter asIntervalMonthDayNano(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getIntervalMonthDayNanoWriter(typeId);
  }

  @Override
  public void write(IntervalMonthDayNanoHolder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeIntervalMonthDayNano(int months, int days, long nanoseconds, byte typeId) {
    data.setTypeId(idx(), typeId);
    getIntervalMonthDayNanoWriter(typeId).setPosition(data.getOffset(idx()));
    getIntervalMonthDayNanoWriter(typeId).writeIntervalMonthDayNano(months, days, nanoseconds);
  }

  private Decimal256Writer getDecimal256Writer(byte typeId) {
    Decimal256Writer writer = (Decimal256Writer) writers[typeId];
    if (writer == null) {
      writer = new Decimal256WriterImpl((Decimal256Vector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public Decimal256Writer asDecimal256(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getDecimal256Writer(typeId);
  }

  @Override
  public void write(Decimal256Holder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeDecimal256(long start, ArrowBuf buffer, byte typeId, ArrowType arrowType) {
    data.setTypeId(idx(), typeId);
    getDecimal256Writer(typeId).setPosition(data.getOffset(idx()));
    getDecimal256Writer(typeId).writeDecimal256(start, buffer, arrowType);
  }

  private DecimalWriter getDecimalWriter(byte typeId) {
    DecimalWriter writer = (DecimalWriter) writers[typeId];
    if (writer == null) {
      writer = new DecimalWriterImpl((DecimalVector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public DecimalWriter asDecimal(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getDecimalWriter(typeId);
  }

  @Override
  public void write(DecimalHolder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeDecimal(long start, ArrowBuf buffer, byte typeId, ArrowType arrowType) {
    data.setTypeId(idx(), typeId);
    getDecimalWriter(typeId).setPosition(data.getOffset(idx()));
    getDecimalWriter(typeId).writeDecimal(start, buffer, arrowType);
  }

  private VarBinaryWriter getVarBinaryWriter(byte typeId) {
    VarBinaryWriter writer = (VarBinaryWriter) writers[typeId];
    if (writer == null) {
      writer = new VarBinaryWriterImpl((VarBinaryVector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public VarBinaryWriter asVarBinary(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getVarBinaryWriter(typeId);
  }

  @Override
  public void write(VarBinaryHolder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeVarBinary(int start, int end, ArrowBuf buffer, byte typeId) {
    data.setTypeId(idx(), typeId);
    getVarBinaryWriter(typeId).setPosition(data.getOffset(idx()));
    getVarBinaryWriter(typeId).writeVarBinary(start, end, buffer);
  }

  private VarCharWriter getVarCharWriter(byte typeId) {
    VarCharWriter writer = (VarCharWriter) writers[typeId];
    if (writer == null) {
      writer = new VarCharWriterImpl((VarCharVector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public VarCharWriter asVarChar(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getVarCharWriter(typeId);
  }

  @Override
  public void write(VarCharHolder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeVarChar(int start, int end, ArrowBuf buffer, byte typeId) {
    data.setTypeId(idx(), typeId);
    getVarCharWriter(typeId).setPosition(data.getOffset(idx()));
    getVarCharWriter(typeId).writeVarChar(start, end, buffer);
  }

  private LargeVarCharWriter getLargeVarCharWriter(byte typeId) {
    LargeVarCharWriter writer = (LargeVarCharWriter) writers[typeId];
    if (writer == null) {
      writer = new LargeVarCharWriterImpl((LargeVarCharVector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public LargeVarCharWriter asLargeVarChar(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getLargeVarCharWriter(typeId);
  }

  @Override
  public void write(LargeVarCharHolder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeLargeVarChar(long start, long end, ArrowBuf buffer, byte typeId) {
    data.setTypeId(idx(), typeId);
    getLargeVarCharWriter(typeId).setPosition(data.getOffset(idx()));
    getLargeVarCharWriter(typeId).writeLargeVarChar(start, end, buffer);
  }

  private LargeVarBinaryWriter getLargeVarBinaryWriter(byte typeId) {
    LargeVarBinaryWriter writer = (LargeVarBinaryWriter) writers[typeId];
    if (writer == null) {
      writer = new LargeVarBinaryWriterImpl((LargeVarBinaryVector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public LargeVarBinaryWriter asLargeVarBinary(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getLargeVarBinaryWriter(typeId);
  }

  @Override
  public void write(LargeVarBinaryHolder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeLargeVarBinary(long start, long end, ArrowBuf buffer, byte typeId) {
    data.setTypeId(idx(), typeId);
    getLargeVarBinaryWriter(typeId).setPosition(data.getOffset(idx()));
    getLargeVarBinaryWriter(typeId).writeLargeVarBinary(start, end, buffer);
  }

  private BitWriter getBitWriter(byte typeId) {
    BitWriter writer = (BitWriter) writers[typeId];
    if (writer == null) {
      writer = new BitWriterImpl((BitVector) data.getVectorByType(typeId));
      writers[typeId] = writer;
    }
    return writer;
  }

  public BitWriter asBit(byte typeId) {
    data.setTypeId(idx(), typeId);
    return getBitWriter(typeId);
  }

  @Override
  public void write(BitHolder holder) {
    throw new UnsupportedOperationException();
  }

  public void writeBit(int value, byte typeId) {
    data.setTypeId(idx(), typeId);
    getBitWriter(typeId).setPosition(data.getOffset(idx()));
    getBitWriter(typeId).writeBit(value);
  }

  public void writeNull() {
  }

  @Override
  public StructWriter struct() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).struct();
  }

  @Override
  public ListWriter list() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).list();
  }

  @Override
  public ListWriter list(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).list(name);
  }

  @Override
  public MapWriter map() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getMapWriter(typeId).map();
  }

  @Override
  public MapWriter map(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).map(name);
  }

  @Override
  public MapWriter map(String name, boolean keysSorted) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).map(name, keysSorted);
  }

  @Override
  public StructWriter struct(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).struct(name);
  }

  @Override
  public TinyIntWriter tinyInt(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).tinyInt(name);
  }

  @Override
  public TinyIntWriter tinyInt() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).tinyInt();
  }
  @Override
  public UInt1Writer uInt1(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).uInt1(name);
  }

  @Override
  public UInt1Writer uInt1() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).uInt1();
  }
  @Override
  public UInt2Writer uInt2(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).uInt2(name);
  }

  @Override
  public UInt2Writer uInt2() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).uInt2();
  }
  @Override
  public SmallIntWriter smallInt(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).smallInt(name);
  }

  @Override
  public SmallIntWriter smallInt() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).smallInt();
  }
  @Override
  public Float2Writer float2(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).float2(name);
  }

  @Override
  public Float2Writer float2() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).float2();
  }
  @Override
  public IntWriter integer(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).integer(name);
  }

  @Override
  public IntWriter integer() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).integer();
  }
  @Override
  public UInt4Writer uInt4(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).uInt4(name);
  }

  @Override
  public UInt4Writer uInt4() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).uInt4();
  }
  @Override
  public Float4Writer float4(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).float4(name);
  }

  @Override
  public Float4Writer float4() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).float4();
  }
  @Override
  public DateDayWriter dateDay(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).dateDay(name);
  }

  @Override
  public DateDayWriter dateDay() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).dateDay();
  }
  @Override
  public IntervalYearWriter intervalYear(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).intervalYear(name);
  }

  @Override
  public IntervalYearWriter intervalYear() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).intervalYear();
  }
  @Override
  public TimeSecWriter timeSec(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).timeSec(name);
  }

  @Override
  public TimeSecWriter timeSec() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).timeSec();
  }
  @Override
  public TimeMilliWriter timeMilli(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).timeMilli(name);
  }

  @Override
  public TimeMilliWriter timeMilli() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).timeMilli();
  }
  @Override
  public BigIntWriter bigInt(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).bigInt(name);
  }

  @Override
  public BigIntWriter bigInt() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).bigInt();
  }
  @Override
  public UInt8Writer uInt8(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).uInt8(name);
  }

  @Override
  public UInt8Writer uInt8() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).uInt8();
  }
  @Override
  public Float8Writer float8(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).float8(name);
  }

  @Override
  public Float8Writer float8() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).float8();
  }
  @Override
  public DateMilliWriter dateMilli(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).dateMilli(name);
  }

  @Override
  public DateMilliWriter dateMilli() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).dateMilli();
  }
  @Override
  public TimeStampSecWriter timeStampSec(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).timeStampSec(name);
  }

  @Override
  public TimeStampSecWriter timeStampSec() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).timeStampSec();
  }
  @Override
  public TimeStampMilliWriter timeStampMilli(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).timeStampMilli(name);
  }

  @Override
  public TimeStampMilliWriter timeStampMilli() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).timeStampMilli();
  }
  @Override
  public TimeStampMicroWriter timeStampMicro(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).timeStampMicro(name);
  }

  @Override
  public TimeStampMicroWriter timeStampMicro() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).timeStampMicro();
  }
  @Override
  public TimeStampNanoWriter timeStampNano(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).timeStampNano(name);
  }

  @Override
  public TimeStampNanoWriter timeStampNano() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).timeStampNano();
  }
  @Override
  public TimeMicroWriter timeMicro(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).timeMicro(name);
  }

  @Override
  public TimeMicroWriter timeMicro() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).timeMicro();
  }
  @Override
  public TimeNanoWriter timeNano(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).timeNano(name);
  }

  @Override
  public TimeNanoWriter timeNano() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).timeNano();
  }
  @Override
  public IntervalDayWriter intervalDay(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).intervalDay(name);
  }

  @Override
  public IntervalDayWriter intervalDay() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).intervalDay();
  }
  @Override
  public IntervalMonthDayNanoWriter intervalMonthDayNano(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).intervalMonthDayNano(name);
  }

  @Override
  public IntervalMonthDayNanoWriter intervalMonthDayNano() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).intervalMonthDayNano();
  }
  @Override
  public Decimal256Writer decimal256(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).decimal256(name);
  }

  @Override
  public Decimal256Writer decimal256() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).decimal256();
  }
  public Decimal256Writer decimal256(String name, int scale, int precision) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).decimal256(name, scale, precision);
  }
  @Override
  public DecimalWriter decimal(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).decimal(name);
  }

  @Override
  public DecimalWriter decimal() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).decimal();
  }
  public DecimalWriter decimal(String name, int scale, int precision) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).decimal(name, scale, precision);
  }
  @Override
  public VarBinaryWriter varBinary(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).varBinary(name);
  }

  @Override
  public VarBinaryWriter varBinary() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).varBinary();
  }
  @Override
  public VarCharWriter varChar(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).varChar(name);
  }

  @Override
  public VarCharWriter varChar() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).varChar();
  }
  @Override
  public LargeVarCharWriter largeVarChar(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).largeVarChar(name);
  }

  @Override
  public LargeVarCharWriter largeVarChar() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).largeVarChar();
  }
  @Override
  public LargeVarBinaryWriter largeVarBinary(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).largeVarBinary(name);
  }

  @Override
  public LargeVarBinaryWriter largeVarBinary() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).largeVarBinary();
  }
  @Override
  public BitWriter bit(String name) {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getStructWriter(typeId).setPosition(data.getOffset(idx()));
    return getStructWriter(typeId).bit(name);
  }

  @Override
  public BitWriter bit() {
    byte typeId = data.getTypeId(idx());
    data.setTypeId(idx(), typeId);
    getListWriter(typeId).setPosition(data.getOffset(idx()));
    return getListWriter(typeId).bit();
  }

  @Override
  public void allocate() {
    data.allocateNew();
  }

  @Override
  public void clear() {
    data.clear();
  }

  @Override
  public void close() throws Exception {
    data.close();
  }

  @Override
  public Field getField() {
    return data.getField();
  }

  @Override
  public int getValueCapacity() {
    return data.getValueCapacity();
  }
}
