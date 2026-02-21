# Demo 68: Stereographic DKC — Knot Computation in the Plane

## Summary

Projected the eigenvector S² onto R² via stereographic projection. The DKC
computation becomes a drawable 2D planar picture with visible decision
boundaries. The key discovery: you can **visualize** on the plane but must
**compute** on the sphere. Euclidean nearest-neighbor in R² gives zero
solutions; S² great-circle metric gives the full 36. Stereographic projection
is conformal (angle-preserving) but area-distorting (16.5× ratio), and the
S² Voronoi structure is perfectly preserved when using the correct metric.

**Test count: 9/9 pass (Parts A–F)**

---

## 1. Stereographic Projection Setup (Part A)

The 13 eigenvector directions on S² from Demo 67 are projected onto R² via
stereographic projection. Key detail: the south pole (0,0,−1) IS one of the
13 catalog directions, so we switch to **north pole** projection to avoid
sending a Voronoi center to infinity.

| Property | Value |
|----------|-------|
| Projection pole | North (0,0,1) |
| South pole in catalog | Yes (direction 1) |
| Round-trip precision | ~1e-15 (machine epsilon) |
| Bounding box | x∈[−2.41, 1.37], y∈[−1.00, 2.41] |
| Spread | dx=3.78, dy=3.41 |

All 13 directions project to finite R² points. The projection spreads them
asymmetrically — body-diagonal directions (±1/√3 components) near the north
pole get pushed to large radius (~2.41), while those near the equator stay
moderate (~0.37–1.37).

## 2. 36/36 Perfect Match with S² Voronoi (Part C)

**Projected Voronoi: 36 solutions at 14 cells — exact match to Demo 67.**

When using the S² great-circle metric (dot product) for Voronoi cell
assignment, the stereographic projection preserves the solution count
exactly. Every one of the 36 winners from Demo 67's custom 13-direction
S² Voronoi appears identically in the projected plane.

All 36 winners show: **7 zero, 7 one, 0 unused** cell labeling.

## 3. Euclidean Metric Gives Zero Solutions — Intrinsically Curved (Part C)

The most important negative result: using Euclidean distance in R² for
nearest-neighbor cell assignment gives **ZERO solutions**.

Root cause: stereographic projection distorts areas by a factor of up to
16.5×. Points that are equidistant on S² (in great-circle distance) are
NOT equidistant in R² (in Euclidean distance). The Voronoi diagram in R²
has different cells than the S² Voronoi — large cells near the projection
pole consume neighbors that should belong to smaller cells.

**The computation is intrinsically curved.** You cannot flatten it without
losing structure. The DKC lives on S², not on R².

## 4. ASCII Visualization: DKC as a Picture (Part D)

The centerpiece of the demo. Each of the 36 winning solutions renders as a
65×33 ASCII art image showing:

- **0/1** regions: the two classes of the XOR function, visible as
  contiguous domains on the projected sphere
- **+** markers: Voronoi cell boundaries (where adjacent pixels belong to
  different cells)
- **\*** markers: the 6 weight positions (3 quaternions + 3 antipodals)
- **0-9, A-C**: the 13 eigenvector Voronoi centers

The decision boundary between 0 and 1 regions forms a visible curve in
the plane — knot computation is literally a picture. The boundary structure
shows a complex interlocking pattern: 7 cells labeled 0 and 7 cells labeled
1, forming two connected domains with a fractal-like boundary.

## 5. Perceptron Formulation (Part E)

Each DKC solution has an explicit neural network interpretation:

```
Architecture: 6 bits → 2 neurons → Voronoi activation → 1 bit

Input layer:   b = (b₁, b₂, b₃, b₄, b₅, b₆) ∈ {0,1}⁶
Hidden layer:  (x,y) = Σᵢ bᵢ · (xᵢ, yᵢ)   [linear combination]
Activation:    cell = Voronoi(x,y)            [13-direction S² metric]
Output:        XOR6 = label[cell]             [lookup table]
```

The 6×2 weight matrix is derived directly from the stereographic projections
of the weight quaternions' eigenvector directions. Example (Winner 0):

| Input | x | y |
|-------|---|---|
| b₁ | +1.000 | +0.000 |
| b₂ | +1.000 | +0.000 |
| b₃ | +0.000 | +0.000 |
| b₄ | +0.000 | +0.000 |
| b₅ | −0.000 | −1.000 |
| b₆ | −0.000 | −1.000 |

These weights come from knot theory, not gradient descent.

## 6. Conformality: 16.5× Area Distortion (Part F)

Stereographic projection is conformal (angle-preserving) but NOT area-
preserving. Monte Carlo sampling of the projected Voronoi cells shows:

| Cell | Area (R²) | % of grid |
|------|-----------|-----------|
| Largest (cell 8) | 4.33 | 17% |
| Smallest (cell 1) | 0.26 | 1% |
| **Ratio** | **16.5×** | |

Despite this massive area distortion:
- Solution count: 36 planar = 36 S² (difference = 0)
- Cell assignments: identical when using S² metric
- Angles: preserved (conformal guarantee)

The area distortion is visible in the ASCII art: cells near the projection
pole are visually much larger than cells near the equator.

## 7. The Lesson: Visualize on the Plane, Compute on the Sphere

| Metric | Solutions | Correct? |
|--------|-----------|----------|
| S² great-circle (dot product) | 36 | Yes |
| R² Euclidean | 0 | No |

The stereographic projection gives a faithful **visualization** of the DKC
computation but is NOT a valid computational domain. The Voronoi structure
on S² encodes the computation; the planar image is a window into it.

This is consistent with Demo 67's finding that the computation lives on S²:
the sphere is not just a convenient coordinate choice, it's the intrinsic
geometry of the problem.

## 8. Planar Grid Results (Part B)

Rectangular grids on R² as a simpler alternative to Voronoi:

| Grid | Cells | Solutions |
|------|-------|-----------|
| 4×4 | 16 | 0 |
| 6×6 | 36 | 4 |
| 8×8 | 64 | 33 |
| 4×8 | 32 | 2 |
| 8×4 | 32 | 7 |

The planar grid at 8×8=64 cells gets 33 solutions — close to the Voronoi's
36 at only 14 cells. The grid wastes cells on empty regions of the plane.

## Cross-Demo Connection

| Demo | What | Key Result |
|------|------|------------|
| 65 | Complex S¹ | k=24 needed (25 cells) |
| 66 | Quaternionic S³ | 24-cell Voronoi (25 cells) |
| 67 | Eigenvector S² | **14 cells** (sweet spot) |
| **68** | **Stereographic R²** | **Visualization preserves structure; intrinsically curved** |

The progression: S¹ → S³ → S² → R² reveals that dimensionality reduction
from S³ to S² gains efficiency, but the final step from S² to R² is
visualization only — you cannot reduce the computation further.

## Compile & Run

```
cc -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion \
   -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings \
   main.c -lm -o demo68
./demo68
```
