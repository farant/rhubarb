# Demo 63: Angular Anatomy of DKC — Findings

## Summary

Demo 63 establishes that the four-tier NPN reachability structure of DKC (Demo 50) is determined by discrete octant-sector geometry, derives the complete parity scaling law for Z[zeta_8], and connects the angular structure to oriented matroid theory. 29/29 tests pass.

## Core Results

### 1. Axiality and the Discrete Angular Alphabet

All 100 Kauffman bracket catalog values at delta=0 (A = -zeta_8) are **axial**: each value is an integer multiple of exactly one basis vector {1, zeta_8, i, zeta_8^3}. This creates an 8-symbol angular alphabet -- every bracket value points in one of 8 discrete directions (octants 0-7 at 0, 45, 90, ..., 315 degrees).

The octant distribution is symmetric: {17, 8, 17, 11, 11, 11, 17, 8} for octants 0-7.

**Why axiality matters**: Individual weights carry *pure angular information*. The computational complexity of DKC lies not in the directions of individual weights (which are discrete and simple) but in the directions of their *sums* (which sweep the full plane).

### 2. Sector-Octant Mapping: The Rosetta Stone

For k-sector MVN activation (mod 2), the class-1 octant sets are:

| k | class-1 octants | count |
|---|----------------|-------|
| 2 | {4,5,6,7} | 4 |
| 3 | {3,4,5} | 3 |
| 4 | {2,3,6,7} | 4 |
| 5 | {2,3,5,6} | 4 |
| 6 | {2,4,5,7} | 4 |
| 7 | {2,4,6} | 3 |
| 8 | {1,3,5,7} | 4 |
| 15 | {1,2,3,4,5,6,7} | 7 |

k=6 is the UNIQUE k in {2,...,16} whose class-1 set equals {2,4,5,7} -- the parity octants from Demo 50. This uniqueness is the geometric reason parity requires exactly k=6 for 3 inputs.

### 3. Four-Tier NPN Structure

The four-tier NPN reachability (5 + 1 + 6 + 1 = 13 classes) is reproduced entirely from octant-sector geometry:

- **Tier 1 (k=2, 5 classes)**: Single hyperplane (real axis). Output = sign(Im(S)), a linear function of weights. Achieves exactly the linearly separable functions.
- **Tier 2 (k=3, +1 class)**: 120-degree boundaries create first non-linear discrimination.
- **Tier 3 (k=4, +6 classes)**: 90-degree boundaries match Z[zeta_8] quadrant structure.
- **Tier 4 (k=6, +1 class)**: 60-degree boundaries create the unique {2,4,5,7} grouping needed for parity.

This connects to Cover's theorem (1965): the 5 Tier-1 NPN classes correspond to the 104 linearly separable functions of 3 binary variables, restricted to the Z[zeta_8] lattice.

### 4. Triskelion Mechanism

All 906 parity (XNOR3) solutions at k=6 are triskelions: one weight in each of the three k=6 odd sectors. Only two octant triples occur: {2,5,7} (636 solutions, 70.2%) and {2,4,7} (270 solutions, 29.8%). Octants 2 and 7 are mandatory in every solution.

The mechanism: single-input sums (individual weights) land in odd sectors -> class 1. Pair sums (two weights from different interleaved rays) swing to even sectors -> class 0. Triple sum swings back to odd -> class 1. This alternation IS parity.

### 5. Oriented Matroid Classification

Each weight triple has an oriented matroid (OM) type determined by the signs of the three pairwise cross products (s12, s13, s23). There are 21 distinct OM types over the catalog.

**Key finding**: Parity requires the **alternating** OM type (-,+,-) or (+,-,+), where s12 = s23 = -s13. These are the only types achieving 13/13 NPN classes at k=6. The uniform types (+,+,+) and (-,-,-) reach only 12/13 -- everything except parity.

The alternating type means the three generators *interleave* around the circle rather than cycling uniformly. This interleaving is the geometric condition that ensures pair sums swing to the opposite parity from single and triple sums.

**Connection to matroid theory**: The alternating condition s12*s13*s23 < 0 is equivalent to the three generators not lying in any open half-circle -- the classical condition for a rank-2 oriented matroid to be in "general position" with a specific chirality.

### 6. Complete Parity Scaling Law for Z[zeta_8]

| n inputs | minimum k | solutions at min k | solutions at k=15 | mechanism |
|----------|-----------|-------------------|-------------------|-----------|
| 3 | 6 | 906 | 23,004 | triskelion |
| 4 | 8 | 96 | 16,108 | tetraskelion |
| 5 | 15 | 3,020 | 3,020 | pentaskelion |
| 6 | -- | 0 | 0 | **impossible** |
| 7 | -- | 0 | 0 | **impossible** |
| 8+ | -- | 0 | 0 | **pigeonhole** |

**The parity ceiling for Z[zeta_8] is n=5.**

- **n=3 -> k=6**: Triskelion. 3 weights in 3 of 4 class-1 octants.
- **n=4 -> k=8**: Tetraskelion. 4 weights in all 4 odd octants {1,3,5,7}.
- **n=5 -> k=15**: Pentaskelion. 5 weights in 5 of 7 class-1 octants.
- **n>=6 -> impossible**: Constraint-geometric wall. Despite 7 class-1 octants at k=15, the C(6,2)=15 pair constraints + C(6,3)=20 triple constraints + ... = 63 total constraints on 6 integer magnitudes admit no solution.
- **n>=8 -> pigeonhole**: Only 7 class-1 octants exist (octant 0 is always class 0). Two weights on the same ray produce a class-1 pair sum, violating parity.

### 7. The Universal k: k = 2M - 1

k=15 = 2*8 - 1 is special because it makes ALL non-zero octants class 1. This generalizes:

**Theorem**: For a lattice with M equally-spaced ray directions, k = 2M - 1 places every non-zero ray in an odd sector.

*Proof sketch*: At k = 2M-1, sector width = 360/(2M-1). Ray j (at angle 360*j/M) falls in sector floor(j(2M-1)/M) = floor(2j - j/M) = 2j - 1 for j = 1, ..., M-1. Since 2j-1 is always odd, every non-zero ray is class 1. Ray 0 is in sector 0 (even). QED

For Z[zeta_8] with M=8 directions: k = 15. For Z[zeta_16] with M=16 directions: k = 31. The universal k grows linearly with the ray count.

### 8. Two Types of Impossibility

The n>=6 impossibility and n>=8 impossibility have fundamentally different characters:

**Pigeonhole (n>=8)**: Combinatorial. Only 7 class-1 octants exist, and parity requires distinct-ray weights. This is trivially provable without computation.

**Constraint wall (n=6,7)**: Geometric. Enough octants are available (7 >= 6), but the system of 63 sector-parity constraints on 6 integer magnitudes is unsatisfiable. This was established by exhaustive search over all magnitude configurations and cannot (as far as we know) be proved by a simple counting argument. It reflects the specific angular geometry of Z[zeta_8].

The gap between the pigeonhole bound (n <= 7) and the actual bound (n <= 5) is the *geometric surplus* -- two additional inputs' worth of constraint structure that the lattice cannot accommodate despite having room combinatorially.

### 9. RT Truncation Connection

The entire angular anatomy rests on delta=0, which has a representation-theoretic meaning:

- delta = 2cos(pi/ell) = 0 gives ell = 2
- At ell = 2, the Reshetikhin-Turaev truncation kills ALL j >= 1 (only the trivial representation survives)
- The Markov trace (topological closure) becomes almost trivial
- But the pre-closure bracket values retain their full algebraic structure

**DKC works because it uses pre-closure information that RT truncation would discard.** The axiality property (single-ray bracket values) is a CONSEQUENCE of delta=0 (closed loops vanish, drastically simplifying the state sum). The "most degenerate" topological point gives the "most structured" algebraic point.

This connects to the proof trilogy (Demos 56-62):
1. **Radical dimension formula**: The fixpt trace kernel = Jacobson radical (algebraic information)
2. **Markov RT truncation**: The Markov trace kernel = radical + negligible ideal (topology discards more)
3. **DKC angular anatomy** (this demo): DKC uses the fixpt information, avoids the Markov loss, and the axiality of delta=0 makes this maximally powerful

## Open Questions

1. **Why exactly n=6?** Is there a clean algebraic proof that 6 axial Z[zeta_8] weights on 6 distinct rays cannot satisfy all 63 subset-sum parity constraints at k=15? Or is this inherently a finite-verification result?

2. **Scaling to Z[zeta_N]**: For Z[zeta_16] (16 directions, phi(16)=8), the pigeonhole ceiling rises to n=15. What is the actual constraint-geometric ceiling? Is the gap always 2 (ceiling = pigeonhole - 2)?

3. **Non-monotonic solution counts**: At k=15, solutions decrease 23004 -> 16108 -> 3020 -> 0 as n increases from 3 to 6. Is this monotonic decrease a theorem, or could there exist an n where solutions increase?

4. **Oriented matroid generalization**: The alternating OM type characterizes 3-input parity. What OM type characterizes n-input parity for n=4,5? Is there a pattern (e.g., maximally alternating chirotope)?

5. **Non-parity ceiling**: Parity is the hardest boolean function (last to become reachable). What is the ceiling for other hard functions? Are there NPN classes achievable at n=6 that are not parity?

## Connections to Literature

- **Cover (1965)**: Our k=2 tier = Cover's linearly separable functions (104 of 256 for n=3 inputs). The k-sector mechanism generalizes Cover's single-hyperplane to multi-hyperplane classification.
- **Reiner (2004)**: Cyclotomic matroids mu_N study the matroid structure of roots of unity. Our Z[zeta_8] lattice is mu_8. Reiner computes Tutte polynomials but doesn't address boolean function classification.
- **Bjorner et al**: Oriented matroid covector lattices name our sector-classification construction, but the specific cyclotomic + boolean-cube + sector-activation setup is novel.
- **Novelty confirmed**: No existing work connects cyclotomic matroids, sector activation, and boolean function classification. The non-monotonicity of solution counts (k=6->k=7->k=8 for 3-input; n=3->n=4->n=5 for parity) is invisible to abstract matroid theory.
