# Demo 67: Coordinate System Zoo — SU(2) Projections for DKC

## Summary

Systematically tested every natural SU(2) coordinate system as a DKC activation
for XOR6. The result: the computation lives on **S²** (the rotation axis), not on
S¹ (the trace) or S³ (the full quaternion). A custom 13-direction Voronoi on S²
achieves 36 solutions at only 14 cells — the most efficient activation found.
The Hopf fiber (phase) is **computationally inert**: zero solutions at all resolutions.

**Test count: 18/18 pass (Parts A–F)**

---

## 1. Eigenvector Extraction (Part A)

The 24 quaternions from the SU(2) braid representation (binary octahedral group)
decompose into eigenvector/eigenvalue pairs:

| Component | Count | Values |
|-----------|-------|--------|
| Eigenvalue angles | 4 | 0°, 45° (π/4), 60° (π/3), 90° (π/2) |
| Eigenvector directions (mod ±) | 13 | 3 axis + 6 edge-midpoint + 4 body-diagonal |
| (angle, direction) pairs | 17 | — |
| Quaternions (mod ±) | 24 | — |
| Bracket values | 100 | — |

The quaternion → eigenvector map is 24 → 17 (mild compression). Three axis-aligned
directions (x, y, z) are shared across two eigenvalue angles (45° and 90°), so the
eigenvector projection loses some information. The 4 body-diagonals appear only at
60°, and the 6 edge-midpoints only at 90°.

The trace sanity check confirms: trace(SU(2)) = 2·cos(θ) for all 24 quaternions.

## 2. S² Is the Computational Sweet Spot (Parts B, B2)

**Custom 13-direction Voronoi: 36 solutions at 14 cells.**

Using the 13 actual eigenvector directions as Voronoi centers on S² (plus 1
degenerate cell for zero/identity), the natural partition achieves MORE solutions
with FEWER cells than any other activation:

| Activation | Dim | Cells | Solutions | Sol/Cell |
|-----------|-----|-------|-----------|----------|
| **Custom 13-dir S² Voronoi** | **2D** | **14** | **36** | **2.57** |
| Hopf base 4×4 | 2D | 16 | 30 | 1.88 |
| 24-cell Voronoi (S³) | 4D | 25 | 35 | 1.40 |
| Eigenvector LatLon 4×4 | 2D | 16 | 2 | 0.13 |
| Eigenvalue k=8 | 1D | 9 | 4 | 0.44 |

The Platonic solid Voronois on S² (octahedral = 6 cells, icosahedral = 12 cells,
cuboctahedral = 12 cells) all yield **zero** solutions. The natural polytopal
geometry of S² doesn't work — you need the actual data-derived directions.

Connection across demos:
- Demo 65: complex S¹ needs k=24 sectors (25 cells with zero)
- Demo 66: quaternionic S³ needs 25 cells (24-cell Voronoi)
- **Demo 67: eigenvector S² needs only 14 cells (13-direction Voronoi)**

The 2D projection is the most efficient. The computation lives on a sphere.

## 3. The Hopf Phase Is Computationally Inert (Part D)

The Hopf fibration decomposes S³ → S² with fiber S¹. In Hopf coordinates
(η, ξ₁, ξ₂), η parameterizes the S² base and ξ₁, ξ₂ are phase angles.

| Hopf Activation | Cells | Solutions |
|----------------|-------|-----------|
| Base (η × ξ₁) 4×4 | 16 | 30 |
| Base 6×6 | 36 | 130 |
| Base 8×8 | 64 | **292** |
| Base 4×6 | 24 | 51 |
| **Phase-only (ξ₁) k=4** | **5** | **0** |
| **Phase-only k=8** | **9** | **0** |
| **Phase-only k=12** | **13** | **0** |
| **Phase-only k=16** | **17** | **0** |
| **Phase-only k=24** | **25** | **0** |
| **Phase-only k=32** | **33** | **0** |

The Hopf phase carries **ZERO** computational information at any resolution.
ALL information lives in the S² base. The Hopf fiber is computationally dead.

This is the deepest theoretical result of the demo. The DKC computation
respects the Hopf fibration structure: the base (S²) carries everything,
the fiber (S¹) carries nothing.

## 4. Eigenvalue-Only Activation (Part C)

Partitioning [0, π] into k equal sectors based on eigenvalue angle θ:

| k | Cells | Solutions |
|---|-------|-----------|
| 4 | 5 | 0 |
| 6 | 7 | 0 |
| 8 | 9 | 4 |
| 12 | 13 | 64 |
| 16 | 17 | 76 |
| 24 | 25 | 72 |
| 32 | 33 | 80 |

First solutions appear at k=8. Peak around k=32. At k=24: 72 solutions —
more than the 24-cell Voronoi's 35, but this is 1D information only.

## 5. The Eigenvalue Paradox

The eigenvalue angle θ succeeds (72 solutions at k=24) even though the Hopf
phase ξ₁ fails completely. This is NOT a contradiction: the eigenvalue θ is
NOT the Hopf phase.

For quaternion q = a + bi + cj + dk:
- Eigenvalue angle: θ = arccos(a)
- Hopf phase: ξ₁ = atan2(b, a)

The eigenvalue θ depends on the real part a, which is encoded across BOTH Hopf
coordinates (it determines η jointly with the imaginary part magnitudes). The
bracket (trace = 2cos θ) succeeds because it extracts θ from the combination
of base + fiber, not from the fiber alone.

## 6. Cayley-Klein Is Worst (Part E)

The "native" SU(2) parameterization (|α|, arg(α)) is consistently the weakest:

| Resolution | Cells | Solutions |
|-----------|-------|-----------|
| 2×4 | 8 | 0 |
| 3×8 | 24 | 0 |
| 4×8 | 32 | 8 |
| 6×6 | 36 | 26 |
| 8×8 | 64 | 106 |

First solutions appear at 32 cells — worse than every other 2D system. The
mathematical "native" parameterization is the worst for computation. Native ≠
natural-for-computation.

## 7. High-Resolution Rankings (64 cells)

| System | Solutions at 64 cells | Sol/Cell |
|--------|----------------------|----------|
| Hopf base 8×8 | **292** | **4.56** |
| Eigenvector LatLon 8×8 | 204 | 3.19 |
| Cayley-Klein 8×8 | 106 | 1.66 |

At high resolution, Hopf base dominates. This makes sense: the Hopf base
coordinates (η, ξ₁) are the optimal 2D parameterization of the computationally
relevant S² — they carve up the base sphere in a way that lat/lon on the
eigenvector S² doesn't quite match.

## 8. Theoretical Implication

DKC computation is fundamentally about **rotation axis direction**. The
eigenvalue (how much rotation) is secondary information. The Hopf phase is
inert. The natural computational space for braid/knot data is S², not S¹
or S³.

This suggests a hierarchy:
- S² (rotation axis): **primary** — 14 cells suffice
- θ (rotation amount): **secondary** — improves resolution but not required
- Hopf phase: **inert** — carries no DKC information
- Full quaternion (S³): **redundant** — the extra dimensions beyond S² add cells but not efficiency

## Open Questions

1. **Why 36 vs 35?** The custom 13-dir Voronoi finds 36 solutions while
   the 24-cell Voronoi finds 35. Is the extra solution a genuinely new
   tuple, or is it a boundary effect from the different cell geometry?

2. **Hopf base vs eigenvector geometry:** At high resolution, Hopf base
   (292) beats eigenvector lat/lon (204). Does the Hopf base projection
   align better with the actual computation? What's the relationship
   between Hopf base S² and eigenvector S²?

3. **Can we go below 14 cells on S²?** The Platonic solids (6, 12 cells)
   fail. Is 13+1=14 the true minimum, or could a different 13-subset of
   directions (not necessarily the catalog directions) work with fewer?

4. **XOR7 on S²:** Demo 65 showed XOR7 needs k=127 sectors on S¹.
   Does S² bring this down dramatically, as it did for XOR6 (24→14)?

5. **Physical interpretation:** The rotation axis lives on S² = CP¹ =
   the Bloch sphere of a qubit. Is there a direct connection between DKC
   computation on the Bloch sphere and quantum information theory?

## Compile & Run

```
cc -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion \
   -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings \
   main.c -lm -o demo67
./demo67
```
