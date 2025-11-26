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
_est_nomen_initium(character c)
{
    redde (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_' || c == ':';
}

interior b32
_est_nomen_character(character c)
{
    redde (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') ||
           c == '_' || c == '-' || c == ':' || c == '.';
}

interior b32
_est_spatium(character c)
{
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

/* Comparare chorda* cum literis C */
interior b32
_chorda_ptr_aequalis_literis(chorda* ch, constans character* cstr)
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
    chorda  input;
    i32     positus;
    i32     linea;
    i32     columna;

    /* Raw content mode */
    b32     in_crudus;
    chorda  crudus_titulus;  /* Tag name we're looking for to close */

    /* Piscina and intern for allocations */
    Piscina*             piscina;
    InternamentumChorda* intern;
} StmlTokenContext;

nomen structura {
    StmlTokenGenus genus;
    chorda         valor;
    i32            positus_initium;
    i32            positus_finis;
    i32            linea;
    i32            columna;

    /* For tags: parsed attributes */
    Xar*           attributa;

    /* For capture operators */
    i32            captio_numerus;

    /* For raw tags with forward capture: captured content */
    chorda         captus_contentus;
    b32            habet_captus;
} StmlToken;

/* ==================================================
 * Tokenizer Functions
 * ================================================== */

interior character
_tok_aspicere(StmlTokenContext* ctx, i32 offset)
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
_tok_progredi(StmlTokenContext* ctx, i32 numerus)
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
_tok_praeterire_spatium(StmlTokenContext* ctx)
{
    dum (ctx->positus < ctx->input.mensura &&
         _est_spatium(_tok_aspicere(ctx, ZEPHYRUM)))
    {
        _tok_progredi(ctx, I);
    }
}

interior chorda
_tok_legere_nomen(StmlTokenContext* ctx)
{
    chorda result;
    i32 initium;

    initium = ctx->positus;

    si (!_est_nomen_initium(_tok_aspicere(ctx, ZEPHYRUM)))
    {
        result.datum = NIHIL;
        result.mensura = ZEPHYRUM;
        redde result;
    }

    dum (ctx->positus < ctx->input.mensura &&
         _est_nomen_character(_tok_aspicere(ctx, ZEPHYRUM)))
    {
        _tok_progredi(ctx, I);
    }

    result.datum = ctx->input.datum + initium;
    result.mensura = ctx->positus - initium;
    redde result;
}

interior chorda
_tok_legere_valor_attributi(StmlTokenContext* ctx)
{
    chorda result;
    character quota;
    i32 initium;

    quota = _tok_aspicere(ctx, ZEPHYRUM);
    si (quota != '"' && quota != '\'')
    {
        result.datum = NIHIL;
        result.mensura = ZEPHYRUM;
        redde result;
    }

    _tok_progredi(ctx, I);  /* Skip opening quote */
    initium = ctx->positus;

    dum (ctx->positus < ctx->input.mensura &&
         _tok_aspicere(ctx, ZEPHYRUM) != quota)
    {
        _tok_progredi(ctx, I);
    }

    result.datum = ctx->input.datum + initium;
    result.mensura = ctx->positus - initium;

    si (_tok_aspicere(ctx, ZEPHYRUM) == quota)
    {
        _tok_progredi(ctx, I);  /* Skip closing quote */
    }

    redde result;
}

/* Parse attributes into Xar of StmlAttributum
 * Handles boolean attributes (no = sign)
 */
interior Xar*
_tok_legere_attributa(StmlTokenContext* ctx)
{
    Xar* attributa;
    chorda titulus_ch;
    chorda valor_ch;
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
        si (_tok_aspicere(ctx, ZEPHYRUM) == '>' ||
            (_tok_aspicere(ctx, ZEPHYRUM) == '/' && _tok_aspicere(ctx, I) == '>') ||
            _tok_aspicere(ctx, ZEPHYRUM) == '(' ||  /* Forward capture */
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
                attr->titulus = titulus_ptr;
                attr->valor = valor_ptr;
            }
            perge;
        }

        /* Skip = */
        _tok_progredi(ctx, I);
        _tok_praeterire_spatium(ctx);

        /* Read value */
        valor_ch = _tok_legere_valor_attributi(ctx);

        titulus_ptr = chorda_internare(ctx->intern, titulus_ch);
        valor_ptr = chorda_internare(ctx->intern, valor_ch);

        attr = xar_addere(attributa);
        si (attr)
        {
            attr->titulus = titulus_ptr;
            attr->valor = valor_ptr;
        }
    }

    redde attributa;
}

/* Parse a comment <!-- ... --> */
interior StmlToken
_tok_legere_commentum(StmlTokenContext* ctx)
{
    StmlToken token;
    i32 initium;
    i32 initium_linea;
    i32 initium_columna;
    i32 contentus_initium;

    initium = ctx->positus;
    initium_linea = ctx->linea;
    initium_columna = ctx->columna;

    /* Skip <!-- */
    _tok_progredi(ctx, IV);
    contentus_initium = ctx->positus;

    /* Find --> */
    dum (ctx->positus < ctx->input.mensura)
    {
        si (_tok_aspicere(ctx, ZEPHYRUM) == '-' &&
            _tok_aspicere(ctx, I) == '-' &&
            _tok_aspicere(ctx, II) == '>')
        {
            token.valor.datum = ctx->input.datum + contentus_initium;
            token.valor.mensura = ctx->positus - contentus_initium;
            _tok_progredi(ctx, III);  /* Skip --> */
            frange;
        }
        _tok_progredi(ctx, I);
    }

    token.genus = STML_TOKEN_COMMENTUM;
    token.positus_initium = initium;
    token.positus_finis = ctx->positus;
    token.linea = initium_linea;
    token.columna = initium_columna;
    token.attributa = NIHIL;
    token.captio_numerus = ZEPHYRUM;
    token.habet_captus = FALSUM;

    redde token;
}

/* Parse processing instruction <?...?> */
interior StmlToken
_tok_legere_processio(StmlTokenContext* ctx)
{
    StmlToken token;
    i32 initium;
    i32 initium_linea;
    i32 initium_columna;
    i32 contentus_initium;

    initium = ctx->positus;
    initium_linea = ctx->linea;
    initium_columna = ctx->columna;

    /* Skip <? */
    _tok_progredi(ctx, II);
    contentus_initium = ctx->positus;

    /* Find ?> */
    dum (ctx->positus < ctx->input.mensura)
    {
        si (_tok_aspicere(ctx, ZEPHYRUM) == '?' &&
            _tok_aspicere(ctx, I) == '>')
        {
            token.valor.datum = ctx->input.datum + contentus_initium;
            token.valor.mensura = ctx->positus - contentus_initium;
            _tok_progredi(ctx, II);  /* Skip ?> */
            frange;
        }
        _tok_progredi(ctx, I);
    }

    token.genus = STML_TOKEN_PROCESSIO;
    token.positus_initium = initium;
    token.positus_finis = ctx->positus;
    token.linea = initium_linea;
    token.columna = initium_columna;
    token.attributa = NIHIL;
    token.captio_numerus = ZEPHYRUM;
    token.habet_captus = FALSUM;

    redde token;
}

/* Parse DOCTYPE <!DOCTYPE ...> */
interior StmlToken
_tok_legere_doctype(StmlTokenContext* ctx)
{
    StmlToken token;
    i32 initium;
    i32 initium_linea;
    i32 initium_columna;
    i32 contentus_initium;
    i32 profunditas;

    initium = ctx->positus;
    initium_linea = ctx->linea;
    initium_columna = ctx->columna;

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

    token.valor.datum = ctx->input.datum + contentus_initium;
    token.valor.mensura = ctx->positus - contentus_initium;

    si (_tok_aspicere(ctx, ZEPHYRUM) == '>')
    {
        _tok_progredi(ctx, I);  /* Skip final > */
    }

    token.genus = STML_TOKEN_DOCTYPE;
    token.positus_initium = initium;
    token.positus_finis = ctx->positus;
    token.linea = initium_linea;
    token.columna = initium_columna;
    token.attributa = NIHIL;
    token.captio_numerus = ZEPHYRUM;
    token.habet_captus = FALSUM;

    redde token;
}

/* Parse a tag <...> */
interior StmlToken
_tok_legere_tag(StmlTokenContext* ctx)
{
    StmlToken token;
    i32 initium;
    i32 initium_linea;
    i32 initium_columna;
    chorda titulus;
    b32 est_crudus;
    i32 captio_numerus;

    initium = ctx->positus;
    initium_linea = ctx->linea;
    initium_columna = ctx->columna;

    token.attributa = NIHIL;
    token.captio_numerus = ZEPHYRUM;
    token.habet_captus = FALSUM;
    token.captus_contentus.datum = NIHIL;
    token.captus_contentus.mensura = ZEPHYRUM;

    /* Skip < */
    _tok_progredi(ctx, I);

    /* Check for sandwich capture <= tag => */
    si (_tok_aspicere(ctx, ZEPHYRUM) == '=')
    {
        _tok_progredi(ctx, I);  /* Skip = */
        _tok_praeterire_spatium(ctx);

        titulus = _tok_legere_nomen(ctx);
        _tok_praeterire_spatium(ctx);

        /* Expect => */
        si (_tok_aspicere(ctx, ZEPHYRUM) == '=' &&
            _tok_aspicere(ctx, I) == '>')
        {
            _tok_progredi(ctx, II);
        }

        token.genus = STML_TOKEN_FARCIMEN;
        token.valor = titulus;
        token.positus_initium = initium;
        token.positus_finis = ctx->positus;
        token.linea = initium_linea;
        token.columna = initium_columna;
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
        _tok_praeterire_spatium(ctx);

        si (_tok_aspicere(ctx, ZEPHYRUM) == '>')
        {
            _tok_progredi(ctx, I);
        }

        token.genus = STML_TOKEN_CAPTIO_RETRO;
        token.valor = titulus;
        token.positus_initium = initium;
        token.positus_finis = ctx->positus;
        token.linea = initium_linea;
        token.columna = initium_columna;
        token.captio_numerus = captio_numerus;
        redde token;
    }

    /* Check for closing tag </tag> */
    si (_tok_aspicere(ctx, ZEPHYRUM) == '/')
    {
        _tok_progredi(ctx, I);
        titulus = _tok_legere_nomen(ctx);

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

        token.genus = STML_TOKEN_CLAUDERE;
        token.valor = titulus;
        token.positus_initium = initium;
        token.positus_finis = ctx->positus;
        token.linea = initium_linea;
        token.columna = initium_columna;
        redde token;
    }

    /* Regular opening tag */
    titulus = _tok_legere_nomen(ctx);

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
    si (captio_numerus == ZEPHYRUM &&
        _tok_aspicere(ctx, ZEPHYRUM) == '/' &&
        _tok_aspicere(ctx, I) == '>')
    {
        _tok_progredi(ctx, II);

        token.genus = STML_TOKEN_AUTO_CLAUDERE;
        token.valor = titulus;
        token.positus_initium = initium;
        token.positus_finis = ctx->positus;
        token.linea = initium_linea;
        token.columna = initium_columna;
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
            /* Raw tag with forward capture - capture lines */
            /* TODO: Implement line capture for raw tags */
            token.genus = STML_TOKEN_CRUDUS;
            token.captio_numerus = captio_numerus;
        }
        alioquin
        {
            token.genus = STML_TOKEN_CAPTIO_ANTE;
            token.captio_numerus = captio_numerus;
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

    token.valor = titulus;
    token.positus_initium = initium;
    token.positus_finis = ctx->positus;
    token.linea = initium_linea;
    token.columna = initium_columna;

    redde token;
}

/* Parse raw content until </tagname> */
interior StmlToken
_tok_legere_contentus_crudus(StmlTokenContext* ctx, chorda titulus)
{
    StmlToken token;
    i32 initium;
    i32 initium_linea;
    i32 initium_columna;
    i32 i;
    b32 inventum;

    initium = ctx->positus;
    initium_linea = ctx->linea;
    initium_columna = ctx->columna;

    /* Search for </tagname> */
    inventum = FALSUM;
    dum (ctx->positus < ctx->input.mensura && !inventum)
    {
        si (_tok_aspicere(ctx, ZEPHYRUM) == '<' &&
            _tok_aspicere(ctx, I) == '/')
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

            si (aequalis &&
                (_tok_aspicere(ctx, II + titulus.mensura) == '>' ||
                 _tok_aspicere(ctx, II + titulus.mensura) == '!' ||
                 _est_spatium(_tok_aspicere(ctx, II + titulus.mensura))))
            {
                inventum = VERUM;
                frange;
            }
        }
        _tok_progredi(ctx, I);
    }

    token.genus = STML_TOKEN_TEXTUS;
    token.valor.datum = ctx->input.datum + initium;
    token.valor.mensura = ctx->positus - initium;
    token.positus_initium = initium;
    token.positus_finis = ctx->positus;
    token.linea = initium_linea;
    token.columna = initium_columna;
    token.attributa = NIHIL;
    token.captio_numerus = ZEPHYRUM;
    token.habet_captus = FALSUM;

    redde token;
}

/* Parse text content */
interior StmlToken
_tok_legere_textus(StmlTokenContext* ctx)
{
    StmlToken token;
    i32 initium;
    i32 initium_linea;
    i32 initium_columna;

    initium = ctx->positus;
    initium_linea = ctx->linea;
    initium_columna = ctx->columna;

    dum (ctx->positus < ctx->input.mensura &&
         _tok_aspicere(ctx, ZEPHYRUM) != '<')
    {
        _tok_progredi(ctx, I);
    }

    token.genus = STML_TOKEN_TEXTUS;
    token.valor.datum = ctx->input.datum + initium;
    token.valor.mensura = ctx->positus - initium;
    token.positus_initium = initium;
    token.positus_finis = ctx->positus;
    token.linea = initium_linea;
    token.columna = initium_columna;
    token.attributa = NIHIL;
    token.captio_numerus = ZEPHYRUM;
    token.habet_captus = FALSUM;

    redde token;
}

/* Get next token */
interior StmlToken
_tok_proximus(StmlTokenContext* ctx)
{
    StmlToken token;

    /* If in raw content mode, get raw content until close tag */
    si (ctx->in_crudus)
    {
        /* Check if we're at the closing tag */
        si (_tok_aspicere(ctx, ZEPHYRUM) == '<' &&
            _tok_aspicere(ctx, I) == '/')
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
        token.genus = STML_TOKEN_FINIS;
        token.valor.datum = NIHIL;
        token.valor.mensura = ZEPHYRUM;
        token.positus_initium = ctx->positus;
        token.positus_finis = ctx->positus;
        token.linea = ctx->linea;
        token.columna = ctx->columna;
        token.attributa = NIHIL;
        token.captio_numerus = ZEPHYRUM;
        token.habet_captus = FALSUM;
        redde token;
    }

    si (_tok_aspicere(ctx, ZEPHYRUM) == '<')
    {
        /* Check for comment */
        si (_tok_aspicere(ctx, I) == '!' &&
            _tok_aspicere(ctx, II) == '-' &&
            _tok_aspicere(ctx, III) == '-')
        {
            redde _tok_legere_commentum(ctx);
        }

        /* Check for processing instruction */
        si (_tok_aspicere(ctx, I) == '?')
        {
            redde _tok_legere_processio(ctx);
        }

        /* Check for DOCTYPE */
        si (_tok_aspicere(ctx, I) == '!' &&
            _tok_aspicere(ctx, II) == 'D')
        {
            redde _tok_legere_doctype(ctx);
        }

        /* Regular tag */
        token = _tok_legere_tag(ctx);

        /* Check if entering raw content mode */
        si (token.genus == STML_TOKEN_CRUDUS && token.captio_numerus == ZEPHYRUM)
        {
            ctx->in_crudus = VERUM;
            ctx->crudus_titulus = token.valor;
        }

        redde token;
    }

    /* Text content */
    redde _tok_legere_textus(ctx);
}

/* ==================================================
 * Parser
 * ================================================== */

nomen structura {
    StmlTokenContext  tok_ctx;
    StmlToken         current;
    Piscina*          piscina;
    InternamentumChorda* intern;

    /* Error info */
    StmlStatus        status;
    i32               linea_erroris;
    i32               columna_erroris;
    chorda            error;
} StmlParserContext;

interior vacuum
_parser_progredi(StmlParserContext* ctx)
{
    ctx->current = _tok_proximus(&ctx->tok_ctx);
}

interior StmlNodus*
_parser_creare_nodus(StmlParserContext* ctx, StmlNodusGenus genus)
{
    StmlNodus* nodus;

    nodus = (StmlNodus*)piscina_allocare(ctx->piscina, magnitudo(StmlNodus));
    si (!nodus)
    {
        redde NIHIL;
    }

    nodus->genus = genus;
    nodus->titulus = NIHIL;
    nodus->valor = NIHIL;
    nodus->attributa = NIHIL;
    nodus->liberi = NIHIL;
    nodus->parens = NIHIL;
    nodus->crudus = FALSUM;
    nodus->captio_directio = STML_CAPTIO_NIHIL;
    nodus->captio_numerus = ZEPHYRUM;

    redde nodus;
}

/* Forward declaration */
interior StmlNodus* _parser_legere_nodus(StmlParserContext* ctx);

/* Parse an element */
interior StmlNodus*
_parser_legere_elementum(StmlParserContext* ctx)
{
    StmlNodus* nodus;
    StmlNodus* liberum;
    chorda titulus;
    chorda* titulus_ptr;

    nodus = _parser_creare_nodus(ctx, STML_NODUS_ELEMENTUM);
    si (!nodus) redde NIHIL;

    titulus = ctx->current.valor;
    titulus_ptr = chorda_internare(ctx->intern, titulus);
    nodus->titulus = titulus_ptr;
    nodus->attributa = ctx->current.attributa;

    nodus->liberi = xar_creare(ctx->piscina, magnitudo(StmlNodus*));

    _parser_progredi(ctx);  /* Consume opening tag */

    /* Parse children until close tag */
    dum (ctx->current.genus != STML_TOKEN_CLAUDERE &&
         ctx->current.genus != STML_TOKEN_FINIS)
    {
        liberum = _parser_legere_nodus(ctx);
        si (liberum)
        {
            StmlNodus** slot;
            liberum->parens = nodus;
            slot = xar_addere(nodus->liberi);
            si (slot) *slot = liberum;
        }
    }

    /* Verify close tag matches */
    si (ctx->current.genus == STML_TOKEN_CLAUDERE)
    {
        si (!chorda_aequalis(ctx->current.valor, *titulus_ptr))
        {
            ctx->status = STML_ERROR_TAG_IMPROPRIE;
            ctx->linea_erroris = ctx->current.linea;
            ctx->columna_erroris = ctx->current.columna;
        }
        _parser_progredi(ctx);  /* Consume close tag */
    }
    alioquin si (ctx->current.genus == STML_TOKEN_FINIS)
    {
        /* Unclosed tag at EOF */
        ctx->status = STML_ERROR_TAG_NON_CLAUSUM;
        ctx->linea_erroris = ctx->current.linea;
        ctx->columna_erroris = ctx->current.columna;
    }

    redde nodus;
}

/* Parse a raw content element */
interior StmlNodus*
_parser_legere_elementum_crudus(StmlParserContext* ctx)
{
    StmlNodus* nodus;
    StmlNodus* textus_nodus;
    chorda titulus;
    chorda* titulus_ptr;

    nodus = _parser_creare_nodus(ctx, STML_NODUS_ELEMENTUM);
    si (!nodus) redde NIHIL;

    titulus = ctx->current.valor;
    titulus_ptr = chorda_internare(ctx->intern, titulus);
    nodus->titulus = titulus_ptr;
    nodus->attributa = ctx->current.attributa;
    nodus->crudus = VERUM;
    nodus->captio_numerus = ctx->current.captio_numerus;

    si (ctx->current.captio_numerus > ZEPHYRUM)
    {
        nodus->captio_directio = STML_CAPTIO_ANTE;
    }

    nodus->liberi = xar_creare(ctx->piscina, magnitudo(StmlNodus*));

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
                textus_nodus->parens = nodus;
                slot = xar_addere(nodus->liberi);
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

    redde nodus;
}

/* Parse a self-closing element */
interior StmlNodus*
_parser_legere_auto_claudere(StmlParserContext* ctx)
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

    redde nodus;
}

/* Parse forward capture element */
interior StmlNodus*
_parser_legere_captio_ante(StmlParserContext* ctx)
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

    redde nodus;
}

/* Parse backward capture element */
interior StmlNodus*
_parser_legere_captio_retro(StmlParserContext* ctx)
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

    redde nodus;
}

/* Parse sandwich capture element */
interior StmlNodus*
_parser_legere_farcimen(StmlParserContext* ctx)
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

    redde nodus;
}

/* ==================================================
 * Smart Whitespace Normalization
 * ================================================== */

/* Check if a line is empty (only whitespace) */
interior b32
_est_linea_vacua(chorda linea)
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
_numerare_indentationem(chorda linea)
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
_continet_novam_lineam(chorda s)
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
_starts_with_at(chorda s, i32 pos, constans character* prefix)
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
_unescape_entities(chorda textus, Piscina* piscina)
{
    ChordaAedificator* aed;
    i32 i;
    b32 has_entity;

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

/* Smart whitespace normalization:
 * - Trim leading/trailing empty lines
 * - Normalize indentation to least-indented non-empty line
 * - Preserve relative indentation
 */
interior chorda
_normalizare_spatium_album(chorda textus, Piscina* piscina)
{
    chorda_fissio_fructus lineae;
    ChordaAedificator* aed;
    i32 initium;
    i32 finis;
    i32 min_indent;
    i32 i;
    i32 indent;
    chorda linea;
    chorda result;

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
        result.datum = NIHIL;
        result.mensura = ZEPHYRUM;
        redde result;
    }

    /* Find first and last non-empty lines */
    initium = lineae.numerus;
    finis = ZEPHYRUM;
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
        result.datum = NIHIL;
        result.mensura = ZEPHYRUM;
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
        result.datum = NIHIL;
        result.mensura = ZEPHYRUM;
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

/* Parse text node */
interior StmlNodus*
_parser_legere_textus(StmlParserContext* ctx)
{
    StmlNodus* nodus;
    chorda contentus;
    chorda normalizatus;
    chorda unescaped;
    chorda* contentus_ptr;

    contentus = ctx->current.valor;

    /* Normalize whitespace (trim indentation, leading/trailing blank lines) */
    normalizatus = _normalizare_spatium_album(contentus, ctx->piscina);

    _parser_progredi(ctx);

    /* If normalized to empty, skip this text node */
    si (normalizatus.mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }

    /* Unescape XML entities (&lt; -> <, etc.) */
    unescaped = _unescape_entities(normalizatus, ctx->piscina);

    nodus = _parser_creare_nodus(ctx, STML_NODUS_TEXTUS);
    si (!nodus) redde NIHIL;

    contentus_ptr = chorda_internare(ctx->intern, unescaped);
    nodus->valor = contentus_ptr;

    redde nodus;
}

/* Parse comment node */
interior StmlNodus*
_parser_legere_commentum(StmlParserContext* ctx)
{
    StmlNodus* nodus;
    chorda* contentus_ptr;

    nodus = _parser_creare_nodus(ctx, STML_NODUS_COMMENTUM);
    si (!nodus) redde NIHIL;

    contentus_ptr = chorda_internare(ctx->intern, ctx->current.valor);
    nodus->valor = contentus_ptr;

    _parser_progredi(ctx);

    redde nodus;
}

/* Parse processing instruction node */
interior StmlNodus*
_parser_legere_processio(StmlParserContext* ctx)
{
    StmlNodus* nodus;
    chorda* contentus_ptr;

    nodus = _parser_creare_nodus(ctx, STML_NODUS_PROCESSIO);
    si (!nodus) redde NIHIL;

    contentus_ptr = chorda_internare(ctx->intern, ctx->current.valor);
    nodus->valor = contentus_ptr;

    _parser_progredi(ctx);

    redde nodus;
}

/* Parse DOCTYPE node */
interior StmlNodus*
_parser_legere_doctype(StmlParserContext* ctx)
{
    StmlNodus* nodus;
    chorda* contentus_ptr;

    nodus = _parser_creare_nodus(ctx, STML_NODUS_DOCTYPE);
    si (!nodus) redde NIHIL;

    contentus_ptr = chorda_internare(ctx->intern, ctx->current.valor);
    nodus->valor = contentus_ptr;

    _parser_progredi(ctx);

    redde nodus;
}

/* Parse any node */
interior StmlNodus*
_parser_legere_nodus(StmlParserContext* ctx)
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

        casus STML_TOKEN_CLAUDERE:
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
_habet_captio(StmlNodus* nodus)
{
    i32 i;
    i32 num;
    StmlNodus* liberum;

    si (nodus->genus == STML_NODUS_ELEMENTUM &&
        nodus->captio_directio != STML_CAPTIO_NIHIL)
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
_est_commentum(StmlNodus* nodus)
{
    redde nodus && nodus->genus == STML_NODUS_COMMENTUM;
}

/* Helper to get child at index */
interior StmlNodus*
_xar_liberum_obtinere(Xar* xar, i32 index)
{
    StmlNodus** slot = xar_obtinere(xar, index);
    si (slot) redde *slot;
    redde NIHIL;
}

/* Process capture operators - restructure tree */
interior vacuum
_processare_captiones(StmlNodus* nodus, Piscina* piscina)
{
    Xar* novi_liberi;
    StmlNodus* liberum;
    StmlNodus* captus;
    i32 num;
    i32 i;
    i32 j;
    i32 captio_count;
    i32 captured_count;
    i32 k;

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
            /* Forward capture */
            si (liberum->captio_directio == STML_CAPTIO_ANTE)
            {
                StmlNodus** slot_c;
                captio_count = liberum->captio_numerus;
                captured_count = ZEPHYRUM;

                /* Capture next N non-comment siblings */
                j = i + I;
                dum (j < num && captured_count < captio_count)
                {
                    captus = _xar_liberum_obtinere(nodus->liberi, j);
                    si (captus && !_est_commentum(captus))
                    {
                        captus->parens = liberum;
                        slot_c = xar_addere(liberum->liberi);
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
                captio_count = liberum->captio_numerus;
                captured_count = ZEPHYRUM;

                /* Capture previous N non-comment siblings from novi_liberi */
                k = xar_numerus(novi_liberi) - I;
                dum (k >= ZEPHYRUM && captured_count < captio_count)
                {
                    captus = *((StmlNodus**)xar_obtinere(novi_liberi, k));
                    si (captus && !_est_commentum(captus))
                    {
                        /* Insert at beginning of liberum's children */
                        Xar* temp;
                        i32 m;
                        i32 temp_num;
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

                        liberum->liberi = temp;
                        captus->parens = liberum;

                        /* Remove from novi_liberi */
                        /* For simplicity, mark as null and skip later */
                        *((StmlNodus**)xar_obtinere(novi_liberi, k)) = NIHIL;

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
                k = xar_numerus(novi_liberi) - I;
                dum (k >= ZEPHYRUM)
                {
                    captus = *((StmlNodus**)xar_obtinere(novi_liberi, k));
                    si (captus && !_est_commentum(captus))
                    {
                        captus->parens = liberum;
                        slot_c = xar_addere(liberum->liberi);
                        si (slot_c) *slot_c = captus;
                        *((StmlNodus**)xar_obtinere(novi_liberi, k)) = NIHIL;
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
                        captus->parens = liberum;
                        slot_c = xar_addere(liberum->liberi);
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
        Xar* clean;
        i32 clean_num;
        StmlNodus* entry;
        StmlNodus** slot_e;

        clean = xar_creare(piscina, magnitudo(StmlNodus*));
        clean_num = xar_numerus(novi_liberi);
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
stml_legere(
    chorda               input,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    StmlResultus result;
    StmlParserContext ctx;
    StmlNodus* documentum;
    StmlNodus* liberum;
    i32 i;
    i32 num;

    /* Initialize result */
    result.successus = FALSUM;
    result.radix = NIHIL;
    result.elementum_radix = NIHIL;
    result.status = STML_SUCCESSUS;
    result.linea_erroris = ZEPHYRUM;
    result.columna_erroris = ZEPHYRUM;
    result.error.datum = NIHIL;
    result.error.mensura = ZEPHYRUM;

    /* Check for empty input */
    si (input.mensura == ZEPHYRUM || !input.datum)
    {
        result.status = STML_ERROR_VACUUM_INPUT;
        redde result;
    }

    /* Initialize tokenizer context */
    ctx.tok_ctx.input = input;
    ctx.tok_ctx.positus = ZEPHYRUM;
    ctx.tok_ctx.linea = I;
    ctx.tok_ctx.columna = I;
    ctx.tok_ctx.in_crudus = FALSUM;
    ctx.tok_ctx.crudus_titulus.datum = NIHIL;
    ctx.tok_ctx.crudus_titulus.mensura = ZEPHYRUM;
    ctx.tok_ctx.piscina = piscina;
    ctx.tok_ctx.intern = intern;

    /* Initialize parser context */
    ctx.piscina = piscina;
    ctx.intern = intern;
    ctx.status = STML_SUCCESSUS;
    ctx.linea_erroris = ZEPHYRUM;
    ctx.columna_erroris = ZEPHYRUM;
    ctx.error.datum = NIHIL;
    ctx.error.mensura = ZEPHYRUM;

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

    /* Parse all top-level nodes */
    dum (ctx.current.genus != STML_TOKEN_FINIS)
    {
        liberum = _parser_legere_nodus(&ctx);
        si (liberum)
        {
            StmlNodus** slot;
            liberum->parens = documentum;
            slot = xar_addere(documentum->liberi);
            si (slot) *slot = liberum;
        }
    }

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
    result.successus = (ctx.status == STML_SUCCESSUS);
    result.radix = documentum;
    result.status = ctx.status;
    result.linea_erroris = ctx.linea_erroris;
    result.columna_erroris = ctx.columna_erroris;
    result.error = ctx.error;

    redde result;
}

StmlResultus
stml_legere_ex_literis(
    constans character*  cstr,
    Piscina*             piscina,
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
        result.successus = FALSUM;
        result.radix = NIHIL;
        result.elementum_radix = NIHIL;
        result.status = STML_ERROR_VACUUM_INPUT;
        result.linea_erroris = ZEPHYRUM;
        result.columna_erroris = ZEPHYRUM;
        result.error.datum = NIHIL;
        result.error.mensura = ZEPHYRUM;
        redde result;
    }

    len = ZEPHYRUM;
    dum (cstr[len] != '\0')
    {
        len++;
    }

    conv.cc = cstr;
    input.datum = conv.uc;
    input.mensura = len;

    redde stml_legere(input, piscina, intern);
}

/* ==================================================
 * Public API - Query Functions
 * ================================================== */

StmlNodus*
stml_invenire_liberum(
    StmlNodus*           nodus,
    constans character*  titulus)
{
    i32 i;
    i32 num;
    StmlNodus* liberum;

    si (!nodus || !nodus->liberi || !titulus)
    {
        redde NIHIL;
    }

    num = xar_numerus(nodus->liberi);

    per (i = ZEPHYRUM; i < num; i++)
    {
        liberum = _xar_liberum_obtinere(nodus->liberi, i);
        si (liberum &&
            liberum->genus == STML_NODUS_ELEMENTUM &&
            liberum->titulus &&
            _chorda_ptr_aequalis_literis(liberum->titulus, titulus))
        {
            redde liberum;
        }
    }

    redde NIHIL;
}

Xar*
stml_invenire_omnes_liberos(
    StmlNodus*           nodus,
    constans character*  titulus,
    Piscina*             piscina)
{
    Xar* result;
    i32 i;
    i32 num;
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
        si (liberum &&
            liberum->genus == STML_NODUS_ELEMENTUM &&
            liberum->titulus &&
            _chorda_ptr_aequalis_literis(liberum->titulus, titulus))
        {
            StmlNodus** slot;
            slot = xar_addere(result);
            si (slot) *slot = liberum;
        }
    }

    redde result;
}

chorda*
stml_attributum_capere(
    StmlNodus*           nodus,
    constans character*  titulus)
{
    i32 i;
    i32 num;
    StmlAttributum* attr;

    si (!nodus || !nodus->attributa || !titulus)
    {
        redde NIHIL;
    }

    num = xar_numerus(nodus->attributa);

    per (i = ZEPHYRUM; i < num; i++)
    {
        attr = (StmlAttributum*)xar_obtinere(nodus->attributa, i);
        si (attr && attr->titulus &&
            _chorda_ptr_aequalis_literis(attr->titulus, titulus))
        {
            redde attr->valor;
        }
    }

    redde NIHIL;
}

b32
stml_attributum_habet(
    StmlNodus*           nodus,
    constans character*  titulus)
{
    redde stml_attributum_capere(nodus, titulus) != NIHIL;
}

chorda
stml_textus_internus(
    StmlNodus* nodus,
    Piscina*   piscina)
{
    ChordaAedificator* aed;
    chorda result;
    i32 i;
    i32 num;
    StmlNodus* liberum;
    chorda sub;

    result.datum = NIHIL;
    result.mensura = ZEPHYRUM;

    si (!nodus)
    {
        redde result;
    }

    aed = chorda_aedificator_creare(piscina, CXXVIII);
    si (!aed)
    {
        redde result;
    }

    si (nodus->genus == STML_NODUS_TEXTUS && nodus->valor)
    {
        chorda_aedificator_appendere_chorda(aed, *nodus->valor);
    }

    si (nodus->liberi)
    {
        num = xar_numerus(nodus->liberi);

        per (i = ZEPHYRUM; i < num; i++)
        {
            liberum = _xar_liberum_obtinere(nodus->liberi, i);
            si (liberum)
            {
                sub = stml_textus_internus(liberum, piscina);
                si (sub.mensura > ZEPHYRUM)
                {
                    chorda_aedificator_appendere_chorda(aed, sub);
                }
            }
        }
    }

    redde chorda_aedificator_finire(aed);
}

i32
stml_numerus_liberorum(StmlNodus* nodus)
{
    si (!nodus || !nodus->liberi)
    {
        redde ZEPHYRUM;
    }
    redde xar_numerus(nodus->liberi);
}

StmlNodus*
stml_liberum_ad_indicem(StmlNodus* nodus, i32 index)
{
    si (!nodus || !nodus->liberi || index < ZEPHYRUM)
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
stml_index_inter_fratres(StmlNodus* nodus)
{
    i32 i;
    i32 num;
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
stml_frater_proximus(StmlNodus* nodus)
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
stml_frater_prior(StmlNodus* nodus)
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
stml_primus_liberum(StmlNodus* nodus)
{
    si (!nodus || !nodus->liberi || xar_numerus(nodus->liberi) == ZEPHYRUM)
    {
        redde NIHIL;
    }

    redde *((StmlNodus**)xar_obtinere(nodus->liberi, ZEPHYRUM));
}

StmlNodus*
stml_ultimus_liberum(StmlNodus* nodus)
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
stml_fratres(StmlNodus* nodus, Piscina* piscina)
{
    Xar* result;
    i32 i;
    i32 num;
    StmlNodus* liberum;
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
stml_maiores(StmlNodus* nodus, Piscina* piscina)
{
    Xar* result;
    StmlNodus* currens;
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
stml_proximus_maior(
    StmlNodus*           nodus,
    constans character*  selector,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    StmlNodus* currens;
    SelectioResultus res;

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
_liberi_removere_ad(Xar* liberi, i32 index, Piscina* piscina)
{
    Xar* novum;
    i32 i;
    i32 num;
    StmlNodus* liberum;
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
            liberum = *((StmlNodus**)xar_obtinere(liberi, i));
            slot = xar_addere(novum);
            si (slot) *slot = liberum;
        }
    }

    redde novum;
}

/* Insert element at index in liberi array (creates new array) */
interior Xar*
_liberi_inserere_ad(Xar* liberi, i32 index, StmlNodus* novum_elem, Piscina* piscina)
{
    Xar* nova_xar;
    i32 i;
    i32 num;
    StmlNodus* liberum;
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
        liberum = *((StmlNodus**)xar_obtinere(liberi, i));
        slot = xar_addere(nova_xar);
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
stml_praeponere(StmlNodus* parens, StmlNodus* liberum, Piscina* piscina)
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

    parens->liberi = novi_liberi;
    liberum->parens = parens;

    redde VERUM;
}

b32
stml_removere(StmlNodus* nodus, Piscina* piscina)
{
    s32 index;
    Xar* novi_liberi;
    StmlNodus* parens;

    si (!nodus || !nodus->parens || !piscina)
    {
        redde FALSUM;
    }

    parens = nodus->parens;
    index = stml_index_inter_fratres(nodus);
    si (index < ZEPHYRUM)
    {
        redde FALSUM;
    }

    novi_liberi = _liberi_removere_ad(parens->liberi, (i32)index, piscina);
    si (!novi_liberi)
    {
        redde FALSUM;
    }

    parens->liberi = novi_liberi;
    nodus->parens = NIHIL;

    redde VERUM;
}

vacuum
stml_vacare_liberos(StmlNodus* nodus)
{
    i32 i;
    i32 num;
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
stml_inserere_ante(StmlNodus* nodus, StmlNodus* novum, Piscina* piscina)
{
    s32 index;
    Xar* novi_liberi;
    StmlNodus* parens;

    si (!nodus || !nodus->parens || !novum || !piscina)
    {
        redde FALSUM;
    }

    parens = nodus->parens;
    index = stml_index_inter_fratres(nodus);
    si (index < ZEPHYRUM)
    {
        redde FALSUM;
    }

    novi_liberi = _liberi_inserere_ad(parens->liberi, (i32)index, novum, piscina);
    si (!novi_liberi)
    {
        redde FALSUM;
    }

    parens->liberi = novi_liberi;
    novum->parens = parens;

    redde VERUM;
}

b32
stml_inserere_post(StmlNodus* nodus, StmlNodus* novum, Piscina* piscina)
{
    s32 index;
    Xar* novi_liberi;
    StmlNodus* parens;

    si (!nodus || !nodus->parens || !novum || !piscina)
    {
        redde FALSUM;
    }

    parens = nodus->parens;
    index = stml_index_inter_fratres(nodus);
    si (index < ZEPHYRUM)
    {
        redde FALSUM;
    }

    novi_liberi = _liberi_inserere_ad(parens->liberi, (i32)(index + I), novum, piscina);
    si (!novi_liberi)
    {
        redde FALSUM;
    }

    parens->liberi = novi_liberi;
    novum->parens = parens;

    redde VERUM;
}

b32
stml_substituere(StmlNodus* vetus, StmlNodus* novum, Piscina* piscina)
{
    s32 index;
    StmlNodus* parens;
    StmlNodus** slot;

    si (!vetus || !vetus->parens || !novum || !piscina)
    {
        redde FALSUM;
    }

    parens = vetus->parens;
    index = stml_index_inter_fratres(vetus);
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

    *slot = novum;
    novum->parens = parens;
    vetus->parens = NIHIL;

    (vacuum)piscina;  /* Suppressio moniti - piscina non utitur hic */

    redde VERUM;
}

/* ==================================================
 * Public API - Cloning
 * ================================================== */

interior StmlNodus*
_duplicare_recursivum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern,
    b32                  profundum)
{
    StmlNodus* novum;
    i32 i;
    i32 num;

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
    novum->genus = nodus->genus;
    novum->titulus = nodus->titulus;  /* Iam internatum */
    novum->valor = nodus->valor;      /* Iam internatum */
    novum->crudus = nodus->crudus;
    novum->captio_directio = nodus->captio_directio;
    novum->captio_numerus = nodus->captio_numerus;
    novum->parens = NIHIL;  /* Novum non habet parentem */

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
                    attr_new->titulus = attr_orig->titulus;  /* Internatum */
                    attr_new->valor = attr_orig->valor;      /* Internatum */
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
                StmlNodus* liberum_orig;
                StmlNodus* liberum_novum;
                StmlNodus** slot;

                liberum_orig = *((StmlNodus**)xar_obtinere(nodus->liberi, i));
                liberum_novum = _duplicare_recursivum(liberum_orig, piscina, intern, VERUM);
                si (liberum_novum)
                {
                    liberum_novum->parens = novum;
                    slot = xar_addere(novum->liberi);
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
stml_duplicare(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    redde _duplicare_recursivum(nodus, piscina, intern, VERUM);
}

StmlNodus*
stml_duplicare_superficialiter(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    redde _duplicare_recursivum(nodus, piscina, intern, FALSUM);
}

/* ==================================================
 * Public API - Node Creation
 * ================================================== */

StmlNodus*
stml_elementum_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulus)
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

    redde nodus;
}

StmlNodus*
stml_elementum_crudum_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulus)
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
stml_textum_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  textus)
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

    redde nodus;
}

StmlNodus*
stml_textum_creare_ex_chorda(
    Piscina*             piscina,
    InternamentumChorda* intern,
    chorda               textus)
{
    StmlNodus* nodus;

    nodus = (StmlNodus*)piscina_allocare(piscina, magnitudo(StmlNodus));
    si (!nodus)
    {
        redde NIHIL;
    }

    nodus->genus = STML_NODUS_TEXTUS;
    nodus->titulus = NIHIL;
    nodus->valor = chorda_internare(intern, textus);
    nodus->attributa = NIHIL;
    nodus->liberi = NIHIL;
    nodus->parens = NIHIL;
    nodus->crudus = FALSUM;
    nodus->captio_directio = STML_CAPTIO_NIHIL;
    nodus->captio_numerus = ZEPHYRUM;

    redde nodus;
}

StmlNodus*
stml_commentum_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  textus)
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

    redde nodus;
}

b32
stml_attributum_addere(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulus,
    constans character*  valor)
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

    attr->titulus = chorda_internare_ex_literis(intern, titulus);
    attr->valor = chorda_internare_ex_literis(intern, valor);

    redde VERUM;
}

b32
stml_attributum_boolean_addere(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulus)
{
    redde stml_attributum_addere(nodus, piscina, intern, titulus, "true");
}

b32
stml_attributum_addere_chorda(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulus,
    chorda               valor)
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

    attr->titulus = chorda_internare_ex_literis(intern, titulus);
    attr->valor = chorda_internare(intern, valor);

    redde VERUM;
}

b32
stml_liberum_addere(
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

    *slot = liberum;
    liberum->parens = parens;

    redde VERUM;
}

b32
stml_textum_addere(
    StmlNodus*           parens,
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  textus)
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
_scribere_indentatio(ChordaAedificator* aed, i32 nivel)
{
    i32 i;
    per (i = ZEPHYRUM; i < nivel; i++)
    {
        chorda_aedificator_appendere_literis(aed, "  ");
    }
}

interior vacuum
_scribere_evasus(ChordaAedificator* aed, chorda* s)
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

b32
stml_scribere_ad_aedificator(
    StmlNodus*          nodus,
    ChordaAedificator*  aedificator,
    b32                 pulchrum,
    i32                 indentatio)
{
    i32 i;
    i32 num;
    StmlNodus* liberum;
    StmlAttributum* attr;
    b32 habet_liberos;

    si (!nodus || !aedificator)
    {
        redde FALSUM;
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
                        stml_scribere_ad_aedificator(liberum, aedificator, pulchrum, indentatio);
                        si (pulchrum && i < num - I)
                        {
                            chorda_aedificator_appendere_character(aedificator, '\n');
                        }
                    }
                }
            }
            frange;

        casus STML_NODUS_ELEMENTUM:
            si (pulchrum)
            {
                _scribere_indentatio(aedificator, indentatio);
            }

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

            /* Check if has children */
            habet_liberos = nodus->liberi && xar_numerus(nodus->liberi) > ZEPHYRUM;

            si (!habet_liberos)
            {
                chorda_aedificator_appendere_literis(aedificator, "/>");
            }
            alioquin
            {
                StmlNodus* first_child;
                chorda_aedificator_appendere_character(aedificator, '>');

                num = xar_numerus(nodus->liberi);
                first_child = _xar_liberum_obtinere(nodus->liberi, ZEPHYRUM);

                /* For raw content or single text child, don't add newlines */
                si (nodus->crudus ||
                    (num == I && first_child &&
                     first_child->genus == STML_NODUS_TEXTUS))
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
                                stml_scribere_ad_aedificator(liberum, aedificator, FALSUM, ZEPHYRUM);
                            }
                        }
                    }
                }
                alioquin
                {
                    si (pulchrum)
                    {
                        chorda_aedificator_appendere_character(aedificator, '\n');
                    }

                    per (i = ZEPHYRUM; i < num; i++)
                    {
                        liberum = _xar_liberum_obtinere(nodus->liberi, i);
                        si (liberum)
                        {
                            stml_scribere_ad_aedificator(liberum, aedificator, pulchrum, indentatio + I);
                            si (pulchrum)
                            {
                                chorda_aedificator_appendere_character(aedificator, '\n');
                            }
                        }
                    }

                    si (pulchrum)
                    {
                        _scribere_indentatio(aedificator, indentatio);
                    }
                }

                chorda_aedificator_appendere_literis(aedificator, "</");
                si (nodus->titulus)
                {
                    chorda_aedificator_appendere_chorda(aedificator, *nodus->titulus);
                }
                chorda_aedificator_appendere_character(aedificator, '>');
            }
            frange;

        casus STML_NODUS_TEXTUS:
            si (nodus->valor)
            {
                _scribere_evasus(aedificator, nodus->valor);
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

        ordinarius:
            frange;
    }

    redde VERUM;
}

chorda
stml_scribere(
    StmlNodus* nodus,
    Piscina*   piscina,
    b32        pulchrum)
{
    ChordaAedificator* aed;
    chorda result;

    result.datum = NIHIL;
    result.mensura = ZEPHYRUM;

    si (!nodus || !piscina)
    {
        redde result;
    }

    aed = chorda_aedificator_creare(piscina, DXII);
    si (!aed)
    {
        redde result;
    }

    stml_scribere_ad_aedificator(nodus, aed, pulchrum, ZEPHYRUM);

    redde chorda_aedificator_finire(aed);
}

/* ==================================================
 * Tituli (Labels) Implementation
 * ================================================== */

/* Helper: get labels attribute value (checks "labels" then "class") */
interior chorda*
_stml_titulos_valor(StmlNodus* nodus)
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
_stml_attributum_index(
    StmlNodus*          nodus,
    constans character* titulus)
{
    s32 i;
    s32 num;
    StmlAttributum* attr;

    si (!nodus || !nodus->attributa)
    {
        redde -I;
    }

    num = (s32)xar_numerus(nodus->attributa);
    per (i = ZEPHYRUM; i < num; i++)
    {
        attr = (StmlAttributum*)xar_obtinere(nodus->attributa, (i32)i);
        si (attr && attr->titulus &&
            chorda_aequalis_literis(*attr->titulus, titulus))
        {
            redde i;
        }
    }

    redde -I;
}

b32
stml_titulum_habet(
    StmlNodus*          nodus,
    constans character* titulum)
{
    chorda* labels_valor;
    i32 target_len;
    i32 i;
    i32 j;
    i32 start;
    i32 end;
    b32 match;

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
        dum (i < labels_valor->mensura &&
             (labels_valor->datum[i] == ' ' ||
              labels_valor->datum[i] == '\t'))
        {
            i++;
        }

        si (i >= labels_valor->mensura)
        {
            frange;
        }

        /* Find end of token */
        start = i;
        dum (i < labels_valor->mensura &&
             labels_valor->datum[i] != ' ' &&
             labels_valor->datum[i] != '\t')
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
stml_titulos_numerus(StmlNodus* nodus)
{
    chorda* labels_valor;
    i32 i;
    i32 count;
    b32 in_token;

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
    count = ZEPHYRUM;
    in_token = FALSUM;

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
stml_titulos_capere(
    StmlNodus* nodus,
    Piscina*   piscina)
{
    chorda* labels_valor;
    chorda_fissio_fructus fissio;
    Xar* result;
    i32 i;
    chorda trimmed;
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
stml_titulum_addere(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulum)
{
    chorda* labels_valor;
    ChordaAedificator* aed;
    chorda new_valor;
    chorda* interned;
    s32 attr_index;
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

    new_valor = chorda_aedificator_finire(aed);
    interned = chorda_internare(intern, new_valor);
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
stml_titulum_removere(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulum)
{
    chorda* labels_valor;
    chorda_fissio_fructus fissio;
    ChordaAedificator* aed;
    chorda trimmed;
    chorda new_valor;
    chorda* interned;
    i32 i;
    s32 attr_index;
    StmlAttributum* attr;
    b32 found;
    b32 first;

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
        interned->datum = (i8*)piscina_allocare(piscina, I);
        interned->mensura = ZEPHYRUM;
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
stml_titulum_commutare(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulum)
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
