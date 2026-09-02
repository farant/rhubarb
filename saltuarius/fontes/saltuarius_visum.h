/* saltuarius_visum.h - Visus fontis (Phase B: stratum 0 coloratum)
 *
 * Scaena plena: cunula (numeri linearum), pannus textus coloratus
 * per tabulam classium libri (lectio tabulae = lectura tabulae -
 * nulla logica lexica in pictore), cursor verus terminalis
 * (tessera_cursorem_ponere), linea status (via | linea:columna |
 * stratum 0 | pittacium). Volumina libri hic aptantur.
 */

#ifndef SALTUARIUS_VISUM_H
#define SALTUARIUS_VISUM_H

#include "latina.h"
#include "tessera.h"
#include "saltuarius_liber.h"
#include "saltuarius_res.h"
#include "saltuarius_origo.h"
#include "saltuarius_structura.h"
#include "saltuarius_quaestio.h"

/* Metra visus - publica pro clico (B3) et probationibus */
nomen structura {
    s32 cunula_lat;     /* digiti numerorum linearum */
    s32 textus_x;       /* cunula + spatium */
    s32 textus_lat;
    s32 contentum_alt;  /* ordines textus (status demptus) */
} SaltuariusVisumMetra;

vacuum
saltuarius_visum_metiri (
    constans SaltuariusLiber* liber,
                         i32  latitudo,
                         i32  altitudo,
        SaltuariusVisumMetra* metra_out);

/* Scaenam pingere (purgat; praesentare non vocat) */
vacuum
saltuarius_visum_pingere (
           SaltuariusLiber* liber,
    constans SaltuariusRes* res,
               TesseraOpus* opus);

/* Tabellam originis pingere SUPER scaenam (post pingere vocanda
 * si origo->apertum; ordo picturae = ordo z - tessera_replere
 * interior opacum facit, casus rectanguli promissus ad 1.1) */
vacuum
saltuarius_visum_tabella (
    constans SaltuariusOrigo* origo,
                 TesseraOpus* opus);

/* Indicem structurae pingere SUPER scaenam: tabula dextra tota
 * altitudine (linea status relicta). Non constans: volumen per
 * aptare sequitur selectionem (exemplar liber_aptare in pingere).
 * NB "index" non "structura" - macro latina (= struct)! */
vacuum
saltuarius_visum_structura (
    SaltuariusStructura* index,
            TesseraOpus* opus);

/* Lineam echo quaestionis pingere SUPER lineam status (ULTIMA
 * pingitur; latus dextrum status dum scribitur cedit - consulto,
 * regula prioritatis B2 lineae NORMALI applicatur, non echo) */
vacuum
saltuarius_visum_quaestio (
    constans SaltuariusQuaestio* quaestio,
                    TesseraOpus* opus);

/* Lineam telemetriae (F2) SUPER lineam status: fructus tesserae
 * (cumulativi - praesentationes, mutatae, octeti, ms MEDIUM) +
 * arena silvae summa/apex MB (telemetria additionum I) + libri
 * N/8 (instrumentum observationis LRU). Totam lineam capit
 * (visus diagnosticus; exemplar echo quaestionis). */
vacuum
saltuarius_visum_fructus (
    constans SaltuariusLiber* liber,
                         i32  libri_numerus,
                 TesseraOpus* opus);

#endif /* SALTUARIUS_VISUM_H */
