#ifndef PARTITIO_AESTIMATIO_H
#define PARTITIO_AESTIMATIO_H

#include "latina.h"
#include "piscina.h"
#include "partitio.h"


/* ==================================================
 * Aestimatio partitionis RETENTA (T35 b, oratio-spec par. 2
 * decisiones XLIX-LIII, par. 7 "Design - T35")
 *
 * Tres partitiones eorundem ordinum: candidata p, aurum (responsa)
 * et sortes (corpora). Pro sorte s retenta numeri disciplinae =
 * totales minus sors s (UNUM corpus relinquitur).
 *
 * - basis sortis = aureus cum plurimis ordinibus disciplinae;
 *   par -> index aurei minimus;
 * - grex SUFFRAGIUM fert si ordines disciplinae >= limen: aureum
 *   disciplinae maximum stricte; par -> basis sortis;
 * - ordo sortis s gregis sine suffragio basi respondetur et inter
 *   inaestimatos numeratur;
 * - recti = responsa aurea; basis = recti basis solius.
 * Summa per ordines (sortes singulae semper redduntur). Numeri
 * integri soli - permille, numquam fluitantes.
 * ================================================== */

nomen structura {
    i32 ordines;
    i32 recti;
    i32 basis;         /* recti responso basis solo */
    i32 inaestimati;   /* ordines quorum grex suffragium non tulit */
} AestimatioSortis;

nomen structura {
                 i32  numerus_gregum;     /* candidatae */
                 i32  numerus_aureorum;
                 i32  numerus_sortium;
    AestimatioSortis* sortes;             /* numerus_sortium */
    AestimatioSortis  summa;
                 s32  lucrum;             /* recti - basis */
                 i32  puritas_intra;      /* permille, in specimine */
                 i32  greges_mutabiles;   /* sortes diversae */
                 b32  inaestimabilis;     /* 2*inaestimati > ordines */
                 i32* suffragia;          /* [g*S+s]; A = nullum */
                 i32* numeri;             /* [g*A+a] ordines */
} PartitioAestimatio;

/* NIHIL si partitio NIHIL, ordines inaequales, aut memoria deficit */
PartitioAestimatio*
partitio_aestimare (
               Piscina* piscina,
     constans Partitio* p,
     constans Partitio* aurum,
     constans Partitio* sortes,
                   i32  limen);


/* ==================================================
 * Catena avida
 *
 * Ab partitione una: gradu quoque infimum catenae cum columna
 * quaque nondum adhibita aestimatur (in regione piscinae notata,
 * numeri soli servantur), ordine rectorum descendente (par: index
 * minimus). Accipitur prima quae (a) lucrum >= lucrum_minimum
 * ordinibus super gradum priorem fert et (b) sortibus vetantibus
 * nullam sortem ordine perdit contra gradum priorem. Nulla -> finis.
 * Gradus quisque priorem refinat: catena accepta = ordo regressus
 * candidatus. Interactionem puram (columnae singulae sine lucro)
 * NON videt - tabula parium instrumenti eam ostendit.
 * ================================================== */

nomen structura {
     PartitioAestimatio*  initium;         /* partitio una */
                    i32   numerus;         /* gradus accepti */
                    i32*  columnae;        /* index columnae gradus */
               Partitio** partes;          /* infimum cumulatum */
     PartitioAestimatio** aestimationes;
} PartitioCatena;

PartitioCatena*
partitio_catenam_struere (
                       Piscina* piscina,
    constans Partitio* constans* columnae,
                            i32 k,
             constans Partitio* aurum,
             constans Partitio* sortes,
                            i32 limen,
                            i32 lucrum_minimum,
                            b32 sortes_vetant);

#endif /* PARTITIO_AESTIMATIO_H */
