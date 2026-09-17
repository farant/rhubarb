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

/* Sedes una in serie sedium multiplicium. */
nomen structura {
                    s32  initium;
                    s32  finis;
                    i32  linea;
     constans character* nota;      /* NIHIL licet */
} ExcerptumSedes;

/* Sedes plures in saepto UNO: ordine fontis, margo semel mensuratus
 * ex linea maxima, linea fontis quaeque SEMEL impressa cum signo uno
 * per sedem infra eam. Sine elisione: linea quaeque numerum suum
 * fert, ergo hiatus numquam ambiguus est.
 *
 * Sedes ORDINATAE esse debent (initium non decrescens); aliter
 * FALSUM. Recusatio clara, non ordinatio tacita: series CONSTANS
 * est, ergo eam hic ordinare sine memoria non possumus, et exitus
 * tacite permutatus peior est quam refutatio nominata.
 *
 * FALSUM etiam si numerus ZEPHYRUM, aut sedes ulla lineam ZEPHYRUM
 * aut initium extra [0, mensura] fert.
 *
 * Cum numero I et nota NIHIL exitus OCTETIM idem est ac
 * excerptum_scribere super eandem sedem. */
b32
excerptum_scribere_multa (
          ChordaAedificator* exitus,
         constans character* fons,
                        i32  mensura,
    constans ExcerptumSedes* sedes,
                        i32  numerus);

#endif /* EXCERPTUM_H */
