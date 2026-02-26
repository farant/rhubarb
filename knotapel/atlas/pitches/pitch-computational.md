# Computational Mathematician Pitch: New Atlas Documents

Date: 2026-02-26

---

## Pitch 1: Scaling Law Catalog

**Location:** `atlas/synthesis/scaling-laws.md`

**What it would contain:**

A unified catalog of every empirical scaling relationship discovered across 101 demos, each with:
- The law itself (formula, exponent, constant)
- The independent variable and its domain
- Number of data points supporting it
- Regime boundaries (where the law breaks or transitions)
- Whether it has a mechanistic explanation or is purely empirical
- Cross-references to the demo(s) that established and tested it

Currently identified scaling laws scattered across data-tables.md and connections.md include:
- `max_xor ~ depth + 6` (D82, linear capacity growth with crossing depth)
- `max_xor ~ 0.62 * log2(catalog) + 4.6` (D81, logarithmic law as corollary of depth law)
- `BFS catalog growth ~ 2x per round` (D82, exponential vocabulary generation)
- `BFS growth ~ d*x per round` at d strands (D99-D101, three data points at d=2,3,4)
- `rad(TL_l) = 2l - 3` (D51-D52, D60, universal radical dimension)
- `rad(TL_{l+1}) = l^2 - l - 3` (D39, next-level radical)
- `Fibonacci rank F(l-1)` for rad^2 bilinear rank total (D52, D60)
- Null fraction dilution: zeta_4=75%, zeta_8=37.5%, zeta_12=3% (D84, D87)
- Decoherence rate (2l-3)/C_l decaying from 60% at l=3 to 2.6% at l=7 (D60)
- Compression ratio growth with braid length (D35, roughly linear in length)
- Funnel coverage collapse: 99.3% -> 39.2% -> 19.2% -> 0% (D64)
- Upward XOR7/XOR6 sector threshold ratio: 91/24 ~ 3.8 (D65)
- Framing cost: exactly 2 XOR levels at every root tested (D83)
- Circuit complexity hierarchy: AND/XOR ratio 1.01 -> 2762 -> infinity (D93)
- Solution count monotonic decrease: 23004 -> 16108 -> 3020 -> 0 as inputs grow (D63)
- Non-monotonic parity solutions: 906@k=6 > 756@k=7 > 96@k=8 (D50)

**Why it matters:**

Scaling laws are the empirical heartbeat of any computational physics program. Right now they are diffused across at least four atlas documents and the explorer's log. Some are proven theorems, some are three-point fits, some are single-regime observations. Without a single document that organizes them by type (algebraic, combinatorial, computational), status (proven/demonstrated/conjectural), and regime, it is too easy to (a) forget a law exists, (b) assume a demonstrated law has been proven, or (c) miss that two laws are the same law in different variables (as D82 showed for the depth/log laws). A catalog would also immediately surface the weakest laws -- the ones with fewest data points or narrowest regimes -- prioritizing which experiments to run next.

**Gap it fills:**

data-tables.md records raw numbers. theorems.md records proven results. Neither organizes the *empirical regularities* that sit between raw data and proven theorems. This document would be the atlas's equivalent of a table of physical constants: the empirical backbone that theory must explain.

---

## Pitch 2: Parameter Space Map

**Location:** `atlas/synthesis/parameter-space.md`

**What it would contain:**

A systematic map of the multi-dimensional parameter space that DKC has explored, organized as a coordinate grid with coverage annotations. The primary axes are:

1. **Root of unity** (zeta_4, zeta_6, zeta_8, zeta_10, zeta_12, zeta_16, zeta_20, zeta_24, zeta_32): group finiteness, catalog size, ring dimension, axiality type
2. **Strand count** (n=2,3,4,5): planar matching dimension, module type
3. **Braid length / crossing depth** (1 through ~8): vocabulary size, BFS generation
4. **Activation type** (half-plane, split-sigmoid, MVN, k-sector, S^2 Voronoi, S^1 x S^2, generalized binary): geometric projection
5. **Encoding** (1wpi multiplicative, +/-q additive, neglecton): what the neuron reads
6. **Module type** (standard V_{n,j}, projective P_{0,0}, Delta_1 irreducible, W_{n,k} non-semisimple extension, W_{n,k} simple): representation-theoretic source

For each explored coordinate, record: demo number, catalog size, max_xor achieved, key finding. For unexplored coordinates, record: why unexplored (combinatorial explosion, known impossibility, not yet attempted).

The document should include:

- A "heat map" table showing max_xor achievement across (root x strand-count x activation) triples
- A "coverage density" analysis: how much of the theoretically reachable parameter space has been probed
- Identified "phase boundaries" where behavior changes qualitatively (finite/infinite group transition at zeta_8/zeta_12, semisimple/non-semisimple transition at n=l, 1wpi/additive encoding function-lock)
- A "diminishing returns" assessment: which parameter-space directions have been thoroughly characterized vs. which still have unexplored interior

**Why it matters:**

With 101 demos it is increasingly difficult to answer the question "have we looked at X?" The parameter space is at least 6-dimensional and exploration has been driven by conceptual arcs rather than systematic grid-filling. This is not a criticism -- conceptual arcs are the right way to do research -- but it means there may be systematically unexplored pockets. For example: what happens at zeta_12 with 3-strand modules? What does the DKC landscape look like for the Delta_1 module at zeta_12 instead of zeta_8? The multi-strand arc (D99-D101) opened a new axis but has only been explored at one root. A parameter-space map would make these gaps immediately visible.

**Gap it fills:**

connections.md maps *conceptual* dependencies between demos. This would map *parameter-space* coverage. The demo-index tells you what each demo did; the parameter-space map tells you what the collective 101 demos have and have not covered. It is the difference between a bibliography and a survey map.

---

## Pitch 3: Numerical Coincidences and Unexplained Constants

**Location:** `atlas/inventory/coincidences.md`

**What it would contain:**

A curated list of every numerical pattern, structural coincidence, or unexplained constant observed computationally but not yet given a theoretical explanation. Each entry with:

- The coincidence (stated precisely)
- Status: EXPLAINED (with citation), PARTIALLY EXPLAINED, UNEXPLAINED
- Plausibility assessment: is this likely deep or likely an artifact?
- Attempted explanations that failed

Currently identified candidates:

- **82.8% additive determinism** (D73): universal across all 36 winning triples, zero variation. Why 82.8%? Is this an algebraic fraction related to the 24-cell?
- **sqrt(5) absence in magnitude quantization** (D75): sum magnitudes take values sqrt{0,1,2,3,4,6} but never sqrt(5). Traced to 24-cell inner products. But why exactly those inner products?
- **6 F4 orbits from 35 solutions** (D66): is 6 = C(4,2)? Is 35 = C(7,3)? The orbit sizes {12,7,5,5,4,2} -- do these come from some partition or Young diagram?
- **k=38 anomaly** (D65): only even k >= 24 with zero generalized XOR6 solutions. 38 = 2 x 19. Why 19?
- **Octant 4 absent from poison values** (D64): 31 poison values avoid octant 4 entirely, spanning all other octants. Why octant 4?
- **13 = 2x6+1 = |eigenvector directions|** (D71): the DOF coincidence may be deeper than the sampling argument.
- **Depth constant +6** (D82): `max_xor ~ depth + 6`. Where does the 6 come from?
- **Framing always costs exactly 2** (D83): writhe normalization costs exactly 2 XOR levels at every root. Why 2?
- **3.8x sector threshold ratio** (D65): XOR7/XOR6 minimum k ratio = 91/24 ~ 3.79. Algebraic?
- **Non-monotonic k-optimum at k=6** (D50): why does k=6 give more parity solutions (906) than k=7 (756) or k=8 (96)?
- **44/31 good/poison split** (D64): why 44 and 31 specifically?

**Why it matters:**

In experimental physics, unexplained constants and coincidences are the primary generators of new theory. Kepler's third law was a numerical coincidence until Newton explained it. The 82.8% determinism constant, the "+6" in the depth law, the "+2" framing cost -- each of these is either a deep structural fact waiting for a theorem or an artifact. A dedicated tracking document prevents these observations from being forgotten during context resets and creates a natural "to-explain" queue for theoretical work. It also serves as an honesty mechanism: claims in narrative.md and novelty.md should distinguish "we proved this" from "we observed this number and it keeps coming up."

**Gap it fills:**

theorems.md tracks results stated as theorems or conjectures. research-questions.md tracks questions. Neither tracks *numbers that appear significant but whose significance is not yet understood*. These are pre-conjectures -- patterns not yet sharp enough to be conjectures but too persistent to ignore. The coincidences document is the nursery where future conjectures incubate.

---

## Pitch 4: Computational Complexity Profile

**Location:** `atlas/inventory/complexity-profile.md`

**What it would contain:**

For every major computational task in the DKC pipeline, a profile recording:

- **Time complexity** (theoretical big-O and measured wall-clock at representative sizes)
- **Space complexity** (peak memory, dominant data structure)
- **Bottleneck analysis** (what dominates runtime as n grows)
- **Feasibility frontier** (largest instance computed, estimated resource for next step)

Pipeline tasks:

1. **State-sum bracket** (D01): O(2^n * n). Feasible to n ~ 20-25 crossings.
2. **TL matrix construction** (D35): O(C_n^2 * C_n). C_8 = 1430, so TL_8 is 1430x1430. Memory: O(C_n^2 * ring_rank).
3. **BFS catalog generation** (D79, D82): exponential in depth rounds. ~2x growth per round at zeta_12.
4. **Exhaustive Boolean search** (D48): O(catalog^k * 2^N). D48 searched 100^4 * 2^3 = 800B operations.
5. **Gram rank computation** (D51-D52): O(C_n^3). At TL_8: 1430^3 ~ 2.9 billion operations.
6. **Multi-strand module construction** (D99-D101): d-strand dimensions grow as C(2d,d). 3-strand: 20. 4-strand: 70. 5-strand: 252. 6-strand: 924.
7. **Raqiya/Scrutinium analysis**: O(n^2) pairwise for n catalog values.

Key feasibility questions:

- Can we compute TL_9 or TL_10 radical anatomy? (C_10 = 16796: ~282M matrix entries, Gaussian elimination ~ 4.7T ops. Probably infeasible without sparse methods.)
- What is the actual wall-clock for 6-strand W_{6,4} BFS? (924-dim module.)
- How far can zeta_12 BFS be pushed before memory becomes limiting?
- At what point does exhaustive search become infeasible and statistical sampling is needed?

**Why it matters:**

The project has hit genuine computational walls (TL_6 P_{0,0} in D86, 100M search in D48, zeta_12 BFS in D81). Some were mathematical impossibilities; some were computational limits. Distinguishing between the two is critical for planning. A complexity profile prevents wasted effort: if 6-strand BFS requires 10 hours, that is useful to know before launching. If it requires 10 years, the research question needs reformulating.

From a "computational experimental physics" perspective, this is knowing the resolution limits of your instruments. Every empirical result has an implicit "at the resolution/scale we could compute" qualifier. Making those qualifiers explicit strengthens every claim.

**Gap it fills:**

code-assets.md catalogs reusable code patterns. floating-point-audit.md classifies FP usage. Neither addresses "how hard is this computation, and how much harder is the next step?" The complexity profile turns "we should try X" into "we can try X at this cost" or "X is infeasible without algorithmic improvement."

---

## Summary Table

| # | Document | Location | Type | Primary Gap |
|---|----------|----------|------|-------------|
| 1 | Scaling Law Catalog | synthesis/scaling-laws.md | Synthesis | Empirical regularities between raw data and proven theorems |
| 2 | Parameter Space Map | synthesis/parameter-space.md | Synthesis | Coverage/gap analysis of the multi-dimensional experiment space |
| 3 | Numerical Coincidences | inventory/coincidences.md | Inventory | Pre-conjectures: significant numbers without explanations |
| 4 | Complexity Profile | inventory/complexity-profile.md | Inventory | Feasibility frontiers and computational cost of next steps |
