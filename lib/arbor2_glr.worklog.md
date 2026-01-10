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

- for statements (Phase C3)
- switch/case/default + jump statements (Phase C4)
- Test with real library files
- Error recovery

---

## 2026-01-10 (Milestone 3 Phase C2: while/do-while Statements)

### Phase C2: While/Do-While Statements Complete

Added while and do-while loop statements:

**New Grammar Rules:**
- P22: `statement -> while_statement` (pass-through)
- P23: `while_statement -> 'while' '(' expression ')' statement` (5 symbols)
- P24: `statement -> do_statement` (pass-through)
- P25: `do_statement -> 'do' statement 'while' '(' expression ')' ';'` (7 symbols)

**New Node Types:**
- `ARBOR2_NODUS_DUM` - While statement
- `ARBOR2_NODUS_FAC` - Do-while statement

**New Non-Terminals:**
- `ARBOR2_NT_DUM`
- `ARBOR2_NT_FAC`

**New AST Structure:**
Both while and do-while share the `iteratio` struct with `conditio` and `corpus` fields.

**New States (39-52):**
- States 39-44: While parsing (`while` -> `(` -> expr -> `)` -> stmt -> reduce)
- States 45-52: Do-while parsing (`do` -> stmt -> `while` -> `(` -> expr -> `)` -> `;` -> reduce)

**Key Design Decisions:**

1. **Shared iteratio struct**: Both while and do-while use the same struct since they both have a condition and a body.

2. **State reuse**: States 40 and 48 (after `(`) reuse expression parsing states via GOTO entries for EXPR/TERM/FACTOR.

3. **Body state reuse**: States 42 and 45 (expecting body) need full statement parsing capability including nested control flow, so they have GOTO entries for SENTENTIA, CORPUS, SI, DUM, FAC.

4. **Nested compound handling**: Uses state 38 for compound bodies to ensure proper reduction before continuing.

**Tests Passing:**
- `while (x) y;` → DUM with conditio=x, corpus=y
- `while (x) { a; b; }` → DUM with CORPUS as corpus
- `do x; while (y);` → FAC with corpus=x, conditio=y
- `do { a; b; } while (c);` → FAC with CORPUS as corpus
- `while (a) while (b) c;` → Nested DUM

**Grammar Now:**
- 53 states (was 39)
- ~367 action entries (was ~249)
- 26 grammar rules (was 22)

---

## 2026-01-10 (Milestone 3 Phase C3: for Statements)

### Phase C3: For Statements Complete

Added for loop statements with optional expression handling:

**New Grammar Rules:**
- P26: `statement -> for_statement` (pass-through)
- P27: `for_statement -> 'for' '(' expr_opt ';' expr_opt ';' expr_opt ')' statement` (9 symbols)
- P28: `expression_opt -> expression` (pass-through)
- P29: `expression_opt -> ε` (epsilon, produces NULL)

**New Node Type:**
- `ARBOR2_NODUS_PER` - For statement

**New Non-Terminals:**
- `ARBOR2_NT_PER`
- `ARBOR2_NT_EXPRESSIO_OPTATIVA`

**New AST Structure:**
The `circuitus` struct contains:
- `initium`: init expression (NULL if omitted)
- `conditio`: condition expression (NULL if omitted)
- `incrementum`: increment expression (NULL if omitted)
- `corpus`: loop body

**New States (53-65):**
- State 53: After `for` - expect `(`
- State 54: After `for (` - parse init or epsilon reduce on `;`
- State 55: After `for ( expression` - reduce P28 on `;`
- State 56: After `for ( expr_opt` - expect `;`
- State 57: After `for ( expr_opt ;` - parse condition or epsilon reduce on `;`
- State 58: After `for ( expr_opt ; expression` - reduce P28 on `;`
- State 59: After `for ( expr_opt ; expr_opt` - expect `;`
- State 60: After `for ( expr_opt ; expr_opt ;` - parse increment or epsilon reduce on `)`
- State 61: After `for ( expr_opt ; expr_opt ; expression` - reduce P28 on `)`
- State 62: After `for ( expr_opt ; expr_opt ; expr_opt` - expect `)`
- State 63: After `for ( ... )` - parse body statement
- State 64: After `for ( ... ) stmt` - reduce P27
- State 65: After for_statement - reduce P26

**Key Design Decisions:**

1. **Optional expressions via expression_opt**: Instead of complex grammar rules, we use a simple `expression_opt` non-terminal with epsilon production. This keeps the stack depth uniform for all for-loop variants.

2. **Epsilon reductions at specific lookaheads**:
   - State 54: When `;` seen immediately, reduce P29 (empty init)
   - State 57: When `;` seen immediately, reduce P29 (empty condition)
   - State 60: When `)` seen immediately, reduce P29 (empty increment)

3. **9-symbol production P27**: The for-statement rule has 9 symbols, the longest in our grammar. Stack layout:
   - valori[8] = 'for' token
   - valori[7] = '(' token
   - valori[6] = expr_opt (init, may be NULL)
   - valori[5] = ';' token
   - valori[4] = expr_opt (condition, may be NULL)
   - valori[3] = ';' token
   - valori[2] = expr_opt (increment, may be NULL)
   - valori[1] = ')' token
   - valori[0] = statement

4. **Internal NT constants**: Added INT_NT_PER (12) and INT_NT_EXPRESSIO_OPT (13) for GOTO lookups.

**Tests Passing:**
- `for (i; c; n) x;` → PER with all 4 parts
- `for (; c; n) x;` → PER with initium=NULL
- `for (i; ; n) x;` → PER with conditio=NULL
- `for (i; c; ) x;` → PER with incrementum=NULL
- `for (;;) x;` → PER with all optional parts NULL
- `for (i; c; n) { a; b; }` → PER with CORPUS as corpus
- `for (;;) for (;;) x;` → Nested PER

**Grammar Now:**
- 66 states (was 53)
- ~500+ action entries (was ~367)
- 30 grammar rules (was 26)

### Still TODO

- switch/case/default + jump statements (Phase C4)
- Test with real library files
- Error recovery

---

## 2026-01-10 (Milestone 3 Phase C4a: Jump Statements)

### Phase C4a: Jump Statements Complete

Added the four C89 jump statements: break, continue, return, goto.

**New Grammar Rules:**
- P30: `statement -> 'break' ';'` (2 symbols)
- P31: `statement -> 'continue' ';'` (2 symbols)
- P32: `statement -> 'return' expr_opt ';'` (3 symbols, reuses P28/P29)
- P33: `statement -> 'goto' IDENTIFIER ';'` (3 symbols)

**New Node Types:**
- `ARBOR2_NODUS_FRANGE` - break statement (no additional data needed)
- `ARBOR2_NODUS_PERGE` - continue statement (no additional data needed)
- `ARBOR2_NODUS_REDDE` - return statement with optional `reditio.valor`
- `ARBOR2_NODUS_SALTA` - goto statement with `saltus.destinatio` label

**New States (66-76):**
- State 66: After `break` - expect `;`
- State 67: After `break ;` - reduce P30
- State 68: After `continue` - expect `;`
- State 69: After `continue ;` - reduce P31
- State 70: After `return` - parse expression or epsilon reduce on `;`
- State 71: After `return expression` - can continue expr with `+` or reduce P28
- State 72: After `return expr_opt` - expect `;`
- State 73: After `return expr_opt ;` - reduce P32
- State 74: After `goto` - expect IDENTIFIER
- State 75: After `goto IDENTIFIER` - expect `;`
- State 76: After `goto IDENTIFIER ;` - reduce P33

**Key Design Decisions:**

1. **Reusing expression_opt**: The `return` statement reuses the existing P28/P29 rules for optional expressions. This avoids duplicating grammar infrastructure.

2. **State 70 expression starters**: State 70 needs shift actions for all expression starters (ID, INT, `(`, `*`, `&`) to handle `return value;`, plus epsilon reduce on `;` for `return;`.

3. **GOTO entries for state 70**: When expressions are parsed after `return`, reductions need GOTO(70, EXPR/TERM/FACTOR/EXPRESSIO_OPT) entries to transition correctly.

**Bug Fixed:**

**Infinite loop in `{ return x + 1; }`**: State 71 (after `return expression`) originally only had an action for SEMICOLON (reduce P28). When the parser saw PLUS after parsing `x`, it had no valid action and got stuck looping. Fixed by adding `{ ARBOR2_LEXEMA_PLUS, ARBOR2_ACTIO_SHIFT, 9 }` to state 71, allowing binary expressions like `x + 1` to continue parsing.

**Tests Passing:**
- `break;` → FRANGE
- `continue;` → PERGE
- `return;` → REDDE with valor=NULL
- `return x;` → REDDE with valor=IDENTIFICATOR(x)
- `goto label;` → SALTA with destinatio="label"
- `while (x) break;` → DUM with FRANGE as corpus
- `{ return x + 1; }` → CORPUS containing REDDE with BINARIUM

**Grammar Now:**
- 77 states (was 66)
- 628 action entries (was ~500+)
- 34 grammar rules (was 30)

### Still TODO

- Labeled statements (Phase C4b): `identifier: statement`
- switch/case/default (Phase C4c)
- Test with real library files
- Error recovery
