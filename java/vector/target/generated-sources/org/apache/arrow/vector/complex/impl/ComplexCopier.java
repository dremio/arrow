

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
 * This class is generated using freemarker and the ComplexCopier.java template.
 */
@SuppressWarnings("unused")
public class ComplexCopier {

  /**
   * Do a deep copy of the value in input into output
   * @param input field to read from
   * @param output field to write to
   */
  public static void copy(FieldReader input, FieldWriter output) {
    writeValue(input, output);
  }

  private static void writeValue(FieldReader reader, FieldWriter writer) {
    final MinorType mt = reader.getMinorType();

      switch (mt) {

      case LIST:
      case LARGELIST:
      case FIXED_SIZE_LIST:
        if (reader.isSet()) {
          writer.startList();
          while (reader.next()) {
            FieldReader childReader = reader.reader();
            FieldWriter childWriter = getListWriterForReader(childReader, writer);
            if (childReader.isSet()) {
              writeValue(childReader, childWriter);
            } else {
              childWriter.writeNull();
            }
          }
          writer.endList();
        } else {
          writer.writeNull();
        }
        break;
      case MAP:
        if (reader.isSet()) {
          UnionMapReader mapReader = (UnionMapReader) reader;
          writer.startMap();
          while (mapReader.next()) {
            FieldReader structReader = reader.reader();
            if (structReader.isSet()) {
              writer.startEntry();
              writeValue(mapReader.key(), getMapWriterForReader(mapReader.key(), writer.key()));
              writeValue(mapReader.value(), getMapWriterForReader(mapReader.value(), writer.value()));
              writer.endEntry();
            } else {
              writer.writeNull();
            }
          }
          writer.endMap();
        } else {
          writer.writeNull();
        }
        break;
      case STRUCT:
        if (reader.isSet()) {
          writer.start();
          for(String name : reader){
            FieldReader childReader = reader.reader(name);
            if (childReader.getMinorType() != Types.MinorType.NULL) {
              FieldWriter childWriter = getStructWriterForReader(childReader, writer, name);
              if (childReader.isSet()) {
                writeValue(childReader, childWriter);
              } else {
                childWriter.writeNull();
              }
            }
          }
          writer.end();
        } else {
          writer.writeNull();
        }
        break;


      case TINYINT:
        if (reader.isSet()) {
          NullableTinyIntHolder tinyIntHolder = new NullableTinyIntHolder();
          reader.read(tinyIntHolder);
          if (tinyIntHolder.isSet == 1) {
            writer.writeTinyInt(tinyIntHolder.value);
          }
        } else {
          writer.writeNull();
        }
        break;



      case UINT1:
        if (reader.isSet()) {
          NullableUInt1Holder uInt1Holder = new NullableUInt1Holder();
          reader.read(uInt1Holder);
          if (uInt1Holder.isSet == 1) {
            writer.writeUInt1(uInt1Holder.value);
          }
        } else {
          writer.writeNull();
        }
        break;



      case UINT2:
        if (reader.isSet()) {
          NullableUInt2Holder uInt2Holder = new NullableUInt2Holder();
          reader.read(uInt2Holder);
          if (uInt2Holder.isSet == 1) {
            writer.writeUInt2(uInt2Holder.value);
          }
        } else {
          writer.writeNull();
        }
        break;



      case SMALLINT:
        if (reader.isSet()) {
          NullableSmallIntHolder smallIntHolder = new NullableSmallIntHolder();
          reader.read(smallIntHolder);
          if (smallIntHolder.isSet == 1) {
            writer.writeSmallInt(smallIntHolder.value);
          }
        } else {
          writer.writeNull();
        }
        break;



      case FLOAT2:
        if (reader.isSet()) {
          NullableFloat2Holder float2Holder = new NullableFloat2Holder();
          reader.read(float2Holder);
          if (float2Holder.isSet == 1) {
            writer.writeFloat2(float2Holder.value);
          }
        } else {
          writer.writeNull();
        }
        break;



      case INT:
        if (reader.isSet()) {
          NullableIntHolder intHolder = new NullableIntHolder();
          reader.read(intHolder);
          if (intHolder.isSet == 1) {
            writer.writeInt(intHolder.value);
          }
        } else {
          writer.writeNull();
        }
        break;



      case UINT4:
        if (reader.isSet()) {
          NullableUInt4Holder uInt4Holder = new NullableUInt4Holder();
          reader.read(uInt4Holder);
          if (uInt4Holder.isSet == 1) {
            writer.writeUInt4(uInt4Holder.value);
          }
        } else {
          writer.writeNull();
        }
        break;



      case FLOAT4:
        if (reader.isSet()) {
          NullableFloat4Holder float4Holder = new NullableFloat4Holder();
          reader.read(float4Holder);
          if (float4Holder.isSet == 1) {
            writer.writeFloat4(float4Holder.value);
          }
        } else {
          writer.writeNull();
        }
        break;



      case DATEDAY:
        if (reader.isSet()) {
          NullableDateDayHolder dateDayHolder = new NullableDateDayHolder();
          reader.read(dateDayHolder);
          if (dateDayHolder.isSet == 1) {
            writer.writeDateDay(dateDayHolder.value);
          }
        } else {
          writer.writeNull();
        }
        break;



      case INTERVALYEAR:
        if (reader.isSet()) {
          NullableIntervalYearHolder intervalYearHolder = new NullableIntervalYearHolder();
          reader.read(intervalYearHolder);
          if (intervalYearHolder.isSet == 1) {
            writer.writeIntervalYear(intervalYearHolder.value);
          }
        } else {
          writer.writeNull();
        }
        break;



      case TIMESEC:
        if (reader.isSet()) {
          NullableTimeSecHolder timeSecHolder = new NullableTimeSecHolder();
          reader.read(timeSecHolder);
          if (timeSecHolder.isSet == 1) {
            writer.writeTimeSec(timeSecHolder.value);
          }
        } else {
          writer.writeNull();
        }
        break;



      case TIMEMILLI:
        if (reader.isSet()) {
          NullableTimeMilliHolder timeMilliHolder = new NullableTimeMilliHolder();
          reader.read(timeMilliHolder);
          if (timeMilliHolder.isSet == 1) {
            writer.writeTimeMilli(timeMilliHolder.value);
          }
        } else {
          writer.writeNull();
        }
        break;



      case BIGINT:
        if (reader.isSet()) {
          NullableBigIntHolder bigIntHolder = new NullableBigIntHolder();
          reader.read(bigIntHolder);
          if (bigIntHolder.isSet == 1) {
            writer.writeBigInt(bigIntHolder.value);
          }
        } else {
          writer.writeNull();
        }
        break;



      case UINT8:
        if (reader.isSet()) {
          NullableUInt8Holder uInt8Holder = new NullableUInt8Holder();
          reader.read(uInt8Holder);
          if (uInt8Holder.isSet == 1) {
            writer.writeUInt8(uInt8Holder.value);
          }
        } else {
          writer.writeNull();
        }
        break;



      case FLOAT8:
        if (reader.isSet()) {
          NullableFloat8Holder float8Holder = new NullableFloat8Holder();
          reader.read(float8Holder);
          if (float8Holder.isSet == 1) {
            writer.writeFloat8(float8Holder.value);
          }
        } else {
          writer.writeNull();
        }
        break;



      case DATEMILLI:
        if (reader.isSet()) {
          NullableDateMilliHolder dateMilliHolder = new NullableDateMilliHolder();
          reader.read(dateMilliHolder);
          if (dateMilliHolder.isSet == 1) {
            writer.writeDateMilli(dateMilliHolder.value);
          }
        } else {
          writer.writeNull();
        }
        break;




      case TIMESTAMPSEC:
        if (reader.isSet()) {
          NullableTimeStampSecHolder timeStampSecHolder = new NullableTimeStampSecHolder();
          reader.read(timeStampSecHolder);
          if (timeStampSecHolder.isSet == 1) {
            writer.writeTimeStampSec(timeStampSecHolder.value);
          }
        } else {
          writer.writeNull();
        }
        break;



      case TIMESTAMPMILLI:
        if (reader.isSet()) {
          NullableTimeStampMilliHolder timeStampMilliHolder = new NullableTimeStampMilliHolder();
          reader.read(timeStampMilliHolder);
          if (timeStampMilliHolder.isSet == 1) {
            writer.writeTimeStampMilli(timeStampMilliHolder.value);
          }
        } else {
          writer.writeNull();
        }
        break;



      case TIMESTAMPMICRO:
        if (reader.isSet()) {
          NullableTimeStampMicroHolder timeStampMicroHolder = new NullableTimeStampMicroHolder();
          reader.read(timeStampMicroHolder);
          if (timeStampMicroHolder.isSet == 1) {
            writer.writeTimeStampMicro(timeStampMicroHolder.value);
          }
        } else {
          writer.writeNull();
        }
        break;



      case TIMESTAMPNANO:
        if (reader.isSet()) {
          NullableTimeStampNanoHolder timeStampNanoHolder = new NullableTimeStampNanoHolder();
          reader.read(timeStampNanoHolder);
          if (timeStampNanoHolder.isSet == 1) {
            writer.writeTimeStampNano(timeStampNanoHolder.value);
          }
        } else {
          writer.writeNull();
        }
        break;







      case TIMEMICRO:
        if (reader.isSet()) {
          NullableTimeMicroHolder timeMicroHolder = new NullableTimeMicroHolder();
          reader.read(timeMicroHolder);
          if (timeMicroHolder.isSet == 1) {
            writer.writeTimeMicro(timeMicroHolder.value);
          }
        } else {
          writer.writeNull();
        }
        break;



      case TIMENANO:
        if (reader.isSet()) {
          NullableTimeNanoHolder timeNanoHolder = new NullableTimeNanoHolder();
          reader.read(timeNanoHolder);
          if (timeNanoHolder.isSet == 1) {
            writer.writeTimeNano(timeNanoHolder.value);
          }
        } else {
          writer.writeNull();
        }
        break;



      case INTERVALDAY:
        if (reader.isSet()) {
          NullableIntervalDayHolder intervalDayHolder = new NullableIntervalDayHolder();
          reader.read(intervalDayHolder);
          if (intervalDayHolder.isSet == 1) {
            writer.writeIntervalDay(intervalDayHolder.days, intervalDayHolder.milliseconds);
          }
        } else {
          writer.writeNull();
        }
        break;



      case INTERVALMONTHDAYNANO:
        if (reader.isSet()) {
          NullableIntervalMonthDayNanoHolder intervalMonthDayNanoHolder = new NullableIntervalMonthDayNanoHolder();
          reader.read(intervalMonthDayNanoHolder);
          if (intervalMonthDayNanoHolder.isSet == 1) {
            writer.writeIntervalMonthDayNano(intervalMonthDayNanoHolder.months, intervalMonthDayNanoHolder.days, intervalMonthDayNanoHolder.nanoseconds);
          }
        } else {
          writer.writeNull();
        }
        break;



      case DECIMAL256:
        if (reader.isSet()) {
          NullableDecimal256Holder decimal256Holder = new NullableDecimal256Holder();
          reader.read(decimal256Holder);
          if (decimal256Holder.isSet == 1) {
            writer.writeDecimal256(decimal256Holder.start, decimal256Holder.buffer, new ArrowType.Decimal(decimal256Holder.precision, decimal256Holder.scale, Decimal256Holder.WIDTH * 8));
          }
        } else {
          writer.writeNull();
        }
        break;



      case DECIMAL:
        if (reader.isSet()) {
          NullableDecimalHolder decimalHolder = new NullableDecimalHolder();
          reader.read(decimalHolder);
          if (decimalHolder.isSet == 1) {
            writer.writeDecimal(decimalHolder.start, decimalHolder.buffer, new ArrowType.Decimal(decimalHolder.precision, decimalHolder.scale, DecimalHolder.WIDTH * 8));
          }
        } else {
          writer.writeNull();
        }
        break;




      case VARBINARY:
        if (reader.isSet()) {
          NullableVarBinaryHolder varBinaryHolder = new NullableVarBinaryHolder();
          reader.read(varBinaryHolder);
          if (varBinaryHolder.isSet == 1) {
            writer.writeVarBinary(varBinaryHolder.start, varBinaryHolder.end, varBinaryHolder.buffer);
          }
        } else {
          writer.writeNull();
        }
        break;



      case VARCHAR:
        if (reader.isSet()) {
          NullableVarCharHolder varCharHolder = new NullableVarCharHolder();
          reader.read(varCharHolder);
          if (varCharHolder.isSet == 1) {
            writer.writeVarChar(varCharHolder.start, varCharHolder.end, varCharHolder.buffer);
          }
        } else {
          writer.writeNull();
        }
        break;



      case LARGEVARCHAR:
        if (reader.isSet()) {
          NullableLargeVarCharHolder largeVarCharHolder = new NullableLargeVarCharHolder();
          reader.read(largeVarCharHolder);
          if (largeVarCharHolder.isSet == 1) {
            writer.writeLargeVarChar(largeVarCharHolder.start, largeVarCharHolder.end, largeVarCharHolder.buffer);
          }
        } else {
          writer.writeNull();
        }
        break;



      case LARGEVARBINARY:
        if (reader.isSet()) {
          NullableLargeVarBinaryHolder largeVarBinaryHolder = new NullableLargeVarBinaryHolder();
          reader.read(largeVarBinaryHolder);
          if (largeVarBinaryHolder.isSet == 1) {
            writer.writeLargeVarBinary(largeVarBinaryHolder.start, largeVarBinaryHolder.end, largeVarBinaryHolder.buffer);
          }
        } else {
          writer.writeNull();
        }
        break;



      case BIT:
        if (reader.isSet()) {
          NullableBitHolder bitHolder = new NullableBitHolder();
          reader.read(bitHolder);
          if (bitHolder.isSet == 1) {
            writer.writeBit(bitHolder.value);
          }
        } else {
          writer.writeNull();
        }
        break;

      }
 }

  private static FieldWriter getStructWriterForReader(FieldReader reader, StructWriter writer, String name) {
    switch (reader.getMinorType()) {
    case TINYINT:
      return (FieldWriter) writer.tinyInt(name);
    
    case UINT1:
      return (FieldWriter) writer.uInt1(name);
    
    case UINT2:
      return (FieldWriter) writer.uInt2(name);
    
    case SMALLINT:
      return (FieldWriter) writer.smallInt(name);
    
    case FLOAT2:
      return (FieldWriter) writer.float2(name);
    
    case INT:
      return (FieldWriter) writer.integer(name);
    
    case UINT4:
      return (FieldWriter) writer.uInt4(name);
    
    case FLOAT4:
      return (FieldWriter) writer.float4(name);
    
    case DATEDAY:
      return (FieldWriter) writer.dateDay(name);
    
    case INTERVALYEAR:
      return (FieldWriter) writer.intervalYear(name);
    
    case TIMESEC:
      return (FieldWriter) writer.timeSec(name);
    
    case TIMEMILLI:
      return (FieldWriter) writer.timeMilli(name);
    
    case BIGINT:
      return (FieldWriter) writer.bigInt(name);
    
    case UINT8:
      return (FieldWriter) writer.uInt8(name);
    
    case FLOAT8:
      return (FieldWriter) writer.float8(name);
    
    case DATEMILLI:
      return (FieldWriter) writer.dateMilli(name);
    
    
    case TIMESTAMPSEC:
      return (FieldWriter) writer.timeStampSec(name);
    
    case TIMESTAMPMILLI:
      return (FieldWriter) writer.timeStampMilli(name);
    
    case TIMESTAMPMICRO:
      return (FieldWriter) writer.timeStampMicro(name);
    
    case TIMESTAMPNANO:
      return (FieldWriter) writer.timeStampNano(name);
    
    
    
    
    
    case TIMEMICRO:
      return (FieldWriter) writer.timeMicro(name);
    
    case TIMENANO:
      return (FieldWriter) writer.timeNano(name);
    
    case INTERVALDAY:
      return (FieldWriter) writer.intervalDay(name);
    
    case INTERVALMONTHDAYNANO:
      return (FieldWriter) writer.intervalMonthDayNano(name);
    
    case DECIMAL256:
      if (reader.getField().getType() instanceof ArrowType.Decimal) {
        ArrowType.Decimal type = (ArrowType.Decimal) reader.getField().getType();
        return (FieldWriter) writer.decimal256(name, type.getScale(), type.getPrecision());
      } else {
        return (FieldWriter) writer.decimal256(name);
      }
    
    case DECIMAL:
      if (reader.getField().getType() instanceof ArrowType.Decimal) {
        ArrowType.Decimal type = (ArrowType.Decimal) reader.getField().getType();
        return (FieldWriter) writer.decimal(name, type.getScale(), type.getPrecision());
      } else {
        return (FieldWriter) writer.decimal(name);
      }
    
    
    case VARBINARY:
      return (FieldWriter) writer.varBinary(name);
    
    case VARCHAR:
      return (FieldWriter) writer.varChar(name);
    
    case LARGEVARCHAR:
      return (FieldWriter) writer.largeVarChar(name);
    
    case LARGEVARBINARY:
      return (FieldWriter) writer.largeVarBinary(name);
    
    case BIT:
      return (FieldWriter) writer.bit(name);
    
    case STRUCT:
      return (FieldWriter) writer.struct(name);
    case FIXED_SIZE_LIST:
    case LIST:
      return (FieldWriter) writer.list(name);
    case MAP:
      return (FieldWriter) writer.map(name);
    default:
      throw new UnsupportedOperationException(reader.getMinorType().toString());
    }
  }

  private static FieldWriter getListWriterForReader(FieldReader reader, ListWriter writer) {
    switch (reader.getMinorType()) {
    case TINYINT:
    return (FieldWriter) writer.tinyInt();
    case UINT1:
    return (FieldWriter) writer.uInt1();
    case UINT2:
    return (FieldWriter) writer.uInt2();
    case SMALLINT:
    return (FieldWriter) writer.smallInt();
    case FLOAT2:
    return (FieldWriter) writer.float2();
    case INT:
    return (FieldWriter) writer.integer();
    case UINT4:
    return (FieldWriter) writer.uInt4();
    case FLOAT4:
    return (FieldWriter) writer.float4();
    case DATEDAY:
    return (FieldWriter) writer.dateDay();
    case INTERVALYEAR:
    return (FieldWriter) writer.intervalYear();
    case TIMESEC:
    return (FieldWriter) writer.timeSec();
    case TIMEMILLI:
    return (FieldWriter) writer.timeMilli();
    case BIGINT:
    return (FieldWriter) writer.bigInt();
    case UINT8:
    return (FieldWriter) writer.uInt8();
    case FLOAT8:
    return (FieldWriter) writer.float8();
    case DATEMILLI:
    return (FieldWriter) writer.dateMilli();
    case TIMESTAMPSEC:
    return (FieldWriter) writer.timeStampSec();
    case TIMESTAMPMILLI:
    return (FieldWriter) writer.timeStampMilli();
    case TIMESTAMPMICRO:
    return (FieldWriter) writer.timeStampMicro();
    case TIMESTAMPNANO:
    return (FieldWriter) writer.timeStampNano();
    case TIMEMICRO:
    return (FieldWriter) writer.timeMicro();
    case TIMENANO:
    return (FieldWriter) writer.timeNano();
    case INTERVALDAY:
    return (FieldWriter) writer.intervalDay();
    case INTERVALMONTHDAYNANO:
    return (FieldWriter) writer.intervalMonthDayNano();
    case DECIMAL256:
    return (FieldWriter) writer.decimal256();
    case DECIMAL:
    return (FieldWriter) writer.decimal();
    case VARBINARY:
    return (FieldWriter) writer.varBinary();
    case VARCHAR:
    return (FieldWriter) writer.varChar();
    case LARGEVARCHAR:
    return (FieldWriter) writer.largeVarChar();
    case LARGEVARBINARY:
    return (FieldWriter) writer.largeVarBinary();
    case BIT:
    return (FieldWriter) writer.bit();
    case STRUCT:
      return (FieldWriter) writer.struct();
    case FIXED_SIZE_LIST:
    case LIST:
    case MAP:
    case NULL:
      return (FieldWriter) writer.list();
    default:
      throw new UnsupportedOperationException(reader.getMinorType().toString());
    }
  }

  private static FieldWriter getMapWriterForReader(FieldReader reader, MapWriter writer) {
    switch (reader.getMinorType()) {
      case TINYINT:
      return (FieldWriter) writer.tinyInt();
      case UINT1:
      return (FieldWriter) writer.uInt1();
      case UINT2:
      return (FieldWriter) writer.uInt2();
      case SMALLINT:
      return (FieldWriter) writer.smallInt();
      case FLOAT2:
      return (FieldWriter) writer.float2();
      case INT:
      return (FieldWriter) writer.integer();
      case UINT4:
      return (FieldWriter) writer.uInt4();
      case FLOAT4:
      return (FieldWriter) writer.float4();
      case DATEDAY:
      return (FieldWriter) writer.dateDay();
      case INTERVALYEAR:
      return (FieldWriter) writer.intervalYear();
      case TIMESEC:
      return (FieldWriter) writer.timeSec();
      case TIMEMILLI:
      return (FieldWriter) writer.timeMilli();
      case BIGINT:
      return (FieldWriter) writer.bigInt();
      case UINT8:
      return (FieldWriter) writer.uInt8();
      case FLOAT8:
      return (FieldWriter) writer.float8();
      case DATEMILLI:
      return (FieldWriter) writer.dateMilli();
      case TIMESTAMPSEC:
      return (FieldWriter) writer.timeStampSec();
      case TIMESTAMPMILLI:
      return (FieldWriter) writer.timeStampMilli();
      case TIMESTAMPMICRO:
      return (FieldWriter) writer.timeStampMicro();
      case TIMESTAMPNANO:
      return (FieldWriter) writer.timeStampNano();
      case TIMEMICRO:
      return (FieldWriter) writer.timeMicro();
      case TIMENANO:
      return (FieldWriter) writer.timeNano();
      case INTERVALDAY:
      return (FieldWriter) writer.intervalDay();
      case INTERVALMONTHDAYNANO:
      return (FieldWriter) writer.intervalMonthDayNano();
      case DECIMAL256:
      return (FieldWriter) writer.decimal256();
      case DECIMAL:
      return (FieldWriter) writer.decimal();
      case VARBINARY:
      return (FieldWriter) writer.varBinary();
      case VARCHAR:
      return (FieldWriter) writer.varChar();
      case LARGEVARCHAR:
      return (FieldWriter) writer.largeVarChar();
      case LARGEVARBINARY:
      return (FieldWriter) writer.largeVarBinary();
      case BIT:
      return (FieldWriter) writer.bit();
      case STRUCT:
        return (FieldWriter) writer.struct();
      case FIXED_SIZE_LIST:
      case LIST:
      case NULL:
        return (FieldWriter) writer.list();
      case MAP:
        return (FieldWriter) writer.map(false);
      default:
        throw new UnsupportedOperationException(reader.getMinorType().toString());
    }
  }
}
