/* membrum__prompta.h - LINEA PROMPTAE widget (nomenclator-plan U3; ex
 * calendario extractum; genus membrum__ = componens UI compositioni
 * destinatum): linea una editabilis (vim, modus inserendi) cum titulo
 * generis ("new:", "title:").
 *
 * Status in ramo insulae HOSPITIS - campi quos manifestum eius
 * declarat, scriptore actione hospitis:
 *   prompta          genus (electio hospitis; absens / "nulla" =
 *                    clausa)
 *   prompta_textus   textus effugitus (scriba_effugere)
 *   prompta_cursor   columna cursoris
 * Hospes genera et actum Reditus iudicat (committit, deinde claudit si
 * actum); membrum aperit, claudit, legit, claves edit, pingit.
 */
#ifndef MEMBRUM__PROMPTA_H
#define MEMBRUM__PROMPTA_H

/* <aedilis corpus="lib/membrum__prompta.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "insula.h"
#include "eventus.h"
#include "mandatum.h"

#define PROMPTA_MAXIMA  CCLVI   /* octeti textus */

nomen enumeratio {
    PROMPTA_NIHIL = ZEPHYRUM,   /* eventus non clavis: non tractatus */
    PROMPTA_EDITA,              /* clavis consumpta (textus aut cursor
                                 * fortasse mutatus) */
    PROMPTA_REDITUS,            /* Enter: *textus datus, prompta manet
                                 * (hospes committit et claudit) */
    PROMPTA_EFFUGIUM            /* Esc: prompta clausa */
} PromptaExitus;

/* aperta? genus in *genus (NIHIL licet) */
b32
prompta_aperta (
    constans InsulaRamus* ramus,
                  chorda* genus);

/* textus solutus in exitus[PROMPTA_MAXIMA]; redde longitudinem */
i32
prompta_textus (
    constans InsulaRamus* ramus,
               character* exitus);

/* aperire genere dato cum textu initiali (cursor in fine) */
b32
prompta_aperire (
            InsulaRamus* ramus,
     constans character* scriptor,
     constans character* genus,
                 chorda  textus,
                Piscina* piscina);

b32
prompta_claudere (
            InsulaRamus* ramus,
     constans character* scriptor,
                Piscina* piscina);

/* clavis in prompta aperta: vim (inserere, sagittae, Home/End,
 * Backspace/Delete); Reditus -> PROMPTA_REDITUS cum *textus (spatiis
 * finalibus demptis, in piscina); Esc claudit; claves ceterae
 * consumuntur (EDITA) */
PromptaExitus
prompta_clavem_agere (
            InsulaRamus* ramus,
     constans character* scriptor,
       constans Eventus* ev,
                 chorda* textus,
                Piscina* piscina);

/* <purus/> titulus generis (obscurus), campus, textus, cursor tenuis
 * in finibus f (spatio vocantis) */
vacuum
prompta_pingere (
                 Mandata* md,
                   Fines  f,
      constans character* titulus_generis,
    constans InsulaRamus* ramus);

#endif /* MEMBRUM__PROMPTA_H */
