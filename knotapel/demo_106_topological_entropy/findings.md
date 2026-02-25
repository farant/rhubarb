# Demo 106: Topological Entropy is Orthogonal to DKC Computation

## Prediction (Registered Before Computation)

**Hypothesis:** XOR-computing braids should be pseudo-Anosov (positive topological
entropy). Non-computing braids should be periodic/reducible (spectral radius = 1).

**Theoretical basis:** Fadell-Neuwirth 1962 (braids = trajectories of non-colliding
particles), Boyland-Aref-Stremler 2000 (pseudo-Anosov braids force chaotic mixing).
If chaotic mixing in configuration space corresponds to computational capacity in DKC,
then XOR-computing braids should have positive topological entropy.

**Result: FALSIFIED.** Both periodic and pseudo-Anosov braids compute XOR at 100%
participation rate with nearly identical mean scores (periodic: 3969, pA: 3857).

## Setup

- Module: W_{4,2} (4-strand, dim 3, non-semisimple, XOR14=70 from D100)
- Dual BFS: TL representation (3x3 over Z[zeta_8]) + reduced Burau at t=-1 (3x3 integer, SL(3,Z))
- Catalog: 32768 entries (BFS to depth 8, hit cap)
- Entropy classification: exact integer arithmetic on characteristic polynomial
  - p(1) != 0 => always pseudo-Anosov (real root outside unit circle or complex pair with modulus > 1)
  - p(1) = 0: check quadratic factor x^2 + (1-tr)x + 1; |1-tr| > 2 => pA, else periodic
- All Burau entries verified det=1 (SL(3,Z))

## Key Findings

### 1. Prediction Falsified: Entropy Does Not Predict XOR Capacity

Contingency table (first 128 entries, k=128):

| Class         | Total | XOR>0 | XOR=0 | Rate  | Mean Score |
|---------------|-------|-------|-------|-------|------------|
| Identity      | 1     | 1     | 0     | 100%  | 4574       |
| Periodic      | 88    | 88    | 0     | 100%  | 3969       |
| Pseudo-Anosov | 39    | 39    | 0     | 100%  | 3857       |

Both classes compute XOR at 100% participation with nearly identical scores.

### 2. Total Boolean Function Null

Entropy classification is orthogonal to ALL 8 Boolean functions tested.
+/- encoding (3 weights -> 8 input patterns), k=128:

| Function | Periodic Rate | pA Rate | Gap    |
|----------|--------------|---------|--------|
| AND3     | 92.0%        | 92.9%   | +0.9pp |
| OR3      | 95.1%        | 93.4%   | -1.7pp |
| XOR3     | 86.9%        | 86.0%   | -0.9pp |
| MAJ3     | 86.3%        | 87.5%   | +1.2pp |
| NAND3    | 92.0%        | 92.9%   | +0.9pp |
| NOR3     | 95.1%        | 93.4%   | -1.7pp |
| AND2     | 88.4%        | 87.9%   | -0.5pp |
| XOR2     | 90.8%        | 87.1%   | -3.7pp |

Maximum gap: 3.7pp (XOR2), well within noise for C(89,3)=113564 vs C(39,3)=9139
triple counts. No function shows a meaningful entropy-class effect.

### 3. Fibonacci Pseudo-Anosov at Depth 2

All 8 depth-2 pseudo-Anosov entries share the same characteristic polynomial
[-1, 4, -4] (i.e., x^3 - 4x^2 + 4x - 1) with spectral radius:

    phi^2 = (3 + sqrt(5)) / 2 = 2.618034...

This gives entropy per crossing = log(phi) = 0.481212..., the theoretical optimum
for mixing braids (Band & Boyland 2007). These are Fibonacci pseudo-Anosov braids —
the simplest possible chaotic braids on 4 strands.

### 4. Pseudo-Anosov Fraction Increases with Depth

| Depth | Total  | Periodic | pA     | pA%   |
|-------|--------|----------|--------|-------|
| 0     | 1      | 0        | 0      | —     |
| 1     | 6      | 6        | 0      | 0.0%  |
| 2     | 26     | 18       | 8      | 30.8% |
| 3     | 98     | 66       | 32     | 32.7% |
| 4     | 338    | 192      | 146    | 43.2% |
| 5     | 1110   | 548      | 562    | 50.6% |
| 6     | 3444   | 1210     | 2234   | 64.9% |
| 7     | 10390  | 2974     | 7416   | 71.4% |
| 8     | 17355  | 3952     | 13403  | 77.2% |

Consistent with the Caruso 2013 genericity theorem: pseudo-Anosov braids become
generic at higher word lengths. The crossover from minority to majority occurs at
depth 5 (50.6%).

### 5. Entropy Per Crossing Decreases with Depth

Mean entropy/crossing for pA entries (first 128): 0.4476
- Depth 2: 0.4812 (= log(phi), optimal)
- Depth 3: 0.4390
- Deeper: decreasing further

Deeper braids grow entropy sublinearly with word length — they are "less efficiently
chaotic" per generator than the minimal Fibonacci braids.

### 6. Main Conclusion

**DKC computation is a property of the REPRESENTATION (TL algebra at the root of
unity zeta_8), not of the INPUT (braid word's dynamical complexity).**

The Temperley-Lieb quotient at delta=0 (A = e^{i*5pi/4}) completely erases the
braid group's dynamical information (Thurston-Nielsen classification, topological
entropy, spectral radius of Burau representation) while preserving the algebraic
structure needed for Boolean computation.

This is because:
- The TL algebra is a quotient of the braid group algebra by the relation
  e_i^2 = delta * e_i. At delta=0, this kills all idempotent squares.
- The Kauffman bracket (= TL representation) factors through this quotient.
- The Burau representation does NOT factor through TL — it retains dynamical
  information that the bracket discards.
- Therefore: bracket values (which is what DKC uses) cannot see entropy.

### 7. Implication for Cross-Stratum Mixing

Cross-stratum mixing (D95) is ALGEBRAIC (derived series of the finite group image
2O), not DYNAMICAL (pseudo-Anosov stretching/folding). Periodic braids can populate
multiple derived strata of 2O just as effectively as chaotic braids, because stratum
assignment depends on the TL matrix's Cyc8 sign pattern — a purely algebraic property
of the representation, not a dynamical property of the braid.

### 8. Key References

- Band & Boyland 2007, "The Burau estimate for the entropy of a braid" — establishes
  that reduced Burau at t=-1 gives exact topological entropy for 3-braids and optimal
  lower bound for braids on more strands.
- Bigelow 1999 / Krammer 2000 — faithfulness of the Burau representation for B_4.
- Caruso 2013 — genericity of pseudo-Anosov braids at large word length.
- Thurston 1988 (FLP) — classification of surface homeomorphisms.

## Why Writhe Predicts but Entropy Doesn't: The TL Visibility Filter

The asymmetry between writhe (which correlates with DKC output, D83) and entropy
(which does not) has a clean algebraic explanation:

**Writhe is visible in the bracket.** Writhe acts multiplicatively on the Kauffman
bracket as a phase rotation: the unnormalized bracket picks up a factor of (-A^3)^w
where w is the writhe. This means writhe information survives the TL quotient — it's
encoded in the output as a global phase shift. The Jones polynomial normalization
divides this out (which is why D83 showed framing = +2 XOR levels lost), but the
unnormalized bracket that DKC uses preserves it.

**Entropy is invisible in the bracket.** Topological entropy measures the growth rate
of matrix entries during iterated multiplication — the spectral radius of the Burau
representation tracks how fast coordinate values expand under the braid action. But
the TL quotient kills this growth: at delta=0, the generators satisfy e_i^2 = 0
(nilpotent). There are no expanding eigenvalues in the TL algebra. Two braids with
wildly different Burau spectral radii produce identical bracket values once evaluated
at A = -zeta_8.

**Analogy:** Writhe is like total displacement of a random walk (always visible at
the endpoint). Entropy is like step-by-step variance (invisible at the endpoint if
you only see where you ended up).

This explains why D106's null result was inevitable once you understand what the TL
quotient preserves and what it discards.

## D106 and the Relational Computation Thesis

D106's null result is not an isolated negative — it fits a consistent pattern across
the entire DKC research program. The strongest predictors of computational capacity
are always RELATIONAL (how entries combine) or ALGEBRAIC (group-theoretic structure).
Individual entry properties are consistently dead:

**Properties that predict DKC capacity:**
- Derived series stratum membership (D93, D95) — algebraic, group-theoretic
- Cross-stratum mixing in combined cells (D93) — relational, between entries
- Orthogonal frame geometry of activation (D97) — relational, angular separation
- k parameter / activation granularity (D50, D105) — relational, resolution of
  combinatorial space
- Non-semisimplicity of the module (D99-D101) — algebraic, representation-theoretic

**Properties that do NOT predict DKC capacity:**
- Topological entropy / Thurston-Nielsen class (D106) — dynamical, individual
- Spectral radius of Burau matrix (D106) — dynamical, individual
- Writhe of braid word (partially visible but not predictive of XOR success, D83)
- Position in BFS catalog / word length (D82 — capacity scales with depth, but
  individual depth doesn't predict individual success)

D106 sharpens this into a thesis: **DKC computation is a collective algebraic
phenomenon, not an individual dynamical one.** The question is never "is this braid
good?" but "does this collection of braids span the right algebraic structure?"

## Technical Notes

- Reduced Burau at t=-1 for B_4 gives 3x3 matrices in SL(3,Z).
- Classification uses exact integer arithmetic on the characteristic polynomial:
  p(x) = x^3 + c2*x^2 + c1*x - 1 (det=1 always for braid Burau).
  If p(1) = c2+c1 != 0, always pA. If p(1) = 0, check |1-tr(M)| > 2.
- Spectral radius computed by bisection on characteristic polynomial (double precision,
  100 iterations, for entropy-per-crossing analysis only — classification is exact integer).
- Boolean function sweep uses +/- encoding (not 1wpi): for each weight, add +w or -w.
  3 weights give 2^3=8 input patterns. Target functions specified as 8-bit truth tables.

## Status

**COMPLETE.** Clean negative result with strongest possible form (total null across
all Boolean functions). 20 pass, 0 fail.
