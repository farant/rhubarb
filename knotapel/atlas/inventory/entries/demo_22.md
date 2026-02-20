## Demo 22: Minimum Braid Complexity

- **Status**: COMPLETE
- **File**: `demo_22_braid_optimizer/main.c` (~976 lines)
- **Tests**: ~6 pass (P1: >=80 functions found, P2: TL_3 braid relation, P3: cross-validation, P4: nilpotent compression, P5: braid < NAND cost, P6: extended search)
- **Depends on**: Demo 21 (reversible gates, closure types), Demo 23 (function zoo, truth table computation)
- **Feeds into**: establishes "atomic weights" for topological computation; periodic table of braid-computable functions

### Headline
Exhaustive search for minimum-length braid words computing each Boolean function on 3 inputs — the "periodic table of topological computation" — with TL_3 nilpotent oracle proving algebraic compression at delta=0.

### Key Results
- **Part A**: Atlas of minimum braid lengths for all achievable 3-input Boolean functions (4-strand braids, lengths 1-5, 4 closure types), >=80 functions found
- **Part B**: TL_3 algebra at delta=0 (A=e^{i*pi/4}) verified — nilpotency (e_i^2=0), braid relation, TL relation (e1*e2*e1=e1); cross-validation shows matrix equality implies bracket equality with zero errors; distinct TL_3 matrices grow slower than 4^k (compression factor at length 6)
- **Part C**: Parity barrier investigation — some XOR-containing functions missing at short lengths; braid weight beats NAND expression cost for multiple functions
- **Part D**: Extended search at length 6 (trace closure, 46656 braids) discovers additional functions; targets include XOR, triple XOR, Toffoli C', MUX/Fredkin, MAJ

### Theorems/Conjectures
- VERIFIED: TL_3 nilpotent algebra (delta=0): e_i^2 = 0, e1*e2*e1 = e1, braid relation holds in 5x5 representation
- VERIFIED: Matrix equality in TL_3 implies bracket equality (zero cross-validation errors across all length-3 B_3 word pairs)
- VERIFIED: Nilpotent compression — distinct TL_3 matrices grow strictly slower than 4^k (many algebraically distinct braids collapse to same TL element)
- CONFIRMED: Braid weight < NAND expression cost for multiple named functions (topological computation is more efficient than gate-tree evaluation)
- OBSERVED: Partial parity barrier — some XOR-family functions missing at short lengths (investigated but not fully resolved)

### Data
- Atlas: 254 possible non-trivial 3-input truth tables; >=80 achieved at lengths 1-5
- Named functions tracked: A, ~A, B, ~B, C, ~C, A&B, A|B, ~(A&B), A^B, A XNOR B, MAJ, MUX, Toffoli C', A^B^C, etc.
- NAND cost reference: identity=0, NOT=6, NAND=5, AND=11, OR=17, XOR=25, Toffoli C'=58, MUX=21
- TL_3 distinct matrix counts per length: subexponential growth vs 4^k naive
- Search spaces: 6^k for 4-strand (6 generators), 4^k for 3-strand (4 generators)
- Evaluation angle: A = e^{i*5*pi/4} (delta=0 for TL_3 oracle at pi/4)

### Code Assets
- `atlas_min_len[256]`, `atlas_word[256]`, `atlas_closure[256]`: minimum-complexity atlas for all truth tables
- `compute_tt()`: evaluates 4-strand braid with 3-bit input encoding, finds best-threshold truth table from amplitude sorting
- `decode_word4()` / `decode_word3()`: enumerates braid words from integer codes (base-6 for 4-strand, base-4 for 3-strand)
- `fn_name()`: lookup table mapping truth table integers to named Boolean functions (~38 entries)
- `tt_has_parity()`: classifies whether a truth table involves XOR/parity
- `nand_cost()`: NAND tree expression cost for named functions (from Demo 21 analysis)
- `Mat5` type: 5x5 complex matrix for TL_3 representation with `mat5_zero/id/scale/add/mul/eq`
- `tl3_init()`: builds TL_3 generator matrices (e1, e2) and sigma matrices at given A
- `tl3_braid_matrix()`: evaluates braid word as TL_3 matrix product
- `bracket_cl()`: state-sum bracket with closure support (trace, plat_A, plat_B, plat_C)
- `braid_loops_cl()`: loop counting with 4 closure types
- `Closure` enum: CL_TRACE, CL_PLAT_A, CL_PLAT_B, CL_PLAT_C

### Literature Touched
- Temperley-Lieb algebra TL_n at delta=0 (nilpotent regime)
- Catalan number C_3 = 5 as TL_3 dimension
- Braid group B_3 representation theory
- Shannon/NAND circuit complexity comparison
- Parity barrier in threshold logic (XOR as canonical hard function)

### Open Questions
- Is the parity barrier fundamental (algebraic) or just a length limitation? Do parity functions appear at length 7+?
- Can the TL_3 oracle be used to prune search space for the 4-strand atlas? (collapse equivalent braids before bracket evaluation)
- What is the asymptotic growth rate of distinct TL_3 matrices? (appears subexponential but not fully characterized)
- Relationship between closure type and function achievability — which closures are most "powerful"?
- Can connected-sum operations on braids compose functions algebraically?
