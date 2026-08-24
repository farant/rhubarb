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
 */
interior Xar*
_tok_legere_attributa (
    StmlTokenContext* ctx)
{
               Xar* attributa;
            chorda  titulus_ch;
            chorda  valor_ch;
            chorda* titulus_ptr;
            chorda* valor_ptr;
    StmlAttributum* attr;

    attributa = xar_creare(ctx->piscina, magnitudo(StmlAttributum));
    si (!attributa)
    {
        redde NIHIL;
    }

    dum (ctx->positus < ctx->input.mensura)
    {
        _tok_praeterire_spatium(ctx);

        /* Check for end of tag */
        si (   _tok_aspicere(ctx, ZEPHYRUM) == '>'
            || (_tok_aspicere(ctx, ZEPHYRUM) == '/' && _tok_aspicere(ctx, I) == '>')
            || _tok_aspicere(ctx, ZEPHYRUM) == '(' ||  /* Forward capture */
            _tok_aspicere(ctx, ZEPHYRUM) == '=')    /* Sandwich capture (after <=) */
        {
            frange;
        }

        /* Check if this looks like an attribute name */
        si (!_est_nomen_initium(_tok_aspicere(ctx, ZEPHYRUM)))
        {
            frange;
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
                attr->titulus  = titulus_ptr;
                attr->valor    = valor_ptr;
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
            attr->titulus  = titulus_ptr;
            attr->valor    = valor_ptr;
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

    token.genus            = STML_TOKEN_COMMENTUM;
    token.positus_initium  = initium;
    token.positus_finis    = ctx->positus;
    token.linea            = initium_linea;
    token.columna          = initium_columna;
    token.attributa        = NIHIL;
    token.captio_numerus   = ZEPHYRUM;
    token.habet_captus     = FALSUM;

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

    token.genus            = STML_TOKEN_PROCESSIO;
    token.positus_initium  = initium;
    token.positus_finis    = ctx->positus;
    token.linea            = initium_linea;
    token.columna          = initium_columna;
    token.attributa        = NIHIL;
    token.captio_numerus   = ZEPHYRUM;
    token.habet_captus     = FALSUM;

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

    token.genus            = STML_TOKEN_DOCTYPE;
    token.positus_initium  = initium;
    token.positus_finis    = ctx->positus;
    token.linea            = initium_linea;
    token.columna          = initium_columna;
    token.attributa        = NIHIL;
    token.captio_numerus   = ZEPHYRUM;
    token.habet_captus     = FALSUM;

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

    token.attributa                 = NIHIL;
    token.captio_numerus            = ZEPHYRUM;
    token.habet_captus              = FALSUM;
    token.captus_contentus.datum    = NIHIL;
    token.captus_contentus.mensura  = ZEPHYRUM;

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

    _tok_praeterire_spatium(ctx);

    /* Parse attributes */
    token.attributa = _tok_legere_attributa(ctx);
    _tok_praeterire_spatium(ctx);

    /* Check for forward capture (( */
    captio_numerus = ZEPHYRUM;
    dum (_tok_aspicere(ctx, ZEPHYRUM) == '(')
    {
        captio_numerus++;
        _tok_progredi(ctx, I);
    }

    /* Skip whitespace after capture parens */
    _tok_praeterire_spatium(ctx);

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

    token.attributa                 = NIHIL;
    token.captio_numerus            = ZEPHYRUM;
    token.habet_captus              = FALSUM;
    token.captus_contentus.datum    = NIHIL;
    token.captus_contentus.mensura  = ZEPHYRUM;

    /* Skip < */
    _tok_progredi(ctx, I);

    /* Skip # */
    _tok_progredi(ctx, I);

    /* Read optional fragment ID */
    fragmentum_id = _tok_legere_fragmentum_id(ctx);

    _tok_praeterire_spatium(ctx);

    /* Parse attributes */
    token.attributa = _tok_legere_attributa(ctx);
    _tok_praeterire_spatium(ctx);

    /* Numerare parentheses capturae <# (> / <#id (> (post
     * attributa, sicut in tags normalibus) */
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

    token.attributa                 = NIHIL;
    token.captio_numerus            = ZEPHYRUM;
    token.habet_captus              = FALSUM;
    token.captus_contentus.datum    = NIHIL;
    token.captus_contentus.mensura  = ZEPHYRUM;

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

    token.attributa                 = NIHIL;
    token.captio_numerus            = ZEPHYRUM;
    token.habet_captus              = FALSUM;
    token.captus_contentus.datum    = NIHIL;
    token.captus_contentus.mensura  = ZEPHYRUM;
    token.valor.datum               = NIHIL;
    token.valor.mensura             = ZEPHYRUM;

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

    token.attributa                 = NIHIL;
    token.captio_numerus            = ZEPHYRUM;
    token.habet_captus              = FALSUM;
    token.captus_contentus.datum    = NIHIL;
    token.captus_contentus.mensura  = ZEPHYRUM;
    token.valor.datum               = NIHIL;
    token.valor.mensura             = ZEPHYRUM;

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

    token.attributa                 = NIHIL;
    token.captio_numerus            = ZEPHYRUM;
    token.habet_captus              = FALSUM;
    token.captus_contentus.datum    = NIHIL;
    token.captus_contentus.mensura  = ZEPHYRUM;

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

    token.attributa                 = NIHIL;
    token.captio_numerus            = ZEPHYRUM;
    token.habet_captus              = FALSUM;
    token.captus_contentus.datum    = NIHIL;
    token.captus_contentus.mensura  = ZEPHYRUM;

    /* Skip << */
    _tok_progredi(ctx, II);

    /* Skip leading whitespace */
    _tok_praeterire_spatium(ctx);
    selector_initium = ctx->positus;

    /* Read until >> */
    dum (ctx->positus < ctx->input.mensura)
    {
        si (   _tok_aspicere(ctx, ZEPHYRUM) == '>'
            && _tok_aspicere(ctx, I)        == '>')
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

    token.genus            = STML_TOKEN_TEXTUS;
    token.valor.datum      = ctx->input.datum + initium;
    token.valor.mensura    = ctx->positus - initium;
    token.positus_initium  = initium;
    token.positus_finis    = ctx->positus;
    token.linea            = initium_linea;
    token.columna          = initium_columna;
    token.attributa        = NIHIL;
    token.captio_numerus   = ZEPHYRUM;
    token.habet_captus     = FALSUM;

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

    token.genus            = STML_TOKEN_TEXTUS;
    token.valor.datum      = ctx->input.datum + initium;
    token.valor.mensura    = ctx->positus - initium;
    token.positus_initium  = initium;
    token.positus_finis    = ctx->positus;
    token.linea            = initium_linea;
    token.columna          = initium_columna;
    token.attributa        = NIHIL;
    token.captio_numerus   = ZEPHYRUM;
    token.habet_captus     = FALSUM;

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
        token.genus            = STML_TOKEN_FINIS;
        token.valor.datum      = NIHIL;
        token.valor.mensura    = ZEPHYRUM;
        token.positus_initium  = ctx->positus;
        token.positus_finis    = ctx->positus;
        token.linea            = ctx->linea;
        token.columna          = ctx->columna;
        token.attributa        = NIHIL;
        token.captio_numerus   = ZEPHYRUM;
        token.habet_captus     = FALSUM;
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

    titulus           = ctx->current.valor;
    titulus_ptr       = chorda_internare(ctx->intern, titulus);
    nodus->titulus    = titulus_ptr;
    nodus->attributa  = ctx->current.attributa;

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
            ctx->status           = STML_ERROR_TAG_IMPROPRIE;
            ctx->linea_erroris    = ctx->current.linea;
            ctx->columna_erroris  = ctx->current.columna;
        }
        _parser_progredi(ctx);  /* Consume close tag */
    }
    alioquin si (ctx->current.genus == STML_TOKEN_FINIS)
    {
        /* Unclosed tag at EOF */
        ctx->status           = STML_ERROR_TAG_NON_CLAUSUM;
        ctx->linea_erroris    = ctx->current.linea;
        ctx->columna_erroris  = ctx->current.columna;
    }

    /* extensio: finis = tokenum ultimum consumptum */
    nodus->positus_finis = ctx->finis_ultimus;

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

    nodus = _parser_creare_nodus(ctx, STML_NODUS_ELEMENTUM);
    si (!nodus) redde NIHIL;

    titulus                = ctx->current.valor;
    titulus_ptr            = chorda_internare(ctx->intern, titulus);
    nodus->titulus         = titulus_ptr;
    nodus->attributa       = ctx->current.attributa;
    nodus->crudus          = VERUM;
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

    nodus = _parser_creare_nodus(ctx, STML_NODUS_ELEMENTUM);
    si (!nodus) redde NIHIL;

    titulus_ptr = chorda_internare(ctx->intern, ctx->current.valor);
    nodus->titulus = titulus_ptr;
    nodus->attributa = ctx->current.attributa;
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

/* Classificatio cursus textus (§1.3), ab ansa liberorum vocata.
 *
 * - cursus TOTUS albus lineam-ferens: NULLUS nodus - octeti per
 *   §1.2 distribuuntur (elisio quae vectigal ambulatorum delet et
 *   numerationem captionum constructione sanat)
 * - cursus albus UNIUS lineae: nodus manet, valor integer
 *   ('<sep>   </sep>' tria spatia sua tenet)
 * - cursus contentum ferens: margo (praefixum/suffixum album
 *   maximale) e valore exit SI lineam novam fert; octeti exeuntes
 *   per §1.2 distribuuntur (pars prioris -> spatia_post eius,
 *   residuum -> spatia_ante huius/sequentis)
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
        ctx->status           = STML_ERROR_TAG_NON_CLAUSUM;
        ctx->linea_erroris    = ctx->current.linea;
        ctx->columna_erroris  = ctx->current.columna;
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
            ctx->status = STML_ERROR_SYNTAXIS;
            ctx->linea_erroris = ctx->current.linea;
            ctx->columna_erroris = ctx->current.columna;
            _parser_progredi(ctx);  /* Consume to avoid infinite loop */
            redde NIHIL;

        casus STML_TOKEN_CLAUDERE:
        casus STML_TOKEN_FRAGMENTUM_CLAUDERE:
        casus STML_TOKEN_PERCENTUM_CLAUDERE:
            /* Orphan closing tag - no matching open tag */
            ctx->status = STML_ERROR_TAG_IMPROPRIE;
            ctx->linea_erroris = ctx->current.linea;
            ctx->columna_erroris = ctx->current.columna;
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
             * parsationis - linea intus, fratres non capiendi) */
            si (   liberum->captio_directio == STML_CAPTIO_ANTE
                && !liberum->crudus)
            {
                StmlNodus** slot_c;
                captio_count    = liberum->captio_numerus;
                captured_count  = ZEPHYRUM;

                /* Capture next N non-comment siblings */
                j = i + I;
                dum (j < num && captured_count < captio_count)
                {
                    captus = _xar_liberum_obtinere(nodus->liberi, j);
                    si (captus && !_est_commentum(captus))
                    {
                        captus->parens  = liberum;
                        slot_c          = xar_addere(liberum->liberi);
                        si (slot_c) *slot_c = captus;
                        captured_count++;
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
            chorda_aedificator_appendere_chorda(aed,
                *liberum->valor);
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
            chorda_aedificator_appendere_chorda(aed, *nodus->valor);
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

/* an chorda spatium album SOLUM ferat (vacua quoque) */
interior b32
_spatium_album_solum (
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
                    attr_new->titulus  = attr_orig->titulus;  /* Internatum */
                    attr_new->valor    = attr_orig->valor;      /* Internatum */
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

    attr->titulus  = chorda_internare_ex_literis(intern, titulus);
    attr->valor    = chorda_internare_ex_literis(intern, valor);

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

    attr->titulus  = chorda_internare_ex_literis(intern, titulus);
    attr->valor    = chorda_internare(intern, valor);

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
    StmlAttributum* attr;
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
     * trivia IGNORAT et dispositionem suam generat. */
    si (fidelitas && nodus->spatia_ante != NIHIL)
    {
        chorda_aedificator_appendere_chorda(aedificator,
                                            *nodus->spatia_ante);
    }

    commutatio (nodus->genus)
    {
        casus STML_NODUS_DOCUMENTUM:
            si (nodus->liberi)
            {
                num = xar_numerus(nodus->liberi);
                per (i = ZEPHYRUM; i < num; i++)
                {
                    liberum = _xar_liberum_obtinere(nodus->liberi, i);
                    si (liberum)
                    {
                        _scribere_nucleus(liberum, aedificator, pulchrum, fidelitas, indentatio, sedes);
                        si (pulchrum && i < num - I)
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
                si (nodus->attributa)
                {
                    num = xar_numerus(nodus->attributa);
                    per (i = ZEPHYRUM; i < num; i++)
                    {
                        attr = (StmlAttributum*)xar_obtinere(nodus->attributa, i);
                        si (attr && attr->titulus)
                        {
                            chorda_aedificator_appendere_character(aedificator, ' ');
                            chorda_aedificator_appendere_chorda(aedificator, *attr->titulus);
                            si (attr->valor && !_chorda_ptr_aequalis_literis(attr->valor, "true"))
                            {
                                chorda_aedificator_appendere_literis(aedificator, "=\"");
                                chorda_aedificator_appendere_chorda(aedificator, *attr->valor);
                                chorda_aedificator_appendere_character(aedificator, '"');
                            }
                        }
                    }
                }

                /* Fragmentum capturans: <# (> / <#id (> - liberi
                 * inline, sine tag claudente (sicut elementa ANTE) */
                si (nodus->captio_directio == STML_CAPTIO_ANTE)
                {
                    i32 j;
                    chorda_aedificator_appendere_character(aedificator, ' ');
                    per (j = ZEPHYRUM; j < nodus->captio_numerus; j++)
                    {
                        chorda_aedificator_appendere_character(aedificator, '(');
                    }
                    chorda_aedificator_appendere_character(aedificator, '>');
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

                /* ordo fluminis (§6, in M1 tractum): liberi capti
                 * RETRO in fonte ANTE tagum captoris stant -
                 * fidelitas non-pulchra ordinem authoris reddit
                 * (quisque liberum trivia sua secum fert); pulcher
                 * formam captor-primum generat ut hodie */
                si (fidelitas && nodus->liberi)
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
                si (nodus->attributa)
                {
                    num = xar_numerus(nodus->attributa);
                    per (i = ZEPHYRUM; i < num; i++)
                    {
                        attr = (StmlAttributum*)xar_obtinere(nodus->attributa, i);
                        si (attr && attr->titulus)
                        {
                            chorda_aedificator_appendere_character(aedificator, ' ');
                            chorda_aedificator_appendere_chorda(aedificator, *attr->titulus);
                            si (attr->valor && !_chorda_ptr_aequalis_literis(attr->valor, "true"))
                            {
                                chorda_aedificator_appendere_literis(aedificator, "=\"");
                                chorda_aedificator_appendere_chorda(aedificator, *attr->valor);
                                chorda_aedificator_appendere_character(aedificator, '"');
                            }
                        }
                    }
                }
                chorda_aedificator_appendere_character(aedificator, '>');
                /* Serialize children inline - modo pulchro solo
                 * (non-pulcher eos IAM ante tagum emisit) */
                si (!fidelitas && nodus->liberi)
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
            }
            alioquin si (nodus->captio_directio == STML_CAPTIO_FARCIMEN)
            {
                /* Sandwich capture: <= tag =>. Ordo fluminis in
                 * fonte: liberum primum (captum retro) ANTE tagum,
                 * reliqua post - non-pulcher eum ordinem reddit;
                 * pulcher captor-primum generat ut hodie. */
                si (   fidelitas
                    && nodus->liberi
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
                chorda_aedificator_appendere_literis(aedificator, "<= ");
                si (nodus->titulus)
                {
                    chorda_aedificator_appendere_chorda(aedificator, *nodus->titulus);
                }
                /* Attributes */
                si (nodus->attributa)
                {
                    num = xar_numerus(nodus->attributa);
                    per (i = ZEPHYRUM; i < num; i++)
                    {
                        attr = (StmlAttributum*)xar_obtinere(nodus->attributa, i);
                        si (attr && attr->titulus)
                        {
                            chorda_aedificator_appendere_character(aedificator, ' ');
                            chorda_aedificator_appendere_chorda(aedificator, *attr->titulus);
                            si (attr->valor && !_chorda_ptr_aequalis_literis(attr->valor, "true"))
                            {
                                chorda_aedificator_appendere_literis(aedificator, "=\"");
                                chorda_aedificator_appendere_chorda(aedificator, *attr->valor);
                                chorda_aedificator_appendere_character(aedificator, '"');
                            }
                        }
                    }
                }
                chorda_aedificator_appendere_literis(aedificator, " =>");
                /* Serialize children inline: non-pulcher ab indice
                 * I (liberum 0 iam ante tagum), pulcher omnes */
                si (nodus->liberi)
                {
                    num = xar_numerus(nodus->liberi);
                    per (i = fidelitas ? I : ZEPHYRUM; i < num; i++)
                    {
                        liberum = _xar_liberum_obtinere(nodus->liberi, i);
                        si (liberum)
                        {
                            _scribere_nucleus(liberum, aedificator, FALSUM, fidelitas, ZEPHYRUM, sedes);
                        }
                    }
                }
            }
            alioquin si (nodus->captio_directio == STML_CAPTIO_ANTE)
            {
                /* Forward capture: <tag (> or <tag ((> */
                i32 j;
                chorda_aedificator_appendere_character(aedificator, '<');
                si (nodus->titulus)
                {
                    chorda_aedificator_appendere_chorda(aedificator, *nodus->titulus);
                }
                si (nodus->crudus)
                {
                    chorda_aedificator_appendere_character(aedificator, '!');
                }
                /* Attributes */
                si (nodus->attributa)
                {
                    num = xar_numerus(nodus->attributa);
                    per (i = ZEPHYRUM; i < num; i++)
                    {
                        attr = (StmlAttributum*)xar_obtinere(nodus->attributa, i);
                        si (attr && attr->titulus)
                        {
                            chorda_aedificator_appendere_character(aedificator, ' ');
                            chorda_aedificator_appendere_chorda(aedificator, *attr->titulus);
                            si (attr->valor && !_chorda_ptr_aequalis_literis(attr->valor, "true"))
                            {
                                chorda_aedificator_appendere_literis(aedificator, "=\"");
                                chorda_aedificator_appendere_chorda(aedificator, *attr->valor);
                                chorda_aedificator_appendere_character(aedificator, '"');
                            }
                        }
                    }
                }
                chorda_aedificator_appendere_character(aedificator, ' ');
                per (j = ZEPHYRUM; j < nodus->captio_numerus; j++)
                {
                    chorda_aedificator_appendere_character(aedificator, '(');
                }
                chorda_aedificator_appendere_character(aedificator, '>');

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
                                /* Raw content - don't escape */
                                chorda_aedificator_appendere_chorda(aedificator, *liberum->valor);
                            }
                            alioquin
                            {
                                _scribere_nucleus(liberum, aedificator, FALSUM, fidelitas, ZEPHYRUM, sedes);
                            }
                        }
                    }
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

                /* Attributes */
                si (nodus->attributa)
                {
                    num = xar_numerus(nodus->attributa);
                    per (i = ZEPHYRUM; i < num; i++)
                    {
                        attr = (StmlAttributum*)xar_obtinere(nodus->attributa, i);
                        si (attr && attr->titulus)
                        {
                            chorda_aedificator_appendere_character(aedificator, ' ');
                            chorda_aedificator_appendere_chorda(aedificator, *attr->titulus);

                            /* Boolean attributes: don't output ="true" */
                            si (attr->valor && !_chorda_ptr_aequalis_literis(attr->valor, "true"))
                            {
                                chorda_aedificator_appendere_literis(aedificator, "=\"");
                                chorda_aedificator_appendere_chorda(aedificator, *attr->valor);
                                chorda_aedificator_appendere_character(aedificator, '"');
                            }
                        }
                    }
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
                         StmlNodus* first_child;
                    memoriae_index  initium_contenti;
                               b32  clausura_tacita;

                    chorda_aedificator_appendere_character(aedificator, '>');
                    initium_contenti = chorda_aedificator_longitudo(aedificator);

                    num = nodus->liberi
                        ? xar_numerus(nodus->liberi) : ZEPHYRUM;
                    first_child = (num > ZEPHYRUM)
                        ? _xar_liberum_obtinere(nodus->liberi, ZEPHYRUM)
                        : NIHIL;

                    /* For raw content or single text child, don't add newlines */
                    si (   nodus->crudus
                        || (num == I && first_child
                        && first_child->genus == STML_NODUS_TEXTUS))
                    {
                        per (i = ZEPHYRUM; i < num; i++)
                        {
                            liberum = _xar_liberum_obtinere(nodus->liberi, i);
                            si (liberum)
                            {
                                si (nodus->crudus && liberum->genus == STML_NODUS_TEXTUS && liberum->valor)
                                {
                                    /* Raw content - don't escape */
                                    chorda_aedificator_appendere_chorda(aedificator, *liberum->valor);
                                }
                                alioquin
                                {
                                    _scribere_nucleus(liberum, aedificator, FALSUM, fidelitas, ZEPHYRUM, sedes);
                                }
                            }
                        }
                    }
                    alioquin
                    {
                        /* TERMINI, non liberi singuli (2026-08-19).
                         *
                         * Regula: terminus spatium album fert NISI
                         * utravis pars textus SIGNIFICANS est. Textus
                         * solum-albus TRANSPARENS est - nec emittitur
                         * nec terminos afficit, ergo vicini VERI inter
                         * se conveniunt.
                         *
                         * Ansa prior spatium per liberum incondite
                         * emittebat, generis immemor, et duo vitia
                         * pariebat: (i) textus verus lineis novis
                         * circumdatus, quae VALORIS eius pars fiebant
                         * ('n' -> '\nn\n' -> '\n\nn\n\n' - CUMULABAT);
                         * (ii) liberus albus, contento recte
                         * suppresso, lineam suam nihilominus addebat -
                         * unde scriptio pulchra puncto fixo carebat
                         * pro OMNI elemento plus quam unum liberum
                         * ferente.
                         *
                         * Indentatio a CASU liberi ipsius emittitur,
                         * non ab hac ansa; ergo termino collapso
                         * vexillum pulchri liberi extingui DEBET,
                         * aliter spatia indentationis textui
                         * agglutinantur - eadem corruptio, habitu
                         * alio. Exemplar: ramus crudus/textus-unicus
                         * supra idem FALSUM iam imponit. Pretium
                         * nominatum: liberus sic vocatus formam suam
                         * INTERNAM quoque planam reddit. */
                        b32 aliquid_emissum;
                        b32 textus_ante;

                        aliquid_emissum  = FALSUM;
                        textus_ante      = FALSUM;

                        per (i = ZEPHYRUM; i < num; i++)
                        {
                            b32 est_textus;
                            b32 arte;

                            liberum = _xar_liberum_obtinere(nodus->liberi, i);
                            si (!liberum)
                            {
                                perge;
                            }

                            si (   pulchrum
                                && liberum->genus == STML_NODUS_TEXTUS
                                && (   liberum->valor == NIHIL
                                    || _spatium_album_solum(liberum->valor)))
                            {
                                perge;
                            }

                            est_textus = (liberum->genus == STML_NODUS_TEXTUS)
                                ? VERUM : FALSUM;
                            arte = (est_textus || textus_ante) ? VERUM : FALSUM;

                            si (pulchrum && !arte)
                            {
                                chorda_aedificator_appendere_character(aedificator, '\n');
                            }

                            _scribere_nucleus(liberum, aedificator,
                                (pulchrum && !arte) ? VERUM : FALSUM, fidelitas,
                                indentatio + I, sedes);

                            aliquid_emissum  = VERUM;
                            textus_ante      = est_textus;
                        }

                        si (pulchrum && aliquid_emissum && !textus_ante)
                        {
                            chorda_aedificator_appendere_character(aedificator, '\n');
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
            /* DUO MODI, DUAE PROMISSIONES (2026-08-06):
             * non-pulcher FIDEM praestat (circuitus octetim), ergo
             * textum verbatim scribit; pulcher LEGIBILITATEM
             * praestat et dispositionem SUAM generat, ergo nodos
             * spatii albi SOLIUS omittit.
             * Aliter compugnant: indentatio servata et indentatio
             * generata se cumulant, et circuitus
             * scribere->legere->rescribere quoque cursu CRESCIT
             * (mensuratum: lineae vacuae duplicantes).
             * Pulcher fidem numquam promisit - reformator est. */
            si (nodus->valor)
            {
                si (   !pulchrum
                    || !_spatium_album_solum(nodus->valor))
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
     * initium_sedis in casu ELEMENTI solo ponitur, cetera genera
     * sentinellam -I tenent et praetereunt. */
    si (sedes != NIHIL && initium_sedis >= ZEPHYRUM)
    {
        StmlSedesNodi* nota;

        nota = xar_addere(sedes);
        si (nota != NIHIL)
        {
            nota->nodus    = nodus;
            nota->initium  = (i32)initium_sedis;
            nota->finis   =
                (i32)chorda_aedificator_longitudo(aedificator);
        }
    }

    /* spatia_post EXTRA extensionem sedium (post notationem);
     * captores ANTE ea iam intra formam emiserunt (§6) */
    si (   fidelitas
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
