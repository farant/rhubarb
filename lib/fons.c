/*
 * fons.c - Implementatio mappationis glypharum
 *
 * Strategia mappationis:
 * 1. ASCII (U+0000-007F) -> directum
 * 2. Latin-1 supplement (U+00A0-00FF) -> directum (accentuata)
 * 3. Typographica communia -> substitutio
 * 4. Cetera -> TOFU
 */

#include "fons.h"

i8
fons_codepoint_ad_glypham (
    s32 codepoint)
{
    /* Codepoint invalidus */
    si (codepoint < ZEPHYRUM)
    {
        redde FONS_TOFU;
    }

    /* ASCII directum (U+0000-007F) */
    si (codepoint <= 0x7F)
    {
        redde (i8)codepoint;
    }

    /* NBSP (U+00A0) -> spatium ordinarium */
    si (codepoint == 0x00A0)
    {
        redde 0x20;
    }

    /* Latin-1 supplement (U+00A1-00FF) -> 0xA1-0xFF
     * Haec includit characteres accentuatos:
     * e-acutum (é), n-tilde (ñ), u-umlaut (ü), etc.
     */
    si (codepoint >= 0x00A1 && codepoint <= 0x00FF)
    {
        redde (i8)codepoint;
    }

    /* Characteres typographici communes (substitutiones) */
    commutatio (codepoint)
    {
        /* Virgulae curvae -> rectae */
        casus 0x2018:  /* ' LEFT SINGLE QUOTATION MARK */
        casus 0x2019:  /* ' RIGHT SINGLE QUOTATION MARK */
        casus 0x201A:  /* ‚ SINGLE LOW-9 QUOTATION MARK */
        casus 0x201B:  /* ‛ SINGLE HIGH-REVERSED-9 QUOTATION MARK */
            redde 0x27;  /* ' APOSTROPHE */

        casus 0x201C:  /* " LEFT DOUBLE QUOTATION MARK */
        casus 0x201D:  /* " RIGHT DOUBLE QUOTATION MARK */
        casus 0x201E:  /* „ DOUBLE LOW-9 QUOTATION MARK */
        casus 0x201F:  /* ‟ DOUBLE HIGH-REVERSED-9 QUOTATION MARK */
            redde 0x22;  /* " QUOTATION MARK */

        /* Lineolae -> hyphen */
        casus 0x2013:  /* – EN DASH */
        casus 0x2014:  /* — EM DASH */
        casus 0x2015:  /* ― HORIZONTAL BAR */
            redde 0x2D;  /* - HYPHEN-MINUS */

        /* Ellipsis -> punctum (vel tria puncta) */
        casus 0x2026:  /* … HORIZONTAL ELLIPSIS */
            redde 0x2E;  /* . FULL STOP */

        /* Minus mathematicum -> hyphen */
        casus 0x2212:  /* − MINUS SIGN */
            redde 0x2D;

        /* Multiplicatio -> x */
        casus 0x00D7:  /* × MULTIPLICATION SIGN */
            redde 0x78;  /* x */

        /* Divisio -> / */
        casus 0x00F7:  /* ÷ DIVISION SIGN */
            redde 0x2F;  /* / */

        /* Bullet -> asteriscum */
        casus 0x2022:  /* • BULLET */
            redde 0x07;  /* In Alcuin habemus bullet */

        /* Gradus -> 0xF8 (in HP 100LX) */
        casus 0x00B0:  /* ° DEGREE SIGN */
            redde 0xF8;

        /* Primum/Secundum ordinis */
        casus 0x00B9:  /* ¹ SUPERSCRIPT ONE */
            redde 0x31;  /* 1 */
        casus 0x00B2:  /* ² SUPERSCRIPT TWO */
            redde 0xFD;  /* HP 100LX habet superscript 2 */
        casus 0x00B3:  /* ³ SUPERSCRIPT THREE */
            redde 0x33;  /* 3 */

        /* Fractiones */
        casus 0x00BC:  /* ¼ VULGAR FRACTION ONE QUARTER */
            redde 0xAC;  /* HP 100LX */
        casus 0x00BD:  /* ½ VULGAR FRACTION ONE HALF */
            redde 0xAB;  /* HP 100LX */
        casus 0x00BE:  /* ¾ VULGAR FRACTION THREE QUARTERS */
            redde 0xAD;  /* HP 100LX */

        /* Paragraphus et sectio */
        casus 0x00A7:  /* § SECTION SIGN */
            redde 0x15;  /* Alcuin habet symbolum */
        casus 0x00B6:  /* ¶ PILCROW SIGN */
            redde 0x14;  /* Alcuin habet symbolum */

        /* Graeca Latinis similia (2026-10-08): fons_6x8 Graeca non
         * habet; quae Latinis pinguntur hic mappantur. Ceterae (alpha,
         * pi, Gamma...) TOFU manent - terminale eas per glyphae_ductae
         * ducit. */
        casus 0x0391: redde 'A';  /* Alpha */
        casus 0x0392: redde 'B';  /* Beta */
        casus 0x0395: redde 'E';  /* Epsilon */
        casus 0x0396: redde 'Z';  /* Zeta */
        casus 0x0397: redde 'H';  /* Eta */
        casus 0x0399: redde 'I';  /* Iota */
        casus 0x039A: redde 'K';  /* Kappa */
        casus 0x039C: redde 'M';  /* Mu */
        casus 0x039D: redde 'N';  /* Nu */
        casus 0x039F: redde 'O';  /* Omicron */
        casus 0x03A1: redde 'P';  /* Rho */
        casus 0x03A4: redde 'T';  /* Tau */
        casus 0x03A5: redde 'Y';  /* Upsilon */
        casus 0x03A7: redde 'X';  /* Chi */
        casus 0x03BF: redde 'o';  /* omicron */

        ordinarius:
            redde FONS_TOFU;
    }
}

b32
fons_est_mappatum (
    s32 codepoint)
{
    redde (fons_codepoint_ad_glypham(codepoint) != FONS_TOFU);
}
