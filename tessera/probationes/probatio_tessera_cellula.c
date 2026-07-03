/* probatio_tessera_cellula.c - Cellula, stilus, signum compactum
 * (Phase A)
 *
 * Signum = octeti UTF-8 compacti in i32 (LSB primus): compingere,
 * mensurare, effundere; ASCII pellucidum (compactum == codepoint);
 * constanta linearum recte compacta.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "tessera_cellula.h"
#include "credo.h"
#include <stdio.h>

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_tessera_cellula",
        65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ========================================================
     * PROBARE: compingere + mensura (ASCII pellucidum)
     * ======================================================== */
    {
        hic_manens constans i8 UNUS[I] = { 'a' };
        hic_manens constans i8 TRES[III] = { 0xE2, 0x94, 0x80 };
        hic_manens constans i8 QUATTUOR[IV] = { 0xF0, 0x9F, 0x8E,
            0xB2 };

        imprimere("\n--- Probans signum compactum ---\n");

        CREDO_AEQUALIS_I32 (tessera_signum_ex_octetis(UNUS, I),
            (i32)'a');
        CREDO_AEQUALIS_I32 (tessera_signum_ex_octetis(TRES, III),
            (i32)TESSERA_SIGNUM_SIMPLEX_H);
        CREDO_AEQUALIS_I32 (tessera_signum_ex_octetis(QUATTUOR, IV),
            0xB28E9FF0);

        CREDO_AEQUALIS_I32 (tessera_signum_mensura(ZEPHYRUM),
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (tessera_signum_mensura((i32)'a'), I);
        CREDO_AEQUALIS_I32 (
            tessera_signum_mensura((i32)TESSERA_SIGNUM_SIMPLEX_H),
            III);
        CREDO_AEQUALIS_I32 (tessera_signum_mensura(0xB28E9FF0), IV);

        /* fines: nihil, nimis */
        CREDO_AEQUALIS_I32 (tessera_signum_ex_octetis(NIHIL, I),
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (tessera_signum_ex_octetis(TRES, ZEPHYRUM),
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (tessera_signum_ex_octetis(TRES, V),
            ZEPHYRUM);
    }

    /* ========================================================
     * PROBARE: effusio (vacuum -> spatium; octeti fideles)
     * ======================================================== */
    {
        ChordaAedificator* aed;
        chorda visus;

        imprimere("\n--- Probans effusionem signi ---\n");

        aed = chorda_aedificator_creare(piscina, LXIV);
        tessera_signum_scribere(aed, (i32)'x');
        tessera_signum_scribere(aed, ZEPHYRUM);
        tessera_signum_scribere(aed, (i32)TESSERA_SIGNUM_SIMPLEX_V);
        visus = chorda_aedificator_spectare(aed);
        CREDO_CHORDA_AEQUALIS_LITERIS (visus, "x \xE2\x94\x82");
    }

    /* ========================================================
     * PROBARE: stilus
     * ======================================================== */
    {
        TesseraStilus a;
        TesseraStilus b;

        imprimere("\n--- Probans stilum ---\n");

        a = tessera_stilus_nativus();
        CREDO_AEQUALIS_I32 (a.color_litterae,
            (i32)TESSERA_COLOR_NATIVUS);
        CREDO_AEQUALIS_I32 (a.color_fundi,
            (i32)TESSERA_COLOR_NATIVUS);
        CREDO_AEQUALIS_I32 (a.ornamenta, ZEPHYRUM);

        b = tessera_stilus(0xFF0000, TESSERA_COLOR_NATIVUS,
            TESSERA_ORNAMENTUM_CRASSUM);
        CREDO_VERUM (tessera_stilus_aequalis(a, a));
        CREDO_FALSUM (tessera_stilus_aequalis(a, b));
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();

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
