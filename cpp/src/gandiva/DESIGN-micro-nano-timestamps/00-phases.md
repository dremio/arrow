# Implementation Phases

Parent design: [`../DESIGN-micro-nano-timestamps.md`](../DESIGN-micro-nano-timestamps.md)

Each phase is independently mergeable and testable. Phases must land in order —
later phases depend on types, constants, and macros introduced by earlier ones.

| Phase | Scope | Files touched |
|---|---|---|
| [01 — Infrastructure](01-infrastructure.md) | Type aliases, unit constants, registry macro, floor-division helpers | `types.h`, `time_constants.h`, `function_registry_common.h`, `time.cc` |
| [02 — Extract functions](02-extract-functions.md) | `EXTRACT_MICRO` / `EXTRACT_NANO` delegation wrappers (14 functions x 2 precisions) | `time.cc`, `types.h` (declarations) |
| [03 — date_trunc functions](03-date-trunc.md) | Fixed-unit expansions + EpochTimePoint delegation wrappers + sub-second levels | `time.cc`, `types.h` (declarations), `function_registry_datetime.cc` |
| [04 — Cast functions](04-cast-functions.md) | `castVARCHAR`, `castTIMESTAMP_utf8`, `castTIMESTAMP_date64`, `castTIMESTAMP_int64`, `castDATE`, `to_timestamp` | `time.cc`, `types.h` (declarations), `function_registry_datetime.cc` |
