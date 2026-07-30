#ifndef SIMILITUDO_H
#define SIMILITUDO_H

#include "latina.h"
#include "chorda.h"

/* similitudo - congruentia subsequentiae nebulosa (genus fzf) pro
 * identificatoribus: "reticulum iaciendi" cum titulus exactus
 * nescitur (natus 2026-07-10, desideratum primum phasis
 * instrumentorum; consumptores: emitte "fortasse", nexus -similis,
 * saltuarius type-jump olim).
 *
 * Regulae: quaestio subsequentia candidati sit (insensibilis
 * casui) aut non congruit; punctatio limites verborum (initium,
 * post '_' '-' '.' '/' ':' spatium, transitus minusculae ->
 * MAIUSCULAE) et cursus continuos et casum exactum praemiat,
 * hiatus punit. Punctum negativum licet (congruentia sparsa).
 * Functiones purae: nulla piscina, nulla allocatio, nullus
 * status. Ambulatio avara a sinistra - alineatio optima (DP) =
 * dilatio nominata, API immutata maneret. */

nomen structura {
    i32 index;      /* index candidati in acie data */
    s32 punctum;
} SimilitudoFructus;

/* VERUM si quaestio candidato congruit; *punctum_exitus tum
 * scriptum (maius = similius). Quaestio vacua OMNIBUS congruit
 * puncto ZEPHYRUM. */
b32
similitudo_punctare (
    chorda  quaestio,
    chorda  candidatus,
       s32* punctum_exitus);

/* Optimos candidatos punctat et ordinat (punctum descendens, tum
 * brevior candidatus, tum index minor). Fructus <= maxima
 * scribuntur; numerus scriptorum redditur. */
i32
similitudo_optima (
    chorda             quaestio,
    constans chorda*   candidati,
    i32                numerus_candidatorum,
    SimilitudoFructus* fructus,
    i32                maxima);

/* Ut optima, sed quaestione a dextra decurtata donec candidati
 * appareant (pavimentum III codicilli - quaestio brevior numquam
 * temptatur): nomen totum male coniectum subsequentia nullius est,
 * cauda substituta quaestionem totam necat - quod homo in fzf
 * delendo facit. Promota regula duorum 2026-07-30 (ansae manu
 * scriptae in emitte + nexus; legatus tertius consumptor). */
i32
similitudo_optima_decurtata (
    chorda             quaestio,
    constans chorda*   candidati,
    i32                numerus_candidatorum,
    SimilitudoFructus* fructus,
    i32                maxima);

#endif /* SIMILITUDO_H */
