/* carmina_infixum.h - textus carmina.stml infixus (liber ordinarius
 * carminum, include/carmen.h). Corpus lib/carmina_infixum.c GENERATUM
 * est per tools/carmina_infigere.sh - numquam manu. */
#ifndef CARMINA_INFIXUM_H
#define CARMINA_INFIXUM_H

/* <aedilis corpus="lib/carmina_infixum.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

/* copia textus libri in piscina */
chorda
carmina_infixum_textus (
    Piscina* piscina);

#endif /* CARMINA_INFIXUM_H */
