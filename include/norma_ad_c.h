/* norma_ad_c.h - normae nominatae -> fons C qui easdem aedificat
 * (norma-spec-2 §III.3). Exitus GENERATUM (linea prima); bin/norma c
 * eum in plagulas scribit, fabrica iudicat.
 *
 * Pro praefixo "vates_norma_":
 *   Norma* vates_norma_<titulus>(Piscina*)          una per normam
 *   JsonValor* vates_norma_gignens_<g>(Sors*, Piscina*, vacuum*)
 *                                                   per gignens="g"
 * - prototypum in capite generato; vocans eam definit, nexor probat.
 * Referentiae <ad> semel per vocationem aedificantur (DAG servatur).
 *
 * STATUS: PROBATUM a Frano 2026-10-08 (norma-plan-3 A0). */
#ifndef NORMA_AD_C_H
#define NORMA_AD_C_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "norma.h"
#include "norma_stml.h"

nomen structura {
    chorda caput;    /* .h */
    chorda corpus;   /* .c */
} NormaFonsC;

/* caput_titulus: nomen plagulae capitis quod corpus includit
 * ("vates_responsum_norma.h"); fons_titulus: via .norma in commento
 * GENERATUM. Recusat (mensurae 0, *causa): titulus non identificator
 * C, nodus pravus, gignens sine titulo, numerus fluitans non finitus,
 * textus > DIX octeti (C89), profunditas > CXXVIII. */
NormaFonsC
norma_ad_c (
    constans NormaNominata* normae,
                       i32  numerus,
        constans character* praefixum,
        constans character* caput_titulus,
        constans character* fons_titulus,
                   Piscina* piscina,
                    chorda* causa);

#endif /* NORMA_AD_C_H */
