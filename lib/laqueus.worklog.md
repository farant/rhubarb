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
