# Concept Prerequisite DAG

Directed acyclic graph of "to understand X, first understand Y" relationships.
Each node: concept name, one-sentence definition, demo pointer.
Arrows point FROM prerequisite TO dependent concept.

---

## Chain 1: State Sum -> Cyclotomic Evaluation -> Exact Arithmetic -> Forward DKC

```
State Sum (D01)
  Computation summing over all 2^n crossing smoothings, weighted by A and loop count delta.
  |
  v
Bracket Polynomial (D01-D02)
  The Kauffman bracket: state-sum invariant <K>(A) for knot/link diagrams.
  |
  v
Cyclotomic Integer (D27, D29)
  Element of Z[zeta_N] from evaluating A at a root of unity; exact integer arithmetic.
  |
  +--> Z[zeta_8] (D27, D29)
  |      Primary ring at A=e^{i*5pi/4}; basis {1, zeta, zeta^2, zeta^3}, zeta^4=-1.
  |      |
  |      v
  |    Forward DKC (D29)
  |      Bracket values as neural network weights; XOR computed without training.
  |
  +--> Z[zeta_12] (D79)
  |      Infinite-group ring; breaks the XOR8 ceiling imposed by zeta_8's finite group.
  |
  +--> Z[zeta_16] (D54, D65)
         Ring at delta=sqrt(2); Z[i]-axial, 2D subspace; generalized XOR6 at k=24.
```

## Chain 2: TL Algebra -> Non-Semisimplicity -> Radical -> Null States

```
Temperley-Lieb Algebra (D03)
  Algebra of non-crossing strand diagrams; multiplication = stacking + loop replacement.
  |
  +--> Link State / Planar Matching (D01, D03)
  |      Non-crossing arc pairings on boundary points; basis elements of TL_n.
  |
  v
Delta=0 (D29)
  At A=e^{i*5pi/4}, delta=0 exactly; only single-loop states survive the state sum.
  |
  v
Non-Semisimplicity (D38, D51)
  TL_n at delta=0 for even n has nonzero radical; cannot decompose into simple pieces.
  |
  +--> Radical (D38, D51, D102)
  |      Zero in every simple quotient; carries only writhe (abelian character).
  |      |
  |      +--> Sandwich Theorem (D51)
  |      |      TL_{2k}(0) Loewy length exactly 3: semisimple / radical / semisimple.
  |      |
  |      +--> Barrington-Radical Principle (D94, D102)
  |             Radical is abelian; abelian groups cannot compute parity (Barrington 1989).
  |
  +--> Module Types (D03, D38, D85, D99)
  |      Standard W_{n,j}, simple L_j, projective P_{j,j}.
  |      |
  |      v
  |    Ext^1 (D99-D101)
  |      Non-split extensions; indecomposable load-bearing structures.
  |
  +--> Null State (D84)
         Bracket-null entries (Re(q)=0); invisible to trace, directionally indispensable at zeta_8.
         |
         v
       Regime Transition (D84, D87)
         Nulls critical at finite groups (zeta_8), dispensable at infinite groups (zeta_12).
```

## Chain 3: Activation -> k-Sector MVN -> Parity Wall / 11/13 Theorem

```
Boolean Functions (D18-D25)
  Functions {0,1}^n -> {0,1}; braids compute NOT, NAND, AND gates.
  |
  v
NPN Classification (D23, D45)
  Equivalence under negation + permutation; 14 classes for n=3 (13 nontrivial).
  |
  v
Activation Function (D27, D47, D50)
  Rule mapping complex neuron output to discrete label; determines computable functions.
  |
  +--> k-Sector MVN (D50)
  |      k equal angular sectors; parity reachable at k=6 (906 solutions).
  |      |
  |      +--> Triskelion Principle (D50, D63)
  |      |      3-input parity at k=6: each weight in own odd sector at ~120-degree spacing.
  |      v
  |    Parity Wall (D48, D76, D78)
  |      Impossibility barrier resolved 3 times by locating obstruction in different components.
  |      |
  |      v
  |    Wall Pattern (D48, D76, D78)
  |      Recurring "wall was X not Y": confirm, identify component, change it.
  |
  +--> 11/13 Theorem (D61-D62)
  |      Half-plane + multiplicative encoding: exactly 11/13 NPN classes reachable always.
  |      |
  |      v
  |    Complement-Blindness Theorem (D93)
  |      combined_cell merges m with ~m; phase_cell recovers all 13.
  |
  +--> Sign-Hash (D104)
         Ternary sign quantization of TL matrix entries; 1-bit compressed sensing.
```

## Chain 4: Braid Group -> SU(2) -> Hopf Fibration -> S^2 Sweet Spot

```
Braid Group (D01-D09)
  Group B_n of n-strand interleavings; generators sigma_i with Yang-Baxter relations.
  |
  +--> Writhe (D09, D83)
  |      Net signed crossing count; independent resource worth +2 XOR levels.
  |      |
  |      v
  |    Framing Cost (D83)
  |      Jones normalization destroys exactly 2 XOR levels at every root tested.
  |
  v
SU(2) Representation (D66)
  Braids map to unit quaternions in S^3; 24 distinct at zeta_8 = 24-cell vertices.
  |
  +--> 24-Cell (D66)
  |      Regular 4-polytope; vertices = binary octahedral group = braid image.
  v
Hopf Fibration (D67, D70)
  S^3 -> S^2; S^2 base (rotation axes) carries all computation; S^1 fiber inert.
  |
  v
S^2 Sweet Spot (D67-D68)
  13-direction Voronoi on S^2 outperforms S^3 and R^2; intrinsically curved.
  |
  v
13=13 Bandwidth Theorem (D71)
  Mode l=6 has 2*6+1=13 components = 13 directions; sharp all-or-nothing transition.
```

## Chain 5: Encoding -> Parity Lock

```
Encoding (D29, D61, D92)
  How input bits become complex numbers; three types: 1wpi, multiplicative, +/-q.
  |
  +--> Parity-Lock Theorem (D92)
  |      +/-q encoding locked to XOR/XNOR; AND/OR/MAJ structurally impossible.
  |
  +--> Complement-Blindness Theorem (D93)
         combined_cell merges m with ~m; AND/XOR ratio = 1 -> 2762 -> infinity.
```

## Chain 6: Crossing Depth -> Depth Law -> Log Law

```
BFS Catalog (D29, D79, D82)
  Distinct bracket values from breadth-first braid generator multiplication, indexed by depth.
  |
  v
Crossing Depth (D82)
  BFS birth round; minimum generator multiplications to produce an entry.
  |
  v
Depth Law (D82)
  max_xor ~ depth + 6; each closure round adds fixed increment to capacity.
  |
  +--> Log Law (D81-D82)
  |      max_xor ~ 0.62 * log2(catalog) + 4.6; corollary via exponential BFS growth.
  |
  +--> Balanced Exponentials (D89-D91)
         BFS ~2x/round vs parity demand ~4x/level; ratio = ~2 rounds per XOR level.
```

## Chain 7: Module Types -> Multi-Strand DKC

```
Module (D03, D38, D99)
  Standard W_{n,j}, simple L_j, projective P_{j,j}; TL representation spaces.
  |
  +--> Ext^1 (D99-D101)
  |      W_{4,2} (radical, XOR14=70) beats W_{5,3} (simple, XOR12 cap).
  v
Multi-Strand DKC (D99-D101)
  First 3-strand XOR via Delta_1; sl_d growth ~d*x per BFS round.
```

---

## CROSS-LINKS (bridges between chains)

| Link | Chains | Why |
|------|--------|-----|
| Delta=0 <-> Cyclotomic Integer | 2 <-> 1 | delta=0 is the evaluation point producing Z[zeta_8] |
| Non-Semisimplicity <-> S^2 Sweet Spot | 2 <-> 4 | Null entries own 6 exclusive S^2 directions |
| Radical <-> Writhe | 2 <-> 4 | Radical carries writhe as abelian character B_n -> Z/8Z |
| Encoding <-> Activation Function | 5 <-> 3 | Jointly determine which NPN classes are computable |
| 24-Cell <-> Null State | 4 <-> 2 | 9/24 vertices are bracket-null |
| Forward DKC <-> NPN Classification | 1 <-> 3 | Exact weights measured by Boolean function classes |
| BFS Catalog <-> Z[zeta_N] | 6 <-> 1 | Catalog is the concrete set of cyclotomic values |
| Crossing Depth <-> Reservoir Computing | 6 <-> meta | Depth law = RC memory capacity theorem on braid reservoir |
| Parity-Lock <-> Depth Law | 5 <-> 6 | Depth law is parity-specific because encoding locks to XOR |
| Barrington-Radical <-> Parity Wall | 2 <-> 3 | Abelian radical explains why parity is hardest |
| Five Pillars | meta | Habiro(1) + Nazer-Gastpar(1) + Abramsky(2) + Aizenberg(3) + RC(6) |

---

## READING ORDER (suggested prerequisite traversal)

1. **Chain 1** (State Sum -> Forward DKC): the foundational "what"
2. **Chain 3** (Boolean Functions -> Parity Wall): the computational "why it matters"
3. **Chain 2** (TL Algebra -> Null States): the algebraic "why it works"
4. **Chain 6** (BFS Catalog -> Depth Law): the scaling "how much"
5. **Chain 4** (Braid Group -> 13=13 Theorem): the geometric "where it lives"
6. **Chain 5** (Encoding -> Parity Lock): the structural constraints
7. **Chain 7** (Modules -> Multi-Strand): the frontier
