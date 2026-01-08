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
