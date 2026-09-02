/* probatio_tessera_eventum.c - Lector + parsator initus (Phase B)
 *
 * Omnis logica parsationis per pontem memoriae: scripta octetorum
 * -> series eventorum. ASCII, UTF-8 (incl. invalidum), regimen
 * (REDITUS/TABULA/RETRORSUM/imperium+littera), CSI (frecce,
 * ~-codices, modificatores, Z), SS3, ALTERUM praefixum, mus SGR
 * (pressus/solutus/rota), CSI ignota tacite, FUGA sola per moram,
 * eventa plura gestata, AMPLITUDO (interrogatio), RESUMPTUM
 * (vexillum), intermittere/resumere (numeri + pictura plena).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tessera_cellula.h"
#include "tessera_pons.h"
#include "tessera_pons_memoriae.h"
#include "tessera_eventum.h"
#include "tessera_opus.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior TesseraPonsMemoriae* pm_globalis;
interior TesseraLector* lector_globalis;

/* Scriptum ponere et eventum unum expectare */
interior TesseraEventum
_unum (
    constans character* octeti)
{
    TesseraEventum ev;

    si (octeti != NIHIL)
    {
        tessera_pons_memoriae_initum(pm_globalis,
            (constans i8*)octeti, (i32)strlen(octeti));
    }
    tessera_eventum_expectare(lector_globalis, &ev, X);
    redde ev;
}

s32
principale (vacuum)
{
        b32  praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_tessera_eventum",
        4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    pm_globalis = tessera_pons_memoriae_creare(piscina, LXXX, XXIV);
    CREDO_NON_NIHIL (pm_globalis);
    lector_globalis = tessera_lector_creare(piscina,
        &pm_globalis->pons);
    CREDO_NON_NIHIL (lector_globalis);


    /* ========================================================
     * PROBARE: runae - ASCII, UTF-8, invalidum
     * ======================================================== */

    {
        TesseraEventum ev;

        imprimere("\n--- Probans runas ---\n");

        ev = _unum("a");
        CREDO_AEQUALIS_I32 ((i32)ev.genus,
            (i32)TESSERA_EVENTUM_CLAVIS);
        CREDO_AEQUALIS_S32 (ev.runa, (s32)'a');
        CREDO_AEQUALIS_I32 (ev.modificatores, ZEPHYRUM);

        /* e cum acuto: C3 A9 = U+00E9 */
        ev = _unum("\xC3\xA9");
        CREDO_AEQUALIS_S32 (ev.runa, 0xE9);

        /* signum trium octetorum */
        ev = _unum("\xE2\x94\x80");
        CREDO_AEQUALIS_S32 (ev.runa, 0x2500);

        /* octetus invalidus abicitur, runa sequens valet */
        ev = _unum("\xFF" "b");
        CREDO_AEQUALIS_S32 (ev.runa, (s32)'b');
    }


    /* ========================================================
     * PROBARE: regimen
     * ======================================================== */

    {
        TesseraEventum ev;

        imprimere("\n--- Probans regimen ---\n");

        ev = _unum("\r");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_REDITUS);
        ev = _unum("\n");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_REDITUS);
        ev = _unum("\t");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_TABULA);
        ev = _unum("\x7F");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_RETRORSUM);

        /* Ctrl+C = 0x03 */
        ev = _unum("\x03");
        CREDO_AEQUALIS_S32 (ev.runa, (s32)'c');
        CREDO_AEQUALIS_I32 (ev.modificatores,
            TESSERA_MODIFICATOR_IMPERIUM);
    }


    /* ========================================================
     * PROBARE: CSI - frecce, ~-codices, modificatores, Z
     * ======================================================== */

    {
        TesseraEventum ev;

        imprimere("\n--- Probans CSI ---\n");

        ev = _unum("\033[A");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_SURSUM);
        ev = _unum("\033[D");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_SINISTRA);
        ev = _unum("\033[H");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_DOMUS);

        ev = _unum("\033[3~");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_DELETIO);
        ev = _unum("\033[5~");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_PAGINA_SURSUM);
        ev = _unum("\033[2~");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_INSERTIO);

        /* F5 = 15~, F6 = 17~, F12 = 24~ */
        ev = _unum("\033[15~");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_FUNCTIO);
        CREDO_AEQUALIS_I32 (ev.numerus, V);
        ev = _unum("\033[17~");
        CREDO_AEQUALIS_I32 (ev.numerus, VI);
        ev = _unum("\033[24~");
        CREDO_AEQUALIS_I32 (ev.numerus, XII);

        /* modificatores: 1;5C = imperium+dextra; 3;2~ =
         * maiuscula+deletio */
        ev = _unum("\033[1;5C");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_DEXTRA);
        CREDO_AEQUALIS_I32 (ev.modificatores,
            TESSERA_MODIFICATOR_IMPERIUM);
        ev = _unum("\033[3;2~");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_DELETIO);
        CREDO_AEQUALIS_I32 (ev.modificatores,
            TESSERA_MODIFICATOR_MAIUSCULA);

        /* Z = tabula retro */
        ev = _unum("\033[Z");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_TABULA);
        CREDO_AEQUALIS_I32 (ev.modificatores,
            TESSERA_MODIFICATOR_MAIUSCULA);
    }


    /* ========================================================
     * PROBARE: SS3 + ALTERUM
     * ======================================================== */

    {
        TesseraEventum ev;

        imprimere("\n--- Probans SS3 et ALTERUM ---\n");

        ev = _unum("\033OB");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_DEORSUM);
        ev = _unum("\033OP");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_FUNCTIO);
        CREDO_AEQUALIS_I32 (ev.numerus, I);

        /* alt+b */
        ev = _unum("\033b");
        CREDO_AEQUALIS_S32 (ev.runa, (s32)'b');
        CREDO_AEQUALIS_I32 (ev.modificatores,
            TESSERA_MODIFICATOR_ALTERUM);

        /* alt+reditus */
        ev = _unum("\033\r");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_REDITUS);
        CREDO_AEQUALIS_I32 (ev.modificatores,
            TESSERA_MODIFICATOR_ALTERUM);
    }


    /* ========================================================
     * PROBARE: mus SGR
     * ======================================================== */

    {
        TesseraEventum ev;

        imprimere("\n--- Probans murem ---\n");

        ev = _unum("\033[<0;5;3M");
        CREDO_AEQUALIS_I32 ((i32)ev.genus, (i32)TESSERA_EVENTUM_MUS);
        CREDO_AEQUALIS_I32 ((i32)ev.mus_genus,
            (i32)TESSERA_MUS_PRESSUS);
        CREDO_AEQUALIS_S32 (ev.mus_x, IV);   /* 1-basata -> 0 */
        CREDO_AEQUALIS_S32 (ev.mus_y, II);
        CREDO_AEQUALIS_I32 (ev.mus_pulsus, ZEPHYRUM);

        ev = _unum("\033[<2;1;1m");
        CREDO_AEQUALIS_I32 ((i32)ev.mus_genus,
            (i32)TESSERA_MUS_SOLUTUS);
        CREDO_AEQUALIS_I32 (ev.mus_pulsus, II);

        ev = _unum("\033[<64;2;2M");
        CREDO_AEQUALIS_I32 ((i32)ev.mus_genus,
            (i32)TESSERA_MUS_ROTA_SURSUM);
        ev = _unum("\033[<65;2;2M");
        CREDO_AEQUALIS_I32 ((i32)ev.mus_genus,
            (i32)TESSERA_MUS_ROTA_DEORSUM);
    }


    /* ========================================================
     * PROBARE: CSI ignota tacite; eventa plura gestata; FUGA
     * sola per moram; NIHIL in vacuo
     * ======================================================== */

    {
        TesseraEventum ev;

        imprimere("\n--- Probans robustitatem ---\n");

        /* CSI ignota (privata '?') consumitur, clavis sequens
         * valet */
        ev = _unum("\033[?1049h" "x");
        CREDO_AEQUALIS_S32 (ev.runa, (s32)'x');

        /* tria eventa uno scripto */
        ev = _unum("ab\033[A");
        CREDO_AEQUALIS_S32 (ev.runa, (s32)'a');
        ev = _unum(NIHIL);
        CREDO_AEQUALIS_S32 (ev.runa, (s32)'b');
        ev = _unum(NIHIL);
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_SURSUM);

        /* FUGA sola: scriptum exhaustum = mora -> FUGA */
        ev = _unum("\033");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_FUGA);

        /* vacuum = NIHIL */
        ev = _unum(NIHIL);
        CREDO_AEQUALIS_I32 ((i32)ev.genus,
            (i32)TESSERA_EVENTUM_NIHIL);
    }


    /* ========================================================
     * PROBARE: AMPLITUDO (interrogatio) + RESUMPTUM (vexillum)
     * ======================================================== */

    {
        TesseraEventum ev;

        imprimere("\n--- Probans amplitudinem et resumptum ---\n");

        tessera_pons_memoriae_amplitudo(pm_globalis, XL, XII);
        ev = _unum(NIHIL);
        CREDO_AEQUALIS_I32 ((i32)ev.genus,
            (i32)TESSERA_EVENTUM_AMPLITUDO);
        CREDO_AEQUALIS_I32 (ev.latitudo, XL);
        CREDO_AEQUALIS_I32 (ev.altitudo, XII);

        /* semel tantum */
        ev = _unum(NIHIL);
        CREDO_AEQUALIS_I32 ((i32)ev.genus,
            (i32)TESSERA_EVENTUM_NIHIL);

        pm_globalis->resumendum  = VERUM;
        ev                       = _unum(NIHIL);
        CREDO_AEQUALIS_I32 ((i32)ev.genus,
            (i32)TESSERA_EVENTUM_RESUMPTUM);
        ev = _unum(NIHIL);
        CREDO_AEQUALIS_I32 ((i32)ev.genus,
            (i32)TESSERA_EVENTUM_NIHIL);
    }


    /* ========================================================
     * PROBARE: intermittere/resumere - numeri pontis + pictura
     * plena post resumptionem
     * ======================================================== */

    {
        TesseraPonsMemoriae* pi = tessera_pons_memoriae_creare(
            piscina, VIII, III);
        TesseraOpus* opus;
             chorda  captum;

        imprimere("\n--- Probans intermittere/resumere ---\n");

        CREDO_NON_NIHIL (pi);
        opus = tessera_aperire(piscina, &pi->pons);
        CREDO_NON_NIHIL (opus);
        tessera_scribere_literis(opus, ZEPHYRUM, ZEPHYRUM, "st",
            tessera_stilus_nativus());
        CREDO_VERUM (tessera_praesentare(opus));

        tessera_pons_memoriae_purgare(pi);
        tessera_intermittere(opus);
        CREDO_FALSUM (pi->intratum);
        CREDO_AEQUALIS_I32 (pi->numerus_exitum, I);
        captum = tessera_pons_memoriae_captum(pi);
        CREDO_CHORDA_AEQUALIS_LITERIS (captum, "\033[0m\033[?25h");

        tessera_resumere(opus);
        CREDO_VERUM (pi->intratum);
        CREDO_AEQUALIS_I32 (pi->numerus_intratum, II);

        /* pictura plena: quadrum proximum ED emittit et omnia
         * pingit */
        tessera_pons_memoriae_purgare(pi);
        CREDO_VERUM (tessera_praesentare(opus));
        captum = tessera_pons_memoriae_captum(pi);
        CREDO_CHORDA_AEQUALIS_LITERIS (captum,
            "\033[?25l\033[2J\033[1;1H\033[0mst");
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
