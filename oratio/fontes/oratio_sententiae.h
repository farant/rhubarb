/* oratio_sententiae.h - Extenta sententiarum ex arbore orationis (T6)
 *
 * Sententia = [octetus primus elementi primi, post octetum ultimum
 * PARTIUM elementi ultimi) - cauda (spatium post) EXCLUSA, ut extenta
 * cum sententia_fissio_indices (quae spatia finalia non fert)
 * conferri possint. Consumptores: oratio/sententiae.sh, porta
 * fissionis, Prosa.sententia (gradus III).
 */

#ifndef ORATIO_SENTENTIAE_H
#define ORATIO_SENTENTIAE_H

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "materia_nodus.h"

nomen structura {
    s32 initium;
    s32 finis;
    i32 linea;      /* lexematis primi */
    i32 elementa;   /* numerus elementorum */
} OratioSententia;

/* Xar de OratioSententia ordine documenti; NIHIL = memoria */
Xar*
oratio_sententiae_extenta (
                  Piscina* piscina,
    constans MateriaNodus* radix);

#endif /* ORATIO_SENTENTIAE_H */
