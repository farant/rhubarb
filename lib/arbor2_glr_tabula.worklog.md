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

## 2026-01-14: Phase 1.1b - Identifier (typedef) Casts

### Overview:
Added support for casting to identifier/typedef types: `(TypeName)x`, `(TypeName*)ptr`, `(TypeName**)pp`.

### Changes Made:

1. **State 6 GLR Fork**: Added GLR fork for IDENTIFICATOR token
   - On ID: Fork to state 4 (expression path) AND state 407 (cast path)
   - Uses VERUM flag on both actions to enable parallel parsing
   - The expression path `(x)` will die when followed by another identifier
   - The cast path `(TypeName)x` will survive and produce CONVERSIO

2. **New States 407-415** (ID cast states):
   - 407: after `( ID` - expects `)` or `*`
   - 408: after `( ID )` - expects factor
   - 409: after `( ID * )` - expects factor
   - 410: after `( ID *` - expects `)` or `*`
   - 411: after `( ID * *` - expects `)`
   - 412: after `( ID * * )` - expects factor
   - 413-415: reduce states for P147, P157, P161

3. **GOTO entries** added for states 408, 409, 412 (factor -> reduce state)

4. **New States 416-421** (sizeof(ID) states - DISABLED):
   - State machine exists but sizeof(ID) path in state 388 is disabled
   - See "Known Limitation" below

### Productions Used:
- P147: `factor -> '(' ID ')' factor` - basic ID cast
- P157: `factor -> '(' ID '*' ')' factor` - ID pointer cast
- P161: `factor -> '(' ID '*' '*' ')' factor` - ID double pointer cast

### Known Limitation - sizeof(ID) Ambiguity:

The sizeof(ID) case (`sizeof(MyType)`) is inherently ambiguous in C:
1. `sizeof(type)` - the identifier is a typedef
2. `sizeof(expr)` - the identifier is a variable in parentheses

When both parse paths survive, the GLR parser creates an AMBIGUUS node. However:
- The test suite expects SIZEOF nodes for `sizeof(x)` (the old behavior)
- There appears to be a bug when processing subsequent operations on AMBIGUUS nodes

**Workaround**: sizeof(ID) path in state 388 is disabled. Use:
- `sizeof(MyType*)` - unambiguous, works correctly (pointer type can only be a cast)
- `sizeof x` - also works (no parentheses, so no ambiguity)

### Test Results:
- All 1173 tests pass (was 1165, added 8 new ID cast tests)
- New tests:
  - `(TypeName)x` - accepts CONVERSIO or AMBIGUUS
  - `(TypeName*)ptr` - expects CONVERSIO with num_stellae=1
  - `(TypeName**)pp` - expects CONVERSIO with num_stellae=2

### Statistics:
- Total states: 422 (was 407)
- Total rules: 173 (unchanged - ID productions P147, P157, P161 already existed)
- Total tests: 1173

### Future Work (Part 2):
- Add struct/union/enum casts starting at state 422
- `(struct foo*)ptr`, `sizeof(struct foo)`, etc.

## 2026-01-14: Phase 1.1b Part 2 - Struct/Union/Enum Casts

### Overview:
Added support for casting to struct/union/enum types: `(struct foo)x`, `(struct foo*)ptr`, `(union bar)u`, `(enum color*)ptr`, plus corresponding sizeof forms.

### Key Insight - No GLR Fork Needed:
Unlike ID casts (Part 1), struct/union/enum casts do NOT require GLR fork because:
- Keywords `struct`, `union`, `enum` definitively indicate a type context
- There's no ambiguity: `(struct foo)x` can only be a cast, not a parenthesized expression
- All shifts use FALSUM flag (deterministic)

### Productions Added (P174-P191):
```c
/* Struct casts */
P174: factor -> '(' STRUCT ID ')' factor (5 symbols)
P175: factor -> '(' STRUCT ID '*' ')' factor (6 symbols)
P176: factor -> '(' STRUCT ID '*' '*' ')' factor (7 symbols)

/* Union casts */
P177-P179: same pattern for UNION

/* Enum casts */
P180-P182: same pattern for ENUM

/* sizeof(struct/union/enum) */
P183-P185: factor -> 'sizeof' '(' STRUCT ID [*]* ')' (5-7 symbols)
P186-P188: factor -> 'sizeof' '(' UNION ID [*]* ')' (5-7 symbols)
P189-P191: factor -> 'sizeof' '(' ENUM ID [*]* ')' (5-7 symbols)
```

### States Added (422-472):
Total: 51 new states

**Struct cast states (422-431):**
- 422: after `( struct` - expects ID
- 423: after `( struct ID` - expects `)` or `*`
- 424: after `( struct ID )` - expects factor
- 425-431: subsequent states for *, **, reduce

**Union cast states (432-441):** Same pattern

**Enum cast states (442-451):** Same pattern

**sizeof(struct/union/enum) states (452-472):** Same pattern

### State Modifications:
- **State 6** (after `(`): Added deterministic shifts
  - STRUCT -> 422, UNION -> 432, ENUM -> 442
- **State 388** (after `sizeof (`): Added deterministic shifts
  - STRUCT -> 452, UNION -> 459, ENUM -> 466

### GOTO Tables Added:
New GOTO arrays for factor-expects states:
- STATUS_424_GOTO, STATUS_427_GOTO, STATUS_430_GOTO (struct)
- STATUS_434_GOTO, STATUS_437_GOTO, STATUS_440_GOTO (union)
- STATUS_444_GOTO, STATUS_447_GOTO, STATUS_450_GOTO (enum)

### AST Handler Updates (arbor2_glr.c):

The CONVERSIO and SIZEOF handlers needed updates to handle the different symbol counts for struct/union/enum:
- Basic type casts: 4=base, 5=*, 6=** (formula: num_stellae = num_pop - 4)
- Struct/union/enum: 5=base, 6=*, 7=** (formula: num_stellae = num_pop - 5)

The handlers now check the production number to determine which formula to use:
- P174-P182 (casts): num_stellae = num_pop - V
- P183-P191 (sizeof): num_stellae = num_pop - V

### Test Results:
- All 1205 tests pass (was 1173, added 32 new tests)
- New tests verify:
  - `(struct foo)x` - CONVERSIO node
  - `(struct foo*)ptr` - CONVERSIO with num_stellae=1
  - `(struct foo**)pp` - CONVERSIO with num_stellae=2
  - `(union bar)u`, `(union bar*)ptr`
  - `(enum color)c`, `(enum color*)ptr`
  - `sizeof(struct foo)`, `sizeof(struct foo*)`, `sizeof(struct foo**)`
  - `sizeof(union bar)`, `sizeof(union bar*)`
  - `sizeof(enum color)`, `sizeof(enum color*)`

### Statistics:
- Total states: 473 (was 422)
- Total rules: 191 (was 173, added 18)
- Total tests: 1205

### Phase 1.1 Summary:
- Part 1a: Basic type pointer casts `(int*)` - COMPLETE
- Part 1b Part 1: ID/typedef casts `(TypeName*)` - COMPLETE
- Part 1b Part 2: Struct/union/enum casts `(struct foo*)` - COMPLETE
- Future: Array types in sizeof `sizeof(int[10])` - deferred to Phase 1.1c

## 2026-01-14: Phase 1.2a - Simple Expression Initializers

### Goal:
Add support for initialized declarations with expression initializers:
```c
int x = 5;          // constant initializer
int y = a + b;      // expression initializer
int z = x * 2 + 1;  // complex expression
```

Brace initializers (`{1,2,3}`) deferred to Phase 1.2b.

### Changes Made:

#### 1. AST Structure (arbor2_glr.h):
Added `initializor` field to ARBOR2_NODUS_DECLARATIO:
```c
structura {
    Arbor2Nodus*        specifier;
    Arbor2Nodus*        declarator;
    Arbor2Nodus*        initializor;    /* NEW: expression or NIHIL */
    b32                 est_typedef;
    i32                 storage_class;
    i32                 qualifiers;
} declaratio;
```

Updated all 17 DECLARATIO node creation sites to initialize `initializor = NIHIL`.

#### 2. Grammar (arbor2_glr_tabula.c):

**New Production:**
- P192: `decl -> type declarator '=' assignatio` (4 symbols)

**New States:**
- State 473: after `type declarator =` - expects expression starters (SHIFT to states 4, 5, 6, 7, 8, etc.)
- State 474: after `type declarator = assignatio` - REDUCE P192

**State Modifications:**
- State 20 (after `type declarator`): Added ASSIGNATIO SHIFT to 473
- State 116 (after `type_spec name`): Added ASSIGNATIO REDUCE P12

**GOTO Entries:**
- STATUS_473_GOTO: Maps all expression NTs, with ASSIGNATIO -> 474

#### 3. AST Handler (arbor2_glr.c):
Added P192 handler before the default P10 case:
```c
alioquin si (actio->valor == 192)
{
    /* P192: decl -> type declarator '=' assignatio */
    /* Create specifier from lexemata[III], get declarator from valori[II],
       get initializer expression from valori[ZEPHYRUM] */
    valor_novus->datum.declaratio.initializor = init_expr;
}
```

### Key Insight:
State 116 (direct declarator after type_spec name) needed ASSIGNATIO REDUCE P12 so the declarator reduces before the `=` is processed. Without this, the parser died at state 116 seeing `=`.

### Tests Added:
- `int x = 5` - constant initializer, verifies INTEGER node
- `int y = a + b` - expression initializer, verifies BINARIUM node
- `int z = x * 2 + 1` - complex expression

### Statistics:
- Total states: 475 (was 473, added 2)
- Total rules: 192 (was 191, added 1)
- Total tests: 1217 (was 1205, added 12)

### Remaining for Phase 1.2a:
- Storage class variants (static, extern, register, auto, const, volatile) with initializers
- Productions P193-P198, states 475-486

### Future (Phase 1.2b):
- Brace initializers: `int arr[] = {1, 2, 3};`
- Designated initializers: `struct Point p = {.x = 1, .y = 2};`


## 2026-01-14: Phase 1.2a Complete - Storage Class Initializers

### What was completed:
Added initializer support for all storage class and qualifier variants.

#### New Productions (P193-P198):
- P193: `decl -> 'static' type declarator '=' assignatio` (5 symbols)
- P194: `decl -> 'extern' type declarator '=' assignatio`
- P195: `decl -> 'register' type declarator '=' assignatio`
- P196: `decl -> 'auto' type declarator '=' assignatio`
- P197: `decl -> 'const' type declarator '=' assignatio`
- P198: `decl -> 'volatile' type declarator '=' assignatio`

#### New States (475-486):
- 475/476: static initializer (expression-expects / reduce)
- 477/478: extern initializer
- 479/480: register initializer
- 481/482: auto initializer
- 483/484: const initializer
- 485/486: volatile initializer

#### State Modifications:
- States 358-363 (after `storage/qual type declarator`): Added ASSIGNATIO SHIFT to 475, 477, 479, 481, 483, 485
- State 18 (after ID in declarator): Added ASSIGNATIO REDUCE P12

#### AST Handler:
Combined handler for P193-P198 using range check:
```c
alioquin si (actio->valor >= 193 && actio->valor <= 198)
{
    /* Set storage_class or qualifiers based on production number */
    si (actio->valor == 193) storage = ARBOR2_STORAGE_STATIC;
    alioquin si (actio->valor == 194) storage = ARBOR2_STORAGE_EXTERN;
    /* ... etc ... */
}
```

### Key Insight:
State 18 needed ASSIGNATIO reduce P12 (same fix as state 116) because the storage class paths go through state 18 after the identifier before reaching states 358-363.

### Tests Added:
- `static int s = 10`
- `extern int e = 20`
- `register int r = 30`
- `auto int a = 40`
- `const int c = 50`
- `volatile int v = 60`
- `static int sum = a + b` (expression with storage class)

### Statistics:
- Total states: 487 (was 475, added 12)
- Total rules: 198 (was 192, added 6)
- Total tests: 1246 (was 1217, added 29)

### Phase 1.2a Summary:
Parser now supports initialized declarations in all forms:
- `int x = 5`
- `int y = a + b`
- `static int z = 10`
- `const int c = 100`
- etc.

### Future (Phase 1.2b):
- Brace initializers: `int arr[] = {1, 2, 3};`
- Designated initializers: `struct Point p = {.x = 1, .y = 2};`

## 2026-01-14: Phase 1.2b - Brace Initializers

### What was implemented:
Brace initializer support for declarations:
- `int a[] = {1}`
- `int b[] = {1, 2, 3}`
- `int c[] = {}`  (empty list)
- `int d[] = {1, 2,}` (trailing comma)
- `int m[2][2] = {{1, 2}, {3, 4}}` (nested braces)
- `int x = {5}` (single value in braces)
- Storage class variants: `static int s[] = {10, 20}`
- Qualifier variants: `const int c[] = {30, 40}`

### New AST Types (arbor2_glr.h):
1. `ARBOR2_NT_INITIALIZOR_LISTA` - Non-terminal for initializer list
2. `ARBOR2_NT_INIT_ITEMS` - Non-terminal for list items (internal)
3. `ARBOR2_NT_INITIALIZER` - Non-terminal for single initializer
4. `ARBOR2_NODUS_INITIALIZOR_LISTA` - Node type with `items` field (Xar of nodes)

### New Productions (P199-P212):
- P199: `decl -> type declarator '=' init_lista`
- P200: `init_lista -> '{' '}'` (empty)
- P201: `init_lista -> '{' init_items '}'`
- P202: `init_lista -> '{' init_items ',' '}'` (trailing comma)
- P203: `init_items -> initializer` (single)
- P204: `init_items -> init_items ',' initializer` (multiple)
- P205: `initializer -> assignatio` (expression)
- P206: `initializer -> init_lista` (nested braces)
- P207-P212: Storage class/qualifier + brace initializer variants

### New States (487-499):
- State 487: After `{` in initializer - parse expressions or nested braces
- States 488-492: Handle `}`, items, comma, trailing comma
- States 493-496: Reduction states for initializer/items
- State 497: After init_lista in basic decl context (P199)
- State 498: After init_lista in static decl context (P207)
- State 499: After init_lista in const decl context (P211)

### Key State Modifications:
1. State 218 (unsized array `[]`): Added ASSIGNATIO reduce for `int a[] = {...}`
2. State 220 (sized array `[n]`): Added ASSIGNATIO reduce for `int m[2][2] = {...}`
3. State 255 (after disiunctio): Added BRACE_CLAUSA reduce for expressions inside braces
4. State 310 (after ternarius): Added BRACE_CLAUSA reduce
5. States 358, 362 (static/const + type + name): Added BRACKET_APERTA for array support
6. States 473, 475, 483 (after `=`): Added BRACE_APERTA shift to state 487
7. States 475, 483: Added INITIALIZOR_LISTA GOTO entries

### Pattern Used:
Followed the existing ARGUMENTA pattern for comma-separated lists:
- Left-recursive grammar: `items -> item | items ',' item`
- NODUS_ERROR for pass-through rules (P203-P206)
- Actual nodes only for outer structure (INITIALIZOR_LISTA)

### Tests Added:
- Basic brace initializer (1 element)
- Multiple values
- Empty braces
- Trailing comma
- Nested braces (2D array)
- Static + brace
- Const + brace
- Single value in braces (not array)
- Verify expression initializers still work

### Statistics:
- Total states: 500 (was 487, added 13)
- Total rules: 213 (was 198, added 14)
- Total tests: 1296 (was 1246, added 50)

### Future (Phase 1.2c):
- Designated initializers: `.field = value`, `[index] = value`

## 2026-01-14: Phase 1.2c - Designated Initializers

### Summary
Added full support for C89/C99 designated initializers:
- Field designators: `.field = value`
- Array index designators: `[index] = value`
- Chained designators: `[0].x`, `.a.b`, `[0][1]`
- Mixed positional and designated: `{1, .b = 2, 3}`
- Nested brace values: `{.a = {1, 2}}`

### Key Fix: Expression Context for Array Designators
The critical issue was that array designators like `[5]` need an expression inside brackets,
followed by `]`. The standard expression states don't know about `]` as a valid follow token.

**Problem**: State 500 (after `[`) was routing expressions through generic states:
- `GOTO(500, EXPR) = 1` (generic)
- Generic state 1 continues reducing to TRANSLATIO -> ... -> ASSIGNATIO
- These states don't have `]` in their action sets

**Solution**: Follow the pattern used by subscript expressions (state 312):
1. Changed `GOTO(500, EXPR) = 501` (custom state)
2. Expanded state 501 to handle both `]` and expression operators (like state 313)
3. State 501 shifts `]` to 502, or continues expression with operators

### Productions Added (P213-P220)
- P213: `designator -> '[' expr ']'` (array index)
- P214: `designator -> '.' IDENTIFIER` (field)
- P215: `designator_list -> designator` (single)
- P216: `designator_list -> designator_list designator` (chain)
- P217: `designator_item -> designator_list '=' initializer` (with expression)
- P218: `init_items -> designator_item` (first designated)
- P219: `init_items -> init_items ',' designator_item` (additional)
- P220: `designator_item -> designator_list '=' init_lista` (nested braces)

### States Added (500-512)
- 500: after `[` - expression starter shifts
- 501: after `[ expr` - expects `]` or operators
- 502: after `[ expr ]` - reduce P213
- 503: after `.` - expects IDENTIFIER
- 504: after `. ID` - reduce P214
- 505: after DESIGNATOR - reduce P215
- 506: after DESIGNATOR_LIST - chain or `=`
- 507: after `=` - expression or nested brace
- 508: after DESIGNATOR in chain - reduce P216
- 509: after INITIALIZER in designator - reduce P217
- 510: after INIT_LISTA in designator - reduce P220
- 511: after DESIGNATOR_ITEM (first) - reduce P218
- 512: after additional DESIGNATOR_ITEM - reduce P219

### GOTO Entries
- States 487, 491: Added `DESIGNATOR`, `DESIGNATOR_LIST`, `DESIGNATOR_ITEM`
- State 500: Custom `EXPR -> 501` (not generic 1)
- State 506: `DESIGNATOR -> 508` for chaining
- State 507: Full expression chain + `INIT_LISTA -> 510`

### Lesson Learned
When adding expression contexts that end with non-standard tokens (like `]`), the GOTO for
EXPRESSIO must route to a custom state that handles both the terminator AND expression
continuation operators. Can't use generic expression states that don't know the context.

### Statistics
- Total states: 513 (was 500, added 13)
- Total rules: 221 (was 213, added 8)
- Total tests: 1331 (was 1296, added 35)

## 2026-01-14: Phase 1.3 - Multiple Declarators

### Goal
Support comma-separated declarators in a single declaration:
```c
int x, y;                // Multiple simple declarators
int x = 1, y = 2;        // Multiple with initializers
int *x, y;               // Mixed pointer/simple (ambiguous, expected)
```

### Approach
Following the parameter list pattern (P41-P42, states 101-104), we introduced:
- `INIT_DECLARATOR`: wraps a declarator with optional initializer
- `INIT_DECLARATOR_LIST`: comma-separated list of init_declarators

### Key Changes to Existing States
1. **State 20** (after `type declarator`): Changed from REDUCE P10 to REDUCE P221, added COMMA handling
2. **State 116** (after `type ID`): Added COMMA to reduction triggers
3. **State 474** (after `declarator '=' assignatio`): Changed from REDUCE P192 to P222
4. **State 497** (after `declarator '=' init_lista`): Changed from REDUCE P199 to P223

### Productions Added (P221-P226)
- P221: `init_decl -> declarator` (single, no init)
- P222: `init_decl -> declarator '=' assignatio` (with expression init)
- P223: `init_decl -> declarator '=' init_lista` (with brace init)
- P224: `init_decl_list -> init_decl` (first item)
- P225: `init_decl_list -> init_decl_list ',' init_decl` (additional items)
- P226: `declaratio -> type init_decl_list` (final declaration)

### States Added (513-528)
- 513: after type INIT_DECLARATOR - reduce P224
- 514: after type INIT_DECLARATOR_LIST - reduce P226 or shift COMMA
- 515: after `init_decl_list ','` - expect declarator
- 516-524: Declarator building in comma context
- 519: after `',' declarator '='` - expression context
- 520-522: Reduction states for initialized declarators
- 525-528: Array declarator support in comma context

### AST Handling
Modified `arbor2_glr.c` to handle the new flow:
1. P221/P222/P223 create a pair Xar `[declarator, initializor]`
2. P224 passes through the pair
3. P225 passes through (TODO: proper multi-declarator list)
4. P226 unpacks the pair into DECLARATIO fields

**Note**: Currently P225 only passes through the last init_decl, so multi-declarator
produces a single DECLARATIO for the last declarator. Full list support would require
emitting multiple DECLARATIO nodes from P226, which is more complex.

### Test Results
All 1331 existing tests pass. New parsing verified:
- `int x, y` ✓
- `int x = 1, y = 2` ✓
- `int *x, y` → AMBIGUUS (expected - expression vs declaration ambiguity)

### Statistics
- Total states: 529 (was 513, added 16)
- Total rules: 227 (was 221, added 6)
- Total tests: 1331 (unchanged - tests added separately)

## 2026-01-14: Phase 1.3 Fix - Full Multi-Declarator Support

### Problem
The initial Phase 1.3 implementation only kept the last declarator in a multi-declarator
statement. For `int x = 1, y = 2`, only `y = 2` was represented in the AST.

### Solution
Added a `proxima` (next) pointer to the DECLARATIO struct to chain declarations together.

#### Changes to include/arbor2_glr.h
Added `Arbor2Nodus* proxima;` field to declaratio struct.

#### Changes to lib/arbor2_glr.c

1. **P224 handler**: Now creates an Xar list containing the single pair (instead of passing through)
   ```c
   Xar* lista = xar_creare(glr->piscina, magnitudo(Xar*));
   Xar** slot = xar_addere(lista);
   *slot = (Xar*)valori[ZEPHYRUM];
   ```

2. **P225 handler**: Now appends the new pair to the existing list
   ```c
   Xar* lista = (Xar*)valori[II];
   Xar** slot = xar_addere(lista);
   *slot = (Xar*)valori[ZEPHYRUM];
   ```

3. **P226 handler**: Now iterates the list and creates chained DECLARATIO nodes
   - Creates type_spec from lexemata if valori[I] is NIHIL (simple type keyword)
   - Uses pre-built type_spec from valori[I] if present (struct/enum/union)
   - Loops through all pairs, creates DECLARATIO for each
   - Chains them via `proxima` pointer
   - Returns first node in chain

4. **All DECLARATIO creation sites**: Added `proxima = NIHIL` initialization (22 sites)

#### Changes to tools/glr_debug.c
Added DECLARATIO case to print specifier, declarator, initializor, and recursively print
chained declarations via proxima.

### Test Results
- All 1331 tests pass
- `int x, y` → two DECLARATIO nodes: x, y
- `int x = 1, y = 2` → two DECLARATIO nodes with INTEGER initializers
- `struct foo x, y` → two DECLARATIO nodes sharing STRUCT_SPECIFIER

### AST Output Example
For `int x = 1, y = 2`:
```
DECLARATIO
  IDENTIFICATOR "int"
  DECLARATOR
  INTEGER "1"
DECLARATIO
  IDENTIFICATOR "int"
  DECLARATOR
  INTEGER "2"
```

### Bug Fix: States 526/527 Wrong Production Numbers
During unit test addition, discovered that states 526 and 527 (array declarators in comma context)
had wrong production numbers:
- State 526: Had P94 (disiunctio rule), fixed to P80 (sized array declarator)
- State 527: Had P95 (coniunctio rule), fixed to P81 (unsized array declarator)

### Unit Tests Added
Added comprehensive multi-declarator tests:
- `int x, y` - simple two declarators
- `int x = 1, y = 2` - two with initializers
- `int x, y, z` - three declarators
- `int x = 1, y` - mixed (some with init)
- `struct foo x, y` - with struct type (verifies shared specifier)
- `int a[] = {1}, b[] = {2}` - arrays with brace initializers

### Final Statistics
- Total states: 529
- Total rules: 227
- Total tests: 1377 (was 1331, added 46 new tests)

## 2026-01-14: Phase 1.4 - Specifier Combinations

### Goal
Support combined storage class + qualifier declarations:
- `static const int x`
- `extern const int y = 5`
- `const volatile int z` (hardware registers)

### Productions Added (P227-P238)

| Prod | Pattern | Symbols |
|------|---------|---------|
| P227 | static const type decl | 4 |
| P228 | extern const type decl | 4 |
| P229 | register const type decl | 4 |
| P230 | auto const type decl | 4 |
| P231 | static volatile type decl | 4 |
| P232 | extern volatile type decl | 4 |
| P233 | const volatile type decl | 4 |
| P234 | volatile const type decl | 4 |
| P235 | static const type decl = assignatio | 6 |
| P236 | extern const type decl = assignatio | 6 |
| P237 | static const type decl = init_lista | 6 |
| P238 | extern const type decl = init_lista | 6 |

### New States (530-559)

| States | After | Purpose |
|--------|-------|---------|
| 530-537 | spec+spec | Expect type (INT/ID/CHAR/VOID) |
| 538-545 | spec+spec+type | Expect declarator (* or ID) |
| 546-553 | spec+spec+type+decl | Reduce or '=' for init |
| 554-555 | spec+spec+type+decl+= | Expect expression/init_lista |
| 556-559 | After init | Reduce P235-P238 |

### State Transitions Modified

Modified states 346-351 to add CONST/VOLATILE transitions:
- State 346 (after 'static'): CONST→530, VOLATILE→534
- State 347 (after 'extern'): CONST→531, VOLATILE→535
- State 348 (after 'register'): CONST→532
- State 349 (after 'auto'): CONST→533
- State 350 (after 'const'): VOLATILE→536
- State 351 (after 'volatile'): CONST→537

### AST Handler Changes (lib/arbor2_glr.c)

Added three new handler blocks:
1. **P227-P234**: 4-symbol combinations - sets both storage_class and qualifiers
2. **P235-P236**: 6-symbol with assignatio initializer
3. **P237-P238**: 6-symbol with brace initializer

### Bug Found: Missing State 529

STATUS_TABULA_PARTIAL is indexed by position, not state number. Skipping state 529 caused
all subsequent states (530+) to be off by one. Fixed by adding placeholder entry for state 529.

### Test Results
- All 1411 tests pass (was 1377, added 34 new tests)
- `static const int x` correctly sets STORAGE_STATIC + QUAL_CONST
- `const volatile int z` correctly sets QUAL_CONST | QUAL_VOLATILE
- `static const int x = 5` correctly creates declaration with initializer

### Final Statistics
- Total states: 560
- Total rules: 239
- Total tests: 1411
