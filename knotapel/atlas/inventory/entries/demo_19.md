## Demo 19: Braid Circuit Compiler

- **Status**: COMPLETE
- **File**: `demo_19_braid_compiler/main.c` (~1017 lines)
- **Tests**: ~15 checks across 5 parts (A-E): root-of-unity catalog, universal gate set, boolean compiler, full adder, efficiency analysis
- **Depends on**: Demo 18 (braid logic gates: NOT and NAND discovery), Demo 15 (delta-zero optimal angles)
- **Feeds into**: Demo 20 (braid error analysis)

### Headline
Builds a complete boolean expression compiler that evaluates arbitrary logic circuits via recursive Kauffman bracket thresholding of braid gates, demonstrating a universal {NOT, NAND} gate set at the 8th root of unity (A = e^{i*5pi/4}) and compiling a full 1-bit adder that passes all 8 input combinations.

### Key Results
- **Root-of-unity catalog** (Part A): tests NOT and NAND gates at 4th, 6th, 8th, 10th, 12th roots of unity. Multiple roots support universal gate sets. Establishes a hierarchy of gate-supporting angles.
- **Universal set at 5pi/4** (Part B): NOT (6-crossing, word: [-1,-1,-1,-1,-1,-1]) and NAND (5-crossing, word: [-1,-1,-1,2,2]) both work at convergence angle. Raw vs reduced bracket separation compared. Fallback angle sweep if 5pi/4 fails.
- **Boolean compiler** (Part C): expression tree with EXPR_VAR / EXPR_NOT / EXPR_NAND nodes. Derived gates: AND = NOT(NAND), OR = NAND(NOT,NOT), XOR = NAND(NAND(a,NAND(a,b)),NAND(b,NAND(a,b))). All truth tables verified for AND (4/4), OR (4/4), XOR (4/4).
- **Full adder** (Part D): sum = XOR(XOR(a,b), c_in), carry = OR(AND(a,b), AND(c_in, XOR(a,b))). All 8 input combinations correct. Reports gate counts per evaluation.
- **Efficiency analysis** (Part E): expression complexity (nodes/gates per operation), linear depth scaling verified (NOT chain), timing benchmarks, crossing count analysis (NOT=6, NAND=5, AND=11, XOR=15, full adder sum up to 30 crossings).

### Theorems/Conjectures
- **Convergence thesis**: the 8th root of unity (5pi/4) is where three phenomena align: Demo 15's classification angles, Demo 18's magic gate angles, and cyclotomic integer bracket values
- Gate composition preserves correctness through cascade thresholding (each gate produces a classical bit that feeds as braid input to the next gate)
- Gate count scales linearly with expression depth (verified with NOT chains of depth 1,2,4,8)

### Data
- NOT gate truth table: input=0 → high amplitude, input=1 → low amplitude (separation > 0.001)
- NAND gate truth table: (0,0),(0,1),(1,0) → high, (1,1) → low (separation > 0.001)
- Full adder: 8 input combinations, sum+carry gate counts, max gates per input
- Timing: full adder evaluations in ms
- Root-of-unity catalog: 5 root families x all angles, NOT/NAND working counts

### Code Assets
- **Expression tree compiler**: `Expr` type with `expr_var()`, `expr_not()`, `expr_nand()` constructors + derived `expr_and()`, `expr_or()`, `expr_xor()`
- **Arena-based expression pool** (`EXPR_POOL_SIZE=1024`): simple allocation with `expr_pool_reset()` — no free needed
- **`eval_expr()`**: recursive evaluator that thresholds bracket amplitudes at each gate, tracks gate count and max depth
- **`expr_size()`**: counts total expression tree nodes
- **Gate evaluation helpers**: `gate_output()` (1-input), `gate_output_2bit()` (2-input), plus `_reduced` variants that divide by |delta|^k
- **`make_gate_braid()`**: concatenates input word + gate word into a single braid
- **`test_not_at_angle()` / `test_nand_at_angle()`**: evaluate gate separation at arbitrary angle, fill Gate struct with threshold/separation
- Reuses: complex arithmetic, braid state-sum oracle, union-find

### Literature Touched
- Universality of {NOT, NAND} gate set for classical boolean logic
- Roots of unity as algebraic sweet spots for Kauffman bracket evaluation
- Cyclotomic integers and exact bracket values at roots of unity
- Full adder circuit decomposition into primitive gates

### Open Questions
- How does gate fidelity degrade as circuit depth increases? (cascade thresholding error accumulation)
- Can the compiler be extended to multi-bit operations (e.g., N-bit ripple-carry adder)?
- Is there a more crossing-efficient XOR gate than the 3-NAND decomposition?
- Does the convergence thesis hold for higher-strand braids (beyond 2-3 strands)?
