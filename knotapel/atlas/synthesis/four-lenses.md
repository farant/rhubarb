# Four Lenses on One Object

The DKC research is one thing seen from four directions. These are not four research programs that happen to share data. They are four coordinate systems on the same manifold. Every major result lives in at least two lenses; the deepest results live in all four.

---

## Lens 1: Representation Theory

**What the research looks like:** The Temperley-Lieb algebra TL_n(delta) at roots of unity is a non-semisimple finite-dimensional algebra whose radical structure controls what the Kauffman bracket can and cannot compute. The radical is not noise. It is the precise algebraic object that separates computable from incomputable Boolean functions.

**Central demos:** D35 (TL_n matrices over Z[zeta_8], the keystone), D51 (radical anatomy at delta=0 -- the sandwich theorem, Loewy length 3, Catalan trace theorem, Peirce block matching), D52 (Chebyshev generalization across ell=2-6, universal nilpotency 3, cross-ell radical dimension formulas), D06 (Jones-Wenzl idempotents as module projectors).

**Core results belonging to this lens:**
- The sandwich theorem: rad^2(TL_{2k}(0)) is isomorphic to TL_{2k-1}(0) as a bimodule. Three Loewy layers with dimensional mirrors.
- Alternating semisimplicity: odd n is semisimple, even n is not. TL_3(0) is semisimple despite nilpotent generators.
- Universal nilpotency 3: rad^3 = 0 at ALL roots of unity tested (ell=2-6). The cellularity of TL algebras forces this.
- Fibonacci rank theorem: the total bilinear rank of the rad^2 generator equals F(ell-1). Fibonacci numbers arise from fusion rules in the Bratteli diagram.
- Cross-ell radical dimension formulas: rad(TL_ell) = 2ell-3, rad(TL_{ell+1}) = ell^2-ell-3.

**Open questions specific to this lens:**
- The Galois symmetry conjecture: does the sandwich duality (top isomorphic to bottom) correspond to a Galois automorphism of Gal(Q(zeta_8)/Q)?
- Palindromic sector rank conjecture for even ell.
- ell=7 predictions: first cubic number field case, would validate all universal formulas.

**Connections to other lenses:**
- To Coding Theory: the radical is what the bracket evaluation quotients out. The semisimple quotient TL/rad is the "visible" part; the radical is the "invisible" lattice dimension. This is exactly the information-theoretic partition into accessible and inaccessible bits.
- To Quantum Mechanics: the radical corresponds to decoherence. The decoherence rate scaling ((2ell-3)/C_ell decreasing with ell) is WHY Fibonacci anyons (ell=5, 17% radical) are better for quantum computing than Ising anyons (ell=4, 36%).
- To Approximation Theory: the quantum dimensions [n]_q satisfy the Chebyshev recurrence. The Jones-Wenzl idempotents ARE Chebyshev polynomials evaluated on TL generators. The radical appears precisely where the Chebyshev polynomial vanishes.

---

## Lens 2: Coding Theory

**What the research looks like:** The bracket catalog is a codebook. Z[zeta_8] is a rank-4 lattice over Z, and each bracket value is a codeword. The question "which Boolean functions can be computed?" becomes "which truth tables are achievable via lattice codewords as neural network weights?" This is Nazer-Gastpar's compute-and-forward applied not to relay channels but to topological invariants.

**Central demos:** D29 (forward DKC -- exact Z[zeta_8] arithmetic, 65K+ XOR triples from lattice), D48 (100M quartet exhaustive search -- the parity wall as a coding-theoretic impossibility), D46 (capacity analysis: ~4.3 bits magnitude + ~1.5 bits phase = ~5.8 bits/symbol), D47 (two-channel coding conjecture, topology-frequency correlation reversal), D12 (quantization geometry, BitNet parallels).

**Core results belonging to this lens:**
- Axiality theorem: at delta=0, bracket values are Z-axial in Z[zeta_8] -- only one coordinate is nonzero. The full rank-4 lattice collapses to rank 1. This is extreme constellation collapse.
- Axiality hierarchy: ell=2 is Z-axial (1D), ell=3 is Z[zeta_6]-axial (1D), ell=4 is Z[i]-axial (2D), ell=5 is full Z[zeta_5] (4D). Richer rings enable richer codebooks.
- Phase decoherence thesis (D46): split-sigmoid accesses ~4.3 bits (magnitude channel), MVN-continuous accesses a different ~1.5 bits (phase channel). These are literally complementary codes.
- Catalog size hierarchy: ell=3 (6 values, binary collapse), ell=4 (56 values), delta=0 (100 values), ell=5 (116 values). Non-monotonic: delta=0 has a uniquely favorable catalog despite the simplest algebra.
- The 11/13 half-plane theorem (D61-62): a semicircular decision region on additive angular sums can reach exactly 11 of 13 NPN classes. This is a covering radius / quantization geometry result.

**Open questions specific to this lens:**
- Two-channel coding conjecture: can a joint decoder for magnitude + phase achieve ~5.8 bits/symbol?
- Does the catalog size obey a lattice theta function identity?
- What is the covering radius of the Z[zeta_8] bracket catalog in the complex plane?
- CSS feasibility is already ruled out (radical is self-orthogonal under Gram form but not standard inner product). Are there other algebraic coding constructions that work?

**Connections to other lenses:**
- To Representation Theory: the radical is the "invisible" sublattice. Bracket evaluation projects through TL/rad, killing radical codewords. The neglecton weights (D49) are first-order perturbations recovering radical information -- 148 new values, but zero new angles. The lattice geometry is preserved by the projection.
- To Quantum Mechanics: the bracket value IS a Chern-Simons amplitude (Costello-Francis-Gwilliam 2026 proved Witten = RT). So the codebook is literally a table of quantum transition amplitudes.
- To Approximation Theory: the 11/13 theorem is fundamentally about angular geometry. The proof that 0x1B and 0x06 are unreachable uses semicircle bounds -- the activation boundary is a half-plane, and certain truth tables require weight angles that cannot simultaneously satisfy the angular constraints. This is polynomial approximation theory on the circle.

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

**Open questions specific to this lens:**
- Fibonacci TQFT boundary (mentioned at D05, n=5): what happens when we cross into the Fibonacci anyon regime?
- Connection to volume conjecture: WRT invariants at large r relate to hyperbolic volume -- does this have DKC implications?
- The entanglement vanishing theorem breaks at delta != 0 (D53). What is the precise crossover?
- Can multi-readout from different braid closures (D21, D25) be understood as a quantum measurement basis?

**Connections to other lenses:**
- To Representation Theory: the R-matrix lives in the TL algebra. The quantum trace is computed via Jones-Wenzl projectors. Schur-Weyl duality decomposes tensor products into TL standard modules. The quantum trace formula unifies separate module traces (D06) into a single computation (D04).
- To Coding Theory: the bracket amplitude at a root of unity IS a lattice point in Z[zeta_{2ell}]. The quantum amplitude is literally an algebraic integer. This is what makes forward DKC possible: quantum mechanics provides exact integer codewords.
- To Approximation Theory: the delta error landscape |delta| = 2|cos(2theta)| is a Chebyshev polynomial evaluated at the angle. Gate viability is determined by approximation quality: how close is the evaluation point to a root of the Chebyshev polynomial?

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

**Open questions specific to this lens:**
- Can the cyclic zero pattern {8,0,0,8,4,4} be explained by a Chebyshev identity?
- Is there a Chebyshev polynomial characterization of which NPN classes require k sectors?
- Can the triskelion principle (each input needs its own odd sector at 120-degree intervals) be derived from approximation-theoretic bounds on angular separation?
- Does the incommensurability hypothesis generalize: for n-input parity, is the minimum sector count k=2n?

**Connections to other lenses:**
- To Representation Theory: the Chebyshev recurrence IS the quantum dimension recurrence. When [ell]_q = 0, the Jones-Wenzl idempotent p_ell is singular and the algebra becomes non-semisimple. Approximation theory predicts EXACTLY where the algebra changes character.
- To Coding Theory: the 11/13 theorem is a covering problem. The 2 unreachable NPN classes are truth tables that fall outside the covering radius of any angular constellation under half-plane quantization. The proofs use semicircle geometry, which is approximation theory.
- To Quantum Mechanics: the delta error landscape |delta| = 2|cos(2theta)| is a Chebyshev polynomial. Gate viability (D20) is controlled by how close the evaluation angle is to a Chebyshev root. The "magic angle" 5pi/4 is magic precisely because it is a root.

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

---

*Generated 2026-02-20. Source: demo-index.md (47 demos), theorems.md, data-tables.md, four-pillars.md.*
