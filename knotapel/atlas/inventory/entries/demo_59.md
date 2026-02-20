## Demo 59: Q=3 Potts DKC (ell=6, delta=sqrt(3))

- **Status**: COMPLETE
- **File**: `demo_59_q3_potts_dkc/main.c` (~1496 lines)
- **Tests**: ~15+ pass (Part A: zeta_24^24=1, zeta_24^12=-1, A*A^{-1}=1, A^24=1, delta=-A^2-A^{-2}, delta real and =sqrt(3), delta^2=3, delta^3 mul==formula, catalog >=20 values, >=2 basis coords used; Part B: additive even-only check, 2-input mult >=8; Part C: NPN=13; Part E: Re>0+mult >=11, additive Re>0 limited; Part F: complement symmetry, theta=0 matches Re>0)
- **Depends on**: Demo 55/57/58 (planar matching + diagram operations, prior ell values), Demo 54 (DKC at ell=4, Z[zeta_16], 11/13 Re>0 wall), Demo 56 (DKC at ell=5, Z[zeta_5]), Demos 48-50 (DKC at ell=∞/delta=0)
- **Feeds into**: cross-ell comparison of the 11/13 Re>0 wall; confirms wall is universal across evaluation levels; rotated half-plane sweep reveals orientation-dependent reachability

### Headline
Third independent verification of the 11/13 Re>0 wall: at ell=6 (Q=3 Potts model, delta=sqrt(3)), bracket values live in Z[zeta_24] (rank 8 over Z), and multiplicative Re>0 still reaches exactly 11/13 NPN classes with 0x1B and 0x06 unreachable — matching ell=4 (Z[i]) and ell=5 (Z[zeta_5]). Rotated half-plane sweep across 24 angles reveals orientation-dependent reachability with complement symmetry. Additive encoding is strictly weaker due to convexity constraint.

### Key Results
- **Part A**: Z[zeta_24] exact arithmetic with 8-component basis, reduction zeta^8=zeta^4-1 (Phi_24); A=zeta_24^7 (order 24); delta=zeta^2-zeta^6=sqrt(3); delta^2=3, delta^3 via formula matches multiplication; precomputed A^k table for k=0..23 (A^12=-1, A^{12+k}=-A^k); bracket catalog built from 2-4 strand braids up to length 6-10; axiality analysis: coefficient usage distribution, sub-ring axiality check (even-only coords → Z[zeta_12])
- **Part B**: 2-input encoding comparison — multiplicative (z=w1^x1*w2^x2) vs additive (z=x1*w1+x2*w2) with Re>0 activation; multiplicative achieves >=8/16 2-input functions; additive constrained: bit 0 always 0 (z(0,0)=0 has Re=0, not >0), so only even truth tables
- **Part C**: 3-input DKC search with both encodings across 8 activation functions (Re>0, Im>0, split-sigmoid, sector k=2/4/6/8, magnitude tau=1); full reachability grids printed; NPN classes 0x1B and 0x06 flagged as key wall indicators
- **Part D**: Head-to-head multiplicative vs additive comparison per activation; identifies MULT-ONLY, ADD-ONLY, BOTH, and NEITHER classes; per-activation best achievability with union counts; add-only classes listed
- **Part E**: Cross-ell headline comparison: ell=4 (11/13, catalog 56), ell=5 (11/13, catalog 116), ell=6 (11/13 with mult Re>0, 0x1B and 0x06 still NO); Im>0+mult may reach 12/13 (0x06 YES, 0x1B still NO), matching ell=4 and ell=5 patterns; wall HOLDS at third independent verification
- **Part F**: Rotated half-plane sweep — sigma_theta(z) = [Re(e^{i*theta}*z) > 0] for 24 angles (0-345 degrees in 15-degree steps); multiplicative encoding; tracks NPN reachability, 0x1B/0x06 status per angle; complement symmetry verified (theta and theta+180 give same NPN count); theta=0 confirms 11/13 matching Part C

### Theorems/Conjectures
- CONFIRMED (third time): Re>0 multiplicative encoding reaches exactly 11/13 NPN classes at ell=6 — 0x1B and 0x06 remain unreachable
- CONFIRMED: Wall is universal across evaluation levels (ell=4, 5, 6 all give 11/13 with same missing classes)
- CONFIRMED: Im>0 breaks 0x06 but not 0x1B (12/13 pattern matches ell=4 and ell=5)
- CONFIRMED: Additive encoding strictly weaker due to convexity constraint (z(0,0,0)=0 forces bit 0 = 0)
- CONFIRMED: NPN complement symmetry for rotated half-plane (theta and theta+180 have identical reachability)
- CONFIRMED: Z[zeta_24] arithmetic consistent (all delta power, A power, cross-checks pass)
- OBSERVED: Catalog size at ell=6 potentially smaller than ell=5 despite higher-rank ring ("smaller catalog despite rank-8 ring is surprising")
- TESTED: Rotated half-plane orientation dependence — some angles may achieve 13/13 (full reachability)

### Data
- Ring: Z[zeta_24] (8-dimensional over Z), basis {1, zeta_24, ..., zeta_24^7}, minimal polynomial Phi_24(x)=x^8-x^4+1
- A = zeta_24^7 = e^{i*7*pi/12}, order 24; A^{-1} = A^{23}
- delta = sqrt(3) = (0,0,2,0,0,0,-1,0) in Cyc24; delta^2 = 3; Q = delta^2 = 3 (3-state Potts)
- Delta powers: delta^{2k} = 3^k, delta^{2k+1} = 3^k * sqrt(3)
- A^k precomputed table: 24 entries, A^12 = -1
- Catalog: 2-4 strand braids, lengths 1-10/6/6 respectively, capped at 100000 braids per (n,length)
- 8 activation functions tested: Re>0, Im>0, split-sigmoid, sector k=2/4/6/8, magnitude tau=1.0
- 3-input search: catalog^3 triples, both multiplicative (z=w1^x1*w2^x2*w3^x3) and additive (z=x1*w1+x2*w2+x3*w3) encodings
- Rotated sweep: 24 angles at 15-degree increments, multiplicative encoding only
- Cross-ell reference: ell=4 (56 values, 11/13), ell=5 (116 values, 11/13), ell=6 (catalog size reported, 11/13)
- Negation coverage tracked (entries with negative in catalog)

### Code Assets
- `Cyc24` type: exact Z[zeta_24] cyclotomic arithmetic (8-component, Phi_24 reduction zeta^8=zeta^4-1); `cyc24_zero/one/add/neg/mul/eq/is_zero/to_cx/print`
- `A_table[24]`: precomputed A^k values for k=0..23; `cyc24_a_power()` with mod-24 lookup
- `cyc24_delta_power()`: exact delta^k using delta^2=3 (even→3^m integer, odd→3^m*sqrt(3))
- `PlanarMatch` / diagram operations: `make_identity_diagram()`, `make_generator_diagram()`, `compose_diagrams()`, `trace_closure_loops()` — shared topology-independent code from Demos 55/57/58
- `BraidCrossing` + `state_sum_bracket()`: state-sum bracket at ell=6 with Cyc24 weights
- `decode_braid()`: integer index → crossing sequence
- `build_catalog()`: enumerates 2-4 strand braids, stores distinct nonzero Cyc24 values + precomputed Cx doubles
- `Activation` table: 8 activation functions with uniform `int (*classify)(Cx, int)` signature; `wrap_split_sigmoid`, `wrap_sector`, `wrap_magnitude`, `wrap_re_positive`, `wrap_im_positive`
- `split_sigmoid_classify()`: smooth XOR of Re/Im sigmoids at threshold 0.5
- `sector_classify()`: k-sector angular classification (sector mod 2)
- `magnitude_classify()`: |z| > tau threshold
- NPN classification: `npn_transform()`, `npn_init()`, `npn_build_classes()`, `npn_canon[256]` — same as Demos 48-50
- `search_3input_batch()`: batch search over catalog^3 triples with both multiplicative and additive encoding, all 8 activations simultaneously; aggregates per NPN class
- `fn2_name()` / `fn3_name()`: truth table → human-readable function names (16 2-input, 15 3-input named functions)
- Part F rotated sweep: 24-angle sigma_theta with precomputed cos/sin, multiplicative encoding, complement symmetry verification

### Literature Touched
- Q=3 Potts model at criticality (delta^2=3, SU(2) level 4)
- Z[zeta_24]: 24th cyclotomic integers, minimal polynomial Phi_24(x)=x^8-x^4+1
- Temperley-Lieb at ell=6 (delta-potent regime: e_i^2=sqrt(3)*e_i)
- NPN classification of 3-input Boolean functions (13 non-trivial classes)
- Half-plane classifiers and rotated decision boundaries
- Multiplicative vs additive input encoding for complex-valued neural networks
- Convexity constraint on additive encoding (z(0,...,0)=0 ∈ boundary)
- Cross-ell universality: the 11/13 Re>0 wall as a structural invariant

### Open Questions
- Is the 11/13 Re>0 wall provably universal across ALL ell values, or could some ell break it?
- What algebraic property of 0x1B and 0x06 makes them specifically unreachable by Re>0 multiplicative?
- Does the rotated half-plane sweep reveal an angle where 13/13 is achieved? If so, what is the minimum such angle?
- Why is the ell=6 catalog smaller than ell=5 despite the higher-rank ring? Is this a search depth limitation or a structural property?
- Can the complement symmetry (theta ↔ theta+180) be extended to a full symmetry group acting on the reachability landscape?
- Is there a single activation function that achieves 13/13 across ALL ell values simultaneously?
- Connection between Q=3 Potts criticality (conformal field theory) and the Boolean function reachability hierarchy?
