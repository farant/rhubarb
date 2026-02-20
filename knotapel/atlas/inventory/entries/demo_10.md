## Demo 10: Numerical Evaluation Engine

- **Status**: COMPLETE
- **File**: `demo_10_numerical/main.c` (~956 lines)
- **Tests**: ~50+ pass (6 parts: A=6 complex arith, B=5 Yang-Baxter/inverses, C=5 oracle match, D=scaling n=2..8 YB+far-comm+oracle, E=10 multi-angle, F=sample-point classification)
- **Depends on**: Demo 04 (quantum trace formula, R-matrix), Demo 05 (symbolic bracket sign convention)
- **Feeds into**: All higher demos needing numerical knot evaluation at scale (DKC pipeline, classification experiments)

### Headline
Replaces symbolic Laurent polynomials with numerical complex evaluation (A = e^{i*theta}), enabling 50-100x memory reduction and scaling from n=5 (32x32) to n=8 (256x256) and beyond, validated against state-sum oracle at multiple angles.

### Key Results
- Complex arithmetic library validated: unit circle, powers, A^10 = 1 for A = e^{i*pi/5}
- Numerical R-matrix (4x4) and Yang-Baxter (8x8) verified at tolerance 1e-9
- 5 braids (identity, trefoil, figure-eight, mirror trefoil, single crossing) match state-sum oracle numerically for n=3
- **Scaling validated n=2 through n=8**: Yang-Baxter, far commutativity, and oracle match at each dimension (up to 256x256)
- Multi-angle validation: trefoil and figure-eight match oracle at 5 different angles (pi/5, pi/7, pi/10, pi/3, 0.7)
- **Classification experiment**: determines minimum number of sample evaluation points on unit circle to distinguish a 6-knot test set (unknot, trefoil, mirror trefoil, figure-eight, torus(2,5), granny knot)

### Data
- 6-knot test set: unknot, trefoil, mirror trefoil, figure-eight, torus(2,5), granny knot
- 8 sample angles: pi/5, pi/7, pi/10, pi/3, 0.7, 1.1, 2.0, pi/13
- Bracket vectors printed for first 3 angles per knot
- Memory: Poly (~808 bytes) replaced by Cx (16 bytes) — 50x reduction per matrix entry

### Code Assets
- `Cx` complex number type with full arithmetic: make, zero, one, real, add, sub, mul, div, neg, abs, abs2, exp_i, pow_int (binary exponentiation), near (approx equality), print
- `CMat` heap-allocated NxN complex matrix: alloc, free, zero, id, mul, scale, add, qtrace, tensor, eq_approx
- `CMAT(m,i,j)` macro for row-major access
- `build_R4()` — numerical 4x4 R-matrix and inverse for given A
- `build_Ri()` — general n-strand generator I^{x(i-1)} x R x I^{x(n-i-1)} via tensor products
- `build_Kn()` — K^{xn} diagonal weights for quantum trace at arbitrary n
- `braid_product_n()` — braid word to matrix product at arbitrary dimension
- `numerical_bracket()` — full pipeline: braid -> matrix product -> quantum trace -> bracket with sign correction (-1)^{n+1}
- `braid_bracket_at()` — state-sum oracle evaluated at specific complex A (for validation)
- Pattern: heap-allocated matrices with explicit free (scales to large n)

### Literature Touched
- Numerical evaluation of Kauffman bracket / Jones polynomial
- Quantum trace formula evaluated on unit circle
- Sample-point knot classification (bridge to DKC — Discrete Knotted Computation)
- Yang-Baxter equation and far commutativity as numerical consistency checks

### Open Questions
- How many sample points suffice for larger knot tables? (experiment seeded here, extended in later demos)
- Numerical stability at very large n (n=10+)?
- Which angles are optimal for discrimination?
