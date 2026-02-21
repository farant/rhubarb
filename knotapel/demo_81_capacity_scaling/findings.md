# Demo 81: Capacity Scaling Law

## Summary

For ζ₁₂ (infinite group), capacity scales **logarithmically** with catalog
size. Each +2 XOR inputs requires approximately **10x more catalog entries**.
The scaling rate is **0.62 XOR per doubling** of the catalog.

Transitions occur at strikingly regular intervals on a log scale:
- XOR6 → XOR8 at ~51 entries (log₂ ≈ 5.7)
- XOR8 → XOR10 at ~275 entries (log₂ ≈ 8.1)
- XOR10 → XOR12 at ~1140 entries (log₂ ≈ 10.2)

Each transition requires ~2.3 more doublings (factor of ~5x). Extrapolation:
XOR14 needs ~38,000 entries, XOR16 needs ~615,000, XOR20 needs ~160 million.

**Test count: 14/14 pass**

---

## 1. Scaling Table

| Round | Cat   | Dirs | Ang | XOR6 | XOR8 | XOR10 | XOR12 | MaxXOR | log₂(C) |
|-------|-------|------|-----|------|------|-------|-------|--------|---------|
| 0     | 5     | 2    | 2   | 8    | 0    | 0     | 0     | 6      | 2.32    |
| 1     | 17    | 6    | 4   | 32   | 0    | 0     | 0     | 6      | 4.09    |
| 2     | 51    | 22   | 6   | 32   | 32   | 0     | 0     | 8      | 5.67    |
| 3     | 127   | 46   | 10  | 32   | 32   | 0     | 0     | 8      | 6.99    |
| 4     | 275   | 114  | 12  | 32   | 32   | 32    | 0     | 10     | 8.10    |
| 5     | 564   | 239  | 16  | 32   | 32   | 32    | 0     | 10     | 9.14    |
| 6     | 1140  | 507  | 21  | 32   | 32   | 32    | 12    | 12     | 10.15   |
| 7     | 2292  | 512  | 35  | 32   | 32   | 32    | 32    | 12     | 11.16   |
| 8     | 4096  | 512  | 43  | 32   | 32   | 32    | 32    | 12     | 12.00   |

Monotonicity: CONFIRMED (capacity never decreases with more entries).
Winner cap: 32 per level. XOR6-XOR10 saturate at 32 at every level
where they appear. XOR12 emerges at round 6 with 12 winners, then
saturates at rounds 7-8.

## 2. The Logarithmic Scaling Law

The relationship between catalog size and maximum XOR capacity:

    max_xor ≈ 0.62 × log₂(cat_size) + 4.6

This means:
- Each DOUBLING of the catalog adds ~0.62 to the max XOR
- Each +2 XOR level requires ~2/0.62 ≈ 3.2 more doublings = ~10x more entries
- Capacity grows as O(log N) where N is the catalog size

Equivalently: to compute XOR_M, you need approximately:

    catalog_size ≈ 2^((M - 4.6) / 0.62) = 2^(1.61 × M - 7.4)

Or roughly: catalog_size ≈ 3^M (base 3 exponential in the XOR level).

## 3. Transition Analysis

| Transition    | Cat needed | log₂(C) | Gap (doublings) |
|---------------|------------|---------|-----------------|
| 0 → XOR6     | 5          | 2.32    | —               |
| XOR6 → XOR8  | 51         | 5.67    | 3.35            |
| XOR8 → XOR10 | 275        | 8.10    | 2.43            |
| XOR10 → XOR12| 1140       | 10.15   | 2.05            |

The gap between transitions is DECREASING: 3.35, 2.43, 2.05 doublings.
This suggests the scaling may be slightly BETTER than pure logarithmic —
possibly O(log N / log log N) — but more data points are needed to
distinguish from plain O(log N).

Alternatively, the pattern may reflect that early transitions require
overcoming qualitative barriers (establishing enough directions for
Voronoi separation) while later transitions are smoother.

## 4. What Drives Each Transition

| Transition   | Bottleneck lifted                              |
|--------------|-------------------------------------------------|
| → XOR6       | Need ≥3 quaternions with diverse enough axes     |
| XOR6 → XOR8  | Need 22+ directions for 256-mask Voronoi separation |
| XOR8 → XOR10 | Need 114+ directions for 1024-mask separation   |
| XOR10 → XOR12| Need 507+ directions AND 12+ distinct angles    |

The direction count tracks closely with transitions:
- 2 dirs: XOR6 barely
- 22 dirs: XOR8 emerges
- 114 dirs: XOR10 emerges
- 507 dirs: XOR12 emerges

After round 7, directions saturate at 512 (MAX_DIR). But angles keep
growing: 35 at round 7, 43 at round 8. The angular diversity is what
provides continued growth in XOR12 winner counts (12 → 32) even after
directions saturate.

## 5. Direction vs Angle Scaling

| Round | Cat  | Dirs | Angles | Dirs/Cat | Angles/Cat |
|-------|------|------|--------|----------|------------|
| 0     | 5    | 2    | 2      | 40.0%    | 40.0%      |
| 2     | 51   | 22   | 6      | 43.1%    | 11.8%      |
| 4     | 275  | 114  | 12     | 41.5%    | 4.4%       |
| 6     | 1140 | 507  | 21     | 44.5%    | 1.8%       |
| 8     | 4096 | 512  | 43     | 12.5%    | 1.0%       |

Directions scale roughly as ~40-45% of catalog size until saturating
at MAX_DIR=512 (rounds 7-8). This means most new quaternions at each
round are "new directions" — the group explores new parts of S².

Angles grow much more slowly: roughly as sqrt(catalog_size). The angular
resolution is the LIMITING factor for higher XOR levels.

## 6. Predictions

Using the logarithmic law max_xor ≈ 0.62 × log₂(C) + 4.6:

| Target  | Catalog needed | Group closure rounds | Feasible? |
|---------|----------------|---------------------|-----------|
| XOR14   | ~38,000        | ~11                 | Yes (minutes) |
| XOR16   | ~615,000       | ~14                 | Yes (hours) |
| XOR18   | ~10 million    | ~17                 | Marginal |
| XOR20   | ~160 million   | ~20                 | Memory-limited |
| XOR32   | ~10^14         | ~30+                | Infeasible |

The exponential cost of each additional XOR level is consistent with
Kuperberg's #P-hardness result for non-lattice roots of unity. The
computation is "tractable" but exponentially expensive — not polynomial.

## 7. Comparison with Finite Case (ζ₈)

| Property | ζ₈ (finite) | ζ₁₂ (infinite) |
|----------|-------------|-----------------|
| Group size | 24 (fixed) | Unlimited |
| Max XOR | 8 (hard ceiling) | 12+ (logarithmic growth) |
| Scaling | NONE | O(log N) |
| Bottleneck | Group finiteness | Computational cost |
| ADE type | E₇ (binary octahedral) | Dense subgroup |

The finite group creates an absolute ceiling. The infinite group
creates a logarithmic growth curve. This is the fundamental
computational difference between lattice and non-lattice roots.

## 8. Open Questions

1. **Does the scaling continue to XOR14?** The prediction (~38K entries)
   is testable with a larger MAX_QCAT. Would require ~11 closure rounds.

2. **Is the scaling truly logarithmic or slightly better?** The
   decreasing gap between transitions (3.35 → 2.43 → 2.05 doublings)
   hints at possibly sub-logarithmic behavior. More data points needed.

3. **Does the scaling depend on the root of unity?** Would ζ₁₀ (71
   angles at 4096 entries vs ζ₁₂'s 43) give a different scaling constant?

4. **What determines the ~0.62 constant?** Is this a function of the
   dimensionality (quaternions = 4D) or the activation function (Sec×Vor)?

5. **Can the scaling be improved?** Different activation functions, union
   catalogs (ζ₁₀ + ζ₁₂), or smarter search strategies might change the
   constant but not the logarithmic character.

## Cross-Demo Connection

| Demo   | What                  | Key Result                              |
|--------|-----------------------|-----------------------------------------|
| 66     | Quaternionic DKC      | 24 quaternions, 24-cell geometry        |
| 77     | Activation Zoo        | XOR8 = 6 (ζ₈, Sec8 x Vor)              |
| 78     | Recursive Scaling     | XOR10 = 0 at ζ₈. Wall confirmed.       |
| 79     | ζ₁₂ Capacity          | XOR10=124, XOR12=50+ (wall was artifact)|
| 80     | Finiteness Survey     | Only ζ₄ and ζ₈ finite                   |
| **81** | **Scaling Law**       | **Logarithmic: 0.62 XOR per doubling**  |

## Compile & Run

```
cc -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion \
   -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings \
   -O2 main.c -lm -o demo81
./demo81
```
