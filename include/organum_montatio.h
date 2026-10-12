/* organum_montatio.h - organum in ramo montare (status-plan S6):
 * manifestum (via) + basis communis, canon generatus, elementum
 * initiale (absentes + addita), restitutio (conservatio), domini,
 * registratio in systematibus. Ligatio documenti (relatio) organi
 * propria manet - documentum eius solum novit. */

#ifndef ORGANUM_MONTATIO_H
#define ORGANUM_MONTATIO_H

/* <aedilis corpus="lib/organum_montatio.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "insula.h"
#include "organum.h"
#include "conservatio.h"
#include "relatio.h"
#include "dimissio.h"

/* systemata status repositorii (status-plan S3-S5): NIHIL in campo =
 * systema absens; NIHIL totum = nulla (probationes) */
nomen structura {
    Conservatio* conservatio;
        Relatio* relatio;
       Dimissio* dimissio;
} StatusSystemata;

/* manifestum ex radice+via cum basi apps/organum_commune.stml;
 * NIHIL si deest aut malum (vitia in stderr, praefixo dato) */
Organum*
organum_legere_viam (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* radix,
     constans character* via,
     constans character* praefixum);

/* ramum parare: canon, initium (absentes + attributa_addita), status
 * servatus (clavis), domini, dimissio. FALSUM si canon aut
 * initiatio deficit. systemata NIHIL = nulla. Ramus radicis (elementum
 * vacuum): canon radicis repositorii fit (montationes canonibus suis
 * iudicantur), initium attributa absentia sola ponit. */
b32
organum_ramum_parare (
        constans Organum* organum,
    constans InsulaRamus* ramus,
         StatusSystemata* systemata,
      constans character* attributa_addita,
      constans character* clavis,
                 Piscina* piscina,
     InternamentumChorda* intern);

#endif /* ORGANUM_MONTATIO_H */
