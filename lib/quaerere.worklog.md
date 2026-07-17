# quaerere.c worklog

## 2026-07-17 — backward-pass wrap = out-of-bounds reads

VANA adjudication (quaestio 01KXQ9NF8J class 2): _quaerere_passus_
posterior scanned backward with unsigned `i32 si_idx` and a dead
`si_idx < 0` break. When the scan reached position 0, si_idx--
wrapped to ~4e9 and `scopus.datum[si_idx]` read wild memory until a
garbage byte happened to match. Not just dead code — live OOB reads
on any backward pass that walks off the front of the target string.
si_idx is now s32 (casts at the i32* positiones boundary); the
break is live.
