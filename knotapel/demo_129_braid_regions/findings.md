# Demo 129: How Many Linear Regions Does a Braid Network Carve? — Findings

Proposal N6 (ledger …N84R01). Plan: `plan.md` (approved 2026-10-16,
amended after the T5.0 review; its "T5a" and "T5b" sections record
each task's decisions). Questions:

1. A braid word w is an exact, bias-free ReLU network on the Dynnikov
   coordinates (D128, KC 55). How does the number of LINEAR REGIONS of
   its iterate w^m grow with m?
2. Does the braid's dynamics decide it: periodic (P), positive entropy
   (E), zero entropy and not periodic (Z)? For E, the dilatation
   lambda?
3. Side question (KC 55.2.3.1): why do some orbits settle with period
   2?

Everything is exact: Magnus integers, exact real roots (`radices`).
Regions on B_3 are counted EXACTLY by `nervus_regiones_plani`, built
for this demo (task 0, with the census helpers in `plexus`; both had
an independent review before the demo used them). The frozen copy is
`demo-snapshot.c` (one file, compiles alone; banner gives the clang
line and commit).

Definitions (plan.md): an activation region of w^m is a maximal set of
inputs with one activation pattern (for w^m: the concatenated
per-pass patterns, the itinerary). Bias-free, so every region is a
cone; in the plane (B_3, 2 coordinates) a pattern's region is convex,
so regions = sectors = distinct patterns. A linear piece is a maximal
connected set on which w^m is one linear map; pieces count the
FUNCTION, regions count what the NETWORK carves (pieces <= regions).

## Result

1. **Regions grow linearly, never exponentially (B_3, exact).** For
   every one of the 2,589 classes up to length 8, m = 1..12, the
   registered rule gives H_lin and not H_exp, on pieces and on the
   word with the most regions. The prediction (H_lin for >= 90% of E,
   H_exp for none) HOLDS, for P and Z too.
2. **Pieces stay bounded; regions grow.** Every E class has exactly 8
   pieces for every m >= 2 (Z: 6 or 8; P: they cycle, with 1 piece at
   m = k because w^k is the identity map). Regions of E grow by 5 to 8
   per pass.
3. **The growth rate follows the word, not lambda.** The regions added
   per pass level off at 8.0 for large |tr| (lambda up to about 47)
   and are smaller for short words. Exploratory (post hoc): over
   m = 6..12 the most-regions word adds exactly 6 x its cyclically
   reduced length for 1,816 of 2,108 E classes (196 fewer, 96 more).
4. **Regions belong to the word, pieces to the braid.** Geodesic
   words of one class can differ: Delta sigma_1 as [1 2 1 1] and
   [2 2 1 2] reaches 12 regions, as [2 1 2 1] stays at 6, all with
   pieces 4 4 1 4 4 1. At m = 12, 408 E classes have words with
   different counts (largest gap 24). Pieces were equal for every
   geodesic word of every class.
5. **The nulls separate the two counts.** Networks with the same
   shape but each instruction's coefficients permuted at random also
   grow their regions linearly (H_lin 2,089 of 2,108, H_exp 0), but
   their PIECES grow too: 46 at m = 12 against the braids' 8. Linear
   region growth is a property of this network shape; bounded pieces
   are what the braid adds.
6. **The lattice method cannot see the growth.** Interior integer
   points on the shells max |x_i| <= 256 find on average 13.3 of the
   87.6 regions of E at m = 12, and no E class's full count. New
   regions are thinner by about lambda per pass. On B_4 the same floor
   stops growing after m = 4. B_4 region growth is therefore left
   open (an exact d >= 3 counter is ledger wish …QK7A9).
7. **Period 2 means the limit lies on a wall; the converse fails.** On
   B_4 up to length 6, every E orbit of period >= 2 ends on a wall
   (an argument of w vanishes at the limit direction), as continuity
   forces. But 568 E braids with period 1 end on a wall too. The
   registered prediction "period 2 iff tie" FAILS in that direction.
   The period belongs to the loop (from which side it approaches),
   the tie to the braid.

## Classes and geodesic words (Part A)

Classes on B_3 are exact. P iff beta^k is a power of Delta^2
(`plexus_periodus`). Otherwise the type is read from the trace of
reduced Burau at t = -1, the image of beta in SL(2, Z): E iff
|tr| > 2, Z iff |tr| = 2; |tr| < 2 is always periodic (B_3 / Z(B_3) =
PSL(2, Z); Murasugi 1974, cited). Checked: the per-length counts equal
D128's table, the trace law holds on every class, and Burau(-1)
spectral radius > 1 iff |tr| > 2 (`plexus_burau_radix_maxima`).

All 13,121 freely reduced words up to length 8 were keyed to their
classes (`plexus_clavis`). A word is geodesic if its length equals its
class's census length; no word was shorter (the census levels are
geodesic).

| len | classes | P | E | Z | geodesic words | per class |
|---|---|---|---|---|---|---|
| 0 | 1 | 1 | 0 | 0 | 1 | 1.0 |
| 1 | 4 | 0 | 0 | 4 | 4 | 1.0 |
| 2 | 12 | 4 | 4 | 4 | 12 | 1.0 |
| 3 | 30 | 10 | 12 | 8 | 36 | 1.2 |
| 4 | 68 | 20 | 32 | 16 | 96 | 1.4 |
| 5 | 148 | 16 | 92 | 40 | 248 | 1.7 |
| 6 | 314 | 34 | 248 | 32 | 624 | 2.0 |
| 7 | 656 | 32 | 544 | 80 | 1,548 | 2.4 |
| 8 | 1,356 | 92 | 1,176 | 88 | 3,804 | 2.8 |

Every geodesic word (6,373) was swept for m = 1..12.

## Regions and pieces of w^m on B_3 (Part A)

Means over classes; regions as the minimum and maximum over each
class's geodesic words, and for the census representative:

| m | E pieces | E r min | E r max | E r rep | Z pieces | Z r max | P pieces | P r max |
|---|---|---|---|---|---|---|---|---|
| 1 | 7.8 | 11.2 | 11.3 | 11.2 | 6.7 | 10.2 | 7.4 | 10.4 |
| 2 | 8.0 | 18.1 | 18.4 | 18.2 | 7.0 | 15.5 | 5.6 | 13.6 |
| 3 | 8.0 | 25.0 | 25.5 | 25.1 | 7.1 | 20.8 | 2.8 | 14.6 |
| 4 | 8.0 | 31.9 | 32.6 | 32.1 | 7.1 | 26.1 | 5.6 | 14.6 |
| 6 | 8.0 | 45.6 | 46.8 | 45.9 | 7.1 | 36.6 | 1.0 | 14.6 |
| 8 | 8.0 | 59.4 | 61.0 | 59.8 | 7.1 | 47.2 | 5.6 | 14.6 |
| 10 | 8.0 | 73.2 | 75.2 | 73.7 | 7.1 | 57.7 | 5.6 | 14.6 |
| 12 | 8.0 | 86.9 | 89.4 | 87.6 | 7.1 | 68.3 | 1.0 | 14.6 |

Pieces at m = 12: every E class 8; Z 6 (128 classes) or 8 (144); P 1
(all 209: their periods 1, 2, 3 divide 12).

**The registered rule** (plan.md, before running; applied twice after
the T5.0 review: to the pieces, and to the regions of the geodesic
word with the most regions at m = 12), over m = 6..12. H_lin: the
increments repeat with some period <= 3. H_exp: the ratio is >= 1.5
for every m in 8..11.

| type | classes | H_lin pieces | H_lin regions | both | H_exp pieces | H_exp regions |
|---|---|---|---|---|---|---|
| P | 209 | 209 | 209 | 209 | 0 | 0 |
| E | 2,108 | 2,108 | 2,108 | 2,108 | 0 | 0 |
| Z | 272 | 272 | 272 | 272 | 0 | 0 |

**Growth against lambda.** E classes by |tr| (lambda = (|tr| +
sqrt(tr^2 - 4)) / 2), mean regions added per pass by the most-regions
word over m = 6..12: |tr| 3: 5.3, 4: 5.4, 5: 6.4, 6: 6.4, 7: 7.1,
8: 6.7, 9: 8.0, 10: 7.1, ... 29 through 47: 8.0. Pieces add 0. The
rate does not grow with lambda: small |tr| occurs only among short
words, and the rate tops out at 8, the longest word length here.

**The rate and the word (exploratory, post hoc).** Regions added
over m = 6..12 by the most-regions word: 12, 18, 24, 30, 36, 42, 48
(4, 12, 36, 128, 336, 612, 980 classes), i.e. 6 x 2..8. Against 6 x
its cyclically reduced length: equal for 1,816 of 2,108, below for
196, above for 96. Not registered; a hypothesis for later.

**Word dependence** at m = 12 (classes whose geodesic words give
different region counts): P 74 of 209 (172 have more than one
geodesic word), largest gap 12; E 408 of 2,108 (880), gap 24; Z 88 of
272 (200), gap 25. Example, Delta sigma_1 (the class of [2 1 2 1]):

| word | r(m), m = 1..6 | pieces |
|---|---|---|
| [1 2 1 1] (census) | 7 10 12 12 12 12 | 4 4 1 4 4 1 |
| [2 1 2 1] | 6 6 6 6 6 6 | 4 4 1 4 4 1 |
| [2 2 1 2] | 7 10 12 12 12 12 | 4 4 1 4 4 1 |

**No braid network has a sector with an identically-zero argument**
(0 over all geodesic words and m), so on braids the lattice rule
"interior = no argument exactly 0" loses no sector to that cause.

## The lattice lower bound on B_3 (Part B)

The census word, the 2,720 points of the shells max(|x|, |y|) = 4,
16, 64, 256, itineraries by refinement (id_m = intern(id_{m-1},
pattern of pass m)), counting only interior points (no argument
exactly 0 in any pass so far): 79,348 of 7,042,080 point-passes ended
on walls. Mean regions at m = 12:

| type | classes | exact | R <= 4 | R <= 16 | R <= 64 | R <= 256 |
|---|---|---|---|---|---|---|
| P | 209 | 14.2 | 6.5 | 10.1 | 13.2 | 14.2 |
| E | 2,108 | 87.6 | 5.8 | 8.3 | 10.9 | 13.3 |
| Z | 272 | 66.5 | 5.8 | 9.4 | 18.7 | 33.6 |

Classes whose bound reaches the exact count at R <= 256: P 209 of
209, E 0 of 2,108, Z 16 of 272. Exactness of the method, checked on
every class and m: the bound never exceeds the exact count; every
interior point lies strictly inside one exact sector whose pattern is
its itinerary; distinct itineraries = distinct sectors hit;
homogeneity (4 p has p's itinerary).

## B_4 (Part C)

Classes = D128's table at every length (E certified at t = -1 or at a
cube root of unity; Z the rest, D128 checked their growth).

**(a) The lattice floor**, B_4 up to length 4, shells 4 and 8,
m = 1..8, mean counts: E 49.5, 76.4, 83.1, then 84.1 from m = 4 on;
P 94.8 from m = 3; Z rises to 115.3. A floor only: the shells see
nothing new after four passes, as Part B predicts for E. Its "shapes"
(H_lin-like 238 of 254 E, H_exp-like 0) say nothing about the braids.

**(b) The period-2 question**, B_4 up to length 6. As in D128 Part D,
each braid runs 60 passes on every loop E_1..E_3; a loop's period q
is the smallest whose pattern repeats over the last >= 20 passes; the
braid's q is the lcm. The counts equal D128's (pinned: E 3,630 /
384, Z 488 / 200 / 248; and in quick mode the frozen D128 snapshot's
own quick-mode table).

For each E loop whose orbit GROWS, exactly:
- mu = the largest real root of the minimal polynomial of the settled
  tail, sampled every Q passes (Q = the braid's period; Krylov
  kernel). Checked: the polynomial holds on the whole tail, and mu is
  > 1 and simple. That mu dominates the complex roots is assumed
  (north-south dynamics), not proved.
- An argument of w VANISHES at the limit iff mu is not a root of the
  minimal polynomial of its own values along the tail (Hankel kernel;
  `radix_signum_polynomii`). A tie = some argument vanishes.

| E braids | q = 1, no tie | q = 1, tie | q = 2, tie | q = 2, no tie |
|---|---|---|---|---|
| Burau(-1) radius > 1 (3,006) | 2,474 | 476 | 56 | 0 |
| only at a cube root (1,008) | 588 | 92 | 328 | 0 |

- **Period >= 2 forces a tie.** If two positions of the cycle have
  different bits for ReLU k, its argument is > 0 on one subsequence
  and <= 0 on the other, both converging to the limit direction v, so
  it is 0 at v. Checked per argument: every bit that varies on a
  settled tail has a vanishing argument. Vanishing is also the same at
  every position of the cycle (one limit direction).
- **The converse fails:** 568 period-1 braids also end on a wall,
  approached from one side. The registered prediction "period 2 iff
  tie" FAILS. Within a braid, loops can differ in period and agree on
  the limit: in the quick run, [1 2 -3 2] has loop E_1 at q = 1 (mu =
  phi^2 per pass) and E_2, E_3 at q = 2 (phi^4 per two passes). All
  growing loops of every E braid reach the same mu and the same tie
  (checked).
- **52 E braids have an invariant loop** (all certified only at a cube
  root, lengths 5-6): that loop's tail polynomial is t - 1, it does
  not grow. These braids have positive entropy and an invariant curve:
  reducible, with a pseudo-Anosov piece (D128's fact-check: E is not
  pA). The loop never reaches the attractor and is left out; the
  first full run caught them by failing "same limit".
- Why period 2 concentrates among the cube-root-only braids (KC
  55.2.3.1) is still open: a tie is necessary, and 92 of their 420
  ties settle with period 1.

## Nulls on B_3 (Part D)

- **(a) A linear network** (Burau(-1) of the word as two affine
  instructions) has 1 region and 1 piece, on every class: the floor.
- **(b) Coefficients permuted** within each instruction of the census
  word's network (same sources, terms, depth, ReLU count; 2,584 of
  2,589 networks changed), m = 1..12, means at m = 12 (none passed
  the 20,000-region stop):

  | original type | classes | r(1) | r(6) | r(12) | p(12) | H_lin | H_exp |
  |---|---|---|---|---|---|---|---|
  | P | 209 | 10.1 | 38.9 | 72.7 | 40.2 | 203 | 0 |
  | E | 2,108 | 10.8 | 42.2 | 79.2 | 46.3 | 2,089 | 0 |
  | Z | 272 | 9.8 | 36.1 | 67.3 | 36.7 | 270 | 0 |

- **(c) Letters shuffled** (a different braid; its type looked up in
  the census and re-derived from its own period and trace): E words
  became P 170, E 1,720, Z 218. By the NEW type: P regions flat at
  12.7, E 43.1 at m = 6 and 82.1 at 12, Z 29.8 and 55.0; H_lin for
  all, H_exp for none.

## Interpretation [interpretation]

- A hypothesis for the bounded pieces: on B_3 the braid may act, in
  suitable coordinates related to the Dynnikov ones by a fixed
  piecewise-linear map h, as its matrix A in SL(2, Z) up to sign. Then
  w^m = h^-1 (+-A^m) h, and the number of pieces cannot grow with m:
  the function stays simple however deep the network. Pieces 8 for
  every E class is consistent with that; neither the coordinates nor
  the bound is established here.
- The network does not know that. Each pass cuts its own walls: a
  wall of pass j is the preimage of a fixed wall under w^(j-1), so the
  walls accumulate toward the repelling direction, a few new ones per
  pass (north-south dynamics). That gives linear growth, and thinner
  sectors by about lambda per pass, which is exactly what the lattice
  cannot see. The number of new walls per pass depends on the word's
  ReLUs, which is why the rate tracks the word and not lambda.
- So D106's question "does chaos help computing?" gets a clean answer
  for capacity on B_3: no. Positive entropy does not buy exponential
  region growth; the regions a braid network carves grow linearly
  whatever the braid, as they do for random networks of the same
  shape. What distinguishes braids is the opposite: their functions
  stay bounded in complexity. Capacity is not computation (KC 31.2 /
  N2).
- The period-2 orbits are orbits converging to a limit on a wall from
  both sides. A tie at the limit is a property of the braid (the
  limit direction is unique); whether a given loop alternates is a
  property of the loop.

## Checks

Full mode 34 (quick 34), all exact:
- Part A (14): classes = D128's table; the trace law; radius > 1 iff
  |tr| > 2; every reduced word finds its class; census levels
  geodesic; each representative among its geodesic words; regions
  monotone in m; every sector of the representative's w^m at its
  medius (pattern, nullae, M x) and against the Dynnikov action m
  times (`ansa_agere`, not nervus); pieces a class invariant;
  periodic r(m) = r(k); periodic w^k = identity, p(m + k) = p(m); the
  rule's code on hand sequences (2).
- Part B (5): bound <= exact; interior points in their sector;
  distinct itineraries = sectors hit; shell union monotone;
  homogeneity on every inner-shell point.
- Part C (11): B_4 classes; lattice refinement and union; lattice
  homogeneity; orbits settle; periods = D128 (both modes); the tail
  polynomial and mu; period >= 2 => tie; per argument: varying bit =>
  vanishing argument; vanishing independent of position; same limit
  for growing loops; every E braid has a growing loop.
- Part D (4): linear floor; permuted shape; permutation changes most
  networks; shuffled-word type re-derived.

## Plants

All through `silva.planta`, all red:
- T5a (10): E threshold |tr| >= 2, reduced-word filter, w^2 not
  composed, pieces = regions, H_lin with no period, H_exp strict at
  1.5, wall points counted interior, itineraries not refined, the
  angular walk reversed, homogeneity pairs never found.
- T5b (10): B_4 E without the cube-root certificate, the tie test
  inverted, mu = the smallest root, period never 1, Krylov ignoring
  the period, B_4 homogeneity pairs, null (b) without a permutation,
  null (c) without the lookup, and (full mode) invariant loops treated
  as growing.
- Checks were added where a plant would otherwise have passed: the
  rule's code, "itineraries = sectors hit", the homogeneity pair
  counts (the B_4 one was vacuous in quick mode with one shell), the
  per-argument theorem (an inverted tie test still passed "q >= 2 has
  a tie"), quick-mode period pins, the two null assertions.

## Not covered

- Exact region counts on B_4 and beyond (d >= 3 needs exact linear
  programming; ledger …QK7A9). Part C (a) is a floor only.
- Why period 2 concentrates among braids certified only at a cube
  root (KC 55.2.3.1).
- Proof that the pieces stay bounded on B_3 (the interpretation
  above), and the rate-equals-cyclic-length hypothesis.
- Computing with the network (N2, ledger …4291V2).

## Run

```text
./bin/aedilis knotapel/demo_129_braid_regions/main.c &&
  bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
```

Full mode about 4 min (34 checks); `D129_CELER=1` (B_3 up to 5, B_4
lattice up to 3 on shells 2 and 4, period question up to 4) about
7 s (34 checks). The snapshot gives byte-identical output (237
lines).
