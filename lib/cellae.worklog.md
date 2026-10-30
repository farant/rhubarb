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

## 2026-10-13 - cellae II T1: point-list catalogs, explicit axis tables

Plan: `project-specs/cellae-plan-2.md` (approved the same day): D94's
engine into cellae for audit P1.

**`catalogus_ex_punctis`.** A catalog from an explicit list, in the
given order: depth, writhe and rounds 0, never truncated. It refuses
(FALSUM, output untouched):
- an empty list, an unordered ring, mixed rings;
- the zero quaternion, which has no sign representative;
- a point repeated up to sign (q and −q, or q twice), which is refused
  rather than silently dropped.
Points are not copied (the same lifetime rule as the closure's
products: valid while their arena lives). Unit norm is not required:
D126's non-unit linear images A·S are legitimate nulls.

**`activatio_creare` and `activatio_axis`.** `activatio_ex_catalogo`
is now `activatio_creare(catalogus, NIHIL, 0, CELLAE_CELLA_D82, …)`.
The constructor split into `_activatio_nova` (allocation),
`_axem_addere` (one axis: key, dedupe, cap) and `_axes_proprii` (the
catalog's own table, first appearance, ±1 skipped).
- Explicit axes must be in the catalog's ring and have a nonzero
  vector part.
- Duplicates, antipodes and multiples merge to the first, which is
  the documented table semantics (`_clavis_axis` divides by the first
  nonzero part, so 2v and −v share a key).
- `CELLAE_CELLA_D94` is refused until T2.

**Axes are stored PURE, (0, v).** Before, the table held whole catalog
elements and found each axis's floats through `axis_elementum` (axis →
catalog index). External axes have no catalog index, so the floats now
come from the axis itself, and the field is gone. Every use (the key,
`quaternio_proximi`, the interval setup) reads only vector parts, so
behavior is unchanged: the whole slice-1 suite (17 sections, D124 and
D82 tables) stayed green before any new test was added.

**Tests (2 sections; 237 checks with slice 1).** The independent route
to D124's 13-axis numbers:
- D124's existing test restricts weights by index inside the full
  catalog (`Quaestio.indices`);
- here each subset is its OWN catalog (`catalogus_ex_punctis` for
  non-null, `catalogus_pars` for null-only), handed the full catalog's
  13 axes through `activatio_axis`;
- both give 328 / 184 (rule) and 328 / 88 (robust) for non-null, and 4
  for null-only.
Also checked:
- the explicit table equal to the own table leaves D124's full numbers
  (1132 / 402, max XOR8) unchanged;
- the non-null subset with its OWN table gives D124's 304 / 106 and has
  fewer than 13 axes (the table matters);
- duplicates, antipodes and doubled axes merge; the cap truncates and
  says so;
- refusals.

**Plants: 10, all red.**
- Point-list catalogs: repeat refusal removed; zero point admitted;
  mixed ring admitted.
- Explicit axes: foreign-ring axis admitted; axis dedupe removed; axes
  stored unpurified.
- Axis floats read from the wrong part. Slice 1's D124 regression
  caught this: the certified filter is only as good as its inputs.
- Truncation not flagged; `activatio_axis` off by one; the phase cell
  admitted before T2.

**Lint note.** The suite prints its section titles by hand in 17
places, which examen flags as house-style (credo v2 prints them
itself). I copied the pattern for the new section, then removed it.
The 17 old ones are left as found.

## 2026-10-13 - cellae II T2: the phase cell (D94)

**The sector reduces to slice 1's machinery.** D94's sector is
#{m in 1..k-1 : cos h <= cos(m pi/k)}, with h = acos(a/|s|) in [0, pi]
(the rotation angle 2h in k sectors, no fold). tan^2 h = |v|^2/a^2 does
not depend on the sign of a, so the per-point data (genus, tan^2 h, the
axis tie set) can still be keyed by {s, -s}; only the sector needs the
sign:
- a > 0: the same count as D82's cell,
  #{m < k/2 : tan^2 h >= tan^2(m pi/k)} (the S_k roots by position).
- a < 0: every boundary m <= k/2 counts (cos >= 0 > cos h). For
  m = k - m' with m' < k/2, cos h <= cos(m pi/k) iff
  tan^2 h <= tan^2(m' pi/k). So
  sector = floor(k/2) + #{m' : tan^2(m' pi/k) >= tan^2 h}. A tie at m'
  means the boundary m = k - m', with rule m and tie set {m - 1, m}.
  The count lands on k - m' for both parities of k (checked by hand,
  then by the oracle).
- a = 0: as D82 (floor(k/2); k even ties with k/2 - 1).
- Real sums: a > 0 gives sector 0, axis nd. a < 0 gives h = pi,
  sector k - 1, axis nd. That is the SAME cell as the zero sum, as in
  D119's exact code: its float clamp (sector k → k - 1) carried into
  the exact layer.

The per-k cell cache inside a point gained the sign of a in its key
(D82 always uses 0). Antipodal searches (`activatio_separat`,
`_exhaustiva`, `_avida`) refuse D94 activations: the antipodal encoding
belongs to D82's cell.

**Oracle, another route.** The plan said a Python oracle. The suite's
own pattern (slice 1: integer Z[sqrt d] oracle in the test) is better
because it runs on every test run. So the test carries D119/D120's
method directly, which differs from the library's:
- the sign of a/|s| - cos(m pi/k) for every m, through squares in
  Z[sqrt2, sqrt3, sqrt6] with `surdus`:
  8A^2 - (4 + 4 cos(j pi/12)) N, j = 24m/k;
- the sector is the number of boundaries with sign <= 0, D119's
  definition as written, with no reflection and no S_k roots;
- axes come from the existing integer Z[sqrt2] oracle.

Every zeta_8 signed sum of up to 4 weights at k = 6, 12, 24: 562,080
cells, rule and tie set, 0 disagreements, in 5.6 s. The regimes are
counted and asserted:
- 18,900 sector ties with a < 0 (the reflected branch);
- 58,608 axis ties;
- 639 real negative sums;
- 624 zero sums.

**Hand cells.**
- D120's cells: sigma_1 at k = 12 is on the boundary m = 3 (sector 3,
  tie {2, 3}); at k = 6, sector 1 with no tie.
- Their mirror -sigma_1: h = 3 pi/4, on m = 9 (sector 9, tie {8, 9});
  at k = 6, sector 4.
- D82 folds sigma and -sigma into one cell; D94 does not.
- The real sums: +1 is cell nd, and -1 is the zero sum's cell.

**Plants: 8, all red.**
- The reflected sector off by one.
- Equality not counted in the reflection.
- Real negative sums put in sector 0.
- The sign left out of the cell cache key, so s and -s shared a cached
  cell.
- The sign of a read from the wrong part.
- Ties dropped in the a < 0 branch.
- Antipodal searches accepting a D94 activation.
- An unknown cell code accepted.
The first six were caught by the oracle comparison alone. The last two
were caught by the refusal checks.

## 2026-10-14 - cellae II T3: signed encoding, `activatio_iudica`

**What it is.** One weight set, one truth table, a ladder of k. Mask bit
i set means +q_i, clear means -q_i (no zero coefficients, unlike the
antipodal search's 3^n vectors). Rule, robust and possible are each a
union over k, as in D94/D119; possible is searched only where robust
failed and possible is not yet established (D119's order, so the
counts line up). Both cells are accepted. D82 folds s and -s, which
has a consequence worth knowing (below).

**Orientation.** The point cache is keyed by {s, -s}, and so is the
combination cache (`#±idx,` folded by the first coefficient). Under
D94, s and -s are different cells, so every vector now carries an
`orientatio` (+1/-1). It says whether its sum is the point's
representative (first nonzero part positive) or its negative. The
combination entry stores the orientation of the canonical combination
(first coefficient +1), and a lookup multiplies by the vector's own
`primum`. The exact cell takes sign(a) = orientation (a != 0; a = 0
and s = 0 pass 0). The SAT's point identity got a second visitation
slot (`_negati`), so s and -s are separate variables under D94.

**Bug found by the concursus oracle: the zero sum has no orientation.**
A signed relation (D126) gives two OPPOSITE masks of the same
combination whose sums are both 0: in 2I, s + s* = 1, so over weights
{1, s, s*} masks 001 (1 - s - s*) and 110 (-1 + s + s*) are both 0.
They share the combination key with opposite `primum`, so they got
opposite orientations, and the D94 concursus check ("same point AND
same orientation") called them different. Fix: the zero point's
orientation is always +1. The test's oracle (plain exact quaternion
sums, compared pairwise, sharing none of the library's dedup) found 32
missed sets in zeta_8 XOR n = 3 alone. The verdict counts were right
all along: the zero cell has one option, so the SAT identity split
never changed an answer. Only the concursus flag was wrong.

**`concursus` is complete, not a by-product.** The first draft set it
inside the SAT search. That would be a partial witness, because the
search runs only at some k and stops at the first conflict, and it sees
point identity only for tied vectors. Now it is computed after the k
loop, only when possible is false (a concursus implies the same cell at
every k, so possible is impossible): every pair of masks with different
values. A pair is skipped when one component's float intervals are
disjoint (D82: also against -w). Otherwise it is decided exactly by
point identity. A concursus clears `indecisa`, because it proves
possible false.

**D82 under signed encoding.** The complement mask has the negated sum,
and D82 folds it into the same point. For odd n, XOR flips parity under
complement, so EVERY odd-n XOR set is a D82 concursus. Signed odd-n XOR
is impossible with the folded cell, which is structural and not a
finding. Hand-tested (n = 1, n = 3).

**The float filter for D94, a < 0.** It is the reflected count from the
largest boundary down, matching the exact branch from T2: count while
terminus > tan^2 h strictly, stop when terminus < tan^2 h strictly,
undecided otherwise. The sign of a comes from the a-interval, which
`_vector_parare` already requires to exclude 0. Equivalence is checked
set by set with the filter off, every n = 3 set and every 5th n = 4 set,
XOR and MAJ: 8,300 judgments, 0 differences. That moved the plan's T4
filter work into T3, so T4 is now measurement only.

**Regression: D119's frozen exact tables, every exhaustive row.**
zeta_8 (24 elements, own 13 axes) and 2I's first 24 (closure of
(s, s*, t, t*) with D120's BFS and its ± dedup, so the closure is 60
points, not 120; own 12 axes). For n = 3..6 and each of XOR, AND, MAJ:
the set count, rule, robust, possible, tied, and no undecided set.
- n = 3, 4: all equal, 5.8 s, plus the concursus oracle on every set.
- n = 5, 6 (credo `lentus`): all equal, 39.7 s, 463 MB peak RSS.
The memory is the activation's per-combination cache growing over
about 1M judgments. Whether to bound it (reset per weight set, or an
LRU) is a T4 question; the D119 sweep does not reuse combinations
across sets much.

**Hand cases.**
- n = 1, weight 1: D94 separates +1 (sector 0) from -1 (the zero-sum
  cell); D82 folds them into one point (concursus).
- n = 3, zeta_8 {0, 1, 2}, XOR: D82 concursus; D94 none.
- 2I {1, s, s*}: XOR concursus under both cells; AND none (the only
  {-1, 0, 1} relations are ±(1, -1, -1)).
- The truth tables, and refusals with `*exitus` untouched.

**Plants: 16, all red (one only after a test was added).**
- The zero sum keeping an orientation (concursus oracle).
- The orientation stored without `primum` (rule counts).
- Concursus ignoring orientation under D94. This one was caught by the
  filter on/off comparison and not by the oracle rows. With the filter
  on, s and -s have disjoint intervals and never reach the exact check.
  So the filter-off run is what guards that line.
- The D82 disjointness test without the opposite sum (hand case n = 1).
- The reflected filter off by one; the float sign inverted; the
  reflected filter deciding an overlap (rule/possible counts).
- s and -s sharing one SAT identity under D94 (possible counts). So the
  second visitation slot changes verdicts, not just bookkeeping.
- possible not seeded by robust; ties checked at the first k only; the
  mask bit inverted; MAJ as >= n/2 (D119 counts).
- A repeated weight or a table value 2 accepted (refusals).
- Concursus never computed (oracle).
- **Survived at first:** the rule verdict taken from the last k only.
  On {6, 12, 24} every boundary of k = 6 and 12 is also a boundary of
  k = 24, so k = 24's cells refine the others, and rule-separable at 6
  or 12 implies rule-separable at 24. Union = last k on that ladder.
  Fixed by judging every N = 3 row again on {24, 12, 6} (the union is
  order-independent); the plant is red there.

## 2026-10-14 - cellae II T4 checkpoint: N = 8, memory, cells once

**Harness.** `mensura.c` (scratchpad, built with aedilis via a relative
path out of the tree) reproduces D120 Parts B-C through
`activatio_iudica`. It uses the same rows, exhaustive where C(24, N)
<= 200,000, and otherwise 100,000 sets from D120's seeded LCG stream
(seed base + N, shared by XOR/AND/MAJ in order). Rows are checked
against D119_B/D119_C. All 36 rows are equal, now including the 12
sampled N = 7, 8 rows that the credo does not cover. The baseline is
D120 `MEDIUS` on the same machine and day: 125 s, 5 MB peak.

**N = 8 was refused.** `activatio_iudica` inherited the antipodal limit
(VII). D94/D119 go to N = 8, so the signed encoding gets its own limit,
`CELLAE_N_SIGNATA_MAXIMA` (VIII); the antipodal searches keep
CELLAE_N_MAXIMUS. T3's regression used only exhaustive rows (N <= 6),
so this surfaced only with the sampled rows. New credo: 21 N = 8 sets
spread over C(24, 8), filter on/off and concursus oracle.

**Memory: count, don't guess.** Peak RSS was 949 MB for zeta_8 to
N = 7. Two guesses moved little:
- exact scratch in `_punctum` moved to the call arena: -5%;
- `per_k[64]` inline in every point became a growing array (4, 8):
  -10%.
Byte counters per allocation site (a temporary instrument, removed
before commit) then showed that the combination cache was 97% of the
arena: 3.25M entries, while there were only 38k distinct points.
- Why it never pays: in the signed encoding a key names the WHOLE
  weight set, so another set never hits it.
- Why it looked useful: the same set judged for another truth table
  does hit it (AND/MAJ ran 5x faster after XOR).
- Hash tables doubling inside an arena abandon each old table, hence
  the jumps.
`activatio_iudica` now keeps the table per call (`piscina_combinationum`
swapped for the call). Points stay on the activation, where they are
heavily shared. Peak RSS: 36 MB to N = 7, 69 MB for the N = 8 rows.
The antipodal searches keep the activation table, since their
sub-combinations do recur across sets. Guard: credo "iudicium
memoria". All ζ₈ N = 4 XOR sets grow the activation arena by less than
5 MB (15 MB before), and a second pass over the same sets allocates
exactly 0 bytes there.

**Cells once per (mask, k).** The tie loop, rule, robust and possible
each recomputed every mask's cell, up to 4 times per k. A per-k memo
(`_cella_vectoris`) is filled lazily, so rule keeps its early exit.
Robust implies rule (the rule cell is among the tie cells), so robust
is skipped when rule fails. Float cells per row roughly halved.

**Exact cells use the vector's float tan^2 h.** `radix_compara` was
19% of the 2I profile: once the vector-level filter gave up, every
boundary was compared exactly. Now `_terminum_comparare` decides each
boundary by the vector's certified tan^2 h interval (tan^2 is
sign-invariant, so it encloses the point's value) against the filter's
own boundary enclosure. Only overlaps reach `radix_compara`. 2I went
from 2-5x slower than zeta_8 to equal. The filter-off credo now also
covers this: with the filter off there is no hint, so that run is the
pure exact path.

**Binary combination keys.** 4 bytes per weight, ±(index + 1),
replacing sprintf text (~5-7% of the profile).

**Standing.**
- Both tables, N = 3-8: about 265 s here, against D120's 125 s;
  36/36 rows equal to D119.
- Profile now: about 46% exact sums and exact point keys through
  general algebraic numbers (`_vector_punctum`), about 20% float
  intervals (`_vector_parare`). The integer image of the catalog
  (T4b) targets the 46%.
- Timing noise: load average was about 5 from other sessions'
  builds. Profiles and per-row filter/exact counts are the reliable
  signal; wall times are +-15%.

**venenum found a dead gate.** `tools/venenum_probare.sh` builds
cellae from a hand-written source list, and since T2 the test needs
`surdus`: the cellae suite did not even link under venenum. Nobody saw
it, because the gate's `<porta>` declaration covered neither
`probatio_cellae.c` nor `lib/cellae.c`, so commissio never owed venenum
for cellae changes. Fixed both: `lib/surdus.c` added to the list, and
cellae (+ surdus) declared. Under poison and ASan/UBSan the cellae suite
passes (17,292 checks), so the `_punctum` arena reset leaves nothing
dangling. The hand-written link lists remain debt (aedilis knows the
sources).

**Plants: 8, all red.**
- Combinations back in the activation: the memory credo.
- The memo always hit: a crash, exit 138.
- Robust searched only when rule fails (inverted guard): D119 robust
  counts.
- Hint sign inverted; hint deciding on overlap; combination key without
  the sign: first red in "separatio contra D124" (the antipodal path
  shares `_vector_cella` and the key; that section runs before the D119
  rows).
- per_k growing without copying: the slice-1 oracle (cached cells
  lost).
- N = 8 refused again: the N = 8 credo.
Not planted: dropping the `_punctum` reset of the call arena. That
scratch dies with the call anyway, so no correctness test can see it
(an equivalent mutant); it only lowers one call's peak.

## 2026-10-14 - cellae II T4b: integer image of the catalog

**Design.** At creation each catalog part becomes `(p + q alpha)/D`:
alpha is the field's generator, D the least common denominator, and
p, q are s64 with |p|, |q| <= 2^40. A signed sum of up to 8 weights is
then 8 integer additions per weight (< 2^43), and two values are equal
iff their integer tuples are equal, because {1, alpha} is a basis over
Q. The tuple is the exact point key. A coefficient over the bound, or
D outside s64, means no image: the activation runs the old algebraic
path, and nothing is rounded.
- Basis {1, alpha}, not {1, sqrt d}. Keys only need some fixed basis,
  so there is no square root to find and no factoring. It should
  also cover `extensio_cosinus` fields of degree 2 (alpha = 2cos 72°
  for n = 5), but the credo does not exercise them yet.
- `surdus` is not used. The sign of s is never needed on the hot
  path. Canonical form of {s, -s} = the tuple whose first nonzero
  integer is positive; epsilon = s / canonical. The real
  representative (first nonzero part positive as a real number) is
  related to the canonical tuple by a sign `relatio`. It is computed
  once, when the point is created (`_orientatio` on the algebraic sum),
  and stored on the Punctum. A sum's D94 orientation is then
  epsilon x relatio, with no algebraic sign at all. The zero sum gets
  epsilon = +1 and relatio = +1 automatically: the T3 bug cannot come
  back through this path.
- Algebraic numbers appear only on a point-cache miss: the quaternion
  sum is built for the point's own data (axes, tan^2 h).
- The combination cache is bypassed on the image path. Complements
  fold through epsilon, and the point table is the only table.

**One value, one point (namespaces).** Image keys are binary: byte 1,
then 8 x 8 bytes. General keys are quaternion text, which never
contains byte 1, so the two namespaces are disjoint. Which namespace a
value uses depends on the value itself, not on the path that produced
it. `activatio_cella` converts its sum into the image when every
coefficient x D is an integer in s64, so an external sum equal to a
search sum finds the same point. Credo "imago nomina" checks this by
counting points: 8 sums through `activatio_cella`, then
`activatio_iudica` on the same set creates no new point. The
discrimination check (cell without the image, judgment with it) does
create points, which shows the test can see a split. Switching the
image between calls is allowed and documented: the same value can then
hold one point per namespace, with identical cells, since both are
exact.

**API.**
- `activatio_imago(act, utere)`, mirroring `activatio_filtrum`; off =
  the old algebraic path, the equivalence oracle.
- `CellaeStatistica` gained:
  - `imagine` / `algebraice`: sums identified by each path. A silent
    fallback cannot pass for speed; the harness prints both.
  - `puncta`: distinct exact points held. It serves the namespace
    test and memory diagnosis; T4's instrument counted the same thing
    by hand.
- Hygiene: `activatio_cella` resets the arena after the D94 sign
  computation, so a cached point and cell allocate nothing.

**Measured** (same harness and rows as the T4 checkpoint, load ~4):
- Both tables, N = 3-8: **113 s** (T4: ~265 s; D120: 125 s).
- 36/36 D119 rows equal.
- Every sum went through the image (algebraice 0); peak RSS 66 MB.
- Profile at N = 8, zeta_8, 15 s of samples:
  - `_vector_parare` (float intervals): ~35%;
  - self time in `activatio_iudica` (inlined `_iudica_intra`,
    concursus pair scan): ~30%;
  - `_cella_vectoris`: ~14%;
  - exact sums + keys + hashing: ~3% (was ~46%).

**Credo (3 new sections).**
- "imago sine iactura":
  - Iudicium identical with and without the image, set by set, filter
    OFF, so every sum goes through a point;
  - zeta_8 and 2I (D94), plus zeta_8 (D82);
  - N = 3 all sets, N = 4 every 5th, N = 8 every 36000th;
  - XOR and MAJ;
  - with the image: algebraice delta 0; without it: imagine delta 0.
- "imago limites", on a catalog of 5 points in Q(sqrt 2):
  - a coefficient of exactly 2^40 gets an image;
  - 2^40 + 1 and a denominator 3^40 (> 2^63) get none, with the same
    verdicts.
- "imago nomina": namespaces (above), plus a non-representable sum
  (1/3 with D = 2) going the algebraic way, with cells equal to the
  image-off activation.

**Plants: 8, all red.**
- Key not canonical (epsilon always +1, so s and -s get two keys):
  red in the D82 hand cases ("iudicium manu", concursus: D82 must
  fold s and -s).
- relatio without epsilon (planted as epsilon x epsilon x relatio;
  the plain removal does not compile): D119 rule counts.
- Key without its last component (values collide): the cells oracle
  in "cellae contra oraculum".
- Bound strict (2^40 refused) and bound loosened (2^41): "imago
  limites", in both directions.
- Remainder ignored (a non-integer value goes into the image): "imago
  nomina".
- `activatio_cella` skips the image: "imago nomina".
- Switch ignored: "imago sine iactura".

## 2026-10-14 - cellae II T5: review fixes (filter scale, caps, image bound)

The T5 review checked ~63k weight sets and ~46k `activatio_cella` cases
against an independent Fraction oracle (scratchpad, not committed). It
found one serious bug (H1), two medium (M1, M2), two low (L1, L2) and
three notes (N1-N3). All were fixed except N2/N3, which stay as notes.

**H1 - the float filter assumed axes near unit scale.** Axis doubles
came from `_duplex_ex`: 20 decimal PLACES, an absolute error. The filter
then treated them as relatively accurate (stage-1 margin 1e-15 |v|).
- An axis around 1e-17 kept ~3 digits: wrong direction, and the true
  maximum was pruned.
- At 1e-25 the axis printed as 0. The unit vector became NaN and was
  pruned by `>=`.
- When every value interval was garbage, `optimus` stayed -1 with one
  candidate: cell index -1 + sector, then an out-of-bounds write
  (SIGBUS in `_separat_ad_k`).
- Filter-off verdicts were always right; filter-on verdicts were wrong,
  including `ligata`.
Fix: `_axis_normata` divides the vector part exactly by its
largest-magnitude component, compared via `algebraicus_compara` on the
squares. So |u| is in [1, sqrt 3] and the 20 decimals are a RELATIVE
error of 1e-19, whatever the axis scale. The filter is scale-invariant
in u ((v.u)^2/|u|^2), so cells do not change. If normalisation or
conversion fails, or |u|^2 is not certainly > 0,
`a->axes_certae = FALSUM`: the filter abstains from axis selection and
the vector goes exact over all axes. Also:
- the vector decides only with a real axis (`optimus >= 0`) and a valid
  tan^2 h;
- tan^2 h is computed only if a^2's interval is > 0 (underflow guard);
- stage 1 and the candidate cut keep NaN entries (written as
  `!(x < y)`).
The last three are defence in depth: after normalisation they cannot
fire, so their plants survive (equivalent mutants, see below).

**M1 - huge elements failed the whole judgment.** Parts >= ~1e43 do not
fit `_duplex_ex`'s 64-byte buffer. `_fluitans` returned NIHIL and
`_vector_parare` returned FALSUM, so `activatio_iudica` returned FALSUM
while filter-off judged correctly. Now the failure is remembered per
element (`fluitantia_parata` = 2) and the vector takes the exact path.
Huge AXES are covered by the normalisation. Honest limit: at 1e45 the
filter decides nothing (credo asserts filtro = 0, exacte > 0). A
relative-precision conversion would lift that; not done.

**M2 - three silent caps of 64.**
- `termini[64]`: distinct k per activation, then FALSUM forever.
- Per-point (k, sign) memory: 64 entries, so 32 k on D94.
- `_primus_k`: the avida scale was cut to its first 64 entries, and the
  rest were dropped SILENTLY.
Whether a call failed depended on call history. Now the termini table
is a growable pointer array (pointers stay stable), per_k just keeps
doubling, and the avida scale lives in the query arena at full length.
`CELLAE_K_MAXIMA` is gone.

**L1 - signed overflow.** `activatio_cella` accepted any s64 image
coefficient. Then `t[m] * epsilon` at -2^63 is undefined behaviour.
External sums now enter the image only when |coefficient x D| <= 2^62
(`CELLAE_IMAGO_SUMMA_LIMES`). Catalog sums stay below 2^43 anyway.

**L2 - D94 judged only at k in {6, 12, 24}.** No odd k, so the a < 0
sector reflection and h = pi at odd k went untested. The reviewer's
plants pl1 ((k+1)/2), pl7 (real negative only for even k) and pl3
(both, consistently in filter and exact) survived the whole credo.
Now:
- the phase oracle (surdus, cos(j pi/12)) also runs k in {1, 2, 3, 4,
  8} for n <= 3, asserting odd-k a < 0 cases > 100 and odd-k real
  negatives > 0;
- hand cases for sigma, -sigma and -1 at k = 3 and 5;
- "iudicium filtrum sine iactura" compares ladders {3, 5} and
  {7, 2, 1} filter on/off for n <= 4.

**N1 - scratch in the activation arena.** `activatio_cella` now runs in
its own dynamic arena per call (`_activatio_cella_intra`). Image test,
key, new point and sign of a all go there, and the key is transcribed
once. `_terminum_comparare` also resets the activation arena around
`radix_compara`, which works on values only. That scratch leaked on
every new (point, k) cell, on the iudica path too. No test can see
either: these are memory-only (equivalent) mutants.
N2 (k (nd+1) wraps at k ~ 2^31 / (nd+1)) and N3 (the `ligatae`
statistic differs between filter settings, since it counts the exact
cells) stay as notes.

**Credo: 4 new sections.** "imago summae limites", "filtrum scalae",
"filtrum testes recensionis", "k sine limite"; plus the L2 extensions.
- "filtrum scalae":
  - zeta_8's own axes x 10^-17, 10^-20, 10^-25, 10^45 as explicit axes;
  - the catalog x 10^45;
  - all n = 3 sets, XOR and MAJ, against filter-off at unit scale;
  - each scaled-axes activation must show filtro > 0, so a filter that
    silently gives up cannot pass.
- "filtrum testes recensionis": the reviewer's witnesses (crash1,
  own-axes 1e-25 / 1e-20), all truth tables for n = 2, 3, k in
  {1, 2, 3, 4, 6, 12}, D82 and D94, filter on = off. Before the fix
  both scale sections died with SIGBUS (rc 138).
- "k sine limite":
  - one point at 40 k x 2 signs, against a fresh activation per k;
  - `activatio_iudica` with k = 1..70, filter on = off;
  - avida with 64 x k = 1 then 6, equal to {1, 6}.
- Totals: 32 sections plus 1 lentus, 110,245 checks, ~50 s (was 46 s);
  lentus 18 s, green.

**Plants: 12; 10 red, 2 survive (equivalent).**
- Axes not normalised (quotient = raw part): "filtrum scalae" (filtro > 0:
  the |u|^2 guard makes the filter abstain, and the check sees it).
- `axes_certae` always FALSUM (filter silently gives up): "filtrum sine
  iactura" (filtro > exacte).
- Huge element -> FALSUM again: "filtrum scalae".
- termini cap 64, per-point cap 64, avida scale 64: "k sine limite"
  (three asserts, one each).
- External image sums unbounded: "imago summae limites".
- pl1, pl7 (reviewer): "cellae phasis contra oraculum" (odd k).
- a < 0 sector (k + 1)/2 in the filter only: the {3, 5} ladder in
  "iudicium filtrum sine iactura".
- Survive: `optimus >= 0` guard and NaN-keeping in stage 1. After
  normalisation every axis interval is finite and > 0, so neither can
  fire; kept as defence in depth.

**Reviewer re-run.** Rebuilt the review driver on the fixed library:
0 disagreements on all its witnesses (t_tiny_own 5,400 cases, was
138/900 wrong; t_scale 1e20/1e43/1e45 with 0 FALSUM refusals;
crash1 and own_crash exit 0; ovf.in ASan/UBSan clean). The full batches (12 catalog runs x
D82/D94, 8 random-point runs; ~47k weight sets) also show 0
disagreements.

## 2026-10-29 — temporary pools start at 4 KB (budget B1 D5)

Temporary piscinae created and destroyed per call in hot paths
(radices `_piscina_temporaria` — radix_compara, _separa_positivas, ...;
cellae `activatio_iudica`) started at 64 KB. Under ASan (venenum gate)
every create/destroy pays malloc, quarantine put/recycle and malloc-fill,
which scale with size (~8% of probatio_cellae's ASan profile, more in
"imago sine iactura"). Now 4 KB first alveus; dynamic piscinae grow when a
call needs more. Behaviour unchanged (no plant possible — sizing only);
radices, cellae, quaternio green.
