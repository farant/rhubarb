# DKC Glossary

Alphabetical glossary of key terms used across the knotapel atlas. Each entry is self-contained.

---

## 11/13 Theorem (Half-Plane Theorem)
**Plain**: When you classify outputs using a simple dividing line (half-plane), exactly 11 of the 13 possible Boolean function types are computable -- the same two always fail, no matter which root of unity you use.
**Precise**: For all half-plane activations on multiplicative encodings z(a,b,c) = w1^a * w2^b * w3^c with Z[zeta_N] weights, exactly 11 of 13 non-trivial NPN classes are reachable; 0x06 (XOR-AND) and 0x1B (CROSS) are unreachable, independent of evaluation level ell.
**Introduced**: D61 (computational proof), D62 (analytical proof).
**See also**: NPN Classification, Activation Function, Complement-Blindness Theorem.

---

## 13=13 Bandwidth Theorem
**Plain**: The number of distinct rotation-axis directions produced by the braid group (13) exactly matches the number of independent components at spherical harmonic bandwidth l=6 (2*6+1=13), creating a sharp all-or-nothing phase transition at that bandwidth.
**Precise**: The minimum spherical harmonic bandwidth for XOR6 DKC on S^2 is l=6, because mode l has 2l+1 independent components and 2*6+1=13 equals the eigenvector direction count of the binary octahedral group; recovery is 0% at l<=5 and 100% at l=6.
**Introduced**: D71.
**See also**: S^2 Sweet Spot, Hopf Fibration, 24-Cell.

---

## 24-Cell
**Plain**: A four-dimensional shape with 24 vertices that naturally emerges from the braid group's quaternion representation -- its vertices are exactly the distinct quaternion states produced by braids at zeta_8.
**Precise**: The 24 vertices of the regular 24-cell in S^3 are the elements of the binary octahedral group 2O, which is exactly the image of the SU(2) braid representation at A = e^{i*5*pi/4}; the k=24 minimum sector count for generalized XOR6 on S^1 (D65) is the trace projection of these 24 vertices.
**Introduced**: D66.
**See also**: Hopf Fibration, S^2 Sweet Spot, Binary Octahedral Group.

---

## Activation Function
**Plain**: The rule that decides how to read a neuron's complex-valued output -- different rules make different Boolean functions computable from the same set of weights.
**Precise**: A map from the complex plane (or S^3) to a discrete label set; the choice of activation (split-sigmoid, k-sector MVN, half-plane, S^2 Voronoi, S^1 x S^2 product) determines which subset of NPN classes is accessible from a fixed bracket catalog. The lattice is fixed; the activation selects.
**Introduced**: D27 (split-sigmoid), D47 (MVN), D50 (k-sector), D67 (S^2 Voronoi), D77 (S^1 x S^2 product).
**See also**: k-Sector MVN, Wall Pattern, NPN Classification.

---

## Axiality Theorem
**Plain**: At delta=0, every Kauffman bracket value points along exactly one of 8 directions in the complex plane -- no bracket value ever uses more than one direction at once.
**Precise**: At delta=0 (A = e^{i*5*pi/4}), for any braid, the Kauffman bracket is axial: <K> = n * A^c for integer n and c in {0,...,7}; the Z[zeta_8] representation has at most one nonzero component. Verified exhaustively for ~131K braids with zero counterexamples.
**Introduced**: D35.
**See also**: Z[zeta_8], Delta=0, Z/4Z Axis-Alignment Theorem.

---

## Barrington's Theorem
**Plain**: A deep result from computational complexity: any function computable by a small Boolean circuit can be computed by a sequence of permutation-group multiplications, but only if the group is non-abelian (non-commutative). Abelian groups provably cannot compute parity.
**Precise**: Barrington (1989): width-5 branching programs over S_5 simulate NC^1; by contrapositive, abelian groups cannot compute parity. Applied to DKC: the radical of a non-semisimple TL module carries an abelian character (writhe homomorphism B_n -> Z/8Z), proving the radical direction is useless for XOR.
**Introduced**: D94 (connection identified), D102 (Barrington-Radical Principle named and proved).
**See also**: Radical, Non-Semisimplicity, Writhe.

---

## BFS Catalog
**Plain**: The full set of distinct bracket values obtained by systematically multiplying braid generators round by round (breadth-first search through the braid group), organized by how many multiplications (depth) were needed.
**Precise**: The catalog of distinct Z[zeta_N] bracket values (or SU(2) quaternions) obtained from BFS closure of the braid generators sigma_1, ..., sigma_{n-1} and their inverses, indexed by depth (number of generator multiplications). At zeta_8 with 2 strands: 24 entries over 3 rounds; at zeta_12: 4096+ entries over 8 rounds.
**Introduced**: D29 (first Z[zeta_8] catalog), D79 (zeta_12 catalog), D82 (depth indexing).
**See also**: Crossing Depth, Log Law, Reservoir Computing.

---

## Bracket Polynomial (Kauffman)
**Plain**: A recipe that assigns a polynomial to any knot or link diagram by summing over all possible ways to smooth its crossings, weighted by a formal variable A.
**Precise**: The Kauffman bracket <K> is a state-sum invariant: enumerate all 2^n smoothings of n crossings, weight each by A^{sigma} * delta^{loops-1} where sigma counts A-type minus B-type smoothings and delta = -(A^2 + A^{-2}); the result is a Laurent polynomial in A, invariant under Reidemeister moves II and III.
**Introduced**: D01 (state-sum oracle), D02 (braid-word computation).
**See also**: State Sum, Delta=0, Kauffman Bracket.

---

## Braid Group
**Plain**: The mathematical group of ways to interweave n strands, where each crossing (one strand passing over another) is a generator, and two braids are equivalent if one can be continuously deformed into the other.
**Precise**: B_n is generated by sigma_1, ..., sigma_{n-1} with relations sigma_i*sigma_{i+1}*sigma_i = sigma_{i+1}*sigma_i*sigma_{i+1} (Yang-Baxter) and sigma_i*sigma_j = sigma_j*sigma_i for |i-j| >= 2. Braid words are the sequential input to the DKC reservoir; crossing count = depth.
**Introduced**: D01-D09 (foundation arc).
**See also**: Temperley-Lieb Algebra, Crossing Depth, Reservoir Computing.

---

## Complement-Blindness Theorem
**Plain**: When you combine magnitude and phase information into a single classification cell, it can no longer distinguish a Boolean input from its bitwise complement -- it merges m with ~m. Separating phase into its own cell recovers all 13 function types.
**Precise**: The combined_cell activation maps bitwise-complement input pairs (m, ~m) to the same cell; the resulting truth table can represent at most 11 NPN classes. The phase_cell (phase information alone) recovers all 13 NPN classes, proving the circuit complexity hierarchy AND/XOR ratio = 1 -> 2762 -> infinity (N=3 -> 8).
**Introduced**: D93.
**See also**: NPN Classification, 11/13 Theorem, Parity-Lock Theorem.

---

## Crossing Depth
**Plain**: How many crossings (braid generator multiplications) were needed to produce a given bracket value -- deeper entries required more complex braids and carry more computational power.
**Precise**: The BFS birth round of a catalog entry: the minimum number of braid generator multiplications needed to produce it. The linear depth law max_xor ~ depth + 6 says XOR capacity scales linearly with crossing depth, and this is the fundamental scaling variable (the log law is a corollary).
**Introduced**: D82.
**See also**: BFS Catalog, Log Law, Reservoir Computing, Balanced Exponentials.

---

## Cyclotomic Integer
**Plain**: A number built by adding and multiplying ordinary integers with roots of unity (special complex numbers on the unit circle whose powers eventually equal 1) -- these are exact, with no rounding error.
**Precise**: An element of the ring Z[zeta_N] = Z[e^{2*pi*i/N}], the ring of integers of the cyclotomic field Q(zeta_N). DKC uses Z[zeta_8] (primary, delta=0), Z[zeta_16] (delta=sqrt(2)), Z[zeta_5] (delta=phi), and Z[zeta_24] (delta=sqrt(3)). All arithmetic is exact integer arithmetic on coefficient tuples.
**Introduced**: D27 (identification), D29 (exact arithmetic implemented).
**See also**: Z[zeta_8], Z[zeta_12], Z[zeta_16], Habiro.

---

## Delta=0
**Plain**: The special evaluation point where the loop value in the Kauffman bracket is zero, meaning any diagram state with more than one closed loop contributes nothing -- only single-loop states survive, making everything simpler and exact.
**Precise**: At A = e^{i*5*pi/4}, delta = -(A^2 + A^{-2}) = 0 exactly. The state sum reduces to single-loop states only. TL generators satisfy e_i^2 = 0 (nilpotent). The algebra is non-semisimple for even n (alternating semisimplicity). This is the primary DKC evaluation point.
**Introduced**: D29.
**See also**: Axiality Theorem, Non-Semisimplicity, Sandwich Theorem.

---

## DKC (Discrete Knotted Computation)
**Plain**: The central thesis of this research: topological invariants from knot theory can be directly compiled into neural network weights that compute Boolean functions without any training.
**Precise**: Forward DKC: evaluate the Kauffman bracket at a root of unity to obtain cyclotomic integer values, use these as weights in a complex-valued neuron with an appropriate activation function; the algebraic structure of the bracket guarantees correct Boolean computation without gradient descent. Reverse DKC: decompose trained neural network weights back into bracket values.
**Introduced**: D26-D29 (reverse then forward).
**See also**: Forward DKC, Bracket Polynomial, Five Pillars.

---

## Dual-Channel Theorem
**Plain**: For a set of bracket values to be capable of computing parity, it needs two things simultaneously: its values must be able to multiply into each other (phase coherence) AND they must span enough different magnitudes (magnitude diversity). Missing either one kills parity.
**Precise**: Parity capability requires both (1) nonzero product closure (multiplicative channel: products of values remain in the vocabulary) and (2) nontrivial v_2 connectivity (additive channel: magnitude diversity across 2-adic valuation classes). Maps onto the T-gate/Hadamard decomposition of quantum circuit synthesis (Amy-Glaudell-Ross 2023). Polarity of the multiplicative channel is encoding-dependent (D109).
**Introduced**: D108 (delta=0), D109 (delta=sqrt(2), encoding dependence).
**See also**: Parity-Lock Theorem, Product Closure, Z[zeta_8].

---

## Encoding
**Plain**: How Boolean input bits are translated into complex numbers before being processed -- different translation rules make different functions computable.
**Precise**: The map from {0,1}^n to weight selections. Three encodings studied: (1) 1-weight-per-input (1wpi): each input bit selects one catalog entry independently, all 13 NPN classes accessible (D48/D50); (2) +/-q paired: each weight contributes +q, -q, or 0, structurally parity-locked (D92); (3) multiplicative: z(a,b,c) = w1^a * w2^b * w3^c, 11/13 classes under half-plane (D61-D62). Encoding determines function.
**Introduced**: D29 (1wpi), D61 (multiplicative), D92 (+/-q).
**See also**: Parity-Lock Theorem, Activation Function, NPN Classification.

---

## Ext^1 (Extension Groups)
**Plain**: A measure of how two algebraic modules can be glued together in nontrivial ways -- when Ext^1 is nonzero, the gluing creates indecomposable structures (modules that cannot be split apart) which carry computational load.
**Precise**: Ext^1_{TL}(L_i, L_j) measures non-split extensions between simple TL modules. When nonzero, it produces indecomposable modules with radical structure. D99-D101 demonstrate that non-semisimple extensions (Ext^1 nonzero) are computationally load-bearing: W_{4,2} (radical, D100) achieves XOR14=70 while W_{5,3} (simple, D101) caps at XOR12.
**Introduced**: D99-D101.
**See also**: Non-Semisimplicity, Radical, Module.

---

## Five Pillars
**Plain**: The five established fields whose intersection defines DKC -- Habiro (bracket values are algebraic integers), Nazer-Gastpar (algebraic lattices compute), Abramsky (TL diagrams encode computation), Aizenberg (roots-of-unity neurons), and Reservoir Computing (fixed dynamics compute through structure).
**Precise**: The five-pillar chain: Abramsky (TL = computation) -> Habiro (bracket values in Z[zeta_N]) -> Aizenberg (MVN readout) -> Nazer-Gastpar (ring structure guarantees exactness) -> Reservoir Computing (braid depth = memory capacity). No paper in any field cites papers from the other four. The intersection is genuinely unoccupied.
**Introduced**: Synthesized across D29-D94; Pillar 5 added after D73/D82.
**See also**: DKC, Reservoir Computing, Temperley-Lieb Algebra.

---

## Forward DKC
**Plain**: The "forward" direction of DKC: start from knot topology, compute bracket values, and discover they already work as neural network weights for Boolean functions -- no training needed.
**Precise**: Exact Z[zeta_8] bracket values at A = e^{i*5*pi/4} compute XOR (and other Boolean functions) without training. 100+ valid XOR triples found with RMS = 0.000 from exact bracket values with gauge rotation. First confirmed in D29 (LANDMARK).
**Introduced**: D29.
**See also**: DKC, BFS Catalog, Z[zeta_8].

---

## Framing Cost
**Plain**: The Jones polynomial normalization removes a writhe-dependent phase factor from the bracket; this removal costs exactly 2 XOR levels of computational capacity at every root of unity tested.
**Precise**: Jones normalization divides by (-A^3)^{-writhe}, absorbing the Reidemeister I framing anomaly. This destroys exactly 2 XOR levels: zeta_12 bracket XOR12 -> Jones XOR10; zeta_8 bracket XOR8 -> Jones XOR6. The TQFT framing anomaly that physicists normalize away is precisely what DKC uses for computation.
**Introduced**: D83.
**See also**: Writhe, Crossing Depth, Bracket Polynomial.

---

## Hopf Fibration
**Plain**: A way of decomposing the 3-sphere (S^3, where unit quaternions live) into a 2-sphere base (S^2, rotation axes) and circular fibers (S^1, phases) -- in DKC, all computation lives on the base and the fiber is completely inert.
**Precise**: The Hopf map S^3 -> S^2 sends a unit quaternion to its rotation axis. The S^1 fiber (Hopf phase xi_1) carries zero DKC information (zero XOR6 solutions at all k=4..32, r=-0.06 with spatial direction). The S^2 base carries all computational content. Music maps to fiber (pitch); logic maps to base (direction).
**Introduced**: D67 (fiber inertness proved), D70 (music-space orthogonality confirmed).
**See also**: S^2 Sweet Spot, 24-Cell, 13=13 Bandwidth Theorem.

---

## Hub (Super-Hub)
**Plain**: A bracket value that appears in an unusually large number of valid Boolean function solutions -- the most versatile weights in the catalog, typically found at intermediate depths.
**Precise**: Catalog entries with high participation rates across winning tuples. In the S^2 Voronoi framework, a "super-hub" is a direction appearing in a disproportionate fraction of XOR solutions. The hub structure reflects the relational nature of DKC: hubs are values that combine well with many partners, not values that are individually special.
**Introduced**: D48-D50 (participation analysis), D88 (rigid anchors vs flexible scaffolding).
**See also**: BFS Catalog, Null State, Relational Computation.

---

## k-Sector MVN
**Plain**: An activation function that divides the complex plane into k equal pie-slice sectors and classifies outputs by which sector they land in -- at k=6 sectors, parity becomes computable from the same catalog that was hopeless under simpler activations.
**Precise**: Aizenberg's multi-valued neuron activation partitioning the complex plane into k angular sectors of width 2*pi/k. At k=6 with Z[zeta_8] weights: all 13 NPN classes computable, parity achieved with 906 solutions. Solution count is non-monotonic in k (906@k=6 > 756@k=7 > 96@k=8) due to incommensurability with the pi/4 lattice structure.
**Introduced**: D50.
**See also**: Activation Function, Triskelion Principle, Parity Wall.

---

## Kauffman Bracket
**Plain**: See Bracket Polynomial. "Kauffman bracket" and "bracket polynomial" are used interchangeably throughout the project.
**Precise**: <K>(A) = sum over smoothing states of A^{sigma(s)} * delta^{loops(s)-1}. Distinguished from the Jones polynomial by the absence of writhe normalization.
**Introduced**: D01.
**See also**: Bracket Polynomial, State Sum, Delta=0.

---

## Link State (Planar Matching)
**Plain**: A pattern of non-crossing arcs connecting pairs of boundary points -- these are the basis elements of the Temperley-Lieb algebra, representing ways strands can reconnect after smoothing crossings.
**Precise**: A planar perfect matching on 2n boundary points: n non-crossing arcs. These form the standard basis for TL_n. The number of link states at strand count n is the Catalan number C_n. Through-strand count j partitions link states into cellular sectors with d(n,j)^2 basis elements each.
**Introduced**: D01 (implicit), D03 (explicit TL basis), D39 (cellular decomposition).
**See also**: Temperley-Lieb Algebra, Planar Matching, State Sum.

---

## Log Law
**Plain**: The maximum XOR arity (number of inputs) computable from a bracket catalog scales logarithmically with catalog size -- a corollary of the more fundamental linear depth law.
**Precise**: max_xor ~ 0.62 * log_2(catalog_size) + 4.6 (measured at zeta_12). This is a corollary of the linear depth law (max_xor ~ depth + 6) combined with exponential BFS growth (~2x entries per depth round), giving log_2(catalog) ~ depth.
**Introduced**: D81 (discovered), D82 (explained as corollary of depth law).
**See also**: Crossing Depth, Balanced Exponentials, BFS Catalog.

---

## Module (Standard, Projective, Simple, Non-Semisimple)
**Plain**: Different "representations" of the Temperley-Lieb algebra -- standard modules are the basic ones (indexed by through-strand count), simple modules are the irreducible building blocks, projective modules are their "covers," and non-semisimple modules are those that cannot be split into simple pieces.
**Precise**: Standard module W_{n,j} = TL_n acting on link states with j through-strands, dimension d(n,j). Simple module L_j = W_{n,j}/rad(W_{n,j}). Projective indecomposable P_{j,j} = projective cover of L_j, dimension C_{n/2+1} for j=0. Non-semisimple = has nonzero radical; at delta=0 this occurs for even n.
**Introduced**: D03 (standard), D38 (quotient), D51 (radical anatomy), D85-D86 (projective covers), D99-D101 (multi-strand modules).
**See also**: Temperley-Lieb Algebra, Radical, Sandwich Theorem, Ext^1.

---

## Neglecton
**Plain**: A bracket value that is normally invisible (zero at delta=0) but becomes visible under a small perturbation away from delta=0 -- these capture information from two-loop states that the strict delta=0 evaluation kills.
**Precise**: First-order perturbation bracket values around delta=0: from states with loops=2, weighted by scale factor 4 = d(delta)/d(epsilon). 148 new Z[zeta_8] values from 15,242 previously invisible braids. All 116 genuinely new values share angles with existing standard values -- no new angular directions. The perturbation approach is closed.
**Introduced**: D49.
**See also**: Delta=0, Null State, Axiality Theorem.

---

## Non-Semisimplicity
**Plain**: A property of the TL algebra at certain evaluation points where it cannot be decomposed into independent simple pieces -- there are "stuck-together" parts (the radical) that create both obstacles and opportunities for computation.
**Precise**: TL_n(delta) is non-semisimple when the radical rad(TL_n) is nonzero. At delta=0, this occurs for even n (alternating semisimplicity). Non-semisimplicity creates the sandwich structure (Loewy length 3), enables exact DKC via single-loop reduction, but blocks parity through radical-carried abelian characters (Barrington-Radical Principle).
**Introduced**: D38 (dimension reduction), D51 (sandwich theorem), D85 (indecomposability parameter b=-5/8), D102 (Barrington connection).
**See also**: Radical, Sandwich Theorem, Barrington's Theorem, Delta=0.

---

## NPN Classification
**Plain**: A way of grouping Boolean functions into equivalence classes by allowing input/output negation and input permutation -- for 3 inputs there are exactly 14 classes (1 trivial + 13 nontrivial), and DKC's goal is to compute as many as possible.
**Precise**: NPN (Negation-Permutation-Negation) equivalence classes of Boolean functions. For n=3 inputs: 256 functions collapse to 14 NPN classes. DKC achieves all 13 nontrivial classes at k=6 MVN (D50), 11/13 under half-plane (D61-D62). The four-tier hierarchy (k=2:5, k=3:+1, k=4:+6, k=6:+1) partitions classes by minimum sector count.
**Introduced**: D23 (classification system), D45 (all 13 mapped), D50 (all 13 achieved).
**See also**: Activation Function, 11/13 Theorem, Parity Wall.

---

## Null State
**Plain**: A bracket catalog entry whose Kauffman trace is zero -- invisible to the standard topological measurement, yet indispensable for computation at finite groups because they provide unique directions in the quaternion space.
**Precise**: Catalog entries with Re(q) = 0 (bracket-null). At zeta_8: 9 of 24 entries, covering 6 exclusive S^2 directions (cube-edge midpoints). Removing nulls drops XOR from 8 to 6 (below random baseline). Null indispensability is regime-dependent: critical at finite groups (zeta_8), dispensable at infinite groups (zeta_12, D87). Related to LCFT Jordan-cell logarithmic partners.
**Introduced**: D84 (null indispensability), D87 (regime transition).
**See also**: BFS Catalog, Crossing Depth, Non-Semisimplicity.

---

## Parity Lock (Parity-Lock Theorem)
**Plain**: Under the +/-q paired encoding, the only Boolean functions that can ever be computed are XOR and XNOR -- AND, OR, majority, and threshold functions are structurally impossible regardless of activation or depth.
**Precise**: Under +/-q encoding with k weights, the (0,0)=(1,1) collision creates 3^k equivalence classes. XOR/XNOR are the only standard Boolean functions constant on all classes. Sign-flip symmetry combined_cell(S) = combined_cell(-S) further halves distinguishable groups. Formally proved (P04, proofs/parity-lock-theorem.md).
**Introduced**: D92.
**See also**: Encoding, Parity Wall, Circuit Complexity.

---

## Parity Wall
**Plain**: An apparent impossibility barrier where parity (XOR) seems unreachable -- the project encounters and resolves this wall three times, each time locating the obstruction in a different component (activation, activation again, then group finiteness).
**Precise**: The "wall was X not Y" pattern: (1) D48->D50: wall was activation (split-sigmoid), not lattice; resolved by k-sector. (2) D76->D77: wall was activation (S^2-only), not root; resolved by S^1 x S^2 product. (3) D78->D79: wall was group finiteness (binary octahedral, 24 elements), not architecture; resolved by zeta_12 (infinite group).
**Introduced**: D48 (first wall), D76 (second wall), D78 (third wall).
**See also**: Wall Pattern, Activation Function, k-Sector MVN.

---

## Planar Matching
**Plain**: See Link State. A non-crossing pairing of boundary points, forming the basis of the TL algebra.
**Precise**: A perfect matching on {1,...,2n} with no crossing arcs. Counted by the Catalan number C_n. Planar matchings are both the TL basis and the smoothing states of the bracket state sum.
**Introduced**: D01.
**See also**: Link State, Temperley-Lieb Algebra, State Sum.

---

## Product Closure
**Plain**: Whether multiplying two values from a vocabulary set produces another value still in that set -- in DKC, high product closure is the single strongest algebraic predictor of parity capability (at delta=0 with additive encoding).
**Precise**: For a set V of Z[zeta_N] values, product closure counts pairs (v_i, v_j) where v_i * v_j (modulo axis, i.e., ignoring the zeta_8^k phase factor) is also in V. At delta=0: parity-capable values have 17 product closure edges (connected graph); poison values have 0 (empty graph). Polarity inverts under multiplicative encoding (D109).
**Introduced**: D108 (product closure as discriminator), D109 (encoding-dependent polarity).
**See also**: Dual-Channel Theorem, Z[zeta_8], Encoding.

---

## Radical
**Plain**: The "stuck-together" part of a non-semisimple algebra -- elements that act as zero in every simple quotient. In DKC, the radical carries only writhe (framing) information and is provably useless for computing parity.
**Precise**: rad(TL_n) = intersection of all maximal left ideals. At delta=0 for even n: rad(TL_{2k}) has dimension C_{2k} - C_{2k-1}; rad^2 has dimension C_{2k-1}; Loewy length is exactly 3 (Sandwich Theorem). The radical carries an abelian character (writhe homomorphism), making it computationally inert by Barrington's theorem.
**Introduced**: D38 (dimension), D51 (anatomy), D52 (cross-ell), D85 (indecomposability parameter b=-5/8), D102 (Barrington-Radical Principle).
**See also**: Sandwich Theorem, Non-Semisimplicity, Barrington's Theorem.

---

## Raqiya
**Plain**: A reusable C89 library for analyzing cyclotomic integer values, providing exact arithmetic and six relationship detectors that reveal algebraic structure in bracket catalogs.
**Precise**: Single-header C89 library (raqiya/raqiya.h) implementing Z[zeta_8] arithmetic and 6 detectors: root orbits, norm classes, Galois orbits, axis alignment, pair histograms, and full analysis. Used for algebraic fingerprinting of bracket catalogs. 86 unit tests. See CLAUDE.md for compilation and usage.
**Introduced**: D107 (library created and validated).
**See also**: Scrutinium, Z[zeta_8], Cyclotomic Integer.

---

## Reservoir Computing
**Plain**: A framework where a fixed, untrained dynamical system processes sequential inputs and only a simple output layer is trained -- DKC is exactly this, with the braid group as the reservoir and the activation function as the readout.
**Precise**: DKC maps to RC: braid word = sequential input, SU(2) matrix multiplication = reservoir dynamics, quaternion state = reservoir state, Voronoi/sector activation = readout. The linear depth law (max_xor ~ depth + 6) is the RC memory capacity theorem applied to the braid reservoir. The finite-to-infinite group transition (zeta_8 -> zeta_12) maps to the ordered-to-edge-of-chaos transition. Pillar 5 of the five-pillar framework.
**Introduced**: D73 (eigenvector automaton = reservoir), D82 (depth law = memory capacity).
**See also**: Five Pillars, Crossing Depth, BFS Catalog, Braid Group.

---

## S^2 Sweet Spot
**Plain**: The 2-sphere of rotation axes is the natural arena for DKC computation -- it outperforms both the full quaternion space (S^3, which has a redundant fiber) and the flat plane (R^2, which loses essential curvature).
**Precise**: A custom 13-direction Voronoi on S^2 (eigenvector directions of the binary octahedral group) achieves 36 XOR6 solutions at 14 cells, exceeding the 25-cell S^3 Voronoi (35 solutions). Platonic solid Voronois yield zero. Euclidean R^2 metric gives zero (D68: intrinsically curved). The computation lives on S^2, not S^3 or R^2.
**Introduced**: D67 (S^2 beats S^3), D68 (intrinsic curvature), D72 (algebraic not geometric).
**See also**: Hopf Fibration, 13=13 Bandwidth Theorem, 24-Cell.

---

## Sandwich Theorem (Loewy Length 3)
**Plain**: The non-semisimple TL algebra at delta=0 has a three-layer structure: a semisimple top, a middle "radical" layer, and a semisimple bottom that is a dimensional mirror of the top -- like a sandwich.
**Precise**: For k >= 2, TL_{2k}(delta=0) has Loewy length exactly 3: Top = TL_{2k}/rad (dim C_{2k-1}, isomorphic to TL_{2k-1}(0)), Middle = rad/rad^2 (dim C_{2k} - 2*C_{2k-1}), Bottom = rad^2 (dim C_{2k-1}, isomorphic to TL_{2k-1}(0) as bimodule). NOVEL result -- papers prove head=socle for individual PIMs but never identify rad^2 AS the previous odd TL algebra.
**Introduced**: D51.
**See also**: Radical, Non-Semisimplicity, Module.

---

## Scrutinium
**Plain**: A generic algebraic analysis framework that provides 14-18 universal detectors for any algebraic type, replacing hand-written per-type analysis code with a single reusable scaffolding system.
**Precise**: Generic algebraic analysis library (scrutinium.h) using scaffolding + genus vtable pattern. 14-18 universal detectors auto-registered per type, ~50 lines to adopt a new type. 13 SCR_WRAP_* macros for wrapper boilerplate reduction (~47%). Includes Scr_Signatura for partition lattice topology fingerprinting. 230 tests across 4 suites, 3 validated types (Z[i], Z[omega], Z[zeta_8]).
**Introduced**: Built as generalization of Raqiya detectors.
**See also**: Raqiya, Z[zeta_8], Cyclotomic Integer.

---

## Sign-Hash
**Plain**: An activation function that projects each entry of a braid representation matrix to its sign ({-1, 0, +1}), creating a binary/ternary fingerprint -- equivalent to 1-bit compressed sensing.
**Precise**: Ternary sign quantization of TL module matrix entries: each entry maps to sgn(entry) in {-1, 0, +1}. Equivalent to 1-bit compressed sensing (Boufounos-Baraniuk 2008). Can expand effective rank beyond Z-linear rank (sign-rank expansion: W_{6,2} goes from 244 to 292). Optimal component count follows an Atkinson sweet-spot curve.
**Introduced**: D104.
**See also**: Activation Function, k-Sector MVN, Module.

---

## State Sum
**Plain**: A computation that sums over all possible configurations (states) of a system, weighting each by some algebraic factor -- the Kauffman bracket is computed this way by summing over all possible crossing smoothings.
**Precise**: <K> = sum_{s in {A,B}^n} A^{sigma(s)} * delta^{loops(s)-1}, where sigma(s) counts A-choices minus B-choices and loops(s) counts closed curves after smoothing. At delta=0, only single-loop states (loops=1) survive.
**Introduced**: D01 (first implementation).
**See also**: Bracket Polynomial, Delta=0, Link State.

---

## Temperley-Lieb Algebra (TL_n)
**Plain**: A finite-dimensional algebra whose basis elements are non-crossing strand diagrams, with multiplication defined by stacking diagrams and replacing closed loops with a scalar delta -- the algebraic engine that connects knot theory to computation.
**Precise**: TL_n(delta) is generated by e_1, ..., e_{n-1} with relations e_i^2 = delta*e_i, e_i*e_{i+1}*e_i = e_i, e_i*e_j = e_j*e_i (|i-j|>=2). Dimension = Catalan number C_n. The bracket factors through TL: braids map to TL via the Kauffman functor, and the bracket is the Markov trace on TL. At delta=0, generators are nilpotent (e_i^2=0).
**Introduced**: D03 (first TL implementation), D35 (TL matrices over Z[zeta_8]).
**See also**: Bracket Polynomial, Module, Radical, Non-Semisimplicity.

---

## Topological Entropy
**Plain**: A measure of how chaotic a braid's dynamics are (how fast nearby points separate under the braid's action) -- surprisingly, this is completely irrelevant to DKC computation.
**Precise**: Thurston-Nielsen classification assigns braids as periodic (zero entropy) or pseudo-Anosov (positive entropy). D106 proves topological entropy is completely orthogonal to DKC: both types compute XOR at 100% participation with near-identical scores. The TL quotient at delta=0 (e_i^2=0) kills expanding eigenvalues, erasing all dynamical information while preserving algebraic structure.
**Introduced**: D106 (proved null).
**See also**: Reservoir Computing, Crossing Depth, Relational Computation.

---

## Triskelion Principle
**Plain**: For 3-input parity at k=6 sectors, each of the 3 input weights must occupy its own odd-numbered sector at roughly 120-degree spacing -- a three-armed geometric pattern.
**Precise**: For 3-input parity with k=6 MVN sectors on Z[zeta_8], all three weights are confined to octants {2,4,5,7} (class-1 angles) and must interleave around the unit circle. Generalizes: 4-input uses tetraskelion at k=8, 5-input uses pentaskelion at k=15. Specific to each input arity; the k=2n generalization was falsified.
**Introduced**: D50 (discovery), D63 (generalization and falsification).
**See also**: k-Sector MVN, Parity Wall, NPN Classification.

---

## Wall Pattern ("Wall Was X Not Y")
**Plain**: A recurring structural pattern in the project: an apparent impossibility is confirmed exhaustively, then the specific component causing the wall is identified and changed, resolving the impossibility while keeping everything else fixed.
**Precise**: Three instances: (1) D48->D50: wall was activation (split-sigmoid), resolution k=6 MVN. (2) D76->D77: wall was activation (S^2-only), resolution S^1 x S^2 product. (3) D78->D79: wall was group finiteness (24-element binary octahedral), resolution zeta_12 (infinite group). The pattern teaches: locate the specific component enforcing the wall before concluding a function is unreachable.
**Introduced**: D48 (first instance).
**See also**: Parity Wall, Activation Function, DKC.

---

## Writhe
**Plain**: The net signed crossing count of a braid -- how much it twists overall in one direction versus the other. In DKC, writhe is an independent computational resource worth +2 XOR levels.
**Precise**: writhe(beta) = sum of signs of crossings in the braid word (sigma_i contributes +1, sigma_i^{-1} contributes -1). Jones normalization removes the factor (-A^3)^{-writhe}. Writhe is independent of depth (r=0.14 correlation). The radical carries the writhe as an abelian character (B_n -> Z/8Z via A=-zeta_8), proved computationally for all 32,768 entries of W_{6,4} (D102).
**Introduced**: D09 (Reidemeister moves), D83 (computational resource), D102 (writhe character theorem).
**See also**: Framing Cost, Barrington's Theorem, Radical.

---

## Z[zeta_8]
**Plain**: The ring of integers built from the 8th root of unity -- every element is a 4-tuple of ordinary integers (a + b*zeta_8 + c*zeta_8^2 + d*zeta_8^3). This is the primary number system for DKC at delta=0, enabling exact computation with zero floating-point error.
**Precise**: Z[zeta_8] = Z[e^{2*pi*i/8}], the ring of integers of Q(zeta_8). Basis {1, zeta_8, zeta_8^2, zeta_8^3} with zeta_8^4 = -1. Class number 1 (unique factorization domain). The BFS catalog at 2 strands contains 24 distinct values; at delta=0 the axiality theorem ensures each value uses at most one basis direction.
**Introduced**: D27 (identified), D29 (implemented).
**See also**: Cyclotomic Integer, Axiality Theorem, Delta=0, Raqiya.

---

## Z[zeta_12]
**Plain**: The cyclotomic ring at the 12th root of unity, used when zeta_8's finite group (24 elements) is too small -- its infinite braid group breaks through the XOR8 ceiling, reaching XOR12 and beyond.
**Precise**: Z[zeta_12] = Z[e^{2*pi*i/12}], evaluation at half-angle pi/6. The SU(2) braid group at zeta_12 is infinite (>4096 entries at 8 closure rounds), breaking the capacity ceiling imposed by zeta_8's finite binary octahedral group. max_xor ~ 0.62 * log_2(catalog) + 4.6. Null fraction drops to 3% (vs 37.5% at zeta_8).
**Introduced**: D79 (first use), D81 (scaling law), D82 (depth law).
**See also**: Z[zeta_8], BFS Catalog, Log Law, Reservoir Computing.

---

## Z[zeta_16]
**Plain**: The cyclotomic ring at the 16th root of unity, corresponding to delta=sqrt(2) -- a richer algebraic setting where bracket values use a 2D subspace (Gaussian integers times axes) instead of the 1D axiality of zeta_8.
**Precise**: Z[zeta_16] = Z[e^{2*pi*i/16}], evaluation at ell=4, delta=sqrt(2). Bracket values are Z[i]-axial (2D subspace). The parity ceiling n=5 is unchanged from Z[zeta_8] under standard activations, but generalized XOR6 appears at k=24 (D65). D109 uses this ring to demonstrate encoding-dependent polarity inversion of the dual-channel theorem.
**Introduced**: D54 (Z[i]-axiality), D65 (generalized XOR6 at k=24), D109 (dual-channel encoding dependence).
**See also**: Z[zeta_8], Cyclotomic Integer, Axiality Theorem, Dual-Channel Theorem.
