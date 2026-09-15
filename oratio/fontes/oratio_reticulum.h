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
 * -CATENA, -CONDICIO, -INFIMUM, -TEGIT, -DUPLEX, -GREX, -GREX-SORS,
 * quisque genus cum ordine COLUMNAE suo. Recusatio nominata (iudicatum
 * FALSUM, causa 'reticulum: ...') numquam relatio vacua.
 * T36 c (decisio LVI): ordines TESTIMONII per gregem (-greges) et per
 * gregem et sortem - materia tabulae ponderum; sortes ALTERNAE
 * (-sortes-alternae N) corpus quodque tenent.
 * T36 b (decisio LIV): tabula INFIMORUM k columnarum (-profunditas,
 * pro paribus), catena SEMINATA (-initium), gradus catenae liberae
 * sortibus cadentibus = candidatus CONDICIONATUS; columnae CONSTANTES
 * (grex unus) extra reticulum et infima.
 * Plana: project-specs/partitio-reticulum-plan.md, -t36-plan.md.
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
     constans character* greges;      /* NIHIL, titulus, catena,
                                         catena-libera, initium */
                    b32  machina;
                    Xar* initium;     /* chorda: columnae seminis */
                    i32  profunditas; /* infima II..IV; ordinarie II */
                    i32  prima;       /* infima per profunditatem, XX */
                    i32  alternae;    /* sortes alternae N (0 = nullae):
                                         sors = index ordinis mod N;
                                         -sortes tum omissa (T36 c) */
} OratioReticulumOptiones;

nomen structura {
       b32 iudicatum;    /* FALSUM = nihil iudicatum */
    chorda causa;        /* si !iudicatum */
    chorda relatio;      /* textus humanus aut ordines machinae */
} OratioReticulumExitus;

/* optiones ordinariae: limen XX, profunditas II, prima XX, listae
 * vacuae (ex piscina) */
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
