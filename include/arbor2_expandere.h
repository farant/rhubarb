#ifndef ARBOR2_EXPANDERE_H
#define ARBOR2_EXPANDERE_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "internamentum.h"
#include "arbor2_lexema.h"
#include "arbor2_token.h"

/* ==================================================
 * ARBOR2 EXPANDERE - Macro Expansion Engine
 *
 * Segment-based layered expansion with full provenance.
 *
 * EXEMPLUM:
 *   Arbor2Expansion* exp = arbor2_expansion_creare(piscina, intern);
 *   arbor2_expansion_addere_macro(exp, "VERSION", "1", NULL);
 *   Xar* tokens = arbor2_expansion_processare(exp, fons, mensura, "file.c");
 * ================================================== */

/* ==================================================
 * Macro Definition
 * ================================================== */

nomen structura {
    chorda*         titulus;            /* Macro name (interned) */
    b32             est_functio;        /* True if function-like */
    b32             est_variadic;       /* True if has ... */
    Xar*            parametra;          /* Xar of chorda* (param names) */
    Xar*            corpus;             /* Xar of Arbor2Lexema* (body tokens) */
    /* Source location (when available) */
    chorda*         via_file;           /* NULL for API-injected w/o file */
    s32             linea_def;          /* -1 if unknown */
    b32             ex_api;             /* True if API-injected */
} Arbor2MacroDef;

/* ==================================================
 * Typedef Information
 *
 * Metadata about discovered typedefs.
 * ================================================== */

nomen structura {
    chorda*         titulus;            /* Typedef name (interned) */
    s32             layer_discovered;   /* Which layer found (-1 = API-injected) */
    Arbor2OrigoMeta* origo;             /* Source location (NULL for API-injected) */
    b32             est_certum;         /* VERUM if keyword, FALSUM if heuristic */
} Arbor2TypedefInfo;

/* ==================================================
 * Expansion Layer
 *
 * Each expansion pass produces a new layer.
 * Layer 0 is source tokens, layer N is after N expansions.
 * ================================================== */

nomen structura {
    Xar*            lexemata;           /* Xar of Arbor2Token* */
    i32             layer_index;        /* 0 = source */
    b32             est_fixpoint;       /* True if no more expansions needed */
} Arbor2Layer;

/* ==================================================
 * Segment
 *
 * A segment is a contiguous range where macro state
 * doesn't change. Created at #define/#undef boundaries.
 * ================================================== */

nomen structura {
    i32             linea_initium;      /* First line of segment (1-based) */
    s32             linea_finis;        /* Last line (exclusive), -1 = to end */
    TabulaDispersa* macros;             /* Snapshot of macro table for this segment */
} Arbor2Segmentum;

/* ==================================================
 * Include Mode
 * ================================================== */

nomen enumeratio {
    ARBOR2_INCLUDE_RESET_PER_PARSE,     /* Reset include tracking each parse */
    ARBOR2_INCLUDE_PERSISTENT           /* Track includes across parses */
} Arbor2IncludeModus;

/* ==================================================
 * Expansion Context
 *
 * Main state for macro expansion.
 * ================================================== */

nomen structura {
    Piscina*                piscina;
    InternamentumChorda*    intern;

    /* Macro state */
    TabulaDispersa*         macros;         /* Current macro table */
    TabulaDispersa*         typedefs;       /* Known typedef names */

    /* Expansion tracking */
    Xar*                    layers;         /* Xar of Arbor2Layer* */
    Xar*                    segmenta;       /* Xar of Arbor2Segmentum* */
    Xar*                    macro_stack;    /* Xar of chorda* - prevents recursion */

    /* Include tracking */
    TabulaDispersa*         included_viae;  /* Files already processed */
    Arbor2IncludeModus      include_modus;

    /* Include search paths */
    Xar*                    system_viae;    /* Xar of chorda* - <> include paths */
    Xar*                    local_viae;     /* Xar of chorda* - "" include paths */
    b32                     system_learning_default;  /* VERUM: system includes are learning mode */
    b32                     local_learning_default;   /* FALSUM: local includes are full mode */

    /* Current source info */
    chorda*                 via_current;    /* Current file being processed */
} Arbor2Expansion;

/* ==================================================
 * API - Context Creation
 * ================================================== */

/* Create expansion context */
Arbor2Expansion* arbor2_expansion_creare(
    Piscina*                piscina,
    InternamentumChorda*    intern);

/* Set include tracking mode */
vacuum arbor2_expansion_ponere_include_modum(
    Arbor2Expansion*        exp,
    Arbor2IncludeModus      modus);

/* ==================================================
 * API - Macro Management
 * ================================================== */

/* Add API-injected macro (object-like) */
vacuum arbor2_expansion_addere_macro(
    Arbor2Expansion*        exp,
    constans character*     nomen_macro,
    constans character*     valor,
    constans character*     via_file);  /* NULL for no associated file */

/* Add API-injected function-like macro */
vacuum arbor2_expansion_addere_macro_functio(
    Arbor2Expansion*        exp,
    constans character*     nomen_macro,
    constans character**    parametra,      /* NULL-terminated array */
    constans character*     corpus,
    constans character*     via_file);

/* Add known typedef */
vacuum arbor2_expansion_addere_typedef(
    Arbor2Expansion*        exp,
    constans character*     nomen_typedef);

/* ==================================================
 * API - Include Paths
 * ================================================== */

/* Add system include search path (<> includes) */
vacuum arbor2_expansion_addere_system_via(
    Arbor2Expansion*        exp,
    constans character*     via);

/* Add local include search path ("" includes) */
vacuum arbor2_expansion_addere_local_via(
    Arbor2Expansion*        exp,
    constans character*     via);

/* Resolve include path to absolute (returns NIHIL if not found)
 *
 * exp:           expansion context
 * via_specifier: path from #include directive
 * genus:         SYSTEM (<>) or LOCAL ("")
 * via_current:   current file path (for relative resolution)
 *
 * Returns: allocated chorda* with absolute path, or NIHIL if not found
 */
chorda* arbor2_expansion_resolvere_include(
    Arbor2Expansion*        exp,
    chorda                  via_specifier,
    i32                     genus,
    chorda*                 via_current);

/* ==================================================
 * API - Expansion
 * ================================================== */

/* Main entry: lex source and expand to fixpoint */
Xar* arbor2_expansion_processare(
    Arbor2Expansion*        exp,
    constans character*     fons,
    i32                     mensura,
    constans character*     via_file);

/* Get all segments (for provenance queries) */
Xar* arbor2_expansion_obtinere_segmenta(
    Arbor2Expansion*        exp);

/* Get macro table state at a given line */
TabulaDispersa* arbor2_expansion_macros_ad_lineam(
    Arbor2Expansion*        exp,
    i32                     linea);

/* Get all layers (for debugging/inspection) */
Xar* arbor2_expansion_obtinere_layers(
    Arbor2Expansion*        exp);

/* ==================================================
 * API - Macro Queries
 * ================================================== */

/* Look up a macro by name */
Arbor2MacroDef* arbor2_expansion_quaerere_macro(
    Arbor2Expansion*        exp,
    chorda                  titulus);

/* Check if name is a typedef */
b32 arbor2_expansion_est_typedef(
    Arbor2Expansion*        exp,
    chorda                  titulus);

/* Get typedef info (returns NULL if not found) */
Arbor2TypedefInfo* arbor2_expansion_quaerere_typedef(
    Arbor2Expansion*        exp,
    chorda                  titulus);

/* ==================================================
 * API - Macro Lookahead
 *
 * Inspect what a macro would expand to without full expansion.
 * Used by GLR parser for disambiguation decisions.
 * ================================================== */

nomen structura {
    Arbor2LexemaGenus   genus;          /* First token's genus */
    b32                 est_recursivum; /* First token is another macro */
    b32                 est_vacuum;     /* Empty body */
} Arbor2ExpansionLookahead;

/* Simple lookahead: returns first token's genus,
 * or ARBOR2_LEXEMA_ERROR if unknown macro */
Arbor2LexemaGenus arbor2_expansion_lookahead_genus(
    Arbor2Expansion*        exp,
    chorda                  nomen_macro);

/* Extended lookahead with additional metadata */
Arbor2ExpansionLookahead arbor2_expansion_lookahead(
    Arbor2Expansion*        exp,
    chorda                  nomen_macro);

/* ==================================================
 * API - Built-in Latin C89 Macros
 * ================================================== */

/* Register all macros from latina.h for Latin C89 parsing.
 * This includes type mappings (integer->int), keywords (si->if),
 * qualifiers (constans->const), and storage specifiers (interior->static).
 */
vacuum arbor2_includere_latina(Arbor2Expansion* exp);

#endif /* ARBOR2_EXPANDERE_H */
