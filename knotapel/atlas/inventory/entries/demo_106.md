## Demo 106: Topological Entropy of XOR-Computing Braids

- **Status**: COMPLETE (RESULT)
- **File**: `demo_106_topological_entropy/main.c` (~1408 lines)
- **Tests**: 20/20 pass
- **Depends on**: Demo 100 (W_{4,2} module, 3x3 TL representation, mat3/Cyc8 code, 3x3_hash activation, XOR framework), Demo 83 (writhe correlation with DKC output), Demo 93 (circuit complexity hierarchy, derived series strata), Demo 95 (cross-stratum mixing), Demo 50 (k parameter / parity reachability)
- **Feeds into**: Demo 107 (Raqiya algebraic graph density -- D106 establishes "capacity is relational," D107 measures relational structure), Demo 108 (Raqiya predicts parity -- closes the loop D106->D107->D108)

### Headline
Pre-registered prediction FALSIFIED: topological entropy (Thurston-Nielsen classification) is completely orthogonal to DKC Boolean computation -- both periodic and pseudo-Anosov braids compute XOR at 100% participation with near-identical scores, because the TL quotient at delta=0 erases all dynamical information while preserving algebraic structure.

### Key Results
- **Prediction falsified**: Both periodic (88 entries, mean score 3969) and pseudo-Anosov (39 entries, mean score 3857) braids compute XOR at 100% participation rate in the first 128 catalog entries. No separation whatsoever.
- **Total Boolean function null**: Entropy classification is orthogonal to ALL 8 Boolean functions tested (AND3, OR3, XOR3, MAJ3, NAND3, NOR3, AND2, XOR2). Maximum gap between periodic and pA rates: 3.7pp (XOR2), well within noise.
- **Fibonacci pseudo-Anosov at depth 2**: All 8 depth-2 pA entries share characteristic polynomial x^3 - 4x^2 + 4x - 1 with spectral radius phi^2 = (3+sqrt(5))/2 = 2.618..., giving entropy per crossing = log(phi) = 0.481212... (theoretical optimum per Band & Boyland 2007).
- **Pseudo-Anosov fraction increases with depth**: From 0% at depth 1 to 77.2% at depth 8, crossover at depth 5 (50.6%). Consistent with Caruso 2013 genericity theorem.
- **Entropy per crossing decreases with depth**: Mean 0.4812 at depth 2 (optimal), 0.4390 at depth 3, decreasing further. Deeper braids are less efficiently chaotic per generator.
- **TL visibility filter explains the null**: Writhe survives the TL quotient (multiplies bracket by (-A^3)^w), so writhe correlates with DKC output (D83). Entropy does NOT survive the TL quotient (e_i^2=0 kills expanding eigenvalues), so entropy is invisible to the bracket.
- **DKC computation is collective/algebraic, not individual/dynamical**: Sharpens the relational computation thesis -- the question is never "is this braid good?" but "does this collection span the right algebraic structure?"

### Theorems/Conjectures
- **TL Visibility Filter (DEMONSTRATED)**: Properties visible in the Kauffman bracket (writhe) correlate with DKC output; properties invisible in the bracket (topological entropy, Burau spectral radius) do not. The TL quotient at delta=0 is the filter.
- **Relational Computation Thesis (STRENGTHENED)**: Across the DKC program, the strongest predictors of computational capacity are always relational/algebraic (derived series strata, cross-stratum mixing, orthogonal frame geometry, k-granularity, non-semisimplicity). Individual dynamical properties (entropy, spectral radius, writhe, word length) are consistently non-predictive or invisible.
- **Caruso Genericity (CONFIRMED)**: Pseudo-Anosov braids become generic at higher word lengths, crossing from minority to majority at depth 5 (50.6%) and reaching 77.2% at depth 8.

### Data

**Contingency table (k=128, first 128 entries):**

| Class         | Total | XOR>0 | XOR=0 | Rate  | Mean Score |
|---------------|-------|-------|-------|-------|------------|
| Identity      | 1     | 1     | 0     | 100%  | 4574       |
| Periodic      | 88    | 88    | 0     | 100%  | 3969       |
| Pseudo-Anosov | 39    | 39    | 0     | 100%  | 3857       |

**Boolean function sweep (periodic rate vs pA rate, k=128):**

| Function | Periodic | pA    | Gap    |
|----------|----------|-------|--------|
| AND3     | 92.0%    | 92.9% | +0.9pp |
| OR3      | 95.1%    | 93.4% | -1.7pp |
| XOR3     | 86.9%    | 86.0% | -0.9pp |
| MAJ3     | 86.3%    | 87.5% | +1.2pp |
| NAND3    | 92.0%    | 92.9% | +0.9pp |
| NOR3     | 95.1%    | 93.4% | -1.7pp |
| AND2     | 88.4%    | 87.9% | -0.5pp |
| XOR2     | 90.8%    | 87.1% | -3.7pp |

**Pseudo-Anosov fraction by depth:**

| Depth | Total  | Periodic | pA     | pA%   |
|-------|--------|----------|--------|-------|
| 0     | 1      | 0        | 0      | --    |
| 1     | 6      | 6        | 0      | 0.0%  |
| 2     | 26     | 18       | 8      | 30.8% |
| 3     | 98     | 66       | 32     | 32.7% |
| 4     | 338    | 192      | 146    | 43.2% |
| 5     | 1110   | 548      | 562    | 50.6% |
| 6     | 3444   | 1210     | 2234   | 64.9% |
| 7     | 10390  | 2974     | 7416   | 71.4% |
| 8     | 17355  | 3952     | 13403  | 77.2% |

**BFS catalog**: 32768 entries (cap hit at depth 8). Module W_{4,2} (4-strand, dim 3, non-semisimple).

**Dual representation**: TL (3x3 over Z[zeta_8]) for bracket + reduced Burau at t=-1 (3x3 integer, SL(3,Z)) for entropy.

**Fibonacci pA characteristic polynomial**: x^3 - 4x^2 + 4x - 1, spectral radius phi^2 = 2.618034..., entropy/crossing = log(phi) = 0.481212...

**Mean entropy/crossing for pA entries (first 128)**: 0.4476

### Code Assets
- **Dual BFS with parallel representations**: Maintains both TL (3x3 Cyc8) and Burau (3x3 integer) matrices simultaneously during BFS, with shared parent/generator backtracking. Novel pattern for correlating algebraic and dynamical properties of the same braid words.
- **Reduced Burau representation at t=-1 for B_4**: 3x3 integer matrices in SL(3,Z). Generators built from the standard reduced Burau formula with t=-1 substitution.
- **Exact integer entropy classification**: Characteristic polynomial p(x) = x^3 + c2*x^2 + c1*x - 1 (det=1 always for braids). If p(1) = c2+c1 != 0, always pseudo-Anosov. If p(1) = 0, check quadratic factor |1-tr(M)| > 2. No floating point needed for the classification itself.
- **Spectral radius by bisection**: Double-precision bisection on characteristic polynomial (100 iterations) for entropy-per-crossing analysis. Classification uses exact integers; bisection used only for the quantitative entropy value.
- **Boolean function sweep with +/- encoding**: 3 weights -> 8 input patterns via signed addition. 8 truth tables tested (AND3, OR3, XOR3, MAJ3, NAND3, NOR3, AND2, XOR2). Reusable pattern for testing entropy correlation against arbitrary Boolean functions.
- **`iMat3` (3x3 integer matrix)**: Separate integer matrix type for Burau representation. Operations: `imat3_zero`, `imat3_identity`, `imat3_mul`, `imat3_eq`, `imat3_det`, `imat3_char_poly`. Parallel to Cyc8-based Mat3 but pure integer for exact determinant/polynomial computation.

### Literature Touched
- **Band & Boyland 2007** -- "The Burau estimate for the entropy of a braid." Reduced Burau at t=-1 gives exact topological entropy for 3-braids and optimal lower bound for 4+ strands. Establishes log(phi) = 0.481212... as the optimal mixing efficiency per crossing.
- **Fadell & Neuwirth 1962** -- Braids = fundamental group of configuration space of non-colliding particles. Theoretical foundation for the phase-space interpretation of DKC.
- **Boyland, Aref & Stremler 2000** -- Pseudo-Anosov braid type forces chaotic fluid mixing. Motivated the (falsified) prediction that computation requires chaos.
- **Thurston 1988 (FLP)** -- Classification of surface homeomorphisms into periodic, reducible, and pseudo-Anosov. Framework for the entropy classification used in D106.
- **Caruso 2013** -- Genericity of pseudo-Anosov braids at large word length. Confirmed by the depth-vs-pA-fraction data.
- **Bigelow 1999 / Krammer 2000** -- Faithfulness of Burau representation for B_4. Ensures the Burau matrices faithfully track braid dynamics (even though this turns out irrelevant to DKC).

### Open Questions
- **What DOES predict per-entry XOR participation strength?** If not entropy, what algebraic property of the TL matrix determines how often an individual entry appears in XOR-computing triples? This is the motivating question for D107-D108.
- **Does the null extend to higher strand counts?** D106 uses W_{4,2} (4-strand). Would the null hold on W_{5,3}, W_{6,4}, etc.? Expected yes, since the TL visibility filter argument is representation-independent.
- **Burau faithfulness vs TL quotient**: The Burau representation is faithful for B_4 (Bigelow/Krammer), retaining all dynamical information. The TL representation is a quotient, discarding dynamics. D106 empirically confirms this algebraic distinction has computational consequences -- the faithful representation's extra information is computationally inert for DKC.
- **Can entropy predict ANYTHING in DKC?** The null is total for Boolean function computation. But could entropy correlate with other catalog properties (e.g., hub frequency, co-occurrence topology, Raqiya graph structure)?
