# cellae worklog

## 2026-10-10 - T2: catalogus (general closure, D82/D84 order)

Plan: project-specs/cellae-plan.md (approved by Fran). T1 (quaternio_proximi) is
in 3f023542.

**Order is semantics.** The catalog order fixes the axis table, the
first-maximum axis rule and the greedy search order. So the closure
implements D82/D84's construction exactly:
- element 0 = 1;
- the generators in the given order (depth 0, their own writhe);
- rounds r = 1, 2, …: every element present at the start of the round,
  in order, times every generator in order;
- equality up to sign;
- stop when a round adds nothing, or after gradus_maximi growing rounds
  (round 0 included);
- the limit cuts mid-round, and the catalog is flagged TRUNCATED rather
  than silently capped.

It is general in the generators and writhes (approved decision):
D82/D84 = (g1, g1*, g2, g2*) with writhes (+1, −1, +1, −1), limit 4096,
16 rounds.

**Duplicates by hash, not by scan.** The float code scans the whole
catalog per candidate (4096 x 4 x 4096 comparisons at zeta_12). Here the
key is the canonical text of the representative of {q, -q}: the first
nonzero part is made positive with the ring's signum, so the ring must
be ordered. That relies on the ring's text form being canonical; the
zeta_8 closure stopping at exactly 24 and the zeta_12 order match test
it. A rejected candidate (already present) resets the piscina to the
mark taken before its product. The venenum gate now builds the cellae
suite to watch that reset.

Credo (30 checks):
- zeta_8 over Q(sqrt2):
  - 24 entries in 4 rounds (depths 5/10/8/1);
  - 9 nulls;
  - not truncated.
- zeta_12 over Q(sqrt3), limit 4096:
  - truncated;
  - depth histogram 5/12/34/76/148/289/576/1152/1804 (D82's findings);
  - entry by entry equal to D82's float closure (rewritten inside the
    test): order, values (extensio's certified decimals, within 1e-12),
    depths, writhes. Same for zeta_8 against D84.
- Limits: limit 10 mid-round gives truncated; 2 rounds give 15 entries,
  not truncated.
- catalogus_pars keeps index order and refuses repeats or an empty list.
- Out-of-range accessors.
- Refusals: zero generators, mixed rings, an unordered ring (Z[t]).

Plants (7, all red):
- key without sign normalization (zeta_8 no longer closes at 24);
- depth off by one;
- writhe of the parent only;
- truncation not flagged;
- rounds limit off by one;
- product order g*i instead of i*g (the order test catches it);
- pars accepting repeats.

Test syntax notes: extensio's parser takes "-a/2" and "(100a - 99)/100",
not "-(a/2)" or "a - 99/100". Lint: `histogramma` became `distributio`
and `f_tors` became `f_torsio`.

## 2026-10-10 - T3: activatio (axis table, exact cells, tie sets)

D82/D84's combined_cell, exactly:
- **Axis table:** catalog vector parts in first-appearance order. The
  key is the vector part divided by its first nonzero component, so
  antipodes and scalings merge. The cap (axes_maximae) sets an explicit
  TRUNCATED flag (`activatio_truncata`: a small API addition in the
  spirit of T2's decision that nothing is capped silently).
- **Cells:**
  - zero sum, real sum and null sum as in D84;
  - the null tie {k/2 − 1, k/2} at even k only;
  - general sums: tan²h = |v|²/a² in Q(√d) placed against tan²(mπ/k),
    roots of S_k by position (D123), with radix_compara.
- **Minimal polynomial of tan²h:** t² − Tr·t + N from extensio's trace
  and norm. The root is picked by sign(T − Tr/2) (larger root iff
  positive); a rational T goes through radix_ex_fractione.
- **Axis ties:** quaternio_proximi (T1) with antipodes identified.
- **Cache:** a Punctum per exact point, keyed by T2's sign-normalized
  text (s and −s share a cell); per-k entries hold the rule and the
  tie list, valid while the activation lives.
- **Ring guard:** an extensio Anulus has contextus = its Extensio, and
  extensio_anulus(contextus) is that same anulus (address arithmetic
  only, nothing dereferenced). Any other ring is refused before its
  parts could be misread as Algebraicus (test: Hurwitz units over Q).

Credo (62 in the suite):
- **Axes:** zeta_8 has 13; the cap of 5 gives 5, truncated; zeta_12
  uncapped has **2,043** (D117's count); the cap of 512 gives 512,
  truncated.
- **Hand cells:** zero sum, real sum, a null element at k = 6 (sector 3,
  tied with 2) and k = 5 (sector 2, untied); refusals.
- **Independent integer oracle** in the test (Z[√2] s64, no cellae, no
  radices). Boundaries are exact where tan²(mπ/k) ∈ Q(√2) (m/k in 1/8,
  1/6, 1/4, 1/3, 3/8), a double with a checked 1e-9 margin elsewhere.
  It compared **90,900** cells (all sums of 1 or 2 entries, triples
  within the first 12; 10 values of k) on rule and full tie set: **0
  disagreements**, 28,850 tied, 13,620 null sums.

Plants (8, all red):
- null tie dropped;
- sector tie dropped;
- boundary equality counted below;
- axis ties reduced to the first;
- the wrong root of tan²h;
- axis key not normalized (13 axes no longer);
- cap not flagged;
- zero sum in sector k.

Lint: `activatio` (the approved API name) is a neo-Latin coinage, so it
got a glossary entry modelled on `glossarium` (the oratio gate runs in
the commit). Other renames: `sectores` became `sectiones`; the test's
oracle names became Latin (`Binomium`, `binomium_*`, `o_axis`, ...).

## 2026-10-12 - T4: separability (rule, robust, possible, fixed rules)

`activatio_separat(activatio, weights, n, k, mode, &verdict)`:
- **Vectors:** 3^n coefficient vectors, parity = number of nonzero
  coefficients mod 2 (each weight's (+q, −q) input pair), folded by ±.
  A vector whose first nonzero coefficient is −1 is skipped: s and −s
  share a cell and a parity.
- **Modes:**
  - rule: the rule cell;
  - robust: every tied cell;
  - fixed rules FIXA(upper, last): the max/min sector and max/min axis
    over the tie set, so FIXA(1, 0) = rule;
  - possible: one parity per exact POINT (the cache's Punctum address is
    the point identity, stamped per call; a point seen with both
    parities fails at once), then D121/D124's DPLL over the cells'
    variables. Over the decision budget (200,000) the call returns
    FALSUM (undecided).
- **Memory:** sums and lookup keys live in a per-call scratch piscina.
  T3's `activatio_cella` leaked the lookup key on every cache HIT; now
  `_punctum` resets to its mark on a hit (a scratch piscina is used
  when one is given), and the key is transcribed only on insert. The
  per-cell parity marks and SAT variable ids live in arrays sized
  k·(nd + 1), cleared by touched lists.

**Bug caught by examen before any test ran:** the SAT value array was
declared i32 (unsigned in this house), so "unassigned (< 0)" was
always false ("comparatio vana") and the solver could never see a free
variable. It is now s32.

Credo (83 in the suite, ~17 s):
- **D124's exact XOR6 triple counts** at k = 1, 2, 4, 6, 8, reproduced
  on the first run: full / non-null / null-only, each under rule,
  robust and possible (45 numbers from an independent exact engine,
  itself oracle-checked).
- On all 12,144 (triple, k) pairs: FIXA(1, 0) equals the rule, and
  robust ⇒ each fixed rule ⇒ possible always holds.
- The D124 review's clash: q15 + q22 = q17 (all null). Every mode
  refuses {15, 17, 22}, since q15 − q17 + q22 = 0 (odd parity) coincides
  with the empty vector (even).
- Refusals: n = 0 or > 7, k = 0, an unknown mode, an index out of range.

Plants (7):
- **Red (6):** parity counted only on +1; robust using the rule cell;
  possible ignoring the same-point clash; SAT unit propagation inverted;
  the fixed sector choice inverted; cell marks not cleared between
  calls.
- **Equivalent (1):** the ± fold dropped. Redundant work only, no
  verdict can change.

## 2026-10-12 - T5: searches and Capacitas

Approved decisions, implemented:
- **True counts are separate from stored winners.** `numerus[n]` is the
  true count (exhaustive: every set; greedy: what the search found).
  `servati[n]` and `victores[n]` are kept up to `limes_servandi`, each
  winner with the FIRST ladder k it passes at. Test: limit 5 gives
  1132 counted, 5 stored; limit 0 gives 1132 counted, 0 stored.
- **The greedy stop is search semantics, not a count:**
  `Quaestio.victores_maximi` (D82/D84: 32).
- **A fixed ladder, "first k that passes".**

Refinements to the sketch:
- the parameters live in a `Quaestio` struct, which adds the triple
  limit (D82/D84: 100), `cellae_maximae` (D82/D84's MAX_ACT 16384,
  which silently skips large k) and `indices` (allowed weights, so "a
  subset's weights with the full table" is expressed directly);
- the exhaustive search is for catalogs or index lists of <= 64 (u64
  mask sets, `Larvae`), the only place it is feasible.

Exhaustive: level-wise, downward closed per k (an n-set is tested only
if every (n − 1)-subset passed at that k), then a union over the ladder.
Greedy: D84's find_xor6 / find_recursive order exactly.

Credo (134 in the suite, ~30 s):
- every D124 exact table reproduced: exhaustive counts per level and
  maxima for full / non-null / null-only under rule, robust and
  possible; null members of all winners (1056, 300); the 13-axis
  variants (via `Quaestio.indices`); all four fixed rules (8, 8, 0);
- D84's greedy search with exact cells: (8, 8, 0), found 32 + 32;
- **winner lists equal to a literal D84 find_xor6 / find_recursive
  written in the test**, element by element, in order, with the same k;
- every stored greedy winner separates at its k, and at no earlier
  ladder k.

Plants (9):
- **Red (7):**
  - the union not deduplicated across k;
  - greedy keeping the last k (the first-k check);
  - the maximum counted from n = 1;
  - the storage limit ignored;
  - the greedy stop ignored;
  - the duplicate check dropped (first SURVIVED: counts and maxima
    looked right; killed by the literal D84 reference).
- **Equivalent (1):** skipping extension position 0. In D82/D84-shaped
  searches every stored winner contains position 0 (the lexicographic
  triples from position 0 fill the 32 slots), so position 0 is always
  "already in the winner".

Deferred from the plan: incremental extension (reusing a parent's
sums). It moves to the performance tasks (T6/T7), measured on zeta_12
next to the float filter.

Lint: `Mascae` became `Larvae`, `ki` became `index_k`. The approved API
name `activatio_exhaustiva` needed a glossary entry `exhaustivus`
(neo-Latin; forms exhaustiva, exhaustivam).

## 2026-10-12 - correction to the T5 entry: the plant count

The T5 entry and commit (b509e3bf) say "Plants (9), Red (7)". The real
count is **7 plants: 6 red** (union not deduplicated, last k, maximum
from n = 1, storage limit ignored, greedy stop ignored, duplicate check
dropped) **and 1 equivalent** (extension position 0).

## 2026-10-12 - T6: certified float filter, one ladder pass, combination cache

**Measured first** (scratch benchmark, D82 shallow 127, zeta_12, exact
rule, D82's greedy search): 21.8 s, almost all in extensio arithmetic
(gcd normalization) on exact sums and their text keys.

Three lossless changes:
1. **Certified float filter.**
   - Catalog entries and sector boundaries become correctly rounded
     doubles (extensio's and radices' certified 20-digit decimals,
     converted lazily).
   - Per sum, interval arithmetic gives a, v, tan²h and every axis
     score. Each operation's interval is widened past its rounding
     error (4.5e-16 relative + 1e-300).
   - The cell is decided only when a ≠ 0, v ≠ 0, tan²h lies strictly
     between boundaries, and one axis score is strictly highest.
   - A decided cell cannot be a tie (an interval never excludes an
     exact equality), so its tie set is that single cell.
   - Otherwise the exact path runs, comparing exact axes only among the
     float candidates (the others are certainly lower).
2. **One pass per weight set over the ladder.**
   - The greedy search used to call separat once per k, recomputing
     everything.
   - `_separat_scala` builds each folded vector's k-independent part
     once (coefficients, parity, intervals, axis decision or candidates,
     tan²h); per k it decides only the sector, stopping at the first k
     that separates.
   - The axis intervals and norms are precomputed per activation.
   - This was the big win.
3. **Combination cache.** A canonical (catalog index, ±1) key, sorted by
   index with the first sign positive, maps straight to the Punctum, so
   a repeated combination skips all extensio arithmetic. The empty
   vector needed a non-empty key ("#").

Timings (D82 greedy, exact rule):

| subset | before | after |
|---|---|---|
| shallow 127 (46 axes) | 21.8 s | **0.50 s** (44x) |
| deep 564 (476 axes) | — | 80 s |
| shallow 564 (239 axes) | — | 37 s |

On deep 564: 14.2M decisions filtered, 1.78M exact, of which **1.63M
genuine ties**.

**Preview of D125:** deep 564 under the exact rule reaches XOR12 with
21 XOR12 winners (D82's float: 19, under the cap of 32, so
tie-dependent). Shallow 564 reaches XOR10 (as D82).

Credo (150 in the suite, ~20 s):
- every earlier check passes with the filter on: all D124 tables, the
  literal-D84 greedy reference, the integer oracle;
- **filter on vs off** on zeta_12 (first 30 entries): all 36,540 (triple
  × k in 6/12/24 × rule/robust/possible) verdicts equal;
- the greedy search on zeta_12's first 51 entries (rule and robust):
  identical winner lists and k;
- statistics: off gives filtered = 0; on gives filtered > exact and
  ties <= exact. Decision COUNTS may differ between on and off. In
  possible mode a filtered vector does not mark its point identity, so
  a later same-point clash is found by the SAT solver instead of the
  early check: same verdict, a few more vectors evaluated.

Plants (7):
- **Red (6):**
  - axis decided with several candidates;
  - a = 0 not sent exact;
  - candidates not mapped back;
  - filtered decisions not counted;
  - combination key ignoring the sign;
  - **no interval widening.** The margins are load-bearing: without
    them float decides some exact ties wrongly.
- **Equivalent (1):** boundary touching allowed (`>=` relaxed to `>`).
  With widening, a true tie's interval never ends exactly on the
  boundary; `>=` stays as the conservative choice.

Open for T7: deep 564's 80 s is the linear interval scan over 476 axes
per vector plus 1.6M tied (exact) decisions. Next to measure: an axis
index (bucketed by region of S²) and incremental extension.

## 2026-10-12 - T7: two-stage axis scan (instead of a bucketed index)

**Measured first.** Deep D82 run (zeta_12, 564 entries from 3532, 476
axes, exact rule, D82 greedy): 75-80 s, about 86% of samples in
`_separat_scala` itself, i.e. the interval scan over every axis for
every vector. The plan's bucketed S^2 index was not needed. A cheaper
lossless change came first:

- **Stage 1 (plain doubles):** f_j = (m . u_j/|u_j|)^2, with m the
  midpoint of v's interval and the unit axes precomputed once
  (`axes_unitae`).
- **Bound:** |f_j - g_j| <= delta/2, where g_j is the true value.
  - |m - v| <= sqrt3 w, with w the max half-width.
  - The unit-vector and dot-product rounding is <= 1e-15 |v|.
  - So the squared error is <= e(2|v| + e).
  - delta is that, doubled, plus 1e-12 |v|^2 + 1e-300.
- **Pruning is sound:** an axis with f_j < F - delta (F = max f) is
  strictly below the true maximum. It can be neither the rule axis nor
  a tied axis.
- **Stage 2:** intervals only for the axes that remain, then the same
  candidate rule as before (supra >= the optimum's infra), compacted in
  place. The order stays ascending, so the "first maximum" is
  unchanged.
- **The exact tie set is unchanged.** `quaternio_proximi` over the
  candidates sees every true maximum.

**Results** (identical output):
- deep 564: 75.4 s -> 17.3 s, still XOR12 with 32 32 32 21;
- shallow 127: 0.45 s -> 0.14 s.

The statistics shift: fewer exact decisions, because pruned near-misses
no longer reach the exact path. Verdicts do not shift.

**Profile after the change** (deep 564):
- `_separat_scala` self is about 36%;
- `madvise` is about 25%. The arena returns pages; this is worth a
  look before a bucketed index;
- the rest is exact arithmetic (magnus divide and gcd).

The bucketed index stays unbuilt until a run needs it.

**Credo, section "scrutinium axium":**
- the deep catalog (476 axes), 200 deterministic triples x k {6, 12,
  24} x the rule, robust and possible modes;
- filter on (two-stage) vs. filter off (`quaternio_proximi` over every
  axis);
- 1800 comparisons, 0 disagreements, 1729 tied decisions exercised;
- 4.7 s.

Plants (5):
- **Red (3):**
  - margin dropped (`>= F`);
  - wrong unit-axis component;
  - midpoint taken from the wrong parts (off by one).
- **Survived (2): margin size, not margin presence.**
  - **Half-width term ignored.** The 1e-12 |v|^2 term covers it on
    every sampled sum. It becomes load-bearing only for strongly
    cancelling sums (small |v| from depth-16 entries), where w/|v|
    passes 1e-12.
  - **delta shrunk to 1e-15 |v|^2.** Rounding in the sample stays below
    that.

  The bound is pinned by the argument above (and the comment in the
  code), not by a test. Building a sum that is cancelling *and* tied,
  where width decides, is open.

## 2026-10-12 - T8: review (recensor-extensio, c996526d) and fixes

**Verdict:** no H or M. The reviewer rebuilt everything independently:
- Python code with exact Q(sqrt3) Fractions and its own closure, axes,
  cells, SAT and D82 greedy, sharing no cellae/radices/extensio code;
- **Catalog:** all 4096 zeta_12 entries match, with depth and writhe.
- **Cells:** 33,000 random deep sums agree on the rule cell and the
  full tie set.
- **Separability:** 2,300 sets through the filter path agree.
- **D82 deep 564 greedy:** the same 117 winners, in the same order, with
  the same first k.
- ASan, UBSan and venenum runs are clean.
- All eight attack points hold (the details are in the report).

The report lives in the scratchpad (RECENSIO_CELLAE.md) and is not
committed. Its findings and fixes:

- **L1: no independent zeta_12 oracle.** Two fixes:
  - **Section "cellae zeta_12 contra oraculum".** The test's integer
    oracle is generalized from Z[sqrt2] to Z[sqrt d]:
    - **Scale:** d = 3 at scale 32 (the reviewer measured catalog
      denominators <= 32), with a check that the scale really clears
      them.
    - **Sign:** an exact mixed-sign test, a^2 vs d b^2 by a 128-bit
      product built from 32-bit halves. Deep sums of 7 terms make p
      about 2^48, so p^2 overflows s64.
    - **Overflow guard:** any product over 2^60 raises `o_vitium`,
      which the section asserts is false.
    - **Exact boundaries per field:** the rationals {1/6, 1/4, 1/3}
      everywhere; 3 -+ 2 sqrt2 at {1/8, 3/8}; 7 -+ 4 sqrt3 at
      {1/12, 5/12}.
    - The section runs 1000 random deep sums of 1-7 entries x 12 values
      of k, including odd 5 and 7, which the zeta_8 oracle never
      covered. Result: 12,000 compared, 0 disagreements, 678 tied.
  - **Section "victores D82 profundi".** The reviewer's independent
    greedy winner list is frozen as a fixture (117 sets with k). Each
    must separate at its k and at no earlier ladder k, under the exact
    rule through the filter. Counts 32/32/32/21, falsi 0.
- **L2: `cellae_maximae` and `limes_triplicum` were never binding.**
  Section "limites quaestionis" (zeta_8):
  - **Cap.** A cap of 84 (= 6 x 14) must equal the ladder {1, 6}
    exactly. The full ladder must find winners at k > 6.
    - Measured: k = 24 alone adds nothing at zeta_8, so a cap that only
      excludes 24 would not bind.
  - **Triple limit.** `limes_triplicum` 5 allows <= 10 triples, all from
    weights < 5; the unlimited run finds 32.
- **L3** (the T7 width term is unpinned) is acknowledged; the bound is
  re-derived sound. Sums of up to 3 deep entries have |v| >= 1/32, so
  only cancelling sums of 6-7 terms make the width term carry the
  margin.
- **P2:**
  - `activatio_separat` now refuses duplicate weights (the header
    already said catalog indices); a test pins it.
  - The combination key buffer gets 16 bytes of slack.

  The literal D84 reference still omits the cap and triple-limit
  branches; the new section covers them.
- **P1 (not done; a proposal).** About 3.8 s of system time out of
  17.5 s is arena churn, all per weight set:
  - a dynamic piscina created and destroyed;
  - per-vector axis arrays of size numerus_axium;
  - `tacta` of n x 2 x nv per k.

  The fix would be one scratch arena per search with notare/reficere,
  a growable tacta, and axis arrays only for vectors that reach
  stage 2.

**Plants (4 red + 1 scratch):**
- **Red (4):**
  - cap ignored (K1, the reviewer's);
  - triple limit ignored (K2);
  - duplicates accepted;
  - odd-k null sector + 1 (red through the k = 5 hand cell).
- **Scratch-build check of the new oracle:** a plant that only touches
  k = 7 (odd k > 5) is red ONLY in "cellae zeta_12 contra oraculum"
  (13 disagreements); the zeta_8 oracle and the hand cells stay green.

Suite: 182 checks, about 16 s.

## 2026-10-12 - P1 (review): arena churn removed

**Measured first** (deep 564, D82 greedy, exact rule; this machine):
- 20.2 s wall: 14.5 user + **4.7 sys**;
- 149 MB peak.

The system time was the churn the reviewer found: macOS returning
pages (`madvise`) for memory allocated per weight set.

**Changes (all lossless):**
- **Searches reuse their arena.**
  - `_primus_k` (greedy) and the new `_separat_unum` (exhaustive) take
    the search's arena and notare/reficere around the separation.
  - `piscina_reficere` keeps its blocks (offsets go to zero; nothing is
    freed), so nothing is returned to the OS per set.
  - `activatio_separat` (public, single call) still creates and destroys
    one arena per call, but now allocates little inside it.
- **`tacta` lives in the activation**, allocated with `visum_par` at
  capacity k x nv.
  - **Bound:** a cell enters `tacta` only while its `visum_par` /
    `variabilis` is zero and is then marked. So it enters at most once,
    and the count is at most the number of cells at k.
  - Before: numerus x 2 x nv per k (about 1.4 MB at n = 6).
- **The SAT literal buffer grows by doubling** in the activation
  (`_literae_capere`). Since the arena never frees, the total stays
  <= 2 x the peak need.
  - The starting size is 64 on purpose: small enough that the suite
    exercises growth.
  - Before: numerus x 2 x nv per k (about 4 MB at n = 7).
- **The axis-scan scratch belongs to the activation:**
  - `valores_axium` and `reliquae`, used within one `_vector_parare`
    call only (there is no re-entrancy);
  - the final candidates are copied into the work arena at exact size,
    usually 1-3 axes instead of 476.

**Result:**
- 14.3 s wall: 13.7 user + **0.08 sys**; 76 MB peak.
- Same output (XOR12, 32 32 32 21) and identical statistics (filtro /
  exacte / ligatae), so the decision paths are unchanged.
- Shallow 127 is unchanged at 0.14 s.

**Plants (4 red):**
- growth drops the old literals (the suite crashes, exit 138);
- capacity check off by one (crashes, exit 139);
- candidates not copied;
- verdict lost after the arena reset.

The two crashes show the growth path now runs in the suite.

## 2026-10-13 - probatio_cellae: module state reset between sections (examen 92)

Found from quinta after merging main (cursores ad gradus T5): the
examen-corpus gate went red - probatio_cellae.c was REICE (14
violations, credo rule 92: a static written in a section that neither
parare nor purgare resets) and not in the pinned exclusions. The
statics: the float catalogue f_cat/f_prof/f_torsio/f_n, the oracle's
o_d/o_scala/o_vitium/o_axis/o_n2/o_nd, ref_victor/ref_k/ref_numerus,
o_status_aleae. o_d/o_scala are the real hazard: the zeta_12 sections
set III/XXXII while others assume the initial II/II - under a different
section order a zeta_8 section would have run in zeta_12's field.
Fix: one `_statum_purgare` restoring every one of them to its initial
value, as purgare of all 17 sections. examen ACCIPE (0 violations; the
17 'domestica' - hand-printed section titles - were there before and
are advisory). 17/17 in normal order and CREDO_ORDO=mixtus:7 and :13.
