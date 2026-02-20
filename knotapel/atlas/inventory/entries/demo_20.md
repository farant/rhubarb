## Demo 20: Braid Error Landscape

- **Status**: COMPLETE
- **File**: `demo_20_braid_error/main.c` (~1017 lines)
- **Tests**: ~7 pass (5 parts: A=1 formula verification, B=2 smoothness+critical delta, C=1 cascade comparison, D=1 delta separation, E=2 transition+region)
- **Depends on**: Demo 18/19 (braid Boolean gates: NOT, NAND words; expression tree compiler; gate output evaluation)
- **Feeds into**: Understanding robustness of braid-based computation; noise tolerance bounds for DKC circuits

### Headline
Maps the error landscape of braid-based Boolean logic as delta moves away from zero, discovering that |delta| = 2|cos(2*theta)| universally predicts gate viability, with smooth degradation and identifiable phase transition thresholds.

### Key Results
- **Part A (Delta landscape)**: Analytical formula |delta| = 2|cos(2*theta)| verified across 360 angles; zeros at theta = pi/4, 3pi/4, 5pi/4, 7pi/4 (8th roots of unity — exactly the angles with NAND support from Demo 19); |delta| values tabulated for 4th, 6th, 8th, 10th, 12th roots of unity
- **Part B (Single-gate error)**: 200-point sweep +/- pi/4 around 5*pi/4 center; NOT separation and NAND separation tracked as functions of |delta|; critical |delta_c| found where NAND first fails; separation degrades smoothly (no jumps > 2.0 between adjacent points)
- **Part C (Cascade error — full adder)**: Full adder (sum + carry via XOR, AND, OR composed from NAND/NOT) evaluated at each sweep angle; 8/8 truth table entries checked; cascade failure compared to single-gate failure; determines whether cascade amplifies error or not
- **Part D (Universal delta curve)**: Gates binned by |delta| ranges [0.0, 0.25), [0.25, 0.5), ..., [1.75, 2.0); average NOT/NAND separation and NAND correctness percentage per bin; roots of unity overlaid on universal curve; |delta| cleanly separates universal (NOT+NAND) from NOT-only angles
- **Part E (Phase transition)**: Finite differences of NAND separation near center; peak-to-average derivative ratio determines sharp vs gradual transition; contiguous "correct regions" (both NOT and NAND work) identified and bounded by theta and |delta|

### Theorems/Conjectures
- **delta = -2*cos(2*theta)**: exact analytical formula, |delta| = 2|cos(2*theta)|
- Delta-zero angles (8th roots of unity) are exactly where braid circuits compute exact Boolean logic
- |delta| is a **universal predictor** of gate viability — cleanly separates universal-computation angles from NOT-only angles across different roots of unity
- Separation degrades smoothly as |delta| increases (no sharp phase transition in the strict sense, but potentially concentrated derivative)

### Data
- 360-angle delta landscape sweep (0 to 2*pi)
- 200-point fine sweep centered at 5*pi/4, range +/- pi/4
- |delta| values at 4th, 6th, 8th, 10th, 12th roots of unity
- 8 |delta| bins with average NOT/NAND separation and correctness percentages
- Full adder correctness (8 truth-table entries) at every 5th sweep point
- Derivative analysis: peak vs average |d(sep)/di| for NOT and NAND

### Code Assets
- `delta_abs_at(theta)` — computes |delta| = |-(A^2 + A^{-2})| at given angle
- `SweepPoint` struct: theta, delta_abs, not_sep, nand_sep, not_correct, nand_correct
- `make_gate_braid()` — constructs braid from input word + gate word
- `gate_output()` — 1-bit gate evaluation (amplitude of bracket)
- `gate_output_2bit()` — 2-bit gate evaluation
- `compute_thresholds()` — dynamic threshold computation (NOT and NAND) per angle
- Expression tree compiler: `Expr` pool allocator, `expr_var/not/nand/and/or/xor` constructors, `eval_expr` recursive evaluator — full adder built from expression trees
- NOT_WORD = {-1,-1,-1,-1,-1,-1} (len 6), NAND_WORD = {-1,-1,-1,2,2} (len 5)

### Literature Touched
- Error analysis of topological/braid-based computation
- Phase transitions in computational gate fidelity
- Robustness of topological quantum computation (analogy: delta=0 as topological protection)
- Universal vs non-universal gate sets parameterized by algebraic structure

### Open Questions
- What is the precise critical |delta_c| where NAND fails? Is it a root of a known polynomial?
- Does cascade error amplify linearly or exponentially with circuit depth?
- Can error correction schemes exploit the smooth degradation profile?
- Is there an analogue of fault-tolerance threshold for braid circuits away from delta=0?
