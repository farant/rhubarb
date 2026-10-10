/* pictor_organum_probationis.h - canon et domini pictoris EX
 * MANIFESTO (status-plan S2) pro probationibus quae insulas suas
 * ipsae struunt: apps/pictor/organum.stml + basis
 * apps/organum_commune.stml, ut pictor_montare. Viae relativae ad
 * radicem repositorii (probationes inde currunt). */
#ifndef PICTOR_ORGANUM_PROBATIONIS_H
#define PICTOR_ORGANUM_PROBATIONIS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "internamentum.h"
#include "stml.h"
#include "canon.h"
#include "organum.h"

/* canon insulae et <domini> generati; FALSUM si manifestum deest aut
 * malum */
interior b32
pictor_organum_parare (
                Piscina*  piscina,
    InternamentumChorda*  intern,
                  Canon** canon,
              StmlNodus** domini)
{
    Organum* o;
    Organum* basis;
        Xar* vitia;
     chorda  causa;

    o = organum_legere(filum_legere_totum("apps/pictor/organum.stml",
        piscina), piscina, intern, &vitia);
    basis = organum_legere(filum_legere_totum(
        "apps/organum_commune.stml", piscina), piscina, intern, &vitia);
    si (!o || !basis || !organum_basim_addere(o, basis))
    {
        redde FALSUM;
    }
    causa.mensura = ZEPHYRUM;
    causa.datum = NIHIL;
    *canon = organum_canonem_generare(o, piscina, intern, &causa);
    *domini = organum_dominos_generare(o, piscina, intern);
    redde *canon && *domini;
}

#endif /* PICTOR_ORGANUM_PROBATIONIS_H */
