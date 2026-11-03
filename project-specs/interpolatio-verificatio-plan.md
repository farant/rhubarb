# interpolatio verificatio: early stopping on primes, with random-point verification

Status: DRAFT v3 (2026-11-02). Q1 DECIDED by Fran 2026-11-02 (accepted,
with conditions). Design review done (consiliarius-verificatio, approve
after fixes; every finding placed, see "Design review" at the end). Q2
and Q3 DECIDED by Fran 2026-11-02 (s = 64; frozen-kernel checker for the
14 × 801 headline). T0 DONE 2026-11-02 (see "T0 amendment"). Next: T1.

Agreed with Fran (2026-11-02): after the TL speed pass
(temperley-celeritas-plan.md, done) and the Regina comparison
(laqueus.worklog.md, 2026-11-02), the next step is early stopping on
primes, then parallel points (L3), then 16 × 801. Ledger: desideratum
01M6F2RT; principle 01M6FMVW.

## Purpose

The bracket's interpolation uses enough primes to satisfy a PROVED bound on
coefficient size (contractio-plan T5a: the spanning-tree bound B(G) from
the Tait graphs). The bound is sound, but the actual coefficients are 3–5×
smaller in bits. Convention in this plan: **k_max** = interpolation primes
by the proved bound, **not counting** the confirming prime ADDITUS;
**k_true** = the first k with max|c| ≤ (M_k − 1)/2, M_k = p₁⋯p_k (computed
by the design review from brackets.txt with the real prime sequences):

| braid | actual bits | limes | k_max (+ ADDITUS) | k_true |
| --- | ---: | ---: | ---: | ---: |
| 9 × 102 | 17 | 88 | 3 + 1 | 1 |
| 10 × 101 | 24 | 85 | 3 + 1 | 1 |
| 11 × 102 | 16 | 82 | 3 + 1 | 1 |
| 12 × 101 | 19 | 86 | 3 + 1 | 1 |
| 12 × 801 | 175 | — | 22 + 1 | 6 |
| 14 × 801 | 195 | 662 | 23 + 1 | 7 |

Across the 45 D136 brackets k_true is 1–3, except 8 × 801 (7) and
12 × 801 (6); TL and vertex prime sequences give the same counts. (T0
prints K_q, limes, k_max and k_true for all 45 and 14 × 801.)

Each prime costs K_q + (puncta addita) evaluations, where K_q is the
a-priori window of the quaestio (`_fenestra_uncini`, imus = −c − 2s_B + 2,
summus = c + 2s_A − 2, step 4), about twice the bracket's real window:
12 × 801 is 23 × 563 = 12,949 evaluations (K_q = 561), 14 × 801 about 540
per prime. The evaluation ratio PROBATUM / VERIFICATUM is about 3.8× at
12 × 801 and 3.4× at 14 × 801 (2.0 h → ~35 min CPU), on every route.

## Q1. The principle (DECIDED 2026-11-02)

contractio-plan rule 1: "**Exact only.** Every result is exact or
refused." The interpolation header puts it concretely: the number of
primes follows from the proved bound, "never from the answer having stopped
changing" (include/interpolatio.h, EXACTITUDO), and contractio-plan T5a
excluded early stopping by name and tightened the proved bound instead.

This plan adds a SECOND kind of certificate next to the first, not a
replacement:

- **PROBATUM** (proved, unchanged, stays the default): k_max primes by the
  proved bound, plus ADDITUS.
- **VERIFICATUM** (opt-in): stop at the first prime count where the
  reconstruction passes m certifying checks. Each check evaluates the
  polynomial at a random point modulo a fresh random prime and compares it
  with the candidate. The result is labelled with its error bound
  (≤ 2^-s, s = 64 by default), its seed, and its checker.

### Q1 decision (Fran, 2026-11-02): accepted, with conditions

Why it is consistent with rule 1: the rule forbids stopping on stability,
a guess with no error bound. Verification is an independent test with a
stated bound; it keeps the rule's reason and changes only its letter. The
two warrants are strong in different places:

| | modulus big enough | computation correct |
| --- | --- | --- |
| PROBATUM | certain | ADDITUS only |
| VERIFICATUM | ≥ 1 − 2^-s | checked by the checker, end to end |

In scholastic terms: demonstration and moral certitude, each labelled.

Conditions:

1. PROBATUM stays the default everywhere (rule 1).
2. **Records policy** (rule 7).
3. **Upgrade path** (rule 8, "Promotion").
4. The error bound reviewed before T1 (done: design review).
5. Ledger PRINCIPIUM with refutatio (01M6FMVW).

### The warrant, stated precisely (design review M5)

> If checker C computes, at every prime q in the check range and every
> usable index, the value mod q of one fixed integer Laurent polynomial
> P_C in the proved window with every |c| < 2^limes, then
> Pr_seed[output ≠ P_C] ≤ 2^-s, assuming ideal randomness of the draws.

What that buys depends on the checker, and the relatio says which:

- **self-check** (main evaluator): the output equals what PROBATUM would
  compute. Covers modulus sufficiency and transient or prime-specific
  faults; not kernel errors.
- **cross-route** (TL ↔ vertex): also kernel errors, except in what the
  routes share: `_punctum_eligere` (i, A, δ) and congruentia.
- **frozen kernel** (79e7fad9 TL, tool only): also faults in everything
  new since celeritas; shares the temperley tables and module weights.

The bound also leans on the proved window and the proved limes (M4): a
VERIFICATUM result is not independent of the T5a bound, it only needs far
fewer primes than the bound asks for.

## Governing rules

1. **Proved stays the default.** Every existing caller and test gets
   PROBATUM exactly as today; VERIFICATUM only on explicit request.
2. **Never silent.** A verified result always carries its kind, its error
   bound in bits, its seed and seed provenance, the PRNG name, its prime
   list, and its checker with the shared-code list. Tools print that line
   next to the polynomial.
3. **Never more primes than proved.** At k_max the run evaluates ADDITUS
   and returns kind PROBATUM (status AD_LIMITEM). A disagreement between
   main evaluator and checker that persists is a refusal (FALSUM,
   status DISCORDIA), never a warning: when they disagree, the library
   cannot tell which one is wrong.
4. **Randomness from the caller.** The library takes a seed and never reads
   entropy itself. The tools take entropy (T0) and print the seed;
   `-semen X` reproduces a run. A caller-chosen seed is provenance DATUM
   and carries no bound in a record; only FORTUITUM seeds do.
5. **Registered before measured**, as in the speed plan.
6. **Same answers.** All 45 D136 brackets and the 14 × 801 bracket come
   out byte-identical in both modes.
7. **Records are proved or labelled.** A result written into a research
   record (knotapel/claims.md, certified brackets, demo results) is
   PROBATUM, or carries kind, error bound, seed, PRNG and checker next to
   it. VERIFICATUM is for exploration and large runs.
8. **Promotion loses nothing.** VERIFICATUM uses a prefix of PROBATUM's
   prime sequence (same route, same quaestio), so promotion costs only the
   remaining primes.
9. **Heuristics may only skip, never accept** (L3). A filter such as
   "max|Q_k| close to M_k/2" may skip a stage's checks; acceptance comes
   only from passed certifying checks.

## Design

### Check primes and points (H1, H2)

- **Check range:** primes ≡ 1 mod 4 in **[2^30, 2^31)**, uniform, minus the
  primes used, for every route and every checker. Both routes accept these
  primes (TL refuses q ≥ 2^31), and index + 2 cannot overflow.
- **N** = 25,348,870 primes ≡ 1 mod 4 in [2^30, 2^31) (design review,
  segmented sieve; cross-check: 25,348,667 primes ≡ 3 mod 4, sum
  50,697,537 = π(2^31) − π(2^30)). T0 recounts it with a house sieve and
  records it. The code uses N_MIN = 2^24 (1.5× margin); this replaces the
  "citable bound" question.
- **Draw procedure** (fixed, documented, reproducible): a pinned PRNG
  (named in relatio) seeded from `semen`; q by rejection: uniform odd
  value in [2^30, 2^31) until it is ≡ 1 mod 4, prime and unused; the index
  uniform in [0, q) by rejection (no modulo bias). Redrawing on INUTILE is
  safe because the INUTILE set depends only on q, not on D; redraws are
  capped (L1).
- For tests only, the quaestio may narrow the check range (calibration
  test); then the library counts N for that range itself, and refuses a
  narrowed range wider than 2^24.

### The checker (H2, H5)

The checker is passed as a struct, so the bound uses what the checker
declares:

    nomen structura {
        InterpolatioAestimare  aestimare;
        vacuum*                contextus;
        i32                    multiplicitas;    /* c: indices in [0, q) per y */
        i32                    inutilia_maxima;  /* r: INUTILE indices per q */
        constans character*    titulus;          /* in relatione */
        constans character*    communia;         /* codex communis cum principali */
    } InterpolatioVerificator;

laqueus declares c = 8 (y = A⁴ = ω⁸, at most gcd(8, q − 1) preimages),
r = 9 (ω ≡ 0 or ω⁸ ≡ −1).

### Interpolation (lib/interpolatio)

- `InterpolatioQuaestio` gains members (defaults keep today's behaviour):
  `modus` (INTERPOLATIO_PROBATUM | INTERPOLATIO_VERIFICATUM),
  `bita_securitatis` (s, default 64), `semen` (i64), `semen_origo`
  (FORTUITUM | DATUM), and the test-only check range.
- `interpolatio_polynomium_relatum(aestimare, contextus, &verificator,
  quaestio, piscina, &exitus, &relatio)`. `interpolatio_polynomium` stays
  and calls it in PROBATUM mode.
- `relatio`: kind, status (ACCEPTUM | AD_LIMITEM | DISCORDIA), route
  title, `primus_limes`, residue filter, the primes used, main evaluations
  at interpolation primes, self-checks, certifying checks, m, e, error
  bound in bits, seed, seed provenance, PRNG name, every (q, index) drawn,
  checker title and shared-code list.
- **Loop, two tiers (H3).** After each prime k < k_max, reconstruct Q_k by
  symmetric CRT, then:
  1. reject without a check any Q_k with a coefficient of ≥ limes bits
     (wrong by the proved limes; M4);
  2. **self-check**: one evaluation of the MAIN evaluator at a random
     (q, index); a mismatch → next prime;
  3. on a pass, **m certifying checks** with the checker, each with a fresh
     q and index; all pass → kind VERIFICATUM, status ACCEPTUM.
  4. Certifying mismatch after a self-pass: keep going; if the self-check
     passes again at a later stage with the same Q and the certifier fails
     again, stop: FALSUM, status DISCORDIA (a systematic disagreement; no
     point burning a full proved run).
  5. At k_max: evaluate ADDITUS as today; kind PROBATUM, status
     AD_LIMITEM, the checks reported as extra (M2).
  The bound comes from the certifying checks alone. Expensive evaluations
  drop to m (plus m per rare false self-pass).
- FALSUM leaves `exitus` untouched; relatio says why. Checker ERRATUM →
  FALSUM immediately (L1).
- **Checker cost rule (H3):** a cross-route or frozen checker is
  "acceptable" when m × its per-point cost ≤ 5% of the main run's
  estimated cost; otherwise the caller uses self-check (or the frozen
  kernel in the tool). P2 and P3 name their checker.
- Incremental Garner per stage is optional (L4); the cost is negligible
  either way.

### Error bound (H1, H2, M3, M4)

Let D = Q − P_C ≠ 0. A certifying check passes wrongly only if:

- (a) D(x) ≡ 0 mod q but D is not zero mod q. In y = x^passus, D has at
  most K_q − 1 roots; each y has at most c indices: at most
  ε_a = c(K_q − 1)/(2^30 − r).
- (b) every coefficient of D is divisible by q. Step 1 of the loop gives
  |D| ≤ |Q| + |P_C| < 2^(limes+1), so one nonzero coefficient has at most
  ⌊(limes + 1)/30⌋ prime factors ≥ 2^30: ε_b = ⌊(limes + 1)/30⌋ / N_MIN.
  ("Divides all" is a subset of "divides one".)

Per check ε₁ = ε_a + ε_b. Certification can happen at most at stages
1 … k_max − 1 (stage k_max is proved), so overall
≤ (k_max − 1) · ε₁^m. **m is computed exactly** (T0 amendment): with
ε₁ = a/b as an exact fraction, m is the smallest m with
(k_max − 1) · a^m · 2^s ≤ b^m, in Magnus. (v3's recipe, e = ⌊−log₂ ε₁⌋
then m = ⌈(s + ⌈log₂(k_max − 1)⌉)/e⌉, rounds twice and asks for one check
too many on 13 of the 36 braids that can stop early; T0 found it.)

Values (T0, exact): 14 × 801 (K_q = 561): m = 4. 12 × 801 (K_q = 561):
m = 4. 11 × 102 (K_q = 73): m = 4. 3 × 50 (K_q = 37, k_max = 2): m = 3.

### Promotion (VERIFICATUM → PROBATUM) (M1)

The prime sequence is deterministic per route and quaestio (residue
filter, `_primus_aptus` with necessaria = K + puncta addita,
`primus_limes`; INUTILE skips are per index, never per prime), so a
verified run with k primes used the first k of the proved run. A verified
Q in the symmetric range of M_k already is the CRT residue mod M_k.

- `interpolatio_promovere(aestimare, contextus, quaestio, &relatio,
  &Q, piscina, &exitus, &relatio_nova)`: Q is `constans` input, the proved
  result a separate output. It regenerates the prime prefix and compares
  it with the relatio's list: any mismatch (route, `primus_limes`, filter,
  code history - celeritas G1 already changed the TL sequence once) →
  FALSUM. Then it evaluates primes k+1 … k_max plus ADDITUS, combines with
  Q by CRT, and must reproduce Q exactly: a different result is FALSUM,
  and it is the refutatio of the principle - reported and filed, never
  repaired silently.
- A PROBATUM-kind result is never promoted.
- Evaluation counters count main-evaluator calls at interpolation primes
  only, apart from checks, so "evaluations = proved − verified" holds.
- Tool: `uncinus_mensura -promovere VIA` reads an UNCINUS line plus its
  RELATIO line. At 14 × 801: 16 of 23 primes plus ADDITUS.

### laqueus (L5)

Callers unchanged (rule 1). A new entry
`laqueus_uncinus_ex_plexu_verificatum(verbum, longitudo, fila,
&optiones, piscina, &f, &via, &relatio)` with an options struct (semen,
semen_origo, bita_securitatis, octeti_maximi, checker choice: AUTOMATICUS
| SUIPSIUS | VIA_ALTERA). `octeti_maximi` bounds main and checker
workspaces together. AUTOMATICUS picks the cross route when it fits the
cost rule and the workspace, else self-check. The existing
`laqueus_uncinus_ex_plexu` is unchanged.

### Tool

`uncinus_mensura` sections VIII and XI gain `-modus verificatum`,
`-semen X` (default: entropy, printed), `-verificator suipsius|via|
congelatus`, `-promovere VIA` and `-verificare VIA` (post-hoc m checks of
a given Q with a chosen checker, outside the loop), and print a RELATIO
line after each UNCINUS line.

The **frozen-kernel checker** (M7) lives in the tool: its own point code
(A = iω² with its own i, not the new kernel's `aestimare`), c = 8, passed
through `interpolatio_polynomium_relatum`.

## Registered predictions (fixed now; T0 computes exact values before T1)

- **P1 (exact).** VERIFICATUM stops at exactly k_true primes: 12 × 801:
  6 (against 22 + ADDITUS); 14 × 801: 7 (against 23 + ADDITUS). Any
  deviation is a bug in one direction or the other.
- **P2 (count, exact).** Main evaluations PROBATUM / VERIFICATUM equal the
  T0-computed ratio (≈ 3.82× at 12 × 801, ≈ 3.4× at 14 × 801).
- **P2t (time).** End-to-end CPU ratio PROBATUM / VERIFICATUM ≥ **2.5×** at
  12 × 801, frozen-kernel checker, median of ≥ 3 ABBA pairs in one binary,
  load recorded not gated (descriptive at 14 × 801).
- **P3 (overhead).** (self-checks + certifying checks) × per-point cost
  (from the A/B runs) + a standalone timing of the staged CRT on stored
  residues < 2% of the verified run's CPU at 12 × 801. Check counts are
  exact (T0 amendment): k_true self-checks (one per stage 1 … k_true; the
  last one passes) + m certifying, no false self-pass expected; at the
  bound, k_max − 1 self-checks.
- **P4 (D136 set, exact).** On all 45 brackets, VERIFICATUM's prime count
  equals the T0-computed k_true for each braid.

## T0 amendment (2026-11-02)

Tool: `uncinus_mensura -sectio XIII` (no evaluation at all: a-priori
window and limes by `laqueus_pd_fenestra`, the interpolation prime
sequence reproduced (residue 1 mod 4, below 2^31 for TL, below 2^32 for
vertex), k_true from the known brackets (`-uncini` brackets.txt and
uncinus_14x801.txt), today's dispatcher route with no memory limit as in
XI, the vertex cost by `laqueus_via_eligere`, m exact in Magnus) and
`-sectio XIV` (segmented sieve). Raw output build/verificatio/
t0_fundamentum.out (not committed). Under custos: 2 MB peak.

Checks that passed on all 46 rows: the tool's writhe equals each known
bracket's writhe (same word); every known bracket lies inside the
a-priori window; no coefficient reaches the proved limes; 12 × 801's
PROBATUM count 12,949 equals the count measured in the speed plan's T0;
the tool's m equals an independent exact-fraction computation.

**N** = 25,348,870 primes ≡ 1 mod 4 in [2^30, 2^31) (and 25,348,667
≡ 3 mod 4; sum 50,697,537 = π(2^31) − π(2^30) = 105,097,565 − 54,400,028,
CONGRUIT; 1.9 s). Equal to the design review's independent count.

**Exact registration** (route = today's dispatcher; evals P = (k_max + 1)
× (K_q + 2), evals V = k_true × (K_q + 2) when k_true < k_max, else = P;
ratio = P / V, interpolation evaluations only; m for s = 64, exact):

| braid | route | K_q | limes | bits | k_max | k_true | m | evals P | evals V | ratio |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| 3 × 20 | TL | 15 | 14 | 2 | 1 | 1 | 0 | 34 | 34 | 1.000 |
| 3 × 50 | TL | 37 | 34 | 5 | 2 | 1 | 3 | 117 | 39 | 3.000 |
| 3 × 102 | TL | 72 | 72 | 11 | 3 | 1 | 4 | 296 | 74 | 4.000 |
| 3 × 202 | TL | 147 | 151 | 24 | 6 | 1 | 4 | 1043 | 149 | 7.000 |
| 4 × 21 | TL | 15 | 16 | 2 | 1 | 1 | 0 | 34 | 34 | 1.000 |
| 4 × 51 | TL | 39 | 36 | 5 | 2 | 1 | 3 | 123 | 41 | 3.000 |
| 4 × 101 | TL | 80 | 66 | 15 | 3 | 1 | 4 | 328 | 82 | 4.000 |
| 4 × 201 | TL | 159 | 127 | 35 | 5 | 2 | 4 | 966 | 322 | 3.000 |
| 5 × 20 | TL | 15 | 15 | 1 | 1 | 1 | 0 | 34 | 34 | 1.000 |
| 5 × 50 | TL | 39 | 40 | 5 | 2 | 1 | 3 | 123 | 41 | 3.000 |
| 5 × 102 | TL | 76 | 82 | 16 | 3 | 1 | 4 | 312 | 78 | 4.000 |
| 5 × 202 | TL | 149 | 157 | 37 | 6 | 2 | 4 | 1057 | 302 | 3.500 |
| 6 × 21 | TL | 17 | 15 | 1 | 1 | 1 | 0 | 38 | 38 | 1.000 |
| 6 × 51 | TL | 40 | 41 | 13 | 2 | 1 | 3 | 126 | 42 | 3.000 |
| 6 × 101 | TL | 80 | 73 | 21 | 3 | 1 | 4 | 328 | 82 | 4.000 |
| 6 × 201 | TL | 145 | 154 | 50 | 6 | 2 | 4 | 1029 | 294 | 3.500 |
| 7 × 20 | vertex | 17 | 13 | 2 | 1 | 1 | 0 | 38 | 38 | 1.000 |
| 7 × 50 | TL | 39 | 32 | 6 | 2 | 1 | 3 | 123 | 41 | 3.000 |
| 7 × 102 | TL | 74 | 87 | 22 | 3 | 1 | 4 | 304 | 76 | 4.000 |
| 7 × 202 | TL | 148 | 167 | 39 | 6 | 2 | 4 | 1050 | 300 | 3.500 |
| 8 × 21 | vertex | 16 | 15 | 1 | 1 | 1 | 0 | 36 | 36 | 1.000 |
| 8 × 51 | TL | 36 | 41 | 7 | 2 | 1 | 3 | 114 | 38 | 3.000 |
| 8 × 101 | TL | 75 | 84 | 25 | 3 | 1 | 4 | 308 | 77 | 4.000 |
| 8 × 201 | TL | 140 | 166 | 39 | 6 | 2 | 4 | 994 | 284 | 3.500 |
| 9 × 20 | vertex | 15 | 15 | 2 | 1 | 1 | 0 | 34 | 34 | 1.000 |
| 9 × 50 | vertex | 37 | 37 | 9 | 2 | 1 | 3 | 117 | 39 | 3.000 |
| 9 × 102 | TL | 72 | 88 | 17 | 3 | 1 | 4 | 296 | 74 | 4.000 |
| 9 × 202 | TL | 141 | 175 | 52 | 6 | 2 | 4 | 1001 | 286 | 3.500 |
| 10 × 21 | vertex | 17 | 17 | 3 | 1 | 1 | 0 | 38 | 38 | 1.000 |
| 10 × 51 | vertex | 40 | 42 | 9 | 2 | 1 | 3 | 126 | 42 | 3.000 |
| 10 × 101 | vertex | 75 | 85 | 24 | 3 | 1 | 4 | 308 | 77 | 4.000 |
| 10 × 201 | TL | 144 | 162 | 42 | 6 | 2 | 4 | 1022 | 292 | 3.500 |
| 11 × 20 | vertex | 17 | 9 | 1 | 1 | 1 | 0 | 38 | 38 | 1.000 |
| 11 × 50 | vertex | 34 | 39 | 3 | 2 | 1 | 3 | 108 | 36 | 3.000 |
| 11 × 102 | vertex | 73 | 82 | 16 | 3 | 1 | 4 | 300 | 75 | 4.000 |
| 11 × 202 | TL | 146 | 175 | 47 | 6 | 2 | 4 | 1036 | 296 | 3.500 |
| 12 × 21 | vertex | 18 | 14 | 2 | 1 | 1 | 0 | 40 | 40 | 1.000 |
| 12 × 51 | vertex | 39 | 43 | 4 | 2 | 1 | 3 | 123 | 41 | 3.000 |
| 12 × 101 | vertex | 76 | 86 | 19 | 3 | 1 | 4 | 312 | 78 | 4.000 |
| 12 × 201 | vertex | 143 | 165 | 42 | 6 | 2 | 4 | 1015 | 290 | 3.500 |
| 8 × 401 | TL | 277 | 329 | 67 | 11 | 3 | 4 | 3348 | 837 | 4.000 |
| 8 × 801 | TL | 583 | 642 | 193 | 22 | 7 | 4 | 13455 | 4095 | 3.286 |
| 10 × 401 | TL | 275 | 334 | 82 | 12 | 3 | 4 | 3601 | 831 | 4.333 |
| 12 × 401 | TL | 277 | 333 | 91 | 12 | 3 | 4 | 3627 | 837 | 4.333 |
| 12 × 801 | TL | 561 | 654 | 175 | 22 | 6 | 4 | 12949 | 3378 | 3.833 |
| 14 × 801 | TL | 561 | 662 | 195 | 23 | 7 | 4 | 13512 | 3941 | 3.429 |

So:
- **P1** = 6 at 12 × 801, 7 at 14 × 801 (unchanged).
- **P2** (interpolation evaluations) = 12,949 / 3,378 = 3.833 at 12 × 801,
  13,512 / 3,941 = 3.429 at 14 × 801; with self-checks counted as main
  evaluations, 12,949 / 3,384 = 3.827 and 13,512 / 3,948 = 3.422.
- **P4** = the k_true column, braid by braid. 10 braids (c ≈ 20) have
  k_max = 1: no stage before the bound, so VERIFICATUM = PROBATUM there
  (status AD_LIMITEM). Over the 45, the mean evaluation ratio is 3.06
  (descriptive; v2's "≥ 2×" is superseded by the exact column).
- 14 × 801: K_q = 561, not ≈ 540 (the review inferred it from "~13,000
  points"); per prime 563 evaluations; PROBATUM 24 primes (23 + ADDITUS).
  2.0 h → about 35 min CPU in VERIFICATUM.

**Vertex at 14 × 801** (`laqueus_via_eligere`, no evaluation): workspace
3,895,358,656 bytes (3.63 GiB), 2.89 × 10¹² multiplications per point,
about 89 min per point at the D136 1.85 ns calibration. Over the 2 GiB
limit, so by the Q3 decision there is no post-hoc vertex check (it would
fit this 16 GB machine if the limit were raised for one run: 4 points ≈
6 h; Fran's call, not planned). 12 × 801 for comparison: 0.52 GiB,
1.44 × 10¹¹, about 4 min per point.

**L2 lever** (the separate proved improvement, desideratum 01M6FNZH):
comparing the exact product with 2^(limes+1) saves one prime on 5 of the
46: 3 × 202, 8 × 801, 10 × 401, 12 × 401, 14 × 801.

**PRNG pinned:** sors = PCG32 XSH-RR (64-bit state, 32-bit output,
O'Neill) seeded through SplitMix64 (`sors_seminare(semen, series)`);
draws by `sors_intra` (Lemire, unbiased). probatio_sors pins golden output
vectors, so the generator cannot drift silently. relatio names it
"sors PCG32-XSH-RR/SplitMix64".

**Entropy source:** `moneta_octeti_fortuiti` (lib/moneta.c; /dev/urandom,
NO fallback: on failure it returns FALSUM and leaves the buffer
untouched, and the caller refuses). That is exactly rule 4's contract,
so no new function is needed; the tools read 8 bytes from it for a
FORTUITUM seed and refuse if it fails.

**Corrections to v3:** m computed exactly (Error bound); self-checks =
k_true (P3, counting-checker test); bound vectors and a double-rounding
plant (Tests). No threshold moved.

## Tests

- All existing suites green and unchanged (PROBATUM default).
- probatio_interpolatio (new sections):
  - VERIFICATUM = PROBATUM on every existing interpolation case;
  - **undersized modulus**: a constructed polynomial too big for the first
    primes; the verified result is correct and uses more than the minimal
    prime count;
  - **counting checker** (H4a): main-evaluator checks = k_true (one self-
    check per stage, T0 amendment), certifying calls = m = relatio.m;
  - **reference draws** (H4b): an independent reimplementation of the
    documented draw procedure (pinned PRNG, range, rejection) reproduces
    relatio's list of (q, index) pairs exactly from the seed;
  - **lying checker**: correct main values, checker always wrong → FALSUM
    DISCORDIA, never acceptance; `exitus` untouched;
  - **tight polynomial** (H4c): a polynomial whose true size needs all
    k_max primes, honest checker: kind PROBATUM, status AD_LIMITEM, today's
    prime list plus ADDITUS;
  - **mirror checker** at this API (H4d), with the laqueus evaluators: the
    checker evaluates at A⁻¹ → FALSUM; precondition asserted: the bracket
    is not mirror-symmetric (Q(A) ≠ Q(A⁻¹));
  - **seeds**: two seeds give different check primes; the same seed
    reproduces exactly;
  - **bound vectors** (M3, T0 values): 14 × 801 (K 561, limes 662,
    k_max 23) → m = 4; 11 × 102 (K 73, limes 82, k_max 3) → 4; 3 × 50
    (K 37, limes 34, k_max 2) → 3 (the double-rounding recipe gives 4);
    synthetic vectors placed so each one-bit error flips m: c = 4 vs 8,
    k_max vs k_max − 1, limes vs limes + 1, q_min 2^31 vs 2^30;
  - **calibration**: narrowed check range (q ≈ 2^10–2^12, ε₁ ≈ 2^-3), many
    seeds on an undersized candidate with m = 1: the observed false-pass
    rate is > 0 and ≤ the formula. The bound is a live upper bound;
  - **promotion**: verified then promoted = PROBATUM directly, byte for
    byte; evaluations = proved − verified; a Q corrupted by **+M_k** on one
    coefficient → FALSUM (passes ADDITUS and limes, so only the R = Q
    comparison catches it); a Q corrupted by +1 → FALSUM; a relatio prime
    list differing from the regenerated prefix → FALSUM.
- probatio_laqueus_rete: verified = proved on the certified brackets
  (3 × 50, 8 × 101, 8 × 201); relatio names the cross route for 8 × 101.
- Plants (named reds, in a script under build/):
  - check compares Q with itself → undersized-modulus red;
  - check prime drawn from the used primes → undersized-modulus red;
  - loop runs m − 1 certifying checks → counting-checker red;
  - same q for all m checks → reference-draws red;
  - index range [0, 2^16) → reference-draws red;
  - check range lower end 2 → reference-draws red;
  - seed ignored (fixed draws) → seeds red;
  - main evaluator used as certifier → mirror red;
  - laqueus always uses self-check → laqueus cross-route red;
  - m computed one too small → bound-vectors red;
  - m by the double-rounding recipe (⌊e⌋, ⌈log₂⌉) → bound-vectors red
    (3 × 50);
  - promotion restarting from prime 1 → promotion count red;
  - promotion skips the R = Q comparison → +M_k corruption red.

## Tasks

- **T0. Baseline and feasibility.** K_q, limes, k_max, k_true and exact
  evaluation counts for the 45 braids and 14 × 801 (no bracket runs; the
  generator and brackets.txt) → exact P1/P2/P4 values. N recounted by a
  house sieve. Vertex workspace and cost at 14 × 801 (sumptus only). The
  PRNG pinned (sors algorithm, named) and the entropy source: a small
  public function (sors or a platform layer, arc4random as uuid.c does).
  A dated amendment; no threshold moves.
- **T1. Interpolation.** Checker struct, API, two-tier loop, integer error
  bound, promotion, tests and plants above.
- **T2. laqueus and tool.** The verified entry point with options, the
  automatic checker choice, the frozen-kernel checker, tool flags and the
  RELATIO line, laqueus tests.
- **T3. Acceptance and measurement.** All 45 brackets in both modes,
  byte-identical; P1–P4; 14 × 801 recomputed in VERIFICATUM with the
  frozen-kernel checker (Q3), plus post-hoc vertex checks if T0 shows the
  workspace fits.
- **T4. Review.** Independent reviewer, own oracle (recomputes the (q,
  index) draws from the seed and the documented procedure), not building
  in the live tree; checks the error bound again.

## Open questions for Fran

- **Q1.** DECIDED 2026-11-02 (see "Q1 decision").
- **Q2.** DECIDED 2026-11-02: s = 64 by default. Review's reasoning: with two tiers the
  certifying cost is m points (4 at 64, 8 at 128); the residual risk is
  set by the model's assumptions (checker correctness, PRNG, hardware
  faults PROBATUM shares) long before 2^-64. Allow up to 128 on request.
- **Q3.** DECIDED 2026-11-02: the frozen kernel checks the 14 × 801 headline
  (~3.8 s/point, m = 4 ≈ 15 s; independent of everything new since
  celeritas, where a 14 × 801-specific fault would live). The vertex route
  at 14 × 801 is estimated at 2.2–9 GB and 20–80 min/point (extrapolated,
  not measured): post-hoc `-verificare` only if T0 shows ≤ 2 GB.

## AUDIENDA

- Vertex workspace and cost at 14 × 801: not measured (T0).
- The sors PRNG's quality: not checked (T0 pins it; the bound assumes
  ideal randomness).
- K_q and limes for most of the 45: not yet printed (T0).
- No timing has been done; P2t and P3 are registered blind.

## Design review (2026-11-02, consiliarius-verificatio)

Verdict: approve after fixes. Verified: N = 25,348,870 by sieve (with the
π cross-check); the point map in `_punctum_eligere` (y = ω⁸, ≤ 8 preimages,
INUTILE ≤ 9 indices, depending on q only); K_q is the a-priori window
(12 × 801: 12,949 = 23 × 563); hand ε₁ and m for 14 × 801, 12 × 801,
11 × 102 (m = 4 throughout; plan's 2^-18 holds, true ≈ 2^-17.6); k_true
for all 45 and 14 × 801 from brackets.txt with real prime sequences;
promotion sound within one route and quaestio (CRT uniqueness; R = Q plus
ADDITUS catch a wrong Q). Literature: the scheme is the standard Monte
Carlo pattern (random points and random primes, candidate fixed before
its draws); promotion is its Las Vegas completion; Kaltofen–Lee early
termination (JSC 2003) needs a degree bound, which here is the proved
window - hence K must be K_q.

Findings and where they went:
- **H1** check-prime domain wrong (vertex main + TL checker draws
  q ≥ 2^31 → ERRATUM → every run FALSUM) and without a lower end (term (b)
  needs q ≥ 2^30) → check range [2^30, 2^31) for every route, N recorded,
  N_MIN = 2^24 ("Check primes and points").
- **H2** Y belongs to the checker; an index range other than [0, q)
  silently voids (a) → index uniform in [0, q) by rejection; checker
  struct declares c and r; ε_a = c(K_q − 1)/(2^30 − r); "~276" corrected
  to K_q.
- **H3** cost model false for an expensive checker (9 vertex points at
  12 × 801 ≈ 40 min against a 780 s proved run) → two tiers (self-check
  filter, m certifying checks), DISCORDIA stop, cost rule (≤ 5%), P2/P3
  name their checker.
- **H4** tests blind: loop-level m untested → counting checker + plant;
  draw procedure untested → reference draws + three plants; bound-fallback
  test contradicted rule 3 → tight polynomial, AD_LIMITEM; mirror test
  unreachable through laqueus → moved to the interpolatio API with a
  precondition, laqueus cross-route test + plant.
- **M1** promotion needs the prime list (routes differ; G1 already changed
  the TL sequence) → relatio records route, limit, filter, primes;
  promovere regenerates and compares; Q constans; counters separate;
  +M_k and +1 corruption tests + plant.
- **M2** behaviour at the bound undefined → AD_LIMITEM, kind PROBATUM;
  statuses ACCEPTUM / AD_LIMITEM / DISCORDIA; FALSUM leaves exitus alone;
  PROBATUM never promoted.
- **M3** floating log2 near thresholds → integer e and m, union over
  k_max − 1, boundary test vectors.
- **M4** (b) bit bound stated right → reject candidates with ≥ limes-bit
  coefficients, b = limes + 1; dependence on proved window and limes stated.
- **M5** warrant stated precisely, per checker; seed provenance; PRNG
  pinned and named ("The warrant, stated precisely", rules 2 and 4).
- **M6** P1 and P4 are computed facts, "8 with slack" contradicted 1/3, the
  table mixed conventions → exact equalities, one convention (k_max
  without ADDITUS), P2 as count + ABBA timing, P3 restated as counts ×
  per-point cost.
- **M7** frozen-kernel checker must not call the new kernel's aestimare →
  own point code, c = 8, tool-level checker.
- **L1** checker ERRATUM → FALSUM; INUTILE redraws capped. **L2** a proved
  lever outside this plan: compare the exact Magnus product with
  2^(limes+1) instead of summing ⌊log₂ p⌋ (saves 1 prime at 14 × 801,
  M₂₂ = 682 bits > 663) → ledger desideratum, own plan step later.
  **L3** heuristics only skip (rule 9). **L4** Garner optional (noted).
  **L5** options struct for laqueus; octeti_maximi covers both
  workspaces.

Q2/Q3 recommendations recorded under "Open questions". Not checked by
the review: exact K_q and limes for most braids, the PRNG's quality, the
vertex workspace at 14 strands, any timing.
