# arbor_formator Specification v2

Phase 8 of arbor_ce_lxxxix: Code formatter for C89 AST with Latin localization.

## Overview

Two operating modes:
- **Fidelis**: Exact byte-for-byte roundtrip (preserves original trivia)
- **Formatus**: Apply formatting rules (this spec)

## Files to Create

| File | Purpose |
|------|---------|
| `include/arbor_formator.h` | Formatter API and options |
| `lib/arbor_formator.c` | Implementation |
| `probationes/probatio_arbor_formator.c` | Tests |

## Dependencies

- `arbor_syntaxis` (AST nodes, trivia)
- `chorda_aedificator` (string building)
- `piscina` (allocation)
- `internamentum` (string interning)
- `xar` (arrays)

## API

```c
/* Formatus mode - apply all formatting rules */
chorda arbor_formator_emittere (
               Piscina* piscina,
            ArborNodus* radix,
    ArborFormatorOptiones* optiones);

/* Fidelis mode - exact roundtrip */
chorda arbor_formator_emittere_fidelis (
       Piscina* piscina,
    ArborNodus* radix);

/* Format single node/subtree */
chorda arbor_formator_emittere_nodum (
               Piscina* piscina,
            ArborNodus* nodus,
    ArborFormatorOptiones* optiones);

/* Format range (line-based) */
chorda arbor_formator_emittere_intervallum (
               Piscina* piscina,
            ArborNodus* radix,
                   i32  linea_initium,
                   i32  linea_finis,
    ArborFormatorOptiones* optiones);
```

### Options Structure

```c
nomen structura {
    b32      latinizare_keywords;     /* Convert C keywords to Latin */
    b32      romanizare_numeros;      /* Convert numbers to Roman numerals */
    i32      max_linea_longitudo;     /* Default: LXXX (80) */
    i32      indentatio_spatia;       /* Default: II (2) */
    chorda*  latina_via;              /* Path to latina.h for numeral updates */
} ArborFormatorOptiones;
```

---

## Formatting Rules

### 1. Braces: Allman Style

```c
si (x)
{
    /* body */
}
```

Opening brace always on its own line.

### 2. Type Alignment: Right-Aligned with 2-char Gutter

```c
structura Piscina {
            Alveus* primus;           /* pointer * in gutter */
            Alveus* nunc;
    memoriae_index  mensura;          /* non-pointer, 2 spaces before name */
         character* titulus;
               b32  est_dynamicum;
};
```

Rules:
- Types right-aligned to longest type in group
- 2-character gutter between type column and name column
- Pointer `*` occupies rightmost position of gutter (1 space before name)
- Non-pointers have 2 spaces before name
- Group boundary: same statement type (declarations together, assignments together)

### 3. Assignment Alignment

```c
alveus->capacitas = capacitas;
alveus->offset    = ZEPHYRUM;
alveus->sequens   = NIHIL;
```

- `=` aligned to longest LHS in group
- Independent from declaration alignment

### 4. Function Definitions

```c
interior memoriae_index
_proxima_ordinatio (
        memoriae_index ptr,
        memoriae_index ordinatio)
{
    /* body */
}
```

- Return type on own line
- Function name + space + `(` on next line
- Each parameter on own line, types right-aligned
- `)` at end of last parameter line
- `{` on own line (Allman)

### 5. Function Calls

```c
/* Short - one line */
func(a, b, c);

/* Long - align with first arg */
long_function_name(first_argument,
                   second_argument,
                   third_argument);
```

### 6. Operators

```c
/* Tight for high precedence */
result = a + b*c - d/e;

/* Space around low precedence */
value = base + offset;

/* No spaces for member access */
obj->member.field;
```

### 7. Control Flow

```c
si (condition)
{
    /* body */
}

per (i = ZEPHYRUM; i < X; i++)
{
    /* body */
}

commutatio (value)
{
casus I:
    /* at switch level */
    frange;
ordinarius:
    frange;
}
```

- Space before control paren: `si (x)` not `si(x)`
- Case labels at switch level, body indented
- Empty blocks: `{ }` on one line

### 8. Casts and Return

```c
ptr = ( Alveus* ) memoriae_allocare(mensura);
redde ( valor );
mensura = magnitudo ( Structura );
```

- Spaces inside cast parens
- Space after cast
- Return always has parens with spaces
- sizeof always has parens with spaces

### 9. Comments

```c
/* Only C89 block comments */

/* Trailing comments align */
value = I;       /* first */
other = II;      /* second */

/* =========================================================
 * Section Headers - Normalized Width
 * ========================================================= */
```

### 10. Preprocessor

```c
#ifndef HEADER_H
#define HEADER_H

#define MACRO(x, y) \
    ((x) +          \
     (y))

#endif
```

- Always at column 0
- Backslashes aligned in multi-line macros

### 11. Latin Localization

**Keywords**: All C keywords to latina.h equivalents
```
if -> si            else -> alioquin     while -> dum
for -> per          return -> redde      switch -> commutatio
case -> casus       break -> frange      continue -> perge
default -> ordinarius   goto -> salta    do -> fac
typedef -> nomen    struct -> structura  union -> unio
enum -> enumeratio  void -> vacuum       const -> constans
volatile -> volatilis   static -> staticus   extern -> externus
signed -> signatus  unsigned -> insignatus  sizeof -> magnitudo
```

**Numbers**: Integers <=4096 to Roman numerals
- `0` -> `ZEPHYRUM`
- `1` -> `I`, `2` -> `II`, ... `4096` -> `MMMMXCVI`
- Negative: `-5` -> `-V`
- Add missing defines to latina.h (sorted ascending)
- `>4096` remains decimal
- Hex/octal unchanged

### 12. General

- Maximum line length: 80 columns
- Indentation: 2 spaces per level
- Initializer lists: always multi-line
- Trailing commas: yes for initializer lists (C89-legal), no for enums
- Multiple pointer declarations: one per line
- Array brackets stay with name: `arr[10]`
- Preserve struct member order
- Preserve blank lines between functions

---

## Implementation Details

### ChordaAedificator API

```c
/* Creation */
ChordaAedificator* chorda_aedificator_creare (
       Piscina* piscina,
    memoriae_index capacitas);
chorda chorda_aedificator_finire (
    ChordaAedificator* aedificator);

/* Appending */
chorda_aedificator_appendere_character(aed, c);
chorda_aedificator_appendere_literis(aed, "text");
chorda_aedificator_appendere_chorda(aed, s);
chorda_aedificator_appendere_s32(aed, num);
chorda_aedificator_appendere_lineam_novam(aed);
chorda_aedificator_appendere_repetita(aed, ' ', count);

/* Indentation (2 spaces per level) */
chorda_aedificator_push_indentationem(aed);
chorda_aedificator_pop_indentationem(aed);
chorda_aedificator_appendere_indentationem(aed, gradus);
```

### Trivia Access Pattern

```c
/* ArborTrivia structure (from arbor_lexema.h) */
structura {
    chorda  valor;           /* Text content (interned) */
    b32     est_commentum;   /* true if comment */
    b32     est_c99;         /* true if // style */
    i32     linea;
    i32     columna;
} ArborTrivia;

/* Access from node */
Xar* trivia_ante = nodus->trivia_ante;
i32 num = xar_numerus(trivia_ante);
per (i = ZEPHYRUM; i < num; i++)
{
    ArborTrivia** tp = xar_obtinere(trivia_ante, i);
    ArborTrivia* t = *tp;
    chorda_aedificator_appendere_chorda(aed, t->valor);
}
```

### Recursive Emission Pattern

```c
interior vacuum
_emittere_nodum (
         ArborNodus* nodus,
    ChordaAedificator* aed,
      FormatorStatus* status)
{
    si (nodus == NIHIL) redde;

    /* Emit trivia_ante for fidelis mode */
    si (status->fidelis)
    {
        _emittere_trivia(nodus->trivia_ante, aed);
    }

    commutatio (nodus->genus)
    {
    casus ARBOR_NODUS_BINARY_EXPRESSION:
        _emittere_nodum(nodus->datum.binarium.sinister, aed, status);
        _emittere_operator(nodus->datum.binarium.operator, aed, status);
        _emittere_nodum(nodus->datum.binarium.dexter, aed, status);
        frange;

    casus ARBOR_NODUS_COMPOUND_STATEMENT:
        chorda_aedificator_appendere_literis(aed, "{\n");
        chorda_aedificator_push_indentationem(aed);
        si (nodus->datum.compositum.sententiae)
        {
            i32 num = xar_numerus(nodus->datum.compositum.sententiae);
            per (i32 i = ZEPHYRUM; i < num; i++)
            {
                ArborNodus** np = xar_obtinere(
                    nodus->datum.compositum.sententiae, i);
                _emittere_nodum(*np, aed, status);
            }
        }
        chorda_aedificator_pop_indentationem(aed);
        chorda_aedificator_appendere_literis(aed, "}\n");
        frange;

    /* ... other cases ... */
    }

    /* Emit trivia_post for fidelis mode */
    si (status->fidelis)
    {
        _emittere_trivia(nodus->trivia_post, aed);
    }
}
```

### Keyword Mapping Table

```c
interior constans structura {
    constans character* latinum;
    constans character* anglicum;
} keyword_tabula[] = {
    { "si",         "if" },
    { "alioquin",   "else" },
    { "commutatio", "switch" },
    { "casus",      "case" },
    { "ordinarius", "default" },
    { "per",        "for" },
    { "dum",        "while" },
    { "fac",        "do" },
    { "frange",     "break" },
    { "perge",      "continue" },
    { "salta",      "goto" },
    { "redde",      "return" },
    { "structura",  "struct" },
    { "unio",       "union" },
    { "enumeratio", "enum" },
    { "nomen",      "typedef" },
    { "vacuum",     "void" },
    { "constans",   "const" },
    { "volatilis",  "volatile" },
    { "staticus",   "static" },
    { "externus",   "extern" },
    { "signatus",   "signed" },
    { "insignatus", "unsigned" },
    { "magnitudo",  "sizeof" },
    { NIHIL, NIHIL }
};
```

### Roman Numeral Conversion

```c
/* Existing in latina.h: ZEPHYRUM (0), I-MMMMXCVI (1-4096) */

interior chorda
_numerus_ad_romanum (
    Piscina* piscina,
        i64  valor)
{
    si (valor == ZEPHYRUM)
    {
        redde chorda_ex_literis(piscina, "ZEPHYRUM");
    }
    si (valor < ZEPHYRUM)
    {
        /* Handle negative: "-" + roman(abs(valor)) */
    }
    si (valor > MMMMXCVI)
    {
        /* Return decimal string */
    }

    /* Standard Roman numeral algorithm */
    constans structura {
        i32                valor;
        constans character* sym;
    } tabula[] = {
        { M,   "M" },
        { CM,  "CM" },
        { D,   "D" },
        { CD,  "CD" },
        { C,   "C" },
        { XC,  "XC" },
        { L,   "L" },
        { XL,  "XL" },
        { X,   "X" },
        { IX,  "IX" },
        { V,   "V" },
        { IV,  "IV" },
        { I,   "I" },
        { ZEPHYRUM, NIHIL }
    };
    /* ... build string ... */
}
```

### Alignment State Tracking

```c
nomen structura {
    i32  colonna_typi;       /* Right edge of type column */
    i32  colonna_nominis;    /* Left edge of name column */
    i32  colonna_operatoris; /* Column for = alignment */
    i32  linea_initium;      /* First line of current group */
    b32  in_declarationibus; /* Currently in declaration group */
    b32  in_assignationibus; /* Currently in assignment group */
} AlignmentStatus;

/* Reset on blank line or different statement type */
interior vacuum
_alignment_reset (
    AlignmentStatus* status)
{
    status->colonna_typi       = ZEPHYRUM;
    status->colonna_nominis    = ZEPHYRUM;
    status->colonna_operatoris = ZEPHYRUM;
    status->in_declarationibus = FALSUM;
    status->in_assignationibus = FALSUM;
}
```

### Two-Pass Algorithm for Alignment

For proper right-alignment, formatter needs two passes:

**Pass 1**: Calculate alignment columns
- Walk declarations/assignments in group
- Find max type length, max LHS length
- Set alignment columns

**Pass 2**: Emit with alignment
- Emit each line using calculated columns
- Right-pad types, left-pad names

```c
/* Pass 1: Measure */
per (i = ZEPHYRUM; i < num_decls; i++)
{
    i32 type_len = _mensura_typi(decls[i]);
    si (type_len > max_type_len)
    {
        max_type_len = type_len;
    }
}

/* Pass 2: Emit */
per (i = ZEPHYRUM; i < num_decls; i++)
{
    i32 type_len = _mensura_typi(decls[i]);
    i32 padding  = max_type_len - type_len;
    chorda_aedificator_appendere_repetita(aed, ' ', padding);
    _emittere_typum(decls[i], aed);
    /* 2-char gutter, pointer * in first position */
    /* ... */
}
```

---

## Implementation Order

1. **Fidelis mode first** - simpler, just emit trivia
2. **Basic emission** - all node types without formatting
3. **Keyword latinization** - lookup table
4. **Roman numeral conversion** - conversion function
5. **Alignment** - two-pass for declarations/assignments
6. **Function formatting** - signature layout
7. **Operator spacing** - precedence-based
8. **latina.h updates** - file modification for missing numerals

---

## Test Strategy

1. **Roundtrip test**: parse -> format fidelis -> compare bytes
2. **Idempotent test**: format -> format -> compare
3. **Style tests**: verify specific formatting rules
4. **Latin tests**: keyword/numeral conversion
5. **Alignment tests**: multi-line declaration groups

---

## Canonical Example

`lib/piscina.c` demonstrates all formatting patterns.

## Critical Files Reference

- `include/arbor_syntaxis.h` - ArborNodus structure, trivia fields
- `include/arbor_lexema.h` - ArborTrivia structure (lines 165-171)
- `include/chorda_aedificator.h` - String builder API
- `include/latina.h` - Keyword macros and Roman numerals
- `lib/stml.c` - Reference emission pattern (stml_scribere_ad_aedificator)
- `lib/arbor_syntaxis.c:3362-3530` - AST printing pattern (arbor_nodus_imprimere)
