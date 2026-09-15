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

/* HABITUS gregis (T36 a, decisio LVI - profilus decisionis XL ad
 * folliculos): coactus = sors quaeque suffragatur unanimis (margo ==
 * disciplina); ordinatus = sors quaeque suffragatur margine > 0 et idem
 * ubique; apertus aliter - sors sine suffragio, PAR disciplinae
 * (suffragium basis est, non gregis), suffragia discordia */
nomen enumeratio {
    PARTITIO_HABITUS_COACTUS,
    PARTITIO_HABITUS_ORDINATUS,
    PARTITIO_HABITUS_APERTUS
} PartitioHabitus;
#define PARTITIO_HABITUS_NUMERUS III
externus constans character* constans
    PARTITIO_TITULI_HABITUUM[PARTITIO_HABITUS_NUMERUS];

nomen structura {
                 i32  numerus_gregum;     /* candidatae */
                 i32  numerus_aureorum;
                 i32  numerus_sortium;
    AestimatioSortis* sortes;             /* numerus_sortium */
    AestimatioSortis  summa;
                 s32  lucrum;             /* recti - basis */
                 i32  puritas_intra;      /* permille, in specimine */
                 i32  greges_mutabiles;   /* sortes diversae */
                 b32  inaestimabilis;     /* 2*inaestimati > ordines
                                             aut captivus >= limen */
                 i32* suffragia;          /* [g*S+s]; A = nullum */
                 i32* numeri;             /* [g*A+a] ordines */
    /* T36 a (decisiones LV, LVI): grex CAPTIVUS = ordines omnes in
     * sorte una (iudicari nequit ea relicta); testimonium per gregem
     * et sortem pro tabula ponderum */
                 i32* captivus;           /* [g]: sors; S = nulla */
                 i32  greges_captivi;
                 i32  ordines_captivi;
                 i32* retenti;            /* [g*S+s] ordines sortis */
                 i32* recti_gregum;       /* [g*S+s] recte responsi */
                 i32* margo;              /* [g*S+s] maximum disciplinae
                                             minus secundum; par 0 */
                 i32* habitus;            /* [g] PartitioHabitus */
                 b32* mutabilis;          /* [g] suffragia diversa */
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
 * NON videt - tabula infimorum instrumenti eam ostendit; SEMEN (T36 a)
 * catenam ab infimo dato incipit: columna intra semen ad idem infimum
 * redit, lucrum 0, limine lucri recusatur.
 * ================================================== */

nomen structura {
                  i32  limen;            /* ordinarie XX */
                  i32  lucrum_minimum;   /* 0 = limen */
                  b32  sortes_vetant;    /* ordinarie VERUM */
    constans Partitio* semen;           /* gradus 0; NIHIL = una */
} PartitioCatenaOptiones;

/* optiones ordinariae: XX, 0, VERUM, NIHIL */
vacuum
partitio_catena_optiones_initium (
    PartitioCatenaOptiones* optiones);

nomen structura {
      constans Partitio*  semen;           /* gradus 0; una si nullum */
     PartitioAestimatio*  initium;         /* aestimatio seminis */
                    i32   numerus;         /* gradus accepti */
                    i32*  columnae;        /* index columnae gradus */
               Partitio** partes;          /* infimum cumulatum */
     PartitioAestimatio** aestimationes;
} PartitioCatena;

/* NIHIL si optiones NIHIL, columna NIHIL, ordines inaequales (semen
 * quoque), aut memoria deficit */
PartitioCatena*
partitio_catenam_struere (
                         Piscina* piscina,
      constans Partitio* constans* columnae,
                              i32 k,
               constans Partitio* aurum,
               constans Partitio* sortes,
    constans PartitioCatenaOptiones* optiones);

#endif /* PARTITIO_AESTIMATIO_H */
