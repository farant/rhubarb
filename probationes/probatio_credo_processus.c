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


/* Externa consulto, non interior: si scriptor optimizans videret
 * scriptionem ad NIHIL numquam legi, eam plicare posset - tunc
 * culpa inserta evanesceret et porta TACERET. Definitio in unitate
 * alia latere non potest (una plagula sumus), ergo vinculum externum
 * optimizatorem prohibet. */
vacuum
probatio_credo_processus_scribere_ad_nihil (
    integer* p);

vacuum
probatio_credo_processus_scribere_ad_nihil (
    integer* p)
{
    *p = XI;
}


/* Gyrus verus - nulla exitus via. Terminum excedere DEBET. */
vacuum
probatio_credo_processus_pendere_semper (vacuum);

vacuum
probatio_credo_processus_pendere_semper (
    vacuum)
{
    dum (VERUM)
    {
        /* consulto vacuum */
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

    CREDO_RUIT_CUM(probatio_credo_processus_pendere_semper(),
                    SIGKILL);


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
