/* norma_canon.h - textus norma.canon infixus (norma-spec-2; regula R1
 * norma-plan-3). Corpus lib/norma_canon.c GENERATUM est per
 * tools/norma_canon_infigere.sh - numquam manu. */
#ifndef NORMA_CANON_H
#define NORMA_CANON_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

/* copia textus canonis in piscina */
chorda
norma_canon_textus (
    Piscina* piscina);

#endif /* NORMA_CANON_H */
