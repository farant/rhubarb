/* sancti_canon.h - textus sancti.canon infixus (project-specs/
 * sancti-plan.md par. II). Corpus lib/sancti_canon.c GENERATUM est per
 * tools/sancti_canon_infigere.sh - numquam manu. */
#ifndef SANCTI_CANON_H
#define SANCTI_CANON_H

/* <aedilis corpus="lib/sancti_canon.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

/* copia textus canonis in piscina */
chorda
sancti_canon_textus (
    Piscina* piscina);

#endif /* SANCTI_CANON_H */
