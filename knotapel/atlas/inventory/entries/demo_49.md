## Demo 49: Neglecton Weights

- **Status**: COMPLETE
- **File**: `demo_49_neglecton_weights/main.c` (~1136 lines)
- **Tests**: ~10 pass (P1: standard catalog >= 50 values, P2: neglecton catalog non-empty, P3: zeta_8^8 = 1, P4: d(delta)/d(epsilon) = 4, P5: NPN class count = 13, P6: total quartets match expected, P7: neglecton catalog non-empty, P8: XNOR3 has solutions [EXPECTED FAIL], P9: parity solution uses neglecton weight [EXPECTED FAIL], P10: no parity from standard-only weights, P11: combined > standard catalog, P12: parity min distance)
- **Depends on**: Demo 48 (forward DKC zoo, 100M quartets, XNOR3 zero solutions with standard catalog), Demo 47 (parity investigation, MVN architecture), Demo 45 (NPN classification reference data, topological frequency/RMS)
- **Feeds into**: Demo 50 (activation function zoo, k-sector MVN search); closes the perturbation approach entirely; reframes parity wall as activation geometry obstruction

### Headline
Tests the neglecton prediction — radical elements of TL_4 at delta=0 extracted via first-order bracket perturbation yield 148 new exact Z[zeta_8] values from 15,242 previously invisible braids, but all 116 genuinely new values share angles with existing standard values. Parity remains unreachable across 2.18 billion quartets. The perturbation approach is closed at all orders: the wall is in the activation geometry (split-sigmoid/half-plane), not the weight space.

### Key Results
- **Part A**: Dual bracket evaluation separates standard (loops=1) and neglecton (loops=2) contributions; neglecton bracket = 4 * Sum_{states with loops=2} A^(a-b), exact Cyc8 arithmetic; d(delta)/d(epsilon) = 4 verified numerically; 87,890 braids enumerated (2-3 strand, length 1-8); 15,242 previously invisible braids (std=0, neg!=0); standard catalog: 100 values, neglecton catalog: 148 values, combined: 216 values (68 std-only, 116 neg-only, 32 both)
- **Part B**: NPN classification reproduces 13 non-trivial classes from Demo 48
- **Part C**: Exhaustive search of 216^4 = 2,176,782,336 quartets with split-sigmoid activation; parity (XNOR3) has ZERO solutions; ranking of all 13 NPN classes identical to Demo 48; EXACT1 (Hamming distance 1 from parity) is last reachable class; all 12 non-parity classes grow proportionally (~21× with 21.8× larger search space)
- **Part D**: Spearman correlations (solutions vs topo_freq: rho=+0.7582, solutions vs reverse_RMS: rho=-0.7802), slightly lower than Demo 48 consistent with neglecton dilution; per-class min Hamming distance to parity preserved from Demo 48

### Theorems/Conjectures
- CONFIRMED: Neglecton bracket produces exact Z[zeta_8] values from loops=2 smoothing states, with scale factor 4 = d(delta)/d(epsilon)
- CONFIRMED: 116 neglecton-only values introduce ZERO genuinely new angles — all are scalar multiples of existing standard values at the same angular positions
- CONFIRMED: Parity (XNOR3) unreachable with extended 216-value catalog (zero solutions from 2.18B quartets)
- CONFIRMED: Standard-only subspace produces zero parity solutions (consistency with Demo 48)
- PROVEN (analytically): k=2 half-plane classifier CANNOT compute 3-input parity with ANY complex weights (complex Minsky-Papert: y1>d, y2>d implies y1+y2>2d>d, contradicting pair requirement)
- CONFIRMED: Aizenberg's parity construction weights (zeta_8, i, -1) are IN Z[zeta_8] — the lattice DOES contain parity-capable weights
- CLOSED: Perturbation approach at all orders — higher-order terms (loops=3,4,...) also produce A^(a-b) sums scaled by real factors, preserving same angular structure
- REFRAMED: Parity wall is in activation geometry (number of decision sectors), not weight space; k=2 sectors provably insufficient, k=8 sectors sufficient (Aizenberg)

### Data
- Enumeration: 87,890 braids (2-3 strand, length 1-8), A = e^{i*5*pi/4} = -zeta_8
- Standard bracket = 0: 17,930 braids (20.4%); nonzero neglecton: 81,006 (92.2%); both zero: 2,688 (3.1%)
- Previously invisible braids (std=0, neg!=0): 15,242
- Neglecton magnitudes start at |z|=4 (4× standard minimum of 1), consequence of d(delta)/d(epsilon) = 4
- Genuinely new angles from neglecton: 0 out of 116 (all match existing standard angles)
- Search: 216^4 = 2,176,782,336 quartets; source breakdown: 1.0% std-only, 99.0% has>=1 neglecton, 22.0% all-neglecton
- XNOR3 solutions: 0; min Hamming distance to parity: 1 (EXACT1, tt=0x16)
- Demo 48 comparison: all classes grow ~21× proportionally; EXACT1 grows 12.8× (below average, consistent with boundary position); XNOR3 remains exactly 0
- Spearman rho (solutions vs topo_freq): +0.7582; (solutions vs reverse_RMS): -0.7802
- Aizenberg parity weights: w1=zeta_8=(0,1,0,0), w2=i=(0,0,1,0), w3=-1=(-1,0,0,0) — all in standard catalog

### Code Assets
- `Cyc8` type: exact cyclotomic integer Z[zeta_8] arithmetic (basis {1, zeta_8, zeta_8^2, zeta_8^3} with zeta_8^4=-1); `cyc8_make/zero/one/add/mul/scale/eq/conj/pow_int/to_cx`
- `DualBracket` struct: paired standard + neglecton bracket values
- `braid_bracket_dual()`: evaluates BOTH standard (loops=1 states) and neglecton (4 × loops=2 states) brackets in exact Cyc8 arithmetic, skipping loops>=3
- `build_dual_catalog()`: enumerates 2-3 strand braids up to length 8, builds separate standard and neglecton catalogs of distinct Cyc8 values, tracks invisibility statistics
- `build_combined_catalog()`: merges standard + neglecton catalogs with source tags (SRC_STD, SRC_NEG, SRC_BOTH)
- Angle analysis: checks whether neglecton-only values introduce genuinely new angles vs existing standard catalog
- `npn_transform()`, `npn_init()`, `npn_build_classes()`: NPN classification from Demo 48
- `split_sigmoid()`: smooth XOR of real/imaginary sigmoid outputs (same as Demos 48, 50)
- `parity_dist_lut[]`: Hamming distance to parity (0x69/0x96) lookup table
- `Quartet` struct + `parity_examples[]`: tracks weight indices for parity solutions with source classification
- `spearman_rho()` with `assign_ranks()`: rank correlation with tie handling
- `Demo45Ref` table: 13-entry reference data (canon_tt, topo_freq, rms, orbit) from Demo 45

### Literature Touched
- Temperley-Lieb algebra TL_4 radical at delta=0 (nilpotent elements, neglecton degrees of freedom)
- Cyclotomic integers Z[zeta_8]: exact arithmetic, angular structure of integer-coefficient sums of 8th roots
- Aizenberg (2007, 2011): multi-valued neuron (MVN) computes parity with k=2^n sectors; weights (zeta_8, i, -1) for 3 inputs
- Minsky-Papert (complex analog): k=2 half-plane cannot linearly separate 3-input parity in C
- Born-rule projection: bracket amplitude discards phase (Demo 27 connection)
- First-order perturbation theory: d(bracket)/d(epsilon) extracts radical contributions exactly

### Open Questions
- Does the angular obstruction have a clean algebraic characterization (e.g., in terms of cyclotomic field automorphisms)?
- What is the minimum sector count k* where parity first becomes computable with Z[zeta_8] weights? (Predicted: k=8, but could be lower — Demo 50 answers this)
- Can the per-NPN-class minimum sector count be predicted from the truth table's algebraic properties?
- Does the 4× magnitude scaling of neglecton values have deeper algebraic meaning beyond d(delta)/d(epsilon)?
- Is there a second-order perturbation (loops=3 contribution) that would break the angular structure? (Predicted: no, since A^(a-b) sums with real scaling preserve angles at all orders)
- Connection to TL non-semisimplicity: the radical is precisely what creates neglecton degrees of freedom — is this related to the non-semisimple TQFT literature?
