/* specula.h - specula plagulae: fossa quae legenda fit cum plagula
 * mutatur (scripta, extensa, deleta, renominata, creata) - ansa
 * (reactor, fenestra) in ea exspectat et plagulam solum tunc legit,
 * numquam per pulsum (vicus quies T6c: liber praeconii).
 *
 * Plagula absens: directorium parentis speculatur donec creatur.
 * Deleta aut renominata (rotatio): via eadem in exhaustione proxima
 * iterum aperitur. Nullum octetum plagulae legitur.
 *
 * Variantes aedilis: lib/specula_macos.c (kqueue EVFILT_VNODE in
 * descriptore O_EVTONLY; etiam iOS), lib/specula_posix.c (Linux,
 * Android: inotify nondum - specula_creare NIHIL, vocator ad pulsus
 * crebros redit). */

#ifndef SPECULA_H
#define SPECULA_H

#include "latina.h"
#include "piscina.h"

nomen structura Specula Specula;

/* NIHIL si via NIHIL, directorium parentis abest, aut systema
 * recusat */
Specula*
specula_creare (
               Piscina* piscina,
    constans character* via);

/* fossa legenda dum mutatio pendet (REACTOR_LEGERE); -1 si clausa */
s32
specula_fossa (
    constans Specula* s);

/* eventus pendentes exhaurire (fossa iterum quieta, numquam obstat);
 * VERUM si plagula mutata est post exhaustionem priorem. Vocandum in
 * omni pulsu quo fossa legenda est - aliter ansa sine mora circumit */
b32
specula_exhaurire (
    Specula* s);

/* idempotens */
vacuum
specula_claudere (
    Specula* s);

#endif /* SPECULA_H */
