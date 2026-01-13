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
