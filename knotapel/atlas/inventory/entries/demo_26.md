## Demo 26: Reverse DKC

- **Status**: COMPLETE
- **File**: `demo_26_reverse_dkc/main.c` (~1311 lines)
- **Tests**: 8/8 pass (inline check() assertions)
- **Depends on**: Demo 13 (forward DKC greedy angle), Demo 19 (bracket oracle, braid gates, XOR circuit), Demo 24 (bracket amplitude integer structure)
- **Feeds into**: Demo 27+ (DKC research arc continues), Demo 45-50 (deeper DKC investigations)

### Headline
Tests whether trained neural network weights can be reverse-decomposed into Kauffman bracket amplitudes, probing the reverse direction of the DKC (Deep Knot Correspondence) thesis.

### Key Results
- Weight-level decomposition partially succeeds: some trained XOR weights land near bracket amplitudes, but the signal is weak because at delta=0, bracket amplitudes ARE integers — so any real number is at most 0.5 away from a catalog entry
- Random baseline comparison included: trained weights only modestly outperform random weights in decomposition quality
- Function-level equivalence trivially holds: both braid XOR circuit and trained NN compute XOR correctly, but by completely different mechanisms (bracket thresholds vs sigmoid cascades)
- Angle sweep (Part F) tests whether delta=0 (8th root of unity) is optimal for decomposition — checks P2 prediction
- Syndrome analysis (Part G) classifies residual structure as UNIFORM/STRUCTURED/RANDOM across seeds
- 4-4-1 scaling test on 2-bit adder carry shows decomposition quality at larger network scale (25 params)

### Theorems/Conjectures
- **P1**: Weight decomposition partially succeeds — CONFIRMED (some close matches, but confounded by integer amplitude structure)
- **P2**: Best-matching angle near 8th root (delta~0) — TESTED via angle sweep across 8 angles
- **P3**: Harder tasks need higher-strand braids — TESTED (4-4-1 adder vs 2-2-1 XOR comparison)
- **P4**: Decomposition error correlates with generalization gap — NOT directly tested (both tasks fully converge)
- **P5**: Function-level equivalence even when weight-level fails — CONFIRMED trivially (both compute XOR)
- **P6**: Residuals may show rational tangle structure — TESTED via syndrome analysis (residual patterns characterized)

### Data
- 10 random seeds for XOR training, >=3 converge (20000 epochs, lr=2.0, sigmoid activation)
- Bracket catalog: all 2-strand braids length 1-8 (510 total) + 3-strand braids length 1-6 (5460 total), yielding >10 distinct amplitudes
- Critical observation: at delta=0, bracket amplitudes are consecutive integers (consistent with Demo 24)
- 8 angles tested in sweep: 5pi/4, pi/4, pi/3, pi/2, 2pi/5, pi/6, 1.805pi, 0.75pi
- 2-bit adder carry: 16 patterns, 4-4-1 network, 25 parameters, 50000 epochs

### Code Assets
- Complete backprop implementation for 2-2-1 and 4-4-1 sigmoid networks (pure C89, no dependencies)
- Bracket amplitude catalog builder with deduplication and binary search
- Weight-to-bracket decomposition with RMS error and random baseline comparison
- Braid XOR circuit evaluation reused from Demo 19
- Reproducible LCG random number generator
- Syndrome classifier for residual analysis (uniform/structured/random)
- Complex arithmetic library (reused from Demo 19)

### Literature Touched
- Kauffman bracket polynomial (state-sum model)
- Neural network / knot theory correspondence (DKC thesis — project-internal)

### Open Questions
- Is the integer structure of bracket amplitudes at delta=0 a fundamental obstruction to weight-level decomposition, or a feature that should be exploited differently?
- Would a different decomposition strategy (e.g., weight ratios, weight products) show stronger bracket correspondence than absolute values?
- The function-level match is trivial — is there a deeper functional correspondence beyond "both compute XOR"?
- Does the syndrome analysis reveal any consistent pattern across seeds that could point to a topological invariant?
