/* excerptum.c - Vide excerptum.h. */

#include "excerptum.h"
#include "utf8.h"

/* margo: figurae (minimum IV latitudinis) aut spatia, deinde " | " */
interior vacuum
_marginem (
    ChordaAedificator* exitus,
                  i32  linea,
                  b32  numerus)
{
    character figurae[XVI];
          i32 n = ZEPHYRUM;
          i32 latitudo;
          i32 k;
          i32 valor = linea;

    fac
    {
        figurae[n]  = (character)('0' + (valor % X));
        valor       = valor / X;
        n++;
    } dum (valor > ZEPHYRUM && n < XV);
    latitudo = n < IV ? (i32)IV : n;
    per (k = ZEPHYRUM; k < latitudo - (numerus ? n : ZEPHYRUM); k++)
    {
        chorda_aedificator_appendere_character(exitus, ' ');
    }
    si (numerus)
    {
        per (k = n; k > ZEPHYRUM; k--)
        {
            chorda_aedificator_appendere_character(exitus,
                figurae[k - I]);
        }
    }
    chorda_aedificator_appendere_literis(exitus, " | ");
}

b32
excerptum_scribere (
     ChordaAedificator* exitus,
    constans character* fons,
                   i32  mensura,
                   s32  initium,
                   s32  finis,
                   i32  linea)
{
    s32 principium;
    s32 terminus;
    s32 finis_lineae;
    s32 i;

    si (   exitus == NIHIL || fons == NIHIL || linea == ZEPHYRUM
        || initium < ZEPHYRUM || initium > (s32)mensura)
    {
        redde FALSUM;
    }
    si (finis < initium)
    {
        finis = initium;
    }
    principium = initium;
    dum (principium > ZEPHYRUM && fons[principium - I] != '\n')
    {
        principium--;
    }
    terminus = initium;
    dum (terminus < (s32)mensura && fons[terminus] != '\n')
    {
        terminus++;
    }
    si (terminus > principium && fons[terminus - I] == '\r')
    {
        terminus--;
    }
    _marginem(exitus, linea, VERUM);
    per (i = principium; i < terminus; i++)
    {
        character c = fons[i];

        si (c != '\t' && (i32)(insignatus character)c < 0x20)
        {
            c = ' ';
        }
        chorda_aedificator_appendere_character(exitus, c);
    }
    chorda_aedificator_appendere_character(exitus, '\n');
    _marginem(exitus, linea, FALSUM);
    per (i = principium; i < initium && i < terminus; i++)
    {
        si (fons[i] == '\t')
        {
            chorda_aedificator_appendere_character(exitus, '\t');
        }
        alioquin si (!utf8_est_continuatio((i8)fons[i]))
        {
            chorda_aedificator_appendere_character(exitus, ' ');
        }
    }
    chorda_aedificator_appendere_character(exitus, '^');
    finis_lineae  = finis < terminus ? finis : terminus;
    i             = initium;
    si (i < finis_lineae)
    {
        i++;
        dum (i < finis_lineae && utf8_est_continuatio((i8)fons[i]))
        {
            i++;
        }
    }
    per (; i < finis_lineae; i++)
    {
        si (!utf8_est_continuatio((i8)fons[i]))
        {
            chorda_aedificator_appendere_character(exitus, '~');
        }
    }
    chorda_aedificator_appendere_character(exitus, '\n');
    redde VERUM;
}
