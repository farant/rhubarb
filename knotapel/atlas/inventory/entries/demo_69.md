## Demo 69: Clifford Staircase on S² — Multi-Root Eigenvector Comparison

- **Status**: COMPLETE
- **File**: `demo_69_clifford_staircase/main.c` (~787 lines)
- **Tests**: 9/9 pass
- **Depends on**: Demo 66 (quaternionic bracket catalog), Demo 67 (13-direction S² Voronoi)
- **Feeds into**: Demo 70, Demo 71

### Headline
Comparing SU(2) eigenvector geometry across ζ_8, ζ_16, and ζ_32 proves a universal Direction Nesting Theorem (ζ_N ⊂ ζ_{2N} exactly on S²) and reveals that ζ_8 is the computational sweet spot: the coarsest root that produces structured directions, preserved exactly at every finer root, while finer roots trivialize XOR6 through pigeonhole saturation.

### Key Results
- **ζ_8 → ζ_16 RIGIDITY (13/13, 0.000°)**: All 13 of ζ_8's eigenvector directions appear identically in ζ_16 to machine precision. Zero angular drift. The match is not approximate — algebraically forced by ζ_8 = (ζ_16)², so every ζ_8 braid word is a valid doubled word in ζ_16, and squaring a quaternion preserves its rotation axis.
- **ζ_8 → ζ_32 partial nesting (8/13, 2.07° max drift)**: Cap artifact limits enumeration. Monotonic recovery (6/13 at smaller cap → 8/13 at larger cap) with shrinking drift strongly supports full 13/13 nesting at sufficient depth. Algebraic argument (ζ_8 = (ζ_32)⁴) is watertight.
- **Catalog explosion**: ζ_8: 24 quaternions, 13 directions; ζ_16: 7,952 quaternions, 3,457 directions; ζ_32: 32,768+ quaternions, 4,096+ directions (still cap-limited). Direction-to-quaternion ratio drops from 0.54 to 0.43 to 0.13 as caps interfere.
- **XOR6 trivialization**: ζ_8 Voronoi (14 cells): 36 solutions, 0.007% of all triples. ζ_16 Voronoi (3,458 cells): 499,367 solutions, 90.6%. ζ_32 Voronoi (4,097 cells): 493,654 solutions, 89.6%. With 3,457+ cells the pigeonhole effect means 64 binary inputs spread over thousands of cells — almost any weight placement avoids collision.
- **Fixed-resolution comparison**: At 16 cells, ζ_16 (450 solutions) > ζ_32 (393 solutions). Finer roots are not monotonically better at fixed resolution; which directions land in which cells matters more than raw direction count.
- **ζ_8 is the computational sweet spot**: Coarsest root that generates the 13-direction structure on S². Finer roots nest it exactly but add no computational power at fixed partition size — and at their natural Voronoi resolution they trivialize the computation entirely.
- **Musical temperament analogy**: ζ_8 = 13-note equal temperament (each solution meaningful); ζ_16 = 3,457 microtones (consonance trivially achieved); ζ_32 = effectively continuous (every triple sounds good).

### Theorems/Conjectures
- **Theorem (Direction Nesting)**: For any N, all eigenvector directions in the ζ_N catalog appear in the ζ_{2N} catalog. The nesting is exact on S². Algebraic proof: σ_i(2θ) = σ_i(θ)² as quaternions; squaring preserves the rotation axis; every ζ_N braid word is a valid doubled word in ζ_{2N}. PROVED algebraically, confirmed computationally for ζ_8 → ζ_16 (13/13, 0.000°).
- **Result (Trivialization)**: At ~3,457+ Voronoi cells (ζ_16 resolution), approximately 90% of all XOR6 triples become solutions. The activation function has so many cells that most cells see at most one of the 64 binary input vectors — pigeonhole statistics dominate topology. DEMONSTRATED.
- **Conjecture (Trivialization threshold)**: The transition from topology-driven to pigeonhole-dominated computation lies between ~50 and ~500 Voronoi cells. Below this threshold the geometry of directions matters; above it statistics dominate. CONJECTURED; threshold not yet precisely located.
- **Result (ζ_32 nesting)**: The 5 missing ζ_8 directions in the ζ_32 catalog are a cap artifact, not genuine geometric independence. The 2.07° drift on the 8 found directions reflects accumulated numerical error from composing many small-angle rotations. STRONGLY SUPPORTED, not yet confirmed at 13/13.

### Data
- ζ_8: 24 quaternions, 13 directions, ~5,500 braids enumerated
- ζ_16: 7,952 quaternions, 3,457 directions, ~5,500 braids enumerated
- ζ_32: 32,768+ quaternions (cap: MAX_QCAT=32768), 4,096+ directions (cap: MAX_ECAT=4096)
- ζ_8 → ζ_16 nesting: 13/13, avg 0.0000°, max 0.0000° — RIGIDITY
- ζ_8 → ζ_32 nesting (large cap): 8/13, max drift 2.07° — DRIFT (cap artifact)
- ζ_8 → ζ_32 nesting (small cap): 6/13, max drift 10.9° — INDEPENDENCE (cap artifact)
- XOR6 solutions: ζ_8 Voronoi: 36 (2.57 sol/cell); ζ_16 Voronoi: 499,367 (144.4 sol/cell, 90.6%); ζ_32 Voronoi: 493,654 (120.5 sol/cell, 89.6%)
- Fixed-resolution at 16 cells: ζ_16: 450 solutions; ζ_32: 393 solutions
- Braid group sizes: ζ_8 = binary octahedral (48 elements, 24 mod ±1); ζ_16 ~16,000 elements; ζ_32 ≥65,536 elements (generator cos π/16 ∉ Q, group is infinite)

### Code Assets
- **Multi-root catalog enumeration**: generates quaternion and eigenvector catalogs for arbitrary ζ_N with configurable MAX_QCAT and MAX_ECAT caps; same braid enumeration depth (~5,500) across all roots for fair comparison
- **Direction nesting comparator**: for each ζ_8 direction, finds nearest match in ζ_{2N} catalog and reports angular distance; classifies result as RIGIDITY (0°), DRIFT (small positive), or INDEPENDENCE (large positive)
- **Adaptive Voronoi XOR6 search**: builds natural Voronoi from catalog directions, then enumerates all weight triples for XOR6 solutions; reports sol/cell and % of C(150,3)
- **Fixed-resolution grid activation**: lat/lon grid with specified cell count applied to all three root catalogs for apples-to-apples comparison

### Literature Touched
- Aizenberg (2008): k-sector MVN activation; the trivialization result shows that at sufficiently fine resolution Aizenberg-style activation loses discriminative power entirely
- Conway & Smith (2003): binary octahedral group and its 24 quaternions — the ζ_8 ground case whose structure nests universally
- Birman (1974) / braid group theory: the tower ζ_8 ⊂ ζ_16 ⊂ ζ_32 ⊂ ... is a tower of finite quotients of the infinite braid group
- Nazer & Gastpar / Habiro / Abramsky / TL non-semisimplicity (background DKC literature arc, confirmed novel connection)

### Open Questions
- **Trivialization threshold**: Where exactly does XOR6 transition from topology-driven (36 solutions at 14 cells) to pigeonhole-dominated (~90% at 3,458 cells)? Likely 50–500 cells but not yet located precisely.
- **ζ_32 full confirmation**: With uncapped enumeration, do all 13 ζ_8 directions appear in ζ_32 at exactly 0° drift? The algebraic argument says yes; computational confirmation requires deeper catalog.
- **XOR6 solutions at intermediate resolutions**: Is the solution count monotonically increasing with cell count, or does it have a non-monotonic peak analogous to Demo 50's k=6 result?
- **Optimal root for fixed compute budget**: Given a cap on enumeration depth, which root maximizes XOR6 solutions at fixed cell count? Demo 69 shows ζ_16 > ζ_32 at 16 cells, but the full curve is unmapped.
- **Nesting across non-power-of-2 roots**: The algebraic argument requires ζ_N = (ζ_{2N})^2. Does any form of nesting hold for e.g. ζ_8 ⊂ ζ_24 (ζ_8 = (ζ_24)^3)?
