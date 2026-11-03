> Raw analyst report, batch D of budget B1 (2026-10-27/28), filed verbatim as evidence for ../budget-fusionis.md. Paths under `scratchpad/b1/` were session scratch (profiles, prototypes) and no longer exist.

# Batch D — math stack under ASan (probatio_cellae)

## TL;DR (ranked, venenum minutes per merge that re-runs cellae)

| # | idea | venenum | radix | effort | API |
|---|---|---|---|---|---|
| 1 | `Magnus` 24 → 16 bytes (pass in registers; kills `__asan_memcpy`) | **≈ 1.6 min** | ≈ 0.1 min | S–M | none (private layout) |
| 2+3 | i64 gcd fast path + extensio normalisation shortcuts | ≈ 0.15–0.25 min on top of 1 | ≈ 0.05 | S | none |
| 8 | split `lentus` section into its own suite (runs in parallel) | **≈ 0.7 min** after 1–3 | ≈ 0.3 min | S | none |
| 4 | hoist exact axis norms out of `quaternio_proximi` | ≈ 0.1–0.2 | ≈ 0.05 | M | additive |
| 5 | stop 64 KB piscina create/destroy per call (radices, iudica) | ≈ 0.1 | small | S/M | none |
| 6 | remaining by-value struct copies (Algebraicus, Intervallum) | ≈ 0.05–0.1 | – | M | none |
| 7 | cellae lentum: incremental (Gray-code) interval sums | ≈ 0.1–0.2 | ≈ 0.1 | M–L | none |
| P1/P2 | policy: omit lentus / thin oracle sets under ASan | see policy | | | |

Measured (concurrent A/B, same load, full suite, all sections green):
ideas 1+2+3 together cut every non-lentus section of probatio_cellae
to **0.20–0.40 × baseline under ASan** (sum 0.33×) and **0.43–0.73×
at -O2**; laqueus under ASan went **98.0 s → 21.6 s**. Projected
onto the gate: cellae ASan 212.7 s → ≈ 100 s (lentus 43 s + rest 58 s);
with idea 8, gate wall ≈ 58 s. Total ≈ **2.5 min per merge** from
ideas 1–3 + 8, with no change to what the gate checks.

Analyst: batch-D agent, 2026-10-27/28. Nothing in the repo was changed.
Scratch prototypes, binaries, profiles and diffs are in
`/private/tmp/claude-501/-Users-francisarant-Documents-projects-rhubarb/e70c1844-689a-4b85-85a0-00afb0db9030/scratchpad/b1/D/`
(`src/` = baseline copy, `v1` = gcd fast path, `v2` = v1 + extensio
shortcuts, `v3` = v2 + 16-byte Magnus, `v4` = 16-byte Magnus only;
`patch_*_v3.diff` = the exact prototype edits; `struere.sh`,
`par.sh`, `par_plenum.sh` = build/timing helpers).

## 0. Ceilings and context you need before reading the ranking

- **Venenum runs its suites in parallel (fabrica composite), so its wall
  ≈ the slowest suite.** Gate-area credo.tsv (same build): cellae
  212.7 s, **laqueus 62.5 s**, reticulum 11.6 s, plexus 6.4 s, extensio
  6.0 s. Cutting cellae below ~62 s buys nothing until laqueus also
  drops. Practical venenum ceiling from cellae alone: 212.7 − 62.5 ≈
  150 s = **2.5 min** (the 3.55 min figure includes load).
- **cellae is also the radix gate's critical path**: radix credo.tsv
  cellae 50.4 s; next vicus_quies 24.4 s, exemplar_applicatio 22.1 s.
  Radix ceiling from cellae: 50.4 − 24.4 ≈ **26 s = 0.43 min**.
- fabrica caches suite verdicts (venenum warm = 1.2 s per its worklog), so
  these savings are only paid on merges whose cellae closure changed
  (magnus/polynomium/extensio/quaternio/radices/cellae/piscina/credo/
  chorda/tabula_dispersa/...). Worth checking how often that is
  (open question 1).
- Build flags of the gate (aedificatio.stml `probationes_veneni`):
  `-g -O1 -fsanitize=address,undefined -fno-sanitize-recover=all
  -DPISCINA_VENENUM=1`, arm64.
- The original 40-s profile (prof_cellae.txt) was a window over
  "scrutinium axium" / "zeta_12 contra oraculum" only and over-states the
  magnus gcd share. I re-profiled the whole run (109k samples, 3 ms;
  `D/prof_full.txt`) and the two heaviest sections separately.
  Whole run, inclusive: `__asan_memcpy` 34.8 % (+ `_platform_memmove`
  7.7 % inside it), `__asan_memset` 4.6 %, `_normalizare` 25.4 %,
  `quaternio_proximi` 28.9 %, `magnus_divisor_communis` 18.0 %, ASan
  malloc/free (piscina create/destroy) ≈ 8–9 %.
- Section shares of the gate run (ms, ASan / radix): D119 lentum
  46 697 / 20 917, imago sine iactura 40 863 / 5 685, scrutinium axium
  23 343 / 4 290, phasis contra oraculum 23 275 / 3 614, D119 15 687 /
  2 579, k sine limite 12 736 / 3 690, zeta_12 oraculum 10 388 / 1 894,
  everything else ≤ 8 s each.

## ROOT CAUSE (one sentence)

On arm64 (AAPCS64) a struct larger than 16 bytes is passed and returned
through memory, and LLVM only turns `memcpy` of ≤ 8 bytes into plain
loads/stores, so every by-value pass, return or assignment of a
24-byte `Magnus` (and of `Fractio` 48 B, `Algebraicus` 48 B,
`RadixRealis` 112 B) is an `llvm.memcpy`, which ASan rewrites into a
**call** to `__asan_memcpy` (shadow checks of both ranges + interceptor
+ memmove). Static count of `__asan_memcpy` call sites (my -O1 ASan
compile): magnus.o 102, polynomium.o 121, fractio.o 127, extensio.o 515,
radices.o 187. With a 16-byte Magnus: **0, 17, 6, 244, 146**.

---

## RANKED IDEAS

(Estimates are against the gate's 212.7 s cellae ASan run; "min saved
per merge" is for venenum unless stated, and is capped by laqueus,
see §0.)

### 1. Make `Magnus` 16 bytes: fold `signum`/`longitudo` into `parvus` when `membra != NIHIL`

- **Where:** `include/magnus.h:36-41` (struct) and `lib/magnus.c` — 16
  sites (`_parvus` 26-37, `_ex_signo_et_modulo` 102-104,
  `_ex_moduli_propriis` 144-146, `_aspectus` 173-174, `magnus_signum`
  598, `magnus_nega` 659, `_per_alternas` 955-957, `_transcribere`
  1046-1047, `magnus_residuum_parvum` 1107-1111). Plus the test helper
  `_canonicus` in `probationes/probatio_magnus.c:297-322`, which reads
  `.signum/.longitudo` directly. No other file touches Magnus fields
  (grep: only magnus.c; probatio_matrix.c reads `.membra` only, which
  stays).
- **Mechanism:** `{ s64 parvus; i32* membra; }`. Inline values unchanged
  (`membra == NIHIL`, value in `parvus`); big values store
  `signum * longitudo` in `parvus` (GMP's `_mp_size` trick). 16 bytes →
  passed and returned in x0/x1 → no memcpy at all for Magnus, and
  Fractio/Algebraicus/RadixRealis shrink by 16 bytes each (fewer bytes
  through the remaining memcpys; coefficient arrays and matrices 1/3
  smaller). Zero bytes are still the valid value 0.
- **Evidence:** prototype `v4` (repack only) and `v3` (repack + ideas 2
  and 3). Same-load concurrent A/B on "scrutinium axium" (gate 23 s):
  base 47.3 s → v4 9.6 s / v3 7.5–10.5 s (sequential, noisy);
  full-suite numbers in the table at the end. Non-ASan -O2 also gains:
  scrutinium 8.5 s → 4.7 s (v4) → 3.0–3.3 s (v3).
  Correctness: under ASan+UBSan+VENENUM the prototype passes
  probatio_magnus (with `_canonicus` adapted), fractio, polynomium,
  extensio, quaternio and cellae (all 33 sections, OMNIA PRAETERIERUNT).
- **Estimate:** §R: ideas 1+2+3 together 0.33× on non-lentus sections,
  of which ≈ 90 % is idea 1 (v4 vs v3: 8 %). ⇒ ≈ 100 s ⇒ **≈ 1.6
  min** venenum for idea 1 alone; radix ≈ 6–8 s (lentus-bound).
  Also helps laqueus/plexus/extensio/matrix suites in both gates (they
  all sit on magnus).
- **API impact:** none at function level. Struct layout changes (the
  header already says "Membra PRIVATA"). The invariant comment in
  magnus.h must be rewritten. Any persisted/serialized Magnus? none found.
- **Maintainability risk:** low. `longitudo ≤ 2^32` fits s64; helpers
  `_signum_membrorum(a)` / `_longitudo_membrorum(a)` keep call sites
  readable. The test's `_canonicus` is the only white-box reader.
- **Effort:** S–M (≈ 30 lines + test helper + worklog).
- **Verify:** `./compile_tests.sh magnus fractio polynomium extensio
  matrix quaternio radices cellae laqueus` + venenum gate. Plants:
  (a) in `_ex_moduli_propriis` store `longitudo` without the sign
  (`parvus = longitudo`) → probatio_magnus property "(a+b)−b = a" and
  negative oracle vectors red; (b) `_longitudo_membrorum` forgets the
  abs → ASan heap-buffer-overflow / huge loop on any negative big value
  (probatio_magnus big-negative vectors).

### 2. `magnus_divisor_communis`: Euclid in `i64` when both operands are inline

- **Where:** `lib/magnus.c:1126-1164` (`magnus_divisor_communis`), small
  path loop 1141-1148.
- **Evidence:** whole run: `magnus_divisor_communis` 18.0 % inclusive;
  in the scrutinium window 42 %, of which 59 % was `__asan_memcpy` at
  lines 1145/1146/1147 (the `magnus_divide(x, y, …)` call, `x = y`,
  `y = r`) and 22 % `magnus_divide`. The operands in cellae are almost
  always inline (Q(√5)/Q(ζ) coordinates with denominators 1/2/4).
- **Mechanism:** before the existing loop:
  `if (a.membra == NIHIL && b.membra == NIHIL) { m=|a|, n=|b| (u64 via
  _absolutum_s64); while (n) { r = m % n; m = n; n = r; } return
  _ex_signo_et_modulo(I, m, piscina); }` — `_ex_signo_et_modulo` already
  handles the one case that leaves s64 (gcd(S64_MIN, 0) = 2^63).
  Optionally also drop into this tail inside the small loop once both
  become inline (mixed big/small case).
- **Evidence (prototype v1, sequential, scrutinium):** ASan 52.8 s →
  34.7 s (−34 %); -O2 8.5 → 6.0 s.
- **Estimate:** whole-run share of the gcd small path ≈ 15 % ⇒ ≈ 30 s
  alone (≈ 0.5 min). **On top of idea 1** much smaller (the memcpys are
  already gone): v4 → v3 difference on scrutinium ≈ 20 %, ≈ 10 s
  whole-run ⇒ **≈ 0.15–0.2 min**. Do it anyway: it is 12 lines and also
  speeds every fractio/extensio user in the non-ASan build.
- **API impact:** none. Results identical (gcd is unique, ≥ 0).
- **Risk:** very low. **Effort:** S.
- **Verify:** probatio_magnus gcd properties + Fibonacci identity;
  probatio_fractio. Plant: return `m` from the wrong variable (`n`) →
  every gcd = 0 → fractio canonical-form checks red; plant: skip `abs`
  (signed `%`) → negative-operand gcd vectors red.

### 3. `extensio`: skip normalisation work that cannot change the result

All four keep the canonical form bit-identical (canonical form is
unique: num/den reduced, den > 0), so equality, text keys and the
combination cache are unaffected.

- **Where:** `lib/extensio.c:150-181` (`_normalizare`), 1824-1842
  (`algebraicus_adde`), 1867-1885 (`algebraicus_multiplica`),
  `lib/polynomium.c:681-693` (`polynomium_contentum`).
- **(a) `_normalizare`: denominator == 1 → return as is.** gcd(anything,
  1) = 1 and the sign is already positive; today it still computes the
  full content of the numerator (d gcds) plus one more gcd.
- **(b) `polynomium_contentum`: stop as soon as g == 1** (gcd(1, x) = 1).
  Public function, same result.
- **(c) `algebraicus_adde`: zero operand → return the other; equal
  denominators → `_normalizare(num_a + num_b, den)`** instead of two
  `multiplica_scalari` + den² (which also makes the later gcd larger).
  `quaternio._summa` starts every dot product/product with `0 + p₁`,
  which today renormalises p₁ in full; `_productum_scalare` also adds a
  dummy 0·0 first term (quaternio.c:745-753).
- **(d) `algebraicus_multiplica`: zero operand → zero.**
- **Evidence:** whole run `_normalizare` 25.4 %, `polynomium_contentum`
  6.6 %, `polynomium_divide_exacte` (called by `_normalizare` with a
  constant polynomial!) 4.1 %. Prototype v2 = v1 + (a)–(d): scrutinium
  ASan 34.7 → 28.1 s (−19 % on top of v1); -O2 6.0 → 5.5 s.
- **(e) cheap follow-up:** `_normalizare` divides by g via
  `polynomium_divide_exacte(numerator, polynomium_constans(g))` — a full
  long division with alternating officinae for what is a per-coefficient
  exact `magnus_divide`. A `polynomium_divide_scalari_exacte` (or a loop
  in extensio over `polynomium_coefficiens` + `polynomium_ex_coefficientibus`)
  removes most of the 4.1 %.
- **Estimate (on top of 1+2):** ≈ 8–12 % of what remains ⇒ **≈ 0.1–0.2
  min** venenum; larger in -O2 where memcpy isn't the bottleneck.
- **API impact:** none ((e) adds one polynomium function if done that
  way). **Risk:** low — the only subtle point is den = −1, which must
  still go through the sign fix (prototype checks `== +1` only).
- **Effort:** S.
- **Verify:** probatio_extensio (canonical-form checks, plants list in
  extensio.worklog "gcd skipped"), probatio_quaternio, cellae. Plants:
  equal-denominator path returns `_elementum(k, sum, den)` without
  `_normalizare` → 1/2 + 1/2 ≠ 1 canonical → extensio equality checks
  red; zero shortcut returns `a` instead of `b` → a+0 tests red.

### 4. Hoist the exact axis norms |u_k|² out of `quaternio_proximi`

- **Where:** `lib/quaternio.c:799-874` (`_proximus`), line 831
  (`quadratum = _productum_scalare(anulus, directiones[k],
  directiones[k], piscina)`); caller `lib/cellae.c:1255` (all axes) and
  1271 (filtered candidates); `Activatio` already caches float norms
  (`axes_intervalla`, cellae.c:570) but not exact ones.
- **Evidence:** whole run `_proximus` 28.8 %; its children: line 829
  (v·u_k) 32.1 %, **line 831 (u_k·u_k) 31.7 % = 9.1 % of the whole
  run**, `_aestimatio` ×2 24 %, compare 10 %. 98 % of
  `quaternio_proximi` time comes from cellae.c:1255 (all axes: the
  filter-off oracle and exact fallbacks), i.e. the same 46–476 axes over
  and over.
- **Mechanism:** new
  `quaternio_proximi_normatis(v, directiones, quadrata, numerus,
  antipodes_idem, piscina, indices, numerus_proximorum)` where
  `quadrata[k]` (anulus elements, may be NIHIL → computed as today) are
  supplied by the caller; `quaternio_proximi` becomes a wrapper passing
  NIHIL. cellae computes `a->quadrata_axium` once in activatio creation
  (next to `axes_intervalla`) and gathers the subset for `selectae`.
  Small extra: cache `scalare_optimum²` across k in `_proximus` (one of
  the four products in the two `_aestimatio` calls is recomputed every
  iteration) — ≈ 1.5 % more.
- **Estimate:** ≈ 9 % of the run before ideas 1–3, same order after
  (it removes arithmetic, not copies) ⇒ ≈ 8–15 s ASan ⇒ **≈ 0.15–0.25
  min**; radix ≈ 4 s.
- **API impact:** additive (one public function in quaternio.h); cellae
  Activatio gets one array.
- **Risk:** low-medium: the cache must be for the *vector part* norm
  and in the same anulus; a stale cache after axes change is the bug to
  test for (axes are immutable after creation in cellae today).
- **Effort:** M.
- **Verify:** probatio_quaternio proximi tie tests (call both variants,
  compare); cellae "scrutinium axium" (filter on vs off, 1800
  comparisons). Plant: pass |u_{k+1}|² for k (off-by-one) → scrutinium
  disagreements; plant: use the full-quaternion norm instead of the
  vector part → proximi tie tests red.

### 5. Stop creating/destroying 64 KB piscinae per call in hot paths

- **Where:** `lib/radices.c:946-962` `radix_compara` (two
  `_piscina_temporaria()` = `piscina_generare_dynamicum(…, 65536)` per
  compare), `lib/radices.c:527-547` `_separa_positivas` (two per call),
  `radix_signum_polynomii` (radices.c:828-834), `lib/cellae.c:3441-3464`
  `activatio_iudica` (one 64 KB piscina per judgement).
- **Evidence:** whole run: `piscina_generare_dynamicum` 3.9 % +
  `piscina_destruere` 4.2 % inclusive, almost all ASan allocator
  (`asan_malloc`, `Quarantine::Put/Recycle`, malloc-fill memset). In
  "imago sine iactura" 10.6 % + 9.0 %; in v3's lentum profile
  (after ideas 1–3) malloc/free/quarantine is ≈ 12 % of the section.
- **Mechanism (pick one):** (a) module-level scratch pair in radices
  (marked once, `piscina_reficere` instead of destroy, like magnus's
  alternae) — VENENUM still poisons the reset bytes; (b) pass an
  `opus` piscina down from the caller (cellae already has `opus`);
  (c) cheapest: smaller initial alveus (4 KB) for these temporaries —
  ASan malloc cost is per call, but quarantine churn and malloc-fill
  scale with size.
- **Estimate:** ≈ 5–8 % of the run ⇒ ≈ 0.1–0.15 min. Non-ASan:
  small (system malloc is cheap) — the T7 worklog did see 25 %
  `madvise` in a deep run, so radix may gain too.
- **API impact:** none for (a)/(c); (b) touches internal signatures.
- **Risk:** (a) adds static state (single-threaded house, but
  re-entrancy: radix_compara → radix_signum_polynomii nesting must not
  share the same scratch) and **trades ASan heap-UAF detection on the
  scratch for VENENUM poison detection** — a gate-strength question for
  Fran. (c) is risk-free.
- **Effort:** S (c) / M (a).
- **Verify:** radices MEMORIA/apex tests (`radicum_apices_purgare`,
  apex bounds), venenum specimen; plant: forget `piscina_reficere` on
  the reused scratch → apex bound test red.

### 6. Remaining by-value struct copies (`Algebraicus` 40 B after idea 1, `RadixRealis`, `Intervallum` stores)

- **Where:** extensio internal helpers (`_elementum`, `_an_bonum`,
  `_an_adde/_an_multiplica` copying `ELEMENTUM(x)` into by-value args),
  radices `_signum_ad` (radices.c:247-249, 1.0 % of run), cellae
  `_vector_parare` (cellae.c:2283-2293: `partes[m] = …`,
  `x->partes[m] = partes[m]` — 16-byte stores through memory are
  memcpy calls under ASan).
- **Mechanism:** internal pointer-based variants (`_adde_in(const
  Algebraicus*, const Algebraicus*, Piscina*, Algebraicus* exitus)`)
  used by the anulus hooks; public by-value API unchanged. For
  Intervallum: assign `.infra/.supra` fields (inline-instrumented
  loads/stores instead of a call).
- **Estimate:** after idea 1, extensio still has 244 `__asan_memcpy`
  sites; my guess ≈ 5–10 % of the remaining ASan time ⇒ ≈ 0.1 min.
  Measure with a v3 profile before doing it.
- **API impact:** none. **Risk:** low but touches many small functions
  ("ASan-shaped" code is a maintainability smell — only do the hot
  ones). **Effort:** M.
- **Verify:** extensio/quaternio suites; plant: write the result to the
  wrong out-pointer in `_an_adde` → quaternio arithmetic tests red.

### 7. (Algorithmic, cellae) `iudicium contra D119 lentum` barely moves with 1–3

- After ideas 1–3 the lentum section is the largest remaining block and
  gained only ≈ 7 % (concurrent A/B: base 86.4 s, v3 80.1 s, v4 80.2 s
  under load). Its v3 profile: `_vector_parare` 33.6 % (self time in the
  float interval loop + ASan stack-redzone setup for the local
  `partes[IV]`, cellae.c:2246-2293), `_cella_vectoris` 31 %,
  piscina churn ≈ 12 % (idea 5), `_sat_solve` 4 %, `tabula_dispersa`
  4 %. Called ~10^8 times (177 k sets × k × sign patterns).
- Idea: enumerate sign patterns in Gray-code order and update the
  interval sums incrementally (one add/sub per step instead of n×4), or
  hoist the per-weight-set part out of `_vector_parare`. M–L effort,
  belongs to the cellae owner; estimate up to −30 % of that section
  (≈ 15 s ASan, 6 s radix). Lower priority than policy P1 for this
  section.

### 8. Split the `lentus` section out of probatio_cellae (parallelism, not weakening)

- **Where:** `probationes/probatio_cellae.c:3285-3301`
  (`_probare_iudicium_d119_lentum`) and the SECTIONES table
  (4441-4442); `aedificatio.stml:1300` member list of
  `probationes_veneni` (and the radix composite).
- **Evidence:** after ideas 1–3 the lentus section is ≈ 43 % of the
  remaining cellae ASan time (concurrent A/B: base 86.4 s, v3 80.1 s —
  it is bound by float-filter self time, SAT and piscina churn, not by
  magnus) and **41 % of the radix cellae time** (gate radix: 20.9 of
  50.4 s; it did not move at -O2: 37.2 → 37.9 s under load).
  Both gates run suites in parallel, so one suite of 58 + 43 s costs
  ≈ 100 s of wall, two suites cost ≈ 58 s.
- **Mechanism:** move the section (and the helpers it needs) into
  `probationes/probatio_cellae_lenta.c`, or compile the same file twice
  with a `-D` that selects the section table (less duplication but
  unusual for the house). Add the new member to both composites. Every
  check still runs, under ASan too.
- **Estimate:** venenum ≈ 100 → ≈ 58 s ⇒ **≈ 0.7 min**; radix (with
  1–3) ≈ 41 → max(21 lentus, 20 rest, 24.4 vicus_quies) ⇒ ≈ 16 s
  ⇒ **≈ 0.27 min**. (Without 1–3 the split alone gives venenum
  212.7 → 166 s ≈ 0.8 min and radix 50.4 → 29.5 s ≈ 0.35 min — it is
  independent of the other ideas and can go first.)
- **API impact:** none. **Risk:** low — shared test helpers (catalog
  builders, `_contra_d119`) need a home (a `probationes/` include or a
  duplicated static); credo section names stay unique per suite.
- **Effort:** S.
- **Verify:** both suites green in `./compile_tests.sh cellae`; check
  the venenum composite lists the new member (a gate that silently
  drops it is the classic failure — plant: remove it from `membra=` and
  confirm the gate's coverage check / tegit list notices).

---

## POLICY QUESTIONS FOR FRAN (not decisions)

- **P1. Run `lentus` sections under venenum?** `iudicium contra D119
  lentum` is tagged `lentus` (probatio_cellae.c:4441) and costs 46.7 s
  ASan (22 % of the suite; after ideas 1–3 ≈ 40 % of what remains). The
  radix run still executes it. `CREDO_SIGNA_OMITTE=lentus` already
  exists (named OMISSA in the verdicts, never silent). The section
  exercises the same code paths as `iudicium contra D119` (n = 5, 6
  instead of 3, 4). Saving ≈ 45 s ⇒ **≈ 0.75 min**, but only if
  idea 8 (split) is done instead — the split keeps the coverage and
  gets the same wall-time win in a parallel gate. P1 only matters if
  the gate is ever run serially.
- **P2. Smaller oracle case sets under ASan?** "imago sine iactura",
  "scrutinium axium", "cellae phasis contra oraculum" (`gravis`), "filtrum
  sine iactura" compare filter-on vs filter-off over thousands of
  cases; under ASan the second, third, … thousand cases add no new
  memory-safety coverage. A compile-time `-DCELLAE_PROBATIO_PARVA`
  (or a credo knob) taking every N-th case under venenum would cut
  these ≈ 100 s (gate) by a large fraction. Risk: a memory bug that
  only one rare case triggers. Fran's call.
- **P3.** Is there value in a `<tolera>`-style per-suite ASan budget
  (gate prints suites > 60 s) so the next slow suite is noticed? (cheap
  observability, not a speed win.)

## CHECKED, NOT WORTH IT / NOT AVAILABLE

- **`-mllvm -asan-memintrin=0`** (leave struct copies uninstrumented):
  the option does not exist in this clang 21 (only
  `--asan-instrument-byval`, irrelevant on arm64 indirect passing). And
  it would weaken the gate anyway.
- **Deferring/batching normalisation (lazy canonical form):** canonical
  form is load-bearing (equality, text keys of the combination cache,
  the 0/±s identity in D94). Ideas 2–3 get most of the cost without
  changing the invariant.
- **Polynomium struct by value:** already 16 bytes (pointer + i32 +
  s32) → registers; not a memcpy source by itself (its memcpys are the
  Magnus coefficients it moves).
- **magnus_transcribe / alternating piscinae:** only for operands > 4
  limbs; never hit in cellae (apex 0); not a cost here.
- **piscina VENENUM memset** (`piscina_reficere` poison fill):
  `__asan_memset` total 4.6 %, mostly Magnus struct zero-init
  (magnus.c:414 `_parvus`) that idea 1 removes; the poison fill itself
  ≈ 0.1 % — keep it, it is the gate's point.

## OPEN QUESTIONS

1. How often does a merge actually invalidate the cellae verdict
   (closure includes piscina, credo, chorda, tabula_dispersa, friatio,
   memoria_processus…)? If most merges touch one of those, the closure
   is the cost driver, not cellae itself (fabrica batch).
2. laqueus (62.5 s ASan) was the next ceiling; idea 1+2+3 cut it
   98.0 → 21.6 s in a concurrent A/B (§R), so it stops binding.
   Is venenum really fully parallel (assumed from fabrica composite +
   3.55 of 4.4 min being cellae)? If serial, the laqueus win
   (~0.7 min of 62 s gate) adds on top.
3. Do we want idea 1 as a house rule ("value types ≤ 16 bytes on the hot
   path")? Fractio (32 B after idea 1) and Algebraicus (40 B) are the
   next candidates but their public by-value API makes it costly.

## §R. MEASUREMENTS

All under load from other agents (load 5–10 on 8 cores); only ratios
from runs that were **concurrent** (same load) are used for estimates.

Concurrent full-suite run (`par_plenum.sh`, 4 ASan + 2 O2 jobs at once;
per-section ms from CREDO_VERDICTA, files `D/*_pp.verdicta.tsv`):

| section | base ASan | v3 ASan | r |
|---|---|---|---|
| cellae phasis contra oraculum | 46 584 | 18 837 | 0.40 |
| scrutinium axium | 40 968 | 9 628 | 0.24 |
| iudicium contra D119 | 34 810 | 6 792 | 0.20 |
| cellae zeta_12 contra oraculum | 17 338 | 4 603 | 0.27 |
| filtrum sine iactura | 14 502 | 5 557 | 0.38 |
| filtrum avidum | 8 786 | 3 166 | 0.36 |
| 22 sections completed by both | 187 771 | 61 925 | **0.33** |
| iudicium D119 lentum (separate concurrent A/B) | 86 400 | 80 060 | 0.93 |

v2 (ideas 2+3 without the repack) on the same 22 sections: 0.64×.
v4 (repack only) vs v3 over the whole suite: v3 is 8 % faster
(ideas 2+3 on top of 1); per section 0–30 %.

-O2 (radix build), same concurrent run, whole suite: base 90.5 s → v3
73.7 s (0.81×); non-lentus sections 0.43–0.73× (scrutinium 0.51,
D119 0.43, imago 0.69), lentus 1.02×.

laqueus suite under ASan, concurrent A/B (lib/laqueus.c etc. unchanged,
only magnus/polynomium/extensio from v3): base 98.0 s → v3 21.6 s.

Correctness of the v3 prototype under ASan+UBSan+VENENUM: probatio_magnus
(with `_canonicus` adapted to the new layout), fractio, polynomium,
extensio, quaternio, cellae (33/33 sections), laqueus — all OMNIA
PRAETERIERUNT. This is a prototype check, not a review: the real change
still needs plants.

Projection onto the gate's 212.7 s: rest 166 s × 0.35 ≈ 58 s; lentus
46.7 s × 0.93 ≈ 43 s ⇒ ≈ 101 s (−112 s ≈ 1.85 min for ideas 1–3);
idea 8 then removes ≈ 43 s more of wall (≈ 0.7 min).
Radix: rest 29.5 s × 0.67 ≈ 20 s, lentus 21 s ⇒ cellae ≈ 41 s (−9 s);
with idea 8 the critical path is vicus_quies 24.4 s (−26 s total).
