# arbor2_glr_tabula.c Worklog

## 2026-01-13: Bitwise Operators - Partial Implementation

### What was done:
1. Added 3 new non-terminals to enum: `PIPA_BITWISE`, `CARET_BITWISE`, `AMPERSAND_BITWISE`
2. Added `INT_NT_*` defines for these (27, 28, 29)
3. Added 6 new rules (P100-P105) for the bitwise operators
4. Modified P95/P96 descriptions (coniunctio -> pipa_bitwise instead of aequalitas)
5. Added PIPA and CARET entries to basic states (1-5) with REDUCE actions

### What's still needed:
The bitwise operators require new precedence levels inserted between `&&` and `==`. This is more complex than adding operators at existing levels because:

1. **New states needed** for "after PIPA_BITWISE", "after CARET_BITWISE", "after AMPERSAND_BITWISE"
2. **New GOTO entries** for all expression-starting states (0, 6, 26, 31, etc.) to the new states
3. **Chain reductions** through pass-through rules (P105 -> P103 -> P101) when seeing lower-precedence operators
4. **SHIFT states** for after each bitwise operator (similar to state 254 for `&&`)

### Estimated remaining work:
- ~6 new states (2 per operator: one for "after NT", one for "after OP")
- ~20-30 new GOTO entries
- ~50-100 new action entries across existing states
- Testing and debugging

### Alternative approach considered:
Could simplify by putting all bitwise ops at the same precedence level (non-standard but simpler). This would avoid the precedence chain complexity.

### Current status:
- All existing tests pass (725 tests)
- `a | b` and similar bitwise expressions do NOT parse yet
- The PIPA/CARET entries added so far just REDUCE through basic expression states

### Clone tool output saved:
```
./glr_quaestio.sh clone DUAMPERSAND PIPA
```
Shows 27 states that need PIPA entries (based on DUAMPERSAND pattern), but the SHIFT targets need to be different.

## 2026-01-13: Bitwise Operators - Complete Implementation

### What was completed:

1. **Added AMPERSAND to basic states (1-5)**: Previously only PIPA/CARET were added. AMPERSAND needed REDUCE entries too.

2. **Fixed state 264 and 239**: Added PIPA, CARET, AMPERSAND entries to continue the reduction chain:
   - State 264 (after TRANSLATIO): REDUCE P92 on bitwise operators
   - State 239 (after COMPARATIO): REDUCE P87 on bitwise operators

3. **Added states 268-288**: These handle the bitwise operator precedence chain:
   - 268/269: After AMPERSAND_BITWISE / after & operator
   - 270/271: After CARET_BITWISE / after ^ operator
   - 272/273: After PIPA_BITWISE / after | operator
   - 274-288: Various nested context states

4. **Added GOTO entries** to all expression-context states (0, 26, 33, 35, 42, 45, 63, 77, 82, 86, 89) for bitwise NTs

5. **Unary operators ~ and !**:
   - Added rules P106 (factor -> '~' factor) and P107 (factor -> '!' factor)
   - Created states 289/290 for ~ handling
   - Created states 291/292 for ! handling
   - Updated all expression-starter states to shift ~/! to 289/291 instead of the old broken state 10

### Parsing now works:
- `a & b` - bitwise AND
- `a | b` - bitwise OR
- `a ^ b` - bitwise XOR
- `a & b | c` → `(a & b) | c` (correct precedence)
- `a | b & c` → `a | (b & c)` (correct precedence)
- `a | b && c` → `(a | b) && c` (| binds tighter than &&)
- `a & ~b` - bitwise AND with unary NOT

### Statistics:
- Total states: 293 (was 268)
- Total rules: 108 (was 106)
- All 725 parser tests pass
- All 75 project tests pass

## 2026-01-13: glr_quaestio Tool Improvements

### New Commands Added:

Based on lessons learned from implementing bitwise operators, added three new commands to `tools/glr_quaestio.c`:

1. **`checklist`** - Complete checklist for adding a new precedence level
   - Lists all expression-context states needing GOTO entries
   - Documents basic states 1-5 needing REDUCE entries
   - Shows the full precedence chain diagram
   - Documents the NT mapping switch requirement (CRITICAL!)
   - Explains unary operator handling (dedicated states, not state 10)
   - Provides file checklist and testing commands

2. **`expr-states`** - Lists all expression-context states
   - Finds states with GOTO(EXPR) entries
   - Shows descriptions from STATUS_TABULA_PARTIAL
   - Found 41 such states in current grammar

3. **`chain-states`** - Lists reduction chain states
   - Finds "after NT" states from descriptions
   - Shows states that may need REDUCE entries for new operators
   - Found 64 such states

### Key Insights Documented:

1. **Expression-context states**: Not just state 0! All states that can start an expression need GOTO entries for new NTs: 0, 6, 26, 31, 33, 35, 40, 42, 45, 48, 54, 57, 60, 63, 70, 77, 80, 82, 84, 86, 89, etc.

2. **Reduction chain**: States 264 (after TRANSLATIO) and 239 (after COMPARATIO) needed REDUCE entries for bitwise operators to chain properly.

3. **NT mapping switch**: `arbor2_glr_quaerere_goto` has a switch that maps ARBOR2_NT_* to INT_NT_*. Missing cases cause GOTO lookups to return -1!

4. **Unary operators**: Need dedicated states with GOTO(FACTOR). State 10 is binary multiplication context - NOT for unary ops!

5. **Basic states 1-5**: Always need REDUCE entries for any new binary operator.

### Usage:
```bash
./glr_quaestio.sh checklist      # Full guide for adding precedence levels
./glr_quaestio.sh expr-states    # List states needing GOTO entries
./glr_quaestio.sh chain-states   # List reduction chain states
```

## 2026-01-13: Assignment and Comma Operators - Complete Implementation

### New Precedence Chain:
```
VIRGA (,)              <- NEW: lowest precedence, left-associative
  |
  v
ASSIGNATIO (= += etc)  <- NEW: right-associative!
  |
  v
DISIUNCTIO (||)        <- existing
  ... rest unchanged ...
```

### What was completed:

1. **Added 2 new non-terminals**: `ARBOR2_NT_VIRGA` (comma) and `ARBOR2_NT_ASSIGNATIO`
2. **Added 14 new rules (P108-P121)**:
   - P108: `virga -> virga ',' assignatio` (left-associative comma)
   - P109: `virga -> assignatio`
   - P110-P120: Assignment operators (`=`, `+=`, `-=`, `*=`, `/=`, `%=`, `&=`, `|=`, `^=`, `<<=`, `>>=`)
   - P121: `assignatio -> disiunctio`

3. **Created 6 new states (293-298)**:
   - 293: after assignment operator - expects expression starters
   - 294: after 'disiunctio = assignatio' - reduce P110
   - 295: after ASSIGNATIO - handle comma or reduce P109
   - 296: after VIRGA - end of expression
   - 297: after comma - expects expression starters
   - 298: after 'virga , assignatio' - reduce P108

4. **Modified State 255**: Changed ACCEPT to REDUCE P121 on EOF/SEMICOLON/COMMA, added SHIFT on all 11 assignment operators to state 293

5. **Added GOTO entries** for ASSIGNATIO and VIRGA to all 17+ expression-context states

6. **Added REDUCE entries** for assignment operators to all reduction chain states:
   - Basic states 1-5
   - State 239 (after COMPARATIO)
   - State 240 (after AEQUALITAS)
   - State 253 (after CONIUNCTIO)
   - State 264 (after TRANSLATIO)
   - States 268, 270, 272 (bitwise chain)

### Right-Associativity Implementation:
For assignment operators, we SHIFT instead of REDUCE when seeing the same-precedence operator:
- Left-assoc `a + b + c`: After `a + b`, REDUCE → `((a + b) + c)`
- Right-assoc `a = b = c`: After `a = b`, SHIFT to parse RHS first → `(a = (b = c))`

### Key Debugging Insight:
When adding new operators at the lowest precedence level (above existing operators), ALL reduction chain states need REDUCE entries for the new operators to trigger the chain. The debug output `GOTO(state, IGNOTUM) = -1` when reducing meant the NT wasn't mapped in the switch statement. `NO ACTIONS - path dies` on a chain state meant that state needed REDUCE entries for the new operator tokens.

### Parsing now works:
- `a = 1` - simple assignment
- `a = b = c` → `a = (b = c)` (right-associative)
- `a += 1`, `a -= 1`, etc. - all compound assignments
- `a, b` - comma expression
- `a, b, c` → `((a, b), c)` (left-associative)
- `a = 1, b = 2` → `(a = 1), (b = 2)` (comma binds looser than assignment)
- `a = b || c` → `a = (b || c)` (assignment below ||)

### Statistics:
- Total states: 299 (was 293)
- Total rules: 122 (was 108)
- All 783 parser tests pass
- All 75 project tests pass

## 2026-01-13: glr_quaestio Tool - Dynamic Checklist & Filtering

### Changes Made:

1. **Dynamic checklist command**: The `checklist` command now dynamically:
   - Finds expression-context states via GOTO(EXPR) lookup (32 states)
   - Finds expression chain states by matching descriptions (42 states)
   - Displays both lists inline instead of static hardcoded text

2. **`chain-states --expr` filtering**: Added optional `--expr` flag to filter chain states to only expression-precedence-related states (42 vs 64 total)

3. **Case-insensitive matching**: The expression chain state finder now handles both uppercase (ASSIGNATIO, VIRGA) and lowercase (comparatio, disiunctio) state descriptions, plus English aliases (expression, term, factor)

4. **Cross-references between commands**: Added "See also" sections:
   - `expr-states` → references `checklist` and `chain-states`
   - `chain-states` → references `checklist` and `expr-states`
   - `checklist` → has TIP about `chain-states --expr`

### Helper Functions Added:
- `str_tolower()` - Converts string to lowercase
- `collectare_expr_chain_states()` - Finds expression chain states with case-insensitive pattern matching

### Usage:
```bash
./glr_quaestio.sh checklist           # Dynamic checklist with inline state lists
./glr_quaestio.sh chain-states        # All 64 chain states
./glr_quaestio.sh chain-states --expr # Only 42 expression chain states
./glr_quaestio.sh expr-states         # 32 expression-context states
```

### Key Insight:
State descriptions use a mix of Latin (ASSIGNATIO, VIRGA, etc.) and English (expression, term, factor) names. The `EXPR_CHAIN_NT_NOMINA` array contains both variants for comprehensive matching.

## 2026-01-13: Bug Found - Assignment in Condition Contexts

### Discovery:
The deterministic chain-states tool found a real bug: `if (x = 1) y;` fails to parse because the if-condition context states were never updated for assignment operators.

### Root Cause:
When assignment/comma operators were added, only "top-level" expression states were updated. Context-specific states (if condition, while condition, etc.) were missed.

### What was fixed for if-condition:
1. **State 32** (after EXPR in if): Added REDUCE P99 for all 11 assignment operators
2. **State 246** (after COMPARATIO in if): Added REDUCE P87 for assignment operators
3. **State 247** (after AEQUALITAS in if): Added REDUCE P105 for assignment operators
4. **State 31 GOTO**: Added missing bitwise NT entries (AMPERSAND_BITWISE, CARET_BITWISE, PIPA_BITWISE)
5. **State 299** (new): Created "after VIRGA in if condition" state with PAREN_CLAUSA -> SHIFT 33
6. **State 31 GOTO(VIRGA)**: Changed from 296 to 299

### Remaining work:
Similar fixes needed for:
- While condition (state 40, 41, 248, etc.)
- For condition (states 48, 249, 250, etc.)
- Do-while condition (state 49, 251)
- Switch condition (state 81, 252)

### Validation:
The deterministic `chain-states --expr` approach correctly identified that these context states ARE chain states (via GOTO targets). The tool finding 61 states instead of 12 was accurate - the extra states are context-specific variants that all need updating.

### Statistics:
- Total states: 300 (was 299)
- All 784+ parser tests pass

## 2026-01-13: All Condition Context States Fixed

### Overview:
Completed fixing assignment operators in ALL condition contexts: while, for (init/condition/increment), do-while, and switch.

### Fixes Applied:

1. **While condition** (states 40, 41, 248, 300):
   - State 41: Added REDUCE P99 for all 11 assignment operators
   - State 40 GOTO: Added bitwise NTs (AMPERSAND_BITWISE, CARET_BITWISE, PIPA_BITWISE)
   - State 248: Added REDUCE P105 for assignment operators
   - State 300 (new): "after VIRGA in while condition" - SHIFT ) to 42

2. **For loop** (states 54, 55, 57, 58, 60, 61, 249, 250, 301-303):
   - State 55: Expanded from just SEMICOLON to full expression handling with assignment operators
   - State 58: Added REDUCE P99 for assignment operators
   - State 61: Expanded from just PAREN_CLAUSA to full expression handling with assignment operators
   - State 54 GOTO: Added full chain NTs including bitwise, VIRGA -> 301
   - State 57 GOTO: Added bitwise NTs, VIRGA -> 302
   - State 60 GOTO: Added full chain NTs including bitwise, VIRGA -> 303
   - State 249: Added REDUCE P87 for assignment operators
   - State 250: Changed from REDUCE P28 to REDUCE P105 for proper chain handling
   - States 301-303 (new): Context-specific VIRGA states for for-init/condition/increment
   - **New Rule P122**: `expr_opt -> virga` - allows full expression chain in for-loop contexts

3. **Do-while condition** (states 48, 49, 251, 304):
   - State 49: Added REDUCE P99 for all 11 assignment operators
   - State 48 GOTO: Added bitwise NTs, VIRGA -> 304
   - State 251: Added REDUCE P105 for assignment operators
   - State 304 (new): "after VIRGA in do-while condition" - SHIFT ) to 50

4. **Switch condition** (states 80, 81, 252, 305):
   - State 81: Added REDUCE P99 for all 11 assignment operators
   - State 80 GOTO: Added bitwise NTs, VIRGA -> 305
   - State 252: Added REDUCE P105 for assignment operators + == and != shifts
   - State 305 (new): "after VIRGA in switch condition" - SHIFT ) to 82

### Key Pattern:
Each condition context requires:
1. "after EXPR" state needs REDUCE P99 for assignment operators
2. GOTO table needs bitwise NTs (AMPERSAND_BITWISE, CARET_BITWISE, PIPA_BITWISE)
3. Context-specific AEQUALITAS state needs REDUCE P105 for assignment operators
4. Context-specific VIRGA state (299-305) to handle ) correctly

### Statistics:
- Total states: 306 (was 300)
- Total rules: 123 (was 122, added P122)
- All 793 parser tests pass

### Parsing now works:
```c
if (x = 1) y;           // if condition
while (x = 1) y;        // while condition
for (a = 1; b; c) d;    // for init
for (a; b = 1; c) d;    // for condition
for (a; b; c = 1) d;    // for increment
do x; while (a = 1);    // do-while condition
switch (a = 1) { }      // switch condition
```

## 2026-01-13: Ternary Conditional Operator - Complete Implementation

### Overview:
Added the ternary conditional operator (`?:`) to the expression grammar. The ternary operator is **right-associative** and sits between assignment and logical-or in precedence.

### Precedence Chain (Updated):
```
VIRGA (,)              <- lowest precedence, left-associative
  |
  v
ASSIGNATIO (= += etc)  <- right-associative
  |
  v
TERNARIUS (? :)        <- NEW: right-associative!
  |
  v
DISIUNCTIO (||)        <- existing
  |
  v
... rest unchanged ...
```

### Grammar Rules Added (P123-P124):
```c
/* P123 */ { ARBOR2_NT_TERNARIUS, 5, ARBOR2_NODUS_TERNARIUS, "ternarius -> disiunctio '?' ternarius ':' ternarius" },
/* P124 */ { ARBOR2_NT_TERNARIUS, 1, ARBOR2_NODUS_ERROR, "ternarius -> disiunctio" },
```

### Changes Made:

1. **Header file (arbor2_glr.h)**:
   - Added `ARBOR2_NT_TERNARIUS` to non-terminal enum
   - Added `ARBOR2_NODUS_TERNARIUS` to node type enum
   - Added `ternarius` union member with `conditio`, `verum`, `falsum` pointers

2. **Parse tables (arbor2_glr_tabula.c)**:
   - Added `INT_NT_TERNARIUS` define (32)
   - Added 2 rules (P123, P124) to REGULAE[]
   - Modified assignment rules (P110-P121) to use `ternarius` instead of `disiunctio`
   - Created states 306-310 for ternary parsing:
     - 306: after `?` - expects expression starters
     - 307: after `disiunctio ? ternarius` - expects `:`
     - 308: after `:` - expects expression starters
     - 309: after `disiunctio ? ternarius : ternarius` - reduce P123
     - 310: after TERNARIUS - assignment or reduce P121
   - Added GOTO(TERNARIUS) to all expression-context states
   - Added `?` and `:` tokens to chain states for reduction triggering

3. **Parser core (arbor2_glr.c)**:
   - Added NT mapping case for ARBOR2_NT_TERNARIUS
   - Added AST construction for ARBOR2_NODUS_TERNARIUS
   - **CRITICAL FIX**: Modified reduction code to explore ALL predecessor paths
     - Previously only followed `praedecessores[ZEPHYRUM]`
     - Now loops over all predecessors for merged nodes
     - Required for correct GLR parsing with merges

4. **Debug tool (glr_debug.c)**:
   - Added display case for ARBOR2_NODUS_TERNARIUS

5. **NT name functions**:
   - Added all missing NT names (PIPA_BITWISE, CARET_BITWISE, AMPERSAND_BITWISE, TRANSLATIO, VIRGA, ASSIGNATIO, TERNARIUS)
   - Debug output now shows actual NT names instead of "IGNOTUM"

### GLR Multi-Predecessor Bug Fixed:
When two parse paths merge (same state, same value), the merged node has multiple predecessors. Previously, reductions only followed the first predecessor, causing one valid path to be ignored. The fix adds a loop over all predecessors, creating reduction results for each path.

Example: In `a ? b : c`, after parsing `c`:
- Label path: state 77 (for `b:` as label)
- Ternary path: state 308 (for `b` as true-branch)
Both paths merge at state 4 (after identifier). Now both are explored during reduction.

### Known Ambiguity:
`a ? b : c` parses as ambiguous when `b` could be a label:
- Ternary interpretation: `a ? b : c`
- Label interpretation: `a ? (b: c)`

Using numeric values avoids this: `a ? 1 : c` parses unambiguously as ternary.

### Parsing now works:
```c
a ? 1 : c           // simple ternary
a ? 1 : b ? 2 : 3   // nested ternary (right-associative)
                    // parses as: a ? 1 : (b ? 2 : 3)
a = x ? 1 : 2       // assignment with ternary (ternary binds tighter)
```

### Statistics:
- Total states: 311 (was 306)
- Total rules: 125 (was 123)
- All 801 parser tests pass
