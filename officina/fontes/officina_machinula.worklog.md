# officina_machinula worklog

## 2026-07-10 — natus + vectis M2b (piscina interpretata)

~1700 lines, compiled clean under the flag wall on the first pass.
Dispatch = switch; integer arithmetic in UNSIGNED i64 (defined wrap)
then canonicalized; signed div/mod discriminate + zero/overflow =
honest halt; signed right shift arithmetic (arm64 choice, noted).
Frames on region stiva: [registra][arcae], cursor restored at redde;
arca 16-aligned always. Formatter: directive walk, each via host
snprintf; %s manual (no length cap, precision+width by hand); %.*s
and * widths supported.

THREE first-run finds (fixture contact):
1. SYMBOLUM operands in vocare args ($forma) — _valor_operandi now
   resolves symbol-as-value to sedes address.
2. Vitium during arg eval didn't stop the call — barreled into the
   builtin with a zero pointer (host segfault). Now: currens check
   after arg loop.
3. Infinite recursion filled 4GB stiva legitimately (268M
   instructions before halt) — added 1M-frame depth cap: halts in
   ~2M instructions ("stiva vocationum exhausta (recursio?)").

Parks: decipula-as-DATA reads undetected (legere from a descriptor
address returns its bytes silently — corpus cases are all inside
sistere-parked regions; named for M2c). errno write-back plumbed
(cella_errno captured) but no builtin sets it yet (strtol at M2c).
exit() unwinds NOTHING (no atexit in corpus — grounded); genus BENE.

BAR: probatio_piscina interpreted — exit 0 + stdout BYTE-IDENTICAL
to native. 46,521 instructions, 2,103 calls, 871 builtins, 3 ms.
