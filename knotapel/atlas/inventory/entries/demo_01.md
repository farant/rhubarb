## Demo 01: Kauffman Bracket

- **Status**: COMPLETE
- **File**: `demo_01_kauffman_bracket/main.c` (~498 lines)
- **Tests**: 9/9 pass
- **Depends on**: none — foundation
- **Feeds into**: Demo 02 (matrix/TL approach comparison), Demo 03 (Yang-Baxter), Demo 04 (braid word compiler)

### Headline
Exact Kauffman bracket computation via state-sum model using integer Laurent polynomials — the brute-force oracle for all later demos.

### Key Results
- Laurent polynomial arithmetic (add, multiply, trim, equality) works correctly over integers — no floating point needed
- Trefoil bracket matches Kauffman's known value (code gives left-handed mirror: -A^{-5} - A^3 + A^7)
- Figure-eight bracket correct: A^8 - A^4 + 1 - A^{-4} + A^{-8}, confirmed palindromic (amphichiral)
- Hopf link bracket correct: -A^4 - A^{-4}
- Knot discrimination works: trefoil and figure-eight produce distinct brackets
- f-polynomial (writhe-normalized bracket) correct for trefoil and figure-eight

### Theorems/Conjectures
- VERIFIED: Kauffman bracket state-sum reproduces known values for 3_1, 4_1, and Hopf link
- VERIFIED: Amphichiral knot (4_1) has palindromic bracket and writhe = 0

### Data
- Trefoil (left-handed): <3_1> = -A^{-5} - A^3 + A^7
- Figure-eight: <4_1> = A^8 - A^4 + 1 - A^{-4} + A^{-8}
- Hopf link: <hopf> = -A^4 - A^{-4}
- Loop value: d = -A^2 - A^{-2}

### Code Assets
- `Poly` struct: Laurent polynomial with integer coefficients (lo exponent + coefficient array, MAX_TERMS=64)
- `p_zero`, `p_mono`, `p_trim`, `p_add`, `p_mul`, `p_eq`, `p_print`: complete polynomial algebra
- `Knot`/`Xing` structs: PD (planar diagram) notation for knot crossings with arc labels and crossing signs
- `count_loops()`: state-sum loop tracer — resolves crossings into A/B smoothings, traces cycles through arc connectivity
- `kauffman_bracket()`: full state-sum computation (O(2^n) over all crossing resolutions)
- `jones_from_bracket()`: writhe normalization to get f-polynomial
- `make_trefoil()`, `make_figure_eight()`, `make_hopf()`: PD notation constructors for standard knots
- Limits: MAX_X=16 crossings, MAX_ARCS=32

### Literature Touched
- Kauffman bracket state-sum model (Kauffman 1987)
- PD notation from KnotAtlas
- Jones polynomial via bracket normalization

### Open Questions
- PD notation chirality: code's "right-handed trefoil" PD gives left-handed bracket — convention subtlety noted but not fully resolved
- State-sum is O(2^n), limiting to ~16 crossings — Demo 02 addresses this with matrix approach
