/* json.c - Implementatio Bibliothecae JSON
 */

#include "json.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


/* ========================================================================
 * CONSTANTAE
 * ======================================================================== */

#define JSON_CAPACITAS_INITIALIS  XVI


/* ========================================================================
 * TYPI INTERNI - LEXEMA
 * ======================================================================== */

nomen enumeratio {
    JSON_TOK_LBRACE,      /* { */
    JSON_TOK_RBRACE,      /* } */
    JSON_TOK_LBRACKET,    /* [ */
    JSON_TOK_RBRACKET,    /* ] */
    JSON_TOK_COLON,       /* : */
    JSON_TOK_COMMA,       /* , */
    JSON_TOK_CHORDA,      /* "..." */
    JSON_TOK_NUMERUS,     /* 123, -45.67, 1e10 */
    JSON_TOK_VERUM,       /* true */
    JSON_TOK_FALSUM,      /* false */
    JSON_TOK_NULLUM,      /* null */
    JSON_TOK_FINIS,       /* EOF */
    JSON_TOK_ERROR        /* Lexer error */
} JsonTokenGenus;

nomen structura {
        JsonTokenGenus  genus;
                   i32  valor_initium;   /* Start position in input */
                   i32  valor_mensura;   /* Length of value */
                   i32  linea;
                   i32  columna;
    constans character* error;      /* Error message (static string) */
} JsonToken;

nomen structura {
    constans i8* datum;
            i32  mensura;
            i32  positio;
            i32  linea;
            i32  columna;
        Piscina* piscina;
} JsonLexema;


/* ========================================================================
 * FUNCTIONES INTERNAE - CLAVES
 * ======================================================================== */

/* clavem internare. internamentum "" RECUSAT (NIHIL - contractus eius,
 * actio/insula/stml eo nituntur; vide internamentum.h), sed "" clavis
 * JSON valida est ({"":1}). Ergo json canonicam PROPRIAM tenet: sedes
 * una statica, ut comparatio per pointer maneat. Ante 2026-09-22
 * clavis NIHIL in objecto ponebatur -> json_scribere et iterator
 * SIGSEGV. NIHIL hinc = allocatio defecit. */
interior chorda*
_clavem_internare (
    chorda clavis)
{
    hic_manens     i8 octetus_vacuus = 0;
    hic_manens chorda vacua;

    si (clavis.mensura == ZEPHYRUM)
    {
        vacua.datum    = &octetus_vacuus;
        vacua.mensura  = ZEPHYRUM;
        redde &vacua;
    }
    redde chorda_internare(internamentum_globale(), clavis);
}

interior chorda*
_clavem_internare_literis (
    constans character* clavis)
{
    chorda vacua;

    si (clavis != NIHIL && clavis[ZEPHYRUM] == '\0')
    {
        vacua.datum    = NIHIL;
        vacua.mensura  = ZEPHYRUM;
        redde _clavem_internare(vacua);
    }
    redde chorda_internare_ex_literis(internamentum_globale(), clavis);
}


/* ========================================================================
 * FUNCTIONES INTERNAE - CHARACTERES
 * ======================================================================== */

interior b32
_est_spatium (
    character c)
{
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

interior b32
_est_digitus (
    character c)
{
    redde c >= '0' && c <= '9';
}

interior b32
_est_hex_digitus (
    character c)
{
    redde (c >= '0' && c <= '9')
        || (c >= 'a' && c <= 'f')
        || (c >= 'A' && c <= 'F');
}

interior i32
_hex_valor (
    character c)
{
    si (c >= '0' && c <= '9')
    {
        redde (i32)(c - '0');
    }
    si (c >= 'a' && c <= 'f')
    {
        redde (i32)(X + (c - 'a'));
    }
    si (c >= 'A' && c <= 'F')
    {
        redde (i32)(X + (c - 'A'));
    }
    redde 0;
}


/* ========================================================================
 * FUNCTIONES INTERNAE - LEXEMA
 * ======================================================================== */

interior vacuum
_lex_initare (
    JsonLexema* lex,
        chorda  input,
       Piscina* piscina)
{
    lex->datum    = input.datum;
    lex->mensura  = input.mensura;
    lex->positio  = 0;
    lex->linea    = I;
    lex->columna  = I;
    lex->piscina  = piscina;
}

interior character
_lex_currens (
    JsonLexema* lex)
{
    si (lex->positio >= lex->mensura)
    {
        redde '\0';
    }
    redde (character)lex->datum[lex->positio];
}

interior character
_lex_aspicere (
    JsonLexema* lex,
           i32  offset)
{
    i32 pos = lex->positio + offset;
    si (pos >= lex->mensura)
    {
        redde '\0';
    }
    redde (character)lex->datum[pos];
}

interior vacuum
_lex_avanzare (
    JsonLexema* lex)
{
    si (lex->positio < lex->mensura)
    {
        si ((character)lex->datum[lex->positio] == '\n')
        {
            lex->linea++;
            lex->columna = I;
        }
        alioquin
        {
            lex->columna++;
        }
        lex->positio++;
    }
}

interior vacuum
_lex_saltare_spatium (
    JsonLexema* lex)
{
    dum (lex->positio < lex->mensura && _est_spatium(_lex_currens(lex)))
    {
        _lex_avanzare(lex);
    }
}

interior JsonToken
_lex_error_ad (
             JsonLexema* lex,
     constans character* msg,
                    i32  linea,
                    i32  columna)
{
    JsonToken tok;
    (vacuum)lex;
    tok.genus          = JSON_TOK_ERROR;
    tok.valor_initium  = 0;
    tok.valor_mensura  = 0;
    tok.linea          = linea;
    tok.columna        = columna;
    tok.error          = msg;
    redde tok;
}

interior JsonToken
_lex_error (
            JsonLexema* lex,
    constans character* msg)
{
    redde _lex_error_ad(lex, msg, lex->linea, lex->columna);
}

interior JsonToken
_lex_simplex (
        JsonLexema* lex,
    JsonTokenGenus  genus)
{
    JsonToken tok;
    tok.genus          = genus;
    tok.valor_initium  = lex->positio;
    tok.valor_mensura  = I;
    tok.linea          = lex->linea;
    tok.columna        = lex->columna;
    tok.error          = NIHIL;
    _lex_avanzare(lex);
    redde tok;
}

/* Legere chorda cum escape sequences */
/* IV cifras hex post 'u' legere (currens = 'u'; post, currens =
 * cifra ultima). FALSUM si non hex aut finis. */
interior b32
_lex_unitatem_legere (
    JsonLexema* lex,
           i32* unitas)
{
    i32 i;

    *unitas = 0;
    per (i = 0; i < IV; i++)
    {
        _lex_avanzare(lex);
        si (   lex->positio >= lex->mensura
            || !_est_hex_digitus(_lex_currens(lex)))
        {
            redde FALSUM;
        }
        *unitas = *unitas * XVI + _hex_valor(_lex_currens(lex));
    }
    redde VERUM;
}

/* Legere chordam (RFC 8259 stricte, 2026-09-22): effugia sola
 * licita \" \\ \/ \b \f \n \r \t \uXXXX (olim '\x' accipiebatur
 * et per scripturam iteratam sensum mutabat); characteres < U+0020
 * crudi refutantur (olim tab et ceteri transibant); surrogatum altum
 * humile sequens postulat, humile solitarium refutatur (olim CESU-8
 * invalidum gignebatur). Locus erroris effugii = '\\' eius. */
interior JsonToken
_lex_chorda (
    JsonLexema* lex)
{
    JsonToken tok;
          i32 initium;

    tok.linea    = lex->linea;
    tok.columna  = lex->columna;
    tok.error    = NIHIL;

    /* Saltare " aperiens */
    _lex_avanzare(lex);
    initium = lex->positio;

    dum (lex->positio < lex->mensura)
    {
        character c = _lex_currens(lex);

        si (c == '"')
        {
            /* Fine chordae */
            tok.genus          = JSON_TOK_CHORDA;
            tok.valor_initium  = initium;
            tok.valor_mensura  = lex->positio - initium;
            _lex_avanzare(lex);  /* Saltare " claudens */
            redde tok;
        }

        si (c == '\\')
        {
            i32 linea_eff    = lex->linea;
            i32 columna_eff  = lex->columna;
            i32 unitas;

            _lex_avanzare(lex);
            si (lex->positio >= lex->mensura)
            {
                redde _lex_error(lex,
                    "Chorda non terminata post effugium");
            }

            c = _lex_currens(lex);
            si (c == 'u')
            {
                si (!_lex_unitatem_legere(lex, &unitas))
                {
                    redde _lex_error_ad(lex,
                        "Unicode effugium invalidum (u + IV hex)",
                        linea_eff, columna_eff);
                }
                si (unitas >= 0xDC00 && unitas <= 0xDFFF)
                {
                    redde _lex_error_ad(lex,
                        "Surrogatum humile solitarium (sine alto)",
                        linea_eff, columna_eff);
                }
                si (unitas >= 0xD800 && unitas <= 0xDBFF)
                {
                    i32 humile;

                    si (   _lex_aspicere(lex, I)  != '\\'
                        || _lex_aspicere(lex, II) != 'u')
                    {
                        redde _lex_error_ad(lex,
                            "Surrogatum altum sine humili sequente",
                            linea_eff, columna_eff);
                    }
                    _lex_avanzare(lex);
                    _lex_avanzare(lex);
                    si (   !_lex_unitatem_legere(lex, &humile)
                        || humile < 0xDC00 || humile > 0xDFFF)
                    {
                        redde _lex_error_ad(lex,
                            "Surrogatum altum sine humili sequente",
                            linea_eff, columna_eff);
                    }
                }
            }
            alioquin si (   c != '"' && c != '\\' && c != '/'
                         && c != 'b' && c != 'f' && c != 'n'
                         && c != 'r' && c != 't')
            {
                redde _lex_error_ad(lex,
                    "Effugium invalidum - licita: \" \\ / b f n r t u",
                    linea_eff, columna_eff);
            }
            _lex_avanzare(lex);
        }
        alioquin si ((insignatus character)c < 0x20)
        {
            redde _lex_error(lex,
                "Character imperans crudus in chorda (< U+0020) -"
                " effugium requiritur");
        }
        alioquin
        {
            _lex_avanzare(lex);
        }
    }

    redde _lex_error(lex, "Chorda non terminata");
}

/* Legere numerum (integer vel float) */
interior JsonToken
_lex_numerus (
    JsonLexema* lex)
{
    JsonToken tok;
          i32 initium = lex->positio;

    tok.linea    = lex->linea;
    tok.columna  = lex->columna;
    tok.error    = NIHIL;

    /* Optional minus */
    si (_lex_currens(lex) == '-')
    {
        _lex_avanzare(lex);
    }

    /* Integer part */
    si (_lex_currens(lex) == '0')
    {
        _lex_avanzare(lex);
    }
    alioquin si (_est_digitus(_lex_currens(lex)))
    {
        dum (_est_digitus(_lex_currens(lex)))
        {
            _lex_avanzare(lex);
        }
    }
    alioquin
    {
        redde _lex_error(lex, "Numerus invalidus");
    }

    /* Optional fractional part */
    si (_lex_currens(lex) == '.')
    {
        _lex_avanzare(lex);
        si (!_est_digitus(_lex_currens(lex)))
        {
            redde _lex_error(lex, "Digitus expectatus post punctum");
        }
        dum (_est_digitus(_lex_currens(lex)))
        {
            _lex_avanzare(lex);
        }
    }

    /* Optional exponent */
    si (_lex_currens(lex) == 'e' || _lex_currens(lex) == 'E')
    {
        _lex_avanzare(lex);
        si (_lex_currens(lex) == '+' || _lex_currens(lex) == '-')
        {
            _lex_avanzare(lex);
        }
        si (!_est_digitus(_lex_currens(lex)))
        {
            redde _lex_error(lex, "Digitus expectatus in exponente");
        }
        dum (_est_digitus(_lex_currens(lex)))
        {
            _lex_avanzare(lex);
        }
    }

    tok.genus          = JSON_TOK_NUMERUS;
    tok.valor_initium  = initium;
    tok.valor_mensura  = lex->positio - initium;
    redde tok;
}

/* Legere keyword (true, false, null) */
interior JsonToken
_lex_keyword (
    JsonLexema* lex)
{
    JsonToken tok;
          i32 initium = lex->positio;

    tok.linea    = lex->linea;
    tok.columna  = lex->columna;
    tok.error    = NIHIL;

    /* true */
    si (_lex_currens(lex) == 't')
    {
        si (   _lex_aspicere(lex, I)   == 'r'
            && _lex_aspicere(lex, II)  == 'u'
            && _lex_aspicere(lex, III) == 'e')
        {
            lex->positio       += IV;
            lex->columna       += IV;
            tok.genus          = JSON_TOK_VERUM;
            tok.valor_initium  = initium;
            tok.valor_mensura  = IV;
            redde tok;
        }
    }

    /* false */
    si (_lex_currens(lex) == 'f')
    {
        si (   _lex_aspicere(lex, I)   == 'a'
            && _lex_aspicere(lex, II)  == 'l'
            && _lex_aspicere(lex, III) == 's'
            && _lex_aspicere(lex, IV)  == 'e')
        {
            lex->positio       += V;
            lex->columna       += V;
            tok.genus          = JSON_TOK_FALSUM;
            tok.valor_initium  = initium;
            tok.valor_mensura  = V;
            redde tok;
        }
    }

    /* null */
    si (_lex_currens(lex) == 'n')
    {
        si (   _lex_aspicere(lex, I)   == 'u'
            && _lex_aspicere(lex, II)  == 'l'
            && _lex_aspicere(lex, III) == 'l')
        {
            lex->positio       += IV;
            lex->columna       += IV;
            tok.genus          = JSON_TOK_NULLUM;
            tok.valor_initium  = initium;
            tok.valor_mensura  = IV;
            redde tok;
        }
    }

    redde _lex_error(lex, "Keyword ignotum");
}

interior JsonToken
_lex_proxima (
    JsonLexema* lex)
{
    character c;

    _lex_saltare_spatium(lex);

    si (lex->positio >= lex->mensura)
    {
        JsonToken tok;
        tok.genus          = JSON_TOK_FINIS;
        tok.valor_initium  = 0;
        tok.valor_mensura  = 0;
        tok.linea          = lex->linea;
        tok.columna        = lex->columna;
        tok.error          = NIHIL;
        redde tok;
    }

    c = _lex_currens(lex);

    commutatio (c)
    {
        casus '{': redde _lex_simplex(lex, JSON_TOK_LBRACE);
        casus '}': redde _lex_simplex(lex, JSON_TOK_RBRACE);
        casus '[': redde _lex_simplex(lex, JSON_TOK_LBRACKET);
        casus ']': redde _lex_simplex(lex, JSON_TOK_RBRACKET);
        casus ':': redde _lex_simplex(lex, JSON_TOK_COLON);
        casus ',': redde _lex_simplex(lex, JSON_TOK_COMMA);
        casus '"': redde _lex_chorda(lex);
        casus 't':
        casus 'f':
        casus 'n':
            redde _lex_keyword(lex);
        ordinarius:
            si (c == '-' || _est_digitus(c))
            {
                redde _lex_numerus(lex);
            }
            redde _lex_error(lex, "Character ignotus");
    }
}


/* ========================================================================
 * FUNCTIONES INTERNAE - UNESCAPE
 * ======================================================================== */

/* Convertere escape sequences in chorda
 * Allocat nova chorda in piscina
 */
/* IV cifras hex ad punctum codicis */
interior i32
_hex_quattuor (
    constans i8* p)
{
    i32 valor = 0;
    i32 j;

    per (j = 0; j < IV; j++)
    {
        valor = valor * XVI + _hex_valor((character)p[j]);
    }
    redde valor;
}

/* Convertere escape sequences in chorda
 * Allocat nova chorda in piscina. Grammatica iam a lexemate probata
 * (effugia licita, surrogata paria); par surrogatum -> UTF-8 IV
 * octetorum (olim CESU-8: VI octeti invalidi). Octeti <= input.
 */
interior chorda
_unescape_chorda (
     chorda  input,
    Piscina* piscina)
{
    chorda  result;
        i8* buffer;
       i32  i;
       i32  out;

    si (input.mensura == 0)
    {
        result.datum    = NIHIL;
        result.mensura  = 0;
        redde result;
    }

    /* Allocare buffer (max same size as input) */
    buffer = (i8*)piscina_allocare(piscina, (i64)(input.mensura + I));
    si (!buffer)
    {
        result.datum    = NIHIL;
        result.mensura  = 0;
        redde result;
    }

    i    = 0;
    out  = 0;

    dum (i < input.mensura)
    {
        character c = (character)input.datum[i];

        si (c == '\\' && i + I < input.mensura)
        {
            character next = (character)input.datum[i + I];
            commutatio (next)
            {
                casus '"':  buffer[out++] = '"';  i += II; frange;
                casus '\\': buffer[out++] = '\\'; i += II; frange;
                casus '/':  buffer[out++] = '/';  i += II; frange;
                casus 'b':  buffer[out++] = '\b'; i += II; frange;
                casus 'f':  buffer[out++] = '\f'; i += II; frange;
                casus 'n':  buffer[out++] = '\n'; i += II; frange;
                casus 'r':  buffer[out++] = '\r'; i += II; frange;
                casus 't':  buffer[out++] = '\t'; i += II; frange;
                casus 'u':
                {
                    si (i + V < input.mensura)
                    {
                        i32 cp         = _hex_quattuor(input.datum
                                         + i + II);
                        i32 consumpta  = VI;

                        /* par surrogatum: \uD8xx\uDCxx -> U+10000.. */
                        si (   cp >= 0xD800 && cp <= 0xDBFF
                            && i + XI < input.mensura
                            && (character)input.datum[i + VI] == '\\'
                            && (character)input.datum[i + VII] == 'u')
                        {
                            i32 humile = _hex_quattuor(input.datum
                                         + i + VIII);
                            si (humile >= 0xDC00 && humile <= 0xDFFF)
                            {
                                cp = 0x10000 + ((cp - 0xD800) << X)
                                    + (humile - 0xDC00);
                                consumpta = XII;
                            }
                        }

                        si (cp < 0x80)
                        {
                            buffer[out++] = (i8)cp;
                        }
                        alioquin si (cp < 0x800)
                        {
                            buffer[out++] = (i8)(0xC0 | (cp >> VI));
                            buffer[out++] = (i8)(0x80 | (cp & 0x3F));
                        }
                        alioquin si (cp < 0x10000)
                        {
                            buffer[out++] = (i8)(0xE0 | (cp >> XII));
                            buffer[out++] = (i8)(0x80
                                | ((cp >> VI) & 0x3F));
                            buffer[out++] = (i8)(0x80 | (cp & 0x3F));
                        }
                        alioquin
                        {
                            buffer[out++] = (i8)(0xF0 | (cp >> XVIII));
                            buffer[out++] = (i8)(0x80
                                | ((cp >> XII) & 0x3F));
                            buffer[out++] = (i8)(0x80
                                | ((cp >> VI) & 0x3F));
                            buffer[out++] = (i8)(0x80 | (cp & 0x3F));
                        }
                        i += consumpta;
                    }
                    alioquin
                    {
                        buffer[out++] = (i8)c;
                        i++;
                    }
                    frange;
                }
                ordinarius:
                    buffer[out++] = (i8)c;
                    i++;
                    frange;
            }
        }
        alioquin
        {
            buffer[out++] = (i8)c;
            i++;
        }
    }

    result.datum    = buffer;
    result.mensura  = out;
    redde result;
}


/* ========================================================================
 * FUNCTIONES INTERNAE - NUMERUS PARSING
 * ======================================================================== */

interior b32
_numerus_est_fluitans (
    chorda valor)
{
    i32 i;
    per (i = 0; i < valor.mensura; i++)
    {
        character c = (character)valor.datum[i];
        si (c == '.' || c == 'e' || c == 'E')
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* integer ex lexemate (grammatica iam a lexemate probata: '-'?
 * cifrae). Quantitas in i64 (INSIGNATO) cumulatur cum custode
 * superfluxus: s64 = [-2^63, 2^63-1]. FALSUM = extra s64, vocans
 * refutat - olim tacite involvebat (9223372036854775808 -> -2^63,
 * superfluxus signatus = UB). */
interior b32
_parse_integer (
    chorda  valor,
       s64* fructus)
{
    i64 quantitas  = 0;
    i64 limes;
    i32 i          = 0;
    b32 negativus  = FALSUM;

    si (valor.mensura > 0 && (character)valor.datum[0] == '-')
    {
        negativus  = VERUM;
        i          = I;
    }

    /* 2^63 - 1 positivis, 2^63 negativis */
    limes = ((i64)I << LXIII) - (negativus ? (i64)0 : (i64)I);

    dum (i < valor.mensura)
    {
        character c = (character)valor.datum[i];
              i64 cifra;

        si (!_est_digitus(c))
        {
            redde FALSUM;
        }
        cifra = (i64)(c - '0');
        si (quantitas > (limes - cifra) / X)
        {
            redde FALSUM;
        }
        quantitas = quantitas * X + cifra;
        i++;
    }

    si (negativus && quantitas > 0)
    {
        /* -(2^63) sine superfluxu: -(q - 1) - 1 */
        *fructus = -(s64)(quantitas - I) - I;
    }
    alioquin
    {
        *fructus = (s64)quantitas;
    }
    redde VERUM;
}

interior f64
_parse_fluitans (
     chorda  valor,
    Piscina* piscina)
{
    character* buffer;
          f64  result;

    /* Copiare ad null-terminated buffer pro strtod */
    buffer = (character*)piscina_allocare(piscina, (i64)(valor.mensura
        + I));
    si (!buffer)
    {
        redde 0.0;
    }
    memcpy(buffer, valor.datum, (size_t)valor.mensura);
    buffer[valor.mensura] = '\0';

    result = strtod(buffer, NIHIL);
    redde result;
}


/* ========================================================================
 * FUNCTIONES INTERNAE - PARSER
 * ======================================================================== */

nomen structura {
    JsonLexema  lex;
     JsonToken  currens;
       Piscina* piscina;
           b32  error;
        chorda  error_msg;
           i32  error_linea;
           i32  error_columna;
           i32  profunditas;   /* objecta/tabulata aperta nunc */
} JsonParser;

/* Forward declarations */
interior JsonValor*
_parse_valor (
    JsonParser* parser);

interior vacuum
_parser_initare (
    JsonParser* parser,
        chorda  input,
       Piscina* piscina)
{
    _lex_initare(&parser->lex, input, piscina);
    parser->piscina            = piscina;
    parser->error              = FALSUM;
    parser->error_msg.datum    = NIHIL;
    parser->error_msg.mensura  = 0;
    parser->error_linea        = 0;
    parser->error_columna      = 0;
    parser->profunditas        = 0;
    parser->currens            = _lex_proxima(&parser->lex);
}

interior vacuum
_parser_avanzare (
    JsonParser* parser)
{
    parser->currens = _lex_proxima(&parser->lex);
}

interior vacuum
_parser_error (
            JsonParser* parser,
    constans character* msg)
{
    si (!parser->error)
    {
        parser->error      = VERUM;
        parser->error_msg  = chorda_ex_literis(msg, parser->piscina);
        /* lexema currens ERROR: causa vera lexematis addita
         * ('Clavis chorda expectata: Chorda non terminata') - olim
         * sub nuntio parsatoris celabatur (2026-09-22) */
        si (   parser->currens.genus == JSON_TOK_ERROR
            && parser->currens.error != NIHIL)
        {
            ChordaAedificator* aed = chorda_aedificator_creare(
                parser->piscina, CXXVIII);
            si (aed != NIHIL)
            {
                chorda_aedificator_appendere_literis(aed, msg);
                chorda_aedificator_appendere_literis(aed, ": ");
                chorda_aedificator_appendere_literis(aed,
                    parser->currens.error);
                parser->error_msg = chorda_aedificator_finire(aed);
            }
        }
        parser->error_linea    = parser->currens.linea;
        parser->error_columna  = parser->currens.columna;
    }
}

interior b32
_parser_expectare (
        JsonParser* parser,
    JsonTokenGenus  genus)
{
    si (parser->currens.genus != genus)
    {
        _parser_error(parser, "Token inexpectatum");
        redde FALSUM;
    }
    _parser_avanzare(parser);
    redde VERUM;
}

/* Extrahere chorda ex token currens (allocat copia) */
interior chorda
_parser_token_valor (
    JsonParser* parser)
{
    chorda  result;
        i8* buffer;
       i32  len = parser->currens.valor_mensura;

    si (len <= 0)
    {
        result.datum    = NIHIL;
        result.mensura  = 0;
        redde result;
    }

    buffer = (i8*)piscina_allocare(parser->piscina, (i64)len);
    si (!buffer)
    {
        result.datum    = NIHIL;
        result.mensura  = 0;
        redde result;
    }

    memcpy(buffer, parser->lex.datum + parser->currens.valor_initium,
        (size_t)len);
    result.datum    = buffer;
    result.mensura  = len;
    redde result;
}

interior JsonValor*
_parse_objectum (
    JsonParser* parser)
{
    JsonValor* obj;

    /* Saltare { */
    _parser_avanzare(parser);

    obj = json_objectum_creare(parser->piscina);
    si (!obj)
    {
        _parser_error(parser, "Allocatio fallita");
        redde NIHIL;
    }

    /* Objectum vacuum? */
    si (parser->currens.genus == JSON_TOK_RBRACE)
    {
        _parser_avanzare(parser);
        redde obj;
    }

    /* Parse pairs */
    fac
    {
           chorda  clavis_raw;
           chorda  clavis_unesc;
           chorda* clavis_intern;
        JsonValor* valor;
          JsonPar* slot;

        /* Expectare chorda pro clave */
        si (parser->currens.genus != JSON_TOK_CHORDA)
        {
            _parser_error(parser, "Clavis chorda expectata");
            redde NIHIL;
        }
        clavis_raw    = _parser_token_valor(parser);
        clavis_unesc  = _unescape_chorda(clavis_raw, parser->piscina);

        /* Internare clavem - "" quoque canonicam habet; NIHIL =
         * allocatio defecit, et clavis NIHIL in objecto scribere et
         * iteratorem necaret */
        clavis_intern = _clavem_internare(clavis_unesc);
        si (!clavis_intern)
        {
            _parser_error(parser, "Allocatio fallita");
            redde NIHIL;
        }

        /* CLAVIS DUPLICATA refutatur (2026-09-22; olim ambae
         * servabantur et capere PRIMAM reddebat - RFC 8259 'SHOULD
         * be unique', eventus annalium cum duobus valoribus unius
         * clavis corruptio est). Comparatio per pointer internatum
         * (post effugia); locus = clavis duplicata, ideo ANTE
         * progressum. Linearis ut capere. */
        {
            i32 k;
            i32 num = (i32)xar_numerus(obj->datum.objectum);

            per (k = 0; k < num; k++)
            {
                JsonPar* prior = (JsonPar*)xar_obtinere(
                    obj->datum.objectum, k);
                si (prior && prior->clavis == clavis_intern)
                {
                    _parser_error(parser, "Clavis duplicata");
                    redde NIHIL;
                }
            }
        }
        _parser_avanzare(parser);

        /* Expectare : */
        si (!_parser_expectare(parser, JSON_TOK_COLON))
        {
            redde NIHIL;
        }

        /* Parse valor */
        valor = _parse_valor(parser);
        si (!valor)
        {
            redde NIHIL;
        }

        /* Addere ad objectum (directe cum pointer internato) */
        slot = (JsonPar*)xar_addere(obj->datum.objectum);
        si (slot)
        {
            slot->clavis  = clavis_intern;
            slot->valor   = valor;
        }

        /* Comma? */
        si (parser->currens.genus != JSON_TOK_COMMA)
        {
            frange;
        }
        _parser_avanzare(parser);

        /* Trailing comma? */
        si (parser->currens.genus == JSON_TOK_RBRACE)
        {
            _parser_error(parser, "Trailing comma non permissa");
            redde NIHIL;
        }

    } dum (parser->currens.genus != JSON_TOK_RBRACE &&
           parser->currens.genus != JSON_TOK_FINIS &&
           parser->currens.genus != JSON_TOK_ERROR);

    si (!_parser_expectare(parser, JSON_TOK_RBRACE))
    {
        redde NIHIL;
    }

    redde obj;
}

interior JsonValor*
_parse_tabulatum (
    JsonParser* parser)
{
    JsonValor* arr;

    /* Saltare [ */
    _parser_avanzare(parser);

    arr = json_tabulatum_creare(parser->piscina);
    si (!arr)
    {
        _parser_error(parser, "Allocatio fallita");
        redde NIHIL;
    }

    /* Tabulatum vacuum? */
    si (parser->currens.genus == JSON_TOK_RBRACKET)
    {
        _parser_avanzare(parser);
        redde arr;
    }

    /* Parse elements */
    fac
    {
        JsonValor* elem = _parse_valor(parser);
        si (!elem)
        {
            redde NIHIL;
        }

        json_tabulatum_addere(arr, elem);

        /* Comma? */
        si (parser->currens.genus != JSON_TOK_COMMA)
        {
            frange;
        }
        _parser_avanzare(parser);

        /* Trailing comma? */
        si (parser->currens.genus == JSON_TOK_RBRACKET)
        {
            _parser_error(parser, "Trailing comma non permissa");
            redde NIHIL;
        }

    } dum (parser->currens.genus != JSON_TOK_RBRACKET &&
           parser->currens.genus != JSON_TOK_FINIS &&
           parser->currens.genus != JSON_TOK_ERROR);

    si (!_parser_expectare(parser, JSON_TOK_RBRACKET))
    {
        redde NIHIL;
    }

    redde arr;
}

interior JsonValor*
_parse_valor (
    JsonParser* parser)
{
    JsonValor* val;

    si (parser->error)
    {
        redde NIHIL;
    }

    si (parser->currens.genus == JSON_TOK_ERROR)
    {
        parser->error = VERUM;
        si (parser->currens.error)
        {
            parser->error_msg = chorda_ex_literis(parser->currens.error,
                                                   parser->piscina);
        }
        alioquin
        {
            parser->error_msg.datum    = NIHIL;
            parser->error_msg.mensura  = 0;
        }
        parser->error_linea    = parser->currens.linea;
        parser->error_columna  = parser->currens.columna;
        redde NIHIL;
    }

    commutatio (parser->currens.genus)
    {
        casus JSON_TOK_LBRACE:
        casus JSON_TOK_LBRACKET:
            /* limes recursionis: textus hostilis acervum non exhaurit
             * (II000000 '[' = SIGSEGV ante 2026-09-22) */
            si (parser->profunditas >= JSON_PROFUNDITAS_MAXIMA)
            {
                _parser_error(parser, "Nidificatio nimis profunda");
                redde NIHIL;
            }
            parser->profunditas++;
            val = (parser->currens.genus == JSON_TOK_LBRACE)
                ? _parse_objectum(parser)
                : _parse_tabulatum(parser);
            parser->profunditas--;
            redde val;

        casus JSON_TOK_CHORDA:
        {
            chorda raw = _parser_token_valor(parser);
            chorda unescaped = _unescape_chorda(raw, parser->piscina);
            val = json_chorda_creare(parser->piscina, unescaped);
            _parser_avanzare(parser);
            redde val;
        }

        casus JSON_TOK_NUMERUS:
        {
            chorda num_str = _parser_token_valor(parser);
                        si (_numerus_est_fluitans(num_str))
                        {
                f64 f  = _parse_fluitans(num_str, parser->piscina);
                /* 1e400 -> infinitum (strtod superfluens HUGE_VAL reddit): JSON infinitum non habet, et
                 * scriptor id 'null' scriberet - refutatur. Subfluxus
                 * (1e-400 -> 0) licet: valor proximus, non falsus. */
                si (f == HUGE_VAL || f == -HUGE_VAL)
                {
                    _parser_error(parser, "Numerus extra f64");
                    redde NIHIL;
                }
                val    = json_fluitans_creare(parser->piscina, f);
                        }
            alioquin
                        {
                s64 n;
                si (!_parse_integer(num_str, &n))
                {
                    _parser_error(parser,
                        "Integer extra s64 (-2^63 .. 2^63-1)");
                    redde NIHIL;
                }
                val    = json_integer_creare(parser->piscina, n);
                        }
            _parser_avanzare(parser);
            redde val;
        }

        casus JSON_TOK_VERUM:
            val = json_boolean_creare(parser->piscina, VERUM);
            _parser_avanzare(parser);
            redde val;

        casus JSON_TOK_FALSUM:
            val = json_boolean_creare(parser->piscina, FALSUM);
            _parser_avanzare(parser);
            redde val;

        casus JSON_TOK_NULLUM:
            val = json_nullum_creare(parser->piscina);
            _parser_avanzare(parser);
            redde val;

        casus JSON_TOK_FINIS:
            _parser_error(parser, "Valor expectatus");
            redde NIHIL;

        ordinarius:
            _parser_error(parser, "Token inexpectatum");
            redde NIHIL;
    }
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - PARSING
 * ======================================================================== */

JsonResultus
json_legere (
     chorda  input,
    Piscina* piscina)
{
    JsonResultus res;
      JsonParser parser;

    res.successus      = FALSUM;
    res.radix          = NIHIL;
    res.error.datum    = NIHIL;
    res.error.mensura  = 0;
    res.linea          = 0;
    res.columna        = 0;

    si (!piscina)
    {
        /* Non possumus allocare error message sine piscina */
        res.error.datum    = NIHIL;
        res.error.mensura  = 0;
        redde res;
    }

    _parser_initare(&parser, input, piscina);

    res.radix = _parse_valor(&parser);

    /* CAUDA: post valorem radicis nihil nisi spatium album. Sine hoc
     * '{"a":1} garbage', '1 2', '{"a":1}}' successus erant et '01'
     * ut 0 legebatur; lineae annalium duos eventus conglutinatos ut
     * unum transmittebant (2026-09-22). Locus = lexema caudae
     * primum (etiam error lexematis, olim tacitus). */
    si (   !parser.error && res.radix
        && parser.currens.genus != JSON_TOK_FINIS)
    {
        _parser_error(&parser, "Contentum post valorem radicis");
    }

    si (parser.error || !res.radix)
    {
        res.successus  = FALSUM;
        res.radix      = NIHIL;
        res.error      = parser.error_msg;
        res.linea      = parser.error_linea;
        res.columna    = parser.error_columna;
    }
    alioquin
    {
        res.successus = VERUM;
    }

    redde res;
}

JsonResultus
json_legere_literis (
    constans character* input,
               Piscina* piscina)
{
    chorda ch;

    si (!input || !piscina)
    {
        JsonResultus res;
        res.successus      = FALSUM;
        res.radix          = NIHIL;
        res.error.datum    = NIHIL;
        res.error.mensura  = 0;
        res.linea          = 0;
        res.columna        = 0;
        redde res;
    }

    ch = chorda_ex_literis(input, piscina);

    redde json_legere(ch, piscina);
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - VERIFICATIO GENERIS
 * ======================================================================== */

JsonGenus
json_genus (
    JsonValor* valor)
{
    si (!valor)
    {
        redde JSON_NULLUM;
    }
    redde valor->genus;
}

b32
json_est_nullum (
    JsonValor* valor)
{
    redde valor && valor->genus == JSON_NULLUM;
}

b32
json_est_boolean (
    JsonValor* valor)
{
    redde valor && valor->genus == JSON_BOOLEAN;
}

b32
json_est_integer (
    JsonValor* valor)
{
    redde valor && valor->genus == JSON_INTEGER;
}

b32
json_est_fluitans (
    JsonValor* valor)
{
    redde valor && valor->genus == JSON_FLUITANS;
}

b32
json_est_chorda (
    JsonValor* valor)
{
    redde valor && valor->genus == JSON_CHORDA;
}

b32
json_est_tabulatum (
    JsonValor* valor)
{
    redde valor && valor->genus == JSON_TABULATUM;
}

b32
json_est_objectum (
    JsonValor* valor)
{
    redde valor && valor->genus == JSON_OBJECTUM;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - EXTRACTIO VALORUM
 * ======================================================================== */

b32
json_ad_boolean (
    JsonValor* valor)
{
    si (!valor || valor->genus != JSON_BOOLEAN)
    {
        redde FALSUM;
    }
    redde valor->datum.boolean_valor;
}

s64
json_ad_integer (
    JsonValor* valor)
{
    si (!valor || valor->genus != JSON_INTEGER)
    {
        redde 0;
    }
    redde valor->datum.integer_valor;
}

f64
json_ad_fluitans (
    JsonValor* valor)
{
    si (!valor || valor->genus != JSON_FLUITANS)
    {
        redde 0.0;
    }
    redde valor->datum.fluitans_valor;
}

chorda
json_ad_chorda (
    JsonValor* valor)
{
    chorda vacua;
    vacua.datum    = NIHIL;
    vacua.mensura  = 0;

    si (!valor || valor->genus != JSON_CHORDA)
    {
        redde vacua;
    }
    redde valor->datum.chorda_valor;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - ACCESSUS TABULATI
 * ======================================================================== */

i32
json_tabulatum_numerus (
    JsonValor* valor)
{
    si (   !valor || valor->genus != JSON_TABULATUM
        || !valor->datum.tabulatum)
    {
        redde 0;
    }
    redde (i32)xar_numerus(valor->datum.tabulatum);
}

JsonValor*
json_tabulatum_obtinere (
    JsonValor* valor,
          i32  index)
{
    JsonValor** ptr;

    si (   !valor || valor->genus != JSON_TABULATUM
        || !valor->datum.tabulatum)
    {
        redde NIHIL;
    }

    si (index < 0 || index >= (i32)xar_numerus(valor->datum.tabulatum))
    {
        redde NIHIL;
    }

    ptr = (JsonValor**)xar_obtinere(valor->datum.tabulatum, index);
    si (!ptr)
    {
        redde NIHIL;
    }
    redde *ptr;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - ACCESSUS OBJECTI
 * ======================================================================== */

i32
json_objectum_numerus (
    JsonValor* valor)
{
    si (   !valor || valor->genus != JSON_OBJECTUM
        || !valor->datum.objectum)
    {
        redde 0;
    }
    redde (i32)xar_numerus(valor->datum.objectum);
}

JsonValor*
json_objectum_capere (
             JsonValor* valor,
    constans character* clavis)
{
    chorda* clavis_intern;
       i32  i;
       i32  num;

    si (!clavis || !valor)
    {
        redde NIHIL;
    }

    si (valor->genus != JSON_OBJECTUM || !valor->datum.objectum)
    {
        redde NIHIL;
    }

    /* Internare clavem - nulla allocatio in piscina valor! */
    clavis_intern = _clavem_internare_literis(clavis);
    si (!clavis_intern)
    {
        redde NIHIL;
    }

    num = (i32)xar_numerus(valor->datum.objectum);

    per (i = 0; i < num; i++)
    {
        JsonPar* par = (JsonPar*)xar_obtinere(valor->datum.objectum, i);
        /* Comparatio pointer - O(1)! */
        si (par && par->clavis == clavis_intern)
        {
            redde par->valor;
        }
    }

    redde NIHIL;
}

JsonValor*
json_objectum_capere_chorda (
    JsonValor* valor,
       chorda  clavis)
{
    chorda* clavis_intern;
       i32  i;
       i32  num;

    si (   !valor || valor->genus != JSON_OBJECTUM
        || !valor->datum.objectum)
    {
        redde NIHIL;
    }

    /* Internare clavem */
    clavis_intern = _clavem_internare(clavis);
    si (!clavis_intern)
    {
        redde NIHIL;
    }

    num = (i32)xar_numerus(valor->datum.objectum);

    per (i = 0; i < num; i++)
    {
        JsonPar* par = (JsonPar*)xar_obtinere(valor->datum.objectum, i);
        /* Comparatio pointer - O(1)! */
        si (par && par->clavis == clavis_intern)
        {
            redde par->valor;
        }
    }

    redde NIHIL;
}

b32
json_objectum_habet (
             JsonValor* valor,
    constans character* clavis)
{
    redde json_objectum_capere(valor, clavis) != NIHIL;
}

JsonPar*
json_objectum_par_obtinere (
    JsonValor* valor,
          i32  index)
{
    si (   !valor || valor->genus != JSON_OBJECTUM
        || !valor->datum.objectum)
    {
        redde NIHIL;
    }

    si (index < 0 || index >= (i32)xar_numerus(valor->datum.objectum))
    {
        redde NIHIL;
    }

    redde (JsonPar*)xar_obtinere(valor->datum.objectum, index);
}

JsonObjectumIterator
json_objectum_iterator (
    JsonValor* valor)
{
    JsonObjectumIterator iter;
    iter.objectum  = valor;
    iter.index     = 0;
    redde iter;
}

b32
json_objectum_iterator_proxima (
    JsonObjectumIterator*  iter,
                  chorda*  clavis_ex,
               JsonValor** valor_ex)
{
    JsonPar* par;

    si (!iter || !iter->objectum)
    {
        redde FALSUM;
    }

    si (iter->objectum->genus != JSON_OBJECTUM)
    {
        redde FALSUM;
    }

    par = json_objectum_par_obtinere(iter->objectum, iter->index);
    si (!par)
    {
        redde FALSUM;
    }

    si (clavis_ex)
    {
        /* Dereference pointer ad chorda internata */
        *clavis_ex = *par->clavis;
    }
    si (valor_ex)
    {
        *valor_ex = par->valor;
    }

    iter->index++;
    redde VERUM;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - CREARE VALORES
 * ======================================================================== */

JsonValor*
json_nullum_creare (
    Piscina* piscina)
{
    JsonValor* val;

    si (!piscina)
    {
        redde NIHIL;
    }

    val = (JsonValor*)piscina_allocare(piscina,
        (i64)magnitudo(JsonValor));
    si (!val)
    {
        redde NIHIL;
    }

    val->genus    = JSON_NULLUM;
    val->piscina  = piscina;
    redde val;
}

JsonValor*
json_boolean_creare (
    Piscina* piscina,
        b32  valor)
{
    JsonValor* val;

    si (!piscina)
    {
        redde NIHIL;
    }

    val = (JsonValor*)piscina_allocare(piscina,
        (i64)magnitudo(JsonValor));
    si (!val)
    {
        redde NIHIL;
    }

    val->genus                = JSON_BOOLEAN;
    val->datum.boolean_valor  = valor;
    val->piscina              = piscina;
    redde val;
}

JsonValor*
json_integer_creare (
    Piscina* piscina,
        s64  valor)
{
    JsonValor* val;

    si (!piscina)
    {
        redde NIHIL;
    }

    val = (JsonValor*)piscina_allocare(piscina,
        (i64)magnitudo(JsonValor));
    si (!val)
    {
        redde NIHIL;
    }

    val->genus                = JSON_INTEGER;
    val->datum.integer_valor  = valor;
    val->piscina              = piscina;
    redde val;
}

JsonValor*
json_fluitans_creare (
    Piscina* piscina,
        f64  valor)
{
    JsonValor* val;

    si (!piscina)
    {
        redde NIHIL;
    }

    val = (JsonValor*)piscina_allocare(piscina,
        (i64)magnitudo(JsonValor));
    si (!val)
    {
        redde NIHIL;
    }

    val->genus                 = JSON_FLUITANS;
    val->datum.fluitans_valor  = valor;
    val->piscina               = piscina;
    redde val;
}

JsonValor*
json_chorda_creare (
    Piscina* piscina,
     chorda  valor)
{
    JsonValor* val;

    si (!piscina)
    {
        redde NIHIL;
    }

    val = (JsonValor*)piscina_allocare(piscina,
        (i64)magnitudo(JsonValor));
    si (!val)
    {
        redde NIHIL;
    }

    val->genus               = JSON_CHORDA;
    val->datum.chorda_valor  = valor;
    val->piscina             = piscina;
    redde val;
}

JsonValor*
json_chorda_creare_literis (
               Piscina* piscina,
    constans character* valor)
{
    chorda  ch;
        i8* buffer;
       i32  len;

    si (!piscina || !valor)
    {
        redde NIHIL;
    }

    len     = (i32)strlen(valor);
    buffer  = (i8*)piscina_allocare(piscina, (i64)(len + I));
    si (!buffer)
    {
        redde NIHIL;
    }
    memcpy(buffer, valor, (size_t)len);

    ch.datum    = buffer;
    ch.mensura  = len;

    redde json_chorda_creare(piscina, ch);
}

JsonValor*
json_tabulatum_creare (
    Piscina* piscina)
{
    JsonValor* val;

    si (!piscina)
    {
        redde NIHIL;
    }

    val = (JsonValor*)piscina_allocare(piscina,
        (i64)magnitudo(JsonValor));
    si (!val)
    {
        redde NIHIL;
    }

    val->genus = JSON_TABULATUM;
    val->datum.tabulatum = xar_creare(piscina,
        (i32)magnitudo(JsonValor*));
    val->piscina = piscina;
    redde val;
}

JsonValor*
json_objectum_creare (
    Piscina* piscina)
{
    JsonValor* val;

    si (!piscina)
    {
        redde NIHIL;
    }

    val = (JsonValor*)piscina_allocare(piscina,
        (i64)magnitudo(JsonValor));
    si (!val)
    {
        redde NIHIL;
    }

    val->genus           = JSON_OBJECTUM;
    val->datum.objectum  = xar_creare(piscina, (i32)magnitudo(JsonPar));
    val->piscina         = piscina;
    redde val;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - MUTARE CONTAINERS
 * ======================================================================== */

vacuum
json_tabulatum_addere (
    JsonValor* tabulatum,
    JsonValor* valor)
{
    JsonValor** slot;

    si (!tabulatum || tabulatum->genus != JSON_TABULATUM)
    {
        redde;
    }

    si (!tabulatum->datum.tabulatum)
    {
        redde;
    }

    slot = (JsonValor**)xar_addere(tabulatum->datum.tabulatum);
    si (slot)
    {
        *slot = valor;
    }
}

vacuum
json_objectum_ponere (
             JsonValor* objectum,
    constans character* clavis,
             JsonValor* valor)
{
     chorda* clavis_intern;
        i32  i;
        i32  num;
    JsonPar* slot;

    si (!objectum || !clavis)
    {
        redde;
    }

    si (objectum->genus != JSON_OBJECTUM || !objectum->datum.objectum)
    {
        redde;
    }

    /* Internare clavem */
    clavis_intern = _clavem_internare_literis(clavis);
    si (!clavis_intern)
    {
        redde;
    }

    /* Probare si clavis iam existit */
    num = (i32)xar_numerus(objectum->datum.objectum);
    per (i = 0; i < num; i++)
    {
        JsonPar* par = (JsonPar*)xar_obtinere(objectum->datum.objectum,
            i);
        si (par && par->clavis == clavis_intern)
        {
            /* Clavis existit - overwrite */
            par->valor = valor;
            redde;
        }
    }

    /* Nova clavis */
    slot = (JsonPar*)xar_addere(objectum->datum.objectum);
    si (slot)
    {
        slot->clavis  = clavis_intern;
        slot->valor   = valor;
    }
}

vacuum
json_objectum_ponere_chorda (
    JsonValor* objectum,
       chorda  clavis,
    JsonValor* valor)
{
     chorda* clavis_intern;
        i32  i;
        i32  num;
    JsonPar* slot;

    si (!objectum || objectum->genus != JSON_OBJECTUM)
    {
        redde;
    }

    si (!objectum->datum.objectum)
    {
        redde;
    }

    /* Internare clavem */
    clavis_intern = _clavem_internare(clavis);
    si (!clavis_intern)
    {
        redde;
    }

    /* Probare si clavis iam existit */
    num = (i32)xar_numerus(objectum->datum.objectum);
    per (i = 0; i < num; i++)
    {
        JsonPar* par = (JsonPar*)xar_obtinere(objectum->datum.objectum,
            i);
        si (par && par->clavis == clavis_intern)
        {
            /* Clavis existit - overwrite */
            par->valor = valor;
            redde;
        }
    }

    /* Nova clavis */
    slot = (JsonPar*)xar_addere(objectum->datum.objectum);
    si (slot)
    {
        slot->clavis  = clavis_intern;
        slot->valor   = valor;
    }
}


/* ========================================================================
 * FUNCTIONES INTERNAE - SERIALIZATION
 * ======================================================================== */

/* fluitantem scribere (2026-09-22). NaN et infinita -> null: JSON ea
 * non habet, scriptor viam erroris non habet, et 'nan'/'inf' textum
 * invalidum dabant (JSON.stringify idem facit). Forma BREVISSIMA quae
 * EXACTE redit (praecisio XV..XVII, strtod probat): 0.1, non
 * 0.10000000000000001. Forma integrum imitans ('1', '-0',
 * '-9007199254740992') '.0' accipit - aliter lectio iterata genus
 * INTEGER dabat. */
interior vacuum
_fluitantem_scribere (
    ChordaAedificator* aed,
                  f64  valor)
{
    character buffer[LXIV];
      integer praecisio;
          i32 i;
          b32 integrum_imitatur = VERUM;

    si (valor != valor || valor == HUGE_VAL || valor == -HUGE_VAL)
    {
        chorda_aedificator_appendere_literis(aed, "null");
        redde;
    }

    per (praecisio = XV; praecisio <= XVII; praecisio++)
    {
        sprintf(buffer, "%.*g", praecisio, valor);
        si (strtod(buffer, NIHIL) == valor)
        {
            frange;
        }
    }

    per (i = 0; buffer[i] != '\0'; i++)
    {
        si (buffer[i] == '.' || buffer[i] == 'e' || buffer[i] == 'E')
        {
            integrum_imitatur = FALSUM;
            frange;
        }
    }

    chorda_aedificator_appendere_literis(aed, buffer);
    si (integrum_imitatur)
    {
        chorda_aedificator_appendere_literis(aed, ".0");
    }
}

interior vacuum
_scribere_valor (
            JsonValor* valor,
    ChordaAedificator* aed,
                  b32  pulchrum,
                  i32  indent);

interior vacuum
_scribere_indentatio (
    ChordaAedificator* aed,
                  i32  level)
{
    i32 i;
    per (i = 0; i < level; i++)
    {
        chorda_aedificator_appendere_literis(aed, "  ");
    }
}

interior vacuum
_scribere_objectum (
            JsonValor* obj,
    ChordaAedificator* aed,
                  b32  pulchrum,
                  i32  indent)
{
    i32 num;
    i32 i;

    chorda_aedificator_appendere_character(aed, '{');

    num = json_objectum_numerus(obj);

    si (num > 0 && pulchrum)
    {
        chorda_aedificator_appendere_character(aed, '\n');
    }

    per (i = 0; i < num; i++)
    {
        JsonPar* par = json_objectum_par_obtinere(obj, i);
        si (!par)
        {
            perge;
        }

        si (pulchrum)
        {
            _scribere_indentatio(aed, indent + I);
        }

        /* Clavis (dereference pointer) */
        chorda_aedificator_appendere_character(aed, '"');
        chorda_aedificator_appendere_evasus_json(aed, *par->clavis);
        chorda_aedificator_appendere_character(aed, '"');

        chorda_aedificator_appendere_character(aed, ':');
        si (pulchrum)
        {
            chorda_aedificator_appendere_character(aed, ' ');
        }

        /* Valor */
        _scribere_valor(par->valor, aed, pulchrum, indent + I);

        si (i < num - I)
        {
            chorda_aedificator_appendere_character(aed, ',');
        }

        si (pulchrum)
        {
            chorda_aedificator_appendere_character(aed, '\n');
        }
    }

    si (num > 0 && pulchrum)
    {
        _scribere_indentatio(aed, indent);
    }

    chorda_aedificator_appendere_character(aed, '}');
}

interior vacuum
_scribere_tabulatum (
            JsonValor* arr,
    ChordaAedificator* aed,
                  b32  pulchrum,
                  i32  indent)
{
    i32 num;
    i32 i;

    chorda_aedificator_appendere_character(aed, '[');

    num = json_tabulatum_numerus(arr);

    si (num > 0 && pulchrum)
    {
        chorda_aedificator_appendere_character(aed, '\n');
    }

    per (i = 0; i < num; i++)
    {
        JsonValor* elem = json_tabulatum_obtinere(arr, i);

        si (pulchrum)
        {
            _scribere_indentatio(aed, indent + I);
        }

        _scribere_valor(elem, aed, pulchrum, indent + I);

        si (i < num - I)
        {
            chorda_aedificator_appendere_character(aed, ',');
        }

        si (pulchrum)
        {
            chorda_aedificator_appendere_character(aed, '\n');
        }
    }

    si (num > 0 && pulchrum)
    {
        _scribere_indentatio(aed, indent);
    }

    chorda_aedificator_appendere_character(aed, ']');
}

interior vacuum
_scribere_valor (
            JsonValor* valor,
    ChordaAedificator* aed,
                  b32  pulchrum,
                  i32  indent)
{
    si (!valor)
    {
        chorda_aedificator_appendere_literis(aed, "null");
        redde;
    }

    commutatio (valor->genus)
    {
        casus JSON_NULLUM:
            chorda_aedificator_appendere_literis(aed, "null");
            frange;

        casus JSON_BOOLEAN:
            si (valor->datum.boolean_valor)
            {
                chorda_aedificator_appendere_literis(aed, "true");
            }
            alioquin
            {
                chorda_aedificator_appendere_literis(aed, "false");
            }
            frange;

        casus JSON_INTEGER:
        {
            character buffer[XXXII];
            sprintf(buffer, "%lld",
                (long long)valor->datum.integer_valor);
            chorda_aedificator_appendere_literis(aed, buffer);
            frange;
        }

                casus JSON_FLUITANS:
            _fluitantem_scribere(aed, valor->datum.fluitans_valor);
            frange;

        casus JSON_CHORDA:
            chorda_aedificator_appendere_character(aed, '"');
            chorda_aedificator_appendere_evasus_json(aed,
                valor->datum.chorda_valor);
            chorda_aedificator_appendere_character(aed, '"');
            frange;

        casus JSON_TABULATUM:
            _scribere_tabulatum(valor, aed, pulchrum, indent);
            frange;

        casus JSON_OBJECTUM:
            _scribere_objectum(valor, aed, pulchrum, indent);
            frange;
    }
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - SERIALIZATION
 * ======================================================================== */

chorda
json_scribere (
    JsonValor* valor,
      Piscina* piscina)
{
    ChordaAedificator* aed;
               chorda  result;

    si (!piscina)
    {
        result.datum    = NIHIL;
        result.mensura  = 0;
        redde result;
    }

    aed = chorda_aedificator_creare(piscina, CCLVI);
    si (!aed)
    {
        result.datum    = NIHIL;
        result.mensura  = 0;
        redde result;
    }

    json_scribere_ad_aedificator(valor, aed);

    redde chorda_aedificator_finire(aed);
}

chorda
json_scribere_pulchrum (
    JsonValor* valor,
      Piscina* piscina)
{
    ChordaAedificator* aed;
               chorda  result;

    si (!piscina)
    {
        result.datum    = NIHIL;
        result.mensura  = 0;
        redde result;
    }

    aed = chorda_aedificator_creare(piscina, CCLVI);
    si (!aed)
    {
        result.datum    = NIHIL;
        result.mensura  = 0;
        redde result;
    }

    json_scribere_ad_aedificator_pulchrum(valor, aed, 0);

    redde chorda_aedificator_finire(aed);
}

vacuum
json_scribere_ad_aedificator (
            JsonValor* valor,
    ChordaAedificator* aed)
{
    si (!aed)
    {
        redde;
    }

    _scribere_valor(valor, aed, FALSUM, 0);
}

vacuum
json_scribere_ad_aedificator_pulchrum (
            JsonValor* valor,
    ChordaAedificator* aed,
                  i32  indentatio)
{
    si (!aed)
    {
        redde;
    }

    _scribere_valor(valor, aed, VERUM, indentatio);
}
