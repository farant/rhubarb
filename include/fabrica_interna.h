/* fabrica_interna.h - INTERNA fabricae (fabrica-6 H2): auxilia quae
 * plagulae bibliothecae fabrica inter se communicant. NON API - solum
 * fabrica.c et fabrica_<pars>.c id includunt; consumptores fabrica.h
 * solum. Praefixum 'fabricae_' (genetivus) ab API 'fabrica_' distinguit
 * et nomina cum ceteris bibliothecis in suite necte-omnia non
 * collidunt. */

#ifndef FABRICA_INTERNA_H
#define FABRICA_INTERNA_H

#include "fabrica.h"

/* liber cursus per actionem: <LIBRI_DIRECTORIUM><titulus>.tsv */
#define LIBRI_DIRECTORIUM "build/fabrica/lectiones/"

/* partes thesauri aedilis (blobi, actiones, generationes) - cache
 * sigillorum, non ingressus (fabrica.c) */
externus constans character* constans fabricae_thesauri_partes[III];

/* genus gradus probationes_c (fabrica_probationes_c.c), in registro
 * graduum (fabrica_gradus.c) */
externus constans FabricaGradus fabricae_gradus_probationes_c;

/* auxilia (fabrica.c) */

chorda
fabricae_iungere (
                Piscina* piscina,
     constans character* ante,
                 chorda  medium,
     constans character* post);

vacuum
fabricae_causam_ponere (
                 chorda* causa_out,
                Piscina* piscina,
     constans character* ante,
                 chorda  medium,
     constans character* post);

Xar*
fabricae_xar_chordarum (
    Piscina* piscina);

vacuum
fabricae_chordam_addere (
       Xar* xar,
    chorda  valor);

/* iudicium et vestigia lectionum (fabrica.c) */

FabricaIudicium
fabricae_iudicium (
           chorda artificium,
    FabricaStatus status,
           chorda causa);

b32
fabricae_actionem_sigillare_semel (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
                   Piscina* piscina,
                  Sigillum* sigillum_out,
                    chorda* causa_out);

Sigillum
fabricae_clavem_memoriae (
    constans FabricaActio* actio,
        constans Sigillum* ingressus);

b32
fabricae_lectionem_sigillare (
    constans FabricaSutura* sutura,
               LectioGenus  genus,
                    chorda  via,
                   Piscina* piscina,
                  Sigillum* sigillum_out);

b32
fabricae_lectiones_transitus_colligere (
    constans FabricaSutura*  sutura,
     constans FabricaActio*  actio,
        constans character*  liber_via,
                   Piscina*  piscina,
                       Xar** lectiones_out,
                    chorda*  causa_out);

b32
fabricae_particulas_transitus (
    constans FabricaSutura*  sutura,
     constans FabricaActio*  actio,
         constans Sigillum*  artificium,
                   Piscina*  piscina,
                       Xar** particulae_out);

/* genera (fabrica_genera.c) */

b32
fabricae_particulas_colligere (
     constans FabricaSutura* sutura,
      constans FabricaActio* actio,
               constans Xar* exclusa,
                    Piscina* piscina,
                        Xar* particulae,
                     chorda* causa_out);

s32
fabricae_particulas_comparare (
    constans vacuum* a,
    constans vacuum* b);

vacuum
fabricae_locum_addere (
                 Xar* loci,
    FabricaFormaLoci  forma,
              chorda  via,
              chorda  suffixa);

vacuum
fabricae_loci_ex_particulis (
    constans Xar* particulae,
         Piscina* piscina,
             Xar* loci);

vacuum
fabricae_viam_dividere (
     chorda  via,
     chorda* directorium_out,
     chorda* titulus_out);

b32
fabricae_locus_tangit (
    constans FabricaLocus* locus,
                   chorda  via,
                   chorda  directorium,
                   chorda  titulus,
                  Piscina* piscina);

b32
fabricae_globus_congruit (
    constans character* exemplar,
    constans character* textus);

/* declarationes (fabrica_declarationes.c) */

b32
fabricae_chordam_continet (
     constans Xar* xar,
           chorda  valor);

/* ordo (fabrica_ordo.c) */

b32
fabricae_in_locis (
     constans Xar* loci,
           chorda  via,
          Piscina* piscina);

b32
fabricae_pendet (
    constans FabricaActio* posterior,
    constans FabricaActio* prior);

/* gradus (fabrica_gradus.c) */

chorda
fabricae_area_membri (
    constans FabricaActio* actio,
                  Piscina* piscina);

#endif /* FABRICA_INTERNA_H */
