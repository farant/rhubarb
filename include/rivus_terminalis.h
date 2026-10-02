/* rivus_terminalis.h - Fons eventuum terminalis: octeti -> Eventus
 * (eventus B3)
 *
 * Pipeline UNA (ex lectore tesserae B1b translata): octeti ->
 * series_terminalis (modo initus) -> interpres_terminalis -> Eventus in
 * caudam. Possidet: buffer crudum, moram ESC et glutini, reliquias post
 * moram (H7/H8), canales crudos (mus X10, caudae alienae CSI [, corpus
 * glutini ?2004).
 *
 * PURUS: nec legit nec horologium tenet (in lib/, sub tessera - pontem
 * eius non novit). Vocans octetos tradit (rivus_tradere), rogat quamdiu
 * exspectet (rivus_mora_ms), silentium nuntiat (rivus_moram), eventa
 * trahit (rivus_eventum). Decodificatio PIGRA: lexema unum quoad
 * eventum
 * adest - motus NON coalescit (proiectio tesserae eventum quemque
 * videt). Consumptores Eventus rivus_eventum_coalitum legunt (motus
 * per lectionem coalescit, ut in fenestra; spec Q10).
 *
 * FACULTATES primum eventum fluxus sunt (spec Q4), iterum cum modi
 * declarantur aut kitty primum videtur.
 *
 * VISUS (textus eventuum) valent usque ad rivus_eventum proximum.
 */

#ifndef RIVUS_TERMINALIS_H
#define RIVUS_TERMINALIS_H

/* <aedilis nexus="purus"/> - clausura sine regula nexus (nulla
 * framework; eventus A1b: bin/aedilis --nexus-purus) */

#include "latina.h"
#include "piscina.h"
#include "eventus.h"
#include "eventus_cauda.h"
#include "interpres_terminalis.h"

#define RIVUS_BUFFER              CCLVI     /* octeti crudi gestati */
#define RIVUS_MORA_FUGAE_MS       XXV       /* ESC solus vs series */
#define RIVUS_MORA_GLUTINI_MS     (III * M) /* silentium: finis */
#define RIVUS_GLUTINUM_CAPACITAS  65536     /* = EVENTUS_CAUDA_TEXTUS */

/* Modi quos applicatio DECLARAT (eventus B3b). Rivus octetos scribendos
 * reddit, non scribit (PURUS). Terminal modum ignotum tacite neglegit.
 *   MUS       ?1000 ?1002 ?1006: pressio, tractus, forma SGR
 *   SUPER     ?1003 hover (spec Q24: solum declaratum); MUS includit
 *   GLUTINUM  ?2004: glutinum uncis inclusum
 *   FOCUS     ?1004: focus I / O
 *   KITTY     CSI > 31 u impellitur, CSI < u extrahitur
 * RIVUS_MODI_MAXIMUM = octeti quos buffer modorum capere debet. */
#define RIVUS_MODUS_MUS       0x01
#define RIVUS_MODUS_SUPER     0x02
#define RIVUS_MODUS_GLUTINUM  0x04
#define RIVUS_MODUS_FOCUS     0x08
#define RIVUS_MODUS_KITTY     0x10
#define RIVUS_MODI_MAXIMUM    LXIV

nomen structura RivusTerminalis RivusTerminalis;

/* Cellula in pixelis nostris (interpres: mus ad centrum cellulae). */
RivusTerminalis*
rivus_creare (
    Piscina* piscina,
        s32  cellula_latitudo,
        s32  cellula_altitudo);

/* Interpres internus (vexilla kitty, facultates). */
InterpresTerminalis*
rivus_interpres (
    RivusTerminalis* rivus);

/* Octeti quos rivus nunc capere potest; vocans non plus tradat. */
i32
rivus_spatium (
    constans RivusTerminalis* rivus);

/* Octetos a fonte lectos tradere (ordine). Redde quot accepti. */
i32
rivus_tradere (
    RivusTerminalis* rivus,
        constans i8* octeti,
                i32  mensura);

/* Eventum proximum. FALSUM: octeti desunt - vocans legat (rivus_mora_ms
 * dicit quamdiu) aut silentium nuntiet (rivus_moram). */
b32
rivus_eventum (
    RivusTerminalis* rivus,
                s64  tempus,
            Eventus* eventus);

/* Ut rivus_eventum, sed octeti praesentes OMNES decodificantur antequam
 * extrahitur: motus consecutivi in eventum unum coalescunt (exempla in
 * datum.mus.exempla). Pro consumptoribus Eventus (non proiectione). */
b32
rivus_eventum_coalitum (
    RivusTerminalis* rivus,
                s64  tempus,
            Eventus* eventus);

/* 0 = nihil pendet (vocans moram suam habet); > 0 = series, runa aut
 * glutinum pendet: post tot ms silentii rivus_moram vocetur. */
s32
rivus_mora_ms (
    constans RivusTerminalis* rivus);

/* Silentium moram exhausit: series pendens evacuatur (ESC = Effugium,
 * ESC ESC = duo, 'ESC x' = alterum + x, cetera abiciuntur; mus SGR
 * dimidia et ESC solus reliquiae fiunt continuationi), glutinum
 * truncatum finitur, runa dimidia abicitur. */
vacuum
rivus_moram (
    RivusTerminalis* rivus,
                s64  tempus);

/* Modos declarare: octetos intrandi in buffer scribit (ordine ?1000
 * ?1002 ?1003 ?1006 ?2004 ?1004, deinde kitty) et mensuram reddit;
 * facultates (super, vexilla kitty) ponit et FACULTATES impellit.
 * Redde 0 (nihil mutatum) si modi iam intrati aut capacitas <
 * RIVUS_MODI_MAXIMUM. */
i32
rivus_modos_intrare (
    RivusTerminalis* rivus,
                i32  modi,
                 i8* buffer,
                i32  capacitas);

/* Octetos exeundi scribere: ordine INVERSO eorum quae intrata sunt
 * (kitty primum extrahitur). Post exitum nihil intratum: iterum
 * vocata 0 reddit. Solum memcpy - in tractatore signorum tutum. */
i32
rivus_modos_exire (
    RivusTerminalis* rivus,
                 i8* buffer,
                i32  capacitas);

/* Octeti in buffere nondum consumpti (ad finem fluxus probandum). */
i32
rivus_pendentes (
    constans RivusTerminalis* rivus);

#endif /* RIVUS_TERMINALIS_H */
