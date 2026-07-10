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

## 2026-07-10 — nuntium EXITUS-1: f32 literal as call argument
## (args evaluated by PARAMETER type)

First-ever wrong-exit suite (188/189 asserts; the one failure:
pi through the nuntium wire came back -1.9253e+29). Minimal repro
(cursor -sola + temp probe): memcpy bit-punning was PERFECT both
ways — the breakage was `_identitas(3.14159f)`: an f32 LITERAL in
direct argument position. Cause: vocare evaluated ALL args as
MEDULLA_TYPUS_S64, so an IMMEDIATUM_F materialized the f64 bit
pattern; the callee's frame-entry canonicalization then kept the
LOW 32 bits of the double — the wrong half. Explains the suite's
exact fingerprint: 0.0f passed (low half of 0.0 is 0), -123.456f
passed (unary minus → negare.f32 → REGISTER arg, correct), only
the bare positive literal broke.

Fix: fetch the callee signature BEFORE argument evaluation and
evaluate each arg with its PARAMETER's declared type when known
(interpreted callees); unknown (builtins/decipulae/args beyond
parametra) stay S64 — which is CORRECT for variadic default
promotions (float→double). Register operands are unaffected
(read as-is). Same principle as the day's width fix: the
signature/suffix is the semantic authority at the boundary —
this was its third missing mirror (params in, redde out, now
immediates at the call site). Regression: vocationes.medulla
$proba_f32 (f32 literal arg, bounds check, folded into →42).

Instrument: cursor -sola <substr> (matching suites only, stdout
VISIBLE — plus fflush(stdout) before _exit, which skips stdio
flush; without it the interpreted output died in the buffer).

## 2026-07-10 — performance decomposition (MEASURED, parked)

Fran asked where the interpreter's time goes. Three 10-minute
experiments (probatio_vim, 9,074,881 instructions, scratchpad
builds — NO shipped code changed; speed is explicitly not the
current focus):

| build                            | vim ms | instr/s | vs -O0 |
|----------------------------------|-------:|--------:|-------:|
| shipped (-O0, separate TUs)      |    507 |   ~18M  |   1.0x |
| machinula.o at -O2               |    310 |   ~29M  |   1.6x |
| -O2, ring+counters REMOVED       |    337 |   ~27M  |  noise |
| -O2 + xar.c in the SAME TU       |    139 |   ~65M  |   3.6x |

Findings:
1. **The always-on flight recorder is FREE** (removal was noise —
   the 4096-entry ring is 128KB, L2-hot, stores pipeline behind
   dispatch). The VISIO's always-on provenance costs nothing.
2. **The dominant cost was cross-TU xar_obtinere on EVERY
   instruction fetch** (+ operand access): a non-inlinable function
   call with segment math per step. Compiling xar into the same TU
   (= what the AMALGAM build does naturally, or -flto) let it
   inline: 2.2x on top of -O2, 3.6x total — pure build
   configuration, zero code changes.
3. Remaining headroom (unmeasured, grounded estimates): flat
   instruction arrays frozen at machinula_creare (contiguous
   pointer walk instead of per-fetch segment math) ~1.3-2x;
   dispatch improvements (C89 has no computed goto — GNU
   extension; superinstructions possible) ~1.5x; frame memset +
   vocare overhead visible in call-heavy code. Realistic ceiling
   for a straightforward pass: ~100-150M instr/s (~30-60x slower
   than native). The rest of the gap is interpretation itself —
   closed only by M5 native codegen.

PARKED BY NAME: "cursor -celer build variant" (amalgam-or-LTO +
-O2 objects for the sweep driver) — pull when lapifex-class
blessing runs justify it (would take them ~10-20 min → ~3-5 min).
The standing -O0 build stays: compile speed + debuggability are
the daily-driver virtues.
