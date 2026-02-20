## Demo 23: Function Zoo

- **Status**: COMPLETE
- **File**: `demo_23_function_zoo/main.c` (~771 lines)
- **Tests**: 4 checks across 4 parts (A-D): exhaustive catalog, NPN classification, closure tuple analysis, extended search
- **Depends on**: Demo 21 (bracket with closure support, Toffoli/Fredkin targets), Demo 18 (braid logic gates, 5pi/4 angle)
- **Feeds into**: Later demos exploring which NPN classes topology can/cannot reach

### Headline
Exhaustive enumeration of which 3-input Boolean functions topology "prefers": catalogs all 4-strand braids of length 1-5 across 4 closure types, classifies by NPN equivalence, analyzes how closure choice multiplies function diversity, and searches for specific target functions (Toffoli, Fredkin).

### Key Results
- **Exhaustive catalog** (Part A): all 6^1 + 6^2 + 6^3 + 6^4 = 1554 braids of length 1-4, each evaluated with 4 closures (trace, plat_A, plat_B, plat_C) at A = e^{i*5pi/4}. Reports distinct non-trivial truth tables found out of 254 possible. Top 20 most common functions listed with hex, binary, count, name, NPN class.
- **NPN classification** (Part B): maps all found truth tables to their NPN canonical form (minimum under negation of inputs + permutation + negation of output). Reports how many of the total 3-variable NPN classes are reached by topology.
- **Closure tuple analysis** (Part C): each braid produces a 4-tuple of truth tables (one per closure). Finds that ~80%+ of braids are closure-dependent (different closures yield different functions). Counts braids with all 4 TTs distinct, and braids with permutation-related TT pairs. Per-closure function diversity reported.
- **Extended search** (Part D): length-5 braids (6^5 = 7776 words). Reports new functions and new NPN classes not seen at lengths 1-4. Specifically tracks Toffoli C' (0x6A), Fredkin B' (0xAC), Fredkin C' (0xCA) hit counts.

### Theorems/Conjectures
- Topology has strong preferences: certain Boolean functions appear far more often than others in the braid→bracket→threshold pipeline
- Closure is a major degree of freedom — different closures of the same braid typically yield different Boolean functions (≥80% closure-dependent)
- Longer braids unlock new NPN classes, suggesting the function zoo grows with braid length

### Data
- 3-input Boolean functions: 256 truth tables (0x00-0xFF), 254 non-trivial
- Generator alphabet: {σ₁, σ₁⁻¹, σ₂, σ₂⁻¹, σ₃, σ₃⁻¹} (6 generators for 4-strand braids)
- Braid lengths searched: 1-4 (Part A, 1554 braids) + 5 (Part D, 7776 braids)
- 4 closure types: trace (strand i top↔bottom), plat_A ((0,1)(2,3)), plat_B ((0,2)(1,3)), plat_C ((0,3)(1,2))
- NPN classification: 96 transforms per truth table (6 perms × 8 input-neg masks × 2 output-neg)
- Up to 10,000 braid tuples stored for closure analysis

### Code Assets
- **NPN equivalence engine**: `npn_transform()` applies permutation + input negation + output negation; `npn_init()` precomputes canonical form for all 256 truth tables; `are_perm_related()` checks input-permutation equivalence
- **`compute_tt()`**: given a braid word + closure + angle, evaluates all 8 input combinations, sorts amplitudes, finds best-gap threshold, returns truth table (or -1 if no clean threshold)
- **`decode_word()`**: converts integer code to braid word using generator map {1,-1,2,-2,3,-3}
- **`fn_name()`**: lookup table mapping ~40 common truth tables to human-readable names (AND, OR, XOR, MUX, MAJ, Toffoli, etc.)
- **`bracket_cl()`**: state-sum Kauffman bracket with closure support (trace, plat_A/B/C) — from Demo 21
- **`braid_loops_cl()`**: union-find loop counter with 4 closure types
- Per-braid tuple storage for closure analysis (up to 10,000 braids × 4 closures)

### Literature Touched
- NPN equivalence classes for Boolean functions (standard combinatorics: 3-variable has a known number of classes)
- Toffoli gate and Fredkin gate as target reversible/classical universal functions
- Plat closure vs trace closure in knot theory
- Connection between braid topology and Boolean function reachability

### Open Questions
- Which NPN classes are topologically unreachable, and is there an algebraic reason?
- Does the function distribution stabilize at some braid length, or do new classes keep appearing?
- Is there a relationship between braid complexity (crossing number) and the "computational complexity" of the realized Boolean function?
- Can closure choice be understood as a form of input/output routing that systematically permutes the truth table?
