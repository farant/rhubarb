/* silva_token.c - Impletio contractus lexematis unificati */

#include "silva_token.h"
#include <string.h>

/* ==================================================
 * Auxiliares interni
 * ================================================== */

interior SilvaToken*
_token_novum (Piscina* piscina)
{
    SilvaToken* token;

    token = (SilvaToken*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilvaToken));
    si (token != NIHIL)
    {
        memset(token, ZEPHYRUM, magnitudo(SilvaToken));
        token->byte_offset = -I;
        token->fons_index  = -I;
    }
    redde token;
}

interior b32
_chordae_pares (chorda a, chorda b)
{
    si (a.mensura != b.mensura)
    {
        redde FALSUM;
    }
    si (a.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    redde (memcmp(a.datum, b.datum, (memoriae_index)a.mensura) == ZEPHYRUM)
        ? VERUM : FALSUM;
}


/* ==================================================
 * Constructores
 * ================================================== */

SilvaToken*
silva_token_ex_fonte (
    Piscina*         piscina,
    SilvaLexemaGenus genus,
    chorda           valor,
    s32              byte_offset,
    i32              linea,
    i32              columna,
    s32              fons_index)
{
    SilvaToken* token;

    token = _token_novum(piscina);
    si (token == NIHIL)
    {
        redde NIHIL;
    }

    token->genus       = genus;
    token->valor       = valor;
    token->byte_offset = byte_offset;
    token->longitudo   = valor.mensura;
    token->linea       = linea;
    token->columna     = columna;
    token->fons_index  = fons_index;
    token->standard    = SILVA_STANDARD_C89;
    token->origo.genus = SILVA_ORIGO_FONS;

    redde token;
}

SilvaToken*
silva_token_ex_expansione (
    Piscina*       piscina,
    SilvaToken*    corpus,
    SilvaToken*    invocatio,
    chorda*        nomen_macro,
    SilvaCaecatio* caecatio)
{
    SilvaToken* token;

    token = _token_novum(piscina);
    si (token == NIHIL)
    {
        redde NIHIL;
    }

    /* Campi lexicales a corpore (def-site identitas lexicalis) */
    token->genus       = corpus->genus;
    token->valor       = corpus->valor;
    token->byte_offset = corpus->byte_offset;
    token->longitudo   = corpus->longitudo;
    token->linea       = corpus->linea;
    token->columna     = corpus->columna;
    token->fons_index  = corpus->fons_index;
    token->standard    = corpus->standard;

    token->origo.genus                   = SILVA_ORIGO_EXPANSIO;
    token->origo.datum.expansio.corpus     = corpus;
    token->origo.datum.expansio.invocatio  = invocatio;
    token->origo.datum.expansio.nomen_macro = nomen_macro;
    token->origo.datum.expansio.caecatio    = caecatio;

    redde token;
}

SilvaToken*
silva_token_ex_pasta (
    Piscina*         piscina,
    SilvaLexemaGenus genus,
    chorda           valor,
    SilvaToken*      sinister,
    SilvaToken*      dexter,
    SilvaToken*      invocatio,
    chorda*          nomen_macro,
    SilvaCaecatio*   caecatio)
{
    SilvaToken* token;

    token = _token_novum(piscina);
    si (token == NIHIL)
    {
        redde NIHIL;
    }

    token->genus     = genus;
    token->valor     = valor;
    token->longitudo = valor.mensura;
    si (sinister != NIHIL)
    {
        token->linea      = sinister->linea;
        token->columna    = sinister->columna;
        token->fons_index = sinister->fons_index;
    }

    token->origo.genus                  = SILVA_ORIGO_PASTA;
    token->origo.datum.pasta.sinister    = sinister;
    token->origo.datum.pasta.dexter      = dexter;
    token->origo.datum.pasta.invocatio   = invocatio;
    token->origo.datum.pasta.nomen_macro = nomen_macro;
    token->origo.datum.pasta.caecatio    = caecatio;

    redde token;
}

SilvaToken*
silva_token_ex_stringificatione (
    Piscina*    piscina,
    chorda      valor,
    SilvaToken* primus,
    chorda*     nomen_macro)
{
    SilvaToken* token;

    token = _token_novum(piscina);
    si (token == NIHIL)
    {
        redde NIHIL;
    }

    token->genus     = SILVA_LEX_STRING_LIT;
    token->valor     = valor;
    token->longitudo = valor.mensura;
    si (primus != NIHIL)
    {
        token->linea      = primus->linea;
        token->columna    = primus->columna;
        token->fons_index = primus->fons_index;
    }

    token->origo.genus                           = SILVA_ORIGO_CHORDA;
    token->origo.datum.stringificatio.primus      = primus;
    token->origo.datum.stringificatio.nomen_macro = nomen_macro;

    redde token;
}

SilvaToken*
silva_token_ex_praedefinito (
    Piscina*         piscina,
    SilvaLexemaGenus genus,
    chorda           valor,
    SilvaToken*      invocatio,
    chorda*          nomen_macro)
{
    SilvaToken* token;

    token = _token_novum(piscina);
    si (token == NIHIL)
    {
        redde NIHIL;
    }

    token->genus     = genus;
    token->valor     = valor;
    token->longitudo = valor.mensura;
    si (invocatio != NIHIL)
    {
        token->linea      = invocatio->linea;
        token->columna    = invocatio->columna;
        token->fons_index = invocatio->fons_index;
    }

    token->origo.genus                            = SILVA_ORIGO_CHORDA;
    token->origo.datum.stringificatio.primus      = invocatio;
    token->origo.datum.stringificatio.nomen_macro = nomen_macro;

    redde token;
}

SilvaToken*
silva_token_ex_api (
    Piscina*         piscina,
    SilvaLexemaGenus genus,
    chorda           valor,
    chorda*          nomen_macro,
    s32              fons_index)
{
    SilvaToken* token;

    token = _token_novum(piscina);
    si (token == NIHIL)
    {
        redde NIHIL;
    }

    token->genus      = genus;
    token->valor      = valor;
    token->longitudo  = valor.mensura;
    token->fons_index = fons_index;

    token->origo.genus                = SILVA_ORIGO_API;
    token->origo.datum.api.nomen_macro = nomen_macro;

    redde token;
}


/* ==================================================
 * Quaestiones catenae
 * ================================================== */

interior SilvaToken*
_praedecessor_use_site (SilvaToken* token)
{
    commutatio (token->origo.genus)
    {
        casus SILVA_ORIGO_EXPANSIO:
            redde token->origo.datum.expansio.invocatio;
        casus SILVA_ORIGO_PASTA:
            redde token->origo.datum.pasta.sinister;
        casus SILVA_ORIGO_CHORDA:
            redde token->origo.datum.stringificatio.primus;
        casus SILVA_ORIGO_FONS:
        casus SILVA_ORIGO_API:
        ordinarius:
            redde NIHIL;
    }
}

SilvaToken*
silva_token_radix (SilvaToken* token)
{
    SilvaToken* praecedens;

    dum (token != NIHIL)
    {
        praecedens = _praedecessor_use_site(token);
        si (praecedens == NIHIL)
        {
            redde token;
        }
        token = praecedens;
    }
    redde NIHIL;
}

i32
silva_token_profunditas (SilvaToken* token)
{
    i32 profunditas;

    profunditas = ZEPHYRUM;
    dum (token != NIHIL)
    {
        SilvaToken* praecedens;

        praecedens = _praedecessor_use_site(token);
        si (praecedens == NIHIL)
        {
            frange;
        }
        profunditas = profunditas + I;
        token = praecedens;
    }
    redde profunditas;
}

b32
silva_token_est_fons (SilvaToken* token)
{
    redde (token->origo.genus == SILVA_ORIGO_FONS) ? VERUM : FALSUM;
}


/* ==================================================
 * Caecatio
 * ================================================== */

SilvaCaecatio*
silva_caecatio_extendere (
    Piscina*       piscina,
    SilvaCaecatio* parens,
    chorda*        titulus)
{
    SilvaCaecatio* cella;

    cella = (SilvaCaecatio*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilvaCaecatio));
    si (cella == NIHIL)
    {
        redde parens;
    }
    cella->titulus = titulus;
    cella->cauda   = parens;
    redde cella;
}

b32
silva_caecatio_continet (
    SilvaCaecatio* caecatio,
    chorda         titulus)
{
    dum (caecatio != NIHIL)
    {
        si (caecatio->titulus != NIHIL
            && _chordae_pares(*caecatio->titulus, titulus))
        {
            redde VERUM;
        }
        caecatio = caecatio->cauda;
    }
    redde FALSUM;
}


/* ==================================================
 * Auxiliares
 * ================================================== */

hic_manens constans character* NOMINA_GENERUM[SILVA_LEX_NUMERUS_GENERUM] = {
    "EOF",
    "OCTETUS_IGNOTUS",
    "STRING_IMPERFECTUM",
    "CHARACTER_IMPERFECTUM",
    "IDENTIFICATOR",
    "INTEGER",
    "FLOAT",
    "CHARACTER_LIT",
    "STRING_LIT",
    "AUTO", "BREAK", "CASE", "CHAR", "CONST", "CONTINUE", "DEFAULT",
    "DO", "DOUBLE", "ELSE", "ENUM", "EXTERN", "FLOAT_KW", "FOR",
    "GOTO", "IF", "INT", "LONG", "REGISTER", "RETURN", "SHORT",
    "SIGNED", "SIZEOF", "STATIC", "STRUCT", "SWITCH", "TYPEDEF",
    "UNION", "UNSIGNED", "VOID", "VOLATILE", "WHILE",
    "QUADRA_APERTA", "QUADRA_CLAUSA",
    "PAREN_APERTA", "PAREN_CLAUSA",
    "BRACE_APERTA", "BRACE_CLAUSA",
    "PUNCTUM", "SAGITTA",
    "INCREMENTUM", "DECREMENTUM",
    "AMPERSAND", "STAR", "PLUS", "MINUS", "TILDE", "EXCLAMATIO",
    "SOLIDUS", "PERCENTUM",
    "SINISTRORSUM", "DEXTRORSUM",
    "MINOR", "MAIOR", "MINOR_AEQUALIS", "MAIOR_AEQUALIS",
    "AEQUALIS_AEQUALIS", "NON_AEQUALIS",
    "CARET", "BARRA", "ET_ET", "VEL_VEL",
    "QUAESTIO", "COLON", "SEMICOLON", "ELLIPSIS",
    "ASSIGNATIO",
    "STAR_ASSIGNATIO", "SOLIDUS_ASSIGNATIO", "PERCENTUM_ASSIGNATIO",
    "PLUS_ASSIGNATIO", "MINUS_ASSIGNATIO",
    "SINISTRORSUM_ASSIGNATIO", "DEXTRORSUM_ASSIGNATIO",
    "AMPERSAND_ASSIGNATIO", "CARET_ASSIGNATIO", "BARRA_ASSIGNATIO",
    "COMMA", "CANCELLUM", "CANCELLUM_CANCELLUM",
    "SPATIA", "TABULAE", "NOVA_LINEA", "CONTINUATIO",
    "COMMENTUM_CLAUSUM", "COMMENTUM_LINEA"
};

constans character*
silva_lexema_genus_nomen (SilvaLexemaGenus genus)
{
    si ((integer)genus >= ZEPHYRUM
        && (integer)genus < (integer)SILVA_LEX_NUMERUS_GENERUM)
    {
        redde NOMINA_GENERUM[genus];
    }
    redde "IGNOTUM";
}

constans character*
silva_origo_genus_nomen (SilvaOrigoGenus genus)
{
    commutatio (genus)
    {
        casus SILVA_ORIGO_FONS:     redde "FONS";
        casus SILVA_ORIGO_EXPANSIO: redde "EXPANSIO";
        casus SILVA_ORIGO_PASTA:    redde "PASTA";
        casus SILVA_ORIGO_CHORDA:   redde "CHORDA";
        casus SILVA_ORIGO_API:      redde "API";
        ordinarius:                 redde "IGNOTUM";
    }
}
