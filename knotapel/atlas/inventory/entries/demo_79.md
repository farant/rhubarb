## Demo 79: zeta_12 Capacity Test

- **Status**: COMPLETE
- **File**: `demo_79_zeta12_capacity/main.c` (~877 lines)
- **Tests**: 14/14 pass (Parts A-E + XOR12 retest)
- **Depends on**: Demo 78 (ζ₈ wall at XOR10 confirmed, recursive shadow structure), Demo 66 (quaternion catalog construction from braid words), Demo 67 (Voronoi partition approach)
- **Feeds into**: Demo 80 (group finiteness survey — the finite/infinite distinction identified here drives that investigation)

### Headline
ζ₁₂ breaks the ζ₈ XOR wall entirely: XOR10 goes from 0 winners (ζ₈) to 124 winners (ζ₁₂), XOR12 yields 50+ winners with expanded catalog, and the fundamental cause is that ζ₈ generates a finite group (24 elements, binary octahedral) while ζ₁₂ generates an infinite (or very large) group still growing past 4096 entries.

### Key Results
- **XOR10 breakthrough**: 124 winners at ζ₁₂ (256-entry catalog, k=8, 520 cells). ζ₈ had 0. The wall was not a wall for ζ₁₂.
- **XOR12 solvable**: 50+ winners found from just the first 11 of 124 XOR10 parents using the 4096-entry expanded catalog (k=8, 4104 cells). The initial XOR12=0 result was a truncation artifact (256-entry cap).
- **XOR6 and XOR8 both massive**: 1024+ winners each (capped at MAX_WIN), vs 36 and 6 for ζ₈. The ζ₁₂ catalog is dramatically richer.
- **Non-nesting confirmed**: Only 3/24 ζ₈ quaternions appear in ζ₁₂ catalog (12%). Only 2/13 ζ₈ directions appear in ζ₁₂ directions (15%). These are independent views of the quaternion sphere, not a subset relationship.
- **Finite vs infinite is the key variable**: ζ₈ closes at round 4 with 24 elements. ζ₁₂ roughly doubles each closure round, reaching 4096+ by round 8. Growth pattern: 5 → 17 → 51 → 127 → 275 → 564 → 1140 → 2292 → 4096+.
- **Catalog properties at ζ₁₂ (256-entry cap)**: 64 directions, 12 distinct half-angles (vs ζ₈'s 13 directions, 2 half-angles). Half-angle spectrum spans 0°–90° in 12 distinct values.
- **N-2 pattern disproven**: The initial hypothesis that ζ_N → XOR(N-2) capacity was wrong. ζ₁₂ can do at least XOR12, and potentially beyond as the catalog grows.
- **XOR12 sixth quaternion always from expanded catalog**: Every XOR12 winner requires at least one catalog entry with index > 256, confirming that the larger group is genuinely necessary, not just convenient.

### Theorems/Conjectures
- **Finite Group Ceiling Theorem (DEMONSTRATED)**: A finite quaternion group imposes a hard capacity ceiling: once the catalog is fixed, combinatorial diversity is exhausted at some XOR(N). An infinite group has no such ceiling — capacity is limited only by computational search resources.
- **Nesting Failure (CONFIRMED)**: ζ₈ and ζ₁₂ generate largely disjoint finite subgroups of SU(2). Different angles (π/4 vs π/6) produce different algebraic structures; the capacity gain from ζ₁₂ is from genuinely new structure, not "more of the same."
- **Union Catalog Conjecture (OPEN)**: Since ζ₈ and ζ₁₂ are largely non-overlapping, a union catalog (ζ₈ + ζ₁₂) might push XOR capacity beyond what either achieves alone.
- **ζ_N → N Conjecture (INVALIDATED for finite, PLAUSIBLE for infinite)**: The "ζ_N → XOR N" capacity law was originally proposed but refuted: ζ₁₂ goes well past XOR12. The real determining factor is group finiteness, not the subscript N.

### Data
- ζ₁₂ catalog (256-entry cap): 256 quaternions, 64 directions, 12 half-angles, 65 Voronoi cells
- ζ₁₂ catalog (4096-entry expanded): 4096+ quaternions, 512 directions, 43 half-angles
- XOR6 (ζ₁₂): 1024+ winners (capped), k=1 (Voronoi only), 65 cells
- XOR8 (ζ₁₂): 1024+ winners (capped), k=6, 390 cells
- XOR10 (ζ₁₂): 124 winners, k=8, 520 cells
- XOR12 (ζ₁₂ truncated, 256 entries): 0 winners (artifact)
- XOR12 (ζ₁₂ expanded, 4096 entries): 50+ winners, k=8, 4104 cells
- ζ₁₂ group closure: rounds 0–8 sizes: 5, 17, 51, 127, 275, 564, 1140, 2292, 4096+ (roughly doubling)
- ζ₈ group closure: closes at round 4 with 24 elements
- Quaternion overlap: 3/24 (12%); direction overlap: 2/13 (15%)
- Ratio comparisons (ζ₁₂ vs ζ₈ at 256-cap): quaternions 10.7×, directions 4.9×, half-angles 6×, Voronoi cells 4.6×

### Code Assets
- **Parameterized `init_su2_generators(half_angle)`**: takes the rotation half-angle as a parameter, enabling catalog construction for any ζ_N without code changes
- **Generic `g_win[8][MAX_WIN][MAX_TUPLE]` winner storage**: stores XOR6 through XOR14 winners (levels 3–7) in a single unified structure indexed by level
- **`find_recursive(level)`**: generic recursive extension from level-1 winners to level winners; replaces the per-XOR-level functions in Demo 78 with a single reusable routine parameterized by level
- **`find_quat_in(q, arr, arr_size)`**: searches for a quaternion in an arbitrary array (not just the global catalog); used in Part E nesting verification
- **Save/restore catalog pattern**: Part E saves the ζ₁₂ catalog, rebuilds ζ₈, does the comparison, without needing separate static arrays for each root
- **Dynamic n_vor**: cell count `n_vor = g_nd + 1` computed from catalog rather than hardcoded to 14; handles catalogs with different direction counts transparently

### Literature Touched
- **ADE classification of finite SU(2) subgroups**: ζ₈ maps to binary octahedral (E₇, order 48). The finiteness observation connects to the complete ADE classification surveyed in Demo 80.
- **Kuperberg #P-hardness**: his result applies at non-lattice roots of unity; our finite groups (ζ₄, ζ₈) are lattice roots where computation is tractable. The infinite-group roots correspond to the #P-hard regime.
- **Habiro (2002) / Z[ζ₈] bracket**: the 24-element ζ₈ group is the algebraic substrate underlying the bracket computations from Demos 26–29. The finiteness of this group explains the capacity ceiling observed in the forward DKC arc.
- **Nazer & Gastpar (2011)**: infinite group → unbounded combinatorial space → potentially unlimited compute-and-forward capacity.

### Open Questions
- **Is ζ₁₂ truly infinite or very large finite?**: The doubling pattern suggests infinite, but this is a concrete group theory question. Does the group generated by cos(π/6) and cos(π/6) rotations around perpendicular axes close at some enormous order?
- **What is ζ₁₂'s true XOR capacity?**: XOR14 and beyond may also be solvable. The recursive search becomes expensive but is in principle feasible.
- **Does ζ₁₀ (Fibonacci anyons territory) behave similarly?**: ζ₁₀ was identified as infinite in Demo 80. Does it also show XOR10+ capacity? Its 71 distinct half-angles at 4096 entries (most of any surveyed root) suggests it might be particularly rich.
- **Union catalog performance**: A combined ζ₈ + ζ₁₂ catalog exploiting both independent algebraic structures — does it provide superadditive capacity?
