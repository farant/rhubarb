/* rubrica.c - rubricae communes (include/rubrica.h); regulae ex
 * agenda.c translatae (A1), rubricam_habet ex kalendarium.c */
#include "rubrica.h"

#include <string.h>

b32
rubrica_valida (
    chorda c)
{
    i32 i;

    si (c.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        si (!(   (c.datum[i] >= 'a' && c.datum[i] <= 'z')
              || (c.datum[i] >= '0' && c.datum[i] <= '9')
              || c.datum[i] == '-'))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

s32
rubricas_comparare (
    chorda a,
    chorda b)
{
    i32 n;
    s32 r;

    n = a.mensura < b.mensura ? a.mensura : b.mensura;
    r = n > ZEPHYRUM ? (s32)memcmp(a.datum, b.datum, (size_t)n)
                     : ZEPHYRUM;
    si (r != ZEPHYRUM)
    {
        redde r;
    }
    redde (s32)a.mensura - (s32)b.mensura;
}

Xar*
rubricas_legere (
     chorda  textus,
    Piscina* piscina)
{
       Xar* l;
    chorda  r;
    chorda* x;
       i32  i;
       i32  j;
       i32  k;

    l = xar_creare(piscina, (i32)magnitudo(chorda));
    si (!l)
    {
        redde NIHIL;
    }
    i = ZEPHYRUM;
    dum (i < textus.mensura)
    {
        dum (i < textus.mensura && textus.datum[i] == ' ')
        {
            i++;
        }
        j = i;
        dum (j < textus.mensura && textus.datum[j] != ' ')
        {
            j++;
        }
        si (j == i)
        {
            frange;
        }
        r.datum    = textus.datum + i;
        r.mensura  = j - i;
        i          = j;
        si (!rubrica_valida(r))
        {
            redde NIHIL;
        }
        /* insertio ordinata, iterata praeteruntur */
        k = ZEPHYRUM;
        dum (   k < xar_numerus(l)
             && rubricas_comparare(*(chorda*)xar_obtinere(l, k), r)
                < ZEPHYRUM)
        {
            k++;
        }
        si (   k < xar_numerus(l)
            && rubricas_comparare(*(chorda*)xar_obtinere(l, k), r)
               == ZEPHYRUM)
        {
            perge;
        }
        (vacuum)xar_addere(l);
        per (j = xar_numerus(l) - I; j > k; j--)
        {
            x   = (chorda*)xar_obtinere(l, j);
            *x  = *(chorda*)xar_obtinere(l, j - I);
        }
        *(chorda*)xar_obtinere(l, k) = r;
    }
    redde l;
}

chorda
rubricas_iungere (
    constans Xar* copia,
         Piscina* piscina)
{
    chorda s;
       i32 i;

    s.datum    = NIHIL;
    s.mensura  = ZEPHYRUM;
    per (i = ZEPHYRUM; copia && i < xar_numerus(copia); i++)
    {
        si (i > ZEPHYRUM)
        {
            s = chorda_concatenare(s, chorda_ex_literis(" ", piscina),
                piscina);
        }
        s = chorda_concatenare(s, *(chorda*)xar_obtinere(copia, i),
            piscina);
    }
    redde s;
}

b32
rubricam_habet (
     constans Xar* copia,
           chorda  rubrica)
{
    i32 i;

    per (i = ZEPHYRUM; copia && i < xar_numerus(copia); i++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(copia, i), rubrica))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}
