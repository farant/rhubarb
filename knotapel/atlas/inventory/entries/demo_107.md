## Demo 107: Algebraic Graph Density -- Cyc8 Relationship Analysis

- **Status**: COMPLETE (RESULT)
- **File**: `demo_107_algebraic_graph/main.c` (~2075 lines)
- **Tests**: 42/42 pass
- **Depends on**: Demo 35 (planar matching types, {0,1}-valued TL generators at delta=0), Demo 99 (W_{3,1} module), Demo 100 (W_{4,2} module, 3x3 braid representation), Demo 101 (W_{5,3} module), Demo 102 (W_{6,0} and W_{6,4} modules), Demo 103 (W_{6,2} dimension scaling, 9x9 generator matrices), Demo 106 (relational computation thesis connecting to DKC capacity)
- **Feeds into**: Demo 108 (Raqiya toolkit predicts parity capability from graph structure), Demo 109 (D55 graph analysis reuses Raqiya)

### Headline
Universal axis-alignment of braid matrices at delta=0 is a provable theorem (not just empirical), with a constructive Z/4Z phase formula based on nesting parity, plus five novel results including the major index bipartite invariant and a q-Catalan identity.

### Key Results

- **Universal axis-alignment (Claim #2)**: ALL 186 distinct Z[zeta_8] values from the 32768-entry W_{4,2} BFS catalog have exactly one nonzero component. Every matrix entry is of the form n*zeta_8^k where n is an integer and k in {0,1,2,3}. The representation NEVER produces mixed-axis values like (1,1,0,0).
- **Fixed phase mask FALSIFIED (Claim #3)**: Every matrix position (r,c) has ALL FOUR zeta-powers appearing across the catalog (0/9 positions fixed). Phase depends on the braid word, not the matrix position.
- **Z/4Z Axis-Alignment Theorem (Claim #7, PROVEN)**: Axis-alignment is provably guaranteed for ALL TL representations at delta=0. Proof chain: (1) TL generators have {0,1} entries, (2) braid generators give entries from {0, A, A^{-1}}, (3) axis-aligned * axis-aligned = axis-aligned, (4) same-axis addition preserves axis, (5) bipartiteness ensures only same-axis values are added at each (r,c).
- **Bipartiteness verified (Claim #10)**: Link-state transition graphs are bipartite for TL_n left regular (n=2,3,4,5,6, all connected) and 6 standard modules W_{3,1}, W_{4,2}, W_{5,3}, W_{6,0}, W_{6,2}, W_{6,4}.
- **Nesting Parity = Z/2Z Grading (Claim #11, NOVEL THEOREM)**: The bipartite 2-coloring of TL link-state graphs IS nesting count mod 2 (a nesting = pair of arcs (a,b),(c,d) with a < c < d < b). Verified for ALL n=2,3,4,5,6 (195 matchings, 0 mismatches). Confirmed by researcher as NOT a consequence of known KLR Z-grading (Plaza-Ryom-Hansen 2013).
- **Constructive Z/4Z phase formula (Claim #11)**: Entry(r,c) = (path count) * zeta_8^{(w - 2*(nest(r) + nest(c))) mod 4}, where w = writhe, nest(s) = nesting number of link state s mod 2. Three numbers fully determine the cyclotomic phase.
- **Major Index Bipartite Invariant (Claim #16, NOVEL)**: For TL standard modules W_{n,j}, the bipartite invariant is maj mod 2 = sum of right-endpoint positions of arcs, mod 2. Verified: W_{4,2} (3/3), W_{6,2} (9/9, the ONLY winning candidate out of 4 tested). For full matchings (j=0), maj mod 2 = nest mod 2 (special case).
- **q-Catalan Identity (Claim #17, NOVEL)**: C_{2m+1}(-1) = (-1)^m * C_m. Gives exact bipartition class sizes for W_{n,0} modules. Proved algebraically, verified against all 5 data points (n=2,3,4,5,6).
- **Orthogonality Theorem (Claim #15)**: Quotient graph analysis reveals product_closure/axis = K_5 (Z/4Z multiplication table, complete 4-regular) while additive_closure/axis = star tree centered on 0 (axis-preserving). Multiplication = Z/4Z rotation across axes; addition = growth within each axis.
- **zeta_8 uniqueness (Claim #8)**: zeta_8 is the unique root of unity where delta=0 AND #axes = cyclotomic rank (both = 4). zeta_4 has delta=0 but only 2 axes; all other zeta_n with n <= 12 have delta != 0.
- **2-adic neutrality (Claim #13, NOVEL observation)**: v_2 distribution decays geometrically (95->48->24->11->7, approx halving per level). TL nilpotency (e_i^2=0) introduces factors of 2 symmetrically.
- **Writhe-parity depth mechanism (Claim #12)**: Full-catalog "even axis dominance" is entirely a depth-8 truncation artifact. At complete depths 0-7: odd axis dominates (72 vs 41). Depth 8 (truncated) is 100% even axis.
- **Galois asymmetry (Claim #18)**: sigma_5 fixes ALL 126 even-axis values; sigma_3 fixes 66; sigma_7 fixes 54. All Galois edge sets are 1-regular (perfect matchings on non-fixed points). b-axis isomorphic to d-axis across all 13 edge types.
- **DKC implication (Claim #14)**: DKC at delta=0 reduces to integer path counting + three-variable phase formula. Effective computation is Z x Z/4Z per matrix entry.

### Theorems/Conjectures

- **Z/4Z Axis-Alignment Theorem** (PROVEN): Every entry of a braid representation matrix over a TL module at delta=0 is axis-aligned (exactly one nonzero cyclotomic component).
- **Nesting Parity Z/2Z Grading** (PROVEN computationally, n=2..6): Nesting count mod 2 equals the bipartite 2-coloring for all TL link-state transition graphs. 195 matchings, 0 mismatches.
- **Major Index Bipartite Invariant** (CONJECTURED for general n,j; verified at W_{4,2} and W_{6,2}): For TL standard modules W_{n,j}, maj mod 2 = BFS bipartite coloring. Generalizes nesting parity (the j=0 case).
- **q-Catalan Identity** (PROVEN algebraically): C_{2m+1}(-1) = (-1)^m * C_m.
- **2-Adic Neutrality of Nilpotent TL** (CONJECTURED): v_2 distribution of TL matrix entries decays geometrically. Five candidate proof frameworks identified but not formalized.
- **Orthogonality Theorem** (PROVEN computationally): Multiplicative quotient graph = K_5 (Z/4Z), additive quotient graph = star tree. Multiplication rotates axes; addition preserves axes.

### Data

**Module**: W_{4,2} (4-strand, dim 3, non-semisimple), generators sigma_i = A*I + A^{-1}*e_i where A = -zeta_8 = (0,-1,0,0), delta = 0.

**BFS catalog**: 32768 entries (depth 8, hit cap). 294,912 matrix components examined.

**Distinct value growth by depth**:

| Depth | Entries | Cumul Distinct | New |
|-------|---------|----------------|-----|
| 0     | 1       | 2              | +2  |
| 1     | 6       | 4              | +2  |
| 2     | 26      | 7              | +3  |
| 3     | 98      | 13             | +6  |
| 4     | 338     | 23             | +10 |
| 5     | 1110    | 41             | +18 |
| 6     | 3444    | 68             | +27 |
| 7     | 10390   | 114            | +46 |
| 8     | 17355   | 186            | +72 |

Value set NOT closed at depth 8 (still growing, 72 new values at final depth).

**186 values decomposed by axis**: a-axis (zeta^0) range [-19,+34] (53 values), b-axis (zeta^1) range [-21,+15] (36 values), c-axis (zeta^2) range [-30,+30] (60 values), d-axis (zeta^3) range [-15,+21] (36 values), plus 1 zero.

**Axis distribution (complete depths 0-7)**: even (a+c) = 41, odd (b+d) = 72. Odd dominates at unbiased depths; even dominance in full catalog is truncation artifact.

**Orbit structure (full 186 values)**: 35 root-of-unity orbits = 35 norm classes (identical for axis-aligned values), 105 Galois orbits (finer partition). Divisibility density: 14.2%.

**2-adic valuation cumulative**: v_2=0: 95, v_2=1: 48, v_2=2: 24, v_2=3: 11, v_2>=4: 7 (geometric decay ~0.5x per level).

**Bipartiteness verification**:

| Module | Dim | Bipartite | Connected | Partition |
|--------|-----|-----------|-----------|-----------|
| TL_2   | 2   | YES       | YES       | 1+1       |
| TL_3   | 5   | YES       | YES       | 3+2       |
| TL_4   | 14  | YES       | YES       | 7+7       |
| TL_5   | 42  | YES       | YES       | 21+21     |
| TL_6   | 132 | YES       | YES       | 66+66     |
| W_{3,1}| 2   | YES       | --        | 1+1       |
| W_{4,2}| 3   | YES       | --        | 2+1       |
| W_{5,3}| 4   | YES       | --        | 2+2       |
| W_{6,0}| 5   | YES       | --        | 3+2       |
| W_{6,2}| 9   | YES       | --        | 5+4       |
| W_{6,4}| 5   | YES       | --        | 3+2       |

**W_{6,2} bipartite invariant candidates**: maj mod 2: 9/9 MATCH, nest mod 2: 7/9 FAIL, tsl mod 2: 6/9 FAIL, lae mod 2: 5/9 FAIL. Major index is the ONLY working invariant.

**Phase 8b verification**: Independent TL action from planar stacking mechanics matches D103's 9x9 generator matrices with 0/45 mismatches.

### Code Assets

- **Raqiya library** (`raqiya/raqiya.h`): Single-header stb-style C89 library for Z[zeta_8] algebraic analysis. 9 relationship detectors (root-of-unity orbits, sign distances, sum/diff zeros, norm-squared classes, product structure, Galois orbits, axis agreement, divisibility, 2-adic valuation). Graph analysis layer with 13 edge generators, quotient graph computation, restricted graph analysis. 208 unit tests in probatio_raqiya.c. Raq_ValueSet (hash-based distinct value collector), Raq_Partition, Raq_PairHist, Raq_Analysis types.
- **Mat3 over Raq_Cyc8**: 3x3 matrix type with multiply, add, scale, equality. Used for W_{4,2} representation.
- **BFS catalog with hash dedup**: FNV-style hash over 36 longs, chained-bucket lookup, MAX_CAT=32768, HASH_SIZE=65537. Depth tracking per entry.
- **Non-crossing matching enumerator**: Iterative stack-based enumeration of planar matchings for TL_n basis states, up to n=6 (C_6=132 states).
- **Nesting counter**: count_nestings() computes arc-nesting count for non-crossing matchings in boundary order.
- **Bipartiteness checker**: Generic BFS 2-coloring for adjacency matrices up to 16x16 (BP_CHECK_MAX).
- **W_{6,2} state enumerator**: Brute-force enumeration of non-crossing partial matchings with 2 through-strands and non-enclosure constraint.
- **TL planar stacking action**: Independent computation of TL generator action on W_{6,2} states from first principles (cup stacking mechanics with 4 cases).

### Literature Touched

- **Temperley-Lieb algebra at delta=0**: TL generators e_i have {0,1}-valued matrices; e_i^2 = delta*e_i = 0 (nilpotent). Basis: non-crossing matchings (Catalan numbers C_n).
- **Habiro (2002)**: Z[zeta_8] cyclotomic integers as the algebraic substrate for colored Jones polynomial brackets. A = -zeta_8, delta = -(A^2 + A^{-2}) = 0.
- **Plaza-Ryom-Hansen (2013)**: KLR Z-grading of TL algebras. Researcher confirmed that TL generators e_i are NOT homogeneous in the KLR grading, meaning the nesting parity Z/2Z grading (Claim #11) is genuinely novel and not a consequence of known graded TL theory.
- **Harary (1990)**: Integral sum graphs. The additive closure graph on 186 values (3432 edges) is an integral sum graph.
- **q-Catalan numbers / Gaussian binomial coefficients**: C_n(q) specialization at q=-1 gives bipartition sizes. Novel identity C_{2m+1}(-1) = (-1)^m * C_m.
- **Galois group Gal(Q(zeta_8)/Q) = Z/2 x Z/2**: Generated by sigma_3: zeta_8 -> zeta_8^3 and sigma_5: zeta_8 -> zeta_8^5 = -zeta_8. sigma_7 = sigma_5 o sigma_3 is complex conjugation.

### Open Questions

1. **Does the value set converge?** 186 values at depth 8 with 72 new at that depth -- still growing fast. Need deeper catalogs (beyond 32768 cap) to check.
2. **zeta_12 axis-alignment prediction** (registered before computation): At delta=sqrt(3) != 0, expect mostly axis-aligned but with occasional mixing. Amount of mixing proportional to |delta|.
3. **Does integer-level Raqiya structure predict computational capacity?** Connects D107 algebraic analysis back to D106's relational computation thesis. (Partially answered by D108: yes, dual-channel theorem.)
4. **Major index theorem for all W_{n,j}?** Verified at W_{4,2} (3/3) and W_{6,2} (9/9). Need W_{8,2}, W_{6,4}, etc. for more evidence. Proof for general n,j open.
5. **Strongly regular parameters?** Some regular subgraphs may have (n,k,lambda,mu) parameters -- not yet checked.
6. **Integer spectrum of transition graphs?** Deferred (lower priority).
7. **q-ballot evaluation**: Does q-Catalan have a clean evaluation at other roots of unity beyond q=-1?
8. **2-adic neutrality formal proof**: Five converging frameworks identified (Kummer carry-counting, nilpotent TL, Goodman-Wenzl, unipotent, Stirling) -- need to select one and formalize.
