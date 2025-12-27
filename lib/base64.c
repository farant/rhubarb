/* base64.c - Base64 Codificatio et Decodificatio
 *
 * Implementatio RFC 4648 Base64.
 */

#include "base64.h"
#include "piscina.h"

#include <string.h>


/* ========================================================================
 * TABULAE
 * ======================================================================== */

/* Alphabetum Base64 standard */
hic_manens constans character*
alphabetum = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/* Tabula decodificationis (character -> valor 0-63, -1 = invalidus) */
hic_manens constans s8 tabula_decodificationis[CXXVIII] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1
};


/* ========================================================================
 * FUNCTIONES - UTILITAS
 * ======================================================================== */

i32
base64_longitudo_codificata(i32 mensura_originalis)
{
    /* Formula: 4 * ceil(n / 3) */
    si (mensura_originalis <= 0)
    {
        redde 0;
    }

    redde ((mensura_originalis + II) / III) * IV;
}

i32
base64_longitudo_decodificata(chorda codificata)
{
    i32 longitudo;
    i32 padding;

    si (codificata.mensura <= 0 || codificata.datum == NIHIL)
    {
        redde 0;
    }

    longitudo = codificata.mensura;
    padding = 0;

    /* Numerare padding characters '=' */
    si (longitudo >= I && codificata.datum[longitudo - I] == '=')
    {
        padding++;
    }
    si (longitudo >= II && codificata.datum[longitudo - II] == '=')
    {
        padding++;
    }

    /* Formula: (longitudo / 4) * 3 - padding */
    redde (longitudo / IV) * III - padding;
}


/* ========================================================================
 * FUNCTIONES - CODIFICATIO
 * ======================================================================== */

chorda
base64_codificare(
    constans i8* datum,
    i32          mensura,
    Piscina*     piscina)
{
    chorda   resultatus;
    i32      longitudo_output;
    i8*      output;
    i32      i;
    i32      j;
    i8       a, b, c;
    i32      triplex;

    si (datum == NIHIL || mensura <= 0 || piscina == NIHIL)
    {
        resultatus.datum = NIHIL;
        resultatus.mensura = 0;
        redde resultatus;
    }

    longitudo_output = base64_longitudo_codificata(mensura);
    output = piscina_allocare(piscina, longitudo_output);

    j = 0;
    per (i = 0; i < mensura; i += III)
    {
        /* Legere 3 bytes (cum zero padding si necessarium) */
        a = datum[i];
        b = (i + I < mensura) ? datum[i + I] : 0;
        c = (i + II < mensura) ? datum[i + II] : 0;

        /* Combinare in 24-bit integer */
        triplex = ((i32)(a & 0xFF) << XVI) | ((i32)(b & 0xFF) << VIII) | (i32)(c & 0xFF);

        /* Extrahere 4 x 6-bit valores */
        output[j++] = (i8)alphabetum[(triplex >> XVIII) & 0x3F];
        output[j++] = (i8)alphabetum[(triplex >> XII) & 0x3F];
        output[j++] = (i8)alphabetum[(triplex >> VI) & 0x3F];
        output[j++] = (i8)alphabetum[triplex & 0x3F];
    }

    /* Addere padding '=' */
    si (mensura % III == I)
    {
        output[j - I] = '=';
        output[j - II] = '=';
    }
    alioquin si (mensura % III == II)
    {
        output[j - I] = '=';
    }

    resultatus.datum = output;
    resultatus.mensura = longitudo_output;

    redde resultatus;
}


/* ========================================================================
 * FUNCTIONES - DECODIFICATIO
 * ======================================================================== */

Base64Fructus
base64_decodificare(
    chorda   codificata,
    Piscina* piscina)
{
    Base64Fructus resultatus;
    i32           longitudo_output;
    i8*           output;
    i32           i;
    i32           j;
    i8            a, b, c, d;
    i32           quadruplex;
    s8            val;

    resultatus.datum = NIHIL;
    resultatus.mensura = 0;

    si (codificata.datum == NIHIL || codificata.mensura <= 0 || piscina == NIHIL)
    {
        redde resultatus;
    }

    /* Verificare longitudo est multiplex IV */
    si (codificata.mensura % IV != 0)
    {
        redde resultatus;
    }

    longitudo_output = base64_longitudo_decodificata(codificata);
    output = piscina_allocare(piscina, longitudo_output);

    j = 0;
    per (i = 0; i < codificata.mensura; i += IV)
    {
        /* Legere 4 characteres */
        a = codificata.datum[i];
        b = codificata.datum[i + I];
        c = codificata.datum[i + II];
        d = codificata.datum[i + III];

        /* Convertere ad valores (ignorare padding) */
        val = (a >= 0 && a < CXXVIII) ? tabula_decodificationis[(integer)a] : -1;
        si (val < 0)
        {
            resultatus.datum = NIHIL;
            resultatus.mensura = 0;
            redde resultatus;
        }
        quadruplex = (i32)val << XVIII;

        val = (b >= 0 && b < CXXVIII) ? tabula_decodificationis[(integer)b] : -1;
        si (val < 0)
        {
            resultatus.datum = NIHIL;
            resultatus.mensura = 0;
            redde resultatus;
        }
        quadruplex |= (i32)val << XII;

        si (c != '=')
        {
            val = (c >= 0 && c < CXXVIII) ? tabula_decodificationis[(integer)c] : -1;
            si (val < 0)
            {
                resultatus.datum = NIHIL;
                resultatus.mensura = 0;
                redde resultatus;
            }
            quadruplex |= (i32)val << VI;
        }

        si (d != '=')
        {
            val = (d >= 0 && d < CXXVIII) ? tabula_decodificationis[(integer)d] : -1;
            si (val < 0)
            {
                resultatus.datum = NIHIL;
                resultatus.mensura = 0;
                redde resultatus;
            }
            quadruplex |= (i32)val;
        }

        /* Extrahere 3 bytes */
        si (j < longitudo_output)
        {
            output[j++] = (i8)((quadruplex >> XVI) & 0xFF);
        }
        si (j < longitudo_output)
        {
            output[j++] = (i8)((quadruplex >> VIII) & 0xFF);
        }
        si (j < longitudo_output)
        {
            output[j++] = (i8)(quadruplex & 0xFF);
        }
    }

    resultatus.datum = output;
    resultatus.mensura = longitudo_output;

    redde resultatus;
}
