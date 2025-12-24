/*
 * fons.h - Mappatio codepoints Unicode ad indices glypharum
 *
 * Convertit codepoints Unicode (0-0x10FFFF) ad indices glypharum (0-255)
 * pro fons_6x8 reddendo.
 */

#ifndef FONS_H
#define FONS_H

#include "latina.h"

/* Glypha TOFU pro codepoints ignotis (domus in fonte HP 100LX) */
#define FONS_TOFU 0x7F

/*
 * fons_codepoint_ad_glypham - Mappare codepoint Unicode ad indicem glyphae
 *
 * @codepoint: Codepoint Unicode (0-0x10FFFF)
 *
 * Redde: Index glyphae (0-255), vel FONS_TOFU si non mappatum
 *
 * Mappatio:
 *   U+0000-007F  -> 0x00-0x7F  (ASCII directum)
 *   U+00A0-00FF  -> 0xA0-0xFF  (Latin-1 supplementum)
 *   U+2018-201D  -> 0x27/0x22  (smart quotes ad rectas)
 *   U+2013-2014  -> 0x2D       (dashes ad hyphen)
 *   U+2026       -> 0x2E       (ellipsis ad punctum)
 *   Aliter       -> FONS_TOFU
 */
i8 fons_codepoint_ad_glypham(s32 codepoint);

/*
 * fons_est_mappatum - An codepoint habet glypham in fonte?
 *
 * @codepoint: Codepoint Unicode
 *
 * Redde: VERUM si mappatum (non TOFU), FALSUM aliter
 */
b32 fons_est_mappatum(s32 codepoint);

#endif /* FONS_H */
