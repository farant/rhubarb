/* probatio_tessera_salve.c - Harnesa tesserae + pignora dependentiarum
 * (Phase 0)
 *
 * Plus quam salutatio: quattuor mores dependentiarum PIGNERANTUR in
 * quibus consilium spec-v2 nititur —
 *   (a) aedificator: exemplar per-quadrum (spectare + reset, buffer
 *       retentus, appendere post reset valet),
 *   (b) compositio effugii ANSI (appendere_character + appendere_s32),
 *   (c) utf8: decodere + proxima_runa in signo trium octetorum (ambulatio
 *       signi compacti),
 *   (d) piscina_allocare_ordinatum ordinationem revera dat (allocare
 *       nudum octetim compactum est - lapsus notus).
 * Si mos dependentiae labitur, suite tesserae id dicit ANTEQUAM codex
 * tesserae ipse erret.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "utf8.h"
#include "credo.h"
#include <stdio.h>

s32
principale (vacuum)
{
        b32  praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_tessera_salve",
        65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: harnesa + bibliothecae radicis attinguntur
     * ======================================================== */

    {
        chorda salve;

        imprimere("\n--- Probans harnesam tesserae ---\n");

        CREDO_AEQUALIS_S32 (II + II, IV);
        salve = chorda_ex_literis("tessera", piscina);
        CREDO_AEQUALIS_I32 (salve.mensura, VII);
        CREDO_CHORDA_AEQUALIS_LITERIS (salve, "tessera");
    }


    /* ========================================================
     * PROBARE: (a)+(b) aedificator - exemplar per-quadrum et
     * compositio effugii (spec-v2 par 1.4)
     * ======================================================== */

    {
        ChordaAedificator* aed;
                   chorda  visus;

        imprimere("\n--- Probans aedificatorem (exemplar quadri) ---\n");

        aed = chorda_aedificator_creare(piscina, CCLVI);
        CREDO_NON_NIHIL (aed);

        /* effugium ANSI: ESC [ 12 ; 34 H */
        chorda_aedificator_appendere_character(aed, '\033');
        chorda_aedificator_appendere_character(aed, '[');
        chorda_aedificator_appendere_s32(aed, XII);
        chorda_aedificator_appendere_character(aed, ';');
        chorda_aedificator_appendere_s32(aed, XXXIV);
        chorda_aedificator_appendere_character(aed, 'H');
        visus = chorda_aedificator_spectare(aed);
        CREDO_CHORDA_AEQUALIS_LITERIS (visus, "\033[12;34H");

        /* reset: contentum abit, buffer manet, appendere valet */
        chorda_aedificator_reset(aed);
        CREDO_AEQUALIS_I32 (
            (i32)chorda_aedificator_longitudo(aed), ZEPHYRUM);
        chorda_aedificator_appendere_literis(aed, "post");
        visus = chorda_aedificator_spectare(aed);
        CREDO_CHORDA_AEQUALIS_LITERIS (visus, "post");
    }


    /* ========================================================
     * PROBARE: (c) utf8 - signum trium octetorum (U+2500 "─",
     * E2 94 80) decoditur et ambulatur (spec-v2 par 1.3)
     * ======================================================== */

    {
        hic_manens constans i8  LINEA[IV] = { 0xE2, 0x94, 0x80, 'x' };
                   constans i8* cursor;
                   constans i8* finis;
                           s32  runa;

        imprimere("\n--- Probans utf8 (signum compactum) ---\n");

        CREDO_AEQUALIS_S32 (utf8_longitudo_byte(LINEA[ZEPHYRUM]), III);
        CREDO_VERUM (utf8_est_continuatio(LINEA[I]));

        cursor  = LINEA;
        finis   = LINEA + IV;
        runa    = utf8_decodere(&cursor, finis);
        CREDO_AEQUALIS_S32 (runa, 0x2500);
        CREDO_AEQUALIS_PTR ((constans vacuum*)cursor,
            (constans vacuum*)(LINEA + III));

        cursor = utf8_proxima_runa(LINEA, finis);
        CREDO_AEQUALIS_PTR ((constans vacuum*)cursor,
            (constans vacuum*)(LINEA + III));
        CREDO_AEQUALIS_S32 (utf8_numerare_runas(LINEA, IV), II);
    }


    /* ========================================================
     * PROBARE: (d) ordinatio - allocare nudum compactum est,
     * ordinatum ordinat (spec-v2 par 1.5; cratis cellularum!)
     * ======================================================== */

    {
        vacuum* primus;
        vacuum* ordinatus;

        imprimere("\n--- Probans ordinationem piscinae ---\n");

        /* offset impar deliberatus */
        primus = piscina_allocare(piscina, III);
        CREDO_NON_NIHIL (primus);
        ordinatus = piscina_allocare_ordinatum(piscina, LXIV, XVI);
        CREDO_NON_NIHIL (ordinatus);
        CREDO_AEQUALIS_I32 (
            (i32)((memoriae_index)ordinatus % XVI), ZEPHYRUM);
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
