# Demo 76: Scaling DKC to 8 Inputs

## Summary

XOR8 has **zero solutions** at ζ_8. DKC at ζ_8 is a 6-input structure — the
13-direction Voronoi partition has exactly the right capacity for 6-input
parity and not enough for 8-input parity. The failure is total, not a
near-miss: best accuracy is 50% (random chance).

**Test count: 10/10 pass (Parts A-E)**

---

## 1. XOR8 Enumeration (Part A)

| Metric | 6-input (baseline) | 8-input |
|--------|-------------------|---------|
| Catalog entries per weight set | 3 (triples) | 4 (quadruples) |
| Inputs | 6 (3 × ±1) | 8 (4 × ±1) |
| Masks | 64 | 256 |
| Possible weight sets | C(24,3) = 2,024 | C(24,4) = 10,626 |
| XOR winners | **36** | **0** |
| Hit rate | 1.78% | 0.00% |

**Zero of 10,626 quadruples compute XOR8.** The search is exhaustive —
every possible 4-element subset of the 24-quaternion catalog was tested
against all 256 input masks.

## 2. Failure Analysis (Part B)

### Conflict distribution

For each quadruple, we count "conflict cells" — Voronoi cells that see
both even-parity and odd-parity masks:

| Conflict cells | Quadruples | Fraction |
|---------------|-----------|----------|
| 0 (= XOR8) | 0 | 0.0% |
| 2 | 11 | 0.1% |
| 3 | 178 | 1.7% |
| 4 | 575 | 5.4% |
| 5-8 | 6,060 | 57.0% |
| 9-14 | 3,802 | 35.8% |

The minimum conflict count is 2, achieved by 11 quadruples (0.1%).
Best quadruple: [0,1,2,3].

### Best quadruple accuracy

Using majority vote per cell, the best quadruple achieves **50.0% accuracy**
(128/256) — exactly random chance. Even with only 2 conflict cells, those
cells contain enough masks from both parities to completely destroy the
computation.

## 3. Why 8 Inputs Fail: The Capacity Argument

At 6 inputs: 64 masks / 14 cells ≈ 4.6 masks per cell. Each cell sees a
small set of masks. It is geometrically possible (and happens for 36
triples) for every cell to be parity-pure.

At 8 inputs: 256 masks / 14 cells ≈ 18.3 masks per cell. By pigeonhole,
most cells must see ~18 masks. The probability that ALL masks in a cell
have the same parity drops exponentially with the number of masks per cell.

**The 14-cell Voronoi partition does not have enough resolution to separate
128 even-parity masks from 128 odd-parity masks.** This is a hard geometric
limit, not a failure of weight selection.

## 4. Direction Structure at 8 Inputs (Part C)

The best quadruple [0,1,2,3] uses only **2 of 14 cells**:

| Cell | Masks | Fraction |
|------|-------|----------|
| 0 | 208 | 81.3% |
| 13 (identity) | 48 | 18.8% |

Extreme concentration: 81% of all 256 masks land in a single cell.
Compare 6-input winners, which typically spread across 8-10 cells.

With 4 quaternions per weight set (vs 3), the sum vectors cluster more
tightly because the additional terms push sums toward the average
direction, reducing directional diversity.

## 5. Algebraic Structure Breakdown (Parts D-E)

### Eigenvalue angles

| Inputs | Distinct angles | Character |
|--------|---------------|-----------|
| 6 (3-term sums) | 7 | Discrete, algebraic (0°, 30°, 35.264°, ...) |
| 8 (4-term sums) | **86** | Nearly continuous (7.861°, 9.736°, ...) |

The clean 7-angle quantization that characterized 6-input DKC is
**specific to 3-term sums from the 24-cell**. At 4-term sums, the
algebraic constraints are too weak to enforce discrete angles — the
sum quaternion can explore most of the rotation angle space.

### Magnitudes

| Inputs | Distinct |S| values | Character |
|--------|----------------------|-----------|
| 6 (3-term sums) | 6 (√{0,1,2,3,4,6}) | Clean algebraic |
| 8 (4-term sums) | **28** | Dense, non-algebraic |

The √n quantization also breaks down. Values like 0.4142 (=√2 - 1),
0.7654, 0.8703 do not follow the simple √n pattern.

## 6. Scaling Path: Higher Roots of Unity

DKC at ζ_8 is a 6-input structure. Scaling to larger inputs requires
higher roots of unity, which provide more quaternions, more eigenvector
directions, and finer Voronoi partitions.

From Demo 69 (Clifford Staircase):

| Root | Quaternions | Directions | Voronoi cells |
|------|-----------|-----------|---------------|
| ζ_8 | 24 | 13 | 14 |
| ζ_16 | 7,952 | 3,457 | 3,458 |
| ζ_32 | 32,768+ | 4,096+ | 4,097+ |

At ζ_16 resolution, 256 masks / 3,458 cells ≈ 0.07 masks per cell.
Most cells would see 0 or 1 masks, making parity purity trivial by
pigeonhole. This is the SAME trivialization Demo 69 observed for XOR6
at ζ_16 (90.6% of triples are solutions).

**The open question**: Is there a root of unity that provides ENOUGH
directions for 8-input parity WITHOUT trivializing it? The relationship
input_count ↔ root_of_unity ↔ Voronoi_resolution may follow a predictable
formula. At ζ_8, the "sweet spot" is 2^6 masks / 14 cells ≈ 4.6 per cell.
For 8 inputs, the sweet spot would need ~2^8 / cells ≈ 4.6, giving
cells ≈ 56. Is there a root with ~56 directions?

## 7. Connection to Demo 71 Bandwidth

Demo 71 showed the XOR6 Voronoi labeling has spherical harmonic bandwidth
l = 6, giving 2l + 1 = 13 = exactly the number of directions. For XOR8,
the bandwidth would need to be higher (more angular variation to encode
128/128 parity on the sphere). The required bandwidth constrains which
roots of unity can support which input sizes.

## Cross-Demo Connection

| Demo | What | Key Result |
|------|------|------------|
| 50 | Parity at k=6 | All 13 NPN classes, 906 solutions |
| 66 | Quaternionic DKC | 24 quaternions, 24-cell geometry |
| 67 | Coordinate Zoo | 13 directions, 36 XOR6 solutions |
| 69 | Clifford Staircase | ζ_8 ⊂ ζ_16 nesting, trivialization at 3,457 cells |
| 71 | Spectral DKC | Bandwidth l=6, 2l+1=13 |
| 74 | Computational Invariant | Incomparable invariants, 6 computational types |
| 75 | Binocular DKC | Angle subsumes cell, 2.3× capacity, √n magnitudes |
| **76** | **Scaling to 8** | **XOR8 = 0 solutions. ζ_8 is a 6-input structure.** |

## Compile & Run

```
cc -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion \
   -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings \
   main.c -lm -o demo76
./demo76
```
