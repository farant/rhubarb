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

## 2026-10-07 - review of 81aa0eef: planarity check, mirrored non-face test

The reviewer found no bugs and checked the simplifier against its own PD
toolkit: about 4,000 random planar diagrams up to 55 crossings, plus 1,000
composites through face, non-face and clasp cores. It confirmed that R1 is
always adjacent and that the R2-without-face argument holds for PLANAR codes.
Two findings, both verified here and fixed:
- **Non-planar codes were accepted.** `[[1,3,2,4],[2,5,3,6],[4,1,5,6]]` is
  valid by every earlier check (labels, orientation, one component) but its
  rotation system has 1 face, where planarity needs V - E + 2 = 5. It is a
  virtual knot diagram. The simplifier "proved" some such codes unknots, and
  both the non-face R2 argument and "c' == 0 = unknot" need planarity.
  `_pd_legere` now ends with `_pd_planus`: count the face orbits of
  next(d) = slot after the other end of d's edge, require F == c + 2.
  Every caller (uncinus, Jones, Alexander, simplification) refuses non-planar
  codes. All 250 table PDs pass; both valid one-crossing codes are planar
  (F = 3), so the c == 1 shortcut is safe.
- **Plant Q2 survived:** "R2 only when e's slot at j is even". The test
  fictum had its core edges at slots (0,1) only. Its mirror (plane
  reflection [a,b,c,d] -> [a,d,c,b]) puts them at (3,0). New test
  `pd_fictum_speculum`: reduces 8 -> 6 with Jones J(3_1)(1/t)^2. Q2 now red.
Plants: E1 (planarity check skipped) red. E2 (faces traced with the slot
BEFORE instead of after) survives, and that is correct: sigma^-1 alpha traces
the faces of the mirrored rotation system, which has the same count. It is
an equivalent implementation, not a bug.

## 2026-10-27 — Kauffman bracket by vertex model and contraction (contractio-plan T4)

New: `laqueus_uncinus_ex_pd_rete`, `laqueus_jones_ex_pd_rete`, and under
them an opaque reusable `LaqueusModulusVertex` (`_creare`,
`_aestimare(omega, p)`, `_gyrationes`, `_sumptus`). No crossing limit
except the officina's byte ceiling; the state sum stays the oracle up to
LAQUEUS_TRANSITUS_MAXIMI (24).

**Model** (project-specs/contractio-plan.md, "The vertex model" and
"Why it is correct"): one dimension-2 index per PD edge, spin +1 = the
loop runs from the edge's first occurrence in the code (end a) to its
second (end b). Turning numbers t (quarter turns, a → b) come from the
face equations — face 0 = orbit of position 0, target +4; every other
face −4; each corner −1 — solved by a BFS spanning tree of the dual
graph, leaves first, non-tree edges m = 0. Crossing tensor = A·[(X0
X1)(X2 X3)] + A⁻¹·[(X0 X3)(X1 X2)]; a pair is nonzero only if one end
enters and the other leaves, with ω⁻¹ for arm p → p+1, else ω⁺¹; the
edge factor ω^(σt) sits at end b. Contraction = d·⟨D⟩ with A = i·ω²,
d = ω⁴ + ω⁻⁴ = −A² − A⁻².

**Kinks (R1 loops):** an edge with both ends on one crossing is not an
index; its spin is summed inside that crossing's tensor at build time.
Each crossing's tensor is stored symbolically as ≤ 32 terms (entry,
A^±1, ω exponent), so a point only fills numbers in: no allocation per
point (ω powers in a table of size 2E + 1, E = max |exponent| ≥ 4).

**Points:** ω = index + 2 (the interpolatio aestimator), A = i·ω² with
i = g^((p−1)/4), g the least quadratic non-residue, cached per prime.
INUTILE at ω ≡ 0 and at d = 0 (A⁴ = −1); ERRATUM for p < 5, p ≢ 1
(mod 4), or i² ≠ −1 (p not prime). Duplicate y = ω⁸ among small ω are
rare and the driver skips them.

**Interpolation:** window [−c − 2|s_B| + 2, c + 2|s_A| − 2] rounded
inward to the class 3w mod 4 (|s_A|, |s_B| = loops of the all-A / all-B
states by union-find), passus 4, limes 2c bits (|coefficient| < 4^c),
primes ≡ 1 mod 4, two extra points per prime. Jones from the bracket
with the PD writhe and the existing `_jones_ex_uncino`.

**Tests** (probationes/probatio_laqueus_rete.c, 9 sections, 55,960
checks, 2.0 s): hand codes — both 2-crossing kinked unknots and the
stabilized trefoil with its negative twin — against the state sum and
the table; all 249 table knots of 3–10 crossings, bracket = state sum
and every state-sum exponent ≡ 3w (mod 4); all 12,966 table knots'
Jones; 200 table knots (2–9 crossings) with 1–3 random R1 kinks (4
kinds), Jones = table, bracket = state sum, `laqueus_pd_simplificare`
back to ≤ c; four 13-crossing knots with 15 kinks (28 crossings): the
state sum refuses, the network gives the table's Jones; random polygonal
knots (8–11 vertices in [−12, 12]³, 2–16 crossings, ≥ 50 tested) against
`diagramma_jones`; every loop of 20 random states on 100 kinked table
knots turns ±4 (computed from the PD and t alone — Umlaufsatz, not the
solver's equations); single points equal the state sum evaluated at A
mod p, a prime switch recomputes i, p = 17 with ω = 5 (A = 2) is
INUTILE; refusals (c = 1, non-planar, bad labels, officina too small)
leave exitus untouched; c = 0 → 1.

R1 insertion (test helper): splitting edge e, labels > e shift by 2,
the incoming occurrence of e becomes e + 2, and the new crossing is one
of [e, e+2, e+1, e+1] (under first, positive), [e, e+1, e+1, e+2]
(under, negative), [e+1, e+1, e+2, e] (over, positive), [e+1, e, e+2,
e+1] (over, negative). All four are planar and accepted by
`_pd_legere`.

**Deviation from the plan's credo:** random R2 insertions were not
written (a PD-level R2 needs the face structure to stay planar). Bigons
are covered by the table diagrams (twist regions) and by the polygonal
knots, whose generic projections are full of bigons and monogons.

**Plants (radix/laqueus_rete), 17/17 red:** outer-face target sign;
corner term dropped; basis without +2; tree solve sign; enter/leave at
the wrong end; crossing turn sign; edge factor sign; kink not folded;
imus off the 3w class; summus window too narrow; limes 2 bits; no
division by d; A⁻¹ sign; d = 0 not INUTILE; i not recomputed on a prime
change; class w instead of 3w; Jones writhe sign. Most are caught inside
the driver (the extra points and extra prime refuse the inconsistent
interpolant); the A⁻¹ and writhe plants by the oracle comparisons.

## 2026-10-27 — T4 review (independent, after 62a02ef5) and fixes

Reviewer, all in scratch under ASan/UBSan: every one of the 65,536
label assignments at c = 2 (64 valid, including face 0 as a monogon,
[[2,1,3,2],[4,3,1,4]], t = 5 there); 1.2M biased random codes at c =
3–5 and 200k garbage codes — the network path accepts and refuses
exactly what the state sum does, with identical bracket and Jones;
10,000 relabeled / permuted / mirrored / reversed / kinked table knots;
2,000 connected sums of 2–5 kinked, scrambled knots (to ~70 crossings);
5,000 nugatory crossings; connected sums with Jones = product at c =
36, 65, 207, 404, 803 and 1606 (122-bit coefficients at c = 207). A
negative control (mirror without t → 1/t) failed 153 times, so the
harness bites. The loops-turn-±4 check alone catches a solver plant (m
= 0): 4,084 of 8,334 checks fail. No H.

Fixed here:
- **M1** `laqueus_modulus_vertex_aestimare` promised ERRATUM for a
  non-prime p but did not deliver: a composite ≡ 1 (mod 4) divisible by
  3 has no g with g^((p−1)/2) = −1, so the search ran ~p modpows (2^32 −
  7 never returned); p = 85 was accepted (g = 13), and with gcd(ω, p) >
  1 the ignored `congruentia_inversa` left w⁻¹ uninitialized; the i² =
  −1 check was dead (true by construction). Now p is tested with
  `congruentia_est_primus` (deterministic below 2^32) before the
  search; ω without an inverse is INUTILE; the dead check is gone. The
  search keeps its g < p bound as a guard. That bound matters: my first
  version dropped it, and the plant that bypasses the primality test
  then looped forever in the parent instead of failing. Only public-API
  misuse reaches any of this — interpolatio passes primes.
- **M2** nothing above 24 crossings had structure beyond R1 kinks
  (which only multiply the bracket by −A^±3: coefficients stayed those
  of ≤ 13-crossing knots, one prime's worth). New section "summae
  connexae": 30 sums of 2–5 table knots plus one of 12 knots of 9–13
  crossings (c ≥ 100), each piece mirrored at random, one kink, labels
  rotated and crossings permuted; Jones = the product of the pieces'
  Jones (t → 1/t for mirrors). The big sum must have a coefficient
  wider than 31 bits, so CRT across primes is tested end to end.
  Builder: K1's incoming occurrence of m1 → m1 + m2, K2's incoming
  occurrence of m2 → m1, other K2 labels + m1.
- **L2** coverage: mirror images (in the sums), face 0 as a monogon
  (hand code u3), kinks at crossing 0 (labels rotated and crossings
  permuted in the R1 section and the sums), composite p (85 →
  ERRATUM; 2^32 − 7 returns, under CREDO_NON_PENDET). Plant R18
  (primality test bypassed) red.

Carried to T5 (plan): **M3** contractions per bracket grow as c² —
(window/4 + 3) points per prime × (2c/31 + 2) primes ≈ c²/16. The
reviewer measured 8.1 s at c = 803 and 68 s at c = 1606 on connected
sums of apex 256, ~90% inside `contractio_exsequi_modulo`; at T5's
braid apex 2^26–2^28 that is hours per knot. The 2c-bit bound is 2–3×
loose in practice (122 of 414 bits at c = 207), costing 2–3× the
primes. **L1** memory: the caller's arena grows superquadratically (11.5
MB at c = 803, 78 MB at 1606), almost all of it interpolatio's
scratch, which shares the caller's piscina with `exitus`.

Tests now: 10 sections, 56,344 checks, 2.2 s.

## 2026-10-27 — T5a: coefficient bound by spanning trees (fewer primes)

`_uncinus_per_rete` now passes limes = bits(B) instead of 2c, where B =
min over the two Tait graphs of Π_{v≠r} deg(v) (loops excluded, r a
vertex of maximal degree). Σ|cₖ| ≤ τ(G) ≤ B by the signed
Thistlethwaite expansion (smoothings are deletion/contraction in the
Tait graph; bridges and loops collapse to −A^(±3); leaves = spanning
trees) and the parent-edge injection. Proof, data and design review in
project-specs/contractio-plan.md §T5a.

**Colours by union-find, not a parity rule.** Region of corner (k, j)
(between arms j and j+1) = `faciei[4k + (j+1) % 4]`, the face numbering
`_gyrationes` already computes (now exported). Opposite corners (0~2,
1~3) are joined; the two classes are the colours. `_limes_arborum`
refuses unless there are exactly two classes and adjacent corners lie
in different ones. Degrees need no colour: per crossing, if two
opposite corners are different regions, each gets +1. Why: my first
Python version assumed corners 0, 2 always share the global colour —
false for non-alternating diagrams (which pair is "black" flips from
crossing to crossing) — and the design review simulated that bug: only
2 of 12,965 knots went red under a bits-only check.

Products: factors multiply into a u64 (< 2^32 before, factor < 2^31)
and flush into a Magnus only when the accumulator passes 2^32, so the
arena sees ~c/30 Magnus multiplications instead of ~c/2.

New API: `laqueus_modulus_vertex_limes` (bits), `_limes_exactus` (B as
Magnus), `_vocationes` (aestimare calls and prime changes since
creation), `_uncinus` (the bracket driver on a network the caller
holds). The last two exist so a test can see the bound in use.

**Tests** (probatio_laqueus_rete, 13 sections, 149,220 checks, 3.5 s):
Σ|cₖ| ≤ B (exact Magnus) and limes < 2c for c ≥ 3 on every table knot,
the kinked knots, the connected sums, and 40 random braid closures
(3–6 strands × 21–82 crossings); exact hand values from the review,
including the tight T(2,31) (B = 31 = Σ|c|, limes 5) and T(2,7) #
T(2,−9) (B = 63 = Σ|c|, limes 6); wiring: primes used = ceil((limes +
1)/31) + 1 and fewer than 2c would need. `_plexus_pd` (braid closure)
now lives in the test as well as the tool.

**Plants (radix/laqueus_rete), 6/6 red:** limes − 1; degree counted at
one end only; two vertices excluded per colour; adjacent corners joined
(the colouring fault: refused as one class); 2c passed instead of limes
(caught only by the wiring test); last u64 flush dropped. First form of
the flush plant did not compile (i64 → s64 conversion); rewritten with
a cast.

**Measured** (tools/uncinus_mensura, same seeds as f8563c60, now on the
library driver): contractions and primes fall ~2.1–2.4×. Connected sums
c = 800: 40,651 → 16,874 contractions, 53 → 22 primes, 8.6 → 4.9 s,
arena 10 → 3.2 MB. Braids 6 × 201: 4.8 → 2.1 s; 8 × 201: 43 → 21 s;
12 × 201 projected 215 → 99 s; 10 × 201 projected 77 → 34 min. Section
I (c = 12–24): 3 → 2 primes, all still equal to the state sum. The
remaining gap (limes 121–171 vs actual 31–39 bits at 201 crossings) is
cancellation, which no counting bound sees.

## 2026-10-27 — T5a review (independent, after 7a2a8deb) and fixes

Reviewer: suite under ASan/UBSan clean; a fuzz harness compared the
library's exact B (Magnus, decimal) with an independent Python
min-over-colours B as exact integers, limes = bits(B), Σ|c| ≤ τ ≤ B,
τ(G) = τ(G*), primes used = ceil((limes+1)/31)+1, and the bracket
against the state sum (c ≤ 16) or a scratch build of laqueus with the
old 2c bound: ~5,900 small diagrams (table ≤ 7 and mirrors, 1,500
kinked/mirrored/relabelled braid closures, 400 connected sums) and 135
large ones (braids 3–8 strands × 17–120, sums of 3–10 knots): 0
mismatches. Flush stress: T(2,±301) (other colour 2^300), 40 summed
trefoils (B = 3^40 = Σ|c|, tight just under 2^64), a 122-bit mixed sum.
Hand values recomputed independently. No H, no M.

Fixed:
- **L1** the random-braid section's state-sum comparison was dead (all
  braids ≥ 21 crossings, branch at ≤ 16): lengths now 8–82, so the
  short ones meet the oracle; the strict "fewer primes than 2c" check
  applies from 40 crossings (below that both can need two primes).
- **L2/L3/N1** the call counter counted every aestimare call (incl.
  INUTILE and refused primes) and the tool built a second network inside
  the timed window. The counter now counts contractions exactly
  (incremented right before `contractio_exsequi_modulo`); the tool
  reuses its probe network, reads counters as differences, and probes
  on 998244353 so the driver's first prime is counted as a switch.
  Like-for-like at c = 800: 8.6 → 3.6 s (2.4×), arena 10 → 2.3 MB (the
  4.9 s in the T5a entry included building a second network).

## 2026-10-27 — laqueus_pd_ex_plexu: braid closure → PD (contractio-plan T5)

Moves the braid-closure builder that lived twice (test, measurement
tool) into the library. Word = s32 letters ±i (σ_i^±1), the same
encoding as `Plexus` (plexus.h) — a caller holding a plexus passes
`p.litterae, p.longitudo, p.fila`. plexus.h is deliberately NOT
included: it pulls matrix, radices, cyclotomia and nervus into every
binary that links laqueus, and tools/tabula_nodorum_generare.sh (a hand
list, tabularium 01M5ZJ7X) has already broken twice on new laqueus
dependencies today.

**Convention, standard and now tested:** σ_i^(+1) gives a POSITIVE
crossing (writhe +1): the closure of σ1³ is the right-handed trefoil,
table 3_1, and (σ1…σ_{p−1})^q is the right-handed T(p, q). The test and
tool copies had the opposite sign (their T(7,8) needed a mirror); with
the library, T(7,8) equals the torus formula and NOT its mirror. Shape:
strands run downward; at crossing k between positions i, i+1, a = upper
left (goes to lower right), b = upper right; positive → [a, c', d', b]
(a under, b over from X3 to X1), negative → [b, a, c', d']. Arms
counterclockwise from the under-strand's entry. Edges relabelled 1..2c
by traversal from edge 0; the result is validated by `_pd_legere`
(c ≥ 2).

Refusals (outputs untouched): fila 0, letter 0 or |letter| ≥ fila,
empty word with fila ≥ 2, an untouched strand, a closure with several
components (permutation not one fila-cycle — e.g. word length and
fila of the same parity). fila 1 with the empty word = unknot
(transitus 0, pd NIHIL); σ1 on 2 strands = the one-crossing kinked
unknot (returned; laqueus' invariants refuse c = 1 as documented).

**Tests** (new section "plexus"): σ1³ = table 3_1, σ1⁻³ = its mirror;
40 random braids (3–6 strands, 10–41 crossings): Jones unchanged under
conjugation g w g⁻¹, cyclic rotation, stabilization σ_n^(±1) on n + 1
strands and σσ⁻¹ insertion; bracket = state sum up to 16 crossings;
Hopf link, three-component, untouched-strand, out-of-range and empty
words refused with outputs untouched. The test and the tool now call
the library (thin `_plexus_pd` wrappers).

**Plants 3/4 red:** forms swapped; closure identification dropped;
empty word accepted. Survivor: the range check `>= fila` relaxed to
`> fila` — the out-of-range word is still refused by later checks
(after an out-of-bounds write into arena memory), so the check is
defense in depth and a test cannot see it without undefined behaviour.

## 2026-10-27 — laqueus_pd_ex_plexu review (independent, after 1ec50212) and fixes

Reviewer (scratch, ASan/UBSan): an independent Temperley–Lieb oracle on
the braid word itself (σ = A·1 + A⁻¹e) against
`laqueus_jones_ex_pd_rete` on the PD — exhaustive over all words for
fila 2 (length ≤ 10), 3 (≤ 6), 4 (≤ 5), 5 (≤ 4) with out-of-range
letters mixed in: 23,420 cases, 6,531 accepted knots, 0 mismatches,
acceptance exactly "permutation is one fila-cycle"; ~600 random knots
to 200 crossings; all 78 torus T(p, q) (p ≤ 8, q ≤ 12) against the
formula, mirror and swapped braid; R3, far commutation, mirror and
reversal on 107 random pairs; σ1³'s PD equals KnotInfo's 3_1 crossing
for crossing, σ1⁵ = table 5_1, σ1³σ2σ1⁻¹σ2 = table 5_2 (no mirror). No H.

Fixed:
- **M** `ossa = fila + 2c` wrapped in i32 for an absurd fila (2^32 − 1
  with the word {1}): idem/caput got one element and `idem[cn]` wrote
  ~16 GB out of bounds (BUS). Now refused up front when fila > c + 1 —
  exact, since every strand must be touched and connected, so a knot
  needs c ≥ fila − 1 — which also bounds ossa ≤ 3c + 1; and c ≤ 2^30 − 1
  so 4c cannot wrap. The plant removing this bound was run under
  `tools/custos.sh`: the test began touching a 16 GB array and the guard
  killed it at 5.9 GB of footprint (effectively red, and the hazard
  itself demonstrated); the kill left the planted line in place,
  restored from the pre-plant copy.
- **L** `(i32)-verbum[k]` negated an s32 — undefined for INT_MIN, inside
  the guard meant to reject it. Both places now negate in unsigned
  arithmetic ((i32)0 − (i32)letter).
- **L** sign pinning rested on σ1 alone. Added: σ1⁵ = table 5_1 and
  σ1³σ2σ1⁻¹σ2 = table 5_2 (mixed signs on σ2, no mirror); R3 and far
  commutation on 20 random braids of 4–8 strands; the exact
  one-crossing codes ([1,1,2,2] for σ1, [2,1,1,2] for σ1⁻¹); refusals of
  fila 2^32 − 1, fila 5 with two letters, and an INT_MIN letter (outputs
  untouched).

Trap: `brevis` is the latina.h macro for `short` (test variable,
renamed `angustum`).

## 2026-10-30 - T2a of project-specs/temperley-lieb-plan.md: window and bound factored out

The Kauffman window and the Tait-graph bound were reachable only through
the vertex structure (laqueus_modulus_vertex_creare builds the whole
network and its workspace). Factored into interior helpers on the read
PD code (_fines, _facies split out of _gyrationes, _fenestra_uncini,
_fenestra_et_limes) and a public laqueus_pd_fenestra (O(c), no network).
Vertex creation split into _modulus_vertex_consilium (order and cost,
no workspace; for the T3 dispatcher, design review H2) plus the
workspace.

Acceptance, as the review asked (Q3: "vertex probatio byte-identical"
cannot see a wider window or a looser bound - extra coefficients come
out zero, a loose bound only costs primes):
- golden values captured at be6be402 BEFORE touching the code, by a
  scratch program that #includes lib/laqueus.c and reads mv->imus,
  mv->summus, mv->limes, mv->limes_exactus: 21 explicit knots, an FNV
  checksum over all 12,965 table knots with c >= 2, and D136's five big
  braids (with an FNV fingerprint of each word, so a change in sors
  cannot swap the braid); the big braids' windows and bounds equal
  D136's printed ones (independent cross-check of the capture);
- the same capture rerun against the refactored laqueus.c: identical;
- laqueus_rete output identical apart from timings (150,422 checks),
  then two new sections (fenestra aurea; fenestra et vertex: bound and
  B equal to the vertex model's on 200 table knots, refusals leave
  outputs untouched): 151,612 checks;
- plants (build/temperley/plantae_t2a.py), all UT DICTUM: window 4
  wider at the top -> ONLY 'fenestra aurea' red (the blind spot the
  review predicted); bound one bit looser; bound from the larger Tait
  graph; window class from w instead of 3w.

Pitfall: linking a scratch program against every build/*.o drags in
AppKit/WebKit/IOKit; `ar rcs` the objects into an archive and link that,
so only needed members are pulled.

## 2026-10-30 - T2b of project-specs/temperley-lieb-plan.md: the TL route

laqueus_modulus_temperley_{creare, aestimare, uncinus, limes} and
laqueus_temperley_sumptus: D136's evaluator in the library. Action
tables copied from temperley once (modules built inside
piscina_notare/reficere, so they never exist twice); two buffers of
max d^2 via piscina_conari_allocare; nothing allocated per point
(tested with piscina_numerus_allocationum).

Shared with the vertex route, so the two are comparable point for point:
_punctum_eligere (A = i omega^2, prime cache, INUTILE where delta =
omega^4 + omega^-4 = 0, ERRATUM on bad primes; the vertex aestimare now
calls it and its suite output is unchanged) and _quaestio_uncini (the
interpolation settings). Window and bound via laqueus_pd_fenestra (T2a).

Tests (7 sections in probatio_laqueus_rete): state sum (c <= 24), vertex
bracket and bound (c to 100), points (status, punctum AND valor equal
the vertex route at p, q, p; nothing allocated; p = 17, omega = 2 gives
A = 1, which pins g minimal), Jones of 3_1, 4_1, 8_19 against the table
and T(4,5) against the vertex route, D136's certified brackets (3 x 50,
8 x 101, 8 x 201, with word fingerprints), Markov stabilization front
and back plus inserted braid/R2 identities at points on 10 x 401 (11
strands; first tried 12 x 101 on 13 strands, 12 s in the unoptimized
test build, moved), refusals and the byte cap at its boundary.

Plants (build/temperley/plantae_t2b.py) 7/7 UT DICTUM: Delta recurrence
with +; /delta dropped (motus stays green: a stabilization identity
scales both sides alike - only absolute oracles catch it); sigma^-1
given sigma's image; byte cap > -> >=; TL limes - 1 (caught by the
limes equality test, and by small knots where the bound becomes too
small to interpolate); punctum reported as A^-1; outputs written before
a refusal.

Plan deviation: the plan's "g not minimal in the point choice" plant is
moot once the point choice is one shared function (both routes would
change together and agree); g minimality is pinned instead by the
p = 17 test (A = 1), and the plant was replaced by "punctum reported as
A^-1".

Build lists: tools/tabula_nodorum_generare.sh and venenum_probare.sh
name their library sources by hand; lib/temperley.c added to both
(fabrica's heal step failed until then). Knot table regenerates
byte-identical (-probare).

## 2026-10-30 - T3 of project-specs/temperley-lieb-plan.md: the dispatcher

laqueus_via_eligere (choice without evaluation), laqueus_uncinus_ex_plexu
and laqueus_jones_ex_plexu. Order: refusals (laqueus_pd_ex_plexu) before
any pricing; c <= 1 closed form (LAQUEUS_VIA_NULLA: 1, -A^(+-3), Jones
1); eligibility (vertex workspace <= cap; TL fila <= XVI and bytes <=
cap); cheaper eligible route by integer saturating price (vertex mult x
185 against TL mult x 150, D136's ns x 100); tie to TL; no retry. The
vertex plan used for pricing lives inside piscina_notare/reficere.

Measured first, as the plan asked (scratch, -O2): planning the vertex
route takes <= 0.2 s even at 12 x 801 (0.03 s at c = 201), and every
registered choice came out as predicted, margins 8x or more.

Two things the tests taught:
- **The cap does not just admit or refuse the vertex route**: with
  CONTRACTIO_OPTIMUS, a cap below the chosen order's workspace makes the
  planner pick another order that fits. The boundary test therefore
  walks the cap down below each reported workspace until nothing fits
  (then FALSUM, outputs untouched), checking every intermediate choice
  reports workspace <= cap.
- **The review's "cap just below the vertex workspace flips the choice
  to TL" did not occur**: in every braid measured, when the vertex route
  is cheaper its workspace is also smaller than TL's buffer bytes
  (8 d_max^2 dominates TL's memory; a cheaper vertex route needs apex
  below about Catalan(n)), so such a cap excludes both. Tested instead:
  each route's own cap boundary (TL exact bytes / one less; vertex
  descent). Observed over the measured braids, not proven. **[WRONG -
  corrected 2026-10-31 by the T4 review (M1): the reviewer found 1,255
  flips in 24,690 random braids, n = 7..14. A cheaper vertex route only
  bounds its apex below ~Catalan(n) = 2.4-2.7 d_max^2, and its workspace
  is 4 B x apex x buffers, so it can exceed TL's ~8 d_max^2. My sample
  of braids was too small and too regular. The flip test is restored
  with the reviewer's 7 x 32 braid.]**

Plants (build/temperley/plantae_t3.py) 5/5 UT DICTUM: cost comparison
inverted; closed-form sign of sigma^-1; vertex workspace cap ignored; TL
byte cap ignored; Jones writhe sign. Untestable, recorded: the tie rule
(equal integer prices do not occur with real braids).

examen reports one "domesticum" (negative sentinel in an unsigned
function) on `redde CONTRACTIO_SATURATUM` in _pretium: ~0 is the
saturation maximum, not a sentinel; contractio.c's own saturating
helpers carry the same two findings.

## 2026-10-31 - T4 review (recensor-temperley) folded in

No H. Fixes: flip test restored (M1, my T3 claim was false); the
"pricing allocates nothing" check now reads the apex of a fresh piscina
(M2: piscina_summa_usus is restored by reficere, so comparing it before
and after pricing checks nothing - use piscina_summa_apex_usus on a
fresh arena); 17-strand braid through the dispatcher (M3); creare rolls
back on every refusal after pricing (L5); stabilization at 3 points;
second-prime pin of A. Review faults A and B now caught by named tests
(build/temperley/plantae_t4.py, both UT DICTUM). Full list in
project-specs/temperley-lieb-plan.md, "Review (T4)".

Lesson: a negative claim ("X cannot happen") drawn from a handful of
measured cases needs a search, not a sample. The reviewer's random
search found the counterexample in minutes.

## 2026-10-31 - celeritas T1: the in-place TL kernel

sigma = A^e (1 + c E), A^w pulled out of the trace; only DELTA rows
change: M[r] <- alpha M[r] + c S_r, alpha = -A^(-4e), c = A^(-2e), one
buffer. The number of DELTA rows per letter is exactly d(n-2, j)
(contract the arc: a bijection onto W_{n-2,j}), so the source lists are
fixed-width tables (sources < fila) with a closed-form byte count; no
per-state action tables are kept.

Result against the registered prediction: P1 (>= 3.0x per point) FELL at
all three sizes - 2.85x (12 x 801), 2.72x (14 x 801), 2.87x (16 x 801),
A/B medians of 9 ABBA pairs, old value = new value at every point. The
operation ratio was 6.7-6.9x; what survives is ~2.8x. Per updated
element ~2 ns, dominated by two 64-bit `%` (primes near 2^32). P2 held
(-47.1% bytes at 16 strands; figure = allocation within 17-73 bytes).
P3 held (12 x 801 bracket byte-identical to D136's brackets.txt; 1,465 s
CPU, descriptive, measured under load).

The "source sum not reduced" plant is an equivalent mutant: S is a
32-bit word, the wrap fix keeps it < 2^32 and congruent mod p, so
(alpha M mod p) + c S < 2^64 for any p < 2^32. The x >= p step is kept
(cheap) but is not what makes the kernel correct.

The 7 x 32 cap-flip test braid stopped flipping in T1 (TL bytes up with
the source tables); replaced by 9 x 74 (K* = 47.8). Memory-only flips
become rare as TL gets cheaper: 254 with K* < 150 but 3 with K* < 60 in
200,000 random braids.

## 2026-10-31 - celeritas T2: one reduction (G1) and Montgomery (G2)

G1: with the TL route restricted to primes below 2^31, alpha M + c S <
2 (p-1)^2 < 2^63 needs one `%` instead of two: 1.80x (12 x 801) and
1.71x (16 x 801) over the T1 kernel. Adopted: TL aestimare refuses
p >= 2^31, its interpolation sets primus_limes = 2^31, step-1 rule 4
amended (the vertex route keeps primes near 2^32).

G2: Montgomery with R = 2^32 folded into the four constants (alpha R,
c R mod p), so the matrix never leaves normal form and the identity
stays 1; p' = -p^-1 mod 2^32 by four Newton steps from x = p (p x = 1
mod 8, bits 3 -> 6 -> 12 -> 24 -> 48); one REDC per element (t + m p <
2^64 because t < 2^63 and m p < 2^63 for p < 2^31). The design review's
microbenchmark had Montgomery 0.76-0.87x of `%`; on the real kernel it
is 1.32-1.45x (gate) and 1.34-1.36x (P4, registered before measuring).
Microbenchmarks of an isolated operation did not predict the kernel;
the gate on the real shape did.

## 2026-10-31 - celeritas T3: acceptance and dispatcher refit

All 45 D136 brackets recomputed through laqueus_uncinus_ex_plexu are
byte-identical to knotapel/demo_136_tl_braid_closures/brackets.txt.
Cumulative per-point speed-up of the TL route over 79e7fad9: 6.82x /
6.98x / 7.42x at 12 / 14 / 16 x 801 (A/B in one binary). TL price
constant refitted 150 -> 21 (vertex constant 185 unchanged); the thinnest
remaining margin is 12 x 201 (K* 17.85 against 21).

Memory-only cap flips (vertex cheaper but bigger than TL's bytes) get
rare as TL gets cheaper: none with K* < 30 among short braids; long
braids (c = 130-200) still have them. Test braid now 11 x 134 (K* 15.7).

## 2026-11-01 - celeritas T4 review (recensor-celeritas) folded in

No H. Correction to the T1 entry above: the "source sum not reduced"
plant was an equivalent mutant ONLY for the T1 code. Since G1/G2 the
reduction `x >= p ? x - p : x` is load-bearing (unreduced, up to n/2
sources wrap the 32-bit word incongruently, and S >= p breaks Montgomery's
t + m p < 2^64); the plant is red against today's code. Do not remove it.
Fixes: boundary refusal test at 2147483693 (a raised threshold returned
wrong values with the suite green); two defensive checks in creare (DELTA
rows map to themselves; an UNUM image is a DELTA row of this letter).
Full list in project-specs/temperley-celeritas-plan.md, "Review (T4)".

## 2026-11-01 - the 14 x 801 bracket

Full TL bracket of the D136 14 x 801 braid: 7,282 s CPU (T0 kernel ~12 h),
6 MB, A^[-543, 557], largest coefficient 195 bits (limes 662). Not oracle-
certified (too big); it passes knot-level checks: <K>(1) = -1 (one
component), exponents all 1 mod 4, V(1) = V(omega) = 1, and V(i) = -1
agreeing with det = 5 mod 8 (Arf 1 both ways). Cheap sanity script idea
for any future big bracket: these four checks need only the coefficient
list and the writhe. Record: project-specs/temperley-celeritas-plan.md
"Headline", line in knotapel/demo_136_tl_braid_closures/uncinus_14x801.txt.

## 2026-11-02 - Regina 7.4.1 as external cross-check (benchmark step 1)

Regina (pip wheel, no deps, run under sandbox-exec, no network; build/regina/,
uncommitted) Link.bracket(Treewidth, threads=1) on the D136 braids via
PD from tools/uncinus_mensura -sectio XII words. Conventions matched first
try (same bracket, same writhe). 32 of 45 braids finished, ALL IDEM with
brackets.txt. Regina is competitive or faster up to ~100 crossings (11x102:
0.023 s vs our 0.26 s vertex; 12x101 0.026 vs 0.07) but its memory explodes
on long braids: 7x202 14.5 s / 2.9 GB (ours 0.10 s TL), 12x201 16.2 s /
2.6 GB (ours 7.2 s vertex), and 8x201, 9x202, 10x201, 11x202 all exceed
8 GB (ours 0.18-7.9 s, MBs). So it is no oracle beyond our own (12x401) -
14x801 stays unconfirmed. Our vertex route loses to Regina on short
many-strand braids: worth a look (its tree decomposition vs our greedy order).

## 2026-11-02 - why Regina wins on short many-strand braids

Not ordering, mostly arithmetic shape. Regina's tree decomposition width on
10x101 / 11x102 / 12x101 is 8 (13-15 on the c ~ 200 braids it can't
finish). Our vertex route on 11x102 (-sectio V): width 12 (2^12 = 4096
entries), 0.8 ms per point, but ~300 points (4 primes x ~75) -> 0.25 s;
Regina does ONE symbolic pass (bracket updates are monomial shifts + adds,
linear in degree) in 0.023 s. Evaluation/interpolation pays per point;
its win is memory (one word per state), which is why it wins from c ~ 200
up. Also: OPTIMUS never tried other candidates here (AVIDUS under the 2^20
multiplication limen, cand 1), so width 12 vs a possible ~10 is unexamined
but irrelevant at 0.25 s.
The useful find: the coefficient bound is 3-5x loose (bits/limes 16/82,
19/86, 24/85, 17/88; 14x801 195/662), so most primes are wasted. Ledger
desideratum 01M6F2RT: early termination + random-point verification.

## 2026-11-02 - interpolatio-verificatio T0: baseline without evaluation

New tool sections in tools/uncinus_mensura.c: XIII (per braid, no evaluation: a-priori window and limes via laqueus_pd_fenestra, the interpolation prime sequence reproduced, k_true from known brackets, dispatcher route, vertex cost via laqueus_via_eligere, exact m) and XIV (segmented sieve of [2^30, 2^31), 1.9 s, N = 25,348,870 primes = 1 mod 4, sum checked against pi(2^31) - pi(2^30)).

Finds:
- The design review's integer recipe for m (e = floor(-log2 eps), m = ceil((s + ceil(log2(k_max - 1)))/e)) rounds twice and asks for one check too many on 13 of 36 braids that can stop early (all c ~ 50 braids: 4 instead of 3; all 801-crossing braids: 5 instead of 4). Fix: exact comparison (k_max - 1) a^m 2^s <= b^m with eps = a/b in Magnus; the tool does it and matches a Python Fraction check on all 46 rows. 3 x 50 is the test vector that separates the two.
- Self-checks per verified run = k_true, not k_true - 1: the stopping stage also self-checks (and passes) before the m certifying checks.
- 14 x 801: K_q = 561 (as 12 x 801), not ~540; vertex workspace 3.63 GiB (over the 2 GiB limit), 2.89e12 multiplications per point (~89 min at 1.85 ns).
- The exact-product lever (desideratum 01M6FNZH) saves a prime on 5 of 46 braids.
- An identifier trap I fell into: `duplex` (= double) and `M` (= 1000) are latina macros; the compile errors were obscure ("expected expression", "passing int to Magnus"). Renamed bis_maximus / productum.

## 2026-11-02 - interpolatio-verificatio T2: verified brackets through laqueus and the tool

`laqueus_uncinus_ex_plexu_verificatum` (options struct `LaqueusOptionesVerificandi`: octeti_maximi for main + checker together, semen, semen_origo, s, verificator AUTOMATICUS / SUI / VIA_ALTERA / EXTERNUS, alienus) and `laqueus_uncinus_promovere` (rebuilds the module of the recorded route; the prime prefix must match). Laqueus checkers declare c = 8, r = 9; the point map reduces omega mod p, so check indices up to q - 1 are safe.

The automatic rule (cross route iff m x price_other <= (k_max + 1)(K + 2) x price_main / 20, and both workspaces fit) almost never picks the cross route: the dispatcher already chose the cheaper route, so the other one is only acceptable when the two cost about the same. On the D136 set it picks it for 9 x 102 (TL -> vertex, ratio 3.35 vs limit 3.70), 10 x 101, 11 x 102 (vertex -> TL, 1.39 vs 3.75), 11 x 202, 12 x 201; NOT for 8 x 101 (vertex 49x dearer) as plan v3 assumed, so the test uses 9 x 102 and 11 x 102 (word hashes 250855745, 1527719524) and checks 8 x 101 stays self.

Mirror test with laqueus evaluators: a checker that reports <D>(A) at the point A^-1 (TL module of the same braid) -> DISCORDIA on 8 x 101 (precondition: bracket not symmetric under A <-> A^-1).

Tool (`uncinus_mensura`): `-modus verificatum` with sections VIII and XI, `-semen_verificationis X` (else 8 bytes from `moneta_octeti_fortuiti`, printed, FORTUITUM; `-semen` stays the section V braid seed), `-verificator automaticus|sui|via|congelatus`, `-promovere VIA` (pairs of UNCINUS + RELATIO lines). The frozen checker wraps section VI's 79e7fad9 kernel with its own point code (own i via the smallest non-residue, own delta). First check: 8 x 401 with the frozen checker stopped at 3 of 11 primes (T0's k_true), 837 evaluations (T0's count), byte-identical to brackets.txt; promotion computed 8 primes and reproduced it.

Latina traps hit in one task: `externus` (= extern) as a struct field, `casus` (= case) as an array name. Renamed alienus / exempla.

Plants: build/verificatio/plantae_t2.py, 4 of 4 red as registered (always self-check; VIA_ALTERA ignored; cost rule without the 5% factor; external checker ignored).

## 2026-11-03 - verificatio T4 review: cost-rule and label tests

The 5% cost rule had no braid between the thresholds, so "5% -> 50%" and "rule ignores m" mutants survived. 8 x 51 (TL main, ratio m * price_vertex / ((k_max+1)(K+2) price_TL) = 0.067) sits above 1/20 but below both 1/2 and m/20, so it pins both; found by scanning T0's FUNDAMENTUM cost columns (build/verificatio/t0_fundamentum.out). Its word hash 2481405640 recomputed in Python from tools/uncinus_mensura -sectio XII words with the test's _misce (reproduces the three known hashes). The test now also asserts each example's m and the communia label (rule 2). r = 9 -> 0 remains untested at this level on purpose: no numerical effect at q >= 2^30.

Plant note: T2 plant B's single-line anchor was re-wrapped by commissio's formatter after the T2 run; planta then refuses the replacement (format guard). Plant scripts kept in build/ go stale this way after their commit - rerun with the current text.

## 2026-11-03 - TL kernel under concurrent copies (L3 groundwork)

Experiment registered and run before drafting the L3 plan (predictions,
results: build/verificatio/concurrentia_praedictio.md; driver in the
session scratchpad, linking uncinus_mensura's objects). N = 1..8 copies
of one-point TL evaluation at 12/14/16 x 801, CPU s/point per copy,
12 strands as the cache-resident control.

Memory bandwidth is NOT the limiter: beyond the control, 16 strands
loses only ~10% at 4-6 copies and ~18% at 8 (predicted <= 15% / 30%).
So column blocks stay deferred; at most they would recover that ~10-18%.
14 strands loses 17% at 4 copies (predicted <= 10%), consistent with
four ~4 MB module matrices sharing the 16 MB P-cluster L2.

The larger effect is size-independent: even the control costs 1.35x per
point at 2 copies and ~1.7x at 6-8. This machine is a fanless MacBook
Air M2 (4P + 4E), other sessions pushed load to 75 during the run.
Practical throughput at 16 strands: ~2.1-2.5x at 4-8 workers, not the 3x
I had guessed. Use that for L3's registered threshold.

## 2026-11-03 - L3 T2: batch hook, spot check, workers in uncinus_mensura

The verified entry takes `fasciculus_temperley` (TL route only). Every
batch is spot-checked on laqueus's own module at index_primus + (p mod
numerus): status equal, punctum/valor equal where UTILE. The fingerprint
`laqueus_modulus_temperley_signum` (FNV-1a 64 over fila, longitudo,
word, writhe, window, limes) has a module-free twin
`laqueus_temperley_signum` so the parent need not build a third module.

The useful lesson came from plant L1 (spot check removed): my first
divergent worker (one letter's sign flipped) did NOT need the spot check.
Its values are random-looking against the true window, so the restored
coefficients blow the coefficient bound and the run refuses anyway. The
review's H2 danger is a worker whose wrong answer is still a *valid
small polynomial*: bracket + A^e with e in the window. With k_max = 1
nothing compares (self-checks go through the same lying batch function
and agree with it), and the run returned VERUM with the wrong bracket.
k_max > 1 was caught by the certificate (in-process SUI) anyway. So: the
spot check is what closes k_max = 1, and a "divergent" test must diverge
plausibly, not randomly.

Tool side (tools/uncinus_mensura.c): workers are a frozen byte copy of
the binary in build/opifices/<pid>/; build id = LC_UUID of the running
image read from memory (`_dyld_get_image_header` + load commands, strict
C89 compiles fine with <mach-o/loader.h>), so it compares running code,
not files. Worker output is parsed strictly: first line and FINIS line
compared as whole strings (sprintf the expected line, memcmp), data lines
token by token, index must be ab + j in order. Parent also counts
coverage. Fault injection `UNCINUS_OPUS_VITIUM="<genus>@<ab>"` exists for
the refusal cases; `statim` (fail before computing) is the one that makes
"other workers stopped" observable - with the others still computing at
12 x 801, a missing abrumpere leaves visible orphans.

Trap hit: `i64` is unsigned here, so `v > (maximum - d) / 10` wraps when
d > maximum; `-3` passed as a letter on 3 strands until guarded.

## 2026-11-04 - L3 T3 (puncta-parallela): the worker deadline was the weak part

First exact run (binary a0c98416): no wrong answer anywhere, but the 10x
deadline (10 x slice x 0.21 ns/mult estimate, floor 60 s) refused healthy
workers twice. Causes stacked: the nominal estimate is ~1.7x low at
12 x 801 (0.035 vs ~0.06 s/point); 8 orphaned `yes > /dev/null` from a
quarta load test held every core for ~17 h (workers got 20-30% of a
core); and a maintenance-sleep / DarkWake cycle - processus' clock is
CLOCK_UPTIME_RAW, which skips true sleep but runs during dark wakes, when
our processes barely run. A total deadline cannot tell "slow" from
"hung", and a 16 x 801 run (hours, no retry, laptop) would likely die.

Fix (Fran, option A): silence timeout. The worker prints SIGNUM as soon
as the module exists, then evaluates ONE index at a time and fflushes
each line; the parent polls processus_effusio_hactenus and kills a
worker whose output length has not changed for max(600 s, 50 x per-point
estimate) on the house clock (mach_absolute_time, stops in sleep); no
total deadline. `-silentium S` for tests; faults `pende` (silent after
line 1, gives up at 60 s so it never orphans forever) and `lente` (200 ms
per line). Plant S3 is the one to remember: without the per-line fflush,
stdout to a pipe is block-buffered, a slow worker's output sits in its
buffer until exit, and the parent sees silence - the same false kill in
a new shape.

Also: custos' wall cap cut four `via` runs (its 12 x 801 vertex
cross-checks run in the parent, 1,500 s CPU, not parallelised); a
SIGKILLed parent leaves build/opifices/<pid>/ (atexit skipped) - five
such directories cleaned by hand. P1's registered route count 32/13 was
copied from a review miscount; the real split is 31 TL / 14 vertex.
