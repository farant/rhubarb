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
