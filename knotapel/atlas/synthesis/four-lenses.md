# Four Lenses on One Object

The DKC research is one thing seen from four directions. These are not four research programs that happen to share data. They are four coordinate systems on the same manifold. Every major result lives in at least two lenses; the deepest results live in all four.

---

## Lens 1: Representation Theory

**What the research looks like:** The Temperley-Lieb algebra TL_n(delta) at roots of unity is a non-semisimple finite-dimensional algebra whose radical structure controls what the Kauffman bracket can and cannot compute. The radical is not noise. It is the precise algebraic object that separates computable from incomputable Boolean functions.

**Central demos:** D35 (TL_n matrices over Z[zeta_8], the keystone), D39 (Parts G-J: three Gram forms, Markov RT truncation, next-level radical formula), D51 (radical anatomy at delta=0 -- the sandwich theorem, Loewy length 3, Catalan trace theorem, Peirce block matching), D52 (Chebyshev generalization across ell=2-6, universal nilpotency 3, cross-ell radical dimension formulas), D06 (Jones-Wenzl idempotents as module projectors).

**Core results belonging to this lens:**
- The sandwich theorem: rad^2(TL_{2k}(0)) is isomorphic to TL_{2k-1}(0) as a bimodule. Three Loewy layers with dimensional mirrors.
- Alternating semisimplicity: odd n is semisimple, even n is not. TL_3(0) is semisimple despite nilpotent generators.
- Universal nilpotency 3: rad^3 = 0 at ALL roots of unity tested (ell=2-6). The cellularity of TL algebras forces this.
- Fibonacci rank theorem: the total bilinear rank of the rad^2 generator equals F(ell-1). Fibonacci numbers arise from fusion rules in the Bratteli diagram.
- Cross-ell radical dimension formulas: rad(TL_ell) = 2ell-3, rad(TL_{ell+1}) = ell^2-ell-3.
- Markov RT truncation: Markov trace kills j >= ell-1, preserves j <= ell-2. The "shadow interpretation": fixpt = algebraic light, Markov = topological light.
- Next-level radical formula: rad(TL_{ell+1}) = ell^2-ell-3, now formally proven.

**Open questions specific to this lens:**
- The Galois symmetry conjecture: does the sandwich duality (top isomorphic to bottom) correspond to a Galois automorphism of Gal(Q(zeta_8)/Q)?
- Palindromic sector rank conjecture for even ell.
- ell=7 predictions: first cubic number field case, would validate all universal formulas. **CONFIRMED (D60).**
- Is there a generator construction that reaches E₆ (binary tetrahedral) and E₈ (binary icosahedral)? Those are the two exceptional ADE cases not yet found by the DKC construction.
- Power-of-two finiteness conjecture: are the finite cases exactly angles π/2^k for k ≥ 1? ζ₃₂ (θ = π/16) is the next untested prediction.

**New results (D66, D69):**
- Quaternionic / SU(2) representation of braids: D66 proves that the SU(2) braid representation with generators σ₁ → (1+i)/√2, σ₂ → (1−k)/√2 maps the braid group to exactly the 24 vertices of the 24-cell (binary octahedral group, 48 elements with negatives). This is a direct representation-theoretic object sitting above the scalar bracket.
- The bracket is the trace of this SU(2) representation after Markov normalization. The 24-cell is the image of the braid group under the quaternionic representation, explaining why the bracket catalog has 100 values but only 24 underlying quaternionic states.
- D69 proves the Direction Nesting Theorem: for any N, all SU(2) eigenvector directions in the ζ_N catalog appear identically in the ζ_{2N} catalog. Algebraic proof: squaring a quaternion preserves its rotation axis. Computationally verified ζ_8 → ζ_16 at 13/13 directions with 0.000° angular drift.
- D71 connects the representation space to spectral theory: the 13 eigenvector directions of the binary octahedral group are a spectral basis for spherical harmonics up to l=6 on S². The 13=13 theorem (2l+1=13 at l=6) is a representation-theoretic fact about the binary octahedral group's direction count.

**New results (D80):**
- ADE classification confirmed computationally: ζ₄ generates the binary dihedral group 2D₂ (order 8, D-series in ADE), and ζ₈ generates the binary octahedral group 2O (order 48, E₇ in ADE). These are the only two finite groups reachable under the two-perpendicular-axes generator construction.
- The quantum dimension [2]_q = q + q⁻¹ = i + (−i) = 0 at ζ₈ (q = i): ζ₈ is the most singular point in quantum group parameter space — the one finite exceptional case where the quantum dimension vanishes entirely.
- The non-monotone finiteness boundary (ζ₄ finite, ζ₆ infinite, ζ₈ finite, all others infinite) shows that algebraic degree of cos(π/N) does not predict finiteness. The finite cases are exactly the angles that are symmetries of the cube/octahedron.
- E₆ (binary tetrahedral) and E₈ (binary icosahedral) are not reachable under the tested generator construction; the ADE exhaustion for the DKC construction stops at E₇.
- D74 incomparability theorem: the DKC Voronoi cell and the Kauffman bracket are incomparable braid invariants — neither subsumes the other. At the braid level, 97.8% of bracket collisions are separated by cell; at the catalog level, 80% are separated. The combination yields 119 equivalence classes vs 100 bracket-alone and 14 cell-alone (+19% finer). Six distinct computational type vectors capture the DKC orbit structure.
- D73 additive-not-multiplicative: the multiplicative braid product formulation (using the braid as a group element) is structurally incapable of computing XOR6. The complement-all-bits map sends M → M⁻¹ which shares the same eigenvector cell, making parity invisible. The additive weight sum formulation computes XOR6 at 82.8% universal determinism — a structural constant identical across all 36 winning triples, following from the binary octahedral group acting transitively on the 13 directions.

**New results (D83, D84):**
- D83 Jones normalization as framing information loss: Jones normalization removes the writhe-dependent phase factor (−A³)^{−w}, stripping exactly the framing information that distinguishes the bracket from the Jones polynomial. From the representation-theory perspective, this is removal of a character-valued class function on the braid group: the writhe phase is a 1-dimensional representation (the abelianization of the braid group), and its removal collapses the writh-graded module structure. Jones XOR capacity = bracket XOR capacity − 2 at every tested root — the 2-level loss is the dimension of the writhe grading discarded.
- D84 non-semisimplicity as the computational resource: at ζ₄ (semisimple TL algebra, [2]_q = ±2), the catalog has 4 entries of which 3 are bracket-null — 75% null fraction and only XOR6 capacity. The semisimple case has nearly trivial structure. At ζ₈ (non-semisimple, [2]_q = 0), 37.5% null fraction and XOR8 capacity — the radical and its null states are not dead weight but the computational resource. The LCFT interpretation: bracket-null entries (Re(q) = 0) are the analogs of LCFT null states — zero two-point function but nonzero vectors. Their logarithmic partners (non-null entries on the same S² direction, the 3 shared coordinate-axis directions) carry computational content through the Jordan-cell coupling. The 6 null-only directions (cube-edge-midpoints) have no logarithmic partner — removing them (by removing the null entries) collapses XOR capacity from 8 to 6. Non-semisimplicity creates the Jordan-cell structure; Jordan cells are the mechanism of the null-state computational contribution.

**Connections to other lenses:**
- To Coding Theory: the radical is what the bracket evaluation quotients out. The semisimple quotient TL/rad is the "visible" part; the radical is the "invisible" lattice dimension. This is exactly the information-theoretic partition into accessible and inaccessible bits.
- To Quantum Mechanics: the radical corresponds to decoherence. The decoherence rate scaling ((2ell-3)/C_ell decreasing with ell) is WHY Fibonacci anyons (ell=5, 17% radical) are better for quantum computing than Ising anyons (ell=4, 36%). The SU(2) quaternionic representation (D66) lives between the abstract TL representation and the scalar bracket trace.
- To Approximation Theory: the quantum dimensions [n]_q satisfy the Chebyshev recurrence. The Jones-Wenzl idempotents ARE Chebyshev polynomials evaluated on TL generators. The radical appears precisely where the Chebyshev polynomial vanishes. The 13 eigenvector directions of the binary octahedral group provide the DOF count that sets the l=6 bandwidth minimum (D71).

---

## Lens 2: Coding Theory

**What the research looks like:** The bracket catalog is a codebook. Z[zeta_8] is a rank-4 lattice over Z, and each bracket value is a codeword. The question "which Boolean functions can be computed?" becomes "which truth tables are achievable via lattice codewords as neural network weights?" This is Nazer-Gastpar's compute-and-forward applied not to relay channels but to topological invariants.

**Central demos:** D29 (forward DKC -- exact Z[zeta_8] arithmetic, 65K+ XOR triples from lattice), D48 (100M quartet exhaustive search -- the parity wall as a coding-theoretic impossibility), D63 (parity ceiling n=5, oriented matroid classification), D46 (capacity analysis: ~4.3 bits magnitude + ~1.5 bits phase = ~5.8 bits/symbol), D47 (two-channel coding conjecture, topology-frequency correlation reversal), D12 (quantization geometry, BitNet parallels).

**Core results belonging to this lens:**
- Axiality theorem: at delta=0, bracket values are Z-axial in Z[zeta_8] -- only one coordinate is nonzero. The full rank-4 lattice collapses to rank 1. This is extreme constellation collapse.
- Axiality hierarchy: ell=2 is Z-axial (1D), ell=3 is Z[zeta_6]-axial (1D), ell=4 is Z[i]-axial (2D), ell=5 is full Z[zeta_5] (4D). Richer rings enable richer codebooks.
- Phase decoherence thesis (D46): split-sigmoid accesses ~4.3 bits (magnitude channel), MVN-continuous accesses a different ~1.5 bits (phase channel). These are literally complementary codes.
- Catalog size hierarchy: ell=3 (6 values, binary collapse), ell=4 (56 values), delta=0 (100 values), ell=5 (116 values). Non-monotonic: delta=0 has a uniquely favorable catalog despite the simplest algebra.
- The 11/13 half-plane theorem (D61-62): a semicircular decision region on additive angular sums can reach exactly 11 of 13 NPN classes. This is a covering radius / quantization geometry result.
- Parity ceiling for Z[zeta_8] is n=5 -- the fundamental capacity limit of the Z[zeta_8] codebook for the hardest boolean function.
- Oriented matroid classification: alternating OM type required for parity. Connection to matroid theory codebook structure.
- Matroid minor chain structure (D64): the XOR hierarchy {XOR2, XOR3, XOR4, XOR5} is deletion-contraction closed (deleting a weight gives XOR_{n-1}, contracting an input gives XNOR_{n-1}). This is a legitimate matroid minor chain.
- Vocabulary stratification (D64): of 83 class-1 catalog values at k=15, exactly 44 are "good" (appear in extensible XOR4 solutions) and 31 are "poison" (appear only in orphan XOR4 solutions). The 44 good values are strictly nested: they are exactly the values appearing in XOR5 solutions. Poison values span octants {1,2,3,5,6,7} but not octant 4.
- Algebra-independence of the parity ceiling (D65): the Z[zeta_16] catalog has different catalog values (36 vs 100 at delta=0) but identical tier thresholds: XOR3 at k=6, XOR4 at k=8, XOR5 at k=14. Parity ceiling n=5 is unchanged. This refutes the "gap-of-2" hypothesis — the gap widens to 9 at Z[zeta_16]. The ceiling is not a property of the ring.
- Self-doubling impossibility theorem (D64): adding a 6th weight equal to any existing weight always fails because scalar doubling preserves the angle of a Z[zeta_8] value, and a class-1 value doubled remains class-1, making an even-parity output class-1 when it must be class-0. Accounts for 89.5% of closest misses at Hamming distance 4.

**New results (D72, D73, D74, D81, D82):**
- D72 compressed sensing vs t-design (DOF-not-sampling): the 13 eigenvector directions fail the spherical t-design property entirely (t=0, worst possible) yet stochastic optimization shows better-distributed directions give fewer XOR6 solutions (36 vs 4). Sampling quality and computational quality are anti-correlated. The reason DKC works at bandwidth l=6 with 13 directions is the DOF coincidence 2×6+1=13 combined with compressed sensing for binary functions at known support positions — not uniform sampling.
- D73 additive-not-multiplicative confirmed: DKC is fundamentally an additive lattice computation. 82.8% of cell transitions are deterministic — a universal structural constant across all 36 winning triples that follows from binary octahedral group transitivity. The multiplicative (group product) formulation has zero XOR6 solutions because parity is invisible to it.
- D74 vocabulary stratification refined: the combined (bracket, cell) invariant gives 119 equivalence classes — 19% finer than bracket alone. The 6 computational type vectors partition cells into orbit-equivalent roles under the binary octahedral symmetry. The incomparability of bracket and cell channels is computationally load-bearing (confirmed by D77: neither channel alone can compute XOR8, but their product can).
- D81 logarithmic scaling law: for ζ₁₂ (infinite group), max_xor ≈ 0.62 × log₂(catalog_size) + 4.6. Each +2 XOR inputs costs approximately 10× more catalog entries. Transition points: XOR8 at ~51 entries, XOR10 at ~275, XOR12 at ~1140. Direction count is the bottleneck: 22 dirs needed for XOR8, 114 for XOR10, 507 for XOR12.
- D82 depth as crossing complexity → Kuperberg #P-hardness connection: the logarithmic law of D81 is a corollary of a linear depth law (max_xor ≈ depth + 6) combined with exponential catalog growth per depth. Since catalog grows as ~2^depth, log₂(catalog) ≈ depth, giving the log scaling. The hardness of generating deep entries (exponential BFS cost) at non-lattice roots is exactly what Kuperberg's #P-hardness theorem predicts — the computational gain is linear in crossing depth while the cost of generating those crossings is exponential. Algebraic coherence beats raw vocabulary: deep entries with fewer directions/angles outperform broad, uniform entries.

**New results (D83, D84):**
- D83 additive resource decomposition (lattice + depth + writhe = total capacity): the framing test establishes that three coding channels are independently additive. Lattice structure alone provides XOR6; each crossing-depth unit adds +2 (D82); writhe (framing) adds another +2 on top, for a total of lattice + 2×depth_rounds + 2×framing_present. The channels are confirmed independent by the depth-writhe correlation r = 0.139 across 4096 entries — nearly orthogonal. This is a clean additive capacity formula across three independent information channels, directly analogous to independent code layers.
- D83 Jones normalization as angular vocabulary reduction: Jones normalization reduces distinct angles from 43 to 31 (−28%) while leaving direction count unchanged (512 → 512). The capacity loss is entirely angular, not directional — it is a channel-specific erasure. Framing information lives in the S¹ angle coordinate, not the S² direction coordinate.
- D84 null entries as effective-rank conditioning: the 9 bracket-null entries in the ζ₈ catalog cover 6 cube-edge-midpoint directions unavailable to the 15 non-null entries. Removing nulls drops XOR capacity from 8 to 6, which is worse than removing a random 9-entry subset (random mean 7.8 vs null-removed 6.0). The null entries are conditioning the weight space: they maximize the effective rank of the 13-direction S² vocabulary by populating the 6 directions that non-null entries cannot reach. Removing them is not just losing 9 entries — it is collapsing the rank of the directional codebook from 13 to 7. The 3× resolution penalty (k=6 → k=16–20) observed when nulls are removed in the k_sec sweep corresponds to a Cramér-Rao cost of operating from an ill-conditioned basis.

**Open questions specific to this lens:**
- Two-channel coding conjecture: can a joint decoder for magnitude + phase achieve ~5.8 bits/symbol?
- Does the catalog size obey a lattice theta function identity?
- What is the covering radius of the Z[zeta_8] bracket catalog in the complex plane?
- CSS feasibility is already ruled out (radical is self-orthogonal under Gram form but not standard inner product). Are there other algebraic coding constructions that work?
- Z[zeta_16] scaling: does moving to ell=3 (rank-8 lattice) push the parity ceiling beyond n=5? What is the general relationship between cyclotomic ring rank and parity capacity? **ANSWERED (D65): ceiling stays at n=5; the ceiling is algebra-independent.**
- Are the 44 good values and 31 poison values related to the matroid structure of mu_8 (Reiner's cyclotomic matroid at 8th roots of unity)?
- Is the 0.62 XOR-per-doubling constant specific to ζ₁₂, or does it depend on the root of unity? Does ζ₁₀ (more half-angles per entry) give a steeper slope?
- What algebraic property do deep entries possess that makes them computationally superior to vocabulary-equivalent shallow entries? Can this be characterized as a structured code property?

**Connections to other lenses:**
- To Representation Theory: the radical is the "invisible" sublattice. Bracket evaluation projects through TL/rad, killing radical codewords. The neglecton weights (D49) are first-order perturbations recovering radical information -- 148 new values, but zero new angles. The lattice geometry is preserved by the projection.
- To Quantum Mechanics: the bracket value IS a Chern-Simons amplitude (Costello-Francis-Gwilliam 2026 proved Witten = RT). So the codebook is literally a table of quantum transition amplitudes. The ζ_8 evaluation point places the T-gate (Clifford hierarchy level 3) at the center of the codebook: self-doubling in Z[zeta_8] corresponds to the T→S gate descent, which is precisely why the wall occurs at n=6.
- To Approximation Theory: the 11/13 theorem is fundamentally about angular geometry. The proof that 0x1B and 0x06 are unreachable uses semicircle bounds -- the activation boundary is a half-plane, and certain truth tables require weight angles that cannot simultaneously satisfy the angular constraints. This is polynomial approximation theory on the circle. The matroid minor chain structure (D64) gives a combinatorial counterpart: the 44-good / 31-poison vocabulary split is a matroid independence constraint on the codebook.

---

## Lens 3: Quantum Mechanics

**What the research looks like:** The Kauffman bracket is the amplitude for a Chern-Simons path integral. Braid words are worldlines of anyons. The Yang-Baxter equation is the consistency condition for particle scattering. Evaluating the bracket at a root of unity selects a topological quantum field theory at a specific level. The DKC construction compiles quantum amplitudes into classical Boolean logic.

**Central demos:** D03-D05 (TL modules, R-matrix, Yang-Baxter equation, quantum trace formula), D04 (Schur-Weyl duality decomposition V^{tensor 3} = V_{3/2} tensor W_3 + V_{1/2} tensor W_1), D18-D19 (braid logic gates -- NOT + NAND = computational universality from topological amplitudes), D20 (delta error landscape -- |delta| = 2|cos(2theta)| as universal gate viability predictor), D09 (Reidemeister invariance -- the bracket IS a topological invariant).

**Core results belonging to this lens:**
- Quantum trace formula: bracket(beta) = tr_q(rho(beta)) / (A^2 + A^{-2}) with (-1)^n sign factor. Verified n=2 through n=5.
- Braid computational universality: specific braid words compute NOT (2 strands, 6 crossings) and NAND (3 strands, 5 crossings). NOT + NAND = Turing complete.
- Delta error landscape: |delta| = 2|cos(2theta)| cleanly separates universal-computation angles from NOT-only angles. Delta-zero angles (8th roots of unity) are where braid circuits compute exact Boolean logic.
- Entanglement vanishing theorem: at delta=0, ALL non-interleaving braids have zero bracket. Delta=0 is an entanglement detector.
- Convergence at WRT angles (D15): the greedy-optimal classification angle (1.805pi) converges to within 1.5% of the WRT level-3 evaluation point (11pi/6). "Topology knows information theory."
- Fibonacci anyon negative result (D58): at ell=5, braid representations are dense in PSU(2) — the condition for topological quantum computation universality. But density in the group does NOT translate to angular diversity in the scalar bracket output. The bracket projects a rich group-theoretic structure down to scalar amplitudes, and the projection destroys the angular diversity that DKC needs. This is why TQC universality and DKC universality are independent properties.
- DKC-RT truncation connection (D63): DKC works because it uses pre-closure bracket information that RT truncation would discard. At delta=0 (ell=2), the most degenerate topological point gives the most structured algebraic point.
- Clifford hierarchy connection (D64): the ring Z[zeta_8] is the algebraic ring of the T-gate (Clifford hierarchy level 3). The self-doubling impossibility in Z[zeta_8] corresponds to the T→S gate descent: doubling a T-gate weight produces an S-gate weight, which is at a lower hierarchy level and is class-1 in any odd sector. This is the precise mechanism of the n=6 wall.
- Algebra-independence of the ceiling (D65): the parity ceiling n=5 is invariant across Z[zeta_8] and Z[zeta_16], suggesting it is a statement about topological state sums, not evaluation algebra. The Clifford hierarchy level of the evaluation point does not determine the parity capacity.
- Hopf fibration structure (D66-D67): the braid group's SU(2) representation maps into the 24-cell sitting inside S³. Under the Hopf fibration S³ → S², all computational information lives in the S² base (the rotation axis direction). The S¹ Hopf fiber is completely computationally inert: phase-only activation yields zero XOR6 solutions at all resolutions tested. The S² base carries everything.
- Entanglement monogamy parallel (D64): the wall anatomy shows that 10 of C(6,2)=15 pairwise weight sums can simultaneously be assigned to even sectors, but all 15 cannot. This is structurally analogous to the CKW entanglement monogamy inequality: not all pairwise entanglement can be simultaneously maximized.
- Contextuality parallel (D64): the parity wall is a consistent-coloring obstruction — there is no assignment of class labels to all C(6,2)=15 weight pairs simultaneously satisfying the XOR6 truth table. This is formally analogous to the Kochen-Specker theorem.
- D71 connects the Hopf base to spectral theory: the 13 eigenvector directions of the binary octahedral group (the S² image of the braid group image) are the support points for a spherical harmonic expansion, and the minimum bandwidth l=6 comes from the counting identity 2l+1=13. Quantum angular momentum modes on the representation space explain the DKC bandwidth.

**New results (D75, D77, D80):**
- D80 quantum dimension [2]_q = 0 at ζ₈ (most singular point): q = i gives [2]_q = i + i⁻¹ = i − i = 0. The quantum dimension vanishes exactly at ζ₈, which is simultaneously the E₇ exceptional ADE point, the T-gate Clifford hierarchy ring Z[ζ_8], and the DKC sweet spot. Finite-group DKC requires the lattice (singular) points of quantum group parameter space — the non-lattice, non-singular points correspond to Kuperberg's #P-hard regime.
- D75 angle-subsumes-cell (Bloch sphere interpretation): for all 36 XOR6 winning triples, the eigenvalue angle of the sum quaternion perfectly predicts parity. The cell (S² eigenvector direction) is a coarsening of the eigenvalue angle (S¹). Seven discrete eigenvalue angles arise from the addition: {0°, 30°, 35.264°, 45°, 60°, 65.905°, 90°}. The three non-canonical angles (30°, 35.264°, 65.905°) arise only through quaternion addition breaking the 24-cell group structure — they are not present in individual catalog elements. The multiplicative channel, which stays inside the 24-cell group (only 4 canonical angles), has zero XOR6 solutions.
- D77 S¹ × S² as quaternionic generalization of MVN: the combined activation reads both eigenvalue angle (S¹) and eigenvector direction (S²) of the sum quaternion — the quaternionic generalization of Aizenberg's MVN which reads only the argument of a complex number (S¹). This product activation is structurally a Hopf fibration: the eigenvalue S¹ fiber and the eigenvector S² base decompose the unit quaternion as S³ → S². Neither channel alone can compute XOR8 (S² gives 0 winners, S¹ alone gives 0 winners), but the S¹ × S² product gives 6 winners at 100% accuracy. Demo 74's incomparability of bracket and cell channels is computationally load-bearing: incomparable invariants encode independent computational information.

**New results (D83, D84):**
- D83 TQFT anomaly as computational content (Witten 1989 maximally degenerate point): TQFT at δ=0 removes the writhe-dependent framing anomaly as its first normalization step. D83 demonstrates that what field theory discards for purity is exactly what DKC computes with. Jones normalization is a TQFT gauge choice — fixing the framing anomaly. The anomaly term contributes 2 XOR levels. This is the sharpest available statement of the Witten connection: the TQFT gauge-fixing operation has a measurable information cost in DKC terms, and the maximally degenerate point δ=0 is where that cost is most precisely isolatable.
- D84 LCFT logarithmic partners and Jordan-cell L₀ structure: bracket-null entries (Re(q)=0) are the DKC analog of LCFT null states — zero Kauffman trace (two-point function vanishes) but nonzero as vectors. The Jordan-cell structure of the Virasoro zero-mode L₀ in LCFT is mirrored by the bracket TL algebra acting on the ζ₈ catalog: null states (bracket-null entries) and logarithmic partners (non-null entries on the same S² direction) form 2×2 Jordan blocks. The 3 shared coordinate-axis directions correspond to null-partner pairs; the 6 null-only directions (cube-edge-midpoints) correspond to unpaired nulls whose logarithmic partners are absent from the catalog. Removing unpartnered null states collapses XOR capacity. The dense polymer model at β=0 (LCFT context) is the physical system where this Jordan-cell structure is natural — and δ=0 in the Kauffman bracket corresponds to exactly this β=0 polymer limit. Non-diagonalizability of L₀ is the algebraic signature of the non-semisimplicity that makes ζ₈ computationally exceptional.

**Open questions specific to this lens:**
- Fibonacci TQFT boundary (mentioned at D05, n=5): what happens when we cross into the Fibonacci anyon regime?
- Connection to volume conjecture: WRT invariants at large r relate to hyperbolic volume -- does this have DKC implications?
- The entanglement vanishing theorem breaks at delta != 0 (D53). What is the precise crossover?
- Can multi-readout from different braid closures (D21, D25) be understood as a quantum measurement basis?
- Does the parity ceiling n=5 hold at Z[zeta_32] and all higher cyclotomic levels? If confirmed, it should be provable as a theorem about topological state sums independent of evaluation point (D65 conjecture).
- Does the Bloch sphere connection (S² = CP¹ = qubit state space) have a direct interpretation in quantum information? The DKC computation lives on the Bloch sphere at l=6 bandwidth (D71).
- Why do XOR8 winning quadruples use only the two canonical octahedral angles (45°, 90°) for individual entries, while additive sums produce non-canonical angles? Is there a quantum-group explanation for the distinction between entry-level and sum-level angle structure?
- Do all infinite-group roots (ζ₆, ζ₁₀, ζ₁₂, ...) have vanishing [2]_q? If not, what quantum-mechanical interpretation distinguishes finite (non-semisimple, lattice) from infinite (generic, #P-hard) roots?

**Connections to other lenses:**
- To Representation Theory: the R-matrix lives in the TL algebra. The quantum trace is computed via Jones-Wenzl projectors. Schur-Weyl duality decomposes tensor products into TL standard modules. The quantum trace formula unifies separate module traces (D06) into a single computation (D04). The SU(2) quaternionic representation (D66) lifts the scalar trace to a full geometric object: the 24-cell.
- To Coding Theory: the bracket amplitude at a root of unity IS a lattice point in Z[zeta_{2ell}]. The quantum amplitude is literally an algebraic integer. This is what makes forward DKC possible: quantum mechanics provides exact integer codewords. The Clifford hierarchy interpretation (D64) gives the T-gate ring its precise physical meaning.
- To Approximation Theory: the delta error landscape |delta| = 2|cos(2theta)| is a Chebyshev polynomial evaluated at the angle. Gate viability is determined by approximation quality: how close is the evaluation point to a root of the Chebyshev polynomial? The Hopf fibration inertness result (D67) says all computation is in the S² base, and spherical harmonics on that base have minimum bandwidth l=6 (D71).

---

## Lens 4: Approximation Theory

**What the research looks like:** The quantum dimensions [n]_q satisfy the Chebyshev recurrence [n+1]_q = [2]_q * [n]_q - [n-1]_q. The Jones-Wenzl idempotents are Chebyshev polynomials in the TL generators. The semicircle law governs eigenvalue distributions. The 11/13 theorem is a polynomial approximation barrier. The axiality theorem is about angular concentration of Fourier coefficients.

**Central demos:** D05 (quantum dimension recurrence -- explicit Chebyshev connection), D52 (Chebyshev generalization across ell values -- the entire demo is named for this lens), D14 (bracket zeros on the unit circle -- zero structure is {0, 4, 8}, universal angles at pi/4 and pi/8), D61-D62 (11/13 half-plane theorem -- the unreachability proofs for 0x1B and 0x06 are angular geometry arguments), D50 (triskelion principle -- sector geometry, incommensurability hypothesis).

**Core results belonging to this lens:**
- Quantum dimension recurrence: [n+1]_q = [2]_q * [n]_q - [n-1]_q. This IS the Chebyshev recurrence. At roots of unity, [n]_q vanishes and the Jones-Wenzl projector becomes singular -- this is where non-semisimplicity appears.
- Zero structure of torus knots: zero counts take values from {0, 4, 8} only. All zeros lie on the pi/8 lattice. The zero count pattern {8,0,0,8,4,4} for T(2,n), n=3,5,7,9,11,13 is cyclic, not monotonic.
- 0x1B unreachability proof: algebraic contradiction via interval squeeze. Three angles phi_1, phi_2, phi_3 cannot simultaneously satisfy the semicircle constraints imposed by the 0x1B truth table. The proof forces phi_3 > pi and phi_3 < pi.
- 0x06 unreachability proof: four-semicircle parallelogram argument. If alpha + beta > pi, all gaps are < pi, making the required intersection empty.
- Incommensurability hypothesis (D50): parity solution count anti-correlates with gcd(k,8). Sector boundaries misaligned with the pi/4 angular lattice break the symmetry blocking parity. k=6 (gcd=2) gives 906 solutions; k=8 (gcd=8) gives only 96.
- DKC-RT truncation connection (D63): DKC works because it uses pre-closure bracket information that RT truncation would discard. At delta=0 (ell=2), the most degenerate topological point gives the most structured algebraic point.
- Generalized activations break the wall (D65): arbitrary binary sector labelings (not just standard odd/even) remove the absolute wall at k<=23 and achieve XOR6 at k=24. This reveals a two-layer structure: the standard activation wall (k=15) is a labeling convention barrier, the generalized wall (k=24) is the true geometric obstruction. Standard and generalized walls are distinct objects.
- Two-layer wall decomposition (D65): Layer 1 (k=6 to k=15): standard odd/even labeling wall. Layer 2 (k=24): generalized minimum where any binary partition can first succeed. Below k=24, no binary coloring of k sectors can separate XOR6 regardless of which sectors are called 0 or 1.
- Zero margin universality (D65): all passing XOR6 tuples at all tested k values have exactly zero margin to the nearest sector boundary. Z[zeta_8] lattice sums land exactly on lattice directions (multiples of pi/4), aligning with sector boundaries only for compatible k values. Solutions are algebraically exact, not floating-point approximate.
- Voronoi activation on S³ (D66): lifting from S¹ to S³ (quaternionic weights) with a 24-cell Voronoi partition (25 cells: 24 vertices + zero cell) achieves 35 XOR6 solutions — more than the 64-cell geographic grid's 34. Data-intrinsic geometry beats geographic grids.
- S² as optimal activation space (D67): projecting quaternion weights to their eigenvector direction on S² and using a 13-direction Voronoi achieves 36 solutions at only 14 cells (2.57 sol/cell). This beats S¹ (25 cells) and S³ (25 cells). The Hopf S¹ fiber is completely inert: phase-only activation gives zero solutions at all resolutions. The computation lives entirely on the S² Hopf base.
- Platonic solids fail (D67, D70): octahedral, icosahedral, cuboctahedral, dodecahedral Voronoi partitions all yield zero XOR6 solutions. Data-intrinsic geometry (13 eigenvector directions of the binary octahedral group) is required; standard symmetric polytopal geometry is insufficient.
- Perceptron formulation (D68): the DKC architecture can be written as 6 bits → 2 neurons (linear combination of stereographic weight projections) → S² Voronoi activation → 1 bit. The 6×2 weight matrix comes from knot theory, not gradient descent. The computation is intrinsically curved: Euclidean nearest-neighbor in R² gives zero solutions; S² great-circle metric gives 36.
- The 13=13 theorem (D71): the minimum bandwidth for XOR6 DKC on S² is l=6 because mode l has (2l+1) independent spherical harmonic components and 2×6+1=13 equals the number of eigenvector directions. At l=5 there are 11 components (underdetermined); at l=6 there are exactly 13 (determined). The phase transition is sharp: 0% recovery at l<=5, 100% at l=6.
- Spectral universality (D71): all 36 XOR6 winning triples share the same spectral envelope — l=6 dominant (~80% power), l=4 secondary (~6%), l=12 tertiary (~6-7%). The spectrum is a structural invariant of XOR6 DKC solutions, not solution-specific noise.
- Super-Nyquist compression (D71): 14 Voronoi cells achieve what Nyquist predicts requires (l+1)²=49 cells — a 3.5× compression. Explained by compressed sensing: XOR6 is a binary function at 13 known support positions, so sparsity plus known support eliminates the generic sampling requirement.

**New results (D72, D73, D82):**
- D72 perturbation sensitivity (~5° robust, collapses at 10-20°): at 1° perturbation mean XOR6 count is 38 (slightly above unperturbed 36, showing near-miss solutions exist near the algebraic configuration). Robust through 5° (mean 35.8). Collapses at 10° (mean 11.5) and 20° (mean 2.1). The algebraic placement is sharp to approximately 5° — consistent with the typical angular Voronoi cell radius for 13 points on S². This is perturbation approximation: the DKC computation is locally stable but not globally continuous.
- D73 automaton determinism as approximation quality metric (82.8%): the fraction of deterministic cell transitions in the additive DKC trajectory is a well-defined approximation-theoretic quantity measuring how well the Voronoi partition approximates the computation as a Markov process. 82.8% universal determinism (48/58 transitions) is a structural constant. The 17% non-determinism comes from discarded magnitude information — the cell captures direction (compass) but not magnitude (odometer). A 27-state signed partition raises this to 87.1% but still not 100%, because 100% requires also tracking the magnitude modulo the weight lattice.
- D82 algebraic coherence vs vocabulary (approximation theory of discrete group actions): strided selection of 564 catalog entries (maximum spatial coverage: 512 dirs, 43 angles) cannot reach XOR12; deep selection of 564 entries (476 dirs, 19 angles) can. Algebraic coherence — the shared intermediate factor structure of depth-8 entries as products of 8 generators — dominates over angular coverage. This is an approximation-theoretic result about discrete group actions: for computing parity via Voronoi activation, the approximation quality is determined not by how well the weight directions sample S² uniformly but by whether the weights encode complementary algebraic structure from deep composition. The direction explosion / angle refinement phase transition at depth 6–7 is a clean approximation-theoretic boundary.

**New results (D83, D84):**
- D83 writhe and depth as independent approximation dimensions (r = 0.14): depth-writhe correlation across 4096 ζ₁₂ entries is near-zero, confirming that writhe and crossing depth are genuinely orthogonal approximation axes. Mean |writhe| grows linearly with depth (0.80 → 3.19 from depth 0 to depth 8), but the low correlation means these dimensions are not redundant — a braid of fixed depth can have nearly any writhe depending on chirality cancellation. The two resources contribute independently to the approximation capacity of the weight set.
- D84 S¹ × S² decomposition of activation space: the k_sec sweep on null-only entries reveals the cleanest decomposition of the two approximation components. Null-only entries have half-angle exactly 90° — a single point on S¹, carrying zero S¹ information. All of their computational contribution (XOR6 capacity, flat across k_sec) comes from S² directional diversity alone. Non-null entries carry both S¹ and S² content. The null/non-null partition is thus a clean anatomical decomposition into pure-S² and mixed-S¹×S² components. At k_sec=1 (direction-only scoring), null-only outperforms non-null-only (9 dirs vs 7 dirs), despite null-only having zero S¹ content — a direct experimental demonstration that S² approximation capacity is independent of S¹ approximation capacity.

**Open questions specific to this lens:**
- Can the cyclic zero pattern {8,0,0,8,4,4} be explained by a Chebyshev identity?
- Is there a Chebyshev polynomial characterization of which NPN classes require k sectors?
- Can the triskelion principle (each input needs its own odd sector at 120-degree intervals) be derived from approximation-theoretic bounds on angular separation? **FALSIFIED** -- triskelion generalization does not hold; the angular separation requirement is specific to n=3, not a universal principle.
- Does the incommensurability hypothesis generalize: for n-input parity, is the minimum sector count k=2n?
- Z[zeta_16] scaling: does the richer angular lattice of ell=3 push the parity ceiling beyond n=5, or is the capacity limit intrinsic to the activation geometry rather than the ring? **ANSWERED (D65): the ceiling is algebra-independent; it stays at n=5.**
- Can the 13=13 theorem be proven analytically: is the minimum bandwidth of any binary Voronoi partition separating N points on S² in general position equal to l=ceil((N-1)/2)?
- Is there a scaling law for XOR_n minimum k, and does the growth rate discriminate polynomial from exponential (D65 data: XOR6 k=24, XOR7 k=91, ratio ~3.8)?
- Does the generalized XOR6 first-pass threshold k=24 change when using Z[zeta_16] weights?
- Is the ~5° robustness threshold exactly equal to the Voronoi cell angular radius, or is there a deeper approximation-theoretic reason the two coincide?
- Can the 82.8% automaton determinism be expressed as a ratio of integers arising from the binary octahedral group, and does it generalize to other finite SU(2) subgroups?
- Is there a clean characterization of which 564 deep entries achieve XOR12 while 564 strided entries do not — i.e., can the algebraic coherence property be stated as an approximation condition on the weight set?

**Connections to other lenses:**
- To Representation Theory: the Chebyshev recurrence IS the quantum dimension recurrence. When [ell]_q = 0, the Jones-Wenzl idempotent p_ell is singular and the algebra becomes non-semisimple. Approximation theory predicts EXACTLY where the algebra changes character. The 13 eigenvector directions are a representation-theoretic count (binary octahedral group mod ±1 = 24 quaternions → 13 directions), and the 13=13 theorem makes this count analytically consequential.
- To Coding Theory: the 11/13 theorem is a covering problem. The 2 unreachable NPN classes are truth tables that fall outside the covering radius of any angular constellation under half-plane quantization. The proofs use semicircle geometry, which is approximation theory. The two-layer wall structure (D65) refines the coding picture: the standard layer is a labeling convention, the generalized layer is a genuine lattice coverage impossibility.
- To Quantum Mechanics: the delta error landscape |delta| = 2|cos(2theta)| is a Chebyshev polynomial. Gate viability (D20) is controlled by how close the evaluation angle is to a Chebyshev root. The "magic angle" 5pi/4 is magic precisely because it is a root. The S² computation space is the Bloch sphere of the SU(2) braid representation, and the l=6 bandwidth corresponds to hexadecapole angular momentum modes (D71).

---

## The Unified View

The deepest results sit at the intersection of all four lenses:

**The parity wall** is simultaneously:
- (Rep Theory) a consequence of non-semisimplicity -- bracket evaluation factors through TL/rad, killing radical degrees of freedom
- (Coding Theory) a lattice covering impossibility -- no four Z[zeta_8] codewords under split-sigmoid can produce the parity truth table
- (Quantum Mechanics) a decoherence effect -- the radical is "invisible" under standard bracket evaluation, just as decoherent states are invisible to quantum measurement
- (Approx Theory) an angular separation barrier -- k=2 sectors cannot place 3 weights in distinct odd sectors (the triskelion requires k=6)

**The axiality theorem** is simultaneously:
- (Rep Theory) a rank collapse: semisimple quotient at delta=0 is so constrained that bracket values use only 1 of 4 cyclotomic coordinates
- (Coding Theory) extreme constellation collapse: a rank-4 lattice degenerates to rank 1
- (Quantum Mechanics) single-loop selection: delta=0 kills all multi-loop states, and surviving states have phases related by A^4 = -1
- (Approx Theory) angular concentration: all bracket values at delta=0 point in one of 8 directions (multiples of pi/4)

**The forward DKC construction** assembles all four:
- Abramsky (Rep Theory): TL diagrams encode computation
- Habiro (Quantum Mechanics): bracket values are cyclotomic integers
- Aizenberg (Approx Theory): k-sector activation reads angular structure
- Nazer-Gastpar (Coding Theory): algebraic integer lattices support exact computation

These are not analogies. They are the same mathematics described in four languages.

**The S² computation result (D66-D71)** is a fifth unification:
- (Rep Theory): the binary octahedral group (image of braid group under SU(2)) has exactly 13 eigenvector directions; the direction count is the representation-theoretic fact underlying everything below
- (QM / TQFT): under the Hopf fibration S³ → S², the S¹ fiber is computationally inert; all information lives in the Hopf base S², which is the Bloch sphere of the SU(2) representation
- (Coding Theory): the 24 quaternions of the binary octahedral group carry 100 distinct bracket values; bracket and quaternion are complementary hash functions (12.86% collision agreement, 80% mutual divergence)
- (Approx Theory): the minimum S² bandwidth for XOR6 is l=6, determined by the counting identity 2×6+1=13; compressed sensing explains the 3.5× sub-Nyquist efficiency of the 14-cell Voronoi

**The parity wall** is simultaneously:
- (Rep Theory): the 13 eigenvector directions = 2l+1 at l=6 is the DOF boundary; n=5 is the last input count for which the Z[zeta_8] bracket solutions have enough directions to span the required computation
- (Coding Theory): the vocabulary stratification (44 good / 31 poison) is the lattice-theoretic shadow of a matroid minor obstruction
- (QM): the Clifford hierarchy level (T-gate = level 3, ring = Z[zeta_8]) determines the angle structure; self-doubling = T→S descent = wall mechanism
- (Approx Theory): the two-layer wall (standard at k=15, generalized at k=24) separates labeling convention from genuine geometric obstruction

**The finite-vs-infinite group boundary (D79-D82)** is a sixth unification:
- (Rep Theory): ζ₄ → Binary Dihedral (D-series, ADE), ζ₈ → Binary Octahedral (E₇, ADE). Finite groups close at fixed catalog sizes; the ADE classification of finite SU(2) subgroups exactly characterizes the tractable DKC regime. Non-exceptional roots generate infinite (dense) SU(2) subgroups.
- (QM): the finite cases are the singular (lattice) points of quantum group parameter space where [2]_q ∈ {0, ±2}. The quantum dimension vanishing at ζ₈ ([2]_q = 0) is the most degenerate case — simultaneously the DKC sweet spot and the TL non-semisimplicity maximum.
- (Coding Theory): finite groups impose hard XOR capacity ceilings (ζ₈ caps at XOR8); infinite groups give logarithmic scaling (ζ₁₂: max_xor ≈ 0.62 × log₂(catalog_size) + 4.6). The coding capacity is determined by group finiteness, not by the ring's algebraic degree or cyclotomic index.
- (Approx Theory): the linear depth law (max_xor ≈ depth + 6) expresses the approximation cost of DKC in terms of knot-crossing complexity. Algebraic coherence (shared deep intermediate products) dominates over angular vocabulary (coverage of S²). The Kuperberg #P-hardness of generating deep entries at non-lattice roots is the approximation-theoretic price of unbounded capacity.

**Framing and null states as resource anatomy (D83-D84)** is a seventh unification:
- (Rep Theory): Jones normalization removes a writhe-graded module structure (the abelianization of the braid group acting by a 1-dimensional character). The 2-level capacity loss is the dimension of the discarded grading. Null states are radical-module elements whose bracket trace vanishes — precisely the objects whose Jordan-cell structure (non-semisimplicity) makes the ζ₈ algebra computationally exceptional. Semisimple ζ₄ has 75% null fraction but only XOR6 capacity; non-semisimple ζ₈ has 37.5% null fraction and XOR8 capacity. Non-semisimplicity IS the computational resource.
- (QM / TQFT): the writhe-phase factor is a TQFT framing anomaly; removing it (Jones normalization) is a gauge-fixing operation, and D83 measures the information cost of that gauge choice in XOR units. Null states have the LCFT interpretation: Re(q)=0 entries are analogs of null states in LCFT (zero two-point function, nonzero vectors), paired with logarithmic partners via Jordan-cell L₀ action. The dense polymer model at β=0 (δ=0 Kauffman bracket) is where Jordan-cell structure is physically natural. Non-diagonalizability of L₀ = non-semisimplicity of TL = computational exceptionality of ζ₈.
- (Coding Theory): the resource decomposition is an additive capacity formula: lattice (XOR6) + depth (+2 per unit) + writhe (+2 framing) = total. These are independent coding channels — confirmed by depth-writhe correlation r=0.14. Null entries condition the effective rank of the directional codebook: removing 9 null entries collapses the direction vocabulary from 13 to 7, performing worse than removing a random 9-entry subset. Null states maximize effective rank by exclusively populating 6 cube-edge-midpoint directions unreachable by non-null entries.
- (Approx Theory): writhe and depth are nearly orthogonal approximation dimensions (r=0.14), contributing independently to the approximation capacity of the weight set. The null/non-null partition cleanly decomposes the S¹ × S² activation space: null entries are pure S² (zero S¹ content, half-angle exactly 90°), non-null entries are mixed S¹ × S². At k_sec=1 (direction-only scoring), null-only outperforms non-null-only — demonstrating that S² and S¹ approximation capacity are independent components.
- (Reservoir Computing — Pillar 5): writhe and depth as independent computational axes directly parallels RC memory depth (depth) and nonlinearity degree (writhe). Null states maintain the separation property of the RC manifold: they hold the reservoir state space open in directions that active (non-null) states do not cover. Removing null reservoir states collapses the manifold in those directions, destroying the linear separability of higher-order parity functions — exactly the mechanism observed in D84. The "+2 per resource unit" pattern (depth and writhe each add +2) matches the independent-channel additivity expected from reservoir theory when channels are orthogonal.

---

## Demo-to-Lens Assignment (Primary / Secondary)

| Demo | Primary Lens | Secondary Lens(es) |
|------|-------------|-------------------|
| D01-D09 | QM (bracket, R-matrix, Reidemeister) | Rep Theory (TL modules) |
| D10-D16 | Coding (quantization, waveforms, classification) | Approx (angle optimization, zeros) |
| D17-D25 | QM (braid gates, universality, closures) | Coding (function zoo, measurement) |
| D26-D29 | Coding (reverse/forward DKC, Z[zeta_8] catalog) | QM (bracket values as amplitudes) |
| D35 | Rep Theory (TL_n matrices, axiality, compression) | All four |
| D45-D48 | Coding (NPN landscape, 100M search, correlation) | QM (decoherence thesis), Approx (angular) |
| D49 | Rep Theory (radical/neglecton perturbation) | Coding (catalog expansion) |
| D50 | Approx (triskelion, sector geometry, incommensurability) | Coding (parity reachability) |
| D51-D52 | Rep Theory (radical anatomy, sandwich, Fibonacci rank) | Approx (Chebyshev recurrence) |
| D53-D54 | Coding (axiality hierarchy, cross-ell catalogs) | Rep Theory (semisimplicity) |
| D55-D59 | Coding (cross-ell DKC landscape) | Approx (11/13 wall universality) |
| D61-D62 | Approx (semicircle proofs for 0x1B, 0x06) | Coding (half-plane quantization) |
| D63 | Coding (parity ceiling n=5, oriented matroid classification) | QM (RT truncation connection), Approx (angular capacity) |
| D39 G-J | Rep Theory (three Gram forms, Markov truncation, radical formula) | QM (shadow interpretation), Approx (Chebyshev) |
| D64 | Coding (matroid minor chain, vocabulary stratification, 44/31 split) | Approx (wall anatomy, self-doubling impossibility), QM (Clifford hierarchy, monogamy parallel) |
| D65 | Coding (Z[zeta_16] comparison, algebra-independence of ceiling) | Approx (generalized activations, two-layer wall, k=24 threshold, zero margin universality), QM (Clifford level independence) |
| D66 | Rep Theory (24-cell = binary octahedral group, SU(2) braid image) | QM (Hopf fibration on S³), Approx (Voronoi on S³, 35 solutions), Coding (complementary hash functions) |
| D67 | Approx (S² as optimal activation, 14 cells, Hopf phase inertness) | QM (Hopf fibration S³ → S²), Rep Theory (eigenvector decomposition, 13 directions) |
| D68 | Approx (stereographic DKC, intrinsic curvature, perceptron formulation) | QM (conformal map, Bloch sphere visualization) |
| D69 | Rep Theory (Direction Nesting Theorem, ζ_N ⊂ ζ_{2N} on S²) | Approx (trivialization at finer roots, ζ_8 as computational sweet spot) |
| D70 | QM (braid melody, Yang-Baxter = identical melody, Hopf orthogonality) | Rep Theory (binary octahedral group, 4 eigenvalue angles), Approx (Platonic solid failure, data-intrinsic geometry) |
| D71 | Approx (spherical harmonics on S², 13=13 theorem, super-Nyquist compression) | Rep Theory (spectral theory of representation space), QM (quantum angular momentum modes, Bloch sphere) |
| D72 | Approx (perturbation robustness ~5°, anti-correlation theorem, DOF-not-design) | Coding (compressed sensing vs t-design, sampling quality anti-correlated with computational quality) |
| D73 | Rep Theory (additive-not-multiplicative, binary octahedral transitivity, 82.8% universal determinism) | Approx (automaton determinism as approximation quality metric, compass without odometer) |
| D74 | Rep Theory (incomparability theorem, 6-type orbit structure, 119 combined classes) | Coding (bracket-blind computation, vocabulary stratification), QM (non-semisimple structure) |
| D75 | QM (angle subsumes cell, Bloch sphere binocular, non-canonical angles from addition) | Coding (2.3× capacity over cell-only, √n magnitude quantization, multiplicative channel blind) |
| D76 | Coding (ζ₈ S²-capacity limit, pigeonhole bound for XOR8, algebraic discreteness breakdown) | Approx (4-term sum angle explosion: 7→86 angles, sweet-spot formula conjecture) |
| D77 | QM (S¹×S² as quaternionic MVN, Hopf fibration decomposition, XOR8 solved) | Coding (incomparability implies product necessity, activation determines capacity), Approx (phase transition at ~100 cells) |
| D78 | Coding (ζ₈ wall theorem, XOR10=0 exhaustive, recursive shadow conjecture confirmed) | Approx (non-monotonicity in sector count, pi/4 incommensurability echoes Demo 50) |
| D79 | Coding (finite group ceiling theorem, ζ₁₂ breaks ζ₈ wall, nesting failure) | Rep Theory (finite SU(2) subgroup capacity, ADE connection), QM (Kuperberg #P-hardness at non-lattice roots) |
| D80 | Rep Theory (ADE classification: ζ₄=2D₂, ζ₈=E₇=Binary Octahedral; finiteness survey) | QM ([2]_q=0 at ζ₈ = most singular point, lattice roots = finite groups), Coding (finite vs infinite group determines capacity ceiling) |
| D81 | Coding (logarithmic scaling law max_xor ≈ 0.62·log₂(catalog)+4.6, direction bottleneck) | Approx (each +2 XOR costs ~10× more entries, transition gap series 3.35→2.43→2.05) |
| D82 | Approx (linear depth law max_xor≈depth+6, algebraic coherence beats vocabulary, Kuperberg connection) | Coding (deep entries 2× more efficient, direction explosion/angle refinement phase transition), Rep Theory (depth = crossing depth, ADE control confirmed at ζ₈) |
| D83 | Coding (additive resource decomposition: lattice+depth+writhe, r=0.14 independent channels) | Rep Theory (Jones normalization = writhe-graded module loss, 2-level capacity cost), QM (TQFT framing anomaly = computational content, Witten degenerate point), Approx (writhe and depth as orthogonal approximation dimensions), RC Pillar 5 (writhe + depth = independent memory/nonlinearity channels) |
| D84 | Rep Theory (non-semisimplicity IS the resource; null states = LCFT Jordan-cell structure; radical modules) | Coding (null entries condition effective rank; removing nulls worse than random; 3× resolution penalty), QM (LCFT logarithmic partners, Jordan-cell L₀, dense polymer β=0, non-diagonalizable Virasoro zero mode), Approx (S¹×S² decomposition; null = pure S²; direction count vs angle count as independent components), RC Pillar 5 (null states maintain manifold separation property) |

---

*Updated 2026-02-21. Source: demo-index.md, theorems.md, data-tables.md, five-pillars.md, demo_64.md–demo_84.md. Added D64 (matroid minor chain, vocabulary stratification), D65 (Z[zeta_16] comparison, algebra-independence, generalized activations, two-layer wall), D66 (quaternionic DKC, 24-cell, binary octahedral group, Hopf structure), D67 (S² as optimal activation space, Hopf phase inertness, 13 eigenvector directions), D68 (stereographic DKC, perceptron formulation, intrinsic curvature), D69 (Direction Nesting Theorem across Clifford hierarchy), D70 (musical mapping, Platonic solid failure, intrinsic geometry conjecture), D71 (spherical harmonics, 13=13 theorem, spectral universality, super-Nyquist compression), D72 (spherical design test, anti-correlation theorem, DOF-not-design, perturbation robustness), D73 (eigenvector automaton, additive DKC, 82.8% universal determinism, compass without odometer), D74 (computational invariant, bracket-cell incomparability, 119 combined classes, 6-type orbit structure), D75 (binocular DKC, angle-subsumes-cell, non-canonical angles, √n magnitude quantization), D76 (XOR8 at S²=0, capacity breakdown, algebraic discreteness at 4-term sums), D77 (activation zoo, S¹×S² quaternionic MVN, XOR8 solved, product necessity), D78 (recursive scaling, ζ₈ wall theorem, XOR10=0 exhaustive, shadow structure confirmed), D79 (ζ₁₂ capacity, finite-group ceiling theorem, XOR10/XOR12 solvable, nesting failure), D80 (ADE classification, ζ₄=2D₂/ζ₈=E₇, [2]_q=0 at ζ₈, finiteness survey), D81 (logarithmic scaling law, direction bottleneck, 0.62 XOR/doubling, XOR14 prediction), D82 (linear depth law, algebraic coherence dominance, crossing depth = computational depth, Kuperberg connection), D83 (framing as computational content, Jones normalization = −2 XOR, writhe-depth independence r=0.14, TQFT anomaly cost, additive resource decomposition), D84 (null states indispensable, non-semisimplicity as resource, LCFT Jordan-cell interpretation, S¹×S² decomposition, null-state separation property).*
