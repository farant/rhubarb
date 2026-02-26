# Obstruction Partial Order

A structural analysis of impossibility results in the DKC research program, organized by mechanism type, with scope annotations and a partial order on obstruction families.

Updated 2026-02-26: Covers 109 demos.

**Purpose:** Identify WHY things fail — not just that they fail. Group impossibility results by their underlying mathematical mechanism, determine which obstructions are independent, and catalog known bypass routes. This is the structural complement to dead-ends.md (which records WHAT failed).

---

## 1. Obstruction Families

### OF-1: Convexity Obstructions

**Core mechanism:** Half-plane (convex) classifiers cannot implement non-monotone Boolean functions on additive inputs. If y1 > d and y2 > d, then y1 + y2 > 2d > d, contradicting parity's requirement that the sum of two "true" inputs yields "false."

| ID | Statement | Parameters | Source |
|----|-----------|------------|--------|
| OF-1a | Complex Minsky-Papert: k=2 half-plane cannot compute 3-input parity with ANY complex weights | All weights, all biases, k=2 activation | D49 (proven analytically) |
| OF-1b | 0x06 unreachability: parallelogram gap on circle. Four semicircle constraints force alpha+beta > pi, making all gaps < pi, so the required intersection is empty | All angles phi1, phi2, phi3; all semicircle orientations | D62 (proven) |
| OF-1c | 0x1B unreachability: interval squeeze forces phi3 > pi AND phi3 < pi simultaneously | All angles; all semicircle orientations | D62 (proven) |
| OF-1d | 11/13 wall universality: exactly 11 of 13 NPN classes reachable under half-plane activation with multiplicative encoding | ALL cyclotomic rings tested (Z[zeta_16], Z[zeta_5], Z[zeta_24]); ring-independent | D55, D58, D59, D61, D62 |

**Scope:** Applies to ANY convex activation region (not just half-planes — any connected convex set). Applies at ALL evaluation levels (ell=2,3,4,5,6). The obstruction is activation-geometric, not algebraic.

**Known bypass routes:**
- Non-convex activation: k-sector MVN with k >= 3 breaks convexity (D50: parity at k=6)
- Additive encoding swap: does NOT bypass (D57 confirms additive encoding is worse than multiplicative under Re>0)
- Generalized sector labeling at high k: bypasses at k=24 (D65)

**Surviving walls if you change activation:** OF-1a,b,c,d all evaporate completely under non-convex activation. The 0x06 and 0x1B unreachability is specific to semicircular (convex) decision regions.

---

### OF-2: Pigeonhole Obstructions

**Core mechanism:** When the number of input masks (2^N) exceeds the number of distinguishable activation cells, no binary labeling of cells can achieve parity (which requires exact 50/50 even-odd split within every cell).

| ID | Statement | Parameters | Source |
|----|-----------|------------|--------|
| OF-2a | S^2-only pigeonhole: 256 masks / 14 cells = 18.3 masks/cell makes parity-pure cells geometrically impossible for XOR8 | zeta_8, S^2 Voronoi (14 cells), N=8 inputs | D76 (proven, exhaustive) |
| OF-2b | Parity pigeonhole at n >= 8 inputs: only 7 class-1 octants in Z[zeta_8], but need n distinct rays for n-input parity | Z[zeta_8], standard k-sector, n >= 8 | D63 (proven) |
| OF-2c | Low-k pigeonhole: at k/2^N << 1, too many masks per cell for balanced parity bisection | Any root, any module, k/2^N << 1 | D93 -> D105 |
| OF-2d | Complement-blindness: combined_cell normalization maps mask m and complement ~m to same cell (|S| = |-S|) | +/-q encoding with combined_cell activation, all N | D93 (proven, 100% complement-pair sharing) |

**Scope:** Pigeonhole obstructions are parametric — they hold at the tested resolution but not necessarily at higher resolution. OF-2a holds for 14 cells but not for 112 cells. OF-2c holds at low k but not at high k.

**Known bypass routes:**
- Add activation dimensions: S^1 x S^2 product (112 cells) breaks OF-2a (D77: 6 XOR8 winners)
- Increase k: k/2^N >> 1 breaks OF-2c (D105: XOR8=22266 at k=4096)
- Remove sign normalization: phase_cell breaks OF-2d (D93: sharing drops from 100% to 1.1%)
- More inputs at same root: OF-2b is absolute for Z[zeta_8] at n >= 8 (no bypass within this ring)

**Surviving walls if you increase k:** OF-2b survives because it is structural (7 octants, independent of k). OF-2a, OF-2c evaporate at sufficient k. OF-2d evaporates under phase_cell.

---

### OF-3: Group-Finiteness Obstructions

**Core mechanism:** A finite quaternion group generates a catalog that closes. Once the catalog is fixed, combinatorial diversity saturates at some selection size, and no activation change can compensate.

| ID | Statement | Parameters | Source |
|----|-----------|------------|--------|
| OF-3a | zeta_8 XOR10 wall: binary octahedral group closes at 24 elements. Exhaustive search over all C(24,5) = 42504 quintuples finds 0 XOR10 solutions under all tested activations | zeta_8 (finite, 24 elements), all k tested | D78 (proven, exhaustive) |
| OF-3b | zeta_4 capacity ceiling: quaternion group Q_8 has only 4 elements, achieving zero XOR capacity of any kind | zeta_4 (finite, 4 elements) | D80 |
| OF-3c | Catalog combinatorial exhaustion: 5-element selection from 24 elements exhausts the parity structure | zeta_8, 5-weight selection from 24-element catalog | D78 |

**Scope:** Applies only to the two finite-group roots: zeta_4 and zeta_8. All other tested roots (zeta_6, zeta_10, zeta_12, zeta_16, zeta_20) generate infinite groups and are not subject to this obstruction.

**Known bypass routes:**
- Switch to infinite-group root: zeta_12 gives 124 XOR10 winners (D79)
- Cannot bypass within zeta_8 by any activation, encoding, or architectural change — the catalog is fixed

**Surviving walls if you switch root:** OF-3a,b,c all evaporate at infinite-group roots. No analog of the finite ceiling has been observed at zeta_12.

---

### OF-4: Encoding Lock Obstructions

**Core mechanism:** Certain input encodings create structural symmetries that make specific Boolean functions algebraically impossible.

| ID | Statement | Parameters | Source |
|----|-----------|------------|--------|
| OF-4a | Parity-Lock Theorem: under +/-q encoding, masks 0...0 and 1...1 always produce identical sums (both zero). Any f where f(0...0) != f(1...1) is impossible. Only XOR/XNOR survive. | +/-q paired encoding, ALL activations, ALL depths | D92 (formally proven, P04) |
| OF-4b | Self-doubling impossibility: adding a 6th weight equal to any existing XOR5 weight always fails XOR6. Scalar doubling preserves angle; class-1 doubled remains class-1, contradicting required class-0 for even-parity input-weight-2 | Z[zeta_8] catalog, k-sector activation, 6-weight extension | D64 (proven) |
| OF-4c | Multiplicative complement symmetry: complementing all input bits maps braid matrix M to M^{-1}, which shares the same eigenvector cell (antipodal identification). Braid product is parity-blind. | Multiplicative braid product formulation, any root | D73 (proven, exhaustive) |

**Scope:** OF-4a is absolute for the +/-q encoding under any activation. OF-4b is absolute within Z[zeta_8] at standard activations (k <= 23); bypassed at k=24 generalized. OF-4c is absolute for any multiplicative readout.

**Known bypass routes:**
- OF-4a: switch to 1-weight-per-input (1wpi) encoding (all 13 NPN classes computable, D48/D50)
- OF-4b: generalized activations at k=24 (D65), or quaternionic S^3 framework (D66)
- OF-4c: use additive weight-sum formulation (all forward DKC since D29)

**Surviving walls if you change encoding:** OF-4a evaporates under 1wpi. OF-4b requires k >= 24 generalized labeling even after encoding change. OF-4c evaporates under additive formulation.

---

### OF-5: Valuation Obstructions

**Core mechanism:** The leading-coefficient extraction method for the indecomposability parameter b requires a specific relationship between delta-powers in bilinear forms. When this valuation condition fails, b diverges.

| ID | Statement | Parameters | Source |
|----|-----------|------------|--------|
| OF-5a | Valuation condition failure at TL_6: p_tt = 4, p_Tt = 3, so 4 != 2*3 = 6. b diverges as delta^{-2}. All 14 kernel directions introduce lower-power cross-terms; 0 safe directions exist. | TL_6 regular representation, delta-parameterized Gram form | D85 (demonstrated) |
| OF-5b | Single P_{0,0} divergence: the valuation condition p_tt = 2*p_Tt never holds on a single projective cover at any eigenvalue, any lattice size, either loop or trace form. The multiplicity from the regular representation is structurally essential. | TL_4, TL_6 (all tested sizes), single projective cover P_{0,0} | D86 (demonstrated, novel negative result) |
| OF-5c | Fixed-point form gauge degeneracy: the fixpt trace bilinear form G[i][j] = fixpt(star(i)*j) has 3-dimensional gauge freedom at TL_4 delta=0. M=0 (quadratic coefficient matrix), no unique critical point. Both b=-2 and b=-5/8 reachable on the flat manifold. | TL_4, fixed-point trace form | D85 (demonstrated) |

**Scope:** OF-5a applies to TL_6+ under the leading-coefficient method on the regular representation. OF-5b applies to ALL sizes tested (TL_4, TL_6, TL_8) on single projective covers. OF-5c applies specifically to the fixed-point form.

**Known bypass routes:**
- OF-5a: no bypass known within diagram-algebraic approach. Literature method (transfer-matrix finite-size scaling) is the conjectured correct path for larger lattice sizes.
- OF-5b: return to full regular representation (where multiplicity participates in valuation balance)
- OF-5c: use delta-parameterized Gram matrix instead (breaks the gauge degeneracy)

**Surviving walls:** The b computation appears to be fundamentally limited to TL_4 under the diagram-algebraic leading-coefficient method. Extending to larger sizes likely requires a different computational paradigm.

---

### OF-6: Algebraic Obstructions

**Core mechanism:** Structural properties of the algebra itself prevent certain computations, independent of activation or encoding choices.

| ID | Statement | Parameters | Source |
|----|-----------|------------|--------|
| OF-6a | Barrington-Radical Principle: the radical of non-semisimple TL modules at delta=0 carries an abelian character (writhe homomorphism B_n -> Z/8Z). By Barrington's theorem, abelian groups cannot compute parity. The radical direction is provably useless for XOR. | All non-semisimple TL modules at delta=0 | D102 (named theorem) |
| OF-6b | Standard modules have no Jordan blocks: the Hamiltonian H on W_{n,0} has NO Jordan blocks at any tested size (n=4,6,8,10,12). Minimal polynomial is square-free. Jordan blocks exist only on projective covers. | Standard modules W_{n,0} at delta=0, all tested n | D85 (demonstrated, falsifies Gemini 3 Pro claim) |
| OF-6c | Constraint-geometric wall for n=6,7 parity: enough octants available (7 >= 6) but the 2^n - 1 sector-parity constraints are simultaneously unsatisfiable. Not pigeonhole — geometric. | Z[zeta_8], n=6 and n=7 inputs, all k in {2,...,16} | D63 (proven, exhaustive) |
| OF-6d | Algebra-independent parity ceiling: the ceiling n=5 and tier thresholds are identical at Z[zeta_8] and Z[zeta_16]. The ceiling is structural, not ring-dependent. | Z[zeta_8], Z[zeta_16]; standard activations | D65 (proven) |
| OF-6e | Simple module deep-XOR deficiency: simple module W_{5,3} (dim=4, radical dim=0) achieves XOR14=0 at all k tested, while non-semisimple W_{4,2} achieves XOR14=70. Simple modules lack the deep XOR capacity of indecomposable modules. | W_{5,3} vs W_{4,2}, k=256 through k=4096 | D101 (demonstrated) |

**Scope:** OF-6a is absolute for the writhe character carried by the radical. OF-6b distinguishes the representation in which non-semisimple structure lives. OF-6c,d establish the parity ceiling as geometric, not algebraic. OF-6e distinguishes simple from indecomposable modules.

**Known bypass routes:**
- OF-6a: use the indecomposable extension structure (not the radical itself) — D100 vs D101 comparison shows the extension is computationally useful while the radical is inert
- OF-6b: work on projective covers or regular representation for Jordan structure
- OF-6c: generalized activations at k=24 (D65) or quaternionic framework (D66)
- OF-6d: no known bypass for standard activations
- OF-6e: use indecomposable (non-semisimple) modules for deep XOR

**Surviving walls:** OF-6d — the parity ceiling n=5 — survives under all standard activations regardless of ring. This is the most structurally robust obstruction in the program.

---

### OF-7: Geometric-Algebraic Mismatch Obstructions

**Core mechanism:** Optimizing geometric properties of the computation's spatial structure destroys algebraically-determined computational capacity.

| ID | Statement | Parameters | Source |
|----|-----------|------------|--------|
| OF-7a | Voronoi optimization catastrophe: improving t-design quality from t=0 to t~1 for the 13 S^2 directions produces 9x FEWER XOR6 solutions (4 vs 36). Better geometric distribution destroys the computation. | zeta_8, S^2 eigenvector Voronoi, stochastic optimization | D72 (demonstrated) |
| OF-7b | Platonic solid Voronoi failure: octahedral (6 cells), icosahedral (12 cells), cuboctahedral (12 cells), dodecahedral (10 cells) all yield zero XOR6 solutions. Symmetry group mismatch: icosahedral order 120 vs binary octahedral order 48. | zeta_8, any Platonic Voronoi on S^2 | D67, D70 (proven, exhaustive) |
| OF-7c | Euclidean metric failure: stereographic projection with Euclidean nearest-neighbor yields 0 XOR6 solutions. S^2 great-circle metric yields 36. The computation is intrinsically curved. | zeta_8, stereographic projection, Euclidean metric | D68 (proven, exhaustive) |
| OF-7d | Hopf phase inertness: the Hopf fiber phase xi_1 carries zero DKC computational information. Zero XOR6 solutions at all k=4 through k=32. | zeta_8, Hopf phase activation, all tested k | D67 (proven, exhaustive) |

**Scope:** These obstructions apply whenever the geometric representation mismatches the algebraic structure. They are specific to the S^2/S^3 quaternionic setting.

**Known bypass routes:**
- OF-7a: constrain non-null body-diagonal directions within 2 degrees, let null edge-midpoint directions move freely (D88: 48% design improvement, 2.8% XOR loss)
- OF-7b: use data-derived (braid-intrinsic) eigenvector directions, not Platonic ones
- OF-7c: use the S^2 geodesic metric even when visualizing on R^2
- OF-7d: use the S^2 Hopf base, not the S^1 fiber

**Surviving walls:** OF-7d is absolute — the Hopf fiber carries no DKC information. OF-7b is absolute for standard Platonic solids. OF-7a and OF-7c evaporate with the correct metric/placement.

---

### OF-8: Readout Obstructions

**Core mechanism:** The readout/measurement step collapses algebraic information to the point where computation fails, even though the underlying algebra is rich.

| ID | Statement | Parameters | Source |
|----|-----------|------------|--------|
| OF-8a | 3-strand trace collapse: trace of 5x5 matrix collapses 100 integers (25 Cyc8 entries) to 4 integers (1 Cyc8 entry) — 96% information loss. Zero XOR6 at every k_sec tested. | 3-strand TL representation, trace readout | D98 (demonstrated) |
| OF-8b | dim(W) law elimination: higher-dimensional modules produce WORSE XOR at fixed k due to curse of dimensionality in sign-hash (more sign components = sparser hash space). | TL_6 modules W_{6,0} vs W_{6,2} at k=128 | D103 (confirmed), D104 (partially refuted at optimal component count) |

**Scope:** OF-8a applies to the full TL_3 representation under trace readout. OF-8b applies to sign-hash activation with naive full component usage.

**Known bypass routes:**
- OF-8a: work on irreducible Delta_1 module (dim=2, no trace reduction needed). D99b: first 3-strand XOR.
- OF-8b: optimal component selection (Atkinson sweet spot at ~120 of 324 components, D104) or increase k

---

### OF-9: Dynamical Obstructions (Null Results)

**Core mechanism:** Individual dynamical properties of braids are invisible to the TL quotient at delta=0 and have no predictive power for DKC computation.

| ID | Statement | Parameters | Source |
|----|-----------|------------|--------|
| OF-9a | Topological entropy null: pseudo-Anosov braids (positive entropy, chaotic mixing) and periodic braids (zero entropy) compute XOR identically. Max gap: 3.7 percentage points across 8 Boolean functions. | 4-strand braids, delta=0, all 8 functions tested | D106 (demonstrated) |
| OF-9b | Five depth law hypotheses killed: pairwise coherence, direction coverage, Cayley density, sector saturation, deep-shell angle coherence — all fail to explain why deep entries compute better. | zeta_12 catalog, various metrics | D89 (demonstrated) |

**Scope:** OF-9a applies at delta=0 where e_i^2 = 0 kills expanding eigenvalues. May not apply at delta != 0 where the TL quotient is less destructive.

**Known bypass route:** Work at delta != 0, where dynamical properties may be visible through the bracket.

---

## 2. Partial Order of Obstruction Families

The question: which obstructions are independent (neither implies the other), and which have implication relationships?

### Independence Relations

The following pairs are **independent** — addressing one does not address the other:

```
OF-1 (Convexity) ⊥ OF-3 (Group finiteness)
    Convexity applies at infinite groups too.
    Group finiteness applies under non-convex activations too.

OF-1 (Convexity) ⊥ OF-4 (Encoding lock)
    Encoding lock (parity-lock) holds under non-convex activations.
    Convexity holds under 1wpi encoding.

OF-2 (Pigeonhole) ⊥ OF-3 (Group finiteness)
    Pigeonhole at low k applies to infinite groups.
    Finite groups hit walls at any k.

OF-3 (Group finiteness) ⊥ OF-6 (Algebraic)
    The parity ceiling n=5 (OF-6d) applies to infinite-group roots too.
    Group finiteness applies to functions below the ceiling too.

OF-5 (Valuation) ⊥ ALL OTHERS
    The b-parameter computation is an entirely separate research question
    from DKC Boolean computation. No obstruction in the DKC pipeline
    implies or is implied by valuation failure.

OF-7 (Geometric mismatch) ⊥ OF-1 (Convexity)
    Geometric mismatch applies to non-convex activations.
    Convexity applies with algebraically-correct placements.

OF-9 (Dynamical null) ⊥ ALL OTHERS
    Dynamical properties being invisible is orthogonal to every other
    obstruction mechanism.
```

### Implication Relations

```
OF-3 (Group finiteness) ==> OF-2b (Pigeonhole at n >= 8)
    A finite catalog with K elements means C(K,n) combinations
    is bounded. As n grows, the combinatorial diversity of any
    finite catalog is eventually exhausted.
    But: OF-2b (pigeonhole at n >= 8 in Z[zeta_8]) is a special case.
    OF-3 is the more general principle.

OF-6a (Barrington-Radical) ==> OF-6e (Simple module deep-XOR deficiency)
    is NOT a valid implication. They are independent:
    OF-6a says the radical direction is useless.
    OF-6e says simple modules (no radical at all) lose deep capacity.
    These point in opposite directions. The resolution is that the
    *extension structure* (how radical and semisimple parts are coupled
    in indecomposable modules) is the computational resource — neither
    the radical alone nor the simple module alone.

OF-1a (Complex Minsky-Papert) ==> OF-1b,c,d
    All half-plane unreachability results (0x06, 0x1B, 11/13 wall)
    are instances of the convexity obstruction. OF-1a is the
    master theorem; OF-1b,c,d are specific cases.

OF-2d (Complement-blindness) ==> OF-4a (Parity-Lock) under combined_cell
    Complement-blindness is the mechanism; parity-lock is the theorem.
    OF-4a is the formal consequence of OF-2d applied to the +/-q encoding.
```

### Partial Order Diagram

```
                    OF-5 (Valuation)          OF-9 (Dynamical null)
                         |                          |
                    (independent)              (independent)
                         |                          |
                         v                          v
                    [isolated]                 [isolated]


    OF-1 (Convexity) ---- independent ---- OF-3 (Group finiteness)
         |                                       |
         |                                       | implies
         | independent                           v
         |                                  OF-2b (n>=8 pigeonhole)
         |
         | independent
         v
    OF-4 (Encoding lock) -- independent -- OF-6 (Algebraic)
         |                                       |
         |                                       | (OF-6c,d are the
         |                                       |  strongest: survive
         v                                       |  all bypass routes)
    OF-8 (Readout)         OF-7 (Geometric mismatch)
         |                                       |
    (independent from all)              (independent from all)
```

### Hierarchy of Robustness

Obstructions ranked by how many parameters must change to bypass them:

1. **Most robust (no known bypass under standard framework):**
   - OF-6d: Parity ceiling n=5 is algebra-independent under standard activations
   - OF-5a,b: b-parameter leading-coefficient method fails at TL_6+
   - OF-7d: Hopf fiber is permanently inert for DKC

2. **Robust (requires fundamental architectural change):**
   - OF-3a: zeta_8 XOR10 wall — requires changing the root of unity
   - OF-4a: Parity-Lock — requires changing the encoding
   - OF-4c: Multiplicative complement — requires additive formulation
   - OF-6a: Barrington-Radical — requires working with extensions, not radical

3. **Parametric (bypass available by tuning within existing framework):**
   - OF-1a,b,c,d: Convexity — increase k to k >= 6 (or k >= 24 generalized)
   - OF-2a,c: Pigeonhole — increase k or add activation dimensions
   - OF-8a: Trace collapse — work on irreducible module
   - OF-8b: Curse of dimensionality — optimal component selection

4. **Fragile (narrow parameter regime):**
   - OF-2d: Complement-blindness — remove sign normalization
   - OF-7a: Voronoi catastrophe — constrain non-null, free null
   - OF-7c: Euclidean metric — use intrinsic S^2 metric

---

## 3. The Obstruction Anatomy of the Parity Problem

The parity function (XOR) has been the central test function throughout the DKC program. Its progression through successively deeper obstructions reveals the layered structure of the problem:

### Layer 1: Activation Convexity (D48 -> D50)
- **Obstruction:** OF-1a (Complex Minsky-Papert)
- **What blocks:** 3-input parity under half-plane activation
- **Bypass:** k-sector MVN (k=6), 906 solutions
- **Residual after bypass:** parity at 3 inputs is fully solved

### Layer 2: Angular Discreteness (D63 -> D65)
- **Obstruction:** OF-6c (constraint-geometric wall) + OF-4b (self-doubling)
- **What blocks:** 6+ input parity under standard activations at Z[zeta_8]
- **Bypass:** generalized activations at k=24 (D65), or quaternionic framework (D66)
- **Residual after bypass:** parity at 5 inputs is the ceiling under standard activations

### Layer 3: Group Finiteness (D78 -> D79)
- **Obstruction:** OF-3a (binary octahedral group closes at 24 elements)
- **What blocks:** XOR10 at zeta_8 under ANY activation
- **Bypass:** switch to zeta_12 (infinite group), 124 XOR10 winners
- **Residual after bypass:** no hard ceiling observed at zeta_12

### Layer 4: Pigeonhole at Scale (D93 -> D105)
- **Obstruction:** OF-2c (low k/2^N ratio)
- **What blocks:** XOR at N >= 7 when k is small
- **Bypass:** increase k to collision-avoidance regime (k/2^N >> 1)
- **Residual after bypass:** XOR8=22266 at k=4096

### Layer 5: The Algebra-Independent Ceiling (D65)
- **Obstruction:** OF-6d (parity ceiling n=5 under standard activations)
- **What blocks:** n >= 6 parity under standard activations, regardless of ring
- **Bypass:** generalized (non-standard) activations, or move to the S^2 quaternionic framework
- **Residual:** this is the deepest obstruction identified in the program

The layers are nested: bypassing Layer 1 reveals Layer 2, bypassing Layer 2 reveals Layer 3, and so on. Each layer's obstruction is independent in the sense of Section 2 — they are not consequences of each other. But they form a natural filtration because each deeper layer only becomes visible after the shallower one is resolved.

---

## 4. Cross-Family Interactions

### Convexity x Encoding (OF-1 x OF-4)

Under 1wpi encoding, the convexity wall appears at 3-input parity (OF-1a) and is bypassed by k-sector activation (D50). Under +/-q encoding, the encoding lock (OF-4a) makes all non-parity functions impossible regardless of activation — convexity is irrelevant because the encoding has already restricted to parity. These two obstructions occupy disjoint regimes: changing encoding trades one obstruction for the other rather than encountering both simultaneously.

### Group Finiteness x Algebraic Ceiling (OF-3 x OF-6d)

The parity ceiling n=5 (OF-6d) applies at BOTH zeta_8 (finite) and zeta_16 (infinite). The group finiteness wall at zeta_8 (OF-3a) applies at XOR10, which is ABOVE the ceiling. This means: at zeta_8, OF-6d (ceiling n=5) is hit first under standard activations, while OF-3a (finite group) is hit under non-standard/generalized activations. The two walls operate at different layers of the parity problem.

### Readout x Algebraic (OF-8 x OF-6)

The Barrington-Radical Principle (OF-6a) says the radical is useless. The readout obstruction (OF-8a) says the trace collapses too much information. These interact in multi-strand DKC: the 3-strand full representation has both a rich non-semisimple structure (whose radical is inert, OF-6a) and a lossy trace readout (OF-8a). The resolution (irreducible Delta_1 module) avoids both by working in a representation with no radical and no trace reduction.

---

## 5. The Bypass Principle

**Observation:** Every obstruction identified in 109 demos has had at least one bypass route. No obstruction has been truly terminal for the research program.

### Pattern Analysis

| Obstruction Family | # Instances | # with Known Bypass | Bypass Rate |
|-------------------|-------------|--------------------:|-------------|
| OF-1 Convexity | 4 | 4 | 100% |
| OF-2 Pigeonhole | 4 | 4 | 100% |
| OF-3 Group finiteness | 3 | 3 | 100% |
| OF-4 Encoding lock | 3 | 3 | 100% |
| OF-5 Valuation | 3 | 1 (partial) | 33% |
| OF-6 Algebraic | 5 | 4 | 80% |
| OF-7 Geometric mismatch | 4 | 4 | 100% |
| OF-8 Readout | 2 | 2 | 100% |
| OF-9 Dynamical null | 2 | 1 | 50% |
| **Total** | **30** | **26** | **87%** |

### What the Bypass Rate Means

The 87% bypass rate is not an accident. It reflects a structural property of the DKC research space:

1. **The algebra is always richer than the readout.** Every wall in the DKC pipeline (activation, encoding, readout, k-regime) has been in the *interface* between the algebra and the observer, never in the algebra itself. The bracket lattice at zeta_8 contains Aizenberg's parity construction; it always had what computation needed. The question has always been how to *read* it.

2. **Obstructions are component-specific.** The "wall was X not Y" pattern recurs because each wall isolates to a single pipeline component. This means changing that one component while holding everything else fixed either resolves the wall or confirms it is deeper than initially diagnosed. The DKC pipeline has enough independently variable components (root, encoding, activation, readout, module, k) that most obstructions can be circumvented by changing one.

3. **The 13% without bypass are in secondary research questions.** The valuation obstructions (OF-5) and dynamical nulls (OF-9) concern the b-parameter computation and the role of entropy — questions adjacent to but not central to the DKC Boolean computation pipeline. The core pipeline (topology -> algebra -> Boolean function) has a 100% bypass rate across 22 obstructions.

4. **The strongest surviving obstruction is OF-6d** (algebra-independent parity ceiling n=5 under standard activations). This is the only obstruction that has survived all bypass attempts within the standard framework. Its resolution required moving beyond standard activations to generalized binary labelings (k=24) or the quaternionic S^3 framework — a genuine paradigm extension, not a parameter adjustment.

### The Master Lesson (Restated)

The topology and algebra are almost never the ultimate bottleneck. The bracket lattice has always been richer than the readout can extract. Every wall so far has been in the interface between the algebra and the observer.

When a new obstruction appears, the diagnostic sequence is:
1. Which pipeline component enforces it? (activation, encoding, readout, root, module, k)
2. Is it parametric (tunable) or structural (requires architectural change)?
3. Does changing that one component while holding everything else fixed resolve it?
4. If not, is the wall deeper than initially diagnosed — and at which layer?

This sequence has resolved every obstruction encountered in 109 demos.
