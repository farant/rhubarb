## Demo 16: Reduced Bracket Characterization — What IS g?

- **Status**: COMPLETE
- **File**: `demo_16_reduced_bracket/main.c` (~975 lines)
- **Tests**: 8 checks across 6 parts (A-F): waveform, peak localization, Pareto frontier, connected-sum factorization, minimal angle set, amplitude statistics
- **Depends on**: Demo 14 (bracket = delta^k * g factorization), Demo 15 (g optimal angles, 4.3x classification improvement)
- **Feeds into**: Demo 17 (multilayer DKC), later demos using g as the primary classification signal

### Headline
Deep characterization of the reduced bracket g(K,A) = bracket(K,A) / delta^k(A): waveform analysis, peak localization showing a broad plateau near 1.266*pi, Pareto frontier proving g needs fewer bits than raw bracket, connected-sum factorization test (fails — structural issue), greedy angle selection showing g achieves higher min_dist, and amplitude statistics by knot family.

### Key Results
- **Waveform** (Part A): g has fewer zeros than raw bracket (removes delta zeros for k>0 knots), but is NOT smoother — dividing by delta near its zeros amplifies roughness. 256-sample characterization with smoothness/range/zero metrics per knot.
- **Peak localization** (Part B): g peak at ~1.266*pi is a broad plateau (half-max width > 0.05*pi, not a spike). g peak min_dist > raw peak min_dist — confirms Demo 15's finding with fine-grained 256-sample scan over [1.0*pi, 1.5*pi].
- **Pareto frontier** (Part C): bit-depth sweep 2-16 bits. g reaches full classification in fewer or equal bits than raw bracket at optimal angles. Uniform quantization with fixed amplitude range ±50.
- **Connected-sum factorization** (Part D): g(K1#K2) vs g(K1)*g(K2) tested at 8 angles for 8 composites. **Finding: g does NOT factorize** — braid-encoded connected sum != standard connected sum. The delta removal doesn't fix this; the issue is structural.
- **Minimal angle set** (Part E): greedy max-min-dist selection at 128-sample resolution. g achieves higher min_dist than raw at 1 angle. Both stop when diminishing returns (<1% improvement).
- **Amplitude statistics** (Part F): knots classified into families (torus, mirror, composite, fig-eight, unknot). Torus and mirror families have similar g amplitudes. Composite g amplitudes differ from prime knots.

### Theorems/Conjectures
- Five predictions tested:
  1. g waveforms smoother — **PARTIALLY FALSE** (fewer zeros, but more rough near delta zeros)
  2. Optimal angle peak is broad plateau — **CONFIRMED** (half-max width > 0.05*pi)
  3. g Pareto frontier 1-2 bits better — **CONFIRMED** (g needs fewer bits)
  4. g factorizes under connected sum — **FALSE** (structural issue with braid concatenation)
  5. g needs fewer angles — tested via greedy; g achieves higher min_dist per angle

### Data
- 21 knots after dedup (from ~25): unknot, T(2,3) through T(2,13) + mirrors, figure-eight, granny, square, T23#T25, T23#T25*, T25#T25, T25#T25*, granny*, T23#fig8
- 8 composite knots with tracked component indices
- Delta exponents (k values) cached per knot
- Waveform metrics: smoothness (mean |derivative|), amplitude range (max/min), zero count
- Peak scan: 256 samples over [pi, 1.5*pi], half-maximum width measurement
- Pareto table: 15 bit-depths x {raw, g} pair separation counts
- Greedy angle selection: up to 20 rounds with diminishing-returns cutoff

### Code Assets
- **`delta_exponent()`**: determines k by testing bracket at 4 delta-zero angles (pi/4, 3pi/4, 5pi/4, 7pi/4) + perturbation test
- **`reduced_bracket_at()`**: computes g = bracket / delta^k at a specific A value
- **`min_dist_from_vals()`**: minimum pairwise distance across all knot pairs from precomputed values
- **`dedup_knots()`**: removes duplicate braids by comparing bracket values at 2 reference angles, maintains composite index remapping
- **`add_connected_sum()`**: builds composite braid by concatenating words, tracks component indices in CompositeInfo struct
- **Greedy angle selection**: precompute-all-then-sweep approach with accumulated distance² per pair — more efficient than Demo 12's per-round recomputation
- Reuses: complex arithmetic, braid state-sum oracle, union-find

### Literature Touched
- Reduced Kauffman bracket (dividing out delta factors)
- Connected sum of knots and multiplicativity of invariants
- Quantization Pareto frontiers (precision vs classification accuracy)
- Greedy feature selection (max-min-dist criterion)

### Open Questions
- Why doesn't g factorize under braid concatenation? Is there a different braid encoding of connected sum that would restore multiplicativity?
- Can the peak plateau shape be explained algebraically (relationship to delta zeros at 1.25*pi)?
- What happens to the Pareto frontier as the knot table grows beyond 21 entries?
