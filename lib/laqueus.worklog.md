# laqueus.worklog.md

## 2026-10-06 — natus

`laqueus` (Latin: noose, snare — a tied loop): exact
polygonal knots and links in Z^3 over `situs`, the first consumer of the
whole math stack (magnus → fractio → polynomium → anulus → matrix →
situs). API approved by Fran: house tier, links (several components)
from the start, a situs variant returning crossing parameters, v1
invariants writhe / linking number / Kauffman bracket / Jones /
Alexander, triangle moves as checked operations. The knotapel demos
(110–112) are the eventual consumer; knotapel itself is not touched.

Conventions (the ones a reader must know to read the code):
- Segment i runs from vertex i to `_sequens(i)` inside its component
  (`initia` has componentes + 1 entries; each component ≥ 3 vertices).
- Diagram: every segment pair is examined. Contiguous pairs go through
  `situs_transitus_vicinus` (shared endpoint is not a crossing), all
  others through `situs_transitus_parametri`. TANGUNT anywhere = the
  projection is not generic (FALSUM). Two crossings at the SAME
  parameter on one segment = a triple point = FALSUM (the per-segment
  events are sorted with `fractio_compara`; equality is the test).
- `percursus` (the Gauss-code walk): 2k = crossing k passed over,
  2k + 1 = passed under; `initia_percursus` splits it by component.
- PD codes: positive [u_in, o_out, u_out, o_in], negative
  [u_in, o_in, u_out, o_out] — chosen so the A smoothing is always (X0 X1)(X2 X3) and B is (X0 X3)(X1 X2).
  PD array is called `ora` (X is a Roman-numeral macro).
- Bracket = Σ A^(#A − #B) d^(loops − 1), d = −A² − A⁻², by union-find
  over the 2c edge labels for each of the 2^c states; state counts are
  tallied in an s64 table indexed by (#A − #B, loops) and the polynomial
  is built once at the end. Refuses above 24 crossings
  (LAQUEUS_TRANSITUS_MAXIMI: the state sum is 2^c; past that a
  different algorithm is owed, not a bigger cap — timing at 24 not
  measured yet).
- Jones = (−A³)^(−w) · bracket with A = t^(−1/4) (`polynomium_contrahe`
  by −4); FALSUM when an exponent is not divisible by 4 (links with an
  even number of components give half-integer powers of t — v1 refuses
  rather than inventing a √t type).
- Alexander via Fox calculus on the Wirtinger presentation: one row per
  crossing, columns = arcs. Positive: (1 − t, t, −1); negative
  (1 − t⁻¹, t⁻¹, −1) at (over arc, incoming under arc, outgoing under
  arc). Delete last row and column, `matrix_determinans` over
  ANULUS_POLYNOMIORUM, normalize (lowest term t^0, leading coefficient
  positive). Arcs are numbered starting after the first under passage
  so arc 0 is a whole arc. Knots only in v1 (multivariable Alexander
  for links is a different object).
- `laqueus_diagramma_genericum` tries (0,0,1), (1,2,3), (2,3,5), …
  — a fixed list, so results are deterministic.
- Moves: addere replaces segment (a, b) by (a, c), (c, b); legal iff the
  triangle abc meets nothing but its own edge ab — non-contiguous
  segments via `situs_triangulum_segmentum`, the two contiguous ones via
  `situs_triangulum_vicinum` (prior segment `vicinum(a, b, c, p)`, next
  `vicinum(b, c, a, q)` — the shared vertex goes first). removere is the
  reverse on vertex i (component needs ≥ 4 vertices). A degenerate
  triangle (c on line ab) is refused by addere; removing a collinear
  in-between vertex is always legal (the triangle is a segment already
  inside the polygon).

Verification — the oracle is the knot TABLE, not a second program:
- Five polygons from classical parametrizations (scratchpad nodi_c.py:
  floats in the generator only, rounded to integers ×100): trefoil 48
  vertices, figure-eight 72, cinquefoil 80, T(2,7) 112, T(3,4) 120. Each
  must produce the published writhe-independent invariants (Jones and
  Alexander from Rolfsen/KnotInfo). Rounding could in principle change
  the knot type; agreeing on BOTH Jones and Alexander is the evidence it
  did not.
- Symmetries with known effect: mirror (z → −z) dilates Jones by −1
  and keeps Alexander; writhe flips only when the projection direction
  is mirrored too ((a, b, c) → (a, b, −c)) — my first test asserted −w
  along the same direction and was WRONG, writhe depends on the
  projection; inversion keeps both; an exact rational rotation
  (quaternion (1,2,3,4)/30) plus translation keeps both (first 2 knots
  only, to keep the suite under a second).
- Links: Hopf bracket −A⁴ − A⁻⁴, linking ±1, Jones/Alexander FALSUM
  (by design); split unlink bracket −A² − A⁻².
- Random walks: two sequences of 200 attempted moves (offsets ±400 and
  ±60) on the trefoil; after every accepted move Alexander must be
  unchanged, Jones too while ≤ 24 crossings. This is the test that
  exercises the moves' legality checks against the invariants rather
  than against hand cases.
- 74 checks, 0.8 s; venenum sana (poisoned piscina + ASan/UBSan).

Plants (all compile):
- Red at once: addere without triangle–segment check (Pa), removere
  without it (Pc), PD sign convention swapped (Pd), Alexander negative
  rows as positive (Pf), events not sorted (Pg).
- Survived the first suite, then got tests: addere without the prior /
  next contiguous check (Pb, Pb2 — square + triangle overlapping a
  neighbour, both sides), triple point not refused (Pj — three segments
  through one projected point at heights 0, 5, 10), genericity and
  simplicity with the contiguous branches disabled (Ph, Pi — collinear
  triangle, triangle projecting to a line). Re-checked on 10-06 with ONE
  branch disabled at a time (all four: simplex i→j, j→i, diagramma i→j,
  j→i): all red. They fail cleanly first (e.g. `genericum(hopf)` at
  probatio line 468: the shared endpoint falls into the general
  predicate and reads as TANGUNT) and then crash (139 / 137) because
  the test continues on an unfilled Diagramma — loud, not silent.
- Equivalent, accepted: Pe (A and B smoothings swapped) — with the
  state loop over all 2^c bitmasks, swapping which bit means A only
  relabels states; the sum is identical.

Not done in v1 (deliberately): links' Jones with half-integer exponents,
multivariable Alexander, bracket beyond 24 crossings (needs a
tangle/planar-algebra algorithm), Reidemeister moves on diagrams (we move
the polygon instead), HOMFLY.

## 2026-10-06 — review I (recensor-mathematicus) and fixes

Verdict: no correctness bugs. The reviewer's independent exact oracle
(own geometry, PD, Fox code) agreed on ~13k random knots/links (box ±2
lattice included: collinear points, vertex-on-segment, overlaps, triple
points), 13.6k individual crossings (supra, infra, s, t, sign) and 22k
move proposals. The sign convention is the PHYSICAL one: the suite's
Hopf link has lk = +1 by the right-hand rule and the library says +1, so
the trefoil table entry really is the right-handed trefoil.

Fixed (verified by my own probe before touching code):
- A. `situs_transitus_parametri` never rolled back, so a diagram kept
  every pair's arithmetic, O(n^2), in the caller's piscina. Integer
  coordinates allocate nothing (fractio small path) and hid it. My probe,
  13-gon with +1/(10^6 + ...) coordinates: 1.45 MB → 234 KB; the
  reviewer's 120-vertex T(3,4): 121 MB → 0.34 MB. Fix: roll back unless
  SECANT (outputs are only written then). Test: the trefoil scaled by
  (10^9 + 7)/(10^6 + 3) must stay under 256 KB — 20.8 KB fixed, 2.35 MB
  with the plant.
- B. `genericum`'s fixed list of 12 directions can be exhausted by a
  simple polygon (13 vertices, one segment parallel to each direction;
  (1, 4, 16) is generic). Fallback: simplicity check, then the moment
  curve (1, k, k^2). Every non-generic condition puts v on a line, a
  plane or a quadric cone; the curve meets a plane ≤ 2 times and a cone
  ≤ 4, so at most n + 2n + 2n^2 + 4·C(n, 3) values of k fail. The loop is
  BOUNDED by that count + 1 (and k^2 < 2^63). Lesson from the plants:
  my first version looped to k ≈ 3·10^9 — the M32 plant (contiguous
  check disabled, so nothing is ever generic) hung the suite for 10
  minutes until killed. An unbounded "provably terminates" loop turns any
  upstream bug into a hang; the bound turns it into a FALSUM.
  A non-simple polygon is never generic (the 3D intersection projects to
  a touching point in every direction) — hence the simplicity check first.
- Jones refuses even-component links BEFORE the 2^c bracket (Jones of a
  μ-component link lies in t^((μ−1)/2) Z[t, t^-1]); the contrahe check
  stays as the backstop. Saves ~7 s at 24 crossings.

Corrections to the natus entry:
- Arc numbering: starting after the first under-passage is NOT needed
  (reviewer's M06, removing the skip, is equivalent: the wrap gives the
  arc containing position 0 index 0 again). Harmless; my reason was wrong.
- Timing, now measured by the reviewer at -O2: one bracket ≈ 3.5 s at 23
  crossings, ≈ 7 s at 24; it doubles per crossing.
- Pe was right to call equivalent, but it is a different plant from the
  reviewer's M11 (real A/B swap keeping the exponent), which is killed.

New tests (each confirmed red under its plant, run sequentially — a
first batch raced my own restore `cp` and showed M26 falsely green):
- M30 (supra/infra swapped everywhere, sign kept): an EXACT cancelling
  pair — reflecting the plane and switching every crossing is a rotation
  by π about an in-plane axis, so every invariant survives. Now the two
  Hopf crossings along (1, 2, 3) are pinned by hand computation: segment
  5 over 1 at (2/3, 1/6), segment 1 over 7 at (5/6, 2/3), both +1.
- Hopf lk asserted = +1 (was ±1).
- M32 / M37 (one contiguous branch skipped): the collinear and the
  projecting-to-a-line triangles in all 6 vertex orders (one order left
  a branch untested).
- M43 (`_collinearia` without the yz plane): addere in the x = 0 plane.
- M26 (removere's next-segment check with the wrong shared vertex):
  removal of (10,10,0) from (0,0,0)-(10,10,0)-(20,0,0)-(30,5,0)-...;
  the next segment is in the triangle's plane but outside the angle at c,
  while (30,5) − a IS inside the angle at a.
- M39 (failed directions not rolled back): genericum on the scaled
  13-gon must cost ≤ 1.5 × the winning direction alone (1706 vs 1704
  bytes; 29 KB with the plant).
- B: the 13-gon is found generic (Alexander 1); a non-simple link gives
  FALSUM and terminates.
- 25 crossings refused (a comb: zigzag + one return line, 28 vertices,
  unknot, Alexander 1). 24 ITSELF stays untested (M42, `>=` for `>`):
  one bracket there is ~7 s at -O2. Recorded gap.
124 checks, 0.8 s; situs 16528; venenum sana.

Not pinned, by choice: parameter VALUES beyond the Hopf case (only their
order feeds the Gauss code), and `numerus_ligationis(d, a, a)` (header
says a != b; returns half the self-crossing sum).

## 2026-10-07 — four functions folded in from Demo 114

Demo 114 (knotapel, exact audit of D110–D112) needed general knot tools
that do not belong in a demo (knotapel only consumes). API approved by Fran
before implementation:

- `laqueus_simplificare`: greedy simplification by LEGAL triangle moves
  only — repeat {first vertex in index order whose removal
  `laqueus_motus_removere` accepts} until none. An ambient isotopy, so the
  knot/link type is unchanged; FALSUM on non-simple input (the move check
  presumes an embedding — D114 showed D112's det-greedy deletions breaking
  the embedding on all 12 paths). Memory O(n): one working vertex buffer,
  `removere` used only as a legality oracle and rolled back at once
  (measured ~150 B per input vertex; a per-move-copy version would be
  O(n²)). Results on the table polygons: 48→7, 72→8, 80→8, 112→11, 120→8;
  each ≥ the published stick number (6, 7, 8, 9, 8) — a bound the code
  knows nothing about, asserted.
- `laqueus_diagramma_minimum(l, radius)`: the generic diagram with the
  fewest crossings over integer directions with |components| ≤ radius (one
  of each ±v, ties → first in enumeration). Radius 2 reaches the crossing
  number for 3₁, 4₁, 5₁ and T(3,4) (asserted), not for T(2,7) (8, not 7).
  Cost ~ (2r+1)³/2 diagrams. Makes Jones affordable and enables unknot
  certificates (Δ = 1 on ≤ 10 crossings).
- `laqueus_speculum`: z → −z. Jones(mirror) = Jones(1/t), Alexander
  unchanged, Hopf linking number +1 → −1 (asserted).
- `diagramma_determinans`: |Δ(−1)| as Magnus; table values 3, 5, 5, 7, 3
  asserted; FALSUM for links (as `diagramma_alexander`). Leaves its
  scratch in the caller's piscina like `diagramma_alexander`: copying a
  rolled-back result would read freed memory (VENENUM would catch it).

Plants (all compile, all red): simplificare without the simple-input
check, with moves unchecked, with a single pass, with `initia[m] >= k`;
minimum → maximum; minimum without negative directions; speculum as a
rotation (x and z negated); determinant evaluated at +1.

## 2026-10-07 — review of the Demo 114 round (recensor, libround-I)

No bugs: the new functions match the reviewer's independent oracle —
simplificare byte-identical on 4,500 random knots and links (11,542
removals, 1,926 at the first or last index of a non-first component),
minimum + determinans (2,600 cases, same chosen direction), reflexio
(4,000 cases incl. collinear triples, denominators ~10^12); all suites
clean under ASan/UBSan/VENENUM. Stick numbers confirmed (7₁ ≥ 9 from
Calvo's 8-stick classification; T(3,4) = 8₁₉ = 8 by Jin's formula); radius
3 reaches T(2,7)'s 7 along (0,1,−3).

Test gaps closed (the plants named survived the first suite):
- L1 link boundaries corrupted on removal: the suite never simplified a
  link that loses vertices. Now Hopf with a midpoint on every edge (16
  vertices) → two triangles (3 + 3), simple, lk +1. My first expectation
  ("back to the original Hopf") was wrong: with midpoints the corner
  triangles shrink and no longer touch the other component.
- L2 (no `frange`, not first-index greedy): exact counts 7/8/8/11/8.
- L3 (ties → last direction): trefoil's radius-2 direction pinned to
  (0, 1, −2).
- Radius: `i32` is unsigned, so a "negative" radius was ~4.3e9 and hung;
  now capped at LAQUEUS_RADIUS_MAXIMUS = 32 (FALSUM above). The test runs
  under CREDO_NON_PENDET so a lost cap fails instead of hanging the suite
  (the unguarded plant hung the runner; its watchdog killed the runner but
  not the test binary, and the plant had to be restored by hand).
- Header: "genericum then applies" overpromised (it fails on non-simple
  input too); reworded.

Known gap, accepted: L4 (no `abs` in determinans) survives — the abs is
required (e.g. Δ = t⁴ + t³ − 3t² + t + 1 gives −3 at −1) but no fixture
knot has a negative normalized Δ(−1).

## 2026-10-07 — PD codes (for the knot table, tabula_nodorum)

The knot-identification library takes PD codes from KnotInfo (pinned
snapshot `soehms/database_knotinfo` 2026.10.5) and computes Alexander and
Jones itself, so laqueus needed invariants from a PD code.

- Refactor, no behaviour change: the bracket's state sum is
  `_uncinus_ex_oris` (any PD array, 0-based labels), Jones' normalisation
  `_jones_ex_uncino`, the Fox matrix `_alexander_ex_arcubus`; the diagram
  path builds its PD array (`_ora_diagrammatis`) and arcs as before and
  calls them. 268 existing checks unchanged.
- New: `diagramma_pd` (knots; KnotTheory convention, labels 1..2c along the
  knot, each crossing [incoming under, then counterclockwise] — laqueus's
  internal convention already was this), `laqueus_uncinus_ex_pd`,
  `laqueus_jones_ex_pd`, `laqueus_alexander_ex_pd`. PD validation: labels
  1..2c each exactly twice, under-edges consecutive, sign from the over
  edges (positive iff X1 = X3 + 1 mod 2c). One crossing is refused: with
  two labels the sign is ambiguous. Alexander from PD finds arcs by
  union-find over over-edges — a different route from the diagram's walk,
  so the round trip is a real cross-check.
- External oracle: KnotInfo's own Jones and Alexander for 3_1, 4_1, 5_1,
  8_20, 10_132 come out exactly from their PD codes — including the chiral
  ones, so KnotInfo's chirality convention equals laqueus's physical one
  (KnotInfo's 3_1 is the right-handed trefoil, −t⁴ + t³ + t; Knot Atlas
  draws the left-handed one: the tables differ per knot, which is why the
  table will compute from PD rather than copy polynomials).
- Round trip on the 5 polygon knots: diagram → diagramma_pd → *_ex_pd ==
  diagram invariants.
- Plants: sign rule inverted, arcs through under-edges, PD export without
  +1, no under-consecutive check — red. "Label used thrice" first SURVIVED:
  the invalid PD made Jones fail incidentally (half-integer exponents), so
  the refusal test via Jones never reached validation; refusals are now
  tested via bracket and Alexander, red. In/out under-arcs swapped at every
  crossing: equivalent (orientation reversal leaves Alexander unchanged).

## 2026-10-07 - laqueus_pd_simplificare (Reidemeister I/II on PD codes) + PD validation hardened

For demo 116: 7_2's honest polygon has 1,432 alternatives whose best
projections have 21-30 crossings - past where the state sum is cheap.
Greedy R1/R2 on the PD code shrinks those diagrams, and c' == 0 is a direct
unknot proof. Fran approved the API (one function, PD in -> PD out).

**Validation gap found and closed.** `_pd_legere` checked "each label twice,
X2 = X0 + 1, sign consistent" but NOT "each label once entering, once
leaving". `[1,1,2,2, 3,3,4,4]` (two disjoint one-crossing loops) passed. The
in/out check is what makes the labeling l -> l+1 run through every label,
i.e. what guarantees ONE component - the simplifier's correctness argument
needs it. Test `pd_orientatio_falsa`; plant S4 red.

**Algorithm.** Work on 0-based labels; after every move, union-find merges the
cut edges and `_pd_renumerare` re-walks the knot (in-slot X0 -> out X2; over
in X3/out X1 positive, X1/X3 negative) to relabel 0..2c'-1; signs carried.
The walk itself re-checks in/out uniqueness and that it visits 2c' labels.
- R1: a label at two slots of one crossing. In a valid code those slots are
  always ADJACENT (opposite slots would need l = l + 1), so it is a monogon.
- R2: crossings j != k sharing edges e (slot a at j, a1 at k) and f (slot
  a+1 at j), same parity of e's slot at j and k (same strand over both).
  Different parity = clasp, never removed (the trefoil's lobes are clasps;
  plant S1 removing the parity test reds 14 laqueus + the 250-table check).

**The face question (I got this wrong first).** I first required e, f to bound
a bigon FACE (faces from the rotation system: next(d) = slot after the other
end of d's edge), worried that a non-face bigon with sub-tangles on both sides
would make the removal invalid. Built test diagrams with a Python helper:
two-crossing core (same strand over both), trefoils tied into both kink loops.
- opposite crossing signs -> e,f bound a face (classic R2);
- equal signs -> NOT a face (one trefoil on each side).
Both diagrams are 3_1 # 3_1 (any two-crossing core is the unknot; tying knots
into its loops is a connected sum), and NAIVE removal of the non-face pair
gives a valid PLANAR 6-crossing granny code (Euler V - E + F = 2 checked).
General argument for a knot: e, f always sit on adjacent slots (else one
strand closes on itself = second component); then either the far side is
empty (face) or a 1-1 tangle hangs off each crossing and the knot is T1 # T2
before and after. So the face condition was merely conservative - and a plant
removing it could never go red (dead code). Dropped it; test `fictum` now
must reduce 8 -> 6 with the granny Jones J(3_1)^2. The clasp version
(`fibula`, also 3_1 # 3_1) must stay untouched - greedy R1/R2 is stuck
there, which is honest (no R3).
Construction of the test codes: core crossings j = [4,2,1,1] (positive),
k = [3,3,4,2] / [3,2,4,3] (positive / negative), clasp k = [2,4,3,3];
loops g1 = label 1 (at j), g2 = label 3 (at k); trefoil KnotInfo PD spliced
into g2 then g1 (cut the trefoil at its last edge, rewire, renumber by walk).

**Tests (343 laqueus, 86 tabula_nodorum):** refusals (orientation, bad
three-crossing code, one-crossing codes failing exactly one of X0/X2, X1/X3);
kinks (1 and 2 kinks -> 0; trefoil + positive/negative kink -> EXACT KnotInfo
code); trefoil unchanged; R2 face and non-face -> 6 with granny Jones and
Alexander; clasp unchanged; polygon projections in 4 directions for 5 knots:
invariants preserved, idempotent, and >= 4 diagrams actually reduced (16 -> 8,
11 -> 4, ...); all 250 table PDs are exact fixed points (a minimal diagram
has no R1/R2).
**Plants (all red):** S1 parity dropped, S2 R2 strands crossed, S3 R1 on
opposite slots, S4 in/out validation dropped, S5 only one move, S6a/S6b
one-crossing checks dropped singly (first test code failed BOTH checks, so a
single-check plant survived - split into two codes), S7 R2 second edge not
adjacent.
