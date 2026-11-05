/* vicus_sonus_porta.h - porta soni vera: concentus super exitum datum
 *
 * Hospes soni communis (concentus.h: mixtura, rector, exitus UNUS);
 * super eum tintinnabulum cum libro ordinario (cantuale_ordinarium,
 * infixus), S5c aetherophonum et carmen (cantor, gabc pars II P5).
 * Exitus a vocante: auditus (CoreAudio) per vicus_sonus_auditus.h in
 * applicationibus vici (vicus.c, vicus_terminalis.c), fictus in
 * probationibus (probatio_vicus_sonus_porta.c) - haec plagula CoreAudio
 * nescit, ergo probationes eam sine compagibus audio nectunt. Non
 * vicus_applicatio: probationes vici sine sono manent.
 *
 * EXITUS PIGER: concentus exitum post quietem claudit et primo sono
 * iterum aperit.
 *
 * TEMPUS: fenestra_tempus_ms (horologium pulsus).
 *
 * USUS (probationes):
 *   ConcentusExitus exitus = { ... fictus ... };
 *   si (vicus_sonus_porta_cum_exitu(&porta, piscina, semen, &exitus))
 *       ... porta.carmen_incipere(porta.ctx, carmen, 0) ...
 */
/* <aedilis corpus="lib/vicus_sonus_porta.c"/> */
#ifndef VICUS_SONUS_PORTA_H
#define VICUS_SONUS_PORTA_H

#include "latina.h"
#include "piscina.h"
#include "vicus_sonus.h"
#include "concentus.h"

/* forma concentus portae: voces (cantilena plena XXXII, voces vivae,
 * carmen) et capacitates anulorum (potentiae II) - exitus auditus
 * eadem forma creatur */
#define VICUS_SONUS_VOCES  XLVIII
#define VICUS_SONUS_ANULI  CXXVIII

/* porta super exitum datum; exitus vivat dum porta. Cantor carminis
 * pigre creatur ad carmen_incipere primum. FALSUM: liber ordinarius
 * fractus aut memoria deest (causa in stderr) - porta non impleta */
b32
vicus_sonus_porta_cum_exitu (
     VicusPortaSoni* porta,
            Piscina* piscina,
                i64  semen,
    ConcentusExitus* exitus);

#endif /* VICUS_SONUS_PORTA_H */
