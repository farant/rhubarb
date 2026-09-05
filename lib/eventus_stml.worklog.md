# eventus_stml.worklog.md

## 2026-09-04 — natus (ludus T5)

Eventus[] <-> STML for replay logs. Title table in enum order; the
five trailing titles are the derived genera T10 adds to the enum —
inert until then, TITULI_NUMERUS guards lookup. Deviation from the
plan: `tempus` is s64 and the draft serialized it through
chorda_ex_s32 — truncates past ~24 days of uptime. Now through f64
with zero decimals (exact for integers to 2^53); the probatio plants
a value beyond s32 and reads it back. `typus` (character) is written
as its byte value.
