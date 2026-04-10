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
`Timestamp(MILLI)`: all extract functions, all `date_trunc` levels, and cast
functions.

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
- **Timestamp arithmetic** (`timestampadd`, `timestampdiff`): not currently
  registered in Gandiva for any precision. Out of scope.
- **`datediff`, `to_utc_timestamp`, `from_utc_timestamp`**: registered for MILLI
  but deferred pending scope decision (see Open Questions).

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

### The `EpochTimePoint` Conversion Problem

`EpochTimePoint` (`precompiled/epoch_time_point.h`) is hardcoded to millisecond
precision — its constructor takes `int64_t millis_since_epoch` and it stores a
`time_point<system_clock, milliseconds>`. Multiple macro families pass their
input directly to `EpochTimePoint` without conversion:

- **Extract macros** (`EXTRACT_YEAR`, `EXTRACT_MONTH`, etc.) construct
  `EpochTimePoint(millis)` — passing microseconds or nanoseconds would be
  misinterpreted as milliseconds, producing wrong dates.
- **`DATE_TRUNC_WEEK`**, **`DATE_TRUNC_MONTH_UNITS`**, and
  **`DATE_TRUNC_YEAR_UNITS`** construct `EpochTimePoint` *and* return
  `.MillisSinceEpoch()` — both input and output would be wrong.
- **`DATE_TRUNC_FIXED_UNIT`** is pure integer math (no `EpochTimePoint`) and
  is safe to use directly with MICRO/NANO constants.

Simply extending `DATE_TYPES` in `time.cc` to include `timestamp_micro` and
`timestamp_nano` would silently produce wrong results for all
EpochTimePoint-dependent functions.

Three approaches were considered:

**Approach A — Add conversion parameters to existing macros.** Modify each
macro to accept `TO_MILLIS` / `FROM_MILLIS` parameters and change the
`DATE_TYPES` macro signature to pass converters alongside the type. This reuses
existing macro bodies but changes the `DATE_TYPES` contract in `time.cc`,
rippling to every consumer. The two-file `DATE_TYPES` divergence (registry has
3-arg `(INNER, NAME, ALIASES)`, implementation has 1-arg `(INNER)`) would grow
into a 3-arg implementation variant, increasing macro complexity.

**Approach B — Delegation wrappers for MICRO/NANO.** Leave existing macros and
`DATE_TYPES` in `time.cc` unchanged. MICRO/NANO extract functions delegate to
the existing MILLI implementation after converting the input; MICRO/NANO
EpochTimePoint-based `date_trunc` functions delegate to the MILLI
implementation then convert the result back. `DATE_TRUNC_FIXED_UNIT`-based
functions (Second, Minute, Hour, Day) are stamped out directly with MICRO/NANO
constants since they are pure integer math. Zero changes to existing code paths.

**Approach C — Templatize `EpochTimePoint`.** Make `EpochTimePoint` a class
template parameterized on `Duration` (`milliseconds`, `microseconds`,
`nanoseconds`), replace `MillisSinceEpoch()` with `UnitsSinceEpoch()`, and
dispatch via `EPT_FOR_##TYPE` macros. This is the cleanest long-term design but
modifies shared infrastructure, requires verifying that templated C++ compiles
correctly through Gandiva's clang-to-LLVM-bitcode precompilation pipeline, and
has the largest blast radius.

**Chosen: Approach B (delegation wrappers).** It carries zero risk to existing
MILLI/date64 behavior and requires no changes to `EpochTimePoint` or the
existing macro structure. The delegation round-trip (convert to millis →
delegate → convert back) is exact for EpochTimePoint-based `date_trunc`
because truncation to day/month/year boundaries always zeroes out the
sub-millisecond remainder. The maintenance cost — parallel wrapper lists that
must be updated if new extract/trunc functions are added upstream — is
acceptable given the function set is stable.

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
  + explicit sub-second date_trunc        DATE_TYPES unchanged (date64, timestamp)
    registrations                         EXTRACT_MICRO / EXTRACT_NANO wrappers
                                          DATE_TRUNC_MICRO / DATE_TRUNC_NANO wrappers
                                          DATE_TRUNC_FIXED_UNIT for sub-day MICRO/NANO
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

**Cast functions**: `castTIMESTAMP_utf8`, `castTIMESTAMP_date64`,
`castTIMESTAMP_int64`, `castDATE_timestamp`, `castVARCHAR_timestamp_int64`,
`to_timestamp_*`

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

Cast inputs are detectable as out-of-range before the multiply
(`|seconds| > LLONG_MAX / NANOS_IN_SEC`); those return null.

### Backward Compatibility

No existing function signatures, return types, or behaviors change. All
additions are new registry entries alongside existing ones.

## Code Changes

### Files Changed

| File | Change |
|---|---|
| `cpp/src/gandiva/precompiled/types.h` | Add `gdv_timestamp_micro`, `gdv_timestamp_nano` type aliases |
| `cpp/src/gandiva/precompiled/time_constants.h` | Add `MICROS_IN_*` and `NANOS_IN_*` `#define` constants |
| `cpp/src/gandiva/precompiled/time.cc` | Delegation wrappers (extract, date_trunc); direct `DATE_TRUNC_FIXED_UNIT` expansions; cast implementations |
| `cpp/src/gandiva/function_registry_common.h` | `timestamp_micro()`, `timestamp_nano()` helpers; extend registry `DATE_TYPES` macro |
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

### Macro Extension (`function_registry_common.h`)

`DATE_TYPES` exists in two forms with different signatures. Only the registry
form is extended; the implementation form in `time.cc` is left unchanged.

**`function_registry_common.h`** (drives registry entry generation) — extended
to 4 entries so all extract and `date_trunc` functions are automatically
registered for MICRO and NANO:
```cpp
inline DataTypePtr timestamp_micro() { return arrow::timestamp(arrow::TimeUnit::MICRO); }
inline DataTypePtr timestamp_nano()  { return arrow::timestamp(arrow::TimeUnit::NANO);  }

#define DATE_TYPES(INNER, NAME, ALIASES)       \
  INNER(NAME, ALIASES, date64),                \
  INNER(NAME, ALIASES, timestamp),             \
  INNER(NAME, ALIASES, timestamp_micro),       \
  INNER(NAME, ALIASES, timestamp_nano)
```

**`precompiled/time.cc`** — `DATE_TYPES(INNER)` is **not extended**. It
remains:
```c
#define DATE_TYPES(INNER) \
  INNER(date64)           \
  INNER(timestamp)
```

This is intentional: the existing macros construct `EpochTimePoint` with the
raw value and assume milliseconds (see "The `EpochTimePoint` Conversion
Problem" above). MICRO/NANO implementations are provided via delegation
wrappers instead.

### Extract Functions (`precompiled/time.cc`)

Coarser-than-second extract functions use `EpochTimePoint`, which expects
milliseconds. MICRO/NANO values must be floor-divided to millis before
delegating — plain C integer division truncates toward zero, which gives
the wrong year/month/day for pre-epoch timestamps:

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
`DATE_TRUNC_FIXED_UNIT`.

MICRO/NANO extract functions are implemented as delegation wrappers that
convert to millis and call the existing `_timestamp` implementation:

```c
#define EXTRACT_MICRO(NAME)                                              \
  FORCE_INLINE                                                           \
  gdv_int64 NAME##_timestamp_micro(gdv_timestamp_micro micros) {         \
    return NAME##_timestamp(MICROS_TO_MILLIS_FLOOR(micros));             \
  }

#define EXTRACT_NANO(NAME)                                               \
  FORCE_INLINE                                                           \
  gdv_int64 NAME##_timestamp_nano(gdv_timestamp_nano nanos) {            \
    return NAME##_timestamp(NANOS_TO_MILLIS_FLOOR(nanos));               \
  }

EXTRACT_MICRO(extractYear)
EXTRACT_MICRO(extractQuarter)
EXTRACT_MICRO(extractMonth)
EXTRACT_MICRO(extractWeek)
EXTRACT_MICRO(extractDay)
EXTRACT_MICRO(extractDow)
EXTRACT_MICRO(extractDoy)
EXTRACT_MICRO(extractHour)
EXTRACT_MICRO(extractMinute)
EXTRACT_MICRO(extractSecond)
EXTRACT_MICRO(extractEpoch)
EXTRACT_MICRO(extractDecade)
EXTRACT_MICRO(extractCentury)
EXTRACT_MICRO(extractMillennium)

EXTRACT_NANO(extractYear)
// ... same list for NANO ...
```

Each wrapper compiles to a floor-division + tail call. With `FORCE_INLINE`, the
LLVM backend should inline the delegation away entirely, meeting the
performance objective.

### `date_trunc_*` Functions (`precompiled/time.cc`)

The existing `date_trunc` macros fall into two categories that require
different treatment:

**Pure integer math (`DATE_TRUNC_FIXED_UNIT`):** Used for Second, Minute, Hour,
Day. These have no `EpochTimePoint` dependency and are stamped out directly
with MICRO/NANO constants:

```c
// MICRO
DATE_TRUNC_FIXED_UNIT(date_trunc_Second, timestamp_micro, MICROS_IN_SEC)
DATE_TRUNC_FIXED_UNIT(date_trunc_Minute, timestamp_micro, MICROS_IN_MIN)
DATE_TRUNC_FIXED_UNIT(date_trunc_Hour,   timestamp_micro, MICROS_IN_HOUR)
DATE_TRUNC_FIXED_UNIT(date_trunc_Day,    timestamp_micro, MICROS_IN_DAY)

// NANO
DATE_TRUNC_FIXED_UNIT(date_trunc_Second, timestamp_nano, NANOS_IN_SEC)
DATE_TRUNC_FIXED_UNIT(date_trunc_Minute, timestamp_nano, NANOS_IN_MIN)
DATE_TRUNC_FIXED_UNIT(date_trunc_Hour,   timestamp_nano, NANOS_IN_HOUR)
DATE_TRUNC_FIXED_UNIT(date_trunc_Day,    timestamp_nano, NANOS_IN_DAY)
```

**EpochTimePoint-based (`DATE_TRUNC_WEEK`, `DATE_TRUNC_MONTH_UNITS`,
`DATE_TRUNC_YEAR_UNITS`):** Used for Week, Month, Quarter, Year, Decade,
Century, Millennium. These construct `EpochTimePoint(millis)` and return
`.MillisSinceEpoch()`, so they cannot be directly expanded for MICRO/NANO.
Instead, delegation wrappers convert in, delegate to the existing `_timestamp`
implementation, and convert back:

```c
#define DATE_TRUNC_MICRO(NAME)                                                  \
  FORCE_INLINE                                                                  \
  gdv_timestamp_micro NAME##_timestamp_micro(gdv_timestamp_micro micros) {      \
    gdv_timestamp millis_result =                                               \
        NAME##_timestamp(MICROS_TO_MILLIS_FLOOR(micros));                       \
    return millis_result * MICROS_IN_MILLIS;                                    \
  }

#define DATE_TRUNC_NANO(NAME)                                                   \
  FORCE_INLINE                                                                  \
  gdv_timestamp_nano NAME##_timestamp_nano(gdv_timestamp_nano nanos) {          \
    gdv_timestamp millis_result =                                               \
        NAME##_timestamp(NANOS_TO_MILLIS_FLOOR(nanos));                         \
    return millis_result * NANOS_IN_MILLIS;                                     \
  }

DATE_TRUNC_MICRO(date_trunc_Week)
DATE_TRUNC_MICRO(date_trunc_Month)
DATE_TRUNC_MICRO(date_trunc_Quarter)
DATE_TRUNC_MICRO(date_trunc_Year)
DATE_TRUNC_MICRO(date_trunc_Decade)
DATE_TRUNC_MICRO(date_trunc_Century)
DATE_TRUNC_MICRO(date_trunc_Millennium)

DATE_TRUNC_NANO(date_trunc_Week)
// ... same list for NANO ...
```

The `millis_result * MICROS_IN_MILLIS` multiply-back is exact: EpochTimePoint-
based truncation always lands on a day boundary (or coarser), so the truncated
millis value has no sub-millisecond remainder to lose.

**New sub-second truncation levels** are pure integer math, registered
explicitly in `function_registry_datetime.cc` since they do not exist for all
types:

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

- **`datediff`, `to_utc_timestamp`, `from_utc_timestamp`**: these are registered
  for `Timestamp(MILLI)` but not mentioned in scope. Should MICRO/NANO variants
  be added in this contribution or deferred to a follow-up?

## References

- Apache Arrow Gandiva source:
  `cpp/src/gandiva/precompiled/time.cc`,
  `cpp/src/gandiva/function_registry_datetime.cc`
- SQL Server `DATEPART` documentation (sub-second semantics reference):
  https://learn.microsoft.com/en-us/sql/t-sql/functions/datepart-transact-sql
