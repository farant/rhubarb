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
