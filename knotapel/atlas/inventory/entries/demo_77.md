## Demo 77: Activation Zoo for 8-Input DKC

- **Status**: COMPLETE
- **File**: `demo_77_activation_zoo/main.c` (~1012 lines)
- **Tests**: 14/14 pass
- **Depends on**: Demo 50 (parity at k=6, all 13 NPN classes), Demo 66 (quaternionic DKC, 24-cell), Demo 67 (13 directions, 36 XOR6 solutions), Demo 74 (computational invariant, bracket vs cell incomparability), Demo 75 (binocular DKC, angle subsumes cell for sums), Demo 76 (XOR8 = 0 solutions under S² Voronoi)
- **Feeds into**: Demo 78 (recursive scaling), Demo 79 (ζ_12 capacity)

### Headline
XOR8 IS computable at ζ_8 with the combined Sec(8) × Voronoi activation (112 cells, 6 winning quadruples): the wall in Demo 76 was the activation function, not the root of unity — the same discovery pattern as Demo 50 where parity was blocked by split-sigmoid, not the lattice.

### Key Results
- **XOR8 solved (Part D/F)**: Sec(8) × Voronoi activation (S¹ × S²) yields 6 winning quadruples out of 10,626. All 6 achieve 100% accuracy on all 256 input masks.
- **Phase transition (Parts D/E)**: Accuracy climbs monotonically: S² Voronoi (50%) → Sec(8) at 85.2% → Polar 14×6 at 99.2% → Sec(8)×Vor at 100%. The phase transition from 0 to 6 winners occurs between 84 cells (k=6, 0 winners, 96.9%) and 112 cells (k=8, 6 winners, 100%). Critical threshold approximately 100 cells.
- **Neither channel alone suffices**: Sector activation alone (S¹, even at k=16 with 16 cells): 0 winners, max 96.9%. S² Voronoi alone (14 cells): 0 winners, 50.0%. Both channels are required — this is computationally necessary given Demo 74's incomparability result.
- **Paired quaternion structure (Part F)**: Every winning quadruple contains exactly one pair of quaternions sharing the same eigenvector direction but differing in eigenvalue angle. Removing either element of the pair leaves an XOR6-winning triple. All 6 winners contribute exactly 2 XOR6 subsets each; 12 of the 24 embedded triples (50%) are XOR6 winners.
- **Eigenvalue angles in winners**: The 6 winning quadruples use only 2 distinct eigenvalue half-angles: 45° and 90° (the two canonical octahedral angles). The non-canonical angles (30°, 35.264°, 65.905°) that appear in additive sums are NOT present in the individual catalog entries forming winning quadruples.
- **Geometry of winners**: 15 of 24 catalog quaternions participate across the 6 winners. Three dominant Voronoi cells (0, 1, 10) each appear in 6 of 24 winner slots — axis-aligned and edge-diagonal directions form the structural skeleton.
- **Polar activation near-miss (Part B)**: Polar 14×6 (S² direction × magnitude bins, 84 cells) reaches 99.2% — one misclassified pair from perfection. The magnitude channel ("odometer" from Demo 75) adds genuine power over direction alone (50% → 99.2%) but the missing information is in the angle channel, not magnitude.
- **S³ Voronoi worse than expected (Part C)**: 24-cell Voronoi on S³ (24 cells) achieves only 90.2% — worse than Sec(16) at 96.9% despite more cells. The S³ partition mixes angle and direction information non-optimally; the factored S¹ × S² decomposition is superior to the unfactored S³ approach.
- **Corrects Demo 76**: Demo 76's conclusion "ζ_8 is a 6-input structure" is activation-specific. ζ_8 with S¹ × S² activation supports at least 8 inputs.

### Theorems/Conjectures
- **Activation Determines Capacity (DEMONSTRATED)**: The root of unity alone does not fix computational capacity. The activation function (how you read the quaternion) determines what functions the computation can express. Same ζ_8, same 24-cell — 0 XOR8 solutions with S² activation, 6 solutions with S¹ × S² activation.
- **Incomparability Implies Product Necessity (DEMONSTRATED)**: Demo 74 proved bracket (eigenvalue) and cell (eigenvector) are incomparable invariants. Demo 77 confirms this incomparability is computationally necessary: neither channel alone can compute XOR8, but their product can. The incomparability is not just structural — it reflects independent computational information.
- **Paired Quaternion Structure (OBSERVED)**: Every XOR8 winner is an XOR6 winner plus a "paired partner" sharing the same eigenvector direction but different eigenvalue angle. CONJECTURED: XOR(n) winners at ζ_8 are built by extending XOR(n-2) winners with paired quaternion partners, giving a recursive structure connecting input sizes.
- **Critical Cell Count ~100 (OBSERVED)**: The phase transition from 0 to non-zero winners occurs between 84 and 112 cells. CONJECTURED: the critical threshold is related to the masks-per-cell dropping below 256/100 ≈ 2.56, with the exact threshold depending on the parity structure of the partition.
- **S¹ × S² Activation as Quaternionic MVN (ESTABLISHED)**: The combined activation is the quaternionic generalization of Aizenberg's MVN: instead of reading the argument of a complex number (S¹ only), read both eigenvalue angle (S¹) and eigenvector direction (S²) of the quaternion (S¹ × S²). Demo 77 provides the first proof that this generalization unlocks computation not achievable by S¹ or S² alone.

### Data
- XOR8 winners (Sec8 × Voronoi, 112 cells): 6 / 10,626 quadruples
- XOR8 winners (S² Voronoi, 14 cells): 0 / 10,626
- Phase transition: 0 winners at 84 cells (Sec6×Vor), 6 winners at 112 cells (Sec8×Vor)
- Accuracy by activation: S²Vor(50%) → Sec4(77.7%) → Sec8(85.2%) → S³Vor(90.2%) → Sec10(92.6%) → Sec12(90.6%) → Polar14×3(93.0%) → Sec4×Vor(93.8%) → Polar14×2(93.8%) → Sec16(96.9%) → Sec6×Vor(96.9%) → Polar14×6(99.2%) → Sec8×Vor(100%)
- Winning quadruples: [0,1,9,23], [0,1,18,22], [2,4,5,23], [2,9,19,21], [4,5,15,16], [17,19,20,21]
- Each winner: exactly 2 XOR6-winning triple subsets; 12 of 24 embedded triples are XOR6 winners
- Eigenvalue angles in winners: only 45° and 90° (canonical octahedral)
- Quaternions participating: 15 of 24
- Dominant Voronoi cells: 0 (axis x), 1 (axis z), 10 (edge diagonal) — 6 appearances each

### Code Assets
- **14-activation comparison harness**: exhaustive search over all 10,626 quadruples under each of 14 activation functions; single loop structure with pluggable activation callback
- **`sector_activate()`**: bins quaternion eigenvalue angle into k equal sectors on [0°, 360°); implements Aizenberg MVN in the quaternion context
- **`polar_activate()`**: joint direction (Voronoi cell) × magnitude bin activation; configurable bin count for magnitude axis
- **`s3_voronoi_activate()`**: nearest-quaternion in the 24-cell catalog on S³ (no antipodal identification)
- **`product_activate()`**: S¹ × S² combined activation; cell index = sector_index * 14 + voronoi_cell; the key new activation function
- **`xor6_subsets()`**: for a winning quadruple, enumerates all C(4,3)=4 triples and identifies which are XOR6 winners; establishes the paired structure
- **`majority_vote_accuracy()`**: given an activation and a weight set, computes XOR8 accuracy by majority vote per activation cell

### Literature Touched
- **Aizenberg (2008, 2011)**: MVN sector activation on S¹; Demo 77 demonstrates that the S¹ component alone (even at k=16) cannot compute XOR8, while the quaternionic S¹ × S² product can. The S¹ → S¹ × S² generalization from C to H is the central contribution.
- **Demo 50**: The parity wall at split-sigmoid, broken at k=6 sectors. Demo 77 is the same story one level up: XOR8 wall at S²-only activation, broken at S¹ × S² combined activation.
- **Demo 74 (Computational Invariant)**: Incomparability of bracket vs. cell invariants at catalog level; Demo 77 proves this incomparability is computationally load-bearing.
- **Nazer & Gastpar (2011)**: Compute-and-forward; the product activation reads both "channels" of the algebraic integer ring, analogous to reading both amplitude and phase in lattice coding.
- **Abramsky / TL categorical quantum mechanics**: The non-semisimplicity of the Temperley-Lieb algebra at roots of unity is where the extra computational power lives; the product activation exposes both the S¹ (eigenvalue) and S² (eigenvector) faces of this structure.
- **Habiro (2002) / cyclotomic invariants**: Z[ζ_8] is richer than any single projection reveals; Demo 77 demonstrates empirically that the algebraic structure of ζ_8 supports at least 8-input computation.
- **literature-survey.md**: comprehensive novelty analysis in the demo folder; DKC fills a gap between knot theory, algebraic number theory, spherical geometry, lattice codes, and complex-valued neural networks that no existing paper occupies.

### Open Questions
- Does the paired quaternion structure (XOR8 winner = XOR6 triple + paired partner) hold recursively? Can XOR10 winners be built from XOR8 winners by adding another paired quaternion?
- Is there a combinatorial formula for the number of XOR(n) winners as a function of input size n at ζ_8 with Sec(n) × Voronoi activation?
- Why do the winning quadruples use only the two canonical octahedral angles (45°, 90°) while additive sums introduce non-canonical angles? Is there a group-theoretic explanation?
- What is the minimum k in Sec(k) × Voronoi for XOR10? Does the cell-count threshold scale as ~2^n / 2.3 for XOR(n)?
- Does the S¹ × S² activation connect to the Hopf fibration S³ → S²? The eigenvalue (S¹) and eigenvector (S²) decomposition of a unit quaternion is structurally a Hopf fibration; is the computational power of the product activation related to the fiber bundle structure?
- Can the 99.2% accuracy of Polar 14×6 be pushed to 100% with a different magnitude binning — or is there a fundamental obstruction at 84 cells?
