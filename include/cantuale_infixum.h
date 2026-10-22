/* cantuale_infixum.h - textus cantuale.stml infixus (liber ordinarius
 * cantilenarum, include/cantuale.h). Corpus lib/cantuale_infixum.c
 * GENERATUM est per tools/cantuale_infigere.sh - numquam manu. */
#ifndef CANTUALE_INFIXUM_H
#define CANTUALE_INFIXUM_H

/* <aedilis corpus="lib/cantuale_infixum.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

/* copia textus libri in piscina */
chorda
cantuale_infixum_textus (
    Piscina* piscina);

#endif /* CANTUALE_INFIXUM_H */
