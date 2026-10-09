/* iussum.c - iussa in textu (vicus-latera S3a). Ratio in capite. */

#include "iussum.h"

#include <string.h>


/* ==================================================
 * Auxilia
 * ================================================== */

/* cellula (coordinatae signatae, iam intra tabulam probatae) */
interior character
cellula (
    constans TabulaCharacterum* t,
                           s32  linea,
                           s32  columna)
{
    redde t->cellulae[(i32)linea * t->latitudo + (i32)columna];
}

interior b32
verbi_character (
    character c)
{
    redde (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9') || c == '_';
}

interior b32
verbi_initium (
    character c)
{
    redde c >= 'a' && c <= 'z';
}

interior b32
verbi_sequens (
    character c)
{
    redde (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_';
}

/* cellula vacua ('\0') ut spatium */
interior b32
spatium_est (
    character c)
{
    redde c == ' ' || c == '\0' || c == '\t';
}

/* cellulae [a, b) lineae, spatiis extremis demptis, in piscinam */
interior chorda
segmentum (
    constans TabulaCharacterum* t,
                           s32  linea,
                           s32  a,
                           s32  b,
                       Piscina* piscina)
{
    chorda c;

    dum (a < b && spatium_est(cellula(t, linea, a)))
    {
        a++;
    }
    dum (b > a && spatium_est(cellula(t, linea, b - I)))
    {
        b--;
    }
    c.mensura  = (i32)(b - a);
    c.datum    = NIHIL;
    si (c.mensura > ZEPHYRUM)
    {
        c.datum = (i8*)piscina_allocare(piscina,
            (memoriae_index)c.mensura);
        memcpy(c.datum, t->cellulae + (i32)linea * t->latitudo + (i32)a,
            (memoriae_index)c.mensura);
    }
    redde c;
}

/* iussum cuius '$' in columna c legere; FALSUM si non iussum */
interior b32
legere (
    constans TabulaCharacterum* t,
                           s32  linea,
                           s32  c,
                   IussumNotum  notum,
                        vacuum* ctx,
                       Piscina* piscina,
                        Iussum* exitus)
{
    s32 latitudo;
    s32 j;
    s32 k;
    s32 a;
    i32 n;
    i32 i;

    latitudo = (s32)t->latitudo;
    si (cellula(t, linea, c) != '$')
    {
        redde FALSUM;
    }
    si (c > ZEPHYRUM && verbi_character(cellula(t, linea, c - I)))
    {
        redde FALSUM;
    }
    j = c + I;
    si (j >= latitudo || !verbi_initium(cellula(t, linea, j)))
    {
        redde FALSUM;
    }
    dum (j < latitudo && verbi_sequens(cellula(t, linea, j)))
    {
        j++;
    }
    exitus->linea    = linea;
    exitus->initium  = c;
    exitus->finis    = j;
    exitus->verbum = segmentum(t, linea, c + I, j,
        piscina);
    exitus->numerus_argumentorum  = ZEPHYRUM;
    exitus->argumenta             = NIHIL;
    si (notum && !notum(exitus->verbum, ctx))
    {
        redde FALSUM;
    }
    si (j >= latitudo || cellula(t, linea, j) != '(')
    {
        redde VERUM;
    }
    /* argumenta: ')' prima in eadem linea, aliter non iussum */
    k = j + I;
    dum (k < latitudo && cellula(t, linea, k) != ')')
    {
        k++;
    }
    si (k >= latitudo)
    {
        redde FALSUM;
    }
    exitus->finis = k + I;
    si (segmentum(t, linea, j + I, k, piscina).mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    n = I;
    per (a = j + I; a < k; a++)
    {
        si (cellula(t, linea, a) == ',')
        {
            n++;
        }
    }
    exitus->argumenta = (chorda*)piscina_allocare(piscina,
        (memoriae_index)n * magnitudo(chorda));
    exitus->numerus_argumentorum  = n;
    a                             = j + I;
    per (i = ZEPHYRUM; i < n; i++)
    {
        s32 b;

        b = a;
        dum (b < k && cellula(t, linea, b) != ',')
        {
            b++;
        }
        exitus->argumenta[i]  = segmentum(t, linea, a, b, piscina);
        a                     = b + I;
    }
    redde VERUM;
}


/* ==================================================
 * Interfacies
 * ================================================== */

b32
iussum_proximum (
    constans TabulaCharacterum* t,
                           s32  linea,
                           s32  a_columna,
                   IussumNotum  notum,
                        vacuum* ctx,
                       Piscina* piscina,
                        Iussum* exitus)
{
    s32 c;

    si (   !t || !piscina || !exitus || linea < ZEPHYRUM
        || linea >= (s32)t->altitudo)
    {
        redde FALSUM;
    }
    per (c = a_columna < ZEPHYRUM ? ZEPHYRUM : a_columna;
         c < (s32)t->latitudo; c++)
    {
        si (legere(t, linea, c, notum, ctx, piscina, exitus))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

b32
iussum_ad_locum (
    constans TabulaCharacterum* t,
                           s32  linea,
                           s32  columna,
                   IussumNotum  notum,
                        vacuum* ctx,
                       Piscina* piscina,
                        Iussum* exitus)
{
    s32 a;

    si (!t || columna < ZEPHYRUM || columna >= (s32)t->latitudo)
    {
        redde FALSUM;
    }
    a = ZEPHYRUM;
    dum (iussum_proximum(t, linea, a, notum, ctx, piscina, exitus))
    {
        si (exitus->initium > columna)
        {
            redde FALSUM;
        }
        si (columna < exitus->finis)
        {
            redde VERUM;
        }
        a = exitus->finis;
    }
    redde FALSUM;
}
