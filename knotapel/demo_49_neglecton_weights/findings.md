# Demo 49: Neglecton Weights — Findings

**Date:** 2026-02-19
**Dependencies:** Demo 48 (Forward DKC Zoo), Demo 47 (Parity Investigation)
**Track:** D — DKC Extensions
**Result:** 10/12 checks pass (P2 and P3 are informative failures — parity remains unreachable)

## Summary

Tested the neglecton prediction: that radical elements of the Temperley-Lieb algebra at delta=0, extracted via a first-order perturbation of the bracket evaluation, would provide new Z[zeta_8] weight values capable of computing parity (XNOR3). Constructed a dual bracket evaluation that separates standard (loops=1) and neglecton (loops=2) contributions, producing 148 new distinct values from 15,242 previously invisible braids. Combined with the 100 standard values into a 216-value catalog, searched all 2,176,782,336 quartets with split-sigmoid activation.

**Headline result: parity is STILL unreachable.** Zero solutions from 2.18 billion quartets. The 116 genuinely new neglecton values all share the same angles as existing standard catalog entries — they are scalar multiples, not angular novelties. This closes the perturbation approach entirely and identifies the parity wall as an angular obstruction in Z[zeta_8], not a magnitude limitation.

**Surprise finding: Aizenberg's known parity construction uses weights IN Z[zeta_8].** The weights (zeta_8, i, -1) are lattice points. The obstruction is in the activation function (split-sigmoid), not the weight space. This reframes the entire narrative: the wall is not about missing weights but about how outputs are classified.

## 1. The Dual Bracket Evaluation

### The Derivative Trick

At delta=0, the standard Kauffman bracket evaluation discards all smoothing states with loops != 1 (because delta^(loops-1) = 0^(loops-1) = 0 for loops >= 2). The first-order perturbation d(bracket)/d(epsilon) at delta=0 extracts the loops=2 contribution EXACTLY:

```
neglecton(braid) = 4 * Sum_{states with loops=2} A^(a-b)
```

This is exact Cyc8 arithmetic — no numerical perturbation needed. The factor of 4 = d(delta)/d(epsilon)|_{epsilon=0} (verified numerically: 4.000000). Higher-order terms (loops >= 3) vanish to first order.

### Catalog Statistics

```
Total braids enumerated: 87,890 (2-3 strand, length 1-8)
Standard bracket = 0:    17,930 (20.4%)
Nonzero neglecton:       81,006 (92.2%)
Both zero:                2,688 (3.1%)
Previously invisible:    15,242 (std=0, neg!=0)

Standard catalog:  100 distinct Z[zeta_8] values
Neglecton catalog: 148 distinct Z[zeta_8] values
Combined catalog:  216 values (68 std-only, 116 neg-only, 32 both)
```

The neglecton bracket is FAR more prolific than the standard bracket: 92.2% of braids have nonzero neglecton brackets, vs only 79.6% with nonzero standard brackets. 15,242 braids that were completely invisible to the standard evaluation now have nonzero values.

### Magnitude Structure

Neglecton values start at |z| = 4 (the minimum magnitude is 4× the standard minimum of 1). This is a direct consequence of the factor of 4 from d(delta)/d(epsilon). The first 10 neglecton values:

```
(0,0,0,4)   |z|=4      (0,-4,0,0)  |z|=4
(8,0,0,0)   |z|=8      (0,-8,0,0)  |z|=8
(0,0,0,8)   |z|=8      (20,0,0,0)  |z|=20
(0,0,-4,0)  |z|=4      (12,0,0,0)  |z|=12
(0,0,4,0)   |z|=4      (0,0,0,36)  |z|=36
```

Compare standard values starting at |z| = 1: (-zeta_8) = (0,-1,0,0).

## 2. Zero New Angles

The critical test: do neglecton values introduce genuinely new angular positions in the complex plane?

```
Neglecton-only values: 116
Same angle as standard: 116
Genuinely new angles:     0
```

**Every single neglecton-only value has the same angle as some existing standard catalog value.** They are scalar multiples — same direction, bigger magnitude. The factor-of-4 scaling preserves the angular structure of A^(a-b), which is always a power of zeta_8 (an 8th root of unity) multiplied by a real integer.

### Why This Is Definitive

All elements of Z[zeta_8] have the form a + b*zeta_8 + c*zeta_8^2 + d*zeta_8^3 with integer coefficients. The angle of such an element is determined by the RATIO of its components. Both standard and neglecton brackets produce sums of A^(a-b) = (-zeta_8)^(a-b) terms — they differ only in WHICH smoothing states contribute and by a real scale factor. Real scaling does not change angles. Therefore:

- Higher-order perturbation terms (loops=3, 4, ...) would also produce A^(a-b) sums scaled by real factors
- NO order of perturbation can produce genuinely new angular information
- The set of angles expressible by the bracket algebra is ALREADY complete at zeroth order

### MVN Consequence

MVN activation (z/|z|) normalizes to the unit circle, preserving only angle. Since neglecton values add zero new angles, MVN activation with neglecton weights is provably equivalent to MVN with standard weights. The perturbation approach is closed at all orders.

## 3. Exhaustive Search Results

```
Combined catalog: 216 values
Quartets searched: 2,176,782,336  (216^4)
Split-sigmoid threshold: 0.5

Source breakdown:
  Standard-only quartets:        21,381,376 (1.0%)
  Has >= 1 neglecton weight:  2,155,400,960 (99.0%)
  All-neglecton quartets:       479,785,216 (22.0%)

XNOR3 (parity) solutions: 0
Min Hamming distance to parity: 1 (truth table 0x16 = EXACT1)
```

### Per-NPN-Class Results

```
Rank  Canon  Name        Solutions        %   TFreq  RMS(D45)
  1   0x03   AND2'       548,396,594   33.2%    290   0.540
  2   0x0F   BUF         402,276,791   24.3%    143   0.528
  3   0x07   ~A~(BC)     268,566,966   16.2%     73   0.629
  4   0x01   AND3'       197,572,533   12.0%    405   0.601
  5   0x19   3v-0x19      75,739,891    4.6%     54   0.680
  6   0x1E   A^(B|C)      45,385,282    2.7%      0   0.792
  7   0x3C   XOR2         39,198,750    2.4%      0   0.788
  8   0x1B   3v-0x1B      30,381,462    1.8%     26   0.739
  9   0x06   ~A(B^C)      23,094,129    1.4%     30   0.778
 10   0x17   MAJ'         15,587,163    0.9%     10   0.655
 11   0x18   ISOLATE       4,948,054    0.3%      0   0.721
 12   0x16   EXACT1        1,622,729    0.1%      1   0.864
 13   0x69   XNOR3                 0    0.0%      0   0.996

Constants: FALSE=412,600,067  TRUE=111,411,925
```

### Comparison with Demo 48 (100-value standard catalog)

The ranking is identical. All 12 non-parity classes grew proportionally with the 21.8× larger search space:

```
Class     Demo 48      Demo 49     Ratio
AND2'     25,085,227   548,396,594  ~21.9×
BUF       16,711,816   402,276,791  ~24.1×
~A~(BC)   13,574,170   268,566,966  ~19.8×
AND3'     10,108,257   197,572,533  ~19.5×
EXACT1       126,666     1,622,729  ~12.8×
XNOR3             0             0  ∞
```

EXACT1's growth ratio (12.8×) is lower than the average (~21×), consistent with its near-boundary position. But parity's growth remains exactly 0.

### Spearman Correlations

```
solutions vs topo_freq (Demo 45):  rho = +0.7582
solutions vs reverse_RMS (Demo 45): rho = -0.7802
```

Slightly lower than Demo 48's (+0.7830, -0.7912), consistent with the larger catalog diluting the topological signal with non-topological (neglecton) values. The preference ordering is unchanged.

## 4. The Wall at Distance 1

```
Class        MinDist  Solutions
EXACT1           1      1,622,729
~A(B^C)          2     23,094,129
MAJ'             2     15,587,163
A^(B|C)          2     45,385,282
AND3'            3    197,572,533
~A~(BC)          3    268,566,966
3v-0x19          3     75,739,891
AND2'            4    548,396,594
BUF              4    402,276,791
ISOLATE          4      4,948,054
3v-0x1B          4     30,381,462
XOR2             4     39,198,750
XNOR3            -              0
```

The distance ordering from Demo 48 is preserved. EXACT1 (distance 1) is still the last reachable function before parity — one bit flip separates the achievable from the impossible.

## 5. The Angular Obstruction Thesis

The parity wall is not about:
- **Missing weights:** 216 values spanning standard + neglecton, still zero parity
- **Magnitude limitations:** The sigmoid saturates at large magnitudes, but MVN (which normalizes magnitude) doesn't help either
- **Insufficient perturbation order:** All orders produce the same angular structure

The wall IS about:
- **The angular geometry of Z[zeta_8]:** The set of angles achievable by integer-coefficient sums of 8th roots of unity cannot produce the interference pattern that parity requires through split-sigmoid activation
- **The activation function:** Split-sigmoid reads magnitude information and destroys the angular information that Aizenberg's sector-based classifier preserves (see Section 6)

## 6. The Aizenberg Surprise

Aizenberg (2007, 2011) proves that a single multi-valued neuron (MVN) can compute parity for any number of inputs with appropriate complex weights. For 3 inputs, the construction is:

```
w1 = exp(2*pi*i/8)   = zeta_8
w2 = exp(2*pi*i*2/8)  = zeta_8^2 = i
w3 = exp(2*pi*i*4/8)  = zeta_8^4 = -1
bias = 0
```

**These are ALL elements of Z[zeta_8].** The weights (0,1,0,0), (0,0,1,0), and (-1,0,0,0) are in our standard catalog. The lattice contains parity-capable weights.

### Why Didn't Demo 47 Find This?

Demo 47's MVN architecture is fundamentally different from Aizenberg's:
- **Demo 47:** 3 inputs -> complex hidden neurons -> REAL output weights -> sigmoid -> threshold
- **Aizenberg:** 3 inputs -> single complex neuron -> SECTOR-BASED classification (upper half-plane = 1, lower = 0)

The real output layer in Demo 47 destroys the angular information that Aizenberg's sector classifier preserves. Demo 47 also used trained (random-initialized) weights, not exhaustive search — the specific triple (zeta_8, i, -1) would only be found by accident.

### Reframing the Narrative

The parity wall from Demos 48-49 is real but its interpretation must be refined:

1. **The lattice Z[zeta_8] DOES contain parity-capable weights** — Aizenberg's construction proves this
2. **The wall is in the activation function**, not the weight space
3. Split-sigmoid and the standard bracket evaluation share the same limitation: they read magnitude/real-part information and discard angular structure
4. **The radical/neglecton story** is about the bracket evaluation's information loss (zeroing out loops >= 2 states), which parallels split-sigmoid's information loss (projecting complex values onto real axes)
5. **Sector-based classification** (Aizenberg's approach) reads the ANGLE directly — the one piece of information that both the bracket evaluation and split-sigmoid destroy

### Prediction for Demo 50

Test: take the exact Z[zeta_8] catalog (100 standard values) as weights, compute z = w1*x1 + w2*x2 + w3*x3 + b for all 8 inputs, classify by SECTOR (arg(z) in [0,pi) -> 1, arg(z) in [pi,2pi) -> 0). Count which Boolean functions this computes.

If parity is reachable under sector classification with standard bracket weights: the wall was never in the lattice. It was always in how we read the output.

## 7. Tower Scroll Summary

Demo 49 tested the neglecton prediction by constructing a dual Kauffman bracket evaluation that extracts radical contributions (loops=2 smoothing states) as exact Z[zeta_8] values scaled by factor 4. The 148 new distinct values from 15,242 previously invisible braids, combined with 100 standard values into a 216-value catalog, were exhaustively searched across 2.18 billion quartets. Parity (XNOR3) remains unreachable — zero solutions. The 116 genuinely new neglecton values introduce zero new angles in the complex plane, closing the perturbation approach at all orders: every Cyc8 element produced by any order of bracket perturbation has the same angular structure as the standard catalog. However, Aizenberg's known parity construction uses weights that ARE in Z[zeta_8], revealing that the wall is in the activation function (split-sigmoid), not the weight space. The next demo should test sector-based classification with exact bracket values.

## Architecture

```
Dual Bracket: For each braid, compute BOTH:
  standard(braid)  = Sum_{states: loops=1} A^(a-b)        [exact Cyc8]
  neglecton(braid) = 4 * Sum_{states: loops=2} A^(a-b)    [exact Cyc8]

A = e^{i*5pi/4} = -zeta_8 = (0,-1,0,0) in Z[zeta_8]
delta = -(A^2 + A^{-2}) = 0

Catalog: 216 combined values (100 standard + 148 neglecton, 32 overlap)
Search: 216^4 = 2,176,782,336 quartets (w1, w2, w3, bias)
Neuron: z = w1*x1 + w2*x2 + w3*x3 + b
Activation: split-sigmoid p = h_r*(1-h_i) + (1-h_r)*h_i, threshold 0.5
Runtime: ~120 seconds (single-threaded)
```

## Files

- `main.c` — ~1135 lines, C89, compiles clean with all warning flags
- `findings.md` — this file
- Compile: `cc -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings main.c -lm -o demo49`
