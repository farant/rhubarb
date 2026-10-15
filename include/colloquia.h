/* colloquia.h - sessiones Claude Code (colloquia-plan T3)
 *
 * Ex libro praeconii (eventus hamorum), caudis transcriptorum
 * (tituli: custom-title > ai-title > nomen directorii) et panellis
 * tmux (multiplexor). Lectio PURA eventuum separata ab I/O.
 *
 * STATUS per sessionem, eventus ordine scripturae:
 *   SessionStart                          OTIOSUM
 *   UserPromptSubmit, PreToolUse          LABORAT
 *   Notification permission_prompt /
 *     agent_needs_input / elicitation_*,
 *     StopFailure                         EXSPECTAT
 *   Notification idle_prompt              OTIOSUM
 *   Stop                                  PERFECIT
 *   SessionEnd; panellus non iam vivit    CLAUSUM
 * Linea: "praeco status" (eventus sine sessione, per panellum ad
 * sessionem ultimam eius panelli) lineam derivatam vincit usque ad
 * promptum proximum.
 */

#ifndef COLLOQUIA_H
#define COLLOQUIA_H

/* <aedilis corpus="lib/colloquia.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "multiplexor.h"

nomen enumeratio {
    COLLOQUIUM_IGNOTUM = ZEPHYRUM,
    COLLOQUIUM_OTIOSUM,      /* SessionStart, idle_prompt: exspectat te
                              * sine urgentia */
    COLLOQUIUM_LABORAT,      /* UserPromptSubmit, PreToolUse */
    COLLOQUIUM_EXSPECTAT,    /* permission_prompt, agent_needs_input,
                              * elicitation, StopFailure: TE eget */
    COLLOQUIUM_PERFECIT,     /* Stop: responsum paratum */
    COLLOQUIUM_CLAUSUM       /* SessionEnd, aut panellus non vivit */
} StatusColloquii;

nomen structura {
              chorda sessio;
              chorda titulus;          /* /rename > ai-title >
                                         * directorium */
              chorda via;              /* cwd */
              chorda transcriptum;
              chorda panellus;         /* ultimus notus ("%17") */
              chorda servitor;         /* socket tmux */
     StatusColloquii status;
              chorda linea;            /* linea status: "praeco
                                         * status" (si recentior
                                         * prompto ultimo), aliter
                                         * derivata ("Bash: Run
                                         * calendar suites", nuntius,
                                         * responsum ultimum) */
                 s64 tempus;           /* eventus ultimi */
                 b32 panellus_vivit;
} Colloquium;

/* PURA: eventus (Xar de PraeconiumEventus, ordine scripturae) ->
 * colloquia (Xar de Colloquium), recentissimum primum; "status" sine
 * sessione ad sessionem cuius panellus idem erat ultimus ante id
 * tempus */
Xar*
colloquia_ex_eventibus (
         Xar* eventus,
     Piscina* piscina);

/* titulus ex cauda transcripti (CCLVI K): custom-title, aliter
 * ai-title; vacuus si nullus */
chorda
colloquia_titulum_legere (
    constans character* transcriptum,
               Piscina* piscina);

/* panellos vivos applicare (Xar de MultiplexorPanellus):
 * panellus_vivit; panellus mortuus -> CLAUSUM */
vacuum
colloquia_panellos_applicare (
    Xar* colloquia,
    Xar* panelli);

/* totum: cauda libri (octeti) + tituli + tmux (m NIHIL = sine tmux;
 * tmux absens: panelli non applicantur) */
Xar*
colloquia_legere (
      constans character* via_libri,
    constans Multiplexor* m,
                     i32  octeti,
                 Piscina* piscina);

#endif /* COLLOQUIA_H */
