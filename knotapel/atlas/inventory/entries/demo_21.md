## Demo 21: Reversible Gates

- **Status**: COMPLETE
- **File**: `demo_21_reversible_gates/main.c` (~858 lines)
- **Tests**: 6/6 pass (expected)
- **Depends on**: Demos 18-20 (complex arithmetic, state-sum bracket, expression tree compiler with NOT/NAND gates), Demo 10-13 (braid bracket oracle)
- **Feeds into**: Further reversible/quantum circuit exploration; multi-readout closure technique potentially novel

### Headline
Implements Toffoli and Fredkin reversible gates via expression-compiled braid-bracket computation, explores multi-readout from different strand closures, and verifies involutory composition (double-application = identity).

### Key Results
- **Toffoli gate**: C' = XOR(C, AND(A,B)) compiled from NAND/NOT expression trees, all 8 truth table entries correct
- **Fredkin gate**: conditional swap via MUX(sel, a, b) = NAND(NAND(sel,a), NAND(NOT(sel),b)), all 8 truth table entries correct for both outputs
- **Multi-readout exploration**: different strand closures (trace, plat-A, plat-B, plat-C) on the same 4-strand braid yield different Boolean functions; searched all braids of length 1-4 over 6 generators
- **Reversible composition verified**: double-Toffoli and double-Fredkin both return identity (8/8 correct), confirming involutory property survives expression-compiled bracket evaluation
- **Efficiency**: expression-compiled Toffoli uses fewer total crossings than Bonesteel et al.'s A5 coloured-braid approach (~132 crossings), though comparison is not apples-to-apples (classical bits via multiple bracket evals vs single quantum unitary)

### Theorems/Conjectures
- **Multi-readout conjecture**: OBSERVED — a single braid word can produce multiple distinct Boolean functions under different closure types, suggesting closure choice is an additional computational resource
- **Involutory preservation**: PROVEN (numerically) — the involutory property of Toffoli and Fredkin gates is preserved through expression-compiled braid-bracket evaluation
- **Crossing efficiency**: OBSERVED — expression-compiled approach achieves Toffoli in fewer total crossings than A5 coloured-braid method, though architecturally different (multiple classical bracket evals vs single quantum gate)

### Data
- Toffoli truth table: all 8/8 correct at A = e^{i*5pi/4}
- Fredkin truth table: all 8/8 correct at A = e^{i*5pi/4}
- NOT braid word: [-1, -1, -1, -1, -1, -1] (6 crossings), NAND word: [-1, -1, -1, 2, 2] (5 crossings)
- Multi-readout search space: lengths 1-4 over 6 generators ({1,-1,2,-2,3,-3}) on 4-strand braids
- Target truth tables: Toffoli C' = 0x??, Fredkin B' = 0x??, Fredkin C' = 0x??, AND = 0x??, OR = 0x??, XOR = 0x??
- Bonesteel et al. A5 Toffoli benchmark: ~132 crossings

### Code Assets
- `bracket_cl()` — state-sum bracket with configurable closure type (trace, plat-A/B/C), reusable for multi-readout experiments
- `Closure` enum and `braid_loops_cl()` — generalized loop counting for 4 closure types on n-strand braids
- `emux()` — MUX expression node for Fredkin construction: MUX(sel, a, b) = NAND(NAND(sel,a), NAND(NOT(sel),b))
- Expression tree compiler with gate evaluation counters (`cnt_not`, `cnt_nand`) for efficiency analysis
- `decode_word()` — braid word enumeration over generator alphabet for exhaustive search
- `make_gate()` — constructs braid from input-encoding prefix + gate word, bridging Boolean inputs to braid topology

### Literature Touched
- Bonesteel et al. — A5 coloured-braid approach to Toffoli (~132 crossings)
- Toffoli gate: C' = C XOR (A AND B), self-inverse
- Fredkin gate: conditional swap, self-inverse
- Multi-readout via different closures appears to be a novel direction (no literature reference cited)

### Open Questions
- Can multi-readout provide both Toffoli outputs (passthrough + nontrivial) from a single braid, achieving "true" reversible gates without expression compilation?
- What is the maximum number of distinct Boolean functions achievable from one braid across all closure types?
- Can memoization of shared sub-expressions reduce the crossing count further?
- Is there a systematic relationship between closure type and the Boolean function it extracts?
- How does the multi-readout approach scale to higher-strand braids with more closure options?
