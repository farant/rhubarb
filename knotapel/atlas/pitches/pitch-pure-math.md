# Pure Mathematics Pitch: New Atlas Documents

*Panel role: Pure Mathematician*
*Date: 2026-02-26*

---

## Idea 1: Obstruction Atlas

**Location:** `atlas/synthesis/obstructions.md`

**What it would contain:**

A systematic catalog of every impossibility result, wall, ceiling, and negative finding in the research, organized not by demo number but by *type of obstruction*. Each entry would include:

- The precise statement of what is impossible
- The mathematical mechanism (convexity, pigeonhole, valuation failure, group-theoretic, spectral, combinatorial)
- The *scope* of the obstruction (does it apply to all roots of unity? all activations? all representations?)
- Known bypass routes (which parameter change dissolves the wall)
- Formal proof status (proven / demonstrated / conjectured)
- Dependency graph: which obstructions imply which others

The document would organize obstructions into a partial order by strength. For example:

```
Convexity obstruction (0x06, 0x1B: half-plane wall)
  |-- bypassed by: non-convex activation (MVN k>=3)
  |-- scope: universal across all rings, all multiplicative encodings
  |-- implies: 11/13 ceiling for Re>0

Parity ceiling n=5 for Z[zeta_8]
  |-- two independent mechanisms:
  |     constraint wall (n=6,7): 63 subset constraints unsatisfiable
  |     pigeonhole (n>=8): only 7 class-1 octants
  |-- bypassed by: switching to infinite group (zeta_12)
  |-- scope: algebra-independent (same at Z[zeta_16])
  |-- does NOT imply: parity ceiling n=5 for zeta_12 (which has no such ceiling)

Valuation condition failure at TL_6
  |-- mechanism: p_tt = 4 != 2*3 = 2*p_Tt
  |-- scope: leading-coefficient extraction on regular rep at n >= 6
  |-- not bypassed by: single P_{0,0} (universal divergence, D86)
  |-- open: transfer-matrix method may bypass entirely

Finite group ceiling (zeta_8: XOR8 max)
  |-- mechanism: 24-element binary octahedral group exhausts combinatorial diversity
  |-- bypassed by: infinite group root (zeta_12)
  |-- scope: all finite SU(2) subgroups (zeta_4 and zeta_8 only)
  |-- implies: logarithmic scaling law is unreachable at finite groups

Complement-blindness (1wpi + combined_cell)
  |-- mechanism: S(~m) = -S(m) and |qa| normalization identifies S with -S
  |-- bypassed by: phase_cell (no sign flip)
  |-- scope: any encoding where complement negates the sum
  |-- independent of: parity-lock (different encoding, different mechanism)

Multiplicative channel blindness (D73)
  |-- mechanism: M -> M^{-1} preserves eigenvector cell, flips parity
  |-- bypassed by: additive formulation
  |-- scope: all group-product encodings on S^2
  |-- implies: DKC is fundamentally additive, not multiplicative

Self-doubling impossibility (D64)
  |-- mechanism: doubling preserves angle, class-1 doubled stays class-1
  |-- scope: any weight repetition in Z[zeta_8]
  |-- accounts for: 89.5% of closest XOR6 misses
  |-- independent of: higher-order (GHZ-type) obstructions (remaining 10.5%)
```

**Why it matters mathematically:**

In every mature mathematical research program, the obstructions become as important as the constructions. The current atlas documents obstructions scattered across narrative.md, theorems.md, and novelty.md -- you can find that the parity ceiling is n=5, or that the valuation condition fails at TL_6, but there is no document that *relates* these obstructions to each other. A mathematician encountering this body of work would immediately ask: "What is the shape of the impossibility landscape? Which walls are the same wall seen from different angles? Which walls are genuinely independent?"

The partial order of obstructions would reveal whether there is a single "master obstruction" (e.g., non-semisimplicity) from which all others follow, or whether the landscape has multiple independent roots. The current evidence suggests at least three independent obstruction families (activation-geometric, algebraic-structural, group-finiteness), but this has never been stated precisely.

**What gap it fills:**

theorems.md lists results including negative ones, but treats them as flat entries without structure. narrative.md tells the story of encountering walls, but the narrative ordering obscures the logical dependencies between walls. No existing document answers: "If I change parameter X, which obstructions survive and which dissolve?" An obstruction atlas would.

---

## Idea 2: Conjecture Dependency Graph

**Location:** `atlas/inventory/conjecture-graph.md`

**What it would contain:**

A formal directed graph of all open conjectures and unproven results, with edges indicating logical dependencies ("proving X would imply Y", "X and Y are equivalent", "X is a special case of Y"). Currently research-questions.md lists ~50 open questions, and theorems.md marks results as CONJECTURED or COMPUTATIONALLY VERIFIED, but neither document captures the *implication structure* between them.

Concretely, the document would include:

1. **Equivalence classes of conjectures.** Which conjectures, if proven, would automatically prove others? For example:
   - Does "Universal Nilpotency 3" (rad^3 = 0 for all TL_n at all roots of unity) follow from Graham-Lehrer cellularity in full generality, or only for specific cellularity parameters? The document would trace the logical chain.
   - Does the "Algebra-independence of parity ceiling" conjecture follow from some property of the activation geometry alone, independent of the ring? The current evidence (tested at Z[zeta_8] and Z[zeta_16]) suggests yes, but the conjecture has scope "all cyclotomic rings."
   - The Sandwich Theorem (rad^2 of TL_{2k}(0) iso TL_{2k-1}(0)) and the Quotient Isomorphism (TL_{2k}(0)/rad iso TL_{2k-1}(0)) -- are these logically equivalent, or does one strictly imply the other? The top-bottom mirror in the Sandwich Theorem goes beyond the quotient result.

2. **Critical path analysis.** Which single conjecture, if resolved, would unlock the most downstream results? This is the mathematical analog of next-priorities.md but focused on *proof strategy* rather than *experimental agenda*. Candidates for highest-impact resolution:
   - The Sandwich Theorem proof would likely unlock the Galois symmetry conjecture (Q2.2) and the palindromic sector rank conjecture (Q2.4) as corollaries.
   - A formal proof of the convexity thesis (Q1.1) would unify the 0x06/0x1B proofs under a single framework and potentially explain the 11/13 constant.
   - Proving the 13=13 theorem analytically (Q6.11) would establish a general binary-Voronoi-on-S^2 bandwidth theorem applicable far beyond DKC.

3. **Upgrade paths.** For each COMPUTATIONALLY VERIFIED result, what would be needed to upgrade it to PROVEN? Classification by proof technique:
   - *Cellular algebra* candidates: Sandwich Theorem, Universal Nilpotency 3, Catalan Trace Theorem
   - *Convex geometry* candidates: convexity thesis formalization, unified 0x06/0x1B proof
   - *Harmonic analysis on S^2* candidates: 13=13 analytical proof, spectral universality
   - *Group theory* candidates: power-of-two finiteness conjecture, ADE exhaustion proof
   - *Combinatorics* candidates: funnel rate formula, n=6 constraint wall algebraic proof
   - *Matroid theory* candidates: vocabulary stratification (Reiner connection), OM type generalization to n=4,5

4. **Conditional results.** Several demonstrated results have the implicit form "X holds, assuming Y is true at all tested cases." Making these conditionals explicit would reveal the logical skeleton:
   - The depth law (max_xor ~ depth + 6) is demonstrated at zeta_12 only. Does it hold at zeta_16, zeta_24?
   - The framing = +2 XOR levels is demonstrated at zeta_8 and zeta_12. Universal?
   - The resource decomposition (lattice + depth + framing) assumes additivity. Tested at two roots.

**Why it matters mathematically:**

This is standard practice in any serious conjecture-driven research program (the Langlands program, the classification of finite simple groups, Thurston's geometrization). The DKC program now has 110+ results at various stages of rigor. Without a dependency graph, it is impossible to assess the *fragility* of the theoretical edifice -- how many unproven assumptions does a given "demonstrated" result rest on?

It would also guide paper writing: a paper that proves two conjectures connected by implication edges is worth more than one that proves two isolated conjectures, because the downstream consequences cascade.

**What gap it fills:**

theorems.md is a flat list. research-questions.md is a flat list organized by topic, not by logical dependency. proofs-index.md tracks 4 proofs but not their relationships to unproven results. No document captures the *graph structure* of the open mathematical questions, and no document answers: "What is the most strategically important thing to prove next?"

---

## Idea 3: Algebraic Structure Catalog

**Location:** `atlas/inventory/algebraic-structures.md`

**What it would contain:**

A systematic reference catalog of every algebraic object that has appeared in the research, with its classification data, relationships to other objects in the catalog, and computational role. This would be organized by type:

**Groups:**

| Group | Order | ADE | Generators | Conj. Classes | S^2 Dirs | Eigenvalue Angles | Solvable | DKC Role | Demo |
|-------|-------|-----|------------|---------------|----------|-------------------|----------|----------|------|
| 2O (binary octahedral) | 48 | E_7 | sigma_1=(1+i)/sqrt(2), sigma_2=(1-k)/sqrt(2) | 8 | 13 | 0,45,60,90 | Yes | Primary weight catalog | D66 |
| 2I (binary icosahedral) | 120 | E_8 | s=(1+i+j+k)/2, t=(phi+phi_inv*i+j)/2 | 9 | 31 | ? | No | Solvability test | D94 |
| Q_8 (binary dihedral) | 8 | D_4 | i, j | 5 | ? | ? | Yes | Degenerate case | D80 |
| 2T (binary tetrahedral) | 24 | E_6 | ? | ? | ? | ? | Yes | Derived subgroup of 2O | D95 |

- Derived series of 2O: 24 > 12 > 4 > 1 (corresponding to 2O > 2T > Q8 > {+/-1})
- Finiteness criterion: only pi/2^k generators close (demonstrated, not proven)
- For each group: center, abelianization, character table reference

**Rings:**

| Ring | Rank | Class Number | Axiality at DKC | Catalog Size | Automorphism Group | Nesting |
|------|------|--------------|-----------------|--------------|--------------------|---------|
| Z[zeta_8] | 4 | 1 (UFD) | Z-axial (1D) | 100 | Gal(Q(zeta_8)/Q) ~ (Z/8Z)* | contains Z[i] |
| Z[i] | 2 | 1 | Z[i]-axial (2D) at ell=4 | 56 | Gal(Q(i)/Q) ~ Z/2Z | contained in Z[zeta_8] |
| Z[omega] | 2 | 1 | Z-axial (1D) at ell=3 | 6 | Gal(Q(omega)/Q) ~ Z/2Z | -- |
| Z[zeta_5] | 4 | 1 | full 4D at ell=5 | 116 | Gal(Q(zeta_5)/Q) ~ (Z/5Z)* | -- |
| Z[zeta_16] | 8 | 1 | Z[i]-axial at ell=4 | 36 | Gal(Q(zeta_16)/Q) | contains Z[zeta_8] |
| Z[zeta_24] | 8 | 1 | full at ell=6 | 29 | -- | -- |

- Direction Nesting Theorem: zeta_N directions nest exactly into zeta_{2N}
- Ring inclusion lattice: Z subset Z[i] subset Z[zeta_8] subset Z[zeta_16] subset ...

**Algebras (TL_n at roots of unity):**

For each tested (n, ell) pair: dimension (C_n), radical dimension, rad^2 dimension, Loewy length, semisimplicity status, Gram rank, cell module dimensions d(n,j), projective cover dimension, Jordan block status on W_{n,0} and P_{0,0}.

Key structural data:
- Wedderburn-Artin decomposition at semisimple cases (odd n at delta=0)
- Cell filtration: sector sizes = d(n,j)^2 (Graham-Lehrer)
- Quotient isomorphism: TL_{2k}(0)/rad iso TL_{2k-1}(0)
- Standard modules tested in multi-strand arc: W_{3,1} (Delta_1), W_{4,2}, W_{5,3}

**Polytopes / Geometric Configurations:**

- 24-cell: 24 vertices, F4 symmetry (order 1152), self-dual, Voronoi structure (25 cells including zero cell), relationship to binary octahedral
- 13-direction eigenvector configuration: explicit coordinates, 6 null-only (cube-edge midpoints), 4 non-null-only (tetrahedral), 3 shared (coordinate axes), spherical harmonic bandwidth l=6, t-design status t=0
- Voronoi partition: 14 cells, 6 computational types (orbit structure under octahedral symmetry)

**Why it matters mathematically:**

The research has assembled a substantial collection of concrete algebraic objects over 101 demos. These objects are currently scattered: the binary octahedral group is discussed in D66, D69, D72, D73, D80, D88, D95, D96, D97; TL algebra data is in D38, D39, D51, D52, D60, D85, D86. A mathematician looking for "what is the derived series of the binary octahedral group" or "what is the Loewy length of TL_6 at delta=sqrt(2)" currently has to search across multiple demos and narrative sections.

Having all the algebraic objects in one place would make structural patterns visible. The ADE classification thread runs through the entire research (D_4 at zeta_4, E_7 at zeta_8, E_8 at 2I in D94) but is never gathered into a single view. The cyclotomic ring inclusion lattice determines the direction nesting theorem but is never drawn as a diagram. The relationship between TL algebra parameters and DKC capacity is tracked demo-by-demo but never consolidated into a single reference table.

This catalog would also be the natural home for recording *what is known vs what is computed* about each object -- e.g., the character table of 2O is classical, but its derived series was first computed explicitly in DKC context in D95.

**What gap it fills:**

data-tables.md contains numerical tables but not structural/classification data. theorems.md contains statements about these objects but not the objects themselves. code-assets.md catalogs implementations but not the mathematical objects they implement. There is no document where a mathematician can look up "what algebraic structures does this research use, what are their key invariants, and how do they relate to each other?"

---

## Idea 4: Proof Strategy Landscape

**Location:** `atlas/synthesis/proof-landscape.md`

**What it would contain:**

A document mapping the *techniques* used across all proofs, proof sketches, and computational verifications, organized by mathematical method rather than by result. The goal is to answer: "What proof tools does this research program have, and where are the gaps?"

**Sections:**

**1. Cellular algebra arguments (Graham-Lehrer theory).**
Used in P01, P02, P03. The workhorse technique. Capabilities: compute radical dimensions, prove nilpotency bounds, identify degenerate cell modules, derive Markov trace truncation formulas. Limitations: cannot prove the Sandwich Theorem (which needs an explicit bimodule isomorphism, not just dimension matching). Current frontier: the Sandwich Theorem remains the most important unproven result approachable by this technique. Key tool: the linking theorem (cell module V_j degenerates when its linked module exists, with corank = dim of linked module).

**2. Exhaustive computational verification.**
Used pervasively (100M quartet search D48, 46.7M angle triples D61, all C(24,4) quadruples D76-77). The program's signature method. This section would classify verified-but-unproven results by analytical tractability:
- *Likely provable*: 82.8% universal automaton constant (from group transitivity), spectral universality (from representation theory of 2O on S^2), non-monotonic parity solutions (from number-theoretic gcd analysis)
- *Needs new technique*: funnel rate formula (99.3%, 39.2%, 19.2%, 0%), vocabulary stratification (44 good / 31 poison), F4 orbit count
- *Essentially computational*: 100-value catalog completeness, specific XOR solution counts

**3. Convex geometry / interval arguments.**
Used in D62 (0x06 and 0x1B proofs, two distinct methods), Complex Minsky-Papert theorem (D49). Small but precise toolkit. The 0x1B proof (interval squeeze) and 0x06 proof (four-semicircle parallelogram) have different structures -- local vs global obstruction. Potential extensions: formal convexity thesis, unified 0x06/0x1B framework.

**4. Counting / pigeonhole / collision arguments.**
Used in n>=8 impossibility, parity-lock theorem (P04), complement-blindness theorem (D93), self-doubling impossibility (D64). These are the cleanest proofs in the program. The pattern: encoding creates exact collisions between inputs that should map to different outputs. The (0,0)=(1,1) collision in parity-lock and the S(~m)=-S(m) collision in complement-blindness are structurally parallel. Scope: applies whenever the encoding has an exact symmetry that the target function does not respect.

**5. Spectral / harmonic analysis on S^2.**
Used in D71 (13=13 theorem, computational proof), D90 (spectral inversion -- bandwidth decreases with depth). Nascent but powerful. The 13=13 theorem has a computational proof but an open analytical proof that would require a general binary-Voronoi bandwidth theorem on S^2. The spectral universality (all 36 winners share the same envelope) likely follows from the representation theory of the binary octahedral group acting on spherical harmonics -- the invariant subspaces under the group action would constrain which modes can appear.

**6. Group theory / ADE classification.**
Used in D80 (finiteness survey), D94 (2I construction), D95 (derived series). The ADE thread connects to the deepest mathematical structures in the research. The finiteness criterion (only pi/2^k generators close) is demonstrated but not formally proven -- a proof would likely use the classification of finite subgroups of SU(2) plus an analysis of which subgroups are reachable by the specific two-perpendicular-axes construction. The solvability bottleneck (non-solvable 2I beats solvable z8) connects to Barrington's theorem from circuit complexity.

**7. Missing techniques -- where the program should grow.**
What proof methods would the program benefit from that it does not currently use?
- *Galois cohomology*: for the Galois symmetry conjecture (Q2.2) -- does sandwich duality correspond to a Galois automorphism?
- *Matroid theory (beyond ad hoc)*: for the funnel rate conjecture, vocabulary stratification, and the Reiner cyclotomic matroid connection flagged in D64
- *Category theory / homological algebra*: for the Ext^1 thesis (non-semisimple extensions are computationally load-bearing), which is currently demonstrated but has no formal framework. The extension 0 -> L(0) -> Delta_1 -> L(1) -> 0 in D99 is a concrete Ext^1 class; formalizing why non-split extensions carry more computational power than direct sums would be a significant theoretical advance.
- *Algebraic K-theory*: for the resource decomposition conjecture (lattice + depth + framing are additive). The additivity of independent computational resources resembles the additivity of K-theory classes under direct sum.
- *Compressed sensing / restricted isometry*: for the super-Nyquist 3.5x compression result (D71). The current explanation invokes compressed sensing informally; a formal restricted isometry argument for binary functions at known support positions would be a standalone result.

**Why it matters mathematically:**

Proof techniques are *transferable* across results in a way that individual theorem statements are not. Knowing that "cellular algebra arguments have been the main workhorse and have proved 3 of 4 formal results" immediately tells a mathematician where the research program's analytical strength lies and where it is weakest. The fact that the spectral/harmonic tools are nascent but yielded the 13=13 theorem suggests they are underexploited. The identification of "missing techniques" points toward productive collaborations and literature studies.

More concretely: the program currently has 110+ results but only 4 formal proofs. The ratio of demonstrated-to-proven is roughly 25:1. A proof strategy landscape would prioritize which of those 100+ unproven results are most tractable and by which method, transforming the research from a primarily computational enterprise into one with a clear analytical agenda.

**What gap it fills:**

proofs-index.md catalogs 4 proofs but not their techniques. theorems.md lists status (PROVEN/VERIFIED/CONJECTURED) but not proof method. research-questions.md lists questions with difficulty ratings but no discussion of *what techniques might attack them*. No existing document provides a bird's-eye view of the proof methodology landscape, and no document answers: "What kind of mathematician should we talk to next?"
