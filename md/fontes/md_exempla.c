/* md_exempla.c - Vide md_exempla.h. */

#include "md_exempla.h"
#include "md_lexema.h"
#include <string.h>

#define LIMES "````````````````````````````````"   /* XXXII graves */

interior b32
_linea_est (
    constans character* fons,
      constans MdLinea* l,
    constans character* litterae)
{
    i32 n = (i32)strlen(litterae);

    redde (b32)(l->mensura == n
                && memcmp(fons + l->offset, litterae, (size_t)n)
                    == ZEPHYRUM);
}

/* Linea praefixo incipit, deinde finis aut spatium (GFM: 'example table') */
interior b32
_praefixum_est (
    constans character* fons,
      constans MdLinea* l,
    constans character* litterae)
{
    i32 n = (i32)strlen(litterae);

    si (   l->mensura < n
        || memcmp(fons + l->offset, litterae, (size_t)n) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde (b32)(l->mensura == n || fons[l->offset + (s32)n] == ' ');
}

/* Lineae [a, b) copiare, quaeque cum '\n', '→' (E2 86 92) -> '\t' */
interior chorda
_copiare (
                Piscina* piscina,
     constans character* fons,
      constans MdLineae* lineae,
                    i32  a,
                    i32  b)
{
       chorda  c;
          i32  capacitas = ZEPHYRUM;
          i32  k;
    character* d;
          i32  n = ZEPHYRUM;
    unio { character* c; i8* m; } u;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    per (k = a; k < b; k++)
    {
        capacitas = capacitas + md_linea(lineae, k)->mensura + I;
    }
    d = (character*)piscina_allocare(piscina, (memoriae_index)capacitas
        + I);
    si (d == NIHIL)
    {
        redde c;
    }
    per (k = a; k < b; k++)
    {
        constans MdLinea* l   = md_linea(lineae, k);
                     s32  i   = l->offset;
                     s32  ad  = l->offset + (s32)l->mensura;

        dum (i < ad)
        {
            si (   i + II < ad && (insignatus character)fons[i] == 0xE2
                && (insignatus character)fons[i + I]  == 0x86
                && (insignatus character)fons[i + II] == 0x92)
            {
                d[n]  = '\t';
                n     = n + I;
                i     = i + III;
                perge;
            }
            d[n]  = fons[i];
            n     = n + I;
            i     = i + I;
        }
        d[n]  = '\n';
        n     = n + I;
    }
    u.c        = d;
    c.datum    = u.m;
    c.mensura  = n;
    redde c;
}

Xar*
md_exempla_legere (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
    MdLineae  lineae;
         Xar* exitus;
         i32  n;
         i32  i;
      chorda  sectio;
         i32  numerus = ZEPHYRUM;

    si (!md_lineae_scindere(piscina, fons, mensura, &lineae))
    {
        redde NIHIL;
    }
    exitus = xar_creare(piscina, (i32)magnitudo(MdExemplum));
    si (exitus == NIHIL)
    {
        redde NIHIL;
    }
    sectio  = md_chorda_fontis(fons, ZEPHYRUM, ZEPHYRUM);
    n       = md_lineae_numerus(&lineae);
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans MdLinea* l = md_linea(&lineae, i);

        si (   l->mensura > III && fons[l->offset] == '#'
            && (fons[l->offset + I] == ' '
                || (fons[l->offset + I] == '#'
                    && fons[l->offset + II] == ' ')))
        {
            s32 ab = l->offset + ((fons[l->offset + I]
                == '#') ? III : II);

            sectio = md_chorda_fontis(fons, ab, l->offset
                + (s32)l->mensura);
            perge;
        }
        si (_praefixum_est(fons, l, LIMES " example"))
        {
                   i32  a        = i + I;
                   s32  punctum  = (s32)-I;
                   s32  clausum  = (s32)-I;
                   i32  k;
            MdExemplum* e;

            per (k = a; k < n; k++)
            {
                constans MdLinea* m = md_linea(&lineae, k);

                si (punctum < ZEPHYRUM && _linea_est(fons, m, "."))
                {
                    punctum = (s32)k;
                }
                alioquin si (_linea_est(fons, m, LIMES))
                {
                    clausum = (s32)k;
                    frange;
                }
            }
            si (punctum < ZEPHYRUM || clausum < ZEPHYRUM)
            {
                frange;   /* limes sine clausura: reliquum omittitur */
            }
            e = (MdExemplum*)xar_addere(exitus);
            si (e == NIHIL)
            {
                redde NIHIL;
            }
            numerus     = numerus + I;
            e->numerus  = numerus;
            e->sectio   = sectio;
            e->linea    = l->numerus;
            e->extensio = md_chorda_fontis(fons,
                l->offset + (s32)strlen(LIMES " example")
                    + ((l->mensura
                    > (i32)strlen(LIMES " example")) ? I : ZEPHYRUM),
                l->offset + (s32)l->mensura);
            e->markdown = _copiare(piscina, fons, &lineae, a,
                (i32)punctum);
            e->html     = _copiare(piscina, fons, &lineae, (i32)punctum
                + I,
                (i32)clausum);
            si (   (e->markdown.datum == NIHIL && (i32)punctum > a)
                || (e->html.datum == NIHIL && clausum > punctum + I))
            {
                redde NIHIL;
            }
            i = (i32)clausum;
        }
    }
    redde exitus;
}
