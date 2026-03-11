# Formulae — Spatial Euclidean Construction

Quick reference of algorithms and formulas.

## Representations

### Point
```
P = (x, y, z)
```

### Line (two-point parametric form)
```
L(P, Q): X = P + t*(Q - P),  t in R
direction d = Q - P
```

### Plane (point-normal form)
```
Pi(P, Q, R):
  normal n = (Q - P) x (R - P)
  equation: n . (X - P) = 0
  expanded: n.x*X + n.y*Y + n.z*Z = n . P
```

### Sphere
```
S(C, P):
  r^2 = |P - C|^2
  equation: |X - C|^2 = r^2
```

---

## Intersection Algorithms

### Line ∩ Plane → Point (Prop II.1)

**Input**: Line through P, Q. Plane with normal n through R.
```
d = Q - P
denom = n . d
if denom == 0: parallel (no intersection or line in plane)
t = n . (R - P) / denom
X = P + t * d
```
**Field**: Q (rational in, rational out)

### Plane ∩ Plane → Line (Prop II.2)

**Input**: Plane 1: normal n1, point R1. Plane 2: normal n2, point R2.
```
d = n1 x n2
if d == (0,0,0): parallel (no intersection or identical)

// find a point on the line
// set the coordinate corresponding to largest |d| component to 0
// solve 2x2 system for the other two

d1 = n1 . R1   (plane 1 constant)
d2 = n2 . R2   (plane 2 constant)

if |d.z| >= |d.x| and |d.z| >= |d.y|:
    // set Z = 0, solve for X, Y
    det = n1.x*n2.y - n1.y*n2.x    (this equals d.z)
    X = (d1*n2.y - d2*n1.y) / det
    Y = (n1.x*d2 - n2.x*d1) / det
    point_on_line = (X, Y, 0)

elif |d.y| >= |d.x|:
    // set Y = 0, solve for X, Z
    det = n1.x*n2.z - n1.z*n2.x    (this equals -d.y)
    X = (d1*n2.z - d2*n1.z) / det
    Z = (n1.x*d2 - n2.x*d1) / det
    point_on_line = (X, 0, Z)

else:
    // set X = 0, solve for Y, Z
    det = n1.y*n2.z - n1.z*n2.y    (this equals d.x)
    Y = (d1*n2.z - d2*n1.z) / det
    Z = (n1.y*d2 - n2.y*d1) / det
    point_on_line = (0, Y, Z)

Result: line with direction d through point_on_line
```
**Field**: Q

### Line ∩ Line → Point (Prop II.3)

**Input**: Line 1 through P1, Q1. Line 2 through P2, Q2. Must be coplanar.
```
d1 = Q1 - P1
d2 = Q2 - P2

// Coplanarity test:
// (P2 - P1) . (d1 x d2) == 0

// Solve P1 + s*d1 = P2 + t*d2 for s (or t)
// Pick two equations from the three components where d1,d2 are not degenerate

// Using x,y components:
det = d1.x*d2.y - d1.y*d2.x
if det != 0:
    dx = P2.x - P1.x
    dy = P2.y - P1.y
    s = (dx*d2.y - dy*d2.x) / det
else: try x,z or y,z pairs

X = P1 + s * d1
```
**Field**: Q

### Skew test for two lines
```
d1 = Q1 - P1
d2 = Q2 - P2
cross = d1 x d2
scalar_triple = (P2 - P1) . cross
if scalar_triple == 0: coplanar (intersecting or parallel)
if scalar_triple != 0: skew (no intersection)
```

---

## Line ∩ Sphere → Points (Book III)

**Input**: Line through P, Q. Sphere centered at C with radius r = |W - C|.
```
d = Q - P
f = P - C

a = d . d
b = 2 * (f . d)
c = f . f - r^2

discriminant = b^2 - 4*a*c

if discriminant < 0: no intersection
if discriminant == 0: one point (tangent)
if discriminant > 0: two points

t = (-b ± sqrt(discriminant)) / (2*a)
X = P + t * d
```
**Field**: Q(√) — introduces square roots

### Discriminant shortcut (integer form)
```
// Use disc_quarter = b_half^2 - a*c instead of full discriminant
// where b_half = f . d (half the b coefficient)
disc_quarter = (f.d)^2 - (d.d)*(f.f - r^2)

// This is an integer when all inputs are integer
// Perfect square → rational solutions
// Not perfect square → irrational (involves √disc_quarter)

t = (-b_half ± sqrt(disc_quarter)) / a
```

### Radical Plane of Two Spheres (Prop III.2)

**Input**: Sphere 1: center C1, passes through P1. Sphere 2: center C2, passes through P2.
```
r1_sq = |P1 - C1|^2
r2_sq = |P2 - C2|^2

// The radical plane equation (quadratic X.X terms cancel!):
normal = 2 * (C2 - C1)
k      = r1_sq - r2_sq - |C1|^2 + |C2|^2

// Plane: normal . X = k
```
**Field**: Q (always rational — the key insight)

**Special case** (equal radii, i.e. S(P,Q) vs S(Q,P)):
```
normal = 2*(Q - P)
d      = |Q|^2 - |P|^2
// This is the perpendicular bisector plane of PQ
```

### Three-Sphere Intersection (Prop III.3)

**Input**: Three spheres S1, S2, S3 with non-collinear centers.
```
1. radical_plane_12 = radical_plane(S1, S2)    // rational
2. radical_plane_13 = radical_plane(S1, S3)    // rational
3. radical_line = plane_plane_intersect(rp12, rp13)  // rational
4. points = line_sphere_intersect(radical_line, S1)   // may be irrational
```
**Result**: 0, 1, or 2 points. Steps 1-3 are rational. Step 4 is quadratic.

### Midpoint Construction (Prop III.5)

**Input**: Two points P and Q.
```
1. Construct S(P,Q) and S(Q,P)
2. Radical plane = perpendicular bisector of PQ
3. Intersect radical plane with L(P,Q)
4. Result: midpoint M = (P + Q) / 2
```
**Field**: Q (radical plane is rational, line-plane intersection is rational)

---

## Key Results

### Tetrahedron Inertness (Theorem II.4)
From 4 points in general position, lines and planes alone produce NO new points.
All opposite edges are skew. All line-face intersections land on existing vertices.

### Bootstrap (Prop III.4)
From {O,A,B,C}, line-sphere intersections produce 12 new INTEGER points:
```
Axis reflections:     (-1,0,0)  (2,0,0)  (0,-1,0)  (0,2,0)  (0,0,-1)  (0,0,2)
Vertex reflections:   (2,-1,0)  (-1,2,0)  (2,0,-1)  (-1,0,2)  (0,2,-1)  (0,-1,2)
```
Total: 16 points after one generation. All integer. Bounding box [-1,2]^3.

### Irrational Scaffolding Principle
Irrational intermediate points (from three-sphere intersection) can define
rational planes, which then produce rational intersection points.
The irrationals are scaffolding that is removed once the plane is built.

### Bootstrap Non-Inertness (Theorem IV.1)
The 16 bootstrap points are NOT inert under T_Q.
One generation of line-plane intersections produces 1,986 new rational points.
```
16 points → 120 lines, 138 planes → 14,688 intersections → 1,986 new points
Denominators: LCD 1..19 (all primes up to 19 appear)
Inside [-1,2]^3: 1,074 points.  Outside: 912 points.
```

### Centroid Construction (Prop IV.2)
```
Given P, Q, R:
1. M_QR = midpoint(Q, R)     // Prop III.5 (spheres)
2. M_PR = midpoint(P, R)     // Prop III.5 (spheres)
3. median_1 = L(P, M_QR)
4. median_2 = L(Q, M_PR)
5. G = median_1 ∩ median_2   // Prop II.3 (line-line)
Result: G = (P + Q + R) / 3
```
Note: centroid (1/3,1/3,0) is NOT in the 1,986 bootstrap points.
It requires midpoints (spheres) as intermediate step.

### Division by n (Prop IV.3 — Thales in 3D)
Divide segment PQ into n equal parts using parallel planes.
Requires: perpendicular bisector plane (Prop III.5) for parallelism.

### All Rationals Constructible (Theorem IV.4)
From {O,A,B,C} using T_E: every point in Q^3 is constructible.
Path: bootstrap → integers → unit fractions (Thales) → all p/q → Q^3.

### Constructible Field (Prop III.6)
3D constructible = 2D constructible = smallest subfield of R containing Q
and closed under √ of positive elements.
A real α is constructible ⟺ [Q(α):Q] = 2^n for some n ≥ 0.
Three dimensions add no new constructible numbers (but new constructible points).

---

## Perpendicular and Angular Constructions (Book V)

### Foot of Perpendicular: Point to Plane (Prop V.1)

**Input**: Point P. Plane with normal n, constant d (n.X = d).
```
t = (n.P - d) / (n.n)
F = P - t * n
```
**Field**: Q (rational in, rational out)

### Foot of Perpendicular: Point to Line (Prop V.2)

**Input**: Point P. Line through Q with direction d.
```
t = (P - Q).d / (d.d)
F = Q + t * d
```
**Field**: Q

### Common Perpendicular of Skew Lines (Prop V.3)

**Input**: L1 through P1 direction d1. L2 through P2 direction d2. Lines must be skew.
```
w = P2 - P1
a = d1.d1,  b = d1.d2,  c = d2.d2
e = w.d1,   f = w.d2
det = a*c - b^2    (= |d1 x d2|^2 by Lagrange identity)

s = (e*c - b*f) / det
t = (b*e - a*f) / det

foot1 = P1 + s * d1
foot2 = P2 + t * d2
dist_sq = |foot2 - foot1|^2
```
**Field**: Q (all quantities rational)
**Note**: Purely 3D — no 2D analogue (no skew lines in the plane)

### Dihedral Angle (Prop V.4)
```
cos^2(theta) = (n1.n2)^2 / (|n1|^2 * |n2|^2)
```
**Field**: Q (cos^2 is always rational; cos may be irrational)

### Right Tetrahedron Metrics (Demo 005)
```
Skew pairs (all 3 are isometric by S3 symmetry):
  L(O,A) ↔ L(B,C):  foot1 = O,  foot2 = (0, 1/2, 1/2),  dist^2 = 1/2
  L(O,B) ↔ L(A,C):  foot1 = O,  foot2 = (1/2, 0, 1/2),  dist^2 = 1/2
  L(O,C) ↔ L(A,B):  foot1 = O,  foot2 = (1/2, 1/2, 0),  dist^2 = 1/2

Dihedral angles:
  3 edges at O (coordinate planes):     cos^2 = 0    → 90°
  3 edges of face ABC (face vs coord):  cos^2 = 1/3  → arccos(1/√3) ≈ 54.74°

Distances:
  dist^2(O, plane x+y+z=1) = 1/3
  dist^2(C, L(A,B)) = 3/2
```

### Rational Metric Principle (Theorem V.5)
All metric computations on rational objects produce rational results at the squared level:
- Perpendicular feet: rational points (constructible by Theorem IV.4)
- Squared distances: rational
- Squared cosines of angles: rational

---

## Reflections and Isometries (Book VI)

### Reflection Across a Plane (Prop VI.1)

**Input**: Point P. Plane with normal n, constant d.
```
R = P - 2 * ((n.P - d) / (n.n)) * n
```
Equivalently: R = 2*F - P where F is the foot (Prop V.1).
**Field**: Q

### Reflection Across a Line (Prop VI.2)

**Input**: Point P. Line through Q with direction d.
```
F = Q + ((P-Q).d / (d.d)) * d    (foot, Prop V.2)
R = 2*F - P
```
**Field**: Q
**Note**: In 3D this is a 180-degree rotation about the line.

### Reflection Through a Point (Prop VI.3)
```
R = 2*C - P
```
**Field**: Q (no division needed)

### Composition of Reflections (Theorem VI.4)
```
Two reflections across intersecting planes = rotation by 2*dihedral angle
Two reflections across parallel planes = translation by 2*distance
Every isometry of R^3 = composition of at most 4 plane reflections
```

### S3 Symmetry of the Right Tetrahedron
```
Generated by transposition planes:
  x = y  (normal (1,-1,0)):  swaps A ↔ B, fixes C, O
  y = z  (normal (0,1,-1)):  swaps B ↔ C, fixes A, O
Composition gives 3-cycle: A → B → C
O is the unique fixed vertex (right-angle vertex)
```

### Orbit of O Under Face-Plane Reflections (Demo 006)
```
Gen 0: 1 point (seed)
Gen 1: 2 total (+1 new)
Gen 2: 5 total (+3 new)
Gen 3: 11 total (+6 new)
Gen 4: 24 total (+13 new)
Denominators: 1, 3, 9 (powers of 3 from n.n = 3 on face plane)
Orbit is infinite — never stabilizes.
```

---

## Regular Polyhedra (Book VII)

### Q(sqrt(5)) Arithmetic
```
phi = (1 + sqrt(5)) / 2
phi^2 = phi + 1
phi * (1/phi) = 1,  where 1/phi = (sqrt(5) - 1) / 2

Representation: a + b*sqrt(5), with a, b in Q
  (a1 + b1*s)(a2 + b2*s) = (a1*a2 + 5*b1*b2) + (a1*b2 + a2*b1)*s
```

### The Rational Solids (Prop VII.1)
```
Regular Tetrahedron:  V = (1,1,1), (1,-1,-1), (-1,1,-1), (-1,-1,1)
  4V, 6E, 4F.  d^2 = 8.  R^2 = 3.  Field: Z^3.

Cube:  V = all 8 sign combos of (1,1,1)
  8V, 12E, 6F.  d^2 = 4.  R^2 = 3.  Field: Z^3.
  Distance catalog: 12 edges (4), 12 face diags (8), 4 space diags (12).

Octahedron:  V = (+-1,0,0), (0,+-1,0), (0,0,+-1)
  6V, 12E, 8F.  d^2 = 2.  R^2 = 1.  Field: Z^3.
  Already present in bootstrap (Prop III.4).
```

### The Golden Solids (Prop VII.2)
```
Icosahedron:  V = cyclic permutations of (0, +-1, +-phi)
  12V, 30E, 20F.  d^2 = 4 (RATIONAL!).  R^2 = 5/2 + sqrt(5)/2.
  Field: Q(sqrt(5))^3.

Dodecahedron:  V = (+-1)^3 union cyclic perms of (0, +-1/phi, +-phi)
  20V, 30E, 12F.  d^2 = 6 - 2*sqrt(5) (IRRATIONAL).  R^2 = 3.
  Field: Q(sqrt(5))^3.
```

### Algebraic Classification (Theorem VII.3)

```
Rational (T_Q): Tetrahedron, Cube, Octahedron — vertices in Z^3
Golden  (T_E):  Icosahedron, Dodecahedron — vertices in Q(sqrt(5))^3

Duality preserves class: Cube <-> Octahedron, Icosahedron <-> Dodecahedron
Tetrahedron is self-dual.

sqrt(5) is unavoidable: A5 representation theory forces it.
```

---

## Volumes and the Cross Product (Book VIII)

### Cross Product (Prop VIII.1)
```
a x b = (a2*b3 - a3*b2, a3*b1 - a1*b3, a1*b2 - a2*b1)

Properties:
  Anti-commutativity: a x b = -(b x a)
  Perpendicularity: (a x b) . a = 0
  Lagrange identity: |a x b|^2 = |a|^2 * |b|^2 - (a.b)^2
  Parallelism: a x b = 0 iff a || b
```
**Field**: Q (rational vectors in, rational vector out)

### Area of Triangle (Prop VIII.2)
```
4 * area^2 = |(Q - P) x (R - P)|^2
```
**Field**: area^2 is always rational. area may be irrational.

### Scalar Triple Product / Volume (Prop VIII.3)
```
a . (b x c) = det(a, b, c)   (signed parallelepiped volume)

Volume of tetrahedron PQRS = |det(Q-P, R-P, S-P)| / 6
```
**Field**: Q. Volume itself (not just volume^2) is rational!

### Volumes of Platonic Solids (Demo 008)
```
Right tetrahedron {O,A,B,C}:  V = 1/6,   V^2 = 1/36
Regular tetrahedron:           V = 8/3,   V^2 = 64/9
Octahedron:                    V = 4/3,   V^2 = 16/9
Cube:                          V = 8,     V^2 = 64

Ratios (to right tetrahedron):
  Regular tet = 16x,  Octahedron = 8x,  Cube = 48x
  Cube = 3 regular tets = 6 octahedra
```

### Face Areas (Demo 008)
```
Right tetrahedron faces:
  OAB, OAC, OBC: area^2 = 1/4 (area = 1/2)
  ABC:            area^2 = 3/4 (area = sqrt(3)/2)

Regular tetrahedron faces: area^2 = 12 (area = 2*sqrt(3))
Cube faces:                area^2 = 16 (area = 4)
Octahedron faces:          area^2 = 3/4 (area = sqrt(3)/2)
```

### Three-Level Rational Metric (Theorem VIII.4)
```
Level 0: perpendicular feet      — always rational
Level 1: distance^2              — always rational
Level 2: area^2                  — always rational
Level 3: volume (not just vol^2) — always rational

Volume is the most rational: it is a determinant (polynomial),
no square root needed. The straightedge alone can compute it.
```

---

## Centers of the Tetrahedron (Book IX)

### Orthocentric Property (Prop IX.1)
A tetrahedron is **orthocentric** if all three pairs of opposite edges are perpendicular.
```
Test: e_ij . e_kl = 0 for all 3 opposite-edge pairs
```
The right tetrahedron {O,A,B,C} is orthocentric:
```
OA . BC = (1,0,0) . (0,-1,1) = 0
OB . AC = (0,1,0) . (-1,0,1) = 0
OC . AB = (0,0,1) . (-1,1,0) = 0
```

### Orthocenter H (Prop IX.1)
Intersection of the four altitudes. For the right tetrahedron:
```
Altitude from A to face OBC (plane x=0): foot = O
Altitude from B to face OAC (plane y=0): foot = O
Altitude from C to face OAB (plane z=0): foot = O
Altitude from O to face ABC (plane x+y+z=1): foot = (1/3,1/3,1/3)

All altitudes pass through O → H = (0, 0, 0)
```
**Key insight**: The right-angle vertex IS the orthocenter.
**Field**: Q

### Circumcenter K (Prop IX.2)
Equidistant from all vertices. Intersection of perpendicular bisector planes.
```
|K-O|^2 = |K-A|^2 → 2*Kx = 1 → Kx = 1/2
|K-O|^2 = |K-B|^2 → 2*Ky = 1 → Ky = 1/2
|K-O|^2 = |K-C|^2 → 2*Kz = 1 → Kz = 1/2

K = (1/2, 1/2, 1/2)
Circumradius^2 = 3/4
```
**Field**: Q (perpendicular bisector planes from Book III)

### Centroid G (Prop IX.3)
Average of vertices. Divides each median in ratio 3:1 from vertex.
```
G = (O + A + B + C) / 4 = (1/4, 1/4, 1/4)
```
**Field**: Q (only addition and division by 4)

### Incenter I (Prop IX.4)
Weighted average of vertices by opposite face areas.
```
Face areas:
  OBC, OAC, OAB: area = 1/2 each (coordinate planes)
  ABC:            area = sqrt(3)/2  (face plane x+y+z=1)

Total surface S = (3 + sqrt(3))/2

I = (area(ABC)*O + area(OBC)*A + area(OAC)*B + area(OAB)*C) / S
  = (1/2, 1/2, 1/2) / ((3+sqrt(3))/2)
  = (1, 1, 1) / (3 + sqrt(3))
  = ((3-sqrt(3))/6) * (1, 1, 1)    [rationalize denominator]

Inradius r = 3V/S = (1/2) / ((3+sqrt(3))/2) = (3-sqrt(3))/6
```
**Field**: Q(√3) — the **only** irrational center. Requires T_E (spheres).

### Euler Line (Theorem IX.5)
All four centers are collinear on x = y = z (the S₃ symmetry axis):
```
Center        Parameter t     Rational?
H  (0,0,0)       0            YES (T_Q)
I  (...)         (3-√3)/6      NO  (T_E)
G  (1/4,...)     1/4           YES (T_Q)
K  (1/2,...)     1/2           YES (T_Q)

Order: H(0) < I(~0.211) < G(0.25) < K(0.5)
```

### Key Relations (Demo 009)
```
G = midpoint(H, K)       ← ratio HG:GK = 1:1 (NOT 2:1 as in triangles)
|H-G|^2 = |G-K|^2 = 3/16
|H-K|^2 = 3/4 = circumradius^2 (since H is a vertex!)
|H-K|^2 = 4 * |H-G|^2   (doubling property)
```

### Regular Tetrahedron (Corollary IX.5a)
```
Vertices: (1,1,1), (1,-1,-1), (-1,1,-1), (-1,-1,1)
All opposite edges perpendicular (orthocentric).
All vertices at |v|^2 = 3 from origin.
H = G = K = I = (0, 0, 0)
```
All four centers coincide — the 3D analogue of the equilateral triangle.

### Rational Classification
```
3 rational centers: H, G, K — constructible by T_Q (lines and planes)
1 irrational center: I — requires T_E (spheres for sqrt(3))

The irrationality comes from face areas (cross product magnitudes),
which involve square roots. The incenter weights by area, not area^2,
so the square root propagates to the final coordinates.
```

---

## Transformations and Constructible Motions (Book X)

### Translation (Prop X.1)
```
T(P) = P + v,  v in Q^3
```
**Field**: Q. Construction: two parallel plane reflections (Thm VI.4).
Preserves all distances (isometry).

### Scaling from Center (Prop X.2)
```
S(P) = C + k * (P - C),  k in Q, C in Q^3

|S(P) - S(Q)|^2 = k^2 * |P - Q|^2
```
**Field**: Q. Construction: Thales (Book IV). Centroid is fixed point.

### Rotation about z-axis (Prop X.3)
```
General:  R(x,y,z) = (x*cos - y*sin, x*sin + y*cos, z)
90 deg:   R(x,y,z) = (-y, x, z)          [cos=0, sin=1]
180 deg:  R(x,y,z) = (-x, -y, z)         [cos=-1, sin=0]
```
Construction: two plane reflections at angle theta/2 (Thm VI.4).

### 120 degree Rotation about (1,1,1) (Prop X.3)
```
R(x,y,z) = (z, x, y)     [cyclic permutation]
R^3 = identity (order 3)
Maps: A -> B -> C -> A,  O fixed
```
**Field**: Q (coordinate permutation, no arithmetic needed).

### Pythagorean Rotations (Prop X.4)
```
A rotation about a coordinate axis has a RATIONAL matrix
iff its angle is Pythagorean: sin AND cos both rational.

(cos, sin) = (a/c, b/c) where a^2 + b^2 = c^2

Small triples:
  (3,4,5):   cos=4/5,  sin=3/5   ~ 36.87 deg
  (5,12,13): cos=12/13, sin=5/13  ~ 22.62 deg
  (8,15,17): cos=15/17, sin=8/17  ~ 28.07 deg
  (7,24,25): cos=24/25, sin=7/25  ~ 16.26 deg
```
Pythagorean angles are dense (parametrize via t -> ((1-t^2)/(1+t^2), 2t/(1+t^2)))
but countable.

### Plane Reflection (from Book VI)
```
R(P) = P - 2 * ((n.P - d) / (n.n)) * n
```
Two reflections at angle alpha = rotation by 2*alpha about their common line.

### Transformation Hierarchy (Theorem X.5)
```
T_Q transformations (rational matrix):
  - Translations by rational vectors
  - Scalings by rational factors
  - Rotations by Pythagorean angles (0, 90, 180, 270, + triples)
  - 120 deg about (1,1,1) (coordinate permutation)
  All map Q^3 -> Q^3.

T_E transformations (constructible, may be irrational):
  - Rotations by constructible angles (cos in constructible field)
  - Example: 60 deg about z (cos=1/2, sin=sqrt(3)/2, field Q(sqrt3))
  May map Q^3 -> constructible irrational points.

Non-constructible:
  - Rotation by 1 radian (transcendental)
  - Rotation by 20 deg (requires angle trisection)
```

### Composition Rules (Demo 010)
```
Composition of isometries = isometry (preserves distances).
Composition of T_Q operations = T_Q.
Every isometry = at most 4 plane reflections (Thm VI.4).
```

---

## Projections and Shadows (Book XI)

### Projection onto a Plane (Prop XI.1)
```
Proj(P) = P - ((n.P - d) / (n.n)) * n
```
**Field**: Q. Same as foot of perpendicular (Prop V.1).
Idempotent: Proj(Proj(P)) = Proj(P).

### Projection onto a Line (Prop XI.2)
```
Proj(P) = ((P.d) / (d.d)) * d     (line through origin, direction d)
```
**Field**: Q. Same as foot of perpendicular (Prop V.2).

### Shadows of the Right Tetrahedron (Prop XI.3, Demo 011)
```
Coordinate plane shadows (each collapses one vertex onto O):
  z=0: O,C collapse → shadow = face OAB (right triangle, edges 1,1,2)
  y=0: O,B collapse → shadow = face OAC
  x=0: O,A collapse → shadow = face OBC

Face-plane shadow (x+y+z=1):
  O → (1/3, 1/3, 1/3) = centroid of ABC
  A, B, C stay fixed (already on plane)
  Triangle ABC is EQUILATERAL: all edges d^2 = 2
  O' equidistant from A,B,C: d^2 = 2/3

S3-axis shadow (line x=y=z):
  O → (0, 0, 0)
  A, B, C all collapse → (1/3, 1/3, 1/3)
  Shadow = line segment, d^2 = 1/3
```

### Complementary Projections (Theorem XI.4)
```
Proj_line(P, d) + Proj_plane(P, d, 0) = P

Pythagorean: |P|^2 = |P_parallel|^2 + |P_perp|^2

For A = (1,0,0) relative to axis (1,1,1):
  A_parallel = (1/3, 1/3, 1/3),   |A_par|^2 = 1/3
  A_perp     = (2/3, -1/3, -1/3), |A_perp|^2 = 2/3
  1/3 + 2/3 = 1 = |A|^2
```

### Projected Area Formula
```
projected area = original area * |cos(angle between normals)|
projected area^2 = original area^2 * cos^2(angle)

For face ABC onto z=0:
  area^2(ABC) = 3/4
  cos^2(angle) = 1/3   (dihedral from Book V)
  shadow area^2 = 3/4 * 1/3 = 1/4
```

### De Gua's Theorem (Theorem XI.5) — 3D Pythagorean Theorem
```
For a right-angled tetrahedron (right angle at O):
  area^2(ABC) = area^2(OAB) + area^2(OAC) + area^2(OBC)
  3/4         = 1/4         + 1/4         + 1/4

The coordinate shadows of the hypotenuse face ARE the right-angle faces.
Three partial views reconstruct the whole in quadrature.
```

### Transformation Classification (Books X + XI)
```
Invertible, distance-preserving:  reflections, rotations, translations
Invertible, distance-scaling:     dilations
Non-invertible, contracting:      projections

All three types preserve rationality of coordinates.
```

---

## Barycentric Coordinates (Book XII)

### Definition
```
P = l0*V0 + l1*V1 + l2*V2 + l3*V3,  l0 + l1 + l2 + l3 = 1

For right tet {O,A,B,C} (O at origin):
  l1 = x,  l2 = y,  l3 = z,  l0 = 1 - x - y - z
```

### Special Points
```
Point           Cartesian        Barycentric (l0, l1, l2, l3)
O (orthocenter) (0,0,0)          (1, 0, 0, 0)
A               (1,0,0)          (0, 1, 0, 0)
Centroid G      (1/4,1/4,1/4)    (1/4, 1/4, 1/4, 1/4)
Circumcenter K  (1/2,1/2,1/2)    (-1/2, 1/2, 1/2, 1/2)
centroid(ABC)   (1/3,1/3,1/3)    (0, 1/3, 1/3, 1/3)
midpoint(A,B)   (1/2,1/2,0)      (0, 1/2, 1/2, 0)
```

### Containment Test (Prop XII.1)
```
Inside:   all l_j > 0
Boundary: all l_j >= 0, at least one = 0
Outside:  any l_j < 0

Boundary classification by number of zeros:
  3 zeros: vertex
  2 zeros: edge interior
  1 zero:  face interior
  0 zeros: strict interior
```

### Affine Combinations (Prop XII.2)
```
Midpoint:     average of 2 barycentric coord vectors
Face centroid: average of 3
Tet centroid:  average of 4
```

### Face Distances (Prop XII.3)
```
dist(P, face_j) = l_j * h_j   where h_j = altitude from vertex j

For right tet:
  dist^2(P, OBC) = l1^2        (h_1^2 = 1)
  dist^2(P, OAC) = l2^2        (h_2^2 = 1)
  dist^2(P, OAB) = l3^2        (h_3^2 = 1)
  dist^2(P, ABC) = l0^2 / 3    (h_0^2 = 1/3)

Example: dist^2(G, OBC) = 1/16,  dist^2(G, ABC) = 1/48
```

### Volume Ratios (Theorem XII.4)
```
l_j = V(P, face_j) / V_total

For G: V(G,A,B,C) = 1/24 = (1/4)(1/6) = l0 * V_total
For K: V(K,A,B,C) = -1/12 = (-1/2)(1/6) = l0 * V_total (negative = outside)

4 sub-volumes from G: 4 * 1/24 = 1/6 = V_total
```

---

## Inversion in a Sphere (Book XIII)

### Inversion Map (Def XIII.1)
```
Inv(P) = P / |P|^2

In coordinates: Inv(x, y, z) = (x/r^2, y/r^2, z/r^2), r^2 = x^2 + y^2 + z^2
```
**Field**: Q (rational in, rational out — division by rational |P|^2)

### Fixed Points (Prop XIII.1)
```
Inv(P) = P  iff  |P|^2 = 1  (P on unit sphere)

Right tetrahedron: A, B, C are fixed. O is the center (excluded).
```

### Interior/Exterior Exchange (Prop XIII.2)
```
|P|^2 < 1  →  |Inv(P)|^2 > 1  (inside → outside)
|P|^2 > 1  →  |Inv(P)|^2 < 1  (outside → inside)

Product property: |P|^2 * |Inv(P)|^2 = 1 always
```

### Involution (Prop XIII.3)
```
Inv(Inv(P)) = P  for all P ≠ O

Inversion is its own inverse — a nonlinear involution.
```

### Plane ↔ Sphere Correspondence (Theorem XIII.4)
```
Plane n.X = d (d ≠ 0) inverts to sphere n.Q = d * |Q|^2 through O.

For plane ABC (x + y + z = 1):
  Image: x^2 + y^2 + z^2 = x + y + z
  Completing square: |X - (1/2,1/2,1/2)|^2 = 3/4
  This is the circumsphere! (center K, radius^2 = 3/4)
```

### Named Inversions (Demo 013)
```
Point           |P|^2    Inv(P)            |Inv(P)|^2
A (1,0,0)       1        (1,0,0)           1          (fixed)
G (1/4,1/4,1/4) 3/16     (4/3,4/3,4/3)     16/3
K (1/2,1/2,1/2) 3/4      (2/3,2/3,2/3)     4/3
M_OA (1/2,0,0)  1/4      (2,0,0)           4
(1,1,1)         3        (1/3,1/3,1/3)     1/3

Notable: Inv(1,1,1) = centroid(ABC), Inv(centroid(ABC)) = (1,1,1)
```

### Classification of Image Objects
```
Plane not through O  →  sphere through O
Sphere through O     →  plane not through O
Plane through O      →  plane through O (self)
Sphere not through O →  sphere not through O
```

---

## Stereographic Projection (Book XIV)

### Definition (Def XIV.1)
```
Stereographic projection from O (on circumsphere) onto face plane x+y+z=1:

  Stereo(Q) = Q / (qx + qy + qz)

Derived from: line X = tQ meets plane x+y+z=1 at t = 1/(qx+qy+qz).
```
**Field**: Q (rational in, rational out)

### The Circumsphere Identity (Theorem XIV.1)
```
For Q on circumsphere |Q-K|^2 = 3/4:

  |Q|^2 = qx + qy + qz

Proof: expand |Q-K|^2 = |Q|^2 - (qx+qy+qz) + 3/4 = 3/4.
```

### Stereographic = Inversion (Theorem XIV.2)
```
For Q on circumsphere, Q != O:

  Stereo(Q) = Q/(qx+qy+qz) = Q/|Q|^2 = Inv(Q)

The two denominators are equal by the circumsphere identity.
The inverse map (face plane -> circumsphere) is also inversion.
```

### Named Projections (Demo 014)
```
Circumsphere point    Stereo image           Name
A = (1,0,0)           (1,0,0)                vertex (fixed)
B = (0,1,0)           (0,1,0)                vertex (fixed)
C = (0,0,1)           (0,0,1)                vertex (fixed)
(1,1,0)               (1/2, 1/2, 0)          midpoint(A,B)
(1,0,1)               (1/2, 0, 1/2)          midpoint(A,C)
(0,1,1)               (0, 1/2, 1/2)          midpoint(B,C)
(1,1,1)               (1/3, 1/3, 1/3)        centroid(ABC)
(3/5, 6/5, 0)         (1/3, 2/3, 0)          on edge AB
```

### Circles Through O → Lines (Prop XIV.3)
```
Coordinate plane circles on circumsphere project to edges of ABC:

  z=0 circle (through O, A, B)  →  edge AB  (z=0, x+y=1)
  y=0 circle (through O, A, C)  →  edge AC  (y=0, x+z=1)
  x=0 circle (through O, B, C)  →  edge BC  (x=0, y+z=1)

General: any circle through the pole O projects to a line.
Circles not through O project to circles.
```

---

## Power of a Point (Book XV)

### Definition (Def XV.1)
```
pow(P, S(C,r)) = |P - C|^2 - r^2

Sign: = 0 on sphere, < 0 inside, > 0 outside
```
**Field**: Q (always rational for rational inputs)

### Power Tables (Demo 015)
```
Unit sphere (center O, r^2=1):
  A: 0       G: -13/16    K: -1/4    (1,1,1): 2    M_OA: -3/4

Circumsphere (center K, r^2=3/4):
  O: 0       A: 0         G: -9/16   (1,1,1): 0    (2,0,0): 2

S(A,O) (center A, r^2=1):
  O: 0       A: -1        B: 1       G: -5/16      K: -1/4
```

### Radical Plane = Equal-Power Locus (Theorem XV.1)
```
Radical plane of S1 and S2: pow_1(P) = pow_2(P)
Quadratic |P|^2 terms cancel → always a plane.

Unit sphere & circumsphere:
  |P|^2 - 1 = |P-K|^2 - 3/4  →  x + y + z = 1  (face ABC!)

Unit sphere & S(A,O):
  |P|^2 - 1 = |P-A|^2 - 1  →  x = 1/2  (perp bisector of OA)
```

### Power-Inversion Identity (Theorem XV.2)
```
pow(P) = -|P|^2 * pow(Inv(P))

Consequences:
  - pow = 0  →  pow(Inv) = 0  (sphere fixed under inversion)
  - pow < 0  →  pow(Inv) > 0  (interior/exterior exchange)
```

### Unification
```
Book III:      radical plane = equal-power locus
Book IX:       circumsphere = zero-power surface
Book XIII:     inversion flips power
Books XIII-XIV: face plane = radical plane of unit sphere & circumsphere
```

---

## Poles and Polars (Book XVI)

### Polar Plane (Def XVI.1)
```
Polar of P (unit sphere): { X : P . X = 1 }
  Normal = P, constant = 1
  Distance from O to polar = 1/|P|

Pole of plane n.X = d:  P = n/d
```
**Field**: Q

### Named Polars (Demo 016)
```
Point            Polar plane        Meaning
A = (1,0,0)      x = 1              tangent at A
B = (0,1,0)      y = 1              tangent at B
C = (0,0,1)      z = 1              tangent at C
(1,1,1)          x+y+z = 1          face plane ABC
K = (1/2,1/2,1/2) x+y+z = 2
G = (1/4,1/4,1/4) x+y+z = 4
(2,0,0)          x = 1/2            radical plane (Book XV)
```

### Reciprocity (Prop XVI.1)
```
P on polar(Q) iff Q on polar(P)
(Because P.Q = Q.P)
```

### Polar Foot Theorem (Theorem XVI.2)
```
Inv(P) = foot of perpendicular from O to polar(P)

Proof: foot = (d/|n|^2) * n = (1/|P|^2) * P = Inv(P)

Inversion IS the perpendicular projection onto the polar plane.
```

### Conjugate Pairs (Prop XVI.3)
```
P and Q conjugate iff P . Q = 1
Self-conjugate (P.P = 1) iff P on sphere

Examples: K ↔ (2,0,0),  G ↔ (4,0,0),  A ↔ A (self)
```

### Face Plane: Four Descriptions
```
x + y + z = 1 is simultaneously:
  - Image of circumsphere under inversion    (Book XIII)
  - Target of stereographic projection       (Book XIV)
  - Radical plane of unit sphere & circumsph  (Book XV)
  - Polar plane of (1,1,1)                   (Book XVI)
```

---

## Cross-Ratio and Harmonic Division (Book XVII)

### Cross-Ratio (Def XVII.1)
```
(A,B;C,D) = ((tC - tA)(tD - tB)) / ((tC - tB)(tD - tA))

Swap reciprocal: (A,B;D,C) = 1/(A,B;C,D)
Identity:        (A,B;C,D) + (A,C;B,D) = 1
```
**Field**: Q (rational in, rational out)

### Harmonic Division (Theorem XVII.2)
```
On any line through O meeting the unit sphere at X, Y:

  (X, Y; P, Inv(P)) = -1   for all P on the line

P and Inv(P) divide the sphere chord harmonically.
```

### Harmonic Division Table (x-axis, X=(-1,0,0), Y=(1,0,0))
```
P             Inv(P)          (X,Y;P,Inv(P))
(2,0,0)       (1/2,0,0)       -1
(3,0,0)       (1/3,0,0)       -1
(1/2,0,0)     (2,0,0)         -1
(-2,0,0)      (-1/2,0,0)      -1
```

### Polar Conjugate (Prop XVII.3)
```
Polar of P meets line OP at Inv(P) = harmonic conjugate.
Unifies:
  - Thm XVI.2: Inv(P) = foot of perp from O to polar(P)
  - Thm XVII.2: Inv(P) = harmonic conjugate of P
```

### CR Preserved by Inversion (Theorem XVII.4)
```
For 4 points on a line through O:
  (Inv(A), Inv(B); Inv(C), Inv(D)) = (A, B; C, D)

Proof: parameters t_i -> 1/(t_i |d|^2), ratios cancel.

X-axis {2,3,5,-1}: CR = 2  ->  Inv {1/2,1/3,1/5,-1}: CR = 2
Y-axis {1/2,2,4,-3}: CR = 5/2  ->  Inv: CR = 5/2
```

### Face Plane: Five Descriptions
```
x + y + z = 1 is simultaneously:
  - Image of circumsphere under inversion    (Book XIII)
  - Target of stereographic projection       (Book XIV)
  - Radical plane of unit sphere & circumsph  (Book XV)
  - Polar plane of (1,1,1)                   (Book XVI)
  - Harmonic structure of chord determines    (Book XVII)
```

---

## Pencils of Spheres (Book XVIII)

### Pencil Definition (Def XVIII.1)
```
Pencil through S1 and S2: (1-t)*f1 + t*f2 = 0

Unit sphere & circumsphere:
  center(t) = (t/2, t/2, t/2)
  r^2(t) = 3t^2/4 + 1 - t
```
**Field**: Q (rational parameter, rational center and radius)

### Named Pencil Members (Demo 018)
```
t       center              r^2     name
0       (0,0,0)             1       unit sphere
1/2     (1/4,1/4,1/4)       11/16   centered at G
2/3     (1/3,1/3,1/3)       2/3     centered at centroid(ABC)
1       (1/2,1/2,1/2)       3/4     circumsphere
2       (1,1,1)             2       centered at (1,1,1)

All centers on Euler line x=y=z (Book IX connection).
```

### Common Circle (Prop XVIII.1)
```
All pencil members pass through the circle:
  plane: x+y+z=1 (radical plane)
  center: (1/3,1/3,1/3)  (centroid of face ABC)
  radius^2: 2/3

Verification: A, B, C lie on every pencil member.
```

### Constant Power (Theorem XVIII.2)
```
For P on radical plane (px+py+pz = 1):
  pow(P, S_t) = |P|^2 - 1   (independent of t)

centroid_ABC: pow = -2/3   (inside all members)
mid(A,B):    pow = -1/2    (inside all members)
A:           pow = 0       (on the common circle)
```

### Orthogonality (Theorem XVIII.4)
```
|C1-C2|^2 = r1^2 + r2^2  iff spheres are orthogonal.

Orthogonal to both generators => orthogonal to ALL members.
Centers of orthogonal spheres lie on the radical plane.

Example: S3 = (center (2,0,-1), r^2=4)
  Note: 2+0+(-1) = 1 (center on radical plane)
  Orthogonal to t=0: |C3|^2 = 5 = 4+1  ✓
  Orthogonal to t=1: 19/4 = 4+3/4      ✓
  Orthogonal to t=1/2: 75/16 = 4+11/16 ✓
  Orthogonal to t=2: 6 = 4+2           ✓
```

---

## Inversive Distance (Book XIX)

### Inversive Product (Def XIX.1)
```
delta(S1, S2) = |C1 - C2|^2 - r1^2 - r2^2

Sign:  delta > 0: external    delta = 0: orthogonal    delta < 0: overlapping
```
**Field**: Q

### Squared Inversive Distance (Def XIX.1)
```
d^2(S1, S2) = delta^2 / (4 * r1^2 * r2^2)
```
**Field**: Q (d itself may be irrational; d^2 always rational)

### Classification (Prop XIX.1)
```
d^2 = 0:           orthogonal (recovers Book XVIII condition)
d^2 = 1:           tangent (external if delta > 0, internal if delta < 0)
d^2 > 1:           separated (no intersection)
0 < d^2 < 1:       intersecting at angle theta, cos^2(theta) = d^2
```

### Sphere Inversion Formula (Prop XIX.2)
```
pow = |C|^2 - r^2  (power of O w.r.t. sphere S)
C' = C / pow
r'^2 = r^2 / pow^2

Special: pow = 1 (orthogonal to unit sphere) => S' = S (fixed)
```

### Verification Table (Demo 019)
```
Sphere                      pow    Image center       Image r^2
SA = ((2,0,0), r^2=1)      3      (2/3, 0, 0)        1/9
SB = ((0,3,0), r^2=1)      8      (0, 3/8, 0)        1/64
SC = ((3,0,0), r^2=4)      5      (3/5, 0, 0)        4/25
SD = ((2,0,-1), r^2=4)     1      (2, 0, -1)         4  (fixed)
```

### d^2 Preserved by Inversion (Theorem XIX.3)
```
d^2(Inv(S1), Inv(S2)) = d^2(S1, S2)

Proof: delta' = delta / (pow1 * pow2)
       4*r1'^2*r2'^2 = 4*r1^2*r2^2 / (pow1^2 * pow2^2)
       d'^2 = delta'^2 / (4*r1'^2*r2'^2) = delta^2 / (4*r1^2*r2^2) = d^2

Corollaries:
  - Orthogonal (d^2=0) stays orthogonal
  - Tangent (d^2=1) stays tangent
  - Separation magnitude preserved exactly
```

### Tangent Chain (Prop XIX.4)
```
U, SA, SC are pairwise tangent (d^2 = 1 for all 3 pairs).
Common tangent point: (1, 0, 0).
  U-SA:  delta = 2  (external)
  U-SC:  delta = 4  (external)
  SA-SC: delta = -4 (internal, SA inside SC)
```

### Invariant Hierarchy
```
Object            Invariant          Preserved by
4 collinear pts   cross-ratio (XVII) projective maps
point & sphere    power (XV)         Euclidean isometries
2 spheres         d^2 (XIX)          inversion (Mobius group)
```

### Face Plane: Six Descriptions
```
x + y + z = 1 is simultaneously:
  1. Image of circumsphere under inversion          (Book XIII)
  2. Target of stereographic projection              (Book XIV)
  3. Radical plane of unit sphere & circumsphere     (Book XV)
  4. Polar plane of (1,1,1)                          (Book XVI)
  5. Harmonic structure: chord determines polar      (Book XVII)
  6. Locus of orthogonal sphere centers (d^2=0)      (Books XVIII-XIX)
```

---

## Conformal Property of Inversion (Book XX)

### Angle Between Two Spheres (Def XX.1)
```
At intersection point P:
  cos^2(theta) = ((P-C1).(P-C2))^2 / (r1^2 * r2^2)
```
**Field**: Q (always rational)

### Angle Between Sphere and Plane (Def XX.1)
```
Sphere S = (C, r), plane with normal n, at point P:
  cos^2(theta) = ((P-C).n)^2 / (r^2 * |n|^2)
```

### Conformal Theorem (Theorem XX.1)
```
For S1, S2 meeting at P:
  cos^2(theta') at Inv(P) = cos^2(theta) at P

Verified with three angles: 1/2, 1/3, 2/3 — all preserved.
```

### Plane Inversion (Prop XX.2)
```
Plane n.X = d (d != 0) inverts to sphere:
  center = n/(2d),  r^2 = |n|^2/(4d^2)

Sphere-plane angle becomes sphere-sphere angle, still preserved.
  x+y=2  ->  ((1/4,1/4,0), r^2=1/8)    cos^2 = 1/2 preserved
  x+y+z=2 -> ((1/4,1/4,1/4), r^2=3/16) cos^2 = 1/3 preserved
```

### Planes Through O (Prop XX.3)
```
Planes through O are fixed by inversion.
Sphere-plane angle preserved directly (sphere changes, plane stays).
  S3 vs y=0 at (2,0,0): cos^2 = 1/3  ->  at (1/2,0,0): cos^2 = 1/3
```

### The Angle-Distance Identity (Theorem XX.4)
```
For intersecting spheres:
  cos^2(theta) = d^2(S1, S2)

Proof:
  delta = |C1-C2|^2 - r1^2 - r2^2 = -2(C1-P).(C2-P)
  d^2 = delta^2/(4*r1^2*r2^2) = ((C1-P).(C2-P))^2/(r1^2*r2^2) = cos^2(theta)

Consequence: Thm XIX.3 (d^2 preserved) = Thm XX.1 (conformal).
Same theorem, two languages.
```

### Invariant Hierarchy (Complete)
```
Conformal property (angles preserved) — the root
  └─ d^2 preserved (XIX) — conformal for sphere pairs
      ├─ tangency preserved (d^2=1)
      ├─ orthogonality preserved (d^2=0, XVIII)
      └─ cross-ratio preserved (XVII) — conformal for collinear points
```

---

## The Descartes Circle Theorem (Book XXI)

### Descartes Formula (Theorem XXI.1)
```
For 4 mutually tangent circles with curvatures k_i = 1/r_i:
  (k1 + k2 + k3 + k4)^2 = 2(k1^2 + k2^2 + k3^2 + k4^2)

Given k1, k2, k3:
  k4 = k1 + k2 + k3 +/- 2*sqrt(k1*k2 + k2*k3 + k3*k1)
```

### Rational Condition (Prop XXI.2)
```
Solutions are rational iff discriminant D = k1k2+k2k3+k3k1
is a perfect square.

(2, 3, 6) configuration:
  D = 6+18+12 = 36 = 6^2  (perfect square!)
  k4 = 11 +/- 12 = 23 (inner) or -1 (outer)
```

### The (2, 3, 6) Configuration (Demo 021)
```
Circle   Center              r      k     r^2
S1       (0, 0, 0)           1/2    2     1/4
S2       (5/6, 0, 0)         1/3    3     1/9
S3       (8/15, 2/5, 0)      1/6    6     1/36
S_in     (117/230, 22/115,0) 1/23   23    1/529
S_out    (3/10, -2/5, 0)     1      -1    1

Descartes checks:
  (2+3+6+23)^2 = 34^2 = 1156 = 2(4+9+36+529) = 2*578  ✓
  (2+3+6-1)^2  = 10^2 = 100  = 2(4+9+36+1)   = 2*50   ✓
```

### Tangency Type (Theorem XXI.5)
```
External tangency: delta = +2*r1*r2,  |C1-C2| = r1+r2
Internal tangency: delta = -2*r1*r2,  |C1-C2| = |r1-r2|
Both:              d^2 = 1

Pair          delta    2r1r2   Type
S1, S2        +1/3     1/3     external
S_in, S3      +1/69    1/69    external
S1, S_out     -1       1       internal

d^2(S_in, S_out) = 49 (not mutually tangent)
```

---

## Soddy Spheres (Book XXII)

### 3D Descartes Formula (Theorem XXII.1)
```
For 5 mutually tangent spheres in R^3 with curvatures k_i = 1/r_i:
  (k1 + k2 + k3 + k4 + k5)^2 = 3(k1^2 + k2^2 + k3^2 + k4^2 + k5^2)

Given k1,...,k4, the quadratic for k5:
  2*k5^2 - 2*S4*k5 + (3*Q4 - S4^2) = 0
  where S4 = sum(k_i), Q4 = sum(k_i^2)

Discriminant: D = 12*S4^2 - 24*Q4

General dimension: in R^n, (n+2) tangent spheres satisfy
  (sum k)^2 = n(sum k^2)
Factor 2 for circles (n=2), factor 3 for spheres (n=3).
```
**Field**: Q (when discriminant is a perfect square)

### Coplanarity Theorem (Theorem XXII.2)
```
If the base satisfies 2D Descartes: S4^2 = 2*Q4
then D = 12*(S4^2 - 2*Q4) = 0.

Double root => unique Soddy curvature: k5 = S4/2.
```

### Soddy Sphere Centers (Prop XXII.3)
```
z = sqrt(3) * r (irrational), z^2 = 3*r^2 (rational).

For z=0 sphere with center P:
  |S - P|^2 = (x-px)^2 + (y-py)^2 + z^2  (rational)

For two Soddy spheres (same side, z > 0):
  za*zb = 3*ra*rb  (rational, since z = sqrt(3)*r)
  |SA - SB|^2 = (xa-xb)^2 + (ya-yb)^2 + z2a + z2b - 6*ra*rb
```

### The (2, 3, 6) Soddy Configuration (Demo 022)
```
Sphere   Center                   r      k     z^2
SA       (87/170, 1/5, sqrt(3)/17) 1/17   17    3/289
SB       (27/50, 7/25, sqrt(3)/5)  1/5    5     3/25

Derived from:
  (2,3,6,23) => S4=34, k5=34/2=17
  (2,3,6,-1)  => S4=10, k5=10/2=5

SA-SB: |SA-SB|^2 = 484/7225 = (22/85)^2
        delta = 2/85 > 0 (external tangency)
        d^2 = 1
```

### Three Quintets (Theorem XXII.6)
```
Quintet          sum    sum_sq   (sum)^2 = 3(sum_sq)
(2,3,6,23,17)   51     867      2601 = 2601  ✓
(2,3,6,-1,5)    15     75       225  = 225   ✓
(2,3,6,17,5)    33     363      1089 = 1089  ✓
```

### Tangency Table (Demo 022)
```
SA tangencies (all external):
  Pair          delta    d^2   Type
  SA, S1        1/17     1     external
  SA, S2        2/51     1     external
  SA, S3        1/51     1     external
  SA, S_in      2/391    1     external

SB tangencies:
  Pair          delta    d^2   Type
  SB, S1        1/5      1     external
  SB, S2        2/15     1     external
  SB, S3        1/15     1     external
  SB, S_out     -2/5     1     internal

SA-SB:
  delta = 2/85, d^2 = 1, external
```

---

## Initial Configuration
```
O = (0, 0, 0)
A = (1, 0, 0)
B = (0, 1, 0)
C = (0, 0, 1)
```

### Derived Objects (from initial 4)
```
6 lines:   L(O,A) L(O,B) L(O,C) L(A,B) L(A,C) L(B,C)
4 planes:  z=0    y=0    x=0    x+y+z=1
12 directed spheres (7 distinct):
  S(O,*)  — unit sphere at O (all 3 give same sphere)
  S(A,O)  — unit sphere at A
  S(B,O)  — unit sphere at B
  S(C,O)  — unit sphere at C
  S(A,B)=S(A,C) — √2-sphere at A
  S(B,A)=S(B,C) — √2-sphere at B
  S(C,A)=S(C,B) — √2-sphere at C
```
