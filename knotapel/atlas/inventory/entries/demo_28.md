## Demo 28: Unitary Activation

- **Status**: COMPLETE
- **File**: `demo_28_unitary_activation/main.c` (~958 lines)
- **Tests**: ~12-15 checks across 6 parts (A-F): train all activations, bracket catalog, gauge comparison, weight analysis, lattice projection, summary
- **Depends on**: Demo 27 (reverse DKC error decomposition: Born-rule vs split-sigmoid), Demo 26 (real 2-2-1 baseline)
- **Feeds into**: Later demos exploring DKC with phase-preserving activations

### Headline
Compares 4 complex activation functions (split-sigmoid, modReLU, cardioid, phase-only) on XOR to test whether U(1)-equivariant activation (modReLU) eliminates the dominant error source in reverse DKC — achieving dramatically lower gauge-corrected RMS and higher lattice projection success rate than split-sigmoid.

### Key Results
- **Training** (Part A): 20 trials per activation, lr=0.5, max 100k epochs on XOR. Reports convergence rates. Phase-only expected <50% convergence (discards amplitude information).
- **Bracket catalog** (Part B): builds complex bracket catalog at A = e^{i*5pi/4} (delta=0), strands 2-3, lengths 1-8, filtering |bracket| > 0.5. Up to 8192 entries.
- **Gauge comparison** (Part C): for each activation's best network, sweeps 360 rotation angles to minimize RMS distance from weights to nearest catalog brackets. Key predictions:
  - P1: modReLU gauge-corrected RMS < 0.15 (vs 0.3494 for split-sigmoid from Demo 27)
  - P2: Cardioid RMS in [0.15, 0.35]
  - P5: Gauge angles near 7pi/4 (315 deg) for all activations
- **Weight phase/amplitude decomposition** (Part D): separates per-weight error into phase error (radians) and amplitude error. P6: modReLU phase error < split-sigmoid (because modReLU preserves phase exactly).
- **Lattice projection** (Part E): for each converged network: gauge-rotate → snap each weight to nearest bracket → inverse-rotate → test if projected network still solves XOR. P4: modReLU ≥80% survival, split-sigmoid <20%.
- **Summary** (Part F): comparison table across all activations showing convergence, gauge RMS, gauge angle, phase error, amplitude error.

### Theorems/Conjectures
- **Core thesis**: modReLU(z) = ReLU(|z|+b) * z/|z| is U(1)-equivariant — preserves phase exactly, eliminating the dominant error source (b) from Demo 27
- Two independent error sources in reverse DKC: (a) Born-rule collapse C→R≥0 (phase loss at readout), (b) split-sigmoid activation distortion (phase noise). Source (b) is DOMINANT.
- modReLU should land trained weights closer to bracket values because phase is preserved through the hidden layer, only amplitude is adjusted
- Cardioid activation (direction-dependent scaling) should be intermediate — not equivariant but still phase-aware

### Data
- 4 activations: split-sigmoid (identity + sigmoid readout), modReLU (U(1)-equivariant), cardioid (direction-dependent), phase-only (unit circle normalization)
- 20 trials per activation, each with independent random init (LCG RNG for reproducibility)
- Architecture: z = w1*x1 + w2*x2 + b → activation → sigmoid(re) + i*sigmoid(im) → smooth XOR readout
- Reference baselines: Demo 27 split-sigmoid RMS=0.3494, Demo 26 real 2-2-1 amp RMS=0.2641

### Code Assets
- **`apply_activation()`**: 4-way activation dispatch (split-sigmoid/modReLU/cardioid/phase-only) with clean implementations
- **`activation_backward()`**: full Jacobian-based backprop through each activation — modReLU symmetric Jacobian, cardioid non-symmetric, phase-only projection matrix. Each returns dL/dz_re, dL/dz_im, dL/db_act.
- **`cx28_forward()` / `cx28_train_xor()`**: complex neuron forward pass and SGD training loop with per-activation backward support
- **`build_complex_catalog()`**: enumerates all braids (2-3 strands, length 1-8) at a given A, stores entries with |bracket| > 0.5
- **`find_nearest_cx()`**: brute-force nearest-neighbor in complex bracket catalog
- **`find_best_gauge()`**: sweeps 360 rotation angles to minimize RMS distance from gauge-rotated weights to catalog
- **Lattice projection pipeline**: gauge → snap to catalog → inverse gauge → verify XOR
- **LCG RNG**: `rng_seed()`, `rng_uniform()`, `rng_normal()` for reproducible experiments

### Literature Touched
- modReLU: Arjovsky, Shah & Bengio (2016) — "Unitary Evolution Recurrent Neural Networks"
- U(1) equivariance / gauge symmetry in complex-valued neural networks
- Cardioid activation function for CVNNs
- Born rule interpretation: measurement collapses complex state to real probability
- Reverse DKC (Diagrammatic Knot Computation): can gradient descent rediscover bracket values?

### Open Questions
- Does modReLU's advantage hold for functions beyond XOR (e.g., 3-input functions from Demo 23)?
- What is the minimum catalog density needed for reliable lattice projection?
- Can the gauge angle be predicted analytically rather than swept numerically?
- Is there an activation that eliminates BOTH error sources (Born-rule + phase distortion) simultaneously?
