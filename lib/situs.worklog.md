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
  (shared endpoint) are always TANGUNT — knot code must skip them.
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
