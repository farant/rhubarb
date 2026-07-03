/* probatio_tessera_opus.c - Opus: pictura, differentia, emissio
 * (Phase A)
 *
 * Probationes aureae octetorum effugii (crates parva 8x3 per pontem
 * memoriae): quadrum primum (celare + ED + pictura), quadrum
 * minimum (cellula una = CUP + SGR + signum), coalitio SGR (cursus
 * eiusdem stili = SGR unum), quadrum vacuum = NIHIL emissum,
 * protocollum cursoris, praecisio, scriptio UTF-8, renovatio
 * magnitudinis, ars linearis, fructus + ASSERTIO APICIS (status
 * stabilis nihil allocat - spec-v2 par 1.4/1.5).
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

interior TesseraStilus
_nativus (vacuum)
{
    redde tessera_stilus_nativus();
}

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;
    TesseraPonsMemoriae* pm;
    TesseraOpus* opus;

    piscina = piscina_generare_dynamicum("probatio_tessera_opus",
        33554432);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    pm = tessera_pons_memoriae_creare(piscina, VIII, III);
    CREDO_NON_NIHIL (pm);
    opus = tessera_aperire(piscina, &pm->pons);
    CREDO_NON_NIHIL (opus);

    /* ========================================================
     * PROBARE: aperire - amplitudo ex ponte, modus intratus
     * ======================================================== */
    {
        imprimere("\n--- Probans aperire ---\n");

        CREDO_AEQUALIS_I32 (tessera_latitudo(opus), VIII);
        CREDO_AEQUALIS_I32 (tessera_altitudo(opus), III);
        CREDO_VERUM (pm->intratum);
        CREDO_AEQUALIS_I32 (
            tessera_cellulam_legere(opus, ZEPHYRUM, ZEPHYRUM).signum,
            ZEPHYRUM);
    }

    /* ========================================================
     * PROBARE: quadrum primum aureum - celare + ED + CUP + SGR
     * + signa (cellulae vacuae praeteritae - 2J eas pinxit)
     * ======================================================== */
    {
        chorda captum;

        imprimere("\n--- Probans quadrum primum (aureum) ---\n");

        tessera_scribere_literis(opus, ZEPHYRUM, ZEPHYRUM, "ab",
            _nativus());
        CREDO_VERUM (tessera_praesentare(opus));
        captum = tessera_pons_memoriae_captum(pm);
        CREDO_CHORDA_AEQUALIS_LITERIS (captum,
            "\033[?25l\033[2J\033[1;1H\033[0mab");
    }

    /* ========================================================
     * PROBARE: quadrum minimum aureum - cellula una mutata
     * ======================================================== */
    {
        chorda captum;

        imprimere("\n--- Probans quadrum minimum (aureum) ---\n");

        tessera_pons_memoriae_purgare(pm);
        tessera_cellulam_ponere(opus, III, I, (i32)'z',
            tessera_stilus(0xFF0000, TESSERA_COLOR_NATIVUS,
                ZEPHYRUM));
        CREDO_VERUM (tessera_praesentare(opus));
        captum = tessera_pons_memoriae_captum(pm);
        CREDO_CHORDA_AEQUALIS_LITERIS (captum,
            "\033[2;4H\033[0;38;2;255;0;0mz");
    }

    /* ========================================================
     * PROBARE: coalitio SGR - cellulae vicinae eiusdem stili =
     * CUP unum + SGR unum + signa
     * ======================================================== */
    {
        chorda captum;

        imprimere("\n--- Probans coalitionem SGR ---\n");

        tessera_pons_memoriae_purgare(pm);
        tessera_cellulam_ponere(opus, ZEPHYRUM, II, (i32)'x',
            _nativus());
        tessera_cellulam_ponere(opus, I, II, (i32)'y', _nativus());
        CREDO_VERUM (tessera_praesentare(opus));
        captum = tessera_pons_memoriae_captum(pm);
        CREDO_CHORDA_AEQUALIS_LITERIS (captum, "\033[3;1H\033[0mxy");
    }

    /* ========================================================
     * PROBARE: quadrum sine mutationibus = NIHIL emissum
     * ======================================================== */
    {
        chorda captum;

        imprimere("\n--- Probans quadrum vacuum ---\n");

        tessera_pons_memoriae_purgare(pm);
        CREDO_VERUM (tessera_praesentare(opus));
        captum = tessera_pons_memoriae_captum(pm);
        CREDO_AEQUALIS_I32 (captum.mensura, ZEPHYRUM);
    }

    /* ========================================================
     * PROBARE: protocollum cursoris - ponere/monstrare/celare;
     * optatum immutatum = nihil
     * ======================================================== */
    {
        chorda captum;

        imprimere("\n--- Probans cursorem ---\n");

        tessera_pons_memoriae_purgare(pm);
        tessera_cursorem_ponere(opus, II, I);
        CREDO_VERUM (tessera_praesentare(opus));
        captum = tessera_pons_memoriae_captum(pm);
        CREDO_CHORDA_AEQUALIS_LITERIS (captum, "\033[2;3H\033[?25h");

        tessera_pons_memoriae_purgare(pm);
        CREDO_VERUM (tessera_praesentare(opus));
        captum = tessera_pons_memoriae_captum(pm);
        CREDO_AEQUALIS_I32 (captum.mensura, ZEPHYRUM);

        tessera_pons_memoriae_purgare(pm);
        tessera_cursorem_ponere(opus, -I, -I);
        CREDO_VERUM (tessera_praesentare(opus));
        captum = tessera_pons_memoriae_captum(pm);
        CREDO_CHORDA_AEQUALIS_LITERIS (captum, "\033[?25l");
    }

    /* ========================================================
     * PROBARE: praecisio taciturna extra fines
     * ======================================================== */
    {
        chorda captum;

        imprimere("\n--- Probans praecisionem ---\n");

        tessera_pons_memoriae_purgare(pm);
        tessera_cellulam_ponere(opus, -I, ZEPHYRUM, (i32)'q',
            _nativus());
        tessera_cellulam_ponere(opus, VIII, ZEPHYRUM, (i32)'q',
            _nativus());
        tessera_cellulam_ponere(opus, ZEPHYRUM, III, (i32)'q',
            _nativus());
        CREDO_VERUM (tessera_praesentare(opus));
        captum = tessera_pons_memoriae_captum(pm);
        CREDO_AEQUALIS_I32 (captum.mensura, ZEPHYRUM);
    }

    /* ========================================================
     * PROBARE: scriptio UTF-8 - runa trium octetorum cellula una;
     * octetus invalidus + regiminis -> '?'
     * ======================================================== */
    {
        imprimere("\n--- Probans scriptionem UTF-8 ---\n");

        tessera_scribere_literis(opus, ZEPHYRUM, ZEPHYRUM,
            "a\xE2\x94\x80" "b", _nativus());
        CREDO_AEQUALIS_I32 (
            tessera_cellulam_legere(opus, ZEPHYRUM, ZEPHYRUM).signum,
            (i32)'a');
        CREDO_AEQUALIS_I32 (
            tessera_cellulam_legere(opus, I, ZEPHYRUM).signum,
            (i32)TESSERA_SIGNUM_SIMPLEX_H);
        CREDO_AEQUALIS_I32 (
            tessera_cellulam_legere(opus, II, ZEPHYRUM).signum,
            (i32)'b');

        tessera_scribere_literis(opus, ZEPHYRUM, I, "\xFF\t",
            _nativus());
        CREDO_AEQUALIS_I32 (
            tessera_cellulam_legere(opus, ZEPHYRUM, I).signum,
            (i32)'?');
        CREDO_AEQUALIS_I32 (
            tessera_cellulam_legere(opus, I, I).signum, (i32)'?');
    }

    /* ========================================================
     * PROBARE: ars linearis - quadrum + linea
     * ======================================================== */
    {
        imprimere("\n--- Probans artem linearem ---\n");

        tessera_purgare(opus, _nativus());
        tessera_quadrum_pingere(opus, ZEPHYRUM, ZEPHYRUM, IV, III,
            TESSERA_LINEA_SIMPLEX, _nativus());
        CREDO_AEQUALIS_I32 (
            tessera_cellulam_legere(opus, ZEPHYRUM, ZEPHYRUM).signum,
            (i32)TESSERA_SIGNUM_SIMPLEX_SS);
        CREDO_AEQUALIS_I32 (
            tessera_cellulam_legere(opus, III, ZEPHYRUM).signum,
            (i32)TESSERA_SIGNUM_SIMPLEX_SD);
        CREDO_AEQUALIS_I32 (
            tessera_cellulam_legere(opus, ZEPHYRUM, II).signum,
            (i32)TESSERA_SIGNUM_SIMPLEX_IS);
        CREDO_AEQUALIS_I32 (
            tessera_cellulam_legere(opus, III, II).signum,
            (i32)TESSERA_SIGNUM_SIMPLEX_ID);
        CREDO_AEQUALIS_I32 (
            tessera_cellulam_legere(opus, I, ZEPHYRUM).signum,
            (i32)TESSERA_SIGNUM_SIMPLEX_H);
        CREDO_AEQUALIS_I32 (
            tessera_cellulam_legere(opus, ZEPHYRUM, I).signum,
            (i32)TESSERA_SIGNUM_SIMPLEX_V);
        /* interior intactum */
        CREDO_AEQUALIS_I32 (
            tessera_cellulam_legere(opus, I, I).signum, ZEPHYRUM);

        tessera_lineam_pingere(opus, V, ZEPHYRUM, III, VERUM,
            TESSERA_LINEA_DUPLEX, _nativus());
        CREDO_AEQUALIS_I32 (
            tessera_cellulam_legere(opus, V, II).signum,
            (i32)TESSERA_SIGNUM_DUPLEX_V);
    }

    /* ========================================================
     * PROBARE: renovatio magnitudinis - amplitudo nova, pictura
     * plena sequitur
     * ======================================================== */
    {
        chorda captum;

        imprimere("\n--- Probans renovationem magnitudinis ---\n");

        tessera_pons_memoriae_amplitudo(pm, IV, II);
        CREDO_VERUM (tessera_magnitudinem_renovare(opus));
        CREDO_AEQUALIS_I32 (tessera_latitudo(opus), IV);
        CREDO_AEQUALIS_I32 (tessera_altitudo(opus), II);

        tessera_pons_memoriae_purgare(pm);
        tessera_purgare(opus, _nativus());
        tessera_scribere_literis(opus, ZEPHYRUM, ZEPHYRUM, "r",
            _nativus());
        CREDO_VERUM (tessera_praesentare(opus));
        captum = tessera_pons_memoriae_captum(pm);
        CREDO_CHORDA_AEQUALIS_LITERIS (captum,
            "\033[?25l\033[2J\033[1;1H\033[0mr");
    }

    /* ========================================================
     * PROBARE: fructus + ASSERTIO APICIS - post calefactionem
     * quadra ulteriora NIHIL allocant (spectare + reset)
     * ======================================================== */
    {
        Piscina* piscina_apicis = piscina_generare_dynamicum(
            "apex_tessera", 33554432);
        TesseraPonsMemoriae* pa;
        TesseraOpus* oa;
        memoriae_index apex_calidus;
        i32 k;

        imprimere("\n--- Probans fructum + apicem ---\n");

        CREDO_NON_NIHIL (piscina_apicis);
        pa = tessera_pons_memoriae_creare(piscina_apicis, XL, XII);
        oa = tessera_aperire(piscina_apicis, &pa->pons);
        CREDO_NON_NIHIL (oa);

        /* calefactio: pictura plena + quadrum mutationis */
        tessera_scribere_literis(oa, ZEPHYRUM, ZEPHYRUM,
            "calefactio", _nativus());
        CREDO_VERUM (tessera_praesentare(oa));
        tessera_pons_memoriae_purgare(pa);
        tessera_cellulam_ponere(oa, I, I, (i32)'w', _nativus());
        CREDO_VERUM (tessera_praesentare(oa));
        tessera_pons_memoriae_purgare(pa);

        apex_calidus = piscina_summa_apex_usus(piscina_apicis);
        per (k = ZEPHYRUM; k < X; k++)
        {
            tessera_cellulam_ponere(oa, (s32)(k % XL), II,
                (i32)('a' + (character)k), _nativus());
            CREDO_VERUM (tessera_praesentare(oa));
            tessera_pons_memoriae_purgare(pa);
        }
        CREDO_VERUM (piscina_summa_apex_usus(piscina_apicis)
            == apex_calidus);

        CREDO_AEQUALIS_I32 (oa->fructus.praesentationes, XII);
        CREDO_VERUM (oa->fructus.cellulae_collatae > ZEPHYRUM);
        CREDO_VERUM (oa->fructus.cellulae_mutatae > ZEPHYRUM);
        CREDO_VERUM (oa->fructus.octeti_emissi > ZEPHYRUM);

        piscina_destruere(piscina_apicis);
    }

    /* ========================================================
     * PROBARE: claudere - reditio SGR + cursor visibilis + exire
     * ======================================================== */
    {
        chorda captum;

        imprimere("\n--- Probans claudere ---\n");

        tessera_pons_memoriae_purgare(pm);
        tessera_claudere(opus);
        captum = tessera_pons_memoriae_captum(pm);
        CREDO_CHORDA_AEQUALIS_LITERIS (captum, "\033[0m\033[?25h");
        CREDO_FALSUM (pm->intratum);
        CREDO_AEQUALIS_I32 (pm->numerus_exitum, I);
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
