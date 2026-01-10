

#arbor-v2-phase1-spec-v2.md
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


#arbor-v2-simulation-spec-v2.md
# Arbor v2 - Implementation Specification (v2)

Date: 2026-01-09
Status: Implementation-Ready Draft
Based on: arbor-v2-simulation-spec.md + codebase exploration

---

## Overview

This document provides codebase-specific implementation details for arbor-v2, incorporating findings from the implementation simulation, interview, and codebase exploration.

---

## Codebase Integration

### Files to Create

```
include/
├── arbor2.h                    # Public API (builder pattern)
├── arbor2_lexema.h             # Extended lexer with NOVA_LINEA
├── arbor2_expandere.h          # Layered expansion types
├── arbor2_token.h              # Arbor2Token with linked provenance
├── arbor2_glr.h                # GLR parser types
├── arbor2_syntaxis.h           # C89 grammar productions
├── arbor2_ambig.h              # Unknown macro/typedef nodes
├── arbor2_conditio.h           # Conditional compilation

lib/
├── arbor2.c                    # Main API implementation
├── arbor2_lexema.c             # Lexer modifications
├── arbor2_expandere.c          # Layered macro expansion
├── arbor2_token.c              # Token/provenance utilities
├── arbor2_glr.c                # GLR parser core
├── arbor2_syntaxis.c           # Grammar productions
├── arbor2_latina.c             # Compiled-in latina.h definitions

probationes/
├── probatio_arbor2.c           # Integration tests
├── probatio_arbor2_lexema.c    # Lexer tests
├── probatio_arbor2_expandere.c # Expansion tests
├── probatio_arbor2_glr.c       # GLR tests
```

### Existing Infrastructure to Reuse

| Component | Location | Use in arbor2 |
|-----------|----------|---------------|
| Piscina | piscina.h/c | Arena allocation, mark/reset for GLR |
| Chorda | chorda.h/c | String handling |
| Xar | xar.h/c | Dynamic arrays |
| Internamentum | internamentum.h/c | String interning |
| TabulaDispersa | tabula_dispersa.h/c | Macro/typedef lookup |
| ArborLexema | arbor_lexema.h/c | Base lexer (with modifications) |

---

## Phase 1: Lexer Enhancement

### Add ARBOR_LEXEMA_NOVA_LINEA

**File: include/arbor_lexema.h (line ~157)**

```c
/* Add before ARBOR_LEXEMA_EOF */
ARBOR_LEXEMA_NOVA_LINEA,        /* Explicit newline token */
ARBOR_LEXEMA_EOF,
ARBOR_LEXEMA_ERROR
```

**File: lib/arbor_lexema.c (line ~190)**

Add to `NOMINA_GENERUM[]` array:
```c
"NOVA_LINEA",
```

**File: lib/arbor_lexema.c (line ~217)**

Modify `_est_spatium()` to exclude newlines:
```c
interior b32
_est_spatium_horizontale(character c)
{
    redde c == ' ' || c == '\t';
}

interior b32
_est_nova_linea(character c)
{
    redde c == '\n' || c == '\r';
}
```

**File: lib/arbor_lexema.c (lines ~378-487)**

In `_colligere_trivia()`, emit NOVA_LINEA tokens when newlines encountered:
- Collect horizontal whitespace as trivia
- When `\n` hit, yield it as separate token
- Handle `\r\n` as single newline

### Alternative: arbor2-specific lexer

Could create `arbor2_lexema.c` that wraps original lexer and injects NOVA_LINEA tokens. This avoids modifying the stable arbor v1 lexer.

```c
/* Wraps arbor_lexema_proximum, inserts NOVA_LINEA */
ArborLexema* arbor2_lexema_proximum(Arbor2Lexator* lex);
```

**Recommendation:** Create arbor2-specific wrapper to avoid breaking v1.

---

## Phase 2: Token & Provenance

### Arbor2Token Structure

**File: include/arbor2_token.h**

```c
nomen structura Arbor2Token {
    ArborLexema*            lexema;
    structura Arbor2Token*  origo_token;    /* Previous in chain, NULL if source */
    Arbor2OrigoMeta*        origo_meta;     /* Expansion metadata */
};

nomen enumeratio {
    ARBOR2_ORIGO_FONS,          /* Original source token */
    ARBOR2_ORIGO_EXPANSIO,      /* From macro expansion */
    ARBOR2_ORIGO_PASTA,         /* From ## token pasting */
    ARBOR2_ORIGO_CHORDA         /* From # stringification */
} Arbor2OrigoGenus;

nomen structura {
    Arbor2OrigoGenus    genus;
    chorda*             nomen_macro;    /* NULL if not from macro */
    chorda*             via_file;       /* Source file path */
    i32                 linea;
    i32                 columna;
    i32                 layer_index;
} Arbor2OrigoMeta;

/* For pasted tokens - extends Arbor2OrigoMeta */
nomen structura {
    Arbor2OrigoMeta     base;
    Arbor2Token*        sinister;       /* Left source token */
    Arbor2Token*        dexter;         /* Right source token */
} Arbor2OrigoPasta;
```

### Provenance API

```c
/* Walk provenance chain */
Arbor2Token* arbor2_token_origo(Arbor2Token* tok);

/* Get root source token */
Arbor2Token* arbor2_token_radix(Arbor2Token* tok);

/* Get expansion depth */
i32 arbor2_token_profunditas(Arbor2Token* tok);

/* Check if from specific macro */
b32 arbor2_token_ex_macro(Arbor2Token* tok, chorda* nomen_macro);
```

### Comparison to Current Arbor

Current arbor (`ArborOrigo` in arbor_praeparator.h:38-45):
- Uses `pater` pointer for linked list
- Has `expansio_index` for depth tracking
- `via_file` is often NULL for macros (known issue)

Arbor2 improvements:
- `origo_token` directly links to previous token (not just metadata)
- Separate `Arbor2OrigoPasta` for merged provenance
- `via_file` always populated

---

## Phase 3: Layered Expansion

### Layer Structure

**File: include/arbor2_expandere.h**

```c
nomen structura {
    Xar*        lexemata;       /* Xar of Arbor2Token* */
    i32         layer_index;
    b32         est_fixpoint;   /* No more expansions possible */
} Arbor2Layer;

nomen structura {
    Xar*                layers;         /* Xar of Arbor2Layer* */
    TabulaDispersa*     macros;         /* name -> Arbor2MacroDef* */
    TabulaDispersa*     typedefs;       /* name -> presence marker */
    Piscina*            piscina;
    InternamentumChorda* intern;
} Arbor2Expansion;

nomen structura {
    chorda*         titulus;        /* Macro name (interned) */
    b32             est_functio;
    b32             est_variadic;
    Xar*            parametra;      /* Xar of chorda* */
    Xar*            corpus;         /* Xar of ArborLexema* */
    chorda*         via_file;       /* Where defined */
    i32             linea_def;
} Arbor2MacroDef;
```

### Expansion Algorithm

```c
/* Create expansion context */
Arbor2Expansion* arbor2_expansion_creare(
    Piscina*              piscina,
    InternamentumChorda*  intern);

/* Add macro definition */
vacuum arbor2_expansion_addere_macro(
    Arbor2Expansion*      exp,
    Arbor2MacroDef*       macro);

/* Process source tokens into layers */
vacuum arbor2_expansion_processare(
    Arbor2Expansion*      exp,
    Xar*                  source_tokens);  /* Xar of ArborLexema* */

/* Get specific layer */
Arbor2Layer* arbor2_expansion_layer(
    Arbor2Expansion*      exp,
    i32                   index);

/* Get final (fixpoint) layer */
Arbor2Layer* arbor2_expansion_layer_finalis(Arbor2Expansion* exp);
```

### Single-Pass Expansion

```c
interior Arbor2Layer*
_expand_one_pass(
    Arbor2Expansion*      exp,
    Arbor2Layer*          layer_prev,
    Piscina*              piscina)
{
    Arbor2Layer* layer_new;
    Xar* tokens_new;
    b32 any_expanded;
    i32 i, numerus;

    layer_new = piscina_allocare(piscina, sizeof(Arbor2Layer));
    tokens_new = xar_creare(piscina, sizeof(Arbor2Token*));
    any_expanded = FALSUM;

    numerus = xar_numerus(layer_prev->lexemata);
    i = ZEPHYRUM;

    dum (i < numerus)
    {
        Arbor2Token* tok = *(Arbor2Token**)xar_obtinere(layer_prev->lexemata, i);

        si (_est_macro_invocatio(exp, tok, layer_prev, i))
        {
            i32 consumed;
            _expandere_macro(exp, layer_prev, i, tokens_new, &consumed, piscina);
            i += consumed;
            any_expanded = VERUM;
        }
        alioquin
        {
            xar_addere(tokens_new, &tok);
            i++;
        }
    }

    layer_new->lexemata = tokens_new;
    layer_new->layer_index = layer_prev->layer_index + I;
    layer_new->est_fixpoint = !any_expanded;

    redde layer_new;
}
```

### Token Pasting (##)

```c
interior Arbor2Token*
_paste_tokens(
    Arbor2Token*          sinister,
    Arbor2Token*          dexter,
    Arbor2OrigoMeta*      origo_parent,
    Piscina*              piscina,
    InternamentumChorda*  intern)
{
    chorda combined;
    ArborLexema* lex_new;
    Arbor2Token* tok_new;
    Arbor2OrigoPasta* origo_pasta;

    /* Concatenate token text */
    combined = chorda_concatenare(
        sinister->lexema->valor,
        dexter->lexema->valor,
        piscina);

    /* Re-lex to get proper token type */
    lex_new = _relex_token(combined, piscina, intern);

    /* Create token with merged provenance */
    tok_new = piscina_allocare(piscina, sizeof(Arbor2Token));
    tok_new->lexema = lex_new;
    tok_new->origo_token = sinister;  /* Primary chain follows left */

    origo_pasta = piscina_allocare(piscina, sizeof(Arbor2OrigoPasta));
    origo_pasta->base.genus = ARBOR2_ORIGO_PASTA;
    origo_pasta->base.nomen_macro = origo_parent->nomen_macro;
    origo_pasta->base.layer_index = origo_parent->layer_index;
    origo_pasta->sinister = sinister;
    origo_pasta->dexter = dexter;

    tok_new->origo_meta = (Arbor2OrigoMeta*)origo_pasta;

    redde tok_new;
}
```

---

## Phase 4: Unknown Macro/Typedef Handling

### Ambiguous Node Types

**File: include/arbor2_ambig.h**

```c
nomen enumeratio {
    ARBOR2_AMBIG_TYPEDEF_IGNOTUM,
    ARBOR2_AMBIG_MACRO_IGNOTUM,
    ARBOR2_AMBIG_ERROR
} Arbor2AmbigGenus;

nomen structura {
    Arbor2AmbigGenus    genus;
    chorda*             identifier;
    Xar*                tokens;         /* Tokens involved */
    chorda*             classificatio;  /* Best-effort: "type", "expr" */
    f32                 confidentia;    /* 0.0-1.0 */
} Arbor2AmbigNodus;
```

### Detection Heuristics

```c
/* In type position: identifier before another identifier or * */
interior b32
_est_probable_typedef(Arbor2Token* tok, Arbor2Token* next)
{
    si (tok->lexema->genus != ARBOR_LEXEMA_IDENTIFICATOR)
        redde FALSUM;

    si (next == NIHIL)
        redde FALSUM;

    /* identifier identifier -> first is probably type */
    si (next->lexema->genus == ARBOR_LEXEMA_IDENTIFICATOR)
        redde VERUM;

    /* identifier * -> probably type */
    si (next->lexema->genus == ARBOR_LEXEMA_ASTERISK)
        redde VERUM;

    redde FALSUM;
}

/* In macro position: identifier with ( in non-function-call context */
interior b32
_est_probable_macro(Arbor2Token* tok, Arbor2Token* next, Arbor2ParseContext* ctx)
{
    si (tok->lexema->genus != ARBOR_LEXEMA_IDENTIFICATOR)
        redde FALSUM;

    si (next == NIHIL || next->lexema->genus != ARBOR_LEXEMA_PAREN_APERTA)
        redde FALSUM;

    /* If we're expecting a type or statement keyword, probably macro */
    si (ctx->expectans == ARBOR2_EXPECT_TYPE ||
        ctx->expectans == ARBOR2_EXPECT_KEYWORD)
        redde VERUM;

    redde FALSUM;
}
```

---

## Phase 5: Built-in Latina Definitions

**File: lib/arbor2_latina.c**

```c
/* Compiled-in latina.h macro definitions */
hic_manens constans Arbor2MacroDef ARBOR2_LATINA_MACROS[] = {
    /* Keywords */
    {"character", FALSUM, FALSUM, NIHIL, /* corpus: char */},
    {"brevis", FALSUM, FALSUM, NIHIL, /* corpus: short */},
    {"integer", FALSUM, FALSUM, NIHIL, /* corpus: int */},
    {"longus", FALSUM, FALSUM, NIHIL, /* corpus: long */},
    {"fluitans", FALSUM, FALSUM, NIHIL, /* corpus: float */},
    {"duplex", FALSUM, FALSUM, NIHIL, /* corpus: double */},
    {"vacuum", FALSUM, FALSUM, NIHIL, /* corpus: void */},
    {"signatus", FALSUM, FALSUM, NIHIL, /* corpus: signed */},
    {"insignatus", FALSUM, FALSUM, NIHIL, /* corpus: unsigned */},
    {"constans", FALSUM, FALSUM, NIHIL, /* corpus: const */},
    {"volatilis", FALSUM, FALSUM, NIHIL, /* corpus: volatile */},
    {"sponte", FALSUM, FALSUM, NIHIL, /* corpus: auto */},
    {"registrum", FALSUM, FALSUM, NIHIL, /* corpus: register */},
    {"staticus", FALSUM, FALSUM, NIHIL, /* corpus: static */},
    {"externus", FALSUM, FALSUM, NIHIL, /* corpus: extern */},
    {"si", FALSUM, FALSUM, NIHIL, /* corpus: if */},
    {"alioquin", FALSUM, FALSUM, NIHIL, /* corpus: else */},
    {"commutatio", FALSUM, FALSUM, NIHIL, /* corpus: switch */},
    {"casus", FALSUM, FALSUM, NIHIL, /* corpus: case */},
    {"ordinarius", FALSUM, FALSUM, NIHIL, /* corpus: default */},
    {"per", FALSUM, FALSUM, NIHIL, /* corpus: for */},
    {"dum", FALSUM, FALSUM, NIHIL, /* corpus: while */},
    {"fac", FALSUM, FALSUM, NIHIL, /* corpus: do */},
    {"frange", FALSUM, FALSUM, NIHIL, /* corpus: break */},
    {"perge", FALSUM, FALSUM, NIHIL, /* corpus: continue */},
    {"salta", FALSUM, FALSUM, NIHIL, /* corpus: goto */},
    {"redde", FALSUM, FALSUM, NIHIL, /* corpus: return */},
    {"structura", FALSUM, FALSUM, NIHIL, /* corpus: struct */},
    {"unio", FALSUM, FALSUM, NIHIL, /* corpus: union */},
    {"enumeratio", FALSUM, FALSUM, NIHIL, /* corpus: enum */},
    {"nomen", FALSUM, FALSUM, NIHIL, /* corpus: typedef */},
    {"magnitudo", FALSUM, FALSUM, NIHIL, /* corpus: sizeof */},
    {"principale", FALSUM, FALSUM, NIHIL, /* corpus: main */},
    {"interior", FALSUM, FALSUM, NIHIL, /* corpus: static */},
    {"hic_manens", FALSUM, FALSUM, NIHIL, /* corpus: static */},
    {"universalis", FALSUM, FALSUM, NIHIL, /* corpus: static */},
    /* ... more ... */
};

constans i32 ARBOR2_LATINA_MACROS_NUMERUS =
    sizeof(ARBOR2_LATINA_MACROS) / sizeof(ARBOR2_LATINA_MACROS[0]);
```

---

## Phase 6: API

**File: include/arbor2.h**

```c
/* Opaque context handle */
nomen structura Arbor2Contextus Arbor2Contextus;

/* Result struct (optional retrieval) */
nomen structura {
    b32                 successus;
    Arbor2Nodus*        radix;
    Xar*                layers;         /* Xar of Arbor2Layer* */
    Arbor2Index*        index;
    Xar*                errores;
} Arbor2Resultus;

/* === Builder API === */

Arbor2Contextus* arbor2_contextus_creare(
    Piscina*              piscina,
    InternamentumChorda*  intern);

/* Configuration */
vacuum arbor2_addere_viam_include(Arbor2Contextus* ctx, constans character* via);
vacuum arbor2_addere_macro(Arbor2Contextus* ctx, constans character* nomen, constans character* valor);
vacuum arbor2_addere_typedef(Arbor2Contextus* ctx, constans character* nomen);
vacuum arbor2_includere_latina(Arbor2Contextus* ctx, b32 includere);
vacuum arbor2_ponere_recovery(Arbor2Contextus* ctx, Arbor2RecoveryModus modus);

/* Parse */
Arbor2Status arbor2_parsere_filum(Arbor2Contextus* ctx, constans character* via);
Arbor2Status arbor2_parsere_fontem(Arbor2Contextus* ctx, constans character* fons, i32 longitudo);

/* Query */
Arbor2Nodus*    arbor2_obtinere_radix(Arbor2Contextus* ctx);
Arbor2Layer*    arbor2_obtinere_layer(Arbor2Contextus* ctx, i32 index);
i32             arbor2_numerus_layers(Arbor2Contextus* ctx);
Xar*            arbor2_obtinere_errores(Arbor2Contextus* ctx);
Arbor2Resultus  arbor2_obtinere_resultum(Arbor2Contextus* ctx);

/* Stringify */
chorda* arbor2_layer_stringify(Arbor2Layer* layer, Piscina* piscina);
chorda* arbor2_nodus_stringify(Arbor2Nodus* nodus, Piscina* piscina);
```

---

## Test Parity Requirements

Based on codebase exploration, arbor2 must pass tests for:

### Lexer (from probatio_arbor_lexema.c)
- 32 C89 keywords
- Identifiers, literals (int, float, string, char)
- All operators and punctuation
- Trivia (whitespace, comments)
- Line/column tracking
- **Plus:** NOVA_LINEA token

### Preprocessor (from probatio_arbor_praeparator.c)
- Object-like and function-like macros
- #define, #undef, #ifdef, #if, #include
- Stringification (#) and token pasting (##)
- **Changed:** Layered expansion instead of modes

### Parser (from probatio_arbor_syntaxis.c)
- All expressions (unary, binary, ternary, postfix)
- All statements (if, while, for, switch, etc.)
- All declarations (variables, functions, structs, unions, enums, typedefs)
- Extensions (__extension__, __attribute__)
- Trivia preservation

### Roundtrip (from probationes/fixa/roundtrip/)
- 20+ test files
- Byte-exact roundtrip
- Latin macro handling

---

## Implementation Order

1. **Week 1: Token Infrastructure**
   - Create arbor2_token.h/c
   - Implement linked provenance
   - Create arbor2_lexema wrapper with NOVA_LINEA
   - Unit tests

2. **Week 2: Layered Expansion**
   - Create arbor2_expandere.h/c
   - Implement single-pass expansion
   - Implement ##, # operators
   - Unit tests with current arbor test snippets

3. **Week 3: Unknown Handling**
   - Create arbor2_ambig.h/c
   - Implement detection heuristics
   - Integration with expansion

4. **Week 4: GLR Foundation**
   - Create arbor2_glr.h/c
   - Implement GSS data structures
   - Basic fork/merge for expressions

5. **Week 5-6: C89 Grammar**
   - Create arbor2_syntaxis.h/c
   - Port productions from arbor_syntaxis.c
   - Integrate macro lookahead

6. **Week 7: Conditional Compilation**
   - Create arbor2_conditio.h/c
   - Implement node-based forks
   - Wildcard query support

7. **Week 8: API & Polish**
   - Create arbor2.h/c
   - Builder pattern implementation
   - Built-in latina definitions
   - Parity tests against arbor v1

---

## Open Questions for Implementation

1. **Lexer modification vs wrapper**: Modify arbor_lexema.c directly or create arbor2-specific wrapper?
   - Recommendation: Wrapper to avoid breaking v1

2. **GLR table generation**: Hand-write tables or build generator?
   - Recommendation: Start hand-written for expressions, evaluate generator need later

3. **Memory budget**: What's acceptable memory growth for GLR forks?
   - Need benchmarking on large files

4. **Conditional explosion**: How to cap #ifdef fork count?
   - Maybe max 16 independent conditions per file?


#arbor-v2-spec-v2.md
# Arbor v2 - Implementation Specification

## Overview

This document extends the arbor-v2 design spec with codebase-specific implementation details based on exploration of the existing arbor and shared infrastructure.

**Shared Infrastructure to Reuse:**
- `piscina` - Arena allocator (with mark/reset for GLR)
- `chorda` - String type (NOT null-terminated, length-prefixed)
- `xar` - Dynamic arrays (stable indices, segmented growth)
- `internamentum` - String interning (O(1) identifier comparison)
- `tabula_dispersa` - Hash tables (for macro/typedef lookup)

**Files to Create:**
- `lib/arbor2.c` - Main API
- `lib/arbor2_lexator.c` - Lexer (may reuse arbor_lexema with modifications)
- `lib/arbor2_expandere.c` - Layered macro expansion
- `lib/arbor2_glr.c` - GLR parser core
- `lib/arbor2_syntaxis.c` - C89 grammar productions
- `include/arbor2.h` - Public API
- `include/arbor2_*.h` - Internal headers

---

## Phase 1: Layered Macro Expansion

### Current Arbor Approach (for reference)

Current arbor has three modes in `arbor_praeparator.c`:
- **PROCESSARE**: Full expansion, loses original tokens
- **PRESERVARE**: No expansion, keeps directives
- **HYBRID**: Learns macros from includes, preserves tokens

### Arbor-v2 Approach: Expansion Layers

Instead of modes, produce discrete layers:

```c
nomen structura {
    Xar*        lexemata;       /* Xar of ArborLexemaOrigo* */
    i32         layer_index;    /* 0 = source, 1 = first expansion, etc. */
    b32         est_fixpoint;   /* true if no more expansions possible */
} Arbor2Layer;

nomen structura {
    Xar*        layers;         /* Xar of Arbor2Layer* */
    TabulaDispersa* macros;     /* macro name -> Arbor2MacroDef* */
    TabulaDispersa* typedefs;   /* typedef name -> void* (presence) */
} Arbor2Expansion;
```

### Expansion Algorithm

```
function expand_all_layers(source_tokens):
    layers = [source_tokens]  // layer 0

    while true:
        prev_layer = layers[-1]
        new_layer = expand_one_pass(prev_layer)

        if new_layer == prev_layer:  // fixpoint
            break
        layers.append(new_layer)

    return layers
```

**Key difference from current arbor:**
- Current arbor expands recursively in single pass (rescan semantics)
- Arbor-v2 uses strict layers: layer N+1 only expands macros present in layer N

### Reusable from Current Arbor

From `arbor_praeparator.c`:
- `ArborMacroDefinitio` struct (line ~100) - macro definition storage
- `_processare_define()` - macro definition parsing
- `_expandere_macro_corpus()` - token substitution logic
- `_est_jam_inclusum()` / `_signare_inclusum()` - circular include detection

### New Structures Needed

```c
/* Token with full provenance chain */
nomen structura {
    ArborLexema*    lexema;
    Xar*            catena_originis;  /* Xar of Arbor2Origo* - full chain */
} Arbor2Token;

/* Origin record in chain */
nomen structura {
    chorda*         nomen_macro;      /* NULL if from include */
    chorda*         via_file;         /* file path */
    i32             linea;
    i32             columna;
    i32             layer_index;      /* which layer this expansion occurred */
} Arbor2Origo;
```

### Include Processing

Reuse from `arbor_praeparator.c`:
- `_processare_include()` (line 2281) - recursive include handling
- Path resolution via `via_*` utilities
- Depth limit `ARBOR_MAX_INCLUDE_PROFUNDITAS` (64)

**Enhancement:** Track which macros/typedefs came from which include for incremental invalidation.

---

## Phase 2: Typedef Collection

### Current Arbor Approach

`_detectare_typedef()` in `arbor_praeparator.c` (line 617):
- Scans included files for typedef patterns
- Handles brace depth for struct typedefs
- Handles parenthesis depth for function pointer typedefs
- Stores in `typedef_nomina` hash table

### Arbor-v2 Enhancement

Run typedef collection on ALL layers (not just includes):

```c
nomen structura {
    chorda*         nomen;            /* typedef name (interned) */
    i32             layer_discovered; /* which layer it was found */
    Arbor2Origo*    origo;            /* where defined */
    b32             est_certum;       /* true if definitely typedef, false if heuristic */
} Arbor2TypedefInfo;
```

**Collection Strategy:**
1. After macro expansion produces layers 0..N
2. For each layer, scan for typedef declarations
3. Typedef from layer K affects parsing of layers K+1..N
4. If typedef discovered late, may need re-parse of earlier layers (rare)

### Heuristic Typedefs

From `arbor_syntaxis.c`, the `_est_probable_typedef_nomen()` heuristic:
- Identifier followed by `*` in type position → probably typedef
- Identifier followed by identifier in type position → probably typedef

Arbor-v2 makes these explicit with "probably typedef" nodes.

---

## Phase 3: GLR Parser Core

### Graph-Structured Stack (GSS)

```c
/* GSS Node - represents a parser state */
nomen structura Arbor2GSSNode {
    i32                     state;      /* LR state number */
    structura Arbor2GSSNode** preds;    /* predecessor nodes (Xar) */
    i32                     pred_count;
    vacuum*                 value;      /* semantic value (AST node) */
    PiscinaNotatio          checkpoint; /* for backtracking */
} Arbor2GSSNode;

/* GLR Parser State */
nomen structura {
    Xar*            active_nodes;   /* current frontier (Xar of Arbor2GSSNode*) */
    Xar*            pending_reductions; /* deferred reductions */
    Piscina*        piscina;
    i32             position;       /* current token index */
} Arbor2GLR;
```

### Memory Strategy: Piscina Mark/Reset

Use `piscina_notare()` / `piscina_reficere()` for fork handling:

```c
/* Before exploring a fork */
PiscinaNotatio checkpoint = piscina_notare(glr->piscina);

/* Allocate fork-specific structures */
Arbor2GSSNode* fork_node = piscina_allocare(glr->piscina, sizeof(Arbor2GSSNode));

/* If fork fails */
piscina_reficere(glr->piscina, checkpoint);

/* If fork succeeds - keep allocations, checkpoint discarded */
```

**Key insight from infrastructure exploration:**
- Piscina mark/reset is O(1) - just adjusts pointers
- All fork allocations freed in bulk on backtrack
- Successful forks "commit" by not resetting

### LR Table Generation

For C89 grammar, need:
1. Grammar definition (BNF-like)
2. LALR(1) or GLR table generator
3. Conflict resolution via forking (not shift/reduce preference)

**Options:**
- Hand-write tables (error-prone but no external deps)
- Build simple table generator in C89
- Pre-generate tables offline, embed as data

**Recommendation:** Start with hand-written tables for core grammar subset, add generator later.

### Macro Lookahead Integration

```c
/* Query macro table during parse */
interior b32
_macro_lookahead_suggests_type(Arbor2GLR* glr, chorda* identifier)
{
    Arbor2MacroDef* macro;
    ArborLexema* first_token;

    macro = tabula_dispersa_invenire(glr->macros, *identifier);
    si (macro == NIHIL) redde FALSUM;

    /* Check what macro expands to */
    first_token = *(ArborLexema**)xar_obtinere(macro->corpus, 0);

    /* If expands to type keyword, suggest this is a type */
    redde _est_type_keyword(first_token->genus);
}
```

---

## Phase 4: AST Structure

### Dual Tree Model

**Source Tree** - preserves original structure:
```c
nomen structura {
    Arbor2SourceNodeGenus   genus;
    Xar*                    children;       /* mixed: nodes, tokens, comments, directives */
    i32                     layer_index;    /* which expansion layer */
} Arbor2SourceNode;
```

**Semantic AST** - grammar-aware:
```c
nomen structura {
    Arbor2NodeGenus         genus;
    /* Token slots with trivia */
    Arbor2TokenSlot*        slots;          /* array of token slots */
    i32                     slot_count;
    /* Children */
    Xar*                    children;       /* Xar of Arbor2Node* */
    /* Metadata */
    Arbor2Origo*            origo;
    i32                     layer_index;
    /* Links */
    structura Arbor2Node*   pater;
    Arbor2SourceNode*       source_node;    /* link to source tree */
} Arbor2Node;
```

### Token Slots (for roundtrip)

Reuse concept from current arbor's trivia handling:

```c
nomen structura {
    Arbor2Token*    token;          /* actual token (any layer) */
    Xar*            trivia_ante;    /* leading whitespace */
    Xar*            trivia_post;    /* trailing whitespace */
} Arbor2TokenSlot;
```

### Comments as Nodes

```c
nomen enumeratio {
    ARBOR2_COMMENT_OWN_LINE,        /* comment on its own line */
    ARBOR2_COMMENT_END_OF_LINE,     /* comment at end of code line */
    ARBOR2_COMMENT_INLINE,          /* comment between tokens on same line */
    ARBOR2_COMMENT_FREESTANDING     /* blank lines above and below */
} Arbor2CommentPosition;

nomen structura {
    chorda              texto;
    Arbor2CommentPosition position;
    b32                 est_multiline;
    b32                 est_banner;     /* ====== style */
    /* Attachment */
    Arbor2Node*         attached_to;    /* NULL if freestanding */
    b32                 before;         /* true = before attached_to */
} Arbor2Comment;
```

### Ambiguous Nodes

```c
nomen enumeratio {
    ARBOR2_AMBIG_ERROR,             /* parse error with recovery */
    ARBOR2_AMBIG_PROBABLY_TYPEDEF,  /* identifier in type position */
    ARBOR2_AMBIG_PROBABLY_MACRO     /* identifier that might expand */
} Arbor2AmbigGenus;

nomen structura {
    Arbor2AmbigGenus    genus;
    chorda*             identifier;     /* the ambiguous name */
    Xar*                skipped_tokens; /* for error recovery */
    chorda              classification; /* best-effort: "declaration", "expression", etc. */
} Arbor2AmbigNode;
```

---

## Phase 5: Conditional Compilation

### All-Branches Parsing

```c
nomen structura {
    chorda*             condition_text; /* "#ifdef FOO" or "#if X > 0" */
    b32                 est_evaluated;  /* true if we know the value */
    b32                 valor;          /* value if evaluated */
    Xar*                lexemata;       /* tokens in this branch */
    Arbor2Node*         parsed;         /* AST for this branch */
} Arbor2ConditionalBranch;

nomen structura {
    Xar*                branches;       /* Xar of Arbor2ConditionalBranch* */
    i32                 linea_if;       /* location of #if */
    i32                 linea_endif;    /* location of #endif */
} Arbor2Conditional;
```

### Query-Time Filtering

```c
/* User specifies which conditions are "true" */
nomen structura {
    TabulaDispersa*     defined_macros;     /* macros to treat as defined */
    TabulaDispersa*     undefined_macros;   /* macros to treat as undefined */
} Arbor2ConditionConfig;

/* Query with condition filtering */
Xar* arbor2_query_functions(
    Arbor2AST* ast,
    Arbor2ConditionConfig* config);  /* NULL = all branches */
```

---

## Phase 6: Identifier Index

### Built During Parse

```c
nomen structura {
    chorda*             identifier;     /* interned */
    Xar*                uses;           /* Xar of Arbor2IdentifierUse* */
} Arbor2IdentifierEntry;

nomen structura {
    Arbor2Token*        token;          /* where used */
    Arbor2Node*         node;           /* containing AST node */
    Arbor2IdentifierContext context;    /* TYPE_POSITION, EXPR_POSITION, etc. */
    i32                 layer_index;
} Arbor2IdentifierUse;

/* Index structure */
nomen structura {
    TabulaDispersa*     entries;        /* identifier -> Arbor2IdentifierEntry* */
} Arbor2IdentifierIndex;
```

### Incremental Re-parse Support

```c
/* Check if new macro affects any identifier in file */
b32 arbor2_macro_affects_file(
    Arbor2IdentifierIndex* index,
    chorda* macro_name)
{
    redde tabula_dispersa_continet(index->entries, *macro_name);
}
```

---

## Phase 7: API Design

### Main Entry Point

```c
/* Parse options */
nomen structura {
    /* Include paths */
    constans character**    include_viae;
    i32                     include_numerus;

    /* Injected definitions */
    constans character**    macro_defs;     /* "NAME=VALUE" strings */
    i32                     macro_def_numerus;
    constans character**    typedef_names;
    i32                     typedef_numerus;

    /* Output mode */
    Arbor2OutputMode        modus;          /* ANALYSIS, EDITOR, TRANSFORM */

    /* Feature flags */
    b32                     keep_alternatives;
    b32                     keep_full_chain;
    b32                     parse_all_branches;
} Arbor2Options;

/* Parse result */
nomen structura {
    b32                     successus;
    Arbor2SourceTree*       source_tree;    /* grammar-agnostic */
    Arbor2AST*              ast;            /* semantic tree */
    Arbor2Expansion*        expansion;      /* all layers */
    Arbor2IdentifierIndex*  index;
    Xar*                    errores;        /* Xar of Arbor2Error* */
    Xar*                    warnings;       /* Dependable C, C99 usage */
} Arbor2Result;

/* Main API */
Arbor2Result arbor2_parse_file(
    constans character*     via,
    Piscina*                piscina,
    InternamentumChorda*    intern,
    Arbor2Options*          options);

Arbor2Result arbor2_parse_source(
    constans character*     source,
    i32                     length,
    Piscina*                piscina,
    InternamentumChorda*    intern,
    Arbor2Options*          options);
```

### Query API

```c
/* Find all uses of identifier */
Xar* arbor2_find_uses(
    Arbor2Result* result,
    chorda* identifier,
    Arbor2ConditionConfig* conditions);  /* NULL = all */

/* Find all function definitions */
Xar* arbor2_find_functions(
    Arbor2Result* result,
    Arbor2ConditionConfig* conditions);

/* Get token at specific layer */
Arbor2Token* arbor2_get_token_at_layer(
    Arbor2Token* token,
    i32 layer_index);

/* Walk provenance chain */
Xar* arbor2_get_provenance(Arbor2Token* token);
```

---

## Implementation Order

### Milestone 1: Foundation
1. Set up arbor2 file structure
2. Define core structs (Arbor2Token, Arbor2Layer, Arbor2Node)
3. Implement layered expansion (reuse macro handling from arbor)
4. Test: expand simple macros, verify layers

### Milestone 2: GLR Core
1. Implement GSS data structures
2. Implement basic GLR algorithm (shift, reduce, fork, merge)
3. Hand-write tables for expression grammar subset
4. Test: parse ambiguous expressions with forking

### Milestone 3: C89 Grammar
1. Add declaration parsing
2. Add statement parsing
3. Add full expression grammar
4. Integrate macro lookahead for disambiguation
5. Test: parse real library files

### Milestone 4: Robustness
1. Add error recovery with classification
2. Add "probably typedef/macro" nodes
3. Add conditional compilation (all-branches)
4. Test: parse files with missing includes/macros

### Milestone 5: Polish
1. Add identifier index
2. Add query API
3. Add output modes (analysis, editor, transform)
4. Add Dependable C annotations
5. Test: roundtrip formatting, queries

---

## Testing Strategy

### Unit Tests
- Layered expansion correctness
- GLR fork/merge behavior
- Individual grammar productions

### Integration Tests
- Parse all files in `probationes/fixa/roundtrip/`
- Compare with current arbor output where applicable

### Stress Tests
- Large files (10K+ lines)
- Deep macro nesting
- Many conditional branches

### Compatibility Tests
- Files that work with current arbor should work with arbor-v2
- Files that fail current arbor (missing macros) should produce useful ambiguous nodes

---

## File Size Estimates

Based on current arbor (10K+ lines total):

| Component | Current Arbor | Arbor-v2 Estimate |
|-----------|---------------|-------------------|
| Lexer | 1,337 lines | ~500 lines (reuse arbor_lexema) |
| Expansion | 3,519 lines | ~2,000 lines (layered approach) |
| Parser | 5,830 lines | ~8,000 lines (GLR + grammar) |
| API | 196 lines | ~500 lines |
| **Total** | ~11,000 lines | ~11,000 lines |

GLR adds complexity but layered expansion simplifies preprocessor modes.

