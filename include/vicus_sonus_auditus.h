/* vicus_sonus_auditus.h - porta soni vici super auditus (CoreAudio)
 *
 * vicus_sonus_porta.h super exitum communem concentus_auditus.h (anulos
 * in piscina PROPRIA per aperitionem tenet - piscina vici aliter per
 * dies cresceret; posix: aperire FALSUM - silentium). Seorsum a
 * vicus_sonus_porta.c ut probationes portae CoreAudio non nectant (gabc
 * pars II P5).
 *
 * USUS:
 *   VicusPortaSoni porta;
 *   si (vicus_sonus_porta(&porta, piscina, semen))
 *       vicus_applicatio_soni_portam_ponere(&app, &porta);
 */
/* <aedilis corpus="lib/vicus_sonus_auditus.c"/> */
#ifndef VICUS_SONUS_AUDITUS_H
#define VICUS_SONUS_AUDITUS_H

#include "latina.h"
#include "piscina.h"
#include "vicus_sonus_porta.h"

/* FALSUM: liber ordinarius fractus aut memoria deest (causa in
 * stderr) - porta non impleta, vicus sine sono pergit */
b32
vicus_sonus_porta (
    VicusPortaSoni* porta,
           Piscina* piscina,
               i64  semen);

#endif /* VICUS_SONUS_AUDITUS_H */
