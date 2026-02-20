## Demo 24: Rational Tangle Arithmetic

- **Status**: COMPLETE
- **File**: `demo_24_rational_tangles/main.c` (~890 lines)
- **Tests**: ~7 pass (5 parts: A=2 alpha/beta formula + state-sum, B=1 multiplicativity, C=2 CF assembly + fraction tracking, D=1 angle correlation, E=1 Fibonacci analysis)
- **Depends on**: Demo 03-04 (bracket, TL algebra concepts), Demo 18-19 (delta=0 insights)
- **Feeds into**: Understanding bracket-rational correspondence; continued fraction approaches to tangle classification

### Headline
Implements Conway's rational tangle arithmetic in the Temperley-Lieb algebra TL_2, proving that at delta=0 the bracket polynomial literally counts twists (|D(n)| = n) and that continued fraction assembly builds rational tangles whose bracket values track p/q.

### Key Results
- **Part A (Integer tangles)**: n-twist tangles verified for n=0..20 at delta=0; alpha_n = A^n, beta_n = n*A^{n-2} confirmed; |D(n)| = n (denominator closure magnitude equals twist count); state-sum oracle cross-check matches TL2 algebra for n=1..10
- **Part B (Twist analysis)**: Twist is a linear/multiplicative map on (alpha, beta) space; alpha ratio is constant A; beta_n/n = A^{n-2}; matrix form at delta=0: [[A, 0], [A^{-1}, A]]
- **Part C (Continued fractions)**: 14 test rationals (0/1 through 7/11) built from continued fraction assembly; |D|/|N| ratio compared to p/q; partial convergent tracking shown for 8/13 = [0;1,1,1,1,2]; majority of test cases have |D|/|N| tracking p/q
- **Part D (Angle sweep)**: 256-angle sweep computing Pearson correlation between |D|/|N| and p/q for 12 test rationals; best angle found (may or may not be delta=0); bracket values compared at best angle vs delta=0
- **Part E (Fibonacci tangles)**: CF = [0;1,1,...,1] (all 1s) for k=1..12; these converge to F(k)/F(k+1) → 1/phi; |D| growth ratio tested for convergence to golden ratio (~1.618)

### Theorems/Conjectures
- **Conway's theorem**: Twist (T(r) = r+1) and Rotate (R(r) = -1/r) generate all rationals on 2-strand tangles
- At delta=0: N(T) = alpha, D(T) = beta (closures simplify)
- Integer n-twist: alpha_n = A^n, beta_n = n*A^{n-2}, so |D(n)| = n — **the bracket counts twists**
- Rotation swaps alpha and beta — corresponds to R(r) = -1/r in tangle arithmetic
- Twist recurrence at delta=0: alpha' = A*alpha, beta' = A^{-1}*alpha + A*beta (upper-triangular structure)

### Data
- 21 integer tangles (n=0..20) with alpha, beta, |N|, |D| at delta=0
- 14 test rationals with continued fractions and closure magnitudes
- Partial convergent tracking for 8/13 (6 steps)
- 256-angle sweep with Pearson correlation
- 12 Fibonacci tangles (k=1..12) with |D| growth ratios

### Code Assets
- `TL2` struct (alpha, beta pair in TL_2 basis {1, e1})
- `tl2_identity()`, `tl2_twist_pos()`, `tl2_twist_neg()`, `tl2_twist_n()` — tangle construction by twisting
- `tl2_rotate()` — 90-degree rotation (swaps alpha/beta)
- `tl2_num_closure()`, `tl2_den_closure()` — numerator and denominator closures
- `cf_to_double()` — evaluate continued fraction as floating-point
- `fraction_to_cf()` — convert p/q to continued fraction via Euclidean algorithm
- `tangle_from_cf()` — inside-out assembly of rational tangle from continued fraction
- `braid_loops_closure()` — state-sum loop counting with selectable closure type (numerator vs denominator)
- `braid_bracket_closure()` — state-sum bracket with closure type for verification

### Literature Touched
- Conway's rational tangle theory
- Temperley-Lieb algebra TL_2 (2-strand basis {1, e1})
- Continued fractions as tangle assembly programs
- Kauffman bracket of tangles (numerator/denominator closures)
- Fibonacci numbers and golden ratio in tangle sequences

### Open Questions
- Does exact |D|/|N| = p/q hold with a different normalization/closure convention?
- What is the best angle (beyond delta=0) for rational tracking, and why?
- How does this extend to 3-strand tangles and non-rational tangles?
- Connection to the fraction group (PSL(2,Z)) action on tangles?
