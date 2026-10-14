/* vicus_quaestio_porta.h - porta $question per adiutorem
 * (quaestio-rivus T8, ruling R10)
 *
 * Porta VicusPortaQuaestionis super bin/quaestio per processum:
 * incipere
 * adiutorem generat (numquam obstat), pulsare tabulas (quaestio.h) ex
 * effusione hactenus legit et textum crudum collectum PLANAT
 * (quaestio_textum_planare, R9), abrumpere adiutorem occidit. Adiutor
 * mortuus sine F, tabula prava, terminus excessus: error nominatus.
 * Clavem SOLUS adiutor legit - haec bibliotheca numquam. */

#ifndef VICUS_QUAESTIO_PORTA_H
#define VICUS_QUAESTIO_PORTA_H

/* <aedilis corpus="lib/vicus_quaestio_porta.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "vicus_applicatio.h"

/* via adiutoris copiatur; mora_maxima_ms terminus durus adiutoris
 * (ZEPHYRUM = CLXXX s) */
VicusPortaQuaestionis
vicus_quaestio_porta (
    constans character* via_adiutoris,
                   i32  mora_maxima_ms,
               Piscina* piscina);

/* R4: adiutor iuxta binarium (".../bin/vicus" -> ".../bin/quaestio");
 * vocatio_prima sine '/' aut NIHIL: "quaestio" (per PATH, execvp) */
constans character*
vicus_quaestio_adiutoris_via (
    constans character* vocatio_prima,
               Piscina* piscina);

#endif /* VICUS_QUAESTIO_PORTA_H */
