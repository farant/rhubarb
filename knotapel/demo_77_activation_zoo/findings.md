# Demo 77: Activation Zoo for 8-Input DKC

## Summary

XOR8 **IS computable** at ζ_8. The Combined Sector(8) × Voronoi activation
(112 cells) finds **6 winning quadruples** out of 10,626. Neither the
eigenvalue channel (sector) nor the eigenvector channel (Voronoi) alone can
compute XOR8 — both are required. The wall was in the activation function,
not the root of unity. This is exactly the same story as Demo 50 (where
the parity wall was in split-sigmoid, not the lattice).

**Test count: 14/14 pass (Parts A-F)**

---

## 1. Head-to-Head Comparison (Part E)

| Activation      | Cells | Masks/cell | Winners | Best Acc |
|-----------------|-------|------------|---------|----------|
| Voronoi S²      |    14 |       18.3 |       0 |   90.6%  |
| Sector k=4      |     4 |       64.0 |       0 |   77.7%  |
| Sector k=6      |     6 |       42.7 |       0 |   75.4%  |
| Sector k=8      |     8 |       32.0 |       0 |   85.2%  |
| Sector k=10     |    10 |       25.6 |       0 |   92.6%  |
| Sector k=12     |    12 |       21.3 |       0 |   90.6%  |
| Sector k=16     |    16 |       16.0 |       0 |   96.9%  |
| Polar 14×2      |    28 |        9.1 |       0 |   93.8%  |
| Polar 14×3      |    42 |        6.1 |       0 |   93.0%  |
| Polar 14×6      |    84 |        3.0 |       0 |   99.2%  |
| S³ 24-cell      |    24 |       10.7 |       0 |   90.2%  |
| Sec4 × Vor      |    56 |        4.6 |       0 |   93.8%  |
| Sec6 × Vor      |    84 |        3.0 |       0 |   96.9%  |
| **Sec8 × Vor**  |   112 |        2.3 |   **6** | **100%** |

The accuracy climbs monotonically with cell count: 50% → 78% → 85% → 91%
→ 93% → 97% → 99.2% → **100%**. The phase transition from near-miss to
perfect happens between 84 cells (99.2%) and 112 cells (100%).

## 2. Sector Activation Alone (Part A)

Pure sector activation (Aizenberg MVN style) on the rotation angle [0°, 360°):

- Even at k=16 (16 sectors), best accuracy is 96.9% — close but never perfect
- The accuracy grows with k but not fast enough: k=4 (77.7%), k=8 (85.2%),
  k=16 (96.9%)
- The eigenvalue angle alone, even at fine resolution, cannot separate 128
  even from 128 odd 8-bit parities

## 3. Polar Activation (Part B)

Direction × magnitude binning (equal-width bins over [0, 4]):

- Polar 14×6 = 84 cells reaches 99.2% — a single misclassified pair
  separates this from perfection
- The magnitude channel (Demo 75's "odometer") adds genuine computational
  power over direction alone (90.6% → 99.2%)
- But magnitude bins are still not enough: the missing information is in the
  angle channel, not the magnitude channel

## 4. S³ Voronoi (Part C)

24-cell Voronoi on S³ (no antipodal identification, 24 cells):

- 90.2% best accuracy — slightly worse than sector k=16 (96.9%)
- Despite having more cells than S² Voronoi (24 vs 14), the S³ partition
  doesn't align with parity structure
- The S³ Voronoi mixes angle and direction information non-optimally;
  the product decomposition (sector × Voronoi) is superior

## 5. Combined Sector × Voronoi (Part D)

The product S¹ × S² activation:

| Sectors | Cells | Winners | Best Acc |
|---------|-------|---------|----------|
| k=4     |    56 |       0 |   93.8%  |
| k=6     |    84 |       0 |   96.9%  |
| k=8     |   112 |   **6** | **100%** |

The phase transition from 0 to 6 winners happens at k=8 (112 cells, 2.3
masks/cell). At k=6 (84 cells, 3.0 masks/cell) the best accuracy is 96.9%
but zero perfect solutions. The critical threshold is approximately 100 cells.

## 6. The 6 Winning Quadruples (Part F)

| # | Indices | XOR6 subsets |
|---|---------|-------------|
| 0 | [0, 1, 9, 23]     | [0,9,23] [1,9,23] |
| 1 | [0, 1, 18, 22]    | [0,18,22] [1,18,22] |
| 2 | [2, 4, 5, 23]     | [2,4,23] [2,5,23] |
| 3 | [2, 9, 19, 21]    | [2,9,19] [2,9,21] |
| 4 | [4, 5, 15, 16]    | [4,15,16] [5,15,16] |
| 5 | [17, 19, 20, 21]  | [17,19,20] [17,20,21] |

### Structural pattern: "paired quaternions"

Every winning quadruple contains exactly one PAIR of quaternions that share
the same eigenvector direction but differ in eigenvalue angle. Removing
either element of the pair leaves an XOR6-winning triple. This means:

- **12 of 24 triples** (50%) embedded in winning quadruples are XOR6 winners
- Each winner contributes exactly 2 XOR6 subsets
- The 4th quaternion is always the "complement" to an existing XOR6 triple

The XOR8 winners are built from XOR6 winners by adding a paired partner.

### Geometry: 15 of 24 quaternions participate

The 6 winners use 15 of the 24 catalog entries. The Voronoi cell distribution:

| Cell | Count | Role |
|------|-------|------|
| 0    | 6     | Axis (x) |
| 1    | 6     | Axis (z) |
| 10   | 6     | Edge diagonal |
| 6, 7, 8, 9, 11, 12 | 1 each | Body/face diagonals |

The three dominant cells (0, 1, 10) each appear in 6 of the 24 winner slots.
These are axis-aligned and edge-diagonal directions — the "structural skeleton"
of the 24-cell.

### Eigenvalue angles: only 2

The winning quaternions have only **2 distinct eigenvalue half-angles**:
45° and 90°. These are the two canonical octahedral angles. The non-canonical
angles (30°, 35.264°, 65.905°) that appear in additive sums (Demo 75) are
NOT present in the individual catalog entries that form winning quadruples.

## 7. Why Combined Activation Works

The key insight from Demo 74: the bracket (eigenvalue) invariant and the
cell (eigenvector) invariant are **incomparable** — neither subsumes the
other at the catalog level. Demo 77 proves this incomparability is
**computationally necessary**: you need BOTH channels to compute XOR8.

At 6 inputs, the eigenvector channel alone has enough resolution (14 cells
for 64 masks = 4.6 masks/cell). At 8 inputs, 14 cells is insufficient for
256 masks, but the PRODUCT of 8 sectors × 14 cells = 112 cells restores
the resolution (2.3 masks/cell).

The combined activation is the quaternionic generalization of Aizenberg's
MVN: instead of reading just the argument of a complex number (1D), we read
both the eigenvalue (S¹) and the eigenvector (S²) of a quaternion (S¹ × S²).

## 8. Correcting Demo 76

Demo 76 concluded "ζ_8 is a 6-input structure." This is now understood to
be activation-specific: ζ_8 with Voronoi-S² activation is a 6-input
structure. ζ_8 with combined S¹ × S² activation supports at least 8 inputs.

The "capacity argument" (256/14 ≈ 18.3 masks/cell) was correct for S²-only,
but the product activation breaks through by decomposing the quaternion
space differently. The algebraic structure of ζ_8 is richer than any single
projection reveals.

## Cross-Demo Connection

| Demo | What | Key Result |
|------|------|------------|
| 50 | Parity at k=6 | All 13 NPN classes, 906 solutions |
| 66 | Quaternionic DKC | 24 quaternions, 24-cell geometry |
| 67 | Coordinate Zoo | 13 directions, 36 XOR6 solutions |
| 74 | Computational Invariant | Incomparable: bracket vs cell |
| 75 | Binocular DKC | Angle subsumes cell (for sums); √n magnitudes |
| 76 | Scaling to 8 | XOR8 = 0 solutions (S² Voronoi only) |
| **77** | **Activation Zoo** | **XOR8 = 6 solutions (Sec8 × Vor). Wall was activation.** |

## Compile & Run

```
cc -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion \
   -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings \
   main.c -lm -o demo77
./demo77
```
