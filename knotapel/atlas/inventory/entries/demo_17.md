## Demo 17: Multi-Layer DKC

- **Status**: COMPLETE
- **File**: `demo_17_multilayer_dkc/main.c` (~1468 lines)
- **Tests**: ~6 pass (5 parts: A=flat baseline, B=Arf split, C=hierarchical tree, D=GD vs analytical, E=Pareto comparison)
- **Depends on**: Demo 10 (numerical engine), Demo 12-16 (greedy angle selection, quantization, Pareto framework, reduced bracket g, delta exponent k)
- **Feeds into**: Later DKC architecture decisions (flat vs hierarchical tradeoffs)

### Headline
Tests whether hierarchical knot classification (Arf-split, greedy binary tree) outperforms flat evaluation at low bit budgets, comparing analytical angle selection against gradient descent on ~21 unique 3-strand knots.

### Key Results
- **Part A (Flat baseline)**: Greedy angle selection over 256-sample survey; Pareto grid at 1/2/4 angles x 1-10 bits; finds minimum bits for full 210/210 pair classification
- **Part B (Arf split)**: Layer 1 = delta exponent k (k=0 vs k>0) separates cross-group pairs for free (1 bit); per-group optimal angles found independently; Pareto shows bits savings vs flat
- **Part C (Hierarchical tree)**: Greedy bisection up to depth 4; splits on real-part threshold at optimal survey angle; counts cross-leaf pairs separated by tree structure alone; Pareto includes tree bits + per-leaf value bits
- **Part D (GD vs analytical)**: 50 random starts x 100 iterations with simulated annealing (step *= 0.95); analytical (exhaustive survey) matches or beats gradient descent within 1% tolerance
- **Part E (Pareto comparison)**: All four methods (flat-1A, flat-2A, arf-split, tree) on same bit-budget axes; identifies which methods achieve full classification first; diagnoses "stubborn pairs" at 1-bit tree

### Five Predictions Tested
1. Arf split saves 1-2 bits over flat
2. Greedy hierarchical tree beats Arf split
3. Analytical matches/beats gradient descent
4. Biggest gains at low bit budgets (4-8 bits)
5. Flat catches up at high bit budgets (16+)

### Data
- ~21 unique knot types after dedup (from 24 candidates: 13 torus knots+mirrors, 8 connected sums, figure-eight, plus 2 not in this table)
- 256-sample survey waveforms for greedy angle selection
- Pareto grid: 1-10 bits x {flat-1A, flat-2A, arf-split, tree}
- Gradient descent: 50 starts x 100 iterations, step annealing 0.95
- Stubborn pair diagnostics at 1-bit resolution

### Theorems/Conjectures
- **DKC thesis**: algebraically compiled weights encode topology, not statistical patterns — hierarchy exploits algebraic structure (Arf invariant, delta exponent) for free bits
- Arf invariant as Layer 1: Craven et al. (2025) found neural networks CANNOT learn the Arf invariant; DKC computes it analytically for free
- Analytical angle selection matches gradient descent because the objective landscape (min pairwise distance) is smooth and low-dimensional

### Code Assets
- `delta_exponent()` — computes k (delta factor) by evaluating bracket at delta-zero angles (pi/4, 3pi/4, 5pi/4, 7pi/4) plus perturbation test
- `reduced_bracket_at()` — g(A) = bracket(A) / delta^k, the reduced invariant
- `k_cache[]` with lazy `ensure_k_cache()` — cached delta exponents per knot
- `g_survey[256][MAX_KNOTS]` with lazy `ensure_survey()` — precomputed reduced bracket at 256 equispaced angles
- `min_dist_subset()` / `min_dist_all()` — min pairwise distance for knot subsets from precomputed values
- `QuantVal` struct with `quantize()` — integer quantization (B bits, clamped)
- `count_separated_pairs()` — multi-angle quantized separation counter
- `find_best_bisection()` — greedy bisection: searches all survey angles x all midpoint thresholds, scores by min(min_dist_left, min_dist_right)
- `TreeNode` and `build_tree()` — recursive binary tree construction up to MAX_TREE_DEPTH=4
- Gradient descent with multi-start and annealing for angle optimization (50 starts x 100 iters)

### Literature Touched
- Craven et al. (2025): neural networks cannot learn the Arf invariant
- Hierarchical classification / decision trees for topological invariants
- Greedy feature selection vs gradient-based optimization
- Quantization-aware classification (bit-budget Pareto analysis)
- Arf invariant as a topological splitting criterion

### Open Questions
- Does the hierarchical advantage grow with knot table size (100+ knots)?
- Can deeper trees (depth > 4) provide further gains, or do they overfit to training angles?
- Are there other algebraic invariants beyond Arf that provide free bits at Layer 1?
