# scrutinium.h worklog

## 2026-02-25 — v1 SHIPPED

### Sprint summary
- **Sprint 4A** (component access): universal detectors via partial automorphism decomposition. 14 detectors for degree-2 types (Z[i], Z[ω]), 18 for degree-4 (Z[ζ₈]). Key insight: true_norm ≠ modulus-squared norm when |Gal| > 2; trace and norm are independent dimensions.
- **Sprint 4B** (wrapper macros): 13 SCR_WRAP_* macros reduce adoption boilerplate ~47%. Z[ω] achieves 100% macroization (0 custom wrappers). Generator macros split 3 ways (ITER/GALOIS2/GALOIS4) reflecting genuine algebraic difference between unit iteration and Galois switching.
- **Sprint 4C** (structural signatures): Scr_Signatura type fingerprints partition lattice topology. Brute-force antichain for width (feasible at k≤20). Topology flags: est_catena, est_V, est_diamas.

### Key results
| Type | Unique Partitions | Dups | Altitude | Width | Incomparable | Complementary | Topology |
|------|:-:|:-:|:-:|:-:|:-:|:-:|---|
| Z[i] | 6 | 3 | 2 | 3 | 8 | 2 | V-SHAPE |
| Z[ω] | 5 | 4 | 2 | 3 | 5 | 1 | V-SHAPE |
| Z[ζ₈] | 11 | 2 | 3 | 7 | 35 | 9 | V-SHAPE + DIAMOND |

### Discoveries
1. Universal V-shape: galois ∥ unit_orbits for generic data in all number rings
2. Subfield diamond visible in Z[ζ₈] via partial norms (Q ⊂ intermediate fields ⊂ Q(ζ₈))
3. Norm ≠ true norm for |Gal| > 2
4. False "galois refines unit" artifact caught and corrected via improved test data
5. Z[ω] has most redundancy (4/9 dups), Z[ζ₈] least (2/13) — partial automorphisms create independent structure

### Test coverage
230 tests total: 100 base + 64 gauss + 38 eisenstein + 28 cyc8. All passing.

### v2 roadmap (deferred)
1. Stability analysis (structural vs data-dependent refinements)
2. Detector composition (meet/join as composite detectors)
3. Ring homomorphisms (cross-ring comparison)
4. New types: Z[√2], Z[ζ₁₂], Z[√-5]
5. Arena allocation via piscina
6. Incremental analysis
