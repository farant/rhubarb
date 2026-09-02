/* officina_regio.h - Regio: memoria possessa machinulae (M2a)
 *
 * DECISUS (officina-m2-interview.md Q9/Q10 + diagnostica):
 * UNA reservatio mmap magna cum BASI FIXA (determinismus: cursus
 * identici -> monstratores identici -> vestigia differibilia; later
 * primus itineris-temporis v2). Paginae pigre committuntur (macOS
 * nativum) - reservatio gratis, pretium physicum = tactum solum.
 *
 * Monstratores veri SUNT monstratores machinulae (DECISUS 6 -
 * probatio_piscina identitatem monstratorum et ordinationem XVI
 * per valores veros asserit; ansae fictae corpus fallerent).
 *
 * TRES AREAE intra reservationem:
 *   globalia - collocatio conexionis (data, descriptores, cellae
 *              externae); cumulus simplex, numquam liberatur
 *   stiva    - arcae + tabulae registrorum machinulae (cursor
 *              machinulae est; regio fines solum praebet)
 *   acervus  - pons malloc/free/realloc (aedificata); allocator
 *              indicis liberi, bases >=XVI ordinatae SEMPER
 *              (piscina %XVI == 0 per valorem verum asserit),
 *              memoria INDETERMINATA manet (numquam zephyrum -
 *              custodia venenum recognoscibile ponit, non zerum)
 *
 * CUSTODIA (vexillum diagnosticum): venenum 0xAA in allocatione,
 * 0xDD in liberatione; probationes validitatis in liberare semper
 * (viles). Machinula regio_continet ad custodes legere/scribere
 * adhibet.
 *
 * UNA regio uno tempore (basis fixa) - figura, non defectus.
 */

#ifndef OFFICINA_REGIO_H
#define OFFICINA_REGIO_H

#include "latina.h"
#include "piscina.h"

nomen structura Regio Regio;


/* ==================================================
 * Vita
 * ================================================== */

/* Reservat aream ad basim fixam; NIHIL si basis non data est
 * (numquam basim aliam tacite accipit - determinismus ante
 * commodum). Structura moderaminis ex piscina. */
Regio*
regio_generare (
    Piscina* piscina);
vacuum
regio_destruere (
    Regio* regio);


/* ==================================================
 * Fines + custodia
 * ================================================== */

vacuum*
regio_basis (
    constans Regio* regio);
memoriae_index
regio_magnitudo_tota (
    constans Regio* regio);
b32
regio_continet (
     constans Regio* regio,
    constans vacuum* locus);

vacuum
regio_custodiam_ponere (
    Regio* regio,
      b32  custodia);
b32
regio_custodia (
    constans Regio* regio);


/* ==================================================
 * Globalia (collocatio conexionis; cumulus, sine liberatione)
 * ================================================== */

/* ordinatio = potestas duorum (0 -> I); NIHIL si spatium deest */
vacuum*
regio_globalia_allocare (
             Regio* regio,
    memoriae_index  magnitudo_octetorum,
    memoriae_index  ordinatio);
memoriae_index
regio_globalia_usus (
    constans Regio* regio);


/* ==================================================
 * Stiva (fines pro machinula)
 * ================================================== */

vacuum*
regio_stiva_initium (
    constans Regio* regio);
memoriae_index
regio_stiva_magnitudo_octetorum (
    constans Regio* regio);


/* ==================================================
 * Acervus (pons malloc/free/realloc)
 * ================================================== */

/* n rotundatur ad XVI; fructus semper XVI-ordinatus; NIHIL si
 * spatium deest aut n == 0 */
vacuum*
regio_allocare (
             Regio* regio,
    memoriae_index  n);

/* FALSUM = monstrator alienus / iam liber / caput corruptum -
 * machinula halitum honestum ex hoc facit */
b32
regio_liberare (
     Regio* regio,
    vacuum* locus);

/* locus NIHIL -> allocare; contractio -> idem monstrator;
 * amplificatio -> novus + copia + liberatio veteris */
vacuum*
regio_reallocare (
             Regio* regio,
            vacuum* locus,
    memoriae_index  n);

/* census (numeratores diagnosticorum) */
memoriae_index
regio_acervus_usus (
    constans Regio* regio);
memoriae_index
regio_acervus_apex (
    constans Regio* regio);
memoriae_index
regio_numerus_allocationum (
    constans Regio* regio);
memoriae_index
regio_numerus_liberationum (
    constans Regio* regio);

#endif /* OFFICINA_REGIO_H */
