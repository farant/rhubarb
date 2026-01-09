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
