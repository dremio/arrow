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
 * This class is generated using FreeMarker on the ComplexWriters.java template.
 */
@SuppressWarnings("unused")
public class DecimalWriterImpl extends AbstractFieldWriter {

  final DecimalVector vector;


public DecimalWriterImpl(DecimalVector vector) {
    this.vector = vector;
  }

  @Override
  public Field getField() {
    return vector.getField();
  }

  @Override
  public int getValueCapacity() {
    return vector.getValueCapacity();
  }

  @Override
  public void allocate() {
    vector.allocateNew();
  }

  @Override
  public void close() {
    vector.close();
  }

  @Override
  public void clear() {
    vector.clear();
  }

  @Override
  protected int idx() {
    return super.idx();
  }





  public void write(DecimalHolder h){
    DecimalUtility.checkPrecisionAndScale(h.precision, h.scale, vector.getPrecision(), vector.getScale());
    vector.setSafe(idx(), h);
    vector.setValueCount(idx() + 1);
  }

  public void write(NullableDecimalHolder h){
    if (h.isSet == 1) {
      DecimalUtility.checkPrecisionAndScale(h.precision, h.scale, vector.getPrecision(), vector.getScale());
    }
    vector.setSafe(idx(), h);
    vector.setValueCount(idx() + 1);
  }

  public void writeDecimal(long start, ArrowBuf buffer){
    vector.setSafe(idx(), 1, start, buffer);
    vector.setValueCount(idx() + 1);
  }

  public void writeDecimal(long start, ArrowBuf buffer, ArrowType arrowType){
    DecimalUtility.checkPrecisionAndScale(((ArrowType.Decimal) arrowType).getPrecision(),
      ((ArrowType.Decimal) arrowType).getScale(), vector.getPrecision(), vector.getScale());
    vector.setSafe(idx(), 1, start, buffer);
    vector.setValueCount(idx() + 1);
  }

  public void writeDecimal(BigDecimal value){
    // vector.setSafe already does precision and scale checking
    vector.setSafe(idx(), value);
    vector.setValueCount(idx() + 1);
  }

  public void writeBigEndianBytesToDecimal(byte[] value, ArrowType arrowType){
    DecimalUtility.checkPrecisionAndScale(((ArrowType.Decimal) arrowType).getPrecision(),
        ((ArrowType.Decimal) arrowType).getScale(), vector.getPrecision(), vector.getScale());
    vector.setBigEndianSafe(idx(), value);
    vector.setValueCount(idx() + 1);
  }

  public void writeBigEndianBytesToDecimal(byte[] value){
    vector.setBigEndianSafe(idx(), value);
    vector.setValueCount(idx() + 1);
  }

  
  public void writeNull() {
    vector.setNull(idx());
    vector.setValueCount(idx()+1);
  }

}

