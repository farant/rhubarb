# Demo 129: How many linear regions does a braid network carve? (plan)

Status: approved by Fran (2026-10-16), with all five proposals in
"Open for Fran": the exact planar sweep in `nervus` (bias-free, 2
inputs); the census helpers `plexus_periodicus` / `plexus_enumerare`
in plexus; itinerary refinement in the demo; scope B_3 to 8 exact,
B_4 to 4 at R <= 8 plus an R = 16 sample; the registered rule as
written. Task T5 of
`project-specs/nervus-plan.md` (opus …F75N). Proposal N6 in
`knotapel/demo-proposals.md` (ledger …N84R01). Built on `nervus` and
`plexus`.

## Question

A braid word w on n strands is an exact, bias-free ReLU network on
the 2n - 4 Dynnikov coordinates (D128, KC 55; now `plexus_nervus`).
Its iterate w^m is a network m times as deep. The standard measure of
what a ReLU network can express is the number of LINEAR REGIONS it
cuts its input space into (Montufar, Pascanu, Cho and Bengio, "On the
number of linear regions of deep neural networks", NeurIPS 2014: deep
networks CAN make exponentially many in depth; that is a statement
about the best weights, not about given ones).

1. How does the number of regions of w^m grow with m, braid by braid?
2. Does it depend on the braid's dynamics: periodic (P), positive
   entropy (E), zero entropy and not periodic (Z)? For E, on the
   dilatation lambda?

This is D106's question ("does chaos help computing?") asked of the
right object: the network itself, not a linear quotient (KC 31.4),
with exact classes (KC 54), not Burau's.

## What to expect, and why it is in doubt

N6 (written after D128 T6b) proposed: regions grow exponentially for
E at a rate tied to lambda, stay bounded for P, grow polynomially for
Z. Two things since then point elsewhere.

- **Itineraries.** The regions of w^m are the cells of the partition
  "which region of w does each of the m passes land in" (the
  itinerary). A pseudo-Anosov map acts on projective lamination space
  with north-south dynamics (Thurston; Fathi, Laudenbach and Poenaru):
  every point except the repelling class flows to the attracting one.
  Once a point's orbit sits in one region (or one cycle of regions)
  near the attractor, its itinerary only repeats. New cells of w^m
  then come only from points ever closer to the repeller, a few per
  step. That suggests LINEAR growth in m, not exponential.
- **D128 Part D.** Every orbit of the census settled within 5 passes
  into a periodic pattern, using 2-3 regions per loop (KC 55.2.1).

Both are suggestive, not proofs; the plan registers both hypotheses
and a rule that decides between them.

A side question with a stake in KC 55.2.3.1: D128 found period 2 for
384 E braids, mostly those certified only at a cube root of unity. If
the attracting lamination lies ON a wall of w's regions (a tie in the
limit), orbits near it alternate between the two sides. Part C tests
whether period 2 coincides with the attractor on a wall.

## What is counted (definitions)

- **Activation region** of a network: a maximal set of inputs with the
  same activation pattern. For w^m the pattern is the concatenation of
  the m per-pass patterns (= the itinerary). Braid networks have no
  biases, so every region is a CONE from the origin.
- **Linear piece**: a maximal connected set on which the map is one
  linear map. Adjacent regions can share a map, so pieces <= regions.
  Both are reported; the headline is regions (what the network's
  ReLUs carve), pieces is the function's own count.
- **Interior point**: an input at which no ReLU argument is exactly 0
  in any pass (`NervusForma.nullae == 0` each pass). An interior point
  lies inside a full-dimensional region. A point on a wall gets the
  pattern of a lower-dimensional cell, which is NOT a region (the T4
  review's N1: |x| has 3 patterns on -5..5 but 2 regions).

Two counting methods:

1. **Exact, in the plane (B_3, d = 2).** Regions are sectors bounded
   by rational rays. A sweep over the circle keeps a list of rational
   rays with, on each arc, the linear form of every register; at each
   ReLU it splits an arc where the argument's linear form changes
   sign (a rational ray, found exactly). After m passes the arcs ARE
   the regions of w^m, counted exactly, walls included. Merging
   adjacent arcs with the same 2x2 matrix gives the pieces.
2. **Lattice lower bound (B_3 cross-check, B_4 with d = 4).** Distinct
   itineraries of INTERIOR integer points on the shell max|x_i| = R
   (by homogeneity, a shell samples directions). Each distinct
   interior itinerary is a distinct full-dimensional region, so the
   count is an exact LOWER bound on the number of regions; the union
   over shells R' <= R is nondecreasing in R. Itineraries are counted
   by refinement: id_m(x) = intern(id_{m-1}(x), key_m(x)), so level m
   costs one hash lookup per point.

The exact polyhedral count in d = 4 needs exact linear programming
(out of scope); the B_4 numbers are lower bounds and say so.

## Parts

- **Part A: exact sector counts on B_3.** Every braid of D128's B_3
  census (2,589 classes up to length 8), m = 1..12: regions and pieces
  of w^m, exactly. Classes from D128's method (P exact periodicity, E
  certified, Z the rest). *Amended (T5.0 review M1, Fran 2026-10-16):*
  regions belong to the WORD, pieces to the braid (three geodesic
  words of Delta sigma_1 give 12, 12 and 6 regions at m = 4, the same
  pieces). Per class: pieces (the headline, a class invariant), and
  regions as MIN and MAX over ALL geodesic words of the class
  (enumerate reduced words up to length 8, group by `plexus_clavis`,
  keep those of the class's length), plus the census
  representative's. The min-max gap is itself a result: how much the
  choice of word carves beyond what the function needs.
- **Part B: the lattice bound against the exact count (B_3).** The same
  braids, shells R = 4, 16, 64, 256: how close the lower bound gets,
  and how many shell points are on walls (nullae > 0). Validates
  method 2 where method 1 is exact.
- **Part C: B_4 lattice bounds.** (Pieces are not countable by the
  lattice method; Part C reports regions per word, min and max over
  the geodesic words of each class, as in Part A.) D128's B_4 census up
  to length 4
  (469 classes), m = 1..8, R = 4 and 8; R = 16 on a sample of 20
  braids per class (marked SAMPLED). Side question: for E braids,
  whether Part D's period-2 braids have their attracting direction
  (the settled orbit's projective limit) on a wall of w.
- **Part D: nulls.** (a) Burau (-1) / any linear network: one region by
  construction (reported as the floor, not as evidence); (b) random
  bias-free networks with the same shape (letters' programs with the
  +-1 coefficients permuted at random, same depth and ReLU count);
  (c) shuffled words (same letters, random order: a different braid,
  so its class is recomputed). Same counting, same m.

## Checks (characterizations, not counts)

- Method 1: the arcs cover the circle exactly once; on every arc, the
  stored matrix equals `nervus_forma_affinis` of the arc's pattern at
  a rational interior direction; evaluating w^m at that direction
  through `plexus_nervus` gives the arc's pattern.
- Periodic braids (a theorem, checked exactly): if beta^k is a power
  of Delta^2, which fixes every loop of D_n, each point returns after
  k passes, so its itinerary is fixed by its first k passes and
  r(m) = r(k) for every m >= k.
- Method 2 vs 1 (B_3): the lattice bound NEVER exceeds the exact count
  (a theorem-check on the code), and every interior shell itinerary
  matches an arc of method 1.
- Homogeneity: scaling a point by 2 never changes its itinerary.
- Monotonicity: regions(w^(m+1)) >= regions(w^m) (refinement).
- Pieces (amended, T5.0 review M1) are NOT monotone: they count the
  function, not a refinement. For a periodic braid w^k is the identity
  map (beta^k a power of Delta^2 fixes every loop), so p(k) = 1 and
  p(m + k) = p(m): pieces cycle with period k, while regions level off.

## Registered before running

- **H_lin vs H_exp** (the main question), decided on Part A's exact
  B_3 counts, per E braid, over m = 6..12, applied TWICE (amended,
  T5.0 review M1): to the pieces p(m), and to the regions r(m) of the
  geodesic word with the most regions at m = 12. Both verdicts are
  reported; "H_lin holds" needs both. Below, r stands for either:
  - H_lin holds for the braid if the increments r(m+1) - r(m) are
    eventually periodic with bounded values (checked: the increments
    over m = 6..12 repeat with some period <= 3);
  - H_exp holds if r(m+1)/r(m) >= 1.5 for every m in 8..11.
  - Prediction: H_lin for >= 90% of E braids on B_3; H_exp for none.
    If H_exp holds anywhere, its rate is compared with lambda.
- **Z braids:** prediction: linear as well (parabolic dynamics, a
  twist curve attracting), slope smaller than typical E. Weakly held.
- **B_4 (lower bounds):** same direction as B_3, H_exp for none at
  m <= 8; stated as "consistent with", never as exact.
- **Period 2 (KC 55.2.3.1):** prediction: the period-2 E braids are
  exactly those whose settled orbit's limit direction makes some ReLU
  argument of w vanish in the limit (a tie). Decided on Part C's B_4
  braids by exact computation on the settled linear map's dominant
  direction where it is rational, otherwise marked undecided.

How the outcome reaches claims.md: a new node under KC 55 ("how many
regions does w^m carve?"), graded [exact] for B_3 counts and
[exact-search] for B_4 lower bounds; KC 31.2's question (does
pseudo-Anosov compute better?) gets a pointer only: regions are
capacity, not computation, and N2 asks that.

## Size and time (estimate)

Measured: about 1.4 million passes per second of a 6-letter B_4 word
(8 million letters/s) on small values; values grow like lambda^m, so
late passes cost more.
- Part A (B_3, exact sweep): arcs per braid ~ tens to hundreds, 12
  passes, 2,589 braids: seconds to a minute.
- Part B: 4 shells up to R = 256 (2,048 points) x 2,589 braids x 12
  passes = 64M passes: about 1-2 minutes.
- Part C: R = 8 shell (32,896 points) x 469 braids x 8 passes = 123M
  passes: about 2-3 minutes; R = 16 on 60 braids (262,400 points):
  126M passes, 2-3 minutes.
- Quick mode (D129_CELER): B_3 up to 5, B_4 up to 3, R <= 8.

## Tasks (one per turn, Fran's go-ahead each)

0. **Library first (see Open for Fran 1-2):** the exact sector sweep
   and the census helpers, with tests, plants, review.
1. **T5a:** Parts A and B (B_3: exact counts, lattice cross-check),
   checks and plants.
2. **T5b:** Parts C and D (B_4 bounds, period-2 side question, nulls).
3. **T5c:** findings.md, claims.md node(s), ledger, archive.sh.
4. **T5d:** independent review with its own oracle, then fixes.

## Decisions made during task 0

- **T5.0b (census helpers):** `plexus_clavis`, `plexus_periodus`
  (renamed from the proposed `plexus_periodicus`: the noun is a known
  word; it returns the MINIMAL period, testing every divisor of n and
  n-1, where D128 tried only n and n-1), `plexus_enumerare` (first
  word reaching a class represents it; within a level sorted by key).
  Checked against D128's pinned counts and, on short censuses,
  against Artin directly. See lib/plexus.worklog.md.
- **T5.0a (planar sweep):** `nervus_regiones_plani(rete, piscina,
  &planum)` returns `NervusPlanum {numerus, regiones, frusta}`, each
  `NervusSector` with walls `initium` / `finis` (primitive integer
  rays, ccw, the first wall at or after (1, 0)), an interior direction
  `medius`, the pattern, the exitus x 2 matrix and its piece index.
  One sector per pattern (regions are convex), so "regions = distinct
  patterns" in the plane. `forma.nullae` of a sector counts ReLUs whose
  argument is IDENTICALLY 0 there. **Consequence for Parts B/C:** 345
  of 400 random networks have such a sector, and the lattice criterion
  "interior = nullae == 0" never sees it. Part B measures whether braid
  networks have any (exact sweep vs lattice on B_3); if they do, the
  B_4 interior test needs the argument forms under the point's pattern
  (decided at T5a). See lib/nervus.worklog.md.

## Out of scope

- Exact polyhedral region counts in d >= 3 (exact LP).
- Computing Boolean functions with the network (N2).
- The exact dilatation from the settled map (plexus v2, …VFF141H).

## Open for Fran

1. **The exact planar sweep lives in a library.** It is a general
   exact algorithm on networks (the activation regions of a bias-free
   network with 2 inputs), so by the knotapel rule it belongs in
   `nervus`, e.g. `nervus_regiones_plani(rete, ..., &arcus)` returning
   rational rays with each arc's pattern and matrix. Proposed:
   bias-free 2-input only (regions are sectors). The general planar
   case with biases (polygons) is more work and has no consumer yet.
2. **Census helpers move to plexus.** D129 needs D128's exact class
   enumeration (shortest-length breadth-first over exact classes) and
   the periodicity test (beta^n or beta^(n-1) a power of Delta^2),
   both general braid algorithms now living in D128's demo code.
   Proposed: `plexus_periodicus(p, &k)` and `plexus_enumerare(n, L,
   ...)` in plexus, tested against D128's pinned counts (2,589 /
   5,121, per-level P/E/Z). Alternative: copy them into D129 (faster,
   but against the rule).
3. **Itinerary refinement stays in the demo** (an intern table of
   (previous id, pass key)), unless N2 turns out to need it too.
4. **Scope:** B_3 to length 8 exact, B_4 to length 4 at R <= 8 plus
   the R = 16 sample (proposed), or larger.
5. **The registered rule above** (H_lin vs H_exp thresholds) as
   written, or tighter.
