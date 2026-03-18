

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
package org.apache.arrow.vector.complex.reader;


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
 * Source code generated using FreeMarker template BaseReader.java
 */
@SuppressWarnings("unused")
public interface BaseReader extends Positionable{
  Field getField();
  MinorType getMinorType();
  void reset();
  void read(UnionHolder holder);
  void read(int index, UnionHolder holder);
  void copyAsValue(UnionWriter writer);
  void read(DenseUnionHolder holder);
  void read(int index, DenseUnionHolder holder);
  void copyAsValue(DenseUnionWriter writer);
  boolean isSet();

  public interface StructReader extends BaseReader, Iterable<String>{
    FieldReader reader(String name);
  }
  
  public interface RepeatedStructReader extends StructReader{
    boolean next();
    int size();
    void copyAsValue(StructWriter writer);
  }
  
  public interface ListReader extends BaseReader{
    FieldReader reader(); 
  }
  
  public interface RepeatedListReader extends ListReader{
    boolean next();
    int size();
    void copyAsValue(ListWriter writer);
  }

  public interface MapReader extends BaseReader{
    FieldReader reader();
  }

  public interface RepeatedMapReader extends MapReader{
    boolean next();
    int size();
    void copyAsValue(MapWriter writer);
  }
  
  public interface ScalarReader extends  
   TinyIntReader,  UInt1Reader,  UInt2Reader,  SmallIntReader,  Float2Reader,  IntReader,  UInt4Reader,  Float4Reader,  DateDayReader,  IntervalYearReader,  TimeSecReader,  TimeMilliReader,  BigIntReader,  UInt8Reader,  Float8Reader,  DateMilliReader,  DurationReader,  TimeStampSecReader,  TimeStampMilliReader,  TimeStampMicroReader,  TimeStampNanoReader,  TimeStampSecTZReader,  TimeStampMilliTZReader,  TimeStampMicroTZReader,  TimeStampNanoTZReader,  TimeMicroReader,  TimeNanoReader,  IntervalDayReader,  IntervalMonthDayNanoReader,  Decimal256Reader,  DecimalReader,  FixedSizeBinaryReader,  VarBinaryReader,  VarCharReader,  LargeVarCharReader,  LargeVarBinaryReader,  BitReader,  
  BaseReader {}
  
  interface ComplexReader{
    StructReader rootAsStruct();
    ListReader rootAsList();
    boolean rootIsStruct();
    boolean ok();
  }
}

