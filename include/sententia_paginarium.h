/* sententia_paginarium.h - "Paginatio Sententiarum"
 *
 * Bibliotheca pro paginatione textuum per sententias
 *
 * PHILOSOPHIA:
 * - In modo prosa: unitas est sententia
 * - In modo versus: unitas est linea (propter enjambment)
 * - Paragraph breaks sunt unitates vacuae
 * - Word-wrapping fit per unitatem
 *
 * USUS:
 *   SententiaPaginariumResultus* res = sententia_paginarium_creare(
 *       textus, latitudo, altitudo, piscina);
 *   SententiaPagina* pag = sententia_paginarium_pagina_obtinere(res, index);
 *   // Reddere lineas paginae...
 */

#ifndef SENTENTIA_PAGINARIUM_H
#define SENTENTIA_PAGINARIUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "paginarium.h"

/* ========================================================================
 * TYPI
 * ======================================================================== */

/* SententiaCopia - Una "unitas sententiae"
 *
 * Potest esse:
 * - Sententia vera (ex prosa)
 * - Linea versus (ex poesi)
 * - Paragraph break (vacua)
 */
nomen structura {
    i32 initium;         /* Byte offset initii in textu fonte */
    i32 finis;           /* Byte offset finis (exclusivus) */
    b32 est_versus;      /* Verum si ex sectione versus */
    b32 est_vacua;       /* Verum si paragraph break */
} SententiaCopia;

/* SententiaPagina - Word-wrapped lineas pro una sententia
 *
 * Continet array de PaginariumLinea pro reddere sententiam
 */
nomen structura {
    Xar* lineae;         /* Array de PaginariumLinea */
    i32  initium;        /* Byte offset initii sententiae */
    i32  finis;          /* Byte offset finis sententiae */
    b32  est_versus;     /* Modus versus pro hac sententia */
} SententiaPagina;

/* SententiaPaginariumResultus - Resultus totalis
 */
nomen structura {
    Xar*     copiae;              /* Array de SententiaCopia */
    Xar*     paginae;             /* Array de SententiaPagina (lazy) */
    i32      numerus_sententiarum;
    chorda   textus;              /* Textus originalis */
    Piscina* piscina;
    i32      cache_latitudo;      /* Pro invalidatione */
    i32      cache_altitudo;
} SententiaPaginariumResultus;

/* ========================================================================
 * FUNCTIONES - CREATIO
 * ======================================================================== */

/* Creare resultus paginationis sententiarum
 *
 * Processus totum textum, detegit paragraphos, versus/prosa,
 * et extrahit sententias vel lineas ut unitates.
 *
 * textus:   Textus fonte
 * latitudo: Characteres per lineam (pro word-wrapping)
 * altitudo: Lineae maximae per pagina (pro truncatione)
 * piscina:  Arena pro allocationibus
 *
 * Redde: Resultus (NIHIL si error)
 */
SententiaPaginariumResultus*
sententia_paginarium_creare(
    chorda   textus,
    i32      latitudo,
    i32      altitudo,
    Piscina* piscina);

/* ========================================================================
 * FUNCTIONES - ACCESSUS
 * ======================================================================== */

/* Obtinere numerum sententiarum totalem
 */
i32
sententia_paginarium_numerus(
    SententiaPaginariumResultus* resultus);

/* Obtinere copiam (unitatem) specificam
 *
 * Redde: Pointer ad SententiaCopia, vel NIHIL si index invalidus
 */
SententiaCopia*
sententia_paginarium_copia_obtinere(
    SententiaPaginariumResultus* resultus,
    i32                          index);

/* Obtinere paginam word-wrapped pro sententia
 *
 * Lazy: creat paginam si non in cache
 *
 * Redde: Pointer ad SententiaPagina, vel NIHIL si index invalidus
 */
SententiaPagina*
sententia_paginarium_pagina_obtinere(
    SententiaPaginariumResultus* resultus,
    i32                          index);

/* ========================================================================
 * FUNCTIONES - REDDITIO
 * ======================================================================== */

/* Reddere lineam sententiae ad chordam
 *
 * Similis ad paginarium_linea_reddere sed pro SententiaPagina
 */
chorda
sententia_paginarium_linea_reddere(
    SententiaPaginariumResultus* resultus,
    PaginariumLinea*             linea);

#endif /* SENTENTIA_PAGINARIUM_H */
