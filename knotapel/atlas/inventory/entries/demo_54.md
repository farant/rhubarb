## Demo 54: DKC at ell=4 (delta=sqrt(2)) — Bracket over Z[zeta_16]

- **Status**: COMPLETE
- **File**: `demo_54_dkc_l4/main.c` (~1857 lines)
- **Tests**: ~40+ pass (Part A: 20 Z[zeta_16] arithmetic sanity checks; Part B: 5 Catalan number checks + identity presence; Part C: ~10 TL relation checks at n=2,3,4 including e_i^2=delta*e_i, e_i*e_j*e_i=e_i, far commutativity; Part D: ~5 braid generator inverse/relation checks; Part E: braid word evaluation checks; Part F: ~6 cross-validation checks matrix vs state-sum for Hopf, trefoil, figure-eight, 4-strand braids)
- **Depends on**: Demo 53 (TL at ell=3/delta=1, planar matching enumeration), Demo 35 (original TL matrix framework), Demos 48-50 (DKC at delta=0, parity wall context)
- **Feeds into**: DKC analysis at non-zero delta; tests whether delta=0 phenomena (axiality, entanglement vanishing, compression) generalize or are regime-specific

### Headline
Two-source hypothesis test: constructs exact Z[zeta_16] cyclotomic arithmetic (8D ring) for TL/braid evaluation at ell=4 (delta=sqrt(2), Ising anyons), verifying all TL relations (e_i^2=sqrt(2)*e_i, delta-potent not nilpotent), braid relations, and matrix/state-sum cross-validation for Hopf link, trefoil, figure-eight, and 4-strand braids. Five experiments probe axiality, entanglement vanishing, compression ratios, bracket catalogs, and Gaussian integer structure at this new evaluation point.

### Key Results
- **Part A**: Z[zeta_16] exact arithmetic with 8-component basis {1, zeta, ..., zeta^7}, zeta^8=-1; A=zeta^5, A^{-1}=-zeta^3, delta=zeta^2-zeta^6=sqrt(2); delta^2=2 verified exactly; A has order 16; float cross-checks all pass to 1e-10
- **Part B**: Planar matching enumeration reproduces Catalan numbers C_2=2, C_3=5, C_4=14, C_5=42
- **Part C**: TL generators at delta=sqrt(2) satisfy e_i^2=delta*e_i (delta-potent, not nilpotent like delta=0 nor idempotent like delta=1), e_i*e_j*e_i=e_i (TL relation), e_1*e_3=e_3*e_1 (far commutativity) for n=2,3,4
- **Part D**: Braid generators sigma_i=A*I+A^{-1}*e_i; sigma*sigma^{-1}=I verified at n=2,3; braid relation s1*s2*s1=s2*s1*s2 verified at n=3
- **Part E**: Braid word evaluation: s1^2*s1^{-2}=I, s1^3 nonzero
- **Part F**: Matrix and state-sum bracket cross-validation for Hopf link (n=2), trefoil (n=2), figure-eight (n=3), 4-strand s1*s2*s3, 4-strand s1^2*s3^2; all matrix==state-sum
- **Experiment 1**: Bracket catalog — n=3 up to length 8 and n=4 up to length 5; distinct values enumerated with Z-axial, Z[i]-axial, and truly mixed classification; Z[i]-decomposition (a+bi)*zeta^c displayed for all values
- **Experiment 2**: Axiality test — checks if all bracket values at delta=sqrt(2) are Z[i]-axial (at most one pair of components (k, k+4) active); tested for n=2 lengths 1-10 and n=3 lengths 1-8
- **Experiment 3**: Entanglement vanishing — non-interleaving 4-strand braids (sigma_1, sigma_3 only) tested at delta=sqrt(2) up to length 5; checks whether entanglement vanishing from delta=0 persists or breaks
- **Experiment 4**: Compression ratios — distinct bracket values vs total braids for n=3 lengths 1-6; measures algebraic collapse rate at delta=sqrt(2) vs delta=0
- **Experiment 5**: Gaussian integer catalog — strips zeta^c axis from Z[i]-axial values, catalogs distinct Gaussian integers (a+bi); tracks negation pairs, conjugation pairs, and units (norm=1); precursor to DKC Boolean function search at ell=4

### Theorems/Conjectures
- VERIFIED: Z[zeta_16] arithmetic is consistent (zeta^8=-1, zeta^16=1, A*A^{-1}=1, delta^2=2, all float cross-checks pass)
- VERIFIED: TL_n at delta=sqrt(2) satisfies e_i^2=delta*e_i (delta-potent regime), e_i*e_j*e_i=e_i, far commutativity — all standard TL relations
- VERIFIED: Braid group representation at ell=4 (sigma_i=A*I+A^{-1}*e_i) satisfies inverse relations and Yang-Baxter/braid relation
- VERIFIED: Matrix method and state-sum method produce identical bracket values for all tested knots (Hopf, trefoil, figure-eight, 4-strand examples)
- TESTED: Z[i]-axiality — whether bracket values factor as (a+bi)*zeta^c at delta=sqrt(2) (contrasts delta=0 where all values are strictly axial in Z[zeta_8])
- TESTED: Entanglement vanishing — whether non-interleaving braids give zero bracket at delta=sqrt(2) (known to hold at delta=0)
- KEY QUESTION: Is delta=0 uniquely special, or do the DKC phenomena generalize across evaluation levels?

### Data
- Ring: Z[zeta_16] (8-dimensional over Z), basis {1, zeta_16, ..., zeta_16^7} with zeta_16^8=-1
- A = zeta_16^5 = e^{i*5*pi/8}, order 16; delta = sqrt(2) = zeta_16^2 - zeta_16^6
- TL dimensions: n=2→dim 2, n=3→dim 5, n=4→dim 14 (Catalan numbers)
- Braid generators: n-1 positive and n-1 negative per strand count
- Experiment search spaces: n=3 up to 4^8=65536 braids, n=4 up to 6^5=7776 braids
- Entanglement test: 4 choices (sigma_1±, sigma_3±) up to length 5
- Gaussian integer decomposition: values decompose as (a+bi)*zeta^c for c in {0,1,2,3}

### Code Assets
- `Cyc16` type: exact Z[zeta_16] cyclotomic arithmetic (8-component, zeta^8=-1); `cyc16_zero/one/make/zeta_power/add/neg/scale/mul/eq/is_zero/to_cx`
- `cyc16_a_power()`: computes A^k = zeta^{5k} with reduction
- `cyc16_delta_power()`: exact delta^k using delta^2=2 (even powers→integer, odd powers→integer*delta)
- `cyc16_is_axial()` / `cyc16_is_zi_axial()`: strict axiality (at most 1 nonzero component) and Z[i]-axiality (at most one pair (k,k+4) active)
- `cyc16_zi_decompose()`: factors Z[i]-axial value as (a+bi)*zeta^c
- `PlanarMatch` / `enumerate_basis()`: planar matching enumeration (from Demo 35/53), boundary order, segment-based recursive enumeration
- `compose_diagrams()`: diagram composition with closed loop counting
- `TLMat` type: matrices over Z[zeta_16] up to 42×42; `tlmat_zero/identity/add/scale/mul/eq/nonzero_count/print`
- `build_generator_matrix()`: TL generator e_i as matrix via diagram composition with delta^loops weight
- `build_braid_generator()`: sigma_i = A*I + A^{-1}*e_i (positive) or A^{-1}*I + A*e_i (negative)
- `BraidCrossing` type + `eval_braid_word()`: evaluates braid word as matrix product
- `decode_braid()`: integer index → crossing sequence
- `compute_bracket()`: matrix-method closure functional (sum over basis with delta^{trace_loops-1} weights)
- `state_sum_bracket()`: independent state-sum evaluation with diagram composition for cross-validation
- `GaussInt` type + `gi_catalog_contains()`: Gaussian integer tracking for Experiment 5
- `catalog_contains()` / `CatalogEntry`: distinct Cyc16 value tracking

### Literature Touched
- SU(2) level 2 = Ising anyons (3 simple objects at ell=4)
- Temperley-Lieb algebra at non-zero delta (delta-potent regime: e_i^2=delta*e_i)
- Z[zeta_16]: 16th cyclotomic integers (8-dimensional ring extension of Z)
- Gaussian integers Z[i] as subring of Z[zeta_16] (zeta^4 = i)
- Kauffman bracket at general A (not restricted to delta=0)
- Entanglement vanishing phenomenon (non-interleaving braids)
- Comparison of evaluation regimes: ell=3 (delta=1, binary), ell=4 (delta=sqrt(2), irrational), ell=∞ (delta=0, nilpotent)

### Open Questions
- Does Z[i]-axiality hold universally at delta=sqrt(2), or does it break at longer braid lengths?
- Does entanglement vanishing persist, partially break, or completely break at delta=sqrt(2)?
- How does the compression ratio (braids→distinct brackets) compare between delta=0 and delta=sqrt(2)?
- Can Gaussian integer negation pairs from Experiment 5 enable DKC-style Boolean function search at ell=4?
- What is the catalog size at ell=4 — richer or sparser than ell=∞ (delta=0)?
- How does the "two-source hypothesis" resolve: was the ell=3 binary collapse due to too few representations or delta=1 being special?
- Can the 4-strand s1^2*s3^2 bracket (entanglement test) distinguish delta=0 from delta=sqrt(2)?
