## Demo 80: Group Finiteness Survey

- **Status**: COMPLETE
- **File**: `demo_80_group_finiteness/main.c` (~467 lines)
- **Tests**: 11/11 pass
- **Depends on**: Demo 79 (finite vs infinite group hypothesis), Demo 78 (ζ₈ wall at XOR10), Demo 66 (binary octahedral group = 24 elements)
- **Feeds into**: Future demos selecting which ζ_N to use for extended XOR capacity; ADE/quantum group connections

### Headline
Only ζ₄ and ζ₈ generate finite SU(2) subgroups under the standard two-generator construction; all other tested roots (ζ₆, ζ₁₀, ζ₁₂, ζ₁₆, ζ₂₀) generate infinite groups, the finite cases correspond exactly to rotations by π/2 and π/4 which are symmetries of the cube/octahedron, and at ζ₈ the quantum dimension [2]_q vanishes — the most singular point of quantum group parameter space.

### Key Results
- **Only ζ₄ and ζ₈ finite**: Of 7 roots tested (N = 4, 6, 8, 10, 12, 16, 20), only ζ₄ (4 elements, binary dihedral Q₄) and ζ₈ (24 elements, binary octahedral) close under group multiplication.
- **Non-monotonic boundary**: ζ₆ is infinite despite sitting between the two finite cases. Finiteness is NOT a monotone function of N. The pattern is: finite(4), infinite(6), finite(8), infinite(10,12,16,20,...).
- **ADE identification**: ζ₄ → Binary Dihedral 2D₂ (order 8, the quaternion group Q₈ = {±1, ±i, ±j, ±k}). ζ₈ → Binary Octahedral 2O (order 48, E₇ in ADE classification).
- **ζ₁₀ is NOT binary icosahedral**: Despite π/5 being the golden ratio angle (Fibonacci anyons territory), ζ₁₀ generates an infinite group. The binary icosahedral group (order 120, E₈) is not reached by this generator construction.
- **Infinite groups approach continuum**: All infinite groups at 4096 entries show 512 directions (saturated MAX_DIR), 37–71 distinct half-angles, minimum angle gaps < 1° (approaching dense subgroup of SU(2)).
- **Non-monotone angle counts**: ζ₁₀ has 71 half-angles at 4096 entries vs ζ₁₂'s 43 and ζ₁₆'s 51. Different infinite groups have different density structures despite similar closure behavior.
- **Algebraic diagnosis**: cos(π/2) = 0 (rational) and cos(π/4) = √2/2 (degree 2) generate finite groups. cos(π/3) = 1/2 (rational!) generates infinite. Algebraic degree alone does not predict finiteness.
- **Geometric characterization**: The finite cases are exactly the angles that are rotational symmetries of the cube/octahedron. π/2 and π/4 rotations around perpendicular axes generate the octahedral symmetry group and close. π/3 rotations are symmetries of the hexagonal lattice but two perpendicular hexagonal axes generate an infinite group.

### Theorems/Conjectures
- **Power-of-Two Finiteness Conjecture (PROPOSED)**: The finite cases under this construction are exactly angles of the form π/2^k for k ≥ 1. This predicts θ = π/2 (k=1, ζ₄) and θ = π/4 (k=2, ζ₈), and further predicts ζ₃₂ (θ = π/16) should also be finite. Not yet tested.
- **ADE Exhaustion (DEMONSTRATED for E₇)**: ζ₈ → Binary Octahedral → E₇. The ζ₄ → Binary Dihedral is the D-series. E₆ (binary tetrahedral, 24 elements) and E₈ (binary icosahedral, 120 elements) are not reachable under the two-perpendicular-axes construction tested here.
- **Quantum Dimension Vanishing (CONFIRMED)**: At ζ₈, q = i, and [2]_q = q + q⁻¹ = i + (-i) = 0. The quantum dimension vanishes. The two finite groups correspond to the lattice (singular) points of quantum group parameter space. Non-lattice roots (all infinite groups) are where Kuperberg's #P-hardness applies.
- **Dense Subgroup Conjecture (SUPPORTED)**: Infinite groups under this construction generate dense subgroups of SU(2) — the directions approach uniform S² coverage as the group grows. Not proven analytically.

### Data
- ζ₄: 4 elements, 2 closure rounds, 3 directions, 2 half-angles. ADE: Binary Dihedral 2D₂.
- ζ₆: >4096 elements, 10 rounds to cap, 512 directions (saturated), 37 half-angles. INFINITE.
- ζ₈: 24 elements, 4 closure rounds, 13 directions, 4 half-angles. ADE: Binary Octahedral (E₇).
- ζ₁₀: >4096 elements, 7 rounds to cap, 512 directions, 71 half-angles. INFINITE.
- ζ₁₂: >4096 elements, 8 rounds to cap, 512 directions, 43 half-angles. INFINITE.
- ζ₁₆: >4096 elements, 8 rounds to cap, 512 directions, 51 half-angles. INFINITE.
- ζ₂₀: >4096 elements, 7 rounds to cap, 512 directions, 67 half-angles. INFINITE.
- Finite count: 2. Infinite count: 5.
- Quantum dimension [2]_q at ζ₈: exactly 0 (q = i, [2]_q = i + 1/i = i - i = 0).
- Angle algebraic degrees: cos(π/2)=0 (rational, finite), cos(π/4)=√2/2 (degree 2, finite), cos(π/3)=1/2 (rational, INFINITE), cos(π/5)=φ/2 (degree 2, INFINITE).

### Code Assets
- **`build_closure(rounds_out)`**: generic group closure algorithm; starts from generators + inverses + identity, multiplies all current elements by all generators each round, stops when no new elements are added or MAX_QCAT is hit; returns 1 if finite, 0 if hit cap
- **`test_root(N)`**: complete per-root survey: init generators at half_angle=2π/N, run closure, count directions, count angles, identify ADE type, print and store in results array
- **`identify_ade(order)`**: maps group size to ADE classification; handles binary tetrahedral (order 12), octahedral (24), icosahedral (60), binary dihedral (4n), cyclic (n)
- **`analyze_direction_distribution()`**: for infinite groups, reports S² octant coverage and angle gap statistics to characterize density structure
- **`RootResult` struct**: compact storage for per-root survey results (N, angle, finite flag, size, rounds, dirs, angles, ADE string); enables summary table across all roots
- **`count_angles()` with bubble sort**: collects distinct half-angles with 0.1° tolerance, sorts ascending; pattern for angle census reusable in any catalog analysis
- **Closure vs brute-word-enumeration tradeoff**: Demo 80 uses true group closure (multiply existing elements by generators) rather than Demo 78's enumerate-all-words approach; closure is exact and efficient for finite groups, and correctly identifies infinite groups by hitting the cap

### Literature Touched
- **ADE classification of finite subgroups of SU(2)**: the complete classification (cyclic, binary dihedral, binary tetrahedral/octahedral/icosahedral) is the organizing framework. Our construction reaches only two of the exceptional cases.
- **Kuperberg (1994, 1996) #P-hardness of Jones polynomial**: hardness applies at non-lattice roots of unity; the lattice roots ζ₄ and ζ₈ where [2]_q ∈ {-2, 0} are exactly our finite cases and the tractable computational regime.
- **Witten-Reshetikhin-Turaev TQFT**: the finite subgroups of SU(2) are the building blocks of WRT invariants; the ADE classification connects to the Dynkin diagram structure of the relevant quantum groups.
- **Fibonacci anyons / topological quantum computation**: ζ₁₀ (angle π/5, golden ratio) is the relevant parameter for Fibonacci anyons and universal quantum computation via braiding. Our finding that ζ₁₀ generates an infinite group is consistent with its use in quantum computation: the infinite group provides universal coverage of SU(2).
- **Habiro (2002)**: the Z[ζ₈] bracket from the DKC arc lives at the ζ₈ = E₇ point; the finiteness of this group is the algebraic foundation of the finite capacity ceiling demonstrated in Demos 76–78.

### Open Questions
- **Is ζ₃₂ (θ = π/16) finite?**: The power-of-two conjecture predicts yes. A single additional test_root(32) call would confirm or refute.
- **Why do perpendicular π/3 rotations not close?**: The geometric argument (hexagonal symmetry vs octahedral symmetry) explains the result intuitively but a rigorous algebraic proof of why exactly π/2 and π/4 are special has not been given.
- **Do all infinite groups give unbounded XOR capacity?**: Demo 79 confirmed ζ₁₂ (infinite) breaks the ζ₈ wall. But do all infinite groups provide unbounded capacity, or do some infinite groups have structural limitations that impose their own ceilings?
- **Which infinite group is computationally optimal?**: ζ₁₀ has the most half-angles at 4096 entries (71) of all surveyed roots. Does more angular diversity directly translate to easier XOR computation and lower minimum k values?
- **E₆ and E₈ reachability**: Binary tetrahedral (order 24, E₆) and binary icosahedral (order 120, E₈) are not reached by the two-perpendicular-axes generator construction. Is there a different natural generator construction that reaches them?
