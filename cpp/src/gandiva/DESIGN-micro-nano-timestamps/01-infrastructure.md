# Phase 1: Infrastructure

**Prerequisite:** None
**Enables:** All subsequent phases

This phase introduces all shared plumbing that later phases depend on. No new
Gandiva function signatures are registered or implemented — only types,
constants, macros, and conversion helpers.

---

## 1.1 Type Aliases (`precompiled/types.h`)

```c
using gdv_timestamp       = int64_t;  // existing: milliseconds since epoch
using gdv_timestamp_micro = int64_t;  // new: microseconds since epoch
using gdv_timestamp_nano  = int64_t;  // new: nanoseconds since epoch
```

These are documentation aliases — the LLVM type is `i64` in all three cases.
The C function name encodes the unit (`extractYear_timestamp_micro`), which is
how Gandiva's registry links the symbol to the correct Arrow type signature.

## 1.2 Unit Constants (`precompiled/time_constants.h`)

Append to the existing `MILLIS_IN_*` block:

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

## 1.3 Registry Macro Extension (`function_registry_common.h`)

Add type helpers and extend the registry `DATE_TYPES` macro so that all
existing `DATE_EXTRACTION_TRUNCATION_FNS` registrations automatically cover
MICRO and NANO:

```cpp
inline DataTypePtr timestamp_micro() { return arrow::timestamp(arrow::TimeUnit::MICRO); }
inline DataTypePtr timestamp_nano()  { return arrow::timestamp(arrow::TimeUnit::NANO);  }

#define DATE_TYPES(INNER, NAME, ALIASES)       \
  INNER(NAME, ALIASES, date64),                \
  INNER(NAME, ALIASES, timestamp),             \
  INNER(NAME, ALIASES, timestamp_micro),       \
  INNER(NAME, ALIASES, timestamp_nano)
```

**Important:** The `DATE_TYPES(INNER)` macro in `precompiled/time.cc` is
**not extended**. It remains `INNER(date64) INNER(timestamp)`. The existing
implementation macros construct `EpochTimePoint` with the raw value and assume
milliseconds — see "The `EpochTimePoint` Conversion Problem" in the parent
design doc. MICRO/NANO implementations are provided via delegation wrappers
in later phases.

## 1.4 Floor-Division Helpers (`precompiled/time.cc`)

Plain C integer division truncates toward zero, which gives wrong
year/month/day for pre-epoch timestamps when converting finer units to millis:

```c
// -1 µs / 1000 = 0 in C  → extractYear would return 1970 (wrong)
// floor(-1 µs / 1000) = -1 ms → extractYear returns 1969 (correct)

#define MICROS_TO_MILLIS_FLOOR(v) \
  ((v) >= 0 ? (v) / MICROS_IN_MILLIS \
            : ((v) - MICROS_IN_MILLIS + 1) / MICROS_IN_MILLIS)

#define NANOS_TO_MILLIS_FLOOR(v) \
  ((v) >= 0 ? (v) / NANOS_IN_MILLIS \
            : ((v) - NANOS_IN_MILLIS + 1) / NANOS_IN_MILLIS)
```

This pattern is consistent with the floor division already used in
`DATE_TRUNC_FIXED_UNIT`.

---

## Test Plan

Phase 1 introduces no callable functions, so there are no functional tests.
Verification:

- **Build**: Gandiva compiles cleanly with the new types, constants, and macro
  changes. No existing tests regress.
- **Registry smoke test**: the extended `DATE_TYPES` macro causes new
  signatures to be registered (e.g., `extractYear(timestamp_micro) -> int64`).
  A test can assert these signatures exist in `GetDateTimeFunctionRegistry()`.
  At this point the symbols they reference (`extractYear_timestamp_micro`, etc.)
  do not yet exist — the registry entries are present but not yet resolvable.
  Attempting to compile an expression using them should produce a clear
  "function not found" or link error, not a silent wrong result.

## Files Changed

| File | Change |
|---|---|
| `precompiled/types.h` | Add `gdv_timestamp_micro`, `gdv_timestamp_nano` aliases + declarations for Phase 2-4 functions |
| `precompiled/time_constants.h` | Add `MICROS_IN_*` and `NANOS_IN_*` constants |
| `function_registry_common.h` | `timestamp_micro()`, `timestamp_nano()` helpers; extend registry `DATE_TYPES` |
| `precompiled/time.cc` | `MICROS_TO_MILLIS_FLOOR`, `NANOS_TO_MILLIS_FLOOR` macros |
