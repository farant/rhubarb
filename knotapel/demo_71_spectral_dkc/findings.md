# Demo 71: Spectral DKC — Spherical Harmonic Decomposition

## Summary

Decomposed the DKC computation on S² into spherical harmonics Y_lm.
The minimum bandwidth for XOR6 is **l=6**, and this is explained by a
clean degrees-of-freedom argument: mode l has 2l+1 components, and
2×6+1 = **13 = number of eigenvector directions**. The sharp threshold
(0% at l=5, 100% at l=6) is a phase transition at the DOF boundary.
The eigenvector Voronoi achieves 3.5x super-Nyquist compression because
the XOR6 labeling is binary at known positions — compressed sensing on S².

**Test count: 10/10 pass (Parts A–H)**

---

## 1. Spherical Harmonic Engine (Part A)

Real spherical harmonics Y_lm with associated Legendre polynomials via
stable three-term recurrence. Gauss-Legendre quadrature (64 theta × 128
phi = 8,192 grid points) for numerical integration on S².

| Test | Result |
|------|--------|
| Y_00 constant | Error = 0.00e+00 |
| Orthonormality (l ≤ 6, 1225 pairs) | Max error = 6.82e-14 |
| Round-trip (4 coefficients) | Max error = 8.38e-15 |

## 2. Eigenvector Distribution Spectrum (Part B)

13 undirected directions represented as von Mises-Fisher density
(kappa=50). Only even modes have power (odd modes ~zero from antipodal
symmetry). Dominated by l=0 (51.6%) and l=8 (35.8%).

The direction density has very high bandwidth because the directions
are point-like (high kappa). This is the spectrum of WHERE the directions
are, not what the computation does with them.

## 3. XOR6 Labeling Function Spectrum (Part C)

For each winning triple, the labeling function maps S² → {+1, -1} via
Voronoi cells. The spectrum of this function reveals the angular
frequency content of the computation.

| Mode l | Power fraction | Note |
|--------|---------------|------|
| 0 | 2.0% | DC (mean value) |
| 2 | 0.02% | Negligible |
| 4 | 6.0% | Secondary |
| **6** | **83.9%** | **DOMINANT** |
| 8 | 0.35% | Small |
| 10 | 0.43% | Small |
| 12 | 7.3% | Tertiary |

Odd modes are exactly zero (antipodal symmetry of the Voronoi partition).
The computation is overwhelmingly concentrated in mode l=6.

## 4. Spectral Universality (Part D)

Across all 36 winning triples:

| Mode l | Average | Min | Max | Spread |
|--------|---------|-----|-----|--------|
| 0 | 1.6% | 0.5% | 2.0% | 1.5% |
| 2 | 4.1% | 0.02% | 17.6% | 17.6% |
| 4 | 6.0% | 5.6% | 6.3% | 0.7% |
| **6** | **80.7%** | **70.0%** | **83.9%** | **13.9%** |
| 12 | 6.4% | 3.6% | 7.3% | 3.7% |

The spectrum is **universal**: all winners have the same shape, with
~80% of power in l=6. The variation is mainly in l=2 (some winners
distribute more power there) but l=6 dominance is consistent.

## 5. Minimum Bandwidth: l=6 (Part E — THE HEADLINE)

| l_cut | Winners recovered | Fraction |
|-------|------------------|----------|
| 0–5 | 0 | 0% |
| **6** | **36** | **100%** |
| 7–12 | 36 | 100% |

**Sharp phase transition**: 0% → 100% at exactly l=6.

| Metric | Value |
|--------|-------|
| Minimum bandwidth | l = 6 |
| Nyquist cell count | (6+1)² = 49 |
| Actual cell count | 14 |
| Compression ratio | 3.5× |

## 6. The 13 = 13 Theorem (Part H)

**Mode l has (2l+1) independent spherical harmonic components.**

| l | 2l+1 | ≥ 13? |
|---|------|-------|
| 4 | 9 | No |
| 5 | 11 | No |
| **6** | **13** | **Yes (exact match)** |
| 7 | 15 | Yes |

The minimum bandwidth is the lowest l where a SINGLE mode has enough
independent components to separately address all 13 directions.
2×6+1 = 13 = number of eigenvector directions. **The bandwidth of DKC
on S² is determined by the number of eigenvector directions. Full stop.**

This explains the sharp threshold: at l=5 you have 11 components trying
to control 13 regions (underdetermined), at l=6 you have exactly 13
(determined). There is no partial recovery because the problem is
all-or-nothing at the DOF boundary.

## 7. Super-Nyquist = Compressed Sensing (Part H)

Why 14 cells work where Nyquist says 49:

1. **Nyquist assumes generic functions**: (l+1)² = 49 samples needed
   for an arbitrary l=6 function on S²
2. **XOR6 is not generic**: it's a BINARY function at 13 KNOWN positions
3. **Information content**: 13 bits (one per direction), not 13 real
   coefficients
4. **Compressed sensing**: sparsity (binary) + known support
   (eigenvector directions) = massive compression

The Voronoi cells don't need to generically sample S² — they only need
to distinguish the 13 directions from each other.

## 8. Musical Connection (Part F)

The mapping 1200/l cents gives:

| Mode l | Cents | Note | Eigenvalue angle match? |
|--------|-------|------|------------------------|
| 2 | 600 | F# | 90° (tritone) |
| 3 | 400 | E | 60° (major 3rd) |
| 4 | 300 | Eb | 45° (minor 3rd) |
| **6** | **200** | **D** | **(whole tone)** |

The computation lives at l=6 (D, whole tone = 200 cents), NOT at the
eigenvalue-resonant modes l=2,3,4. The XOR6 labeling requires HIGHER
angular frequency than the eigenvalue structure itself provides.

The eigenvalue angles give the notes (C, Eb, E, F#). The computation
on those notes requires the whole tone mode — one step finer than the
tritone that defines the eigenvalue range.

## 9. Trivialization Prediction

For ζ_16 with 3,457 directions: predicted bandwidth l ≥ 1728
(2×1728+1 = 3,457). Nyquist cells: 1,729² = 2,989,441. At such
enormous bandwidth, ANY coarse partition resolves XOR6 — explaining
the ~90% solution rate observed in Demo 69.

## Cross-Demo Connection

| Demo | What | Key Result |
|------|------|------------|
| 67 | Eigenvector S² | 13 directions, 36 XOR6 solutions |
| 68 | Stereographic R² | Visualization; intrinsically curved |
| 69 | Clifford Staircase | ζ_8 nests in ζ_16; finer roots trivialize |
| 70 | Musica Nodorum | 4 notes C-Eb-E-F#; YB = identical melodies |
| **71** | **Spectral DKC** | **Bandwidth l=6; 2l+1=13=directions; compressed sensing** |

The spectral decomposition ties together the entire S² arc: the
13-direction structure from Demo 67 is the CAUSE of the l=6 bandwidth,
the trivialization from Demo 69 is PREDICTED by the DOF formula, and
the musical notes from Demo 70 sit below the computation's frequency.

## Compile & Run

```
cc -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion \
   -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings \
   main.c -lm -o demo71
./demo71
```
