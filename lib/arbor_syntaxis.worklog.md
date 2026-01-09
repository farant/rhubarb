# arbor_syntaxis.c worklog

## 2026-01-07: Trivia Preservation for Fidelis Mode

### Problem

Fidelis mode (byte-for-byte roundtrip) was losing whitespace around punctuation tokens. Input `int x = 42;` would output `int x  =42;` (double space before `=`, no space after).

### Root Cause

When tokens like `=`, `{`, `}`, `(`, `)`, `,`, `;` are consumed via `_consumere()` or `_expectare()`, their `trivia_ante` and `trivia_post` are discarded. Only "primary" tokens (identifiers, literals) preserve trivia via `_creare_nodum()` and `_finire_nodum()`.

### Solution Pattern

For each punctuation token consumed, capture and attach trivia:

```c
/* trivia_ante of consumed token -> preceding node's trivia_post */
_copiare_trivia_ad_xar(syn, &prev->trivia_post, token->trivia_ante);

/* token text as synthetic trivia -> preceding node's trivia_post */
_addere_synth_trivia(syn, &prev->trivia_post, ",");

/* trivia_post of consumed token -> following node's trivia_ante */
_copiare_trivia_ad_xar(syn, &next->trivia_ante, token->trivia_post);
```

### Key Rule: When NOT to call `_finire_nodum`

`_finire_nodum` captures `trivia_post` from the previous token (positus - 1). If a child node already captured that trivia, calling `_finire_nodum` on the parent causes **double emission**.

**Do NOT call `_finire_nodum` for nodes whose last child captures final trivia:**

- `_parsere_if` - consequens/alternans have trivia_post from closing `}`
- `_parsere_while` - corpus has trivia_post from closing `}`
- `_parsere_for` - corpus has trivia_post from closing `}`
- `_parsere_binary` - dexter (right operand) has trivia_post
- `_parsere_ternary` - falsum_ramus has trivia_post
- `_parsere_assignment` - valor has trivia_post
- `_parsere_unary` - operandum has trivia_post
- `_parsere_declarator` - children have trivia

Each of these has a comment: `/* NON vocare _finire_nodum - [child] iam habet trivia_post */`

### Formatter Coordination

The formatter must NOT emit hardcoded punctuation when the parser handles it via trivia. For control flow statements (IF, WHILE, FOR), the formatter now only emits keywords and children - punctuation comes from trivia.

### Trivia Ownership Model

For input `) { x`:
- `)` token: trivia_post = " " (space after it)
- `{` token: trivia_ante = "" (empty - space was consumed as `)`'s trivia_post), trivia_post = " "

The lexer assigns trailing same-line whitespace to `trivia_post`. Anything not consumed as trailing becomes `trivia_ante` of the next token. A single space between tokens belongs to exactly one of them, never both.

### Test Coverage

55+ roundtrip tests in `probatio_arbor_formator.c` across four test suites:

**Basic roundtrip (21 tests):**
- declaration, function, struct, control, expressions, call, subscript
- pointer, ternary, assign, unary, parens, for, while, nested-if
- complex, member, compare, bitwise, multi-decl, func-params

**Irregular whitespace (16 tests):**
- double-space-assign, no-space-assign, triple-space
- mixed-spacing-expr, no-space-expr
- double-space-comma, no-space-comma
- space-inside-parens, space-inside-braces, no-space-before-brace
- double-space-else, compact-for, extra-space-for
- asymmetric-left, asymmetric-right, irregular-multi-decl

**Complex structures (19 tests):**
- deep-nested-if, nested-for, for-while, pointer-params
- deep-parens, chained-arrow, mixed-member, multi-subscript
- complex-member-subscript, inc-dec, addr-deref
- nested-ternary, logical-chain, compound-assign
- comma-in-for, do-while, break-continue, complex-return, multi-statement

**Tabs/newlines (7 tests):**
- tabs-basic, mixed-tab-space, tabs-expr, tabs-braces
- tabs-for, tabs-if, tabs-function

---

## 2026-01-07: Function Parameter Trivia Fix

### Problem

Input `int f( int x ) { return x; }` was outputting `int f(int x  ) { return x; }`:
- Space after `(` was lost
- Double space before `)`

### Root Cause

Two issues in function declarator parameter parsing:

1. **`(`'s trivia_post lost**: When skipping `(`, its trivia_post (space after `(`) was not captured and applied to the first parameter.

2. **Double trivia_post emission**: `_finire_nodum(param)` captured trivia_post from the identifier token, but the identifier node inside the declarator ALREADY captured that same trivia. Result: emitted twice.

### Fix

1. Capture `(`'s trivia_post before skipping, prepend to first parameter's first specifier's trivia_ante
2. Remove `_finire_nodum(param)` - declarator children already have trivia
3. Capture `)`'s trivia_ante and append to last parameter's trivia_post

```c
/* Capere trivia_post de '(' ante progredi */
open_paren_lex = _currens_lex(syn);
trivia_post_open_paren = (open_paren_lex != NIHIL) ? open_paren_lex->trivia_post : NIHIL;

/* In first param: prepend trivia_post_open_paren to first specifier */
si (first_param && trivia_post_open_paren != NIHIL)
{
    _copiare_trivia_ad_xar(syn, &first_spec->trivia_ante, trivia_post_open_paren);
}

/* Before consuming ')': capture trivia_ante for last param */
close_paren_lex = _currens_lex(syn);
si (close_paren_lex != NIHIL && prev_param != NIHIL)
{
    _copiare_trivia_ad_xar(syn, &prev_param->trivia_post, close_paren_lex->trivia_ante);
}
```

### Extends the Pattern

Parameter declarations now follow the same rule: don't call `_finire_nodum` when children capture final trivia.

---

---

## 2026-01-07: Do-While Trivia Fix

### Problem

Input `void f() { do { x++; } while (x < 10); }` was outputting `void f() { do{ x++; } while(x < 10); }`:
- Space after `do` lost
- Space after `while` lost

### Fix

Applied same pattern as for/while:
1. Capture `do` keyword's trivia_post → prepend to corpus's trivia_ante
2. Capture `while` keyword's trivia_ante + "while" → append to corpus's trivia_post
3. Capture `while` keyword's trivia_post + "(" + `(`.trivia_post → prepend to conditio's trivia_ante
4. Capture `)` and `;` trivia → append to conditio's trivia_post

Updated formatter to not emit hardcoded `while`, `(`, `)`, `;` - these are now in trivia.

---

## 2026-01-07: Tab Whitespace Tests

Tabs work correctly for most constructs. Added tests:
- tabs-basic, mixed-tab-space, tabs-expr, tabs-braces, tabs-for, tabs-if

### Known Issues

1. **Return keyword trivia**: Tab between `return` and value lost in `return\t0;`
2. **Newline trivia**: Multi-line code gets extra whitespace - leading trivia handling needs work

---

## 2026-01-07: Return Statement Trivia Fix

### Problem

Input `return\t0;` (tab after return) was outputting `return0;` - tab lost.
Input `void f(){ return; }` was outputting `void f(){ return; ; }` - double semicolon.

### Root Cause

Two issues in `_parsere_return`:

1. **`return` keyword's trivia_post lost**: When `_progredi` skips the `return` keyword, its trivia_post (whitespace after `return`) is discarded.

2. **Double semicolon emission for return without value**: The formatter's RETURN_STATEMENT case was manually emitting `nodus->trivia_post`, but `_emittere_nodum_fidelis` also emits `nodus->trivia_post` at line 842. Result: semicolon emitted twice.

### Fix

1. Capture `return` keyword's trivia_post before `_progredi`
2. Prepend to valor's trivia_ante (or nodus's trivia_post for bare `return;`)
3. Capture `;` trivia_ante, synthetic ";", and trivia_post → append to valor's trivia_post
4. Remove `_finire_nodum` - valor already has trivia_post
5. **Formatter**: Remove manual `nodus->trivia_post` emission - let line 842 handle it

```c
/* Capere trivia_post de 'return' ante progredi */
return_lex = _currens_lex(syn);
trivia_post_return = (return_lex != NIHIL) ? return_lex->trivia_post : NIHIL;

_progredi(syn);

/* Cum valore */
si (!_congruit(syn, ARBOR_LEXEMA_SEMICOLON))
{
    valor = _parsere_expressio(syn, PREC_NIHIL);

    /* Praepone trivia_post_return ad valor.trivia_ante */
    si (trivia_post_return != NIHIL)
    {
        old_ante = valor->trivia_ante;
        valor->trivia_ante = NIHIL;
        _copiare_trivia_ad_xar(syn, &valor->trivia_ante, trivia_post_return);
        _copiare_trivia_ad_xar(syn, &valor->trivia_ante, old_ante);
    }

    /* Capere ';' trivia */
    semi_lex = _currens_lex(syn);
    _copiare_trivia_ad_xar(syn, &valor->trivia_post, semi_lex->trivia_ante);
    _addere_synth_trivia(syn, &valor->trivia_post, ";");
    _copiare_trivia_ad_xar(syn, &valor->trivia_post, semi_lex->trivia_post);
}
```

### Key Insight: Formatter Double Emission

`_emittere_nodum_fidelis` always emits `nodus->trivia_post` at the end (line 842). If a case handler also emits it manually, you get double emission. For return without value, the fix was to let line 842 handle it instead of emitting in the case handler.

---

## 2026-01-07: Sizeof Trivia Fix

### Problem

Input `sizeof (int)` (with space before paren) was outputting `sizeof(int)` - space lost.

### Root Cause

Two issues in `_parsere_sizeof`:

1. **`sizeof` keyword's trivia_post lost**: When `_progredi` skips `sizeof`, its trivia_post (space after `sizeof`) is discarded.

2. **Formatter hardcodes punctuation**: For `sizeof(type)`, formatter hardcoded `"("` and `")"`, losing any space before `(`.

### Fix

Applied same trivia pattern:

1. Capture `sizeof` keyword's trivia_post before `_progredi`
2. For sizeof(type):
   - Prepend trivia_post_sizeof + "(" + trivia_post_open to type's trivia_ante
   - Append trivia_ante_close + ")" + trivia_post_close to type's trivia_post
3. For sizeof expr:
   - Prepend trivia_post_sizeof to expr's trivia_ante
4. Formatter: just emit "sizeof" + operandum (trivia carries everything else)
5. Don't call `_finire_nodum` - operandum already has trivia_post

---

## 2026-01-07: Newline Trivia Duplication Fix

### Problem

Multi-line code with newlines and indentation caused double whitespace emission:

Input: `void f() {\n    int a;\n    int b;\n}`
Output: `void f() {\n    \n    int a;\n    \n    int b;\n}` (extra blank lines)

### Root Cause

When `_creare_nodum` creates a node, it captures `trivia_ante` from the current token. For declarations:

1. `_creare_nodum(DECLARATION)` captures `int`'s trivia_ante (`\n    `)
2. `_parsere_specifiers()` creates TYPE_SPECIFIER, which ALSO calls `_creare_nodum` and captures the SAME `int`'s trivia_ante (`\n    `)

Both parent (DECLARATION) and child (TYPE_SPECIFIER) have the same trivia_ante. When emitted via `_emittere_nodum_fidelis`, both emit their trivia_ante → double emission.

### Fix

After parsing specifiers, clear the declaration's trivia_ante since the first specifier will have it:

```c
/* Parse specifiers */
_parsere_specifiers(syn, specifiers);

/* Evitare trivia duplicata - primus specifier iam habet trivia_ante.
 * Si non clearimus, emittitur bis: semel de declaration, semel de specifier. */
si (xar_numerus(specifiers) > ZEPHYRUM)
{
    nodus->trivia_ante = NIHIL;
}
```

### Key Rule: Parent-Child Trivia Duplication

When a parent node and its first child are created at the same token position, they both capture the same trivia_ante. The parent should clear its trivia_ante to avoid double emission.

This applies to any nested `_creare_nodum` pattern where the outer node is created before parsing children that will capture the same starting token's trivia.

---

## 2026-01-07: Cast Expression Parsing

### Problem

Parser couldn't parse cast expressions like `(int)x`. Input `int f() { return (int)x; }` would fail to parse entirely.

### Root Cause

`_parsere_parenthesis` always assumed content after `(` was an expression, not a type-name. It would try to parse `int` as an expression and fail.

### Solution

Added type detection in `_parsere_parenthesis`:

1. Created `_est_type_initium` helper to check if current token starts a type-name:
   - Type specifiers: `void`, `char`, `int`, `long`, `float`, `double`, `signed`, `unsigned`, `struct`, `union`, `enum`
   - Type qualifiers: `const`, `volatile`
   - Typedef names (using existing `_est_typedef_nomen` lookup)

2. Modified `_parsere_parenthesis`:
   - After `(`, call `_est_type_initium`
   - If true: create CAST_EXPRESSION node, parse type, consume `)`, parse cast operand
   - If false: parse as parenthesized expression (existing behavior)

3. Trivia handling for cast:
   - Type node's `trivia_ante`: `(` trivia + synthetic `"("` + post-`(` trivia
   - Type node's `trivia_post`: pre-`)` trivia + synthetic `")"` + post-`)` trivia
   - Expression inherits its own trivia

4. Formatter updated to just emit `typus` and `expressio` - parentheses come from trivia.

### Test Coverage

Added roundtrip tests:
- `(int)x` - simple cast
- `(long)y + (char)z` - multiple casts
- `(int)(x + y)` - cast of parenthesized expression

### Limitations

Current implementation only handles simple single-specifier types. Complex type-names like `(const int *)x` or `(struct foo *)x` would need additional work to parse the full type-name including abstract declarators.

---

## 2026-01-07: Typedef Names in Struct Member Declarations - FIXED

### Problem

Parser couldn't parse typedef names as types inside struct member declarations:

```c
typedef long Size;
struct S { Size len; };  /* FAILED - parser didn't recognize "Size" as a type */
```

Parsing succeeded for regular declarations (`Size x;`) but failed when the same typedef name appeared in a struct member.

### Root Cause

`_parsere_struct_declaration` had a simpler type parsing loop than regular declarations. It only checked:

```c
dum (lex != NIHIL &&
     (_est_type_specifier(lex->genus) || _est_type_qualifier(lex->genus)))
```

This recognized keywords (`int`, `char`, `const`, etc.) but NOT typedef names (which are IDENTIFIER tokens that need lookup in the typedef table).

Meanwhile, regular declaration parsing in `_parsere_declaratio` had full typedef name handling via `_est_typedef_nomen()`.

### Fix

Expanded `_parsere_struct_declaration`'s type parsing loop to match regular declarations:

```c
/* Parse type specifiers (including typedef names) */
{
    b32 habet_typedef_nomen = FALSUM;
    lex = _currens_lex(syn);
    dum (lex != NIHIL)
    {
        ArborNodus* spec = NIHIL;

        si (_est_type_specifier(lex->genus))
        {
            spec = _parsere_type_specifier(syn);
        }
        alioquin si (_est_type_qualifier(lex->genus))
        {
            spec = _parsere_type_qualifier(syn);
        }
        alioquin si (lex->genus == ARBOR_LEXEMA_IDENTIFICATOR && !habet_typedef_nomen)
        {
            /* Verifica typedef nomen - solum unum permissum */
            chorda* titulus = chorda_internare(syn->intern, lex->valor);
            si (_est_typedef_nomen(syn, titulus))
            {
                spec = _creare_nodum(syn, ARBOR_NODUS_TYPEDEF_NAME);
                si (spec != NIHIL)
                {
                    spec->datum.folium.valor = titulus;
                    _progredi(syn);
                    _finire_nodum(syn, spec);
                }
                habet_typedef_nomen = VERUM;
            }
            alioquin
            {
                frange;
            }
        }
        alioquin
        {
            frange;
        }

        /* ... add spec to specifiers ... */
    }
}
```

Also added forward declaration for `_parsere_type_qualifier`.

### Test Coverage

Added to `probatio_arbor_syntaxis.c`:
- `typedef long Size; struct S { Size len; };` - single typedef member
- `typedef int Int; typedef char Char; struct S { Int x; Char c; };` - multiple typedef members

Added to `probatio_arbor_typus.c`:
- `probatio_typus_typedef_in_struct` - verifies typedef types resolve correctly in struct members

### Key Pattern

Anywhere the parser needs to recognize types, it must check for typedef names in addition to type keywords. This includes:
- Regular declarations (already working)
- Struct/union member declarations (now fixed)
- Cast expressions (already working)
- Function parameter declarations (already working)

---

## 2026-01-07: Complex Cast Types - IMPLEMENTED

### Problem

Parser only supported simple single-specifier casts like `(int)x`. Complex casts with pointers or multiple specifiers failed:
- `(int *)x` - pointer cast
- `(const int *)x` - qualified pointer cast
- `(unsigned long)x` - multi-specifier cast

Similarly, `sizeof(int *)` was not supported.

### Solution

Created `_parsere_type_name()` function that parses the C grammar for type-name:
```
type-name:
    specifier-qualifier-list abstract-declarator_opt
```

The function:
1. Creates a PARAMETER_DECLARATION node (which has `specifiers` + `declarator` fields)
2. Parses multiple specifiers via existing `_parsere_specifiers()`
3. If `*` or `(` follows, parses abstract declarator via existing `_parsere_declarator()`

The abstract declarator is the pointer chain without an identifier - e.g., for `int *`, the declarator is just a POINTER node.

### Files Modified

**`lib/arbor_syntaxis.c`**:
- Added `_parsere_type_name()` function (~50 lines)
- Modified `_parsere_parenthesis()` to use `_parsere_type_name()` for casts
- Modified `_parsere_sizeof()` to use `_parsere_type_name()` for sizeof(type)
- Added forward declarations

**`probationes/probatio_arbor_formator.c`**:
- Added 9 roundtrip tests for complex casts

### Trivia Handling

For casts, trivia is attached as follows:
- `(` trivia → prepended to first specifier's trivia_ante
- `)` trivia → appended to type_name node's trivia_post
- The type_name's trivia_post is emitted after specifiers/declarator

### Test Cases

All passing:
- `(int)x` - simple cast (baseline)
- `(int *)x` - pointer cast
- `(int **)x` - double pointer cast
- `(const int *)x` - const pointer cast
- `(unsigned long)x` - multi-specifier cast
- `(void *)x` - void pointer cast
- `(char *)x` - char pointer cast
- `sizeof(int *)` - sizeof with pointer type
- `sizeof(const int)` - sizeof with multiple specifiers

### Key Design Decision

Reused PARAMETER_DECLARATION node type for type-names since it has exactly the structure needed (`Xar* specifiers` + `ArborNodus* declarator`). The formatter's existing PARAMETER_DECLARATION handler already emits specifiers + declarator correctly.

---

## 2026-01-07: Whole-File Roundtrip Testing Fixes

### Goal

Set up whole-file roundtrip testing by copying real C files to `probationes/fixa/roundtrip/` and testing byte-exact formatting.

### Issues Fixed

**1. Empty Compound Statement Trivia Loss**

Problem: `void f()\n{\n}\n` was outputting `void f()\n{}\n` - newline between braces lost.

Root cause: In `_parsere_compound`, when the body is empty (no statements), the `}` token's trivia_ante (the newline) was only captured if `ultimum_item != NIHIL`. For empty bodies, this trivia was lost.

Fix: Added `trivia_vacuum` field to compositum structure. When body is empty, store `}` trivia_ante in this field. Formatter emits it between `{` and `}`:

```c
/* In parser - _parsere_compound */
si (ultimum_item != NIHIL)
{
    _copiare_trivia_ad_xar(syn, &ultimum_item->trivia_post, close_lex->trivia_ante);
}
alioquin
{
    _copiare_trivia_ad_xar(syn, &nodus->datum.compositum.trivia_vacuum, close_lex->trivia_ante);
}

/* In formatter */
si (num > ZEPHYRUM) { /* emit statements */ }
alioquin
{
    si (nodus->datum.compositum.trivia_vacuum)
    {
        _emittere_trivia(status, nodus->datum.compositum.trivia_vacuum);
    }
}
```

**2. Void Parameter Lost**

Problem: `void f(void)` was outputting `void f()` - the explicit `void` parameter was lost.

Root cause: Parser special-cased `(void)` to mean "no parameters" and just skipped the `void` token entirely (lines 3828-3837). This was semantically correct but broke fidelis roundtrip.

Fix: Instead of skipping `void`, create a proper PARAMETER_DECLARATION node with a TYPE_SPECIFIER for `void`:

```c
void_param = _creare_nodum(syn, ARBOR_NODUS_PARAMETER_DECLARATION);
void_spec = _creare_nodum(syn, ARBOR_NODUS_TYPE_SPECIFIER);
void_spec->datum.folium.keyword = ARBOR_LEXEMA_VOID;
/* Capture trivia from void token */
/* Add to specifiers and parameter list */
```

**3. Comment Duplication in Translation Unit**

Problem: Leading comments were emitted twice. `/* comment */\nint x;` became `/* comment */\n/* comment */\nint x;`.

Root cause: Both the translation unit AND its first child (external declaration) captured the same first token's trivia_ante via `_creare_nodum`. When `_emittere_nodum_fidelis` emits trivia_ante for both, it's emitted twice.

Fix: Skip trivia_ante emission for TRANSLATION_UNIT in the formatter since it has no actual leading token - it's just a container:

```c
si (nodus->genus != ARBOR_NODUS_TRANSLATION_UNIT)
{
    _emittere_trivia(status, nodus->trivia_ante);
}
```

**4. File Roundtrip Test strlen Bug**

Problem: Test was hanging indefinitely when reading files.

Root cause: `_credo_roundtrip_filum` used `_parsere_fontem` helper which calls `strlen(fons)`. But chorda data from `filum_legere_totum` is NOT null-terminated. This caused `strlen` to read past the buffer until finding a random null byte.

Fix: Call `arbor_syntaxis_parsere_fontem` directly with `fons.mensura`:

```c
res = arbor_syntaxis_parsere_fontem(
    syn,
    (constans character*)fons.datum,
    fons.mensura,  /* Use actual length, not strlen */
    via);
```

### Test Results

All 54 formator tests pass including whole-file roundtrip of `simple.c` (419 bytes).

## 2026-01-07: Preprocessor Directive Preservation

### Problem

Real C files contain preprocessor directives (`#include`, `#define`, `#ifdef`, etc.) that were being expanded/processed by the preprocessor, making byte-exact roundtrip formatting impossible.

### Solution: PRESERVARE Mode

Added a mode flag to the preprocessor that passes all tokens through unchanged, allowing the parser to preserve directives as AST nodes.

### Changes Made

**1. include/arbor_praeparator.h**
- Added `ArborPPModus` enum with `ARBOR_PP_MODUS_PROCESSARE` (default: expand) and `ARBOR_PP_MODUS_PRESERVARE` (pass through)
- Added `arbor_praeparator_ponere_modum()` and `arbor_praeparator_obtinere_modum()` API functions

**2. include/arbor_syntaxis.h**
- Added `ARBOR_NODUS_DIRECTIVE` to `ArborNodusGenus` enum
- Added `datum.directiva` struct with `genus` (ArborDirectivaGenus) and `lexemata` (Xar of tokens)

**3. lib/arbor_praeparator.c**
- Added `modus` field to `ArborPraeparator` struct
- In `arbor_praeparator_processare_lexemata()`, when PRESERVARE mode, simply wrap all tokens as ArborLexemaOrigo and pass through without processing

**4. lib/arbor_syntaxis.c**
- Added `_est_directiva_initium()` - detects `#` at line start
- Added `_identificare_directiva_genus()` - identifies directive type from keyword
- Added `_parsere_directiva()` - collects all tokens on directive line into DIRECTIVE node
- Modified `_parsere_translation_unit()` to check for directives before declarations
- Modified `_parsere_compound()` to check for directives before statements/declarations

**5. lib/arbor_formator.c**
- Added `ARBOR_NODUS_DIRECTIVE` case: emit all tokens with their trivia
- Skip `trivia_ante` for DIRECTIVE nodes (since token trivia handles it)

### Key Design Decisions

1. **Mode on praeparator, not separate code path**: Reuses existing infrastructure for lexing, keeps directive identification logic
2. **Parser detects directives itself**: No special markers needed from praeparator; parser checks `#` at line start just like praeparator does
3. **Directive tokens stored in node**: All tokens from `#` through end of line stored in `datum.directiva.lexemata`, preserving exact formatting
4. **Skip double trivia emission**: DIRECTIVE nodes skip `trivia_ante` emission in formatter since individual token trivia is emitted

### Test Coverage

Added 10 preserve mode roundtrip tests:
- `#include <stdio.h>` - system include
- `#include` + declaration - mixed content
- `#define FOO 42` - simple macro
- `#define` + use - macro with usage
- Multiple defines - consecutive directives
- `#ifdef` block - conditional compilation
- `#ifndef` guard - include guard pattern
- `#if`/`#else`/`#endif` - full conditional
- `#pragma once` - pragma directive
- Mixed directives and code - directives inside functions

### Usage

```c
/* For fidelis formatting with directive preservation */
ArborPraeparator* pp = arbor_praeparator_creare(piscina, intern);
arbor_praeparator_ponere_modum(pp, ARBOR_PP_MODUS_PRESERVARE);
/* ... then normal parse/format flow */
```

### Test Results

All 54 formator tests pass including 10 new preserve mode tests. All 71 project tests pass.

---

## 2026-01-07: Directive and Declaration Trivia Duplication Fixes

### Problem 1: Directive Trivia Duplication

Input `#endif /* FOO_H */\n` was outputting `#endif /* FOO_H */\n /* FOO_H */\n` - comment duplicated.

### Root Cause

Multiple layers capturing the same trivia:

1. **Directive token trivia**: The `endif` token had ` /* FOO_H */` as `trivia_post`, emitted when formatting directive lexemata
2. **Directive node trivia_post**: `_finire_nodum` set directive's `trivia_post` from `endif`'s `trivia_post` → duplicate
3. **Translation unit trivia_post**: `_finire_nodum` for translation_unit also captured `endif`'s `trivia_post`, PLUS EOF trivia was appended → triple

### Fix

1. **Directive parsing**: Don't call `_finire_nodum` for directive nodes. Only set byte/line/column positions, not `trivia_post`. Directive tokens already have all trivia.

2. **Translation unit parsing**: Don't call `_finire_nodum` for translation_unit. Only capture EOF's `trivia_ante` as the unit's `trivia_post`, not from the last consumed token.

3. **Formatter**: Skip `nodus->trivia_post` emission for DIRECTIVE nodes (token trivia handles it).

---

### Problem 2: Multi-line Declaration Trivia Duplication

Input `void\nfoo(void);` was outputting `void\n\nfoo(void);` - extra newline.

### Root Cause

When parsing a declaration, multiple nested nodes captured the same `trivia_ante` from the `foo` token:

- `INIT_DECLARATOR.trivia_ante = "\n"` (from `_creare_nodum`)
- `DECLARATOR.trivia_ante = "\n"` (from `_creare_nodum` inside `_parsere_declarator`)
- `IDENTIFIER.trivia_ante = "\n"` (from `_parsere_identificator`)

All three emit their trivia_ante → three newlines instead of one.

### Fix

Added trivia deduplication checks:

1. **In `_parsere_declaratio`**: After creating `init_decl` and parsing `decl`:
   ```c
   si (init_decl->trivia_ante == decl->trivia_ante && decl->trivia_ante != NIHIL)
   {
       init_decl->trivia_ante = NIHIL;
   }
   ```

2. **In `_parsere_declarator`**: After creating `nodus` and parsing `inner` identifier:
   ```c
   si (nodus->trivia_ante == inner->trivia_ante && inner->trivia_ante != NIHIL)
   {
       nodus->trivia_ante = NIHIL;
   }
   ```

### Key Pattern

When parent and child nodes are created at the same token position, they capture the same `trivia_ante`. Clear the parent's `trivia_ante` to avoid double emission. The child (innermost node) should own the trivia.

---

### Problem 3: Custom Type Names in PRESERVARE Mode - NOT FIXED

Files with custom typedef names like `TabulaPixelorum*` or `i32` cause infinite loops in PRESERVARE mode.

### Root Cause

In PRESERVARE mode, `#include` directives are not processed, so typedef names from headers are never registered. When the parser encounters `TabulaPixelorum* x` in a parameter list:

- `TabulaPixelorum` is just an IDENTIFIER token
- Parser doesn't recognize it as a type
- Parsing enters infinite loop trying to find a valid parameter

### Current Status

This is a fundamental limitation of PRESERVARE mode. Files using custom types cannot roundtrip until either:

1. Type annotations/configuration allow pre-registering typedef names
2. Parser uses heuristics (e.g., identifier followed by `*` before `,` or `)` is likely a type)
3. Multi-pass approach: first pass to collect typedef names, second pass to parse

### Test Coverage

Added 8 new preserve mode tests (all passing):
- `#endif /* FOO_H */` - directive with trailing comment
- `void\nfoo(void);` - multi-line simple function declaration
- `void\nfoo(\n    int* x,\n    int y);` - multi-line with params
- `/* comment */\nvoid\nfoo(...);` - comment block then function
- Full header pattern with guards, includes, blank lines

---

## 2026-01-08: Typedef Name Heuristics in PRESERVARE Mode

### Problem

In PRESERVARE mode, `#include` directives are not processed, so typedef names from headers are never registered. Files using custom types like `TabulaPixelorum*` or `i32` caused infinite loops because the parser didn't recognize them as types.

### Solution: Heuristic Type Detection

Added `_est_probable_typedef_nomen()` function that uses heuristics to detect likely typedef names based on what follows the identifier:

```c
interior b32
_est_probable_typedef_nomen(ArborSyntaxis* syn)
{
    ArborLexema* next = _prospicere_lex(syn, I);
    si (next == NIHIL) redde FALSUM;

    /* IDENTIFIER * - pointer to type (e.g., TabulaPixelorum*) */
    si (next->genus == ARBOR_LEXEMA_ASTERISCUS)
        redde VERUM;

    /* IDENTIFIER IDENTIFIER - type followed by name (e.g., i32 x) */
    si (next->genus == ARBOR_LEXEMA_IDENTIFICATOR)
        redde VERUM;

    redde FALSUM;
}
```

Modified `_parsere_specifiers` to use this heuristic when no registered typedef is found:
```c
si (_est_typedef_nomen(syn, titulus) || _est_probable_typedef_nomen(syn))
```

### Initial Bug: Overly Aggressive Heuristic

First implementation also checked for:
- `IDENTIFIER )` - end of parameter list
- `IDENTIFIER ,` - parameter separator
- `IDENTIFIER [` - array declarator

This caused a **segfault** in `probatio_arbor_typus` with function pointer declarations like `int (*fp)(int);`. When parsing at `fp`, the lookahead was `)`, which incorrectly triggered the heuristic - treating `fp` as a typedef name instead of a declarator identifier.

### Fix

Removed the aggressive patterns. The simplified heuristic only checks:
1. `IDENTIFIER *` - pointer to type
2. `IDENTIFIER IDENTIFIER` - type followed by name

These patterns are reliable because:
- A type name followed by `*` is unambiguously a pointer-to-type declaration
- A type name followed by another identifier is unambiguously type + variable name

The removed patterns had too many false positives in declarator contexts.

### Test Results

- cursor.h (1052 bytes) roundtrips successfully with the simplified heuristic
- All 179 arbor_typus tests pass (no segfault)
- All 54 arbor_formator tests pass
- All 71 project tests pass

### Key Insight

Heuristics for type detection must be conservative to avoid breaking declarator parsing. Function pointers like `int (*fp)(int)` have identifiers in positions that superficially look like type positions. The `*` and `IDENTIFIER IDENTIFIER` patterns are safe because they're unambiguous in C grammar.

---

## 2026-01-08: HYBRID Preprocessor Mode and Keyword Macro Roundtrip

### Problem

cursor.c (2813 bytes) uses Latin keyword macros from `latina.h` like `si` (→`if`), `redde` (→`return`), `perge` (→`continue`), `per` (→`for`). These couldn't roundtrip because:

1. **PROCESSARE mode**: Expands macros → loses original token text
2. **PRESERVARE mode**: Doesn't process includes → parser doesn't recognize Latin keywords

### Solution: HYBRID Mode

Created `ARBOR_PP_MODUS_HYBRID` that:
1. **Follows `#include`** to learn macro definitions (like PROCESSARE)
2. **Preserves directive tokens** in output (like PRESERVARE)
3. **Does NOT expand macros** in code - keeps original tokens
4. **Exports keyword macro table** for parser to recognize Latin keywords

### Key Implementation Details

**1. Keyword Macro Table (arbor_praeparator.c)**

When processing `#define`, check if body is a single keyword token:
```c
si (def->corpus != NIHIL && xar_numerus(def->corpus) == I)
{
    ArborLexema* body_tok = *(ArborLexema**)xar_obtinere(def->corpus, ZEPHYRUM);
    si (body_tok != NIHIL && _est_keyword_vel_typus(body_tok->genus))
    {
        i32* slot = tabula_dispersa_inserere(pp->keyword_macros, def->titulus);
        si (slot) *slot = (i32)body_tok->genus;
    }
}
```

**2. Parser Integration (arbor_syntaxis.c)**

Multiple changes to check keyword_macros table:

- `_est_declaration_start()`: Check if identifier maps to declaration keyword
- `_parsere_sententia()`: Use `effective_genus` from keyword_macros for statement dispatch
- `_congruit_effectivum()`: New helper that checks both actual genus AND keyword_macros
- Used for `else` handling in if-statements

**3. Keyword Text Preservation for Roundtrip**

Control flow keywords needed original text preserved in trivia. Created `_addere_synth_trivia_chorda()` helper.

For each keyword statement:
- **if-statement**: Add `if_lex->valor` to condition's trivia_ante
- **else**: Add `else_lex->valor` to consequens's trivia_post
- **for-statement**: Add `for_lex->valor` to init's trivia_ante
- **return-statement**: Add `return_lex->valor` to valor's trivia_ante (or nodus if no value)
- **continue-statement**: Add `continue_lex->valor` to nodus's trivia_ante

**4. Formatter Updates (arbor_formator.c)**

Remove hardcoded keyword strings - they now come from trivia:
- IF_STATEMENT: Remove `"if"` output
- FOR_STATEMENT: Remove `"for"` output
- RETURN_STATEMENT: Remove `"return"` output
- CONTINUE_STATEMENT: Remove `"continue"` and `";"` output

### Edge Cases

**Control flow keywords in `_est_keyword_vel_typus()`**: Had to add `IF`, `ELSE`, `SWITCH`, `CASE`, `DEFAULT`, `FOR`, `WHILE`, `DO`, `BREAK`, `CONTINUE`, `GOTO`, `RETURN`, `SIZEOF` for keyword macro detection.

**`_est_declaration_start()` conflict**: When identifier maps to control flow keyword (like `si` → IF), must return FALSUM to prevent treating it as declaration start. Previously `_est_probable_typedef_nomen()` would return true for `si (x)` pattern.

### Test Results

- cursor.c (2813 bytes) roundtrips byte-exact
- All 54 formator tests pass
- All project tests pass

### Files Modified

- `include/arbor_praeparator.h`: Added `ARBOR_PP_MODUS_HYBRID`, `arbor_praeparator_obtinere_keyword_macros()`
- `lib/arbor_praeparator.c`: HYBRID mode implementation, keyword macro table building
- `include/arbor_syntaxis.h`: (already had `arbor_syntaxis_ponere_keyword_macros`)
- `lib/arbor_syntaxis.c`: keyword_macros integration in parser, keyword text preservation
- `lib/arbor_formator.c`: Removed hardcoded keyword strings
- `probationes/probatio_arbor_formator.c`: Added `_parsere_fontem_hybrid()` and HYBRID mode tests

---

## 2026-01-08: Expression Statement Trivia Transfer Fix

### Problem

Multi-line code was getting extra blank lines after `{` in compound statements. Input like:
```c
void f() {
    int x;
}
```
Was outputting:
```c
void f() {

    int x;
}
```

### Root Cause

The `_clarum_duplicatum_trivia()` function compared Xar **pointers** to detect duplicate trivia between parent and child nodes. When `_parsere_parenthesis` created a new Xar with copied trivia (not the same pointer), the duplicate check failed, causing both the expression and the expression statement to emit the same trivia.

### Fix

Removed `_clarum_duplicatum_trivia()` entirely. Instead, in `_parsere_expression_statement`, explicitly transfer trivia ownership from expression to statement:

```c
/* Transfer leading trivia from expression to statement
 * This avoids duplication when parenthesized expressions
 * prepend trivia to a new Xar (breaking pointer equality check).
 */
nodus->trivia_ante = expr->trivia_ante;
expr->trivia_ante = NIHIL;
```

This ensures the statement owns the leading trivia, and the expression's trivia_ante is cleared.

---

## 2026-01-08: Keyword Macro Support in Struct Members

### Problem

Struct member declarations using Latin keyword macros weren't parsing:
```c
structura Piscina {
    vacuum* alveus_nunc;  /* FAILED - "Expectabatur ;" */
};
```

The parser recognized `vacuum` as a typedef macro but not as a type specifier inside struct declarations.

### Root Cause

`_parsere_struct_declaration()` only checked raw token types (`ARBOR_LEXEMA_VOID`, `ARBOR_LEXEMA_INT`, etc.) for type specifiers. It didn't check the `keyword_macros` table to see if an identifier like `vacuum` maps to `ARBOR_LEXEMA_VOID`.

### Fix

Added keyword macro lookup to `_parsere_struct_declaration()`:

```c
/* Verifica keyword macros (vacuum -> void, integer -> int, etc.) */
si (syn->keyword_macros != NIHIL)
{
    vacuum* keyword_val = NIHIL;
    b32 found = tabula_dispersa_invenire(syn->keyword_macros, *titulus, &keyword_val);
    si (found)
    {
        i32 keyword_type = *(i32*)keyword_val;
        /* Type specifiers */
        si (keyword_type == ARBOR_LEXEMA_VOID ||
            keyword_type == ARBOR_LEXEMA_CHAR ||
            keyword_type == ARBOR_LEXEMA_INT || ...)
        {
            spec = _creare_nodum(syn, ARBOR_NODUS_TYPE_SPECIFIER);
            spec->datum.folium.keyword = (ArborLexemaGenus)keyword_type;
            spec->datum.folium.valor = titulus;
            _progredi(syn);
            _finire_nodum(syn, spec);
            handled = VERUM;
        }
        /* Type qualifiers (constans -> const) similarly... */
    }
}
```

Also handles typedef macros (e.g., `i32` → `int` typedef) within struct members.

### Test Results

- piscina.h (3181 bytes) roundtrips byte-exact
- All 8 candidate files pass roundtrip tests
- 54 formator tests pass

---

---

## 2026-01-08: Goto Statement Keyword Preservation

### Problem

`salta` (Latin for `goto`) was being output as `goto` in roundtrip tests. The goto statement parsing didn't preserve the original keyword token.

### Solution

Updated `_parsere_goto()` to capture the keyword token and assemble trivia properly:

```c
/* Capere goto keyword lexeme */
goto_lex = _currens_lex(syn);
_progredi(syn);

/* ... get label and semicolon ... */

/* Attachere keyword + trivia */
si (goto_lex != NIHIL)
{
    _addere_synth_trivia_chorda(syn, &nodus->trivia_ante, goto_lex->valor);
}
_copiare_trivia_ad_xar(syn, &nodus->trivia_ante, goto_lex->trivia_post);
_addere_synth_trivia_chorda(syn, &nodus->trivia_ante, label_lex->valor);
/* ... etc ... */
```

Also updated the formatter (`arbor_formator.c`) to not emit anything for GOTO_STATEMENT - the trivia handles the entire statement.

### Test Results

- tractator.c (7722 bytes) roundtrips byte-exact
- All 71 project tests pass

---

## Known TODOs

All basic constructs now have trivia handling. Remaining advanced features:

1. **Array types in casts**: `(int[10])x` - array declarators in type-names
2. **Function pointer casts**: `(int (*)(void))x` - function pointer type-names

---

## 2026-01-08: Recursion Depth Limit

### Problem

Deeply nested code (e.g., `if(1)if(1)if(1)...` repeated 1000+ times) could cause stack overflow since the parser uses true recursion for statements and compound blocks.

### Solution

Added recursion depth tracking with a limit of 256 levels:

**1. Added depth field to ArborSyntaxis struct:**
```c
#define ARBOR_MAX_RECURSIO_PROFUNDITAS  CCLVI  /* 256 */
i32 recursio_profunditas;  /* in struct */
```

**2. Added helper function:**
```c
interior b32
_recursio_nimis_profunda(ArborSyntaxis* syn)
{
    redde syn->recursio_profunditas >= ARBOR_MAX_RECURSIO_PROFUNDITAS;
}
```

**3. Added depth checks to recursive functions:**
- `_parsere_compound()` - increments at start, decrements at end
- `_parsere_sententia()` - increments at start, decrements at end

When depth limit is hit, an error is added and NULL is returned, preventing stack overflow.

### Test Results

All 71 project tests pass.

---
