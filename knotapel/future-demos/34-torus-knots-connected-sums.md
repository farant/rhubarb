# Torus Knots and Connected Sums

**Track:** B — Pipeline Composition
**Status:** Proposed
**Smaragda ID:** 01KHM3RSS3D9FNPACCHYJTXJDJ
**Dependencies:** Demo 01 (oracle), Demo 05 (spin chain), Demo 07 (braid_to_pd), Demo 08 (pd_to_braid)

## Hypothesis

Torus knots T(p,q) have a beautiful closed-form braid representation: the braid word (sigma_1 sigma_2 ... sigma_{p-1})^q on p strands. The Kauffman bracket of torus knots also has known closed forms. This demo extends the knot table beyond the 3 test knots (trefoil=T(2,3), Hopf=T(2,2)) to include T(2,5) (5_1 knot), T(2,7) (7_1 knot), T(3,4), T(3,5), and verifies the full pipeline for each. Also tests connected sums (trefoil # trefoil) via braid concatenation.

## Approach

Generate torus knot braids programmatically. Compute brackets via both state-sum and matrix pipeline. Compare against known values from the knot table. Implement connected sum at the braid level (concatenation with strand count adjustment). Test that bracket of connected sum = product of individual brackets (times a loop factor).
