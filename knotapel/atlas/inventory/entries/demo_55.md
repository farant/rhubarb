## Demo 55: DKC Boolean Search at ell=4 (delta=sqrt(2))

- **Status**: COMPLETE
- **File**: `demo_55_dkc_boolean/main.c` (~1062 lines)
- **Tests**: ~7 checks (4 parts: A=4 catalog verification + Z[i]-axial + algebra identities, B=1 XOR achievable, C=1 NPN count, D=1 parity achievable)
- **Depends on**: Demo 54 (bracket catalog at delta=sqrt(2), Z[i]-axial structure), Demo 48/50 (delta=0 baseline: forward DKC zoo, activation zoo)
- **Feeds into**: Understanding Boolean computability at non-zero delta; multiplicative vs additive DKC encoding comparison; ell-parameterized DKC theory

### Headline
Searches for Boolean functions computable by DKC neurons at ell=4 (delta=sqrt(2)) using multiplicative encoding (z = w1^x1 * w2^x2 * w3^x3) across five activation function families, comparing the achievable function landscape to delta=0 results from Demos 48-50.

### Key Results
- **Part A (Catalog)**: State-sum bracket catalog built at delta=sqrt(2) using Z[zeta_16] exact arithmetic (A = zeta_16^5); enumerates n=2 (len 1-10), n=3 (len 1-6), n=4 (len 1-6); all values confirmed 100% Z[i]-axial; algebraic identities verified (A * A^{-1} = 1, delta^2 = 2)
- **Part B (2-input search)**: 10 activation functions tested on all catalog pairs with multiplicative encoding; quartet (1, w2, w1, w1*w2) produces 4-bit truth tables; activations include Re>0, Im>0, split-sigmoid, sector k=2/4/6/8, magnitude tau=0.5/1.0/1.5; XOR/XNOR confirmed achievable
- **Part C (3-input search)**: 8 activation functions tested on all catalog triples; octet (1, w3, w2, w2w3, w1, w1w3, w1w2, w1w2w3) produces 8-bit truth tables; NPN classification into 13 non-trivial classes; per-activation reachability reported
- **Part D (Analysis)**: k-sector sweep k=2..8 with 3-input multiplicative encoding; parity solution counts tracked per k; reachable NPN class counts per k; comparison with delta=0 additive encoding results

### Theorems/Conjectures
- **Multiplicative encoding**: z(x) = w1^x1 * w2^x2 * ... * wn^xn (product of weights raised to binary input powers) is a natural encoding for delta != 0 because bracket values at non-zero delta have multiplicative structure (delta^k scaling)
- **Z[i]-axiality at delta=sqrt(2)**: Every bracket value equals (a+bi) * zeta_16^c for some Gaussian integer a+bi and power c — confirmed by Demo 54, verified here in catalog construction
- **Encoding comparison caveat**: Delta=0 (Demos 48-50) used additive encoding (z = sum wi*xi); delta=sqrt(2) uses multiplicative encoding — not directly comparable, but structural parallels in reachability hierarchy are informative
- **Bit 0 is fixed**: Under multiplicative encoding, (0,...,0) → z = 1 always, so bit 0 of the truth table is determined by the activation function's classification of z=1 (not a free parameter)

### Data
- Bracket catalog at delta=sqrt(2): distinct nonzero values (first 20 printed with Z[zeta_16] coordinates and magnitudes)
- 2-input: 16 possible truth tables × 10 activations, achievable counts per function per activation
- 3-input: 13 NPN classes × 8 activations, solution counts per class per activation
- k-sector sweep (k=2..8): reachable NPN class count and parity solution count per k
- Comparison table: delta=0 (64 catalog values, parity at k=6 with 906 solutions, additive) vs delta=sqrt(2) (multiplicative)

### Code Assets
- `Cyc16` struct: exact cyclotomic integer type Z[zeta_16] with 8-element basis {1, zeta, ..., zeta^7} where zeta^8 = -1
- `cyc16_zero()`, `cyc16_one()`, `cyc16_zeta_power()`, `cyc16_add()`, `cyc16_neg()`, `cyc16_mul()`, `cyc16_eq()`, `cyc16_is_zero()`, `cyc16_to_cx()`, `cyc16_print()` — full Z[zeta_16] arithmetic
- `cyc16_a_power()` — A^n where A = zeta^5
- `cyc16_delta_power()` — delta^k where delta = sqrt(2) = zeta^2 - zeta^6; even powers are integers (2^{k/2}), odd powers are multiples of delta
- `cyc16_is_zi_axial()` — checks if value has at most one active coordinate pair (Z[i]-axiality)
- `PlanarMatch` struct: planar matching diagram for n strands (2n endpoints)
- `make_identity_diagram()`, `make_generator_diagram()` — TL diagram constructors
- `compose_diagrams()` — diagram composition with glue-loop counting (returns number of closed loops)
- `trace_closure_loops()` — trace closure loop counting for bracket computation
- `state_sum_bracket()` — full state-sum bracket at delta=sqrt(2) using planar matching diagrams instead of union-find (exact Z[zeta_16] arithmetic)
- `split_sigmoid_classify()` — XOR-like activation: sigma(Re)*(1-sigma(Im)) + (1-sigma(Re))*sigma(Im) > 0.5
- `sector_classify()`, `magnitude_classify()`, `re_positive_classify()`, `im_positive_classify()` — multiple activation functions with uniform interface
- `search_2input()`, `search_3input()` — exhaustive search with multiplicative encoding, per-activation truth table counting
- NPN classification machinery (from Demo 48/50): `npn_init()`, `npn_transform()`, `npn_build_classes()`

### Literature Touched
- Multi-valued neuron (MVN) with k-sector classification applied to multiplicative encoding
- Kauffman bracket at non-zero delta (ell=4, Jones-Wenzl level)
- Z[zeta_16] cyclotomic ring as algebraic setting for ell=4 bracket values
- Gaussian integers Z[i] as structural constraint on bracket catalog
- Multiplicative vs additive neural encoding for Boolean functions
- Planar matching diagrams for Temperley-Lieb algebra composition

### Open Questions
- How does the reachability landscape at delta=sqrt(2) compare quantitatively to delta=0? (Encoding difference makes direct comparison difficult)
- Does multiplicative encoding have inherent advantages for certain function classes (e.g., AND-like functions where product structure is natural)?
- At what ell values does parity first become achievable under multiplicative encoding?
- Can additive encoding be used at delta=sqrt(2), and if so, how does reachability change?
- Does the four-tier hierarchy from Demo 50 (k=2/3/4/6) persist under multiplicative encoding, or does the tier structure shift?
