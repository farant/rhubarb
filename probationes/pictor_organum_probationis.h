/* pictor_organum_probationis.h - canon et domini pictoris EX
 * MANIFESTO (status-plan S2) pro probationibus quae insulas suas
 * ipsae struunt (numeros dominorum asserunt - organum_ramum_parare
 * ramum quoque initiat): manifestum per organum_legere_viam (S6a),
 * ut pictor_montare. Viae relativae ad
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
#include "organum_montatio.h"
#include "organum_probationis.h"

/* canon insulae et <domini> generati; FALSUM si manifestum deest aut
 * malum */
interior b32
pictor_organum_parare (
                Piscina*  piscina,
    InternamentumChorda*  intern,
                  Canon** canon,
              StmlNodus** domini)
{
    redde organum_probationis_parare(piscina, intern,
        "apps/pictor/organum.stml", canon, domini);
}

#endif /* PICTOR_ORGANUM_PROBATIONIS_H */
