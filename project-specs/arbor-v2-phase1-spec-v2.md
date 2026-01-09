# Arbor v2 Phase 1 - Implementation Specification (v2)

Date: 2026-01-09
Status: Implementation-Ready
Based on: arbor-v2-phase1-spec.md, codebase exploration

---

## Overview

This v2 specification provides codebase-specific implementation details for Phase 1 of arbor-v2, based on exploration of the existing arbor infrastructure.

---

## Codebase Context

### Existing Infrastructure (No Changes Needed)

| Component | Location | Use in arbor2 |
|-----------|----------|---------------|
| Piscina | piscina.h/c | Arena allocation |
| Chorda | chorda.h/c | String handling (NOT null-terminated) |
| Xar | xar.h/c | Variable-length arrays |
| Internamentum | internamentum.h/c | String interning |
| TabulaDispersa | tabula_dispersa.h/c | Hash tables for macros/typedefs |
| Credo | credo.h/c | Test framework |

### Current Arbor (Reference Only)

| File | Lines | Notes |
|------|-------|-------|
| arbor_lexema.c | 1337 | Reference for fork |
| arbor_praeparator.c | 3519 | Reference for macro handling |

### Files to Create

```
include/
├── arbor2_lexema.h         # Forked lexer with NOVA_LINEA
├── arbor2_token.h          # Token with provenance
├── arbor2_expandere.h      # Expansion engine
├── arbor2_segmentum.h      # Segment tracking

lib/
├── arbor2_lexema.c         # Forked lexer (~1500 lines)
├── arbor2_token.c          # Token utilities (~200 lines)
├── arbor2_expandere.c      # Expansion (~1500 lines)
├── arbor2_segmentum.c      # Segment management (~300 lines)

probationes/
├── probatio_arbor2_lexema.c
├── probatio_arbor2_expandere.c
```

---

## Part 1: Forked Lexer (arbor2_lexema)

### Fork Strategy

Copy `lib/arbor_lexema.c` to `lib/arbor2_lexema.c` and modify.

### Key Modifications

#### 1.1 New Token Type

**Location:** New `include/arbor2_lexema.h`, around line 42 of original enum

```c
nomen enumeratio {
    /* ... existing 163 types ... */
    ARBOR2_LEXEMA_NOVA_LINEA,    /* Explicit newline token */
    ARBOR2_LEXEMA_EOF,
    ARBOR2_LEXEMA_ERROR
} Arbor2LexemaGenus;
```

Add to NOMINA_GENERUM array (around line 150 in arbor_lexema.c):
```c
"NOVA_LINEA",
```

#### 1.2 Extended Trivia Structure

**Original (arbor_lexema.h:168-174):**
```c
nomen structura {
    chorda      valor;
    b32         est_commentum;
    b32         est_c99;
    i32         linea;
    i32         columna;
} ArborTrivia;
```

**New (arbor2_lexema.h):**
```c
nomen structura {
    chorda      valor;
    b32         est_commentum;
    b32         est_c99;
    b32         est_continuatio;    /* NEW: true if \\\n continuation */
    i32         linea;
    i32         columna;
} Arbor2Trivia;
```

#### 1.3 Modify _colligere_trivia()

**Location:** Lines 378-487 in arbor_lexema.c

**Current behavior:** Collects whitespace INCLUDING newlines into trivia.

**New behavior:** Exit loop at newline boundary.

```c
/* In whitespace collection loop (~line 393) */
dum (!_finis(lex))
{
    character c = _prospicere(lex, ZEPHYRUM);

    /* NEW: Exit at newline - don't consume it */
    si (c == '\n')
        frange;

    si (_est_spatium_horizontale(c))
    {
        /* Collect horizontal whitespace only */
        /* ... existing code ... */
    }
    alioquin
    {
        frange;
    }
}
```

Add helper:
```c
interior b32
_est_spatium_horizontale(character c)
{
    redde c == ' ' || c == '\t';
}
```

#### 1.4 Add NOVA_LINEA Emission

**Location:** In arbor2_lexema_proximum() around line 940

```c
/* After _colligere_trivia(), before EOF check */
si (c == '\n')
{
    i32 initium = lex->positus;
    i32 linea_initium = lex->linea;
    i32 columna_initium = lex->columna;

    _progredi(lex, I);  /* Consume \n, updates linea/columna */

    lexema = _creare_lexema(lex, ARBOR2_LEXEMA_NOVA_LINEA,
                            initium, I, linea_initium, columna_initium);
    _assignare_trivia_ante(lex, lexema);
    /* No trailing trivia for newlines */
    redde lexema;
}

/* Handle \r\n as single newline */
si (c == '\r')
{
    i32 initium = lex->positus;
    i32 linea_initium = lex->linea;
    i32 columna_initium = lex->columna;
    character c2 = _prospicere(lex, I);
    i32 len = (c2 == '\n') ? II : I;

    _progredi(lex, len);

    lexema = _creare_lexema(lex, ARBOR2_LEXEMA_NOVA_LINEA,
                            initium, len, linea_initium, columna_initium);
    _assignare_trivia_ante(lex, lexema);
    redde lexema;
}
```

#### 1.5 Line Continuation Handling

**Detection function:**
```c
interior b32
_est_continuatio(Arbor2Lexator* lex)
{
    si (_finis(lex)) redde FALSUM;

    character c = _prospicere(lex, ZEPHYRUM);
    si (c != '\\') redde FALSUM;

    character c2 = _prospicere(lex, I);
    redde (c2 == '\n' || c2 == '\r');
}
```

**Integration in trivia collection:**
```c
/* In _colligere_trivia(), when at potential continuation */
dum (_est_continuatio(lex))
{
    /* Consume backslash */
    chorda_constructor_addere_char(&trivia_buf, '\\');
    _progredi(lex, I);

    /* Consume newline */
    si (_prospicere(lex, ZEPHYRUM) == '\r')
    {
        chorda_constructor_addere_char(&trivia_buf, '\r');
        _progredi(lex, I);
    }
    si (_prospicere(lex, ZEPHYRUM) == '\n')
    {
        chorda_constructor_addere_char(&trivia_buf, '\n');
        _progredi(lex, I);
    }

    /* Mark trivia as continuation */
    trivia->est_continuatio = VERUM;
}
```

---

## Part 2: Token with Provenance (arbor2_token)

### Structures

**File: include/arbor2_token.h**

```c
#ifndef ARBOR2_TOKEN_H
#define ARBOR2_TOKEN_H

#include "arbor2_lexema.h"
#include "piscina.h"

/* ================================================================
 * Provenance Types
 * ================================================================ */

nomen enumeratio {
    ARBOR2_ORIGO_FONS,          /* Original source token */
    ARBOR2_ORIGO_EXPANSIO,      /* From macro expansion */
    ARBOR2_ORIGO_PASTA,         /* From ## token pasting */
    ARBOR2_ORIGO_CHORDA,        /* From # stringification */
    ARBOR2_ORIGO_API            /* From API-injected macro */
} Arbor2OrigoGenus;

nomen structura {
    Arbor2OrigoGenus    genus;
    chorda*             nomen_macro;    /* NULL if FONS or API w/o name */
    chorda*             via_file;       /* NULL if API w/o file */
    i32                 linea;
    i32                 columna;
    i32                 layer_index;
} Arbor2OrigoMeta;

/* Extended provenance for token paste */
nomen structura {
    Arbor2OrigoMeta             base;
    structura Arbor2Token*      sinister;   /* Left source token */
    structura Arbor2Token*      dexter;     /* Right source token */
} Arbor2OrigoPasta;

/* ================================================================
 * Token Structure
 * ================================================================ */

nomen structura Arbor2Token {
    Arbor2Lexema*               lexema;
    structura Arbor2Token*      origo_token;    /* Previous in chain, NULL for source */
    Arbor2OrigoMeta*            origo_meta;     /* Always allocated */
} Arbor2Token;

/* ================================================================
 * API Functions
 * ================================================================ */

/* Create source token (layer 0) */
Arbor2Token* arbor2_token_ex_lexema(
    Arbor2Lexema*           lexema,
    chorda*                 via_file,
    Piscina*                piscina);

/* Create expanded token */
Arbor2Token* arbor2_token_ex_expansione(
    Arbor2Lexema*           lexema,
    Arbor2Token*            origo_token,
    chorda*                 nomen_macro,
    i32                     layer_index,
    Piscina*                piscina);

/* Create pasted token */
Arbor2Token* arbor2_token_ex_pasta(
    Arbor2Lexema*           lexema,
    Arbor2Token*            sinister,
    Arbor2Token*            dexter,
    chorda*                 nomen_macro,
    i32                     layer_index,
    Piscina*                piscina);

/* Walk provenance chain */
Arbor2Token* arbor2_token_origo(Arbor2Token* tok);
Arbor2Token* arbor2_token_radix(Arbor2Token* tok);
i32 arbor2_token_profunditas(Arbor2Token* tok);
b32 arbor2_token_ex_macro(Arbor2Token* tok, chorda* nomen_macro);

#endif /* ARBOR2_TOKEN_H */
```

### Implementation

**File: lib/arbor2_token.c**

```c
#include "arbor2_token.h"

Arbor2Token*
arbor2_token_ex_lexema(
    Arbor2Lexema*   lexema,
    chorda*         via_file,
    Piscina*        piscina)
{
    Arbor2Token* tok = piscina_allocare(piscina, sizeof(Arbor2Token));
    si (tok == NIHIL) redde NIHIL;

    tok->lexema = lexema;
    tok->origo_token = NIHIL;

    /* Always allocate origo_meta */
    tok->origo_meta = piscina_allocare(piscina, sizeof(Arbor2OrigoMeta));
    si (tok->origo_meta == NIHIL) redde NIHIL;

    tok->origo_meta->genus = ARBOR2_ORIGO_FONS;
    tok->origo_meta->nomen_macro = NIHIL;
    tok->origo_meta->via_file = via_file;
    tok->origo_meta->linea = lexema->linea;
    tok->origo_meta->columna = lexema->columna;
    tok->origo_meta->layer_index = ZEPHYRUM;

    redde tok;
}

Arbor2Token*
arbor2_token_ex_pasta(
    Arbor2Lexema*   lexema,
    Arbor2Token*    sinister,
    Arbor2Token*    dexter,
    chorda*         nomen_macro,
    i32             layer_index,
    Piscina*        piscina)
{
    Arbor2Token* tok = piscina_allocare(piscina, sizeof(Arbor2Token));
    si (tok == NIHIL) redde NIHIL;

    tok->lexema = lexema;
    tok->origo_token = sinister;  /* Primary chain follows left */

    /* Allocate extended provenance */
    Arbor2OrigoPasta* pasta = piscina_allocare(piscina, sizeof(Arbor2OrigoPasta));
    si (pasta == NIHIL) redde NIHIL;

    pasta->base.genus = ARBOR2_ORIGO_PASTA;
    pasta->base.nomen_macro = nomen_macro;
    pasta->base.via_file = sinister->origo_meta->via_file;
    pasta->base.linea = sinister->lexema->linea;
    pasta->base.columna = sinister->lexema->columna;
    pasta->base.layer_index = layer_index;
    pasta->sinister = sinister;
    pasta->dexter = dexter;

    tok->origo_meta = (Arbor2OrigoMeta*)pasta;

    redde tok;
}

Arbor2Token*
arbor2_token_radix(Arbor2Token* tok)
{
    dum (tok->origo_token != NIHIL)
        tok = tok->origo_token;
    redde tok;
}

i32
arbor2_token_profunditas(Arbor2Token* tok)
{
    i32 depth = ZEPHYRUM;
    dum (tok->origo_token != NIHIL)
    {
        depth++;
        tok = tok->origo_token;
    }
    redde depth;
}
```

---

## Part 3: Segment-Based Expansion

### Structures

**File: include/arbor2_segmentum.h**

```c
#ifndef ARBOR2_SEGMENTUM_H
#define ARBOR2_SEGMENTUM_H

#include "tabula_dispersa.h"
#include "xar.h"

nomen structura {
    i32                 linea_initium;  /* First line (inclusive) */
    i32                 linea_finis;    /* Last line (exclusive) */
    TabulaDispersa*     macros;         /* Macro table snapshot */
} Arbor2Segmentum;

/* Query macro state at line */
TabulaDispersa* arbor2_macros_ad_lineam(Xar* segmenta, i32 linea);

/* Get segment containing line */
Arbor2Segmentum* arbor2_segmentum_ad_lineam(Xar* segmenta, i32 linea);

#endif
```

### Expansion Engine

**File: include/arbor2_expandere.h**

```c
#ifndef ARBOR2_EXPANDERE_H
#define ARBOR2_EXPANDERE_H

#include "arbor2_token.h"
#include "arbor2_segmentum.h"

/* ================================================================
 * Macro Definition
 * ================================================================ */

nomen structura {
    chorda*         titulus;            /* Name (interned) */
    b32             est_functio;
    b32             est_variadic;
    Xar*            parametra;          /* Xar of chorda* */
    Xar*            corpus;             /* Xar of Arbor2Lexema* */
    chorda*         via_file;           /* NULL if API-injected */
    i32             linea_def;          /* -1 if unknown */
    b32             ex_api;
} Arbor2MacroDef;

/* ================================================================
 * Layer
 * ================================================================ */

nomen structura {
    Xar*            lexemata;           /* Xar of Arbor2Token* */
    i32             layer_index;
    b32             est_fixpoint;
} Arbor2Layer;

/* ================================================================
 * Expansion Context
 * ================================================================ */

nomen structura {
    Xar*                    layers;         /* Xar of Arbor2Layer* */
    Xar*                    segmenta;       /* Xar of Arbor2Segmentum* */
    TabulaDispersa*         macros;         /* Current macro table */
    TabulaDispersa*         typedefs;       /* Typedef names */
    TabulaDispersa*         included_viae;  /* Processed includes */
    Xar*                    macro_stack;    /* Recursion prevention */
    Piscina*                piscina;
    InternamentumChorda*    intern;
    i32                     linea_segment_start;
} Arbor2Expansion;

/* ================================================================
 * API
 * ================================================================ */

Arbor2Expansion* arbor2_expansion_creare(
    Piscina*                piscina,
    InternamentumChorda*    intern);

vacuum arbor2_expansion_addere_macro(
    Arbor2Expansion*        exp,
    chorda*                 nomen,
    chorda*                 valor,
    chorda*                 via_file);

vacuum arbor2_expansion_addere_typedef(
    Arbor2Expansion*        exp,
    chorda*                 nomen);

vacuum arbor2_expansion_processare(
    Arbor2Expansion*        exp,
    Xar*                    source_lexemata,    /* Xar of Arbor2Lexema* */
    chorda*                 via_file);

Arbor2Layer* arbor2_expansion_layer(Arbor2Expansion* exp, i32 index);
Arbor2Layer* arbor2_expansion_layer_finalis(Arbor2Expansion* exp);
i32 arbor2_expansion_numerus_layers(Arbor2Expansion* exp);

#endif
```

### Expansion Implementation Pattern

**File: lib/arbor2_expandere.c (core loop)**

```c
interior Arbor2Layer*
_expand_layer(
    Arbor2Expansion*    exp,
    Arbor2Layer*        layer_prev)
{
    Arbor2Layer* layer_new = piscina_allocare(exp->piscina, sizeof(Arbor2Layer));
    layer_new->lexemata = xar_creare(exp->piscina, sizeof(Arbor2Token*));
    layer_new->layer_index = layer_prev->layer_index + I;

    b32 any_expanded = FALSUM;
    b32 at_line_start = VERUM;
    i32 i = ZEPHYRUM;
    i32 num = xar_numerus(layer_prev->lexemata);

    dum (i < num)
    {
        Arbor2Token* tok = *(Arbor2Token**)xar_obtinere(layer_prev->lexemata, i);

        /* Track line boundaries */
        si (tok->lexema->genus == ARBOR2_LEXEMA_NOVA_LINEA)
        {
            Arbor2Token** slot = xar_addere(layer_new->lexemata);
            *slot = tok;
            at_line_start = VERUM;
            i++;
            perge;
        }

        /* Detect and process directives */
        si (at_line_start && tok->lexema->genus == ARBOR2_LEXEMA_HASH)
        {
            i32 dir_end;
            b32 consumed = _processare_directiva(exp, layer_prev, i, &dir_end);
            si (consumed)
            {
                /* Directive consumed - don't add to output */
                /* Start new segment if #define/#undef */
                i = dir_end;
                at_line_start = VERUM;
                perge;
            }
        }

        /* Check for macro expansion */
        si (tok->lexema->genus == ARBOR2_LEXEMA_IDENTIFICATOR)
        {
            Arbor2MacroDef* macro = _lookup_macro(exp, tok->lexema->valor);
            si (macro != NIHIL && !_est_in_expansione(exp, macro->titulus))
            {
                i32 consumed;
                _expandere_macro(exp, layer_prev, i, layer_new->lexemata,
                                 layer_new->layer_index, &consumed);
                i += consumed;
                any_expanded = VERUM;
                at_line_start = FALSUM;
                perge;
            }
        }

        /* Pass through unchanged */
        Arbor2Token** slot = xar_addere(layer_new->lexemata);
        *slot = tok;
        at_line_start = FALSUM;
        i++;
    }

    layer_new->est_fixpoint = !any_expanded;
    redde layer_new;
}
```

### Segment Creation

```c
interior vacuum
_finire_segmentum(Arbor2Expansion* exp, i32 linea_finis)
{
    Arbor2Segmentum* seg = piscina_allocare(exp->piscina, sizeof(Arbor2Segmentum));
    seg->linea_initium = exp->linea_segment_start;
    seg->linea_finis = linea_finis;

    /* Snapshot current macro table */
    seg->macros = _snapshot_macros(exp);

    Arbor2Segmentum** slot = xar_addere(exp->segmenta);
    *slot = seg;

    exp->linea_segment_start = linea_finis;
}

interior vacuum
_processare_define(Arbor2Expansion* exp, Arbor2Layer* layer, i32 pos, i32 linea)
{
    /* If not at segment start, finish previous segment */
    si (exp->linea_segment_start != linea)
    {
        _finire_segmentum(exp, linea);
    }

    /* Parse and add macro definition */
    /* ... */
}
```

---

## Part 4: Tests

### Test File Pattern

**File: probationes/probatio_arbor2_lexema.c**

```c
#include "../include/arbor2_lexema.h"
#include "../include/credo.h"
#include "../include/piscina.h"
#include "../include/internamentum.h"

hic_manens vacuum
test_nova_linea_emission(Credo* credo)
{
    Piscina* piscina = piscina_creare(MILLE * MILLE);
    InternamentumChorda* intern = internamentum_creare(piscina);

    constans character* fons = "int x;\nint y;";
    i32 mensura = (i32)strlen(fons);

    Arbor2Lexator* lex = arbor2_lexator_creare(piscina, intern, fons, mensura);

    /* int */
    Arbor2Lexema* tok = arbor2_lexema_proximum(lex);
    CREDO_AEQUALIS_I32(credo, tok->genus, ARBOR2_LEXEMA_INT);

    /* x */
    tok = arbor2_lexema_proximum(lex);
    CREDO_AEQUALIS_I32(credo, tok->genus, ARBOR2_LEXEMA_IDENTIFICATOR);

    /* ; */
    tok = arbor2_lexema_proximum(lex);
    CREDO_AEQUALIS_I32(credo, tok->genus, ARBOR2_LEXEMA_SEMICOLON);

    /* NOVA_LINEA */
    tok = arbor2_lexema_proximum(lex);
    CREDO_AEQUALIS_I32(credo, tok->genus, ARBOR2_LEXEMA_NOVA_LINEA);
    CREDO_AEQUALIS_I32(credo, tok->linea, I);

    /* int */
    tok = arbor2_lexema_proximum(lex);
    CREDO_AEQUALIS_I32(credo, tok->genus, ARBOR2_LEXEMA_INT);
    CREDO_AEQUALIS_I32(credo, tok->linea, II);

    piscina_destruere(piscina);
}

integer principale(vacuum)
{
    Credo* credo = credo_aperire();

    test_nova_linea_emission(credo);
    /* ... more tests ... */

    credo_imprimere_compendium(credo);
    redde credo_omnia_praeterierunt(credo) ? ZEPHYRUM : I;
}
```

---

## Implementation Order

### Week 1: Lexer Fork

1. Copy arbor_lexema.h/c to arbor2_lexema.h/c
2. Add ARBOR2_LEXEMA_NOVA_LINEA
3. Modify _colligere_trivia() to exit at newline
4. Add newline token emission
5. Add line continuation handling
6. Write probatio_arbor2_lexema.c tests

### Week 2: Token Infrastructure

1. Create arbor2_token.h/c
2. Implement Arbor2Token creation functions
3. Implement provenance chain walking
4. Test with manual token creation

### Week 3: Expansion Engine

1. Create arbor2_expandere.h/c
2. Implement Layer 0 creation from lexemes
3. Implement #define parsing
4. Implement object-like macro expansion
5. Implement segment tracking

### Week 4: Function Macros & Operators

1. Implement function-like macro argument collection
2. Implement # stringify (standard behavior)
3. Implement ## token paste with best-effort lex
4. Implement #undef and segment boundaries
5. Integration tests

---

## Verification

### Test Coverage Required

1. **Lexer:**
   - NOVA_LINEA emitted for each newline
   - Line continuation with metadata preserved
   - Trivia correctly attached around NOVA_LINEA

2. **Token:**
   - Source tokens have ARBOR2_ORIGO_FONS
   - Expanded tokens chain correctly
   - Pasted tokens have both sources accessible

3. **Expansion:**
   - Layer 0 matches source structure
   - Macros expand correctly across layers
   - Segments created at #define/#undef
   - Query returns correct macro state per line

4. **Operators:**
   - # collapses whitespace, escapes quotes
   - ## concatenates and re-lexes
   - Invalid paste produces error or best-effort token
