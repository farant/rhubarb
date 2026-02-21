## Demo 67: Coordinate System Zoo — SU(2) Projections for DKC

- **Status**: COMPLETE
- **File**: `demo_67_coordinate_zoo/main.c` (~1776 lines)
- **Tests**: 18/18 pass
- **Depends on**: Demo 66 (quaternionic bracket catalog, 24-cell Voronoi, 35 solutions)
- **Feeds into**: Demo 68, Demo 69, Demo 70, Demo 71

### Headline
Systematic test of every natural SU(2) coordinate system as a DKC activation for XOR6 proves that the computation lives on S² (rotation axis direction): a custom 13-direction Voronoi achieves 36 solutions at only 14 cells, while the Hopf phase is completely computationally inert at every resolution tested.

### Key Results
- **Part A — Eigenvector extraction**: The 24 quaternions from the binary octahedral group decompose into 4 eigenvalue angles (0°, 45°, 60°, 90°), 13 eigenvector directions (mod ±), and 17 (angle, direction) pairs. Axis-aligned directions are shared across two eigenvalue angles; body-diagonals appear only at 60°; edge-midpoints only at 90°. Trace sanity check confirms trace(SU(2)) = 2·cos(θ) for all 24 quaternions.
- **Part B — S² is the computational sweet spot**: Custom 13-direction Voronoi on S² achieves 36 solutions at 14 cells (2.57 sol/cell) — more solutions with fewer cells than any other activation. Platonic solid Voronois (octahedral=6, icosahedral=12, cuboctahedral=12 cells) all yield zero solutions: natural polytopal geometry fails; data-derived directions are required.
- **Part C — Eigenvalue-only activation**: First solutions appear at k=8 (4 solutions). Peak around k=32 (80 solutions). At k=24: 72 solutions, more than the 24-cell Voronoi's 35, using only 1D eigenvalue information.
- **Part D — Hopf phase is computationally inert**: Phase-only activation yields ZERO solutions at every resolution tested (k=4 through k=32). All DKC information lives in the S² Hopf base; the S¹ fiber carries nothing. This is the deepest theoretical result of the demo.
- **Part E — Cayley-Klein is worst**: The native SU(2) parameterization (|α|, arg(α)) requires 32 cells for first solutions (8 solutions) — worse than every other 2D system. Native does not mean natural-for-computation.
- **Part F — High-resolution rankings at 64 cells**: Hopf base 8×8 = 292 solutions (4.56 sol/cell); Eigenvector LatLon 8×8 = 204 solutions (3.19 sol/cell); Cayley-Klein 8×8 = 106 solutions (1.66 sol/cell). At high resolution, Hopf base dominates because it carves up the computationally relevant S² more naturally than lat/lon on eigenvector S².
- **Progression across demos**: D65 complex S¹ needs k=24 sectors (25 cells); D66 quaternionic S³ needs 25 cells; D67 eigenvector S² needs only 14 cells. The 2D projection is the most efficient.

### Theorems/Conjectures
- **Result (Hopf phase inertness)**: The Hopf fiber phase ξ₁ carries zero DKC computational information at all resolutions (k=4 through k=32, exhaustive). The computation is entirely determined by the S² Hopf base. PROVEN (exhaustive).
- **Result (S² efficiency)**: A 14-cell custom Voronoi on S² achieves more solutions (36) than the full 25-cell S³ Voronoi (35). Lower-dimensional projection outperforms the ambient space. DEMONSTRATED.
- **Result (eigenvalue paradox resolution)**: The eigenvalue angle θ = arccos(a) depends on the real quaternion component and succeeds as an activation; the Hopf phase ξ₁ = atan2(b, a) fails completely. These are distinct quantities: θ extracts information from the combination of base and fiber, not the fiber alone. PROVEN.
- **Conjecture (14-cell minimum)**: The natural 13+1 eigenvector Voronoi may be the true minimum cell count on S² for XOR6. Platonic geometries at 6 and 12 cells fail. Whether a different 13-direction subset could work with fewer cells is open. CONJECTURED.

### Data
- Eigenvalue angles: 4 (0°, 45°, 60°, 90°)
- Eigenvector directions (mod ±): 13 (3 axis + 6 edge-midpoint + 4 body-diagonal)
- (angle, direction) pairs: 17; quaternions (mod ±): 24; bracket values: 100
- Custom 13-dir S² Voronoi: 36 solutions at 14 cells (2.57 sol/cell)
- Hopf base activation: 30@16 cells, 130@36 cells, 292@64 cells (8×8)
- Hopf phase-only: 0 solutions at all resolutions (k=4,8,12,16,24,32)
- Eigenvalue k-sector: 0@k=4,6; 4@k=8; 64@k=12; 76@k=16; 72@k=24; 80@k=32
- Cayley-Klein: 0@8 cells, 0@24 cells, 8@32 cells, 26@36 cells, 106@64 cells
- Computational hierarchy: S² primary (14 cells sufficient), θ secondary, Hopf phase inert, full S³ redundant

### Code Assets
- **`eigenvector_extract()`**: decomposes SU(2) quaternion into eigenvector direction (mod ±) and eigenvalue angle
- **Custom Voronoi on S²**: assigns quaternion to nearest of 13 data-derived directions; handles identity/zero as degenerate cell 0
- **Platonic solid Voronois**: octahedral, icosahedral, cuboctahedral cell partitions on S² (all fail for XOR6)
- **Hopf coordinate decomposition**: converts quaternion to Hopf (η, ξ₁, ξ₂); separates base and fiber activations
- **Cayley-Klein parameterization**: (|α|, arg(α)) grid activation
- **Eigenvalue k-sector activation**: partitions [0, π] into k equal sectors on eigenvalue angle θ
- **Comparative activation harness**: unified XOR6 DKC search usable across all coordinate systems

### Literature Touched
- Hopf (1931): original Hopf fibration S³ → S² with fiber S¹; this demo confirms the fibration structure is respected by DKC computation
- Aizenberg (2008): k-sector MVN activation on S¹; the present work extends and contrasts the S¹ approach with S² and S³ activations
- Conway & Smith (2003): quaternion groups and the binary octahedral group; the 24 quaternions and their eigenvector decomposition
- Bloch (1946): Bloch sphere S² = CP¹; S² as qubit state space raises the question of a DKC/quantum information connection

### Open Questions
- **Why 36 vs 35?** The 13-dir S² Voronoi finds 36 solutions vs the 24-cell S³ Voronoi's 35. Is the extra solution a genuinely new tuple or a boundary effect from different cell geometry?
- **Hopf base vs eigenvector geometry**: At high resolution, Hopf base (292) beats eigenvector lat/lon (204). Does the Hopf base projection align better with the computation? What is the geometric relationship between Hopf base S² and eigenvector S²?
- **Below-14 minimum on S²**: Platonic solids at 6 and 12 cells fail. Is 13+1=14 the true minimum, or could a non-catalog 13-direction subset work with fewer cells?
- **XOR7 on S²**: Demo 65 showed XOR7 needs k=127 sectors on S¹. Does S² reduce this as dramatically as it did for XOR6 (24→14)?
- **Bloch sphere connection**: The rotation axis lives on S² = CP¹ = the Bloch sphere of a qubit. Is there a direct connection between DKC computation on the Bloch sphere and quantum information theory?
