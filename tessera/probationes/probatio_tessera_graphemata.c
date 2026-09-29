/* probatio_tessera_graphemata.c - Cellulae graphematum (runae U5b)
 *
 * Graphema plurium runarum in cellula (signum = ID internatum,
 * TESSERA_ORNAMENTUM_GRAPHEMA); latitudo graphematis (Ghostty);
 * internatio (eadem graphemata = idem ID); octeti quadri primi cum CUP
 * post graphema (continentia); limes octetorum (ad runam primam
 * redit); signum solum sine basi omissum; status stabilis nihil
 * allocat; scriptio ASCII super graphema vexillum aufert.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "tessera_cellula.h"
#include "tessera_pons.h"
#include "tessera_pons_memoriae.h"
#include "tessera_opus.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define E_ACUTUM   "e\xCC\x81"
#define SYLLABA_PRIMA         "\xE0\xA4\xB9\xE0\xA4\xBF"
#define SYLLABA_CONIUNCTA        "\xE0\xA4\xA8\xE0\xA5\x8D\xE0\xA4\xA6\xE0\xA5\x80"
#define COR_EMOJI  "\xE2\x9D\xA4\xEF\xB8\x8F"
#define FAMILIA    "\xF0\x9F\x91\xA8\xE2\x80\x8D\xF0\x9F\x91\xA9\xE2\x80\x8D" \
                   "\xF0\x9F\x91\xA7"
#define VEXILLA    (TESSERA_ORNAMENTUM_LATUM | TESSERA_ORNAMENTUM_CONTINUATIO \
                    | TESSERA_ORNAMENTUM_GRAPHEMA)

/* Cellula: octeti exspectati et vexilla (LATUM/CONTINUATIO/GRAPHEMA) */
interior b32
_cella_est (
           TesseraOpus* opus,
                   s32  x,
                   s32  y,
    constans character* octeti,
                   i32  vexilla)
{
     i8 lecti[TESSERA_GRAPHEMA_OCTETI_MAXIMI];
    i32 n = tessera_cellulae_octeti(opus, x, y, lecti,
        TESSERA_GRAPHEMA_OCTETI_MAXIMI);
    i32 v = tessera_cellulam_legere(opus, x, y).ornamenta & VEXILLA;

    si (   v != vexilla || n != (i32)strlen(octeti)
        || memcmp(lecti, octeti, (memoriae_index)n) != ZEPHYRUM)
    {
        imprimere("  FRACTA: cellula (%d,%d): %u octeti, vexilla %03x; "
            "exspectati %u, vexilla %03x\n", (integer)x, (integer)y,
            (insignatus integer)n, (insignatus integer)v,
            (insignatus integer)strlen(octeti),
            (insignatus integer)vexilla);
        redde FALSUM;
    }
    redde VERUM;
}

s32
principale (vacuum)
{
                    b32  praeteritus;
                Piscina* piscina;
    TesseraPonsMemoriae* pm;
            TesseraOpus* opus;
          TesseraStilus  nat;

    piscina = piscina_generare_dynamicum("probatio_tessera_graphemata",
        33554432);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    nat   = tessera_stilus_nativus();
    pm    = tessera_pons_memoriae_creare(piscina, XVI, IV);
    opus  = tessera_aperire(piscina, &pm->pons);
    CREDO_NON_NIHIL (opus);

    /* e + acutum: graphema unum, latitudo I; quadrum primum cum CUP */
    {
        chorda captum;

        imprimere("\n--- e + acutum (aureum) ---\n");
        tessera_scribere_literis(opus, ZEPHYRUM, ZEPHYRUM, E_ACUTUM "x",
            nat);
        CREDO_VERUM (_cella_est(opus, ZEPHYRUM, ZEPHYRUM, E_ACUTUM,
            TESSERA_ORNAMENTUM_GRAPHEMA));
        CREDO_VERUM (_cella_est(opus, I, ZEPHYRUM, "x", ZEPHYRUM));
        CREDO_VERUM (tessera_praesentare(opus));
        captum = tessera_pons_memoriae_captum(pm);
        CREDO_CHORDA_AEQUALIS_LITERIS (captum,
            "\033[?2026h" "\033[?25l\033[2J\033[1;1H\033[0m" E_ACUTUM
            "\033[1;2Hx" "\033[?2026l");
    }

    /* Hindi: duo graphemata lata (Mc amplificat; GB9c coniungit) */
    imprimere("\n--- Hindi ---\n");
    tessera_scribere_literis(opus, ZEPHYRUM, I,
        SYLLABA_PRIMA SYLLABA_CONIUNCTA, nat);
    CREDO_VERUM (_cella_est(opus, ZEPHYRUM, I, SYLLABA_PRIMA,
        TESSERA_ORNAMENTUM_GRAPHEMA | TESSERA_ORNAMENTUM_LATUM));
    CREDO_VERUM (_cella_est(opus, I, I, "",
        TESSERA_ORNAMENTUM_CONTINUATIO));
    CREDO_VERUM (_cella_est(opus, II, I, SYLLABA_CONIUNCTA,
        TESSERA_ORNAMENTUM_GRAPHEMA | TESSERA_ORNAMENTUM_LATUM));
    CREDO_VERUM (_cella_est(opus, III, I, "",
        TESSERA_ORNAMENTUM_CONTINUATIO));

    /* Super continuationem graphematis lati: initium vacuatur ET
     * vexillum GRAPHEMA aufertur (aliter signum 0 = ID alienum) */
    tessera_cellulam_ponere(opus, I, I, (i32)'q', nat);
    CREDO_VERUM (_cella_est(opus, ZEPHYRUM, I, "", ZEPHYRUM));
    CREDO_VERUM (_cella_est(opus, I, I, "q", ZEPHYRUM));

    /* Emoji: cor cum VS16 et familia ZWJ - graphemata lata */
    imprimere("\n--- Emoji ---\n");
    tessera_scribere_literis(opus, ZEPHYRUM, II, COR_EMOJI FAMILIA "z",
        nat);
    CREDO_VERUM (_cella_est(opus, ZEPHYRUM, II, COR_EMOJI,
        TESSERA_ORNAMENTUM_GRAPHEMA | TESSERA_ORNAMENTUM_LATUM));
    CREDO_VERUM (_cella_est(opus, II, II, FAMILIA,
        TESSERA_ORNAMENTUM_GRAPHEMA | TESSERA_ORNAMENTUM_LATUM));
    CREDO_VERUM (_cella_est(opus, IV, II, "z", ZEPHYRUM));

    /* Internatio: idem graphema = idem ID */
    imprimere("\n--- Internatio ---\n");
    tessera_scribere_literis(opus, V, ZEPHYRUM, E_ACUTUM, nat);
    CREDO_AEQUALIS_I32 (tessera_cellulam_legere(opus, V,
        ZEPHYRUM).signum,
        tessera_cellulam_legere(opus, ZEPHYRUM, ZEPHYRUM).signum);

    /* Limes octetorum: a + XL acuta (LXXXI octeti) -> runa prima sola */
    {
        character longum[CXXVIII];
              i32 k;

        imprimere("\n--- Limes octetorum ---\n");
        longum[ZEPHYRUM] = 'a';
        per (k = ZEPHYRUM; k < XL; k++)
        {
            longum[I + k * II]      = (character)0xCC;
            longum[I + k * II + I]  = (character)0x81;
        }
        longum[LXXXI] = '\0';
        tessera_scribere_literis(opus, X, III, longum, nat);
        CREDO_VERUM (_cella_est(opus, X, III, "a", ZEPHYRUM));
    }

    /* Signum solum sine basi: omissum */
    imprimere("\n--- Signum sine basi ---\n");
    tessera_scribere_literis(opus, ZEPHYRUM, III, "\xCC\x81q", nat);
    CREDO_VERUM (_cella_est(opus, ZEPHYRUM, III, "q", ZEPHYRUM));

    /* Status stabilis: graphemata nota iterum pingere nihil allocat */
    {
        memoriae_index ante;
        memoriae_index post;

        imprimere("\n--- Status stabilis ---\n");
        (vacuum)tessera_praesentare(opus);
        ante = piscina_summa_usus(piscina);
        tessera_scribere_literis(opus, ZEPHYRUM, I,
            SYLLABA_PRIMA SYLLABA_CONIUNCTA, nat);
        tessera_scribere_literis(opus, ZEPHYRUM, II, COR_EMOJI FAMILIA,
            nat);
        (vacuum)tessera_praesentare(opus);
        post = piscina_summa_usus(piscina);
        CREDO_AEQUALIS_I32 ((i32)post, (i32)ante);
    }

    /* ASCII super graphema: vexillum aufertur */
    imprimere("\n--- ASCII super graphema ---\n");
    tessera_cellulam_ponere(opus, ZEPHYRUM, ZEPHYRUM, (i32)'q', nat);
    CREDO_VERUM (_cella_est(opus, ZEPHYRUM, ZEPHYRUM, "q", ZEPHYRUM));

        /* Politica SIMPLEX (Terminal.app): hi manet latum (II);
     * familia = tria emoji lata (ZWJ non iungit) */
    {
        TesseraPonsMemoriae* pm_s =
            tessera_pons_memoriae_creare(piscina,
            XVI, II);
        TesseraOpus* s = tessera_aperire(piscina, &pm_s->pons);

        imprimere("\n--- Politica SIMPLEX ---\n");
        CREDO_NON_NIHIL (s);
        tessera_politicam_ponere(s, TESSERA_POLITICA_SIMPLEX);
        tessera_scribere_literis(s, ZEPHYRUM, ZEPHYRUM,
            SYLLABA_PRIMA "x",
            nat);
        CREDO_VERUM (_cella_est(s, ZEPHYRUM, ZEPHYRUM, SYLLABA_PRIMA,
            TESSERA_ORNAMENTUM_GRAPHEMA | TESSERA_ORNAMENTUM_LATUM));
        CREDO_VERUM (_cella_est(s, II, ZEPHYRUM, "x", ZEPHYRUM));
        tessera_scribere_literis(s, ZEPHYRUM, I, FAMILIA, nat);
        CREDO_VERUM (_cella_est(s, ZEPHYRUM, I,
            "\xF0\x9F\x91\xA8\xE2\x80\x8D",
            TESSERA_ORNAMENTUM_GRAPHEMA | TESSERA_ORNAMENTUM_LATUM));
        CREDO_VERUM (_cella_est(s, II, I,
            "\xF0\x9F\x91\xA9\xE2\x80\x8D",
            TESSERA_ORNAMENTUM_GRAPHEMA | TESSERA_ORNAMENTUM_LATUM));
        CREDO_VERUM (_cella_est(s, IV, I, "\xF0\x9F\x91\xA7",
            TESSERA_ORNAMENTUM_LATUM));
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
