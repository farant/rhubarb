# Demo 72: Spherical Design Test — Are the 13 Directions Optimal?

## Summary

The 13 eigenvector directions are NOT a spherical t-design (t=0 directed).
They fail the design property at l=1, despite the Delsarte bound allowing
up to t=7 for 13 points. The DKC computation (bandwidth l=6 from Demo 71)
works entirely through degrees-of-freedom counting and compressed sensing,
NOT through classical sampling/design optimality. This is a key negative
result: the algebra selects directions for computation, not for integration.

**Test count: 8/8 pass (Parts A–H)**

---

## 1. t-Design Test: t=0 (Part A)

A spherical t-design requires Σ Y_lm(x_i) = 0 for all 1 ≤ l ≤ t.

| l | max |S_lm| | Pass? | Note |
|---|-------------|-------|------|
| 1 | 2.43 | no | Centroid off-center |
| 2 | 5.11e-16 | YES | **Perfect** |
| 3 | 1.11 | no | |
| 4 | 0.576 | no | |
| 5 | 1.15 | no | |
| 6 | 0.121 | no | Relatively small |
| 7 | 2.07 | no | |
| 8 | 4.53 | no | |

The directions fail at l=1 (the centroid is not at the origin), so
t=0 as a directed set. But l=2 passes perfectly (machine precision),
suggesting partial structure at even modes.

## 2. Bandwidth Gap (Part B)

| Metric | Value |
|--------|-------|
| DKC bandwidth (Demo 71) | l = 6 |
| t-design parameter | t = 0 |
| Gap | 6 modes unintegrated |

The computation succeeds despite the directions being a terrible
spherical design. Compressed sensing and the DOF argument (2l+1=13)
do all the work. Classical sampling theory does not apply.

## 3. Even vs Odd Modes (Part C)

| Test | Result | Significance |
|------|--------|-------------|
| Directed even | t_even = 2 | Passes l=2, fails l=4 |
| Directed odd | t_odd = 0 | Fails immediately at l=1 |
| Undirected (26 pts) | t = 3 | Passes l=1,2,3 (odd auto-vanish) |

**Key finding**: The directions are BETTER at sampling even modes,
which is exactly what DKC uses (Demo 71: only even modes have power).

The undirected test (both +dir and -dir, 26 points) achieves t=3
because odd modes vanish by antipodal symmetry. The first genuine
failure is at l=4 (even mode), with residual 1.15.

This means:
- For DKC purposes, the effective design parameter is the EVEN mode
  design level, which is t_even = 2 (just l=2)
- Even this modest even-mode design doesn't reach l=6

## 4. S³ Design: t=1 (Part D)

The 48-element binary octahedral group (both q and -q) on S³:

| Degree | max error | Pass? |
|--------|-----------|-------|
| 1 | 0 (exact) | YES |
| 2 | 0.125 | no |
| 3 | 0 (exact) | no |
| 4 | 0.0625 | no |

Pattern: odd degrees are exactly zero (symmetry), even degrees fail.
The group achieves only t=1 on S³.

For comparison: the 24-cell vertices (binary tetrahedral group, a
subgroup of the binary octahedral) form a 5-design on S³. The full
octahedral group adds 24 more elements that break the design property
at degree 2.

## 5. Delsarte Bounds (Part E)

| t | Min N (Delsarte) | N=13 ≥ min? | Status |
|---|-----------------|-------------|--------|
| 6 | 10 | yes | Possible but NOT achieved |
| 7 | 10 | yes | Possible but NOT achieved |
| 8 | 15 | NO | Impossible with 13 points |

13 points could theoretically support a 7-design on S². The actual
achievement (t=0) is the worst possible for a set of 13 points that
happens to compute XOR6.

## 6. Residual Analysis (Part F)

First failing mode l=1:

| m | S_{1,m} | |S_{1,m}| |
|---|---------|----------|
| -1 | -0.076 | 0.076 |
| 0 | -1.744 | 1.744 |
| +1 | -2.435 | 2.435 |

All three l=1 components are large — the centroid of the 13 directions
is far from the origin. But l=1 is ODD, so it's irrelevant for DKC
(killed by antipodal symmetry).

The effective first failure for DKC is at l=4 (even mode, |S_lm|=0.576).

## 7. The Compressed Sensing Picture (Synthesis)

The picture that emerges from Demos 71+72:

1. **The computation needs l=6** (Demo 71): 2l+1 = 13 DOF = 13 directions
2. **The directions are not optimal for integration** (Demo 72): t=0
3. **But integration isn't what's happening**: the XOR6 labeling is a
   BINARY function at KNOWN positions
4. **What matters is DOF, not sampling**: 13 components in the l=6
   harmonic mode can assign independent binary labels to 13 regions
5. **Compressed sensing fills the gap**: binary at known positions means
   the information content is 13 bits, not the ~50 samples Nyquist demands

The algebra doesn't optimize directions for sampling. It produces
directions with a specific number (13) that happens to match a DOF
threshold. The computation works because of dimensional coincidence,
not geometric optimality.

## 8. Optimized Design Gets Fewer Solutions (Part G)

Stochastic optimization found 13 points with t=6 design residual 0.93
(vs eigenvector residual 2.43 — 2.6x better geometry).

| Directions | Design residual | XOR6 solutions |
|------------|----------------|----------------|
| Eigenvector | 2.43 | **36** |
| Optimized | 0.93 | **4** |

**Better geometry → FEWER solutions.** The algebra provides something
that geometric optimality cannot: the specific angular relationships
between directions that make the Voronoi partition compute XOR6.

## 9. Perturbation Sensitivity (Part H)

| Epsilon | Mean XOR6 | Min | Max | Loss from 36 |
|---------|-----------|-----|-----|-------------|
| 1° | 38.0 | 36 | 44 | -2.0 |
| 5° | 35.8 | 32 | 42 | 0.2 |
| 10° | 11.5 | 1 | 25 | 24.5 |
| 20° | 2.1 | 0 | 8 | 33.9 |

The solutions are **robust to ~5°** perturbation (within Voronoi cell
angular size) but collapse at 10-20°. The algebraic placement is sharp
but not infinitely fragile.

Note: at 1° the mean (38) slightly EXCEEDS the unperturbed count (36).
Small perturbations can create near-miss configurations that happen to
resolve. This suggests the 36 solutions aren't the maximum possible for
all 13-point configurations — they're the count for the specific algebraic
configuration, which is a local (not global) maximum.

## 10. Synthesis: Four-Level Confirmation

| Level | Finding | Demo |
|-------|---------|------|
| Spectral | Bandwidth l=6, 2l+1=13 | 71 |
| Design | t=0, NOT geometrically optimal | 72A-F |
| Comparative | Better geometry → fewer solutions | 72G |
| Perturbative | Sharp at ~5°, collapses at 10-20° | 72H |

The 13 eigenvector directions are simultaneously the **worst possible
choice for integration** (t=0) and the **best available choice for
computation** (36 vs 4 solutions). This anti-correlation between
sampling quality and computational quality is the central result of
Demo 72.

The DKC computation is algebraic, not geometric. The directions aren't
optimal for anything in classical sampling theory. They compute because
the algebra provides exactly 13 directions — enough DOF for l=6 — placed
at specific angles that encode the Boolean function through the Voronoi
partition. Moving them to "better" positions destroys the computation.

This wraps the "why does it work" investigation arc (Demos 66-72):
the answer is algebraic structure (number of directions, specific
angular relationships) rather than geometric optimality (uniform
distribution, spherical design property).

## Cross-Demo Connection

| Demo | What | Key Result |
|------|------|------------|
| 67 | Eigenvector S² | 13 directions, 36 XOR6 solutions |
| 69 | Clifford Staircase | ζ_8 nests in ζ_16; finer roots trivialize |
| 70 | Musica Nodorum | 4 notes; YB = identical melodies |
| 71 | Spectral DKC | Bandwidth l=6; 2l+1=13=directions |
| **72** | **Spherical Design** | **NOT a t-design (t=0); optimal design gets fewer solutions; sharp at ~5°** |

## Compile & Run

```
cc -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion \
   -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings \
   main.c -lm -o demo72
./demo72
```
