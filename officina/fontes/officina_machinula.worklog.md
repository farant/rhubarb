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

## 2026-07-10 — time builtin (ISO) + AEDIFICATA_NUMERUS was READING
## PAST THE TABLE

Walked in to add builtin #43 and found `#define AEDIFICATA_NUMERUS
44` over a 42-entry table: the ligatio loop in machinula_creare has
been reading TWO AedificatumNota structs past the end of AEDIFICATA
on every world bind — silent UB that stayed benign only because the
trailing const-section bytes never chorda-matched a decipula name.
Fix: the count is now STRUCTURAL (magnitudo(AEDIFICATA) /
magnitudo(AEDIFICATA[0])) — it can never drift again. Lesson filed:
a hand-maintained count next to a hand-maintained table is two
sources of truth; the sizeof idiom was always the C89 answer.

time itself: _aed_time follows the strtol pointer-write pattern —
host time(NULL) once, write-through via memcpy when the arg is
non-null, return the SAME value (write-through == return is
deterministic within one call, which is what the regression pins;
host time_t = longus = one 8-byte verbum). Regression: inline
tempus.medulla module in probatio_officina_machinula (write-through
equality + post-2020 floor + cross-call monotonicity, → 42,
aedificata == 2). NULL arg arrives as immediate 0 under S64 eval
(decipula has no signature) — correct.

Suite 67/67; amalgama VERIFICATUM; multipart PRAETERIIT solo
(43,038 instr — was DECIPULA time).

## 2026-07-10 — M2d Chunk B: ansae plagularum + aedificata POSIX

Handle table born (DECISUS Q1 honored): FILE* = opaque ansa,
0/1/2 = std streams, fopen scans slots 3..64 for first NIHIL
(fclose vacates — unlimited total opens, 64 concurrent).
_ansam_solvere is the ONE resolution point; fprintf/fflush
rewired through it (fflush 0 = flush-all, ambiguity with stdin
noted and accepted — nobody loses).

15 new builtins (corpus-measured list, not the spec's maximal
one): fopen fclose fread fwrite fseek ftell fgets fputc remove
rename stat mkdir getcwd gettimeofday localtime → 58 total.
stat/gettimeofday write through GUEST pointers with the HOST
struct — sound because auspex_posix certifies our systema_posix
layouts ≡ host layouts (that gate is what makes these builtins
one-liners). localtime returns the host's static tm (outside the
region — custodia would flag it, diagnosis-only); reads of the
9 standard fields are prefix-safe; mktime-with-guest-tm = NAMED
hazard, corpus audited clean (nobody calls it).

Regression: ansae.medulla inline module — full lifecycle
fopen→fputc/fwrite/ftell→fclose→stat (st_size read at offset 96
through the certified layout!)→fopen/fread ('A'+'B'+'C'=198)→
remove→gettimeofday→getcwd, 12 builtins counted, →42.

Suite 71/71 → (post-localtime) green; amalgama VERIFICATUM.
Sweep: 55→66/73 (POSIX-transitive 9 + via + arbor_praeparator +
arbor_quaestio + fasti + sputnik_interpres — the last via the
DEMISSIO paren-conversion find, see officina_demissio.worklog.md).

## 2026-07-11 — M3 chunk ①: congelatio plana (3.2x, first-pass clean)

FunctioPlana (instructiones contiguae + blocci_initia strictly
increasing w/ closing sentinel + operanda plana) frozen as the 4th
machinula_creare pass, AFTER decipulae ligatio (the copy captures
the bound image — order matters). planae[] parallel to the conexio
function table (the index VOCARE/initium already resolve).
Tabulatum.bloccus DELETED — the runtime coordinate is now
(functio, flat index); block identity derived by binary search
over blocci_initia at PRINT time only (halt report). Ring entries
carry plana + flat index. tabulatum_summum cache (Xar element
pointers stable) killed the per-instruction frame re-fetch. The
fetch went from 3 cross-TU xar_obtinere calls to zero.

**MEASURED (seal pairs, -O0 daily build, same config):**
- vim solo: 550 → 171 ms @ 9,075,028 instr = **3.2x** (~16.5M →
  ~53M instr/s)
- sputnik_interpres 1076→395, tabula_characterum 299→92, stml
  163→60 (2.7-3.25x across the board)
- full sweep wall 4:23.98 → 3:45.93; **arbor_formator ESCAPED the
  TEMPUS bucket** (>30s → 15.7s, 673M instr, stdout idem) —
  sweep 75→76 praeteritae, 69 eaedem / 0 DIFFERUNT, tempora 6→5.
- The 1.3-2x grounded estimate undershot because THREE xar layers
  died at once: instruction fetch + top-frame re-fetch + VOCARE
  operand pool. This is ~LTO-experiment territory (3.6x) achieved
  by structure alone in the un-inlined -O0 build.
- Freeze cost: invisible at suite granularity — small suites are
  NET faster (xml 20→7ms includes the freeze). Sim C4's lazy-freeze
  fallback stays unneeded.

**Finds:**
1. medulla_functio_terminata has ZERO callers — the lector never
   validates terminator discipline. Unterminated blocks (possible
   only in hand-written .medulla) preserve today's honest runtime
   "fluxus extra bloccum" via MACHINULA_OP_FLUXUS_CUSTOS, a
   machinula-PRIVATE op (= MEDULLA_OP_NUMERUS; numeri_op array
   oversized by MACHINULA_OPS_PRIVATAE so the counter write stays
   branchless; slot NUMERUS+1 pre-reserved for chunk ④'s PAUSA).
   Guards only on non-terminated blocks → corpus stays exactly 1:1
   with the canonical walk (pact in officina_medulla.h).
2. SALIRE/RAMUS gained an explicit target bounds check (one
   unsigned compare) — today's bad-block honesty came free from
   medulla_bloccum_obtinere returning NIHIL; flat indexing needs
   it explicit. i32-unsigned compare catches negative garbage too.
3. xar_copiare_ad_tabulam was on the amalgamator EXCLUDENDA list
   (first officina use ever) — the standalone gate clamavit
   exactly as documented; one manifest line deleted.
4. sputnik_interpres instruction count moved −2,740 between runs
   (16,161,601 → 16,158,861) with stdout still idem — its
   interpreted workload has a time-dependent path (clock builtin).
   Instruction counts are NOT a determinism invariant for suites
   that read the clock; stdout remains the bar.

Cross-phase note: the pinning probatio (demission-time indices ==
frozen indices) lands in chunk ③ with indicium's side — the freeze
alone has nothing to compare against; blocci_initia strict
monotonicity is asserted implicitly by the parity bar (any drift
= wrong branch targets = instant corpus carnage).
