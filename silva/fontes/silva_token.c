/* silva_token.c - Impletio contractus lexematis unificati */

#include "silva_token.h"
#include <string.h>


/* ==================================================
 * Cauda C89 (phasis V, T7b) - quod MateriaToken non fert
 * ================================================== */

nomen structura {
    SilvaOrigo  origo;       /* INSERTA - numquam NIHIL */
           i32  longitudo;   /* octeti FONTIS (laminae incl.) */
            i8  standard;    /* SILVA_STANDARD_* */
           Xar* scissurae;   /* SilvaScissura; NIHIL fere semper */
} SilvaTokenCauda;

hic_manens constans MateriaTokenForma FORMA_C89 =
    { (i32)magnitudo(SilvaTokenCauda) };

/* Cauda C89 lexematis - MUTABILIS etiam ex lexemate constanti (ut
 * materia_token_cauda: datum frontis est, non lexematis) */
interior SilvaTokenCauda*
_cauda_c89 (
    constans SilvaToken* token)
{
    redde (SilvaTokenCauda*)materia_token_cauda(token);
}


/* ==================================================
 * Auxiliares interni
 * ================================================== */

interior SilvaToken*
_token_novum (
    Piscina* piscina)
{
    chorda vacua;

    vacua.mensura  = ZEPHYRUM;
    vacua.datum    = NIHIL;
    /* Cauda ad zephyrum posita (materia_token_creare): origo FONS,
     * standard C89, scissurae NIHIL - ut memset prior */
    redde materia_token_creare(piscina, &FORMA_C89, ZEPHYRUM, vacua,
        -I, ZEPHYRUM, ZEPHYRUM, -I);
}

interior b32
_chordae_pares (
    chorda a,
    chorda b)
{
    si (a.mensura != b.mensura)
    {
        redde FALSUM;
    }
    si (a.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    redde (memcmp(a.datum, b.datum, (memoriae_index)a.mensura)
        == ZEPHYRUM)
        ? VERUM : FALSUM;
}


/* ==================================================
 * Constructores
 * ================================================== */

SilvaToken*
silva_token_ex_fonte (
             Piscina* piscina,
    SilvaLexemaGenus  genus,
              chorda  valor,
                 s32  byte_offset,
                 i32  linea,
                 i32  columna,
                 s32  fons_index)
{
    SilvaToken* token;

    token = _token_novum(piscina);
    si (token == NIHIL)
    {
        redde NIHIL;
    }

    token->genus                    = (s32)genus;
    token->valor                    = valor;
    token->byte_offset              = byte_offset;
    _cauda_c89(token)->longitudo    = valor.mensura;
    token->linea                    = linea;
    token->columna                  = columna;
    token->fons_index               = fons_index;
    _cauda_c89(token)->standard     = SILVA_STANDARD_C89;
    _cauda_c89(token)->origo.genus  = SILVA_ORIGO_FONS;

    redde token;
}

SilvaToken*
silva_token_ex_expansione (
          Piscina* piscina,
       SilvaToken* corpus,
       SilvaToken* invocatio,
           chorda* nomen_macro,
    SilvaCaecatio* caecatio)
{
    SilvaToken* token;

    token = _token_novum(piscina);
    si (token == NIHIL)
    {
        redde NIHIL;
    }

    /* Campi lexicales a corpore (def-site identitas lexicalis) */
    token->genus                  = corpus->genus;
    token->valor                  = corpus->valor;
    token->byte_offset            = corpus->byte_offset;
    _cauda_c89(token)->longitudo  = _cauda_c89(corpus)->longitudo;
    token->linea                  = corpus->linea;
    token->columna                = corpus->columna;
    token->fons_index             = corpus->fons_index;
    _cauda_c89(token)->standard   = _cauda_c89(corpus)->standard;

    _cauda_c89(token)->origo.genus =
        SILVA_ORIGO_EXPANSIO;
    _cauda_c89(token)->origo.datum.expansio.corpus       = corpus;
    _cauda_c89(token)->origo.datum.expansio.invocatio    = invocatio;
    _cauda_c89(token)->origo.datum.expansio.nomen_macro  = nomen_macro;
    _cauda_c89(token)->origo.datum.expansio.caecatio     = caecatio;

    redde token;
}

SilvaToken*
silva_token_ex_pasta (
             Piscina* piscina,
    SilvaLexemaGenus  genus,
              chorda  valor,
          SilvaToken* sinister,
          SilvaToken* dexter,
          SilvaToken* invocatio,
              chorda* nomen_macro,
       SilvaCaecatio* caecatio)
{
    SilvaToken* token;

    token = _token_novum(piscina);
    si (token == NIHIL)
    {
        redde NIHIL;
    }

    token->genus                  = (s32)genus;
    token->valor                  = valor;
    _cauda_c89(token)->longitudo  = valor.mensura;
    si (sinister != NIHIL)
    {
        token->linea       = sinister->linea;
        token->columna     = sinister->columna;
        token->fons_index  = sinister->fons_index;
    }

    _cauda_c89(token)->origo.genus =
        SILVA_ORIGO_PASTA;
    _cauda_c89(token)->origo.datum.pasta.sinister     = sinister;
    _cauda_c89(token)->origo.datum.pasta.dexter       = dexter;
    _cauda_c89(token)->origo.datum.pasta.invocatio    = invocatio;
    _cauda_c89(token)->origo.datum.pasta.nomen_macro  = nomen_macro;
    _cauda_c89(token)->origo.datum.pasta.caecatio     = caecatio;

    redde token;
}

SilvaToken*
silva_token_ex_stringificatione (
       Piscina* piscina,
        chorda  valor,
    SilvaToken* primus,
        chorda* nomen_macro)
{
    SilvaToken* token;

    token = _token_novum(piscina);
    si (token == NIHIL)
    {
        redde NIHIL;
    }

    token->genus                  = SILVA_LEX_STRING_LIT;
    token->valor                  = valor;
    _cauda_c89(token)->longitudo  = valor.mensura;
    si (primus != NIHIL)
    {
        token->linea       = primus->linea;
        token->columna     = primus->columna;
        token->fons_index  = primus->fons_index;
    }

    _cauda_c89(token)->origo.genus =
        SILVA_ORIGO_CHORDA;
    _cauda_c89(token)->origo.datum.stringificatio.primus = primus;
    _cauda_c89(token)->origo.datum.stringificatio.nomen_macro =
        nomen_macro;

    redde token;
}

SilvaToken*
silva_token_ex_praedefinito (
             Piscina* piscina,
    SilvaLexemaGenus  genus,
              chorda  valor,
          SilvaToken* invocatio,
              chorda* nomen_macro)
{
    SilvaToken* token;

    token = _token_novum(piscina);
    si (token == NIHIL)
    {
        redde NIHIL;
    }

    token->genus                  = (s32)genus;
    token->valor                  = valor;
    _cauda_c89(token)->longitudo  = valor.mensura;
    si (invocatio != NIHIL)
    {
        token->linea       = invocatio->linea;
        token->columna     = invocatio->columna;
        token->fons_index  = invocatio->fons_index;
    }

    _cauda_c89(token)->origo.genus =
        SILVA_ORIGO_CHORDA;
    _cauda_c89(token)->origo.datum.stringificatio.primus =
        invocatio;
    _cauda_c89(token)->origo.datum.stringificatio.nomen_macro =
        nomen_macro;

    redde token;
}

SilvaToken*
silva_token_ex_api (
             Piscina* piscina,
    SilvaLexemaGenus  genus,
              chorda  valor,
              chorda* nomen_macro,
                 s32  fons_index)
{
    SilvaToken* token;

    token = _token_novum(piscina);
    si (token == NIHIL)
    {
        redde NIHIL;
    }

    token->genus                  = (s32)genus;
    token->valor                  = valor;
    _cauda_c89(token)->longitudo  = valor.mensura;
    token->fons_index             = fons_index;

    _cauda_c89(token)->origo.genus                  = SILVA_ORIGO_API;
    _cauda_c89(token)->origo.datum.api.nomen_macro  = nomen_macro;

    redde token;
}


/* ==================================================
 * Quaestiones catenae
 * ================================================== */

interior SilvaToken*
_praedecessor_use_site (
    SilvaToken* token)
{
    commutatio (_cauda_c89(token)->origo.genus)
    {
        casus SILVA_ORIGO_EXPANSIO:
            redde _cauda_c89(token)->origo.datum.expansio.invocatio;
        casus SILVA_ORIGO_PASTA:
            redde _cauda_c89(token)->origo.datum.pasta.sinister;
        casus SILVA_ORIGO_CHORDA:
            redde _cauda_c89(token)->origo.datum.stringificatio.primus;
        casus SILVA_ORIGO_FONS:
        casus SILVA_ORIGO_API:
        ordinarius:
            redde NIHIL;
    }
}

SilvaToken*
silva_token_radix (
    SilvaToken* token)
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
silva_token_profunditas (
    SilvaToken* token)
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
        profunditas  = profunditas + I;
        token        = praecedens;
    }
    redde profunditas;
}

b32
silva_token_est_fons (
    SilvaToken* token)
{
    redde (_cauda_c89(token)->origo.genus
        == SILVA_ORIGO_FONS) ? VERUM : FALSUM;
}


/* ---------- accessores campi C89 (phasis V, T7a - sutura) ---------- */

SilvaOrigo*
silva_token_origo (
    constans SilvaToken* token)
{
    redde &_cauda_c89(token)->origo;
}

SilvaLexemaGenus
silva_token_genus (
    constans SilvaToken* token)
{
    redde (SilvaLexemaGenus)token->genus;
}

i32
silva_token_longitudo (
    constans SilvaToken* token)
{
    redde _cauda_c89(token)->longitudo;
}

vacuum
silva_token_longitudo_ponere (
     SilvaToken* token,
            i32  longitudo)
{
    _cauda_c89(token)->longitudo = longitudo;
}

i8
silva_token_standard (
    constans SilvaToken* token)
{
    redde _cauda_c89(token)->standard;
}

vacuum
silva_token_standard_ponere (
     SilvaToken* token,
             i8  standard)
{
    _cauda_c89(token)->standard = standard;
}

Xar*
silva_token_scissurae (
    constans SilvaToken* token)
{
    redde _cauda_c89(token)->scissurae;
}

vacuum
silva_token_scissurae_ponere (
    SilvaToken* token,
           Xar* scissurae)
{
    _cauda_c89(token)->scissurae = scissurae;
}

b32
silva_token_initium_lineae (
    constans SilvaToken* token)
{
    redde materia_token_initium_lineae(token);
}

vacuum
silva_token_initium_lineae_ponere (
     SilvaToken* token,
            b32  valor)
{
    materia_token_initium_lineae_ponere(token, valor);
}

i32
silva_token_ante_numerus (
    constans SilvaToken* token)
{
    redde token->numerus_ante;
}

SilvaToken*
silva_token_ante (
    constans SilvaToken* token,
                    i32  index)
{
    si (index >= token->numerus_ante)
    {
        redde NIHIL;
    }
    redde token->spatia_ante[index];
}

i32
silva_token_post_numerus (
    constans SilvaToken* token)
{
    redde token->numerus_post;
}

SilvaToken*
silva_token_post (
    constans SilvaToken* token,
                    i32  index)
{
    si (index >= token->numerus_post)
    {
        redde NIHIL;
    }
    redde token->spatia_post[index];
}

b32
silva_token_ante_addere (
       Piscina* piscina,
    SilvaToken* token,
    SilvaToken* trivium)
{
    SilvaToken** series;
           i32   n;
           i32   i;

    /* Materia trivia SEMEL ponit ut seriem exactam: series n+I
     * construitur et iterum ponitur (ponere iterari licet). Numeri
     * parvi - trivia per lexema pauca. */
    n = token->numerus_ante;
    series = (SilvaToken**)piscina_allocare(piscina,
        (memoriae_index)(n + I) * magnitudo(SilvaToken*));
    si (series == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        series[i] = token->spatia_ante[i];
    }
    series[n] = trivium;
    redde materia_token_trivia_ante_ponere(token, piscina, series, n
        + I);
}

b32
silva_token_post_addere (
       Piscina* piscina,
    SilvaToken* token,
    SilvaToken* trivium)
{
    SilvaToken** series;
           i32   n;
           i32   i;

    /* Materia trivia SEMEL ponit ut seriem exactam: series n+I
     * construitur et iterum ponitur (ponere iterari licet). Numeri
     * parvi - trivia per lexema pauca. */
    n = token->numerus_post;
    series = (SilvaToken**)piscina_allocare(piscina,
        (memoriae_index)(n + I) * magnitudo(SilvaToken*));
    si (series == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        series[i] = token->spatia_post[i];
    }
    series[n] = trivium;
    redde materia_token_trivia_post_ponere(token, piscina, series, n
        + I);
}

b32
silva_token_ante_ponere (
        Piscina*  piscina,
     SilvaToken*  token,
     SilvaToken** series,
            i32   numerus)
{
    redde materia_token_trivia_ante_ponere(token, piscina, series,
        numerus);
}

b32
silva_token_post_ponere (
        Piscina*  piscina,
     SilvaToken*  token,
     SilvaToken** series,
            i32   numerus)
{
    redde materia_token_trivia_post_ponere(token, piscina, series,
        numerus);
}

vacuum
silva_token_ante_vacare (
    SilvaToken* token)
{
    (vacuum)materia_token_trivia_ante_ponere(token, NIHIL, NIHIL,
        ZEPHYRUM);
}

vacuum
silva_token_post_vacare (
    SilvaToken* token)
{
    (vacuum)materia_token_trivia_post_ponere(token, NIHIL, NIHIL,
        ZEPHYRUM);
}


/* ==================================================
 * Caecatio
 * ================================================== */

SilvaCaecatio*
silva_caecatio_extendere (
          Piscina* piscina,
    SilvaCaecatio* parens,
           chorda* titulus)
{
    SilvaCaecatio* cella;

    cella = (SilvaCaecatio*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilvaCaecatio));
    si (cella == NIHIL)
    {
        redde parens;
    }
    cella->titulus  = titulus;
    cella->cauda    = parens;
    redde cella;
}

b32
silva_caecatio_continet (
    SilvaCaecatio* caecatio,
           chorda  titulus)
{
    dum (caecatio != NIHIL)
    {
        si (   caecatio->titulus != NIHIL
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

hic_manens constans character* NOMINA_GENERUM[SILVA_LEX_NUMERUS_GENERUM] =
    {
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
silva_lexema_genus_nomen (
    SilvaLexemaGenus genus)
{
    si (   (integer)genus >= ZEPHYRUM
        && (integer)genus < (integer)SILVA_LEX_NUMERUS_GENERUM)
    {
        redde NOMINA_GENERUM[genus];
    }
    redde "IGNOTUM";
}

constans character*
silva_origo_genus_nomen (
    SilvaOrigoGenus genus)
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
