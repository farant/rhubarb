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

#endif /* ARBOR2_EXPANDERE_H */
