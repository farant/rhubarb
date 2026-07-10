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

## 2026-07-10 — M2c wall find: width-sensitive ops must canonicalize
## at READ (Q4 DECISUS refinement)

THE WALL: first Wave-0 sweep — 34 of 73 suites segfaulted the host,
ALL at the same instruction (`tabula_dispersa_inserere
@si_finis_14+4`, callers varied: chorda_internare, lexema creation,
stml/json/xml parsers — everything that hashes).

Root cause (one line of C): `positus_primus_deletum != -1` where the
variable is house-i32 (UNSIGNED). Demissio lowers the variable init
via `movere.i32` (canonical = zero-extended 0x00000000FFFFFFFF) and
the literal via `negare.s32 1` (canonical = sign-extended
0xFFFFFFFFFFFFFFFF), then compares with `inaequalis.i32`. Machinula
compared FULL 64-bit words ("registers read as-is" rule) → unequal →
wrong branch → _invenire_slotum returned -1 → inserere indexed
sloti + 0xFFFFFFFF*40 → ~171GB past the region.

THE PRINCIPLE (Q4 canonicalize-on-write refinement): "read as-is" is
only sound for TRUNCATING ops (addere/subtrahere/multiplicare/
bitalia/scribere — low bits in, low bits out, canonicalize-on-write
heals the top). For WIDTH-SENSITIVE ops the upper bits leak into the
result, so the op suffix must be authoritative at the read:
- comparationes (all 6) — the wall's cause
- dividere/residuum (integer paths)
- dextrorsum (what shifts in from above)
- fluitare (whole value becomes the float)
- redde (return crosses frames; params were already canonicalized
  per signature at _tabulatum_addere — redde now symmetric)

Fix: _valor_canonicus() = _canonicum(_valor_operandi()) at those
five sites (also applied to the shared addere-block reads — provably
identical semantics for truncating ops). _canonicum is total across
types (identity f64/i64/s64, zero-extend f32) so the helper is
unconditional. NOTE: this is NOT a demissio bug — with
suffix-authoritative reads, `inaequalis.i32` on an s32-written
register IS the C implicit conversion, visible in the text. The
alternative (demissio emitting movere conversions per mismatched
operand) would make the IR bigger and keep instruction semantics
history-dependent. Instruction semantics must be self-contained
under the suffix — textual-form-as-thousand-year-surface demands it.

Regression: comparationes.medulla c8 (aequalis.i32 between
i32-canonical and s32-canonical "-1" → must be 1; sum now 7*6=42).

Instruments born on this hunt (cursor v2.1): fork-per-suite
isolation (host segfault = one RUINA row, world stays cached in
parent, COW); -custodia flag (regio in-region checks convert wild
derefs into honest VITIUM + flight-recorder position — this is what
turned 34 anonymous signum-11s into ONE named chokepoint);
fusor -textus <substr> (dump one TU's canonical medulla to stdout).
Custodia stays OFF in the standing sweep (alloc poison vs native
fresh-page zero could diverge behavior); it is the diagnosis mode.
