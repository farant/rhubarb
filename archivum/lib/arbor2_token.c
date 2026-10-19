/* ==================================================
 * ARBOR2 TOKEN - Token with Provenance Chain Implementation
 *
 * Tracks token origin through macro expansion layers.
 * ================================================== */

#include "arbor2_token.h"

/* ==================================================
 * Origo Type Names (pro debugging)
 * ================================================== */

hic_manens constans character* ORIGO_NOMINA[] = {
    "FONS",
    "EXPANSIO",
    "PASTA",
    "CHORDA",
    "API"
};

/* ==================================================
 * Token Creation - Source Token (Layer 0)
 * ================================================== */

Arbor2Token*
arbor2_token_ex_lexema(
    Piscina*                piscina,
    Arbor2Lexema*           lexema,
    chorda*                 via_file,
    i32                     linea,
    i32                     columna)
{
    Arbor2Token* token;
    Arbor2OrigoMeta* meta;

    token = piscina_allocare(piscina, magnitudo(Arbor2Token));
    token->lexema = lexema;
    token->origo_token = NIHIL;  /* Source token has no predecessor */

    meta = piscina_allocare(piscina, magnitudo(Arbor2OrigoMeta));
    meta->genus = ARBOR2_ORIGO_FONS;
    meta->nomen_macro = NIHIL;
    meta->via_file = via_file;
    meta->linea = linea;
    meta->columna = columna;
    meta->layer_index = ZEPHYRUM;  /* Layer 0 */

    token->origo_meta = meta;

    redde token;
}

/* ==================================================
 * Token Creation - Macro Expansion
 * ================================================== */

Arbor2Token*
arbor2_token_ex_expansione(
    Piscina*                piscina,
    Arbor2Lexema*           lexema,
    Arbor2Token*            origo_token,
    chorda*                 nomen_macro,
    chorda*                 via_file,
    i32                     layer_index)
{
    Arbor2Token* token;
    Arbor2OrigoMeta* meta;

    token = piscina_allocare(piscina, magnitudo(Arbor2Token));
    token->lexema = lexema;
    token->origo_token = origo_token;

    meta = piscina_allocare(piscina, magnitudo(Arbor2OrigoMeta));
    meta->genus = ARBOR2_ORIGO_EXPANSIO;
    meta->nomen_macro = nomen_macro;
    meta->via_file = via_file;
    meta->linea = (origo_token != NIHIL) ? origo_token->lexema->linea : ZEPHYRUM;
    meta->columna = (origo_token != NIHIL) ? origo_token->lexema->columna : ZEPHYRUM;
    meta->layer_index = layer_index;

    token->origo_meta = meta;

    redde token;
}

/* ==================================================
 * Token Creation - Token Paste (## Operator)
 * ================================================== */

Arbor2Token*
arbor2_token_ex_pasta(
    Piscina*                piscina,
    Arbor2Lexema*           lexema,
    Arbor2Token*            sinister,
    Arbor2Token*            dexter,
    chorda*                 nomen_macro,
    i32                     layer_index)
{
    Arbor2Token* token;
    Arbor2OrigoPasta* pasta;

    token = piscina_allocare(piscina, magnitudo(Arbor2Token));
    token->lexema = lexema;
    token->origo_token = sinister;  /* Primary chain follows left */

    pasta = piscina_allocare(piscina, magnitudo(Arbor2OrigoPasta));
    pasta->base.genus = ARBOR2_ORIGO_PASTA;
    pasta->base.nomen_macro = nomen_macro;
    pasta->base.via_file = (sinister != NIHIL && sinister->origo_meta != NIHIL)
                            ? sinister->origo_meta->via_file : NIHIL;
    pasta->base.linea = (sinister != NIHIL) ? sinister->lexema->linea : ZEPHYRUM;
    pasta->base.columna = (sinister != NIHIL) ? sinister->lexema->columna : ZEPHYRUM;
    pasta->base.layer_index = layer_index;
    pasta->sinister = sinister;
    pasta->dexter = dexter;

    token->origo_meta = (Arbor2OrigoMeta*)pasta;

    redde token;
}

/* ==================================================
 * Token Creation - Stringification (# Operator)
 * ================================================== */

Arbor2Token*
arbor2_token_ex_chorda(
    Piscina*                piscina,
    Arbor2Lexema*           lexema,
    Arbor2Token*            origo_token,
    chorda*                 nomen_macro,
    i32                     layer_index)
{
    Arbor2Token* token;
    Arbor2OrigoMeta* meta;

    token = piscina_allocare(piscina, magnitudo(Arbor2Token));
    token->lexema = lexema;
    token->origo_token = origo_token;

    meta = piscina_allocare(piscina, magnitudo(Arbor2OrigoMeta));
    meta->genus = ARBOR2_ORIGO_CHORDA;
    meta->nomen_macro = nomen_macro;
    meta->via_file = (origo_token != NIHIL && origo_token->origo_meta != NIHIL)
                      ? origo_token->origo_meta->via_file : NIHIL;
    meta->linea = (origo_token != NIHIL) ? origo_token->lexema->linea : ZEPHYRUM;
    meta->columna = (origo_token != NIHIL) ? origo_token->lexema->columna : ZEPHYRUM;
    meta->layer_index = layer_index;

    token->origo_meta = meta;

    redde token;
}

/* ==================================================
 * Token Creation - API Injected Macro
 * ================================================== */

Arbor2Token*
arbor2_token_ex_api(
    Piscina*                piscina,
    Arbor2Lexema*           lexema,
    chorda*                 nomen_macro,
    chorda*                 via_file,
    i32                     layer_index)
{
    Arbor2Token* token;
    Arbor2OrigoMeta* meta;

    token = piscina_allocare(piscina, magnitudo(Arbor2Token));
    token->lexema = lexema;
    token->origo_token = NIHIL;  /* No predecessor for API tokens */

    meta = piscina_allocare(piscina, magnitudo(Arbor2OrigoMeta));
    meta->genus = ARBOR2_ORIGO_API;
    meta->nomen_macro = nomen_macro;
    meta->via_file = via_file;  /* May be NULL */
    meta->linea = ZEPHYRUM;
    meta->columna = ZEPHYRUM;
    meta->layer_index = layer_index;

    token->origo_meta = meta;

    redde token;
}

/* ==================================================
 * Token Queries
 * ================================================== */

Arbor2Token*
arbor2_token_radix(Arbor2Token* token)
{
    si (token == NIHIL)
    {
        redde NIHIL;
    }

    dum (token->origo_token != NIHIL)
    {
        token = token->origo_token;
    }

    redde token;
}

i32
arbor2_token_profunditas(Arbor2Token* token)
{
    i32 profunditas;

    profunditas = ZEPHYRUM;

    si (token == NIHIL)
    {
        redde ZEPHYRUM;
    }

    dum (token->origo_token != NIHIL)
    {
        profunditas++;
        token = token->origo_token;
    }

    redde profunditas;
}

Arbor2OrigoPasta*
arbor2_token_pasta(Arbor2Token* token)
{
    si (token == NIHIL || token->origo_meta == NIHIL)
    {
        redde NIHIL;
    }

    si (token->origo_meta->genus == ARBOR2_ORIGO_PASTA)
    {
        redde (Arbor2OrigoPasta*)token->origo_meta;
    }

    redde NIHIL;
}

b32
arbor2_token_est_fons(Arbor2Token* token)
{
    si (token == NIHIL || token->origo_meta == NIHIL)
    {
        redde FALSUM;
    }

    redde token->origo_meta->genus == ARBOR2_ORIGO_FONS;
}

constans character*
arbor2_origo_genus_nomen(Arbor2OrigoGenus genus)
{
    si (genus >= ZEPHYRUM && genus <= ARBOR2_ORIGO_API)
    {
        redde ORIGO_NOMINA[genus];
    }
    redde "IGNOTUM";
}
