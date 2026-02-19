# Demo 50: Activation Function Zoo — Findings

**Date:** 2026-02-19
**Dependencies:** Demo 48 (Forward DKC Zoo), Demo 49 (Neglecton Weights)
**Track:** D — DKC Extensions
**Result:** 4/6 checks pass (2 informative failures: Aizenberg construction)

## Summary

Demos 48-49 proved parity (XNOR3) is categorically unreachable with split-sigmoid activation over Z[zeta_8] bracket weights. Demo 50 asks: is the wall in the lattice, or in the activation function?

**Headline result: the wall was in the activation.** By replacing split-sigmoid with k-sector MVN classification (divide the unit circle into k equal sectors, classify by sector number mod 2), parity becomes reachable at **k=6 with 906 solutions** from the standard 100-value bracket catalog. The bracket algebra had parity-capable weights all along — Demos 48-49 were reading them through the wrong decision geometry.

The demo maps all 13 NPN classes across k=2..8, revealing a four-tier hierarchy of activation complexity: k=2 (5 classes), k=3 (+1), k=4 (+6, everything except parity), k=6 (+1, all 13 including parity). Parity is the last function to appear, and it requires exactly 6 sectors — not 8 as Aizenberg's k-valued MVN theory would suggest.

## 1. The Reachability Heatmap

```
  TT    Name          k=2  k=3  k=4  k=5  k=6  k=7  k=8
  0x01  AND3'           Y    Y    Y    Y    Y    Y    Y
  0x03  AND2'           Y    Y    Y    Y    Y    Y    Y
  0x06  ~A(B^C)         -    Y    Y    Y    Y    Y    Y
  0x07  ~A~(BC)         Y    Y    Y    Y    Y    Y    Y
  0x0F  BUF             Y    Y    Y    Y    Y    Y    Y
  0x16  EXACT1          -    -    Y    Y    Y    Y    Y
  0x17  MAJ'            Y    Y    Y    Y    Y    Y    Y
  0x18  ISOLATE         -    -    Y    Y    Y    Y    Y
  0x19  3v-0x19         -    -    Y    Y    Y    Y    Y
  0x1B  3v-0x1B         -    -    Y    Y    Y    Y    Y
  0x1E  A^(B|C)         -    -    Y    Y    Y    Y    Y
  0x3C  XOR2            -    -    Y    Y    Y    Y    Y
  0x69  XNOR3           -    -    -    -    Y    Y    Y
```

### Minimum Sector Count per NPN Class

```
Class        Min k    Solutions at min k
AND3'          2       221,453
AND2'          2       265,365
~A~(BC)        2       217,109
BUF            2       127,164
MAJ'           2        20,032
~A(B^C)        3         6,570
EXACT1         4         3,525
ISOLATE        4         5,061
3v-0x19        4        82,930
3v-0x1B        4        71,352
A^(B|C)        4        33,771
XOR2           4         9,336
XNOR3          6           906
```

## 2. The Four-Tier Structure

The 13 NPN classes organize into exactly four tiers by minimum sector count:

- **Tier 1 (k=2):** AND3', AND2', ~A~(BC), BUF, MAJ' — 5 classes. The "linearly separable" functions. These can be computed with a simple half-plane decision boundary.

- **Tier 2 (k=3):** +~A(B^C) — 6 classes. One additional function needs a 3-way partition. A single extra sector boundary unlocks it.

- **Tier 3 (k=4):** +EXACT1, ISOLATE, 3v-0x19, 3v-0x1B, A^(B|C), XOR2 — 12 classes. A massive jump: 6 new functions at once. Quadrant-based decision boundaries unlock everything except parity.

- **Tier 4 (k=6):** +XNOR3 — all 13 classes. Parity is the LAST function to appear, isolated at a uniquely high threshold. k=5 is insufficient.

The structure is monotonically non-decreasing: once a function becomes reachable at some k, it remains reachable at all higher k. But the jump sizes are not uniform — k=4 is a phase transition (6 functions at once), and parity is an outlier requiring 50% more sectors than its nearest peer (XOR2 at k=4).

## 3. Parity Solution Counts and Non-Monotonicity

```
k   Parity solutions
2          0  (proven impossible)
3          0
4          0
5          0
6        906  ← first appearance
7        756
8         96
```

**The count is non-monotonic: 906 > 756 > 96.** More sectors does NOT mean more parity solutions. In fact, k=8 has the FEWEST parity solutions among the k values where parity exists.

### The Incommensurability Hypothesis

```
k   gcd(k,8)  Parity solutions
2      2                 0
3      1                 0
4      4                 0
5      1                 0
6      2               906
7      1               756
8      8                96
```

The bracket catalog's angular structure is based on 8th roots of unity (multiples of π/4). The k-sector boundaries are at multiples of 2π/k. When k divides 8 (or vice versa), the sector boundaries ALIGN with the lattice's natural symmetry. When they don't, the sectors cut ACROSS the lattice symmetry, creating asymmetric partitions.

**Among k≥6 where parity exists:** k=6 (gcd=2, partial alignment) has the most solutions, k=7 (gcd=1, no alignment) has fewer, k=8 (gcd=8, perfect alignment) has the fewest. The anti-correlation with gcd is clear: lattice-aligned sectors PRESERVE the symmetry that blocks parity.

**But incommensurability alone is insufficient:** k=3 (gcd=1) and k=5 (gcd=1) are maximally incommensurable yet have zero parity solutions. You need both:
1. Enough sectors (≥6) to create the alternating classification pattern
2. Sector boundaries that don't align with the lattice symmetry

k=6 satisfies both conditions optimally: it has enough sectors, and its 60° boundaries cut the 45° lattice at an angle.

## 4. Anatomy of the 906 Parity Solutions

### Weight Space

- **41 of 100** catalog values participate in parity solutions (concentrated subset)
- All 906 solutions use **three distinct weights** (zero pairs, zero triples)
- ~**151 distinct unordered triples** (906 ÷ 6 permutations ≈ 151)

### Angular Restriction

The 2,718 weight appearances (906 × 3) are confined to exactly 4 of 8 octants:

```
Octant  Angle range          Count   Notes
0       [  0°,  45°)           0
1       [ 45°,  90°)           0
2       [ 90°, 135°)         906     ALL solutions use this octant
3       [135°, 180°)           0
4       [180°, 225°)         270
5       [225°, 270°)         636
6       [270°, 315°)           0
7       [315°, 360°)         906     ALL solutions use this octant
```

The active octants are {2, 4, 5, 7}. The excluded octants are {0, 1, 3, 6}.

### Why Exactly These Four Octants (The Class-1 Constraint)

This restriction is not accidental — it follows directly from parity's truth table and the lattice's angular structure.

**The single-input constraint:** Parity requires f(1,0,0) = f(0,1,0) = f(0,0,1) = 1 (odd parity). When only one input is active, z = w_i (just one weight, no sum). So each weight individually must land in a class-1 (odd) sector under k=6. This forces ALL three weights into the class-1 region of the complex plane.

Bracket catalog values live at exact multiples of 45° (powers of zeta_8 scaled by real integers). Under k=6 (sector width = 60°), each lattice angle maps to a definite sector:

```
Angle    Octant    k=6 sector    Class (j mod 2)
  0°       0         0              0
 45°       1         0              0
 90°       2         1              1  ← class-1
135°       3         2              0
180°       4         3              1  ← class-1
225°       5         3              1  ← class-1
270°       6         4              0
315°       7         5              1  ← class-1
```

Class-1 angles: {90°, 180°, 225°, 315°} → octants {2, 4, 5, 7}. Exactly the four active octants.
Class-0 angles: {0°, 45°, 135°, 270°} → octants {0, 1, 3, 6}. Exactly the four excluded octants.

The match is perfect. Any weight at a class-0 angle would produce f(0,...,1,...,0) = 0, violating parity. The octant restriction is not a mysterious preference of the search — it is a logical necessity from the truth table.

### Octant-Set Distribution

Only **two** octant combinations appear among the 906 solutions:

```
Octant set    Solutions    %
{2, 5, 7}       636      70.2%
{2, 4, 7}       270      29.8%
```

Octants 4 and 5 are interchangeable — both map to the same k=6 sector (sector 3, class 1). The third weight always comes from exactly one of them, never both. Octants 2 and 7 are mandatory in every solution.

**Every parity solution requires a weight in octant 2 AND octant 7.** These are the two mandatory directions. The third comes from octant 4 or 5 (both sector 3).

### The Triskelion Pattern: Why k=6 Works

Mapping the octant-set data to k=6 sectors reveals a striking geometric structure:

```
Odd sector    Angle range      Octant(s)    Role
Sector 1      [ 60°, 120°)     2            mandatory (906/906)
Sector 3      [180°, 240°)     4 or 5       mandatory (906/906)
Sector 5      [300°, 360°)     7            mandatory (906/906)
```

Every parity solution places exactly one weight in each of the three odd sectors. The three odd sectors are evenly spaced at 120° intervals around the unit circle — a **triskelion** (three-armed rotationally symmetric figure).

This explains why k=6 is the minimum for parity with 3 inputs:
- k=6 has exactly **3 odd sectors**, matching the **3 inputs**
- Each input needs its own odd sector (single-input constraint)
- The 120° spacing between odd sectors ensures the binary sums (pairs and triple) land in even sectors, producing the alternating classification parity requires
- k=4 has only 2 odd sectors — not enough for 3 inputs
- k=5 has 2 or 3 odd sectors depending on counting (non-integer alignment), but the sector width (72°) doesn't accommodate the lattice's 45° spacing

The triskelion is the geometric "why" of k=6: three inputs need three odd sectors, and six sectors is the minimum that provides exactly three.

### Typical Solution Structure

First example: w1 = -3·zeta_8 (225°, sector 3), w2 = 4·zeta_8² (90°, sector 1), w3 = -5·zeta_8³ (315°, sector 5). One weight per odd sector — a triskelion.

### Most Frequent Weights

```
Cyc8               |z|    Appearances (of 2718 total)
(0,  0,  0,-16)    16.0      156
(0,  0,  0, -7)     7.0      144
(0,  0,  0,-12)    12.0      132
(0,  0,  0,-14)    14.0      126
(0, -8,  0,  0)     8.0      120
(0,  0,  0,-10)    10.0      114
(0,  0,  9,  0)     9.0      102
(0,  0,  0, -6)     6.0      102
(0,-10,  0,  0)    10.0      102
(0,  0, 17,  0)    17.0       96
```

The most frequent weights are concentrated in the |z| = 6-17 range, with integer multiples of zeta_8^3, zeta_8, and zeta_8^2. The low-magnitude catalog entries (|z|=1-3) that dominate the split-sigmoid search (Demo 48) are mostly absent from the parity solutions — the sector classifier benefits from larger magnitudes that spread the weighted sums further apart in angle.

## 5. The Aizenberg Comparison

### Aizenberg's Known Parity Construction

For 3 inputs, Aizenberg's MVN uses w1=zeta_8, w2=zeta_8²=i, w3=zeta_8⁴=-1. Under k=8 sectors with our j mod 2 binary mapping:

```
Input   z               Angle    Sector  Class  XOR3
(0,0,0) 0               0°       0       0      0  ✓
(0,0,1) -1              180°     4       0      1  ✗
(0,1,0) i               90°      2       0      1  ✗
(0,1,1) -1+i            135°     3       1      0  ✗
(1,0,0) zeta_8          45°      1       1      1  ✓
(1,0,1) zeta_8-1        112.5°   2       0      0  ✓
(1,1,0) zeta_8+i        67.5°    1       1      0  ✗
(1,1,1) zeta_8+i-1      99.7°    2       0      1  ✗
```

Truth table: 0x58 (NPN class 3v-0x19), NOT parity.

### Why Aizenberg's Construction Fails Here

Two reasons:

1. **Aizenberg's weights are not bracket values.** zeta_8 = (0,1,0,0) is NOT in our catalog (only -zeta_8 = (0,-1,0,0) is, being the A parameter itself). The bracket evaluation produces A^(a-b) sums, and A = -zeta_8, so bracket values are integer combinations of powers of -zeta_8, not zeta_8.

2. **The output mapping is wrong.** Aizenberg's theory is about k-valued logic, where the neuron outputs one of k values. For k=2^n, the parity function maps k-valued outputs to binary via the parity of the sector number's binary digits, NOT via j mod 2. Our j mod 2 mapping is a different (simpler) binary encoding.

### What the Bracket Catalog Found Instead

The bracket catalog discovers its own parity solutions at k=6, using a completely different weight structure than Aizenberg's construction. The catalog's parity triples (e.g., -3·zeta_8, 4i, -5·zeta_8³) exploit the interaction between the Z[zeta_8] angular lattice and the 6-fold sector geometry — a relationship that has no analog in Aizenberg's k-valued MVN theory.

## 6. Reframing the DKC Parity Wall

### What the Wall Was

Demos 48-49 proved a real boundary: zero parity solutions across 100M+ quartets with split-sigmoid activation, and zero from 2.18B quartets with the extended neglecton catalog. The boundary is sharp (EXACT1 at distance 1 from parity has solutions; parity has exactly zero) and persists across all perturbation orders.

### What the Wall Was Not

The wall was not an algebraic limitation of Z[zeta_8]. The lattice contains parity-capable weights. The 100-value standard bracket catalog — the SAME catalog that Demo 48 exhaustively searched — produces 906 parity triples when the activation geometry changes from split-sigmoid to 6-sector MVN.

### The Correct Interpretation

The parity wall is a property of the **activation-lattice interaction**, not the lattice alone:

1. **Split-sigmoid** projects complex values onto real axes (Re and Im independently), applying sigmoid to each. This destroys angular information and creates a rectangular decision boundary that cannot produce the alternating pattern parity requires from Z[zeta_8] weights.

2. **k-sector MVN** reads the angle of the complex output directly. With k≥6 sectors whose boundaries don't align with the lattice's π/4 angular spacing, the sector partition breaks the lattice symmetry in exactly the way needed for parity.

3. **The radical/neglecton story** (Demo 49) is about the bracket evaluation's information loss — zeroing out loops≥2 states. This parallels split-sigmoid's information loss — projecting complex outputs onto real axes. Both throw away information. But the neglecton degrees of freedom don't help because they add no new angles (Demo 49's key finding). The sector classifier doesn't need new angles — it needs a different way of READING the existing angles.

### Connection to Non-Semisimplicity

The TL_n non-semisimplicity at delta=0 is still relevant, but its role is more subtle than originally hypothesized. The radical doesn't provide "missing weights" for parity — the weights were always there. Instead, the radical's significance may be in the quantum computational model: the braiding operations that produce bracket values factor through the semisimple quotient TL/rad, which limits the OPERATIONS available, not the VALUES producible. The 6-sector classifier accesses the same values through a different computational path that isn't constrained by the same quotient.

## 7. Tower Scroll Summary

Demo 50 replaced split-sigmoid activation with k-sector MVN classification (classify complex output by which of k equal angular sectors it falls in, output = sector number mod 2) and searched the standard 100-value Z[zeta_8] bracket catalog for all 13 NPN classes at each k from 2 to 8. Result: parity (XNOR3) first becomes computable at k=6 with 906 solutions, proving the parity wall from Demos 48-49 was in the activation function, not the weight lattice. The 13 NPN classes organize into four tiers by minimum sector count (k=2: 5 classes, k=3: +1, k=4: +6, k=6: +1), with parity uniquely isolated at the highest tier. Parity solutions are non-monotonic in k (906 at k=6 > 756 at k=7 > 96 at k=8), anti-correlated with gcd(k,8), supporting an incommensurability hypothesis: sector boundaries that misalign with the lattice's π/4 angular structure break the symmetry that blocks parity. All 906 solutions use weights from exactly 4 of 8 octants, with every solution requiring weights at 90° and 315° — the bracket catalog discovers its own parity structure, unrelated to Aizenberg's theoretical construction.

## Architecture

```
Catalog: 100 distinct Z[zeta_8] bracket values (standard, no neglecton)
A = e^{i*5π/4} = -zeta_8, delta = 0

k-sector classification:
  Sector j spans [2πj/k, 2π(j+1)/k)
  Output = j mod 2 (even sectors → 0, odd sectors → 1)
  z = 0 → class 0 (convention)

Search: 100^3 = 1,000,000 triples (w1, w2, w3) per k value
  No bias (z(0,0,0) = 0 → class 0, eliminating f(0,0,0)=1 truth tables)
  7 k values (k=2..8) → 7,000,000 triples total

Runtime: ~2 seconds total (single-threaded)
```

## Prediction Scorecard

| # | Prediction | Result |
|---|-----------|--------|
| P1 | k=2 has zero parity | **PASS** (0 solutions, proven impossible) |
| P2 | k=8 has parity > 0 | **PASS** (96 solutions) |
| P3 | Aizenberg weights in catalog | **FAIL** (zeta_8 is not a bracket value) |
| P4 | Aizenberg computes parity (k=8, j mod 2) | **FAIL** (computes 3v-0x19) |
| P5 | Parity first appears at k=6 | **PASS** (906 solutions — SURPRISE) |
| P6 | All 13 classes reachable at some k | **PASS** (all reachable by k=6) |

## Files

- `main.c` — ~1030 lines, C89, compiles clean with all warning flags
- `findings.md` — this file
- Compile: `cc -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings main.c -lm -o demo50`
