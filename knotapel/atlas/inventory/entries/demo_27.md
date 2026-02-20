## Demo 27: Complex-Valued Reverse DKC

- **Status**: COMPLETE
- **File**: `demo_27_complex_reverse_dkc/main.c` (~1049 lines)
- **Tests**: ~7 pass (P1: complex neuron solves XOR, P2: bracket values are cyclotomic, P3: complex catalog has more distinct values than amplitude-only, P4: gauge-corrected complex RMS < 0.1, P5: complex RMS better than amplitude RMS, P6: complex matching reduces degeneracy, P7: complex (6 params) beats real (9 params), P8: complex RMS improves on Demo 26 reference)
- **Depends on**: Demo 26 (real-valued reverse DKC, amplitude-only matching, RMS=0.2641 reference), Demo 10 (unit-circle bracket evaluation, complex arithmetic)
- **Feeds into**: establishes that complex bracket VALUES (not just amplitudes) match trained NN weights; cyclotomic Z[zeta_8] structure; U(1) gauge symmetry in weight-bracket correspondence

### Headline
Complex-valued neural network (single complex neuron with split-sigmoid) solves XOR with 6 real parameters where real networks need 9, and gauge-corrected complex bracket matching outperforms Demo 26's amplitude-only decomposition — proving phase carries computational information discarded by the Born-rule projection.

### Key Results
- **Part A**: Single complex neuron (Nitta architecture) with split-sigmoid solves XOR; convergence rate tracked across 20 trials; 6 real parameters (w1, w2, b each complex) vs 9 for real 2-2-1 network
- **Part B**: Complex bracket catalog at A = e^{i*5*pi/4} (delta=0); distinct complex values exceed distinct amplitudes (constellation expansion); majority of values near Z[zeta_8] or Z[i] cyclotomic lattice; degeneracy reduction from amplitude to complex measured
- **Part C**: Complex decomposition with U(1) gauge correction — 360-angle sweep finds optimal rotation aligning weights to bracket values; gauge-corrected RMS < 0.1 (beats Demo 26's 0.2641)
- **Part D**: Phase distribution across 8 sectors (45-degree bins matching 8th roots of unity); phase-writhe correlation for first 20 braids; phases are structured (non-uniform)
- **Part E**: Degeneracy test — complex matching dramatically reduces ambiguity vs amplitude-only (fewer braids match each weight within tolerance)
- **Part F**: Head-to-head: complex neuron (6 params, gauge-corrected) beats real 2-2-1 (9 params, amplitude-only) on bracket decomposition RMS

### Theorems/Conjectures
- CONFIRMED: Single complex neuron with split-sigmoid solves XOR (Nitta 2003 result reproduced)
- CONFIRMED: Bracket values at A = e^{i*5*pi/4} are cyclotomic integers in Z[zeta_8] (coordinates are multiples of 1/sqrt(2) or integers)
- CONFIRMED: Complex bracket catalog contains strictly more distinct values than amplitude-only catalog (constellation expansion)
- CONFIRMED: U(1) gauge symmetry — global phase rotation of all weights preserves network function; optimal gauge angle aligns weights to bracket lattice
- CONFIRMED: Complex matching reduces degeneracy vs amplitude-only (phase information discriminates between braids sharing the same amplitude)
- CONFIRMED: Complex (6 params) outperforms real (9 params) on bracket decomposition — fewer parameters, better RMS
- KEY INSIGHT: Demo 26's amplitude-only matching is a Born-rule projection (C -> R>=0) that discards phase; phase carries computational information

### Data
- Complex neuron: w1, w2 complex weights + b complex bias = 6 real parameters
- 20 training trials with deterministic RNG seeds
- Bracket catalog: 2-3 strand braids up to length 8 at A = e^{i*5*pi/4}, up to 8192 entries
- Amplitude threshold: > 0.5 for catalog inclusion
- Distinct values: complex count > amplitude count (constellation expansion factor reported)
- Cyclotomic lattice test: majority near Z[zeta_8] (tolerance 0.01 on scaled coordinates)
- U(1) gauge sweep: 360 angles, best angle and RMS reported
- Degeneracy: tolerance 0.5 for matching radius
- Real 2-2-1 reference: 9 parameters, amplitude-only RMS
- Demo 26 reference: RMS = 0.2641

### Code Assets
- `Cx` type: complex arithmetic (`cx_make`, `cx_add`, `cx_sub`, `cx_mul`, `cx_div`, `cx_abs`, `cx_phase`, `cx_exp_i`, `cx_scale`, `cx_pow_int`, `cx_neg`)
- `CxNeuron` struct: single complex neuron (w1, w2, b), `cx_neuron_init()`, `cx_neuron_forward()`, `cx_neuron_xor_output()`, `cx_neuron_predict()`, `cx_neuron_train_xor()` with full backprop
- `cx_sigmoid()`: split-sigmoid activation (sigmoid applied independently to Re and Im)
- Smooth XOR decision: `p = h.re*(1-h.im) + (1-h.re)*h.im`
- `CxCatalogEntry` struct: braid + full complex bracket + amplitude + phase
- `build_complex_catalog()`: enumerates braids on 2-3 strands up to given length, stores full complex bracket values
- `find_nearest_cx()` / `find_nearest_amp()`: nearest-neighbor matching in complex plane vs amplitude line
- U(1) gauge sweep: rotates all weights by e^{i*theta}, minimizes complex RMS over 360 angles
- `RealXorNet` struct: 2-2-1 real network for comparison, with `real_net_init()`, `real_net_forward()`, `real_net_train_xor()`
- `braid_bracket_at()`: full complex bracket (not just amplitude) via state-sum
- Union-find Seifert resolution for loop counting

### Literature Touched
- Nitta (2003): single complex neuron solves XOR via orthogonal decision boundaries
- Aizenberg: multi-valued neuron (MVN) with complex-valued weights
- Cyclotomic integers Z[zeta_8]: bracket values as sums of 8th roots of unity
- Born rule projection: |<b>(A)|^2 discards phase information
- U(1) gauge symmetry in neural networks: global phase rotation invariance
- Freedman-Kitaev-Wang: topological quantum computation (complex amplitudes fundamental)
- Demo 26 established amplitude-only baseline (RMS=0.2641)

### Open Questions
- Can the U(1) gauge angle be predicted from the braid structure rather than swept numerically?
- What happens at other cyclotomic evaluation points (not just A = e^{i*5*pi/4})?
- Does the constellation expansion factor have a closed-form expression in terms of the cyclotomic field degree?
- Can multiple complex neurons compose to match multi-weight braid decompositions (moving toward DKC "forward" direction)?
- Relationship between split-sigmoid orthogonal boundaries and the Z[zeta_8] lattice structure?
- Does the degeneracy reduction scale with braid length — do longer braids produce more phase diversity?
