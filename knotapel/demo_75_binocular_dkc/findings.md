# Demo 75: Binocular DKC (Multi-Output)

## Summary

A single braid evaluation produces multiple output channels. The eigenvalue
angle channel **subsumes** the Voronoi cell channel — every angle value maps
to a single parity. Binocular DKC is not stereo vision (two independent
eyes) but a **zoom lens** (one channel at two resolutions). The additive
magnitude |S| is quantized to √{0,1,2,3,4,6} — square roots of small
integers. The multiplicative product channel has high entropy but is
completely blind to XOR6.

**Test count: 23/23 pass (Parts A-I)**

---

## 1. Eigenvalue Quantization (Part A)

Seven distinct eigenvalue angles emerge from the additive sum quaternion:

| Angle | Canonical? | Geometric meaning |
|-------|-----------|-------------------|
| 0.000° | Yes | Identity (null rotation) |
| 30.000° | **No** | π/6 — hexagonal symmetry |
| 35.264° | **No** | arctan(1/√2) — **tetrahedral angle** |
| 45.000° | Yes | π/4 — octahedral face diagonal |
| 60.000° | Yes | π/3 — body diagonal |
| 65.905° | **No** | Complement to tetrahedral angle |
| 90.000° | Yes | π/2 — axis rotation |

Three non-canonical angles arise from quaternion **addition**: summing
unit quaternions from the 24-cell creates directions not present in the
cell itself. These are geometric invariants of the cube/octahedron/
tetrahedron family — the 35.264° tetrahedral angle (angle between cube
edge and body diagonal) is the most striking.

Per winning triple: typically 6 of 7 angles are used, with 45° dominant
(37.5% of inputs for winner[0]).

## 2. Angle Subsumes Cell (Parts B-D)

The eigenvalue angle perfectly predicts parity for ALL 36 winning triples.
Every angle value maps to exactly one parity bit (even or odd). This means:

- The cell channel (1 bit, XOR6) is a **coarsening** of the angle channel
- The angle channel (~2 bits, 6-7 levels) is a **refinement** of the cell
- Knowing the angle tells you the parity; knowing the parity does NOT
  tell you the angle

For winner[0]: 0°, 35.264°, 90° → even parity; 45°, 30°, 60° → odd.

Eight distinct angle functions exist across 36 triples, grouped into 8
families of 3-8 triples each. The cell function is always the same (XOR6).
Binocular ≡ angle (the cell adds no new information beyond what the angle
already encodes).

## 3. Mutual Information (Part C)

| Metric | Value |
|--------|-------|
| Mean MI(angle; cell) | 0.7218 bits |
| Independent triples (MI < 0.01) | 0/36 |
| Triples with MI = 1.0 | 20/36 |
| Range | 0.14 – 1.0 bits |

The channels are NEVER independent. For 20/36 triples, the angle perfectly
determines the cell (MI = 1.0). The remaining 16 triples have lower but
still substantial MI. This is expected: both channels read the same sum
quaternion, just at different resolutions.

## 4. Capacity (Part F)

| Channel | Entropy (bits) |
|---------|---------------|
| Cell (direction) | 1.000 |
| Eigenvalue angle | 2.004 |
| Joint (binocular) | 2.282 |
| Additive bound | 3.004 |
| Mutual information | 0.722 |

**Gain over cell-only: +1.282 bits (2.3×).** The gain comes entirely from
reading 7 angle levels instead of 2 parity levels. Efficiency is 76% of
the additive bound (the 24% loss is the MI between channels).

## 5. Eight-Fold Triple Classification (Part E)

36 winning triples produce only 8 distinct angle functions:

| Group | Triples | First winner | Size |
|-------|---------|-------------|------|
| 0 | 8 | [0,4,19] | Largest |
| 1 | 4 | [0,4,23] | |
| 2 | 4 | [0,9,19] | |
| 3 | 6 | [0,9,23] | |
| 4 | 4 | [2,4,19] | |
| 5 | 3 | [2,4,23] | |
| 6 | 3 | [2,9,19] | |
| 7 | 4 | [2,9,23] | |

Pattern: triples sharing the same "axis pair" (first two quaternion indices
modulo cell type) tend to share angle functions. The 8 groups suggest a
natural classification of winning triples beyond the cell-type structure.

## 6. Additive Magnitude Channel (Part G)

The magnitude |S| of the additive sum is quantized to exactly **6 values**:

| |S| | |S|² | Interpretation |
|-----|------|----------------|
| 0 | 0 | Perfect cancellation |
| 1 | 1 | Single quaternion / balanced |
| √2 | 2 | Two orthogonal |
| √3 | 3 | Three mutually orthogonal |
| 2 | 4 | Two aligned |
| √6 | 6 | Maximum non-trivial alignment |

These are √n for n ∈ {0,1,2,3,4,6}. The missing √5 (and √7, √8, ...)
reflects the inner product structure of the 24-cell: when you sum unit
quaternions from this lattice, |S|² = Σ wᵢ·wⱼ sums inner products drawn
from {-1, -½, 0, ½, 1}, producing only these specific integer sums.

**Magnitude does NOT reliably predict parity**: 16/36 triples (44%).
This confirms the magnitude carries genuinely different information from
the direction. H(|S|) = 1.839 bits, MI(|S|; cell) = 0.7237 bits.

The magnitude is Demo 73's "odometer" — the information the automaton
lost when it could only read direction. The odometer reads in algebraic
numbers, not arbitrary reals.

## 7. Multiplicative Product Channel (Part H)

The quaternion PRODUCT M = Π w_i (selected weights multiplied in order)
produces a unit quaternion with:

- **4 eigenvalue angles** — exactly the canonical set (0°, 45°, 60°, 90°)
- **13 Voronoi cells** — uses ALL cells (vs direction's parity-biased subset)
- **XOR6: 0/36 triples** — completely blind to parity
- **Angle predicts parity: 0/36** — neither channel helps

| Metric | Multiplicative | Additive |
|--------|---------------|----------|
| Eigenvalue angles | 4 (canonical only) | 7 (+ 3 non-canonical) |
| H(cell) | 2.750 bits | 1.000 bits |
| H(angle) | 1.491 bits | 2.004 bits |
| Total entropy | 4.241 bits | 3.004 bits |
| Computes XOR6 | NO | YES |

The multiplicative approach has 41% MORE total entropy but ZERO
computational utility for XOR6. It's a kaleidoscope — rich structure,
computationally blind.

The key structural difference: multiplication preserves the 24-cell
group structure (product of group elements stays in the group), so
eigenvalue angles are restricted to the 4 canonical values. Addition
breaks out of the group, creating 3 new angles from vector combination.
**DKC computation requires breaking the group structure.**

## 8. Channel Comparison (Part I)

MI(additive cell; multiplicative cell) = 0.5174 bits — moderate
correlation. The two "cell" channels share about half their information.

**Complete channel inventory:**

| Channel | H (bits) | XOR6? | Parity? | Nature |
|---------|---------|-------|---------|--------|
| Add cell | 1.000 | 36/36 | 36/36 | Direction → binary |
| Add angle | 2.004 | subsumes | 36/36 | Direction → 7 levels |
| Add |S| | 1.839 | — | 16/36 | Alignment strength |
| Mult cell | 2.750 | 0/36 | 0/36 | Group orbit |
| Mult angle | 1.491 | 0/36 | 0/36 | Group eigenvalue |

## 9. Reconciliation with Demo 74

Demo 74 showed bracket and cell are **incomparable** at the CATALOG level
(individual quaternions): 97.8% of bracket collisions are separated by
cell, and 54% of cell collisions are separated by bracket.

Demo 75 shows the eigenvalue angle **subsumes** cell at the COMPUTATION
level (sums of quaternions): knowing the angle tells you the parity.

These are not contradictory — they concern different mathematical objects:
- Demo 74: individual quaternions from the 24-cell → bracket vs cell
- Demo 75: sums of 24-cell quaternions → angle vs cell

Sums create a finer resolution than individual quaternions because the
sum space is larger (non-unit quaternions with non-canonical angles).

## Cross-Demo Connection

| Demo | What | Key Result |
|------|------|------------|
| 66 | Quaternionic DKC | 24 quaternions, 100 brackets, 24-cell |
| 67 | Coordinate Zoo | 13 directions, 36 XOR6 solutions |
| 70 | Music Space | r = -0.06 (geometric ⊥ computational) |
| 71 | Spectral DKC | Bandwidth l=6; 2l+1=13=directions |
| 72 | Spherical Design | NOT a t-design; algebra > geometry |
| 73 | Automaton | NOT an automaton (82.8%); additive, compass w/o odometer |
| 74 | Computational Invariant | Incomparable invariants; 6 types; 119 combined |
| **75** | **Binocular DKC** | **Angle subsumes cell; √n magnitudes; mult blind; 2.3× capacity** |

## Compile & Run

```
cc -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion \
   -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings \
   main.c -lm -o demo75
./demo75
```
