# Demo 113: Rosetta — the δ = 0 bracket is the determinant

**Date:** 2026-10-03
**Dependencies:** Demo 29 (exact Z[ζ8] arithmetic, the state sum), Demo 35 (magnitudes = determinants, observed)
**Result:** 33/33 tests; all six predictions confirmed, both negative controls fail as they must

## Question

What *is* the set of bracket values the DKC demos use as weights? Every value
demo 29 printed was a single integer times a single 8th root of unity, and
demo 35's magnitudes (Hopf 2, trefoil 3, figure-eight 5) are knot
determinants.

## Predictions (written in the header before the first run)

Classical background, not new mathematics: ⟨L⟩ = (−A³)^w V_L(t) with
t = A⁻⁴. At A = e^{i5π/4}: t = −1, t^{1/2} = A⁻² = −i, |V_L(−1)| = det(L),
and V_L ∈ t^{(c−1)/2} Z[t^{±1}] (c = components). So:

| | prediction | result |
|---|---|---|
| P1 | every bracket at δ = 0 is a monomial n·ζ8^k | 151,559 / 151,559 |
| P2 | \|n\| = det(L), det computed independently (reduced Burau, det(I − B)/(1+t+…+t^{n−1}) at t = −1) | 151,559 / 151,559 |
| P3 | bracket = ζ8^{3ws}·u·det, u = ±1 for odd c, ±i for even c, for one convention s | s = −1: 151,559 / 151,559 (s = +1: 133,941) |
| P4 | bracket = 0 ⇔ det = 0 | 151,559 / 151,559 |
| P5 | a catalog's distinct values are determinant × phase | 100 values over 30 determinants |
| P6 | all of it at 2, 3 and 4 strands | yes (complete enumeration, no caps) |

Catalogs: every braid word on 2 strands up to length 12 (8,191), on 3 up to
8 (87,381), on 4 up to 6 (55,987). Exact arithmetic only (Z[ζ8] for the
bracket, Z[t^{±1}] for Burau); no floating point. Runtime ≈ 9 s.

Negative controls: N1, |Δ(+1)| in place of |Δ(−1)|, matches only 78,513 /
151,559; N2, the textbook convention s = +1 fails on 17,618 braids. The
tests can fail; these did.

## Findings

1. **The DKC weight set is "a determinant and one of eight phases".** At
   δ = 0 the bracket of a braid closure is ζ8^{−3w} · u · det(L), with
   u ∈ {±1} for an odd number of components and u ∈ {±i} for an even
   number. Magnitude is the knot determinant; phase is fixed by the writhe
   and the component parity (and one sign).
2. **The convention is mirrored (s = −1),** as docs note 2 predicted: the
   knotapel crossing convention gives the mirror image, so the writhe
   factor appears as ζ8^{−3w} rather than the textbook (−A³)^w = ζ8^{3w}.
3. **Demo 29's catalog has 100 distinct values, not 64.** Demo 29 stored at
   most MAX_CATALOG = 8,192 braids; enumerating its whole catalog definition
   (2–3 strands, length 1–8, nonzero) gives 100 values over 30 determinants
   (1 to 45). The per-determinant phase sets are printed in Part F.
4. **"δ = 0 detects entanglement" is "split links have determinant 0".**
   P4 holds for every braid: the bracket vanishes exactly when the
   determinant does.

## What this changes

- Statements about the Z[ζ8] "lattice" of bracket values are statements
  about determinants (which magnitudes occur) and writhe classes (which
  phases occur). The values are not a lattice but an eight-pointed star of
  integer multiples.
- Questions the atlas asks about catalog structure can now be asked in
  classical terms, with classical tools (determinants of 2-bridge knots are
  their fraction numerators; det = |Δ(−1)| is computable from any diagram).
- The null models proposed in docs/knotapel.html ("Future experiments")
  become concrete: replace the catalog with {phase × integer} using the
  same phase law but magnitudes drawn independently of any knot, and see
  which DKC results survive.

## Open questions

1. Which determinants occur at each braid length and strand count, and
   which phases accompany each (Part F prints the 2–3 strand table)?
2. Does demo 50's parity-at-six-sectors result survive the star null model
   (same eight directions, knot-independent integer magnitudes)?
3. At other roots (δ ≠ 0) the bracket is V_L at another t; which classical
   evaluation does each DKC root correspond to?
