/* saltuarius_columnae.h - Visus columnarum molendinarii (Phase A)
 *
 * Descriptio scaenae PASSIVA + functio pingendi. Limes disci
 * tabularia et textus praevisus implet (principale/probationes);
 * pingere solum legit et cellas ponit - NIHIL per quadrum
 * allocat (primitivum: runae directe in cellas, sine buffer
 * intermedio; praecisio ad limitem panni et purgatio octetorum
 * imperii in UNA via aedificata).
 *
 * Ratio pannorum 1:3:4 (ranger). Ordo 0 = via, ordines medii =
 * contentum, ordo ultimus = status.
 */

#ifndef SALTUARIUS_COLUMNAE_H
#define SALTUARIUS_COLUMNAE_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tessera.h"
#include "saltuarius_tabularium.h"
#include "saltuarius_res.h"

nomen enumeratio {
    SALT_PRAEVISUS_NIHIL = 0,
    SALT_PRAEVISUS_DIRECTORIUM,  /* praevisus_tab pingitur */
    SALT_PRAEVISUS_TEXTUS,       /* praevisus_textus pingitur */
    SALT_PRAEVISUS_NUNTIUS       /* praevisus_nuntius (celatum) */
} SaltuariusPraevisusGenus;

nomen structura {
    SaltuariusTabularium*    parens;    /* NIHIL ad radicem "/" */
    SaltuariusTabularium*    currens;
    SaltuariusPraevisusGenus praevisus_genus;
    SaltuariusTabularium*    praevisus_tab;
    chorda                   praevisus_textus;
    chorda                   praevisus_nuntius;
} SaltuariusColumnae;

/* Mensurae dispositionis - publicae quia clicus (A3) pannum ex
 * coordinatis invenire debet et probationes mathesin figunt */
nomen structura {
    s32 parens_x;
    s32 parens_lat;
    s32 currens_x;
    s32 currens_lat;
    s32 praevisus_x;
    s32 praevisus_lat;
    s32 contentum_y;    /* primus ordo contenti */
    s32 contentum_alt;  /* ordines contento disponibiles */
} SaltuariusMetra;

/* Creare descriptionem scaenae vacuam */
SaltuariusColumnae*
saltuarius_columnae_creare (Piscina* piscina);

/* Dispositionem computare pro dimensionibus datis */
vacuum
saltuarius_columnae_metiri (i32 latitudo, i32 altitudo,
    SaltuariusMetra* metra_out);

/* Scaenam integram pingere (purgat, pannos, separatores, viam,
 * statum; praesentare NON vocat - principale/probationes id
 * faciunt aut omittunt). Tabularia aptantur (volumen) hic. */
vacuum
saltuarius_columnae_pingere (SaltuariusColumnae* columnae,
    constans SaltuariusRes* res, TesseraOpus* opus);

#endif /* SALTUARIUS_COLUMNAE_H */
