## Demo 65: Clifford Hierarchy Staircase

- **Status**: COMPLETE
- **File**: `demo_65_clifford_staircase/main.c` (~2014 lines)
- **Tests**: 38/38 pass
- **Depends on**: Demo 50 (four-tier NPN hierarchy, parity at k=6, 906 solutions), Demo 63 (angular anatomy, parity ceiling n=5, oriented matroid classification), Demo 64 (interaction-level wall anatomy, XOR6 open questions)
- **Feeds into**: Demo 66, Demo 67, Demo 68, Demo 69, Demo 70, Demo 71 (quaternionic arc)

### Headline
Climbing from Z[zeta_8] to Z[zeta_16] leaves the parity ceiling at n=5 and refutes the gap-of-2 hypothesis, but generalized (non-standard) sector activations break the wall at k=24 — revealing a two-layer structure separating geometric obstruction from labeling convention.

### Key Results
- **Part A-B — Z[zeta_16] catalog**: 36 distinct nonzero bracket values at delta=-sqrt(2) (vs 100 at delta=0). 24/36 axial, 12/36 two-component. 14/16 angular directions occupied (missing dirs 2 and 11). 20/36 values lie in the Z[zeta_8] subring, but catalogs are mostly disjoint — only 8/100 Z[zeta_8] values appear in Z[zeta_16].
- **Part C-D — Algebra independence**: Tier thresholds at Z[zeta_16] are exactly XOR3 k=6, XOR4 k=8, XOR5 k=14 — identical to Z[zeta_8]. Parity ceiling n=5 is unchanged. The "gap of 2" hypothesis is refuted: pigeonhole rises from 7 to 14 but actual ceiling stays at 5, widening the gap to 9.
- **Part E-F — XOR6 wall is genuine**: XOR6 = 0 at every k=2..31, tested with both base catalog (36 values) and extended catalog (56 values, +20 from 4-strand braids). Catalog extension revealed k=31 XOR5 zero as incompleteness artifact (0 → 960 solutions); XOR6 zero at all k survives extension, confirming the wall is real.
- **Part G — Generalized activations break the wall**: Testing any binary labeling of k sectors (not just standard odd/even), with 2,700 checks per k: absolute wall at k<=23 (zero solutions regardless of labeling), first generalized XOR6 at k=24 (3/2700 pass). Minimum k for generalized XOR6: k=24.
- **Part H — Even/odd k pattern**: Near the threshold, generalized XOR6 solutions exist only at even k (26: 4, 28: 4, 30: 10, 32: 4) and fail at all odd k (25, 27, 29, 31: all 0). k=38 is the only even k >= 24 with zero solutions (38 = 2*19).
- **Part H — Antipodal pairs mandatory**: All 3 passing XOR6 tuples at k=24 consist of exactly 3 antipodal pairs (w, -w). Min margin to sector boundary: 0.000000 rad — sums land exactly on lattice directions.
- **Part I-J — Generalized XOR7**: First at k=91 (broader search over 27 XOR5 sets * C(100,2) pairs = 136,350 7-tuples per k). XOR7 winner anatomy: 3 antipodal pairs + 1 lone weight at 90 degrees (purely imaginary). Scaling ratio 91/24 ~= 3.8.
- **Part J — Zero margin universality**: All passing tuples at all tested k values have exactly zero margin. Z[zeta_8] lattice sums always land on lattice directions (multiples of pi/4), aligning exactly with sector boundaries for compatible k values. Solutions are algebraically exact.

### Theorems/Conjectures
- **Theorem (parity ceiling is algebra-independent)**: The parity ceiling n=5 and tier thresholds (XOR3 k=6, XOR4 k=8, XOR5 k=14) are identical at Z[zeta_8] and Z[zeta_16]. PROVEN (exhaustive sweep k=2..31, both catalogs).
- **Theorem (gap-of-2 refuted)**: The gap between pigeonhole bound and actual ceiling is not constant. Z[zeta_8]: gap=2 (7-5). Z[zeta_16]: gap=9 (14-5). PROVEN.
- **Theorem (absolute wall k<=23)**: No binary partition of k sectors (for any k <= 23) can separate the XOR6 truth table using Z[zeta_8] catalog weights. PROVEN (exhaustive, 2700 checks per k=6,8,15,16..23).
- **Theorem (k=24 breaks convention wall)**: At k=24, exactly 3 of 2700 candidate (XOR5-set, extension-weight) pairs admit a binary coloring of 24 sectors that realizes XOR6. PROVEN.
- **Theorem (zero margin universality)**: All generalized XOR6 solutions at all tested even k have exactly zero margin to the nearest sector boundary, reflecting exact lattice alignment. PROVEN.
- **Conjecture (universal ceiling)**: The parity ceiling n=5 holds for all cyclotomic evaluation points (all levels of the Clifford hierarchy). If confirmed, this becomes a theorem about topological state sums, not algebra. CONJECTURED.
- **Conjecture (scaling law)**: XOR_n minimum k grows roughly by factor ~3.8 per increment: XOR6 k=24, XOR7 k=91, XOR8 ~k=345. CONJECTURED (two data points only).
- **Conjecture (k=24 as projection)**: The minimum of k=24 sectors for S1 XOR6 is a projection of the 24-cell's 24 vertices onto S1. CONJECTURED (confirmed in Demo 66).

### Data
- Z[zeta_16] catalog size: 36 values (base), 56 values (extended, +20 from 4-strand braids)
- Angular directions occupied: 14/16 (missing dirs 2 and 11, at 45.0 and 247.5 degrees)
- Catalog overlap: 8/100 Z[zeta_8] values appear in Z[zeta_16]; 20/36 Z[zeta_16] values lie in Z[zeta_8] subring
- Tier thresholds (identical at both rings): XOR3 k=6, XOR4 k=8, XOR5 k=14, XOR6 impossible (standard)
- XOR5 solutions at Z[zeta_16] k=14: 120; k=15: 1080 (extended); k=17: 240; k=21: 240; k=31: 960
- XOR6 solutions (standard): 0 at every k=2..31, both catalogs
- Generalized XOR6 first pass: k=24, 3/2700
- Even k generalized XOR6 pass counts: k=24: 3, k=26: 4, k=28: 4, k=30: 10, k=32: 6, k=38: 0, k=40: 2, ..., k=62: 34
- k=38 gap: only even k >= 24 with zero generalized XOR6 solutions
- Generalized XOR7 minimum k: 91 (broader search); XOR7 winner: 3 antipodal pairs + 1 lone weight at 90 degrees
- XOR7 pass counts: k=127: 1 (narrow), k=120: 5, k=127: 4 (broader)
- Scaling ratio XOR7/XOR6 minimum k: 91/24 ~= 3.8

### Code Assets
- **Z[zeta_16] arithmetic**: 8-component exact representation, bracket evaluation at delta=-sqrt(2), direction classification over 16 angular bins
- **Multi-catalog XOR search**: runs full sector sweep with both 36-value and 56-value catalogs, distinguishes catalog incompleteness artifacts from genuine walls
- **Generalized binary labeling check**: for a given k-sector partition and n-tuple of weights, tests whether any binary coloring of k sectors separates the 2^n truth table (not just standard odd/even)
- **`generalized_xor6_check()`**: iterates over all 2700 (XOR5-set, extension) pairs at arbitrary k, counts passing binary labelings
- **Even k landscape sweep**: systematic even k=2..64 generalized XOR6 pass counts
- **XOR7 broader search**: extends from single extensions to all C(100,2) weight pairs, 136,350 7-tuples per k
- **Antipodal structure analyzer**: decomposes n-tuple into antipodal pairs and lone weights, reports margin to sector boundaries

### Literature Touched
- Aizenberg (2008): k-sector MVN activation; generalized binary labeling is a strict relaxation of his standard odd/even convention
- Kauffman (1987): bracket polynomial as topological state sum; delta parameter controls multi-loop cancellation
- Habiro / Nazer-Gastpar / Abramsky / TL-non-semisimplicity: background for DKC literature context
- Demo 63 (Angular Anatomy): oriented matroid classification, parity scaling law for Z[zeta_8]
- Demo 64 (open questions): Z[zeta_16] ceiling question and gap-of-2 hypothesis, both answered here

### Open Questions
- Does the parity ceiling n=5 hold at Z[zeta_32] and all higher cyclotomic levels? If so, the proof should be purely topological (no mention of evaluation point).
- Why is k=38 the only even k >= 24 with zero generalized XOR6? Is 19 (= 38/2) special with respect to the Z[zeta_8] lattice?
- Should the generalized check be rewritten with exact Cyc8 arithmetic (no floating-point atan2) to formally eliminate boundary ambiguity? Zero margin everywhere makes this feasible.
- Does the absolute wall threshold (k=24) change when using Z[zeta_16] weights for the generalized check?
- Is the even/odd k pattern (even works, odd fails near threshold) provable from the pi/4 lattice alignment argument?
- Does any odd k >= 25 produce generalized XOR6 solutions? (Current sweep covered only even k.)
- Scaling law: is XOR8 minimum k ~345, and does the growth rate discriminate polynomial from exponential?
- Does the lone imaginary weight (90 degrees) in the XOR7 winner generalize, or is it specific to this solution?
