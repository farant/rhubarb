## Demo 53: DKC at delta=1 — Bracket Catalog over Z[zeta_3]

- **Status**: COMPLETE
- **File**: `demo_53_dkc_d1/main.c` (~1474 lines)
- **Tests**: ~40+ checks across 6 parts (A-F) + 4 experiments: Eisenstein arithmetic, planar matchings, TL generators at delta=1, braid generators, braid word evaluation, closure cross-validation, bracket catalog, axiality stress test, entanglement vanishing, compression ratios
- **Depends on**: Demo 35 (delta=0 bracket catalog over Z[zeta_8], strict axiality), Demo 15 (delta-zero optimal angles)
- **Feeds into**: Later demos comparing DKC behavior across different roots of unity

### Headline
Explores the Kauffman bracket at delta=1 (A = omega = e^{i*2pi/3}, cube root of unity), where values live in Z[omega] (Eisenstein integers) instead of Z[zeta_8], TL generators become idempotent (e_i^2 = e_i) instead of nilpotent, all smoothing states contribute (delta^L = 1), and two key delta=0 phenomena — strict axiality and entanglement vanishing — are tested for breakdown.

### Key Results
- **Eisenstein arithmetic** (Part A): verifies omega^3 = 1, omega^2 + omega + 1 = 0, conjugation (norm = a^2 - ab + b^2), A^n power cycling (mod 3), float cross-checks, axiality classification (multiples of 1, omega, or omega^2)
- **Planar matchings** (Part B): enumerates Catalan-number basis sets (C_2=2, C_3=5, C_4=14, C_5=42), identical to Demo 35 (pure combinatorics, ring-independent)
- **TL generators at delta=1** (Part C): e_i^2 = e_i (idempotent!) verified for n=2,3,4. Braid relations e_i*e_j*e_i = e_i (for |i-j|=1) verified. Far commutativity e_1*e_3 = e_3*e_1 verified. At delta=1, every column has exactly one 1 (nonzero count = dim)
- **Braid generators** (Part D): sigma * sigma^{-1} = I verified for n=2,3. Braid relation s1*s2*s1 = s2*s1*s2 verified.
- **Braid word evaluation** (Part E): s1^2 * s1^{-2} = I, trefoil (s1^3) nonzero
- **Closure cross-validation** (Part F): matrix method vs state-sum method agree for Hopf link, trefoil, figure-eight, 4-strand s1*s2*s3, and 4-strand s1^2*s3^2. At delta=1 state-sum simplifies: every state contributes (delta^{L-1} = 1)
- **Experiment 1 — Bracket catalog**: n=3 braids (lengths 1-8, 4 crossing choices) and n=4 braids (lengths 1-5, 6 crossing choices). Tracks distinct values, axiality breakdown (axial vs MIXED values)
- **Experiment 2 — Axiality stress test**: tests all n=2 braids (lengths 1-10) and n=3 braids (lengths 1-8) for axiality. Expects axiality to BREAK at delta=1 (mixed values = not scalar multiples of {1, omega, omega^2})
- **Experiment 3 — Entanglement vanishing**: non-interleaving braids (sigma_1 and sigma_3 only, in B_4, lengths 1-5). At delta=0 ALL such brackets are zero; at delta=1 expects NONZERO (entanglement vanishing breaks)
- **Experiment 4 — Compression ratios**: n=3 braids lengths 1-6, counts distinct TL matrices vs total braids to measure how many-to-one the braid→matrix map is

### Theorems/Conjectures
- **Idempotency at delta=1**: e_i^2 = delta * e_i = 1 * e_i = e_i. This is the defining algebraic difference from delta=0 (where e_i^2 = 0). All smoothing states survive, no loop-killing.
- **Axiality breakdown**: at delta=0 (Z[zeta_8]), every bracket value lies on a single cyclotomic axis (strict axiality — Demo 35 result). At delta=1 (Z[omega]), mixed values (off-axis Eisenstein integers) are expected.
- **Entanglement vanishing breakdown**: at delta=0, non-interleaving braids (using only far-apart generators) always produce bracket=0. At delta=1, delta^L = 1 means closed loops don't kill states, so non-interleaving braids can have nonzero bracket.
- **Ring comparison**: Z[omega] is 2-dimensional (a + b*omega, a,b in Z) vs Z[zeta_8] which is 4-dimensional. Simpler ring, but different algebraic properties.

### Data
- A = omega = e^{i*2pi/3}, delta = 1, A has order 3
- Eisenstein integers Z[omega]: elements a + b*omega with a,b in Z
- n=2 axiality test: up to length 10 (2^10 = 1024 braids)
- n=3 axiality test: up to length 8 (4^8 = 65536 braids)
- n=3 catalog: lengths 1-8 (up to 65536 braids), up to 1000 distinct bracket values stored
- n=4 catalog: lengths 1-5 (up to 7776 braids)
- Entanglement: n=4 non-interleaving, lengths 1-5 (4^k braids each)
- Compression: n=3, lengths 1-6, up to 2000 distinct TL matrices

### Code Assets
- **Eisenstein integer arithmetic**: `Eis` type (a + b*omega), `eis_add/sub/neg/mul/conj`, `eis_a_power` (A^n mod 3), `eis_is_axial` (test if value is scalar multiple of 1, omega, or omega^2), `eis_to_cx` (convert to floating-point complex)
- **Planar matching enumeration**: `enumerate_basis()`, `compose_diagrams()` with closed-loop counting — reused from Demo 35, ring-independent
- **TL matrix algebra over Eisenstein integers**: `TLMat` type (up to 42x42), `tlmat_zero/identity/add/scale_eis/mul/eq/is_zero/nonzero_count`
- **`build_generator_matrix_d1()`**: TL generator at delta=1 (every composition survives, coefficient always 1)
- **`build_braid_generator_d1()`**: sigma_i = A*I + A^{-1}*e_i with A=omega
- **`eval_braid_word()`**: multiplies sequence of braid generator matrices
- **`compute_bracket_d1()`**: matrix-method trace closure (sum identity column, delta^{L-1}=1)
- **`state_sum_bracket_d1()`**: direct state-sum over 2^n smoothing states (each contributes A^{a_power}, no delta suppression)
- **`decode_braid()`**: integer-to-crossing-sequence decoder for exhaustive enumeration

### Literature Touched
- Eisenstein integers Z[omega] and their arithmetic properties
- Kauffman bracket at roots of unity (cube root vs 8th root)
- Temperley-Lieb algebra at delta=1: idempotent generators, semisimple regime
- Entanglement vanishing in the Kauffman bracket (connection to nilpotency at delta=0)
- Connected to the delta=0 vs delta!=0 dichotomy in topological quantum computation

### Open Questions
- How does the bracket catalog at delta=1 compare in size/structure to delta=0?
- What fraction of Eisenstein integer values are actually reachable by the bracket?
- Does the compression ratio stabilize, and how does it compare to delta=0?
- Is there a delta value between 0 and 1 where axiality gradually breaks, or is it a sharp transition?
- Can the idempotent TL algebra at delta=1 support any form of DKC (knot→computation correspondence)?
