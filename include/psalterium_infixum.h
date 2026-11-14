/* psalterium_infixum.h - psalmi I-CL (musica/psalmi/N.txt, textus
 * punctatus, include/psalmodia.h) et licentia eorum (musica/psalmi/
 * LICENTIA, MIT Divini Officii) infixi: vicus et binaria sine
 * plagulis repositorii vivunt (spec project-specs/psalterium-spec.md
 * D5, IV). Corpus lib/psalterium_infixum.c GENERATUM est per
 * tools/psalterium_infigere.sh - numquam manu. */
#ifndef PSALTERIUM_INFIXUM_H
#define PSALTERIUM_INFIXUM_H

/* <aedilis corpus="lib/psalterium_infixum.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

/* copia textus psalmi numerus (I-CL, Vulgata) in piscina, octetis
 * aequalis plagulae; mensura 0 extra I-CL aut si memoria deficit */
chorda
psalterium_infixum_psalmus (
         i32  numerus,
     Piscina* piscina);

/* copia licentiae (notitia MIT servanda) in piscina */
chorda
psalterium_infixum_licentia (
    Piscina* piscina);

#endif /* PSALTERIUM_INFIXUM_H */
