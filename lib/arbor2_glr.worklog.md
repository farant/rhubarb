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

---

## 2026-01-10 (Milestone 3 Phase C4b: Labeled Statements)

### Phase C4b: Labeled Statements Complete

Added labeled statements for goto targets (`identifier: statement`):

**New Grammar Rule:**
- P34: `statement -> IDENTIFIER ':' statement` (3 symbols)

**New Node Type:**
- `ARBOR2_NODUS_TITULATUM` - Labeled statement with `titulatum.titulus` (label name) and `titulatum.sententia` (the labeled statement)

**New States (77-78):**
- State 77: After `IDENTIFIER :` - expects statement (all statement starters)
- State 78: After `IDENTIFIER : statement` - reduce P34

**Key Design Decisions:**

1. **Right-recursive through statement**: The rule `statement -> IDENTIFIER ':' statement` is right-recursive, allowing nested labels like `foo: bar: x;`. This naturally parses as `foo: (bar: (x;))`.

2. **Clean addition to state 4**: State 4 (after IDENTIFIER) already handles identifiers in various contexts (expression, declaration). Adding COLON as a shift action doesn't conflict with existing actions since COLON wasn't in state 4's action set.

3. **State 77 mirrors statement-expecting states**: State 77 has the same structure as states 33, 35, 42, 45, 63 (all states that expect a statement). It shifts on expression starters, control flow keywords, and braces.

4. **GOTO entries for state 77**: Full set of GOTO entries needed for all constructs that can appear as labeled statements (expressions, compounds, control flow).

**Implementation Details:**

- Added `ARBOR2_LEXEMA_COLON` shift to state 77 in state 4's actions
- State 77 has 15 actions for statement starters
- State 78 has 18 reduce actions on all statement followers
- 9 GOTO entries for state 77 covering EXPR, TERM, FACTOR, SENTENTIA, CORPUS, SI, DUM, FAC, PER

**Tests Passing:**
- `foo: x;` → TITULATUM("foo", SENTENTIA(x))
- `foo: ;` → TITULATUM("foo", SENTENTIA_VACUA)
- `foo: bar: x;` → TITULATUM("foo", TITULATUM("bar", SENTENTIA(x)))
- `foo: { x; }` → TITULATUM("foo", CORPUS)
- `foo: if (x) y;` → TITULATUM("foo", SI)
- `{ goto foo; foo: x; }` → CORPUS containing SALTA and TITULATUM

**Grammar Now:**
- 79 states (was 77)
- ~661 action entries (was 628)
- 35 grammar rules (was 34)

### Still TODO

- switch/case/default (Phase C4c)
- Test with real library files
- Error recovery

---

## 2026-01-10 (Milestone 3 Phase C4c: Switch/Case/Default)

### Phase C4c: Switch/Case/Default Complete

Added switch statement with case and default labels:

**New Grammar Rules:**
- P35: `statement -> 'switch' '(' expression ')' statement` (5 symbols)
- P36: `statement -> 'case' expression ':' statement` (4 symbols)
- P37: `statement -> 'default' ':' statement` (3 symbols)

**New Node Types:**
- `ARBOR2_NODUS_COMMUTATIO` - Switch statement with `selectivum.expressio` and `selectivum.corpus`
- `ARBOR2_NODUS_CASUS` - Case label with `electio.valor` (constant expr) and `electio.sententia`
- `ARBOR2_NODUS_ORDINARIUS` - Default label with `defectus.sententia`

**Note on naming:** The union field names differ from the node type names because `commutatio`, `casus`, and `ordinarius` are C89 keywords via `latina.h` macros.

**New States (79-90):**
- State 79: After `switch` - expect `(`
- State 80: After `switch (` - parse expression
- State 81: After `switch ( expr` - expect `)`
- State 82: After `switch ( expr )` - parse body statement
- State 83: After `switch ( expr ) stmt` - reduce P35
- State 84: After `case` - parse expression
- State 85: After `case expr` - expect `:`
- State 86: After `case expr :` - parse statement
- State 87: After `case expr : stmt` - reduce P36
- State 88: After `default` - expect `:`
- State 89: After `default :` - parse statement
- State 90: After `default : stmt` - reduce P37

**Key Design Decisions:**

1. **Case/default as statement variants**: In C89, `case` and `default` are labeled statement forms (like `identifier:`), not separate constructs. They can only appear inside switch bodies (semantic constraint, not syntactic).

2. **Fall-through semantics**: The grammar naturally supports fall-through because `case 1: case 2: x;` parses as `case 1: (case 2: (x;))` via right-recursion in the statement rule.

3. **Case expressions are general expressions**: While C89 requires constant expressions in case labels, we parse any expression and leave semantic validation for later.

**Bug Fixes During Implementation:**

1. **Infinite loop (initial)**: Tests killed after 33 seconds. Root cause: reduce states didn't have SWITCH/CASE/DEFAULT tokens in their reduce sets, and the main parse loop didn't clear `frons_activa` when no shifts were possible. Fixed by:
   - Adding SWITCH/CASE/DEFAULT to ~20 reduce states (22, 23, 27, 28, 29, 34, 36, 37, 38, 43, 44, 51, 52, 64, 65, 67, 69, 73, 76, 78, 83, 87, 90)
   - Modifying main loop to clear `frons_activa` and break when no shifts occur

2. **Case expressions failing**: After fixing infinite loop, `case` tests failed while `default` worked. Root cause: expression reduce states (2, 3, 4, 5, 12, 13, 14, 15, 16) didn't have COLON in their reduce sets. When parsing `case 1:`, the `1` couldn't reduce through factor→term→expression because there was no reduce action for COLON. Fixed by adding COLON reduce actions to all expression-related states.

3. **ACTIO_INDICES off-by-one**: State 4 had 8 actions in the comment but actually 9 (two COLON entries: one SHIFT for labels, one REDUCE for case expressions). This caused all subsequent state indices to be off by varying amounts. Fixed by recalculating all indices after adding COLON to states 2, 3, 4, 5, 12, 13, 14, 15, 16.

**Tests Passing:**
- `switch (x) { case 1: y; }` → COMMUTATIO with CORPUS containing CASUS
- `switch (x) { default: y; }` → COMMUTATIO with CORPUS containing ORDINARIUS
- `switch (x) { case 1: break; }` → COMMUTATIO with CASUS containing FRANGE
- `switch (x) { case 1: case 2: y; }` → Fall-through with nested CASUS
- `switch (x) { case 1 + 2: y; }` → CASUS with BINARIUM expression

**Grammar Now:**
- 91 states (was 79)
- ~891 action entries (was ~661)
- 38 grammar rules (was 35)

### Control Flow Grammar Complete

With Phase C4c, all C89 control flow constructs are now supported:
- Expression statements (`x;`, `;`)
- Compound statements (`{ stmt; stmt; }`)
- If/else statements
- While/do-while loops
- For loops
- Jump statements (break, continue, return, goto)
- Labeled statements (`label: stmt`)
- Switch/case/default

### Still TODO

- Function definitions and declarations
- More declaration specifiers (struct, union, enum, const, etc.)
- Array and function declarators
- Test with real library files
- Error recovery

---

## 2026-01-10 (Milestone 3 Phase D1: Function Declarators)

### Phase D1: Function Declarators Complete

Added function declarator syntax (`id()`, `id(void)`) as the first step toward function definitions:

**New Grammar Rules:**
- P38: `declarator -> declarator '(' ')'` (3 symbols, function with unspecified params)
- P39: `declarator -> declarator '(' VOID ')'` (4 symbols, function with no params)

**New Node Type:**
- `ARBOR2_NODUS_DECLARATOR_FUNCTI` - Function declarator with `declarator_functi` struct

**New AST Structure:**
```c
/* DECLARATOR_FUNCTI (function declarator) */
structura {
    Arbor2Nodus*        declarator_interior;  /* Inner declarator (name + pointers) */
    Xar*                parametri;            /* NULL for (), params for D2 */
    b32                 habet_void;           /* true if explicitly (void) */
    s32                 num_stellae;          /* Pointer depth (for * fn()) */
} declarator_functi;
```

**New States (91-94):**
- State 91: After `declarator (` - expect `)` or `void`
- State 92: After `declarator ( )` - reduce P38 on statement followers
- State 93: After `declarator ( void` - expect `)`
- State 94: After `declarator ( void )` - reduce P39 on statement followers

**Key Design Decisions:**

1. **State 18 reduces before shift**: Originally tried to shift `(` directly in state 18 (after IDENTIFIER), but P38 expects `declarator ( )`, not `IDENTIFIER ( )`. Fixed by having state 18 reduce P12 (IDENTIFIER → declarator) when it sees `(`, then state 19 handles the `(` shift.

2. **P11 preserves DECLARATOR_FUNCTI**: When parsing `* fn()`, P38 first creates DECLARATOR_FUNCTI for `fn()`, then P11 wraps it with `*`. Originally P11 always created DECLARATOR, overwriting the function declarator. Fixed by checking if inner is DECLARATOR_FUNCTI and preserving the genus while incrementing `num_stellae`.

3. **num_stellae field**: Added to DECLARATOR_FUNCTI to track pointer depth when P11 wraps function declarators. For `int *fn()`, the result is DECLARATOR_FUNCTI with num_stellae=1 (not a plain DECLARATOR wrapping DECLARATOR_FUNCTI).

**Implementation Details:**

Modified arbor2_glr.c P11 handling (lines 680-725):
```c
/* P11: declarator -> '*' declarator */
/* Si interior est DECLARATOR_FUNCTI, preservare genus functi */
si (valori[ZEPHYRUM] != NIHIL &&
    valori[ZEPHYRUM]->genus == ARBOR2_NODUS_DECLARATOR_FUNCTI)
{
    /* Wrap function declarator with pointer - preservare FUNCTI */
    valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
    valor_novus->genus = ARBOR2_NODUS_DECLARATOR_FUNCTI;
    /* ... copy declarator_interior, parametri, habet_void from inner ... */
    valor_novus->datum.declarator_functi.num_stellae =
        valori[ZEPHYRUM]->datum.declarator_functi.num_stellae + I;
}
```

**Tests Passing:**
- `MyType * fn()` → DECLARATIO with DECLARATOR_FUNCTI (habet_void=false)
- `MyType * fn(void)` → DECLARATIO with DECLARATOR_FUNCTI (habet_void=true)

**Grammar Now:**
- 95 states (was 91)
- ~900+ action entries (was ~891)
- 40 grammar rules (was 38)

### Still TODO

- Phase D3: Function definitions (`type fn() { body }`)
- More declaration specifiers (struct, union, enum, const, etc.)
- Array declarators `int arr[10]`
- Test with real library files
- Error recovery

---

## 2026-01-10 (Milestone 3 Phase D2: Parameter Lists)

### Phase D2: Parameter Lists Complete

Added typed parameter lists for function declarations:

**New Grammar Rules:**
- P40: `declarator -> declarator '(' parameter_list ')'` (4 symbols)
- P41: `parameter_list -> parameter_declaration` (1 symbol, starts list)
- P42: `parameter_list -> parameter_list ',' parameter_declaration` (3 symbols, extends list)
- P43: `parameter_declaration -> type_specifier declarator` (2 symbols)

**New Node Type:**
- `ARBOR2_NODUS_PARAMETER_DECL` - Parameter declaration with `parameter_decl.type_specifier` and `parameter_decl.declarator`

**New Non-Terminals:**
- `ARBOR2_NT_PARAMETER_LIST`
- `ARBOR2_NT_PARAMETER_DECL`

**New States (95-111):**
First parameter path (95-101):
- State 95: After `( type_spec` - expect `*` or param name
- State 96: After `( type_spec *` - more pointers or param name
- State 97: After `( type_spec name` - reduce P12 (declarator → ID)
- State 98: After `( type_spec *...* name` - reduce P12
- State 99: After `( type_spec declarator` - reduce P43 (param_decl)
- State 100: After `( type_spec * declarator` - reduce P11 (declarator → * declarator)
- State 101: After `( param_decl` - reduce P41 (param_list → param_decl)

Parameter list continuation (102-103):
- State 102: After `( param_list` - expect `)` or `,`
- State 103: After `( param_list )` - reduce P40

Subsequent parameters (104-111):
- State 104: After `param_list ,` - expect next type
- State 105-108: Same pattern as 95-98 for subsequent params
- State 109-111: Same pattern as 99-101 for subsequent params

**Key Bug Found:**

**Token mismatch for `int` keyword**: Initial tests failed with "NO ACTIONS" at state 91 for token `INT`. The issue was that `int` is a keyword (ARBOR2_LEXEMA_INT), not an identifier. State 91 only had an action for IDENTIFICATOR.

**Fix**: Added INT and CHAR keywords as valid type specifiers in states 91 and 104:
```c
/* State 91: After 'declarator (' - expect ')' or 'void' or parameter type */
{ ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT, 92 },   /* () */
{ ARBOR2_LEXEMA_VOID,           ARBOR2_ACTIO_SHIFT, 93 },   /* (void */
{ ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 95 },   /* param type (typedef) */
{ ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_SHIFT, 95 },   /* param type (int) */
{ ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_SHIFT, 95 },   /* param type (char) */
```

**AST Construction:**

P41 and P42 build an Xar of parameter nodes instead of wrapping in an AST node:
```c
/* P41: parameter_list -> parameter_declaration */
Xar* lista = xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
Arbor2Nodus** slot = xar_addere(lista);
*slot = valori[ZEPHYRUM];
valor_novus = (Arbor2Nodus*)lista;  /* Cast Xar* to Arbor2Nodus* */

/* P42: parameter_list -> parameter_list ',' parameter_declaration */
Xar* lista = (Xar*)valori[II];  /* Existing list */
Arbor2Nodus** slot = xar_addere(lista);
*slot = valori[ZEPHYRUM];  /* Append new param */
valor_novus = lista;
```

P40 stores the list in `declarator_functi.parametri`:
```c
/* P40: declarator -> declarator '(' parameter_list ')' */
valor_novus->datum.declarator_functi.parametri = (Xar*)valori[I];
```

**Tests Passing:**
- `MyType * fn(int x)` → DECLARATIO with DECLARATOR_FUNCTI with 1 param
- `MyType * fn(int x, int y)` → DECLARATOR_FUNCTI with 2 params
- `MyType * fn(int *ptr)` → DECLARATOR_FUNCTI with pointer param

**Grammar Now:**
- 112 states (was 95)
- ~950 action entries (was ~900)
- 44 grammar rules (was 40)

---

## 2026-01-10

### Phase D3: Function Definitions

Implemented function definition syntax, combining declarations with bodies:

```c
MyType fn(void) { }
MyType fn(void) { return x; }
MyType foo(int x, int y) { return x; }
MyType * bar(void) { }
```

**Grammar Rule Added:**
- P44: `function_definition -> type_specifier declarator compound_statement` (3 symbols)

**New Types:**
- `ARBOR2_NT_DEFINITIO_FUNCTI` - non-terminal for function definitions
- `ARBOR2_NODUS_DEFINITIO_FUNCTI` - AST node type
- `definitio_functi` struct with: specifier, declarator, corpus

**State Machine Changes:**

1. **State 4 (after IDENTIFIER):** Added IDENTIFIER → SHIFT 116 for direct declarators (`MyType fn` without `*`)

2. **State 20 (after type_specifier declarator):** Added:
   - `(` → SHIFT 91 (function params)
   - `{` → SHIFT 25 (function body)

3. **State 19 (after `* declarator`):** Added `{` → REDUCE P11 to allow `MyType * fn() {}`

4. **States 92, 94, 103 (after function declarator reductions):** Added `{` → REDUCE for function body support

5. **New States:**
   - State 113: After `type_spec declarator compound` → REDUCE P44
   - State 114: After function_definition → ACCEPT
   - State 116: After direct declarator (`type_spec name`) → REDUCE P12

6. **New GOTO entries:**
   - GOTO(20, CORPUS) = 113 (after function body)
   - GOTO(0, DEFINITIO) = 114 (accept function definition)

**Key Implementation Insight:**

The compound statement parsing (`{` to `}`) is reused from the existing statement grammar. The key was adding `{` as a valid lookahead in the function declarator reduction states (92, 94, 103, 19). When we see `{` after a function declarator:
1. Reduce the declarator (P38/P39/P40/P11)
2. GOTO to state 20
3. Shift `{` to state 25 (compound entry)
4. Parse compound statement, reduce P16 (CORPUS)
5. GOTO(20, CORPUS) = 113
6. State 113 reduces P44 (function_definition)
7. GOTO(0, DEFINITIO) = 114, ACCEPT

**Side Effect - Direct Declarators:**

Adding state 116 also enables simple declarations like `MyType x` (without pointer). This was previously only supported via the `*` path.

**Tests Passing:**
- `MyType fn(void) { }` → DEFINITIO_FUNCTI with empty body
- `MyType fn(void) { return x; }` → DEFINITIO_FUNCTI with return statement
- `MyType foo(int x, int y) { return x; }` → DEFINITIO_FUNCTI with 2 params
- `MyType * bar(void) { }` → pointer return function definition

**Grammar Summary:**
- 117 states (was 112)
- ~962 action entries (was ~950)
- 45 grammar rules (was 44)

### Debugging Notes

**ACTIO_INDICES Bug:** When adding actions to existing states, must carefully update all subsequent state indices. An off-by-one error in state 4's action count caused a segfault in the ambiguity handling.

**Missing Lookahead Bug:** The function declarator reduction states (92, 94, 103) originally didn't have `{` in their lookahead sets, causing parse failures for function definitions. Same issue affected state 19 (pointer declarator).

### Still TODO

- More declaration specifiers (struct, union, enum, const, etc.)
- Array declarators `int arr[10]`
- Multiple declarators in one declaration (`int a, b, c;`)
- Initializers (`int x = 5;`)
- Test with real library files
- Error recovery

---

## 2026-01-10 (Phase E1: Struct Definitions)

### Phase E1: Struct Definitions Complete

Added struct type specifier support:

```c
struct foo { int x; char y; };      /* Named struct definition */
struct foo x;                        /* Struct variable declaration */
struct { int x; } anon;             /* Anonymous struct */
struct foo * ptr;                   /* Pointer to struct */
```

**Grammar Rules Added (P45-P51):**
- P45: `struct_specifier -> 'struct' IDENTIFIER '{' struct_member_list '}'` (5 symbols)
- P46: `struct_specifier -> 'struct' '{' struct_member_list '}'` (4 symbols, anonymous)
- P47: `struct_specifier -> 'struct' IDENTIFIER` (2 symbols, forward ref)
- P48: `struct_member_list -> type_specifier IDENTIFIER ';'` (3 symbols, first member)
- P49: `struct_member_list -> struct_member_list type_specifier IDENTIFIER ';'` (4 symbols, append)
- P50: `struct_member_list -> type_specifier '*' IDENTIFIER ';'` (4 symbols, first pointer member)
- P51: `struct_member_list -> struct_member_list type_specifier '*' IDENTIFIER ';'` (5 symbols, append pointer)

**New Types:**
- `ARBOR2_NT_STRUCT_SPECIFIER` - non-terminal for struct specifiers
- `ARBOR2_NT_STRUCT_MEMBER_LIST` - non-terminal for member lists
- `ARBOR2_NODUS_STRUCT_SPECIFIER` - AST node type
- `struct_specifier` struct with: tag (Arbor2Nodus* for name or NULL), membra (Xar* of member declarations)

**State Machine Changes:**

Added ~20 new states (117-136):
- State 117: After `struct` - expect IDENTIFIER or `{`
- State 118: After `struct ID` - expect `{` or reduce P47 (forward ref)
- State 119: After `struct {` (anonymous) - start member list
- State 120: After `struct ID {` (named) - start member list
- States 121-126: First member path (non-pointer and pointer variants)
- States 127-130: After member list in anonymous/named struct
- States 131-136: Subsequent member path (for P49/P51 vs P48/P50)

**Key Design Decisions:**

1. **Separate paths for first vs subsequent members**: P48/P50 create a new Xar, while P49/P51 append to an existing Xar. This required separate state paths (121-126 for first member, 131-136 for subsequent) to ensure the correct reduction rule is applied.

2. **struct_specifier integrates with type_specifier**: After reducing struct_specifier, GOTO leads to the type_specifier path, allowing `struct foo { int x; } var;` to parse correctly.

3. **Members reuse declaration patterns**: Each struct member is a declaration ending with `;`. The member states parallel the regular declaration states but with `;` as the terminator.

**Key Bugs Fixed:**

1. **Wrong switch case for P48-P51**: The grammar rules specify `nodus = ARBOR2_NODUS_DECLARATIO` for P48-P51, which routed them to the generic DECLARATIO case instead of the custom handling in the `ordinarius` section. Fixed by adding P48-P51 handling at the start of the DECLARATIO case before the generic handling.

2. **Wrong valori/lexemata indexing**: The valori array is populated from top of stack (rightmost symbol at index 0) to bottom (leftmost symbol at highest index). Initial code had indices reversed. For P45 `struct ID { member_list }` (5 symbols):
   - valori[4] = struct
   - valori[3] = ID
   - valori[2] = {
   - valori[1] = member_list  ← this is what we need for membra
   - valori[0] = }

3. **Missing INT/CHAR handling in member states**: States expecting member type specifiers initially only handled IDENTIFICATOR, causing failures for `int` and `char` members.

**Tests Passing:**
- `struct foo x` → DECLARATIO with forward reference STRUCT_SPECIFIER
- `struct foo { int x; }` → STRUCT_SPECIFIER with 1 member
- `struct foo { int x; int y; }` → STRUCT_SPECIFIER with 2 members
- `struct foo { int x; } var` → DECLARATIO with struct type

**Grammar Summary:**
- 137 states (was 117)
- ~1020 action entries (was ~962)
- 52 grammar rules (was 45)

### Still TODO

- Enum support (different: constant list)
- Bit fields: `int x : 3;`
- Nested structs
- `typedef struct { } Foo;` pattern
- Test with real library files
- Error recovery

---

## 2026-01-10 (Phase E2: Union Definitions)

### Phase E2: Union Definitions Complete

Added union type specifier support (mirrors struct syntax exactly):

```c
union foo { int x; char y; };       /* Named union definition */
union foo x;                         /* Union variable declaration */
union { int x; } anon;              /* Anonymous union */
union foo * ptr;                    /* Pointer to union */
```

**Design Decision: est_unio Flag**

Rather than creating a separate union node type, added `est_unio` boolean to existing `struct_specifier`:

```c
structura {
    Arbor2Nodus*        tag;        /* Tag name or NIHIL */
    Xar*                membra;     /* Member declarations */
    b32                 est_unio;   /* VERUM for union, FALSUM for struct */
} struct_specifier;
```

**Rationale:**
- Minimal code changes (no new node type)
- Struct and union are semantically similar (aggregate types)
- Shared printing/traversal logic with a simple flag check
- Member list rules P48-P51 are reused for both

**Grammar Rules Added (P52-P54):**
- P52: `struct_specifier -> 'union' IDENTIFIER '{' struct_member_list '}'` (5 symbols)
- P53: `struct_specifier -> 'union' '{' struct_member_list '}'` (4 symbols, anonymous)
- P54: `struct_specifier -> 'union' IDENTIFIER` (2 symbols, forward ref)

Note: These produce `ARBOR2_NODUS_STRUCT_SPECIFIER` with `est_unio = VERUM`.

**New States (137-144):**
- State 137: After `union` - expect ID or `{`
- State 138: After `union ID` - expect `{` or reduce P54
- State 139: After `union {` (anonymous) - start member list
- State 140: After `union ID {` (named) - start member list
- State 141: After member_list in anonymous union - expect `}`
- State 142: After anonymous union `}` - reduce P53
- State 143: After member_list in named union - expect `}`
- State 144: After named union `}` - reduce P52

**State Reuse:**
States 139 and 140 share member parsing with struct states via GOTO entries. The same member states (121-136) handle both struct and union members. Only the final reduction states (141-144 for union vs 127-130 for struct) differ.

**GOTO Entries Added:**
```c
{ 139, INT_NT_STRUCT_MEMBERS, 141 },  /* anon union member list */
{ 140, INT_NT_STRUCT_MEMBERS, 143 },  /* named union member list */
{ 141, INT_NT_STRUCT_MEMBERS, 141 },  /* more anon union members */
{ 143, INT_NT_STRUCT_MEMBERS, 143 },  /* more named union members */
```

**AST Construction:**
In arbor2_glr.c, the STRUCT_SPECIFIER case now handles both struct and union:
- P45/P46/P47: `est_unio = FALSUM` (determined by `actio->valor != 52/53/54`)
- P52/P53/P54: `est_unio = VERUM` (determined by `actio->valor == 52/53/54`)

**Test Printing:**
Updated `_imprimere_arborem` to show "UNION_SPECIFIER" vs "STRUCT_SPECIFIER" based on `est_unio` flag.

**Tests Passing:**
- `union foo` → UNION_SPECIFIER (forward reference)
- `union foo { int x; }` → UNION_SPECIFIER with 1 member
- `union foo { int x; int y; }` → UNION_SPECIFIER with 2 members
- `union { int x; } anon` → DECLARATIO with anonymous union type

**Grammar Summary:**
- 145 states (was 137)
- ~1064 action entries (was ~1020)
- 55 grammar rules (was 52)

### Still TODO

- Bit fields: `int x : 3;`
- Nested structs/unions/enums
- `typedef struct { } Foo;` pattern
- Test with real library files
- Error recovery

---

## 2026-01-10 (Phase E3: Enum Definitions)

### Phase E3: Enum Definitions Complete

Added enum type specifier support:

```c
enum foo { A, B, C };                /* Named enum definition */
enum { A, B, C };                    /* Anonymous enum */
enum foo { A = 1, B, C = 10 };       /* With explicit values */
enum foo x;                          /* Forward reference / variable */
enum { A = 1 + 2 };                  /* Expression as value */
```

**Key Difference from Struct/Union:**
- Enumerators are comma-separated, not semicolon-terminated
- Enumerators are just identifiers with optional `= constant_expression`
- No nested type declarations, just simple name/value pairs

**Design Decision: Separate Node Types**

Unlike union (which reused struct_specifier with `est_unio` flag), enum gets dedicated node types:

```c
ARBOR2_NODUS_ENUM_SPECIFIER     /* enum tag { enumerators } */
ARBOR2_NODUS_ENUMERATOR         /* NAME or NAME = value */
```

**Rationale:**
- Enum semantics differ significantly from struct/union
- Enumerators have completely different structure (name + optional value)
- Separate handling is cleaner than overloading struct_specifier further

**Grammar Rules Added (P55-P61):**
- P55: `enum_specifier -> 'enum' IDENTIFIER '{' enumerator_list '}'` (5 symbols, named)
- P56: `enum_specifier -> 'enum' '{' enumerator_list '}'` (4 symbols, anonymous)
- P57: `enum_specifier -> 'enum' IDENTIFIER` (2 symbols, forward ref)
- P58: `enumerator_list -> enumerator` (1 symbol, starts Xar)
- P59: `enumerator_list -> enumerator_list ',' enumerator` (3 symbols, appends)
- P60: `enumerator -> IDENTIFIER` (1 symbol, plain enumerator)
- P61: `enumerator -> IDENTIFIER '=' expression` (3 symbols, with value)

**New AST Structures:**

```c
/* ENUM_SPECIFIER */
structura {
    Arbor2Nodus*        tag;           /* Tag name or NIHIL */
    Xar*                enumeratores;  /* List of ENUMERATOR nodes */
} enum_specifier;

/* ENUMERATOR */
structura {
    chorda              titulus;       /* Enumerator name */
    Arbor2Nodus*        valor;         /* Optional value expr, NIHIL if not specified */
} enumerator;
```

**New States (145-161):**
- State 145: After `enum` - expect ID or `{`
- State 146: After `enum ID` - expect `{` or reduce P57
- State 147: After `enum {` (anon) - start enumerator list
- State 148: After `enum ID {` (named) - start enumerator list
- State 149: After first enumerator ID - expect `=` or `,` or `}`
- State 150: After first `ID =` - parse expression
- State 151: After first `ID = expr` - continue expr with `+` or reduce P61
- State 152: After first enumerator - reduce P58 (creates Xar)
- State 153: After enum_list in anon - expect `,` or `}`
- State 154: After `enum { list }` - reduce P56
- State 155: After enum_list in named - expect `,` or `}`
- State 156: After `,` - expect next enumerator
- State 157: After `enum ID { list }` - reduce P55
- State 158: After subsequent ID - expect `=` or `,` or `}`
- State 159: After subsequent `ID =` - parse expression
- State 160: After subsequent `ID = expr` - continue expr with `+` or reduce P61
- State 161: After subsequent enumerator - reduce P59

**Expression Reuse:**
Expression parsing for enum values reuses existing states via GOTO entries:
```c
{ 150, INT_NT_EXPR,   151 },
{ 150, INT_NT_TERM,   2 },
{ 150, INT_NT_FACTOR, 3 },
{ 159, INT_NT_EXPR,   160 },
{ 159, INT_NT_TERM,   2 },
{ 159, INT_NT_FACTOR, 3 },
```

**Key Bug Fixed:**

**Expressions failing in enum values**: Initial implementation could not parse `enum { A = 1 + 2 }`. After reducing `1` to expression, the parser ended up in state 151 but couldn't handle `+`.

**Root cause:** State 151 (after `ID = expr`) only had COMMA and BRACE_CLAUSA actions (to reduce P61). It didn't have PLUS to continue the expression.

**Fix:** Added PLUS -> SHIFT 9 to states 151 and 160 to allow expression continuation:
```c
/* State 151: After 'ID = expr' - continue expr or reduce P61 */
{ ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9 },   /* expr + term */
{ ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 61 },
{ ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 61 },
```

**Additional Changes:**
- Added COMMA to expression reduce sets (states 2, 3, 4, 5, 12, 13, 14, 15, 16) so expressions terminate on `,` in enum context

**Tests Passing:**
- `enum foo` → ENUM_SPECIFIER (forward reference)
- `enum { A, B, C }` → ENUM_SPECIFIER (anonymous, 3 enumerators)
- `enum foo { A, B, C }` → ENUM_SPECIFIER (named)
- `enum foo { A = 1, B, C = 10 }` → ENUM_SPECIFIER with mixed values
- `enum foo x` → DECLARATIO with enum type
- `enum { A = 1 + 2 }` → ENUM_SPECIFIER with binary expression value

**Grammar Summary:**
- 162 states (was 145)
- ~1119 action entries (was ~1064)
- 62 grammar rules (was 55)

### Still TODO

- Bit fields: `int x : 3;`
- Nested struct/union/enum
- `typedef enum { } Foo;` pattern
- Array declarators `int arr[10]`
- Test with real library files
- Error recovery

---

## 2026-01-10 (Phase E4: Bit Fields)

### Phase E4: Bit Fields Complete

Added bit field syntax for struct and union members:

```c
struct foo { int x : 3; };          /* Named 3-bit field */
struct foo { int : 4; };            /* Anonymous 4-bit padding */
struct foo { int : 0; };            /* Anonymous - forces alignment */
struct foo { int x : 1 + 2; };      /* Expression width */
union foo { int x : 3; };           /* Bit field in union */
```

**Design Decision: Extend DECLARATOR**

Rather than creating a new node type, extended the existing DECLARATOR with a bit field width:

```c
structura {
    s32                 num_stellae;    /* Number of * pointers */
    chorda              titulus;        /* Variable name (empty for anonymous bit fields) */
    Arbor2Nodus*        latitudo_biti;  /* Bit field width expr, NIHIL if not bit field */
} declarator;
```

**Anonymous bit fields:** Use empty `titulus` (mensura = 0) to indicate anonymous field.

**Rationale:**
- Minimal code changes (no new node type)
- Bit fields are just a property of declarators
- Shared printing/traversal with existing declarator handling

**Grammar Rules Added (P62-P65):**
- P62: `struct_member_list -> type_spec ID ':' expr ';'` (5 symbols, first named bit field)
- P63: `struct_member_list -> member_list type_spec ID ':' expr ';'` (6 symbols, append named)
- P64: `struct_member_list -> type_spec ':' expr ';'` (4 symbols, first anonymous bit field)
- P65: `struct_member_list -> member_list type_spec ':' expr ';'` (5 symbols, append anonymous)

**Note:** No pointer bit field rules needed - pointer bit fields are invalid C.

**New States (162-173):**

Named bit field paths (from states 123/133 after `type_spec ID`):
- State 162: After `type_spec ID :` (first) - parse expression
- State 163: After `type_spec ID : expr` (first) - expect `;` or continue expr
- State 164: After `type_spec ID : expr ;` (first) - reduce P62
- State 165-167: Same pattern for subsequent named bit fields (reduce P63)

Anonymous bit field paths (from states 121/131 after `type_spec`):
- State 168: After `type_spec :` (first) - parse expression
- State 169: After `type_spec : expr` (first) - expect `;` or continue expr
- State 170: After `type_spec : expr ;` (first) - reduce P64
- State 171-173: Same pattern for subsequent anonymous bit fields (reduce P65)

**State Machine Modifications:**

Added COLON shifts to existing member states:
- State 121: `COLON` → SHIFT 168 (anonymous bit field, first)
- State 123: `COLON` → SHIFT 162 (named bit field, first)
- State 131: `COLON` → SHIFT 171 (anonymous bit field, subsequent)
- State 133: `COLON` → SHIFT 165 (named bit field, subsequent)

**Expression Reuse:**

Bit field width expressions reuse existing expression parsing via GOTO entries:
```c
{ 162, INT_NT_EXPR,   163 },  { 162, INT_NT_TERM, 2 },  { 162, INT_NT_FACTOR, 3 },
{ 165, INT_NT_EXPR,   166 },  { 165, INT_NT_TERM, 2 },  { 165, INT_NT_FACTOR, 3 },
{ 168, INT_NT_EXPR,   169 },  { 168, INT_NT_TERM, 2 },  { 168, INT_NT_FACTOR, 3 },
{ 171, INT_NT_EXPR,   172 },  { 171, INT_NT_TERM, 2 },  { 171, INT_NT_FACTOR, 3 },
```

Expression continuation (`+`) is supported in states 163, 166, 169, 172 for expressions like `int x : 1 + 2;`.

**AST Construction:**

For P62-P65, the bit field width expression is stored in `decl_node->datum.declarator.latitudo_biti`. For anonymous bit fields (P64/P65), `titulus.mensura = 0` indicates no name.

Updated P48-P51 to initialize `latitudo_biti = NIHIL` for regular (non-bit-field) members.

**ACTIO_INDICES Calculation:**

Adding COLON to states 121, 123, 131, 133 required careful index adjustment:
- +1 after state 121 (now 5 actions instead of 4)
- +2 after state 123 (cumulative)
- +3 after state 131 (cumulative)
- +4 after state 133 (cumulative, propagates to all states 134+)

**Tests Passing:**
- `struct foo { int x : 3; }` → STRUCT_SPECIFIER with bit field member
- `struct foo { int x : 3; int y : 5; }` → 2 bit field members
- `struct foo { int : 4; }` → Anonymous bit field (titulus.mensura = 0)
- `struct foo { int x : 3; int : 0; int y : 5; }` → Mixed named/anonymous
- `struct foo { int x : 1 + 2; }` → Expression width (BINARIUM node)
- `union foo { int x : 3; }` → Bit field in union

**Grammar Summary:**
- 174 states (was 162)
- ~1171 action entries (was ~1119)
- 66 grammar rules (was 62)

### Still TODO

- Nested struct/union/enum
- `typedef struct { } Foo;` pattern
- Array declarators `int arr[10]`
- Test with real library files
- Error recovery

---

## 2026-01-10

### Phase E5: Nested struct/union/enum as member types

Added support for using struct/union/enum as member type specifiers within other struct/union definitions.

**New Grammar Rules (P66-P73):**
```
P66: struct_member_list -> struct_specifier ID ';'               (3 symbols, first)
P67: struct_member_list -> struct_specifier '*' ID ';'           (4 symbols, first ptr)
P68: struct_member_list -> member_list struct_specifier ID ';'   (4 symbols, append)
P69: struct_member_list -> member_list struct_specifier '*' ID ';' (5 symbols, append ptr)
P70-P73: Same pattern for enum_specifier
```

**Key insight:** struct_specifier covers both `struct` and `union` (via `est_unio` flag), so P66-P69 handle nested structs AND unions.

**State Machine Changes:**

Added UNION/ENUM shifts to states 119, 120, 127, 129, 139, 140, 141, 143:
```c
{ ARBOR2_LEXEMA_UNION,  ARBOR2_ACTIO_SHIFT, 137 },  /* nested union */
{ ARBOR2_LEXEMA_ENUM,   ARBOR2_ACTIO_SHIFT, 145 },  /* nested enum */
```

**New States (174-197):**

24 new states for nested type member paths, split into:
- First member struct path: 174-179
- Subsequent member struct path: 180-185
- First member enum path: 186-191
- Subsequent member enum path: 192-197

Each path handles: `specifier → '*'? → ID → ';' → reduce`

**GOTO Entries:**

Added GOTO from struct member states (119/120/127/129) and union member states (139/140/141/143) to route STRUCT_SPECIFIER and ENUM_SPECIFIER to new paths:
```c
{ 119, INT_NT_STRUCT_SPEC,  174 },  /* nested struct/union → first member path */
{ 119, INT_NT_ENUM_SPEC,    186 },  /* nested enum → first member path */
{ 127, INT_NT_STRUCT_SPEC,  180 },  /* nested struct/union → subsequent member path */
/* ... etc */
```

**AST Construction (P66-P73):**

Key difference from P48-P51: the specifier is already a node in `valori`, not a token:
```c
/* P66: struct_specifier ID ';' */
Arbor2Nodus* spec_node = valori[II];  /* Already STRUCT_SPECIFIER or ENUM_SPECIFIER */
member->datum.declaratio.specifier = spec_node;  /* Use node directly! */
```

**ACTIO_INDICES Calculation:**

Adding 2 actions to each of states 119, 120, 127, 129 required cumulative offset adjustment:
- +2 after state 119
- +4 after state 120
- +6 after state 127
- +8 after state 129 (propagates to all states 130+)

**Tests Passing:**
- `struct outer { struct inner { int a; } x; }` → nested struct definition
- `struct outer { struct { int a; } *ptr; }` → pointer to anonymous nested
- `struct outer { union { int i; int j; } data; }` → nested union
- `struct outer { enum { A, B } status; }` → nested enum
- `struct outer { int x; struct { int a; } nested; int y; }` → mixed members

**Grammar Summary:**
- 198 states (was 174)
- ~1271 action entries (was ~1171)
- 74 grammar rules (was 66)

### Still TODO

- `typedef struct { } Foo;` pattern
- Array declarators `int arr[10]`
- Test with real library files
- Error recovery

---

## 2026-01-10 (Phase E6: Typedef Declarations)

### Phase E6: Typedef Declarations Complete

Added typedef declarations as a parseable syntactic construct:

```c
typedef int MyInt;                    /* Simple typedef */
typedef int* IntPtr;                  /* Pointer typedef */
typedef struct { int x; } Point;      /* Typedef struct */
typedef struct { int x; } *PointPtr;  /* Typedef struct pointer */
typedef enum { A, B } Status;         /* Typedef enum */
typedef enum { A, B } *StatusPtr;     /* Typedef enum pointer */
```

**Design Decision: est_typedef Flag**

Rather than creating a new node type, added `est_typedef` boolean to existing `declaratio`:

```c
structura {
    Arbor2Nodus*        specifier;      /* Type specifier (identifier or struct/enum) */
    Arbor2Nodus*        declarator;     /* The declarator (*name or name) */
    b32                 est_typedef;    /* VERUM if this is a typedef declaration */
} declaratio;
```

**Rationale:**
- Typedef IS a declaration, just with special storage class semantics
- Minimal AST change (1 field)
- Existing code that handles DECLARATIO continues to work
- Consistent with how C compilers model typedef

**Grammar Rules Added (P74-P79):**
- P74: `typedef_decl -> 'typedef' type_specifier ID ';'` (4 symbols, simple)
- P75: `typedef_decl -> 'typedef' type_specifier '*' ID ';'` (5 symbols, pointer)
- P76: `typedef_decl -> 'typedef' struct_specifier ID ';'` (4 symbols, struct)
- P77: `typedef_decl -> 'typedef' struct_specifier '*' ID ';'` (5 symbols, struct ptr)
- P78: `typedef_decl -> 'typedef' enum_specifier ID ';'` (4 symbols, enum)
- P79: `typedef_decl -> 'typedef' enum_specifier '*' ID ';'` (5 symbols, enum ptr)

All produce `ARBOR2_NODUS_DECLARATIO` with `est_typedef = VERUM`.

**State Machine Changes:**

Added TYPEDEF entry point to state 0:
```c
{ ARBOR2_LEXEMA_TYPEDEF, ARBOR2_ACTIO_SHIFT, 198 }
```

**New States (198-216):**
- State 198: After `typedef` - expect type_specifier (routes to 117/137/145 for struct/union/enum)
- State 199: After `typedef type_spec` - expect `*` or ID
- State 200: After `typedef type_spec *` - more `*` or ID
- State 201: After `typedef type_spec ID` - expect `;`
- State 202: After `typedef type_spec *... ID` - expect `;`
- State 203: Reduce P74 (typedef type_spec ID ;)
- State 204: Reduce P75 (typedef type_spec * ID ;)
- State 205: After `typedef struct_spec` - expect `*` or ID
- State 206: After `typedef struct_spec *` - more `*` or ID
- State 207: After `typedef struct_spec ID` - expect `;`
- State 208: After `typedef struct_spec *... ID` - expect `;`
- State 209: Reduce P76 (typedef struct_spec ID ;)
- State 210: Reduce P77 (typedef struct_spec * ID ;)
- State 211-216: Same pattern for enum_specifier → P78/P79

**State Reuse:**
State 198 shifts to existing struct/union/enum entry states:
- STRUCT → 117 (reuse struct parsing)
- UNION → 137 (reuse union parsing)
- ENUM → 145 (reuse enum parsing)

**GOTO Entries Added:**
```c
{ 198, INT_NT_STRUCT_SPEC, 205 },  /* typedef struct { } → ID path */
{ 198, INT_NT_ENUM_SPEC,   211 }   /* typedef enum { } → ID path */
```

**AST Construction:**

For P74/P75 (simple typedef):
- Type specifier is a token, so create new IDENTIFICATOR node
- Set `est_typedef = VERUM`

For P76-P79 (struct/enum typedef):
- Specifier is already a node in `valori` (STRUCT_SPECIFIER or ENUM_SPECIFIER)
- Use node directly as specifier
- Set `est_typedef = VERUM`

**ACTIO_INDICES Update:**
- Updated state 0 from 21 to 22 actions
- Incremented all subsequent indices by 1 (states 2-197)
- Added new entries for states 198-216
- Updated NUM_STATES from 198 to 217

**Tests Passing:**
- `typedef int MyInt;` → DECLARATIO with est_typedef=VERUM, num_stellae=0
- `typedef int* IntPtr;` → DECLARATIO with est_typedef=VERUM, num_stellae=1
- `typedef struct { int x; } Point;` → DECLARATIO with STRUCT_SPECIFIER
- `typedef struct { int x; } *PointPtr;` → DECLARATIO with STRUCT_SPECIFIER, num_stellae=1
- `typedef enum { A, B } Status;` → DECLARATIO with ENUM_SPECIFIER
- `typedef enum { A, B } *StatusPtr;` → DECLARATIO with ENUM_SPECIFIER, num_stellae=1

**Grammar Summary:**
- 217 states (was 198)
- ~1332 action entries (was ~1271)
- 80 grammar rules (was 74)

### Deferred to Future Phase

- Function pointer typedefs: `typedef int (*Callback)(int, int);`
- Multiple declarators: `typedef int Int, *IntPtr;`

### Still TODO

- Array declarators `int arr[10]`
- Test with real library files
- Error recovery

---

## 2026-01-10 (Phase E7: Array Declarators - Partial)

### Phase E7: Array Declarators - Foundation Only

Added foundation for array declarator support but deferred state machine completion:

```c
int arr[10];              /* Fixed size array */
int arr[];                /* Unsized array */
int matrix[10][20];       /* Multi-dimensional */
int *arr[10];             /* Array of pointers */
```

**What Was Completed:**

1. **AST Extension (arbor2_glr.h):**
   Extended DECLARATOR struct with `dimensiones` field:
   ```c
   structura {
       s32                 num_stellae;    /* Number of * pointers */
       chorda              titulus;        /* Variable name */
       Arbor2Nodus*        latitudo_biti;  /* Bit field width expr */
       Xar*                dimensiones;    /* NEW: Array dims, NIHIL if not array */
   } declarator;
   ```
   Each dimension entry is an expression node (for `[10]`) or NIHIL (for `[]`).

2. **Grammar Rules Added (arbor2_glr_tabula.c):**
   ```c
   /* P80: declarator -> declarator '[' expression ']' (4 symbols, sized) */
   { ARBOR2_NT_DECLARATOR, 4, ARBOR2_NODUS_DECLARATOR }

   /* P81: declarator -> declarator '[' ']' (3 symbols, unsized) */
   { ARBOR2_NT_DECLARATOR, 3, ARBOR2_NODUS_DECLARATOR }
   ```

3. **Reduction Handling (arbor2_glr.c):**
   Added P80/P81 cases to handle AST construction:
   - P80: Copies base declarator, creates/extends dimensiones Xar, appends size expression
   - P81: Same as P80 but appends NIHIL for unsized dimension

4. **Initialized dimensiones = NIHIL:**
   Updated all existing DECLARATOR creation sites (P11, P12, P38-P43, P48-P51, P62-P65, P66-P73, P74-P79) to set `dimensiones = NIHIL`.

**What Was NOT Completed (Deferred):**

The state machine changes proved extremely error-prone due to manual ACTIO_INDICES maintenance:

- States 217-220 for array parsing NOT added
- BRACKET_APERTA transitions NOT added to states 18, 19, 20, 116, 123, 133, 201
- BRACKET_CLAUSA NOT added to expression reduce states (2, 3, 4, 5)
- GOTO entries for expression routing from state 217 NOT added
- ACTIO_INDICES NOT updated

**Why Deferred:**

Multiple attempts to add the state machine changes resulted in cascading ACTIO_INDICES calculation errors:
- Adding BRACKET_CLAUSA to states 2-5 shifts ALL subsequent indices
- Adding BRACKET_APERTA to 6+ existing states shifts many indices
- Manual index calculation is extremely error-prone
- Off-by-one errors cause segfaults or wrong state transitions
- Function declarators, switch statements, and other tests broke repeatedly

The current approach of hand-maintained cumulative indices in ACTIO_INDICES does not scale well for modifications that touch many existing states.

**Test Status:**

Array declarator tests were added to probatio_arbor2_glr.c but will fail until state machine is complete. Consider:
- Removing/commenting array tests temporarily
- OR marking them as expected-to-fail with a flag

**Future Options:**

1. **Careful manual calculation:** Re-attempt with more careful index tracking
2. **Build-time index calculation:** Write a script/tool to generate ACTIO_INDICES from ACTIONES
3. **Different table representation:** Use per-state arrays instead of flat array with offsets

**Grammar Summary (Partial):**
- 217 states (unchanged from E6)
- ~1332 action entries (unchanged)
- 82 grammar rules (was 80, added P80-P81 to REGULAE only)

---

## 2026-01-10: Phase E7 State Machine Implementation COMPLETE

### Problem Solved: Macro-based ACTIO_INDICES

The previous implementation attempt failed due to manual ACTIO_INDICES calculation errors. This was solved by introducing chained macros:

```c
#define STATE_0_COUNT   24
#define STATE_1_COUNT   4
...
#define IDX_STATE_0     0
#define IDX_STATE_1     (IDX_STATE_0 + STATE_0_COUNT)
#define IDX_STATE_2     (IDX_STATE_1 + STATE_1_COUNT)
...
```

This approach allows any state's action count to be modified by changing just ONE macro - all subsequent indices auto-recalculate at compile time.

### Changes Made

**States Modified:**
- State 0: Added INT and CHAR as type specifiers (count 22 → 24)
- States 2-5: Added BRACKET_CLAUSA for array size expressions (each +1)
- State 12: Added BRACKET_CLAUSA for parenthesized expressions (+1)
- States 13-16: Added BRACKET_CLAUSA for expression continuations (each +1)
- State 18: Already had BRACKET_APERTA, fixed count 5 → 6
- State 19: Added BRACKET_APERTA for pointer declarator arrays (+1)
- State 20: Added BRACKET_APERTA → SHIFT 217 for array declarators (+1)
- State 116: Added BRACKET_APERTA for direct declarator arrays (+1)

**New States Added:**
- State 217: After `declarator [` - expects expression or `]`
- State 218: After `declarator []` - reduces P81 (unsized array)
- State 219: After `declarator [ expr` - expects `]`
- State 220: After `declarator [ expr ]` - reduces P80 (sized array)

**GOTO Table:**
```c
{ 217, INT_NT_EXPR,   219 },  /* array size expression */
{ 217, INT_NT_TERM,   2 },
{ 217, INT_NT_FACTOR, 3 }
```

### Tests Passing

All core array declarator tests pass:
- `int arr[10]` - fixed size array
- `int arr[]` - unsized array
- `int matrix[10][20]` - multi-dimensional array
- `int *arr[10]` - array of pointers
- `int arr[1 + 2]` - expression as array size

### Known Limitations

Array declarators in these contexts require additional grammar changes:
- **Struct members:** `struct { int arr[5]; }` - P48-P51 rules don't use declarator NT
- **Typedefs:** `typedef int IntArray[10];` - P74-P79 rules don't use declarator NT

These tests are disabled with `#if 0` and TODO comments.

### Grammar Summary

- 221 states (was 217, added 4 for array parsing)
- 82 grammar rules (P80-P81 already in REGULAE)
- Action table uses chained macro indices for maintainability

---

## 2026-01-10

### Phase E8: Struct Member Array Support

Implemented array support for struct members by refactoring to use the declarator non-terminal. This was the first part of the E8 plan.

**Problem:** Struct member rules (P48-P51) used explicit patterns like `type_spec ID ;` that didn't route through declarator, so array syntax `int arr[5];` wasn't possible.

**Solution:** Refactor state machine to route struct member declarations through existing declarator states (17, 18, 19) which already handle arrays via states 217-220.

### Changes Made

1. **States 18 and 19**: Added SEMICOLON and COLON to follow sets (REDUCE P12/P11)
   - Allows declarator reduction when followed by `;` (member end) or `:` (bit field)

2. **State 121** (first member after type_spec): Changed routing
   - ID → state 18 (was 123, now uses declarator path)
   - `*` → state 17 (was 122, now uses declarator path)
   - `:` → state 168 (unchanged, anonymous bit field)

3. **State 131** (subsequent member): Same changes as state 121
   - ID → state 18 (was 133)
   - `*` → state 17 (was 132)

4. **New States 221-224**:
   - State 221: After `type_spec declarator` in struct (first member)
     - SEMICOLON → 222, BRACKET_APERTA → 217, COLON → 162
   - State 222: Reduce P48 (first member)
   - State 223: After `list type_spec declarator` (subsequent member)
     - SEMICOLON → 224, BRACKET_APERTA → 217, COLON → 165
   - State 224: Reduce P49 (subsequent member)

5. **GOTO Table**: Added entries
   - { 121, INT_NT_DECLARATOR, 221 }
   - { 131, INT_NT_DECLARATOR, 223 }

### Tests Passing

All struct member array tests now pass:
- `struct { int arr[5]; }` - simple array member
- `struct { int *ptrs[5]; }` - pointer array member
- `struct { int matrix[3][4]; }` - multi-dimensional array member
- `struct { int a[5]; int b[10]; }` - multiple array members

### State Machine Summary

- 225 states (was 221, added 4 for struct member declarator completion)
- Grammar rule symbol counts unchanged (P48-P51 still 3/4 symbols)
- Chained macro indices handle automatic recalculation

### Next: Typedef Arrays

The typedef rules (P74-P79) need similar refactoring to support `typedef int IntArray[10];`. This will follow the same pattern: route typedef states through declarator path and add completion states.

### Phase E8 Part 2: Typedef Array Support

Extended the declarator refactor to typedef rules (P74-P79).

**Problem:** Typedef rules used explicit patterns like `typedef type_spec ID ;` that didn't route through declarator, so array syntax `typedef int IntArray[10];` wasn't possible.

**Solution:** Same approach as struct members - route typedef states through existing declarator states.

### Changes Made

1. **States 199, 205, 211** (after typedef type/struct/enum_spec): Changed routing
   - ID → state 18 (was 201/207/213, now uses declarator path)
   - `*` → state 17 (was 200/206/212, now uses declarator path)

2. **New States 225-230**:
   - State 225: After `typedef type_spec declarator` - expect `;` or `[`
   - State 226: Reduce P74 (simple typedef)
   - State 227: After `typedef struct_spec declarator` - expect `;` or `[`
   - State 228: Reduce P76 (struct typedef)
   - State 229: After `typedef enum_spec declarator` - expect `;` or `[`
   - State 230: Reduce P78 (enum typedef)

3. **GOTO Table**: Added entries
   - { 199, INT_NT_DECLARATOR, 225 }
   - { 205, INT_NT_DECLARATOR, 227 }
   - { 211, INT_NT_DECLARATOR, 229 }

4. **Reduction Handlers**: Updated P74, P76, P78 to use pre-built declarator from valori[1]. Marked P75, P77, P79 as UNUSED.

### Tests Passing

All typedef array tests now pass:
- `typedef int IntArray[10];` - simple array typedef
- `typedef int* IntPtrArray[10];` - pointer array typedef
- `typedef int Matrix[3][4];` - multi-dimensional array typedef

All existing typedef tests continue to pass:
- `typedef int MyInt;`
- `typedef int* IntPtr;`
- `typedef struct { int x; } Point;`
- `typedef struct { int x; } *PointPtr;`
- etc.

### State Machine Summary

- 231 states (was 225, added 6 for typedef declarator completion)
- P75, P77, P79 marked UNUSED (pointer typedefs now handled via declarator)
- Chained macro indices handle automatic recalculation

### Phase E8 Complete

Both struct member arrays and typedef arrays are now fully supported via the declarator refactor. The grammar reduces 18 explicit rules to 9 unified rules that support all declarator forms (simple, pointer, array, multi-dimensional).

## 2026-01-10: Phase E8 Part 3 Complete - Nested Type Member Arrays

Completed array declarator support for nested struct/enum members. All three parts of Phase E8 are now complete.

### Changes Made

**State Machine (arbor2_glr_tabula.c):**
- Modified states 174, 180, 186, 192 to route through declarator states (17, 18)
- Added GOTO entries for DECLARATOR from these states to completion states 231, 233, 235, 237
- Created completion states 231-238:
  - 231/232: First nested struct member completion/reduce P66
  - 233/234: Subsequent nested struct member completion/reduce P68
  - 235/236: First nested enum member completion/reduce P70
  - 237/238: Subsequent nested enum member completion/reduce P72
- All completion states allow `[` for arrays (→217) or `;` for end of member

**Reduction Handlers (arbor2_glr.c):**
- Updated P66, P68, P70, P72 to use pre-built declarator from `valori[I]`
- Marked P67, P69, P71, P73 as UNUSED (pointer variants handled via declarator)

**Tests Added:**
- `struct { struct Inner items[10]; }` - array of nested structs
- `struct { enum { A, B } status[5]; }` - array of nested enums
- `struct { struct Inner *ptrs[3]; }` - pointer arrays of nested structs

### Phase E8 Summary

All three parts completed:
1. **Part 1**: Struct member arrays (P48-P51) - type_spec declarator path
2. **Part 2**: Typedef arrays (P74-P79) - typedef declarator path  
3. **Part 3**: Nested type member arrays (P66-P73) - nested specifier declarator path

State count: 239 states total (217-220 for array declarator, 221-230 for struct/typedef completions, 231-238 for nested type completions).

---

## 2026-01-10: Phase E9 Complete - Arithmetic Operators Part 1

Added `-`, `/`, `%` operators at existing precedence levels (same as `+` and `*`).

### What Was Already in Place

- Lexer tokens: MINUS, SOLIDUS, PERCENTUM already existed
- Binary node structure: `binarium.operator` stores any `Arbor2LexemaGenus`
- Reduction handler: Generic BINARIUM case works for any operator

### Grammar Rules Added (P82-P84)

```c
/* P82: expression -> expression '-' term */
{ ARBOR2_NT_EXPRESSIO, 3, ARBOR2_NODUS_BINARIUM }

/* P83: term -> term '/' factor */
{ ARBOR2_NT_TERMINUS, 3, ARBOR2_NODUS_BINARIUM }

/* P84: term -> term '%' factor */
{ ARBOR2_NT_TERMINUS, 3, ARBOR2_NODUS_BINARIUM }
```

### State Machine Changes

**Key states modified:**
- State 1: Added `MINUS → SHIFT 9` (same as PLUS)
- State 2: Added `SOLIDUS/PERCENTUM → SHIFT 10` (same as ASTERISCUS), `MINUS → REDUCE P2`
- State 13: Added `SOLIDUS/PERCENTUM → SHIFT 10` (higher precedence), `MINUS → REDUCE P1`
- State 14: Added `SOLIDUS/PERCENTUM/MINUS → REDUCE P3`

**All states with `+` or `*` in follow sets updated:**
States 3, 4, 5, 11, 12, 15, 16, 18, 19, 92, 94, 103, 116

### Bug Fixed: State Count Macros

After adding entries, tests failed with segfault. Root cause: STATE_*_COUNT macros were not updated. Used awk to count actual entries:

```bash
awk '/\/\* State [0-9]+:.*\*\//,/\/\* State [0-9]+:/{print}' ... | grep -c ARBOR2_ACTIO
```

Corrected counts for all modified states.

### Tests Added

```c
5 - 3       → BINARIUM(op=MINUS, left=5, right=3)
6 / 2       → BINARIUM(op=SOLIDUS, left=6, right=2)
7 % 3       → BINARIUM(op=PERCENTUM, left=7, right=3)
10 - 6 / 2  → BINARIUM(op=MINUS, left=10, right=BINARIUM(op=SOLIDUS, left=6, right=2))
```

The mixed precedence test confirms `/` binds tighter than `-`.

### Grammar Summary

- 239 states (unchanged)
- 85 grammar rules (was 82, added P82-P84)
- All existing tests pass

### Future Work

- Part 2: Comparison operators (<, >, <=, >=) - DONE (see below)
- Part 3: Equality operators (==, !=) - DONE (see below)
- Part 4: Logical operators (&&, ||)
- Part 5: Bitwise operators (&, |, ^, <<, >>)

---

## 2026-01-10: Phase E9 Part 2 Complete - Comparison & Equality Operators

Added relational (`<`, `>`, `<=`, `>=`) and equality (`==`, `!=`) operators with two new precedence levels below expression.

### New Precedence Hierarchy

```
aequalitas (==, !=)     ← lowest (new)
comparatio (<, >, <=, >=)  (new)
expressio (+, -)        ← was lowest, now middle
terminus (*, /, %)
factor (unary, atoms)   ← highest
```

### New Non-Terminals (arbor2_glr.h)

Added at TOP of enum (shifts all other values):
- `ARBOR2_NT_AEQUALITAS` - equality expressions
- `ARBOR2_NT_COMPARATIO` - relational expressions

Internal macros in arbor2_glr_tabula.c:
- `INT_NT_AEQUALITAS 22`
- `INT_NT_COMPARATIO 23`

### Grammar Rules Added (P85-P92)

```c
P85: aequalitas -> aequalitas '==' comparatio
P86: aequalitas -> aequalitas '!=' comparatio
P87: aequalitas -> comparatio              (pass-through)
P88: comparatio -> comparatio '<' expressio
P89: comparatio -> comparatio '>' expressio
P90: comparatio -> comparatio '<=' expressio
P91: comparatio -> comparatio '>=' expressio
P92: comparatio -> expressio               (pass-through)
```

### New States (239-245)

- State 239: After comparatio - shift ==|!= or reduce P87 (to aequalitas)
- State 240: After aequalitas at top-level - ACCEPT on EOF, shift ==|!= to continue chain
- State 241: After comparison operator (<|>|<=|>=) - expect expression starters
- State 242: After equality operator (==|!=) - expect expression starters
- State 243: After comparatio op expression - shift +/- to continue, reduce P88 on other tokens
- State 244: After aequalitas op comparatio - reduce P85
- State 245: After aequalitas inside parentheses - shift `)` to complete paren-expression

### Key Design Decisions

1. **State 1 restructured**: Now reduces P92 (expression → comparatio) instead of ACCEPT. Comparison operators shift to state 241.

2. **Separate inside-parens state (245)**: State 240 is for top-level accept. For expressions inside parentheses, GOTO(6, AEQUALITAS) = 245 which shifts `)` to state 12 instead of reducing P0.

3. **State 243 continues expression with +/-**: Originally reduced P88 on `+`/`-`, but since `+`/`-` have higher precedence than comparison, we should continue parsing the expression. Fixed to shift `+`/`-` to state 9.

4. **State 240 continues equality chain**: Shifts `==`/`!=` to state 242 instead of reducing P85, enabling chained equalities like `1 == 2 == 3`.

### Bugs Fixed

1. **GOTO entries used non-existent state 245**: The GOTO entries for state 242 (after equality op) were incorrectly labeled as state 245. Fixed to use 242.

2. **State 240 had wrong actions for ==|!=**: Was REDUCE P85 (which expects 3 symbols but we only have 1). Changed to SHIFT 242 to continue chain.

3. **Infinite loop in parenthesized expressions**: GOTO(6, AEQUALITAS) = 240 caused loop because state 240 reduced P0 on `)`. Fixed by creating state 245 specifically for inside-parens context.

4. **`1 + 2 < 3 + 4` failed**: State 243 reduced P88 on `+` before parsing `+ 4`. Fixed by shifting `+`/`-` to continue expression first.

### Tests Passing

All comparison/equality operator tests pass:
- `1 < 2` → BINARIUM(op=MINOR)
- `3 > 1` → BINARIUM(op=MAIOR)
- `2 <= 3` → BINARIUM(op=MINOR_AEQ)
- `5 >= 4` → BINARIUM(op=MAIOR_AEQ)
- `1 == 1` → BINARIUM(op=AEQUALIS)
- `1 != 2` → BINARIUM(op=NON_AEQUALIS)
- `1 + 2 < 3 + 4` → BINARIUM(op=MINOR, left=BINARIUM(+), right=BINARIUM(+))
- `1 < 2 == 3 < 4` → BINARIUM(op=AEQUALIS, left=BINARIUM(<), right=BINARIUM(<))
- `1 + 2 < 3 * 4` → BINARIUM(op=MINOR, left=BINARIUM(+), right=BINARIUM(*))
- `(1 < 2)` → BINARIUM(op=MINOR) (parens unwrapped correctly)

### Grammar Summary

- 246 states (was 239, added 7 for comparison/equality)
- 93 grammar rules (was 85, added P85-P92)
- Validation script confirms all STATE_*_COUNT macros match actual entries

---

## 2026-01-10

### Phase E9.2 Completion: Missing GOTO entries for comparison operators in control flow

After Phase E9.2 added the comparatio/aequalitas precedence levels, expressions in control flow bodies and conditions failed to parse because the expression chain (expression → comparatio → aequalitas) couldn't complete without proper GOTO entries.

### Problem

When parsing `if (x) y;`, the expression `y` in the then-branch:
1. Reduces through factor → term → expression (to state 1)
2. State 1 reduces P92 (expression → comparatio)
3. GOTO(33, COMPARATIO) was missing → parse fails

Similarly, conditions like `while (a == b)` and `for (; i < 10; )` failed because their expression states didn't handle comparison operators.

### Fixes Applied

#### 1. Statement body states - added COMPARATIO/AEQUALITAS GOTO entries

States that can contain expression statements needed GOTO entries to complete the precedence chain:
- State 26 (compound statement body)
- State 33 (if then-branch)
- State 35 (if else-branch)
- State 42 (while body)
- State 45 (do body)
- State 63 (for body)
- State 77 (label body)
- State 82 (switch body)
- State 86 (case body)
- State 89 (default body)

All route COMPARATIO → 239, AEQUALITAS → 240 (reusing top-level states).

#### 2. Condition states - added comparison operator handling

States after condition expressions needed comparison operator shifts:
- State 32 (if condition) - already had from earlier
- State 41 (while condition) - added 7 new actions
- State 49 (do-while condition) - added 7 new actions
- State 58 (for condition) - added 8 new actions
- State 81 (switch condition) - added 7 new actions

Each reduces P92 on `)` or `;` (depending on context), shifts comparison/equality operators to continue.

#### 3. New aequalitas states for each condition context

Created context-specific aequalitas states that complete to the correct follow state:
- State 248: while condition → shifts `)` to state 42 (while body)
- State 249: for condition comparatio → reduces P87 on `;`
- State 250: for condition aequalitas → reduces P28 (expr_opt) on `;`
- State 251: do-while condition → shifts `)` to state 50
- State 252: switch condition → shifts `)` to state 82 (switch body)

Also added GOTO entries to condition states (40, 48, 57, 80) for COMPARATIO and AEQUALITAS.

### Tests Now Passing

All edge case tests for comparison operators in control flow:
- `if (x < 10) y;` → SI with BINARIUM(op=MINOR) condition
- `while (a == b) x;` → DUM with BINARIUM(op=AEQUALIS) condition  
- `for (; i < 10; ) x;` → PER with BINARIUM(op=MINOR) condition

Plus earlier tests:
- `((1 < 2))` → nested parens with comparison
- `1 == 2 == 3` → chained equality (left-associative)
- `1 < 2 < 3` → chained comparison (left-associative)
- `1 < 2 * 3 + 4` → correct precedence with complex RHS

### State Summary

- 253 states (was 248, added 5 new aequalitas context states)
- Updated state counts: 41, 49, 58, 81 (each +7 for comparison operators)

## 2026-01-12: Phase E10 Complete - Logical Operators (&&, ||)

Added logical AND (`&&`) and logical OR (`||`) operators with proper C89 precedence:

```
disiunctio (||)    <- lowest
coniunctio (&&)
aequalitas (==, !=)
comparatio (<, >, <=, >=)
expressio (+, -)
terminus (*, /, %)
factor (unary, atoms) <- highest
```

### Implementation Summary

#### 1. New non-terminals
- `ARBOR2_NT_CONIUNCTIO` (&&) - logical AND expressions
- `ARBOR2_NT_DISIUNCTIO` (||) - logical OR expressions

#### 2. New grammar rules (P93-P96)
- P93: `disiunctio -> disiunctio '||' coniunctio` (BINARIUM)
- P94: `disiunctio -> coniunctio` (pass-through)
- P95: `coniunctio -> coniunctio '&&' aequalitas` (BINARIUM)
- P96: `coniunctio -> aequalitas` (pass-through)

#### 3. State machine changes
- State 240 (after aequalitas): Changed from ACCEPT to REDUCE P96
- New states 253-263 for logical operator parsing:
  - 253: after coniunctio at top-level
  - 254: after 'coniunctio &&'
  - 255: after disiunctio at top-level (new ACCEPT state)
  - 256: after 'disiunctio ||'
  - 257-263: context states for && and || in various contexts

#### 4. Many states updated to handle && and ||
Added REDUCE actions for DUAMPERSAND and DUPIPA to states:
1, 2, 3, 4, 5, 11, 12, 13, 14, 15, 16, 18, 19, 239, 244

These all reduce to lower-precedence non-terminals when seeing && or ||.

#### 5. Build system change
Added `-fbracket-depth=512` to compiler flags because the chained IDX_STATE_* macros exceeded clang's default limit of 256 with 264 states.

### Tests
- `a && b` -> BINARIUM(op=DUAMPERSAND)
- `a || b` -> BINARIUM(op=DUPIPA)
- `a || b && c` -> DISIUNCTIO(a, CONIUNCTIO(b, c)) - correct precedence
- `a && b && c` -> CONIUNCTIO(CONIUNCTIO(a, b), c) - left-associative
- `a == b && c < d` -> CONIUNCTIO(AEQUALITAS(a, b), COMPARATIO(c, d))

### State Summary
- 264 states (was 253, added 11 new logical operator states)
- All existing tests still pass

---

## 2026-01-15: Phase 1.5 - Translation Unit Support

### Goal
Enable parsing complete C files with multiple top-level declarations and functions:
```c
int x = 5;
int y = 10;
int foo(int a) { return a + x; }
```

### Design Decision: Wrapper vs Grammar Approach

Considered two approaches:

**Option A - Grammar-based:** Add productions P252-P255 for `external_declaration` and `translation_unit`, plus ~20 new states. Would require modifying accept states 21, 24, 114.

**Option B - Wrapper-based:** Keep existing single-item parsing, wrap in loop that scans for item boundaries and parses each chunk.

**Chose Option B** because:
- No grammar/state table changes needed
- Lower risk of breaking existing tests
- Simpler to implement and debug

### Key Implementation Detail: Semicolons

Initial implementation failed because our declaration grammar does NOT include the semicolon:
```
declaration → type init_decl_list    (P226)
```

The semicolon is handled at statement level (`statement → declaration SEMICOLON`).

This meant when parsing `int x;`:
- Parser reduced `int x` to declaration
- Reached state 21 expecting EOF
- But saw SEMICOLON instead → failed

**Fix:** The boundary finder returns two values:
- `parse_finis`: tokens to include in parse (EXCLUDING semicolon for declarations)
- `proximus`: where to continue (AFTER semicolon)

For functions (end with `}`), both are the same since closing brace IS part of the grammar.

### New Types Added

```c
/* Result struct */
typedef struct {
    b32             successus;
    Arbor2Nodus*    radix;
    Xar*            ambigui;
    Xar*            errores;
    i32             tokens_consumed;  /* NEW */
} Arbor2GLRResultus;

/* Node type */
ARBOR2_NODUS_TRANSLATION_UNIT

/* Node data */
struct {
    Xar*    declarationes;  /* Xar of Arbor2Nodus* */
} translation_unit;
```

### API
```c
Arbor2GLRResultus arbor2_glr_parsere_translation_unit(
    Arbor2GLR*  glr,
    Xar*        lexemata);
```

### Stats
- 1523 tests (up from 1491, +32 new)
- 611 states unchanged
- 252 productions unchanged

## 2026-01-17 - GLR-Expansion Integration (Phase 3.2/3.3)

### 3.2a: Replaced `_macro_suggerit_typum()` with Public API

**Old implementation** (lines 1001-1048): Manual macro lookup with direct corpus access.

**New implementation** (lines 999-1059): Uses `arbor2_expansion_lookahead()` public API.

Key improvements:
1. Uses tested public API instead of duplicating logic
2. Follows one level of macro recursion (e.g., `i32` → `integer` → `int`)
3. Added `ARBOR2_LEXEMA_TYPEDEF`, `ARBOR2_LEXEMA_CONST`, `ARBOR2_LEXEMA_VOLATILE` to type keywords

```c
/* Old: manual lookup */
macro = arbor2_expansion_quaerere_macro(glr->expansion, tok->lexema->valor);
si (macro == NIHIL) redde FALSUM;
si (xar_numerus(macro->corpus) > ZEPHYRUM) { ... }

/* New: uses public API with recursion support */
lookahead = arbor2_expansion_lookahead(glr->expansion, tok->lexema->valor);
si (lookahead.est_recursivum) {
    /* Follow one level */
    def = arbor2_expansion_quaerere_macro(...);
    primus = *(Arbor2Lexema**)xar_obtinere(def->corpus, ZEPHYRUM);
    lookahead = arbor2_expansion_lookahead(glr->expansion, primus->valor);
}
```

### 3.3: AMBIGUUS Identifier Tracking

**Location:** `_creare_nodum_ambiguum()` lines 1105-1115

**Before:** `identificator` was always set to `NIHIL`.

**After:** When `lexema` is an identifier, allocates and stores the identifier causing ambiguity:

```c
si (lexema != NIHIL && lexema->lexema != NIHIL &&
    lexema->lexema->genus == ARBOR2_LEXEMA_IDENTIFICATOR)
{
    nodus->datum.ambiguus.identificator = piscina_allocare(glr->piscina, magnitudo(chorda));
    *(nodus->datum.ambiguus.identificator) = lexema->lexema->valor;
}
```

This enables future resolution by looking up the identifier that caused the ambiguity.

### Test Coverage

Added 3 new test sections to `probatio_arbor2_glr.c`:
1. Latin type macro disambiguation: `integer x` → DECLARATIO
2. Nested macro resolution: `i32 x` (i32→integer→int) → DECLARATIO  
3. AMBIGUUS tracking: `foo * bar` creates AMBIGUUS node with 2+ interpretations

Tests: 1856 (up from 1847, +9 new assertions)


---

## 2026-01-17: Phase 4 - Error Recovery

### Overview

Implemented error recovery for the GLR parser. The parser now continues past errors and produces partial ASTs with ERROR nodes containing information about skipped tokens.

### Design Decisions

1. **Recovery granularity: Statement-level only**
   - Uses `_invenire_finem_declarationis()` to find item boundaries (`;` and `}`)
   - Simpler and fewer false recoveries than expression-level recovery
   - Recovery happens in `arbor2_glr_parsere_translation_unit()`

2. **Error limit: 10 errors max**
   - `ARBOR2_GLR_MAX_ERRORES X` (10 in Roman numerals)
   - Prevents infinite loops on pathological input
   - After 10 errors, returns partial result with `successus = FALSUM`

3. **Error classification: None**
   - Just stores error message and skipped tokens
   - No "probably a declaration" guessing - keeps it simple

4. **Error node placement: At translation unit level**
   - ERROR nodes become siblings of declarations/functions in the AST
   - The translation unit wrapper handles recovery between top-level items

### Implementation

#### New struct member (arbor2_glr.h)
```c
/* Error recovery */
i32  num_errores;    /* Error count for recovery limit */
```

#### New constant (arbor2_glr.h)
```c
#define ARBOR2_GLR_MAX_ERRORES X  /* 10 errors max */
```

#### Error node creation (arbor2_glr.c:_creare_nodum_error)
- Populates `ARBOR2_NODUS_ERROR` with:
  - `nuntius`: error message from the parse attempt
  - `lexemata_saltata`: Xar of tokens that were part of the failed parse
- Sets location info from the first token

#### Translation unit parser modification
When sub-parse fails:
1. Increment `num_errores`
2. Check if over limit → return partial result
3. Collect tokens from `positus` to `finis_info.proximus`
4. Extract error message from `sub_res.errores`
5. Create ERROR node with skipped tokens
6. Add ERROR node to translation unit
7. Advance position and continue to next item

### Test Cases

Added 4 new test sections to `probatio_arbor2_glr.c`:
1. **Invalid decl + valid decl**: `int @ x; int y;` → ERROR + DECLARATIO
2. **Multiple consecutive errors**: `@ x; @ y; @ z;` → 3 ERROR nodes
3. **Error + valid function**: `@ badstuff; int foo() { return 0; }` → ERROR + DEFINITIO_FUNCTI
4. **Error collection**: Verifies `res.errores` contains error messages

Tests: 1877 (up from 1856, +21 new assertions)

### Notes

- The `_recuperare_panic()` function was designed but removed since the translation unit parser already knows item boundaries via `_invenire_finem_declarationis()`
- `successus = VERUM` even if there were errors (as long as under the limit) since we produced a valid AST
- Errors are available in both `res.errores` (messages) and in ERROR nodes (skipped tokens)

---

## 2026-01-17: Phase 5 - Conditional Compilation

### Overview

Implemented recognition of preprocessor conditionals (`#ifdef`, `#ifndef`, `#else`, `#endif`) in the translation unit parser. Creates `ARBOR2_NODUS_CONDITIONALIS` nodes that preserve ALL branches (not just the "active" one).

### Key Design Decisions

1. **Parser-level handling, not expander**
   - The expander passes conditional directives through unchanged
   - The GLR parser recognizes and collects them into CONDITIONALIS nodes
   - Rationale: The expander's job is macro expansion; structure belongs in the parser

2. **All branches preserved**
   - Unlike a traditional preprocessor that picks one branch, we keep all branches
   - Each branch stores its tokens in `Arbor2CondRamus.lexemata`
   - Enables queries like "what code exists under DEBUG?" without re-parsing

3. **Nested conditionals as token inclusion**
   - Nested `#ifdef` blocks are included as tokens within the parent branch
   - Depth tracking (`profunditas`) ensures we match the right `#endif`
   - Keeps the outer CONDITIONALIS node flat (branches don't nest in the structure)

### Bug Fix: #else Detection

Initial implementation failed to detect `#else` because:
- `else` is tokenized as `ARBOR2_LEXEMA_ELSE` (keyword), not `ARBOR2_LEXEMA_IDENTIFICATOR`
- The directive check only looked for identifiers after `#`

Fix: Added explicit checks for keyword tokens before the identifier fallback:
```c
/* #else - 'else' is tokenized as ARBOR2_LEXEMA_ELSE keyword */
si (next_tok->lexema->genus == ARBOR2_LEXEMA_ELSE)
{
    si (genus_out != NIHIL) *genus_out = ARBOR2_DIRECTIVUM_ELSE;
    redde VERUM;
}
```

Same pattern for `#if` (ARBOR2_LEXEMA_IF keyword).

### Data Structures Added

**arbor2_glr.h:**
- `Arbor2DirectivumGenus` enum: IFDEF, IFNDEF, IF, ELIF, ELSE, ENDIF
- `Arbor2CondRamus` struct: branch with genus, conditio, lexemata, parsed, linea
- `conditionalis` union member: rami array, linea_if, linea_endif

### Functions Added

**arbor2_glr.c (~400 lines):**
- `_est_conditionale_directivum()` - detect # followed by conditional keyword
- `_est_initium_conditionale()` - check if position starts #ifdef/#ifndef/#if
- `_saltare_ad_finem_lineae()` - skip past directive line using trivia newlines
- `_obtinere_conditio()` - extract macro name from #ifdef/#ifndef
- `_colligere_conditionale()` - main collection function, handles:
  - Branch creation and switching at #else/#elif
  - Depth tracking for nested conditionals
  - Token collection per branch

### Test Cases Added

6 new test sections in `probatio_arbor2_glr.c` (+294 lines):
1. Simple #ifdef/#endif → 1 CONDITIONALIS with 1 branch
2. #ifdef/#else/#endif → 1 CONDITIONALIS with 2 branches
3. #ifndef → verifies IFNDEF genus and condition string capture
4. Multiple independent conditionals → 2 separate CONDITIONALIS nodes
5. Conditional mixed with declarations → decl + CONDITIONALIS + decl
6. Nested conditionals → outer CONDITIONALIS contains inner's tokens

Tests: 1907 total (up from 1877)

### Out of Scope (Phase 5b)

- `#if EXPR` with expression evaluation
- `#elif EXPR` condition extraction
- `defined(MACRO)` operator
- Branch parsing (`ramus->parsed` remains NIHIL for now)

## 2026-01-19: Fixed Nested Ternary in False Branch

### Problem

Test `a ? b : c ? d : e` (nested ternary in false branch with all identifiers) was failing. The parser returned `c ? d : e` instead of the full expression. The existing test `a ? 1 : b ? 2 : 3` (with integer literals) worked correctly.

### Root Cause

In the GLR parser, both parses were being created correctly:
1. Inner ternary: `c ? d : e` 
2. Outer ternary: `a ? b : (c ? d : e)` with `falsum.genus = TERNARIUS`

But the `_nodi_aequales` function only compared node `genus` (type), not structure:
```c
si (a->genus != b->genus) redde FALSUM;
/* For now, just compare genus - could add deeper comparison */
redde VERUM;
```

Both parses had `genus = ARBOR2_NODUS_TERNARIUS`, so they were considered equal. The first one (wrong one) was selected.

### Fix

1. Added recursive structure comparison for TERNARIUS in `_nodi_aequales`:
```c
si (a->genus == ARBOR2_NODUS_TERNARIUS)
{
    si (!_nodi_aequales(a->datum.ternarius.conditio, b->datum.ternarius.conditio))
        redde FALSUM;
    si (!_nodi_aequales(a->datum.ternarius.verum, b->datum.ternarius.verum))
        redde FALSUM;
    si (!_nodi_aequales(a->datum.ternarius.falsum, b->datum.ternarius.falsum))
        redde FALSUM;
}
```

2. Added heuristic in ambiguity resolution to prefer TERNARIUS with nested TERNARIUS in falsum:
```c
alioquin si (primus->valor->genus == ARBOR2_NODUS_TERNARIUS &&
             alius->valor->genus == ARBOR2_NODUS_TERNARIUS)
{
    b32 primus_habet_ternarium = (primus->valor->datum.ternarius.falsum != NIHIL &&
        primus->valor->datum.ternarius.falsum->genus == ARBOR2_NODUS_TERNARIUS);
    b32 alius_habet_ternarium = (alius->valor->datum.ternarius.falsum != NIHIL &&
        alius->valor->datum.ternarius.falsum->genus == ARBOR2_NODUS_TERNARIUS);

    si (!primus_habet_ternarium && alius_habet_ternarium)
        primus->valor = alius->valor;
}
```

This ensures right-associative parsing: `a ? b : c ? d : e` → `a ? b : (c ? d : e)`

### Files Changed
- lib/arbor2_glr.c: `_nodi_aequales` and ambiguity resolution
- probationes/probatio_arbor2_scribere.c: uncommented test at line 943

Tests: 269 pass (scribere), 77 total pass


## 2026-01-19: Phase 5.4 - Branch Parsing

Implemented parsing of conditional branch contents. Previously, `ARBOR2_NODUS_CONDITIONALIS` nodes collected tokens in each branch's `lexemata` field but `parsed` was always NIHIL.

### Changes

1. **New helper `_parsere_ramus()`** (lines 6737-6785):
   - Returns NIHIL for empty branches (no tokens)
   - Creates copy of branch tokens with synthetic EOF appended
   - Calls `arbor2_glr_parsere_translation_unit()` recursively
   - Returns the parsed AST (TRANSLATION_UNIT node)

2. **Branch parsing loop in `_colligere_conditionale()`** (lines 7031-7042):
   - After collecting all branch tokens but before creating the CONDITIONALIS node
   - Iterates through all branches and calls `_parsere_ramus()` for each
   - Populates `ramus->parsed` with the resulting AST

### Design Notes

- Nested conditionals are handled naturally - when branch tokens contain `#ifdef`/`#endif`, the recursive parse creates nested CONDITIONALIS nodes
- Empty branches (e.g., `#ifdef X\n#else\n...`) correctly produce `parsed = NIHIL`
- Each branch parses independently - partial constructs split across branches will produce ERROR nodes (acceptable limitation)
- Error counter resets per-branch since `arbor2_glr_parsere_translation_unit` clears errors at start

### Tests Added

5 new test blocks in probatio_arbor2_glr.c:
- Basic parsed branch (1 declaration)
- Multiple declarations in branch
- Nested conditional in branch (verifies inner CONDITIONALIS created)
- Empty branch (verifies parsed = NIHIL)
- ifdef with else (both branches parse independently)

Tests: 2069 total pass (24 new assertions)

---

## 2026-01-19

### Preprocessor Directive Roundtrip Support

Implemented full roundtrip (parse → serialize → identical output) for all preprocessor directives.

#### Problem

Preprocessor directives were being parsed but could not roundtrip because:
- CONDITIONALIS nodes didn't store the directive line tokens (just parsed the body)
- No parsing existed for #define, #undef, #pragma, #include (HASH token had 0 parser actions)

#### Solution

1. **Extended Arbor2CondRamus struct** with `directivum_lexemata` field to store each directive line's tokens (#ifdef FOO, #else, etc.)

2. **Extended conditionalis datum** with `endif_lexemata` field to store #endif tokens

3. **Added new node types**: `ARBOR2_NODUS_DEFINE`, `ARBOR2_NODUS_UNDEF`, `ARBOR2_NODUS_PRAGMA`

4. **Created helper functions**:
   - `_colligere_directivum_lexemata()` - collects tokens from # to newline
   - `_obtinere_pp_genus()` - identifies directive type (define/undef/pragma/include)
   - `_creare_pp_nodus()` - creates appropriate node for each directive type

5. **Updated `_colligere_conditionale()`** to capture directive tokens for each branch

6. **Updated translation unit parser** to detect and create nodes for non-conditional directives

7. **Added serializer cases** for all preprocessor node types

#### Key Design Decisions

- Directive tokens stored verbatim (not parsed into sub-structure) for perfect roundtrip
- Each branch's directive line is stored separately from body tokens
- Non-conditional directives (#define etc.) handled at translation unit level, not in grammar tables

#### Tests Added

17 new roundtrip tests:
- #define (simple, empty, function-like)
- #undef
- #pragma
- #include (angle brackets and quotes)
- Conditional compilation (#ifdef, #ifndef, #else, #elif, #if)
- Mixed preprocessor with declarations

All 396 scribere tests pass, all 2069 GLR tests pass.


## 2026-01-20: Fix Qualifier Tracking in Parameter Declarations

### Problem

Type qualifiers (`const`, `volatile`) were being dropped from parameter declarations during roundtrip:

```c
// Input:
void f(const int x) { (void)x; }

// Output (qualifiers dropped):
void f(int x) { (void)x; }
```

### Root Cause Analysis

1. **Missing fields in `parameter_decl` struct** - The struct in `arbor2_glr.h` lacked `tok_const` and `tok_volatile` fields (unlike `declaratio` which had them).

2. **Missing production handlers** - Productions P342-P345 in the grammar table handle qualified parameters:
   - P342: `param -> 'const' type declarator` (3 symbols)
   - P343: `param -> 'volatile' type declarator` (3 symbols)
   - P344: `param -> 'const' type` (2 symbols, abstract)
   - P345: `param -> 'volatile' type` (2 symbols, abstract)

   These were falling through to the generic `casus ARBOR2_NODUS_PARAMETER_DECL:` handler which didn't capture qualifier tokens.

3. **Serializer ignored qualifiers** - The PARAMETER_DECL case in `arbor2_scribere.c` had no code to emit qualifier tokens.

### Key Bug in Initial Fix Attempt

First fix attempt produced "const const x" instead of "const int x". The issue: when `type_specifier` is NIHIL (for built-in types like `int`), the serializer falls back to emitting `nodus->lexema`. Initially I set `nodus->lexema = lexemata[II]` (qualifier token) instead of `lexemata[I]` (type token).

### Solution

1. **Added fields to struct** (`arbor2_glr.h`):
   ```c
   structura {
       Arbor2Token*  tok_const;      /* 'const' qualifier (NIHIL si nullus) */
       Arbor2Token*  tok_volatile;   /* 'volatile' qualifier (NIHIL si nullus) */
       Arbor2Nodus*  type_specifier;
       Arbor2Nodus*  declarator;
   } parameter_decl;
   ```

2. **Added P342-P345 handlers** in `casus ARBOR2_NODUS_PARAMETER_DECL:`:
   - Set `tok_const`/`tok_volatile` from `lexemata[II]` (for 3-symbol) or `lexemata[I]` (for 2-symbol)
   - Critical: Set `nodus->lexema` to **type token** (not qualifier) for fallback serialization

3. **Updated serializer** to emit qualifiers before type:
   ```c
   si (nodus->datum.parameter_decl.tok_const != NIHIL)
       arbor2_scribere_lexema(output, nodus->datum.parameter_decl.tok_const);
   si (nodus->datum.parameter_decl.tok_volatile != NIHIL)
       arbor2_scribere_lexema(output, nodus->datum.parameter_decl.tok_volatile);
   ```

### Valori/Lexemata Indexing (for reference)

For reductions, indices go from rightmost (0) to leftmost (N-1):
- 3-symbol `const type declarator`: lexemata[2]=const, [1]=type, [0]=decl
- 4-symbol `const type decl body`: lexemata[3]=const, [2]=type, [1]=decl, [0]=body

### Files Modified

- `include/arbor2_glr.h` - Added tok_const/tok_volatile to parameter_decl
- `lib/arbor2_glr.c` - Added P340-P345 handlers in PARAMETER_DECL case
- `lib/arbor2_scribere.c` - Emit qualifier tokens before type

---

## 2026-01-21: GLR Forking Phase 2 - Multi-ID Type Specifier Chains

### Problem

Phase 1 (commit b83f165) handled 2-ID chains like `MyType myVar;` but only captured the last identifier. For 3+ identifier chains like `MyStorage MyType myVar;` or `hic_manens constans character*`, the earlier identifiers were lost:

- Input: `MyStorage MyType myVar;`
- Output: `MyType myVar;` (MyStorage lost!)

### Solution Overview

1. Created `type_spec_list` non-terminal that accumulates identifier tokens in a Xar array
2. Added States 1600-1621 for type_spec_list path through declarations
3. Added State 1650 for struct/enum specifiers (direct to declarator, no fork)
4. Added `extra_specifiers` field to Arbor2Declaratio for middle tokens
5. Added productions P535-P538

### Critical Bug Discovery: Dual ordinarius Blocks

The main debugging challenge was that P537 was receiving NULL for the type_spec_list (`valori[1]=0x0`). Traced this to discovering there are TWO `ordinarius:` (default) case blocks in the GLR parser:

- **First block** (~line 4195) - frons_activa (active front) reduction processing
- **Second block** (~line 6344) - reducenda queue reduction processing

P535-P538 handlers were initially only added to the first block. When P535 reduced via the reducenda queue path, the second `ordinarius:` block didn't have the handler, so `valor_novus` remained NIHIL.

**Fix**: Added P535-P538 handlers to BOTH `ordinarius:` blocks.

### Productions Added

```
P535: type_spec_list -> ID                        (start list)
P536: type_spec_list -> type_spec_list ID         (accumulate)
P537: declaratio -> type_spec_list init_decl_list (build declaration)
P538: func_def -> type_spec_list declarator compound (build function)
```

### Key Implementation Details

- P535/P536 store tokens in a Xar cast to Arbor2Nodus* for transport through the parser
- P537 unpacks: first token → tok_storage, middle tokens → extra_specifiers, last token → type specifier
- State 1601 decides: more IDs coming → REDUCE P536 (accumulate); terminal → REDUCE P12 (this is declarator)
- State 1650 prevents struct/enum from entering type_spec_list fork (preserves `struct` keyword)

### Files Modified

- `include/arbor2_glr.h` - Added `extra_specifiers` (Xar*) field to declaratio struct
- `lib/arbor2_glr.c` - Added P535-P538 handlers to BOTH ordinarius blocks (~243 lines)
- `lib/arbor2_glr_tabula.c` - Added INT_NT_TYPE_SPEC_LIST, States 1600-1621, State 1650, P535-P538 rules (~225 lines)
- `lib/arbor2_scribere.c` - Output extra_specifiers after tok_storage
- `probationes/probatio_arbor2_file_roundtrip.c` - Enabled id_chain_extended.c test

### Test Cases

```c
/* probationes/fixa/roundtrip/id_chain_extended.c */
MyStorage MyType myVar;              /* 3 IDs */
MyStorage MyQual MyType myVar2;      /* 4 IDs */
MyA MyB MyC MyD myFinal;             /* 5 IDs */
```

All 46 roundtrip tests pass.

---

## 2026-01-21: Phase 2 - specifiers_ordine Implementation (File-Scope)

### Overview

Implemented `specifiers_ordine` population for file-scope declarations. This field preserves specifier token order for accurate roundtrip serialization.

### Problem

Specifier order wasn't being preserved during roundtrip. For example, `const volatile int x;` vs `volatile const int x;` - both are semantically equivalent but should serialize back to their original form.

### Solution

Added code to populate `specifiers_ordine` (a Xar of Arbor2Token*) in reduction handlers. The serializer (arbor2_scribere.c) already checks for this field and outputs tokens in order when populated.

### Productions Updated

| Group | Productions | Description |
|-------|-------------|-------------|
| Single specifier | P148-P153 | `static/extern/const/volatile type declarator` |
| Single specifier + init | P193-P198 | With `= expr` initializer |
| Single specifier + brace init | P207-P212 | With `= { ... }` initializer |
| Double specifier | P227-P234 | `static const`, `const volatile`, etc. |
| Double specifier + init | P235-P238 | With initializers |
| Modifier combos | P420-P436 | `unsigned int`, `long long`, etc. |
| Implicit int | P527-P534 | `long x`, `unsigned x`, etc. |
| Qualifier + modifier | P437-P504 | `const unsigned int`, `volatile const long`, etc. |
| Typedef | P74, P76, P78, P525, P526 | `typedef type name` |

### Grammar Gaps Discovered

The following valid C89 patterns are NOT in the grammar:

1. **Reversed specifier order**: `const static` (only `static const` supported)
2. **Compound specifier + initializer**: `unsigned int x = 1;` at file scope
3. **Typedef with compound specifiers**: `typedef unsigned int MyUInt;`

These are grammar completeness issues, not C language restrictions. Future work could:
- Add individual missing productions (tedious)
- Refactor to use a general `specifier_list` non-terminal (cleaner but larger change)

### Test Files Added

- `specifier_order.c` - Tests qualifier order preservation (const/volatile)
- `modifier_order.c` - Tests modifier combinations (unsigned long, const unsigned int)
- `typedef_specifiers.c` - Tests typedef declarations and usage

### Results

- All 2176 GLR tests pass
- All 55 roundtrip tests pass

### Next Steps

- Phase 4: Port specifiers_ordine to other contexts (parameters, return types, struct members, locals)
- Consider grammar refactor to specifier_list approach for complete C89 coverage
