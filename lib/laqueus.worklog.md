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
