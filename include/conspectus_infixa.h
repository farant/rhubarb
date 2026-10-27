/* conspectus_infixa.h - declarationes lib/conspectus_*.stml infixae
 * (proiectiones D4b). Corpus lib/conspectus_infixa.c GENERATUM est per
 * tools/conspectus_infigere.sh - numquam manu. */
#ifndef CONSPECTUS_INFIXA_H
#define CONSPECTUS_INFIXA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

/* textus declarationis lib/conspectus_<titulus>.stml in piscina;
 * vacua si ignota */
chorda
conspectus_declaratio_infixa (
    constans character* titulus,
               Piscina* piscina);

#endif /* CONSPECTUS_INFIXA_H */
