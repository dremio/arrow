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



// Source code generated using FreeMarker template HolderReaderImpl.java

@SuppressWarnings("unused")
public class Decimal256HolderReaderImpl extends AbstractFieldReader {

  private Decimal256Holder holder;
  public Decimal256HolderReaderImpl(Decimal256Holder holder) {
    this.holder = holder;
  }

  @Override
  public int size() {
    throw new UnsupportedOperationException("You can't call size on a Holder value reader.");
  }

  @Override
  public boolean next() {
    throw new UnsupportedOperationException("You can't call next on a single value reader.");

  }

  @Override
  public void setPosition(int index) {
    throw new UnsupportedOperationException("You can't call next on a single value reader.");
  }

  @Override
  public MinorType getMinorType() {
        return MinorType.DECIMAL256;
  }

  @Override
  public boolean isSet() {
    return true;
  }

  @Override
  public void read(Decimal256Holder h) {
    h.start = holder.start;
    h.buffer = holder.buffer;
    h.scale = holder.scale;
    h.precision = holder.precision;
  }

  @Override
  public void read(NullableDecimal256Holder h) {
    h.start = holder.start;
    h.buffer = holder.buffer;
    h.scale = holder.scale;
    h.precision = holder.precision;
    h.isSet = isSet() ? 1 : 0;
  }

  // read friendly type
  @Override
  public BigDecimal readBigDecimal() {

    byte[] bytes = new byte[32];
    holder.buffer.getBytes(holder.start, bytes, 0, 32);
    BigDecimal value = new BigDecimal(new BigInteger(bytes), holder.scale);
    return value;
  }

  @Override
  public Object readObject() {
    return readBigDecimal();
  }

  public void copyAsValue(Decimal256Writer writer){
    writer.write(holder);
  }
}


