/* probatio_fons.c - Probationes mappationis glypharum */
#include "latina.h"
#include "fons.h"
#include "fons_6x8.h"
#include "fons_8x16.h"
#include "internamentum.h"
#include "thema.h"
#include "mandatum.h"
#include "figura.h"
#include "tabula_pixelorum.h"
#include "delineare_mandata.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>

/* index glyphae ut numerus (i8 signatum: 0x80.. negativa essent) */
interior s32
glypha (
    s32 cp)
{
    redde (s32)(insignatus character)fons_codepoint_ad_glypham(cp);
}

/* index glyphae fons_8x16 ut numerus */
interior s32
g816 (
    s32 cp)
{
    redde (s32)(insignatus character)fons_8x16_codepoint_ad_glypham(cp);
}

s32 principale(vacuum)
{
    Piscina* piscina;
        b32  praeteritus;

    piscina = piscina_generare_dynamicum("test_fons", IV * MXXIV);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * Probare ASCII directum
     * ================================================== */

    {
        imprimere("\n--- Probans ASCII directum ---\n");

        /* Characteres communes */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham('A'), 'A');
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham('Z'), 'Z');
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham('a'), 'a');
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham('z'), 'z');
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham('0'), '0');
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham('9'), '9');

        /* Punctuatio */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham('.'), '.');
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(','), ',');
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham('!'), '!');
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham('?'), '?');

        /* Spatium et nova linea */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(' '), ' ');
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham('\n'), '\n');
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham('\t'), '\t');

        /* Limites ASCII */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x00), 0x00);
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x7F), 0x7F);
    }


    /* ==================================================
     * Probare Latin-1 supplement
     * ================================================== */

    {
        imprimere("\n--- Probans Latin-1 supplement ---\n");

        /* Litterae accentuatae */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x00E9),
            0xE9);  /* e-acutum */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x00F1),
            0xF1);  /* n-tilde */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x00FC),
            0xFC);  /* u-umlaut */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x00E0),
            0xE0);  /* a-grave */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x00C9),
            0xC9);  /* E-acutum */

        /* Limites Latin-1 supplement */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x00A0),
            0x20);  /* NBSP -> space */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x00A1),
            0xA1);  /* inverted ! */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x00FF),
            0xFF);  /* y-umlaut */
    }


    /* ==================================================
     * Probare smart quotes -> rectae
     * ================================================== */

    {
        imprimere("\n--- Probans smart quotes ---\n");

        /* Single quotes -> apostrophe */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x2018),
            0x27);  /* ' left */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x2019),
            0x27);  /* ' right */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x201A),
            0x27);  /* ‚ low-9 */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x201B),
            0x27);  /* ‛ reversed */

        /* Double quotes -> quotation mark */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x201C),
            0x22);  /* " left */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x201D),
            0x22);  /* " right */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x201E),
            0x22);  /* „ low-9 */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x201F),
            0x22);  /* ‟ reversed */
    }


    /* ==================================================
     * Probare dashes -> hyphen
     * ================================================== */

    {
        imprimere("\n--- Probans dashes ---\n");

        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x2013),
            0x2D);  /* en dash */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x2014),
            0x2D);  /* em dash */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x2015),
            0x2D);  /* horizontal bar */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x2212),
            0x2D);  /* minus sign */
    }


    /* ==================================================
     * Probare ellipsis et alia
     * ================================================== */

    {
        imprimere("\n--- Probans ellipsis et alia ---\n");

        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x2026),
            0x2E);  /* ellipsis */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x2022),
            0x07);  /* bullet */
    }


    /* ==================================================
     * Probare codepoints ignoti -> TOFU
     * ================================================== */

    {
        imprimere("\n--- Probans TOFU ---\n");

        /* Codepoints invalidi */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(-I),
            FONS_TOFU);

        /* Codepoints non mappati (e.g. emoji, CJK) */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x1F600),
            FONS_TOFU);  /* emoji */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x4E00),
            FONS_TOFU);   /* CJK */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x0100),
            FONS_TOFU);   /* Latin Extended-A */

        /* Graeca (non mappata) */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x03B1),
            FONS_TOFU);  /* alpha */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x03C0),
            FONS_TOFU);  /* pi */
    }


    /* ==================================================
     * Graeca Latinis similes (2026-10-08): maiusculae quae litteris
     * Latinis pinguntur, et omicron; ceterae (alpha, pi) TOFU manent -
     * terminale eas per glyphae_ductae ducit
     * ================================================== */

    {
        hic_manens constans s32 similes[][II] = {
            { 0x0391, 'A' }, { 0x0392, 'B' }, { 0x0395, 'E' },
            { 0x0396, 'Z' }, { 0x0397, 'H' }, { 0x0399, 'I' },
            { 0x039A, 'K' }, { 0x039C, 'M' }, { 0x039D, 'N' },
            { 0x039F, 'O' }, { 0x03A1, 'P' }, { 0x03A4, 'T' },
            { 0x03A5, 'Y' }, { 0x03A7, 'X' }, { 0x03BF, 'o' } };
        i32 k;

        imprimere("\n--- Probans Graeca Latinis similia ---\n");
        per (k = ZEPHYRUM; k < (i32)(magnitudo(similes)
                / magnitudo(similes[ZEPHYRUM])); k++)
        {
            CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(
                similes[k][ZEPHYRUM]), similes[k][I]);
        }
        /* Gamma, Delta Latinis dissimiles: TOFU */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x0393),
            FONS_TOFU);
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x0394),
            FONS_TOFU);
    }


    /* ==================================================
     * Probare fons_est_mappatum
     * ================================================== */

    {
        imprimere("\n--- Probans fons_est_mappatum ---\n");

        /* ASCII mappatum */
        CREDO_VERUM(fons_est_mappatum('A'));
        CREDO_VERUM(fons_est_mappatum(' '));

        /* Latin-1 mappatum */
        CREDO_VERUM(fons_est_mappatum(0x00E9));  /* e-acutum */

        /* Smart quotes mappatum (substitutio) */
        CREDO_VERUM(fons_est_mappatum(0x201C));  /* left double quote */

        /* Non mappatum */
        CREDO_FALSUM(fons_est_mappatum(0x1F600));  /* emoji */
        CREDO_FALSUM(fons_est_mappatum(0x03C0));   /* pi */
    }


    /* ==================================================
     * Probare nullam glypham mappatam vacuam (2026-10-09)
     * ================================================== */

    {
        s32 cp;
        i32 g;
        i32 r;
        b32 plena;
        i32 vacuae;

        imprimere("\n--- Probans nullam glypham invisibilem ---\n");
        /* lineola mollis consulto invisibilis manet (non TOFU) */
        CREDO_AEQUALIS_S32(glypha(0x00AD), 0xAD);
        /* '×' olim sedem vacuam habebat: invisibile, non TOFU */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x00D7),
            0xD7);
        /* quidquid mappatur pixela habet, praeter consulto vacua:
         * NUL, spatium, lineola mollis (U+00AD) */
        vacuae = ZEPHYRUM;
        per (cp = ZEPHYRUM; cp < 0x3000; cp++)
        {
            g = (i32)glypha(cp);
            si (g == ZEPHYRUM || g == 0x20 || g == 0xAD)
            {
                perge;
            }
            plena = FALSUM;
            per (r = ZEPHYRUM; r < VIII; r++)
            {
                si (fons_6x8[g][r] != ZEPHYRUM)
                {
                    plena = VERUM;
                }
            }
            si (!plena)
            {
                imprimere("  vacua: U+%04X -> 0x%02X\n", (unsigned)cp,
                    (unsigned)g);
                vacuae++;
            }
        }
        CREDO_AEQUALIS_I32(vacuae, ZEPHYRUM);
        /* Latin-1 totum glyphas suas habet (sedes vacua TOFU daret):
         * XL glyphae 2026-10-09 additae hic tenentur */
        vacuae = ZEPHYRUM;
        per (cp = 0x00A1; cp <= 0x00FF; cp++)
        {
            si (   cp         != 0x00AD
                && glypha(cp) != cp)
            {
                imprimere("  sine glypha: U+%04X\n", (unsigned)cp);
                vacuae++;
            }
        }
        CREDO_AEQUALIS_I32(vacuae, ZEPHYRUM);
    }


    /* ==================================================
     * Compendium
     * ================================================== */

    imprimere("\n");
    /* fons_8x16 (V3a): Nix8810 M15 ordine CP437 nativo; 'A' bytes ut
     * in Bm437_Nix8810_M15.FON (fon_ad_fons legit); spatium vacuum;
     * ASCII imprimibile totum pictum; sedes CP437 (0x7F domus, 0x82 e
     * acutum, 0xDB plenum); nihil ultra XVI ordines */
    {
        hic_manens constans i8 a_fontis[XVI] = {
            0x00, 0x38, 0x7C, 0xEE, 0xC6, 0xC6, 0xC6, 0xFE,
            0xFE, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00, 0x00 };
        i32 c;
        i32 r;
        b32 omnes;
        b32 vacua;

        CREDO_AEQUALIS_I32(FONS_8X16_LATITUDO, VIII);
        CREDO_AEQUALIS_I32(FONS_8X16_ALTITUDO, XVI);
        omnes = VERUM;
        per (r = ZEPHYRUM; r < XVI; r++)
        {
            si (fons_8x16['A'][r] != a_fontis[r])
            {
                omnes = FALSUM;
            }
        }
        CREDO_VERUM(omnes);
        vacua = VERUM;
        per (r = ZEPHYRUM; r < XVI; r++)
        {
            si (fons_8x16[' '][r] != ZEPHYRUM)
            {
                vacua = FALSUM;
            }
        }
        CREDO_VERUM(vacua);
        omnes = VERUM;
        per (c = 0x21; c <= 0x7F; c++)
        {
            vacua = VERUM;
            per (r = ZEPHYRUM; r < XVI; r++)
            {
                si (fons_8x16[c][r] != ZEPHYRUM)
                {
                    vacua = FALSUM;
                }
            }
            si (vacua)
            {
                omnes = FALSUM;
            }
        }
        CREDO_VERUM(omnes);
        /* 0xDB plenum: ordines medii omnes 0xFF */
        CREDO_AEQUALIS_I32(fons_8x16[0xDB][VIII], 0xFF);
        /* 0x82 e acutum: aliquid supra (accentus) et in medio */
        CREDO_VERUM(   fons_8x16[0x82][I] != ZEPHYRUM
                    || fons_8x16[0x82][II] != ZEPHYRUM
                    || fons_8x16[0x82][III] != ZEPHYRUM);
    }

    /* V3b: fons_8x16_codepoint_ad_glypham (CP437), metrica, pictura
     * per Mandatum.fons */
    {
        InternamentumChorda* in;
                    Mandata* md;
            TabulaPixelorum* t;
                        i32  r;
                        i32  b;
                        b32  aequalis;

        CREDO_AEQUALIS_S32(g816('A'), 0x41);
        CREDO_AEQUALIS_S32(g816(0x00E9), 0x82);  /* e acutum */
        CREDO_AEQUALIS_S32(g816(0x00BD), 0xAB);  /* dimidium */
        CREDO_AEQUALIS_S32(g816(0x2588), 0xDB);  /* plenum */
        CREDO_AEQUALIS_S32(g816(0x263A), 0x01);  /* facies */
        CREDO_AEQUALIS_S32(g816(0x03C0), 0xE3);  /* pi */
        CREDO_AEQUALIS_S32(g816(0x00C0), 'A');   /* A grave: basis */
        CREDO_AEQUALIS_S32(g816(0x00F8), 'o');   /* o obliquum: basis */
        CREDO_AEQUALIS_S32(g816(0x2019), 0x27);  /* virgula curva */
        CREDO_AEQUALIS_S32(g816(0x2014), 0x2D);  /* lineola longa */
        CREDO_AEQUALIS_S32(g816(0x4E2D), 0x7F);  /* ignotum: domus */
        CREDO_AEQUALIS_S32(g816(-I), 0x7F);
        CREDO_AEQUALIS_S32(fons_latitudo(FONS_PARVUS), VI);
        CREDO_AEQUALIS_S32(fons_altitudo(FONS_PARVUS), VIII);
        CREDO_AEQUALIS_S32(fons_latitudo(FONS_TITULI), VIII);
        CREDO_AEQUALIS_S32(fons_altitudo(FONS_TITULI), XVI);
        CREDO_AEQUALIS_S32(fons_latitudo(XLII), VI);   /* ignotus */
        /* Mandatum cum fons = FONS_TITULI: 'A' 8x16 pixelis veris; 'B'
         * post VIII pixela (progressus) */
        thema_initiare();
        in = internamentum_creare(piscina);
        md = mandata_creare(piscina, in);
        mandata_textus(md, ZEPHYRUM, ZEPHYRUM, chorda_ex_literis("AB",
            piscina), FONS_TITULI, figura_color_thematis(COLOR_TEXT));
        t = tabula_pixelorum_creare_nuda(piscina, XVI, XVI);
        tabula_pixelorum_vacare(t, ZEPHYRUM);
        delineare_mandata(md, t, NIHIL, NIHIL);
        aequalis = VERUM;
        per (r = ZEPHYRUM; r < XVI; r++)
        {
            per (b = ZEPHYRUM; b < XVI; b++)
            {
                b32 debet;

                debet = (fons_8x16[b < VIII ? 'A' : 'B'][r]
                    & (0x80 >> (b % VIII))) != ZEPHYRUM;
                si (debet != (tabula_pixelorum_obtinere_pixelum(t, b, r)
                        != ZEPHYRUM))
                {
                    aequalis = FALSUM;
                }
            }
        }
        CREDO_VERUM(aequalis);
    }

    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    credo_claudere();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
