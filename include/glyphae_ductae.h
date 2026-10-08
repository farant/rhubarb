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
