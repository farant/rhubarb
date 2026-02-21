# Demo 82: Crossing Depth and Computational Role

## Summary

Each quaternion in the closure catalog is "born" at a specific round (depth).
**Deeper entries are computationally superior**: at matched catalog sizes, pure
depth-8 entries outperform shallow entries. 564 deep entries reach XOR12, while
the full catalog needed ~1140 entries (from Demo 81). Depth is not noise — it
IS crossing depth in the knot-theory sense.

**Test count: 17/17 pass**

---

## 1. Depth Census (Part A)

| Depth | Count | Cumulative | % of Total |
|-------|-------|------------|------------|
| 0     | 5     | 5          | 0.1%       |
| 1     | 12    | 17         | 0.4%       |
| 2     | 34    | 51         | 1.2%       |
| 3     | 76    | 127        | 3.1%       |
| 4     | 148   | 275        | 6.7%       |
| 5     | 289   | 564        | 13.8%      |
| 6     | 576   | 1140       | 27.8%      |
| 7     | 1152  | 2292       | 56.0%      |
| 8     | 1804  | 4096       | 100.0%     |

Growth roughly doubles each round. Round 8 alone produces 44% of all entries.
The group expands fastest at the frontier.

## 2. Winner Depth Analysis (Part B)

| Level | #Win | MinD | MaxD | MeanD | Depth histogram |
|-------|------|------|------|-------|-----------------|
| XOR6  | 32   | 0    | 2    | 0.5   | d0:66 d1:10 d2:20 |
| XOR8  | 32   | 0    | 3    | 0.6   | d0:96 d1:2 d2:16 d3:14 |
| XOR10 | 32   | 0    | 5    | 1.0   | d0:96 d1:32 d3:10 d4:11 d5:11 |
| XOR12 | 32   | 0    | 8    | 2.0   | d0:96 d1:32 d3:2 d4:19 d5:15 d6:9 d7:15 d8:4 |

Key observations:

1. **All levels have MinD=0**: depth-0 entries (generators + identity) appear
   in EVERY winner at EVERY level. The generators are irreplaceable.

2. **Depth-0 dominates**: 66/96 of XOR6 entries, 96/192 of XOR12 entries.
   Each winner always begins with shallow entries as its "core."

3. **Mean depth rises**: 0.52 → 0.63 → 1.00 → 1.98 from XOR6 to XOR12.
   Higher XOR levels recruit progressively deeper entries.

4. **XOR12 is the first level to use depth-8 entries** (4 entries total
   across 32 winners).

5. **Sample XOR12 winner**: [0,1,3,6,52,388] with depths [0,0,0,1,3,5].
   Pattern: shallow core + progressively deeper "harmonics."

The structure of every winner is: **generators + deep extensions**. The
generators provide the fundamental axes; the deep entries provide the
fine angular resolution needed for higher-order parity separation.

## 3. Vocabulary by Depth (Part C)

| Depth | NewDirs | NewAng | CumDirs | CumAng |
|-------|---------|--------|---------|--------|
| 0     | 2       | 2      | 2       | 2      |
| 1     | 4       | 2      | 6       | 4      |
| 2     | 16      | 2      | 22      | 6      |
| 3     | 24      | 4      | 46      | 10     |
| 4     | 68      | 2      | 114     | 12     |
| 5     | 125     | 4      | 239     | 16     |
| 6     | 268     | 5      | 507     | 21     |
| 7     | 5       | 14     | 512     | 35     |
| 8     | 0       | 8      | 512     | 43     |

Two distinct regimes:

**Depths 0-6: Direction explosion.** New directions grow roughly exponentially
(2, 4, 16, 24, 68, 125, 268). The group is exploring new parts of S².
Angles grow slowly (2-5 new per depth).

**Depths 7-8: Angle refinement.** Directions saturate at 512 (MAX_DIR).
But angles accelerate: depth 7 adds 14 new angles, depth 8 adds 8 more.
Deeper entries don't find new parts of S² — they find new ROTATION AMOUNTS
at already-explored directions.

This is the angular resolution bottleneck identified in Demo 81. The
transition from XOR10 to XOR12 requires not more directions but finer
angular discrimination. Deep entries provide exactly this.

## 4. Shallow vs Strided vs Deep (Part D) — THE CRUX

| Subset      | Size | Dirs | Ang | XOR6 | XOR8 | XOR10 | XOR12 | Max |
|-------------|------|------|-----|------|------|-------|-------|-----|
| Shallow 127 | 127  | 46   | 10  | 32   | 32   | 0     | 0     | 8   |
| Strided 127 | 127  | 125  | 37  | 32   | 32   | 26    | 0     | 10  |
| Deep 127    | 127  | 119  | 19  | 32   | 32   | 32    | 0     | 10  |
| Shallow 564 | 564  | 239  | 16  | 32   | 32   | 32    | 0     | 10  |
| Strided 564 | 564  | 512  | 43  | 32   | 32   | 32    | 0     | 10  |
| Deep 564    | 564  | 476  | 19  | 32   | 32   | 32    | 19    | 12  |

("Strided" = every Nth entry from the full 4096 catalog, spanning all depths
proportionally. Deterministic, evenly-spaced sample.)

### Finding 1: Deep entries BEAT shallow at matched sizes

- At 127 entries: Deep reaches XOR10, Shallow only XOR8 (+2 XOR levels)
- At 564 entries: Deep reaches XOR12, Shallow only XOR10 (+2 XOR levels)

This is a controlled experiment: same number of entries, different depths.
Depth-8 entries are strictly better per-entry than depth 0-3 or 0-5 entries.

### Finding 2: Deep 564 reaches XOR12 earlier than the full catalog

Demo 81 showed XOR12 first appears at ~1140 entries (round 6 of the full
closure). But 564 pure depth-8 entries already reach XOR12 with 19 winners.
The deep entries are **2x more efficient** than the natural closure order.

### Finding 3: Vocabulary alone doesn't explain it — algebraic coherence does

Strided 564 has the MOST vocabulary (512 dirs, 43 angles — matching the full
catalog!) yet doesn't reach XOR12. Deep 564 has FEWER directions (476) and
FEWER angles (19) but reaches XOR12 anyway.

This is the most important comparison in the demo:
- **Strided 564**: maximum spatial coverage, maximum angular diversity → XOR10
- **Deep 564**: less coverage, less diversity, more depth → **XOR12**

More vocabulary LOSES to less vocabulary with more depth. This means:

1. Spatial coverage (directions) is necessary but not sufficient
2. Angular diversity is not sufficient
3. The **algebraic relationships** between deep entries — the fact that they
   share common generator factors, are related by the group multiplication
   structure — is what enables higher-order XOR separation
4. Deep entries have **algebraic coherence** that random/strided samples lack

A strided sample grabs entries from all depths, destroying the algebraic
relationships. A deep sample preserves them: every depth-8 entry is a product
of 8 generators, and they share intermediate products as common factors.
This shared algebraic structure creates the right interference patterns
for parity separation.

### Finding 4: Direction count is necessary but not sufficient

| Comparison | Dirs | Ang | Max XOR |
|------------|------|-----|---------|
| Shallow 127 | 46 | 10 | 8 |
| Deep 127 | 119 | 19 | 10 |
| Strided 127 | 125 | 37 | 10 |

More directions help (46→119 enables XOR10). But strided 127 has more
directions (125) AND more angles (37) than deep 127 (119 dirs, 19 ang)
yet achieves the same max XOR. The algebraic structure matters beyond
raw vocabulary.

## 5. The Two-Component Architecture of Winners

From Parts B and D together, the picture is:

**Every winner = shallow core + deep extensions**

- The shallow core (generators, depth 0-1) provides the fundamental
  rotation axes. These are ALWAYS present in winners.
- The deep extensions (depth 3+) provide the fine angular resolution
  needed for higher-order parity separation.

But Part D shows that deep entries ALONE (without generators!) can
compute XOR10 and even XOR12. This means deep entries contain their own
"effective generators" — rotation axes that emerge from the algebraic
structure of many-generator products.

The generators are useful but not irreplaceable. What matters is the
algebraic richness of the entries, which increases with depth.

## 6. zeta_8 Control (Part E)

| Property | Value |
|----------|-------|
| Catalog size | 24 |
| Rounds | 4 (5+10+8+1) |
| Max XOR | 8 |
| XOR6 mean depth | 0.45 |
| XOR8 mean depth | 0.73 |

Same pattern as zeta_12: higher XOR levels use deeper entries. But the
group closes at 24, so "depth 3" is the maximum (only 1 entry at that
depth). The finite group limits both the catalog size AND the depth.

## 7. Reinterpretation of the Scaling Law

Demo 81 found: max_xor ≈ 0.62 × log₂(catalog_size) + 4.6

**Demo 82 reveals this is an artifact of exponential growth per round.**

The transitions from Demo 81 happen at specific DEPTHS (closure rounds):

| Depth (round) | Catalog size | Max XOR | Delta |
|---------------|-------------|---------|-------|
| 0             | 5           | 6       | —     |
| 2             | 51          | 8       | +2    |
| 4             | 275         | 10      | +2    |
| 6             | 1140        | 12      | +2    |

Each +2 closure rounds adds +2 to max XOR. The relationship is LINEAR:

    **max_xor ≈ depth + 6**

Or equivalently: **capacity scales linearly with depth, not logarithmically
with catalog size.** The logarithmic appearance was because the catalog
grows exponentially per round (~2x). Taking the log of an exponential
gives a linear function — the "log scaling law" was just an artifact of
measuring the wrong variable.

This is a much cleaner and more interpretable relationship:
- Each unit of crossing depth (one more round of generator multiplication)
  adds a fixed increment to computational capacity
- The COST is exponential (each round doubles the catalog) but the GAIN
  is linear in depth
- This is consistent with Kuperberg's #P-hardness: the hardness is in
  generating the deep entries, not in the algebraic structure itself

Furthermore, Demo 82 Part D shows that with pure-deep entry selection:
- XOR12 is reachable at ~564 entries instead of ~1140
- Deep entries are **2x more efficient** than the natural closure mix

The fundamental scaling variable is DEPTH (crossing complexity), not
catalog size. Catalog size is just the cost of reaching a given depth
via group closure.

## 8. Open Questions

1. **What is the optimal depth for a given XOR level?** Is pure depth-8
   the best, or would a specific depth (e.g., depth 6) be even better?

2. **Can deep entries be generated directly?** Instead of building the
   full closure and taking the last entries, can we characterize the
   algebraic properties that make depth-8 entries computationally rich?

3. **What makes the strided sample fail at XOR12?** It has full
   vocabulary but can't reach XOR12. What algebraic property is it
   missing that the deep subset has?

4. **Does this hold for other roots of unity?** Would zeta_10 deep
   entries similarly outperform shallow ones?

5. **Connection to knot complexity**: depth = number of R-matrix
   multiplications = number of crossings in the knot diagram. A more
   complexly knotted strand computes more than many simply knotted
   strands, even if the simple ones collectively cover more of the
   frame space. Does this connect to Kuperberg's #P-hardness?

## Cross-Demo Connection

| Demo   | What                  | Key Result                              |
|--------|-----------------------|-----------------------------------------|
| 66     | Quaternionic DKC      | 24 quaternions, 24-cell geometry        |
| 77     | Activation Zoo        | XOR8 = 6 (ζ₈, Sec8 x Vor)              |
| 78     | Recursive Scaling     | XOR10 = 0 at ζ₈. Wall confirmed.       |
| 79     | ζ₁₂ Capacity          | XOR10=124, XOR12=50+ (wall was artifact)|
| 80     | Finiteness Survey     | Only ζ₄ and ζ₈ finite                   |
| 81     | Scaling Law           | Logarithmic: 0.62 XOR per doubling      |
| **82** | **Crossing Depth**    | **Deep entries 2x more efficient. Depth IS computational richness.** |

## Compile & Run

```
cc -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion \
   -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings \
   -O2 main.c -lm -o demo82
./demo82
```
