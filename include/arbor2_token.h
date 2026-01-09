#ifndef ARBOR2_TOKEN_H
#define ARBOR2_TOKEN_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "arbor2_lexema.h"

/* ==================================================
 * ARBOR2 TOKEN - Token with Provenance Chain
 *
 * Each token tracks its origin through macro expansion.
 * origo_meta is ALWAYS allocated (no NULL checks).
 *
 * EXEMPLUM:
 *   // Create source token (layer 0)
 *   Arbor2Token* tok = arbor2_token_ex_lexema(piscina, lexema, "file.c", 10, 5);
 *
 *   // Walk back to original source
 *   Arbor2Token* radix = arbor2_token_radix(tok);
 * ================================================== */

/* ==================================================
 * Origo (Origin) Types
 * ================================================== */

nomen enumeratio {
    ARBOR2_ORIGO_FONS,          /* Original source token */
    ARBOR2_ORIGO_EXPANSIO,      /* From macro expansion */
    ARBOR2_ORIGO_PASTA,         /* From ## token pasting */
    ARBOR2_ORIGO_CHORDA,        /* From # stringification */
    ARBOR2_ORIGO_API            /* From API-injected macro (no source file) */
} Arbor2OrigoGenus;

/* ==================================================
 * Origo Metadata
 *
 * Always allocated for every token.
 * For FONS tokens: nomen_macro = NULL, layer_index = 0
 * For expanded tokens: tracks which macro produced it
 * ================================================== */

nomen structura {
    Arbor2OrigoGenus    genus;
    chorda*             nomen_macro;    /* NULL if FONS */
    chorda*             via_file;       /* NULL if API-injected without file */
    i32                 linea;          /* Line in source/macro */
    i32                 columna;        /* Column in source/macro */
    i32                 layer_index;    /* Expansion layer (0 = source) */
} Arbor2OrigoMeta;

/* ==================================================
 * Origo Pasta (Token Paste Provenance)
 *
 * Extended metadata for ## operator results.
 * Both source tokens are linked for full provenance.
 * ================================================== */

nomen structura Arbor2Token Arbor2Token;

nomen structura {
    Arbor2OrigoMeta     base;           /* genus = ARBOR2_ORIGO_PASTA */
    Arbor2Token*        sinister;       /* Left source token */
    Arbor2Token*        dexter;         /* Right source token */
} Arbor2OrigoPasta;

/* ==================================================
 * Token Structure
 *
 * Wraps an Arbor2Lexema with provenance chain.
 * origo_token links to previous token in chain (NULL for source).
 * origo_meta is ALWAYS allocated.
 * ================================================== */

structura Arbor2Token {
    Arbor2Lexema*       lexema;         /* The actual token data */
    Arbor2Token*        origo_token;    /* Previous in chain, NULL for source */
    Arbor2OrigoMeta*    origo_meta;     /* Always allocated */
};

/* ==================================================
 * API - Token Creation
 * ================================================== */

/* Creare token ex lexema (layer 0, source token) */
Arbor2Token* arbor2_token_ex_lexema(
    Piscina*                piscina,
    Arbor2Lexema*           lexema,
    chorda*                 via_file,
    i32                     linea,
    i32                     columna);

/* Creare token ex expansione macro */
Arbor2Token* arbor2_token_ex_expansione(
    Piscina*                piscina,
    Arbor2Lexema*           lexema,
    Arbor2Token*            origo_token,
    chorda*                 nomen_macro,
    chorda*                 via_file,
    i32                     layer_index);

/* Creare token ex pasta (## operator) */
Arbor2Token* arbor2_token_ex_pasta(
    Piscina*                piscina,
    Arbor2Lexema*           lexema,
    Arbor2Token*            sinister,
    Arbor2Token*            dexter,
    chorda*                 nomen_macro,
    i32                     layer_index);

/* Creare token ex stringificatione (# operator) */
Arbor2Token* arbor2_token_ex_chorda(
    Piscina*                piscina,
    Arbor2Lexema*           lexema,
    Arbor2Token*            origo_token,
    chorda*                 nomen_macro,
    i32                     layer_index);

/* Creare token ex API (macro injected via API) */
Arbor2Token* arbor2_token_ex_api(
    Piscina*                piscina,
    Arbor2Lexema*           lexema,
    chorda*                 nomen_macro,
    chorda*                 via_file,   /* May be NULL */
    i32                     layer_index);

/* ==================================================
 * API - Token Queries
 * ================================================== */

/* Walk chain to root (original source token) */
Arbor2Token* arbor2_token_radix(Arbor2Token* token);

/* Count depth of provenance chain */
i32 arbor2_token_profunditas(Arbor2Token* token);

/* Get pasta provenance if token is from ## (returns NULL otherwise) */
Arbor2OrigoPasta* arbor2_token_pasta(Arbor2Token* token);

/* Check if token is from source (layer 0) */
b32 arbor2_token_est_fons(Arbor2Token* token);

/* Get origin type name for debugging */
constans character* arbor2_origo_genus_nomen(Arbor2OrigoGenus genus);

#endif /* ARBOR2_TOKEN_H */
