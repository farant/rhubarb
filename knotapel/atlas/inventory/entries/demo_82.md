## Demo 82: Crossing Depth and Computational Role

- **Status**: COMPLETE
- **File**: `demo_82_crossing_depth/main.c` (~930 lines)
- **Tests**: 17/17 pass
- **Depends on**: Demo 66 (quaternionic DKC, 24-cell geometry), Demo 79 (ζ₁₂ capacity, XOR12 reachable), Demo 81 (logarithmic scaling law, 0.62 XOR per doubling)
- **Feeds into**: Future work on direct deep-entry generation, optimal subset selection for XOR computation, connection between crossing number and computational capacity

### Headline
Quaternion "depth" (birth round in group closure = number of generator multiplications = crossing depth in knot-theory sense) is the fundamental variable governing XOR capacity: max_xor ≈ depth + 6, deep entries are 2× more efficient than shallow ones, and algebraic coherence beats raw vocabulary.

### Key Results
- **Linear depth law**: max_xor ≈ depth + 6. Each unit of crossing depth adds a fixed increment to computational capacity. The logarithmic law from Demo 81 was an artifact of exponential catalog growth per round — taking log of an exponential gives a linear function.
- **Deep entries 2× more efficient**: 564 pure depth-8 entries reach XOR12 with 19 winners; the full mixed catalog needs ~1140 entries to first reach XOR12 (round 6). Deep subset is half the size.
- **Algebraic coherence beats vocabulary**: Strided 564 has maximum spatial coverage (512 dirs, 43 angles — matching the full 4096-entry catalog) yet only reaches XOR10. Deep 564 has fewer directions (476) and fewer angles (19) but reaches XOR12. More vocabulary loses to less vocabulary with more depth.
- **Two-component winner architecture**: every XOR winner = shallow core + deep extensions. Depth-0 entries (generators + identity) appear in every winner at every XOR level. Mean entry depth rises monotonically: 0.52 (XOR6) → 0.63 (XOR8) → 1.00 (XOR10) → 1.98 (XOR12).
- **Deep entries contain effective generators**: Part D shows deep-only subsets (without generators) can compute XOR10 and XOR12. Deep entries encode effective rotation axes from their algebraic structure as many-generator products.
- **Two vocabulary regimes**: depths 0–6 produce a direction explosion (new directions grow roughly exponentially, 2/4/16/24/68/125/268 per depth); depths 7–8 shift to angle refinement (directions saturate at MAX_DIR=512, but angles accelerate: +14 at depth 7, +8 at depth 8).
- **ζ₈ control confirms pattern**: finite group closes at 24 entries with max depth 3; same depth→XOR trend holds but the group ceiling limits both catalog size and achievable depth.

### Theorems/Conjectures
- **Linear Depth Law (DEMONSTRATED)**: max_xor ≈ depth + 6 across all measured transition points (depth 0→XOR6, depth 2→XOR8, depth 4→XOR10, depth 6→XOR12). The Demo 81 logarithmic law is a corollary: catalog grows as ~2^depth, so log₂(catalog) ≈ depth, giving max_xor ≈ 0.62 × log₂(catalog) + const.
- **Algebraic Coherence Dominance (DEMONSTRATED)**: vocabulary (direction count + angle count) is necessary but not sufficient for XOR capacity. Strided 564 achieves the maximum possible vocabulary yet cannot reach XOR12; Deep 564 with strictly less vocabulary does reach XOR12. The determining factor is the shared algebraic structure of deep entries — every depth-8 entry is a product of 8 generators, creating common intermediate factors.
- **Depth IS Crossing Depth (INTERPRETATION)**: Each closure round = one generator multiplication = one crossing in the braid/knot sense. A depth-d quaternion encodes d crossings of the associated knot diagram. The linear capacity law says computational power scales linearly with knot crossing complexity. Consistent with Kuperberg's #P-hardness: the hardness is in generating deep entries (exponential cost), not in their algebraic structure itself (linear gain).
- **Direction Explosion / Angle Refinement Phase Transition (OBSERVED)**: There is a clean phase boundary at depth 6–7. Below: new directions dominate, catalog explores S². Above: directions saturate, angle refinement dominates. The XOR10→XOR12 transition is driven by angle refinement, not new directions.

### Data
- Depth census (depth, count, cumulative): (0,5,5), (1,12,17), (2,34,51), (3,76,127), (4,148,275), (5,289,564), (6,576,1140), (7,1152,2292), (8,1804,4096)
- Catalog growth per round: roughly 2× each round (exponential); round 8 alone is 44% of all entries
- Winner mean depth: XOR6=0.52, XOR8=0.63, XOR10=1.00, XOR12=1.98
- Sample XOR12 winner: indices [0,1,3,6,52,388], depths [0,0,0,1,3,5] (shallow core + progressively deeper harmonics)
- Shallow vs. strided vs. deep at 127 entries: max XOR 8 / 10 / 10
- Shallow vs. strided vs. deep at 564 entries: max XOR 10 / 10 / 12
- Strided 564 vocabulary: 512 dirs, 43 angles (matches full 4096-entry catalog)
- Deep 564 vocabulary: 476 dirs, 19 angles (less than strided, yet 2 XOR levels higher)
- Depth 7 new angles: 14; depth 8 new angles: 8 (angle acceleration at saturation)
- ζ₈ control: 24 entries, 4 rounds, max depth 3, max XOR 8; same depth→XOR pattern confirmed

### Code Assets
- **Group closure with depth tracking**: `g_depth[]` array assigned round index at birth; identity + generators get depth 0; subsequent products get depth = current round number
- **`save_catalog()` / `restore_catalog()`**: full catalog snapshot/restore (quaternions + depths) enabling repeated subset experiments without rebuilding closure
- **Subset construction patterns**: three reusable strategies implemented — shallow (first N entries by natural closure order), strided (every K-th entry from full catalog, deterministic), deep (last N entries, all from deepest round); pattern is copy-and-overwrite `g_cat[]` then set `g_cat_size`
- **`find_capacity()`**: unified capacity measurement driver — builds dirs, counts angles, runs find_xor6 + find_recursive chain up to XOR12; returns `CapResult` struct
- **Winner depth analysis**: per-level depth histogram and min/max/mean computation; iterates over all winner tuples and indexes into `g_depth[]`; checks that mean depth is monotonically non-decreasing across XOR levels
- **Vocabulary-by-depth scan**: single-pass O(N²) scan accumulating unique directions and angles in per-depth buckets; separates direction explosion regime from angle refinement regime
- **`CapResult` struct**: captures dirs, angles, xor6, xor8, xor10, xor12, max_xor in one place; enables clean side-by-side comparisons in summary tables

### Literature Touched
- **Kuperberg (2015)**: #P-hardness of Jones polynomial for non-lattice roots; the linear depth law says the hardness is precisely in generating deep entries (exponential BFS cost) while the algebraic gain is linear — matching #P structure
- **Knot theory / braid groups (Birman 1974, Jones 1985)**: depth = number of generator multiplications = crossing number of braid word; the demo gives computational meaning to crossing depth
- **Aizenberg (2008)**: k-sector MVN activation; the Sec×Vor activation on S² here shows that algebraic coherence (not just angular coverage) determines computational capacity
- **Nazer & Gastpar (2011)**: compute-and-forward / structured codes; the algebraic coherence result (shared intermediate products enabling higher-order parity separation) connects to structured computation in coding theory

### Open Questions
- **What is the optimal depth for a target XOR level?** Pure depth-8 is demonstrated to be better than shallow or strided at matched size, but is pure depth-8 optimal or would a mixed depth (e.g., pure depth-6 or depth-7) achieve XOR12 with even fewer entries?
- **Can deep entries be generated directly?** Rather than building the full BFS closure and taking the last entries, can the algebraic properties of depth-d entries (shared intermediate products, angular refinement structure) be characterized and sampled directly without computing all shallower depths?
- **What algebraic property does strided-564 lack?** It has full vocabulary (512 dirs, 43 angles) but cannot reach XOR12. Deep-564 has less vocabulary but can. The missing property is presumably the shared common-factor structure of deep entries — can this be made precise?
- **Does the depth law hold for other roots of unity?** ζ₁₀ deep entries vs. shallow comparison has not been run. The linear law max_xor ≈ depth + const may have a different constant or break down at a different root.
- **Connection to knot complexity**: a more complexly knotted strand computes more than many simply knotted strands with equal or greater total vocabulary. Does this connect to known results about the computational power of specific knot types (torus knots, hyperbolic knots)?
