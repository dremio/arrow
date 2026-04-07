# Gandiva: Add Microsecond and Nanosecond Timestamp Support

**Author:** Tim Hurski  
**Date:** 2026-04-01  
**Status:** Draft

---

## Overview

Apache Arrow defines three timestamp precision types: `Timestamp(MILLI)`,
`Timestamp(MICRO)`, and `Timestamp(NANO)`. Gandiva — Arrow's expression
compilation layer — currently registers functions only for `Timestamp(MILLI)`.
Any expression (`extract`, `date_trunc`, cast) over a microsecond or
nanosecond timestamp column therefore cannot be compiled by Gandiva, forcing
consumers to fall back to a non-LLVM execution path.

This document proposes adding native `Timestamp(MICRO)` and `Timestamp(NANO)`
support to Gandiva, covering the full function set currently available for
`Timestamp(MILLI)`: all extract functions, all `date_trunc` levels, cast
functions, and timestamp arithmetic.

**Performance objective:** Expression evaluation over MICRO/NANO timestamp
columns should match MILLI throughput — no additional branching per row.

## Non-Goals

- **Sub-second extract functions** (`extractMillisecond`, `extractMicrosecond`,
  `extractNanosecond`): out of scope for this contribution. These are a
  meaningful addition but belong in a follow-up PR to keep the diff reviewable.
- **Fixing the pre-1970 `castVARCHAR` bug**: a pre-existing issue in the MILLI
  implementation. The MICRO/NANO implementations will carry the same behavior;
  this is not addressed in this contribution.
- **Java / Flight SQL / Substrait layers**: Gandiva C++ only. Consumers that
  already route expressions to Gandiva need no changes once the registry
  contains the new signatures.
- **Runtime overflow detection for arithmetic**: overflow behavior is documented
  rather than checked (see Overflow Behavior section).

## System Design

### Current State

Gandiva's datetime support is driven by two macro expansion systems:

**`function_registry_common.h`** defines type helpers and iteration macros used
when registering functions:

```cpp
inline DataTypePtr timestamp() { return arrow::timestamp(arrow::TimeUnit::MILLI); }

#define DATE_TYPES(INNER, NAME, ALIASES) \
  INNER(NAME, ALIASES, date64), INNER(NAME, ALIASES, timestamp)
```

`DATE_TYPES` is consumed by `DATE_EXTRACTION_TRUNCATION_FNS` and registered in
`GetDateTimeFunctionRegistry()` in `function_registry_datetime.cc`:

```cpp
DATE_EXTRACTION_TRUNCATION_FNS(EXTRACT_SAFE_NULL_IF_NULL, extract),
DATE_EXTRACTION_TRUNCATION_FNS(TRUNCATE_SAFE_NULL_IF_NULL, date_trunc_),
```

`EXTRACT_SAFE_NULL_IF_NULL` stringifies the C symbol name:

```cpp
#define EXTRACT_SAFE_NULL_IF_NULL(NAME, ALIASES, TYPE)                            \
  NativeFunction(#NAME, ..., DataTypeVector{TYPE()}, int64(), kResultNullIfNull,  \
                 ARROW_STRINGIFY(NAME##_##TYPE))
```

So `extractYear` for `timestamp` registers the symbol `extractYear_timestamp`.

**`precompiled/time.cc`** has its own `DATE_TYPES(INNER)` macro (different
signature — no `NAME` or `ALIASES`) used to stamp out C implementations:

```c
#define DATE_TYPES(INNER) \
  INNER(date64)           \
  INNER(timestamp)

DATE_TYPES(EXTRACT_YEAR)    // → extractYear_date64, extractYear_timestamp
DATE_TRUNC_FUNCTIONS(timestamp)
```

`DATE_TRUNC_FIXED_UNIT` — the core `date_trunc` macro — already implements
correct floor division for pre-epoch timestamps:

```c
#define DATE_TRUNC_FIXED_UNIT(NAME, TYPE, N)                              \
  FORCE_INLINE gdv_##TYPE NAME##_##TYPE(gdv_##TYPE millis) {              \
    return millis >= 0 ? ((millis / N) * N)                               \
                       : (((millis - N + 1) / N) * N);                   \
  }
```

Extract functions delegate to `EpochTimePoint`, which takes milliseconds:

```c
FORCE_INLINE gdv_int64 extractYear_timestamp(gdv_timestamp millis) {
  EpochTimePoint tp(millis);
  return 1900 + tp.TmYear();
}
```

### Target State

```
function_registry_common.h              precompiled/time_constants.h
  timestamp_micro() → MICRO               MICROS_IN_SEC, MICROS_IN_DAY …
  timestamp_nano()  → NANO                NANOS_IN_SEC, NANOS_IN_DAY …
  DATE_TYPES extended → 4 entries       precompiled/types.h
                                           gdv_timestamp_micro = int64_t
function_registry_datetime.cc             gdv_timestamp_nano  = int64_t
  all existing registrations now
  auto-expanded for MICRO and NANO      precompiled/time.cc
  + explicit sub-second date_trunc        DATE_TYPES extended → 4 entries
    registrations                         MICROS_TO_MILLIS_FLOOR / NANOS_TO_MILLIS_FLOOR
                                          extract variants via EpochTimePoint
                                          date_trunc via DATE_TRUNC_FIXED_UNIT
                                          castVARCHAR extended format/length
                                          castTIMESTAMP_utf8 extended parser
```

### Valid Timestamp Ranges

Since all three types use `int64` storage, representable range narrows with
finer precision:

| Unit | Storage | Min date | Max date |
|---|---|---|---|
| MILLI | ms since epoch | ~year −292,271,023 | ~year +292,271,023 |
| MICRO | µs since epoch | ~year −290,308 | ~year +292,278 |
| NANO | ns since epoch | **~1677-09-21** | **~2262-04-11** |

The NANO range (~585 years) is a meaningful operational constraint. It is
documented in all affected function headers. Runtime range checks are not added
for arithmetic operations (see Overflow Behavior).

## API Changes

### New Function Signatures

All functions currently registered for `Timestamp(MILLI)` gain parity
registrations for `Timestamp(MICRO)` and `Timestamp(NANO)`. This includes
both UTC and timezone-aware variants.

**Extract functions** (return `int64`):
`extractYear`, `extractQuarter`, `extractMonth`, `extractWeek`,
`extractDay`, `extractDow`, `extractDoy`, `extractHour`, `extractMinute`,
`extractSecond`, `extractEpoch`, `extractDecade`, `extractCentury`,
`extractMillennium`

**`date_trunc_*` functions** (return timestamp in same unit):
`date_trunc_Year`, `date_trunc_Quarter`, `date_trunc_Month`, `date_trunc_Week`,
`date_trunc_Day`, `date_trunc_Hour`, `date_trunc_Minute`, `date_trunc_Second`,
`date_trunc_Decade`, `date_trunc_Century`, `date_trunc_Millennium`

**New `date_trunc` levels** (only meaningful for sub-millisecond types):

| Function | MILLI | MICRO | NANO |
|---|---|---|---|
| `date_trunc_Millisecond` | identity — not registered | new | new |
| `date_trunc_Microsecond` | N/A | identity — not registered | new |

**Cast and arithmetic functions**: `castTIMESTAMP_utf8`, `castTIMESTAMP_date64`,
`castTIMESTAMP_int64`, `castDATE_timestamp`, `castVARCHAR_timestamp_int64`,
`to_timestamp_*`, `timestampadd_*`, `timestampdiff_*`

### `castVARCHAR` Format Extension

The output format is extended to show full sub-second precision:

| Unit | Length | Format |
|---|---|---|
| MILLI (existing) | 23 | `yyyy-MM-dd HH:mm:ss.SSS` |
| MICRO | 26 | `yyyy-MM-dd HH:mm:ss.SSSSSS` |
| NANO | 29 | `yyyy-MM-dd HH:mm:ss.SSSSSSSSS` |

### `castTIMESTAMP_utf8` Parsing

The string parser currently normalizes fractional seconds to 3 digits via
`normalize_subseconds_to_millis`. Two new parallel helpers are added:
`normalize_subseconds_to_micros` (6 digits) and
`normalize_subseconds_to_nanos` (9 digits). Inputs with fewer fractional digits
than the target precision are zero-padded on the right (left-anchored); inputs
with more digits are truncated, not rounded.

Example: `"2021-01-01 12:00:00.1"` parsed into NANO = `100,000,000 ns`.

### Overflow Behavior

Consistent with Arrow's existing conventions — no runtime checks; behavior is
documented in function header comments:

| Operation | MICRO | NANO | Treatment |
|---|---|---|---|
| `castTIMESTAMP_int64` / `to_timestamp_*` out of range | safe | overflows past 2262/1677 | **null output** |
| `castTIMESTAMP_utf8` out of range date | safe | overflows past 2262/1677 | **null output** |
| `timestampadd_*` interval multiply | rare | operational (>~107K days) | documented undefined |
| `timestampdiff_*` subtraction at range extremes | safe | overflows (range ~585 yrs) | documented undefined |

Cast inputs are detectable as out-of-range before the multiply
(`|seconds| > LLONG_MAX / NANOS_IN_SEC`); those return null. Arithmetic
overflows are not checked, matching the behavior of existing MILLI arithmetic
functions under equivalent conditions.

### Backward Compatibility

No existing function signatures, return types, or behaviors change. All
additions are new registry entries alongside existing ones.

## Code Changes

### Files Changed

| File | Change |
|---|---|
| `cpp/src/gandiva/precompiled/types.h` | Add `gdv_timestamp_micro`, `gdv_timestamp_nano` type aliases |
| `cpp/src/gandiva/precompiled/time_constants.h` | Add `MICROS_IN_*` and `NANOS_IN_*` `#define` constants |
| `cpp/src/gandiva/precompiled/time.cc` | All new implementations (extract, date_trunc, cast, arithmetic) |
| `cpp/src/gandiva/function_registry_common.h` | `timestamp_micro()`, `timestamp_nano()` helpers; extend both `DATE_TYPES` macros |
| `cpp/src/gandiva/function_registry_datetime.cc` | Register all new signatures; explicit sub-second `date_trunc` entries |
| `cpp/src/gandiva/gandiva_test.cc` (or new file) | Unit tests |

### Type Aliases (`precompiled/types.h`)

```c
using gdv_timestamp       = int64_t;  // existing: milliseconds since epoch
using gdv_timestamp_micro = int64_t;  // new: microseconds since epoch
using gdv_timestamp_nano  = int64_t;  // new: nanoseconds since epoch
```

These are documentation aliases — the LLVM type is `i64` in all three cases.
The C function name encodes the unit (`extractYear_timestamp_micro`), which is
how Gandiva's registry links the symbol to the correct Arrow type signature.

### Unit Constants (`precompiled/time_constants.h`)

```c
// Microsecond constants (following existing MILLIS_IN_* style)
#define MICROS_IN_MILLIS  INT64_C(1000)
#define MICROS_IN_SEC     INT64_C(1000000)
#define MICROS_IN_MIN     INT64_C(60000000)
#define MICROS_IN_HOUR    INT64_C(3600000000)
#define MICROS_IN_DAY     INT64_C(86400000000)
#define MICROS_IN_WEEK    INT64_C(604800000000)

// Nanosecond constants
#define NANOS_IN_MICROS   INT64_C(1000)
#define NANOS_IN_MILLIS   INT64_C(1000000)
#define NANOS_IN_SEC      INT64_C(1000000000)
#define NANOS_IN_MIN      INT64_C(60000000000)
#define NANOS_IN_HOUR     INT64_C(3600000000000)
#define NANOS_IN_DAY      INT64_C(86400000000000)
#define NANOS_IN_WEEK     INT64_C(604800000000000)
```

### Macro Extension (`function_registry_common.h` and `precompiled/time.cc`)

`DATE_TYPES` exists in two forms — both must be extended.

**`function_registry_common.h`** (drives registry entry generation):
```cpp
inline DataTypePtr timestamp_micro() { return arrow::timestamp(arrow::TimeUnit::MICRO); }
inline DataTypePtr timestamp_nano()  { return arrow::timestamp(arrow::TimeUnit::NANO);  }

#define DATE_TYPES(INNER, NAME, ALIASES)       \
  INNER(NAME, ALIASES, date64),                \
  INNER(NAME, ALIASES, timestamp),             \
  INNER(NAME, ALIASES, timestamp_micro),       \
  INNER(NAME, ALIASES, timestamp_nano)
```

**`precompiled/time.cc`** (drives C function implementation generation):
```c
#define DATE_TYPES(INNER) \
  INNER(date64)           \
  INNER(timestamp)        \
  INNER(timestamp_micro)  \
  INNER(timestamp_nano)
```

These two extensions together propagate all macro-generated implementations
and registrations to both new types with no further per-function changes.

### Extract Functions (`precompiled/time.cc`)

Coarser-than-second extract functions use `EpochTimePoint`, which expects
milliseconds. MICRO/NANO values must be floor-divided to millis before
constructing `EpochTimePoint` — plain C integer division truncates toward zero,
which gives the wrong year/month/day for pre-epoch timestamps:

```c
// e.g. -1 µs / 1000 = 0 in C  → extractYear returns 1970 (wrong)
//      floor(-1 µs / 1000) = -1 ms → extractYear returns 1969 (correct)

#define MICROS_TO_MILLIS_FLOOR(v) \
  ((v) >= 0 ? (v) / MICROS_IN_MILLIS \
            : ((v) - MICROS_IN_MILLIS + 1) / MICROS_IN_MILLIS)

#define NANOS_TO_MILLIS_FLOOR(v) \
  ((v) >= 0 ? (v) / NANOS_IN_MILLIS \
            : ((v) - NANOS_IN_MILLIS + 1) / NANOS_IN_MILLIS)
```

This pattern is consistent with the floor division already used in
`DATE_TRUNC_FIXED_UNIT`. Example expansion (generated via `DATE_TYPES` macro):

```c
FORCE_INLINE gdv_int64 extractYear_timestamp_micro(gdv_timestamp_micro micros) {
  EpochTimePoint tp(MICROS_TO_MILLIS_FLOOR(micros));
  return 1900 + tp.TmYear();
}
```

### `date_trunc_*` Functions (`precompiled/time.cc`)

`DATE_TRUNC_FIXED_UNIT` is parameterized by the unit constant, so MICRO/NANO
expansions simply pass the appropriate constant. Floor division correctness
is already built into the macro:

```c
// Generated for MICRO by DATE_TRUNC_FUNCTIONS(timestamp_micro):
DATE_TRUNC_FIXED_UNIT(date_trunc_Second, timestamp_micro, MICROS_IN_SEC)
// expands to:
FORCE_INLINE gdv_timestamp_micro
date_trunc_Second_timestamp_micro(gdv_timestamp_micro micros) {
  return micros >= 0 ? ((micros / MICROS_IN_SEC) * MICROS_IN_SEC)
                     : (((micros - MICROS_IN_SEC + 1) / MICROS_IN_SEC) * MICROS_IN_SEC);
}
```

The sub-second truncation levels are new function names registered explicitly
in `function_registry_datetime.cc` since they do not exist for all types:

```c
DATE_TRUNC_FIXED_UNIT(date_trunc_Millisecond, timestamp_micro, MICROS_IN_MILLIS)
DATE_TRUNC_FIXED_UNIT(date_trunc_Millisecond, timestamp_nano,  NANOS_IN_MILLIS)
DATE_TRUNC_FIXED_UNIT(date_trunc_Microsecond, timestamp_nano,  NANOS_IN_MICROS)
```

### `castVARCHAR` (`precompiled/time.cc`)

Subsecond digits for MICRO/NANO cannot use `EpochTimePoint.TimeOfDay().subseconds()`
(millisecond resolution only). They are computed directly from the epoch value:

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

Format strings and buffer lengths are unit-specific:

```c
// MICRO — kTimeStampStringLen = 26
snprintf(..., "%04" PRId64 "-%02" PRId64 "-%02" PRId64
              " %02" PRId64 ":%02" PRId64 ":%02" PRId64 ".%06" PRId64,
         year, month, day, hour, minute, second, micros_of_sec);

// NANO — kTimeStampStringLen = 29
snprintf(..., "%04" PRId64 "-%02" PRId64 "-%02" PRId64
              " %02" PRId64 ":%02" PRId64 ":%02" PRId64 ".%09" PRId64,
         year, month, day, hour, minute, second, nanos_of_sec);
```

**Known pre-existing issue:** `castVARCHAR_timestamp_int64` has a bug for
pre-1970 timestamps in the MILLI implementation. The MICRO/NANO implementations
will carry the same behavior; this is not fixed in this change.

## Test Strategy

Tests target `cpp/src/gandiva/` and follow Arrow's existing test conventions,
in `gandiva_test.cc` or a new `gandiva_datetime_precision_test.cc`.

- **Extract functions** (`extractYear` → `extractDecade`): both MICRO and NANO,
  with at least one post-epoch and one pre-epoch value per function. Pre-epoch
  cases explicitly validate the floor-to-millis conversion — e.g., `-1 µs`
  (1969-12-31 23:59:59.999999) must yield year 1969, not 1970.

- **`date_trunc_*`**: all existing levels for MICRO and NANO; new sub-second
  levels (`date_trunc_Millisecond`, `date_trunc_Microsecond`); pre-epoch values
  for each.

- **`castVARCHAR`**: 6 and 9 decimal place output for representative MICRO and
  NANO timestamps.

- **`castTIMESTAMP_utf8`**: 6-digit and 9-digit fractional seconds; fewer digits
  (left-anchor zero-padding); more digits (truncation, not rounding);
  out-of-range NANO date produces null output.

- **`castTIMESTAMP_int64`**: out-of-range epoch-seconds input for NANO returns
  null.

- **`date_trunc_Millisecond` / `date_trunc_Microsecond`**: truncation to correct
  boundary in native units, with pre-epoch values.

- **Timezone variants**: representative extract and `date_trunc` functions with
  non-UTC offsets for MICRO and NANO.

- **Registry smoke test**: all new signatures present; no existing MILLI
  signatures removed or altered.

Pre-epoch correctness is the highest-risk area — the floor-to-millis conversion
is easy to get wrong with naive C division — so every function family includes
at least one pre-epoch test. Overflow behavior for arithmetic is
documented-undefined and is not tested, consistent with how existing MILLI
arithmetic overflow is handled.

## Open Questions

None — all design decisions are resolved.

## References

- Apache Arrow Gandiva source:
  `cpp/src/gandiva/precompiled/time.cc`,
  `cpp/src/gandiva/function_registry_datetime.cc`
- SQL Server `DATEPART` documentation (sub-second semantics reference):
  https://learn.microsoft.com/en-us/sql/t-sql/functions/datepart-transact-sql
