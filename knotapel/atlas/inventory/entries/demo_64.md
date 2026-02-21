## Demo 64: Parity Matroid Recursion

- **Status**: COMPLETE
- **File**: `demo_64_parity_matroid_recursion/main.c` (~1745 lines)
- **Tests**: 22/22 pass
- **Depends on**: Demo 50 (four-tier NPN hierarchy, parity at k=6, 906 solutions), Demo 63 (angular anatomy, 5-input parity at k=15, parity ceiling n=5)
- **Feeds into**: Demo 65 (Clifford staircase — Z[zeta_16] scaling, XOR6 at higher cyclotomic ring)

### Headline
The DKC parity hierarchy {XOR2, XOR3, XOR4, XOR5} is a funnel-shaped matroid minor chain with perfect downward deletion-contraction closure and collapsing upward extensibility, whose wall at n=6 is geometrically explained by the self-doubling impossibility in Z[zeta_8].

### Key Results
- **Matroid deletion-contraction closure (100% downward)**: Deleting any weight from an XOR_n solution always yields an XOR_{n-1} solution (100%, all levels n=5→4→3→2). Contracting any input to 1 always yields an XNOR_{n-1} solution (100%, all levels). The parity hierarchy is a legitimate matroid minor chain.
- **Funnel theorem (coverage collapses upward)**: Upward extensibility degrades sharply — XOR3→XOR2: 99.3% (1459/1469), XOR4→XOR3: 39.2% (1503/3834), XOR5→XOR4: 19.2% (129/673), XOR6→XOR5: 0% (0/27). The n=6 wall casts a shadow all the way down: 80% of XOR4 weight sets are already dead ends.
- **Parent-child multiplicity (near-tree at wall, dense mesh away)**: XOR5→XOR4: max 2 parents, mean 1.05 (near-tree). XOR4→XOR3: max 8 parents, mean 1.79. XOR3→XOR2: max 29 parents, mean 7.88. The 6 collisions in the XOR5→XOR4 deletion map account exactly for 27×5=135 instances → 129 distinct children + 6 collisions.
- **Vocabulary stratification (44 good values, 31 poison)**: Of 83 class-1 catalog values at k=15, exactly 44 appear in XOR5 solutions and exactly the same 44 appear in extensible XOR4 solutions — strictly nested. 31 values are poison (appear only in orphan XOR4 solutions that cannot extend to XOR5). Poison values span octants {1,2,3,5,6,7} but not octant 4.
- **Wall anatomy (minimum Hamming distance 4, errors only at weight 2+)**: 2,241 extension attempts (27 XOR5 sets × 83 values). Closest failures land at d=4 (38 cases). Zero errors at input weight 0 or weight 1 — the wall manifests only at the pairwise interaction level and above. A second wall layer exists: d≥7 cases with 0 broken pairs but weight-3+ errors (GHZ-type higher-order obstruction).
- **Self-doubling impossibility theorem**: Self-extensions (w[5]=w[i]) always fail because 2·w[i] preserves the angle of w[i]; class-1 values (odd sector) doubled remain class-1 (odd sector), making the pair output for weight-2 input class-1 when it must be class-0. Accounts for 34/38 = 89.5% of all closest misses.
- **Exact arithmetic required**: Floating-point accumulation produces 420 spurious failures at XOR5→XNOR4 from association-order artifacts near sector boundaries. Eliminated entirely by exact Z[zeta_8] integer arithmetic (Cyc8 representation).

### Theorems/Conjectures
- **Theorem (matroid minor chain)**: {XOR2, XOR3, XOR4, XOR5} is deletion-contraction closed: deletion gives XOR_{n-1}, contraction gives XNOR_{n-1}. Verified exhaustively, all levels. PROVEN.
- **Theorem (self-doubling impossibility)**: Adding a 6th weight equal to any existing weight always fails. Proof: scalar doubling preserves angle; class-1 doubled is class-1; class-1 output contradicts required class-0 for even-parity input weight. PROVEN.
- **Theorem (minimum wall distance d=4)**: The closest any 6-weight candidate comes to satisfying XOR6 is 4 output errors (60/64 = 93.75% correct). No candidate achieves d=0,1,2,3. PROVEN (exhaustive).
- **Conjecture (funnel rate)**: The coverage fractions 99.3% → 39.2% → 19.2% → 0% follow some algebraic or exponential decay law as a function of n. OPEN.
- **Conjecture (Clifford staircase)**: XOR6 becomes achievable at Z[zeta_16] (next cyclotomic ring, fourth-level Clifford hierarchy), suggesting n-input parity climbs the Clifford hierarchy one rung per additional input. OPEN (target of Demo 65).

### Data
- Distinct unordered weight sets: XOR5=27, XOR4=673, XOR3=3834, XOR2=1469
- XOR5 ordered solutions: 3,020 (≈112 per set, close to 5!=120 confirming near-complete permutation symmetry)
- Upward coverage: XOR3→XOR2: 99.3%, XOR4→XOR3: 39.2%, XOR5→XOR4: 19.2%, XOR6→XOR5: 0%
- Orphan counts: 10 (XOR2), 2331 (XOR3), 544 (XOR4), 27 (XOR5)
- Extensible vocabulary: 44 values (good); poison vocabulary: 31 values; total class-1 at k=15: 83 - note: 75 total appear in all XOR4
- Poison octant coverage: {1,2,3,5,6,7} — octant 4 absent
- Wall Hamming distances: minimum d=4 (38 cases), 195 near-misses at d≤8, peak near d=15-16
- Error weight distribution (near-misses): weight-0: 0, weight-1: 0, weight-2: 237, weight-3: 370, weight-4: 446 (peak), weight-5: 172, weight-6: 23
- Self-doubling accounts for 34/38 = 89.5% of closest misses
- Second wall layer: d=7 (2 cases, 0 broken pairs), d=8 (16 cases, 0 broken pairs) — higher-order interaction failure

### Code Assets
- **Exact Cyc8 arithmetic**: Z[zeta_8] integer representation eliminating floating-point association-order failures near sector boundaries; critical for correct deletion-contraction verification
- **Deletion map with collision tracking**: exhaustive weight-set subtraction at each level, counting distinct children vs. total instances to identify collisions
- **Upward extension search**: for each XOR_n weight set, tests all 83 class-1 catalog values as candidate 6th weight, records Hamming distance to XOR6 truth table
- **Vocabulary stratification analysis**: separates catalog values into good (extensible), poison (orphan-only), and absent categories with octant annotations
- **Error-weight decomposition**: for each failed extension, bins output errors by input Hamming weight to isolate interaction-level vs. single-weight failures
- **Self-doubling classifier**: detects w[5]=w[i] cases and computes angle-preservation argument symbolically

### Literature Touched
- Aizenberg (2008): k-sector MVN activation — direct ancestor of sector/class framework used throughout
- Kauffman bracket / TL category theory: catalog values as quantum amplitudes (Z[zeta_8] = T-gate algebraic ring)
- Reiner (2004, arXiv:math/0402206): cyclotomic matroids mu_N — candidate framework for 44 good / 31 poison value split
- CKW inequality (Coffman-Kundu-Wootters): entanglement monogamy parallel — 10 pairwise sums simultaneously compatible, 15 cannot be
- Kochen-Specker theorem: contextuality parallel — wall as consistent-assignment obstruction for all C(6,2)=15 pair sums
- Clifford hierarchy (Gottesman-Chuang 1999): self-doubling maps to T→S gate descent; XOR6 ceiling interpreted as hierarchy level boundary

### Open Questions
- Does XOR6 become achievable at Z[zeta_16]? If so, the parity function literally climbs the Clifford hierarchy one rung per additional input (Demo 65 target).
- Can the triple-interaction constraints (weight-3+ errors at d≥7) be characterized explicitly, analogous to the self-doubling theorem for weight-2?
- Are the 27 XOR5 weight sets built primarily from antipodal pairs (w + (-w) = 0 always safe)? How much of the vocabulary structure depends on antipodal geometry?
- Is there a closed-form formula for upward coverage fraction as a function of n (exponential decay, algebraic, other)?
- What is the geometric explanation for the poison values avoiding octant 4 entirely?
- Are the 44 good values and 31 poison values related to the matroid structure of mu_8 (Reiner's cyclotomic matroid at 8th roots)?
