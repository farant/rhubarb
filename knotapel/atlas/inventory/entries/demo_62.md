## Demo 62: Analytical Proof — Why 0x06 and 0x1B Are Unreachable

- **Status**: COMPLETE
- **File**: `demo_62_analytical_proof/main.c` (~600 lines)
- **Tests**: 7 checks: 0x1B never achieved numerically, 0x1B absent in exhaustive N=120, 0x06 never achieved numerically, 0x06 absent in exhaustive N=120, 0x06 orbit = 24, 0x1B orbit = 24, total unreachable = 48
- **Depends on**: Demo 61 (computational proof that exactly 11/13 NPN classes reachable by half-plane activations on multiplicative encodings)
- **Feeds into**: Closes the half-plane wall investigation definitively; theoretical foundation for DKC activation design

### Headline
Provides two independent analytical proofs that exactly 0x06 (~A(B^C)) and 0x1B (CROSS) are unreachable by semicircle (half-plane) activations on multiplicative encodings — pure algebra with no search, working for continuous angles (which subsumes all discrete cases). The 0x1B proof is a 3-step algebraic contradiction (lower bound > upper bound on phi1); the 0x06 proof is a four-semicircle parallelogram argument showing all gaps < pi, making the intersection empty.

### Key Results
- **Proof A — 0x1B (CROSS)**: Direct algebraic contradiction in 3 steps.
  - Step 1: WLOG set theta=0 (since 0 is in TRUE set). phi1, phi3 in [0,pi), phi2 in [pi,2pi).
  - Step 2: Write phi2 = pi+x, phi3 = y. Constraint phi2+phi3 in S forces x+y >= pi (I). Constraint phi1+phi2+phi3 in S-bar forces phi1 >= 2pi-x-y (II). Constraint phi1+phi2 in S-bar forces phi1 < pi-x (III).
  - Step 3: Combining (II) and (III): 2pi-x-y <= phi1 < pi-x, which requires 2pi-y < pi, i.e. pi < y. But y = phi3 < pi. **CONTRADICTION.** QED.
- **Proof B — 0x06 (XOR-AND)**: Four-semicircle parallelogram argument.
  - The 4 constraints on phi1 (phi1 not in S, phi1+alpha not in S, phi1+beta not in S, phi1+alpha+beta not in S) define 4 semicircles whose midpoints form a parallelogram on the circle.
  - Key inequality: alpha+beta > pi (forced by phi2+phi3 not in S).
  - This makes ALL four gaps between consecutive midpoints < pi.
  - Intersection of 4 semicircles is non-empty iff some gap >= pi. Since all gaps < pi, intersection is **EMPTY.** QED.
- **NPN symmetry**: proofs extend to full NPN orbits via input permutation (relabeling phi_i), input negation (reflecting angles), and output negation (swapping S with S-bar). Orbit of 0x06 = 24 truth tables, orbit of 0x1B = 24. Total unreachable = 48/256.
- **Numerical verification**: hand-picked near-miss configurations tested at 1000 theta values — neither 0x06 nor 0x1B ever achieved.
- **Exhaustive verification**: all (p1,p2,p3) in Z_120^3 with all 8 possible threshold anchors — neither NPN class found, confirming both proofs.
- **Gap analysis demo**: shows parallelogram gap structure for near-miss configurations, illustrating why the proof works — the "big gap" always stays below pi.

### Theorems/Conjectures
- **Theorem (0x1B)**: For any angles phi1, phi2, phi3 in [0,2pi) and any semicircle S = [theta, theta+pi), the truth table of in_semi(x1*phi1 + x2*phi2 + x3*phi3, theta) cannot be NPN-equivalent to 0x1B. Proof by direct algebraic contradiction (interval squeeze).
- **Theorem (0x06)**: For any angles phi1, phi2, phi3 in [0,2pi) and any semicircle S, the truth table cannot be NPN-equivalent to 0x06. Proof by parallelogram gap argument (4 semicircles with all gaps < pi have empty intersection).
- **Corollary**: Exactly 11/13 NPN classes of 3-input Boolean functions are realizable by semicircle thresholding of angle sums. The wall is geometric, not computational — no refinement of the angular grid will ever find these two classes.
- **Key insight**: The additive structure of multiplicative encodings (z = w1^a * w2^b * w3^c, so angles ADD) creates a parallelogram constraint on the circle that is incompatible with the alternating parity structure of 0x06 and 0x1B.

### Data
- Near-miss numerical tests: 5 configurations per target, 1000 theta sweeps each
- Exhaustive integer search: N=120 (120^3 = 1,728,000 triples x 8 threshold anchors)
- NPN orbit sizes: 0x06 has 24 members, 0x1B has 24 members, 48 total unreachable out of 256

### Code Assets
- **`prove_0x1B()`**: prints the 3-step algebraic proof with full reasoning, then numerically verifies + exhaustive integer check
- **`prove_0x06()`**: prints the parallelogram proof with full reasoning, then numerically verifies + exhaustive integer check
- **`gap_analysis_demo()`**: for near-miss configurations, computes and displays the parallelogram gap structure (alpha, beta-alpha, alpha, 2pi-alpha-beta) showing all gaps < pi
- **`npn_symmetry_argument()`**: explains how input permutation, input negation, and output negation extend proofs to full NPN orbits; verifies orbit sizes
- **`truth_table()`**: computes 8-bit truth table from three angles and a threshold angle
- **`in_semi()`**: semicircle membership test (angle in [theta, theta+pi) mod 2pi)
- **`wrap()`**: angle normalization to [0, 2pi)
- **NPN canonicalization**: standard 96-transform minimum (reused from Demo 61)

### Literature Touched
- Semicircle / half-plane activation functions and their geometric constraints
- NPN equivalence and orbit structure for 3-input Boolean functions
- Parallelogram / gap analysis on the circle (covering arguments)
- Connection to classical perceptron limitations: the wall is a higher-dimensional analog of the XOR problem, but for multiplicative (angular) encodings rather than additive (linear) ones
- Intersection of semicircles on S^1: non-empty iff max gap >= pi

### Open Questions
- Can the parallelogram argument be generalized to 4+ input functions to characterize unreachable NPN classes at higher arities?
- Is there a unified proof that handles both 0x06 and 0x1B simultaneously (they use different argument structures)?
- What is the exact characterization of reachable NPN classes for other activation geometries (sectors, magnitude thresholds)?
- Does the 11/13 ratio have a deeper algebraic meaning (e.g., related to the structure of the Boolean lattice or the symmetric group S_3)?
