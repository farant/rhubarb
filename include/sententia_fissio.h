#ifndef SENTENTIA_FISSIO_H
#define SENTENTIA_FISSIO_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"

/* ==================================================
 * SENTENTIA FISSIO
 * Bibliotheca pro findere textum in sententias
 *
 * Tractat:
 * - Abbreviationes (Mr., Dr., etc.)
 * - Quotes rectas et curvas (Unicode)
 * - Numeros decimales ($3.50)
 * - Ellipsis (...)
 * - Attributiones dialogi ("Hello!" she said.)
 * - Numeros Romanos (George II.)
 * ================================================== */


/* ==================================================
 * Structura Fructus - Array sententiarum (copiae)
 * ================================================== */

nomen structura {
    chorda* sententiae;   /* Array chordarum sententiarum (copiatae) */
    i32     numerus;      /* Numerus sententiarum */
} SententiaeFissioFructus;


/* ==================================================
 * Structura Indices - Byte offsets sententiarum
 * ================================================== */

nomen structura {
    i32 initium;          /* Byte offset initii sententiae */
    i32 finis;            /* Byte offset finis sententiae (exclusivus) */
} SententiaIndices;

nomen structura {
    SententiaIndices* indices;   /* Array indicum */
    i32               numerus;   /* Numerus sententiarum */
} SententiaIndicesFructus;


/* ==================================================
 * Iterator pro fissione pigra
 * ================================================== */

nomen structura SententiaIterator SententiaIterator;


/* ==================================================
 * API Principalis
 * ================================================== */

/* Findere textum in omnes sententias simul (copiae)
 *
 * textus:  textus findendus
 * piscina: piscina pro allocare
 *
 * Redde: structura cum array sententiarum et numero
 */
SententiaeFissioFructus
sententia_fissio(
    chorda   textus,
    Piscina* piscina);


/* Numerare sententias sine plena allocazione
 *
 * textus: textus numerandus
 *
 * Redde: numerus sententiarum
 */
i32
sententia_numerare(
    chorda textus);


/* Findere textum in indices sententiarum (non copiae)
 *
 * textus:  textus findendus
 * piscina: piscina pro allocare
 *
 * Redde: structura cum array indicum et numero
 */
SententiaIndicesFructus
sententia_fissio_indices(
    chorda   textus,
    Piscina* piscina);


/* ==================================================
 * API Iterator (fissio pigra cum cache)
 * ================================================== */

/* Creare iterator pro transgressu pigro sententiarum
 *
 * textus:  textus findendus
 * piscina: piscina pro allocare
 *
 * Redde: iterator novus (NIHIL si error)
 */
SententiaIterator*
sententia_iterator_creare(
    chorda   textus,
    Piscina* piscina);


/* Obtinere proximam sententiam
 *
 * iter: iterator
 *
 * Redde: proxima sententia (chorda vacua si ad finem)
 */
chorda
sententia_iterator_proxima(
    SententiaIterator* iter);


/* Obtinere priorem sententiam
 *
 * iter: iterator
 *
 * Redde: prior sententia (chorda vacua si ad initium)
 */
chorda
sententia_iterator_prior(
    SententiaIterator* iter);


/* Obtinere indicem currentem (0-basis)
 *
 * iter: iterator
 *
 * Redde: index currens
 */
i32
sententia_iterator_index(
    SententiaIterator* iter);


/* Obtinere numerum totalem (cogit plenam analysim si non in cache)
 *
 * iter: iterator
 *
 * Redde: numerus totalis sententiarum
 */
i32
sententia_iterator_numerus(
    SententiaIterator* iter);


/* Resettare ad initium
 *
 * iter: iterator
 */
vacuum
sententia_iterator_reset(
    SententiaIterator* iter);


#endif /* SENTENTIA_FISSIO_H */
