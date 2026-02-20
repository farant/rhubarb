## Demo 09: Reidemeister Move Invariance

- **Status**: COMPLETE
- **File**: `demo_09_reidemeister/main.c` (~717 lines)
- **Tests**: 30/30 pass (5 parts: R1 bracket, R1 writhe/Jones, R2, R3 braid, combined)
- **Depends on**: Demo 07 (braid closure → PD), Demo 08 (PD bracket, Vogel algorithm)
- **Feeds into**: Later demos using Jones polynomial invariance as ground truth

### Headline
Proves computationally that the Kauffman bracket satisfies the R1 twist relation and is invariant under R2/R3, and that the Jones polynomial (writhe-normalized bracket) is a true knot invariant under all three Reidemeister moves.

### Key Results
- **R1 (twist/kink)**: bracket(K + kink_s) = -A^{3s} * bracket(K), writhe changes by s, Jones invariant — verified on unknot, trefoil, figure-eight, Hopf link with both positive and negative kinks
- **R2 (poke)**: bracket, writhe, and Jones all unchanged — verified on R2-poked unknot, trefoil, figure-eight, Hopf link
- **R3 (slide)**: tested via braid relation sigma_1*sigma_2*sigma_1 = sigma_2*sigma_1*sigma_2 — bracket and Jones identical for both positive and negative generators, also on 4-strand braids
- **Combined moves**: R1(+) then R1(-) bracket factors cancel; R1+R2 Jones invariant; double R1(+) Jones invariant

### Code Assets
- **`writhe()`**: computes writhe (sum of crossing signs) from PD notation
- **`jones_poly()`**: Jones/f-polynomial = (-A^3)^{-w} * bracket(K) — clean 4-line implementation
- **`apply_r1()`**: adds a positive or negative kink to a specified edge in PD notation — splits edge, creates loop arc, adds crossing
- **`apply_r2()`**: adds an R2 poke (pair of opposite-sign crossings) between two edges — splits both edges, creates intermediate arcs
- **`remap_second()`**: utility to replace second occurrence of an arc label (for edge splitting)
- **`make_r2_unknot()`**: manually constructed 2-crossing unknot (R2 poke of trivial unknot)
- Reuses: Laurent polynomial ring, PD bracket state-sum, braid-to-PD conversion, known PD knots (trefoil, figure-eight, Hopf)

### Literature Touched
- Reidemeister's theorem: any two diagrams of equivalent links are connected by R1, R2, R3 moves
- Kauffman bracket behavior under R1: not invariant (picks up -A^{3s} factor), hence is a "regular isotopy" invariant
- Writhe normalization: (-A^3)^{-w} corrects R1 factor to produce true (ambient isotopy) invariant
- Jones polynomial as writhe-normalized Kauffman bracket

### Open Questions
- R3 is tested only via the braid relation (algebraic equivalence), not via direct PD-level R3 move application — a direct `apply_r3()` on PD crossings would be more complete but is more complex to implement
