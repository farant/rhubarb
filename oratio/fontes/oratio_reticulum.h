/* oratio_reticulum.h - Instrumentum reticuli partitionum (T35 d)
 *
 * Ordines machinae cum ordine COLUMNAE (decisio LI) per TITULUM
 * leguntur: columna quaeque nota partitio ordinum est, aurum
 * (praefixum 'aurum-', decisio LII) et sortes (corpora) item.
 * Bibliotheca partitio aestimat: unum corpus relinquitur (decisio
 * XLIX), catena avida sortibus vetantibus et sine veto (decisio L),
 * paria, reticulum columnarum; inaestimabilis nominatur (decisio LIII).
 *
 * Relatio humana aut ordines machinae RETICULUM-COLUMNA, -SORS,
 * -CATENA, -PAR, -TEGIT, -DUPLEX, quisque genus cum ordine COLUMNAE
 * suo. Recusatio nominata (iudicatum FALSUM, causa 'reticulum: ...')
 * numquam relatio vacua.
 * Planum: project-specs/partitio-reticulum-plan.md.
 */

#ifndef ORATIO_RETICULUM_H
#define ORATIO_RETICULUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

/* -ubi titulus=v1,v2: ordo manet si valor columnae inter valores;
 * condiciones plures omnes tenendae */
nomen structura {
     chorda  titulus;
     chorda* valores;
        i32  numerus;
} OratioReticulumUbi;

nomen structura {
     constans character* genus;       /* -genus */
     constans character* aurum;       /* -aurum (praefixum aurum-) */
     constans character* sortes;      /* -sortes */
                    Xar* ubi;         /* OratioReticulumUbi */
                    Xar* columnae;    /* chorda; vacua = omnes */
                    Xar* praeter;     /* chorda */
                    Xar* gradus;      /* chorda: 0 | 1 | plures */
                    i32  limen;       /* ordinarie XX */
                    i32  lucrum;      /* 0 = limen */
     constans character* greges;      /* NIHIL, titulus, aut "catena" */
                    b32  machina;
} OratioReticulumOptiones;

nomen structura {
       b32 iudicatum;    /* FALSUM = nihil iudicatum */
    chorda causa;        /* si !iudicatum */
    chorda relatio;      /* textus humanus aut ordines machinae */
} OratioReticulumExitus;

/* optiones ordinariae: limen XX, listae vacuae (ex piscina) */
vacuum
oratio_reticulum_optiones_initium (
                    Piscina* piscina,
    OratioReticulumOptiones* optiones);

OratioReticulumExitus
oratio_reticulum_currere (
                             Piscina* piscina,
                              chorda  tsv,
                  constans character* via,
    constans OratioReticulumOptiones* optiones);

#endif /* ORATIO_RETICULUM_H */
