/* probatio_credo_processus.c - PORTA NATIVA assertionum processus.
 *
 * Detector ruinarum qui ruinas non deprehendit viridem in aeternum
 * refert - peior quam nullus omnino, quia fiduciam falsam praebet.
 * Ergo CULPA INSERTA: haec probatio consulto ruit et affirmat
 * apparatum id VIDERE.
 *
 * Spec: project-specs/stml-instrumentum-spec.md par. 7.5.0
 */
#include "postulata_posix.h"
#include "latina.h"
#include "credo.h"
#include "piscina.h"
#include <stdio.h>
#include <signal.h>


/* VOLATILIS, non vinculum externum: sub -O2 (vexilla 2026-09-02)
 * clang functionem in eadem unitate inlineavit, monstratorem NIHIL
 * constantem vidit, scriptionem indefinitam DELEVIT - filius normaliter
 * exiit, porta 'Speratus SEGV, Receptus exitus normalis' rubuit. Per
 * variabilem volatilem valor tempore cursus legendus est: scriptio
 * fit, SIGSEGV venit. Culpa plantata optimizatori invisibilis esse
 * debet, non modo lectori. */
vacuum
probatio_credo_processus_scribere_ad_nihil (
    integer* p);

vacuum
probatio_credo_processus_scribere_ad_nihil (
    integer* p)
{
    integer* volatilis opacus = p;

    *opacus = XI;
}


/* Gyrus verus - nulla exitus via. Terminum excedere DEBET. Numerator
 * volatilis: effectus lateralis quem optimizator tollere nequit. */
vacuum
probatio_credo_processus_pendere_semper (vacuum);

vacuum
probatio_credo_processus_pendere_semper (
    vacuum)
{
    volatilis i32 gyrus = ZEPHYRUM;

    dum (VERUM)
    {
        gyrus = gyrus + I;
    }
}

s32
principale (vacuum)
{
         b32  praeteritus;
     Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_credo_processus",
                                         65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * PROBARE: expressiones innocuae praetereunt
     * ================================================== */

    imprimere("\n--- Probans casus innocuos ---\n");

    CREDO_NON_RUIT(imprimere(""));
    CREDO_NON_PENDET(imprimere(""), 2000);


    /* ==================================================
     * CULPA INSERTA I: ruina vera
     *
     * Si haec probatio FRACTA est, detector CAECUS est - et omnis
     * alia assertio quae ei nititur mendacium est.
     * ================================================== */

    imprimere("\n--- Culpa inserta I: scriptio ad NIHIL ---\n");

    CREDO_RUIT_CUM(
        probatio_credo_processus_scribere_ad_nihil((integer*)NIHIL),
        SIGSEGV);


    /* ==================================================
     * CULPA INSERTA II: gyrus infinitus
     *
     * SIGKILL, non ruina propria - NOS eum occidimus cum terminus
     * excessus est. Signum distinctum probat nos utrumque casum
     * discernere, non solum "mortuus est".
     * ================================================== */

    imprimere("\n--- Culpa inserta II: gyrus infinitus ---\n");

    /* CCC ms, non V s: filius numquam redit, ergo terminus totus
     * semper exspectatur - V s hic somnus erat (VI s cursus). */
    CREDO_RUIT_CUM_INTRA(probatio_credo_processus_pendere_semper(),
                         SIGKILL, CCC);


    /* ==================================================
     * Compendium
     * ================================================== */

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
