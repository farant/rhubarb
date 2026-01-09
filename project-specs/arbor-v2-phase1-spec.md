# Arbor v2 Phase 1 - Refined Specification

Date: 2026-01-09
Status: Draft (post-simulation refinement)
Based on: arbor-v2-simulation-spec-v2.md, second implementation simulation, follow-up interview

---

## Overview

This specification refines Phase 1 of arbor-v2 based on a second implementation simulation that surfaced complications #18-27 and subsequent interview decisions.

**Phase 1 Scope:**
- Forked lexer with NOVA_LINEA and line continuation
- Arbor2Token with linked provenance
- Segment-based layered expansion
- ##, # operator handling

---

## Lexer: Full Fork

### Decision

Fork `arbor_lexema.c` entirely to create `arbor2_lexema.c`. Do not wrap.

**Rationale:** Modifications needed (NOVA_LINEA, line continuation, trivia handling) are pervasive enough that a wrapper adds complexity. Clean fork allows free modification without affecting v1.

### New Token Type

```c
/* In Arbor2LexemaGenus enum */
ARBOR2_LEXEMA_NOVA_LINEA,    /* Explicit newline token */
```

Emitted for each newline in source. Not hidden in trivia.

### Line Continuation

Backslash-newline (`\\\n`) handling:
- Lexer joins logical lines (standard translation phase 2)
- Metadata preserved for roundtrip: trivia records that continuation occurred
- NOVA_LINEA NOT emitted for continued lines

```c
nomen structura {
    chorda      valor;
    b32         est_commentum;
    b32         est_c99;
    b32         est_continuatio;    /* NEW: true if this trivia was \\\n */
    i32         linea;
    i32         columna;
} Arbor2Trivia;
```

---

## Token Structure

### Always Allocate origo_meta

Every `Arbor2Token` has `origo_meta` allocated (no NULL checks):

```c
nomen structura Arbor2Token {
    Arbor2Lexema*           lexema;
    structura Arbor2Token*  origo_token;    /* Previous in chain, NULL for source */
    Arbor2OrigoMeta*        origo_meta;     /* Always allocated */
};

nomen enumeratio {
    ARBOR2_ORIGO_FONS,          /* Original source token */
    ARBOR2_ORIGO_EXPANSIO,      /* From macro expansion */
    ARBOR2_ORIGO_PASTA,         /* From ## token pasting */
    ARBOR2_ORIGO_CHORDA,        /* From # stringification */
    ARBOR2_ORIGO_API            /* From API-injected macro (no source file) */
} Arbor2OrigoGenus;

nomen structura {
    Arbor2OrigoGenus    genus;
    chorda*             nomen_macro;    /* NULL if FONS */
    chorda*             via_file;       /* NULL if API-injected without file */
    i32                 linea;
    i32                 columna;
    i32                 layer_index;
} Arbor2OrigoMeta;
```

### Token Paste Provenance

For ## operator, both source tokens are linked:

```c
nomen structura {
    Arbor2OrigoMeta     base;           /* genus = ARBOR2_ORIGO_PASTA */
    Arbor2Token*        sinister;       /* Left source token */
    Arbor2Token*        dexter;         /* Right source token */
} Arbor2OrigoPasta;
```

- `origo_token` follows sinister (left) as primary chain
- Both sinister and dexter accessible via cast to `Arbor2OrigoPasta*`

---

## Segment-Based Expansion

### Concept

A **segment** is a contiguous range of source where macro state doesn't change. Segments are created only when #define or #undef occurs.

```c
nomen structura {
    i32                 linea_initium;  /* First line of segment */
    i32                 linea_finis;    /* Last line (exclusive) */
    TabulaDispersa*     macros;         /* Macro table state for this segment */
} Arbor2Segmentum;
```

Most files have 1 segment (no mid-file #define/#undef changes).

### Query API

Line-based granularity (sufficient because directives are line-terminated):

```c
/* Get macro table state at a given line */
TabulaDispersa* arbor2_macros_ad_lineam(
    Arbor2Expansion*    exp,
    i32                 linea);

/* Get all segments */
Xar* arbor2_obtinere_segmenta(Arbor2Expansion* exp);
```

### Expansion Flow

```
1. Lex source → Arbor2Lexema stream (with NOVA_LINEA)
2. Wrap into Arbor2Token → Layer 0
3. Scan layer 0 sequentially:
   - On #define: add to current macro table, start new segment if first #define on this line
   - On #undef: remove from macro table, start new segment
   - On macro invocation: expand using current macro table
   - Regular tokens: pass through
4. Result is Layer 1 (and segments for provenance)
5. Repeat 3-4 until fixpoint
```

---

## Macro Definition Storage

### Structure

```c
nomen structura {
    chorda*         titulus;            /* Macro name (interned) */
    b32             est_functio;
    b32             est_variadic;
    Xar*            parametra;          /* Xar of chorda* (param names) */
    Xar*            corpus;             /* Xar of Arbor2Lexema* */
    /* Source location (when available) */
    chorda*         via_file;           /* NULL for API-injected w/o file */
    i32             linea_def;          /* -1 if unknown */
    b32             ex_api;             /* true if API-injected */
} Arbor2MacroDef;
```

### API-Injected Macros

When user provides macros via API:

```c
vacuum arbor2_addere_macro(
    Arbor2Contextus*    ctx,
    constans character* nomen,
    constans character* valor,
    constans character* via_file);  /* NULL for no associated file */
```

If `via_file` provided, tokens expanding from this macro will have that provenance.
If NULL, `origo_meta->genus = ARBOR2_ORIGO_API` and `via_file = NULL`.

---

## Function-Like Macro Expansion

### Argument Collection

- Collect tokens between `(` and matching `)`
- Track paren depth for nested calls
- Split by `,` at depth 1
- **Include NOVA_LINEA** in argument token lists (preserve source structure)

### Per-Operation Normalization

| Operation | NOVA_LINEA Handling |
|-----------|---------------------|
| Normal substitution | Keep with trivia intact |
| Stringify (#) | Collapse to single space (per standard) |
| Token paste (##) | Ignore; paste adjacent token values only |

---

## Stringification (#)

Standard-compliant implementation:

1. Each whitespace/newline sequence → single space
2. Leading/trailing whitespace removed
3. Escape `"` → `\"`
4. Escape `\` → `\\`

```c
interior Arbor2Token*
_stringify_arg(
    Xar*                    arg_tokens,
    Arbor2OrigoMeta*        origo_parent,
    Piscina*                piscina,
    InternamentumChorda*    intern)
{
    chorda_constructor buf;
    b32 need_space = FALSUM;
    b32 in_whitespace = VERUM;  /* Trim leading */

    per (i = 0; i < xar_numerus(arg_tokens); i++)
    {
        Arbor2Token* tok = *(Arbor2Token**)xar_obtinere(arg_tokens, i);

        /* Skip NOVA_LINEA, collapse to space */
        si (tok->lexema->genus == ARBOR2_LEXEMA_NOVA_LINEA)
        {
            si (!in_whitespace) need_space = VERUM;
            perge;
        }

        /* Handle trivia (whitespace) */
        si (_has_whitespace_trivia(tok))
        {
            si (!in_whitespace) need_space = VERUM;
            perge;
        }

        /* Output space if needed */
        si (need_space)
        {
            chorda_constructor_addere_char(&buf, ' ');
            need_space = FALSUM;
        }

        /* Output token value with escaping */
        _append_escaped(&buf, tok->lexema->valor);
        in_whitespace = FALSUM;
    }

    /* Create string literal token */
    /* ... */
}
```

---

## Token Pasting (##)

### Algorithm

1. After parameter substitution, scan for `##`
2. Get tokens immediately adjacent (last of left, first of right)
3. Concatenate their text values
4. Re-lex the concatenated text (best-effort)
5. Create new token with `Arbor2OrigoPasta` provenance

### Best-Effort Lexing

If concatenation produces invalid token, lexer returns whatever it can:
- Valid token type → use it
- Invalid → `ARBOR2_LEXEMA_ERROR` with concatenated text as valor

```c
interior Arbor2Token*
_paste_tokens(
    Arbor2Token*            sinister,
    Arbor2Token*            dexter,
    Arbor2OrigoMeta*        origo_parent,
    Piscina*                piscina,
    InternamentumChorda*    intern)
{
    /* Concatenate text */
    chorda combined = chorda_concatenare(
        sinister->lexema->valor,
        dexter->lexema->valor,
        piscina);

    /* Re-lex (single token) */
    Arbor2Lexema* lex = _relex_single_token(combined, piscina, intern);

    /* Create token with merged provenance */
    Arbor2Token* tok = piscina_allocare(piscina, sizeof(Arbor2Token));
    tok->lexema = lex;
    tok->origo_token = sinister;  /* Primary chain follows left */

    Arbor2OrigoPasta* pasta = piscina_allocare(piscina, sizeof(Arbor2OrigoPasta));
    pasta->base.genus = ARBOR2_ORIGO_PASTA;
    pasta->base.nomen_macro = origo_parent->nomen_macro;
    pasta->base.layer_index = origo_parent->layer_index + 1;
    pasta->sinister = sinister;
    pasta->dexter = dexter;

    tok->origo_meta = (Arbor2OrigoMeta*)pasta;

    redde tok;
}
```

---

## Include Handling

### Include State Tracking

Configurable: per-parse reset or persistent across calls.

```c
nomen enumeratio {
    ARBOR2_INCLUDE_RESET_PER_PARSE,
    ARBOR2_INCLUDE_PERSISTENT
} Arbor2IncludeModus;

vacuum arbor2_ponere_include_modum(
    Arbor2Contextus*    ctx,
    Arbor2IncludeModus  modus);
```

### Circular Detection

Track processed files in hash table:

```c
nomen structura Arbor2Expansion {
    /* ... */
    TabulaDispersa*     included_viae;  /* via -> processed flag */
};
```

---

## Typedef Handling

### Heuristic Detection

From context includes, detect typedefs by pattern matching:
- `typedef ... Name ;`
- Track brace/paren depth
- Last identifier before `;` at depth 0 is typedef name

### User Override

API allows explicit typedef declaration (overrides heuristics):

```c
vacuum arbor2_addere_typedef(
    Arbor2Contextus*    ctx,
    constans character* nomen);
```

### GLR Integration

GLR uses typedef knowledge to guide parsing:
- Known typedef → parse as type
- Unknown → may fork on ambiguity
- User-declared takes precedence

---

## Files to Create

```
include/
├── arbor2_lexema.h         # Forked lexer header
├── arbor2_token.h          # Token with provenance
├── arbor2_expandere.h      # Expansion engine
├── arbor2_segmentum.h      # Segment tracking

lib/
├── arbor2_lexema.c         # Forked lexer (from arbor_lexema.c)
├── arbor2_token.c          # Token utilities
├── arbor2_expandere.c      # Expansion implementation
├── arbor2_segmentum.c      # Segment management

probationes/
├── probatio_arbor2_lexema.c
├── probatio_arbor2_expandere.c
```

---

## Open Questions

1. **Segment storage efficiency**: Store full macro table per segment (simple but memory-heavy) or deltas (compact but complex lookups)?

2. **Multi-file expansion**: When target file includes another target file (not context include), how do segments interact across files?

3. **#if constant evaluation**: Segment boundaries at `#if` depend on evaluating conditions. Defer to Phase 5 (conditional compilation)?
