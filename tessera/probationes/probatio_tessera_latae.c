/* probatio_tessera_latae.c - Cellulae latae (runae U5)
 *
 * Latitudo ex runae_latitudo: runa II cellulas tenet (LATUM +
 * CONTINUATIO in ornamentis, non SGR). Probantur: cellulae "a中b";
 * octeti quadri primi (CUP post cellulam latam - continentia);
 * quadrum immutatum nihil; scriptio super continuationem initium
 * vacuat et vice versa; columna ultima (spatium, numquam scissa);
 * tessera_replere super dimidium; latum super latum translatum; signum
 * componens (latitudo 0) omissum; vexilla ex stilo vocantis ablata.
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

#define LITTERA_SINICA "\xE4\xB8\xAD"   /* U+4E2D, latitudo II */
#define SIGNUM_SINICUM 0xADB8E4

interior TesseraCellula
_cella (
    TesseraOpus* opus,
            s32  x,
            s32  y)
{
    redde tessera_cellulam_legere(opus, x, y);
}

/* Cellula: signum et vexilla latitudinis */
interior b32
_cella_est (
    TesseraOpus* opus,
            s32  x,
            s32  y,
            i32  signum,
            i32  vexilla)
{
    TesseraCellula c = _cella(opus, x, y);
               i32 v = c.ornamenta & (TESSERA_ORNAMENTUM_LATUM
                           | TESSERA_ORNAMENTUM_CONTINUATIO);

    si (c.signum != signum || v != vexilla)
    {
        imprimere("  FRACTA: cellula (%d,%d): signum %06x vexilla %02x, "
            "exspectata %06x et %02x\n", (integer)x, (integer)y,
            (insignatus integer)c.signum, (insignatus integer)v,
            (insignatus integer)signum, (insignatus integer)vexilla);
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

    piscina = piscina_generare_dynamicum("probatio_tessera_latae",
        33554432);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    nat   = tessera_stilus_nativus();
    pm    = tessera_pons_memoriae_creare(piscina, VIII, III);
    opus  = tessera_aperire(piscina, &pm->pons);
    CREDO_NON_NIHIL (opus);

    /* Cellulae "a中b" */
    imprimere("\n--- Cellulae a中b ---\n");
    tessera_scribere_literis(opus, ZEPHYRUM, ZEPHYRUM,
        "a" LITTERA_SINICA "b", nat);
    CREDO_VERUM (_cella_est(opus, ZEPHYRUM, ZEPHYRUM, (i32)'a',
        ZEPHYRUM));
    CREDO_VERUM (_cella_est(opus, I, ZEPHYRUM, SIGNUM_SINICUM,
        TESSERA_ORNAMENTUM_LATUM));
    CREDO_VERUM (_cella_est(opus, II, ZEPHYRUM, ZEPHYRUM,
        TESSERA_ORNAMENTUM_CONTINUATIO));
    CREDO_VERUM (_cella_est(opus, III, ZEPHYRUM, (i32)'b', ZEPHYRUM));

    /* Quadrum primum: CUP ante 'b' (continentia post cellulam latam) */
    {
        chorda captum;

        imprimere("\n--- Quadrum primum (aureum) ---\n");
        CREDO_VERUM (tessera_praesentare(opus));
        captum = tessera_pons_memoriae_captum(pm);
        CREDO_CHORDA_AEQUALIS_LITERIS (captum,
            "\033[?2026h" "\033[?25l\033[2J\033[1;1H\033[0ma" LITTERA_SINICA
            "\033[1;4Hb" "\033[?2026l");
    }

    /* Quadrum immutatum: nihil */
    {
        imprimere("\n--- Quadrum immutatum ---\n");
        tessera_pons_memoriae_purgare(pm);
        CREDO_VERUM (tessera_praesentare(opus));
        CREDO_AEQUALIS_I32 (tessera_pons_memoriae_captum(pm).mensura,
            ZEPHYRUM);
    }

    /* Scriptio super continuationem: initium vacuatur */
    {
        chorda captum;

        imprimere("\n--- Super continuationem ---\n");
        tessera_pons_memoriae_purgare(pm);
        tessera_cellulam_ponere(opus, II, ZEPHYRUM, (i32)'x', nat);
        CREDO_VERUM (_cella_est(opus, I, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM));
        CREDO_VERUM (_cella_est(opus, II, ZEPHYRUM, (i32)'x',
            ZEPHYRUM));
        CREDO_VERUM (tessera_praesentare(opus));
        captum = tessera_pons_memoriae_captum(pm);
        CREDO_CHORDA_AEQUALIS_LITERIS (captum,
            "\033[?2026h" "\033[1;2H\033[0m x" "\033[?2026l");
    }

    /* Scriptio super initium: continuatio vacuatur */
    imprimere("\n--- Super initium ---\n");
    tessera_scribere_literis(opus, I, I, LITTERA_SINICA, nat);
    CREDO_VERUM (_cella_est(opus, I, I, SIGNUM_SINICUM,
        TESSERA_ORNAMENTUM_LATUM));
    tessera_cellulam_ponere(opus, I, I, (i32)'y', nat);
    CREDO_VERUM (_cella_est(opus, I, I, (i32)'y', ZEPHYRUM));
    CREDO_VERUM (_cella_est(opus, II, I, ZEPHYRUM, ZEPHYRUM));

    /* Columna ultima: spatium, numquam scissa */
    imprimere("\n--- Columna ultima ---\n");
    tessera_scribere_literis(opus, VI, II, "a" LITTERA_SINICA, nat);
    CREDO_VERUM (_cella_est(opus, VI, II, (i32)'a', ZEPHYRUM));
    CREDO_VERUM (_cella_est(opus, VII, II, ZEPHYRUM, ZEPHYRUM));

    /* tessera_replere super dimidium */
    imprimere("\n--- Replere super dimidium ---\n");
    tessera_scribere_literis(opus, III, I, LITTERA_SINICA, nat);
    tessera_replere(opus, IV, I, I, I, (i32)'z', nat);
    CREDO_VERUM (_cella_est(opus, III, I, ZEPHYRUM, ZEPHYRUM));
    CREDO_VERUM (_cella_est(opus, IV, I, (i32)'z', ZEPHYRUM));

    /* Latum super latum translatum */
    imprimere("\n--- Latum super latum ---\n");
    tessera_scribere_literis(opus, ZEPHYRUM, II, LITTERA_SINICA, nat);
    tessera_scribere_literis(opus, I, II, LITTERA_SINICA, nat);
    CREDO_VERUM (_cella_est(opus, ZEPHYRUM, II, ZEPHYRUM, ZEPHYRUM));
    CREDO_VERUM (_cella_est(opus, I, II, SIGNUM_SINICUM,
        TESSERA_ORNAMENTUM_LATUM));
    CREDO_VERUM (_cella_est(opus, II, II, ZEPHYRUM,
        TESSERA_ORNAMENTUM_CONTINUATIO));

    /* Signum componens omissum (latitudo 0) */
    imprimere("\n--- Signum componens ---\n");
    tessera_scribere_literis(opus, IV, ZEPHYRUM, "e\xCC\x81q", nat);
    CREDO_VERUM (_cella_est(opus, IV, ZEPHYRUM, (i32)'e', ZEPHYRUM));
    CREDO_VERUM (_cella_est(opus, V, ZEPHYRUM, (i32)'q', ZEPHYRUM));

    /* Vexilla latitudinis ex stilo vocantis ablata */
    imprimere("\n--- Stilus sine vexillis ---\n");
    tessera_cellulam_ponere(opus, VI, ZEPHYRUM, (i32)'w',
        tessera_stilus(TESSERA_COLOR_NATIVUS, TESSERA_COLOR_NATIVUS,
            TESSERA_ORNAMENTUM_CRASSUM | 0xC0));
    CREDO_AEQUALIS_I32 (_cella(opus, VI, ZEPHYRUM).ornamenta,
        TESSERA_ORNAMENTUM_CRASSUM);

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
