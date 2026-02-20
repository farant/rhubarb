## Demo 13: DKC Test

- **Status**: COMPLETE
- **File**: `demo_13_dkc_test/main.c` (~949 lines)
- **Tests**: ~8+ pass (6 parts: A=separation, B=multi-angle, C=Pareto, D=train/test+self-class+alt-trefoil, E=baseline, F=scaling)
- **Depends on**: Demo 10 (numerical evaluation engine), Demo 11/12 (greedy angle selection, quantization, Pareto frontier)
- **Feeds into**: Later DKC demos (scaling to larger knot tables, forward DKC)

### Headline
Scales DKC (Deterministic Knowledge Compilation) from 10 knots to ~28 candidate braids (deduplicated to ~20+ unique knot types), validating that algebraically compiled weights generalize without training via train/test split, random baseline comparison, and scaling-law analysis.

### Key Results
- 28 candidate 3-strand braids initialized across 3 groups: torus knots T(2,n) with mirrors (n=3,5,7,9,11,13), connected sums (granny, square, T23#T25, etc.), and miscellaneous braid patterns (commutator, mixed-sign words)
- Deduplication via bracket evaluation at 2 reference angles removes equivalent braids
- Greedy angle selection from 256-sample waveforms, up to 8 angles
- **Multi-angle separation**: determines minimum angles for 100% pairwise separation at scale
- **Precision x Width Pareto frontier**: bits-per-component vs number of angles grid; finds minimum total bits per knot for full separation (compared to Demo 12's 8 bits for 10 knots)
- **Train/test DKC validation**: 70/30 split, training self-classification must be 100%, test knots correctly differentiated from training set, invariance test (alt trefoil s2.s1.s2 matches canonical s1.s2.s1)
- **Random baseline**: 100 LCG trials confirm DKC achieves 100% separation while random signatures don't guarantee it
- **Scaling law**: min pairwise separation decreases with knot count (below Demo 12's 0.907 for 10 knots); extrapolation suggests 1/sqrt(N) decay, predicting quantization limits

### Data
- 28 candidate braids, 3 groups: torus knots (14 with mirrors), connected sums (8), other 3-strand braids (6)
- 256-sample waveforms for greedy angle selection
- Pareto grid: bits {1,2,3,4,8} x angles {1,2,3,4,8}
- Train/test split: 70% train, 30% test
- Random baseline: 100 trials, LCG RNG (seed 12345)
- Scaling analysis: subset sizes {5, 10, 15, 20, 25} + full table

### Theorems/Conjectures
- **DKC thesis**: weights compiled from algebraic structure (Kauffman bracket) generalize without training because they encode topology, not statistical patterns
- Min separation scales as ~1/sqrt(N) — implies quantization bit requirements grow logarithmically with knot table size
- 4-bit quantization fails when step_size (range/15) exceeds min_sep

### Code Assets
- `add_torus_2n()` — generates T(2,n) torus knot braid word with sign control (mirror)
- `add_connected_sum()` — concatenates two braid words for connected sum construction
- `dedup_knots()` — removes duplicate knot types by bracket comparison at 2 reference angles
- `select_greedy_angles()` — max-min-distance greedy selection from 256-sample waveforms (heap-allocated)
- `quantize()` / `cx_quantize()` — uniform scalar/complex quantization with configurable bits
- `test_quantized_sep()` — counts separated pairs after quantization at given bits and angles
- `classify_against_table()` — nearest-neighbor classifier against reference signature table
- LCG random number generator for reproducible baseline trials
- `count_separated_pairs()` — utility for multi-angle pairwise distance check

### Literature Touched
- Deterministic Knowledge Compilation (DKC) — algebraic classifier without training
- Kauffman bracket as topological invariant for classification
- Greedy sensor/feature selection (max-min pairwise distance criterion)
- Quantization theory for discrete classifiers
- Scaling laws in combinatorial discrimination

### Open Questions
- How does the Pareto frontier evolve at 50, 100, 250+ knots?
- Is 1/sqrt(N) the correct scaling exponent, or is it knot-family-dependent?
- Can connected sums always be distinguished from prime knots at finite precision?
