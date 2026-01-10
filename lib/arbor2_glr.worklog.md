# arbor2_glr.c worklog

## 2026-01-09

### Initial implementation complete

Created the basic GLR parser infrastructure:
- `arbor2_glr.h`: Core types (GSS nodes, action/goto tables, AST nodes)
- `arbor2_glr_tabula.c`: Hand-written LR tables for expression grammar
- `arbor2_glr.c`: GLR parser core algorithm
- `probatio_arbor2_glr.c`: Test suite

### Key design decisions

1. **Separated reduction from shift processing**: The original design had reductions adding directly to `frons_activa` during iteration, causing an infinite loop. Fixed by using a separate `reducenda` queue that processes reductions until exhausted, then shifts are processed.

2. **Non-terminal separation**: The grammar needed three distinct NTs for the expression hierarchy:
   - `ARBOR2_NT_EXPRESSIO` (expression level)
   - `ARBOR2_NT_TERMINUS` (term level for `*` precedence)
   - `ARBOR2_NT_FACTOR` (factor level for atoms and unary ops)

3. **Pass-through rule handling**: For 3-symbol pass-through rules like `(expr)`, the middle value (index 1) must be returned, not the rightmost value (index 0).

4. **Accepting node tracking**: The ACCEPT action happens during reduction processing, not in `frons_activa`, so we track `nodus_acceptatus` to return the correct final AST value.

### Bugs fixed

- **Infinite loop**: Reduction adding to `frons_activa` while iterating over it
- **Wrong NT mappings**: Rules P3/P4 (term productions) incorrectly used `ARBOR2_NT_EXPRESSIO` instead of `ARBOR2_NT_TERMINUS`
- **Wrong accepting node**: Returning `frons_activa[0]` instead of the node that reached ACCEPT
- **Parenthesized expressions returning null**: Taking `valori[0]` (`)`) instead of `valori[1]` (the inner expression)

### Tests passing

- Simple identifier: `foo`
- Simple integer: `42`
- Binary addition: `1 + 2`
- Operator precedence: `1 + 2 * 3` parses as `1 + (2 * 3)`
- Parenthesized: `(1 + 2)`
- Unary dereference: `*ptr`
- Unary address-of: `&x`

### Still TODO

- Add ambiguous states to grammar tables (IDENTIFIER could start expr or decl)
- Action filtering in `_processare_unam_actionem()` to prune using typedef info
- Error recovery

---

## 2026-01-09 (continued)

### Fork/Merge/Typedef Pruning Infrastructure Complete

Added infrastructure for handling C89 ambiguities like `foo * bar`:

**Fork tracking:**
- Added `furca_id` field to `Arbor2GSSNodus` to group forked paths
- When multiple actions found, `num_furcae` is incremented

**Merge mechanism:**
- `_mergere_compatibiles()`: Scans frontier for nodes with same state
- `_nodi_aequales()`: Compares AST nodes for equality (shallow)
- `_creare_nodum_ambiguum()`: Creates `ARBOR2_NODUS_AMBIGUUS` holding multiple interpretations
- `_copiare_praedecessores()`: Copies predecessor links when merging
- Merge is called in `_processare_actiones()` after processing each token

**Typedef pruning API:**
- `_potest_esse_typus()`: Checks if identifier is registered typedef via `arbor2_expansion_est_typedef()`
- `_macro_suggerit_typum()`: Checks if macro expands to type keyword
- `arbor2_glr_est_probabiliter_typus()`: Public API combining both checks

**Grammar extensions:**
- Added rules P10-P12 for declarations (type_specifier, declarator)
- Added `ARBOR2_NT_DECLARATIO` and `ARBOR2_NT_DECLARATOR` non-terminals
- Added `ARBOR2_NODUS_DECLARATIO` and `ARBOR2_NODUS_DECLARATOR` node types

**Note:** The infrastructure is in place but the current action tables don't have ambiguous states (each state/token pair has exactly one action). To test actual fork/merge/pruning, we need to add states where IDENTIFIER can start either expression or declaration.

### New tests

- Typedef detection: `MyType` registered as typedef is correctly detected
- Non-typedef: `unknown` is correctly identified as not a typedef
- `foo * bar`: Parses as multiplication with current expression-only grammar

---

## 2026-01-09 (Milestone 3: Ambiguous Grammar)

### Ambiguous Grammar States Added

Extended the LR tables to create actual ambiguity for `IDENTIFIER *`:

**State 4 (after IDENTIFIER)** now has TWO actions for `*`:
1. REDUCE P5 (factor → ID) - expression path
2. SHIFT S17 - declaration path

**New states added:**
- State 17: After `*` in declarator path
- State 18: After declarator name (reduce P12)
- State 19: After `* declarator` (reduce P11)
- State 20: After `type_specifier declarator` (reduce P10)
- State 21: Accept declaration

### Multiple Accept Handling

Fixed GLR to properly handle multiple accepting paths:
- Collect ALL accepting nodes in `acceptati` array instead of stopping at first
- Continue processing reductions even after first accept
- If multiple paths accept with different values, create AMBIGUUS node

### AST Construction for Declarations

Added proper AST node construction for:
- `ARBOR2_NODUS_DECLARATIO`: type_specifier + declarator
- `ARBOR2_NODUS_DECLARATOR`: handles `*` chains and identifier name

### Typedef Pruning Integrated

In `_processare_unam_actionem()`, when multiple actions found:
- Check if node holds identifier that is known typedef via `arbor2_glr_est_probabiliter_typus()`
- If typedef, filter out REDUCE actions (expression path)
- Keep only SHIFT actions (declaration path)
- Pruning happens before fork count increment

### Test Results

- `foo * bar` (unknown identifier) → AMBIGUUS with 2 interpretations, furcae: 1
- `MyType * ptr` (registered typedef) → DECLARATIO only, furcae: 0

### Still TODO

- Add more statement types (if, while, for, compound, etc.)
- Test with real library files
- Error recovery

---

## 2026-01-09 (Milestone 3 continued: Expression Statements)

### Phase A: Expression Statements Complete

Added support for basic C89 statements:

**New Grammar Rules:**
- P13: `statement -> expression ';'`
- P14: `statement -> ';'` (empty statement)

**New Node Types:**
- `ARBOR2_NODUS_SENTENTIA` - Expression statement (wrapper around expression)
- `ARBOR2_NODUS_SENTENTIA_VACUA` - Empty statement

**New Non-Terminal:**
- `ARBOR2_NT_SENTENTIA`

**Implementation:**
1. Added SEMICOLON action to State 0 (shift to state 23 for empty statement)
2. Added SEMICOLON action to State 1 (shift to state 22 after expression)
3. Added SEMICOLON to reduce sets of states 2, 3, 4, 5, 12, 13, 14, 15, 16
4. Added State 22: After `expression ;` → reduce P13
5. Added State 23: After lone `;` → reduce P14
6. Added State 24: Accept state after statement
7. Added GOTO[0, SENTENTIA] = 24

**Key Fix:**
Initial implementation hung because states 2-5, 12-16 didn't have SEMICOLON in their reduce sets. The parser couldn't see how to continue after building an expression. Added SEMICOLON to all states that can precede the end of an expression.

**Tests Passing:**
- `x + 1;` → SENTENTIA containing BINARIUM
- `;` → SENTENTIA_VACUA
- `42;` → SENTENTIA containing INTEGER

**Grammar Now:**
- 25 states (was 22)
- 103 action entries (was 94)
- 15 grammar rules (was 13)

---

## 2026-01-09 (Milestone 3 Phase B: Compound Statements)

### Phase B: Compound Statements Complete

Added support for compound statements `{ stmt1; stmt2; ... }`:

**New Grammar Rules:**
- P15: `statement -> compound_statement` (pass-through)
- P16: `compound_statement -> '{' statement_list '}'`
- P17: `statement_list -> statement_list statement` (left-recursive)
- P18: `statement_list -> ε` (epsilon for empty list)

**New Node Type:**
- `ARBOR2_NODUS_CORPUS` - Compound statement containing `Xar* sententiae`

**New Non-Terminals:**
- `ARBOR2_NT_CORPUS`
- `ARBOR2_NT_ELENCHUS_SENTENTIARUM`

**New States:**
- State 25: After `{` - epsilon reduce P18 for all lookaheads
- State 26: After `{ statement_list` - expect more statements or `}`
- State 27: After `{ statement_list }` - reduce P16
- State 28: After statement in list - reduce P17
- State 29: After compound_statement - accept or reduce P15

**Key Design Decisions:**

1. **Left-recursive statement_list**: The rule `list -> list stmt` is left-recursive, which is natural for LR parsing. The epsilon production `list -> ε` provides the base case.

2. **Epsilon reduction strategy**: State 25 (after `{`) immediately does an epsilon reduction to create an empty statement_list. This transitions to state 26 where we can add statements or close with `}`.

3. **Reusing expression states**: States 2, 3, 4, 5, etc. for expression parsing are reused inside compound statements. The GOTO entries determine flow based on the originating state (0 vs 26).

4. **BRACE_CLAUSA in reduce sets**: Just like we added SEMICOLON to expression reduce states for Phase A, we added BRACE_CLAUSA (`}`) to states 2-5, 12-16 so expressions can complete inside compound statements.

5. **Statement starters in reduce sets**: States 22 and 23 (after statement completes) now reduce on all statement starters (ID, INT, `;`, `{`, etc.) so we can continue parsing the next statement in a list.

**GOTO Entries Added:**
- GOTO(0, CORPUS) = 29
- GOTO(25, ELENCHUS) = 26
- GOTO(26, EXPR) = 1 (reuse)
- GOTO(26, TERM) = 2 (reuse)
- GOTO(26, FACTOR) = 3 (reuse)
- GOTO(26, SENTENTIA) = 28
- GOTO(26, CORPUS) = 29

**Tests Passing:**
- `{ }` → CORPUS with 0 statements
- `{ x; }` → CORPUS with 1 statement
- `{ x; y + 1; }` → CORPUS with 2 statements
- `{ { x; } }` → Nested CORPUS

**Grammar Now:**
- 30 states (was 25)
- 169 action entries (was 103)
- 19 grammar rules (was 15)

### Still TODO

- Control flow statements (if, while, for, etc.)
- Test with real library files
- Error recovery

---

## 2026-01-10 (Milestone 3 Phase C1: if/else Statements)

### Phase C1: If/Else Statements Complete

Added if/else statements with proper dangling-else resolution:

**New Grammar Rules:**
- P19: `statement -> if_statement` (pass-through)
- P20: `if_statement -> 'if' '(' expression ')' statement` (5 symbols)
- P21: `if_statement -> 'if' '(' expression ')' statement 'else' statement` (7 symbols)

**New Node Type:**
- `ARBOR2_NODUS_SI` - If statement with conditio, consequens, alternans

**New Non-Terminal:**
- `ARBOR2_NT_SI`

**New States:**
- State 30: After `if` - expect `(`
- State 31: After `if (` - parse condition expression
- State 32: After `if ( expr` - expect `)`
- State 33: After `if ( expr )` - parse then-branch statement
- State 34: After `if ( expr ) stmt` - dangling else resolution
- State 35: After `if ( expr ) stmt else` - parse else-branch statement
- State 36: After `if ( expr ) stmt else stmt` - reduce P21
- State 37: After if_statement - reduce P19 (stmt -> if_stmt)
- State 38: Nested compound statement - always reduces P15 (never accepts)

**Key Design Decisions:**

1. **Dangling Else Resolution**: In state 34 (after parsing if-body), when we see `else`, we prefer SHIFT (continuing to P21) over REDUCE (completing P20). This causes `else` to bind to the nearest `if`, matching C89 semantics.

2. **Separate State for Nested Compounds**: State 38 is like state 29 but without the EOF accept action. When compound statements appear as if/else bodies, they go through state 38 which reduces P15 (statement -> compound_statement), ensuring the if statement gets the SENTENTIA and can complete.

3. **GOTO Entries for Expression Parsing**: States 31, 33, and 35 need GOTO entries for EXPR, TERM, FACTOR to handle expressions (condition and expression-statements in bodies).

**Bugs Fixed:**

- **Infinite loop at semicolon**: States 33 and 35 were missing GOTO entries for EXPR/TERM/FACTOR, so after parsing expression in the body, reductions couldn't find their next state.

- **Wrong result for compound body**: `if (x) { y; }` was returning CORPUS instead of SI because GOTO(33, CORPUS) = 29, and state 29 accepts on EOF. Fixed by creating state 38 which always reduces P15.

**Tests Passing:**
- `if (x) y;` → SI with conditio=x, consequens=y, alternans=NULL
- `if (x) y; else z;` → SI with conditio=x, consequens=y, alternans=z
- `if (x) { y; }` → SI with CORPUS as consequens
- `if (a) if (b) c; else d;` → Nested SI, else binds to inner if

**Grammar Now:**
- 39 states (was 30)
- ~249 action entries (was 169)
- 22 grammar rules (was 19)

### Still TODO

- while/do-while statements (Phase C2)
- for statements (Phase C3)
- switch/case/default + jump statements (Phase C4)
- Test with real library files
- Error recovery
