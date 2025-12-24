/* probatio_fons.c - Probationes mappationis glypharum */
#include "latina.h"
#include "fons.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>

s32 principale(vacuum)
{
    Piscina* piscina;
    b32 praeteritus;

    piscina = piscina_generare_dynamicum("test_fons", MMMMXCVI);
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
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x00E9), 0xE9);  /* e-acutum */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x00F1), 0xF1);  /* n-tilde */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x00FC), 0xFC);  /* u-umlaut */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x00E0), 0xE0);  /* a-grave */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x00C9), 0xC9);  /* E-acutum */

        /* Limites Latin-1 supplement */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x00A0), 0x20);  /* NBSP -> space */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x00A1), 0xA1);  /* inverted ! */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x00FF), 0xFF);  /* y-umlaut */
    }


    /* ==================================================
     * Probare smart quotes -> rectae
     * ================================================== */

    {
        imprimere("\n--- Probans smart quotes ---\n");

        /* Single quotes -> apostrophe */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x2018), 0x27);  /* ' left */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x2019), 0x27);  /* ' right */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x201A), 0x27);  /* ‚ low-9 */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x201B), 0x27);  /* ‛ reversed */

        /* Double quotes -> quotation mark */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x201C), 0x22);  /* " left */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x201D), 0x22);  /* " right */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x201E), 0x22);  /* „ low-9 */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x201F), 0x22);  /* ‟ reversed */
    }


    /* ==================================================
     * Probare dashes -> hyphen
     * ================================================== */

    {
        imprimere("\n--- Probans dashes ---\n");

        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x2013), 0x2D);  /* en dash */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x2014), 0x2D);  /* em dash */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x2015), 0x2D);  /* horizontal bar */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x2212), 0x2D);  /* minus sign */
    }


    /* ==================================================
     * Probare ellipsis et alia
     * ================================================== */

    {
        imprimere("\n--- Probans ellipsis et alia ---\n");

        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x2026), 0x2E);  /* ellipsis */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x2022), 0x07);  /* bullet */
    }


    /* ==================================================
     * Probare codepoints ignoti -> TOFU
     * ================================================== */

    {
        imprimere("\n--- Probans TOFU ---\n");

        /* Codepoints invalidi */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(-I), FONS_TOFU);

        /* Codepoints non mappati (e.g. emoji, CJK) */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x1F600), FONS_TOFU);  /* emoji */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x4E00), FONS_TOFU);   /* CJK */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x0100), FONS_TOFU);   /* Latin Extended-A */

        /* Graeca (non mappata) */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x03B1), FONS_TOFU);  /* alpha */
        CREDO_AEQUALIS_S32((s32)fons_codepoint_ad_glypham(0x03C0), FONS_TOFU);  /* pi */
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
     * Compendium
     * ================================================== */

    imprimere("\n");
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
