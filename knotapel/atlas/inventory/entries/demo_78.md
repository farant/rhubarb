## Demo 78: Recursive Scaling + Phase Diagram

- **Status**: COMPLETE
- **File**: `demo_78_recursive_scaling/main.c` (~1052 lines)
- **Tests**: 9/9 pass
- **Depends on**: Demo 66 (24-quaternion catalog, 24-cell geometry), Demo 67 (13 directions, 36 XOR6 solutions), Demo 77 (XOR8 = 6 solutions via Sec8×Vor activation, shadow structure)
- **Feeds into**: Demo 79 (zeta_12 capacity, breaking the wall), Demo 80 (group finiteness survey)

### Headline
XOR10 = 0 winners at ζ₈, confirmed by both recursive search (120 candidates) and exhaustive brute-force (all 42,504 quintuples); the ζ₈ XOR hierarchy terminates at 8 inputs, with non-monotonic accuracy in sector count echoing Demo 50's pi/4 incommensurability.

### Key Results
- **XOR10 = 0 (exhaustive)**: All C(24,5) = 42,504 quintuples tested at k=8, 10, 12, 16. Zero winners at any sector count. Best accuracy 98.4% (1008/1024 masks) at k=10 and k=16.
- **Non-monotonicity**: k=12 accuracy (96.9%) is worse than k=10 (98.4%). The pi/4 lattice incommensurability creates destructive interference at certain k values, mirroring Demo 50's finding (906 solutions at k=6 > 756 at k=7 > 96 at k=8).
- **XOR12 skipped**: No XOR10 parents to extend, so recursive XOR12 search was vacuously skipped.
- **Recursive structure confirmed (Part E)**: All 6 XOR8 winners contain exactly 2 XOR6 sub-triples (100% verified). Each shadow entry shares its Voronoi cell with at least one parent member (12/12 = 100%). All shadows have eigenvalue half-angle = 45 degrees. Shadow pairs are {0,1}, {4,5}, {19,21} — each pair shares direction and half-angle, differs only in sign/orientation.
- **Phase diagram**: XOR6 (14 cells, 36 winners, Voronoi) → XOR8 (112 cells, 6 winners, Sec8×Vor) → XOR10 (WALL, 0 winners). Capacity/masks ratios: XOR6 = 4.6, XOR8 = 2.3.
- **Catalog exhaustion diagnosis**: The ζ₈ catalog has only 13 distinct eigenvector directions and 2 half-angles (45°, 90°). Selecting 5 entries from 24 exhausts structural diversity — too many entries share properties for 1024-mask separation.

### Theorems/Conjectures
- **ζ₈ Wall Theorem (CONFIRMED)**: The binary octahedral group (24 elements) generates a fixed catalog whose combinatorial diversity is exhausted at 5-element selection. XOR8 is the maximum computable XOR function at ζ₈.
- **Recursive Shadow Conjecture (CONFIRMED for XOR6→XOR8)**: Every XOR(N+2) winner = XOR(N) winner + one "shadow" quaternion that shares the same eigenvector direction but provides additional eigenvalue resolution. All 6 XOR8 winners decompose this way.
- **Non-Monotonicity Conjecture (SUPPORTED)**: The pi/4 incommensurability of the ζ₈ lattice creates sector boundaries that produce destructive interference at specific k values, causing accuracy to decrease as k increases in some ranges.

### Data
- XOR6: 36 winners, 14 cells minimum, Voronoi-only activation, 1.779% hit rate
- XOR8: 6 winners, 112 cells minimum (k=8), Sec8×Vor activation, 0.057% hit rate
- XOR10: 0 winners, 42,504 quintuples exhaustively tested
- Best XOR10 accuracy: 98.4% = 1008/1024 masks at k=10 and k=16
- Non-monotonic dip: k=12 achieves only 96.9% (worse than k=10)
- Shadow cell-sharing: 12/12 (100%) of XOR8 shadows share Voronoi cell with a parent
- All 6 XOR8 shadows have half-angle exactly 45 degrees
- Catalog: 24 quaternions, 13 directions, 2 half-angles (45°, 90°)

### Code Assets
- **`test_combined(indices, n_idx, k_sec, acc_out)`**: Generic XOR tester using Combined Sec(k)×Vor activation; pre-computes all 2^N sums once and tests multiple k values; handles up to 4096 masks and 512 cells
- **Brute-force XOR10 harness (Part B)**: Computes all 1024 sums once per quintuple, caches angle and Voronoi cell, then sweeps k values cheaply — avoids redundant sum computation across multiple k tests
- **`part_e_recursive()`**: Verifies parent-shadow structure for any XOR(N) → XOR(N-2) decomposition; reports cell sharing, half-angle, and paired quaternion structure for each shadow
- **Sorted tuple insertion**: Pattern for maintaining sorted index arrays when extending N-tuples to (N+1)-tuples without full re-sort (find insertion position, shift)
- **Deduplication with lookup**: is_win6/is_win8/is_win10 linear-scan lookups prevent redundant testing when multiple parent winners produce the same extended tuple

### Literature Touched
- **Aizenberg (2008)**: Combined Sec(k)×Voronoi activation generalizes the k-sector MVN; the wall at XOR10 sets a concrete capacity limit for the ζ₈ instantiation
- **Nazer & Gastpar (2011)**: The information-theoretic framing of catalog exhaustion — fixed algebraic structure provides fixed diversity, determining the compute-and-forward capacity ceiling
- **Demo 50 connection**: Non-monotonicity in sector count previously observed (906@k=6 > 756@k=7 > 96@k=8 for XOR4 parity); the pi/4 incommensurability pattern is confirmed as a recurring structural feature

### Open Questions
- **16 misclassified masks at XOR10**: The 98.4% best accuracy leaves 16 masks wrong. Do these form a structured pattern (same Hamming weight? same bit-pattern class?)
- **Information-theoretic lower bound**: Is there a clean formula relating max computable N to catalog size |C|, direction count, and angle count?
- **What does ζ_16 buy?**: Higher roots produce larger catalogs with more directions and angles. Is there a scaling law max_N = f(|catalog|, n_directions, n_angles)?
- **Is the wall at 8 specific to the octahedral group?**: Other finite groups (binary tetrahedral at ζ₄, binary icosahedral in principle) should have their own walls at different N values.
