## Demo 14: Zero Structure

- **Status**: COMPLETE
- **File**: `demo_14_zero_structure/main.c` (~831 lines)
- **Tests**: 7/7 pass (compiles clean with all strict flags including -Werror)
- **Depends on**: Demo 11 (initial observation of bracket amplitude zeros), Demo 13 (23-knot table + dedup)
- **Feeds into**: (further invariant refinement, classification power analysis)

### Headline
Investigates zeros of |bracket(e^{i*theta})| as a knot invariant: finds that zeros occur only at universal angles (multiples of pi/4), counts take values from {0, 4, 8}, and chiral pairs have perfectly reflected zero positions.

### Key Results
- **Zero counts partition into {0, 4, 8} only** for torus knots T(2,n) — not monotonic with crossing number; pattern is 8,0,0,8,4,4 for n=3,5,7,9,11,13
- **All zeros occur at universal angles**: pi/4 multiples (the delta zeros: pi/4, 3pi/4, 5pi/4, 7pi/4) and pi/8 multiples. Perfectly uniform spacing (ratio=1.00 for all knots)
- **Chiral pairs have reflected zero positions**: theta <-> 2pi-theta, all pairs 100% matched
- **Figure-eight has 0 zeros** with min amplitude 0.021 (amphichiral, bracket never vanishes on unit circle)
- **All composites have 0 zeros** — braid connected sums don't factor like diagram connected sums
- **Braid vs diagram connected sum**: factorization bracket(K1#K2) = bracket(K1)*bracket(K2)/delta does NOT hold for braid concatenation (error ~6.35), confirming braid concatenation != diagram connected sum
- **Clear amplitude gap**: zero-class knots have min_amp < 7.3e-15, nonzero-class have min_amp > 0.021 (gap ratio ~2.85 trillion)
- **Classification power**: zeros separate 61% of pairs (128/210), but 0-zero class is huge (12 knots), leaving 82 indistinguishable pairs

### Theorems/Conjectures
- **Confirmed**: Zero count in {0, 4, 8} for torus knots T(2,n) in this table
- **Confirmed**: Chiral pairs share zero count and have reflected positions
- **Confirmed**: All zeros lie on the pi/8 lattice (subset of delta zeros or their bisections)
- **Confirmed**: Braid concatenation != diagram connected sum (factorization fails)
- **Observed**: Zero count is NOT monotonic with crossing number — cyclic pattern for torus knots
- **Limitation**: Zeros alone cannot distinguish most 0-zero knots (composites, figure-eight, T(2,5), T(2,7) all indistinguishable)

### Data
- 21 unique knots after dedup (T23#fig8 == T(2,5)* removed)
- Zero counts: unknot=4, T(2,3)/T(2,3)*=8, T(2,5)/T(2,5)*=0, T(2,7)/T(2,7)*=0, T(2,9)/T(2,9)*=8, T(2,11)/T(2,11)*=4, T(2,13)/T(2,13)*=4, fig-eight=0, all composites=0
- 4-zero positions: 0.2500, 0.7500, 1.2500, 1.7500 (*pi) — exactly the delta zeros
- 8-zero positions: 0.1250, 0.3750, 0.6250, 0.8750, 1.1250, 1.3750, 1.6250, 1.8750 (*pi) — pi/8 lattice
- Delta zeros: pi/4, 3pi/4, 5pi/4, 7pi/4 (= -2*cos(2*theta) = 0)
- 3 distinct zero count classes across 21 knots
- 1024 sample points, bisection with 50 iterations, threshold 1e-6

### Code Assets
- `bracket_amp()`: evaluate |bracket| at angle theta on unit circle
- `bisect_zero()`: golden-section-like bisection to find precise zero locations (50 iterations)
- `part_a_find_zeros()`: high-resolution zero finder (1024 samples, per-knot zero arrays)
- Knot table infrastructure: `init_knots()` with 22 braids (torus knots T(2,3)-T(2,13) + mirrors, figure-eight, 7 connected sums), `dedup_knots()` using bracket evaluation at two reference angles
- `add_torus_2n()`, `add_connected_sum()`: knot table builder helpers
- Reusable complex arithmetic (`Cx` type), state-sum bracket oracle, union-find — all copied from earlier demos

### Literature Touched
- Kauffman bracket zeros on the unit circle
- Delta = -A^2 - A^{-2} = -2*cos(2*theta) zeros at pi/4 multiples
- Connected sum bracket factorization: bracket(K1#K2) = bracket(K1)*bracket(K2)/delta (diagram level)
- Chiral pair reflection symmetry (A <-> A^{-1} = theta <-> 2pi-theta)

### Open Questions
- Why does the zero count for torus knots follow the cyclic pattern {8,0,0,8,4,4}? Is there a number-theoretic explanation (n mod something)?
- Can zeros distinguish within the large 0-zero class at all, or is additional invariant data always needed?
- The pi/8 lattice for 8-zero knots — is this exactly the 8th roots of unity? What's the algebraic explanation?
- Dedup found T23#fig8 == T(2,5)* — is this a real equivalence or a dedup false positive?
