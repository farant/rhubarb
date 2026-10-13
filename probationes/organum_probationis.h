/* organum_probationis.h - canon et domini organi EX MANIFESTO
 * (status-plan S2/S6) pro probationibus quae insulas suas ipsae struunt
 * (numeros dominorum aut insulam manu factam asserunt -
 * organum_ramum_parare ramum quoque initiat). Manifestum per
 * organum_legere_viam cum basi communi. Viae relativae ad radicem
 * repositorii (probationes inde currunt). */
#ifndef ORGANUM_PROBATIONIS_H
#define ORGANUM_PROBATIONIS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include "canon.h"
#include "organum.h"
#include "organum_montatio.h"

/* canon insulae et <domini> generati ex manifesto viae; FALSUM si
 * manifestum deest aut malum */
interior b32
organum_probationis_parare (
                Piscina*  piscina,
    InternamentumChorda*  intern,
     constans character*  via,
                  Canon** canon,
              StmlNodus** domini)
{
    Organum* o;
     chorda  causa;

    o = organum_legere_viam(piscina, intern, NIHIL, via, "probatio");
    si (!o)
    {
        redde FALSUM;
    }
    causa.mensura = ZEPHYRUM;
    causa.datum = NIHIL;
    *canon = organum_canonem_generare(o, piscina, intern, &causa);
    *domini = organum_dominos_generare(o, piscina, intern);
    redde *canon && *domini;
}

#endif /* ORGANUM_PROBATIONIS_H */
