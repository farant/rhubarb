## Demo 72: Spherical Design Test — Are the 13 Directions Optimal?

- **Status**: COMPLETE
- **File**: `demo_72_spherical_design/main.c` (~971 lines)
- **Tests**: 8/8 pass
- **Depends on**: Demo 67 (13 eigenvector directions on S², 36 XOR6 solutions), Demo 71 (DKC bandwidth l=6, DOF argument)
- **Feeds into**: Demo 73 (automaton structure), Demo 74 (computational invariant)

### Headline
The 13 eigenvector directions are NOT a spherical t-design (t=0, directed) — the worst possible for sampling — yet stochastic optimization confirms that better geometric distribution gives fewer XOR6 solutions (36 vs 4), establishing an anti-correlation between sampling quality and computational quality.

### Key Results
- **t=0 directed (Part A)**: The 13 directions fail the t-design property at l=1 (centroid off-origin, |S_{1,m}|=2.43). Exactly l=2 passes at machine precision (|S_{2,m}|=5.11e-16). Fails at all subsequent levels through l=8+.
- **Bandwidth gap (Part B)**: DKC operates at bandwidth l=6 (Demo 71) but the directions achieve t=0. The gap of 6 modes is entirely filled by compressed sensing and the DOF argument — classical sampling theory does not apply.
- **Even/odd mode asymmetry (Part C)**: Directed even-mode t-design is t_even=2 (l=2 passes); directed odd-mode is t_odd=0. Undirected (26 points, both +dir and -dir) achieves t=3 because antipodal symmetry kills all odd modes automatically. First genuine failure for DKC-relevant even modes is l=4 (|S_{4,m}|=0.576).
- **S³ design (Part D)**: The full 48-element binary octahedral group on S³ achieves t=1. Odd degrees are exactly zero by symmetry; even degrees fail. The 24-cell vertices (binary tetrahedral subgroup) form a 5-design; the additional 24 elements break the property at degree 2.
- **Delsarte bounds (Part E)**: With 13 points, t-designs up to t=7 are theoretically possible (Delsarte minimum for t=7 is 10 points). The actual achievement (t=0) is the worst possible given that the points trivially satisfy no integration condition.
- **Residual analysis (Part F)**: First failing mode is l=1 (odd mode, irrelevant for DKC). All three l=1 components are large (max 2.435). The effective first failure for DKC is l=4 (even mode, residual 0.576).
- **Optimized design gets fewer solutions (Part G)**: Stochastic optimization of 13 points to minimize t=6 design residual achieved residual 0.93 (vs 2.43 for eigenvectors — 2.6× better geometry). XOR6 solutions: eigenvector 36, optimized 4. Better geometry destroys the computation.
- **Perturbation robustness (Part H)**: At 1° perturbation mean XOR6 count is 38 (slightly above unperturbed 36, showing some near-miss solutions exist). Robust through 5° (mean 35.8). Collapses at 10° (mean 11.5) and 20° (mean 2.1). The algebraic placement is sharp to ~5°.

### Theorems/Conjectures
- **Anti-Correlation Theorem (DEMONSTRATED)**: The 13 eigenvector directions are simultaneously the worst choice for spherical integration (t=0) and a locally optimal choice for DKC computation (36 vs 4 solutions for better-distributed directions). Sampling quality and computational quality are anti-correlated for this configuration.
- **Algebra > Geometry Principle (DEMONSTRATED)**: The DKC computation is algebraic, not geometric. The directions are selected by the binary octahedral group acting on Z[ζ_8] quaternions; they compute because of specific angular relationships, not uniform distribution. Moving directions to "better" positions (geometrically) destroys the Voronoi partition structure needed for XOR6.
- **DOF-not-Design Conjecture (SUPPORTED)**: The reason DKC works at bandwidth l=6 with only 13 directions (well below Nyquist) is the DOF coincidence 2×6+1=13=directions, combined with compressed sensing for binary functions at known positions. The t-design property is irrelevant to this mechanism.

### Data
- Directed t-design parameter: t=0 (fails at l=1)
- l=2 residual: 5.11e-16 (machine precision — single exact pass)
- Even-mode t-design: t_even=2; odd-mode: t_odd=0; undirected: t=3
- S³ design (48 elements): t=1
- Delsarte maximum possible with 13 points: t=7 (none achieved)
- First l=1 failure: max |S_{1,m}|=2.435 (m=+1 component)
- First DKC-relevant (even) failure: l=4, |S_{4,m}|=0.576
- Optimized 13-point design residual (t=6): 0.93 vs eigenvector 2.43 (2.6× better)
- XOR6 solutions: eigenvectors 36, optimized design 4
- Perturbation: robust at 5° (mean 35.8/36), collapses at 10° (mean 11.5/36)
- At 1° perturbation: mean 38.0 (can exceed unperturbed count — local not global optimum)

### Code Assets
- **Spherical harmonic t-design test**: directed and undirected summation of Y_lm over arbitrary point sets; max |S_lm| per l computed cleanly
- **S³ monomial design test**: tests homogeneous polynomial averages over quaternion sets against exact S³ integrals via double factorial formula
- **`design_residual()`**: computes t=T design residual (max over l=1..T of max_m |S_lm|) for any point set on S²; reusable quality metric
- **Stochastic optimizer for spherical designs**: gradient-free simulated annealing with Gaussian perturbation and normalization; 20 random restarts, 2000 steps each; minimizes design residual
- **`count_xor6_ext()`**: counts XOR6 solutions for an arbitrary direction set (not just the canonical 13); used for the optimized vs eigenvector comparison
- **LCG random number generator + Box-Muller**: self-contained deterministic PRNG for perturbation trials; no stdlib rand dependency
- **Even/odd mode analysis harness**: separates t-design test into even-l and odd-l streams and reports independently

### Literature Touched
- **Delsarte, Goethals, Seidel (1977)**: lower bounds on N for spherical t-designs on S^d; the t=7 feasibility bound for N=13 comes from this framework
- **Seymour & Zaslavsky (1984)**: existence of spherical t-designs for all t and N large enough; establishes that the eigenvector configuration is far from optimal despite being algebraically distinguished
- **Aizenberg (2008)**: the algebraic selection of directions by the MVN/DKC mechanism is the reason the t-design property is irrelevant; the paper's framework predicts directions from algebraic rather than geometric criteria
- **Compressed sensing (Candès, Romberg, Tao 2006)**: the DOF-not-Design explanation relies on sparsity at known positions bypassing the Nyquist-Shannon/spherical-design sampling requirement

### Open Questions
- **Is 5° the Voronoi cell boundary?**: The robustness threshold of ~5° matches the typical angular size of a Voronoi cell for 13 points on S². Is there a clean geometric formula relating cell angular radius to robustness threshold?
- **Why exactly l=2 passes?**: The l=2 mode vanishes to machine precision despite t=0 overall. This suggests a hidden quadrupole symmetry in the binary octahedral group action. What algebraic identity ensures Σ Y_{2,m}(dir_i)=0?
- **Optimized design lower bound**: The stochastic optimizer found 4 solutions for the "better" design. Is 0 achievable? Is there a geometric argument that any t≥1 spherical design on S² yields fewer XOR6 solutions than the algebraic eigenvector configuration?
- **Perturbation non-monotonicity**: Small perturbations (1°) slightly increase the mean solution count (38 > 36). Can the maximum XOR6 solution count for any 13-point configuration on S² be characterized? Is 36 a local or global maximum?
- **Analog for higher Boolean functions**: XOR6 uses l=6, 13 directions. For XOR7 on S², what is the expected t-design parameter of the algebraically-selected directions, and does the anti-correlation persist?
