## Demo 75: Binocular DKC (Multi-Output)

- **Status**: COMPLETE
- **File**: `demo_75_binocular_dkc/main.c` (~1495 lines)
- **Tests**: 23/23 pass
- **Depends on**: Demo 66 (quaternionic DKC, 24-cell catalog), Demo 67 (13 directions, 36 XOR6 solutions), Demo 70 (music space, geometric vs computational independence), Demo 71 (spectral DKC, bandwidth l=6), Demo 72 (spherical design), Demo 73 (eigenvector automaton, additive compass without odometer), Demo 74 (computational invariant, bracket vs cell incomparability)
- **Feeds into**: Demo 76 (scaling to 8 inputs), Demo 77 (activation zoo)

### Headline
A single braid evaluation yields two simultaneous output channels — eigenvalue angle and eigenvector cell — where the angle subsumes the cell entirely, giving 2.3× capacity over cell-only while the multiplicative product channel has maximum entropy but is completely blind to XOR6.

### Key Results
- **Angle subsumes cell (Parts B-D)**: The eigenvalue angle perfectly predicts parity for ALL 36 winning triples. Every angle value maps to exactly one parity bit. The cell channel is a coarsening of the angle channel; knowing the angle tells you the cell parity but not vice versa.
- **Seven eigenvalue angles**: Sum quaternions from 36 winning triples produce exactly 7 distinct eigenvalue angles: 0°, 30°, 35.264° (tetrahedral), 45°, 60°, 65.905°, 90°. Three of these (30°, 35.264°, 65.905°) are non-canonical — they arise from vector addition breaking the 24-cell group structure.
- **Mutual information never zero**: MI(angle; cell) ranges 0.14–1.0 bits, mean 0.7218 bits. 20/36 triples achieve MI = 1.0 (angle perfectly determines cell). No triple has independent channels.
- **Capacity (Part F)**: Cell entropy = 1.000 bit, angle entropy = 2.004 bits, joint = 2.282 bits. Gain over cell-only: +1.282 bits (2.3×). Efficiency 76% of the 3.004-bit additive bound (24% lost to inter-channel MI).
- **Eight triple families (Part E)**: 36 winning triples partition into exactly 8 groups based on their angle function, containing 3–8 triples each. The cell function is identical across all groups; the angle function distinguishes them.
- **√n magnitude quantization (Part G)**: The additive sum magnitude |S| takes exactly 6 values: √0, √1, √2, √3, √4, √6. The value √5 is absent, reflecting the inner product structure of the 24-cell (pairwise inner products drawn from {-1, -½, 0, ½, 1}). H(|S|) = 1.839 bits. Magnitude does NOT reliably predict parity (only 16/36 triples at 44%).
- **Multiplicative channel blind to XOR6 (Part H)**: The quaternion product M = Π w_i produces only 4 eigenvalue angles (the canonical set: 0°, 45°, 60°, 90°), uses all 13 Voronoi cells, has 4.241 bits total entropy — but 0/36 triples compute XOR6 from either channel. Multiplication preserves the 24-cell group structure; DKC requires breaking it.
- **Reconciliation with Demo 74**: Demo 74 showed bracket and cell are incomparable at the individual-quaternion level; Demo 75 shows angle subsumes cell at the sum-quaternion level. These concern different mathematical objects (individual vs. summed quaternions).

### Theorems/Conjectures
- **Angle Subsumption (DEMONSTRATED)**: For all 36 XOR6 winning triples, the eigenvalue angle of the sum quaternion perfectly predicts parity. The Voronoi cell assignment carries no information beyond what the angle already encodes at the computation level.
- **DKC Requires Group-Breaking (OBSERVED)**: The 3 non-canonical angles (30°, 35.264°, 65.905°) arise only through quaternion addition, which breaks the 24-cell group closure. The multiplicative channel, which stays inside the group (only 4 canonical angles), has zero XOR6 solutions. Conjecture: computation requires the non-canonical angles that addition creates.
- **√n Quantization (PROVEN)**: |S|² = Σ w_i · w_j where inner products are from {-1, -½, 0, ½, 1}, so integer sums can only reach {0, 1, 2, 3, 4, 6} — the perfect squares and non-square integers achievable from these half-integer building blocks. √5 is unreachable.

### Data
- Distinct eigenvalue angles: 7 (3 non-canonical: 30°, 35.264°, 65.905°)
- Angle entropy: 2.004 bits; cell entropy: 1.000 bit; joint: 2.282 bits
- Mean MI(angle; cell): 0.7218 bits; range: 0.14–1.0 bits; 20/36 triples achieve MI = 1.0
- Capacity gain over cell-only: +1.282 bits (2.3× improvement)
- |S| values: {0, 1, √2, √3, 2, √6}; |S| entropy: 1.839 bits
- Magnitude predicts parity: 16/36 triples (44%)
- Multiplicative channel: 4 eigenvalue angles, 13 Voronoi cells, 4.241 bits total entropy, 0/36 XOR6 solutions
- Triple families: 8 groups of 3–8 triples; group 0 is largest (8 triples)
- MI(additive cell; multiplicative cell): 0.5174 bits (moderate correlation)

### Code Assets
- **Quaternion arithmetic suite**: add, multiply, conjugate, normalize, eigenvalue-angle extraction; reused across demos 75–77
- **`eigenvalue_angle()`**: computes the rotation half-angle arccos(q.a) for a unit quaternion
- **`mutual_information()`**: entropy-based MI computation for two discrete channels over 64 mask inputs
- **`classify_magnitude()`**: bins |S| values into the 6 algebraic classes with tolerance; identifies √n pattern
- **`triple_angle_function()`**: maps each of 64 input masks to an angle class for a given winning triple
- **`product_channel_analysis()`**: computes quaternion product and extracts eigenvalue + eigenvector for the multiplicative channel

### Literature Touched
- **Aizenberg (2008)**: MVN sector activation on S¹; Demo 75 shows the additive angle channel is the S¹ component of a product space, and angle subsumes the S² Voronoi cell
- **Demo 74 / this series**: Bracket-vs-cell incomparability at catalog level vs. angle-subsumes-cell at sum level — two layers of the same structure
- **Nazer & Gastpar (2011)**: Compute-and-forward; additive combination of algebraic integers for computation is the structural parallel

### Open Questions
- Can the 8-family partition of winning triples be explained algebraically from the 24-cell geometry, or is it an empirical coincidence of DKC at ζ_8?
- Does the 2.3× capacity gain over cell-only hold for other roots of unity, or is it specific to ζ_8?
- Is there a combinatorial argument for why √5 is unreachable given the 24-cell inner product structure?
- Can the binocular channel be decoded jointly (not independently) to recover more than 2.282 bits — or does the 24% MI loss set a hard ceiling?
- What is the angle channel equivalent for 8-input DKC (sum of 4 quaternions)?
