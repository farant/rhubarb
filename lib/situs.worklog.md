# situs.worklog.md

## 2026-10-05 — natus

`situs` (Latin "position, arrangement"): exact geometric predicates over
`fractio` coordinates — #9 in Fran's math library map
(`project-specs/mathematica-bibliothecae-visio.md`), taken early because it
gates every geometric decision, and because every knotapel construction-word
failure (docs/knotapel.html notes 58–59) was a predicate decided in floats or
in overflowing integers. API reviewed with Fran first (name, over/under here
rather than in the future knot library, rationals only for now).

Design:

- **Everything is a determinant sign.** 2D orientation = cross product;
  3D = triple product det[u, v, w].
- **Degenerate cases get their own answer**: every contact test returns
  DISIUNCTA / SECANT (proper interior crossing) / TANGUNT (degenerate:
  endpoint on the other, collinear overlap, coplanar contact, non-generic
  projection). Demo 110's crossing finder silently gave a self-intersection
  a crossing of sign 0; here the caller is told. Adjacent polygon edges
  (shared endpoint) are always TANGUNT for the general predicates — use
  the `*_vicin*` predicates for them (see the review III entry below).
- **3D segments**: non-coplanar → disjoint; coplanar → drop a coordinate
  where the plane's normal is non-zero (injective projection) and decide in
  2D; all collinear → 1D intervals on a coordinate the line is not
  perpendicular to.
- **Triangle–segment** (the triangle-move test): side-of-plane signs, then
  the three signed volumes of (p, q, edge) — the line meets the closed
  triangle iff they don't disagree in strict sign. Degenerate (collinear)
  triangle → edges only, never SECANT.
- **Over/under without projected coordinates** (`situs_transitus`): in the
  projection along v, the 2D cross becomes det[x, y, v] and the 2D dot
  (x·y)(v·v) − (x·v)(y·v), both scaled by positive factors, so all signs are
  exact for ANY non-zero v, including (2,3,5). Crossing parameters by Cramer
  with det[u, w, v]; depth compared as v·P (viewer at +v). A segment that
  projects to a point is handled explicitly (its "betweenness" is equality
  of projections — the generic dot test would wrongly say yes).
- **Crossing sign** = sign det[over, under, v] (right-hand rule). Viewing
  from −v swaps over/under but keeps the sign — a property of the knot, not
  the picture; the tests assert it.
- **Memory**: public sign/contact predicates mark the piscina and roll back
  (net zero — safe in O(n²) loops; nothing they allocate outlives the call,
  so the credo/rollback hazard can't arise). `situs_volumen_sexies` keeps its
  result: it is the certificate of an orientation sign.

Verification:

- **Oracle with DIFFERENT algorithms** (Python Fraction, scratchpad
  situs/oraculum.py): Cramer parameters and interval classification for
  segments, plane intersection + barycentric coordinates for triangles,
  explicit rational basis ⊥ v + 2D solve + depth for crossings — where the
  library uses only orientation signs. Coordinates mostly in −2..2 so
  degeneracies are common; coplanar crossings and coplanar quadruples were
  constructed (random points almost never produce them). 1220 vectors:
  plana 240, spatii 300 (61 proper coplanar crossings), trianguli 260,
  transitus 260, orientatio 160 (45 zero). All agree.
- **Symmetries over 2000 random cases** (sors): orientation antisymmetry,
  translation invariance, mirror flip, volume = certificate of the sign;
  segment order/direction invariance; triangle vertex permutation; crossing:
  swapping segments flips over/under and keeps the sign, reversing one
  segment flips the sign, viewing from −v flips over/under and keeps the
  sign.
- Hand cases: the textbook positive crossing (also from (2,3,5)), T
  junction, collinear overlap, parallel, shared endpoint, triangle pierced
  through middle / vertex / edge, endpoint on plane outside, coplanar inside.
- **Planted faults**, all red: touching counted as crossing; point-projected
  segment "between"; crossing sign flipped; edge hit counted as proper;
  rollback omitted; coplanarity check skipped. The rollback plant first
  SURVIVED: with small coordinates nothing is allocated (magnus fast path),
  so the memory test was vacuous — fixed with a ~10^30-coordinate test that
  also asserts `volumen_sexies` DOES grow the piscina (proves the test
  allocates).
- 16499 checks (mostly coordinate parses); formator CONFORMIS; Latin lint
  NOVA 0 after renaming abbreviations (pq, lo1, hi1, op, oq, s_uw, sup, sg)
  and adding `orientatio` to the glossary (good neo-Latin, used in house
  comments, but absent).

Deferred: an integer fast path (invisible; when benchmarks ask), adaptive
float filters (allowed by the map, not needed yet), point-in-polygon,
incircle/insphere.

## 2026-10-05 — review III: neighbor predicates and coverage gaps

The reviewer's third pass found no wrong answers but a usability trap:
every general predicate says TANGUNT for adjacent polygon edges (the shared
vertex IS contact), and my header told knot code to "skip" them. Skipping
is wrong: two adjacent edges can fold back onto each other, and the edges
next to a triangle move can lie inside the triangle. So three predicates
ask "is there contact OTHER than the shared vertex?" — DISIUNCTA (only the
vertex) or TANGUNT (overlap or degenerate), never SECANT:

- `situs_segmenta_vicina(a, b, c)`: [a,b], [b,c] overlap iff u = a − b and
  w = c − b are parallel (u × w = 0) and point the same way (u · w > 0).
- `situs_triangulum_vicinum(t0, t1, t2, x)`: segment [t0,x] against the
  triangle sharing t0. The triangle is convex and contains t0, so contact
  beyond t0 iff d = x − t0 is coplanar AND in the closed cone of e1, e2.
  Cone test without solving: for d = αe1 + βe2, (e1 × d)·n = β|n|² and
  (d × e2)·n = α|n|². Callers rotate the vertex order for other corners
  (header USUS shows the triangle move AB → AC + CB).
- `situs_transitus_vicinus(a, b, c, v)`: the projections overlap beyond b
  iff one projects to a point, or det[u, w, v] = 0 with positive projected
  dot.

Verification: hand cases (straight, fold-back, perpendicular, zero length,
a == c; triangle above / inside / along edge / beyond along edge / outside
the cone; projection parallel, same direction, edge along v, v = 0). Then
a cross-check against the GENERAL predicates over 2000 random cases with
the degeneracies constructed (collinear c, coplanar x, projection-parallel
c): shrink the second segment to start at b + w/1000 — it no longer
contains the shared vertex, so the general predicate must give the same
answer (1/1000 is below every length on this coordinate grid; argued from
the grid's 1/3 and 1/9 granularity). Symmetries: swap the outer points,
swap t1/t2, flip v. Planted faults, all red: direction test inverted;
half the cone test dropped; coplanarity check dropped; point-projection
check dropped; projected-parallel check dropped; rollback omitted (big-
coordinate memory test).

Coverage gaps the reviewer listed, now asserted: the sign of
`situs_orientatio_plana` itself (only symmetries were tested), points
differing only in z, `situs_segmenta(P,P,P,P)` → TANGUNT, and
`situs_segmenta(P,P,Q,Q)` → DISIUNCTA. 16528 checks. Lint: test locals
`ax/ay/az` became `axis_x/…`, `_coefficiens` became `_factor_fortuitus`.

## 2026-10-05 — review IV: neighbor predicates confirmed

Independent oracle (parametric intersection + Fourier–Motzkin, no ε): 0
mismatches over 44k constructed cases and 649,539 exhaustive {−1,0,1}
grid cases. The triangle-move recipe is complete for simple polygons
n ≥ 3 (n = 3 with Z = W, n = 4, C = Z or W, C on line AB, fold-back at A
checked). The w/1000 shrink is sound on this grid with ≥ 3× margin
(worst cases 1/21, ~300, 1/111); if generator ranges widen, derive ε per
case — too large an ε fails red, too small could only hide an overlap
shorter than ε. 13 of 18 mutants killed, the 5 survivors equivalent.
Header now says the reverse move (AC + CB → AB) uses the same calls.

## 2026-10-06 — situs_transitus_parametri (for laqueus)

`laqueus` needs, for each crossing, WHERE on each segment it lies (to
order crossings along a segment: Gauss code, PD code, triple-point
test). Added `situs_transitus_parametri(a, b, c, d, v, piscina,
&superius, &signum, &s, &t)`: same verdict as `situs_transitus`, plus
the two exact parameters as Fractio. The internal `_transitus` gained
two `Fractio*` outputs, written only when the verdict is SECANT;
`situs_transitus` passes NIHIL and is unchanged (16528 checks green).
No rollback inside: the parameters live in the caller's piscina, so the
caller owns their lifetime (laqueus keeps them in the Diagramma).

## 2026-10-06 — situs_transitus_parametri now rolls back unless SECANT

Review laqueus-I (A): "no rollback" kept EVERY pair's arithmetic in the
caller's piscina, not just the parameters — O(n^2) per diagram, hidden
by integer coordinates (fractio's small path allocates nothing). Now:
notare, `_transitus`, reficere unless SECANT (the outputs, parameters
included, are written only then). Header updated. Scaled trefoil diagram
2.35 MB → 20.8 KB (laqueus test pins < 256 KB); situs 16528 green.

## 2026-10-07 — situs_reflexio

Reflection of p through the plane of a, b, c: p − 2 (n·(p−a))/(n·n) n with
n = (b−a)×(c−a); FALSUM for collinear a, b, c (exitus untouched). Folded in
from Demo 114 (D110–D112's construction words are exactly this
operation). Oracle without the formula: the image is the unique point with
(i) the midpoint of p and its image on the plane (`situs_orientatio`, other
code) and (ii) the difference perpendicular to b−a and c−a; plus
involution, points on the plane fixed, orientation flipped — 500 random
rational cases. Plants: projection instead of reflection (factor 1), no
collinearity guard — both red. (A first rewrite script replaced the wrong
function — `rindex` excluded the match ending at the anchor; the file was
restored from git and the function re-appended; additions-only diff
verified.)
