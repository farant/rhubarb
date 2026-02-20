## Demo 15: Angle Optimization — The WRT Convergence Test

- **Status**: COMPLETE
- **File**: `demo_15_angle_optimization/main.c` (~884 lines)
- **Tests**: ~8 pass (WRT convergence, greedy vs WRT single-angle, reduced bracket wins, multi-angle separation, survey best)
- **Depends on**: Demo 14 (delta factorization, reduced bracket g), Demo 11 (greedy angle selection, 1.805*pi), Demo 10 (unit-circle evaluation)
- **Feeds into**: later demos using optimal angle sets for knot classification; validates Chern-Simons theory empirically

### Headline
Greedy-selected optimal angle (1.805*pi) independently converges to within 1.5% of the WRT level-3 evaluation point (11*pi/6), empirically validating that algebraically special angles ARE informationally optimal angles.

### Key Results
- **WRT convergence**: closest WRT angle to greedy (1.805*pi) is within 5% of pi — the information-theoretic optimum independently found the Chern-Simons evaluation point
- **Single-angle head-to-head**: greedy outperforms all WRT angles on min_dist (hardest-pair separation); WRT angles cluster near 2*pi where brackets converge
- **Reduced bracket g vs raw**: g wins or ties at majority of angles; g has tighter dynamic range (max/min ratio)
- **Multi-angle**: 3 WRT angles on reduced bracket g separate ALL pairs in the 21-knot table; 4 WRT angles also achieve full separation
- **Complete 128-sample survey**: identifies top-5 angles for both raw and reduced bracket; survey best >= greedy for g
- 25 knots deduplicated to ~21 unique types (granny* == granny under bracket equivalence, etc.)

### Theorems/Conjectures
- CONFIRMED (numerically): WRT evaluation points are near-optimal for knot classification — within 1.5% of greedy optimum at r=3
- CONFIRMED: Reduced bracket g outperforms raw bracket at majority of angles (by min_dist metric)
- CONFIRMED: 3 WRT angles + reduced bracket suffice to separate all 21 knot types
- CONJECTURED: Algebraically special angles (roots of unity from Chern-Simons) ARE informationally optimal — "topology knows information theory"

### Data
- 25 knots (deduplicated to ~21): unknot, T(2,3) through T(2,13) + mirrors, figure-eight, granny, square, T23#T25, T25#T25 + variants, T23#fig8
- WRT angle table: r=3 (11*pi/6 = 1.833*pi), r=4, r=5, r=6, r=8, r=10, r=12
- Greedy angle: 1.805*pi from Demo 11
- Delta exponents (k) per knot: 0, 1, or 2
- Separation matrix: pairs separated at each angle, min_dist, mean_dist
- Top-5 angles for raw bracket and reduced bracket g
- 6 random baselines for comparison

### Code Assets
- `wrt_angle(r)`: computes WRT evaluation angle for level r via theta = 2*pi - pi/(2r)
- `SepMetrics` struct: separated count, min_dist, mean_dist, sum_log_dist
- `measure_separation()`: evaluates full pairwise separation metrics for a set of angles, supports both raw and reduced bracket
- `count_separated_pairs()`: convenience wrapper for pair count
- `delta_exponent()`: detects delta power k by counting zeros at pi/4 multiples + second-order derivative check
- `reduced_bracket_at()`: computes g(K,A) = bracket(K,A) / delta^k(A) with zero-avoidance fallback
- `dedup_knots()`: removes duplicate knots by comparing bracket values at two reference angles
- `add_torus_2n()`, `add_connected_sum()`: knot table constructors for torus knots and connected sums

### Literature Touched
- Witten-Reshetikhin-Turaev (WRT) invariants at level r
- Chern-Simons theory evaluation points (t = A^{-4} mapping)
- Jones polynomial via bracket-to-Jones: t = A^{-4}
- Information-theoretic angle optimization (Pareto frontier concept)
- Greedy sensor placement / optimal sampling

### Open Questions
- Does the WRT-greedy convergence hold for larger knot tables (beyond 13 crossings)?
- Is there a proof that WRT points are optimal, or is this an empirical observation?
- The Pareto frontier (Prediction 5 from header) is mentioned but not explicitly computed — could be a follow-up
- How does the required number of WRT angles scale with knot complexity?
- Connection to volume conjecture (WRT invariants at large r relate to hyperbolic volume)
