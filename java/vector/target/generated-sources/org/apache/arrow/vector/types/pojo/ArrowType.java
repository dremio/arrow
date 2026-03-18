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
package org.apache.arrow.vector.types.pojo;

import com.google.flatbuffers.FlatBufferBuilder;

import java.util.Objects;

import org.apache.arrow.flatbuf.Type;
import org.apache.arrow.memory.BufferAllocator;
import org.apache.arrow.vector.types.*;
import org.apache.arrow.vector.FieldVector;

import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.annotation.JsonSubTypes;
import com.fasterxml.jackson.annotation.JsonTypeInfo;

/**
 * Arrow types
 * Source code generated using FreeMarker template ArrowType.java
 **/
@JsonTypeInfo(
  use = JsonTypeInfo.Id.NAME,
  include = JsonTypeInfo.As.PROPERTY,
  property = "name")
@JsonSubTypes({
  @JsonSubTypes.Type(value = ArrowType.Null.class, name = "null"),
  @JsonSubTypes.Type(value = ArrowType.Struct.class, name = "struct"),
  @JsonSubTypes.Type(value = ArrowType.List.class, name = "list"),
  @JsonSubTypes.Type(value = ArrowType.LargeList.class, name = "largelist"),
  @JsonSubTypes.Type(value = ArrowType.FixedSizeList.class, name = "fixedsizelist"),
  @JsonSubTypes.Type(value = ArrowType.Union.class, name = "union"),
  @JsonSubTypes.Type(value = ArrowType.Map.class, name = "map"),
  @JsonSubTypes.Type(value = ArrowType.Int.class, name = "int"),
  @JsonSubTypes.Type(value = ArrowType.FloatingPoint.class, name = "floatingpoint"),
  @JsonSubTypes.Type(value = ArrowType.Utf8.class, name = "utf8"),
  @JsonSubTypes.Type(value = ArrowType.LargeUtf8.class, name = "largeutf8"),
  @JsonSubTypes.Type(value = ArrowType.Binary.class, name = "binary"),
  @JsonSubTypes.Type(value = ArrowType.LargeBinary.class, name = "largebinary"),
  @JsonSubTypes.Type(value = ArrowType.FixedSizeBinary.class, name = "fixedsizebinary"),
  @JsonSubTypes.Type(value = ArrowType.Bool.class, name = "bool"),
  @JsonSubTypes.Type(value = ArrowType.Decimal.class, name = "decimal"),
  @JsonSubTypes.Type(value = ArrowType.Date.class, name = "date"),
  @JsonSubTypes.Type(value = ArrowType.Time.class, name = "time"),
  @JsonSubTypes.Type(value = ArrowType.Timestamp.class, name = "timestamp"),
  @JsonSubTypes.Type(value = ArrowType.Interval.class, name = "interval"),
  @JsonSubTypes.Type(value = ArrowType.Duration.class, name = "duration"),
})
public abstract class ArrowType {

  public static abstract class PrimitiveType extends ArrowType {

    private PrimitiveType() {
    }

    @Override
    public boolean isComplex() {
      return false;
    }
  }

  public static abstract class ComplexType extends ArrowType {

    private ComplexType() {
    }

    @Override
    public boolean isComplex() {
      return true;
    }
  }

  public static enum ArrowTypeID {
    Null(Type.Null),
    Struct(Type.Struct_),
    List(Type.List),
    LargeList(Type.LargeList),
    FixedSizeList(Type.FixedSizeList),
    Union(Type.Union),
    Map(Type.Map),
    Int(Type.Int),
    FloatingPoint(Type.FloatingPoint),
    Utf8(Type.Utf8),
    LargeUtf8(Type.LargeUtf8),
    Binary(Type.Binary),
    LargeBinary(Type.LargeBinary),
    FixedSizeBinary(Type.FixedSizeBinary),
    Bool(Type.Bool),
    Decimal(Type.Decimal),
    Date(Type.Date),
    Time(Type.Time),
    Timestamp(Type.Timestamp),
    Interval(Type.Interval),
    Duration(Type.Duration),
    NONE(Type.NONE);

    private final byte flatbufType;

    public byte getFlatbufID() {
      return this.flatbufType;
    }

    private ArrowTypeID(byte flatbufType) {
      this.flatbufType = flatbufType;
    }
  }

  @JsonIgnore
  public abstract ArrowTypeID getTypeID();
  @JsonIgnore
  public abstract boolean isComplex();
  public abstract int getType(FlatBufferBuilder builder);
  public abstract <T> T accept(ArrowTypeVisitor<T> visitor);

  /**
   * to visit the ArrowTypes
   * <code>
   *   type.accept(new ArrowTypeVisitor&lt;Type&gt;() {
   *   ...
   *   });
   * </code>
   */
  public static interface ArrowTypeVisitor<T> {
    T visit(Null type);
    T visit(Struct type);
    T visit(List type);
    T visit(LargeList type);
    T visit(FixedSizeList type);
    T visit(Union type);
    T visit(Map type);
    T visit(Int type);
    T visit(FloatingPoint type);
    T visit(Utf8 type);
    T visit(LargeUtf8 type);
    T visit(Binary type);
    T visit(LargeBinary type);
    T visit(FixedSizeBinary type);
    T visit(Bool type);
    T visit(Decimal type);
    T visit(Date type);
    T visit(Time type);
    T visit(Timestamp type);
    T visit(Interval type);
    T visit(Duration type);
    default T visit(ExtensionType type) {
      return type.storageType().accept(this);
    }
  }

  /**
   * to visit the Complex ArrowTypes and bundle Primitive ones in one case
   */
  public static abstract class ComplexTypeVisitor<T> implements ArrowTypeVisitor<T> {

    public T visit(PrimitiveType type) {
      throw new UnsupportedOperationException("Unexpected Primitive type: " + type);
    }

    public final T visit(Null type) {
      return visit((PrimitiveType) type);
    }
    public final T visit(Int type) {
      return visit((PrimitiveType) type);
    }
    public final T visit(FloatingPoint type) {
      return visit((PrimitiveType) type);
    }
    public final T visit(Utf8 type) {
      return visit((PrimitiveType) type);
    }
    public final T visit(LargeUtf8 type) {
      return visit((PrimitiveType) type);
    }
    public final T visit(Binary type) {
      return visit((PrimitiveType) type);
    }
    public final T visit(LargeBinary type) {
      return visit((PrimitiveType) type);
    }
    public final T visit(FixedSizeBinary type) {
      return visit((PrimitiveType) type);
    }
    public final T visit(Bool type) {
      return visit((PrimitiveType) type);
    }
    public final T visit(Decimal type) {
      return visit((PrimitiveType) type);
    }
    public final T visit(Date type) {
      return visit((PrimitiveType) type);
    }
    public final T visit(Time type) {
      return visit((PrimitiveType) type);
    }
    public final T visit(Timestamp type) {
      return visit((PrimitiveType) type);
    }
    public final T visit(Interval type) {
      return visit((PrimitiveType) type);
    }
    public final T visit(Duration type) {
      return visit((PrimitiveType) type);
    }
  }

  /**
   * to visit the Primitive ArrowTypes and bundle Complex ones under one case
   */
  public static abstract class PrimitiveTypeVisitor<T> implements ArrowTypeVisitor<T> {

    public T visit(ComplexType type) {
      throw new UnsupportedOperationException("Unexpected Complex type: " + type);
    }

    public final T visit(Struct type) {
      return visit((ComplexType) type);
    }
    public final T visit(List type) {
      return visit((ComplexType) type);
    }
    public final T visit(LargeList type) {
      return visit((ComplexType) type);
    }
    public final T visit(FixedSizeList type) {
      return visit((ComplexType) type);
    }
    public final T visit(Union type) {
      return visit((ComplexType) type);
    }
    public final T visit(Map type) {
      return visit((ComplexType) type);
    }
  }

  public static class Null extends PrimitiveType {
    public static final ArrowTypeID TYPE_TYPE = ArrowTypeID.Null;
    public static final Null INSTANCE = new Null();

    @Override
    public ArrowTypeID getTypeID() {
      return TYPE_TYPE;
    }

    @Override
    public int getType(FlatBufferBuilder builder) {
      org.apache.arrow.flatbuf.Null.startNull(builder);
      return org.apache.arrow.flatbuf.Null.endNull(builder);
    }

    public String toString() {
      return "Null"
      ;
    }

    @Override
    public int hashCode() {
      return java.util.Arrays.deepHashCode(new Object[] {});
    }

    @Override
    public boolean equals(Object obj) {
      if (!(obj instanceof Null)) {
        return false;
      }
      return true;
    }

    @Override
    public <T> T accept(ArrowTypeVisitor<T> visitor) {
      return visitor.visit(this);
    }
  }
  public static class Struct extends ComplexType {
    public static final ArrowTypeID TYPE_TYPE = ArrowTypeID.Struct;
    public static final Struct INSTANCE = new Struct();

    @Override
    public ArrowTypeID getTypeID() {
      return TYPE_TYPE;
    }

    @Override
    public int getType(FlatBufferBuilder builder) {
      org.apache.arrow.flatbuf.Struct_.startStruct_(builder);
      return org.apache.arrow.flatbuf.Struct_.endStruct_(builder);
    }

    public String toString() {
      return "Struct"
      ;
    }

    @Override
    public int hashCode() {
      return java.util.Arrays.deepHashCode(new Object[] {});
    }

    @Override
    public boolean equals(Object obj) {
      if (!(obj instanceof Struct)) {
        return false;
      }
      return true;
    }

    @Override
    public <T> T accept(ArrowTypeVisitor<T> visitor) {
      return visitor.visit(this);
    }
  }
  public static class List extends ComplexType {
    public static final ArrowTypeID TYPE_TYPE = ArrowTypeID.List;
    public static final List INSTANCE = new List();

    @Override
    public ArrowTypeID getTypeID() {
      return TYPE_TYPE;
    }

    @Override
    public int getType(FlatBufferBuilder builder) {
      org.apache.arrow.flatbuf.List.startList(builder);
      return org.apache.arrow.flatbuf.List.endList(builder);
    }

    public String toString() {
      return "List"
      ;
    }

    @Override
    public int hashCode() {
      return java.util.Arrays.deepHashCode(new Object[] {});
    }

    @Override
    public boolean equals(Object obj) {
      if (!(obj instanceof List)) {
        return false;
      }
      return true;
    }

    @Override
    public <T> T accept(ArrowTypeVisitor<T> visitor) {
      return visitor.visit(this);
    }
  }
  public static class LargeList extends ComplexType {
    public static final ArrowTypeID TYPE_TYPE = ArrowTypeID.LargeList;
    public static final LargeList INSTANCE = new LargeList();

    @Override
    public ArrowTypeID getTypeID() {
      return TYPE_TYPE;
    }

    @Override
    public int getType(FlatBufferBuilder builder) {
      org.apache.arrow.flatbuf.LargeList.startLargeList(builder);
      return org.apache.arrow.flatbuf.LargeList.endLargeList(builder);
    }

    public String toString() {
      return "LargeList"
      ;
    }

    @Override
    public int hashCode() {
      return java.util.Arrays.deepHashCode(new Object[] {});
    }

    @Override
    public boolean equals(Object obj) {
      if (!(obj instanceof LargeList)) {
        return false;
      }
      return true;
    }

    @Override
    public <T> T accept(ArrowTypeVisitor<T> visitor) {
      return visitor.visit(this);
    }
  }
  public static class FixedSizeList extends ComplexType {
    public static final ArrowTypeID TYPE_TYPE = ArrowTypeID.FixedSizeList;

    int listSize;


    @JsonCreator
    public FixedSizeList(
      @JsonProperty("listSize") int listSize
    ) {
      this.listSize = listSize;
    }

    public int getListSize() {
      return listSize;
    }

    @Override
    public ArrowTypeID getTypeID() {
      return TYPE_TYPE;
    }

    @Override
    public int getType(FlatBufferBuilder builder) {
      org.apache.arrow.flatbuf.FixedSizeList.startFixedSizeList(builder);
      org.apache.arrow.flatbuf.FixedSizeList.addListSize(builder, this.listSize);
      return org.apache.arrow.flatbuf.FixedSizeList.endFixedSizeList(builder);
    }

    public String toString() {
      return "FixedSizeList"
        + "("
        +   listSize
        + ")"
      ;
    }

    @Override
    public int hashCode() {
      return java.util.Arrays.deepHashCode(new Object[] {listSize});
    }

    @Override
    public boolean equals(Object obj) {
      if (!(obj instanceof FixedSizeList)) {
        return false;
      }
      FixedSizeList that = (FixedSizeList) obj;
      return Objects.deepEquals(this.listSize, that.listSize) ;
    }

    @Override
    public <T> T accept(ArrowTypeVisitor<T> visitor) {
      return visitor.visit(this);
    }
  }
  public static class Union extends ComplexType {
    public static final ArrowTypeID TYPE_TYPE = ArrowTypeID.Union;

    UnionMode mode;
    int[] typeIds;


    @JsonCreator
    public Union(
      @JsonProperty("mode") UnionMode mode, 
      @JsonProperty("typeIds") int[] typeIds
    ) {
      this.mode = mode;
      this.typeIds = typeIds;
    }

    public UnionMode getMode() {
      return mode;
    }
    public int[] getTypeIds() {
      return typeIds;
    }

    @Override
    public ArrowTypeID getTypeID() {
      return TYPE_TYPE;
    }

    @Override
    public int getType(FlatBufferBuilder builder) {
      int typeIds = this.typeIds == null ? -1 : org.apache.arrow.flatbuf.Union.createTypeIdsVector(builder, this.typeIds);
      org.apache.arrow.flatbuf.Union.startUnion(builder);
      org.apache.arrow.flatbuf.Union.addMode(builder, this.mode.getFlatbufID());
      if (this.typeIds != null) {
        org.apache.arrow.flatbuf.Union.addTypeIds(builder, typeIds);
      }
      return org.apache.arrow.flatbuf.Union.endUnion(builder);
    }

    public String toString() {
      return "Union"
        + "("
        +   mode + ", " 
        +   java.util.Arrays.toString(typeIds)
        + ")"
      ;
    }

    @Override
    public int hashCode() {
      return java.util.Arrays.deepHashCode(new Object[] {mode, typeIds});
    }

    @Override
    public boolean equals(Object obj) {
      if (!(obj instanceof Union)) {
        return false;
      }
      Union that = (Union) obj;
      return Objects.deepEquals(this.mode, that.mode) &&
Objects.deepEquals(this.typeIds, that.typeIds) ;
    }

    @Override
    public <T> T accept(ArrowTypeVisitor<T> visitor) {
      return visitor.visit(this);
    }
  }
  public static class Map extends ComplexType {
    public static final ArrowTypeID TYPE_TYPE = ArrowTypeID.Map;

    boolean keysSorted;


    @JsonCreator
    public Map(
      @JsonProperty("keysSorted") boolean keysSorted
    ) {
      this.keysSorted = keysSorted;
    }

    public boolean getKeysSorted() {
      return keysSorted;
    }

    @Override
    public ArrowTypeID getTypeID() {
      return TYPE_TYPE;
    }

    @Override
    public int getType(FlatBufferBuilder builder) {
      org.apache.arrow.flatbuf.Map.startMap(builder);
      org.apache.arrow.flatbuf.Map.addKeysSorted(builder, this.keysSorted);
      return org.apache.arrow.flatbuf.Map.endMap(builder);
    }

    public String toString() {
      return "Map"
        + "("
        +   keysSorted
        + ")"
      ;
    }

    @Override
    public int hashCode() {
      return java.util.Arrays.deepHashCode(new Object[] {keysSorted});
    }

    @Override
    public boolean equals(Object obj) {
      if (!(obj instanceof Map)) {
        return false;
      }
      Map that = (Map) obj;
      return Objects.deepEquals(this.keysSorted, that.keysSorted) ;
    }

    @Override
    public <T> T accept(ArrowTypeVisitor<T> visitor) {
      return visitor.visit(this);
    }
  }
  public static class Int extends PrimitiveType {
    public static final ArrowTypeID TYPE_TYPE = ArrowTypeID.Int;

    int bitWidth;
    boolean isSigned;


    @JsonCreator
    public Int(
      @JsonProperty("bitWidth") int bitWidth, 
      @JsonProperty("isSigned") boolean isSigned
    ) {
      this.bitWidth = bitWidth;
      this.isSigned = isSigned;
    }

    public int getBitWidth() {
      return bitWidth;
    }
    public boolean getIsSigned() {
      return isSigned;
    }

    @Override
    public ArrowTypeID getTypeID() {
      return TYPE_TYPE;
    }

    @Override
    public int getType(FlatBufferBuilder builder) {
      org.apache.arrow.flatbuf.Int.startInt(builder);
      org.apache.arrow.flatbuf.Int.addBitWidth(builder, this.bitWidth);
      org.apache.arrow.flatbuf.Int.addIsSigned(builder, this.isSigned);
      return org.apache.arrow.flatbuf.Int.endInt(builder);
    }

    public String toString() {
      return "Int"
        + "("
        +   bitWidth + ", " 
        +   isSigned
        + ")"
      ;
    }

    @Override
    public int hashCode() {
      return java.util.Arrays.deepHashCode(new Object[] {bitWidth, isSigned});
    }

    @Override
    public boolean equals(Object obj) {
      if (!(obj instanceof Int)) {
        return false;
      }
      Int that = (Int) obj;
      return Objects.deepEquals(this.bitWidth, that.bitWidth) &&
Objects.deepEquals(this.isSigned, that.isSigned) ;
    }

    @Override
    public <T> T accept(ArrowTypeVisitor<T> visitor) {
      return visitor.visit(this);
    }
  }
  public static class FloatingPoint extends PrimitiveType {
    public static final ArrowTypeID TYPE_TYPE = ArrowTypeID.FloatingPoint;

    FloatingPointPrecision precision;


    @JsonCreator
    public FloatingPoint(
      @JsonProperty("precision") FloatingPointPrecision precision
    ) {
      this.precision = precision;
    }

    public FloatingPointPrecision getPrecision() {
      return precision;
    }

    @Override
    public ArrowTypeID getTypeID() {
      return TYPE_TYPE;
    }

    @Override
    public int getType(FlatBufferBuilder builder) {
      org.apache.arrow.flatbuf.FloatingPoint.startFloatingPoint(builder);
      org.apache.arrow.flatbuf.FloatingPoint.addPrecision(builder, this.precision.getFlatbufID());
      return org.apache.arrow.flatbuf.FloatingPoint.endFloatingPoint(builder);
    }

    public String toString() {
      return "FloatingPoint"
        + "("
        +   precision
        + ")"
      ;
    }

    @Override
    public int hashCode() {
      return java.util.Arrays.deepHashCode(new Object[] {precision});
    }

    @Override
    public boolean equals(Object obj) {
      if (!(obj instanceof FloatingPoint)) {
        return false;
      }
      FloatingPoint that = (FloatingPoint) obj;
      return Objects.deepEquals(this.precision, that.precision) ;
    }

    @Override
    public <T> T accept(ArrowTypeVisitor<T> visitor) {
      return visitor.visit(this);
    }
  }
  public static class Utf8 extends PrimitiveType {
    public static final ArrowTypeID TYPE_TYPE = ArrowTypeID.Utf8;
    public static final Utf8 INSTANCE = new Utf8();

    @Override
    public ArrowTypeID getTypeID() {
      return TYPE_TYPE;
    }

    @Override
    public int getType(FlatBufferBuilder builder) {
      org.apache.arrow.flatbuf.Utf8.startUtf8(builder);
      return org.apache.arrow.flatbuf.Utf8.endUtf8(builder);
    }

    public String toString() {
      return "Utf8"
      ;
    }

    @Override
    public int hashCode() {
      return java.util.Arrays.deepHashCode(new Object[] {});
    }

    @Override
    public boolean equals(Object obj) {
      if (!(obj instanceof Utf8)) {
        return false;
      }
      return true;
    }

    @Override
    public <T> T accept(ArrowTypeVisitor<T> visitor) {
      return visitor.visit(this);
    }
  }
  public static class LargeUtf8 extends PrimitiveType {
    public static final ArrowTypeID TYPE_TYPE = ArrowTypeID.LargeUtf8;
    public static final LargeUtf8 INSTANCE = new LargeUtf8();

    @Override
    public ArrowTypeID getTypeID() {
      return TYPE_TYPE;
    }

    @Override
    public int getType(FlatBufferBuilder builder) {
      org.apache.arrow.flatbuf.LargeUtf8.startLargeUtf8(builder);
      return org.apache.arrow.flatbuf.LargeUtf8.endLargeUtf8(builder);
    }

    public String toString() {
      return "LargeUtf8"
      ;
    }

    @Override
    public int hashCode() {
      return java.util.Arrays.deepHashCode(new Object[] {});
    }

    @Override
    public boolean equals(Object obj) {
      if (!(obj instanceof LargeUtf8)) {
        return false;
      }
      return true;
    }

    @Override
    public <T> T accept(ArrowTypeVisitor<T> visitor) {
      return visitor.visit(this);
    }
  }
  public static class Binary extends PrimitiveType {
    public static final ArrowTypeID TYPE_TYPE = ArrowTypeID.Binary;
    public static final Binary INSTANCE = new Binary();

    @Override
    public ArrowTypeID getTypeID() {
      return TYPE_TYPE;
    }

    @Override
    public int getType(FlatBufferBuilder builder) {
      org.apache.arrow.flatbuf.Binary.startBinary(builder);
      return org.apache.arrow.flatbuf.Binary.endBinary(builder);
    }

    public String toString() {
      return "Binary"
      ;
    }

    @Override
    public int hashCode() {
      return java.util.Arrays.deepHashCode(new Object[] {});
    }

    @Override
    public boolean equals(Object obj) {
      if (!(obj instanceof Binary)) {
        return false;
      }
      return true;
    }

    @Override
    public <T> T accept(ArrowTypeVisitor<T> visitor) {
      return visitor.visit(this);
    }
  }
  public static class LargeBinary extends PrimitiveType {
    public static final ArrowTypeID TYPE_TYPE = ArrowTypeID.LargeBinary;
    public static final LargeBinary INSTANCE = new LargeBinary();

    @Override
    public ArrowTypeID getTypeID() {
      return TYPE_TYPE;
    }

    @Override
    public int getType(FlatBufferBuilder builder) {
      org.apache.arrow.flatbuf.LargeBinary.startLargeBinary(builder);
      return org.apache.arrow.flatbuf.LargeBinary.endLargeBinary(builder);
    }

    public String toString() {
      return "LargeBinary"
      ;
    }

    @Override
    public int hashCode() {
      return java.util.Arrays.deepHashCode(new Object[] {});
    }

    @Override
    public boolean equals(Object obj) {
      if (!(obj instanceof LargeBinary)) {
        return false;
      }
      return true;
    }

    @Override
    public <T> T accept(ArrowTypeVisitor<T> visitor) {
      return visitor.visit(this);
    }
  }
  public static class FixedSizeBinary extends PrimitiveType {
    public static final ArrowTypeID TYPE_TYPE = ArrowTypeID.FixedSizeBinary;

    int byteWidth;


    @JsonCreator
    public FixedSizeBinary(
      @JsonProperty("byteWidth") int byteWidth
    ) {
      this.byteWidth = byteWidth;
    }

    public int getByteWidth() {
      return byteWidth;
    }

    @Override
    public ArrowTypeID getTypeID() {
      return TYPE_TYPE;
    }

    @Override
    public int getType(FlatBufferBuilder builder) {
      org.apache.arrow.flatbuf.FixedSizeBinary.startFixedSizeBinary(builder);
      org.apache.arrow.flatbuf.FixedSizeBinary.addByteWidth(builder, this.byteWidth);
      return org.apache.arrow.flatbuf.FixedSizeBinary.endFixedSizeBinary(builder);
    }

    public String toString() {
      return "FixedSizeBinary"
        + "("
        +   byteWidth
        + ")"
      ;
    }

    @Override
    public int hashCode() {
      return java.util.Arrays.deepHashCode(new Object[] {byteWidth});
    }

    @Override
    public boolean equals(Object obj) {
      if (!(obj instanceof FixedSizeBinary)) {
        return false;
      }
      FixedSizeBinary that = (FixedSizeBinary) obj;
      return Objects.deepEquals(this.byteWidth, that.byteWidth) ;
    }

    @Override
    public <T> T accept(ArrowTypeVisitor<T> visitor) {
      return visitor.visit(this);
    }
  }
  public static class Bool extends PrimitiveType {
    public static final ArrowTypeID TYPE_TYPE = ArrowTypeID.Bool;
    public static final Bool INSTANCE = new Bool();

    @Override
    public ArrowTypeID getTypeID() {
      return TYPE_TYPE;
    }

    @Override
    public int getType(FlatBufferBuilder builder) {
      org.apache.arrow.flatbuf.Bool.startBool(builder);
      return org.apache.arrow.flatbuf.Bool.endBool(builder);
    }

    public String toString() {
      return "Bool"
      ;
    }

    @Override
    public int hashCode() {
      return java.util.Arrays.deepHashCode(new Object[] {});
    }

    @Override
    public boolean equals(Object obj) {
      if (!(obj instanceof Bool)) {
        return false;
      }
      return true;
    }

    @Override
    public <T> T accept(ArrowTypeVisitor<T> visitor) {
      return visitor.visit(this);
    }
  }
  public static class Decimal extends PrimitiveType {
    public static final ArrowTypeID TYPE_TYPE = ArrowTypeID.Decimal;

    int precision;
    int scale;
    int bitWidth;


    // Needed to support golden file integration tests.
    @JsonCreator
    public static Decimal createDecimal(
      @JsonProperty("precision") int precision,
      @JsonProperty("scale") int scale,
      @JsonProperty("bitWidth") Integer bitWidth) {

      return new Decimal(precision, scale, bitWidth == null ? 128 : bitWidth);
    }

    /**
     * Construct Decimal with 128 bits.
     * 
     * This is kept mainly for the sake of backward compatibility.
     * Please use {@link org.apache.arrow.vector.types.pojo.ArrowType.Decimal#Decimal(int, int, int)} instead.
     *
     * @deprecated This API will be removed in a future release.
     */
    @Deprecated
    public Decimal(int precision, int scale) {
      this(precision, scale, 128);
    }

    public Decimal(
      @JsonProperty("precision") int precision, 
      @JsonProperty("scale") int scale, 
      @JsonProperty("bitWidth") int bitWidth
    ) {
      this.precision = precision;
      this.scale = scale;
      this.bitWidth = bitWidth;
    }

    public int getPrecision() {
      return precision;
    }
    public int getScale() {
      return scale;
    }
    public int getBitWidth() {
      return bitWidth;
    }

    @Override
    public ArrowTypeID getTypeID() {
      return TYPE_TYPE;
    }

    @Override
    public int getType(FlatBufferBuilder builder) {
      org.apache.arrow.flatbuf.Decimal.startDecimal(builder);
      org.apache.arrow.flatbuf.Decimal.addPrecision(builder, this.precision);
      org.apache.arrow.flatbuf.Decimal.addScale(builder, this.scale);
      org.apache.arrow.flatbuf.Decimal.addBitWidth(builder, this.bitWidth);
      return org.apache.arrow.flatbuf.Decimal.endDecimal(builder);
    }

    public String toString() {
      return "Decimal"
        + "("
        +   precision + ", " 
        +   scale + ", " 
        +   bitWidth
        + ")"
      ;
    }

    @Override
    public int hashCode() {
      return java.util.Arrays.deepHashCode(new Object[] {precision, scale, bitWidth});
    }

    @Override
    public boolean equals(Object obj) {
      if (!(obj instanceof Decimal)) {
        return false;
      }
      Decimal that = (Decimal) obj;
      return Objects.deepEquals(this.precision, that.precision) &&
Objects.deepEquals(this.scale, that.scale) &&
Objects.deepEquals(this.bitWidth, that.bitWidth) ;
    }

    @Override
    public <T> T accept(ArrowTypeVisitor<T> visitor) {
      return visitor.visit(this);
    }
  }
  public static class Date extends PrimitiveType {
    public static final ArrowTypeID TYPE_TYPE = ArrowTypeID.Date;

    DateUnit unit;


    @JsonCreator
    public Date(
      @JsonProperty("unit") DateUnit unit
    ) {
      this.unit = unit;
    }

    public DateUnit getUnit() {
      return unit;
    }

    @Override
    public ArrowTypeID getTypeID() {
      return TYPE_TYPE;
    }

    @Override
    public int getType(FlatBufferBuilder builder) {
      org.apache.arrow.flatbuf.Date.startDate(builder);
      org.apache.arrow.flatbuf.Date.addUnit(builder, this.unit.getFlatbufID());
      return org.apache.arrow.flatbuf.Date.endDate(builder);
    }

    public String toString() {
      return "Date"
        + "("
        +   unit
        + ")"
      ;
    }

    @Override
    public int hashCode() {
      return java.util.Arrays.deepHashCode(new Object[] {unit});
    }

    @Override
    public boolean equals(Object obj) {
      if (!(obj instanceof Date)) {
        return false;
      }
      Date that = (Date) obj;
      return Objects.deepEquals(this.unit, that.unit) ;
    }

    @Override
    public <T> T accept(ArrowTypeVisitor<T> visitor) {
      return visitor.visit(this);
    }
  }
  public static class Time extends PrimitiveType {
    public static final ArrowTypeID TYPE_TYPE = ArrowTypeID.Time;

    TimeUnit unit;
    int bitWidth;


    @JsonCreator
    public Time(
      @JsonProperty("unit") TimeUnit unit, 
      @JsonProperty("bitWidth") int bitWidth
    ) {
      this.unit = unit;
      this.bitWidth = bitWidth;
    }

    public TimeUnit getUnit() {
      return unit;
    }
    public int getBitWidth() {
      return bitWidth;
    }

    @Override
    public ArrowTypeID getTypeID() {
      return TYPE_TYPE;
    }

    @Override
    public int getType(FlatBufferBuilder builder) {
      org.apache.arrow.flatbuf.Time.startTime(builder);
      org.apache.arrow.flatbuf.Time.addUnit(builder, this.unit.getFlatbufID());
      org.apache.arrow.flatbuf.Time.addBitWidth(builder, this.bitWidth);
      return org.apache.arrow.flatbuf.Time.endTime(builder);
    }

    public String toString() {
      return "Time"
        + "("
        +   unit + ", " 
        +   bitWidth
        + ")"
      ;
    }

    @Override
    public int hashCode() {
      return java.util.Arrays.deepHashCode(new Object[] {unit, bitWidth});
    }

    @Override
    public boolean equals(Object obj) {
      if (!(obj instanceof Time)) {
        return false;
      }
      Time that = (Time) obj;
      return Objects.deepEquals(this.unit, that.unit) &&
Objects.deepEquals(this.bitWidth, that.bitWidth) ;
    }

    @Override
    public <T> T accept(ArrowTypeVisitor<T> visitor) {
      return visitor.visit(this);
    }
  }
  public static class Timestamp extends PrimitiveType {
    public static final ArrowTypeID TYPE_TYPE = ArrowTypeID.Timestamp;

    TimeUnit unit;
    String timezone;


    @JsonCreator
    public Timestamp(
      @JsonProperty("unit") TimeUnit unit, 
      @JsonProperty("timezone") String timezone
    ) {
      this.unit = unit;
      this.timezone = timezone;
    }

    public TimeUnit getUnit() {
      return unit;
    }
    public String getTimezone() {
      return timezone;
    }

    @Override
    public ArrowTypeID getTypeID() {
      return TYPE_TYPE;
    }

    @Override
    public int getType(FlatBufferBuilder builder) {
      int timezone = this.timezone == null ? -1 : builder.createString(this.timezone);
      org.apache.arrow.flatbuf.Timestamp.startTimestamp(builder);
      org.apache.arrow.flatbuf.Timestamp.addUnit(builder, this.unit.getFlatbufID());
      if (this.timezone != null) {
        org.apache.arrow.flatbuf.Timestamp.addTimezone(builder, timezone);
      }
      return org.apache.arrow.flatbuf.Timestamp.endTimestamp(builder);
    }

    public String toString() {
      return "Timestamp"
        + "("
        +   unit + ", " 
        +   timezone
        + ")"
      ;
    }

    @Override
    public int hashCode() {
      return java.util.Arrays.deepHashCode(new Object[] {unit, timezone});
    }

    @Override
    public boolean equals(Object obj) {
      if (!(obj instanceof Timestamp)) {
        return false;
      }
      Timestamp that = (Timestamp) obj;
      return Objects.deepEquals(this.unit, that.unit) &&
Objects.deepEquals(this.timezone, that.timezone) ;
    }

    @Override
    public <T> T accept(ArrowTypeVisitor<T> visitor) {
      return visitor.visit(this);
    }
  }
  public static class Interval extends PrimitiveType {
    public static final ArrowTypeID TYPE_TYPE = ArrowTypeID.Interval;

    IntervalUnit unit;


    @JsonCreator
    public Interval(
      @JsonProperty("unit") IntervalUnit unit
    ) {
      this.unit = unit;
    }

    public IntervalUnit getUnit() {
      return unit;
    }

    @Override
    public ArrowTypeID getTypeID() {
      return TYPE_TYPE;
    }

    @Override
    public int getType(FlatBufferBuilder builder) {
      org.apache.arrow.flatbuf.Interval.startInterval(builder);
      org.apache.arrow.flatbuf.Interval.addUnit(builder, this.unit.getFlatbufID());
      return org.apache.arrow.flatbuf.Interval.endInterval(builder);
    }

    public String toString() {
      return "Interval"
        + "("
        +   unit
        + ")"
      ;
    }

    @Override
    public int hashCode() {
      return java.util.Arrays.deepHashCode(new Object[] {unit});
    }

    @Override
    public boolean equals(Object obj) {
      if (!(obj instanceof Interval)) {
        return false;
      }
      Interval that = (Interval) obj;
      return Objects.deepEquals(this.unit, that.unit) ;
    }

    @Override
    public <T> T accept(ArrowTypeVisitor<T> visitor) {
      return visitor.visit(this);
    }
  }
  public static class Duration extends PrimitiveType {
    public static final ArrowTypeID TYPE_TYPE = ArrowTypeID.Duration;

    TimeUnit unit;


    @JsonCreator
    public Duration(
      @JsonProperty("unit") TimeUnit unit
    ) {
      this.unit = unit;
    }

    public TimeUnit getUnit() {
      return unit;
    }

    @Override
    public ArrowTypeID getTypeID() {
      return TYPE_TYPE;
    }

    @Override
    public int getType(FlatBufferBuilder builder) {
      org.apache.arrow.flatbuf.Duration.startDuration(builder);
      org.apache.arrow.flatbuf.Duration.addUnit(builder, this.unit.getFlatbufID());
      return org.apache.arrow.flatbuf.Duration.endDuration(builder);
    }

    public String toString() {
      return "Duration"
        + "("
        +   unit
        + ")"
      ;
    }

    @Override
    public int hashCode() {
      return java.util.Arrays.deepHashCode(new Object[] {unit});
    }

    @Override
    public boolean equals(Object obj) {
      if (!(obj instanceof Duration)) {
        return false;
      }
      Duration that = (Duration) obj;
      return Objects.deepEquals(this.unit, that.unit) ;
    }

    @Override
    public <T> T accept(ArrowTypeVisitor<T> visitor) {
      return visitor.visit(this);
    }
  }

  /**
   * A user-defined data type that wraps an underlying storage type.
   */
  public abstract static class ExtensionType extends ComplexType {
    /** The on-wire type for this user-defined type. */
    public abstract ArrowType storageType();
    /** The name of this user-defined type. Used to identify the type during serialization. */
    public abstract String extensionName();
    /** Check equality of this type to another user-defined type. */
    public abstract boolean extensionEquals(ExtensionType other);
    /** Save any metadata for this type. */
    public abstract String serialize();
    /** Given saved metadata and the underlying storage type, construct a new instance of the user type. */
    public abstract ArrowType deserialize(ArrowType storageType, String serializedData);
    /** Construct a vector for the user type. */
    public abstract FieldVector getNewVector(String name, FieldType fieldType, BufferAllocator allocator);

    /** The field metadata key storing the name of the extension type. */
    public static final String EXTENSION_METADATA_KEY_NAME = "ARROW:extension:name";
    /** The field metadata key storing metadata for the extension type. */
    public static final String EXTENSION_METADATA_KEY_METADATA = "ARROW:extension:metadata";

    @Override
    public ArrowTypeID getTypeID() {
      return storageType().getTypeID();
    }

    @Override
    public int getType(FlatBufferBuilder builder) {
      return storageType().getType(builder);
    }

    public String toString() {
      return "ExtensionType(" + extensionName() + ", " + storageType().toString() + ")";
    }

    @Override
    public int hashCode() {
      return java.util.Arrays.deepHashCode(new Object[] {storageType(), extensionName()});
    }

    @Override
    public boolean equals(Object obj) {
      if (!(obj instanceof ExtensionType)) {
        return false;
      }
      return this.extensionEquals((ExtensionType) obj);
    }

    @Override
    public <T> T accept(ArrowTypeVisitor<T> visitor) {
      return visitor.visit(this);
    }
  }

  private static final int defaultDecimalBitWidth = 128;

  public static org.apache.arrow.vector.types.pojo.ArrowType getTypeForField(org.apache.arrow.flatbuf.Field field) {
    switch(field.typeType()) {
    case Type.Null: {
      org.apache.arrow.flatbuf.Null nullType = (org.apache.arrow.flatbuf.Null) field.type(new org.apache.arrow.flatbuf.Null());
      return new Null();
    }
    case Type.Struct_: {
      org.apache.arrow.flatbuf.Struct_ struct_Type = (org.apache.arrow.flatbuf.Struct_) field.type(new org.apache.arrow.flatbuf.Struct_());
      return new Struct();
    }
    case Type.List: {
      org.apache.arrow.flatbuf.List listType = (org.apache.arrow.flatbuf.List) field.type(new org.apache.arrow.flatbuf.List());
      return new List();
    }
    case Type.LargeList: {
      org.apache.arrow.flatbuf.LargeList largelistType = (org.apache.arrow.flatbuf.LargeList) field.type(new org.apache.arrow.flatbuf.LargeList());
      return new LargeList();
    }
    case Type.FixedSizeList: {
      org.apache.arrow.flatbuf.FixedSizeList fixedsizelistType = (org.apache.arrow.flatbuf.FixedSizeList) field.type(new org.apache.arrow.flatbuf.FixedSizeList());
      int listSize = fixedsizelistType.listSize();
      return new FixedSizeList(listSize);
    }
    case Type.Union: {
      org.apache.arrow.flatbuf.Union unionType = (org.apache.arrow.flatbuf.Union) field.type(new org.apache.arrow.flatbuf.Union());
      short mode = unionType.mode();
      int[] typeIds = new int[unionType.typeIdsLength()];
      for (int i = 0; i< typeIds.length; ++i) {
        typeIds[i] = unionType.typeIds(i);
      }
      return new Union(UnionMode.fromFlatbufID(mode), typeIds);
    }
    case Type.Map: {
      org.apache.arrow.flatbuf.Map mapType = (org.apache.arrow.flatbuf.Map) field.type(new org.apache.arrow.flatbuf.Map());
      boolean keysSorted = mapType.keysSorted();
      return new Map(keysSorted);
    }
    case Type.Int: {
      org.apache.arrow.flatbuf.Int intType = (org.apache.arrow.flatbuf.Int) field.type(new org.apache.arrow.flatbuf.Int());
      int bitWidth = intType.bitWidth();
      boolean isSigned = intType.isSigned();
      return new Int(bitWidth, isSigned);
    }
    case Type.FloatingPoint: {
      org.apache.arrow.flatbuf.FloatingPoint floatingpointType = (org.apache.arrow.flatbuf.FloatingPoint) field.type(new org.apache.arrow.flatbuf.FloatingPoint());
      short precision = floatingpointType.precision();
      return new FloatingPoint(FloatingPointPrecision.fromFlatbufID(precision));
    }
    case Type.Utf8: {
      org.apache.arrow.flatbuf.Utf8 utf8Type = (org.apache.arrow.flatbuf.Utf8) field.type(new org.apache.arrow.flatbuf.Utf8());
      return new Utf8();
    }
    case Type.LargeUtf8: {
      org.apache.arrow.flatbuf.LargeUtf8 largeutf8Type = (org.apache.arrow.flatbuf.LargeUtf8) field.type(new org.apache.arrow.flatbuf.LargeUtf8());
      return new LargeUtf8();
    }
    case Type.Binary: {
      org.apache.arrow.flatbuf.Binary binaryType = (org.apache.arrow.flatbuf.Binary) field.type(new org.apache.arrow.flatbuf.Binary());
      return new Binary();
    }
    case Type.LargeBinary: {
      org.apache.arrow.flatbuf.LargeBinary largebinaryType = (org.apache.arrow.flatbuf.LargeBinary) field.type(new org.apache.arrow.flatbuf.LargeBinary());
      return new LargeBinary();
    }
    case Type.FixedSizeBinary: {
      org.apache.arrow.flatbuf.FixedSizeBinary fixedsizebinaryType = (org.apache.arrow.flatbuf.FixedSizeBinary) field.type(new org.apache.arrow.flatbuf.FixedSizeBinary());
      int byteWidth = fixedsizebinaryType.byteWidth();
      return new FixedSizeBinary(byteWidth);
    }
    case Type.Bool: {
      org.apache.arrow.flatbuf.Bool boolType = (org.apache.arrow.flatbuf.Bool) field.type(new org.apache.arrow.flatbuf.Bool());
      return new Bool();
    }
    case Type.Decimal: {
      org.apache.arrow.flatbuf.Decimal decimalType = (org.apache.arrow.flatbuf.Decimal) field.type(new org.apache.arrow.flatbuf.Decimal());
      int precision = decimalType.precision();
      int scale = decimalType.scale();
      int bitWidth = decimalType.bitWidth();
      if (bitWidth != defaultDecimalBitWidth && bitWidth != 256) {
        throw new IllegalArgumentException("Library only supports 128-bit and 256-bit decimal values");
      }
      return new Decimal(precision, scale, bitWidth);
    }
    case Type.Date: {
      org.apache.arrow.flatbuf.Date dateType = (org.apache.arrow.flatbuf.Date) field.type(new org.apache.arrow.flatbuf.Date());
      short unit = dateType.unit();
      return new Date(DateUnit.fromFlatbufID(unit));
    }
    case Type.Time: {
      org.apache.arrow.flatbuf.Time timeType = (org.apache.arrow.flatbuf.Time) field.type(new org.apache.arrow.flatbuf.Time());
      short unit = timeType.unit();
      int bitWidth = timeType.bitWidth();
      return new Time(TimeUnit.fromFlatbufID(unit), bitWidth);
    }
    case Type.Timestamp: {
      org.apache.arrow.flatbuf.Timestamp timestampType = (org.apache.arrow.flatbuf.Timestamp) field.type(new org.apache.arrow.flatbuf.Timestamp());
      short unit = timestampType.unit();
      String timezone = timestampType.timezone();
      return new Timestamp(TimeUnit.fromFlatbufID(unit), timezone);
    }
    case Type.Interval: {
      org.apache.arrow.flatbuf.Interval intervalType = (org.apache.arrow.flatbuf.Interval) field.type(new org.apache.arrow.flatbuf.Interval());
      short unit = intervalType.unit();
      return new Interval(IntervalUnit.fromFlatbufID(unit));
    }
    case Type.Duration: {
      org.apache.arrow.flatbuf.Duration durationType = (org.apache.arrow.flatbuf.Duration) field.type(new org.apache.arrow.flatbuf.Duration());
      short unit = durationType.unit();
      return new Duration(TimeUnit.fromFlatbufID(unit));
    }
    default:
      throw new UnsupportedOperationException("Unsupported type: " + field.typeType());
    }
  }

  public static Int getInt(org.apache.arrow.flatbuf.Field field) {
    org.apache.arrow.flatbuf.Int intType = (org.apache.arrow.flatbuf.Int) field.type(new org.apache.arrow.flatbuf.Int());
    return new Int(intType.bitWidth(), intType.isSigned());
  }
}


