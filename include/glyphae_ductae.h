/* glyphae_ductae.h - characteres capsarum, quadrorum et braille ut
 * figurae ductae ad magnitudinem cellulae, non ex fonte
 * (aemulator-plan D7c; Ghostty font/sprite/draw: box.zig, block.zig,
 * braille.zig)
 *
 * Lineae cellularum contiguarum congruunt quia ex mensura cellulae
 * ducuntur. Purum: nulla allocatio, nullum mandatum - LARVA
 * opacitatis (octetus per pixelum, 0 vacuum ... CCLV plenum, ordine
 * linearum) quam vocans in mandata aut tabulam vertit.
 *
 * Ambitus v1: U+2500-257F (lineae capsarum: leves, graves, duplices,
 * lineolatae, arcus, diagonales, dimidiae), U+2580-259F (elementa
 * quadrata: dimidia, octavae, quadrantes; umbrae ░▒▓ ut
 * opacitas),
 * U+2800-28FF (braille, puncta II x IV).
 *
 * Ambitus v2 (symbola, census Claude Code 2026-10-07): LVII picturae
 * VI x VIII manu ductae, per proximum scalatae - sagittae, notae
 * (✓ ✗), circuli et puncta (● ○ ◐ •), stellae
 * (✻ ✶ ✳ ✢ ✽ ✦ ★), anguli (❯ ›), media
 * (⏺ ⏵ ⏸ ▶ ▲ ▼ ■ □), claves (⌘ ⌥ ⏎),
 * ⚠ ⎿ ⧉ ⋮ ☐ ☒ ≤ ≥ ≡, et – — …
 * (fons ad '-' '.' vertebat).
 *
 * Ambitus v3 (litterae notatae, Franus 2026-10-28): litterae cum
 * notis COMPOSITAE - basis (pictura nostra aut fons VI x VIII) et
 * notae supra in ordinibus 0-I (corpus minusculae ab ordine II;
 * punctum i demptum); maiusculae notatae ad corpus (ordines II-VI)
 * contractae, ordo aequalis sequenti demptus, ut notae easdem II
 * ordines habeant. Notae: macron, breve, acutus/tonos, gravis,
 * circumflexus (perispomeni), diaeresis, spiritus lenis et asper
 * (sinistra) cum accentu (dextra), iota subscriptum (ordo VII).
 * Tabula (CCLIX): Latina Ext-A (vocales cum macron et breve, et
 * maiusculae), Graeca cum tonis (U+0386-03CE), Graeca extensa
 * (U+1F00-1FFF) - ex decompositione Unicode semel scripta. ɣ (U+0263)
 * pictura sola.
 *
 * Cantus Latinus (gabc pars II P4, 2026-11-04): œ Œ ℟ ℣ picturae v2,
 * ǽ composita (æ fontis cum acuto).
 */
#ifndef GLYPHAE_DUCTAE_H
#define GLYPHAE_DUCTAE_H

#include "latina.h"

/* runa a nobis ducitur? */
b32
glyphae_ductae_est (
    s32 runa);

/* larvam latitudo x altitudo implet (prius vacuat); FALSUM si runa
 * non nostra aut magnitudo nulla (larva tunc intacta) */
b32
glyphae_ductae_pingere (
    s32  runa,
    i32  latitudo,
    i32  altitudo,
     i8* larva);

#endif
