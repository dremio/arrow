# Phase 2: Extract Functions

**Prerequisite:** Phase 1 (types, constants, floor-division helpers, registry macro)
**Enables:** Independent of Phases 3 and 4

This phase adds MICRO and NANO implementations for all 14 extract functions.

---

## Approach

All extract functions delegate to `EpochTimePoint`, which is hardcoded to
milliseconds. Rather than modifying `EpochTimePoint` or the existing extract
macros, MICRO/NANO variants are thin delegation wrappers: convert to millis
via floor-division, then call the existing `_timestamp` implementation.

Each wrapper compiles to a floor-division + tail call. With `FORCE_INLINE`,
the LLVM backend should inline the delegation away entirely, meeting the
performance objective of no additional branching per row.

## Implementation (`precompiled/time.cc`)

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
EXTRACT_NANO(extractQuarter)
EXTRACT_NANO(extractMonth)
EXTRACT_NANO(extractWeek)
EXTRACT_NANO(extractDay)
EXTRACT_NANO(extractDow)
EXTRACT_NANO(extractDoy)
EXTRACT_NANO(extractHour)
EXTRACT_NANO(extractMinute)
EXTRACT_NANO(extractSecond)
EXTRACT_NANO(extractEpoch)
EXTRACT_NANO(extractDecade)
EXTRACT_NANO(extractCentury)
EXTRACT_NANO(extractMillennium)
```

## Declarations (`precompiled/types.h`)

Each wrapper needs an `extern` declaration in `types.h` so the precompiled
bitcode exports the symbol. Example pattern:

```c
gdv_int64 extractYear_timestamp_micro(gdv_timestamp_micro micros);
gdv_int64 extractYear_timestamp_nano(gdv_timestamp_nano nanos);
// ... 14 x 2 = 28 declarations total ...
```

## Registry

No changes to `function_registry_datetime.cc` — Phase 1 already extended
`DATE_TYPES` in the registry macro, so `DATE_EXTRACTION_TRUNCATION_FNS` auto-
registers all 14 extract functions for `timestamp_micro` and `timestamp_nano`.

---

## Test Plan

Each extract function is tested for both MICRO and NANO with at least:

- **One post-epoch value**: e.g., `2021-03-15 10:30:45.123456` in MICRO
  (= `1615804245123456 µs`). Verify `extractYear` = 2021, `extractMonth` = 3,
  `extractDay` = 15, `extractHour` = 10, `extractMinute` = 30,
  `extractSecond` = 45.

- **One pre-epoch value**: e.g., `-1 µs` (= `1969-12-31 23:59:59.999999`).
  This is the highest-risk case — naive C division of `-1 / 1000` yields `0`
  (1970), not `-1` (1969). Verify:
  - `extractYear` = 1969 (not 1970)
  - `extractMonth` = 12
  - `extractDay` = 31
  - `extractHour` = 23
  - `extractMinute` = 59
  - `extractSecond` = 59

- **NANO range boundary**: a value near `2262-04-11` (max NANO) to confirm no
  overflow in the floor-division step.

- **`extractEpoch`**: verify it returns millis (the existing behavior) — i.e.,
  `extractEpoch_timestamp_micro(1000000)` = `1000` (1 second = 1000 ms epoch).

## Files Changed

| File | Change |
|---|---|
| `precompiled/time.cc` | `EXTRACT_MICRO` / `EXTRACT_NANO` macros and 28 invocations |
| `precompiled/types.h` | 28 `extern` declarations |
| test file | Extract function tests for MICRO and NANO |
