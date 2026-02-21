# Demo 78: Recursive Scaling + Phase Diagram

## Summary

XOR10 has **ZERO winners** at zeta_8. Both recursive search (extending 6 XOR8
winners, 120 candidates) and exhaustive brute-force (all C(24,5) = 42,504
quintuples) agree: no combination of 5 catalog entries computes XOR10 with
Combined Sec(k) x Vor activation for k = 8, 10, 12, 16.

Best brute-force accuracy: 98.4% at k=10 and k=16 (1008/1024 masks correct).
XOR12 skipped (no XOR10 parents to extend).

**Test count: 9/9 pass (Parts A-E)**

---

## 1. Phase Diagram

| N inputs | 2^N masks | Min cells | Winners | Activation     | Hit rate |
|----------|-----------|-----------|---------|----------------|----------|
| 6        | 64        | 14        | 36      | Voronoi        | 1.779%   |
| 8        | 256       | 112       | 6       | Sec8 x Vor     | 0.057%   |
| 10       | 1024      | ---       | 0       | ---            | ---      |
| 12       | 4096      | ---       | 0       | ---            | ---      |

The pattern: XOR6 -> XOR8 is a 6x/8x jump in both masks and minimum cells.
The capacity ratio (masks/cells) at each level:
- XOR6: 64/14 = 4.6 masks/cell
- XOR8: 256/112 = 2.3 masks/cell
- XOR10 (would need): 1024/??? -- even at k=16 (224 cells), that's 4.6 masks/cell

The 4.6 masks/cell ratio appears again at XOR10's best attempt — the same
ratio where XOR6 barely succeeded. But XOR6 had 36 solutions at this ratio
while XOR10 has zero. The difference: the catalog's structural diversity is
exhausted.

## 2. Brute-Force XOR10 Results (Part B)

| Sectors | Cells | Winners | Best Accuracy |
|---------|-------|---------|---------------|
| k=8     | 112   | 0       | 94.1%         |
| k=10    | 140   | 0       | 98.4%         |
| k=12    | 168   | 0       | 96.9%         |
| k=16    | 224   | 0       | 98.4%         |

The accuracy is **non-monotonic** in k: k=12 (96.9%) is worse than k=10
(98.4%). This echoes Demo 50's finding of non-monotonicity in sector count
(906 at k=6 > 756 at k=7 > 96 at k=8 for XOR4). The pi/4 lattice
incommensurability appears again.

Peak accuracy 98.4% = 1008/1024 = 16 masks misclassified. Whether these 16
form a structured pattern (e.g., a particular Hamming weight or bit-pattern
class) is an open question for a future demo.

## 3. Recursive Structure (Part E)

### XOR8 -> XOR6 confirmation

All 6 XOR8 winners contain exactly 2 XOR6 sub-triples:

| XOR8 winner       | Parent 1    | Shadow 1 | Parent 2    | Shadow 2 |
|--------------------|-------------|----------|-------------|----------|
| [0, 1, 9, 23]     | [1,9,23]    | 0        | [0,9,23]    | 1        |
| [0, 1, 18, 22]    | [1,18,22]   | 0        | [0,18,22]   | 1        |
| [2, 4, 5, 23]     | [2,5,23]    | 4        | [2,4,23]    | 5        |
| [2, 9, 19, 21]    | [2,9,21]    | 19       | [2,9,19]    | 21       |
| [4, 5, 15, 16]    | [5,15,16]   | 4        | [4,15,16]   | 5        |
| [17, 19, 20, 21]  | [17,20,21]  | 19       | [17,19,20]  | 21       |

### Shadow properties (100% consistent):

- **Cell sharing**: 12/12 shadows (100%) share their Voronoi cell with at
  least one parent member
- **Half-angle**: ALL shadows have eigenvalue half-angle = 45 degrees
- **Dominant cells**: 0 (axis-x), 1 (axis-z), 10 (edge-diagonal) — the
  structural skeleton of the 24-cell

### Paired quaternion structure

The shadow pairs are: {0,1}, {4,5}, {19,21}. Each pair:
- Shares the same Voronoi cell (same eigenvector direction)
- Has the same eigenvalue half-angle (45 degrees)
- Differs only in the sign/orientation within that cell

This confirms Demo 77's finding that XOR8 winners are XOR6 winners with a
"complementary quaternion" added — one that occupies the same spatial
direction but provides additional eigenvalue resolution.

## 4. Why XOR10 Fails

Three converging explanations:

**A. Catalog exhaustion.** The zeta_8 catalog has 24 entries but only 13
distinct eigenvector directions and 2 distinct eigenvalue half-angles (45
and 90 degrees). For XOR8, selecting 4 entries from 24 provides enough
combinatorial room. For XOR10, selecting 5 entries exhausts the catalog's
structural diversity — too many entries share properties.

**B. Information-theoretic limit.** The combined activation Sec(k) x Vor
provides k * 14 cells. At k=16, that's 224 cells for 1024 masks (4.6
masks/cell). While this ratio sufficed for XOR6 (36 winners), it fails at
XOR10 because the masks are more numerous AND the catalog's angular
structure can't generate sufficiently diverse sums.

**C. Incommensurability.** The pi/4 angles of the zeta_8 lattice create
systematic coincidences when 5 entries are summed — too many 10-input
masks land in the same cell. The non-monotonicity at k=12 (worse than k=10)
suggests the sector boundaries themselves create destructive interference
at certain k values.

## 5. Implications

The zeta_8 XOR hierarchy is: 6 (Voronoi) -> 8 (Sec8 x Vor) -> WALL.

This means the root of unity ℓ directly constrains the maximum computable
input size. The natural next question: **what does ℓ=12 or ℓ=16 buy?**
Higher roots of unity produce larger catalogs with more eigenvector
directions and eigenvalue angles, which should push the wall higher.

The scaling law (if one exists) might be:
- Max inputs = f(|catalog|, n_directions, n_angles)
- Where |catalog| grows with ℓ, n_directions grows with ℓ, and n_angles
  grows with ℓ

## Cross-Demo Connection

| Demo | What | Key Result |
|------|------|------------|
| 50 | Parity at k=6 | All 13 NPN classes, 906 solutions |
| 66 | Quaternionic DKC | 24 quaternions, 24-cell geometry |
| 67 | Coordinate Zoo | 13 directions, 36 XOR6 solutions |
| 74 | Computational Invariant | Incomparable: bracket vs cell |
| 75 | Binocular DKC | Angle subsumes cell (for sums) |
| 76 | Scaling to 8 | XOR8 = 0 solutions (S2 Voronoi only) |
| 77 | Activation Zoo | XOR8 = 6 solutions (Sec8 x Vor) |
| **78** | **Recursive Scaling** | **XOR10 = 0 solutions. Wall at 10 inputs.** |

## Compile & Run

```
cc -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion \
   -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings \
   main.c -lm -o demo78
./demo78
```
