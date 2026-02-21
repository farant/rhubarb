## Demo 68: Stereographic DKC — Knot Computation in the Plane

- **Status**: COMPLETE
- **File**: `demo_68_stereographic_dkc/main.c` (~1171 lines)
- **Tests**: 9/9 pass
- **Depends on**: Demo 67 (13-direction S² Voronoi, 36 solutions, eigenvector decomposition)
- **Feeds into**: Demo 69, Demo 70, Demo 71

### Headline
Stereographic projection of the eigenvector S² onto R² preserves all 36 DKC solutions exactly when using the S² great-circle metric, but gives zero solutions with Euclidean distance, proving the computation is intrinsically curved and cannot be flattened — the plane is a window, not a workspace.

### Key Results
- **S² Voronoi preserved under projection (Part C)**: Using the S² great-circle metric (dot product) for Voronoi cell assignment in R² gives 36 solutions at 14 cells — identical to Demo 67. All 36 winners show 7 zero / 7 one cell labeling, exact match.
- **Euclidean metric gives zero solutions (Part C)**: Switching to R² Euclidean distance for nearest-neighbor assignment collapses solutions to zero. Area distortion from stereographic projection (up to 16.5×) breaks cell boundaries; large cells near the projection pole consume neighbors that belong to smaller cells. The computation is intrinsically curved.
- **ASCII visualization: DKC as a drawable picture (Part D)**: Each of the 36 winning solutions renders as a 65×33 ASCII image. Regions of 0s and 1s form contiguous visible domains; `+` marks Voronoi boundaries; `*` marks the 6 weight positions; `0–9, A–C` labels the 13 Voronoi centers. The decision boundary is a visible curve in the plane.
- **Perceptron formulation (Part E)**: Architecture is 6 bits → 2 neurons (linear combination of projected weight coordinates) → Voronoi activation (S² metric) → 1 bit. The 6×2 weight matrix comes from stereographic projections of weight quaternion eigenvector directions, not gradient descent.
- **Conformality: 16.5× area distortion, zero solution difference (Part F)**: Monte Carlo cell area measurement shows largest cell (cell 8) is 17% of the bounding grid, smallest (cell 1) is 1%, ratio 16.5×. Despite this distortion, solution count is unchanged and cell assignments are identical under S² metric.
- **Planar grids waste cells (Part B)**: Rectangular grids in R² require 64 cells to reach 33 solutions (8×8), versus 14 cells for 36 solutions with S² Voronoi. Grids cover empty regions of the bounding box; the Voronoi covers only the data.
- **North pole projection chosen (Part A)**: South pole (0,0,−1) is one of the 13 catalog directions, so north pole projection is used to keep all 13 centers at finite R² coordinates. Round-trip precision ~1e-15 (machine epsilon). Bounding box: x∈[−2.41, 1.37], y∈[−1.00, 2.41].

### Theorems/Conjectures
- **Result (intrinsic curvature)**: DKC computation on the eigenvector S² cannot be faithfully transferred to R² by stereographic projection when using Euclidean nearest-neighbor. Euclidean metric yields zero solutions; S² great-circle metric yields 36. The computation is intrinsically curved. PROVEN (exhaustive over all 36 solutions).
- **Result (conformality sufficiency)**: Stereographic projection is angle-preserving (conformal) but not area-preserving. The 16.5× area ratio between largest and smallest cells does not affect solution count or correctness when the S² metric is used. Angles are sufficient; areas are irrelevant to the cell assignment problem. DEMONSTRATED.
- **Result (planarity as visualization)**: The stereographic image of the S² Voronoi is a valid 2D picture of the computation but not a valid computational domain. "Visualize on the plane, compute on the sphere." DEMONSTRATED.
- **Conjecture (perceptron knot weights)**: The 6×2 weight matrices derived from stereographic projection of DKC solutions constitute a family of two-neuron perceptrons solving XOR6 without any training. Whether these weights can be found by gradient descent from random initialization is open.

### Data
- Tests: 9/9 pass (Parts A–F)
- Round-trip projection precision: ~1e-15
- Bounding box: x∈[−2.41, 1.37], y∈[−1.00, 2.41]; spread dx=3.78, dy=3.41
- S² Voronoi in R² (S² metric): 36 solutions, 14 cells — exact match to Demo 67
- R² Euclidean Voronoi: 0 solutions
- Area distortion: largest cell 4.33 (17%), smallest cell 0.26 (1%), ratio 16.5×
- ASCII visualization: 65×33 per solution, 36 images total
- Planar grid results: 4×4=0, 6×6=4, 8×8=33, 4×8=2, 8×4=7
- Progression: D65 S¹ (25 cells) → D66 S³ (25 cells) → D67 S² (14 cells) → D68 R² (visualization only)

### Code Assets
- **Stereographic projection / inverse**: north-pole projection of S² to R² and back; round-trip precision verified at machine epsilon
- **Projected Voronoi with S² metric**: assigns R² query point to nearest of 13 projected centers using back-projected dot product; handles pole-projection degeneracy
- **Euclidean Voronoi in R²**: same structure but using Euclidean distance; preserved for the negative-result comparison
- **ASCII renderer**: 65×33 grid over bounding box, renders 0/1 regions, Voronoi boundaries (`+`), weight positions (`*`), and center labels (`0–9, A–C`)
- **Perceptron weight extractor**: builds 6×2 matrix from stereographic projections of winning solution weight quaternions
- **Monte Carlo area estimator**: samples Voronoi cell areas in R² for conformality analysis

### Literature Touched
- Riemann (1851): stereographic projection as a conformal map; the area-distortion / angle-preservation tradeoff is classical complex analysis
- Hopf (1931): the S² Hopf base as the computationally relevant space (established in D67); D68 confirms this by showing R² is a view into S², not a replacement
- Aizenberg (2008): MVN / k-sector activation on S¹ — D68 closes the loop by showing the planar picture of DKC is still not an S¹ activation; curvature is essential
- Bloch (1946): Bloch sphere S² = CP¹; the ASCII visualization is literally a stereographic picture of a Bloch sphere computation

### Open Questions
- **Gradient descent to these weights?** The perceptron weight matrices are derived from knot theory. Can gradient descent find them from random init, and if so, does it rediscover the S² structure implicitly?
- **Other conformal maps?** Stereographic projection is one conformal map S² → R²; Mercator and others exist. Do different conformal charts give better or worse ASCII visualization of the decision boundary?
- **XOR7 picture?** Demo 65 showed XOR7 needs k=127 on S¹. If S² reduces XOR6 to 14 cells, what does the stereographic picture of XOR7 on S² look like — and how many cells?
- **Minimal bounding box tiling?** The 8×8 planar grid wastes cells on empty corners. A tighter irregular planar partition covering only the convex hull of the 13 projected centers might approach the 14-cell Voronoi count. How close can it get?
- **Three-neuron perceptron?** The current formulation uses 2 hidden neurons (x, y coordinates). Adding a third (e.g., radius from origin) might enable purely Euclidean nearest-neighbor in R³, recovering the S² metric implicitly. Does this work?
