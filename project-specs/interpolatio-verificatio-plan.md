# interpolatio verificatio: early stopping on primes, with random-point verification

Status: DRAFT v2 (2026-11-02), not yet reviewed. Q1 DECIDED by Fran
2026-11-02: accepted, with the conditions in "Q1 decision" below. Next:
design review, with the error bound as its main subject.

Agreed with Fran (2026-11-02): after the TL speed pass
(temperley-celeritas-plan.md, done) and the Regina comparison
(laqueus.worklog.md, 2026-11-02), the next step is early stopping on
primes, then parallel points (L3), then 16 × 801. Ledger: desideratum
01M6F2RT.

## Purpose

The bracket's interpolation uses enough primes to satisfy a PROVED bound on
coefficient size (contractio-plan T5a: the spanning-tree bound B(G) from
the Tait graphs). The bound is sound, but the actual coefficients are 3–5×
smaller in bits (`uncinus_mensura -sectio V`; 14 × 801 from
temperley-celeritas-plan "Headline"):

| braid | actual bits | limes | primes (proved) | primes that would suffice |
| --- | ---: | ---: | ---: | ---: |
| 9 × 102 | 17 | 88 | 4 | 1 |
| 10 × 101 | 24 | 85 | 4 | 1 |
| 11 × 102 | 16 | 82 | 4 | 1 |
| 12 × 101 | 19 | 86 | 4 | 1 |
| 14 × 801 | 195 | 662 | 23 | 7 |

Evaluations are about points × primes, so the bound is a direct factor of
the running time: at 14 × 801 about 3× (2.0 h → ~40 min CPU), on every
route.

## Q1. The principle (decide before T1)

contractio-plan rule 1: "**Exact only.** Every result is exact or
refused." The interpolation header puts it concretely: the number of
primes follows from the proved bound, "never from the answer having stopped
changing" (include/interpolatio.h, EXACTITUDO), and contractio-plan T5a
excluded early stopping by name and tightened the proved bound instead.

This plan proposes a SECOND kind of certificate next to the first, not a
replacement:

- **PROBATUM** (proved, unchanged, stays the default): enough primes by the
  proved bound, plus today's extra confirming prime.
- **VERIFICATUM** (opt-in): stop at the first prime count where the
  reconstruction passes m checks. Each check evaluates the polynomial at a
  random point modulo a fresh random prime, using an independent
  evaluator, and compares it with the candidate. The result is labelled
  with its error bound (≤ 2^-s, s = 64 by default) and its seed.

The result is still an exact integer polynomial; what changes is the
warrant. A wrong polynomial is accepted only if every check hits a root of
the difference (see Error bound): with s = 64 that is less likely than a
hardware fault during the run, and unlike the bound, the check also
catches a corrupted computation (bit flips, a faulty prime) end to end.

If Fran declines: this plan shrinks to the proved-only lever, exact τ(G)
by the matrix-tree determinant instead of B(G) (T5a measured ~1 prime in
7 saved, ~14%), and stops there.

If Fran accepts: a ledger PRINCIPIUM records the amendment ("exact or
refused; a result's warrant is PROBATUM or VERIFICATUM, and says which"),
with its refutatio (what would overturn it: a verified result found wrong,
or a flaw in the error bound).

### Q1 decision (Fran, 2026-11-02): accepted, with conditions

Why it is consistent with rule 1: the rule forbids stopping on stability,
a guess with no error bound. Verification is an independent test with a
stated bound; it keeps the rule's reason and changes only its letter. The
two warrants are strong in different places:

| | modulus big enough | computation correct |
| --- | --- | --- |
| PROBATUM | certain | one extra prime only |
| VERIFICATUM | ≥ 1 − 2^-s | independent evaluator, end to end |

In scholastic terms: demonstration and moral certitude, each labelled.

Conditions (each becomes a rule or design item below):

1. PROBATUM stays the default everywhere (rule 1).
2. **Records policy** (rule 7): anything recorded as a research result
   (knotapel/claims.md, the certified brackets, demo results) is PROBATUM,
   or carries the VERIFICATUM label and its relatio in the record itself.
   VERIFICATUM is for exploration and large runs.
3. **Upgrade path** (rule 8, "Promotion" below): a verified result can
   be promoted to PROBATUM by computing only the remaining primes; no work
   is lost. Proof deferred, not proof abandoned.
4. The error bound is reviewed in the design review before T1; a wrong
   bound makes the label lie, which is worse than no label.
5. A ledger PRINCIPIUM with its refutatio.

## Governing rules

1. **Proved stays the default.** Every existing caller and test gets
   PROBATUM exactly as today; VERIFICATUM only on explicit request.
2. **Never silent.** A verified result always carries its kind, its error
   bound in bits, its seed, its prime count, and which evaluator checked
   it. Tools print that line next to the polynomial.
3. **Never more primes than proved.** VERIFICATUM falls back to the proved
   rule at the bound. If it gets there, it still runs its checks; a
   disagreement then is a refusal (FALSUM), not a warning.
4. **Randomness from the caller.** The library takes a seed and never reads
   entropy itself. The tools take entropy (decision in T0) and print the
   seed; `-semen X` reproduces a run.
5. **Registered before measured**, as in the speed plan.
6. **Same answers.** All 45 D136 brackets and the 14 × 801 bracket come
   out byte-identical in both modes.
7. **Records are proved or labelled.** A result written into a research
   record (claims.md, certified brackets, demo results) is PROBATUM, or
   carries kind, error bound, seed and evaluator next to it.
8. **Promotion loses nothing.** VERIFICATUM uses a prefix of PROBATUM's
   prime sequence, so promotion costs only the remaining primes.

## Design

### Interpolation (lib/interpolatio)

- `InterpolatioQuaestio` gains members (defaults keep today's behaviour):
  `modus` (INTERPOLATIO_PROBATUM | INTERPOLATIO_VERIFICATUM),
  `bita_securitatis` (s, default 64), `semen` (i64).
- A new entry point takes a second evaluator for the checks and returns a
  report: `interpolatio_polynomium_relatum(aestimare, contextus,
  verificare, contextus_verificandi, quaestio, piscina, &exitus,
  &relatio)`. `relatio` holds: kind, primes used, checks run, error bound
  achieved (bits), seed, the check primes. `interpolatio_polynomium`
  stays and calls it in PROBATUM mode.
- Loop (VERIFICATUM): after each prime, reconstruct the candidate Q by
  symmetric CRT, as today but per stage. Then one check:
  1. draw a check prime q uniformly from the eligible primes (rejection
     sampling: random value in range until prime; same residue filter and
     primus_limes as the route; not one of the primes used);
  2. draw a random index; the evaluator maps it to a point x and returns
     P(x) mod q (INUTILE → draw again);
  3. compute Q(x) mod q by Horner over the coefficients reduced mod q, and
     compare.

  On a match, run the remaining m − 1 checks with fresh q and x; all
  pass → accept as VERIFICATUM. Any mismatch → add the next prime.
- Cost per check: one evaluation, plus a reduction of Q's coefficients
  mod q. At 14 × 801 a prime costs ~276 evaluations, so the checks are
  noise.

### Promotion (VERIFICATUM → PROBATUM)

The primes are a fixed descending sequence below primus_limes (residue
filter included), so a verified run with k primes used exactly the first
k primes of the proved run. A verified polynomial Q with coefficients in
the symmetric range of M = p₁⋯p_k already is the CRT residue mod M; no
stored residues are needed. Promotion evaluates only primes k+1 … k_max
(plus the confirming prime), combines them with Q by CRT, and must
reproduce Q exactly: a different result is FALSUM (the verified result
was wrong, which is the refutatio of the principle; it must be reported
and filed, not repaired silently).

- API: `interpolatio_promovere(aestimare, contextus, quaestio, &exitus,
  primi_facti, piscina, &relatio)`, where `exitus` holds Q on entry and
  the proved result on return.
- Tool: `uncinus_mensura -promovere VIA` reads an UNCINUS line plus its
  RELATIO line and computes the rest. At 14 × 801: 16 of 23 primes, ~70%
  of a proved run.
- A test: verified, then promoted = PROBATUM directly, byte for byte, with
  evaluation count = proved count − verified count (no prime recomputed).

### Error bound

Let D = Q − P ≠ 0 (a wrong candidate). In y = x^passus, D has at most
K − 1 roots, K = (summus − imus)/passus + 1. A check passes wrongly only if:

- (a) D(x) ≡ 0 mod q but D is not zero mod q: at most (K − 1)/Y, where Y is
  the number of distinct y values the evaluator's points reach mod q
  (vertex/TL: A = i ω², y = A⁴, so Y ≥ (q − 1)/8); or
- (b) every coefficient of D is divisible by q: the errors are nonzero
  multiples of M, at most limes + 1 bits each, so at most (limes + 1)/30
  primes divide one of them. Among N eligible primes drawn uniformly that
  is at most ((limes + 1)/30)/N.

Per check: ε₁ ≤ (K − 1)/Y + ((limes + 1)/30)/N. With staged checks, a union
bound over at most k_max stages (the proved prime count) gives overall
≤ k_max · ε₁^m. The code computes m as the smallest m with
log₂(k_max) + m·log₂(ε₁) ≤ −s. At 14 × 801: ε₁ ≈ 2⁻¹⁸, k_max = 23 → m = 4
for s = 64. N needs a provable lower count of primes ≡ 1 mod 4 in
[2^30, 2^31) (a Rosser–Schoenfeld-type bound, or a table checked once);
the design review checks the whole formula.

### Independent evaluator

The check is worth most when the evaluator shares no kernel code with the
one that produced the values:

- TL main → vertex check, when the vertex workspace fits (octeti_maximi)
  and its per-point cost is acceptable;
- vertex main → TL check, when n ≤ 16;
- otherwise the same route checks itself. That still catches an undersized
  modulus and corrupted values, but not a systematic kernel error such as
  a mirrored convention.

The report names the evaluator used. Known cost (`-sectio VII`, T3): the
vertex route at 12 × 801 is 1.44 × 10¹¹ multiplications per point (~4–5
min) with a 557 MB workspace; 14 × 801 is unmeasured (T0) and may exceed
the 2 GB workspace limit. In that case the tool-level 14 × 801 check uses
the frozen 79e7fad9 TL kernel in `uncinus_mensura` section VI: a different
kernel implementation (no in-place update, no Montgomery), though it
shares the temperley tables.

### laqueus

Callers unchanged (rule 1). A new entry
`laqueus_uncinus_ex_plexu_verificatum(verbum, longitudo, fila,
octeti_maximi, semen, bita_securitatis, piscina, &f, &via, &relatio)`
chooses the route as today and the checking evaluator as above. The
existing `laqueus_uncinus_ex_plexu` is unchanged.

### Tool

`uncinus_mensura` sections VIII and XI gain `-modus verificatum` and
`-semen X` (default: entropy, printed), and print a RELATIO line after
each UNCINUS line: kind, primes, checks, bits, seed, evaluator.

## Registered predictions (fixed now; T0 computes the exact values before T1)

- **P1 (count, exact).** In VERIFICATUM, the number of interpolation
  primes at 12 × 801 and 14 × 801 is at most 1/3 of PROBATUM's (14 × 801:
  7 against 23 by the bits above, 8 allowing one stage of slack).
- **P2 (time).** End-to-end CPU ratio PROBATUM / VERIFICATUM at least
  **2.5×** at 12 × 801 (one run each; descriptive at 14 × 801).
- **P3 (overhead).** Checks plus per-stage reconstruction under 2% of the
  verified run's CPU at 12 × 801.
- **P4 (D136 set).** Over the 45 brackets, the mean prime ratio
  PROBATUM / VERIFICATUM is at least 2× (computed, not timed).

## Tests

- All existing suites green and unchanged (PROBATUM default).
- probatio_interpolatio (new sections):
  - VERIFICATUM = PROBATUM on every existing interpolation case;
  - **undersized modulus**: a polynomial with coefficients too big for the
    first primes (constructed); the verified result is correct and uses
    more than the minimal prime count;
  - **lying evaluator**: correct on the main primes, wrong on check
    primes → FALSUM (rule 3), never acceptance;
  - **seeds matter**: two seeds give different check primes (from
    relatio); the same seed reproduces exactly;
  - the error-bound function against hand-computed values;
  - the bound fallback: with checks forced to fail, the result is
    PROBATUM with today's prime count;
  - **promotion**: verified then promoted = PROBATUM directly, byte for
    byte; evaluations = proved − verified; a corrupted Q handed to
    promotion → FALSUM.
- probatio_laqueus_rete: verified = proved on the certified brackets
  (3 × 50, 8 × 101, 8 × 201) with the cross-route evaluator; a
  mirror-convention evaluator (A ↔ A⁻¹) as checker → FALSUM.
- Plants (named reds, in a script under build/):
  - the check compares Q with itself → undersized-modulus test red;
  - the check prime drawn from the used primes (Q ≡ P there, so it always
    passes) → undersized-modulus test red;
  - the seed ignored (fixed draws) → seeds test red;
  - the main evaluator used as checker → mirror test red;
  - m computed one too small → bound-function test red;
  - promotion restarting from prime 1 (recomputing) → promotion count
    test red.

## Tasks

- **T0. Baseline and feasibility.** Proved prime and evaluation counts for
  the 45 braids and 14 × 801 (from the limes; no runs), actual bits from
  brackets.txt → exact P1/P4 expected values. Vertex cost and workspace at
  14 × 801 (`-sectio VII`-style, sumptus only). The entropy source: a
  small public function (e.g. in sors or a platform layer, from
  arc4random as uuid.c already does). A dated amendment; no threshold
  moves.
- **T1. Interpolation.** API, staged loop, error-bound function,
  promotion, tests and plants above.
- **T2. laqueus and tool.** The verified entry point, the cross-route
  evaluator choice, tool flags (`-modus`, `-semen`, `-promovere`) and the
  RELATIO line, laqueus tests.
- **T3. Acceptance and measurement.** All 45 brackets in both modes,
  byte-identical; P1–P4; 14 × 801 recomputed in VERIFICATUM, with the
  most independent evaluator T0 found feasible (the headline: an
  independent confirmation of the 2026-11-01 result).
- **T4. Review.** Independent reviewer, own oracle, not building in the
  live tree; checks the error bound formally.

## Open questions for Fran

- **Q1.** DECIDED 2026-11-02: accepted with conditions (see "Q1
  decision").
- **Q2.** Default s: 64 bits (proposed)? s only changes m (about 4 checks
  at 14 × 801).
- **Q3.** For the 14 × 801 headline: use the vertex check even if it costs
  hours (if it fits in memory), or the frozen kernel check, which costs
  seconds?

## AUDIENDA

- Vertex route cost at 14 × 801: not measured (T0).
- The count N of eligible primes needs a citable lower bound; not checked.
- Whether the evaluators' point maps give uniform y (assumed from
  A = i ω², ω uniform); the review checks it.
- Per-stage CRT cost estimated as negligible against 276 evaluations per
  prime; not measured (P3 measures it).
- No literature check yet on early termination in modular algorithms
  beyond memory of common practice (e.g. modular GCD); worth a look in
  the design review.
