# arbor_praeparator.c worklog

## 2026-01-08: HYBRID Preprocessor Mode

### Problem

Files using Latin keyword macros from `latina.h` couldn't roundtrip:
- **PROCESSARE mode**: Expands macros, loses original token text
- **PRESERVARE mode**: Doesn't process includes, parser doesn't learn macros

### Solution: ARBOR_PP_MODUS_HYBRID

Created a new mode that combines learning from PROCESSARE with output preservation from PRESERVARE:

1. **Follows `#include`** recursively to learn macro definitions
2. **Preserves directive tokens** in output (like PRESERVARE)
3. **Does NOT expand macros** in code - keeps original tokens
4. **Exports keyword macro table** for parser to recognize Latin keywords

### Key Implementation

**1. Keyword Macro Table**

Added `TabulaDispersa* keyword_macros` field to `ArborPraeparator` struct. When processing `#define`, check if body is a single keyword token:

```c
si (_est_keyword_vel_typus(body_tok->genus))
{
    keyword_ptr = piscina_allocare(pp->piscina, magnitudo(i32));
    *keyword_ptr = (i32)body_tok->genus;
    tabula_dispersa_inserere(pp->keyword_macros, *macro->titulus, keyword_ptr);
}
```

**2. `_est_keyword_vel_typus()` Helper**

Extended to recognize all keyword types that can be mapped from macros:
- Storage class: `static`, `extern`, `auto`, `register`, `typedef`
- Type qualifiers: `const`, `volatile`
- Type specifiers: `void`, `char`, `int`, `long`, `float`, `double`, `signed`, `unsigned`
- Struct/union/enum keywords
- Control flow: `if`, `else`, `switch`, `case`, `default`, `for`, `while`, `do`, `break`, `continue`, `goto`, `return`, `sizeof`

**3. HYBRID Mode Processing Loop**

```c
si (pp->modus == ARBOR_PP_MODUS_HYBRID)
{
    dum (pos < num)
    {
        /* Detectare directiva: # ad initium lineae */
        si (ad_initium && _est_hash(lex_curr))
        {
            /* Process directive internally (learn macros, follow includes) */
            _processare_directiva(pp, lexemata, pos, num, via_file, NIHIL);

            /* Emit original directive tokens (not processed output) */
            dum (pos < pos_finis) { /* emit tokens */ }
        }

        /* Regular tokens: emit as-is (no expansion) */
        lo = _creare_lexema_origo(pp, lex_curr, NIHIL);
        /* ... */
    }
}
```

**4. API Addition**

```c
TabulaDispersa* arbor_praeparator_obtinere_keyword_macros(ArborPraeparator* pp);
```

Parser calls this after preprocessing to get the keyword macro table.

### Chained Macro Resolution

Handles cases like:
```c
#define hic_manens static
#define interior hic_manens  /* interior -> hic_manens -> static */
```

When processing `interior`'s definition, check if body identifier is itself a keyword macro and chain the resolution.

### Test Results

- cursor.c (2813 bytes) roundtrips byte-exact
- All 71 project tests pass

---

## 2026-01-08: Typedef Detection Brace Tracking

### Problem

Typedef detection in HYBRID mode failed for struct/union typedefs with inline definitions:

```c
nomen structura MachoIteratorMandatum {
    constans MachO* macho;
    i32 index_currens;
} MachoIteratorMandatum;
```

The `_detectare_typedef()` function was finding `macho` as the typedef name instead of `MachoIteratorMandatum` because it broke on the first semicolon encountered (inside the struct body).

### Solution

Added brace depth tracking to both:

1. **Pre-scan loop** in HYBRID mode processing - already had this fix but needed the next fix too

2. **`_detectare_typedef()` function** - track brace depth when iterating through tokens:
   - Only break on semicolons when `brace_depth == 0`
   - Only track identifiers as potential typedef names when `brace_depth == 0`

```c
per (i = pos_initium + I; i < pos_finis; i++)
{
    /* Traceare braces */
    si (lex->genus == ARBOR_LEXEMA_BRACE_APERTA)
        brace_depth++;
    alioquin si (lex->genus == ARBOR_LEXEMA_BRACE_CLAUSA)
        brace_depth--;

    /* Si semicolon ad brace_depth 0, finis declarationis */
    si (lex->genus == ARBOR_LEXEMA_SEMICOLON && brace_depth == ZEPHYRUM)
        frange;

    /* Traceare ultimum identifier (extra braces tantum) */
    si (lex->genus == ARBOR_LEXEMA_IDENTIFICATOR && brace_depth == ZEPHYRUM)
        lex_last_id = lex;
}
```

### Test Results

- sectio.h (7858 bytes) now roundtrips byte-exact
- All 8 candidate files pass roundtrip tests
- 54 formator tests pass

---

## 2026-01-08: Function Pointer Typedef Detection

### Problem

Function pointer typedefs were not being detected by `_detectare_typedef()`:

```c
nomen b32 (*FunctioTractandi)(ContextusTractandi* ctx);
```

The original logic looked for the last identifier before the semicolon, which found `ctx` (the parameter name) instead of `FunctioTractandi` (the typedef name).

### Solution

Extended `_detectare_typedef()` to detect the function pointer pattern `(* IDENTIFIER )`:

1. Track parenthesis depth in addition to brace depth
2. Look for the pattern: `(` followed by `*` followed by `IDENTIFIER`
3. If found, use that identifier as the typedef name instead of the last identifier

```c
/* Detectare function pointer pattern: (* IDENTIFIER ) */
si (in_fn_ptr_name && !fn_ptr_found && brace_depth == ZEPHYRUM)
{
    si (prev_lex != NIHIL &&
        prev_lex->genus == ARBOR_LEXEMA_ASTERISCUS &&
        lex->genus == ARBOR_LEXEMA_IDENTIFICATOR)
    {
        fn_ptr_name = lex;
        fn_ptr_found = VERUM;
    }
}
```

### Test Results

- tractator.c (7722 bytes) now roundtrips byte-exact
- All 71 project tests pass

---

## 2026-01-09: PROCESSARE Mode Origin Chain Cycle Fix

### Problem

PROCESSARE mode tests were hanging infinitely when processing files with nested includes:

```
include_piscina.c → piscina.h → latina.h → stddef.h
```

Debug output showed the hang occurred when processing tokens from nested includes. Cycle detection revealed parent chain depths exceeding 100, indicating infinite loops in the origin traversal.

### Root Cause

In `_processare_include()`, when merging tokens from an included file, the code was mutating shared origin objects:

```c
/* Old problematic code */
ArborOrigo* o = lo->origo;
dum (o->pater != NIHIL)
    o = o->pater;
o->pater = origo_include;  /* MUTATION OF SHARED OBJECT */
```

When multiple tokens share the same origin object (common for tokens from the same line), setting `o->pater` for one token affected all tokens sharing that origin. This created cycles when:

1. Token A's origin chain: `A_origin → root → NULL`
2. We set `root->pater = origo_include`
3. Token B also uses `A_origin`, now chain is: `A_origin → root → origo_include`
4. If `origo_include`'s chain loops back, we get a cycle

### Solution

Instead of traversing and mutating the shared origin chain, simply replace the origin reference:

```c
/* Fixed code */
/* Non mutare origines existentes - hoc evitat cycles quando
 * plura lexemata habent eundem originem obiectum.
 * Perditur catena originis interior sed evitantur cycles. */
lo->origo = origo_include;
```

This loses the inner origin chain information but:
- Avoids all cycle issues
- Origin chain is primarily for error reporting - knowing token came from include is sufficient
- All tests pass (72/72)

### Test Results

- All 72 tests pass
- PROCESSARE mode now works correctly for files with nested includes
- `probatio_processare_parse` tests: minimal.c, minimal2.c, minimal3.c, simple.c, include_test.c, include_stddef.c, piscina_mini.c all pass

---
