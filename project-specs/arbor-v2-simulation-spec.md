# Arbor v2 - Simulation-Refined Specification

Date: 2026-01-09
Status: Draft (post-simulation refinement)
Based on: arbor-v2-spec.md, arbor-v2-spec-implementation.md, 3-hour implementation simulation

---

## Overview

This specification refines the arbor-v2 design based on findings from a 3-hour implementation simulation that surfaced 16+ complications. Key refinements address include processing, provenance chains, unknown macro/typedef handling, and API design.

---

## Refined Architecture

### Include Processing Model

**Two types of includes:**

1. **Context Includes** (learning only)
   - Specified via `include_viae` option
   - Only extract macro and typedef definitions
   - Tokens NOT merged into main stream
   - Used for: system headers, external library headers

2. **Target Files** (full parse)
   - Specified via main API invocation (file, array of files, globs)
   - Full token stream preserved
   - Tokens track which file they came from
   - Used for: user code being analyzed

**Processing order:**
1. Resolve context includes, build macro/typedef tables
2. Layer 0 = target file tokens (post-include resolution for definitions)
3. Each token tracks its source file via provenance

### Layer Model (Refined)

```
Layer 0: Target file tokens with provenance to source files
         Includes resolved for definitions only (not token merging)
         #define/#include directives present as tokens

Layer 1: First expansion pass
         Directives consumed/processed
         Macros expanded ONE level

Layer N: Nth expansion pass
         Continue until fixpoint (no expansions possible)

Final:   Fully expanded, ready for GLR parsing
```

**Key refinement:** Layer 0 is NOT a raw concatenation of all files. It's the target file(s) with macro/typedef context from includes.

### Token Provenance (Refined)

Tokens form a linked-list-like chain for provenance:

```c
nomen structura Arbor2Token {
    ArborLexema*        lexema;
    Arbor2Token*        origo_token;    /* Previous token in chain, or NULL */
    Arbor2OrigoMeta*    origo_meta;     /* Expansion metadata */
};

nomen structura Arbor2OrigoMeta {
    chorda*             nomen_macro;    /* NULL if from include/source */
    chorda*             via_file;       /* Source file path */
    i32                 linea;
    i32                 columna;
    i32                 layer_index;
    Arbor2OrigoGenus    genus;          /* SOURCE, EXPANSION, PASTE, STRINGIFY */
};

nomen enumeratio {
    ARBOR2_ORIGO_SOURCE,      /* Original source token */
    ARBOR2_ORIGO_EXPANSION,   /* From macro expansion */
    ARBOR2_ORIGO_PASTE,       /* From ## token pasting */
    ARBOR2_ORIGO_STRINGIFY    /* From # stringification */
} Arbor2OrigoGenus;
```

**For pasted tokens (##):**
```c
nomen structura Arbor2OrigoPaste {
    Arbor2OrigoMeta     base;
    Arbor2Token*        sinister;    /* Left token */
    Arbor2Token*        dexter;      /* Right token */
};
```

### Lexer Enhancement

**Add NEWLINE token type:**

```c
/* In ArborLexemaGenus enum */
ARBOR_LEXEMA_NOVA_LINEA,    /* Explicit newline token */
```

This simplifies directive boundary detection. The lexer emits newline as a token (not just trivia) when in preprocessor-aware mode.

---

## Unknown Macro/Typedef Handling

### Node Types

```c
nomen enumeratio {
    ARBOR2_AMBIG_TYPEDEF_IGNOTUM,   /* Unknown typedef - identifier in type position */
    ARBOR2_AMBIG_MACRO_IGNOTUM,     /* Unknown macro - identifier with arguments or in macro-likely position */
    ARBOR2_AMBIG_ERROR              /* Parse error with recovery */
} Arbor2AmbigGenus;

nomen structura {
    Arbor2AmbigGenus    genus;
    chorda*             identifier;
    Xar*                tokens_skipped;    /* For error recovery */
    chorda*             classificatio;     /* Best-effort: "type", "expression", etc. */
    f32                 confidentia;       /* 0.0-1.0 confidence in classification */
} Arbor2AmbigNodus;
```

### Precedence Rules

When an identifier is ambiguous:
1. **Typedef takes precedence** if in type position
2. **Macro takes precedence** if has arguments in non-call position
3. **Context-based heuristics** for other cases

### GLR Forking for Unknown

When encountering potential unknown macro:
- Fork both "is macro" and "is not macro" interpretations
- For keyword positions (where 'si', 'dum', etc. expected), test keyword macro hypothesis
- Prune forks as syntax rules eliminate possibilities

---

## Conditional Compilation (Refined)

### Node-Based Forks

Instead of whole-file forks, #ifdef creates local AST forks:

```c
nomen structura {
    chorda*             conditio;       /* "#ifdef FOO" or "#if X > 0" */
    Xar*                rami;           /* Xar of Arbor2ConditioRamus* */
    i32                 linea_if;
    i32                 linea_endif;
} Arbor2ConditioNodus;

nomen structura {
    chorda*             conditio;       /* Specific condition for this branch */
    Arbor2Nodus*        corpus;         /* Parsed content of this branch */
    b32                 habet_typedef;  /* Does this branch define typedefs? */
    b32                 habet_macro;    /* Does this branch define macros? */
} Arbor2ConditioRamus;
```

### Condition Tracking

- Track mutual exclusivity (if/else pairs)
- Track linked conditions (same #ifdef in multiple places)
- Minimize parsing forks by identifying independent conditions

### Query API

```c
/* Query with wildcard (default) - searches all branches */
Xar* arbor2_quaerere(Arbor2Contextus* ctx, chorda* quaestio);

/* Query with specific conditions */
Xar* arbor2_quaerere_cum_conditionibus(
    Arbor2Contextus*        ctx,
    chorda*                 quaestio,
    Arbor2ConditioConfig*   config);

nomen structura {
    Xar*    definita;       /* Macros to treat as defined */
    Xar*    indefinita;     /* Macros to treat as undefined */
} Arbor2ConditioConfig;
```

---

## API Design (Refined)

### Builder Pattern

```c
/* Create context */
Arbor2Contextus* arbor2_contextus_creare(
    Piscina*              piscina,
    InternamentumChorda*  intern);

/* Configure */
vacuum arbor2_addere_viam_include(Arbor2Contextus* ctx, constans character* via);
vacuum arbor2_addere_macro_def(Arbor2Contextus* ctx, constans character* def);
vacuum arbor2_addere_typedef_def(Arbor2Contextus* ctx, constans character* nomen);
vacuum arbor2_includere_latina(Arbor2Contextus* ctx, b32 includere);

/* Parse target(s) */
Arbor2Status arbor2_parsere_filum(Arbor2Contextus* ctx, constans character* via);
Arbor2Status arbor2_parsere_fila(Arbor2Contextus* ctx, constans character** viae, i32 numerus);
Arbor2Status arbor2_parsere_fontem(Arbor2Contextus* ctx, constans character* fons, i32 longitudo);

/* Query results */
Arbor2Nodus*  arbor2_obtinere_radix(Arbor2Contextus* ctx);
Arbor2Layer*  arbor2_obtinere_layer(Arbor2Contextus* ctx, i32 index);
i32           arbor2_numerus_layers(Arbor2Contextus* ctx);
Xar*          arbor2_obtinere_errores(Arbor2Contextus* ctx);

/* Complete result struct (optional) */
Arbor2Resultus arbor2_obtinere_resultum(Arbor2Contextus* ctx);
```

### Built-in Latina Definitions

```c
/* Compiled-in latina.h definitions */
externus constans Arbor2MacroDef ARBOR2_LATINA_MACROS[];
externus constans i32 ARBOR2_LATINA_MACROS_NUMERUS;

/* Convenience: include all latina definitions */
vacuum arbor2_includere_latina(Arbor2Contextus* ctx, b32 includere);

/* Or add individually */
vacuum arbor2_addere_macro_defs(Arbor2Contextus* ctx,
    constans Arbor2MacroDef* defs, i32 numerus);
```

### No Modes - Layers Replace Modes

The PROCESSARE/PRESERVARE/HYBRID modes are eliminated. Instead:
- Always produce all layers
- Query layer 0 for "source view"
- Query final layer for "expanded view"
- Query intermediate layers for debugging/analysis

---

## Source View

Each layer maintains a "source view" - an array of tokens with trivia:

```c
nomen structura {
    Xar*        lexemata;       /* Xar of Arbor2Token* */
    i32         layer_index;
    b32         est_fixpoint;
} Arbor2Layer;

/* Get source view for any layer */
Xar* arbor2_layer_obtinere_tokens(Arbor2Layer* layer);

/* Stringify layer to source text */
chorda* arbor2_layer_stringify(Arbor2Layer* layer, Piscina* piscina);
```

The AST also supports roundtrip via token slots:
```c
/* Stringify AST node (preserves trivia) */
chorda* arbor2_nodus_stringify(Arbor2Nodus* nodus, Piscina* piscina);
```

---

## Error Recovery (Configurable)

```c
nomen enumeratio {
    ARBOR2_RECOVERY_BOUNDARIES,     /* Recover at ; and } only */
    ARBOR2_RECOVERY_AGGRESSIVE      /* Try to salvage partial expressions */
} Arbor2RecoveryModus;

vacuum arbor2_ponere_recovery_modum(Arbor2Contextus* ctx, Arbor2RecoveryModus modus);
```

Default: ARBOR2_RECOVERY_BOUNDARIES

---

## Comment Handling

Comments auto-attached to adjacent nodes during parse:

```c
nomen structura {
    chorda              textus;
    Arbor2CommentumPos  positio;        /* OWN_LINE, END_LINE, INLINE, FREESTANDING */
    b32                 est_multilinea;
    b32                 est_banner;     /* ====== style */
    Arbor2Nodus*        attachatum_ad;  /* NULL if freestanding */
    b32                 ante;           /* true = before attached node */
} Arbor2Commentum;
```

Attachment heuristics (auto-applied):
- End-of-line comment → attach to preceding node
- Comment followed by code (no blank line) → attach to following node
- Comment with blank lines above and below → freestanding

---

## Identifier Index

Unified across all layers:

```c
nomen structura {
    TabulaDispersa*     per_identifier;  /* identifier → Arbor2IdentifierInfo* */
} Arbor2Index;

nomen structura {
    chorda*             identifier;
    Xar*                usus;            /* Xar of Arbor2IdentifierUsus* */
} Arbor2IdentifierInfo;

nomen structura {
    Arbor2Token*        token;
    Arbor2Nodus*        nodus;
    Arbor2IdentifierContext contextus;   /* TYPE_POSITION, EXPR_POSITION, etc. */
    i32                 layer_index;
} Arbor2IdentifierUsus;
```

Used for:
- Incremental re-parse detection
- Query: "find all uses of X"
- Ambiguity resolution: "is X used as type elsewhere?"

---

## Incremental Updates

When new typedef/macro definition provided:

```c
/* Update unknown nodes that match identifier */
Arbor2Status arbor2_resolvere_ignotum(
    Arbor2Contextus*    ctx,
    chorda*             identifier,
    Arbor2DefGenus      genus,      /* TYPEDEF or MACRO */
    constans character* definitio); /* Optional: actual definition */
```

Implementation: query identifier index, update matching unknown nodes, may trigger localized re-parse if definition changes parse tree structure.

---

## Dependable C

Post-parse or on-demand:

```c
/* Check for Dependable C violations */
Xar* arbor2_verificare_dependable_c(Arbor2Contextus* ctx);

/* Query specific rule */
Xar* arbor2_quaerere_violationes(Arbor2Contextus* ctx, chorda* regula);
```

Returns list of violations with location and description.

---

## Memory Model

Single arena (piscina) for all allocations:
- GLR forks use mark/reset
- Failed forks reset to checkpoint
- Successful forks accumulate
- Accept memory growth for complex files

```c
/* GLR checkpoint */
PiscinaNotatio checkpoint = piscina_notare(ctx->piscina);

/* On fork failure */
piscina_reficere(ctx->piscina, checkpoint);

/* On fork success - checkpoint discarded, allocations kept */
```

---

## Implementation Phases (Refined)

### Phase 1: Token Infrastructure
- Arbor2Token with linked provenance
- Arbor2Layer structure
- NEWLINE token in lexer
- Source view maintenance

### Phase 2: Expansion Engine
- Fresh implementation (not fork of praeparator)
- Strict layer semantics
- Handle ##, # operators with merged provenance
- Function-like macro argument handling

### Phase 3: Unknown Handling
- Arbor2AmbigNodus types
- Typedef/macro inference heuristics
- Unknown node creation during expansion

### Phase 4: GLR Parser
- Graph-structured stack
- Fork/merge mechanics
- Macro lookahead integration
- Unknown macro forking

### Phase 5: Conditional Compilation
- Node-based forks
- Condition tracking
- Wildcard query support

### Phase 6: API & Polish
- Builder pattern API
- Built-in latina definitions
- Identifier index
- Dependable C checks

---

## Testing Strategy

1. **Module tests** - Unit tests for each phase as developed
2. **Parity tests** - Port arbor v1 test cases to verify equivalent behavior
3. **Roundtrip tests** - Use existing probationes/fixa/roundtrip files
4. **Regression suite** - Accumulated test cases from development

---

## Complications Addressed

| # | Complication | Resolution |
|---|--------------|------------|
| 1 | ArborLexema origo vs chain | Linked list via origo_token pointer |
| 2 | Fixpoint detection | any_expanded flag in layer |
| 3 | Recursive vs layered expansion | Fresh implementation with strict layers |
| 4 | Macro args with macros | Expand arguments as part of same expansion step |
| 5 | Lexema to Token conversion | Wrap on creation with provenance |
| 6 | #define in layers | Present in layer 0, consumed in 0→1 |
| 7 | Include processing | Context includes (learn only) vs target files (full) |
| 8 | No NEWLINE token | Add ARBOR_LEXEMA_NOVA_LINEA |
| 9 | Line continuation | Lexer handles, use line numbers |
| 10 | Directive boundaries | NEWLINE token simplifies detection |
| 11 | Praeparator coupling | Fresh implementation |
| 12 | Reuse vs rewrite | Reuse test cases, fresh code |
| 13 | xar_inserere missing | Build provenance chain oldest-first |
| 14 | File path tracking | In Arbor2OrigoMeta |
| 15 | Include token merging | Don't merge - track via provenance |
| 16 | Layer 0 definition | Target files with context from includes |
| 17 | Token pasting provenance | Arbor2OrigoPaste with left/right sources |

---

## Open Questions

1. **Argument expansion timing**: Expand args before or during macro body substitution?
2. **Conditional explosion**: How to cap fork count for deeply nested #if?
3. **Performance targets**: Acceptable parse time for 10K line file?
