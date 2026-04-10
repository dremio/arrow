# Phase 3: date_trunc Functions

**Prerequisite:** Phase 1 (types, constants, floor-division helpers, registry macro)
**Enables:** Independent of Phases 2 and 4

This phase adds MICRO and NANO implementations for all 11 existing `date_trunc`
levels plus 3 new sub-second levels.

---

## Approach

The existing `date_trunc` macros fall into two categories:

| Category | Levels | Mechanism | MICRO/NANO strategy |
|---|---|---|---|
| Pure integer math | Second, Minute, Hour, Day | `DATE_TRUNC_FIXED_UNIT` | Stamp out directly with MICRO/NANO constants |
| EpochTimePoint-based | Week, Month, Quarter, Year, Decade, Century, Millennium | `DATE_TRUNC_WEEK`, `DATE_TRUNC_MONTH_UNITS`, `DATE_TRUNC_YEAR_UNITS` | Delegation wrappers |

The delegation round-trip (convert to millis, delegate, convert back) is exact:
EpochTimePoint-based truncation always lands on a day boundary or coarser, so
the truncated millis value has no sub-millisecond remainder to lose.

## 3.1 Fixed-Unit Expansions (`precompiled/time.cc`)

These are pure integer math — no `EpochTimePoint`, no delegation needed:

```c
// MICRO — Second, Minute, Hour, Day
DATE_TRUNC_FIXED_UNIT(date_trunc_Second, timestamp_micro, MICROS_IN_SEC)
DATE_TRUNC_FIXED_UNIT(date_trunc_Minute, timestamp_micro, MICROS_IN_MIN)
DATE_TRUNC_FIXED_UNIT(date_trunc_Hour,   timestamp_micro, MICROS_IN_HOUR)
DATE_TRUNC_FIXED_UNIT(date_trunc_Day,    timestamp_micro, MICROS_IN_DAY)

// NANO — Second, Minute, Hour, Day
DATE_TRUNC_FIXED_UNIT(date_trunc_Second, timestamp_nano, NANOS_IN_SEC)
DATE_TRUNC_FIXED_UNIT(date_trunc_Minute, timestamp_nano, NANOS_IN_MIN)
DATE_TRUNC_FIXED_UNIT(date_trunc_Hour,   timestamp_nano, NANOS_IN_HOUR)
DATE_TRUNC_FIXED_UNIT(date_trunc_Day,    timestamp_nano, NANOS_IN_DAY)
```

## 3.2 EpochTimePoint Delegation Wrappers (`precompiled/time.cc`)

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
DATE_TRUNC_NANO(date_trunc_Month)
DATE_TRUNC_NANO(date_trunc_Quarter)
DATE_TRUNC_NANO(date_trunc_Year)
DATE_TRUNC_NANO(date_trunc_Decade)
DATE_TRUNC_NANO(date_trunc_Century)
DATE_TRUNC_NANO(date_trunc_Millennium)
```

## 3.3 New Sub-Second Levels (`precompiled/time.cc` + `function_registry_datetime.cc`)

These are new function names that only apply to finer-than-millisecond types.
They are pure integer math:

```c
// in time.cc
DATE_TRUNC_FIXED_UNIT(date_trunc_Millisecond, timestamp_micro, MICROS_IN_MILLIS)
DATE_TRUNC_FIXED_UNIT(date_trunc_Millisecond, timestamp_nano,  NANOS_IN_MILLIS)
DATE_TRUNC_FIXED_UNIT(date_trunc_Microsecond, timestamp_nano,  NANOS_IN_MICROS)
```

Because these are new function names (not covered by `DATE_EXTRACTION_TRUNCATION_FNS`),
they need explicit registry entries in `function_registry_datetime.cc`:

```cpp
// in GetDateTimeFunctionRegistry():
NativeFunction("date_trunc_Millisecond", {}, DataTypeVector{timestamp_micro()},
               timestamp_micro(), kResultNullIfNull,
               "date_trunc_Millisecond_timestamp_micro"),

NativeFunction("date_trunc_Millisecond", {}, DataTypeVector{timestamp_nano()},
               timestamp_nano(), kResultNullIfNull,
               "date_trunc_Millisecond_timestamp_nano"),

NativeFunction("date_trunc_Microsecond", {}, DataTypeVector{timestamp_nano()},
               timestamp_nano(), kResultNullIfNull,
               "date_trunc_Microsecond_timestamp_nano"),
```

Note: `date_trunc_Millisecond(timestamp)` and `date_trunc_Microsecond(timestamp_micro)`
would be identity functions and are intentionally not registered.

## Declarations (`precompiled/types.h`)

All new functions need `extern` declarations. Fixed-unit expansions (8),
delegation wrappers (14), and sub-second levels (3) = 25 declarations total.

---

## Test Plan

### Fixed-unit levels (Second, Minute, Hour, Day)

For each level, for both MICRO and NANO:

- **Post-epoch**: `2021-03-15 10:30:45.123456789` truncated to Second yields
  `2021-03-15 10:30:45.000000000`; to Minute yields `2021-03-15 10:30:00.000000000`;
  etc. Verify the result is in the correct unit (micros or nanos).

- **Pre-epoch**: `-1` (one unit before epoch). `date_trunc_Day` should yield
  `1969-12-31 00:00:00.000000` in MICRO (= `-86400000000 µs`), not `0`.

### EpochTimePoint-based levels (Week through Millennium)

- **Post-epoch**: `2021-03-15` (a Monday).
  - `date_trunc_Week` → `2021-03-15 00:00:00` (Monday)
  - `date_trunc_Month` → `2021-03-01 00:00:00`
  - `date_trunc_Quarter` → `2021-01-01 00:00:00`
  - `date_trunc_Year` → `2021-01-01 00:00:00`
  Verify results are in native units (µs or ns), not millis.

- **Pre-epoch**: `1969-06-15 12:00:00` — verify all levels produce correct
  truncation with negative epoch values.

- **Exactness check**: `date_trunc_Month` of `2021-03-15 10:30:45.123456 µs`
  should yield exactly `2021-03-01 00:00:00.000000 µs` — the multiply-back
  introduces no sub-millisecond artifact.

### Sub-second levels

- `date_trunc_Millisecond(1234567 µs)` → `1234000 µs`
- `date_trunc_Millisecond(1234567890 ns)` → `1234000000 ns`
- `date_trunc_Microsecond(1234567890 ns)` → `1234567000 ns`
- Pre-epoch: `date_trunc_Millisecond(-1 µs)` → `-1000 µs` (not `0`)

## Files Changed

| File | Change |
|---|---|
| `precompiled/time.cc` | Fixed-unit expansions (8), `DATE_TRUNC_MICRO`/`DATE_TRUNC_NANO` macros + invocations (14), sub-second levels (3) |
| `precompiled/types.h` | 25 `extern` declarations |
| `function_registry_datetime.cc` | 3 explicit sub-second registry entries |
| test file | date_trunc tests for MICRO and NANO |
