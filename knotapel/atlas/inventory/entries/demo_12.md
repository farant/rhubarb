## Demo 12: Quantization

- **Status**: COMPLETE
- **File**: `demo_12_quantization/main.c` (~780 lines)
- **Tests**: 6 parts (A-F): reference signatures, bit-depth sweep, component analysis, precision x width matrix, weight table, nearest-neighbor classifier
- **Depends on**: Demo 10 (numerical bracket evaluation), Demo 11 (waveform / greedy angle selection)
- **Feeds into**: Later demos exploring DKC and classification with quantized representations

### Headline
Investigates how aggressively Kauffman bracket complex weights can be quantized while retaining 100% knot classification across 10 test knots, finding a Pareto-optimal precision x width tradeoff and drawing an explicit parallel to BitNet's 1.58-bit LLM weights.

### Key Results
- **Bit-depth sweep** (Part B): scans 16/12/8/6/4/3/2/1 bits per component at a single optimal angle, then binary searches for minimum bits achieving 100% pair separation
- **Component analysis** (Part C): amplitude-only FAILS (granny/square have identical |bracket| — predicted from Demo 11), phase-only also insufficient alone, sign-only (quadrant / 1-bit) and ternary ({-1,0,+1} / 1.58-bit) tested
- **Precision x width matrix** (Part D): explores {1,2,3,4,8} bits x {1,2,3,4,8} angles, finds Pareto optimum (minimum total bits per knot for 100% classification)
- **Compiled weight table** (Part E): prints full-precision and quantized weight vectors, reports total model size in bits/bytes
- **Nearest-neighbor classifier** (Part F): self-classification of all 10 knots at full precision, plus invariance tests with alternate braid representations (alt trefoil s2*s1*s2, alt figure-eight s2^{-1}*s1*s2^{-1}*s1)

### Theorems/Conjectures
- Granny and square knots have identical bracket amplitude at any single angle (they're connected sums of trefoil with itself vs its mirror) — phase is required to distinguish them
- BitNet parallel: if LLMs work with 1.58-bit weights, can knot classification work with similarly extreme quantization? The precision x width matrix explores this tradeoff systematically

### Data
- 10-knot test suite: unknot, trefoil, mirror trefoil, figure-eight, torus(2,5), torus(2,7), granny, square, cinquefoil mirror, single crossing
- 256-sample waveform discretization for greedy angle selection (up to 8 angles)
- Tradeoff matrix: bits x angles → pairs separated (out of 45 total pairs)

### Code Assets
- **`quantize()`**: uniform quantize-dequantize for a double in [lo, hi] to B bits
- **`cx_quantize()`**: independently quantizes real and imaginary components
- **`cx_ternary()`**: ternary quantization {-1, 0, +1} per component with threshold (BitNet-style 1.58-bit)
- **`cx_sign()`**: pure quadrant / 1-bit per component sign extraction
- **`test_quantized_separation()`**: tests whether B-bit quantization at M angles separates all knot pairs — core of the sweep
- **`classify_braid()`**: nearest-neighbor classifier in quantized weight space — computes bracket at greedy angles, quantizes, finds closest reference
- **`select_greedy_angles()`**: greedy max-min angle selection from Demo 11 (reused)
- Reuses: complex arithmetic, braid state-sum bracket, union-find loop counter

### Literature Touched
- BitNet / extreme quantization in neural networks ({-1, 0, +1} weights)
- Knot classification via bracket evaluation at specific angles (waveform approach from Demo 11)
- Connected sum structure: bracket of granny vs square

### Open Questions
- What is the actual Pareto-optimal point? (depends on runtime — not hardcoded in source)
- Does the ternary/sign quantization achieve 100% with multiple angles even though it fails at 1 angle?
- How does quantization robustness scale as the knot table grows beyond 10 entries?
