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
package org.apache.arrow.adapter.jdbc.binder;

import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.sql.Types;
import java.util.Calendar;
import org.apache.arrow.vector.TimeStampVector;
import org.apache.arrow.vector.TimeStampWithPrecisionVector;
import org.apache.arrow.vector.types.pojo.ArrowType;

/** A column binder for timestamps. */
public class TimestampWithPrecisionBinder extends BaseColumnBinder<TimeStampVector> {
  private final Calendar calendar;
  private final long unitsPerSecond;
  private final long nanosPerUnit;

  /** Create a binder for a timestamp vector using the default JDBC type code. */
  public TimestampWithPrecisionBinder(TimeStampWithPrecisionVector vector, Calendar calendar) {
    this(
        vector,
        calendar,
        isZoned(vector.getField().getType()) ? Types.TIMESTAMP_WITH_TIMEZONE : Types.TIMESTAMP);
  }

  /**
   * Create a binder for a timestamp vector.
   *
   * @param vector The vector to pull values from.
   * @param calendar Optionally, the calendar to pass to JDBC.
   * @param jdbcType The JDBC type code to use for null values.
   */
  public TimestampWithPrecisionBinder(
      TimeStampWithPrecisionVector vector, Calendar calendar, int jdbcType) {
    super(vector, jdbcType);
    this.calendar = calendar;

    final ArrowType.TimestampWithPrecision type =
        (ArrowType.TimestampWithPrecision) vector.getField().getType();
    switch (type.getPrecision()) {
      case 0:
        this.unitsPerSecond = 1;
        this.nanosPerUnit = 1_000_000_000;
        break;
      case 1:
        this.unitsPerSecond = 10;
        this.nanosPerUnit = 1_000_000_00;
        break;
      case 2:
        this.unitsPerSecond = 100;
        this.nanosPerUnit = 1_000_000_0;
        break;
      case 3:
        this.unitsPerSecond = 1_000;
        this.nanosPerUnit = 1_000_000;
        break;
      case 4:
        this.unitsPerSecond = 10_000;
        this.nanosPerUnit = 1_000_00;
        break;
      case 5:
        this.unitsPerSecond = 100_000;
        this.nanosPerUnit = 1_000_0;
        break;
      case 6:
        this.unitsPerSecond = 1_000_000;
        this.nanosPerUnit = 1_000;
        break;
      case 7:
        this.unitsPerSecond = 10_000_000;
        this.nanosPerUnit = 1_00;
        break;
      case 8:
        this.unitsPerSecond = 100_000_000;
        this.nanosPerUnit = 1_0;
        break;
      case 9:
        this.unitsPerSecond = 1_000_000_000;
        this.nanosPerUnit = 1;
        break;
      default:
        throw new IllegalArgumentException("Invalid time unit in " + type);
    }
  }

  @Override
  public void bind(PreparedStatement statement, int parameterIndex, int rowIndex)
      throws SQLException {
    // TODO: option to throw on truncation (vendor Guava IntMath#multiply) or overflow
    final long rawValue =
        vector.getDataBuffer().getLong((long) rowIndex * TimeStampWithPrecisionVector.TYPE_WIDTH);
    final long seconds = rawValue / unitsPerSecond;
    final int nanos = (int) ((rawValue - (seconds * unitsPerSecond)) * nanosPerUnit);
    final Timestamp value = new Timestamp(seconds * 1_000);
    value.setNanos(nanos);
    if (calendar != null) {
      // Timestamp == Date == UTC timestamp (confusingly). Arrow's timestamp with timezone is a UTC
      // value with a
      // zone offset, so we don't need to do any conversion.
      statement.setTimestamp(parameterIndex, value, calendar);
    } else {
      // Arrow timestamp without timezone isn't strictly convertible to any timezone. So this is
      // technically wrong,
      // but there is no 'correct' interpretation here. The application should provide a calendar.
      statement.setTimestamp(parameterIndex, value);
    }
  }

  private static boolean isZoned(ArrowType type) {
    final String timezone = ((ArrowType.TimestampWithPrecision) type).getTimezone();
    return timezone != null && !timezone.isEmpty();
  }
}
