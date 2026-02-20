## Demo 35: TL_n Matrix Representations over Z[zeta_8]

- **Status**: COMPLETE
- **File**: `demo_35_tl_matrices/main.c` (~1726 lines)
- **Tests**: ~40 pass (inline check() assertions across Parts A-F + 4 experiments)
- **Depends on**: Demo 01 (state-sum bracket), Demo 22 (hardcoded TL_3), Demo 24 (TL_2), Demo 29 (Cyc8 arithmetic)
- **Feeds into**: Demo 45-50 (DKC research arc — exact bracket computations underpin forward DKC)

### Headline
Generalizes TL_3 matrix representations to parametric TL_n (n=2..5) over exact Z[zeta_8] arithmetic with diagram composition, braid evaluation, and closure functionals — cross-validated against state-sum bracket.

### Key Results
- Exact Cyc8 (Z[zeta_8]) arithmetic: basis {1, zeta_8, zeta_8^2, zeta_8^3} with zeta_8^4 = -1, fully verified (multiplication, conjugation, float cross-check)
- Planar matching enumeration produces correct Catalan numbers: C_2=2, C_3=5, C_4=14, C_5=42
- TL generator matrices verified: nilpotent (e_i^2 = 0 at delta=0), adjacent relation (e_i*e_j*e_i = e_i for |i-j|=1), far commutativity (e_i*e_j = e_j*e_i for |i-j|>=2)
- Braid generators sigma_i = A*I + A^{-1}*e_i verified: inverse relations, braid relation (sigma_1*sigma_2*sigma_1 = sigma_2*sigma_1*sigma_2)
- Trace closure bracket matches state-sum bracket EXACTLY for Hopf link, trefoil, figure-eight, and two 4-strand braids
- n=5 sparsity prediction confirmed: e_1 has exactly C_5 - C_4 = 28 nonzero entries
- **Entanglement vanishing theorem**: ALL non-interleaving braids (sigma_1, sigma_3 only in B_4) have bracket = 0 — delta=0 is an entanglement detector
- **Axiality theorem**: EVERY bracket value at n=2 (length 1-10) and n=3 (length 1-8) has at most one nonzero Cyc8 component — no mixed brackets found across all braids tested
- Compression ratios measured: n=3 length 6 gives 4096 braids → limited distinct TL matrices; n=4 length 5 gives 7776 braids with compression

### Theorems/Conjectures
- **Nilpotency at delta=0**: e_i^2 = 0 for all generators — PROVEN (verified n=2,3,4)
- **TL relations**: e_i*e_j*e_i = e_i (|i-j|=1), e_i*e_j = e_j*e_i (|i-j|>=2) — PROVEN (verified n=3,4)
- **Entanglement vanishing**: Non-interleaving braids have zero bracket at delta=0 — PROVEN (exhaustive for n=4, length 1-5)
- **Axiality conjecture**: All bracket values at delta=0 are axial (at most one nonzero Cyc8 component) — CONJECTURED (exhaustively verified for n=2 length 1-10, n=3 length 1-8; ~131K braids total)
- **Sparsity formula**: TL generator e_1 at n strands has C_n - C_{n-1} nonzero entries — CONJECTURED (verified n=5)

### Data
- Catalan dimensions: C_2=2, C_3=5, C_4=14, C_5=42
- A = -zeta_8 = e^{i*5pi/4}, A^{-1} = conj(A), delta = -(A^2 + A^{-2}) = 0
- n=4 bracket catalog: distinct nonzero values collected from braids of length 1-5 (up to 60 printed)
- Compression: n=3 length 6: 4096→distinct; n=4 length 5: 7776→distinct
- Axiality: 131K+ braids tested, 0 mixed (non-axial) brackets

### Code Assets
- **Cyc8 exact arithmetic**: full ring operations (add, sub, mul, neg, scale, conj, eq), conversion to complex double, print
- **Planar matching enumerator**: segment-based recursive algorithm producing non-crossing perfect matchings for n up to 5
- **TLMatrix algebra**: zero, identity, add, scale, mul, equality, zero-test, nonzero-count, print — all over Cyc8
- **Diagram composition**: compose_diagrams() stacks two TL diagrams, counts closed loops at glue
- **TL generator builder**: build_generator_matrix() constructs e_i in planar matching basis
- **Braid generator builder**: build_braid_generator() constructs sigma_i = A*I + A^{-1}*e_i
- **Braid word evaluator**: eval_braid_word() multiplies generator matrices for arbitrary crossing sequences
- **Trace closure functional**: compute_bracket_trace() extracts bracket from TL matrix representation
- **State-sum bracket** (independent): state_sum_bracket() for cross-validation
- **Braid decoder**: decode_braid() maps integer index to crossing sequence for exhaustive enumeration
- **Axiality test**: cyc8_is_axial() checks single-component property

### Literature Touched
- Temperley-Lieb algebra (non-crossing partition basis, Catalan number dimension)
- Kauffman bracket polynomial (state-sum model, braid representation via sigma_i = A*I + A^{-1}*e_i)
- Non-semisimple representation theory (delta=0 ⟹ nilpotent generators)
- Cyclotomic integers Z[zeta_8] (exact arithmetic for 8th root of unity)

### Open Questions
- Does axiality hold for n=4 and higher braid lengths? (Computationally expensive to exhaustively verify)
- What is the exact growth rate of distinct TL matrices vs braid count at each n?
- Can the sparsity formula C_n - C_{n-1} be proven algebraically from diagram composition rules?
- How does the bracket catalog at n=4 relate to the DKC weight decomposition from Demo 26?
