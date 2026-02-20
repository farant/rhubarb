## Demo 57: Additive Encoding DKC

- **Status**: COMPLETE
- **File**: `demo_57_additive_dkc/main.c` (~1140 lines)
- **Tests**: ~8 checks across 5 parts (A-E): catalog verification, 2-input encoding comparison, 3-input additive NPN search, head-to-head multiplicative vs additive, union analysis
- **Depends on**: Demo 56 (five-lens analysis predicting additive encoding dissolves 0x1B/0x06 wall), Demo 55 (ell=4 bracket catalog at delta=sqrt(2))
- **Feeds into**: Further encoding/activation investigation in DKC program

### Headline
Tests Demo 56's prediction that additive encoding (z = x1*w1 + x2*w2 + x3*w3) should dissolve the 0x1B and 0x06 wall that exists with multiplicative encoding + half-plane (Re>0) activation. **Key discovery: the prediction was wrong** — the wall is about ACTIVATION CONVEXITY, not encoding geometry. Additive is actually WORSE (5/13 vs mult's 11/13 with Re>0) because additive sums preserve convexity of half-planes, while multiplicative products can rotate out of half-planes (anti-convex). Non-convex activations (split-sigmoid, sector) work with either encoding.

### Key Results
- **Catalog** (Part A): builds same ell=4 (delta=sqrt(2)) bracket catalog as Demo 55 using Z[zeta_16] exact arithmetic. All values are Z[i]-axial. Verifies A*A^{-1}=1, delta^2=2.
- **2-input comparison** (Part B): with Re>0 activation, multiplicative achieves >=8/16 truth tables while additive is constrained by z(0,0)=0 forcing bit 0=0 (all achieved TTs are even)
- **3-input additive NPN search** (Part C): exhaustive catalog^3 triples across 8 activations. With Re>0, 0x1B and 0x06 remain UNREACHABLE — wall holds. Additive reaches only ~5/13 NPN classes with Re>0.
- **Head-to-head** (Part D): for each of 8 activations, compares multiplicative vs additive reachability. With Re>0: additive is a strict subset of multiplicative (0 add-only classes). With non-convex activations (split-sigmoid, sector): both encodings reach all 13.
- **Union analysis** (Part E): Re>0 union of mult+add = 11/13 (the two wall classes 0x1B and 0x06 unreachable by BOTH encodings). Non-convex activations reach all 13 with either encoding alone.

### Prediction Scorecard
| # | Prediction | Result | Notes |
|---|-----------|--------|-------|
| 1 | Re>0 + additive reaches 0x1B and 0x06 | REFUTED | Wall is convexity, not encoding |
| 2 | Some multiplicative-easy become additive-hard | CONFIRMED | Additive only 5/13 vs mult 11/13 at Re>0 |
| 3 | Union covers all 13 NPN with Re>0 | REFUTED | Union = 11/13, wall classes unreachable by both |
| 4 | z(0,0,0) = 0 constrains bit 0 to 0 | CONFIRMED | All additive TTs are even |

### Theorems/Conjectures
- **Convexity thesis**: the 0x1B/0x06 wall is fundamentally about activation convexity, not encoding geometry. Half-planes {z : Re(z) > 0} are convex sets. Additive sums of points in a convex set stay in that convex set (preserve convexity). Multiplicative products of complex numbers can rotate out of half-planes (anti-convex). Non-convex activations (split-sigmoid, sector) bypass the wall entirely.
- **Additive zero constraint**: z(0,...,0) = 0 always, constraining bit 0 of the truth table to 0 for all activations. This eliminates half the truth table space immediately.
- **Encoding complementarity is minimal at Re>0**: additive is a strict subset of multiplicative at Re>0 (no add-only classes). The two encodings are NOT complementary with convex activations.

### Data
- Same bracket catalog as Demo 55 (ell=4, delta=sqrt(2), Z[zeta_16] exact arithmetic)
- Catalog built from n=2 (lengths 1-10), n=3 (lengths 1-6), n=4 (lengths 1-6) braids, capped at 512 values
- 8 activations: Re>0, Im>0, split-sigmoid, sector k=2/4/6/8, magnitude tau=1
- 2-input: catalog^2 pairs, 16 possible truth tables
- 3-input: catalog^3 triples (can be large), 256 truth tables, 13 NPN classes
- Encoding modes: multiplicative z = w1^x1 * w2^x2 * w3^x3 vs additive z = x1*w1 + x2*w2 + x3*w3

### Code Assets
- **Z[zeta_16] arithmetic**: `Cyc16` type (8 coefficients), `cyc16_add/neg/mul/eq/is_zero`, `cyc16_zeta_power`, `cyc16_a_power` (A = zeta^5), `cyc16_delta_power`, `cyc16_to_cx`, `cyc16_is_zi_axial`
- **`search_3input_batch()`**: unified search engine that runs ALL 8 activations in a single pass through catalog triples, parameterized by encode_mode (ENCODE_MUL or ENCODE_ADD). Computes 8 truth tables per triple simultaneously. Aggregates per NPN class.
- **NPN classification**: `npn_init()`, `npn_build_classes()`, `npn_transform()` — standard 96-transform canonicalization for 3-input functions
- **Activation wrappers**: uniform `int classify(Cx, int)` signature for all 8 activations (Re>0, Im>0, split-sigmoid, sector, magnitude)
- **State-sum bracket**: full planar matching + diagram composition + trace closure, evaluating at delta=sqrt(2)
- Reuses: planar matching, diagram composition, trace closure, braid decoding

### Literature Touched
- Convexity theory in activation functions and decision boundaries
- Linear separability: additive encoding with half-plane activation = linear classifier (can only implement linearly separable functions)
- Connection to XOR problem in classic perceptron theory (additive + threshold = linear = can't do XOR)
- Forward DKC: bracket values as codebook, multiplicative vs additive encoding as two "channel" options

### Open Questions
- Is there an encoding between additive and multiplicative that provides better complementarity with convex activations?
- Does the convexity insight generalize to 4+ inputs, or do higher dimensions introduce new phenomena?
- Can lattice-constrained additive encoding (restricting sums to stay on the Eisenstein/cyclotomic lattice) change the reachability picture?
- What is the theoretical minimum number of activation functions needed to cover all 13 NPN classes with additive encoding?
