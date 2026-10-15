/* membrum__praecepta.c - tabula praeceptorum (include/
 * membrum__praecepta.h). Formae clavium et verificatio ex calendario
 * translatae (kalendarium M1, "tabula mandatorum"). */
#include "membrum__praecepta.h"

#include <string.h>

b32
praecepta_verificare (
    constans Praeceptum* tabula)
{
    constans Praeceptum* a;
    constans Praeceptum* b;

    si (!tabula)
    {
        redde FALSUM;
    }
    per (a = tabula; a->titulus; a++)
    {
        si (!a->descriptio || !a->fn || !a->visus)
        {
            redde FALSUM;
        }
        per (b = a + I; b->titulus; b++)
        {
            /* titulus unicus (invocare); clavis semel per visum */
            si (   strcmp(a->titulus, b->titulus) == ZEPHYRUM
                || (   a->clavis && b->clavis && (a->visus & b->visus)
                    && strcmp(a->clavis, b->clavis) == ZEPHYRUM))
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

constans Praeceptum*
praecepta_invenire (
    constans Praeceptum* tabula,
     constans character* titulus)
{
    constans Praeceptum* t;

    per (t = tabula; t && titulus && t->titulus; t++)
    {
        si (strcmp(t->titulus, titulus) == ZEPHYRUM)
        {
            redde t;
        }
    }
    redde NIHIL;
}

b32
praecepta_clavis_congruit (
    constans character* forma,
      constans Eventus* ev)
{
    i32 mod;

    si (!forma || !ev || ev->genus != EVENTUS_CLAVIS_DEPRESSUS)
    {
        redde FALSUM;
    }
    mod = ev->datum.clavis.modificantes;
    si (strcmp(forma, "Enter") == ZEPHYRUM)
    {
        redde ev->datum.clavis.clavis == CLAVIS_REDITUS;
    }
    si (strcmp(forma, "Esc") == ZEPHYRUM)
    {
        redde ev->datum.clavis.clavis == CLAVIS_EFFUGIUM;
    }
    si (forma[ZEPHYRUM] == 'C' && forma[I] == '-')
    {
        redde (mod & (i32)MOD_IMPERIUM)
            && ev->datum.clavis.runa == (s32)(insignatus character)
            forma[II];
    }
    redde !(mod & (i32)(MOD_IMPERIUM | MOD_SUPER))
        && ev->datum.clavis.producta == (s32)(insignatus character)
        forma[ZEPHYRUM];
}

constans Praeceptum*
praecepta_ad_clavem (
    constans Praeceptum* tabula,
                    i32  visus,
       constans Eventus* ev)
{
    constans Praeceptum* t;

    per (t = tabula; t && t->titulus; t++)
    {
        si (   (t->visus & visus) && t->clavis
            && praecepta_clavis_congruit(t->clavis, ev))
        {
            redde t;
        }
    }
    redde NIHIL;
}

PraeceptumStatus
praecepta_status (
    constans Praeceptum* p,
                    i32  visus,
     PraeceptaRequisita  requisita,
                 vacuum* ctx)
{
    si (!p || !(p->visus & visus))
    {
        redde PRAECEPTUM_ABSENS;
    }
    si (requisita && p->requisita && !requisita(p->requisita, ctx))
    {
        redde PRAECEPTUM_OBSCURUM;
    }
    redde PRAECEPTUM_PRAESTO;
}

b32
praecepta_invocare (
    constans Praeceptum* tabula,
     constans character* titulus,
                    i32  visus,
     PraeceptaRequisita  requisita,
                 vacuum* ctx)
{
    constans Praeceptum* t;

    t = praecepta_invenire(tabula, titulus);
    redde t && praecepta_status(t, visus, requisita, ctx)
        == PRAECEPTUM_PRAESTO && t->fn(ctx);
}
