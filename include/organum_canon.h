/* organum_canon.h - textus organum.canon infixus (status-plan S1).
 * Corpus lib/organum_canon.c GENERATUM est per
 * tools/organum_canon_infigere.sh - numquam manu. */
#ifndef ORGANUM_CANON_H
#define ORGANUM_CANON_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

/* copia textus canonis in piscina */
chorda
organum_canon_textus (
    Piscina* piscina);

#endif /* ORGANUM_CANON_H */
