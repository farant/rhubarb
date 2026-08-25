/* stml.c - STML (Structured Text Markup Language) Parser
 *
 * XML superset with:
 * - Boolean attributes
 * - Raw content tags (! suffix)
 * - Capture operators
 */

#include "stml.h"
#include "selectio.h"
#include <string.h>


/* ==================================================
 * Adiutores Interni - Character Classification
 * ================================================== */

interior b32
_est_nomen_initium (
    character c)
{
    redde (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || c == '_' || c == ':';
}

interior b32
_est_nomen_character (
    character c)
{
    redde (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9')
        || c == '_' || c == '-' || c == ':' || c == '.';
}

interior b32
_est_spatium (
    character c)
{
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

/* Character valid for fragment ID: alphanumeric, underscore, hyphen */
interior b32
_est_fragmentum_id_character (
    character c)
{
    redde (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9')
        || c == '_' || c == '-';
}

/* Comparare chorda* cum literis C */
interior b32
_chorda_ptr_aequalis_literis (
                chorda* ch,
    constans character* cstr)
{
    si (!ch)
    {
        redde FALSUM;
    }
    redde chorda_aequalis_literis(*ch, cstr);
}


/* ==================================================
 * Tokenizer Context
 * ================================================== */

nomen structura {
    chorda input;
       i32 positus;
       i32 linea;
       i32 columna;

    /* Raw content mode */
       b32 in_crudus;
    chorda crudus_titulus;  /* Tag name we're looking for to close */

    /* Piscina and intern for allocations */
                Piscina* piscina;
    InternamentumChorda* intern;
} StmlTokenContext;

nomen structura {
    StmlTokenGenus genus;
            chorda valor;
               i32 positus_initium;
               i32 positus_finis;
               i32 linea;
               i32 columna;

    /* For tags: parsed attributes */
    Xar* attributa;

    /* For capture operators */
    i32 captio_numerus;

    /* For raw tags with forward capture: captured content */
    chorda captus_contentus;
       b32 habet_captus;

    /* Signum '\' generi textus (spec triviae §1.4): '<tag\>' aut
     * '<tag!\>' - lineae novae CONTENTUM, dedentatio in
     * parsatione. Ordo '\!' = ERRATUM (lexatio mutaretur). */
    b32 multilinea;

    /* Trivia intra tagum (§1.6): spatium ante '>' aut '/>' tagi
     * aperientis (vacuum = nullum). Formis capturae NON notatur
     * (dispositio capturae canonica manet). */
    chorda spatia_prae_finem;
} StmlToken;


/* ==================================================
 * Tokenizer Functions
 * ================================================== */

interior character
_tok_aspicere (
    StmlTokenContext* ctx,
                 i32  offset)
{
    i32 pos;
    pos = ctx->positus + offset;
    si (pos < ZEPHYRUM || pos >= ctx->input.mensura)
    {
        redde '\0';
    }
    redde (character)ctx->input.datum[pos];
}

interior vacuum
_tok_progredi (
    StmlTokenContext* ctx,
                 i32  numerus)
{
    i32 i;
    per (i = ZEPHYRUM; i < numerus && ctx->positus < ctx->input.mensura; i++)
    {
        si ((character)ctx->input.datum[ctx->positus] == '\n')
        {
            ctx->linea++;
            ctx->columna = I;
        }
        alioquin
        {
            ctx->columna++;
        }
        ctx->positus++;
    }
}

interior vacuum
_tok_praeterire_spatium (
    StmlTokenContext* ctx)
{
    dum (   ctx->positus < ctx->input.mensura
         && _est_spatium(_tok_aspicere(ctx, ZEPHYRUM)))
    {
        _tok_progredi(ctx, I);
    }
}

/* Nomen vacuum coram charactere non-structurali = initium tituli
 * illegale (e.g. '<.x>', '<9bad>'). Olim positus non progrediebatur
 * et clausura anonyma vitium TACITE devorabat (arbor corrupta,
 * successus=VERUM). '<>' lenis manet (strictum TITULUS_VACUUS). */
interior b32
_titulus_male_incipit (
    StmlTokenContext* ctx,
              chorda  titulus)
{
    character c;

    si (titulus.mensura > ZEPHYRUM)
    {
        redde FALSUM;
    }

    c = _tok_aspicere(ctx, ZEPHYRUM);
    redde c != '>' && c != '/' && c != '!' && c != '('
        && c != '=' && c != '\0' && !_est_spatium(c);
}

interior chorda
_tok_legere_nomen (
    StmlTokenContext* ctx)
{
    chorda result;
       i32 initium;

    initium = ctx->positus;

    /* Punctum UNUM ducens in titulis elementorum licitum (spatium
     * generum: '<.species>' generat quod '.species' citat; punctum
     * pars NOMINIS est). Attributa numquam puncta accipiunt - custos
     * eorum (_est_nomen_initium ante vocationem) punctum reicit
     * priusquam huc veniat. */
    si (   _tok_aspicere(ctx, ZEPHYRUM) == '.'
        && _est_nomen_initium(_tok_aspicere(ctx, I)))
    {
        _tok_progredi(ctx, I);
    }
    alioquin si (!_est_nomen_initium(_tok_aspicere(ctx, ZEPHYRUM)))
    {
        result.datum    = NIHIL;
        result.mensura  = ZEPHYRUM;
        redde result;
    }

    dum (   ctx->positus < ctx->input.mensura
         && _est_nomen_character(_tok_aspicere(ctx, ZEPHYRUM)))
    {
        _tok_progredi(ctx, I);
    }

    result.datum    = ctx->input.datum + initium;
    result.mensura  = ctx->positus - initium;
    redde result;
}

/* Read fragment ID (alphanumeric, underscore, hyphen) */
interior chorda
_tok_legere_fragmentum_id (
    StmlTokenContext* ctx)
{
    chorda result;
       i32 initium;

    initium = ctx->positus;

    dum (   ctx->positus < ctx->input.mensura
         && _est_fragmentum_id_character(_tok_aspicere(ctx, ZEPHYRUM)))
    {
        _tok_progredi(ctx, I);
    }

    result.datum    = ctx->input.datum + initium;
    result.mensura  = ctx->positus - initium;
    redde result;
}

interior chorda
_tok_legere_valor_attributi (
    StmlTokenContext* ctx)
{
       chorda result;
    character quota;
    character ch;
          i32 initium;

    ch = _tok_aspicere(ctx, ZEPHYRUM);

    /* Valor cum quotis (quoted value) */
    si (ch == '"' || ch == '\'')
    {
        quota = ch;
        _tok_progredi(ctx, I);  /* Skip opening quote */
        initium = ctx->positus;

        dum (   ctx->positus < ctx->input.mensura
             && _tok_aspicere(ctx, ZEPHYRUM) != quota)
        {
            _tok_progredi(ctx, I);
        }

        result.datum    = ctx->input.datum + initium;
        result.mensura  = ctx->positus - initium;

        si (_tok_aspicere(ctx, ZEPHYRUM) == quota)
        {
            _tok_progredi(ctx, I);  /* Skip closing quote */
        }

        redde result;
    }

    /* Valor nudus (bare value) - alphanumericus, hyphen, underscore */
    initium = ctx->positus;
    dum (ctx->positus < ctx->input.mensura)
    {
        ch = _tok_aspicere(ctx, ZEPHYRUM);
        si (   (ch >= 'a' && ch <= 'z')
            || (ch >= 'A' && ch <= 'Z')
            || (ch >= '0' && ch <= '9')
            || ch == '-' || ch == '_')
        {
            _tok_progredi(ctx, I);
        }
        alioquin
        {
            frange;
        }
    }

    result.datum    = ctx->input.datum + initium;
    result.mensura  = ctx->positus - initium;
    redde result;
}

/* Parse attributes into Xar of StmlAttributum
 * Handles boolean attributes (no = sign)
 *
 * TRIVIA INTRA TAGUM (§1.6): spatium ANTE quodque attributum
 * notatur (attr->spatia_ante; spatium unicum canonicum = NIHIL);
 * spatium ante finem tagi in 'cauda_spatii' redditur (vocans id
 * in spatia_prae_finem condit nisi forma capturae). Spatium circa
 * '=' NON modellatur - ad formam strictam normalizatur (exceptio
 * angusta nominata). */
interior Xar*
_tok_legere_attributa (
    StmlTokenContext* ctx,
              chorda* cauda_spatii)
{
               Xar* attributa;
            chorda  titulus_ch;
            chorda  valor_ch;
            chorda  spatium;
            chorda* titulus_ptr;
            chorda* valor_ptr;
            chorda* spatium_ptr;
    StmlAttributum* attr;
               i32  spatii_initium;

    cauda_spatii->datum    = NIHIL;
    cauda_spatii->mensura  = ZEPHYRUM;

    attributa = xar_creare(ctx->piscina, magnitudo(StmlAttributum));
    si (!attributa)
    {
        redde NIHIL;
    }

    dum (ctx->positus < ctx->input.mensura)
    {
        spatii_initium = ctx->positus;
        _tok_praeterire_spatium(ctx);
        spatium.datum    = ctx->input.datum + spatii_initium;
        spatium.mensura  = ctx->positus - spatii_initium;

        /* Check for end of tag */
        si (   _tok_aspicere(ctx, ZEPHYRUM) == '>'
            || (_tok_aspicere(ctx, ZEPHYRUM) == '/' && _tok_aspicere(ctx, I) == '>')
            || _tok_aspicere(ctx, ZEPHYRUM) == '(' ||  /* Forward capture */
            _tok_aspicere(ctx, ZEPHYRUM) == '=')    /* Sandwich capture (after <=) */
        {
            *cauda_spatii = spatium;
            frange;
        }

        /* Check if this looks like an attribute name */
        si (!_est_nomen_initium(_tok_aspicere(ctx, ZEPHYRUM)))
        {
            *cauda_spatii = spatium;
            frange;
        }

        /* spatium ante attributum: unicum canonicum = NIHIL */
        spatium_ptr = NIHIL;
        si (!(   spatium.mensura == I
              && (character)spatium.datum[ZEPHYRUM] == ' '))
        {
            spatium_ptr = chorda_internare(ctx->intern, spatium);
        }

        /* Read attribute name */
        titulus_ch = _tok_legere_nomen(ctx);
        si (titulus_ch.mensura == ZEPHYRUM)
        {
            frange;
        }

        _tok_praeterire_spatium(ctx);

        /* Check for = sign */
        si (_tok_aspicere(ctx, ZEPHYRUM) != '=')
        {
            /* Boolean attribute - no value, use "true" */
            titulus_ptr = chorda_internare(ctx->intern, titulus_ch);
            valor_ptr = chorda_internare_ex_literis(ctx->intern, "true");

            attr = xar_addere(attributa);
            si (attr)
            {
                attr->titulus      = titulus_ptr;
                attr->valor        = valor_ptr;
                attr->spatia_ante  = spatium_ptr;
            }
            perge;
        }

        /* Skip = */
        _tok_progredi(ctx, I);
        _tok_praeterire_spatium(ctx);

        /* Read value */
        valor_ch = _tok_legere_valor_attributi(ctx);

        titulus_ptr  = chorda_internare(ctx->intern, titulus_ch);
        valor_ptr    = chorda_internare(ctx->intern, valor_ch);

        attr = xar_addere(attributa);
        si (attr)
        {
            attr->titulus      = titulus_ptr;
            attr->valor        = valor_ptr;
            attr->spatia_ante  = spatium_ptr;
        }
    }

    redde attributa;
}

/* Parse a comment <!-- ... --> */
interior StmlToken
_tok_legere_commentum (
    StmlTokenContext* ctx)
{
    StmlToken token;
          i32 initium;
          i32 initium_linea;
          i32 initium_columna;
          i32 contentus_initium;

    initium          = ctx->positus;
    initium_linea    = ctx->linea;
    initium_columna  = ctx->columna;

    /* Skip <!-- */
    _tok_progredi(ctx, IV);
    contentus_initium = ctx->positus;

    /* Find --> */
    dum (ctx->positus < ctx->input.mensura)
    {
        si (   _tok_aspicere(ctx, ZEPHYRUM) == '-'
            && _tok_aspicere(ctx, I)        == '-'
            && _tok_aspicere(ctx, II)       == '>')
        {
            token.valor.datum    = ctx->input.datum + contentus_initium;
            token.valor.mensura  = ctx->positus - contentus_initium;
            _tok_progredi(ctx, III);  /* Skip --> */
            frange;
        }
        _tok_progredi(ctx, I);
    }

    token.genus                      = STML_TOKEN_COMMENTUM;
    token.positus_initium            = initium;
    token.positus_finis              = ctx->positus;
    token.linea                      = initium_linea;
    token.columna                    = initium_columna;
    token.attributa                  = NIHIL;
    token.captio_numerus             = ZEPHYRUM;
    token.habet_captus               = FALSUM;
    token.multilinea                 = FALSUM;
    token.spatia_prae_finem.datum    = NIHIL;
    token.spatia_prae_finem.mensura  = ZEPHYRUM;

    redde token;
}

/* Parse processing instruction <?...?> */
interior StmlToken
_tok_legere_processio (
    StmlTokenContext* ctx)
{
    StmlToken token;
          i32 initium;
          i32 initium_linea;
          i32 initium_columna;
          i32 contentus_initium;

    initium          = ctx->positus;
    initium_linea    = ctx->linea;
    initium_columna  = ctx->columna;

    /* Skip <? */
    _tok_progredi(ctx, II);
    contentus_initium = ctx->positus;

    /* Find ?> */
    dum (ctx->positus < ctx->input.mensura)
    {
        si (   _tok_aspicere(ctx, ZEPHYRUM) == '?'
            && _tok_aspicere(ctx, I)        == '>')
        {
            token.valor.datum    = ctx->input.datum + contentus_initium;
            token.valor.mensura  = ctx->positus - contentus_initium;
            _tok_progredi(ctx, II);  /* Skip ?> */
            frange;
        }
        _tok_progredi(ctx, I);
    }

    token.genus                      = STML_TOKEN_PROCESSIO;
    token.positus_initium            = initium;
    token.positus_finis              = ctx->positus;
    token.linea                      = initium_linea;
    token.columna                    = initium_columna;
    token.attributa                  = NIHIL;
    token.captio_numerus             = ZEPHYRUM;
    token.habet_captus               = FALSUM;
    token.multilinea                 = FALSUM;
    token.spatia_prae_finem.datum    = NIHIL;
    token.spatia_prae_finem.mensura  = ZEPHYRUM;

    redde token;
}

/* Parse DOCTYPE <!DOCTYPE ...> */
interior StmlToken
_tok_legere_doctype (
    StmlTokenContext* ctx)
{
    StmlToken token;
          i32 initium;
          i32 initium_linea;
          i32 initium_columna;
          i32 contentus_initium;
          i32 profunditas;

    initium          = ctx->positus;
    initium_linea    = ctx->linea;
    initium_columna  = ctx->columna;

    /* Skip <!DOCTYPE */
    _tok_progredi(ctx, IX);
    _tok_praeterire_spatium(ctx);
    contentus_initium = ctx->positus;

    /* Find matching > (handle nested <>) */
    profunditas = I;
    dum (ctx->positus < ctx->input.mensura && profunditas > ZEPHYRUM)
    {
        si (_tok_aspicere(ctx, ZEPHYRUM) == '<')
        {
            profunditas++;
        }
        alioquin si (_tok_aspicere(ctx, ZEPHYRUM) == '>')
        {
            profunditas--;
        }
        si (profunditas > ZEPHYRUM)
        {
            _tok_progredi(ctx, I);
        }
    }

    token.valor.datum    = ctx->input.datum + contentus_initium;
    token.valor.mensura  = ctx->positus - contentus_initium;

    si (_tok_aspicere(ctx, ZEPHYRUM) == '>')
    {
        _tok_progredi(ctx, I);  /* Skip final > */
    }

    token.genus                      = STML_TOKEN_DOCTYPE;
    token.positus_initium            = initium;
    token.positus_finis              = ctx->positus;
    token.linea                      = initium_linea;
    token.columna                    = initium_columna;
    token.attributa                  = NIHIL;
    token.captio_numerus             = ZEPHYRUM;
    token.habet_captus               = FALSUM;
    token.multilinea                 = FALSUM;
    token.spatia_prae_finem.datum    = NIHIL;
    token.spatia_prae_finem.mensura  = ZEPHYRUM;

    redde token;
}

/* Parse a tag <...> */
interior StmlToken
_tok_legere_tag (
    StmlTokenContext* ctx)
{
    StmlToken token;
          i32 initium;
          i32 initium_linea;
          i32 initium_columna;
       chorda titulus;
          b32 est_crudus;
          i32 captio_numerus;

    initium          = ctx->positus;
    initium_linea    = ctx->linea;
    initium_columna  = ctx->columna;

    token.attributa                  = NIHIL;
    token.captio_numerus             = ZEPHYRUM;
    token.habet_captus               = FALSUM;
    token.multilinea                 = FALSUM;
    token.spatia_prae_finem.datum    = NIHIL;
    token.spatia_prae_finem.mensura  = ZEPHYRUM;
    token.captus_contentus.datum     = NIHIL;
    token.captus_contentus.mensura   = ZEPHYRUM;
    token.multilinea                 = FALSUM;
    token.spatia_prae_finem.datum    = NIHIL;
    token.spatia_prae_finem.mensura  = ZEPHYRUM;

    /* Skip < */
    _tok_progredi(ctx, I);

    /* Check for sandwich capture <= tag => */
    si (_tok_aspicere(ctx, ZEPHYRUM) == '=')
    {
        _tok_progredi(ctx, I);  /* Skip = */
        _tok_praeterire_spatium(ctx);

        titulus = _tok_legere_nomen(ctx);
        si (_titulus_male_incipit(ctx, titulus))
        {
            token.genus            = STML_TOKEN_ERRATUM;
            token.valor            = titulus;
            token.positus_initium  = initium;
            token.positus_finis    = ctx->positus;
            token.linea            = initium_linea;
            token.columna          = initium_columna;
            redde token;
        }
        _tok_praeterire_spatium(ctx);

        /* Expect => */
        si (   _tok_aspicere(ctx, ZEPHYRUM) == '='
            && _tok_aspicere(ctx, I)        == '>')
        {
            _tok_progredi(ctx, II);
        }

        token.genus            = STML_TOKEN_FARCIMEN;
        token.valor            = titulus;
        token.positus_initium  = initium;
        token.positus_finis    = ctx->positus;
        token.linea            = initium_linea;
        token.columna          = initium_columna;
        redde token;
    }

    /* Check for backward capture <) tag > */
    si (_tok_aspicere(ctx, ZEPHYRUM) == ')')
    {
        captio_numerus = ZEPHYRUM;
        dum (_tok_aspicere(ctx, ZEPHYRUM) == ')')
        {
            captio_numerus++;
            _tok_progredi(ctx, I);
        }

        _tok_praeterire_spatium(ctx);
        titulus = _tok_legere_nomen(ctx);
        si (_titulus_male_incipit(ctx, titulus))
        {
            token.genus            = STML_TOKEN_ERRATUM;
            token.valor            = titulus;
            token.positus_initium  = initium;
            token.positus_finis    = ctx->positus;
            token.linea            = initium_linea;
            token.columna          = initium_columna;
            redde token;
        }
        _tok_praeterire_spatium(ctx);

        si (_tok_aspicere(ctx, ZEPHYRUM) == '>')
        {
            _tok_progredi(ctx, I);
        }

        token.genus            = STML_TOKEN_CAPTIO_RETRO;
        token.valor            = titulus;
        token.positus_initium  = initium;
        token.positus_finis    = ctx->positus;
        token.linea            = initium_linea;
        token.columna          = initium_columna;
        token.captio_numerus   = captio_numerus;
        redde token;
    }

    /* Check for closing tag </tag> */
    si (_tok_aspicere(ctx, ZEPHYRUM) == '/')
    {
        _tok_progredi(ctx, I);
        titulus = _tok_legere_nomen(ctx);
        si (_titulus_male_incipit(ctx, titulus))
        {
            token.genus            = STML_TOKEN_ERRATUM;
            token.valor            = titulus;
            token.positus_initium  = initium;
            token.positus_finis    = ctx->positus;
            token.linea            = initium_linea;
            token.columna          = initium_columna;
            redde token;
        }

        /* Handle ! in closing tags for raw content */
        si (_tok_aspicere(ctx, ZEPHYRUM) == '!')
        {
            _tok_progredi(ctx, I);
        }

        _tok_praeterire_spatium(ctx);

        si (_tok_aspicere(ctx, ZEPHYRUM) == '>')
        {
            _tok_progredi(ctx, I);
        }

        token.genus            = STML_TOKEN_CLAUDERE;
        token.valor            = titulus;
        token.positus_initium  = initium;
        token.positus_finis    = ctx->positus;
        token.linea            = initium_linea;
        token.columna          = initium_columna;
        redde token;
    }

    /* Regular opening tag */
    titulus = _tok_legere_nomen(ctx);
    si (_titulus_male_incipit(ctx, titulus))
    {
        token.genus            = STML_TOKEN_ERRATUM;
        token.valor            = titulus;
        token.positus_initium  = initium;
        token.positus_finis    = ctx->positus;
        token.linea            = initium_linea;
        token.columna          = initium_columna;
        redde token;
    }

    /* Check for ! suffix (raw content) */
    est_crudus = FALSUM;
    si (_tok_aspicere(ctx, ZEPHYRUM) == '!')
    {
        est_crudus = VERUM;
        _tok_progredi(ctx, I);
    }

    /* Signum '\' (multilinea, §1.4) - nomini agglutinatum, post
     * '!' si adest ('<tag\>', '<tag!\>'). Ordo inversus '\!' =
     * ERRATUM clarum: '!' lexationem ipsam mutat, ergo primus
     * ligare debet - fidelitas ordinem authoris meminisse non
     * debet. */
    si (_tok_aspicere(ctx, ZEPHYRUM) == '\\')
    {
        token.multilinea = VERUM;
        _tok_progredi(ctx, I);

        si (_tok_aspicere(ctx, ZEPHYRUM) == '!')
        {
            token.genus            = STML_TOKEN_ERRATUM;
            token.valor            = titulus;
            token.positus_initium  = initium;
            token.positus_finis    = ctx->positus;
            token.linea            = initium_linea;
            token.columna          = initium_columna;
            redde token;
        }
    }

    /* Parse attributes (spatium ante attributum primum ansa ipsa
     * notat - praeteritio hic spatium primum devoraret, §1.6) */
    {
        chorda cauda_spatii;

        token.attributa = _tok_legere_attributa(ctx, &cauda_spatii);
        token.spatia_prae_finem = cauda_spatii;
    }

    /* Check for forward capture (( */
    captio_numerus = ZEPHYRUM;
    dum (_tok_aspicere(ctx, ZEPHYRUM) == '(')
    {
        captio_numerus++;
        _tok_progredi(ctx, I);
    }

    /* Skip whitespace after capture parens (spatium '( >' non
     * modellatur - exceptio angusta nominata, §1.6) */
    _tok_praeterire_spatium(ctx);

    /* Forma capturae: spatium prae parenthesibus NOTATUR (§1.6
     * emendatum 2026-08-24) - '<tag(>' glutinata et '<tag (>'
     * spatiata ambae octetim redduntur; olim delebatur (foramen
     * fidelitatis latens). */

    /* Check for self-closing /> */
    si (   captio_numerus               == ZEPHYRUM
        && _tok_aspicere(ctx, ZEPHYRUM) == '/'
        && _tok_aspicere(ctx, I)        == '>')
    {
        _tok_progredi(ctx, II);

        token.genus            = STML_TOKEN_AUTO_CLAUDERE;
        token.valor            = titulus;
        token.positus_initium  = initium;
        token.positus_finis    = ctx->positus;
        token.linea            = initium_linea;
        token.columna          = initium_columna;
        redde token;
    }

    /* Expect > */
    si (_tok_aspicere(ctx, ZEPHYRUM) == '>')
    {
        _tok_progredi(ctx, I);
    }

    /* Determine token type */
    si (captio_numerus > ZEPHYRUM)
    {
        si (est_crudus)
        {
            /* Captura lineae crudae <tag! (>: reliquum lineae CRUDUM
             * in lexemate ipso fertur (captus_contentus) - sine tags,
             * sine entiis. '\n' flumini normali relinquitur.
             * captio_numerus > 1 notatur sed adhuc UNAM lineam capit
             * (multi-linea + dedentatio = futura). */
            i32 contentum_initium;

            token.genus           = STML_TOKEN_CRUDUS;
            token.captio_numerus  = captio_numerus;
            contentum_initium     = ctx->positus;
            dum (   ctx->positus < ctx->input.mensura
                 && _tok_aspicere(ctx, ZEPHYRUM) != '\n')
            {
                _tok_progredi(ctx, I);
            }
            token.captus_contentus.datum =
                ctx->input.datum + contentum_initium;
            token.captus_contentus.mensura =
                ctx->positus - contentum_initium;
            /* praecisio '\r' finalis olim hic - post
             * canonicalizationem CRLF in introitu (§3) '\r' ante
             * '\n' exsistere nequit; regula per-genus DELETA */
            token.habet_captus = VERUM;
        }
        alioquin
        {
            token.genus           = STML_TOKEN_CAPTIO_ANTE;
            token.captio_numerus  = captio_numerus;
        }
    }
    alioquin si (est_crudus)
    {
        token.genus = STML_TOKEN_CRUDUS;
    }
    alioquin
    {
        token.genus = STML_TOKEN_APERIRE;
    }

    token.valor            = titulus;
    token.positus_initium  = initium;
    token.positus_finis    = ctx->positus;
    token.linea            = initium_linea;
    token.columna          = initium_columna;

    redde token;
}

/* Parse fragment tag <#> or <#id> */
interior StmlToken
_tok_legere_fragmentum (
    StmlTokenContext* ctx)
{
    StmlToken token;
          i32 initium;
          i32 initium_linea;
          i32 initium_columna;
       chorda fragmentum_id;
          i32 captio_numerus;

    initium          = ctx->positus;
    initium_linea    = ctx->linea;
    initium_columna  = ctx->columna;

    token.attributa                  = NIHIL;
    token.captio_numerus             = ZEPHYRUM;
    token.habet_captus               = FALSUM;
    token.multilinea                 = FALSUM;
    token.spatia_prae_finem.datum    = NIHIL;
    token.spatia_prae_finem.mensura  = ZEPHYRUM;
    token.captus_contentus.datum     = NIHIL;
    token.captus_contentus.mensura   = ZEPHYRUM;
    token.multilinea                 = FALSUM;
    token.spatia_prae_finem.datum    = NIHIL;
    token.spatia_prae_finem.mensura  = ZEPHYRUM;

    /* Skip < */
    _tok_progredi(ctx, I);

    /* Skip # */
    _tok_progredi(ctx, I);

    /* Read optional fragment ID */
    fragmentum_id = _tok_legere_fragmentum_id(ctx);

    /* Parse attributes (spatium primum ansa notat, §1.6) */
    {
        chorda cauda_spatii;

        token.attributa = _tok_legere_attributa(ctx, &cauda_spatii);
        token.spatia_prae_finem = cauda_spatii;
    }

    /* Numerare parentheses capturae <# (> / <#id (> (post
     * attributa, sicut in tags normalibus). Spatium prae
     * parenthesibus notatur (§1.6 emendatum); spatium '( >' non
     * modellatur. */
    captio_numerus = ZEPHYRUM;
    dum (_tok_aspicere(ctx, ZEPHYRUM) == '(')
    {
        captio_numerus++;
        _tok_progredi(ctx, I);
    }
    _tok_praeterire_spatium(ctx);

    /* Check for self-closing <#/> or <#id/> */
    si (   captio_numerus               == ZEPHYRUM
        && _tok_aspicere(ctx, ZEPHYRUM) == '/'
        && _tok_aspicere(ctx, I)        == '>')
    {
        _tok_progredi(ctx, II);

        token.genus            = STML_TOKEN_FRAGMENTUM_AUTO;
        token.valor            = fragmentum_id;
        token.positus_initium  = initium;
        token.positus_finis    = ctx->positus;
        token.linea            = initium_linea;
        token.columna          = initium_columna;
        redde token;
    }

    /* Expect > */
    si (_tok_aspicere(ctx, ZEPHYRUM) == '>')
    {
        _tok_progredi(ctx, I);
    }

    /* Fragmentum capturans = lexema se ipso continens (idem
     * exemplar quo CRUDUS: genus AUTO, campus captio_numerus
     * discriminat) - fratres post parsationem capiuntur */
    si (captio_numerus > ZEPHYRUM)
    {
        token.genus           = STML_TOKEN_FRAGMENTUM_AUTO;
        token.captio_numerus  = captio_numerus;
    }
    alioquin
    {
        token.genus = STML_TOKEN_FRAGMENTUM_APERIRE;
    }
    token.valor            = fragmentum_id;
    token.positus_initium  = initium;
    token.positus_finis    = ctx->positus;
    token.linea            = initium_linea;
    token.columna          = initium_columna;
    redde token;
}

/* Parse fragment closing tag </#> */
interior StmlToken
_tok_legere_fragmentum_claudere (
    StmlTokenContext* ctx)
{
    StmlToken token;
          i32 initium;
          i32 initium_linea;
          i32 initium_columna;

    initium          = ctx->positus;
    initium_linea    = ctx->linea;
    initium_columna  = ctx->columna;

    token.attributa                  = NIHIL;
    token.captio_numerus             = ZEPHYRUM;
    token.habet_captus               = FALSUM;
    token.multilinea                 = FALSUM;
    token.spatia_prae_finem.datum    = NIHIL;
    token.spatia_prae_finem.mensura  = ZEPHYRUM;
    token.captus_contentus.datum     = NIHIL;
    token.captus_contentus.mensura   = ZEPHYRUM;

    /* Skip </ */
    _tok_progredi(ctx, II);

    /* Skip # */
    _tok_progredi(ctx, I);

    _tok_praeterire_spatium(ctx);

    /* Expect > */
    si (_tok_aspicere(ctx, ZEPHYRUM) == '>')
    {
        _tok_progredi(ctx, I);
    }

    token.genus            = STML_TOKEN_FRAGMENTUM_CLAUDERE;
    token.valor.datum      = NIHIL;
    token.valor.mensura    = ZEPHYRUM;
    token.positus_initium  = initium;
    token.positus_finis    = ctx->positus;
    token.linea            = initium_linea;
    token.columna          = initium_columna;
    redde token;
}

/* Parse augmentation tag <% &clavis;> - clavis destinata sola,
 * sine attributis (verbum in signo latet: unum verbum legale,
 * appositio). Stricte a nativitate: clavis sigillata OBLIGATORIA
 * ('<%>', '<% laika>', '<% .canis>', '<% &;>' = ERRATUM), post
 * clavem spatium tum '>' solum. Clavis VERBATIM cum sigillis
 * servatur ('&c;' - eaedem litterae quibus citationes canonis
 * comparantur). */
interior StmlToken
_tok_legere_percentum (
    StmlTokenContext* ctx)
{
    StmlToken token;
          i32 initium;
          i32 initium_linea;
          i32 initium_columna;
          i32 clavis_initium;
          b32 sana;

    initium          = ctx->positus;
    initium_linea    = ctx->linea;
    initium_columna  = ctx->columna;

    token.attributa                  = NIHIL;
    token.captio_numerus             = ZEPHYRUM;
    token.habet_captus               = FALSUM;
    token.multilinea                 = FALSUM;
    token.spatia_prae_finem.datum    = NIHIL;
    token.spatia_prae_finem.mensura  = ZEPHYRUM;
    token.captus_contentus.datum     = NIHIL;
    token.captus_contentus.mensura   = ZEPHYRUM;
    token.valor.datum                = NIHIL;
    token.valor.mensura              = ZEPHYRUM;

    /* Skip <% */
    _tok_progredi(ctx, II);
    _tok_praeterire_spatium(ctx);

    sana            = FALSUM;
    clavis_initium  = ctx->positus;
    si (_tok_aspicere(ctx, ZEPHYRUM) == '&')
    {
        _tok_progredi(ctx, I);
        dum (   ctx->positus < ctx->input.mensura
             && _est_nomen_character(_tok_aspicere(ctx, ZEPHYRUM)))
        {
            _tok_progredi(ctx, I);
        }
        /* ';' post nomen non vacuum ('&;' reicitur) */
        si (   _tok_aspicere(ctx, ZEPHYRUM) == ';'
            && ctx->positus - clavis_initium > I)
        {
            _tok_progredi(ctx, I);
            token.valor.datum    = ctx->input.datum + clavis_initium;
            token.valor.mensura  = ctx->positus - clavis_initium;
            _tok_praeterire_spatium(ctx);
            si (_tok_aspicere(ctx, ZEPHYRUM) == '>')
            {
                _tok_progredi(ctx, I);
                sana = VERUM;
            }
        }
    }

    si (sana)
    {
        token.genus = STML_TOKEN_PERCENTUM_APERIRE;
    }
    alioquin
    {
        token.genus = STML_TOKEN_ERRATUM;
    }
    token.positus_initium  = initium;
    token.positus_finis    = ctx->positus;
    token.linea            = initium_linea;
    token.columna          = initium_columna;
    redde token;
}

/* Parse augmentation closing tag </%> - stricte ('</%x>' =
 * ERRATUM) */
interior StmlToken
_tok_legere_percentum_claudere (
    StmlTokenContext* ctx)
{
    StmlToken token;
          i32 initium;
          i32 initium_linea;
          i32 initium_columna;

    initium          = ctx->positus;
    initium_linea    = ctx->linea;
    initium_columna  = ctx->columna;

    token.attributa                  = NIHIL;
    token.captio_numerus             = ZEPHYRUM;
    token.habet_captus               = FALSUM;
    token.multilinea                 = FALSUM;
    token.spatia_prae_finem.datum    = NIHIL;
    token.spatia_prae_finem.mensura  = ZEPHYRUM;
    token.captus_contentus.datum     = NIHIL;
    token.captus_contentus.mensura   = ZEPHYRUM;
    token.valor.datum                = NIHIL;
    token.valor.mensura              = ZEPHYRUM;

    /* Skip </% */
    _tok_progredi(ctx, III);
    _tok_praeterire_spatium(ctx);

    si (_tok_aspicere(ctx, ZEPHYRUM) == '>')
    {
        _tok_progredi(ctx, I);
        token.genus = STML_TOKEN_PERCENTUM_CLAUDERE;
    }
    alioquin
    {
        token.genus = STML_TOKEN_ERRATUM;
    }
    token.positus_initium  = initium;
    token.positus_finis    = ctx->positus;
    token.linea            = initium_linea;
    token.columna          = initium_columna;
    redde token;
}

/* Parse bare capture sugar <(> or <((> - anonymous fragment.
 * Saccharum authoris: scriptor ad <# (> normalizat (forma nuda
 * ephemera est - stampatio gestarum lineam rescribit). */
interior StmlToken
_tok_legere_captio_nuda (
    StmlTokenContext* ctx)
{
    StmlToken token;
          i32 initium;
          i32 initium_linea;
          i32 initium_columna;
          i32 captio_numerus;

    initium          = ctx->positus;
    initium_linea    = ctx->linea;
    initium_columna  = ctx->columna;

    token.attributa                  = NIHIL;
    token.captio_numerus             = ZEPHYRUM;
    token.habet_captus               = FALSUM;
    token.multilinea                 = FALSUM;
    token.spatia_prae_finem.datum    = NIHIL;
    token.spatia_prae_finem.mensura  = ZEPHYRUM;
    token.captus_contentus.datum     = NIHIL;
    token.captus_contentus.mensura   = ZEPHYRUM;

    /* Skip < */
    _tok_progredi(ctx, I);

    captio_numerus = ZEPHYRUM;
    dum (_tok_aspicere(ctx, ZEPHYRUM) == '(')
    {
        captio_numerus++;
        _tok_progredi(ctx, I);
    }
    _tok_praeterire_spatium(ctx);

    /* Expect > */
    si (_tok_aspicere(ctx, ZEPHYRUM) == '>')
    {
        _tok_progredi(ctx, I);
    }

    token.genus            = STML_TOKEN_FRAGMENTUM_AUTO;
    token.captio_numerus   = captio_numerus;
    token.valor.datum      = NIHIL;
    token.valor.mensura    = ZEPHYRUM;
    token.positus_initium  = initium;
    token.positus_finis    = ctx->positus;
    token.linea            = initium_linea;
    token.columna          = initium_columna;
    redde token;
}

/* Parse transclusion <<selector>> */
interior StmlToken
_tok_legere_transclusio (
    StmlTokenContext* ctx)
{
    StmlToken token;
          i32 initium;
          i32 initium_linea;
          i32 initium_columna;
          i32 selector_initium;
          i32 selector_finis;

    initium          = ctx->positus;
    initium_linea    = ctx->linea;
    initium_columna  = ctx->columna;

    token.attributa                  = NIHIL;
    token.captio_numerus             = ZEPHYRUM;
    token.habet_captus               = FALSUM;
    token.multilinea                 = FALSUM;
    token.spatia_prae_finem.datum    = NIHIL;
    token.spatia_prae_finem.mensura  = ZEPHYRUM;
    token.captus_contentus.datum     = NIHIL;
    token.captus_contentus.mensura   = ZEPHYRUM;

    /* Skip << */
    _tok_progredi(ctx, II);

    /* Skip leading whitespace */
    _tok_praeterire_spatium(ctx);
    selector_initium = ctx->positus;

    /* Ad '>>' legere - citationum-conscius (macros v1): '>>' intra
     * valorem citatum argumenti ('<<#f a="x>>y">>') lexema non
     * claudit. Citatio non clausa = scansio ad finem (lexema tunc
     * non clauditur - lenitas exsistens intacta). */
    dum (ctx->positus < ctx->input.mensura)
    {
        character citatio;

        citatio = _tok_aspicere(ctx, ZEPHYRUM);
        si (citatio == '"' || citatio == '\'')
        {
            _tok_progredi(ctx, I);
            dum (   ctx->positus < ctx->input.mensura
                 && _tok_aspicere(ctx, ZEPHYRUM) != citatio)
            {
                _tok_progredi(ctx, I);
            }
            si (ctx->positus < ctx->input.mensura)
            {
                _tok_progredi(ctx, I);
            }
            perge;
        }
        si (   citatio               == '>'
            && _tok_aspicere(ctx, I) == '>')
        {
            frange;
        }
        _tok_progredi(ctx, I);
    }

    selector_finis = ctx->positus;

    /* Trim trailing whitespace from selector */
    dum (   selector_finis > selector_initium
         && _est_spatium((character)ctx->input.datum[selector_finis - I]))
    {
        selector_finis--;
    }

    /* Skip >> */
    si (   _tok_aspicere(ctx, ZEPHYRUM) == '>'
        && _tok_aspicere(ctx, I)        == '>')
    {
        _tok_progredi(ctx, II);
    }

    token.genus            = STML_TOKEN_TRANSCLUSIO;
    token.valor.datum      = ctx->input.datum + selector_initium;
    token.valor.mensura    = selector_finis - selector_initium;
    token.positus_initium  = initium;
    token.positus_finis    = ctx->positus;
    token.linea            = initium_linea;
    token.columna          = initium_columna;
    redde token;
}

/* Parse raw content until </tagname> */
interior StmlToken
_tok_legere_contentus_crudus (
    StmlTokenContext* ctx,
              chorda  titulus)
{
    StmlToken token;
          i32 initium;
          i32 initium_linea;
          i32 initium_columna;
          i32 i;
          b32 inventum;

    initium          = ctx->positus;
    initium_linea    = ctx->linea;
    initium_columna  = ctx->columna;

    /* Search for </tagname> */
    inventum = FALSUM;
    dum (ctx->positus < ctx->input.mensura && !inventum)
    {
        si (   _tok_aspicere(ctx, ZEPHYRUM) == '<'
            && _tok_aspicere(ctx, I)        == '/')
        {
            /* Check if this is our closing tag */
            b32 aequalis;
            aequalis = VERUM;

            per (i = ZEPHYRUM; i < titulus.mensura; i++)
            {
                si (_tok_aspicere(ctx, II + i) != (character)titulus.datum[i])
                {
                    aequalis = FALSUM;
                    frange;
                }
            }

            si (   aequalis
                && (_tok_aspicere(ctx, II + titulus.mensura) == '>'
                || _tok_aspicere(ctx, II + titulus.mensura) == '!'
                || _est_spatium(_tok_aspicere(ctx, II + titulus.mensura))))
            {
                inventum = VERUM;
                frange;
            }
        }
        _tok_progredi(ctx, I);
    }

    token.genus                      = STML_TOKEN_TEXTUS;
    token.valor.datum                = ctx->input.datum + initium;
    token.valor.mensura              = ctx->positus - initium;
    token.positus_initium            = initium;
    token.positus_finis              = ctx->positus;
    token.linea                      = initium_linea;
    token.columna                    = initium_columna;
    token.attributa                  = NIHIL;
    token.captio_numerus             = ZEPHYRUM;
    token.habet_captus               = FALSUM;
    token.multilinea                 = FALSUM;
    token.spatia_prae_finem.datum    = NIHIL;
    token.spatia_prae_finem.mensura  = ZEPHYRUM;

    redde token;
}

/* Parse text content */
interior StmlToken
_tok_legere_textus (
    StmlTokenContext* ctx)
{
    StmlToken token;
          i32 initium;
          i32 initium_linea;
          i32 initium_columna;

    initium          = ctx->positus;
    initium_linea    = ctx->linea;
    initium_columna  = ctx->columna;

    dum (   ctx->positus < ctx->input.mensura
         && _tok_aspicere(ctx, ZEPHYRUM) != '<')
    {
        _tok_progredi(ctx, I);
    }

    token.genus                      = STML_TOKEN_TEXTUS;
    token.valor.datum                = ctx->input.datum + initium;
    token.valor.mensura              = ctx->positus - initium;
    token.positus_initium            = initium;
    token.positus_finis              = ctx->positus;
    token.linea                      = initium_linea;
    token.columna                    = initium_columna;
    token.attributa                  = NIHIL;
    token.captio_numerus             = ZEPHYRUM;
    token.habet_captus               = FALSUM;
    token.multilinea                 = FALSUM;
    token.spatia_prae_finem.datum    = NIHIL;
    token.spatia_prae_finem.mensura  = ZEPHYRUM;

    redde token;
}

/* Get next token */
interior StmlToken
_tok_proximus (
    StmlTokenContext* ctx)
{
    StmlToken token;

    /* If in raw content mode, get raw content until close tag */
    si (ctx->in_crudus)
    {
        /* Check if we're at the closing tag */
        si (   _tok_aspicere(ctx, ZEPHYRUM) == '<'
            && _tok_aspicere(ctx, I)        == '/')
        {
            /* Check if this is our closing tag */
            b32 aequalis;
            i32 i;
            aequalis = VERUM;

            per (i = ZEPHYRUM; i < ctx->crudus_titulus.mensura; i++)
            {
                si (_tok_aspicere(ctx, II + i) != (character)ctx->crudus_titulus.datum[i])
                {
                    aequalis = FALSUM;
                    frange;
                }
            }

            si (aequalis)
            {
                /* Exit raw mode and parse close tag */
                ctx->in_crudus = FALSUM;
                redde _tok_legere_tag(ctx);
            }
        }

        /* Still in raw mode - get content */
        redde _tok_legere_contentus_crudus(ctx, ctx->crudus_titulus);
    }

    /* EOF check */
    si (ctx->positus >= ctx->input.mensura)
    {
        token.genus                      = STML_TOKEN_FINIS;
        token.valor.datum                = NIHIL;
        token.valor.mensura              = ZEPHYRUM;
        token.positus_initium            = ctx->positus;
        token.positus_finis              = ctx->positus;
        token.linea                      = ctx->linea;
        token.columna                    = ctx->columna;
        token.attributa                  = NIHIL;
        token.captio_numerus             = ZEPHYRUM;
        token.habet_captus               = FALSUM;
        token.multilinea                 = FALSUM;
        token.spatia_prae_finem.datum    = NIHIL;
        token.spatia_prae_finem.mensura  = ZEPHYRUM;
        redde token;
    }

    si (_tok_aspicere(ctx, ZEPHYRUM) == '<')
    {
        /* Check for transclusion <<selector>> */
        si (_tok_aspicere(ctx, I) == '<')
        {
            redde _tok_legere_transclusio(ctx);
        }

        /* Check for comment */
        si (   _tok_aspicere(ctx, I)   == '!'
            && _tok_aspicere(ctx, II)  == '-'
            && _tok_aspicere(ctx, III) == '-')
        {
            redde _tok_legere_commentum(ctx);
        }

        /* Check for processing instruction */
        si (_tok_aspicere(ctx, I) == '?')
        {
            redde _tok_legere_processio(ctx);
        }

        /* Check for DOCTYPE */
        si (   _tok_aspicere(ctx, I)  == '!'
            && _tok_aspicere(ctx, II) == 'D')
        {
            redde _tok_legere_doctype(ctx);
        }

        /* Check for fragment closing tag </#> */
        si (   _tok_aspicere(ctx, I)  == '/'
            && _tok_aspicere(ctx, II) == '#')
        {
            redde _tok_legere_fragmentum_claudere(ctx);
        }

        /* Check for augmentation closing tag </%> */
        si (   _tok_aspicere(ctx, I)  == '/'
            && _tok_aspicere(ctx, II) == '%')
        {
            redde _tok_legere_percentum_claudere(ctx);
        }

        /* Check for fragment opening tag <#> or <#id> */
        si (_tok_aspicere(ctx, I) == '#')
        {
            redde _tok_legere_fragmentum(ctx);
        }

        /* Check for augmentation opening tag <% &clavis;> */
        si (_tok_aspicere(ctx, I) == '%')
        {
            redde _tok_legere_percentum(ctx);
        }

        /* Saccharum capturae nudae <(> - fragmentum anonymum */
        si (_tok_aspicere(ctx, I) == '(')
        {
            redde _tok_legere_captio_nuda(ctx);
        }

        /* Regular tag */
        token = _tok_legere_tag(ctx);

        /* Check if entering raw content mode */
        si (token.genus == STML_TOKEN_CRUDUS && token.captio_numerus == ZEPHYRUM)
        {
            ctx->in_crudus       = VERUM;
            ctx->crudus_titulus  = token.valor;
        }

        redde token;
    }

    /* Text content */
    redde _tok_legere_textus(ctx);
}


/* ==================================================
 * Canonicalizatio CRLF (spec triviae §3)
 * ==================================================
 *
 * UNA transformatio, in INTROITU solo: "\r\n" -> "\n" super
 * documentum totum, ANTE lexationem. Contractus fidelitatis super
 * octetos canonicalizatos definitur - regulae CRLF per-genus infra
 * NUSQUAM exsistunt ('\r' solivagum contentum manet). Sine '\r\n'
 * input INTACTUM redditur (nulla copia, nulla allocatio).
 */

interior chorda
_crlf_canonicalizare (
     chorda  input,
    Piscina* piscina,
        b32* factum)
{
    chorda exitus;
       i32 i;
       i32 scriptum;
       b32 inventum;

    *factum = FALSUM;

    inventum = FALSUM;
    per (i = ZEPHYRUM; i + I < input.mensura; i++)
    {
        si (   input.datum[i]     == (i8)'\r'
            && input.datum[i + I] == (i8)'\n')
        {
            inventum = VERUM;
            frange;
        }
    }
    si (!inventum)
    {
        redde input;
    }

    exitus.datum = (i8*)piscina_allocare(piscina,
        (memoriae_index)input.mensura);
    si (exitus.datum == NIHIL)
    {
        redde input;
    }

    scriptum = ZEPHYRUM;
    per (i = ZEPHYRUM; i < input.mensura; i++)
    {
        si (   input.datum[i]     == (i8)'\r'
            && i + I < input.mensura
            && input.datum[i + I] == (i8)'\n')
        {
            perge;  /* '\r' ante '\n' cadit; '\n' ipse sequetur */
        }
        exitus.datum[scriptum] = input.datum[i];
        scriptum++;
    }
    exitus.mensura = scriptum;

    *factum = VERUM;
    redde exitus;
}


/* ==================================================
 * Lexemata - fluxus publicus (sine arbore)
 * ==================================================
 *
 * Modum CRUDUM ('<tag!') tokenizator IPSE regit (_tok_proximus eum
 * ponit cum tag crudum legit, et exit cum clausuram congruentem
 * videt) - ergo gyrus nudus tokenum eum recte tractat, parsatore
 * non opus. Id ipsum hanc superficiem possibilem facit.
 */

Xar*
stml_lexemata_colligere (
                 chorda  input,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    StmlTokenContext  ctx;
                 Xar* exitus;
                 b32  crlf_factum;

    si (   input.datum == NIHIL || input.mensura <= ZEPHYRUM
        || piscina     == NIHIL || intern == NIHIL)
    {
        redde NIHIL;
    }

    /* fluxus super octetos CANONICALIZATOS - extensiones lexematum
     * ad formam LF referuntur, eandem quam arbor videt (§3) */
    input = _crlf_canonicalizare(input, piscina, &crlf_factum);

    ctx.input                   = input;
    ctx.positus                 = ZEPHYRUM;
    ctx.linea                   = I;
    ctx.columna                 = I;
    ctx.in_crudus               = FALSUM;
    ctx.crudus_titulus.datum    = NIHIL;
    ctx.crudus_titulus.mensura  = ZEPHYRUM;
    ctx.piscina                 = piscina;
    ctx.intern                  = intern;

    exitus = xar_creare(piscina, (i32)magnitudo(StmlLexema));
    si (exitus == NIHIL)
    {
        redde NIHIL;
    }

    per (;;)
    {
          StmlToken  token;
         StmlLexema* sedes;
                i32  ante;

        ante   = ctx.positus;
        token  = _tok_proximus(&ctx);
        si (token.genus == STML_TOKEN_FINIS)
        {
            frange;
        }

        sedes = (StmlLexema*)xar_addere(exitus);
        si (sedes == NIHIL)
        {
            redde NIHIL;
        }
        sedes->genus    = token.genus;
        sedes->valor    = token.valor;
        sedes->initium  = token.positus_initium;
        sedes->finis    = token.positus_finis;
        sedes->linea    = token.linea;
        sedes->columna  = token.columna;

        /* CUSTOS PROGRESSUS: lexema quod positum non promovet
         * gyrum infinitum faceret. Comparatio POST vocationem
         * contra positum ANTE captum fit - non contra sentinellam
         * negativam, quia 'i32' INSIGNATUS est (latina.h) et '-I'
         * in 0xFFFFFFFF verteretur, unde custos semper verus et
         * fluxus semper vacuus - ramus qui tacite numquam currit. */
        si (ctx.positus <= ante)
        {
            frange;
        }
    }

    redde exitus;
}


/* ==================================================
 * Parser
 * ================================================== */

nomen structura {
       StmlTokenContext  tok_ctx;
              StmlToken  current;
                Piscina* piscina;
    InternamentumChorda* intern;

    /* Finis octetorum tokeni ULTIMO CONSUMPTI.
     *
     * SUTURA UNA pro extensionibus nodorum: quisque parsator
     * nodi, cum redit, tokenum ultimum suum iam consumpsit -
     * ergo 'nodus->positus_finis = ctx->finis_ultimus' uno more
     * omnibus generibus nodorum valet (elementum cum clausura,
     * elementum crudum, fragmentum, textus, commentum...).
     * Alternativa - finem ad quemque locum clausurae manu capere
     * - eandem rem sexies derivaret, unde sex occasiones
     * divergendi. */
    i32 finis_ultimus;

    /* Error info */
    StmlStatus status;
           i32 linea_erroris;
           i32 columna_erroris;
        chorda error;
} StmlParserContext;

interior vacuum
_parser_progredi (
    StmlParserContext* ctx)
{
    /* tokenum EXEUNTEM notare, antequam obruatur */
    ctx->finis_ultimus  = ctx->current.positus_finis;
    ctx->current        = _tok_proximus(&ctx->tok_ctx);
}

/* Errorem PRIMUM servare (2026-08-24): errores sequentes saepe
 * CASCATA prioris sunt (recusatio nodum negat -> clausura orba
 * TAG_IMPROPRIE superscribebat causam veram). Primus nominat,
 * ceteri tacent. */
interior vacuum
_errorem_ponere (
    StmlParserContext* ctx,
           StmlStatus  status,
                  i32  linea,
                  i32  columna)
{
    si (ctx->status != STML_SUCCESSUS)
    {
        redde;
    }
    ctx->status           = status;
    ctx->linea_erroris    = linea;
    ctx->columna_erroris  = columna;
}

interior StmlNodus*
_parser_creare_nodus (
    StmlParserContext* ctx,
       StmlNodusGenus  genus)
{
    StmlNodus* nodus;

    nodus = (StmlNodus*)piscina_allocare(ctx->piscina, magnitudo(StmlNodus));
    si (!nodus)
    {
        redde NIHIL;
    }

    nodus->genus             = genus;
    nodus->titulus           = NIHIL;
    nodus->valor             = NIHIL;
    nodus->attributa         = NIHIL;
    nodus->liberi            = NIHIL;
    nodus->parens            = NIHIL;
    nodus->crudus            = FALSUM;
    nodus->captio_directio   = STML_CAPTIO_NIHIL;
    nodus->captio_numerus    = ZEPHYRUM;
    nodus->clausura_anonyma  = FALSUM;
    /* olim fragmentum/fragmentum_id hic NON initiabantur - piscina
     * recens zephyrata eos texit, sed piscina reusa non spondet.
     * Omnes campi explicite (2026-08-10). */
    nodus->fragmentum        = FALSUM;
    nodus->fragmentum_id     = NIHIL;
    nodus->augmentum_clavis  = NIHIL;

    /* trivia (§1): nulla nativitate - distributio in _liberos_legere
     * et classificatio in _textum_tractare eas ponunt */
    nodus->spatia_ante         = NIHIL;
    nodus->spatia_post         = NIHIL;
    nodus->spatia_clausurae    = NIHIL;
    nodus->spatia_intra_tagum  = NIHIL;
    nodus->multilinea          = FALSUM;
    nodus->indentatio          = NIHIL;

    /* nodus nascitur dum token aperiens CURRENS est - linea eius
     * est linea nodi (tokenizator lineas iam numerat, 1-basatas) */
    nodus->linea = ctx->current.linea;

    /* EXTENSIO: initium ex tokeno aperiente; finis PRAEFINITUS
     * eiusdem tokeni finis est, quod nodis uno tokeno constantibus
     * (textus, commentum, transclusio) IAM rectum est. Nodi qui
     * liberos et clausuram habent eum in reditu suo corrigunt ex
     * 'finis_ultimus'. Ergo campus numquam ZEPHYRUM manet per
     * oblivionem - praefinitum SEMPER sanum est. */
    nodus->positus_initium  = ctx->current.positus_initium;
    nodus->positus_finis    = ctx->current.positus_finis;

    redde nodus;
}

/* Forward declaration */
interior StmlNodus*
_parser_legere_nodus (
    StmlParserContext* ctx);

/* Ansa liberorum UNA pro omnibus parentibus (elementum, fragmentum,
 * percentum, documentum) - distributio triviae §1.2 hic vivit, uno
 * loco. Definitio post adiutores textus infra. */
interior vacuum
_liberos_legere (
     StmlParserContext* ctx,
             StmlNodus* parens,
        StmlTokenGenus  terminator);

/* Transformatio '<tag\>' (§1.4) post ansam liberorum et recusatio
 * nominata - definitiones infra apud adiutores textus. */
interior vacuum
_multilineam_transformare (
    StmlParserContext* ctx,
            StmlNodus* nodus);

/* prototypa (definitiones infra - sectiones capturae/scriptionis) */
interior StmlNodus*
_xar_liberum_obtinere (
    Xar* xar,
    i32  index);

interior vacuum
_valorem_praefixo_scribere (
    ChordaAedificator* aed,
               chorda  valor,
      constans chorda* praefixum,
                  b32  evadendum,
                  b32  prima_quoque);

interior vacuum
_multilineam_recusare (
     StmlParserContext* ctx,
    constans character* causa,
                   i32  linea);

/* Parse an element */
interior StmlNodus*
_parser_legere_elementum (
    StmlParserContext* ctx)
{
    StmlNodus* nodus;
       chorda  titulus;
       chorda* titulus_ptr;

    nodus = _parser_creare_nodus(ctx, STML_NODUS_ELEMENTUM);
    si (!nodus) redde NIHIL;

    titulus            = ctx->current.valor;
    titulus_ptr        = chorda_internare(ctx->intern, titulus);
    nodus->titulus     = titulus_ptr;
    nodus->attributa   = ctx->current.attributa;
    nodus->multilinea  = ctx->current.multilinea;
    si (ctx->current.spatia_prae_finem.mensura > ZEPHYRUM)
    {
        nodus->spatia_intra_tagum = chorda_internare(ctx->intern,
            ctx->current.spatia_prae_finem);
    }

    nodus->liberi = xar_creare(ctx->piscina, magnitudo(StmlNodus*));

    _parser_progredi(ctx);  /* Consume opening tag */

    /* Parse children until close tag */
    _liberos_legere(ctx, nodus, STML_TOKEN_CLAUDERE);

    /* Verify close tag matches */
    si (ctx->current.genus == STML_TOKEN_CLAUDERE)
    {
        si (ctx->current.valor.mensura == ZEPHYRUM)
        {
            /* clausura anonyma </>: elementum apertum proximum
             * claudit (recursio ipsa 'proximum' dat - elementum
             * currens primum eam videt). Forma authoris in
             * scriptore servatur. 01KYSPRF9R */
            nodus->clausura_anonyma = VERUM;
        }
        alioquin si (!chorda_aequalis(ctx->current.valor,
                     *titulus_ptr))
        {
            _errorem_ponere(ctx, STML_ERROR_TAG_IMPROPRIE,
                            ctx->current.linea, ctx->current.columna);
        }
        _parser_progredi(ctx);  /* Consume close tag */
    }
    alioquin si (ctx->current.genus == STML_TOKEN_FINIS)
    {
        /* Unclosed tag at EOF */
        _errorem_ponere(ctx, STML_ERROR_TAG_NON_CLAUSUM,
                        ctx->current.linea, ctx->current.columna);
    }

    /* extensio: finis = tokenum ultimum consumptum */
    nodus->positus_finis = ctx->finis_ultimus;

    si (nodus->multilinea)
    {
        _multilineam_transformare(ctx, nodus);
    }

    redde nodus;
}

/* Parse a raw content element */
interior StmlNodus*
_parser_legere_elementum_crudus (
    StmlParserContext* ctx)
{
    StmlNodus* nodus;
    StmlNodus* textus_nodus;
       chorda  titulus;
       chorda* titulus_ptr;

    /* recusatio composita (§1.4): captura in '<tag!\>' vetita -
     * forma '<tag!\>' ipsa capturam lineae crudae supersedet */
    si (   ctx->current.multilinea
        && ctx->current.captio_numerus > ZEPHYRUM)
    {
        _multilineam_recusare(ctx,
            "captura in elemento multilineae ('<tag!\\ (>') vetita",
            ctx->current.linea);
        _parser_progredi(ctx);
        redde NIHIL;
    }

    nodus = _parser_creare_nodus(ctx, STML_NODUS_ELEMENTUM);
    si (!nodus) redde NIHIL;

    titulus            = ctx->current.valor;
    titulus_ptr        = chorda_internare(ctx->intern, titulus);
    nodus->titulus     = titulus_ptr;
    nodus->attributa   = ctx->current.attributa;
    nodus->crudus      = VERUM;
    nodus->multilinea  = ctx->current.multilinea;
    si (ctx->current.spatia_prae_finem.mensura > ZEPHYRUM)
    {
        nodus->spatia_intra_tagum = chorda_internare(ctx->intern,
            ctx->current.spatia_prae_finem);
    }
    nodus->captio_numerus  = ctx->current.captio_numerus;

    si (ctx->current.captio_numerus > ZEPHYRUM)
    {
        nodus->captio_directio = STML_CAPTIO_ANTE;
    }

    nodus->liberi = xar_creare(ctx->piscina, magnitudo(StmlNodus*));

    /* Captura lineae crudae: contentum in lexemate ipso venit
     * (captus_contentus, CRUDUM - sine unescape); elementum captum
     * tag clausum proprium NON habet - CLAUDERE sequens parentis
     * est, non tangendum (vitium pristinum: </parentis> devorabat) */
    si (nodus->captio_numerus > ZEPHYRUM)
    {
        si (   ctx->current.habet_captus
            && ctx->current.captus_contentus.mensura > ZEPHYRUM)
        {
            textus_nodus = _parser_creare_nodus(ctx,
                STML_NODUS_TEXTUS);
            si (textus_nodus)
            {
                StmlNodus** slot;
                textus_nodus->valor = chorda_internare(ctx->intern,
                    ctx->current.captus_contentus);
                textus_nodus->parens  = nodus;
                slot                  = xar_addere(nodus->liberi);
                si (slot) *slot = textus_nodus;
            }
        }
        _parser_progredi(ctx);
        redde nodus;
    }

    _parser_progredi(ctx);  /* Consume raw opening tag */

    /* For raw tags, next token should be text content */
    si (ctx->current.genus == STML_TOKEN_TEXTUS)
    {
        si (ctx->current.valor.mensura > ZEPHYRUM)
        {
            textus_nodus = _parser_creare_nodus(ctx, STML_NODUS_TEXTUS);
            si (textus_nodus)
            {
                StmlNodus** slot;
                textus_nodus->valor = chorda_internare(
                    ctx->intern, ctx->current.valor);
                textus_nodus->parens  = nodus;
                slot                  = xar_addere(nodus->liberi);
                si (slot) *slot = textus_nodus;
            }
        }
        _parser_progredi(ctx);
    }

    /* Expect close tag */
    si (ctx->current.genus == STML_TOKEN_CLAUDERE)
    {
        _parser_progredi(ctx);
    }

    /* extensio: finis = tokenum ultimum consumptum */
    nodus->positus_finis = ctx->finis_ultimus;

    si (nodus->multilinea)
    {
        _multilineam_transformare(ctx, nodus);
    }

    redde nodus;
}

/* Parse a self-closing element */
interior StmlNodus*
_parser_legere_auto_claudere (
    StmlParserContext* ctx)
{
    StmlNodus* nodus;
       chorda* titulus_ptr;

    nodus = _parser_creare_nodus(ctx, STML_NODUS_ELEMENTUM);
    si (!nodus) redde NIHIL;

    titulus_ptr = chorda_internare(ctx->intern, ctx->current.valor);
    nodus->titulus = titulus_ptr;
    nodus->attributa = ctx->current.attributa;
    nodus->multilinea = ctx->current.multilinea;
    si (ctx->current.spatia_prae_finem.mensura > ZEPHYRUM)
    {
        nodus->spatia_intra_tagum = chorda_internare(ctx->intern,
            ctx->current.spatia_prae_finem);
    }
    nodus->liberi = xar_creare(ctx->piscina, magnitudo(StmlNodus*));

    _parser_progredi(ctx);

    /* extensio: finis = tokenum ultimum consumptum */
    nodus->positus_finis = ctx->finis_ultimus;

    redde nodus;
}

/* Parse forward capture element */
interior StmlNodus*
_parser_legere_captio_ante (
    StmlParserContext* ctx)
{
    StmlNodus* nodus;
       chorda* titulus_ptr;

    /* recusatio composita (§1.4): captura in '<tag\ (>' vetita */
    si (ctx->current.multilinea)
    {
        _multilineam_recusare(ctx,
            "captura in elemento multilineae ('<tag\\ (>') vetita",
            ctx->current.linea);
        _parser_progredi(ctx);
        redde NIHIL;
    }

    nodus = _parser_creare_nodus(ctx, STML_NODUS_ELEMENTUM);
    si (!nodus) redde NIHIL;

    titulus_ptr = chorda_internare(ctx->intern, ctx->current.valor);
    nodus->titulus = titulus_ptr;
    nodus->attributa = ctx->current.attributa;
    si (ctx->current.spatia_prae_finem.mensura > ZEPHYRUM)
    {
        nodus->spatia_intra_tagum = chorda_internare(ctx->intern,
            ctx->current.spatia_prae_finem);
    }
    nodus->captio_directio = STML_CAPTIO_ANTE;
    nodus->captio_numerus = ctx->current.captio_numerus;
    nodus->liberi = xar_creare(ctx->piscina, magnitudo(StmlNodus*));

    _parser_progredi(ctx);

    /* extensio: finis = tokenum ultimum consumptum */
    nodus->positus_finis = ctx->finis_ultimus;

    redde nodus;
}

/* Parse backward capture element */
interior StmlNodus*
_parser_legere_captio_retro (
    StmlParserContext* ctx)
{
    StmlNodus* nodus;
       chorda* titulus_ptr;

    nodus = _parser_creare_nodus(ctx, STML_NODUS_ELEMENTUM);
    si (!nodus) redde NIHIL;

    titulus_ptr = chorda_internare(ctx->intern, ctx->current.valor);
    nodus->titulus = titulus_ptr;
    nodus->attributa = ctx->current.attributa;
    nodus->captio_directio = STML_CAPTIO_RETRO;
    nodus->captio_numerus = ctx->current.captio_numerus;
    nodus->liberi = xar_creare(ctx->piscina, magnitudo(StmlNodus*));

    _parser_progredi(ctx);

    /* extensio: finis = tokenum ultimum consumptum */
    nodus->positus_finis = ctx->finis_ultimus;

    redde nodus;
}

/* Parse sandwich capture element */
interior StmlNodus*
_parser_legere_farcimen (
    StmlParserContext* ctx)
{
    StmlNodus* nodus;
       chorda* titulus_ptr;

    nodus = _parser_creare_nodus(ctx, STML_NODUS_ELEMENTUM);
    si (!nodus) redde NIHIL;

    titulus_ptr = chorda_internare(ctx->intern, ctx->current.valor);
    nodus->titulus = titulus_ptr;
    nodus->attributa = ctx->current.attributa;
    nodus->captio_directio = STML_CAPTIO_FARCIMEN;
    nodus->liberi = xar_creare(ctx->piscina, magnitudo(StmlNodus*));

    _parser_progredi(ctx);

    /* extensio: finis = tokenum ultimum consumptum */
    nodus->positus_finis = ctx->finis_ultimus;

    redde nodus;
}


/* ==================================================
 * Smart Whitespace Normalization
 * ================================================== */

/* Check if a line is empty (only whitespace) */
interior b32
_est_linea_vacua (
    chorda linea)
{
    i32 i;
    per (i = ZEPHYRUM; i < linea.mensura; i++)
    {
        character c = (character)linea.datum[i];
        si (c != ' ' && c != '\t' && c != '\r')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Count leading whitespace characters in a line */
interior i32
_numerare_indentationem (
    chorda linea)
{
    i32 spatia = ZEPHYRUM;
    i32 i;
    per (i = ZEPHYRUM; i < linea.mensura; i++)
    {
        character c = (character)linea.datum[i];
        si (c == ' ' || c == '\t')
        {
            spatia++;
        }
        alioquin
        {
            frange;
        }
    }
    redde spatia;
}

/* Check if string contains a newline */
interior b32
_continet_novam_lineam (
    chorda s)
{
    i32 i;
    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        si ((character)s.datum[i] == '\n')
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* Check if string at position starts with prefix */
interior b32
_starts_with_at (
                chorda  s,
                   i32  pos,
    constans character* prefix)
{
    i32 len;
    i32 i;

    len = (i32)strlen(prefix);
    si (pos + len > s.mensura)
    {
        redde FALSUM;
    }

    per (i = ZEPHYRUM; i < len; i++)
    {
        si ((character)s.datum[pos + i] != prefix[i])
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Unescape XML entities in text content:
 * &lt; -> <, &gt; -> >, &amp; -> &, &quot; -> ", &apos; -> '
 */
interior chorda
_unescape_entities (
     chorda  textus,
    Piscina* piscina)
{
    ChordaAedificator* aed;
                  i32  i;
                  b32  has_entity;

    /* Fast path: no ampersand, no entities to unescape */
    has_entity = FALSUM;
    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        si ((character)textus.datum[i] == '&')
        {
            has_entity = VERUM;
            frange;
        }
    }

    si (!has_entity)
    {
        redde textus;
    }

    aed = chorda_aedificator_creare(piscina, textus.mensura);
    si (!aed)
    {
        redde textus;
    }

    i = ZEPHYRUM;
    dum (i < textus.mensura)
    {
        si ((character)textus.datum[i] == '&')
        {
            /* Check for known entities */
            si (_starts_with_at(textus, i, "&lt;"))
            {
                chorda_aedificator_appendere_character(aed, '<');
                i += IV;
            }
            alioquin si (_starts_with_at(textus, i, "&gt;"))
            {
                chorda_aedificator_appendere_character(aed, '>');
                i += IV;
            }
            alioquin si (_starts_with_at(textus, i, "&amp;"))
            {
                chorda_aedificator_appendere_character(aed, '&');
                i += V;
            }
            alioquin si (_starts_with_at(textus, i, "&quot;"))
            {
                chorda_aedificator_appendere_character(aed, '"');
                i += VI;
            }
            alioquin si (_starts_with_at(textus, i, "&apos;"))
            {
                chorda_aedificator_appendere_character(aed, '\'');
                i += VI;
            }
            alioquin
            {
                /* Unknown entity - preserve as-is */
                chorda_aedificator_appendere_character(aed, '&');
                i++;
            }
        }
        alioquin
        {
            chorda_aedificator_appendere_character(aed, (character)textus.datum[i]);
            i++;
        }
    }

    redde chorda_aedificator_finire(aed);
}


/* ==================================================
 * Distributio triviae (spec triviae §1.2/§1.3)
 * ================================================== */

/* an cursus spatium album SOLUM ferat (non vacuus praesumptus) */
interior b32
_cursus_albus (
    chorda cursus)
{
    i32 i;

    per (i = ZEPHYRUM; i < cursus.mensura; i++)
    {
        si (!_est_spatium((character)cursus.datum[i]))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* cursum ad primam '\n' INCLUSIVE partiri (lex possessoris §1.2,
 * e silva transcripta): 'prius' priori nodo (spatia_post),
 * 'reliquum' sequenti (spatia_ante). Vocatur solum cum cursus
 * lineam novam fert. */
interior vacuum
_spatium_partiri (
    chorda  cursus,
    chorda* prius,
    chorda* reliquum)
{
    i32 i;

    i = ZEPHYRUM;
    dum (i < cursus.mensura && (character)cursus.datum[i] != '\n')
    {
        i++;
    }

    prius->datum       = cursus.datum;
    prius->mensura     = i + I;
    reliquum->datum    = cursus.datum + i + I;
    reliquum->mensura  = cursus.mensura - (i + I);
}

/* duas chordas iungere (casus rari ubi trivia confluunt, e.g.
 * trans nodum ERRATUM) */
interior chorda
_chordae_iungere (
    StmlParserContext* ctx,
               chorda  a,
               chorda  b)
{
    ChordaAedificator* aed;

    si (a.mensura == ZEPHYRUM)
    {
        redde b;
    }
    si (b.mensura == ZEPHYRUM)
    {
        redde a;
    }

    aed = chorda_aedificator_creare(ctx->piscina,
                                    a.mensura + b.mensura);
    si (aed == NIHIL)
    {
        redde a;
    }
    chorda_aedificator_appendere_chorda(aed, a);
    chorda_aedificator_appendere_chorda(aed, b);
    redde chorda_aedificator_finire(aed);
}

/* addendum in campum triviae ponere aut appendere (campus NIHIL =
 * nullum adhuc) */
interior vacuum
_spatia_apponere (
    StmlParserContext*  ctx,
               chorda** campus,
               chorda   addendum)
{
    si (addendum.mensura == ZEPHYRUM)
    {
        redde;
    }
    si (*campus == NIHIL)
    {
        *campus = chorda_internare(ctx->intern, addendum);
    }
    alioquin
    {
        *campus = chorda_internare(ctx->intern,
            _chordae_iungere(ctx, **campus, addendum));
    }
}

/* Regula capturae (§1.2 emendatum 2026-08-24, collapsus): post
 * tagum formae capturae spatium EIUSDEM LINEAE ad spatia_post
 * captoris pertinet - separator syntaxis capturae, non contentum.
 * Numquam nodus textus, numquam pars valoris: '<a(> <b/>'
 * elementum capit (non spatium), '<a(> foo' valorem "foo" fert.
 * Captor crudus exclusus (linea capta octetos crudos suos fert;
 * casus per constructionem mutus - cursus post lineam crudam '\n'
 * incipit). Praecedens: casus lineifer iam per legem generalem in
 * post captoris cadit; §6 ordo emissionis [tag][post][capti]
 * sedem probat. */
interior b32
_prior_est_captor (
    constans StmlNodus* prior)
{
    redde (b32)(prior != NIHIL
        && prior->captio_directio != STML_CAPTIO_NIHIL
        && !prior->crudus);
}

/* Classificatio cursus textus (§1.3), ab ansa liberorum vocata.
 *
 * - cursus TOTUS albus lineam-ferens: NULLUS nodus - octeti per
 *   §1.2 distribuuntur (elisio quae vectigal ambulatorum delet et
 *   numerationem captionum constructione sanat)
 * - cursus albus UNIUS lineae: nodus manet, valor integer
 *   ('<sep>   </sep>' tria spatia sua tenet) - NISI captorem
 *   sequitur (regula capturae supra: spatia_post eius fit)
 * - cursus contentum ferens: margo (praefixum/suffixum album
 *   maximale) e valore exit SI lineam novam fert; octeti exeuntes
 *   per §1.2 distribuuntur (pars prioris -> spatia_post eius,
 *   residuum -> spatia_ante huius/sequentis); margo ducens sine
 *   linea nova post captorem = spatia_post captoris
 *
 * Extensio positus cursum TOTUM tegit margine incluso (nodus ANTE
 * progressionem creatur - fons-honesta; via vetus extensionem
 * degenerem [finis,finis) dabat quia post progressionem creabat).
 *
 * 'pendens' residuum ante-destinatum inter vocationes fert: in
 * introitu praefixum spatiorum ante nodi fit, in exitu residuum
 * marginis caudalis pro nodo sequente conditur. */
interior StmlNodus*
_textum_tractare (
    StmlParserContext* ctx,
            StmlNodus* prior,
               chorda* pendens)
{
    StmlNodus* nodus;
       chorda  crudum;
       chorda  ante_mea;
       chorda  post_mea;
       chorda  cauda_residuum;
       chorda  nucleus;
       chorda  unescaped;
          i32  init;
          i32  fin;

    crudum = ctx->current.valor;

    si (crudum.mensura == ZEPHYRUM)
    {
        _parser_progredi(ctx);
        redde NIHIL;
    }

    si (_cursus_albus(crudum) && _continet_novam_lineam(crudum))
    {
        /* ELISIO (§1.3): nullus nodus, distributio §1.2 */
        si (prior != NIHIL)
        {
            chorda pars_prioris;
            chorda residuum;

            _spatium_partiri(crudum, &pars_prioris, &residuum);
            _spatia_apponere(ctx, &prior->spatia_post,
                             pars_prioris);
            *pendens = _chordae_iungere(ctx, *pendens, residuum);
        }
        alioquin
        {
            /* post tag apertum / initium parentis: TOTUM cursum
             * liberum primum accipit (§1.2) */
            *pendens = _chordae_iungere(ctx, *pendens, crudum);
        }
        _parser_progredi(ctx);
        redde NIHIL;
    }

    si (_cursus_albus(crudum) && _prior_est_captor(prior))
    {
        /* Regula capturae (§1.2 emendatum): cursus solum-albus
         * eiusdem lineae post captorem = spatia_post captoris
         * (separator formae) - nullus nodus. '<a(> <b/>'
         * elementum capit, non spatium. */
        _spatia_apponere(ctx, &prior->spatia_post, crudum);
        _parser_progredi(ctx);
        redde NIHIL;
    }

    init                    = ZEPHYRUM;
    fin                     = crudum.mensura;
    ante_mea.datum          = NIHIL;
    ante_mea.mensura        = ZEPHYRUM;
    post_mea.datum          = NIHIL;
    post_mea.mensura        = ZEPHYRUM;
    cauda_residuum.datum    = NIHIL;
    cauda_residuum.mensura  = ZEPHYRUM;

    si (!_cursus_albus(crudum))
    {
        chorda margo;

        /* margo ducens */
        dum (   init < crudum.mensura
             && _est_spatium((character)crudum.datum[init]))
        {
            init++;
        }
        margo.datum    = crudum.datum;
        margo.mensura  = init;
        si (margo.mensura > ZEPHYRUM && _continet_novam_lineam(margo))
        {
            si (prior != NIHIL)
            {
                chorda pars_prioris;
                chorda residuum;

                _spatium_partiri(margo, &pars_prioris, &residuum);
                _spatia_apponere(ctx, &prior->spatia_post,
                                 pars_prioris);
                ante_mea = residuum;
            }
            alioquin
            {
                ante_mea = margo;
            }
        }
        alioquin si (   margo.mensura > ZEPHYRUM
                     && _prior_est_captor(prior))
        {
            /* Regula capturae (§1.2 emendatum): margo ducens sine
             * linea nova post captorem = spatia_post captoris -
             * '<a(> foo' valorem "foo" fert, non " foo" */
            _spatia_apponere(ctx, &prior->spatia_post, margo);
        }
        alioquin
        {
            init = ZEPHYRUM;  /* sine linea nova: in valore manet */
        }

        /* margo caudalis */
        dum (   fin > init
             && _est_spatium((character)crudum.datum[fin - I]))
        {
            fin--;
        }
        margo.datum    = crudum.datum + fin;
        margo.mensura  = crudum.mensura - fin;
        si (margo.mensura > ZEPHYRUM && _continet_novam_lineam(margo))
        {
            _spatium_partiri(margo, &post_mea, &cauda_residuum);
        }
        alioquin
        {
            fin = crudum.mensura;  /* sine linea nova: manet */
        }
    }

    /* praefixum pendens (ex cursu eliso priore) ante marginem
     * proprium confluit */
    ante_mea          = _chordae_iungere(ctx, *pendens, ante_mea);
    pendens->datum    = NIHIL;
    pendens->mensura  = ZEPHYRUM;

    nucleus.datum    = crudum.datum + init;
    nucleus.mensura  = fin - init;
    unescaped        = _unescape_entities(nucleus, ctx->piscina);

    /* nodus ANTE progressionem - extensio tokeni ipsius */
    nodus = _parser_creare_nodus(ctx, STML_NODUS_TEXTUS);
    si (nodus == NIHIL)
    {
        _parser_progredi(ctx);
        redde NIHIL;
    }
    nodus->valor = chorda_internare(ctx->intern, unescaped);
    si (ante_mea.mensura > ZEPHYRUM)
    {
        nodus->spatia_ante = chorda_internare(ctx->intern, ante_mea);
    }
    si (post_mea.mensura > ZEPHYRUM)
    {
        nodus->spatia_post = chorda_internare(ctx->intern, post_mea);
    }

    _parser_progredi(ctx);

    *pendens = cauda_residuum;
    redde nodus;
}

/* Ansa liberorum UNA (elementum/fragmentum/percentum/documentum):
 * tokens TEXTUS per classificationem §1.3 eunt, cetera per
 * _parser_legere_nodus. Trivia loco uno distribuuntur: pars
 * pendens nodo proximo (spatia_ante), residuum finale parenti
 * (spatia_clausurae - pro documento cauda plagulae). */
interior vacuum
_liberos_legere (
     StmlParserContext* ctx,
             StmlNodus* parens,
        StmlTokenGenus  terminator)
{
    StmlNodus* prior;
       chorda  pendens;

    prior            = NIHIL;
    pendens.datum    = NIHIL;
    pendens.mensura  = ZEPHYRUM;

    dum (   ctx->current.genus != terminator
         && ctx->current.genus != STML_TOKEN_FINIS)
    {
        StmlNodus* liberum;

        si (ctx->current.genus == STML_TOKEN_TEXTUS)
        {
            liberum = _textum_tractare(ctx, prior, &pendens);
        }
        alioquin
        {
            liberum = _parser_legere_nodus(ctx);
            si (liberum != NIHIL && pendens.mensura > ZEPHYRUM)
            {
                liberum->spatia_ante =
                    chorda_internare(ctx->intern, pendens);
                pendens.datum    = NIHIL;
                pendens.mensura  = ZEPHYRUM;
            }
        }

        si (liberum != NIHIL)
        {
            StmlNodus** cella;

            liberum->parens  = parens;
            cella            = xar_addere(parens->liberi);
            si (cella) *cella = liberum;
            prior = liberum;
        }
    }

    si (pendens.mensura > ZEPHYRUM)
    {
        _spatia_apponere(ctx, &parens->spatia_clausurae, pendens);
    }
}


/* ==================================================
 * Multilinea '<tag\>' (spec triviae §1.4)
 * ================================================== */

interior vacuum
_multilineam_recusare (
     StmlParserContext* ctx,
    constans character* causa,
                   i32  linea)
{
    si (ctx->status != STML_SUCCESSUS)
    {
        redde;  /* error primus iam nominat */
    }
    ctx->status           = STML_ERROR_MULTILINEA;
    ctx->linea_erroris    = linea;
    ctx->columna_erroris  = I;
    ctx->error            = chorda_ex_literis(causa, ctx->piscina);
}

/* Transformatio '<tag\>' post ansam liberorum (crudo et non-crudo
 * communis - liber textus unus aut nullus post elisionem).
 *
 * MARGO DUCENS ad primam '\n' SOLAM reducitur - regula generalis
 * (praefixum maximale) indentationem lineae primae devoraret et
 * structuram relativam deleret ubi linea prima profundius quam
 * sequentes indentata est. Contentum in linea tagi
 * ('<m\>abc...') legale sed a computatione dedentationis EXCLUSUM
 * (regula PEP-257: indentationem nullam constructione habet et
 * praefixum ad nihil figeret).
 *
 * MARGO CAUDALIS = regula generalis §1.3 (suffixum maximale
 * lineam-ferens exit totum; hic pro CRUDO applicatur cui
 * classificatio generalis numquam currit; non-crudo iam facta -
 * iteratio innocua).
 *
 * DEDENTATIO: praefixum album commune linearum non vacuarum
 * (OCTETIM - tab et spatium praefixum commune nullum habent, ergo
 * mixtura nihil demit, deterministice) e valore demptum, in
 * elemento conditum (indentatio). Linea interior spatio sola NEC
 * vacua = vitium nominatum - sola classis quae reassemblationem
 * praefixi ambiguam faceret. Lineae vere vacuae manent vacuae
 * (scriptor praefixum lineis non vacuis solis reinserit). */
interior vacuum
_multilineam_transformare (
    StmlParserContext* ctx,
            StmlNodus* nodus)
{
                StmlNodus* textus;
    chorda_fissio_fructus  lineae;
        ChordaAedificator* aed;
                   chorda  interior_totum;
                   chorda  corpus;
                   chorda  praefixum;
                   chorda  linea;
                      b32  prima_exclusa;
                      b32  praefixum_visum;
                      i32  numerus_liberorum;
                      i32  i;
                      i32  ab;

    numerus_liberorum = nodus->liberi
        ? xar_numerus(nodus->liberi) : ZEPHYRUM;

    /* liberi textus soli (v1): elementum/commentum interius =
     * recusatio nominata (liberi inlineares v2 reservati) */
    per (i = ZEPHYRUM; i < numerus_liberorum; i++)
    {
        StmlNodus* liberum;

        liberum = _xar_liberum_obtinere(nodus->liberi, i);
        si (liberum != NIHIL && liberum->genus != STML_NODUS_TEXTUS)
        {
            _multilineam_recusare(ctx,
                "liberi inlineares in '<tag\\>' v2 reservati "
                "(textus solus licet)", nodus->linea);
            redde;
        }
    }

    si (numerus_liberorum == ZEPHYRUM)
    {
        redde;  /* contentum totum album elisum - vacuum genuinum */
    }

    textus = _xar_liberum_obtinere(nodus->liberi, ZEPHYRUM);
    si (textus == NIHIL)
    {
        redde;
    }

    /* interius reconstruere: margo ducens generalis (ante) + valor.
     * Pro crudo ante NIHIL est et valor verbatim - via eadem. */
    {
        chorda ante_vetus;
        chorda valor_vetus;

        ante_vetus.datum    = NIHIL;
        ante_vetus.mensura  = ZEPHYRUM;
        si (textus->spatia_ante != NIHIL)
        {
            ante_vetus = *textus->spatia_ante;
        }
        valor_vetus.datum    = NIHIL;
        valor_vetus.mensura  = ZEPHYRUM;
        si (textus->valor != NIHIL)
        {
            valor_vetus = *textus->valor;
        }
        interior_totum = _chordae_iungere(ctx, ante_vetus,
                                          valor_vetus);
    }

    si (interior_totum.mensura == ZEPHYRUM)
    {
        redde;
    }

    /* margo caudalis (crudo necessaria, non-crudo iam facta):
     * suffixum album maximale lineam-ferens exit - pars per primam
     * '\n' fit spatia_post, residuum clausurae parentis (§1.2) */
    {
        i32 fin;

        fin = interior_totum.mensura;
        dum (   fin > ZEPHYRUM
             && _est_spatium(
                    (character)interior_totum.datum[fin - I]))
        {
            fin--;
        }
        si (fin < interior_totum.mensura)
        {
            chorda margo;

            margo.datum    = interior_totum.datum + fin;
            margo.mensura  = interior_totum.mensura - fin;
            si (_continet_novam_lineam(margo))
            {
                chorda pars_post;
                chorda residuum;

                _spatium_partiri(margo, &pars_post, &residuum);
                _spatia_apponere(ctx, &textus->spatia_post,
                                 pars_post);
                _spatia_apponere(ctx, &nodus->spatia_clausurae,
                                 residuum);
                interior_totum.mensura = fin;
            }
        }
    }

    si (interior_totum.mensura == ZEPHYRUM)
    {
        redde;
    }

    /* margo ducens multilineae: prima '\n' SOLA */
    si ((character)interior_totum.datum[ZEPHYRUM] == '\n')
    {
        textus->spatia_ante = chorda_internare_ex_literis(
            ctx->intern, "\n");
        corpus.datum    = interior_totum.datum + I;
        corpus.mensura  = interior_totum.mensura - I;
        prima_exclusa   = FALSUM;
    }
    alioquin
    {
        textus->spatia_ante  = NIHIL;
        corpus               = interior_totum;
        prima_exclusa        = VERUM;  /* linea tagi - extra dedentationem */
    }

    lineae = chorda_fissio(corpus, '\n', ctx->piscina);
    si (lineae.numerus == ZEPHYRUM || lineae.elementa == NIHIL)
    {
        textus->valor = chorda_internare(ctx->intern, corpus);
        redde;
    }

    /* praefixum commune (octetim) linearum participantium non
     * vacuarum; linea spatio sola nec vacua = vitium */
    praefixum.datum    = NIHIL;
    praefixum.mensura  = ZEPHYRUM;
    praefixum_visum    = FALSUM;
    ab                 = prima_exclusa ? I : ZEPHYRUM;

    per (i = ab; i < lineae.numerus; i++)
    {
        i32 albi;

        linea = lineae.elementa[i];
        si (linea.mensura == ZEPHYRUM)
        {
            perge;
        }

        albi = ZEPHYRUM;
        dum (   albi < linea.mensura
             && (   (character)linea.datum[albi] == ' '
                 || (character)linea.datum[albi] == '\t'))
        {
            albi++;
        }
        si (albi == linea.mensura)
        {
            _multilineam_recusare(ctx,
                "linea interior spatio sola nec vacua in '<tag\\>' "
                "(reassemblatio praefixi ambigua fieret)",
                nodus->linea);
            redde;
        }

        si (!praefixum_visum)
        {
            praefixum.datum    = linea.datum;
            praefixum.mensura  = albi;
            praefixum_visum    = VERUM;
        }
        alioquin
        {
            i32 commune;
            i32 tectum;

            tectum = (praefixum.mensura < albi)
                ? praefixum.mensura : albi;
            commune = ZEPHYRUM;
            dum (   commune < tectum
                 && praefixum.datum[commune]
                        == linea.datum[commune])
            {
                commune++;
            }
            praefixum.mensura = commune;
        }
    }

    /* valorem dedentatum reaedificare */
    aed = chorda_aedificator_creare(ctx->piscina,
                                    corpus.mensura);
    si (aed == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < lineae.numerus; i++)
    {
        linea = lineae.elementa[i];
        si (i > ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(aed, '\n');
        }
        si (linea.mensura == ZEPHYRUM)
        {
            perge;
        }
        si (   i             >= ab
            && praefixum.mensura > ZEPHYRUM
            && linea.mensura >= praefixum.mensura)
        {
            chorda pars;

            pars.datum    = linea.datum + praefixum.mensura;
            pars.mensura  = linea.mensura - praefixum.mensura;
            chorda_aedificator_appendere_chorda(aed, pars);
        }
        alioquin
        {
            chorda_aedificator_appendere_chorda(aed, linea);
        }
    }

    textus->valor = chorda_internare(ctx->intern,
                        chorda_aedificator_finire(aed));
    si (praefixum.mensura > ZEPHYRUM)
    {
        nodus->indentatio = chorda_internare(ctx->intern, praefixum);
    }
}

/* Smart whitespace normalization:
 * - Trim leading/trailing empty lines
 * - Normalize indentation to least-indented non-empty line
 * - Preserve relative indentation
 */
interior chorda
_normalizare_spatium_album (
     chorda  textus,
    Piscina* piscina)
{
    chorda_fissio_fructus  lineae;
        ChordaAedificator* aed;
                      i32  initium;
                      i32  finis;
                      i32  min_indent;
                      i32  i;
                      i32  indent;
                   chorda  linea;
                   chorda  result;

    /* Fast path: no newlines -> simple trim */
    si (!_continet_novam_lineam(textus))
    {
        redde chorda_praecidere(textus);
    }

    /* Only apply smart trim if starts with newline or whitespace */
    {
        character primus = (character)textus.datum[ZEPHYRUM];
        si (primus != '\n' && primus != ' ' && primus != '\t' && primus != '\r')
        {
            /* Inline text - just trim ends */
            redde chorda_praecidere(textus);
        }
    }

    /* Split into lines */
    lineae = chorda_fissio(textus, '\n', piscina);
    si (lineae.numerus == ZEPHYRUM || !lineae.elementa)
    {
        result.datum    = NIHIL;
        result.mensura  = ZEPHYRUM;
        redde result;
    }

    /* Find first and last non-empty lines */
    initium  = lineae.numerus;
    finis    = ZEPHYRUM;
    per (i = ZEPHYRUM; i < lineae.numerus; i++)
    {
        si (!_est_linea_vacua(lineae.elementa[i]))
        {
            si (initium == lineae.numerus)
            {
                initium = i;
            }
            finis = i;
        }
    }

    /* All empty */
    si (initium == lineae.numerus)
    {
        result.datum    = NIHIL;
        result.mensura  = ZEPHYRUM;
        redde result;
    }

    /* Find minimum indentation of non-empty lines */
    min_indent = 9999;
    per (i = initium; i <= finis; i++)
    {
        linea = lineae.elementa[i];
        si (!_est_linea_vacua(linea))
        {
            indent = _numerare_indentationem(linea);
            si (indent < min_indent)
            {
                min_indent = indent;
            }
        }
    }

    si (min_indent == 9999)
    {
        min_indent = ZEPHYRUM;
    }

    /* Build result with dedented lines */
    aed = chorda_aedificator_creare(piscina, textus.mensura);
    si (!aed)
    {
        result.datum    = NIHIL;
        result.mensura  = ZEPHYRUM;
        redde result;
    }

    per (i = initium; i <= finis; i++)
    {
        linea = lineae.elementa[i];

        /* Add newline between lines (not before first) */
        si (i > initium)
        {
            chorda_aedificator_appendere_character(aed, '\n');
        }

        /* Empty lines stay empty */
        si (_est_linea_vacua(linea))
        {
            /* Add nothing - just the newline above */
        }
        alioquin
        {
            /* Remove min_indent characters from start */
            si (min_indent > ZEPHYRUM && min_indent <= linea.mensura)
            {
                chorda dedented = chorda_sectio(linea, min_indent, linea.mensura);
                chorda_aedificator_appendere_chorda(aed, dedented);
            }
            alioquin
            {
                chorda_aedificator_appendere_chorda(aed, linea);
            }
        }
    }

    redde chorda_aedificator_finire(aed);
}

/* Parse text node - via VERBATIM.
 *
 * ATTINGI NON DEBET in fluxu ordinario: ansae liberorum tokens
 * TEXTUS per _textum_tractare (classificatio §1.3) intercipiunt.
 * Manet ut via tuta si TEXTUS alibi emergat - textus verbatim
 * servatur potius quam tacite perditur. Nodus ANTE progressionem
 * creatur ut extensio tokenum ipsum tegat (via vetus [finis,finis)
 * degenerem dabat). */
interior StmlNodus*
_parser_legere_textus (
    StmlParserContext* ctx)
{
    StmlNodus* nodus;
       chorda  contentus;
       chorda  unescaped;

    contentus = ctx->current.valor;

    /* nodus vere vacuus nihil fert; spatium album CONTENTUS est */
    si (contentus.mensura == ZEPHYRUM)
    {
        _parser_progredi(ctx);
        redde NIHIL;
    }

    /* Unescape XML entities (&lt; -> <, etc.) */
    unescaped = _unescape_entities(contentus, ctx->piscina);

    nodus = _parser_creare_nodus(ctx, STML_NODUS_TEXTUS);
    si (!nodus)
    {
        _parser_progredi(ctx);
        redde NIHIL;
    }

    nodus->valor = chorda_internare(ctx->intern, unescaped);

    _parser_progredi(ctx);

    redde nodus;
}

/* Parse comment node */
interior StmlNodus*
_parser_legere_commentum (
    StmlParserContext* ctx)
{
    StmlNodus* nodus;
       chorda* contentus_ptr;

    nodus = _parser_creare_nodus(ctx, STML_NODUS_COMMENTUM);
    si (!nodus) redde NIHIL;

    contentus_ptr  = chorda_internare(ctx->intern, ctx->current.valor);
    nodus->valor   = contentus_ptr;

    _parser_progredi(ctx);

    /* extensio: finis = tokenum ultimum consumptum */
    nodus->positus_finis = ctx->finis_ultimus;

    redde nodus;
}

/* Parse processing instruction node */
interior StmlNodus*
_parser_legere_processio (
    StmlParserContext* ctx)
{
    StmlNodus* nodus;
       chorda* contentus_ptr;

    nodus = _parser_creare_nodus(ctx, STML_NODUS_PROCESSIO);
    si (!nodus) redde NIHIL;

    contentus_ptr  = chorda_internare(ctx->intern, ctx->current.valor);
    nodus->valor   = contentus_ptr;

    _parser_progredi(ctx);

    /* extensio: finis = tokenum ultimum consumptum */
    nodus->positus_finis = ctx->finis_ultimus;

    redde nodus;
}

/* Parse fragment node <#> or <#id> */
interior StmlNodus*
_parser_legere_fragmentum (
    StmlParserContext* ctx)
{
    StmlNodus* nodus;
       chorda* titulus_ptr;
       chorda* fragmentum_id_ptr;

    nodus = _parser_creare_nodus(ctx, STML_NODUS_ELEMENTUM);
    si (!nodus) redde NIHIL;

    /* Set fragment flag and tag name */
    nodus->fragmentum  = VERUM;
    titulus_ptr        = chorda_internare_ex_literis(ctx->intern, "#");
    nodus->titulus     = titulus_ptr;

    /* Store fragment ID if present */
    si (ctx->current.valor.mensura > ZEPHYRUM)
    {
        fragmentum_id_ptr = chorda_internare(ctx->intern, ctx->current.valor);
        nodus->fragmentum_id = fragmentum_id_ptr;
    }
    alioquin
    {
        nodus->fragmentum_id = NIHIL;
    }

    /* Copy attributes */
    nodus->attributa = ctx->current.attributa;
    si (ctx->current.spatia_prae_finem.mensura > ZEPHYRUM)
    {
        nodus->spatia_intra_tagum = chorda_internare(ctx->intern,
            ctx->current.spatia_prae_finem);
    }

    /* Initialize children array */
    nodus->liberi = xar_creare(ctx->piscina, magnitudo(StmlNodus*));

    _parser_progredi(ctx);

    /* Parse children until </#> */
    _liberos_legere(ctx, nodus, STML_TOKEN_FRAGMENTUM_CLAUDERE);

    /* Consume closing tag </#> */
    si (ctx->current.genus == STML_TOKEN_FRAGMENTUM_CLAUDERE)
    {
        _parser_progredi(ctx);
    }

    /* extensio: finis = tokenum ultimum consumptum */
    nodus->positus_finis = ctx->finis_ultimus;

    redde nodus;
}

/* Parse self-closing fragment <#/> or <#id/> */
interior StmlNodus*
_parser_legere_fragmentum_auto (
    StmlParserContext* ctx)
{
    StmlNodus* nodus;
       chorda* titulus_ptr;
       chorda* fragmentum_id_ptr;

    nodus = _parser_creare_nodus(ctx, STML_NODUS_ELEMENTUM);
    si (!nodus) redde NIHIL;

    /* Set fragment flag and tag name */
    nodus->fragmentum  = VERUM;
    titulus_ptr        = chorda_internare_ex_literis(ctx->intern, "#");
    nodus->titulus     = titulus_ptr;

    /* Store fragment ID if present */
    si (ctx->current.valor.mensura > ZEPHYRUM)
    {
        fragmentum_id_ptr = chorda_internare(ctx->intern, ctx->current.valor);
        nodus->fragmentum_id = fragmentum_id_ptr;
    }
    alioquin
    {
        nodus->fragmentum_id = NIHIL;
    }

    /* Copy attributes */
    nodus->attributa = ctx->current.attributa;
    si (ctx->current.spatia_prae_finem.mensura > ZEPHYRUM)
    {
        nodus->spatia_intra_tagum = chorda_internare(ctx->intern,
            ctx->current.spatia_prae_finem);
    }

    /* Campi capturae (<# (> / <#id (>): receptaculum liberis
     * necessarium - _processare_captiones fratres huc movet */
    nodus->captio_numerus = ctx->current.captio_numerus;
    si (ctx->current.captio_numerus > ZEPHYRUM)
    {
        nodus->captio_directio = STML_CAPTIO_ANTE;
        nodus->liberi = xar_creare(ctx->piscina,
            magnitudo(StmlNodus*));
    }

    _parser_progredi(ctx);

    /* extensio: finis = tokenum ultimum consumptum */
    nodus->positus_finis = ctx->finis_ultimus;

    redde nodus;
}

/* Parse augmentation <% &clavis;> ... </%> - elementum titulo "%"
 * clavem destinatam ferens. Non clausum ad finem = vitium (aliter
 * augmentatio reliquum documenti tacite devoraret). */
interior StmlNodus*
_parser_legere_percentum (
    StmlParserContext* ctx)
{
    StmlNodus* nodus;
       chorda* titulus_ptr;

    nodus = _parser_creare_nodus(ctx, STML_NODUS_ELEMENTUM);
    si (!nodus) redde NIHIL;

    titulus_ptr     = chorda_internare_ex_literis(ctx->intern, "%");
    nodus->titulus  = titulus_ptr;
    nodus->augmentum_clavis = chorda_internare(ctx->intern,
                                               ctx->current.valor);

    nodus->liberi = xar_creare(ctx->piscina, magnitudo(StmlNodus*));

    _parser_progredi(ctx);

    /* Parse children until </%> */
    _liberos_legere(ctx, nodus, STML_TOKEN_PERCENTUM_CLAUDERE);

    si (ctx->current.genus == STML_TOKEN_PERCENTUM_CLAUDERE)
    {
        _parser_progredi(ctx);
    }
    alioquin
    {
        _errorem_ponere(ctx, STML_ERROR_TAG_NON_CLAUSUM,
                        ctx->current.linea, ctx->current.columna);
    }

    /* extensio: finis = tokenum ultimum consumptum */
    nodus->positus_finis = ctx->finis_ultimus;

    redde nodus;
}

/* Parse transclusion <<selector>> */
interior StmlNodus*
_parser_legere_transclusio (
    StmlParserContext* ctx)
{
    StmlNodus* nodus;
       chorda* selector_ptr;

    nodus = _parser_creare_nodus(ctx, STML_NODUS_TRANSCLUSIO);
    si (!nodus) redde NIHIL;

    /* Store selector in valor field */
    selector_ptr = chorda_internare(ctx->intern, ctx->current.valor);
    nodus->valor = selector_ptr;

    _parser_progredi(ctx);

    /* extensio: finis = tokenum ultimum consumptum */
    nodus->positus_finis = ctx->finis_ultimus;

    redde nodus;
}

/* Parse DOCTYPE node */
interior StmlNodus*
_parser_legere_doctype (
    StmlParserContext* ctx)
{
    StmlNodus* nodus;
       chorda* contentus_ptr;

    nodus = _parser_creare_nodus(ctx, STML_NODUS_DOCTYPE);
    si (!nodus) redde NIHIL;

    contentus_ptr  = chorda_internare(ctx->intern, ctx->current.valor);
    nodus->valor   = contentus_ptr;

    _parser_progredi(ctx);

    /* extensio: finis = tokenum ultimum consumptum */
    nodus->positus_finis = ctx->finis_ultimus;

    redde nodus;
}

/* Parse any node */
interior StmlNodus*
_parser_legere_nodus (
    StmlParserContext* ctx)
{
    commutatio (ctx->current.genus)
    {
        casus STML_TOKEN_APERIRE:
            redde _parser_legere_elementum(ctx);

        casus STML_TOKEN_CRUDUS:
            redde _parser_legere_elementum_crudus(ctx);

        casus STML_TOKEN_AUTO_CLAUDERE:
            redde _parser_legere_auto_claudere(ctx);

        casus STML_TOKEN_CAPTIO_ANTE:
            redde _parser_legere_captio_ante(ctx);

        casus STML_TOKEN_CAPTIO_RETRO:
            redde _parser_legere_captio_retro(ctx);

        casus STML_TOKEN_FARCIMEN:
            redde _parser_legere_farcimen(ctx);

        casus STML_TOKEN_TEXTUS:
            redde _parser_legere_textus(ctx);

        casus STML_TOKEN_COMMENTUM:
            redde _parser_legere_commentum(ctx);

        casus STML_TOKEN_PROCESSIO:
            redde _parser_legere_processio(ctx);

        casus STML_TOKEN_DOCTYPE:
            redde _parser_legere_doctype(ctx);

        casus STML_TOKEN_FRAGMENTUM_APERIRE:
            redde _parser_legere_fragmentum(ctx);

        casus STML_TOKEN_FRAGMENTUM_AUTO:
            redde _parser_legere_fragmentum_auto(ctx);

        casus STML_TOKEN_PERCENTUM_APERIRE:
            redde _parser_legere_percentum(ctx);

        casus STML_TOKEN_TRANSCLUSIO:
            redde _parser_legere_transclusio(ctx);

        casus STML_TOKEN_ERRATUM:
            /* Titulus illegaliter incipiens ('<.x>', '<9bad>') -
             * olim arbor tacite corrumpebatur (2026-08-10) */
            _errorem_ponere(ctx, STML_ERROR_SYNTAXIS,
                ctx->current.linea, ctx->current.columna);
            _parser_progredi(ctx);  /* Consume to avoid infinite loop */
            redde NIHIL;

        casus STML_TOKEN_CLAUDERE:
        casus STML_TOKEN_FRAGMENTUM_CLAUDERE:
        casus STML_TOKEN_PERCENTUM_CLAUDERE:
            /* Orphan closing tag - no matching open tag */
            _errorem_ponere(ctx, STML_ERROR_TAG_IMPROPRIE,
                ctx->current.linea, ctx->current.columna);
            _parser_progredi(ctx);  /* Consume to avoid infinite loop */
            redde NIHIL;

        ordinarius:
            redde NIHIL;
    }
}


/* ==================================================
 * Capture Processing
 * ================================================== */

interior b32
_habet_captio (
    StmlNodus* nodus)
{
          i32  i;
          i32  num;
    StmlNodus* liberum;

    si (   nodus->genus           == STML_NODUS_ELEMENTUM
        && nodus->captio_directio != STML_CAPTIO_NIHIL)
    {
        redde VERUM;
    }

    si (nodus->liberi)
    {
        StmlNodus** slot;
        num = xar_numerus(nodus->liberi);
        per (i = ZEPHYRUM; i < num; i++)
        {
            slot = xar_obtinere(nodus->liberi, i);
            si (slot)
            {
                liberum = *slot;
                si (liberum && _habet_captio(liberum))
                {
                    redde VERUM;
                }
            }
        }
    }

    redde FALSUM;
}

interior b32
_est_commentum (
    StmlNodus* nodus)
{
    redde nodus && nodus->genus == STML_NODUS_COMMENTUM;
}

/* Helper to get child at index */
interior StmlNodus*
_xar_liberum_obtinere (
    Xar* xar,
    i32  index)
{
    StmlNodus** slot = xar_obtinere(xar, index);
    si (slot) redde *slot;
    redde NIHIL;
}

/* Captorem ANTE satiare: liberos ex flumine fratrum [j..num)
 * capere; indicem primum non consumptum reddit.
 *
 * SATIATIO RECURSIVA (T3, 2026-08-24): captus qui IPSE captor
 * ANTE insatiatus est prius ex flumine reliquo satiatur - spinae
 * '<a(> <b(> x' resolvuntur (a capit b, b capit x). Ansa vetus
 * plana erat: a capiebat b NUDUM et x frater manebat - catenae
 * numquam probatae erant (corpus formam spinae nondum fert;
 * collapsus eam paritura est).
 *
 * COMMENTA: transparentia numerationi sed SERVATA - liberos
 * captoris intrant ordine fluminis (§6 'skipped-but-preserved';
 * ansa vetus ea tacite perdebat: saltabantur nec usquam
 * addebantur). */
interior i32
_captorem_ante_satiare (
    StmlNodus* captor,
          Xar* flumen,
          i32  j,
          i32  num)
{
    i32 capti;

    capti = ZEPHYRUM;
    dum (j < num && capti < captor->captio_numerus)
    {
        StmlNodus*  captus;
        StmlNodus** slot;

        captus = _xar_liberum_obtinere(flumen, j);
        si (captus == NIHIL)
        {
            j++;
            perge;
        }
        si (_est_commentum(captus))
        {
            captus->parens  = captor;
            slot            = xar_addere(captor->liberi);
            si (slot) *slot = captus;
            j++;
            perge;
        }
        j++;
        si (   captus->genus           == STML_NODUS_ELEMENTUM
            && captus->captio_directio == STML_CAPTIO_ANTE
            && !captus->crudus)
        {
            j = _captorem_ante_satiare(captus, flumen, j, num);
        }
        captus->parens  = captor;
        slot            = xar_addere(captor->liberi);
        si (slot) *slot = captus;
        capti++;
    }
    redde j;
}

/* Process capture operators - restructure tree */
interior vacuum
_processare_captiones (
    StmlNodus* nodus,
      Piscina* piscina)
{
          Xar* novi_liberi;
    StmlNodus* liberum;
    StmlNodus* captus;
          i32  num;
          i32  i;
          i32  j;
          i32  captio_count;
          i32  captured_count;
          s32  k;

    si (!nodus || !nodus->liberi)
    {
        redde;
    }

    /* First pass: process children recursively */
    num = xar_numerus(nodus->liberi);
    per (i = ZEPHYRUM; i < num; i++)
    {
        liberum = _xar_liberum_obtinere(nodus->liberi, i);
        si (liberum)
        {
            _processare_captiones(liberum, piscina);
        }
    }

    /* Second pass: handle capture operators */
    novi_liberi = xar_creare(piscina, magnitudo(StmlNodus*));

    i = ZEPHYRUM;
    dum (i < num)
    {
        liberum = _xar_liberum_obtinere(nodus->liberi, i);

        si (!liberum)
        {
            i++;
            perge;
        }

        /* Skip comments (transparent) */
        si (_est_commentum(liberum))
        {
            StmlNodus** slot_n = xar_addere(novi_liberi);
            si (slot_n) *slot_n = liberum;
            i++;
            perge;
        }

        si (liberum->genus == STML_NODUS_ELEMENTUM)
        {
            /* Forward capture (crudae iam satiatae tempore
             * parsationis - linea intus, fratres non capiendi).
             * Satiatio recursiva: spinae + commenta servata (vide
             * _captorem_ante_satiare). */
            si (   liberum->captio_directio == STML_CAPTIO_ANTE
                && !liberum->crudus)
            {
                j = _captorem_ante_satiare(liberum, nodus->liberi,
                                           i + I, num);
                {
                    StmlNodus** slot_n = xar_addere(novi_liberi);
                    si (slot_n) *slot_n = liberum;
                }
                i = j;
                perge;
            }

            /* Backward capture */
            si (liberum->captio_directio == STML_CAPTIO_RETRO)
            {
                captio_count    = liberum->captio_numerus;
                captured_count  = ZEPHYRUM;

                /* Capture previous N non-comment siblings from novi_liberi */
                k = (s32)xar_numerus(novi_liberi) - I;
                dum (k >= ZEPHYRUM && captured_count < captio_count)
                {
                    captus = *((StmlNodus**)xar_obtinere(novi_liberi, (i32)k));
                    si (captus && !_est_commentum(captus))
                    {
                        /* Insert at beginning of liberum's children */
                              Xar*  temp;
                              i32   m;
                              i32   temp_num;
                        StmlNodus** slot_t;

                        temp = xar_creare(piscina, magnitudo(StmlNodus*));
                        slot_t = xar_addere(temp);
                        si (slot_t) *slot_t = captus;

                        temp_num = xar_numerus(liberum->liberi);
                        per (m = ZEPHYRUM; m < temp_num; m++)
                        {
                            StmlNodus* temp_elem;
                            temp_elem = _xar_liberum_obtinere(liberum->liberi, m);
                            slot_t = xar_addere(temp);
                            si (slot_t) *slot_t = temp_elem;
                        }

                        liberum->liberi  = temp;
                        captus->parens   = liberum;

                        /* Remove from novi_liberi */
                        /* For simplicity, mark as null and skip later */
                        *((StmlNodus**)xar_obtinere(novi_liberi, (i32)k)) = NIHIL;

                        captured_count++;
                    }
                    k--;
                }

                {
                    StmlNodus** slot_n = xar_addere(novi_liberi);
                    si (slot_n) *slot_n = liberum;
                }
                i++;
                perge;
            }

            /* Sandwich capture */
            si (liberum->captio_directio == STML_CAPTIO_FARCIMEN)
            {
                StmlNodus** slot_c;
                /* Capture one previous non-comment sibling */
                k = (s32)xar_numerus(novi_liberi) - I;
                dum (k >= ZEPHYRUM)
                {
                    captus = *((StmlNodus**)xar_obtinere(novi_liberi, (i32)k));
                    si (captus && !_est_commentum(captus))
                    {
                        captus->parens  = liberum;
                        slot_c          = xar_addere(liberum->liberi);
                        si (slot_c) *slot_c = captus;
                        *((StmlNodus**)xar_obtinere(novi_liberi, (i32)k)) = NIHIL;
                        frange;
                    }
                    k--;
                }

                /* Capture one next non-comment sibling */
                j = i + I;
                dum (j < num)
                {
                    captus = _xar_liberum_obtinere(nodus->liberi, j);
                    si (captus && !_est_commentum(captus))
                    {
                        captus->parens  = liberum;
                        slot_c          = xar_addere(liberum->liberi);
                        si (slot_c) *slot_c = captus;
                        j++;
                        frange;
                    }
                    j++;
                }

                {
                    StmlNodus** slot_n = xar_addere(novi_liberi);
                    si (slot_n) *slot_n = liberum;
                }
                i = j;
                perge;
            }
        }

        /* Regular node */
        {
            StmlNodus** slot_n = xar_addere(novi_liberi);
            si (slot_n) *slot_n = liberum;
        }
        i++;
    }

    /* Remove null entries from novi_liberi */
    {
              Xar*  clean;
              i32   clean_num;
        StmlNodus*  entry;
        StmlNodus** slot_e;

        clean      = xar_creare(piscina, magnitudo(StmlNodus*));
        clean_num  = xar_numerus(novi_liberi);
        per (i = ZEPHYRUM; i < clean_num; i++)
        {
            entry = *((StmlNodus**)xar_obtinere(novi_liberi, i));
            si (entry)
            {
                slot_e = xar_addere(clean);
                si (slot_e) *slot_e = entry;
            }
        }
        nodus->liberi = clean;
    }
}


/* ==================================================
 * Public API - Parsing
 * ================================================== */

StmlResultus
stml_legere (
                 chorda  input,
                Piscina* piscina,
    InternamentumChorda* intern)
{
         StmlResultus  result;
    StmlParserContext  ctx;
            StmlNodus* documentum;
                  i32  i;
                  i32  num;

    /* Initialize result */
    result.successus             = FALSUM;
    result.radix                 = NIHIL;
    result.elementum_radix       = NIHIL;
    result.status                = STML_SUCCESSUS;
    result.linea_erroris         = ZEPHYRUM;
    result.columna_erroris       = ZEPHYRUM;
    result.error.datum           = NIHIL;
    result.error.mensura         = ZEPHYRUM;
    result.crlf_canonicalizatum  = FALSUM;

    /* Check for empty input */
    si (input.mensura == ZEPHYRUM || !input.datum)
    {
        result.status = STML_ERROR_VACUUM_INPUT;
        redde result;
    }

    /* canonicalizatio CRLF -> LF, semel, ante omnia (§3): arbor,
     * extensiones, et contractus fidelitatis formam LF vident */
    input = _crlf_canonicalizare(input, piscina,
                                 &result.crlf_canonicalizatum);

    /* Initialize tokenizer context */
    ctx.tok_ctx.input                   = input;
    ctx.tok_ctx.positus                 = ZEPHYRUM;
    ctx.tok_ctx.linea                   = I;
    ctx.tok_ctx.columna                 = I;
    ctx.tok_ctx.in_crudus               = FALSUM;
    ctx.tok_ctx.crudus_titulus.datum    = NIHIL;
    ctx.tok_ctx.crudus_titulus.mensura  = ZEPHYRUM;
    ctx.tok_ctx.piscina                 = piscina;
    ctx.tok_ctx.intern                  = intern;

    /* Initialize parser context */
    ctx.piscina          = piscina;
    ctx.intern           = intern;
    ctx.status           = STML_SUCCESSUS;
    ctx.linea_erroris    = ZEPHYRUM;
    ctx.columna_erroris  = ZEPHYRUM;
    ctx.error.datum      = NIHIL;
    ctx.error.mensura    = ZEPHYRUM;

    /* ctx.current VACUUM adhuc est - _parser_creare_nodus campos
     * lineae/extensionis ex tokeno CURRENTE legit, et documentum
     * ante tokenum primum nascitur. Sine hac praeparatione campi
     * documenti e trunco indeterminato veniebant (mensuratum
     * 2026-08-24: aurea internus non-deterministica). Documentum
     * conventione 'non e parsatione': linea I, extensio ZEPHYRUM. */
    ctx.current.genus            = STML_TOKEN_FINIS;
    ctx.current.valor.datum      = NIHIL;
    ctx.current.valor.mensura    = ZEPHYRUM;
    ctx.current.positus_initium  = ZEPHYRUM;
    ctx.current.positus_finis    = ZEPHYRUM;
    ctx.current.linea            = I;
    ctx.current.columna          = I;
    ctx.current.attributa        = NIHIL;
    ctx.current.captio_numerus   = ZEPHYRUM;
    ctx.current.habet_captus     = FALSUM;
    ctx.finis_ultimus            = ZEPHYRUM;

    /* Create document node */
    documentum = _parser_creare_nodus(&ctx, STML_NODUS_DOCUMENTUM);
    si (!documentum)
    {
        result.status = STML_ERROR_MEMORIA;
        redde result;
    }

    documentum->liberi = xar_creare(piscina, magnitudo(StmlNodus*));

    /* Get first token */
    _parser_progredi(&ctx);

    /* Parse all top-level nodes; residuum triviae fit cauda
     * documenti (spatia_clausurae documenti, §1.1) */
    _liberos_legere(&ctx, documentum, STML_TOKEN_FINIS);

    /* Process capture operators */
    si (_habet_captio(documentum))
    {
        _processare_captiones(documentum, piscina);
    }

    /* Find first element child */
    num = xar_numerus(documentum->liberi);
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlNodus* lib = _xar_liberum_obtinere(documentum->liberi, i);
        si (lib && lib->genus == STML_NODUS_ELEMENTUM)
        {
            result.elementum_radix = lib;
            frange;
        }
    }

    /* Set result */
    result.successus        = (ctx.status == STML_SUCCESSUS);
    result.radix            = documentum;
    result.status           = ctx.status;
    result.linea_erroris    = ctx.linea_erroris;
    result.columna_erroris  = ctx.columna_erroris;
    result.error            = ctx.error;

    redde result;
}

StmlResultus
stml_legere_ex_literis (
     constans character* cstr,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    chorda input;
       i32 len;
    unio {
        constans character* cc;
                        i8* uc;
    } conv;

    si (!cstr)
    {
        StmlResultus result;
        result.successus             = FALSUM;
        result.radix                 = NIHIL;
        result.elementum_radix       = NIHIL;
        result.status                = STML_ERROR_VACUUM_INPUT;
        result.linea_erroris         = ZEPHYRUM;
        result.columna_erroris       = ZEPHYRUM;
        result.error.datum           = NIHIL;
        result.error.mensura         = ZEPHYRUM;
        result.crlf_canonicalizatum  = FALSUM;
        redde result;
    }

    len = ZEPHYRUM;
    dum (cstr[len] != '\0')
    {
        len++;
    }

    conv.cc        = cstr;
    input.datum    = conv.uc;
    input.mensura  = len;

    redde stml_legere(input, piscina, intern);
}


/* ==================================================
 * Public API - Query Functions
 * ================================================== */

StmlNodus*
stml_invenire_liberum (
             StmlNodus* nodus,
    constans character* titulus)
{
          i32  i;
          i32  num;
    StmlNodus* liberum;

    si (!nodus || !nodus->liberi || !titulus)
    {
        redde NIHIL;
    }

    num = xar_numerus(nodus->liberi);

    per (i = ZEPHYRUM; i < num; i++)
    {
        liberum = _xar_liberum_obtinere(nodus->liberi, i);
        si (   liberum
            && liberum->genus == STML_NODUS_ELEMENTUM
            && liberum->titulus
            && _chorda_ptr_aequalis_literis(liberum->titulus, titulus))
        {
            redde liberum;
        }
    }

    redde NIHIL;
}

Xar*
stml_invenire_omnes_liberos (
             StmlNodus* nodus,
    constans character* titulus,
               Piscina* piscina)
{
          Xar* result;
          i32  i;
          i32  num;
    StmlNodus* liberum;

    result = xar_creare(piscina, magnitudo(StmlNodus*));
    si (!result || !nodus || !nodus->liberi || !titulus)
    {
        redde result;
    }

    num = xar_numerus(nodus->liberi);

    per (i = ZEPHYRUM; i < num; i++)
    {
        liberum = _xar_liberum_obtinere(nodus->liberi, i);
        si (   liberum
            && liberum->genus == STML_NODUS_ELEMENTUM
            && liberum->titulus
            && _chorda_ptr_aequalis_literis(liberum->titulus, titulus))
        {
            StmlNodus** slot;
            slot = xar_addere(result);
            si (slot) *slot = liberum;
        }
    }

    redde result;
}

chorda*
stml_attributum_capere (
             StmlNodus* nodus,
    constans character* titulus)
{
               i32  i;
               i32  num;
    StmlAttributum* attr;

    si (!nodus || !nodus->attributa || !titulus)
    {
        redde NIHIL;
    }

    num = xar_numerus(nodus->attributa);

    per (i = ZEPHYRUM; i < num; i++)
    {
        attr = (StmlAttributum*)xar_obtinere(nodus->attributa, i);
        si (   attr && attr->titulus
            && _chorda_ptr_aequalis_literis(attr->titulus, titulus))
        {
            redde attr->valor;
        }
    }

    redde NIHIL;
}

b32
stml_attributum_habet (
             StmlNodus* nodus,
    constans character* titulus)
{
    redde stml_attributum_capere(nodus, titulus) != NIHIL;
}

/* Contributio LIBERI ad textum internum parentis (spec triviae
 * §2): ante + (valor textus | liberi recursivi + clausurae) +
 * post. Trivia OMNIA reassuuntur ut octeti veteres (cum spatium
 * in nodis vivebat) immoti maneant - PONS migrationis EXACTUS.
 *
 * CLAUSURAE INCLUSAE (emendatio §2, mensurata 2026-08-24): specus
 * eas excludebat ('<a>\n</a>' vacuum legendum) credens id
 * mutationem UNAM esse - aurea VI milia nodorum divergentium
 * ostenderunt (indentatio ante tagum claudentem cuiusque elementi
 * nidificati periebat). internus lectio OCTETORUM est - semantica
 * 'vacuum-cum-dispositione vacuum legitur' accessori
 * stml_textus_valor (M3) pertinet, qui margines praecidit natura
 * sua. */
interior vacuum
_internus_liberi_contribuere (
            StmlNodus* nodus,
    ChordaAedificator* aed)
{
           i32  i;
           i32  num;
     StmlNodus* liberum;

    si (nodus->liberi == NIHIL)
    {
        redde;
    }

    num = xar_numerus(nodus->liberi);
    per (i = ZEPHYRUM; i < num; i++)
    {
        liberum = _xar_liberum_obtinere(nodus->liberi, i);
        si (liberum == NIHIL)
        {
            perge;
        }

        si (liberum->spatia_ante != NIHIL)
        {
            chorda_aedificator_appendere_chorda(aed,
                *liberum->spatia_ante);
        }
        si (liberum->genus == STML_NODUS_TEXTUS && liberum->valor)
        {
            /* interius '<tag\>': praefixum (indentatio) lineis
             * reinsertum - octeti veteres exacti (§2) */
            si (nodus->multilinea && nodus->indentatio != NIHIL)
            {
                _valorem_praefixo_scribere(aed, *liberum->valor,
                    nodus->indentatio, FALSUM,
                    liberum->spatia_ante != NIHIL ? VERUM : FALSUM);
            }
            alioquin
            {
                chorda_aedificator_appendere_chorda(aed,
                    *liberum->valor);
            }
        }
        alioquin
        {
            _internus_liberi_contribuere(liberum, aed);
            si (liberum->spatia_clausurae != NIHIL)
            {
                chorda_aedificator_appendere_chorda(aed,
                    *liberum->spatia_clausurae);
            }
        }
        si (liberum->spatia_post != NIHIL)
        {
            chorda_aedificator_appendere_chorda(aed,
                *liberum->spatia_post);
        }
    }
}

chorda
stml_textus_internus (
    StmlNodus* nodus,
      Piscina* piscina)
{
    ChordaAedificator* aed;
               chorda  result;

    result.datum    = NIHIL;
    result.mensura  = ZEPHYRUM;

    si (!nodus)
    {
        redde result;
    }

    aed = chorda_aedificator_creare(piscina, CXXVIII);
    si (!aed)
    {
        redde result;
    }

    si (nodus->genus == STML_NODUS_TEXTUS)
    {
        /* nodus textus ipse quaesitus: octeti sui reassuti (ante +
         * valor + post) - pars marginis quae priori fratri cessit
         * (§1.2) hinc inattingibilis est, divergentia nominata */
        si (nodus->spatia_ante != NIHIL)
        {
            chorda_aedificator_appendere_chorda(aed,
                *nodus->spatia_ante);
        }
        si (nodus->valor != NIHIL)
        {
            si (   nodus->parens             != NIHIL
                && nodus->parens->multilinea
                && nodus->parens->indentatio != NIHIL)
            {
                _valorem_praefixo_scribere(aed, *nodus->valor,
                    nodus->parens->indentatio, FALSUM,
                    nodus->spatia_ante != NIHIL ? VERUM : FALSUM);
            }
            alioquin
            {
                chorda_aedificator_appendere_chorda(aed,
                    *nodus->valor);
            }
        }
        si (nodus->spatia_post != NIHIL)
        {
            chorda_aedificator_appendere_chorda(aed,
                *nodus->spatia_post);
        }
    }
    alioquin
    {
        _internus_liberi_contribuere(nodus, aed);
        /* clausurae propriae interiores sunt - pars octetorum
         * contenti (documento: cauda plagulae) */
        si (nodus->spatia_clausurae != NIHIL)
        {
            chorda_aedificator_appendere_chorda(aed,
                *nodus->spatia_clausurae);
        }
    }

    redde chorda_aedificator_finire(aed);
}

/* Ultra hoc contentum, clausura NOMEN retinet. Ratio: si contentum
 * tot lineas occupat, tag apertum et clausum in schermo simul non
 * sunt, ubi nomen legenti AUXILIUM est, non strepitus. Infra,
 * nomen repetitum strepitus merus est et '</>' sufficit. */
#define STML_CLAUSURA_TACITA_LINEAE XXX

/* Captura multiplex (§0.2 decretum sextum): elementum II aut III
 * liberorum intra hoc tectum altitudinis formam '((>'/'(((>'
 * capit - ultra, forma bloci cum clausura manet (Fran: "~10-15
 * lineae"; numquam ultra tres liberos). */
#define STML_CAPTURA_MULTIPLEX_LINEAE XII

/* Quot lineas contentum iam emissum occupet, ab 'initium' ad finem
 * buffri. Numeratur ex OCTETIS EMISSIS, non ex arbore: sic mensura
 * eadem est quam lector in schermo videt, et decisio a dispositione
 * pendet quam scriptor ipse modo generavit.
 *
 * PUNCTUM FIXUM: forma clausurae ('</>' an '</nomen>') numerum
 * linearum NON mutat - utraque in eadem linea sedet - ergo decisio
 * per scriptiones iteratas stabilis manet et oscillare nequit. */
interior i32
_lineae_contenti (
    ChordaAedificator* aedificator,
       memoriae_index  initium)
{
            chorda vista;
    memoriae_index k;
               i32 lineae;

    vista   = chorda_aedificator_spectare(aedificator);
    lineae  = ZEPHYRUM;

    per (k = initium; k < (memoriae_index)vista.mensura; k++)
    {
        character c;

        c = (character)vista.datum[k];
        si (c == '\n')
        {
            lineae++;
        }
    }
    redde lineae;
}

/* prototypum (definitio in sectione scriptionis) */
interior vacuum
_scribere_evasus (
    ChordaAedificator* aed,
               chorda* s);

/* Valorem '<tag\>' cum praefixo (indentatio) reinserito emittere:
 * praefixum lineis NON VACUIS solis praeponitur (spec §1.4 -
 * lineae vacuae vacuae manent), forma evasa (textus ordinarius)
 * aut cruda (interius '!\' et internus). Praefixum NIHIL =
 * emissio simplex. 'prima_quoque' FALSUM = linea prima sine
 * praefixo (contentum lineae tagi - a dedentatione exclusum erat,
 * ergo praefixum numquam tulit). */
interior vacuum
_valorem_praefixo_scribere (
    ChordaAedificator* aed,
               chorda  valor,
      constans chorda* praefixum,
                  b32  evadendum,
                  b32  prima_quoque)
{
    i32 initium;
    i32 i;

    initium = ZEPHYRUM;
    per (i = ZEPHYRUM; i <= valor.mensura; i++)
    {
        si (i == valor.mensura || (character)valor.datum[i] == '\n')
        {
            chorda pars;

            pars.datum    = valor.datum + initium;
            pars.mensura  = i - initium;
            si (pars.mensura > ZEPHYRUM)
            {
                si (   praefixum != NIHIL
                    && (prima_quoque || initium > ZEPHYRUM))
                {
                    chorda_aedificator_appendere_chorda(aed,
                        *praefixum);
                }
                si (evadendum)
                {
                    _scribere_evasus(aed, &pars);
                }
                alioquin
                {
                    chorda_aedificator_appendere_chorda(aed, pars);
                }
            }
            si (i < valor.mensura)
            {
                chorda_aedificator_appendere_character(aed, '\n');
            }
            initium = i + I;
        }
    }
}

chorda
stml_textus_normalizatus (
    StmlNodus* nodus,
      Piscina* piscina)
{
    chorda crudus;

    crudus = stml_textus_internus(nodus, piscina);
    si (crudus.mensura == ZEPHYRUM)
    {
        redde crudus;
    }

    redde _normalizare_spatium_album(crudus, piscina);
}

/* Contributio ad VALOREM (§2 M3): valores nodorum textus ordine
 * documenti, recursivi, sine triviis. Commenta et processiones
 * nihil conferunt. Genus declaratum in exemplari vivit (contentum
 * '\' dedentatum conditur, crudum verbatim) - hic nihil de
 * generibus sciendum. */
interior vacuum
_valorem_contribuere (
            StmlNodus* nodus,
    ChordaAedificator* aed)
{
           i32  i;
           i32  num;
     StmlNodus* liberum;

    si (nodus->genus == STML_NODUS_TEXTUS)
    {
        si (nodus->valor != NIHIL)
        {
            chorda_aedificator_appendere_chorda(aed, *nodus->valor);
        }
        redde;
    }
    si (   nodus->genus != STML_NODUS_ELEMENTUM
        && nodus->genus != STML_NODUS_DOCUMENTUM)
    {
        redde;
    }
    si (nodus->liberi == NIHIL)
    {
        redde;
    }
    num = xar_numerus(nodus->liberi);
    per (i = ZEPHYRUM; i < num; i++)
    {
        liberum = _xar_liberum_obtinere(nodus->liberi, i);
        si (liberum != NIHIL)
        {
            _valorem_contribuere(liberum, aed);
        }
    }
}

chorda
stml_textus_valor (
    StmlNodus* nodus,
      Piscina* piscina)
{
    ChordaAedificator* aed;
               chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (nodus == NIHIL)
    {
        redde vacua;
    }
    aed = chorda_aedificator_creare(piscina, CXXVIII);
    si (aed == NIHIL)
    {
        redde vacua;
    }
    _valorem_contribuere(nodus, aed);
    redde chorda_aedificator_finire(aed);
}

/* Octetos verbatim in fluxum appendere (contentum generis marcati
 * intra fluxum): octetus primus spatium molle debitum solvit. */
interior vacuum
_fluxum_octetos_appendere (
    ChordaAedificator* aed,
      constans chorda* valor,
                  b32* pendens,
                  b32* emissum)
{
    si (valor == NIHIL || valor->mensura == ZEPHYRUM)
    {
        redde;
    }
    si (*pendens && *emissum)
    {
        chorda_aedificator_appendere_character(aed, ' ');
    }
    *pendens = FALSUM;
    *emissum = VERUM;
    chorda_aedificator_appendere_chorda(aed, *valor);
}

/* Valorem fluminis appendere: cursus albi lineam ferentes fiunt
 * spatium molle unum (cum limitibus vicinis coalescens); cursus
 * albi eiusdem lineae LITTERALES manent (§2 - forma datorum
 * sumus). */
interior vacuum
_fluxum_valorem_appendere (
    ChordaAedificator* aed,
      constans chorda* valor,
                  b32* pendens,
                  b32* emissum)
{
    i32 i;

    si (valor == NIHIL)
    {
        redde;
    }
    i = ZEPHYRUM;
    dum (i < valor->mensura)
    {
        si (_est_spatium((character)valor->datum[i]))
        {
            i32 finis;
            b32 fert_lineam;

            finis        = i;
            fert_lineam  = FALSUM;
            dum (   finis < valor->mensura
                 && _est_spatium((character)valor->datum[finis]))
            {
                si ((character)valor->datum[finis] == '\n')
                {
                    fert_lineam = VERUM;
                }
                finis++;
            }
            si (fert_lineam)
            {
                *pendens = VERUM;
            }
            alioquin
            {
                si (*pendens && *emissum)
                {
                    chorda_aedificator_appendere_character(aed, ' ');
                }
                *pendens = FALSUM;
                *emissum = VERUM;
                dum (i < finis)
                {
                    chorda_aedificator_appendere_character(aed,
                        (character)valor->datum[i]);
                    i++;
                }
            }
            i = finis;
        }
        alioquin
        {
            si (*pendens && *emissum)
            {
                chorda_aedificator_appendere_character(aed, ' ');
            }
            *pendens = FALSUM;
            *emissum = VERUM;
            chorda_aedificator_appendere_character(aed,
                (character)valor->datum[i]);
            i++;
        }
    }
}

/* Contentum generis marcati ('\' aut crudum) in fluxum: valor
 * intactus - genus declaratum accessorem vincit (§2). */
interior vacuum
_fluxum_marcatum_contribuere (
    ChordaAedificator* aed,
            StmlNodus* nodus,
                  b32* pendens,
                  b32* emissum)
{
           i32  i;
           i32  num;
     StmlNodus* liberum;

    si (nodus->genus == STML_NODUS_TEXTUS)
    {
        _fluxum_octetos_appendere(aed, nodus->valor, pendens,
                                  emissum);
        redde;
    }
    si (   nodus->genus != STML_NODUS_ELEMENTUM
        && nodus->genus != STML_NODUS_DOCUMENTUM)
    {
        redde;
    }
    si (nodus->liberi == NIHIL)
    {
        redde;
    }
    num = xar_numerus(nodus->liberi);
    per (i = ZEPHYRUM; i < num; i++)
    {
        liberum = _xar_liberum_obtinere(nodus->liberi, i);
        si (liberum != NIHIL && !_est_commentum(liberum))
        {
            _fluxum_marcatum_contribuere(aed, liberum, pendens,
                                         emissum);
        }
    }
}

/* an trivia lineam ferant */
interior b32
_trivia_ferunt_lineam (
    constans chorda* spatia)
{
    i32 i;

    si (spatia == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < spatia->mensura; i++)
    {
        si ((character)spatia->datum[i] == '\n')
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* Fluxum elementi fluminis contribuere (§2): liberi ordine, limes
 * lineam ferens inter eos (post prioris + ante sequentis) spatium
 * molle UNUM debet; commenta contentum nullum conferunt sed
 * limites eorum numerantur (coalescentia); liberum marcatum
 * valorem intactum confert; liberum planum recursive. Trivia
 * orae (ante primi, post ultimi, clausurae) numquam spatium dant:
 * spatium molle solum inter contentum solvitur. */
interior vacuum
_fluxum_contribuere (
    ChordaAedificator* aed,
            StmlNodus* nodus,
                  b32* pendens,
                  b32* emissum)
{
           i32  i;
           i32  num;
     StmlNodus* liberum;
     StmlNodus* prior;

    si (nodus->liberi == NIHIL)
    {
        redde;
    }
    num    = xar_numerus(nodus->liberi);
    prior  = NIHIL;
    per (i = ZEPHYRUM; i < num; i++)
    {
        liberum = _xar_liberum_obtinere(nodus->liberi, i);
        si (liberum == NIHIL)
        {
            perge;
        }
        /* limes ante liberum: post prioris + ante liberi (prior
         * NIHIL = ora - trivia orae spatium nullum dant, sed
         * *emissum id iam custodit trans recursiones) */
        si (   (prior != NIHIL
                && _trivia_ferunt_lineam(prior->spatia_post))
            || _trivia_ferunt_lineam(liberum->spatia_ante))
        {
            *pendens = VERUM;
        }
        si (_est_commentum(liberum))
        {
            prior = liberum;
            perge;
        }
        si (liberum->genus == STML_NODUS_TEXTUS)
        {
            _fluxum_valorem_appendere(aed, liberum->valor, pendens,
                                      emissum);
        }
        alioquin si (liberum->genus == STML_NODUS_ELEMENTUM)
        {
            si (liberum->crudus || liberum->multilinea)
            {
                _fluxum_marcatum_contribuere(aed, liberum, pendens,
                                             emissum);
            }
            alioquin
            {
                _fluxum_contribuere(aed, liberum, pendens, emissum);
            }
        }
        prior = liberum;
    }
}

chorda
stml_textus_fluxus (
    StmlNodus* nodus,
      Piscina* piscina)
{
    ChordaAedificator* aed;
               chorda  vacua;
                  b32  pendens;
                  b32  emissum;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (nodus == NIHIL)
    {
        redde vacua;
    }
    aed = chorda_aedificator_creare(piscina, CXXVIII);
    si (aed == NIHIL)
    {
        redde vacua;
    }
    pendens = FALSUM;
    emissum = FALSUM;
    si (nodus->genus == STML_NODUS_TEXTUS)
    {
        si (   nodus->parens != NIHIL
            && (nodus->parens->crudus || nodus->parens->multilinea))
        {
            _fluxum_octetos_appendere(aed, nodus->valor, &pendens,
                                      &emissum);
        }
        alioquin
        {
            _fluxum_valorem_appendere(aed, nodus->valor, &pendens,
                                      &emissum);
        }
    }
    alioquin si (nodus->crudus || nodus->multilinea)
    {
        _fluxum_marcatum_contribuere(aed, nodus, &pendens,
                                     &emissum);
    }
    alioquin
    {
        _fluxum_contribuere(aed, nodus, &pendens, &emissum);
    }
    redde chorda_aedificator_finire(aed);
}

i32
stml_numerus_liberorum (
    StmlNodus* nodus)
{
    si (!nodus || !nodus->liberi)
    {
        redde ZEPHYRUM;
    }
    redde xar_numerus(nodus->liberi);
}

StmlNodus*
stml_liberum_ad_indicem (
    StmlNodus* nodus,
          i32  index)
{
    si (!nodus || !nodus->liberi)
    {
        redde NIHIL;
    }

    si (index >= xar_numerus(nodus->liberi))
    {
        redde NIHIL;
    }

    redde *((StmlNodus**)xar_obtinere(nodus->liberi, index));
}


/* ==================================================
 * Public API - Traversal (Navigatio)
 * ================================================== */

s32
stml_index_inter_fratres (
    StmlNodus* nodus)
{
          i32  i;
          i32  num;
    StmlNodus* liberum;

    si (!nodus || !nodus->parens || !nodus->parens->liberi)
    {
        redde -I;
    }

    num = xar_numerus(nodus->parens->liberi);
    per (i = ZEPHYRUM; i < num; i++)
    {
        liberum = *((StmlNodus**)xar_obtinere(nodus->parens->liberi, i));
        si (liberum == nodus)
        {
            redde (s32)i;
        }
    }

    redde -I;
}

StmlNodus*
stml_frater_proximus (
    StmlNodus* nodus)
{
    s32 index;
    i32 num;

    si (!nodus || !nodus->parens || !nodus->parens->liberi)
    {
        redde NIHIL;
    }

    index = stml_index_inter_fratres(nodus);
    si (index < ZEPHYRUM)
    {
        redde NIHIL;
    }

    num = xar_numerus(nodus->parens->liberi);
    si ((i32)(index + I) >= num)
    {
        redde NIHIL;  /* Iam ultimus */
    }

    redde *((StmlNodus**)xar_obtinere(nodus->parens->liberi, (i32)(index + I)));
}

StmlNodus*
stml_frater_prior (
    StmlNodus* nodus)
{
    s32 index;

    si (!nodus || !nodus->parens || !nodus->parens->liberi)
    {
        redde NIHIL;
    }

    index = stml_index_inter_fratres(nodus);
    si (index <= ZEPHYRUM)
    {
        redde NIHIL;  /* Iam primus vel non inventus */
    }

    redde *((StmlNodus**)xar_obtinere(nodus->parens->liberi, (i32)(index - I)));
}

StmlNodus*
stml_primus_liberum (
    StmlNodus* nodus)
{
    si (!nodus || !nodus->liberi || xar_numerus(nodus->liberi) == ZEPHYRUM)
    {
        redde NIHIL;
    }

    redde *((StmlNodus**)xar_obtinere(nodus->liberi, ZEPHYRUM));
}

StmlNodus*
stml_ultimus_liberum (
    StmlNodus* nodus)
{
    i32 num;

    si (!nodus || !nodus->liberi)
    {
        redde NIHIL;
    }

    num = xar_numerus(nodus->liberi);
    si (num == ZEPHYRUM)
    {
        redde NIHIL;
    }

    redde *((StmlNodus**)xar_obtinere(nodus->liberi, num - I));
}

Xar*
stml_fratres (
    StmlNodus* nodus,
      Piscina* piscina)
{
          Xar*  result;
          i32   i;
          i32   num;
    StmlNodus*  liberum;
    StmlNodus** slot;

    si (!nodus || !nodus->parens || !nodus->parens->liberi || !piscina)
    {
        redde NIHIL;
    }

    result = xar_creare(piscina, magnitudo(StmlNodus*));
    si (!result)
    {
        redde NIHIL;
    }

    num = xar_numerus(nodus->parens->liberi);
    per (i = ZEPHYRUM; i < num; i++)
    {
        liberum = *((StmlNodus**)xar_obtinere(nodus->parens->liberi, i));
        si (liberum != nodus)
        {
            slot = xar_addere(result);
            si (slot) *slot = liberum;
        }
    }

    redde result;
}

Xar*
stml_maiores (
    StmlNodus* nodus,
      Piscina* piscina)
{
          Xar*  result;
    StmlNodus*  currens;
    StmlNodus** slot;

    si (!nodus || !piscina)
    {
        redde NIHIL;
    }

    result = xar_creare(piscina, magnitudo(StmlNodus*));
    si (!result)
    {
        redde NIHIL;
    }

    currens = nodus->parens;
    dum (currens != NIHIL)
    {
        slot = xar_addere(result);
        si (slot) *slot = currens;
        currens = currens->parens;
    }

    redde result;
}

StmlNodus*
stml_proximus_maior (
              StmlNodus* nodus,
     constans character* selector,
                Piscina* piscina,
    InternamentumChorda* intern)
{
           StmlNodus* currens;
    SelectioResultus  res;

    si (!nodus || !selector || !piscina || !intern)
    {
        redde NIHIL;
    }

    /* Legere selectorem */
    res = selectio_legere_ex_literis(selector, piscina, intern);
    si (!res.successus || !res.selectio)
    {
        redde NIHIL;
    }

    /* Verificare se et omnes maiores */
    currens = nodus;
    dum (currens != NIHIL)
    {
        si (selectio_congruit(res.selectio, currens))
        {
            redde currens;
        }
        currens = currens->parens;
    }

    redde NIHIL;
}


/* ==================================================
 * Internal Helpers - Array Manipulation
 * ================================================== */

/* Remove element at index from liberi array (creates new array) */
interior Xar*
_liberi_removere_ad (
        Xar* liberi,
        i32  index,
    Piscina* piscina)
{
          Xar*  novum;
          i32   i;
          i32   num;
    StmlNodus*  liberum;
    StmlNodus** slot;

    si (!liberi || !piscina)
    {
        redde NIHIL;
    }

    novum = xar_creare(piscina, magnitudo(StmlNodus*));
    si (!novum)
    {
        redde NIHIL;
    }

    num = xar_numerus(liberi);
    per (i = ZEPHYRUM; i < num; i++)
    {
        si (i != index)
        {
            liberum  = *((StmlNodus**)xar_obtinere(liberi, i));
            slot     = xar_addere(novum);
            si (slot) *slot = liberum;
        }
    }

    redde novum;
}

/* Insert element at index in liberi array (creates new array) */
interior Xar*
_liberi_inserere_ad (
          Xar* liberi,
          i32  index,
    StmlNodus* novum_elem,
      Piscina* piscina)
{
          Xar*  nova_xar;
          i32   i;
          i32   num;
    StmlNodus*  liberum;
    StmlNodus** slot;

    si (!piscina || !novum_elem)
    {
        redde NIHIL;
    }

    nova_xar = xar_creare(piscina, magnitudo(StmlNodus*));
    si (!nova_xar)
    {
        redde NIHIL;
    }

    num = liberi ? xar_numerus(liberi) : ZEPHYRUM;

    per (i = ZEPHYRUM; i < num; i++)
    {
        si (i == index)
        {
            slot = xar_addere(nova_xar);
            si (slot) *slot = novum_elem;
        }
        liberum  = *((StmlNodus**)xar_obtinere(liberi, i));
        slot     = xar_addere(nova_xar);
        si (slot) *slot = liberum;
    }

    /* Si index >= num, addere ad finem */
    si (index >= num)
    {
        slot = xar_addere(nova_xar);
        si (slot) *slot = novum_elem;
    }

    redde nova_xar;
}


/* ==================================================
 * Public API - Mutation
 * ================================================== */

b32
stml_praeponere (
    StmlNodus* parens,
    StmlNodus* liberum,
      Piscina* piscina)
{
    Xar* novi_liberi;

    si (!parens || !liberum || !piscina)
    {
        redde FALSUM;
    }

    /* Si parens non habet liberos, creare */
    si (!parens->liberi)
    {
        parens->liberi = xar_creare(piscina, magnitudo(StmlNodus*));
        si (!parens->liberi)
        {
            redde FALSUM;
        }
    }

    novi_liberi = _liberi_inserere_ad(parens->liberi, ZEPHYRUM, liberum, piscina);
    si (!novi_liberi)
    {
        redde FALSUM;
    }

    parens->liberi   = novi_liberi;
    liberum->parens  = parens;

    redde VERUM;
}

b32
stml_removere (
    StmlNodus* nodus,
      Piscina* piscina)
{
          s32  index;
          Xar* novi_liberi;
    StmlNodus* parens;

    si (!nodus || !nodus->parens || !piscina)
    {
        redde FALSUM;
    }

    parens  = nodus->parens;
    index   = stml_index_inter_fratres(nodus);
    si (index < ZEPHYRUM)
    {
        redde FALSUM;
    }

    novi_liberi = _liberi_removere_ad(parens->liberi, (i32)index, piscina);
    si (!novi_liberi)
    {
        redde FALSUM;
    }

    parens->liberi  = novi_liberi;
    nodus->parens   = NIHIL;

    redde VERUM;
}

vacuum
stml_vacare_liberos (
    StmlNodus* nodus)
{
          i32  i;
          i32  num;
    StmlNodus* liberum;

    si (!nodus || !nodus->liberi)
    {
        redde;
    }

    /* Ponere parens ad NIHIL pro omnibus liberis */
    num = xar_numerus(nodus->liberi);
    per (i = ZEPHYRUM; i < num; i++)
    {
        liberum = *((StmlNodus**)xar_obtinere(nodus->liberi, i));
        si (liberum)
        {
            liberum->parens = NIHIL;
        }
    }

    /* Vacare xar (segmenta manent, numerus = 0) */
    xar_vacare(nodus->liberi);
}

b32
stml_inserere_ante (
    StmlNodus* nodus,
    StmlNodus* novum,
      Piscina* piscina)
{
          s32  index;
          Xar* novi_liberi;
    StmlNodus* parens;

    si (!nodus || !nodus->parens || !novum || !piscina)
    {
        redde FALSUM;
    }

    parens  = nodus->parens;
    index   = stml_index_inter_fratres(nodus);
    si (index < ZEPHYRUM)
    {
        redde FALSUM;
    }

    novi_liberi = _liberi_inserere_ad(parens->liberi, (i32)index, novum, piscina);
    si (!novi_liberi)
    {
        redde FALSUM;
    }

    parens->liberi  = novi_liberi;
    novum->parens   = parens;

    redde VERUM;
}

b32
stml_inserere_post (
    StmlNodus* nodus,
    StmlNodus* novum,
      Piscina* piscina)
{
          s32  index;
          Xar* novi_liberi;
    StmlNodus* parens;

    si (!nodus || !nodus->parens || !novum || !piscina)
    {
        redde FALSUM;
    }

    parens  = nodus->parens;
    index   = stml_index_inter_fratres(nodus);
    si (index < ZEPHYRUM)
    {
        redde FALSUM;
    }

    novi_liberi = _liberi_inserere_ad(parens->liberi, (i32)(index + I), novum, piscina);
    si (!novi_liberi)
    {
        redde FALSUM;
    }

    parens->liberi  = novi_liberi;
    novum->parens   = parens;

    redde VERUM;
}

b32
stml_substituere (
    StmlNodus* vetus,
    StmlNodus* novum,
      Piscina* piscina)
{
          s32   index;
    StmlNodus*  parens;
    StmlNodus** slot;

    si (!vetus || !vetus->parens || !novum || !piscina)
    {
        redde FALSUM;
    }

    parens  = vetus->parens;
    index   = stml_index_inter_fratres(vetus);
    si (index < ZEPHYRUM)
    {
        redde FALSUM;
    }

    /* Directe substituere in array */
    slot = (StmlNodus**)xar_obtinere(parens->liberi, (i32)index);
    si (!slot)
    {
        redde FALSUM;
    }

    *slot          = novum;
    novum->parens  = parens;
    vetus->parens  = NIHIL;

    (vacuum)piscina;  /* Suppressio moniti - piscina non utitur hic */

    redde VERUM;
}


/* ==================================================
 * Public API - Cloning
 * ================================================== */

interior StmlNodus*
_duplicare_recursivum (
              StmlNodus* nodus,
                Piscina* piscina,
    InternamentumChorda* intern,
                    b32  profundum)
{
    StmlNodus* novum;
          i32  i;
          i32  num;

    si (!nodus || !piscina || !intern)
    {
        redde NIHIL;
    }

    novum = (StmlNodus*)piscina_allocare(piscina, magnitudo(StmlNodus));
    si (!novum)
    {
        redde NIHIL;
    }

    /* Copiare campos basicos */
    novum->genus             = nodus->genus;
    novum->titulus           = nodus->titulus;  /* Iam internatum */
    novum->valor             = nodus->valor;      /* Iam internatum */
    novum->crudus            = nodus->crudus;
    novum->captio_directio   = nodus->captio_directio;
    novum->captio_numerus    = nodus->captio_numerus;
    novum->clausura_anonyma  = nodus->clausura_anonyma;
    novum->linea             = nodus->linea;
    /* fragmentum/augmentum: olim NON copiabantur - duplicatum
     * fragmenti elementum ordinarium '#' tacite fiebat (piscina
     * zephyrata culpam texit). Explicite (2026-08-10). */
    novum->fragmentum        = nodus->fragmentum;
    novum->fragmentum_id     = nodus->fragmentum_id;  /* Internatum */
    novum->augmentum_clavis  = nodus->augmentum_clavis;  /* Internatum */
    novum->parens            = NIHIL;  /* Novum non habet parentem */
    /* trivia copiantur (internata) - subarbor duplicata
     * dispositionem suam secum fert */
    novum->spatia_ante         = nodus->spatia_ante;
    novum->spatia_post         = nodus->spatia_post;
    novum->spatia_clausurae    = nodus->spatia_clausurae;
    novum->spatia_intra_tagum  = nodus->spatia_intra_tagum;
    novum->multilinea          = nodus->multilinea;
    novum->indentatio          = nodus->indentatio;
    /* linea/extensio: metadatum parsationis - duplicatum non e
     * parsatione venit */
    novum->positus_initium  = ZEPHYRUM;
    novum->positus_finis    = ZEPHYRUM;

    /* Copiare attributa */
    si (nodus->attributa && xar_numerus(nodus->attributa) > ZEPHYRUM)
    {
        novum->attributa = xar_creare(piscina, magnitudo(StmlAttributum));
        si (novum->attributa)
        {
            num = xar_numerus(nodus->attributa);
            per (i = ZEPHYRUM; i < num; i++)
            {
                StmlAttributum* attr_orig;
                StmlAttributum* attr_new;

                attr_orig = (StmlAttributum*)xar_obtinere(nodus->attributa, i);
                attr_new = (StmlAttributum*)xar_addere(novum->attributa);
                si (attr_new && attr_orig)
                {
                    attr_new->titulus      = attr_orig->titulus;  /* Internatum */
                    attr_new->valor        = attr_orig->valor;      /* Internatum */
                    attr_new->spatia_ante  = attr_orig->spatia_ante;
                }
            }
        }
    }
    alioquin
    {
        novum->attributa = NIHIL;
    }

    /* Copiare liberos (si profundum) */
    si (profundum && nodus->liberi && xar_numerus(nodus->liberi) > ZEPHYRUM)
    {
        novum->liberi = xar_creare(piscina, magnitudo(StmlNodus*));
        si (novum->liberi)
        {
            num = xar_numerus(nodus->liberi);
            per (i = ZEPHYRUM; i < num; i++)
            {
                StmlNodus*  liberum_orig;
                StmlNodus*  liberum_novum;
                StmlNodus** slot;

                liberum_orig = *((StmlNodus**)xar_obtinere(nodus->liberi, i));
                liberum_novum = _duplicare_recursivum(liberum_orig, piscina, intern, VERUM);
                si (liberum_novum)
                {
                    liberum_novum->parens  = novum;
                    slot                   = xar_addere(novum->liberi);
                    si (slot) *slot = liberum_novum;
                }
            }
        }
    }
    alioquin
    {
        novum->liberi = NIHIL;
    }

    redde novum;
}

StmlNodus*
stml_duplicare (
              StmlNodus* nodus,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    redde _duplicare_recursivum(nodus, piscina, intern, VERUM);
}

StmlNodus*
stml_duplicare_superficialiter (
              StmlNodus* nodus,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    redde _duplicare_recursivum(nodus, piscina, intern, FALSUM);
}


/* ==================================================
 * Public API - Node Creation
 * ================================================== */

StmlNodus*
stml_elementum_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* titulus)
{
    StmlNodus* nodus;

    nodus = (StmlNodus*)piscina_allocare(piscina, magnitudo(StmlNodus));
    si (!nodus)
    {
        redde NIHIL;
    }

    nodus->genus = STML_NODUS_ELEMENTUM;
    nodus->titulus = chorda_internare_ex_literis(intern, titulus);
    nodus->valor = NIHIL;
    nodus->attributa = xar_creare(piscina, magnitudo(StmlAttributum));
    nodus->liberi = xar_creare(piscina, magnitudo(StmlNodus*));
    nodus->parens = NIHIL;
    nodus->crudus = FALSUM;
    nodus->captio_directio = STML_CAPTIO_NIHIL;
    nodus->captio_numerus = ZEPHYRUM;
    nodus->clausura_anonyma = FALSUM;
    nodus->fragmentum = FALSUM;
    nodus->fragmentum_id = NIHIL;
    nodus->augmentum_clavis = NIHIL;
    nodus->linea = ZEPHYRUM;   /* non e parsatione */
    nodus->positus_initium = ZEPHYRUM;
    nodus->positus_finis = ZEPHYRUM;
    nodus->spatia_ante = NIHIL;
    nodus->spatia_post = NIHIL;
    nodus->spatia_clausurae = NIHIL;
    nodus->spatia_intra_tagum = NIHIL;
    nodus->multilinea = FALSUM;
    nodus->indentatio = NIHIL;

    redde nodus;
}

StmlNodus*
stml_elementum_crudum_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* titulus)
{
    StmlNodus* nodus;

    nodus = stml_elementum_creare(piscina, intern, titulus);
    si (nodus)
    {
        nodus->crudus = VERUM;
    }

    redde nodus;
}

StmlNodus*
stml_textum_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* textus)
{
    StmlNodus* nodus;

    nodus = (StmlNodus*)piscina_allocare(piscina, magnitudo(StmlNodus));
    si (!nodus)
    {
        redde NIHIL;
    }

    nodus->genus = STML_NODUS_TEXTUS;
    nodus->titulus = NIHIL;
    nodus->valor = chorda_internare_ex_literis(intern, textus);
    nodus->attributa = NIHIL;
    nodus->liberi = NIHIL;
    nodus->parens = NIHIL;
    nodus->crudus = FALSUM;
    nodus->captio_directio = STML_CAPTIO_NIHIL;
    nodus->captio_numerus = ZEPHYRUM;
    nodus->clausura_anonyma = FALSUM;
    nodus->fragmentum = FALSUM;
    nodus->fragmentum_id = NIHIL;
    nodus->augmentum_clavis = NIHIL;
    nodus->linea = ZEPHYRUM;   /* non e parsatione */
    nodus->positus_initium = ZEPHYRUM;
    nodus->positus_finis = ZEPHYRUM;
    nodus->spatia_ante = NIHIL;
    nodus->spatia_post = NIHIL;
    nodus->spatia_clausurae = NIHIL;
    nodus->spatia_intra_tagum = NIHIL;
    nodus->multilinea = FALSUM;
    nodus->indentatio = NIHIL;

    redde nodus;
}

StmlNodus*
stml_textum_creare_ex_chorda (
                Piscina* piscina,
    InternamentumChorda* intern,
                 chorda  textus)
{
    StmlNodus* nodus;

    nodus = (StmlNodus*)piscina_allocare(piscina, magnitudo(StmlNodus));
    si (!nodus)
    {
        redde NIHIL;
    }

    nodus->genus               = STML_NODUS_TEXTUS;
    nodus->titulus             = NIHIL;
    nodus->valor               = chorda_internare(intern, textus);
    nodus->attributa           = NIHIL;
    nodus->liberi              = NIHIL;
    nodus->parens              = NIHIL;
    nodus->crudus              = FALSUM;
    nodus->captio_directio     = STML_CAPTIO_NIHIL;
    nodus->captio_numerus      = ZEPHYRUM;
    nodus->clausura_anonyma    = FALSUM;
    nodus->fragmentum          = FALSUM;
    nodus->fragmentum_id       = NIHIL;
    nodus->augmentum_clavis    = NIHIL;
    nodus->linea               = ZEPHYRUM;   /* non e parsatione */
    nodus->positus_initium     = ZEPHYRUM;
    nodus->positus_finis       = ZEPHYRUM;
    nodus->spatia_ante         = NIHIL;
    nodus->spatia_post         = NIHIL;
    nodus->spatia_clausurae    = NIHIL;
    nodus->spatia_intra_tagum  = NIHIL;
    nodus->multilinea          = FALSUM;
    nodus->indentatio          = NIHIL;

    redde nodus;
}

StmlNodus*
stml_commentum_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* textus)
{
    StmlNodus* nodus;

    nodus = (StmlNodus*)piscina_allocare(piscina, magnitudo(StmlNodus));
    si (!nodus)
    {
        redde NIHIL;
    }

    nodus->genus = STML_NODUS_COMMENTUM;
    nodus->titulus = NIHIL;
    nodus->valor = chorda_internare_ex_literis(intern, textus);
    nodus->attributa = NIHIL;
    nodus->liberi = NIHIL;
    nodus->parens = NIHIL;
    nodus->crudus = FALSUM;
    nodus->captio_directio = STML_CAPTIO_NIHIL;
    nodus->captio_numerus = ZEPHYRUM;
    nodus->clausura_anonyma = FALSUM;
    nodus->fragmentum = FALSUM;
    nodus->fragmentum_id = NIHIL;
    nodus->augmentum_clavis = NIHIL;
    nodus->linea = ZEPHYRUM;   /* non e parsatione */
    nodus->positus_initium = ZEPHYRUM;
    nodus->positus_finis = ZEPHYRUM;
    nodus->spatia_ante = NIHIL;
    nodus->spatia_post = NIHIL;
    nodus->spatia_clausurae = NIHIL;
    nodus->spatia_intra_tagum = NIHIL;
    nodus->multilinea = FALSUM;
    nodus->indentatio = NIHIL;

    redde nodus;
}

b32
stml_attributum_addere (
              StmlNodus* nodus,
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* titulus,
     constans character* valor)
{
    StmlAttributum* attr;

    (vacuum)piscina;

    si (!nodus || !nodus->attributa)
    {
        redde FALSUM;
    }

    attr = xar_addere(nodus->attributa);
    si (!attr)
    {
        redde FALSUM;
    }

    attr->titulus      = chorda_internare_ex_literis(intern, titulus);
    attr->valor        = chorda_internare_ex_literis(intern, valor);
    attr->spatia_ante  = NIHIL;

    redde VERUM;
}

b32
stml_attributum_boolean_addere (
              StmlNodus* nodus,
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* titulus)
{
    redde stml_attributum_addere(nodus, piscina, intern, titulus, "true");
}

b32
stml_attributum_addere_chorda (
              StmlNodus* nodus,
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* titulus,
                 chorda  valor)
{
    StmlAttributum* attr;

    (vacuum)piscina;

    si (!nodus || !nodus->attributa)
    {
        redde FALSUM;
    }

    attr = xar_addere(nodus->attributa);
    si (!attr)
    {
        redde FALSUM;
    }

    attr->titulus      = chorda_internare_ex_literis(intern, titulus);
    attr->valor        = chorda_internare(intern, valor);
    attr->spatia_ante  = NIHIL;

    redde VERUM;
}

b32
stml_liberum_addere (
    StmlNodus* parens,
    StmlNodus* liberum)
{
    StmlNodus** slot;

    si (!parens || !liberum)
    {
        redde FALSUM;
    }

    si (!parens->liberi)
    {
        redde FALSUM;
    }

    slot = xar_addere(parens->liberi);
    si (!slot)
    {
        redde FALSUM;
    }

    *slot            = liberum;
    liberum->parens  = parens;

    redde VERUM;
}

b32
stml_textum_addere (
              StmlNodus* parens,
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* textus)
{
    StmlNodus* textus_nodus;

    textus_nodus = stml_textum_creare(piscina, intern, textus);
    si (!textus_nodus)
    {
        redde FALSUM;
    }

    redde stml_liberum_addere(parens, textus_nodus);
}


/* ==================================================
 * Public API - Serialization
 * ================================================== */

interior vacuum
_scribere_indentatio (
    ChordaAedificator* aed,
                  i32  nivel)
{
    i32 i;
    per (i = ZEPHYRUM; i < nivel; i++)
    {
        chorda_aedificator_appendere_literis(aed, "  ");
    }
}

interior vacuum
_scribere_evasus (
    ChordaAedificator* aed,
               chorda* s)
{
          i32 i;
    character c;

    si (!s)
    {
        redde;
    }

    per (i = ZEPHYRUM; i < s->mensura; i++)
    {
        c = (character)s->datum[i];

        commutatio (c)
        {
            casus '<':
                chorda_aedificator_appendere_literis(aed, "&lt;");
                frange;
            casus '>':
                chorda_aedificator_appendere_literis(aed, "&gt;");
                frange;
            casus '&':
                chorda_aedificator_appendere_literis(aed, "&amp;");
                frange;
            casus '"':
                chorda_aedificator_appendere_literis(aed, "&quot;");
                frange;
            ordinarius:
                chorda_aedificator_appendere_character(aed, c);
                frange;
        }
    }
}

/* Nucleus scriptionis - UNUS ambulator recursivus (lex superficiei
 * duplicatae: numquam alter). 'sedes' optionalis (NIHIL licet):
 * Xar de StmlSedesNodi, elementum quodque notatum ubi scriptio
 * eius FINITUR (post-ordo), extensione [initium, finis) in
 * aedificatore - semantica positus parsatoris.
 *
 * DUO VEXILLA, DUAE QUAESTIONES (2026-08-24): 'pulchrum' de
 * DISPOSITIONE respondet (indentatio, lineae novae) et in arte
 * contenti mixti liberis EXSTINGUITUR; 'fidelitas' de TRIVIIS
 * respondet (reassemblatio §4) et per recursionem totam IMMOTUM
 * fluit. Olim vexillum unum ambas quaestiones ferebat - liberus
 * arte vocatus ('ne te indentes') trivia sua falso emittebat. */

/* Attributa emittere - superficies UNA (quinquies duplicata erat,
 * lex superficiei duplicatae). Fidelitas: spatium notatum ante
 * attributum (§1.6) redditur; aliter spatium unicum canonicum.
 * Attributa boolean ="true" non emittunt; spatium circa '='
 * normalizatum manet (exceptio angusta §1.6). */
interior vacuum
_attributa_scribere (
    ChordaAedificator* aedificator,
            StmlNodus* nodus,
                  b32  fidelitas)
{
               i32  i;
               i32  num;
    StmlAttributum* attr;

    si (nodus->attributa == NIHIL)
    {
        redde;
    }

    num = xar_numerus(nodus->attributa);
    per (i = ZEPHYRUM; i < num; i++)
    {
        attr = (StmlAttributum*)xar_obtinere(nodus->attributa, i);
        si (attr == NIHIL || attr->titulus == NIHIL)
        {
            perge;
        }
        si (fidelitas && attr->spatia_ante != NIHIL)
        {
            chorda_aedificator_appendere_chorda(aedificator,
                *attr->spatia_ante);
        }
        alioquin
        {
            chorda_aedificator_appendere_character(aedificator, ' ');
        }
        chorda_aedificator_appendere_chorda(aedificator,
            *attr->titulus);
        si (   attr->valor
            && !_chorda_ptr_aequalis_literis(attr->valor, "true"))
        {
            chorda_aedificator_appendere_literis(aedificator, "=\"");
            chorda_aedificator_appendere_chorda(aedificator,
                *attr->valor);
            chorda_aedificator_appendere_character(aedificator, '"');
        }
    }
}

/* Tectum lineae pulchrae (§0.2 decretum tertium): LXXII planum,
 * sub indentatione gravi minimum XL columnarum contenti manet -
 * sine fundo fracturae profundae in scalas cumularentur */
interior i32
_tectum_lineae (
    i32 gradus)
{
    i32 tectum;

    tectum = gradus * II + XL;
    si (tectum < LXXII)
    {
        tectum = LXXII;
    }
    redde tectum;
}

/* an attributa visibilia adsint (titulus praesens) */
interior b32
_attributa_adsunt (
    StmlNodus* nodus)
{
    i32 i;
    i32 num;

    si (nodus->attributa == NIHIL)
    {
        redde FALSUM;
    }
    num = xar_numerus(nodus->attributa);
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlAttributum* attr;

        attr = (StmlAttributum*)xar_obtinere(nodus->attributa, i);
        si (attr != NIHIL && attr->titulus != NIHIL)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* Latitudo tagi inline ('<' per attributa, SINE clausura) -
 * arithmetica pura: valores attributorum lineam novam numquam
 * ferunt (§0.2 decretum quintum) */
interior i32
_tagum_latitudo_inline (
    StmlNodus* nodus)
{
    i32 latitudo;
    i32 i;
    i32 num;

    latitudo = I;
    si (nodus->titulus != NIHIL)
    {
        latitudo += nodus->titulus->mensura;
    }
    si (nodus->crudus)
    {
        latitudo += I;
    }
    si (nodus->multilinea)
    {
        latitudo += I;
    }
    si (nodus->attributa != NIHIL)
    {
        num = xar_numerus(nodus->attributa);
        per (i = ZEPHYRUM; i < num; i++)
        {
            StmlAttributum* attr;

            attr = (StmlAttributum*)xar_obtinere(nodus->attributa,
                                                 i);
            si (attr == NIHIL || attr->titulus == NIHIL)
            {
                perge;
            }
            latitudo += I + attr->titulus->mensura;
            si (   attr->valor != NIHIL
                && !_chorda_ptr_aequalis_literis(attr->valor,
                                                 "true"))
            {
                latitudo += III + attr->valor->mensura;
            }
        }
    }
    redde latitudo;
}

/* Columna '=' formae multilineae (§0.2 decretum quintum): post
 * titulum tagi cadit (attributa sub tago conduntur), margine
 * minimo duorum spatiorum a '<'; nomen longissimum aut clausura
 * capturae lata (clausura_lata = numerus parenthesium) columnam
 * dextrorsum trudit quantum necesse */
interior i32
_attributorum_columna (
    StmlNodus* nodus,
          i32  basis,
          i32  clausura_lata)
{
    i32 columna;
    i32 candidata;
    i32 i;
    i32 num;

    columna = basis + I;
    si (nodus->titulus != NIHIL)
    {
        columna += nodus->titulus->mensura;
    }
    si (nodus->crudus)
    {
        columna += I;
    }
    si (nodus->multilinea)
    {
        columna += I;
    }
    si (nodus->attributa != NIHIL)
    {
        num = xar_numerus(nodus->attributa);
        per (i = ZEPHYRUM; i < num; i++)
        {
            StmlAttributum* attr;

            attr = (StmlAttributum*)xar_obtinere(nodus->attributa,
                                                 i);
            si (attr == NIHIL || attr->titulus == NIHIL)
            {
                perge;
            }
            candidata = basis + II + attr->titulus->mensura;
            si (candidata > columna)
            {
                columna = candidata;
            }
        }
    }
    candidata = basis + II + clausura_lata;
    si (candidata > columna)
    {
        columna = candidata;
    }
    redde columna;
}

interior vacuum
_spatia_scribere (
    ChordaAedificator* aed,
                  i32  quantum)
{
    i32 i;

    per (i = ZEPHYRUM; i < quantum; i++)
    {
        chorda_aedificator_appendere_character(aed, ' ');
    }
}

/* Attributa multilinea scribere (§0.2 decretum quintum):
 * attributum per lineam, nomina dextro-alineata in columnam '='
 * communem. Vocans praefixum tagi ('<titulus' cum sigillis) iam
 * emisit; clausuram glutinantem ('>', '/>') vocans post appendit,
 * clausuram capturae (clausura_lata > ZEPHYRUM) haec functio
 * linea propria emittit - '>' in columna '='. Columnam reddit
 * (linea clausurae basis mensurae nova sarcinatori). */
interior i32
_attributa_multilinea_scribere (
    ChordaAedificator* aedificator,
            StmlNodus* nodus,
                  i32  basis,
                  i32  clausura_lata)
{
    i32 columna;
    i32 i;
    i32 num;

    columna = _attributorum_columna(nodus, basis, clausura_lata);
    num     = (nodus->attributa != NIHIL)
        ? xar_numerus(nodus->attributa) : ZEPHYRUM;
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlAttributum* attr;

        attr = (StmlAttributum*)xar_obtinere(nodus->attributa, i);
        si (attr == NIHIL || attr->titulus == NIHIL)
        {
            perge;
        }
        chorda_aedificator_appendere_character(aedificator, '\n');
        _spatia_scribere(aedificator,
                         columna - attr->titulus->mensura);
        chorda_aedificator_appendere_chorda(aedificator,
                                            *attr->titulus);
        si (   attr->valor != NIHIL
            && !_chorda_ptr_aequalis_literis(attr->valor, "true"))
        {
            chorda_aedificator_appendere_literis(aedificator,
                                                 "=\"");
            chorda_aedificator_appendere_chorda(aedificator,
                                                *attr->valor);
            chorda_aedificator_appendere_character(aedificator,
                                                   '"');
        }
    }
    si (clausura_lata > ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(aedificator, '\n');
        _spatia_scribere(aedificator, columna - clausura_lata);
        per (i = ZEPHYRUM; i < clausura_lata; i++)
        {
            chorda_aedificator_appendere_character(aedificator,
                                                   '(');
        }
        chorda_aedificator_appendere_character(aedificator, '>');
    }
    redde columna;
}

/* an tagum formam multilineam capiat (§0.2 decretum quintum):
 * attributa adsunt et redditio inline (clausura inclusa) tectum
 * gradus fallit. Positione bloci sola vocatur (pulchrum VERUM) -
 * liberi inline mediis lineis numquam franguntur. */
interior b32
_attributa_multilinea_oportet (
    StmlNodus* nodus,
          i32  indentatio,
          i32  latitudo_clausurae)
{
    si (!_attributa_adsunt(nodus))
    {
        redde FALSUM;
    }
    redde (b32)(indentatio * II
                    + _tagum_latitudo_inline(nodus)
                    + latitudo_clausurae
                > _tectum_lineae(indentatio));
}

/* an nodus textus interius elementi '<tag\>' sit - dispositio
 * eius DECLARATA est (§1.4), ambobus modis emittitur */
interior b32
_intra_multilineam (
    constans StmlNodus* nodus)
{
    redde (b32)(nodus->genus == STML_NODUS_TEXTUS
        && nodus->parens != NIHIL
        && nodus->parens->multilinea);
}

/* an elementum liberum textus quodvis ferat - decisio dispositionis
 * M2 (§4): textus praesens -> INLINE, aliter BLOCUS */
interior b32
_habet_liberum_textus (
    constans StmlNodus* nodus)
{
    i32 i;
    i32 num;

    si (nodus->liberi == NIHIL)
    {
        redde FALSUM;
    }
    num = xar_numerus(nodus->liberi);
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlNodus* liberum;

        liberum = _xar_liberum_obtinere(nodus->liberi, i);
        si (liberum != NIHIL && liberum->genus == STML_NODUS_TEXTUS)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* Interius '\' modo pulchro (§4 M2): lineas valoris ad nivel
 * REGENERATUM indentare - praefixum conditum ignoratur, ergo nodi
 * moti recte reformantur. Linea 0 sine praefixo novo si in linea
 * tagi sedet (prima_quoque FALSUM); lineae vacuae sine praefixo
 * (forma normativa §1.4 - reassemblatio praefixum lineis non-vacuis
 * solis inserit). */
interior vacuum
_valorem_pulchre_indentare (
     ChordaAedificator* aed,
                chorda  valor,
                   i32  nivel,
                   b32  evadendum,
                   b32  prima_quoque)
{
    i32 initium;
    i32 i;
    b32 prima;

    initium  = ZEPHYRUM;
    prima    = VERUM;
    per (i = ZEPHYRUM; i <= valor.mensura; i++)
    {
        si (i == valor.mensura || (character)valor.datum[i] == '\n')
        {
            chorda linea;

            linea.datum    = valor.datum + initium;
            linea.mensura  = i - initium;

            si (!prima || prima_quoque)
            {
                chorda_aedificator_appendere_character(aed, '\n');
                si (linea.mensura > ZEPHYRUM)
                {
                    _scribere_indentatio(aed, nivel);
                }
            }
            si (linea.mensura > ZEPHYRUM)
            {
                si (evadendum)
                {
                    _scribere_evasus(aed, &linea);
                }
                alioquin
                {
                    chorda_aedificator_appendere_chorda(aed, linea);
                }
            }
            prima    = FALSUM;
            initium  = i + I;
        }
    }
}


/* ==================================================
 * COLLAPSUS SPINAE (§4, T3c 2026-08-24)
 *
 * Pulcher spinam unigenam in formam capturae in linea una
 * convertit - '<t1(> <t2(> <t3(> foo' - si intra tectum LXXII
 * columnarum ab indentatione currenti cadit (totum-aut-nihil,
 * §0.2). Redditio optimistica in aedificatorem principalem;
 * reversio per truncare (aedificator + tabula sedium). Forma
 * capturae memoria formae est: non-pulcher formam authoris
 * octetim servat, pulcher re-derivat - doctrina clausurae
 * tacitae capturis adhibita. Post redditum, transitus pulchri
 * sequentes formam per ramum captoris servant (separator
 * canonicus ' ' pro captoribus unigenis) - punctum fixum tenet.
 * ================================================== */

interior b32
_scribere_nucleus (
            StmlNodus* nodus,
    ChordaAedificator* aedificator,
                  b32  pulchrum,
                  b32  fidelitas,
                  i32  indentatio,
                  Xar* sedes);

/* an valor lineam novam non ferat */
interior b32
_valor_unilinearis (
    constans chorda* valor)
{
    i32 i;

    si (valor == NIHIL)
    {
        redde VERUM;
    }
    per (i = ZEPHYRUM; i < valor->mensura; i++)
    {
        si ((character)valor->datum[i] == '\n')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* an valor textus capi aut reflui possit (§4 M3): non vacuus,
 * margines MUNDI - octetus primus ET ultimus non albi. Margo
 * ducens sordidus in relectione ad post captoris migraret (regula
 * capturae), caudalis in trivia sequentia praecideretur - valor
 * mutaretur et fluxus cum eo; valor solum-albus ('<sep>   </sep>')
 * totus periret. Cauda alba = foramen M2b latens, numquam morsum,
 * nunc pinnatum. Multilinearis LICET (M3): emissio lectionem
 * IUNCTAM scribit - cursus lineam ferentes spatium unum sunt. */
interior b32
_valor_capturabilis (
    constans chorda* valor)
{
    si (   valor          == NIHIL
        || valor->mensura == ZEPHYRUM
        || _est_spatium((character)valor->datum[ZEPHYRUM])
        || _est_spatium((character)
               valor->datum[valor->mensura - I]))
    {
        redde FALSUM;
    }
    redde VERUM;
}

/* Liberum unicum vinculi spinae reddere; NIHIL si nodus vinculum
 * non est. Vinculum = elementum sine fragmento/augmento, non
 * crudum nec multilineum, liberum unicum non-commentum ferens -
 * aut sine captura (forma convertitur) aut captor ANTE unigenis
 * (M2b: dispositio auctoris via eadem re-derivatur, unificatio
 * §4). Multi-parentheses angulis M4 relinquuntur. */
interior StmlNodus*
_spinae_liberum_unicum (
    constans StmlNodus* nodus)
{
    StmlNodus* liberum;

    si (   nodus->genus               != STML_NODUS_ELEMENTUM
        || nodus->fragmentum
        || nodus->augmentum_clavis    != NIHIL
        || nodus->crudus
        || nodus->multilinea
        || nodus->liberi              == NIHIL
        || xar_numerus(nodus->liberi) != I)
    {
        redde NIHIL;
    }
    si (   nodus->captio_directio != STML_CAPTIO_NIHIL
        && !(   nodus->captio_directio == STML_CAPTIO_ANTE
             && nodus->captio_numerus == I))
    {
        redde NIHIL;
    }
    liberum = _xar_liberum_obtinere(nodus->liberi, ZEPHYRUM);
    si (liberum == NIHIL || _est_commentum(liberum))
    {
        redde NIHIL;
    }
    redde liberum;
}

/* an elementum forma exotica careat: captee tutum formae blocorum
 * aut inline (captura, fragmentum, augmentum, crudus, multilinea
 * excluduntur - conversio conservativa, formae auctoris manent) */
interior b32
_elementum_planum (
    constans StmlNodus* nodus)
{
    redde (b32)(   nodus->genus == STML_NODUS_ELEMENTUM
                && nodus->captio_directio == STML_CAPTIO_NIHIL
                && !nodus->fragmentum
                && nodus->augmentum_clavis == NIHIL
                && !nodus->crudus
                && !nodus->multilinea);
}

/* an elementum capturae multiplici structuraliter idoneum sit
 * (§0.2 decretum sextum): II aut III liberi, omnes elementa
 * simplicia (commentum sedem captei raperet; retro/farcimen
 * retrorsum attingunt; fragmenta extra ambitum). Altitudo
 * SEORSUM probatur (redditio + reversio in
 * _capturam_multiplicem_conari). */
interior b32
_capturae_multiplicis_idoneum (
    constans StmlNodus* nodus)
{
    i32 num;
    i32 i;

    si (   nodus->genus            != STML_NODUS_ELEMENTUM
        || nodus->crudus
        || nodus->multilinea
        || nodus->fragmentum
        || nodus->augmentum_clavis != NIHIL
        || (   nodus->captio_directio != STML_CAPTIO_NIHIL
            && nodus->captio_directio != STML_CAPTIO_ANTE))
    {
        redde FALSUM;
    }
    num = nodus->liberi ? xar_numerus(nodus->liberi) : ZEPHYRUM;
    si (num < II || num > III)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlNodus* liberum;

        liberum = _xar_liberum_obtinere(nodus->liberi, i);
        si (   liberum                  == NIHIL
            || liberum->genus           != STML_NODUS_ELEMENTUM
            || liberum->fragmentum
            || liberum->captio_directio == STML_CAPTIO_RETRO
            || liberum->captio_directio == STML_CAPTIO_FARCIMEN)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* an nodus spinam capturae incipere possit (§4 verticalis):
 * vinculum cuius liberum aut textus capturabilis (terminalis) aut
 * vinculum ulterius aut elementum planum (terminalis inline aut
 * bloci) aut capturae multiplici idoneum (terminalis bloci -
 * sine hoc relectio formae '((>' sub spina in cascadem glutinatam
 * CAPTIO_ANTE caderet et punctum fixum periret). Liberum exoticum
 * spinam recusat - nodus formam solitam retinet. */
interior b32
_spina_idonea (
    constans StmlNodus* nodus)
{
    StmlNodus* liberum;

    liberum = _spinae_liberum_unicum(nodus);
    si (liberum == NIHIL)
    {
        redde FALSUM;
    }
    si (liberum->genus == STML_NODUS_TEXTUS)
    {
        redde _valor_capturabilis(liberum->valor);
    }
    si (liberum->genus != STML_NODUS_ELEMENTUM)
    {
        redde FALSUM;
    }
    redde (b32)(   _spina_idonea(liberum)
                || _elementum_planum(liberum)
                || _capturae_multiplicis_idoneum(liberum));
}

/* an elementum terminale inline unilineare reddatur: folium aut
 * liberi omnes textus unilineares (recusatio conservativa v1 -
 * crudus/multilinea/fragmentum/captio excluduntur) */
interior b32
_terminalis_inline (
    constans StmlNodus* nodus)
{
    i32 i;
    i32 num;

    si (   nodus->genus            != STML_NODUS_ELEMENTUM
        || nodus->captio_directio  != STML_CAPTIO_NIHIL
        || nodus->fragmentum
        || nodus->augmentum_clavis != NIHIL
        || nodus->crudus
        || nodus->multilinea)
    {
        redde FALSUM;
    }
    num = nodus->liberi ? xar_numerus(nodus->liberi) : ZEPHYRUM;
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlNodus* liberum;

        liberum = _xar_liberum_obtinere(nodus->liberi, i);
        si (   liberum        == NIHIL
            || liberum->genus != STML_NODUS_TEXTUS
            || !_valor_unilinearis(liberum->valor))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Notam sedium unam appendere - derivatio UNA pro locis omnibus
 * (vinculum, multiplex, rami capturae nuclei, finis-casus), ne
 * quinque occasiones divergendi sint. */
interior vacuum
_sedes_notare (
          Xar* sedes,
    StmlNodus* nodus,
          i32  initium,
          i32  finis)
{
    StmlSedesNodi* nota;

    si (sedes == NIHIL)
    {
        redde;
    }
    nota = xar_addere(sedes);
    si (nota != NIHIL)
    {
        nota->nodus    = nodus;
        nota->initium  = initium;
        nota->finis    = finis;
    }
}

/* Vinculum unum spinae scribere: tag ut captor unigena forma
 * canonica (§0.2: glutinata sine attributis, spatium post
 * attributa). Separator sequens (spatium aut fractio lineae) a
 * vocante emittitur - post captoris in relectione fit. Sedes
 * vinculi TAGUM SOLUM tegunt (extensio captoris apud parsatorem
 * in consumptione tokeni ponitur, ante reparentationem capturae) -
 * scriptor eandem semanticam reddit, probatio paritatis iudex. */
interior vacuum
_vinculum_scribere (
            StmlNodus* nodus,
    ChordaAedificator* aedificator,
                  Xar* sedes)
{
    i32 initium;

    initium = (i32)chorda_aedificator_longitudo(aedificator);

    chorda_aedificator_appendere_character(aedificator, '<');
    si (nodus->titulus)
    {
        chorda_aedificator_appendere_chorda(aedificator,
                                            *nodus->titulus);
    }
    _attributa_scribere(aedificator, nodus, FALSUM);
    si (   nodus->attributa != NIHIL
        && xar_numerus(nodus->attributa) > ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(aedificator, ' ');
    }
    chorda_aedificator_appendere_literis(aedificator, "(>");

    _sedes_notare(sedes, nodus, initium,
                  (i32)chorda_aedificator_longitudo(aedificator));
}

/* Vinculum captoris forma multilinea (§0.2 decretum quintum):
 * '<titulus' + attributa stackata + '(>' linea propria, '>' in
 * columna '='. Sedes tagum totum notat (initium '<', finis post
 * '>' - extensio parsatoris eadem, lineis interioribus inclusis).
 * Columnam '=' reddit - vocans lineam clausurae basim mensurae
 * novam facit. Spina sola huc venit (non-crudus, parenthesis
 * una), ergo sigilla nulla. */
interior i32
_vinculum_multilineum_scribere (
            StmlNodus* nodus,
    ChordaAedificator* aedificator,
                  i32  basis,
                  Xar* sedes)
{
    i32 initium;
    i32 columna;

    initium = (i32)chorda_aedificator_longitudo(aedificator);

    chorda_aedificator_appendere_character(aedificator, '<');
    si (nodus->titulus)
    {
        chorda_aedificator_appendere_chorda(aedificator,
                                            *nodus->titulus);
    }
    columna = _attributa_multilinea_scribere(aedificator, nodus,
                                             basis, I);

    _sedes_notare(sedes, nodus, initium,
                  (i32)chorda_aedificator_longitudo(aedificator));
    redde columna;
}

/* Valorem fluminis IUNCTUM evasumque scribere (§4 M3): cursus
 * albi lineam ferentes spatio uni fiunt (lectio fluxus - pulcher
 * prosam possidet); segmenta cetera litteralia per evasionem
 * solitam. */
interior vacuum
_fluxu_evasum_scribere (
    ChordaAedificator* aedificator,
      constans chorda* valor)
{
    i32 i;

    i = ZEPHYRUM;
    dum (i < valor->mensura)
    {
        i32 finis;

        /* segmentum litterale usque ad cursum lineiferum */
        finis = i;
        dum (finis < valor->mensura)
        {
            si (_est_spatium((character)valor->datum[finis]))
            {
                i32 f2;
                b32 f;

                f2  = finis;
                f   = FALSUM;
                dum (   f2 < valor->mensura
                     && _est_spatium((character)valor->datum[f2]))
                {
                    si ((character)valor->datum[f2] == '\n')
                    {
                        f = VERUM;
                    }
                    f2++;
                }
                si (f)
                {
                    frange;
                }
                finis = f2;
            }
            alioquin
            {
                finis++;
            }
        }
        si (finis > i)
        {
            chorda pars;

            pars.datum    = valor->datum + i;
            pars.mensura  = finis - i;
            _scribere_evasus(aedificator, &pars);
        }
        i = finis;
        si (i < valor->mensura)
        {
            /* cursus lineifer -> spatium unum */
            dum (   i < valor->mensura
                 && _est_spatium((character)valor->datum[i]))
            {
                i++;
            }
            si (i < valor->mensura)
            {
                chorda_aedificator_appendere_character(aedificator,
                                                       ' ');
            }
        }
    }
}

/* Atomum proximum ab initio dato: finis exclusivus redditur -
 * usque ad candidatum fracturae (cursus lineifer aut spatium
 * SINGULUM); cursus spatiorum multiplicium eiusdem lineae intra
 * atomum manent (§4: infrangibiles - fractura intra eos lectionem
 * fluxus mutaret). */
interior i32
_atomi_finis (
    constans chorda* valor,
                i32  initium)
{
    i32 finis;

    finis = initium;
    dum (finis < valor->mensura)
    {
        si (_est_spatium((character)valor->datum[finis]))
        {
            i32 f2;
            b32 fert;

            f2    = finis;
            fert  = FALSUM;
            dum (   f2 < valor->mensura
                 && _est_spatium((character)valor->datum[f2]))
            {
                si ((character)valor->datum[f2] == '\n')
                {
                    fert = VERUM;
                }
                f2++;
            }
            si (fert || f2 - finis == I)
            {
                frange;   /* candidatus fracturae */
            }
            finis = f2;   /* cursus multiplex: pars atomi */
        }
        alioquin
        {
            finis++;
        }
    }
    redde finis;
}

/* Re-involutio stackata (§4 M3, decretum quartum; emendatum
 * 2026-08-25 impletio SUSPENSA): elementum liberi textus unici
 * mundi quod inline non cadit - captor canonicus ('<t(>' /
 * attributa stackata cum '(>' linea propria), textus avare
 * impletus SINE clausura. Linea clausurae linea TAGI est:
 * contentum eam sequitur ubi cadit (sicut sarcinator captis
 * elementis iam facit) - textus in linea clausurae incipit et
 * lineae sequentes sub textu primo SUSPENSAE alineantur, si (a)
 * atomum primum intra tectum cadit ET (b) columna suspensionis
 * XL columnas contenti sub tecto relinquit (numerus fundi
 * profunditatis - captores inline lati et indentatio gravis in
 * fasciam non suspendunt). Aliter forma VERTICALIS: impletio
 * gradu uno altius in lineis recentibus. Conatus atomi primi
 * per redditionem + reversionem (evasio longitudinem mutare
 * potest). Invariams M2b tenet: clausurae ibi solae ubi forma
 * blocorum multi-liberorum. Sedes tagum solum (semantica
 * captoris, via _vinculum_scribere). */
interior b32
_textum_refluere_conari (
            StmlNodus* nodus,
    ChordaAedificator* aedificator,
                  i32  indentatio,
                  Xar* sedes)
{
          StmlNodus* liberum;
     memoriae_index  initium_lineae;
    constans chorda* valor;
                i32  tectum;
                i32  columna_impletionis;
                i32  i;
                b32  suspensum;
                b32  primum_lineae;

    liberum = _spinae_liberum_unicum(nodus);
    si (   liberum        == NIHIL
        || liberum->genus != STML_NODUS_TEXTUS
        || !_valor_capturabilis(liberum->valor))
    {
        redde FALSUM;
    }
    valor = liberum->valor;

    /* captor: canonicus in linea, aut attributa stackata cum
     * '(>' linea propria (§0.2 decretum quintum; III = spatium
     * + parenthesis + '>'). Sedes TAGUM SOLUM utrimque. Linea
     * currens notatur - basis mensurae suspensionis. */
    initium_lineae = chorda_aedificator_longitudo(aedificator)
        - (memoriae_index)(indentatio * II);
    si (_attributa_multilinea_oportet(nodus, indentatio, III))
    {
        i32 columna;

        columna = _vinculum_multilineum_scribere(
            nodus, aedificator, indentatio * II, sedes);
        initium_lineae = chorda_aedificator_longitudo(aedificator)
            - (memoriae_index)(columna + I);
    }
    alioquin
    {
        _vinculum_scribere(nodus, aedificator, sedes);
    }

    /* conatus suspensionis: custos columnae, deinde atomum
     * primum in linea clausurae per redditionem + reversionem */
    tectum               = _tectum_lineae(indentatio);
    columna_impletionis  = (i32)(chorda_aedificator_longitudo(
        aedificator) - initium_lineae) + I;
    suspensum      = FALSUM;
    primum_lineae  = FALSUM;
    i              = ZEPHYRUM;
    si (columna_impletionis + XL <= tectum)
    {
        memoriae_index signum;
                   i32 finis;
                chorda pars;

        finis   = _atomi_finis(valor, ZEPHYRUM);
        signum  = chorda_aedificator_longitudo(aedificator);
        chorda_aedificator_appendere_character(aedificator, ' ');
        pars.datum    = valor->datum;
        pars.mensura  = finis;
        _scribere_evasus(aedificator, &pars);
        si (   chorda_aedificator_longitudo(aedificator)
                   - initium_lineae <= (memoriae_index)tectum)
        {
            suspensum  = VERUM;
            i          = finis;
            dum (   i < valor->mensura
                 && _est_spatium((character)valor->datum[i]))
            {
                i++;
            }
        }
        alioquin
        {
            chorda_aedificator_truncare(aedificator, signum);
        }
    }
    si (!suspensum)
    {
        /* forma verticalis: impletio gradu uno altius */
        tectum               = _tectum_lineae(indentatio + I);
        columna_impletionis  = (indentatio + I) * II;
        chorda_aedificator_appendere_character(aedificator, '\n');
        initium_lineae = chorda_aedificator_longitudo(aedificator);
        _spatia_scribere(aedificator, columna_impletionis);
        primum_lineae = VERUM;
    }

    dum (i < valor->mensura)
    {
        memoriae_index signum;
                   i32 finis;

        finis = _atomi_finis(valor, i);

        /* atomum emittere (conatus + reversio) */
        signum = chorda_aedificator_longitudo(aedificator);
        si (!primum_lineae)
        {
            chorda_aedificator_appendere_character(aedificator, ' ');
        }
        {
            chorda pars;

            pars.datum    = valor->datum + i;
            pars.mensura  = finis - i;
            _scribere_evasus(aedificator, &pars);
        }
        si (   chorda_aedificator_longitudo(aedificator)
                   - initium_lineae > (memoriae_index)tectum
            && !primum_lineae)
        {
            chorda_aedificator_truncare(aedificator, signum);
            chorda_aedificator_appendere_character(aedificator,
                                                   '\n');
            initium_lineae =
                chorda_aedificator_longitudo(aedificator);
            _spatia_scribere(aedificator, columna_impletionis);
            {
                chorda pars;

                pars.datum    = valor->datum + i;
                pars.mensura  = finis - i;
                _scribere_evasus(aedificator, &pars);
            }
        }
        primum_lineae = FALSUM;

        /* candidatum consumere */
        i = finis;
        dum (   i < valor->mensura
             && _est_spatium((character)valor->datum[i]))
        {
            i++;
        }
    }
    redde VERUM;
}

/* Lineae vacuae authoratae (§4): separatio paragraphorum non
 * deletur - pulcher unam aut duas servat, plures ad duas cadunt.
 * Numeratio ex spatia_ante sequentis (lex proprietatis: '\n'
 * primum fractionis ad post prioris it, omne ultra ad ante
 * sequentis) supra lineam basalem: basis I positione liberi primi
 * (tagum apertum totum cursum ei dat), ZEPHYRUM inter fratres et
 * ante tagum claudentem. */
interior i32
_lineae_vacuae (
    constans chorda* spatia,
                i32  basis)
{
    i32 i;
    i32 numerus;

    si (spatia == NIHIL)
    {
        redde ZEPHYRUM;
    }
    numerus = ZEPHYRUM;
    per (i = ZEPHYRUM; i < spatia->mensura; i++)
    {
        si ((character)spatia->datum[i] == '\n')
        {
            numerus++;
        }
    }
    si (numerus <= basis)
    {
        redde ZEPHYRUM;
    }
    numerus -= basis;
    redde (numerus > II) ? II : numerus;
}

/* Altitudinem nodi aestimare (positione bloci), tecto dato -
 * maturius exit ubi tectum exceditur. EX ARBORE, non redditione:
 * mensura per redditionem EXPONENTIALIS erat (parens limitaneus
 * reversus liberos in forma blocorum re-reddebat, quisque gradus
 * nidificationis opus duplicabat - porta plagulae arboris in
 * horas abiit). Aestimatio conservativa: collapsus veros
 * (spinae, multiplex, suspensio) non simulat, ergo aestimatum
 * >= altitudo vera - coetus limitaneus formam blocorum tenet,
 * pretium aestheticum leve, punctum fixum sanum (arbor eadem ->
 * aestimatum idem). */
interior i32
_lineas_aestimare (
    constans StmlNodus* nodus,
                   i32  tectum)
{
    i32 summa;
    i32 num;
    i32 i;

    si (   nodus->genus == STML_NODUS_TEXTUS
        || nodus->genus == STML_NODUS_COMMENTUM)
    {
        summa = I;
        si (nodus->valor != NIHIL)
        {
            per (i = ZEPHYRUM;
                 i < nodus->valor->mensura && summa <= tectum;
                 i++)
            {
                si ((character)nodus->valor->datum[i] == '\n')
                {
                    summa++;
                }
            }
        }
        redde summa;
    }
    si (nodus->genus != STML_NODUS_ELEMENTUM)
    {
        redde I;
    }
    num = nodus->liberi ? xar_numerus(nodus->liberi) : ZEPHYRUM;
    si (num == ZEPHYRUM || _habet_liberum_textus(nodus))
    {
        /* folium aut dispositio inline: linea una */
        redde I;
    }
    summa = I;   /* linea tagi */
    per (i = ZEPHYRUM; i < num && summa <= tectum; i++)
    {
        StmlNodus* liberum;

        liberum = _xar_liberum_obtinere(nodus->liberi, i);
        si (liberum)
        {
            summa += _lineae_vacuae(liberum->spatia_ante, I)
                + _lineas_aestimare(liberum, tectum - summa);
        }
    }
    redde summa + I;   /* clausura potentialis */
}

/* Captura multiplex stackata (§0.2 decretum sextum, 2026-08-25):
 * elementum II aut III liberorum, omnium elementorum simplicium
 * (commentum nodus est et sedem captei raperet; captura retro/
 * farcimen trans limitem retrorsum attingit; fragmenta extra
 * ambitum v1), intra tectum altitudinis - forma capturae
 * multiplicis: '<t((>' aut '<t(((>', liberi lineis sequentibus
 * positione bloci solita, SINE clausura. NUMQUAM in linea tagi:
 * spatium inter captos eiusdem lineae nodus textus fieret et
 * numerationem corrumperet - forma stackata angulum M4 vitat
 * (cursus albi lineiferi nodi non sunt, §1.3). Altitudo ex
 * arbore AESTIMATA (vide _lineas_aestimare - redditio cum
 * reversione exponentialis erat); liberi octetim iidem ac forma
 * bloci, sola clausura perit. Lineae vacuae inter captos
 * servatae (basis eadem ac blocus); vacuae caudales cadunt
 * (nihil eas ancorat). Sedes TAGUM SOLUM (semantica captoris). */
interior b32
_capturam_multiplicem_conari (
            StmlNodus* nodus,
    ChordaAedificator* aedificator,
                  i32  indentatio,
                  Xar* sedes)
{
         StmlNodus* liberum;
    memoriae_index  signum_originis;
    memoriae_index  initium_liberorum;
               i32  num;
               i32  i;
               i32  altitudo;

    si (!_capturae_multiplicis_idoneum(nodus))
    {
        redde FALSUM;
    }
    num = xar_numerus(nodus->liberi);

    /* tectum altitudinis: summa aestimata liberorum (ex arbore -
     * ante ullam redditionem, ergo sine reversione) */
    altitudo = ZEPHYRUM;
    per (i = ZEPHYRUM;
         i < num && altitudo <= STML_CAPTURA_MULTIPLEX_LINEAE;
         i++)
    {
        liberum = _xar_liberum_obtinere(nodus->liberi, i);
        si (liberum)
        {
            altitudo += _lineae_vacuae(liberum->spatia_ante, I)
                + _lineas_aestimare(liberum,
                      STML_CAPTURA_MULTIPLEX_LINEAE - altitudo);
        }
    }
    si (altitudo > STML_CAPTURA_MULTIPLEX_LINEAE)
    {
        redde FALSUM;
    }

    signum_originis = chorda_aedificator_longitudo(aedificator);

    /* tagum captoris; attributa lata stackata cum clausura
     * alineata (§0.2 decretum quintum; latitudo inline = spatium
     * + parentheses + '>') */
    chorda_aedificator_appendere_character(aedificator, '<');
    si (nodus->titulus)
    {
        chorda_aedificator_appendere_chorda(aedificator,
                                            *nodus->titulus);
    }
    si (_attributa_multilinea_oportet(nodus, indentatio, II + num))
    {
        _attributa_multilinea_scribere(aedificator, nodus,
                                       indentatio * II, num);
    }
    alioquin
    {
        _attributa_scribere(aedificator, nodus, FALSUM);
        si (   nodus->attributa != NIHIL
            && xar_numerus(nodus->attributa) > ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(aedificator,
                                                   ' ');
        }
        per (i = ZEPHYRUM; i < num; i++)
        {
            chorda_aedificator_appendere_character(aedificator,
                                                   '(');
        }
        chorda_aedificator_appendere_character(aedificator, '>');
    }
    initium_liberorum = chorda_aedificator_longitudo(aedificator);

    /* liberi positione bloci solita (altitudo iam supra
     * aestimata). Basis linearum vacuarum liberi primi:
     * forma authorata parentis eam regit - tagum apertum cursum
     * TOTUM libero primo dat (basis I), captor authoratus lineam
     * primam in POST suo fert (lex proprietatis §1.2), ergo
     * capteus relectus uno '\n' positionali minus portat (basis
     * ZEPHYRUM). Sine hoc punctum fixum periret: linea vacua
     * authorata transitu secundo evanesceret. */
    per (i = ZEPHYRUM; i < num; i++)
    {
        liberum = _xar_liberum_obtinere(nodus->liberi, i);
        si (liberum)
        {
            i32 vacuae;

            chorda_aedificator_appendere_character(aedificator,
                                                   '\n');
            per (vacuae = _lineae_vacuae(
                     liberum->spatia_ante,
                     (   i == ZEPHYRUM
                      && nodus->captio_directio
                             == STML_CAPTIO_NIHIL)
                         ? I : ZEPHYRUM);
                 vacuae > ZEPHYRUM; vacuae--)
            {
                chorda_aedificator_appendere_character(aedificator,
                                                       '\n');
            }
            _scribere_nucleus(liberum, aedificator, VERUM, FALSUM,
                              indentatio + I, sedes);
        }
    }

    /* nota sedium POST liberos appensa - tabula post-ordinem
     * servat (elementum ubi clauditur notatur; captor clausuram
     * non fert, sed ordo contractus est) */
    _sedes_notare(sedes, nodus, (i32)signum_originis,
                  (i32)initium_liberorum);
    redde VERUM;
}

/* Spinam pulchre scribere (§4 collapsus verticalis, M2b): forma
 * capturae UNIVERSALIS spinis idoneis - latitudo dispositionem
 * eligit, non formam. Vincula in linea currenti avare sarcinantur
 * dum tectum LXXII tenet; fractio lineae gradum UNUM addit (per
 * LINEAM, non per vinculum). Terminalis inline in fine lineae
 * ultimae vehitur (cum vinculo suo mensuratus); terminalis bloci
 * lineam recentem gradu uno altius aperit forma solita, clausura
 * suo gradu (optio 2 - captor tagum aperiens bloci numquam
 * sorbet). FALSUM solum si spina non idonea aut casus degener:
 * spina tota = vinculum unicum cum textu ultra tectum (fixum
 * LXXII/LXXIII - forma aperta manet). */
/* Segmentum tabulae sedium revertere in situ (permutationes
 * cellularum, sine allocatione). Custos quantum < II: nihil
 * revertendum - et 'initium + quantum - I' cum quantum ZEPHYRUM
 * et initio ZEPHYRUM in i32 INSIGNATO ad ~2^32 subfluebat, unde
 * ansa ~2^31 iterationum vacuarum (8s in probatio_stml_sedes:
 * spina terminali textus prima documenti - totum == vincula). */
interior vacuum
_sedes_revertere (
    Xar* sedes,
    i32  initium,
    i32  quantum)
{
    i32 a;
    i32 b;

    si (quantum < II)
    {
        redde;
    }
    a = initium;
    b = initium + quantum - I;
    dum (a < b)
    {
        StmlSedesNodi* pa;
        StmlSedesNodi* pb;
        StmlSedesNodi  medium;

        pa = (StmlSedesNodi*)xar_obtinere(sedes, a);
        pb = (StmlSedesNodi*)xar_obtinere(sedes, b);
        si (pa != NIHIL && pb != NIHIL)
        {
            medium  = *pa;
            *pa     = *pb;
            *pb     = medium;
        }
        a++;
        b--;
    }
}

/* Post-ordinem tabulae sedium restituere (quaestio 01M0WVG98N):
 * sarcinator vincula PRAE-ordine notat (vinculum dum scribitur -
 * extensio eius tunc iam tota nota est, tagum solum), contractus
 * tabulae autem POST-ordO est (nodus notatur ubi clauditur;
 * ambulatio post-ordinis parsatoris ordinem eundem parit).
 * Segmentum ab initio: [L1..Lk, T...] ubi T (subtree terminalis,
 * si adest) intus iam post-ordinatum; optatum [T..., Lk..L1].
 * Reversio duplex in situ: totum reverte -> [rev(T), Lk..L1],
 * deinde partem T reverte -> [T..., Lk..L1]. */
interior vacuum
_sedes_postordinare (
    Xar* sedes,
    i32  initium,
    i32  vincula)
{
    i32 totum;

    si (sedes == NIHIL || vincula == ZEPHYRUM)
    {
        redde;
    }
    totum = xar_numerus(sedes) - initium;
    _sedes_revertere(sedes, initium, totum);
    _sedes_revertere(sedes, initium, totum - vincula);
}

interior b32
_spinam_pulchre_scribere (
            StmlNodus* nodus,
    ChordaAedificator* aedificator,
                  i32  indentatio,
                  Xar* sedes)
{
         StmlNodus* currens;
         StmlNodus* liberum;
    memoriae_index  signum_originis;
    memoriae_index  initium_lineae;
               i32  gradus;
               i32  sedes_originis;
               i32  vincula;
               b32  primum_lineae;

    si (!_spina_idonea(nodus))
    {
        redde FALSUM;
    }

    sedes_originis = (sedes != NIHIL)
        ? xar_numerus(sedes) : ZEPHYRUM;
    vincula        = ZEPHYRUM;

    signum_originis  = chorda_aedificator_longitudo(aedificator);
    /* vocans indentationem iam emisit - linea ibi incepit */
    initium_lineae   = signum_originis
        - (memoriae_index)(indentatio * II);
    gradus         = indentatio;
    primum_lineae  = VERUM;
    currens        = nodus;

    dum (VERUM)
    {
        memoriae_index signum;
        memoriae_index post_vinculum;
                   i32 signum_sedes;
                   i32 tectum;
                   b32 finalis_textus;
                   b32 finalis_inline;
                   b32 finalis_blocus;

        liberum         = _spinae_liberum_unicum(currens);
        finalis_textus  = (b32)(liberum->genus == STML_NODUS_TEXTUS);
        finalis_inline  = FALSUM;
        finalis_blocus  = FALSUM;
        si (!finalis_textus && !_spina_idonea(liberum))
        {
            si (_terminalis_inline(liberum))
            {
                finalis_inline = VERUM;
            }
            alioquin
            {
                finalis_blocus = VERUM;
            }
        }

        /* conatus: [separator] vinculum [+ terminalis inline] */
        signum        = chorda_aedificator_longitudo(aedificator);
        signum_sedes  = (sedes != NIHIL)
            ? xar_numerus(sedes) : ZEPHYRUM;
        si (!primum_lineae)
        {
            chorda_aedificator_appendere_character(aedificator, ' ');
        }
        _vinculum_scribere(currens, aedificator, sedes);
        post_vinculum = chorda_aedificator_longitudo(aedificator);
        si (finalis_textus)
        {
            chorda_aedificator_appendere_character(aedificator, ' ');
            /* lectio IUNCTA (§4 M3): cursus lineiferi spatio uni -
             * valor multilinearis captus prosam suam unam lineam
             * scribit */
            _fluxu_evasum_scribere(aedificator, liberum->valor);
        }
        alioquin si (finalis_inline)
        {
            chorda_aedificator_appendere_character(aedificator, ' ');
            _scribere_nucleus(liberum, aedificator, FALSUM, FALSUM,
                              ZEPHYRUM, sedes);
        }

        /* tectum lineae: LXXII planum, sed sub indentatione gravi
         * minimum XL columnarum contenti manet (decretum tertium
         * §0.2) - sine fundo fracturae profundae cumularentur
         * (linea quaeque fracta gradu altior duabusque columnis
         * angustior) in scalas linearum unius vinculi */
        tectum = _tectum_lineae(gradus);
        si (chorda_aedificator_longitudo(aedificator)
                - initium_lineae > (memoriae_index)tectum)
        {
            si (!primum_lineae)
            {
                /* fractio lineae: gradus unus altius, conatum
                 * itera in linea recenti */
                chorda_aedificator_truncare(aedificator, signum);
                si (sedes != NIHIL)
                {
                    xar_truncare(sedes, signum_sedes);
                }
                chorda_aedificator_appendere_character(aedificator,
                                                       '\n');
                gradus++;
                initium_lineae =
                    chorda_aedificator_longitudo(aedificator);
                _scribere_indentatio(aedificator, gradus);
                primum_lineae = VERUM;
                perge;
            }
            si (finalis_textus)
            {
                /* textus ne primo lineae quidem cadit: vinculum
                 * ultimum formam apertam retinet (fixum
                 * LXXII/LXXIII) */
                chorda_aedificator_truncare(aedificator, signum);
                si (sedes != NIHIL)
                {
                    xar_truncare(sedes, signum_sedes);
                }
                si (signum == signum_originis)
                {
                    /* spina tota = vinculum hoc: recusatio plena,
                     * vocans formam solitam scribit */
                    redde FALSUM;
                }
                /* vincula priora manent; linea fracta iam indentata
                 * - indentationem tollere, nucleus se ipse gradu
                 * fractionis indentat (gradu uno sub linea captorum
                 * ultima) */
                chorda_aedificator_truncare(aedificator,
                                            initium_lineae);
                _scribere_nucleus(currens, aedificator, VERUM,
                                  FALSUM, gradus, sedes);
                /* nota conatus currentis truncata - vincula
                 * priora sola supersunt */
                _sedes_postordinare(sedes, sedes_originis,
                                    vincula);
                redde VERUM;
            }
            si (   post_vinculum - initium_lineae
                       > (memoriae_index)tectum
                && _attributa_adsunt(currens))
            {
                /* vinculum SOLUM ultra tectum cum attributis:
                 * forma multilinea in situ (§0.2 decretum
                 * quintum) - linea clausurae basis mensurae
                 * nova, spina in ea pergit */
                i32 columna;

                chorda_aedificator_truncare(aedificator, signum);
                si (sedes != NIHIL)
                {
                    xar_truncare(sedes, signum_sedes);
                }
                columna = _vinculum_multilineum_scribere(
                    currens, aedificator, gradus * II, sedes);
                initium_lineae =
                    chorda_aedificator_longitudo(aedificator)
                        - (memoriae_index)(columna + I);
                si (finalis_inline)
                {
                    chorda_aedificator_appendere_character(
                        aedificator, ' ');
                    _scribere_nucleus(liberum, aedificator,
                                      FALSUM, FALSUM, ZEPHYRUM,
                                      sedes);
                }
            }
            /* aliter: vinculum aut terminalis inline primum
             * lineae ultra tectum acceptum - nihil angustius
             * praesto est */
        }

        si (finalis_textus || finalis_inline)
        {
            /* vinculum currentis stat - numeratur, deinde ordo
             * post-ordinis restituitur (01M0WVG98N) */
            vincula++;
            _sedes_postordinare(sedes, sedes_originis, vincula);
            redde VERUM;
        }
        si (finalis_blocus)
        {
            chorda_aedificator_appendere_character(aedificator,
                                                   '\n');
            _scribere_nucleus(liberum, aedificator, VERUM, FALSUM,
                              gradus + I, sedes);
            vincula++;
            _sedes_postordinare(sedes, sedes_originis, vincula);
            redde VERUM;
        }
        primum_lineae  = FALSUM;
        vincula++;
        currens        = liberum;
    }
}

interior b32
_scribere_nucleus (
            StmlNodus* nodus,
    ChordaAedificator* aedificator,
                  b32  pulchrum,
                  b32  fidelitas,
                  i32  indentatio,
                  Xar* sedes)
{
          i32  i;
          i32  num;
          s32  initium_sedis;
    StmlNodus* liberum;
          b32  habet_liberos;
          b32  post_iam_emissum;

    si (!nodus || !aedificator)
    {
        redde FALSUM;
    }
    initium_sedis     = -I;
    post_iam_emissum  = FALSUM;

    /* reassemblatio (§4): modus non-pulcher trivia verbatim
     * reassuit. spatia_ante ANTE octetum primum nodi - extensio
     * sedium ea excludit (initium post haec capitur). Pulcher
     * trivia IGNORAT et dispositionem suam generat - PRAETER
     * interius '<tag\>', cuius dispositio declarata est. */
    si (   (fidelitas || _intra_multilineam(nodus))
        && nodus->spatia_ante != NIHIL)
    {
        chorda_aedificator_appendere_chorda(aedificator,
                                            *nodus->spatia_ante);
    }

    commutatio (nodus->genus)
    {
        casus STML_NODUS_DOCUMENTUM:
            si (nodus->liberi)
            {
                b32 in_linea;

                /* eadem regula ac elementa (M2 §4): textus in
                 * gradu documenti -> INLINE (separator '\n' textum
                 * eiusdem-lineae in trivia verteret relectum) */
                in_linea  = _habet_liberum_textus(nodus);
                num       = xar_numerus(nodus->liberi);
                per (i = ZEPHYRUM; i < num; i++)
                {
                    liberum = _xar_liberum_obtinere(nodus->liberi, i);
                    si (liberum)
                    {
                        /* lineae vacuae inter liberos gradus
                         * documenti servatae (§4); vacuae plagulae
                         * ducentes cadunt (i == 0 praeteritur) */
                        si (pulchrum && !in_linea && i > ZEPHYRUM)
                        {
                            i32 vacuae;

                            per (vacuae = _lineae_vacuae(
                                     liberum->spatia_ante, ZEPHYRUM);
                                 vacuae > ZEPHYRUM; vacuae--)
                            {
                                chorda_aedificator_appendere_character(aedificator, '\n');
                            }
                        }
                        _scribere_nucleus(liberum, aedificator,
                            in_linea ? FALSUM : pulchrum, fidelitas,
                            indentatio, sedes);
                        si (pulchrum && !in_linea && i < num - I)
                        {
                            chorda_aedificator_appendere_character(aedificator, '\n');
                        }
                    }
                }
            }
            /* cauda plagulae post radicem clausam (§1.1) */
            si (fidelitas && nodus->spatia_clausurae != NIHIL)
            {
                chorda_aedificator_appendere_chorda(aedificator,
                    *nodus->spatia_clausurae);
            }
            frange;

        casus STML_NODUS_ELEMENTUM:
            si (pulchrum)
            {
                _scribere_indentatio(aedificator, indentatio);
            }
            /* COLLAPSUS (§4 M2b): spina unigena -> forma capturae,
             * latitudine dispositionem (unam lineam aut sarcinatam
             * verticalem) eligente; captores ANTE auctoris unigenae
             * via eadem re-derivantur (unificatio). Recusatio spinae
             * (textus ultra tectum) in RE-INVOLUTIONEM cadit (§4
             * M3): forma bloci, textus impletus. Sedes suas uterque
             * ipse notat (initium_sedis sentinellam -I tenet, ne
             * bis notetur) */
            si (pulchrum)
            {
                si (_spinam_pulchre_scribere(nodus, aedificator,
                                             indentatio, sedes))
                {
                    frange;
                }
                si (_textum_refluere_conari(nodus, aedificator,
                                            indentatio, sedes))
                {
                    frange;
                }
                /* II-III liberi -> captura multiplex stackata
                 * (§0.2 decretum sextum); recusatio (altitudo,
                 * liberi inidonei) in formam bloci cadit */
                si (_capturam_multiplicem_conari(nodus, aedificator,
                                                 indentatio, sedes))
                {
                    frange;
                }
            }
            /* sedes: ab primo octeto tagi, POST indentationem -
             * semantica positus_initium parsatoris */
            initium_sedis =
                (s32)chorda_aedificator_longitudo(aedificator);

            /* Augmentatio <% &clavis;>: clavis sola, sine
             * attributis; liberi inline, clausura </%> semper.
             * Forma canonica '<% ' + clavis + '>' - iter rotundum
             * octetim super formam canonicam. */
            si (nodus->augmentum_clavis)
            {
                chorda_aedificator_appendere_literis(aedificator,
                                                     "<% ");
                chorda_aedificator_appendere_chorda(aedificator,
                    *nodus->augmentum_clavis);
                chorda_aedificator_appendere_character(aedificator,
                                                       '>');
                si (nodus->liberi)
                {
                    num = xar_numerus(nodus->liberi);
                    per (i = ZEPHYRUM; i < num; i++)
                    {
                        liberum = _xar_liberum_obtinere(
                            nodus->liberi, i);
                        si (liberum)
                        {
                            _scribere_nucleus(liberum,
                                aedificator, FALSUM, fidelitas, ZEPHYRUM, sedes);
                        }
                    }
                }
                chorda_aedificator_appendere_literis(aedificator,
                                                     "</%>");
                frange;
            }

            /* Handle fragment elements specially */
            si (nodus->fragmentum)
            {
                /* Opening tag: <# or <#id */
                chorda_aedificator_appendere_literis(aedificator, "<#");

                /* Fragment ID if present */
                si (nodus->fragmentum_id && nodus->fragmentum_id->mensura > ZEPHYRUM)
                {
                    chorda_aedificator_appendere_chorda(aedificator, *nodus->fragmentum_id);
                }

                /* Attributes */
                                _attributa_scribere(aedificator, nodus, fidelitas);

                /* trivia intra tagum ante finem (§1.6) */
                si (fidelitas && nodus->spatia_intra_tagum != NIHIL)
                {
                    chorda_aedificator_appendere_chorda(aedificator,
                        *nodus->spatia_intra_tagum);
                }

                /* Fragmentum capturans: <# (> / <#id (> - liberi
                 * inline, sine tag claudente (sicut elementa ANTE) */
                si (nodus->captio_directio == STML_CAPTIO_ANTE)
                {
                    i32 j;
                    i32 finis_tagi;
                    /* spatium prae parenthesibus: fidelitas octetos
                     * conditos SUPRA reddidit (spatia_intra_tagum);
                     * pulcher canonicam generat (§0.2) */
                    si (   !fidelitas
                        && nodus->attributa != NIHIL
                        && xar_numerus(nodus->attributa) > ZEPHYRUM)
                    {
                        chorda_aedificator_appendere_character(aedificator, ' ');
                    }
                    per (j = ZEPHYRUM; j < nodus->captio_numerus; j++)
                    {
                        chorda_aedificator_appendere_character(aedificator, '(');
                    }
                    chorda_aedificator_appendere_character(aedificator, '>');
                    finis_tagi =
                        (i32)chorda_aedificator_longitudo(aedificator);
                    /* post captoris inter tagum et captos (§6) */
                    si (fidelitas)
                    {
                        si (nodus->spatia_post != NIHIL)
                        {
                            chorda_aedificator_appendere_chorda(
                                aedificator, *nodus->spatia_post);
                        }
                        post_iam_emissum = VERUM;
                    }
                    si (nodus->liberi)
                    {
                        num = xar_numerus(nodus->liberi);
                        per (i = ZEPHYRUM; i < num; i++)
                        {
                            liberum = _xar_liberum_obtinere(nodus->liberi, i);
                            si (liberum)
                            {
                                _scribere_nucleus(liberum, aedificator, FALSUM, fidelitas, ZEPHYRUM, sedes);
                            }
                        }
                    }
                    /* extensio: tagum solum - notata POST captos,
                     * unde post-ordo tabulae (01M0X12PWS) */
                    _sedes_notare(sedes, nodus, (i32)initium_sedis,
                                  finis_tagi);
                    initium_sedis = -I;
                    frange;
                }

                /* Check if has children (self-closing vs content) */
                habet_liberos = nodus->liberi && xar_numerus(nodus->liberi) > ZEPHYRUM;

                si (!habet_liberos)
                {
                    /* Self-closing fragment: <#/> or <#id/> */
                    chorda_aedificator_appendere_literis(aedificator, "/>");
                }
                alioquin
                {
                    chorda_aedificator_appendere_character(aedificator, '>');

                    /* Serialize children */
                    num = xar_numerus(nodus->liberi);
                    per (i = ZEPHYRUM; i < num; i++)
                    {
                        liberum = _xar_liberum_obtinere(nodus->liberi, i);
                        si (liberum)
                        {
                            _scribere_nucleus(liberum, aedificator, FALSUM, fidelitas, ZEPHYRUM, sedes);
                        }
                    }

                    /* Closing tag: always </#> */
                    chorda_aedificator_appendere_literis(aedificator, "</#>");
                }
                frange;
            }

            /* Handle capture operators specially */
            si (nodus->captio_directio == STML_CAPTIO_RETRO)
            {
                /* Backward capture: <) tag> or <)) tag> */
                i32 j;
                i32 initium_tagi;

                /* ordo fluminis (§6): liberi capti RETRO in fonte
                 * ANTE tagum captoris stant - AMBO modi eum ordinem
                 * reddunt (M2: forma captor-primum pulchri erat
                 * mendax - relecta directionem capturae invertebat,
                 * captor nihil capiebat et liberum frater fiebat).
                 * Fidelitas trivia liberorum fert; pulcher eos
                 * inline emittit. */
                si (nodus->liberi)
                {
                    num = xar_numerus(nodus->liberi);
                    per (i = ZEPHYRUM; i < num; i++)
                    {
                        liberum = _xar_liberum_obtinere(nodus->liberi, i);
                        si (liberum)
                        {
                            _scribere_nucleus(liberum, aedificator, FALSUM, fidelitas, ZEPHYRUM, sedes);
                        }
                    }
                }
                initium_tagi =
                    (i32)chorda_aedificator_longitudo(aedificator);
                chorda_aedificator_appendere_character(aedificator, '<');
                per (j = ZEPHYRUM; j < nodus->captio_numerus; j++)
                {
                    chorda_aedificator_appendere_character(aedificator, ')');
                }
                chorda_aedificator_appendere_character(aedificator, ' ');
                si (nodus->titulus)
                {
                    chorda_aedificator_appendere_chorda(aedificator, *nodus->titulus);
                }
                /* Attributes */
                                _attributa_scribere(aedificator, nodus, fidelitas);
                chorda_aedificator_appendere_character(aedificator, '>');
                /* extensio: tagum solum, quod POST captos in fonte
                 * stat - captis iam notatis post-ordo tenet
                 * (01M0X12PWS) */
                _sedes_notare(sedes, nodus, initium_tagi,
                              (i32)chorda_aedificator_longitudo(aedificator));
                initium_sedis = -I;
            }
            alioquin si (nodus->captio_directio == STML_CAPTIO_FARCIMEN)
            {
                /* Sandwich capture: <= tag =>. Ordo fluminis in
                 * fonte: liberum primum (captum retro) ANTE tagum,
                 * reliqua post - AMBO modi eum ordinem reddunt (M2:
                 * captor-primum pulchri relectum liberum primum in
                 * fratrem vertebat). */
                i32 initium_tagi;
                i32 finis_tagi;

                si (   nodus->liberi
                    && xar_numerus(nodus->liberi) > ZEPHYRUM)
                {
                    liberum = _xar_liberum_obtinere(nodus->liberi,
                                                    ZEPHYRUM);
                    si (liberum)
                    {
                        _scribere_nucleus(liberum, aedificator,
                            FALSUM, fidelitas, ZEPHYRUM, sedes);
                    }
                }
                initium_tagi =
                    (i32)chorda_aedificator_longitudo(aedificator);
                chorda_aedificator_appendere_literis(aedificator, "<= ");
                si (nodus->titulus)
                {
                    chorda_aedificator_appendere_chorda(aedificator, *nodus->titulus);
                }
                /* Attributes */
                                _attributa_scribere(aedificator, nodus, fidelitas);
                chorda_aedificator_appendere_literis(aedificator, " =>");
                finis_tagi =
                    (i32)chorda_aedificator_longitudo(aedificator);
                /* Liberi reliqui ab indice I (liberum 0 iam ante
                 * tagum, ambobus modis) */
                si (nodus->liberi)
                {
                    num = xar_numerus(nodus->liberi);
                    per (i = I; i < num; i++)
                    {
                        liberum = _xar_liberum_obtinere(nodus->liberi, i);
                        si (liberum)
                        {
                            _scribere_nucleus(liberum, aedificator, FALSUM, fidelitas, ZEPHYRUM, sedes);
                        }
                    }
                }
                /* extensio: tagum solum (inter captum retro et
                 * captos ante in fonte) - notata POST captos
                 * omnes, unde post-ordo tabulae (01M0X12PWS) */
                _sedes_notare(sedes, nodus, initium_tagi,
                              finis_tagi);
                initium_sedis = -I;
            }
            alioquin si (nodus->captio_directio == STML_CAPTIO_ANTE)
            {
                /* Forward capture: <tag (> or <tag ((> */
                i32 j;
                i32 finis_tagi;
                chorda_aedificator_appendere_character(aedificator, '<');
                si (nodus->titulus)
                {
                    chorda_aedificator_appendere_chorda(aedificator, *nodus->titulus);
                }
                si (nodus->crudus)
                {
                    chorda_aedificator_appendere_character(aedificator, '!');
                }
                /* Attributes (§0.2 decretum quintum: positione
                 * bloci ultra tectum - attributa stackata,
                 * clausura capturae linea propria; latitudo
                 * inline = spatium + parentheses + '>') */
                si (   pulchrum
                    && _attributa_multilinea_oportet(nodus,
                           indentatio,
                           II + nodus->captio_numerus))
                {
                    _attributa_multilinea_scribere(aedificator,
                        nodus, indentatio * II,
                        nodus->captio_numerus);
                }
                alioquin
                {
                _attributa_scribere(aedificator, nodus, fidelitas);
                /* spatium prae parenthesibus (§1.6 emendatum):
                 * fidelitas octetos conditos reddit (NIHIL =
                 * glutinata '<tag(>'); pulcher formam canonicam
                 * generat - glutinata sine attributis, spatium
                 * unicum post attributa (decretum §0.2) */
                si (fidelitas)
                {
                    si (nodus->spatia_intra_tagum != NIHIL)
                    {
                        chorda_aedificator_appendere_chorda(
                            aedificator, *nodus->spatia_intra_tagum);
                    }
                }
                alioquin si (   nodus->attributa != NIHIL
                             && xar_numerus(nodus->attributa) > ZEPHYRUM)
                {
                    chorda_aedificator_appendere_character(aedificator, ' ');
                }
                per (j = ZEPHYRUM; j < nodus->captio_numerus; j++)
                {
                    chorda_aedificator_appendere_character(aedificator, '(');
                }
                chorda_aedificator_appendere_character(aedificator, '>');
                }
                finis_tagi =
                    (i32)chorda_aedificator_longitudo(aedificator);

                /* ORDO FLUMINIS (§6, in M1 tractum): captor NON
                 * crudus - post inter tagum et captos in fonte
                 * sedet ('<a (>\n<b/>'), ergo ANTE liberos
                 * emittitur. Captor CRUDUS - linea capta INLINE
                 * tagum sequitur et '\n' eam TERMINAT
                 * ('<v! (>linea\n'), ergo post POST liberos venit.
                 * Uncus 'SED SEMEL TANTUM' (2026-08-06) DELETUS:
                 * terminator nunc spatia_post captoris est,
                 * explicitus; custos grammaticae manet arboribus
                 * manu structis (linea capta '\n' terminari DEBET,
                 * aliter frater sequens in relectione devoratur). */
                si (fidelitas && !nodus->crudus)
                {
                    si (nodus->spatia_post != NIHIL)
                    {
                        chorda_aedificator_appendere_chorda(
                            aedificator, *nodus->spatia_post);
                    }
                    post_iam_emissum = VERUM;
                }
                alioquin si (   !nodus->crudus
                             && nodus->captio_numerus == I)
                {
                    /* pulcher: separator canonicus post '(>' pro
                     * captoribus UNIGENIS (§0.2; regula capturae
                     * eum relegit in spatia_post). Multi-captei
                     * glutinati manent: spatium inter captos nodus
                     * textus fieret et numerationem corrumperet
                     * (angulus M4). Punctum fixum collapsus hinc
                     * pendet. */
                    chorda_aedificator_appendere_character(
                        aedificator, ' ');
                }

                /* Serialize children inline (no closing tag for captures) */
                si (nodus->liberi)
                {
                    num = xar_numerus(nodus->liberi);
                    per (i = ZEPHYRUM; i < num; i++)
                    {
                        liberum = _xar_liberum_obtinere(nodus->liberi, i);
                        si (liberum)
                        {
                            si (nodus->crudus && liberum->genus == STML_NODUS_TEXTUS && liberum->valor)
                            {
                                /* Raw content - non evasus; interius '!\'
                                 * trivia sua et praefixum secum fert */
                                si (nodus->multilinea)
                                {
                                    si (liberum->spatia_ante != NIHIL)
                                    {
                                        chorda_aedificator_appendere_chorda(aedificator, *liberum->spatia_ante);
                                    }
                                    _valorem_praefixo_scribere(aedificator, *liberum->valor, nodus->indentatio, FALSUM, liberum->spatia_ante != NIHIL ? VERUM : FALSUM);
                                    si (liberum->spatia_post != NIHIL)
                                    {
                                        chorda_aedificator_appendere_chorda(aedificator, *liberum->spatia_post);
                                    }
                                }
                                alioquin
                                {
                                    chorda_aedificator_appendere_chorda(aedificator, *liberum->valor);
                                }
                            }
                            alioquin
                            {
                                _scribere_nucleus(liberum, aedificator, FALSUM, fidelitas, ZEPHYRUM, sedes);
                            }
                        }
                    }
                }

                si (nodus->crudus)
                {
                    /* crudus: lexema capturae lineam captam IPSAM
                     * fert ('\n' flumini relicto) - extensio
                     * parsatoris eam includit, scriptor congruit */
                    finis_tagi =
                        (i32)chorda_aedificator_longitudo(aedificator);
                }
                si (fidelitas && nodus->crudus)
                {
                    si (nodus->spatia_post != NIHIL)
                    {
                        chorda_aedificator_appendere_chorda(
                            aedificator, *nodus->spatia_post);
                    }
                    alioquin
                    {
                        /* custos grammaticae (arbor manu structa) */
                        chorda_aedificator_appendere_character(
                            aedificator, '\n');
                    }
                    post_iam_emissum = VERUM;
                }
                /* extensio: tagum solum (crudus: + linea capta) -
                 * notata POST captos, unde post-ordo tabulae
                 * (01M0X12PWS) */
                _sedes_notare(sedes, nodus, (i32)initium_sedis,
                              finis_tagi);
                initium_sedis = -I;
            }
            alioquin
            {
                /* Normal element (no capture) */
                chorda_aedificator_appendere_character(aedificator, '<');
                si (nodus->titulus)
                {
                    chorda_aedificator_appendere_chorda(aedificator, *nodus->titulus);
                }

                /* Raw content marker */
                si (nodus->crudus)
                {
                    chorda_aedificator_appendere_character(aedificator, '!');
                }

                /* Signum multilineae (§1.4) - post '!' si adest */
                si (nodus->multilinea)
                {
                    chorda_aedificator_appendere_character(aedificator, '\\');
                }

                /* Attributes (§0.2 decretum quintum: positione
                 * bloci ultra tectum - attributum per lineam,
                 * clausura glutinata valori ultimo; latitudo
                 * clausurae I = '>', II = '/>') */
                si (   pulchrum
                    && _attributa_multilinea_oportet(nodus,
                           indentatio,
                           (   nodus->liberi != NIHIL
                            && xar_numerus(nodus->liberi)
                                   > ZEPHYRUM) ? I : II))
                {
                    _attributa_multilinea_scribere(aedificator,
                        nodus, indentatio * II, ZEPHYRUM);
                }
                alioquin
                {
                    _attributa_scribere(aedificator, nodus,
                                        fidelitas);
                }

                /* trivia intra tagum ante '>' aut '/>' (§1.6) */
                si (fidelitas && nodus->spatia_intra_tagum != NIHIL)
                {
                    chorda_aedificator_appendere_chorda(aedificator,
                        *nodus->spatia_intra_tagum);
                }

                /* Check if has children. Elementum sine liberis sed
                 * cum clausurae ('<a>\n</a>' - contentum totum album
                 * ad trivia migravit, §1.3) formam apertam tenet in
                 * modo non-pulchro - '/>' octetos interiores
                 * perderet. */
                habet_liberos = nodus->liberi && xar_numerus(nodus->liberi) > ZEPHYRUM;
                si (fidelitas && nodus->spatia_clausurae != NIHIL)
                {
                    habet_liberos = VERUM;
                }

                si (!habet_liberos)
                {
                    chorda_aedificator_appendere_literis(aedificator, "/>");
                }
                alioquin
                {
                    memoriae_index initium_contenti;
                               b32 clausura_tacita;

                    chorda_aedificator_appendere_character(aedificator, '>');
                    initium_contenti = chorda_aedificator_longitudo(aedificator);

                    num = nodus->liberi
                        ? xar_numerus(nodus->liberi) : ZEPHYRUM;

                    /* DISPOSITIO M2 (§4): decisio UNA per elementum,
                     * ex liberis derivata - liberum textus quodvis
                     * -> INLINE (liberi in linea tagi, sine terminis
                     * additis; textus solum-albus eiusdem lineae
                     * CONTENTUM est et SERVATUR, §1.3); aliter ->
                     * BLOCUS (liberum per lineam, indentatum).
                     * Machina TERMINI (2026-08-19) DELETA - vitium
                     * quartum familiae deletionem meruit:
                     * transparentia albi mortua quia cursus albi
                     * lineiferi nodi esse desierunt (§1.3), et
                     * vexillum liberi extinctum fidem iam non
                     * laedit quia fidelitas seorsum fluit. */
                    si (pulchrum && nodus->multilinea)
                    {
                        /* interius '\' et '!\': dispositio contenti
                         * declarata, sed praefixum ad profunditatem
                         * nidificationis REGENERATUM (§4) - nodi
                         * moti recte reformantur; interior numquam
                         * refluit. */
                        per (i = ZEPHYRUM; i < num; i++)
                        {
                            liberum = _xar_liberum_obtinere(nodus->liberi, i);
                            si (   liberum
                                && liberum->genus == STML_NODUS_TEXTUS
                                && liberum->valor)
                            {
                                _valorem_pulchre_indentare(
                                    aedificator,
                                    *liberum->valor,
                                    indentatio + I,
                                    nodus->crudus ? FALSUM : VERUM,
                                    liberum->spatia_ante != NIHIL
                                        ? VERUM : FALSUM);
                            }
                        }
                        chorda_aedificator_appendere_character(aedificator, '\n');
                        _scribere_indentatio(aedificator, indentatio);
                    }
                    alioquin si (nodus->crudus)
                    {
                        /* crudus: interior verbatim, numquam tactus */
                        per (i = ZEPHYRUM; i < num; i++)
                        {
                            liberum = _xar_liberum_obtinere(nodus->liberi, i);
                            si (liberum)
                            {
                                si (liberum->genus == STML_NODUS_TEXTUS && liberum->valor)
                                {
                                    /* Raw content - non evasus; interius '!\'
                                     * trivia sua et praefixum secum fert */
                                    si (nodus->multilinea)
                                    {
                                        si (liberum->spatia_ante != NIHIL)
                                        {
                                            chorda_aedificator_appendere_chorda(aedificator, *liberum->spatia_ante);
                                        }
                                        _valorem_praefixo_scribere(aedificator, *liberum->valor, nodus->indentatio, FALSUM, liberum->spatia_ante != NIHIL ? VERUM : FALSUM);
                                        si (liberum->spatia_post != NIHIL)
                                        {
                                            chorda_aedificator_appendere_chorda(aedificator, *liberum->spatia_post);
                                        }
                                    }
                                    alioquin
                                    {
                                        chorda_aedificator_appendere_chorda(aedificator, *liberum->valor);
                                    }
                                }
                                alioquin
                                {
                                    _scribere_nucleus(liberum, aedificator, FALSUM, fidelitas, ZEPHYRUM, sedes);
                                }
                            }
                        }
                    }
                    alioquin si (!pulchrum || _habet_liberum_textus(nodus))
                    {
                        /* series plana: fidelitas semper (liberi
                         * trivia sua ordine ferunt); pulcher INLINE
                         * quia textus adest */
                        per (i = ZEPHYRUM; i < num; i++)
                        {
                            liberum = _xar_liberum_obtinere(nodus->liberi, i);
                            si (liberum)
                            {
                                _scribere_nucleus(liberum, aedificator, FALSUM, fidelitas, ZEPHYRUM, sedes);
                            }
                        }
                    }
                    alioquin
                    {
                        /* BLOCUS: liberum per lineam, indentatum;
                         * lineae vacuae authoratae servatae (§4 -
                         * basis I libero primo: tagum apertum
                         * cursum totum ei dat) */
                        per (i = ZEPHYRUM; i < num; i++)
                        {
                            liberum = _xar_liberum_obtinere(nodus->liberi, i);
                            si (liberum)
                            {
                                i32 vacuae;

                                chorda_aedificator_appendere_character(aedificator, '\n');
                                per (vacuae = _lineae_vacuae(
                                         liberum->spatia_ante,
                                         (i == ZEPHYRUM) ? I : ZEPHYRUM);
                                     vacuae > ZEPHYRUM; vacuae--)
                                {
                                    chorda_aedificator_appendere_character(aedificator, '\n');
                                }
                                _scribere_nucleus(liberum, aedificator, VERUM, fidelitas, indentatio + I, sedes);
                            }
                        }
                        {
                            i32 vacuae;

                            chorda_aedificator_appendere_character(aedificator, '\n');
                            per (vacuae = _lineae_vacuae(
                                     nodus->spatia_clausurae,
                                     ZEPHYRUM);
                                 vacuae > ZEPHYRUM; vacuae--)
                            {
                                chorda_aedificator_appendere_character(aedificator, '\n');
                            }
                            _scribere_indentatio(aedificator, indentatio);
                        }
                    }

                    /* Clausura TACITA (2026-08-19): in modo pulchro
                     * nomen repetitum strepitus est cum contentum
                     * breve sit; ultra STML_CLAUSURA_TACITA_LINEAE
                     * nomen rursus AUXILIUM fit (tag apertum extra
                     * schermum abiit). Crudi clausuram EXPRESSAM
                     * tenent - grammatica '</>' eos claudere non
                     * sinit - ergo excepti. Modus NON-pulcher formam
                     * AUTHORIS intactam servat: ibi fidelitas
                     * octetim regnat, non dispositio nostra. */
                    clausura_tacita = nodus->clausura_anonyma;
                    si (pulchrum && !nodus->crudus)
                    {
                        clausura_tacita =
                            (_lineae_contenti(aedificator, initium_contenti)
                                <= STML_CLAUSURA_TACITA_LINEAE)
                            ? VERUM : FALSUM;
                    }

                    /* interius ante tagum claudentem (§1.1) */
                    si (fidelitas && nodus->spatia_clausurae != NIHIL)
                    {
                        chorda_aedificator_appendere_chorda(
                            aedificator, *nodus->spatia_clausurae);
                    }

                    chorda_aedificator_appendere_literis(aedificator, "</");
                    si (nodus->titulus && !clausura_tacita)
                    {
                        chorda_aedificator_appendere_chorda(aedificator, *nodus->titulus);
                    }
                    chorda_aedificator_appendere_character(aedificator, '>');
                }
            }
            frange;

        casus STML_NODUS_TEXTUS:
            /* Textus CONTENTUM est et SEMPER emittitur (M2 §4):
             * omissio pulchra nodorum solum-alborum deleta cum
             * TERMINIS - post §1.3 nodus textus superstes aut
             * contentum verum aut spatium eiusdem-lineae
             * DELIBERATUM est ('<sep>   </sep>'); utrumque sensus,
             * neutrum dispositio. Cumulatio vetus non redit quia
             * cursus albi lineiferi nodi esse desierunt. */
            si (nodus->valor)
            {
                si (   nodus->parens != NIHIL
                    && nodus->parens->multilinea)
                {
                    /* interius '<tag\>' in fidelitate: praefixum
                     * CONDITUM reinseritur (pulcher hunc casum non
                     * attingit - parens eum ipse emittit, praefixo
                     * ad profunditatem regenerato) */
                    _valorem_praefixo_scribere(aedificator,
                        *nodus->valor, nodus->parens->indentatio, VERUM,
                        nodus->spatia_ante != NIHIL ? VERUM : FALSUM);
                }
                alioquin
                {
                    _scribere_evasus(aedificator, nodus->valor);
                }
            }
            frange;

        casus STML_NODUS_COMMENTUM:
            si (pulchrum)
            {
                _scribere_indentatio(aedificator, indentatio);
            }
            chorda_aedificator_appendere_literis(aedificator, "<!--");
            si (nodus->valor)
            {
                chorda_aedificator_appendere_chorda(aedificator, *nodus->valor);
            }
            chorda_aedificator_appendere_literis(aedificator, "-->");
            frange;

        casus STML_NODUS_PROCESSIO:
            si (pulchrum)
            {
                _scribere_indentatio(aedificator, indentatio);
            }
            chorda_aedificator_appendere_literis(aedificator, "<?");
            si (nodus->valor)
            {
                chorda_aedificator_appendere_chorda(aedificator, *nodus->valor);
            }
            chorda_aedificator_appendere_literis(aedificator, "?>");
            frange;

        casus STML_NODUS_DOCTYPE:
            si (pulchrum)
            {
                _scribere_indentatio(aedificator, indentatio);
            }
            chorda_aedificator_appendere_literis(aedificator, "<!DOCTYPE ");
            si (nodus->valor)
            {
                chorda_aedificator_appendere_chorda(aedificator, *nodus->valor);
            }
            chorda_aedificator_appendere_character(aedificator, '>');
            frange;

        casus STML_NODUS_TRANSCLUSIO:
            /* Indentatio ut ceteri (2026-08-19): casus hic soli
             * ELEMENTO/COMMENTO/PROCESSIONI/DOCTYPE aderat, unde
             * transclusio inter fratres indentatos nuda stabat */
            si (pulchrum)
            {
                _scribere_indentatio(aedificator, indentatio);
            }
            chorda_aedificator_appendere_literis(aedificator, "<<");
            si (nodus->valor)
            {
                chorda_aedificator_appendere_chorda(aedificator, *nodus->valor);
            }
            chorda_aedificator_appendere_literis(aedificator, ">>");
            frange;

        ordinarius:
            frange;
    }

    /* Elementum notare ubi CLAUDITUR (unde post-ordo tabulae);
     * initium_sedis in casu ELEMENTI solo ponitur - rami capturae
     * (ANTE/RETRO/FARCIMEN/fragmentum capturans) notam propriam
     * scribunt et sentinellam -I reponunt (extensio eorum tagum
     * solum est, non captos - quaestio 01M0X12PWS), cetera genera
     * sentinellam numquam tangunt. */
    si (sedes != NIHIL && initium_sedis >= ZEPHYRUM)
    {
        _sedes_notare(sedes, nodus, (i32)initium_sedis,
                      (i32)chorda_aedificator_longitudo(aedificator));
    }

    /* spatia_post EXTRA extensionem sedium (post notationem);
     * captores ANTE ea iam intra formam emiserunt (§6) */
    si (   (fidelitas || _intra_multilineam(nodus))
        && !post_iam_emissum
        && nodus->spatia_post != NIHIL)
    {
        chorda_aedificator_appendere_chorda(aedificator,
                                            *nodus->spatia_post);
    }

    redde VERUM;
}

b32
stml_scribere_ad_aedificator (
            StmlNodus* nodus,
    ChordaAedificator* aedificator,
                  b32  pulchrum,
                  i32  indentatio)
{
    redde _scribere_nucleus(nodus, aedificator, pulchrum,
                            pulchrum ? FALSUM : VERUM,
                            indentatio, NIHIL);
}

chorda
stml_scribere (
    StmlNodus* nodus,
      Piscina* piscina,
          b32  pulchrum)
{
    redde stml_scribere_sedibus(nodus, piscina, pulchrum, NIHIL);
}

chorda
stml_scribere_sedibus (
    StmlNodus* nodus,
      Piscina* piscina,
          b32  pulchrum,
          Xar* sedes)
{
    ChordaAedificator* aed;
               chorda  result;

    result.datum    = NIHIL;
    result.mensura  = ZEPHYRUM;

    si (!nodus || !piscina)
    {
        redde result;
    }

    aed = chorda_aedificator_creare(piscina, DXII);
    si (!aed)
    {
        redde result;
    }

    _scribere_nucleus(nodus, aed, pulchrum,
                      pulchrum ? FALSUM : VERUM, ZEPHYRUM, sedes);

    redde chorda_aedificator_finire(aed);
}


/* ==================================================
 * Tituli (Labels) Implementation
 * ================================================== */

/* Helper: get labels attribute value (checks "labels" then "class") */
interior chorda*
_stml_titulos_valor (
    StmlNodus* nodus)
{
    chorda* valor;

    valor = stml_attributum_capere(nodus, "labels");
    si (valor)
    {
        redde valor;
    }

    /* HTML compat: also check "class" */
    redde stml_attributum_capere(nodus, "class");
}

/* Helper: find attribute index by name */
interior s32
_stml_attributum_index (
             StmlNodus* nodus,
    constans character* titulus)
{
               s32  i;
               s32  num;
    StmlAttributum* attr;

    si (!nodus || !nodus->attributa)
    {
        redde -I;
    }

    num = (s32)xar_numerus(nodus->attributa);
    per (i = ZEPHYRUM; i < num; i++)
    {
        attr = (StmlAttributum*)xar_obtinere(nodus->attributa, (i32)i);
        si (   attr && attr->titulus
            && chorda_aequalis_literis(*attr->titulus, titulus))
        {
            redde i;
        }
    }

    redde -I;
}

b32
stml_titulum_habet (
             StmlNodus* nodus,
    constans character* titulum)
{
    chorda* labels_valor;
       i32  target_len;
       i32  i;
       i32  j;
       i32  start;
       i32  end;
       b32  match;

    si (!nodus || !titulum)
    {
        redde FALSUM;
    }

    labels_valor = _stml_titulos_valor(nodus);
    si (!labels_valor || labels_valor->mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }

    /* Get target length */
    target_len = ZEPHYRUM;
    dum (titulum[target_len] != '\0')
    {
        target_len++;
    }

    si (target_len == ZEPHYRUM)
    {
        redde FALSUM;
    }

    /* Scan through labels, comparing each space-separated token */
    i = ZEPHYRUM;
    dum (i < labels_valor->mensura)
    {
        /* Skip leading spaces */
        dum (   i < labels_valor->mensura
             && (labels_valor->datum[i] == ' '
            || labels_valor->datum[i] == '\t'))
        {
            i++;
        }

        si (i >= labels_valor->mensura)
        {
            frange;
        }

        /* Find end of token */
        start = i;
        dum (   i < labels_valor->mensura
             && labels_valor->datum[i] != ' '
             && labels_valor->datum[i] != '\t')
        {
            i++;
        }
        end = i;

        /* Check if this token matches */
        si ((end - start) == target_len)
        {
            match = VERUM;
            per (j = ZEPHYRUM; j < target_len; j++)
            {
                si ((character)labels_valor->datum[start + j] != titulum[j])
                {
                    match = FALSUM;
                    frange;
                }
            }

            si (match)
            {
                redde VERUM;
            }
        }
    }

    redde FALSUM;
}

i32
stml_titulos_numerus (
    StmlNodus* nodus)
{
    chorda* labels_valor;
       i32  i;
       i32  count;
       b32  in_token;

    si (!nodus)
    {
        redde ZEPHYRUM;
    }

    labels_valor = _stml_titulos_valor(nodus);
    si (!labels_valor || labels_valor->mensura == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }

    /* Count non-empty tokens separated by spaces */
    count     = ZEPHYRUM;
    in_token  = FALSUM;

    per (i = ZEPHYRUM; i < labels_valor->mensura; i++)
    {
        si (labels_valor->datum[i] == ' ' || labels_valor->datum[i] == '\t')
        {
            in_token = FALSUM;
        }
        alioquin
        {
            si (!in_token)
            {
                count++;
                in_token = VERUM;
            }
        }
    }

    redde count;
}

Xar*
stml_titulos_capere (
    StmlNodus* nodus,
      Piscina* piscina)
{
                   chorda* labels_valor;
    chorda_fissio_fructus  fissio;
                      Xar* result;
                      i32  i;
                   chorda  trimmed;
                   chorda* slot;

    si (!nodus || !piscina)
    {
        redde NIHIL;
    }

    labels_valor = _stml_titulos_valor(nodus);
    si (!labels_valor || labels_valor->mensura == ZEPHYRUM)
    {
        /* Return empty array */
        redde xar_creare(piscina, magnitudo(chorda));
    }

    /* Split by space */
    fissio = chorda_fissio(*labels_valor, ' ', piscina);
    si (!fissio.elementa)
    {
        redde xar_creare(piscina, magnitudo(chorda));
    }

    result = xar_creare(piscina, magnitudo(chorda));
    si (!result)
    {
        redde NIHIL;
    }

    per (i = ZEPHYRUM; i < fissio.numerus; i++)
    {
        trimmed = chorda_praecidere(fissio.elementa[i]);
        si (trimmed.mensura > ZEPHYRUM)
        {
            slot = (chorda*)xar_addere(result);
            si (slot)
            {
                *slot = trimmed;
            }
        }
    }

    redde result;
}

b32
stml_titulum_addere (
              StmlNodus* nodus,
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* titulum)
{
               chorda* labels_valor;
    ChordaAedificator* aed;
               chorda  new_valor;
               chorda* interned;
                  s32  attr_index;
       StmlAttributum* attr;

    si (!nodus || !piscina || !intern || !titulum)
    {
        redde FALSUM;
    }

    /* Check if already has this label */
    si (stml_titulum_habet(nodus, titulum))
    {
        redde FALSUM;  /* Already exists */
    }

    labels_valor = _stml_titulos_valor(nodus);

    si (!labels_valor)
    {
        /* No labels attribute exists - add a new one */
        redde stml_attributum_addere(nodus, piscina, intern, "labels", titulum);
    }

    si (labels_valor->mensura == ZEPHYRUM)
    {
        /* Labels attribute exists but is empty - update it */
        attr_index = _stml_attributum_index(nodus, "labels");
        si (attr_index < ZEPHYRUM)
        {
            attr_index = _stml_attributum_index(nodus, "class");
        }

        si (attr_index >= ZEPHYRUM)
        {
            attr = (StmlAttributum*)xar_obtinere(nodus->attributa, (i32)attr_index);
            si (attr)
            {
                attr->valor = chorda_internare_ex_literis(intern, titulum);
                redde attr->valor != NIHIL;
            }
        }
        redde FALSUM;
    }

    /* Append to existing labels */
    aed = chorda_aedificator_creare(piscina, CXXVIII);
    si (!aed)
    {
        redde FALSUM;
    }

    chorda_aedificator_appendere_chorda(aed, *labels_valor);
    chorda_aedificator_appendere_character(aed, ' ');
    chorda_aedificator_appendere_literis(aed, titulum);

    new_valor  = chorda_aedificator_finire(aed);
    interned   = chorda_internare(intern, new_valor);
    si (!interned)
    {
        redde FALSUM;
    }

    /* Update existing attribute */
    attr_index = _stml_attributum_index(nodus, "labels");
    si (attr_index < ZEPHYRUM)
    {
        attr_index = _stml_attributum_index(nodus, "class");
    }

    si (attr_index >= ZEPHYRUM)
    {
        attr = (StmlAttributum*)xar_obtinere(nodus->attributa, (i32)attr_index);
        si (attr)
        {
            attr->valor = interned;
            redde VERUM;
        }
    }

    redde FALSUM;
}

b32
stml_titulum_removere (
              StmlNodus* nodus,
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* titulum)
{
                   chorda* labels_valor;
    chorda_fissio_fructus  fissio;
        ChordaAedificator* aed;
                   chorda  trimmed;
                   chorda  new_valor;
                   chorda* interned;
                      i32  i;
                      s32  attr_index;
           StmlAttributum* attr;
                      b32  found;
                      b32  first;

    si (!nodus || !piscina || !intern || !titulum)
    {
        redde FALSUM;
    }

    labels_valor = _stml_titulos_valor(nodus);
    si (!labels_valor || labels_valor->mensura == ZEPHYRUM)
    {
        redde FALSUM;  /* No labels to remove from */
    }

    /* Split and rebuild without the target */
    fissio = chorda_fissio(*labels_valor, ' ', piscina);
    si (!fissio.elementa)
    {
        redde FALSUM;
    }

    aed = chorda_aedificator_creare(piscina, CXXVIII);
    si (!aed)
    {
        redde FALSUM;
    }

    found = FALSUM;
    first = VERUM;

    per (i = ZEPHYRUM; i < fissio.numerus; i++)
    {
        trimmed = chorda_praecidere(fissio.elementa[i]);
        si (trimmed.mensura == ZEPHYRUM)
        {
            perge;
        }

        si (chorda_aequalis_literis(trimmed, titulum))
        {
            found = VERUM;
            perge;  /* Skip this one */
        }

        si (!first)
        {
            chorda_aedificator_appendere_character(aed, ' ');
        }
        chorda_aedificator_appendere_chorda(aed, trimmed);
        first = FALSUM;
    }

    si (!found)
    {
        redde FALSUM;  /* Label wasn't present */
    }

    new_valor = chorda_aedificator_finire(aed);

    /* Find the attribute to update */
    attr_index = _stml_attributum_index(nodus, "labels");
    si (attr_index < ZEPHYRUM)
    {
        attr_index = _stml_attributum_index(nodus, "class");
    }

    si (attr_index < ZEPHYRUM)
    {
        redde FALSUM;  /* Attribute not found */
    }

    attr = (StmlAttributum*)xar_obtinere(nodus->attributa, (i32)attr_index);
    si (!attr)
    {
        redde FALSUM;
    }

    /* Handle empty result (all labels removed) */
    si (new_valor.mensura == ZEPHYRUM)
    {
        /* Create an empty chorda - allocate space for the chorda struct */
        interned = (chorda*)piscina_allocare(piscina, magnitudo(chorda));
        si (!interned)
        {
            redde FALSUM;
        }
        /* Allocate 1 byte so datum is valid, but mensura is 0 */
        interned->datum    = (i8*)piscina_allocare(piscina, I);
        interned->mensura  = ZEPHYRUM;
    }
    alioquin
    {
        interned = chorda_internare(intern, new_valor);
        si (!interned)
        {
            redde FALSUM;
        }
    }

    attr->valor = interned;
    redde VERUM;
}

b32
stml_titulum_commutare (
              StmlNodus* nodus,
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* titulum)
{
    si (!nodus || !piscina || !intern || !titulum)
    {
        redde FALSUM;
    }

    si (stml_titulum_habet(nodus, titulum))
    {
        stml_titulum_removere(nodus, piscina, intern, titulum);
        redde FALSUM;  /* Now doesn't have it */
    }
    alioquin
    {
        stml_titulum_addere(nodus, piscina, intern, titulum);
        redde VERUM;  /* Now has it */
    }
}


/* ==================================================
 * Strictum - forma BENE FORMATA super parsationem
 *
 * Vide stml.h pro doctrina (parser lenis, politia vocantis) et
 * pro eo QUOD HIC NON EST: attributum sine quotis et ens ignotum
 * mensurata et ACCEPTA sunt - non omne discrimen ab XML defectus.
 * ================================================== */

interior vacuum
_strictum_addere (
                  Xar* vitia,
    StmlStrictumGenus  genus,
            StmlNodus* nodus,
               chorda* causa)
{
    StmlStrictumVitium* v;

    v = (StmlStrictumVitium*)xar_addere(vitia);
    si (!v)
    {
        redde;
    }

    v->genus = genus;
    v->nodus = nodus;
    v->causa = causa;
}

/* an chorda spatium album solum ferat */
interior b32
_strictum_album_solum (
    constans chorda* s)
{
    i32 i;

    per (i = ZEPHYRUM; i < s->mensura; i++)
    {
        character c;

        c = (character)s->datum[i];
        si (c != ' ' && c != '\t' && c != '\n' && c != '\r')
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}

interior vacuum
_strictum_nodum (
    StmlNodus* nodus,
          Xar* vitia)
{
    i32 numerus;
    i32 i;

    si (!nodus)
    {
        redde;
    }

    si (nodus->genus == STML_NODUS_ELEMENTUM)
    {
        /* elementum sine nomine: '<>' */
        si (!nodus->titulus || nodus->titulus->mensura == ZEPHYRUM)
        {
            si (!nodus->fragmentum)
            {
                _strictum_addere(vitia,
                    STML_STRICTUM_TITULUS_VACUUS, nodus, NIHIL);
            }
        }

        /* attributum bis: quadraticum consulto - elementa attributa
         * pauca ferunt (in corpore naturae maximum VII), et tabula
         * dispersa hic plus constaret quam solveret */
        numerus = xar_numerus(nodus->attributa);
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            StmlAttributum* a;
                       i32  j;

            a = (StmlAttributum*)xar_obtinere(nodus->attributa, i);
            per (j = ZEPHYRUM; j < i; j++)
            {
                StmlAttributum* prius;

                prius = (StmlAttributum*)xar_obtinere(
                    nodus->attributa, j);
                si (   prius->titulus && a->titulus
                    && chorda_aequalis(*prius->titulus, *a->titulus))
                {
                    _strictum_addere(vitia,
                        STML_STRICTUM_ATTRIBUTUM_DUPLICATUM,
                        nodus, a->titulus);
                    frange;
                }
            }
        }
    }

    numerus = stml_numerus_liberorum(nodus);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        _strictum_nodum(stml_liberum_ad_indicem(nodus, i), vitia);
    }
}

Xar*
stml_strictum (
    StmlNodus* radix,
      Piscina* piscina)
{
    Xar* vitia;
    i32  numerus;
    i32  i;
    i32  radices;

    si (!piscina)
    {
        redde NIHIL;
    }

    vitia = xar_creare(piscina, (i32)magnitudo(StmlStrictumVitium));
    si (!vitia || !radix)
    {
        redde vitia;
    }

    /* gradus documenti: radices plures + textus vagus */
    radices = ZEPHYRUM;
    numerus = stml_numerus_liberorum(radix);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* liberum;

        liberum = stml_liberum_ad_indicem(radix, i);
        si (!liberum)
        {
            perge;
        }

        si (liberum->genus == STML_NODUS_ELEMENTUM)
        {
            radices++;
            si (radices == II)
            {
                _strictum_addere(vitia,
                    STML_STRICTUM_RADICES_PLURES, liberum, NIHIL);
            }
        }
        alioquin si (liberum->genus == STML_NODUS_TEXTUS)
        {
            si (   liberum->valor
                && !_strictum_album_solum(liberum->valor))
            {
                _strictum_addere(vitia,
                    STML_STRICTUM_TEXTUS_EXTRA_RADICEM, liberum,
                    NIHIL);
            }
        }
    }

    _strictum_nodum(radix, vitia);

    redde vitia;
}

constans character*
stml_strictum_nuntius (
    StmlStrictumGenus genus)
{
    commutatio (genus)
    {
        casus STML_STRICTUM_ATTRIBUTUM_DUPLICATUM:
            redde "attributum idem bis (primum vincit in lectione, "
                  "utrumque in scriptura - plagula aliud dicit "
                  "quam lectores adhibent)";
        casus STML_STRICTUM_RADICES_PLURES:
            redde "elementa radicis plura - reliqua tacite "
                  "ignorantur";
        casus STML_STRICTUM_TEXTUS_EXTRA_RADICEM:
            redde "textus extra elementum radicis";
        casus STML_STRICTUM_TITULUS_VACUUS:
            redde "elementum sine nomine";
        ordinarius:
            redde "vitium ignotum";
    }
}
