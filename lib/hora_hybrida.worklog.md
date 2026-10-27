# hora_hybrida worklog

## 2026-10-20 - born (genera-datorum D2)

Packed s64: ms << 16 | counter. Numeric order = event order; ties
across machines broken by the machine id beside it (in the volume,
UNIQUE (hora, machina)). `proxima` = max(physical << 16, last + 1):
a full counter (0xFFFF) carries into the next ms by plain +1, so no
special case. Left out on purpose until something pulls: the receive
rule with a future-refusal bound (sync) and a base32 text form (no
reader yet). Two plants did not compile at first (unused parameter /
set-but-unused) - rewritten as `ultimum - ultimum` and `< ZEPHYRUM`
variants, both red.
