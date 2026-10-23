/* vicus_sonus_porta.h - porta soni vera: concentus super auditus
 *
 * Hospes soni communis (concentus.h: mixtura, rector, exitus UNUS)
 * cuius exitus auditus est (vivum, CoreAudio; posix: aperire FALSUM -
 * silentium); super eum tintinnabulum cum libro ordinario
 * (cantuale_ordinarium, infixus) et S5c aetherophonum. Solum
 * applicationes vici eam nectunt (vicus.c, vicus_terminalis.c), non
 * vicus_applicatio - probationes vici sine sono et sine CoreAudio
 * manent.
 *
 * EXITUS PIGER: concentus exitum post quietem claudit et primo sono
 * iterum aperit; exitus communis concentus_auditus.h anulos in
 * piscina PROPRIA per aperitionem tenet - piscina vici aliter per dies
 * cresceret.
 *
 * TEMPUS: fenestra_tempus_ms (horologium pulsus).
 *
 * USUS:
 *   VicusPortaSoni porta;
 *   si (vicus_sonus_porta(&porta, piscina, semen))
 *       vicus_applicatio_soni_portam_ponere(&app, &porta);
 */
/* <aedilis corpus="lib/vicus_sonus_porta.c"/> */
#ifndef VICUS_SONUS_PORTA_H
#define VICUS_SONUS_PORTA_H

#include "latina.h"
#include "piscina.h"
#include "vicus_sonus.h"

/* FALSUM: liber ordinarius fractus aut memoria deest (causa in
 * stderr) - porta non impleta, vicus sine sono pergit */
b32
vicus_sonus_porta (
    VicusPortaSoni* porta,
           Piscina* piscina,
               i64  semen);

#endif /* VICUS_SONUS_PORTA_H */
