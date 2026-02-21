# Demo 79: zeta_12 Capacity Test

## Summary

**UPDATED**: The initial run found XOR12 = 0, but this was an artifact of
a truncated catalog (256 entries). The ζ₁₂ group is MUCH larger — still
growing past 4096 entries (group closure doubles each round). A follow-up
test with 4096-entry catalog found **50+ XOR12 winners** immediately.

The zeta_12 catalog at full size: **4096+ quaternions** (still growing),
**512 directions**, **43 distinct half-angles**. Even the truncated 256-entry
subset breaks through the zeta_8 wall: XOR10 goes from 0 (zeta_8) to 124
winners (zeta_12).

**Test count: 14/14 pass (Parts A-E) + XOR12 retest**

**Critical finding**: The ζ₈ group is **finite** (24 elements, closes at
round 4 = binary octahedral group). The ζ₁₂ group appears to be **infinite
or very large** (still growing past 4096). This may be the fundamental
difference: finite group → hard capacity ceiling, infinite group → no ceiling.

---

## 1. Catalog Comparison

| Property         | zeta_8      | zeta_12      | Ratio   |
|------------------|-------------|--------------|---------|
| Quaternions      | 24          | 256 (CAPPED) | 10.7x   |
| Directions       | 13          | 64           | 4.9x    |
| Half-angles      | 2 (45, 90)  | 12           | 6x      |
| Voronoi cells    | 14          | 65           | 4.6x    |

### zeta_12 half-angle spectrum

| Degrees | Count |
|---------|-------|
| 0.0     | 1     |
| 29.0    | 8     |
| 30.0    | 24    |
| 41.4    | 16    |
| 49.5    | 42    |
| 51.3    | 16    |
| 60.0    | 12    |
| 64.3    | 44    |
| 75.5    | 16    |
| 77.5    | 43    |
| 82.8    | 8     |
| 90.0    | 26    |

Compare zeta_8: only 45 and 90 degrees. The 12 distinct angles at zeta_12
provide dramatically more eigenvalue resolution for the Sec(k) activation.

## 2. XOR Capacity Ladder

| Level | Masks | Method      | Winners | Min k | Cells | Note         |
|-------|-------|-------------|---------|-------|-------|--------------|
| XOR6  | 64    | Brute force | 1024+   | 1     | 65    | Voronoi-only |
| XOR8  | 256   | Recursive   | 1024+   | 6     | 390   | Sec6 x Vor   |
| XOR10 | 1024  | Recursive   | **124** | 8     | 520   | Sec8 x Vor   |
| XOR12 | 4096  | Recursive*  | 0       | ---   | ---   | (truncated)  |
| XOR12 | 4096  | Retest**    | **50+** | 8     | 4104  | Sec8 x Vor   |

\* = 256-entry truncated catalog. ** = 4096-entry expanded catalog.

Key observations:
- XOR6: Voronoi-only suffices (65 cells for 64 masks). Compare zeta_8 where
  Voronoi-only also sufficed (14 cells for 64 masks, 36 winners).
- XOR8: Needs k=6 sectors minimum (390 cells for 256 masks). Compare zeta_8
  which needed k=8 (112 cells).
- XOR10: **BREAKTHROUGH** — 124 winners at k=8 (520 cells). zeta_8 had ZERO.
- XOR12: **ALSO SOLVABLE** with expanded catalog. 50+ winners found from
  just the first 11 of 124 XOR10 parents. Min k=8 (4104 cells with 513
  Voronoi cells). The 6th quaternion always comes from the expanded catalog
  (index > 256).
- Winner counts capped at MAX_WIN=1024 for XOR6 and XOR8.

## 3. Phase Diagram (Combined with Demo 78)

| Root | N  | 2^N   | Min cells | Winners | Activation |
|------|----|-------|-----------|---------|------------|
| z8   | 6  | 64    | 14        | 36      | Voronoi    |
| z8   | 8  | 256   | 112       | 6       | Sec8xVor   |
| z8   | 10 | 1024  | WALL      | 0       | ---        |
| z12  | 6  | 64    | 65        | 1024+   | Voronoi    |
| z12  | 8  | 256   | 390       | 1024+   | Sec6xVor   |
| z12  | 10 | 1024  | 520       | 124     | Sec8xVor   |
| z12  | 12 | 4096  | 4104      | 50+     | Sec8xVor   |

**The N-2 pattern is WRONG.** ζ₁₂ can do at least XOR12 with expanded catalog.
The real pattern may be **ζ_N → N** (or beyond) — the ζ₁₂ group is so large
that XOR14 and beyond may also be solvable.

Key difference: ζ₈ generates a **finite** group (24 elements, binary octahedral)
while ζ₁₂ generates an apparently **infinite** group (still growing past 4096).
Finite group → hard ceiling. Infinite group → capacity limited only by
computational resources.

## 4. Nesting Verification

zeta_8 and zeta_12 generate **largely different** quaternion groups:
- Only 3/24 zeta_8 quaternions found in zeta_12 catalog (12%)
- Only 2/13 zeta_8 directions found in zeta_12 directions (15%)

These are NOT nested subgroups. The different half-angles (pi/4 vs pi/6)
generate different finite subgroups of SU(2). This means:
- Each root of unity provides an independent "view" of the quaternion sphere
- The capacity gain isn't from "more of the same" but from genuinely
  different algebraic structure
- A union catalog (zeta_8 + zeta_12) might push capacity further

## 5. Why zeta_12 Breaks the zeta_8 Wall

Three factors that explain the XOR10 breakthrough:

**A. More angular diversity.** zeta_12 has 12 distinct half-angles vs 2.
The Sec(k) activation bins sums by rotation angle. With only 2 source
angles (45, 90), 5-quaternion sums have limited angular diversity. With 12
source angles, sums explore a much richer set of rotation angles.

**B. More directional diversity.** 64 directions vs 13. The Voronoi
partition of S2 is 4.6x finer, so sums that coincide under zeta_8's
coarse partition may separate under zeta_12's finer one.

**C. Larger combinatorial space.** C(256,5) = 8.4 billion quintuples vs
C(24,5) = 42,504. Even with recursive search (not exhaustive at XOR10),
the larger catalog provides vastly more candidates.

## 6. The Finite vs Infinite Group Question

The ζ₈ group (binary octahedral, order 48 → 24 up to ±) is **finite**.
The ζ₁₂ group appears **infinite** or very large finite:

| Round | ζ₈ size | ζ₁₂ size |
|-------|---------|----------|
| 0     | 5       | 5        |
| 1     | 15      | 17       |
| 2     | 23      | 51       |
| 3     | 24      | 127      |
| 4     | 24 (closed) | 275  |
| 5     | -       | 564      |
| 6     | -       | 1140     |
| 7     | -       | 2292     |
| 8     | -       | 4096+ (cap) |

ζ₁₂ roughly doubles each round. At 4096 entries: 512 directions, 43
half-angles (vs 13 directions, 4 angles at ζ₈).

This is likely the fundamental explanation for the capacity difference:
- **Finite group (ζ₈)**: Fixed number of quaternions → fixed combinatorial
  space → hard capacity ceiling at XOR8
- **Infinite group (ζ₁₂)**: Unlimited quaternions → unlimited combinatorial
  space → capacity limited only by computational search

The angle π/4 (ζ₈) gives cos(π/4) = √2/2, which generates a finite group
because √2 is algebraic degree 2 and all products close. The angle π/6
(ζ₁₂) gives cos(π/6) = √3/2, which appears to generate an infinite group.
This is a question about the group theory of SU(2): for which angles θ do
rotations by θ around perpendicular axes generate finite subgroups?

## 7. Open Questions

1. **Is the ζ₁₂ group truly infinite?** Or does it close at some very
   large finite order? This is a concrete group theory question.

2. **What is the true capacity of ζ₁₂?** Can it do XOR14, XOR16, ...?
   The recursive approach works but requires building a very large catalog.

3. **Finite group classification**: Which ζ_N give finite SU(2) subgroups?
   The finite subgroups of SU(2) are classified (cyclic, binary dihedral,
   binary tetrahedral/octahedral/icosahedral). Does ζ₈ → binary octahedral?

4. **Union catalogs**: Since ζ₈ and ζ₁₂ are largely non-overlapping,
   could combining them help?

5. **The ζ₈ wall is real, the ζ₁₂ "wall" was not**: This is the key
   insight. The capacity ceiling is determined by group finiteness, not
   by the root of unity per se.

## Cross-Demo Connection

| Demo   | What                  | Key Result                          |
|--------|-----------------------|-------------------------------------|
| 50     | Parity at k=6         | All 13 NPN classes, 906 solutions   |
| 66     | Quaternionic DKC      | 24 quaternions, 24-cell geometry    |
| 67     | Coordinate Zoo        | 13 directions, 36 XOR6 solutions    |
| 74     | Computational Inv.    | Incomparable: bracket vs cell       |
| 75     | Binocular DKC         | Angle subsumes cell (for sums)      |
| 76     | Scaling to 8          | XOR8 = 0 (S2 Voronoi only)         |
| 77     | Activation Zoo        | XOR8 = 6 (Sec8 x Vor)              |
| 78     | Recursive Scaling     | XOR10 = 0. Wall at 10 inputs.       |
| **79** | **zeta_12 Capacity**  | **XOR10=124, XOR12=50+. Finite vs infinite group.**|

## Compile & Run

```
cc -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion \
   -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings \
   main.c -lm -o demo79
./demo79
```
