## Demo 76: Scaling DKC to 8 Inputs

- **Status**: COMPLETE
- **File**: `demo_76_scaling_8input/main.c` (~959 lines)
- **Tests**: 10/10 pass
- **Depends on**: Demo 50 (parity at k=6 sectors, all 13 NPN classes), Demo 66 (quaternionic DKC, 24-cell catalog), Demo 67 (13 directions, 36 XOR6 solutions), Demo 69 (Clifford staircase, ζ_16 trivialization), Demo 71 (spectral DKC, bandwidth l=6), Demo 74 (computational invariant, 6 types), Demo 75 (binocular DKC, √n magnitudes)
- **Feeds into**: Demo 77 (activation zoo — corrects this demo's conclusion)

### Headline
XOR8 has zero solutions at ζ_8 under S² Voronoi activation: the 14-cell partition lacks the resolution to separate 256 masks, and the algebraic structure of 4-term sums breaks down from 7 discrete angles to 86 near-continuous angles.

### Key Results
- **Total failure (Part A)**: 0 of 10,626 possible weight quadruples (C(24,4)) compute XOR8 with S² Voronoi activation. Best achievable accuracy: 50.0% (random chance). Hit rate: 0.00% vs. 1.78% for 6-input.
- **Capacity argument (Part C)**: At 6 inputs, 64 masks / 14 cells ≈ 4.6 masks per cell — sparse enough for parity-pure cells. At 8 inputs, 256 masks / 14 cells ≈ 18.3 masks per cell — parity-pure cells are geometrically impossible by pigeonhole.
- **Conflict distribution (Part B)**: Minimum conflict cells for any quadruple is 2 (achieved by 11 quadruples, 0.1%). Most quadruples (93%) have 5 or more conflict cells. Even the best 2-conflict quadruple achieves only 50.0% accuracy — the conflicting cells each contain balanced even/odd masks.
- **Direction concentration (Part C)**: Best quadruple [0,1,2,3] sends 81.3% of 256 masks to a single cell. 4-term sums cluster more tightly than 3-term sums, reducing directional diversity vs. 6-input winners which spread across 8–10 cells.
- **Algebraic structure breakdown (Parts D-E)**: 3-term sums give 7 discrete eigenvalue angles (algebraic, clean). 4-term sums give 86 eigenvalue angles (near-continuous, values like 7.861°, 9.736°). The √n magnitude quantization also collapses: 6 values for 3-term sums vs. 28 values for 4-term sums including non-algebraic values like 0.4142, 0.7654.
- **Scaling path (Part E)**: From Demo 69, ζ_16 has 3,457 directions. At ζ_16, 256 masks / 3,458 cells ≈ 0.07 masks per cell — trivially easy. The open question is whether a root of unity with ~56 directions provides the non-trivial sweet spot for 8-input DKC.
- **Bandwidth constraint**: Demo 71 showed XOR6 requires spherical harmonic bandwidth l=6 with 2l+1=13 directions. XOR8 would need higher bandwidth and more directions. The required bandwidth constrains which roots of unity can support which input sizes.

### Theorems/Conjectures
- **ζ_8 S²-capacity limit (DEMONSTRATED)**: The 14-cell Voronoi partition of S² at ζ_8 is a hard geometric ceiling for 6-input computation. Exhaustive search over all C(24,4)=10,626 quadruples confirms 0 solutions.
- **Algebraic discreteness requires 3-term sums (OBSERVED)**: The 7-angle quantization is specific to 3-term sums from the 24-cell. At 4-term sums the algebraic constraints weaken and angles explore most of the rotation space (86 values). CONJECTURED: k-term sums produce O(k²) distinct angles, with discrete structure only at k=3 for this lattice.
- **Sweet-spot formula (CONJECTURED)**: The non-trivial DKC sweet spot occurs when masks/cells ≈ 4.6. For n inputs (2^n masks) this requires cells ≈ 2^n / 4.6. For n=8: cells ≈ 56. Is there a root of unity with ~56 eigenvector directions?
- **NOTE — CORRECTED BY DEMO 77**: The conclusion that "ζ_8 is a 6-input structure" is activation-specific. With S¹ × S² product activation (112 cells), XOR8 IS solvable at ζ_8. Demo 76's failure is a failure of S²-only activation, not of the root of unity.

### Data
- XOR8 solutions (S² Voronoi): 0 / 10,626 quadruples
- XOR6 solutions (S² Voronoi, baseline): 36 / 2,024 triples (1.78%)
- Minimum conflict cells achieved: 2 (11 quadruples, 0.1% of total)
- Best quadruple accuracy: 50.0% (128/256 masks correct)
- Best quadruple [0,1,2,3]: 81.3% of masks in single cell (cell 0), 18.8% in identity cell (13)
- 3-term sum eigenvalue angles: 7 (discrete, algebraic)
- 4-term sum eigenvalue angles: 86 (near-continuous)
- 3-term sum |S| values: 6 (√{0,1,2,3,4,6})
- 4-term sum |S| values: 28 (including non-algebraic like 0.4142, 0.7654)
- ζ_16 direction count: 3,457 (from Demo 69); masks/cell at ζ_16 for XOR8: ~0.07

### Code Assets
- **Exhaustive quadruple search**: C(24,4)=10,626 quadruple enumeration with conflict-cell counting; reusable for n-input DKC search at any k
- **`conflict_count()`**: counts Voronoi cells containing both even and odd parity masks for a given weight set
- **`best_accuracy()`**: majority-vote accuracy per cell for non-zero-conflict quadruples
- **Direction concentration metric**: counts cells used and fraction of masks per dominant cell; characterizes collapse vs. spread of sum-quaternion directions
- **Eigenvalue angle histogram**: bins sum-quaternion angles to discover algebraic structure (or lack thereof) at different input counts

### Literature Touched
- **Demo 69 (Clifford Staircase)**: ζ_8 ⊂ ζ_16 nesting and trivialization rates; the scaling path requires higher roots
- **Demo 71 (Spectral DKC)**: Bandwidth l=6 requires 2l+1=13 directions; XOR8 bandwidth would require more directions and constrains feasible roots
- **Pigeonhole / information-theoretic capacity**: The 256/14 masks-per-cell argument is an elementary counting bound that establishes the hard geometric limit

### Open Questions
- Is there a root of unity with approximately 56 eigenvector directions that would provide the sweet spot for 8-input DKC at S²?
- Does the masks-per-cell ≈ 4.6 sweet spot hold across different input sizes and roots of unity, or is it a coincidence of ζ_8 at 6 inputs?
- Can the algebraic structure breakdown (7 → 86 angles) be explained as a function of the number of summands relative to the lattice symmetry group?
- What is the minimum root of unity that supports XOR8 without trivializing (as ζ_16 trivializes XOR6)?
- (Now answered by Demo 77): The S² wall was activation-specific. Does the same correction apply to XOR10, XOR12 — are those also solvable at ζ_8 with sufficiently fine product activation?
