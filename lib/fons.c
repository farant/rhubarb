/*
 * fons.c - Implementatio mappationis glypharum
 *
 * Strategia mappationis:
 * 1. ASCII (U+0000-007F) -> directum
 * 2. Latin-1 supplement (U+00A0-00FF) -> directum (accentuata), nisi
 *    sedes vacua (tunc 3, deinde 4)
 * 3. Typographica communia -> substitutio
 * 4. Cetera -> TOFU
 */

#include "fons.h"
#include "fons_6x8.h"

/* sedes fontis sine pixelo (U+00AD, lineola mollis, consulto vacua) */
interior b32
glypha_vacua (
    s32 codepoint)
{
    i32 r;

    per (r = ZEPHYRUM; r < VIII; r++)
    {
        si (fons_6x8[codepoint][r] != ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

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
    si (   codepoint >= 0x00A1 && codepoint <= 0x00FF
        && (codepoint == 0x00AD || !glypha_vacua(codepoint)))
    {
        redde (i8)codepoint;
    }
    /* sedes vacua (2026-10-09: '×' et XL alii vacui pingebantur,
     * substitutiones infra numquam attingebantur): ad substitutiones,
     * deinde TOFU - numquam invisibile */

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


/* ==================================================
 * fons_8x16 (V3b): Unicode -> CP437
 * ================================================== */

/* CP437 0x80-0xFF -> Unicode */
hic_manens constans s32 codicis_superior[CXXVIII] = {
    0x00C7, 0x00FC, 0x00E9, 0x00E2, 0x00E4, 0x00E0, 0x00E5, 0x00E7,
    0x00EA, 0x00EB, 0x00E8, 0x00EF, 0x00EE, 0x00EC, 0x00C4, 0x00C5,
    0x00C9, 0x00E6, 0x00C6, 0x00F4, 0x00F6, 0x00F2, 0x00FB, 0x00F9,
    0x00FF, 0x00D6, 0x00DC, 0x00A2, 0x00A3, 0x00A5, 0x20A7, 0x0192,
    0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x00F1, 0x00D1, 0x00AA, 0x00BA,
    0x00BF, 0x2310, 0x00AC, 0x00BD, 0x00BC, 0x00A1, 0x00AB, 0x00BB,
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556,
    0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x255B, 0x2510,
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F,
    0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567,
    0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B,
    0x256A, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580,
    0x03B1, 0x00DF, 0x0393, 0x03C0, 0x03A3, 0x03C3, 0x00B5, 0x03C4,
    0x03A6, 0x0398, 0x03A9, 0x03B4, 0x221E, 0x03C6, 0x03B5, 0x2229,
    0x2261, 0x00B1, 0x2265, 0x2264, 0x2320, 0x2321, 0x00F7, 0x2248,
    0x00B0, 0x2219, 0x00B7, 0x221A, 0x207F, 0x00B2, 0x25A0, 0x00A0
};

/* CP437 0x01-0x1F (signa) -> Unicode */
hic_manens constans s32 codicis_signa[XXXI] = {
    0x263A, 0x263B, 0x2665, 0x2666, 0x2663, 0x2660, 0x2022, 0x25D8,
    0x25CB, 0x25D9, 0x2642, 0x2640, 0x266A, 0x266B, 0x263C, 0x25BA,
    0x25C4, 0x2195, 0x203C, 0x00B6, 0x00A7, 0x25AC, 0x21A8, 0x2191,
    0x2193, 0x2192, 0x2190, 0x221F, 0x2194, 0x25B2, 0x25BC
};

/* Latin-1 quae CP437 non habet -> littera basis (aut signum simile) */
hic_manens constans s32 codicis_bases[][II] = {
    { 0x00C0, 'A' }, { 0x00C1, 'A' }, { 0x00C2, 'A' }, { 0x00C3, 'A' },
    { 0x00C8, 'E' }, { 0x00CA, 'E' }, { 0x00CB, 'E' }, { 0x00CC, 'I' },
    { 0x00CD, 'I' }, { 0x00CE, 'I' }, { 0x00CF, 'I' }, { 0x00D0, 'D' },
    { 0x00D2, 'O' }, { 0x00D3, 'O' }, { 0x00D4, 'O' }, { 0x00D5, 'O' },
    { 0x00D8, 'O' }, { 0x00D9, 'U' }, { 0x00DA, 'U' }, { 0x00DB, 'U' },
    { 0x00DD, 'Y' }, { 0x00DE, 'P' }, { 0x00E3, 'a' }, { 0x00F0, 'd' },
    { 0x00F5, 'o' }, { 0x00F8, 'o' }, { 0x00FD, 'y' }, { 0x00FE, 'p' },
    { 0x00D7, 'x' }, { 0x00A9, 'c' }, { 0x00AE, 'R' }, { 0x00A6, '|' },
    { 0x00A8, '"' }, { 0x00AF, '-' }, { 0x00B4, 0x27 }, { 0x00B8, ',' },
    { 0x00B9, '1' }, { 0x00B3, '3' }, { 0x00AD, '-' },
    /* substitutiones ut fons_6x8 */
    { 0x2018, 0x27 }, { 0x2019, 0x27 }, { 0x201A, 0x27 },
    { 0x201B, 0x27 }, { 0x201C, '"' }, { 0x201D, '"' }, { 0x201E, '"' },
    { 0x201F, '"' }, { 0x2013, '-' }, { 0x2014, '-' }, { 0x2015, '-' },
    { 0x2026, '.' }, { 0x2212, '-' },
    /* Graeca Latinis similia quae CP437 non habet */
    { 0x0391, 'A' }, { 0x0392, 'B' }, { 0x0395, 'E' }, { 0x0396, 'Z' },
    { 0x0397, 'H' }, { 0x0399, 'I' }, { 0x039A, 'K' }, { 0x039C, 'M' },
    { 0x039D, 'N' }, { 0x039F, 'O' }, { 0x03A1, 'P' }, { 0x03A4, 'T' },
    { 0x03A5, 'Y' }, { 0x03A7, 'X' }, { 0x03BF, 'o' }, { 0x03B2, 0xE1 },
    { ZEPHYRUM, ZEPHYRUM }
};

i8
fons_8x16_codepoint_ad_glypham (
    s32 codepoint)
{
    i32 i;

    si (codepoint < ZEPHYRUM)
    {
        redde (i8)0x7F;
    }
    si (codepoint < 0x80)
    {
        redde (i8)codepoint;
    }
    si (codepoint == 0x00A0)
    {
        redde (i8)0x20;
    }
    si (codepoint == 0x2302)
    {
        redde (i8)0x7F;
    }
    per (i = ZEPHYRUM; i < CXXVIII; i++)
    {
        si (codicis_superior[i] == codepoint)
        {
            redde (i8)(0x80 + i);
        }
    }
    per (i = ZEPHYRUM; i < XXXI; i++)
    {
        si (codicis_signa[i] == codepoint)
        {
            redde (i8)(I + i);
        }
    }
    per (i = ZEPHYRUM; codicis_bases[i][ZEPHYRUM]; i++)
    {
        si (codicis_bases[i][ZEPHYRUM] == codepoint)
        {
            redde (i8)codicis_bases[i][I];
        }
    }
    redde (i8)0x7F;
}

s32
fons_latitudo (
    i32 fons)
{
    redde fons == FONS_TITULI ? VIII : VI;
}

s32
fons_altitudo (
    i32 fons)
{
    redde fons == FONS_TITULI ? XVI : VIII;
}
