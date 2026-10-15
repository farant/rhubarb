/* membrum__elenchi.c - menu hospitis (include/membrum__elenchi.h). Ex
 * calendario translatum (kalendarium M2c: elenchum_legere, _agere). */
#include "membrum__elenchi.h"

#include <stdio.h>
#include <string.h>

nomen structura {
    constans character* titulus;
    constans character* valor;
} ParCampi;

nomen structura {
    ParCampi* pares;
         i32  numerus;
} GrexCamporum;

interior vacuum
campos_ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
     GrexCamporum* grex;
         ParCampi* par;
              i32  i;

    grex = (GrexCamporum*)ctx;
    per (i = ZEPHYRUM; i < grex->numerus; i++)
    {
        par = grex->pares + i;
        si (!par->valor || !par->valor[ZEPHYRUM])
        {
            (vacuum)insula_attributum_tollere(radix, par->titulus);
            perge;
        }
        insula_attributum_ponere(radix, p, in, par->titulus,
            par->valor);
    }
}

/* campi (elenchus, elenchus_electa) scriptura una, scriptore hospitis;
 * titulus_secundus NIHIL = campus unus */
interior b32
campos_scribere (
            InsulaRamus* ramus,
     constans character* scriptor,
     constans character* titulus_primus,
     constans character* valor_primus,
     constans character* titulus_secundus,
     constans character* valor_secundus,
                Piscina* p)
{
        ParCampi pares[II];
    GrexCamporum grex;
          chorda prior;
             b32 ok;

    pares[ZEPHYRUM].titulus  = titulus_primus;
    pares[ZEPHYRUM].valor    = valor_primus;
    pares[I].titulus         = titulus_secundus;
    pares[I].valor           = valor_secundus;
    grex.pares               = pares;
    grex.numerus             = titulus_secundus ? II : I;
    prior                    = ramus->repo->scriptor;
    insula_scriptorem_ponere(ramus->repo, chorda_ex_literis(scriptor,
        p));
    ok = mutare_ramum(ramus, campos_ponere, &grex);
    insula_scriptorem_ponere(ramus->repo, prior);
    redde ok;
}

b32
elenchi_verificare (
    constans ElenchiIntroitus* introitus,
          constans Praeceptum* praecepta)
{
    constans ElenchiIntroitus* r;

    per (r = introitus; r && r->titulus; r++)
    {
        si (!praecepta_invenire(praecepta, r->titulus))
        {
            redde FALSUM;
        }
    }
    redde introitus != NIHIL;
}

b32
elenchi_aperire (
            InsulaRamus* ramus,
     constans character* scriptor,
                    i32  contextus,
                Punctum  ancora,
                Piscina* piscina)
{
    character valor[XLVIII];

    si (!ramus || !scriptor || !piscina || !contextus)
    {
        redde FALSUM;
    }
    sprintf(valor, "%u %d %d", (insignatus integer)contextus,
        (integer)ancora.x, (integer)ancora.y);
    redde campos_scribere(ramus, scriptor, "elenchus", valor,
        "elenchus_electa", "", piscina);
}

b32
elenchi_claudere (
            InsulaRamus* ramus,
     constans character* scriptor,
                Piscina* piscina)
{
    si (!ramus || !scriptor || !piscina)
    {
        redde FALSUM;
    }
    redde campos_scribere(ramus, scriptor, "elenchus", "",
        "elenchus_electa", "", piscina);
}

b32
elenchi_legere (
      constans InsulaRamus* ramus,
    constans ElenchiHospes* hospes,
                   Piscina* piscina,
                  Elenchus* e,
                   Punctum* ancora)
{
                       chorda* a;
                  ElenchusRes* res;
          constans Praeceptum* t;
    constans ElenchiIntroitus* r;
           insignatus integer  contextus;
                      integer  x;
                      integer  y;
                          s32  electa;
                          i32  n;
                          b32  divisor;

    si (!ramus || !hospes || !piscina || !e || !ancora)
    {
        redde FALSUM;
    }
    a = insula_ramus_attributum(ramus, "elenchus");
    si (   !a || a->mensura == ZEPHYRUM
        || sscanf(chorda_ut_cstr(*a, piscina), "%u %d %d", &contextus,
               &x, &y) != III
        || contextus == ZEPHYRUM)
    {
        redde FALSUM;
    }
    n = ZEPHYRUM;
    per (r = hospes->introitus; r && r->titulus; r++)
    {
        n++;
    }
    res = (ElenchusRes*)piscina_allocare(piscina, magnitudo(ElenchusRes)
        * (n > ZEPHYRUM ? n : I));
    si (!res)
    {
        redde FALSUM;
    }
    e->res      = res;
    e->numerus  = ZEPHYRUM;
    divisor     = FALSUM;
    per (r = hospes->introitus; r && r->titulus; r++)
    {
        si (!(r->contextus & (i32)contextus))
        {
            perge;
        }
        divisor  = divisor || r->divisor;
        t        = praecepta_invenire(hospes->praecepta, r->titulus);
        si (!t || !(t->visus & hospes->visus))
        {
            perge;
        }
        res[e->numerus].titulus  = t->titulus;
        res[e->numerus].textus   = t->descriptio;
        res[e->numerus].clavis   = t->clavis;
        res[e->numerus].habilis  = praecepta_status(t, hospes->visus,
            hospes->requisita, hospes->ctx) == PRAECEPTUM_PRAESTO;
        res[e->numerus].divisor  = divisor && e->numerus > ZEPHYRUM;
        divisor                  = FALSUM;
        e->numerus++;
    }
    a          = insula_ramus_attributum(ramus, "elenchus_electa");
    e->electa  = (a && chorda_ut_s32(*a, &electa) && electa >= ZEPHYRUM
                  && electa < (s32)e->numerus) ? electa : -I;
    ancora->x = (s32)x;
    ancora->y = (s32)y;
    redde VERUM;
}

/* claudere, deinde praeceptum rei agere */
interior vacuum
rem_agere (
               InsulaRamus* ramus,
        constans character* scriptor,
    constans ElenchiHospes* hospes,
         constans Elenchus* e,
                       s32  index,
                   Piscina* piscina)
{
    (vacuum)elenchi_claudere(ramus, scriptor, piscina);
    (vacuum)praecepta_invocare(hospes->praecepta, e->res[index].titulus,
        hospes->visus, hospes->requisita, hospes->ctx);
}

b32
elenchi_agere (
               InsulaRamus* ramus,
        constans character* scriptor,
    constans ElenchiHospes* hospes,
         constans Elenchus* e,
        constans Componens* nodus,
          constans Eventus* ev,
                   Piscina* piscina)
{
               s32 index;
               s32 electa;
    ElenchusExitus exitus;
         character numerus[XVI];

    si (!ramus || !scriptor || !hospes || !e || !ev || !piscina)
    {
        redde FALSUM;
    }
    index = elenchus_index(nodus);
    si (ev->genus == EVENTUS_MUS_MOTUS)
    {
        si (   index < ZEPHYRUM || index >= (s32)e->numerus
            || !e->res[index].habilis || index == e->electa)
        {
            redde index >= ZEPHYRUM;
        }
        sprintf(numerus, "%d", (integer)index);
        redde campos_scribere(ramus, scriptor, "elenchus_electa",
            numerus,
            NIHIL, NIHIL, piscina);
    }
    /* duplex = echo pressionis iam tractatae (menu id aperuit aut
     * clausit): deglutitur */
    si (ev->genus == EVENTUS_MUS_DUPLEX)
    {
        redde VERUM;
    }
    si (ev->genus == EVENTUS_MUS_DEPRESSUS)
    {
        si (index >= ZEPHYRUM && index < (s32)e->numerus)
        {
            si (   e->res[index].habilis
                && ev->datum.mus.botton == MUS_SINISTER)
            {
                rem_agere(ramus, scriptor, hospes, e, index, piscina);
            }
            redde VERUM;
        }
        si (!(nodus && nodus->partes == PARTES_DIALOGUS))
        {
            (vacuum)elenchi_claudere(ramus, scriptor, piscina);
        }
        redde VERUM;
    }
    si (ev->genus == EVENTUS_TEXTUS)
    {
        redde VERUM;
    }
    si (ev->genus != EVENTUS_CLAVIS_DEPRESSUS)
    {
        redde FALSUM;
    }
    si (ev->datum.clavis.clavis == CLAVIS_EFFUGIUM)
    {
        (vacuum)elenchi_claudere(ramus, scriptor, piscina);
        redde VERUM;
    }
    electa = e->electa;
    exitus = elenchus_clavem_agere(e, ev, &electa);
    si (exitus == ELENCHUS_MOTUM)
    {
        sprintf(numerus, "%d", (integer)electa);
        (vacuum)campos_scribere(ramus, scriptor, "elenchus_electa",
            numerus, NIHIL, NIHIL, piscina);
    }
    alioquin si (exitus == ELENCHUS_ELECTUM)
    {
        rem_agere(ramus, scriptor, hospes, e, electa, piscina);
    }
    redde VERUM;
}
