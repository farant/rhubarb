/* organa_infixa.h - manifesta organorum infixa (status-plan S7c):
 * apps/<genus>/organum.stml et apps/organum_commune.stml in
 * bibliotheca, ut omnis consumptor (fenestra in ~/.bin, vicus,
 * probationes) eadem legat sine arbore. Corpus lib/organa_infixa.c
 * GENERATUM est per tools/organa_infigere.sh - numquam manu; porta
 * generata eum cum fontibus comparat. */
#ifndef ORGANA_INFIXA_H
#define ORGANA_INFIXA_H

/* <aedilis corpus="lib/organa_infixa.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

/* textus manifesti generis (copia in piscina); mensura ZEPHYRUM si
 * genus ignotum. "organum_commune" = basis. */
chorda
organum_infixum_textus (
    constans character* genus,
               Piscina* piscina);

#endif /* ORGANA_INFIXA_H */
