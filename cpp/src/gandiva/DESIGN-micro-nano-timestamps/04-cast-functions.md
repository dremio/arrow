# Phase 4: Cast Functions

**Prerequisite:** Phase 1 (types, constants, floor-division helpers)
**Enables:** Independent of Phases 2 and 3

This phase adds MICRO and NANO implementations for all cast and conversion
functions. These are mostly standalone implementations (not macro-generated),
so each is described individually.

---

## 4.1 `castVARCHAR` — timestamp to string

**Symbol names:** `castVARCHAR_timestamp_micro_int64`,
`castVARCHAR_timestamp_nano_int64`

Subsecond digits cannot use `EpochTimePoint.TimeOfDay().subseconds()` (millis
resolution only). The fractional-second component is computed directly:

```c
// MICRO: fractional-second µs component, 0–999999, handles pre-epoch
gdv_int64 micros_of_sec = micros >= 0
    ? micros % MICROS_IN_SEC
    : MICROS_IN_SEC + (micros % MICROS_IN_SEC);

// NANO: fractional-second ns component, 0–999999999
gdv_int64 nanos_of_sec = nanos >= 0
    ? nanos % NANOS_IN_SEC
    : NANOS_IN_SEC + (nanos % NANOS_IN_SEC);
```

Date/time fields (year, month, day, hour, minute, second) are still extracted
via `EpochTimePoint` after floor-dividing to millis.

Format strings and buffer lengths are unit-specific:

| Unit | `kTimeStampStringLen` | Format suffix |
|---|---|---|
| MILLI (existing) | 23 | `.%03` PRId64 |
| MICRO | 26 | `.%06` PRId64 |
| NANO | 29 | `.%09` PRId64 |

**Known pre-existing issue:** `castVARCHAR_timestamp_int64` has a bug for
pre-1970 timestamps in the MILLI implementation. The MICRO/NANO
implementations will carry the same behavior; this is not fixed in this phase.

### Registry

```cpp
NativeFunction("castVARCHAR", {"varchar"},
               DataTypeVector{timestamp_micro(), int64()},
               utf8(), kResultNullIfNull,
               "castVARCHAR_timestamp_micro_int64",
               NativeFunction::kNeedsContext),

NativeFunction("castVARCHAR", {"varchar"},
               DataTypeVector{timestamp_nano(), int64()},
               utf8(), kResultNullIfNull,
               "castVARCHAR_timestamp_nano_int64",
               NativeFunction::kNeedsContext),
```

---

## 4.2 `castTIMESTAMP_utf8` — string to timestamp

**Symbol names:** `castTIMESTAMP_micro_utf8`, `castTIMESTAMP_nano_utf8`

The existing parser normalizes fractional seconds to 3 digits via
`normalize_subseconds_to_millis`. Two new parallel helpers:

```c
static inline int64_t normalize_subseconds_to_micros(int32_t subseconds,
                                                     int32_t num_digits) {
  if (num_digits <= 0 || num_digits == 6) return subseconds;
  int32_t digit_diff = num_digits - 6;
  while (digit_diff > 0) { subseconds /= 10; digit_diff--; }
  while (digit_diff < 0) { subseconds *= 10; digit_diff++; }
  return subseconds;
}

static inline int64_t normalize_subseconds_to_nanos(int32_t subseconds,
                                                    int32_t num_digits) {
  if (num_digits <= 0 || num_digits == 9) return subseconds;
  int32_t digit_diff = num_digits - 9;
  while (digit_diff > 0) { subseconds /= 10; digit_diff--; }
  while (digit_diff < 0) { subseconds *= 10; digit_diff++; }
  return subseconds;
}
```

Inputs with fewer fractional digits than the target precision are zero-padded
on the right (left-anchored); inputs with more digits are truncated, not
rounded.

Example: `"2021-01-01 12:00:00.1"` parsed into NANO = `100,000,000 ns`
subsecond component.

The final epoch value is computed as:
```c
// MICRO
gdv_timestamp_micro result = sys_days_micros
    + hours * MICROS_IN_HOUR + minutes * MICROS_IN_MIN
    + seconds * MICROS_IN_SEC + normalized_subseconds;

// NANO
gdv_timestamp_nano result = sys_days_nanos
    + hours * NANOS_IN_HOUR + minutes * NANOS_IN_MIN
    + seconds * NANOS_IN_SEC + normalized_subseconds;
```

### Registry

```cpp
NativeFunction("castTIMESTAMP", {}, DataTypeVector{utf8()},
               timestamp_micro(), kResultNullIfNull,
               "castTIMESTAMP_micro_utf8",
               NativeFunction::kNeedsContext | NativeFunction::kCanReturnErrors),

NativeFunction("castTIMESTAMP", {}, DataTypeVector{utf8()},
               timestamp_nano(), kResultNullIfNull,
               "castTIMESTAMP_nano_utf8",
               NativeFunction::kNeedsContext | NativeFunction::kCanReturnErrors),
```

---

## 4.3 `castTIMESTAMP_date64` — date to timestamp

**Symbol names:** `castTIMESTAMP_micro_date64`, `castTIMESTAMP_nano_date64`

`date64` is always millis since epoch. Conversion to MICRO/NANO is a multiply:

```c
gdv_timestamp_micro castTIMESTAMP_micro_date64(gdv_date64 date_in_millis) {
  return date_in_millis * MICROS_IN_MILLIS;
}

gdv_timestamp_nano castTIMESTAMP_nano_date64(gdv_date64 date_in_millis) {
  return date_in_millis * NANOS_IN_MILLIS;
}
```

No overflow concern: `date64` represents dates, not arbitrary `int64` values.

### Registry

```cpp
NativeFunction("castTIMESTAMP", {}, DataTypeVector{date64()},
               timestamp_micro(), kResultNullIfNull,
               "castTIMESTAMP_micro_date64"),

NativeFunction("castTIMESTAMP", {}, DataTypeVector{date64()},
               timestamp_nano(), kResultNullIfNull,
               "castTIMESTAMP_nano_date64"),
```

---

## 4.4 `castTIMESTAMP_int64` — epoch seconds to timestamp

**Symbol names:** `castTIMESTAMP_micro_int64`, `castTIMESTAMP_nano_int64`

The existing MILLI version is `return in;` (identity — interprets `int64` as
millis). MICRO/NANO variants multiply by the units-per-second constant, with
an overflow guard for NANO:

```c
gdv_timestamp_micro castTIMESTAMP_micro_int64(gdv_int64 in) {
  return in * MICROS_IN_SEC;
}

gdv_timestamp_nano castTIMESTAMP_nano_int64(gdv_int64 in) {
  // Guard: |in| > LLONG_MAX / NANOS_IN_SEC would overflow
  if (in > INT64_MAX / NANOS_IN_SEC || in < INT64_MIN / NANOS_IN_SEC) {
    return 0;  // null output via error context, or 0 as sentinel
  }
  return in * NANOS_IN_SEC;
}
```

**Note:** The exact null-return mechanism depends on whether these are
registered with `kCanReturnErrors` / `kNeedsContext`. The existing MILLI
version has neither flag. If the NANO variant needs to return null on overflow,
it may need to be registered with those flags and accept a context parameter.
This is a minor implementation detail to resolve during coding.

### Registry

```cpp
NativeFunction("castTIMESTAMP", {}, DataTypeVector{int64()},
               timestamp_micro(), kResultNullIfNull,
               "castTIMESTAMP_micro_int64"),

NativeFunction("castTIMESTAMP", {}, DataTypeVector{int64()},
               timestamp_nano(), kResultNullIfNull,
               "castTIMESTAMP_nano_int64"),
```

---

## 4.5 `castDATE_timestamp` — timestamp to date

**Symbol names:** `castDATE_timestamp_micro`, `castDATE_timestamp_nano`

`date64` is millis. Floor-divide to millis, then truncate to day boundary
(matching existing MILLI behavior):

```c
gdv_date64 castDATE_timestamp_micro(gdv_timestamp_micro micros) {
  return castDATE_timestamp(MICROS_TO_MILLIS_FLOOR(micros));
}

gdv_date64 castDATE_timestamp_nano(gdv_timestamp_nano nanos) {
  return castDATE_timestamp(NANOS_TO_MILLIS_FLOOR(nanos));
}
```

### Registry

```cpp
NativeFunction("castDATE", {"to_date"}, DataTypeVector{timestamp_micro()},
               date64(), kResultNullIfNull,
               "castDATE_timestamp_micro"),

NativeFunction("castDATE", {"to_date"}, DataTypeVector{timestamp_nano()},
               date64(), kResultNullIfNull,
               "castDATE_timestamp_nano"),
```

---

## 4.6 `to_timestamp` — numeric to timestamp

**Symbol names:** `to_timestamp_micro_<type>`, `to_timestamp_nano_<type>`
for each base numeric type.

The existing `to_timestamp` interprets the input as seconds and returns millis.
MICRO/NANO variants multiply by the appropriate constant:

```c
#define TO_TIMESTAMP_MICRO_INTEGER(TYPE)                                   \
  FORCE_INLINE                                                             \
  gdv_timestamp_micro to_timestamp_micro##_##TYPE(gdv_##TYPE seconds) {    \
    return static_cast<gdv_int64>(seconds) * MICROS_IN_SEC;                \
  }

#define TO_TIMESTAMP_NANO_INTEGER(TYPE)                                    \
  FORCE_INLINE                                                             \
  gdv_timestamp_nano to_timestamp_nano##_##TYPE(gdv_##TYPE seconds) {      \
    return static_cast<gdv_int64>(seconds) * NANOS_IN_SEC;                 \
  }

// Similar for REAL types (with double → int64 cast)

INTEGER_NUMERIC_TYPES(TO_TIMESTAMP_MICRO_INTEGER)
REAL_NUMERIC_TYPES(TO_TIMESTAMP_MICRO_REAL)
INTEGER_NUMERIC_TYPES(TO_TIMESTAMP_NANO_INTEGER)
REAL_NUMERIC_TYPES(TO_TIMESTAMP_NANO_REAL)
```

### Registry

New `TO_TIMESTAMP_MICRO_SAFE_NULL_IF_NULL` / `TO_TIMESTAMP_NANO_SAFE_NULL_IF_NULL`
macros or explicit entries, following the pattern of the existing
`TO_TIMESTAMP_SAFE_NULL_IF_NULL`:

```cpp
#define TO_TIMESTAMP_MICRO_SAFE_NULL_IF_NULL(NAME, ALIASES, TYPE)         \
  NativeFunction(#NAME, std::vector<std::string> ALIASES,                 \
                 DataTypeVector{TYPE()}, timestamp_micro(),                \
                 kResultNullIfNull, ARROW_STRINGIFY(NAME##_##TYPE))

#define TO_TIMESTAMP_NANO_SAFE_NULL_IF_NULL(NAME, ALIASES, TYPE)          \
  NativeFunction(#NAME, std::vector<std::string> ALIASES,                 \
                 DataTypeVector{TYPE()}, timestamp_nano(),                 \
                 kResultNullIfNull, ARROW_STRINGIFY(NAME##_##TYPE))

// In GetDateTimeFunctionRegistry():
BASE_NUMERIC_TYPES(TO_TIMESTAMP_MICRO_SAFE_NULL_IF_NULL, to_timestamp_micro, {}),
BASE_NUMERIC_TYPES(TO_TIMESTAMP_NANO_SAFE_NULL_IF_NULL, to_timestamp_nano, {}),
```

---

## Test Plan

### `castVARCHAR`

- MICRO: `1615804245123456 µs` → `"2021-03-15 10:30:45.123456"` (26 chars)
- NANO: `1615804245123456789 ns` → `"2021-03-15 10:30:45.123456789"` (29 chars)
- Pre-epoch MICRO: `-1 µs` → `"1969-12-31 23:59:59.999999"`
- Truncation via `length` parameter: verify output respects the `int64` length
  argument, same as existing MILLI behavior.

### `castTIMESTAMP_utf8`

- 6-digit fractional: `"2021-01-01 12:00:00.123456"` → correct µs value
- 9-digit fractional: `"2021-01-01 12:00:00.123456789"` → correct ns value
- Fewer digits (zero-padding): `"2021-01-01 12:00:00.1"` into NANO →
  subsecond = `100000000 ns`
- More digits (truncation): `"2021-01-01 12:00:00.1234567899"` into NANO →
  subsecond = `123456789 ns` (truncated, not rounded)
- Out-of-range NANO date: `"2300-01-01 00:00:00"` → null output

### `castTIMESTAMP_date64`

- `0 ms` (epoch) → `0 µs` / `0 ns`
- `86400000 ms` (1 day) → `86400000000 µs` / `86400000000000 ns`

### `castTIMESTAMP_int64`

- `1` (1 second) → `1000000 µs` / `1000000000 ns`
- NANO overflow: value > `LLONG_MAX / NANOS_IN_SEC` → null / zero

### `castDATE_timestamp`

- MICRO: `1615804245123456 µs` → same `date64` as MILLI `1615804245123 ms`
- Pre-epoch: `-1 µs` → `date64` for `1969-12-31`

### `to_timestamp`

- `to_timestamp_micro_int64(60)` → `60000000 µs`
- `to_timestamp_nano_int64(60)` → `60000000000 ns`
- `to_timestamp_micro_float64(1.5)` → `1500000 µs`

## Files Changed

| File | Change |
|---|---|
| `precompiled/time.cc` | `castVARCHAR`, `castTIMESTAMP_utf8`, `castTIMESTAMP_date64`, `castTIMESTAMP_int64`, `castDATE`, `to_timestamp` implementations |
| `precompiled/types.h` | `extern` declarations for all new cast/conversion symbols |
| `function_registry_datetime.cc` | Registry entries for all new cast/conversion signatures |
| test file | Cast function tests for MICRO and NANO |
