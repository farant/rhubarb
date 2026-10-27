/* conspectus_canon.h - textus conspectus.canon infixus (proiectiones
 * D4a). Corpus lib/conspectus_canon.c GENERATUM est per
 * tools/conspectus_canon_infigere.sh - numquam manu. */
#ifndef CONSPECTUS_CANON_H
#define CONSPECTUS_CANON_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

/* copia textus canonis in piscina */
chorda
conspectus_canon_textus (
    Piscina* piscina);

#endif /* CONSPECTUS_CANON_H */
