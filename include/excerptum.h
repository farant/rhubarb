/* excerptum.h - Excerptum fontis cum signo sub tractu (forma
 * diagnosticorum compilatorum): margo numeri lineae, linea fontis
 * (CR sublato, moderatores praeter tabulam ut spatium), deinde
 * '^' ad initium et '~' per characterem UTF-8 usque ad finem
 * tractus aut lineae. Tabula in margine sub linea servatur, ergo
 * signum columnam veram tenet.
 *
 * PICTURA sola: sedem materia dat (MateriaTractus), formam haec.
 * Ergo nihil de arboribus scit et a quolibet fonte vocari potest.
 */
#ifndef EXCERPTUM_H
#define EXCERPTUM_H

#include "latina.h"
#include "chorda_aedificator.h"

/* FALSUM si initium extra [0, mensura] aut linea ZEPHYRUM. */
b32
excerptum_scribere (
     ChordaAedificator* exitus,
    constans character* fons,
                   i32  mensura,
                   s32  initium,
                   s32  finis,
                   i32  linea);

#endif /* EXCERPTUM_H */
