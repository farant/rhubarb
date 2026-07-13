/* vindex_onerator.h - onerator mundi APP-LATERIS (M3 chunk 6)
 *
 * EXTRACTIO oneratoris (consumptor quartus): systema ISO+POSIX +
 * capita praeparata + demissio per-TU (lineae + INDICIUM collecta
 * in fenestra collige-ante-destrue) + nexus + machinula, super
 * superficies PUBLICAS solas (officina.h + silva.h). Familiae
 * oneratorum DUAE: fontes-latus (cursor/interpretare/vectis ⑤ -
 * typi domus, exemplaria propria) et APP-latus (vindex + sessio).
 * INTERLUDIUM POST-M4b (2026-07-13): praeparatio contextus (systema/
 * capita/bis-analysis) in praeparator.{h,c} MIGRATA - familia
 * app-lateris consolidata; migratio fontes-lateris = tactu proximo
 * cuiusque.
 *
 * C89 vanilla - hospites amalgamata consumunt, latina absens. */

#ifndef VINDEX_ONERATOR_H
#define VINDEX_ONERATOR_H

#include "officina.h"
#include "silva.h"

typedef struct VindexOnerator VindexOnerator;

/* Praeparat systema + capita (ambulatio radicis). NULL si systema
 * deest (curre ex radice repositorii aut da radicem veram). */
VindexOnerator* vindex_onerator_creare(const char* radix);

/* Demittit unam plagulam .c (via RELATIVA radici - eadem chorda
 * fit via indicii/linearum). Colligit lineas + indicium. */
int vindex_onerator_demittere(VindexOnerator* onerator,
    const char* via);

/* Nectit mundum, machinulam creat, lineas praebet, indicium
 * scribit ad viam datam et RECLUDIT. NULL si nexus fractus. */
Machinula* vindex_onerator_nectere(VindexOnerator* onerator,
    const char* via_indicii);

/* Post nectere: lector plagulae indicii (proprietas oneratoris). */
IndiciumLector* vindex_onerator_lector(VindexOnerator* onerator);

/* Symbolum nexus insolutum novissimum (diagnosis nexus fracti). */
OfficinaChorda vindex_onerator_querela(VindexOnerator* onerator);

#endif /* VINDEX_ONERATOR_H */
