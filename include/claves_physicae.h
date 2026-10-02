/* claves_physicae.h - Codices clavium platformarum -> EventusCodex
 * (eventus A3; spec D3)
 *
 * Tabulae PURAE: codex virtualis platformae (macOS kVK_*; terminalis
 * kitty in phasi B) -> codex physicus vocabularii (W3C 'code'). Positio
 * clavis, non dispositio: kVK 0 est "KeyA" in QWERTY, AZERTY, Dvorak
 * idem. Ignotus -> EVENTUS_CODEX_IGNOTUS. Sine Cocoa: fenestra_macos.m
 * eam vocat, probationes sine fenestra.
 */

#ifndef CLAVES_PHYSICAE_H
#define CLAVES_PHYSICAE_H

/* <aedilis nexus="purus"/> - clausura sine regula nexus (nulla
 * framework; eventus A1b: bin/aedilis --nexus-purus) */

#include "latina.h"
#include "eventus.h"

/* macOS: codex virtualis (NSEvent keyCode, kVK_* in Carbon Events.h)
 * -> codex physicus. */
EventusCodex
claves_codex_ex_macos (
    s32 codex_virtualis);

/* Character dispositionis US basicae (minusculae, numeri, signa,
 * spatium) -> codex physicus: clavis BASIS kitty ('base layout key',
 * vexillum IV) positio est, non dispositio currens (eventus B2b).
 * Maiusculae et non-ASCII -> IGNOTUS (kitty basem minusculam
 * mittit). */
EventusCodex
claves_codex_ex_littera (
    s32 runa);

/* kitty: numeri clavium functionalium (Escape 27, Enter 13, Tab 9,
 * Backspace 127, Caps Lock 57358, modificatores laterales 57441..57450)
 * -> codex physicus. Ceteri (runae, F13+, tabula numerica sine codice
 * in vocabulario) -> IGNOTUS. */
EventusCodex
claves_codex_ex_kitty (
    s32 numerus);

#endif /* CLAVES_PHYSICAE_H */
