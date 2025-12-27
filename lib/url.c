/* url.c - URL Percent Encoding/Decoding
 *
 * Implementatio RFC 3986 URL encoding.
 */

#include "url.h"
#include "piscina.h"
#include "chorda.h"

#include <string.h>


/* ========================================================================
 * TABULAE
 * ======================================================================== */

/* Characteres hexadecimales */
hic_manens constans character* hex_characteres = "0123456789ABCDEF";


/* ========================================================================
 * FUNCTIONES INTERNAE
 * ======================================================================== */

/* Verificare si character est unreserved (RFC 3986) */
interior b32
est_unreserved(i8 c)
{
    /* A-Z */
    si (c >= 'A' && c <= 'Z') redde VERUM;
    /* a-z */
    si (c >= 'a' && c <= 'z') redde VERUM;
    /* 0-9 */
    si (c >= '0' && c <= '9') redde VERUM;
    /* -, _, ., ~ */
    si (c == '-' || c == '_' || c == '.' || c == '~') redde VERUM;

    redde FALSUM;
}

/* Convertere hex character ad valor */
interior s32
hex_ad_valor(i8 c)
{
    si (c >= '0' && c <= '9') redde c - '0';
    si (c >= 'A' && c <= 'F') redde c - 'A' + X;
    si (c >= 'a' && c <= 'f') redde c - 'a' + X;
    redde -1;
}


/* ========================================================================
 * FUNCTIONES - CODIFICATIO
 * ======================================================================== */

chorda
url_codificare(
    chorda   originalis,
    Piscina* piscina)
{
    chorda resultatus;
    i32    longitudo_output;
    i8*    output;
    i32    i;
    i32    j;
    i8     c;

    si (originalis.datum == NIHIL || originalis.mensura <= 0 || piscina == NIHIL)
    {
        resultatus.datum = NIHIL;
        resultatus.mensura = 0;
        redde resultatus;
    }

    /* Calculare longitudo maxima (omnes characteres encoded = 3x) */
    longitudo_output = 0;
    per (i = 0; i < originalis.mensura; i++)
    {
        si (est_unreserved(originalis.datum[i]))
        {
            longitudo_output++;
        }
        alioquin
        {
            longitudo_output += III;
        }
    }

    output = piscina_allocare(piscina, longitudo_output);

    j = 0;
    per (i = 0; i < originalis.mensura; i++)
    {
        c = originalis.datum[i];

        si (est_unreserved(c))
        {
            output[j++] = c;
        }
        alioquin
        {
            output[j++] = '%';
            output[j++] = (i8)hex_characteres[(c >> IV) & 0x0F];
            output[j++] = (i8)hex_characteres[c & 0x0F];
        }
    }

    resultatus.datum = output;
    resultatus.mensura = j;

    redde resultatus;
}


/* ========================================================================
 * FUNCTIONES - DECODIFICATIO
 * ======================================================================== */

chorda
url_decodificare(
    chorda   codificata,
    Piscina* piscina)
{
    chorda resultatus;
    i8*    output;
    i32    i;
    i32    j;
    i8     c;
    s32    high;
    s32    low;

    si (codificata.datum == NIHIL || codificata.mensura <= 0 || piscina == NIHIL)
    {
        resultatus.datum = NIHIL;
        resultatus.mensura = 0;
        redde resultatus;
    }

    /* Output est semper <= input */
    output = piscina_allocare(piscina, codificata.mensura);

    j = 0;
    per (i = 0; i < codificata.mensura; i++)
    {
        c = codificata.datum[i];

        si (c == '%' && i + II < codificata.mensura)
        {
            high = hex_ad_valor(codificata.datum[i + I]);
            low = hex_ad_valor(codificata.datum[i + II]);

            si (high >= 0 && low >= 0)
            {
                output[j++] = (i8)((high << IV) | low);
                i += II;
            }
            alioquin
            {
                /* Invalid escape, keep as-is */
                output[j++] = c;
            }
        }
        alioquin si (c == '+')
        {
            /* Plus -> space (form encoding) */
            output[j++] = ' ';
        }
        alioquin
        {
            output[j++] = c;
        }
    }

    resultatus.datum = output;
    resultatus.mensura = j;

    redde resultatus;
}


/* ========================================================================
 * FUNCTIONES - QUERY STRING
 * ======================================================================== */

chorda
url_params_construere(
    chorda*  claves,
    chorda*  valores,
    i32      numerus,
    Piscina* piscina)
{
    chorda  resultatus;
    chorda* claves_encoded;
    chorda* valores_encoded;
    i32     longitudo_totalis;
    i8*     output;
    i32     i;
    i32     j;
    i32     k;

    si (claves == NIHIL || valores == NIHIL || numerus <= 0 || piscina == NIHIL)
    {
        resultatus.datum = NIHIL;
        resultatus.mensura = 0;
        redde resultatus;
    }

    /* Encode omnes claves et valores */
    claves_encoded = piscina_allocare(piscina, (i32)(magnitudo(chorda)) * numerus);
    valores_encoded = piscina_allocare(piscina, (i32)(magnitudo(chorda)) * numerus);

    per (i = 0; i < numerus; i++)
    {
        claves_encoded[i] = url_codificare(claves[i], piscina);
        valores_encoded[i] = url_codificare(valores[i], piscina);
    }

    /* Calculare longitudo totalis */
    longitudo_totalis = 0;
    per (i = 0; i < numerus; i++)
    {
        longitudo_totalis += claves_encoded[i].mensura;
        longitudo_totalis += I; /* = */
        longitudo_totalis += valores_encoded[i].mensura;
        si (i < numerus - I)
        {
            longitudo_totalis += I; /* & */
        }
    }

    output = piscina_allocare(piscina, longitudo_totalis);

    j = 0;
    per (i = 0; i < numerus; i++)
    {
        /* Clavis */
        per (k = 0; k < claves_encoded[i].mensura; k++)
        {
            output[j++] = claves_encoded[i].datum[k];
        }

        /* = */
        output[j++] = '=';

        /* Valor */
        per (k = 0; k < valores_encoded[i].mensura; k++)
        {
            output[j++] = valores_encoded[i].datum[k];
        }

        /* & */
        si (i < numerus - I)
        {
            output[j++] = '&';
        }
    }

    resultatus.datum = output;
    resultatus.mensura = j;

    redde resultatus;
}

UrlParams
url_params_parse(
    chorda   query_string,
    Piscina* piscina)
{
    UrlParams resultatus;
    i32       numerus_pairs;
    i32       i;
    i32       initium;
    i32       j;
    chorda    par;
    chorda    clavis;
    chorda    valor;
    i32       eq_pos;

    resultatus.claves = NIHIL;
    resultatus.valores = NIHIL;
    resultatus.numerus = 0;

    si (query_string.datum == NIHIL || query_string.mensura <= 0 || piscina == NIHIL)
    {
        redde resultatus;
    }

    /* Numerare pairs (numerus & + 1) */
    numerus_pairs = I;
    per (i = 0; i < query_string.mensura; i++)
    {
        si (query_string.datum[i] == '&')
        {
            numerus_pairs++;
        }
    }

    resultatus.claves = piscina_allocare(piscina, (i32)(magnitudo(chorda)) * numerus_pairs);
    resultatus.valores = piscina_allocare(piscina, (i32)(magnitudo(chorda)) * numerus_pairs);

    /* Parse pairs */
    j = 0;
    initium = 0;
    per (i = 0; i <= query_string.mensura; i++)
    {
        si (i == query_string.mensura || query_string.datum[i] == '&')
        {
            /* Extrahere pair */
            par.datum = query_string.datum + initium;
            par.mensura = i - initium;

            /* Invenire '=' */
            per (eq_pos = 0; eq_pos < par.mensura; eq_pos++)
            {
                si (par.datum[eq_pos] == '=')
                {
                    frange;
                }
            }

            si (eq_pos < par.mensura)
            {
                /* Clavis = ante '=' */
                clavis.datum = par.datum;
                clavis.mensura = eq_pos;

                /* Valor = post '=' */
                valor.datum = par.datum + eq_pos + I;
                valor.mensura = par.mensura - eq_pos - I;
            }
            alioquin
            {
                /* Nullum '=', clavis tantum */
                clavis = par;
                valor.datum = NIHIL;
                valor.mensura = 0;
            }

            /* Decode et store */
            resultatus.claves[j] = url_decodificare(clavis, piscina);
            resultatus.valores[j] = url_decodificare(valor, piscina);
            j++;

            initium = i + I;
        }
    }

    resultatus.numerus = j;

    redde resultatus;
}
