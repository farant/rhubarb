/* briar_proiectio.h - Proiectio STML documenti thistle ut textus, sine
 * stml.h in capite: consumptor qui silvam quoque tenet (tools/briar.c)
 * stml.h et silva.h in eadem unitate includere non potest (caput
 * amalgamae enumerationes stml suas fert - vide briar_silva.h).
 * Implementatio in briar_proiectio.c latus stml tenet.
 */

#ifndef BRIAR_PROIECTIO_H
#define BRIAR_PROIECTIO_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "materia_nodus.h"

/* textus STML proiectionis (consilium briar); mensura 0 + *causa si
 * fracta (lexicon recusatum, scriptura fracta) */
chorda
briar_proiectionem_scribere (
                  Piscina*  piscina,
    constans MateriaNodus*  radix,
       constans character** causa);

#endif /* BRIAR_PROIECTIO_H */
