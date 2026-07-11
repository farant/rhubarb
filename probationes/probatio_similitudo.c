/* probatio_similitudo.c - Probationes Similitudinis */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "similitudo.h"
#include "credo.h"
#include <stdio.h>

s32 principale (vacuum)
{
         b32  praeteritus;
    Piscina*  piscina;

    piscina = piscina_generare_dynamicum("probatio_similitudo",
        4096);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ========================================================
     * PROBARE: similitudo_punctare - fundamenta
     * ======================================================== */
    {
        s32 punctum;

        imprimere("\n--- Probans punctare fundamenta ---\n");

        /* congruentia exacta */
        CREDO_VERUM (similitudo_punctare(
            chorda_ex_literis("xar", piscina),
            chorda_ex_literis("xar", piscina), &punctum));
        CREDO_VERUM (punctum > ZEPHYRUM);

        /* non subsequentia -> non congruit */
        CREDO_FALSUM (similitudo_punctare(
            chorda_ex_literis("lineas", piscina),
            chorda_ex_literis("demissio_currere", piscina),
            &punctum));

        /* quaestio vacua omnibus congruit, puncto zephyro */
        CREDO_VERUM (similitudo_punctare(
            chorda_ex_literis("", piscina),
            chorda_ex_literis("quidlibet", piscina), &punctum));
        CREDO_AEQUALIS_S32 (punctum, ZEPHYRUM);

        /* candidatus vacuus (quaestione plena) non congruit */
        CREDO_FALSUM (similitudo_punctare(
            chorda_ex_literis("x", piscina),
            chorda_ex_literis("", piscina), &punctum));

        /* insensibilis casui */
        CREDO_VERUM (similitudo_punctare(
            chorda_ex_literis("medulla", piscina),
            chorda_ex_literis("MEDULLA_LINEAE", piscina),
            &punctum));
    }

    /* ========================================================
     * PROBARE: punctatio - limites, continua, casus, hiatus
     * ======================================================== */
    {
        s32 p_limes;
        s32 p_interior;
        s32 p_casus_exactus;
        s32 p_casus_alius;
        s32 p_hiatus_brevis;
        s32 p_hiatus_longus;

        imprimere("\n--- Probans pondera punctationis ---\n");

        /* limites verborum vincunt interiores: "lc" */
        CREDO_VERUM (similitudo_punctare(
            chorda_ex_literis("lc", piscina),
            chorda_ex_literis("lineas_colligere", piscina),
            &p_limes));
        CREDO_VERUM (similitudo_punctare(
            chorda_ex_literis("lc", piscina),
            chorda_ex_literis("alchemia", piscina), &p_interior));
        CREDO_VERUM (p_limes > p_interior);

        /* transitus CamelCase = limes: "ml" in MedullaLineae */
        CREDO_VERUM (similitudo_punctare(
            chorda_ex_literis("ml", piscina),
            chorda_ex_literis("MedullaLineae", piscina),
            &p_limes));
        CREDO_VERUM (similitudo_punctare(
            chorda_ex_literis("ml", piscina),
            chorda_ex_literis("simulare", piscina), &p_interior));
        CREDO_VERUM (p_limes > p_interior);

        /* casus exactus parum praemiatur */
        CREDO_VERUM (similitudo_punctare(
            chorda_ex_literis("Med", piscina),
            chorda_ex_literis("MedullaLineae", piscina),
            &p_casus_exactus));
        CREDO_VERUM (similitudo_punctare(
            chorda_ex_literis("med", piscina),
            chorda_ex_literis("MedullaLineae", piscina),
            &p_casus_alius));
        CREDO_VERUM (p_casus_exactus > p_casus_alius);

        /* hiatus brevior melior */
        CREDO_VERUM (similitudo_punctare(
            chorda_ex_literis("sc", piscina),
            chorda_ex_literis("s_colligere", piscina),
            &p_hiatus_brevis));
        CREDO_VERUM (similitudo_punctare(
            chorda_ex_literis("sc", piscina),
            chorda_ex_literis("s_xyz_colligere", piscina),
            &p_hiatus_longus));
        CREDO_VERUM (p_hiatus_brevis > p_hiatus_longus);
    }

    /* ========================================================
     * PROBARE: similitudo_optima - casus scrutinii (natalis!)
     * ======================================================== */
    {
        chorda candidati[VI];
        SimilitudoFructus fructus[V];
        i32 n;

        imprimere("\n--- Probans optima (casus scrutinii) ---\n");

        /* vectis emitte 2026-07-10: symbolum male coniectum -
         * "signatura" candidatos veros scrutinii invenire debet */
        candidati[0] = chorda_ex_literis(
            "scr_partition_free", piscina);
        candidati[1] = chorda_ex_literis(
            "scr_partition_related_pairs", piscina);
        candidati[2] = chorda_ex_literis(
            "scr_signatura_ex_partitione", piscina);
        candidati[3] = chorda_ex_literis(
            "scr_partition_max_size", piscina);
        candidati[4] = chorda_ex_literis(
            "demissio_lineas_colligere", piscina);
        candidati[5] = chorda_ex_literis(
            "scr_sig_dump", piscina);

        n = similitudo_optima(
            chorda_ex_literis("signatura", piscina),
            candidati, VI, fructus, V);
        CREDO_VERUM (n >= I);
        CREDO_AEQUALIS_I32 (fructus[0].index, II);

        /* recordatio conceptus: "lineas" nomen plenum invenit */
        n = similitudo_optima(
            chorda_ex_literis("lineas", piscina),
            candidati, VI, fructus, V);
        CREDO_VERUM (n >= I);
        CREDO_AEQUALIS_I32 (fructus[0].index, IV);
    }

    /* ========================================================
     * PROBARE: optima - ordo, fines, paritates
     * ======================================================== */
    {
        chorda candidati[IV];
        SimilitudoFructus fructus[II];
        i32 n;

        imprimere("\n--- Probans optima ordinem finesque ---\n");

        candidati[0] = chorda_ex_literis("xar_addere", piscina);
        candidati[1] = chorda_ex_literis("xar", piscina);
        candidati[2] = chorda_ex_literis("exarare", piscina);
        candidati[3] = chorda_ex_literis("nihil_tale", piscina);

        /* paritas puncti -> brevior vincit; maxima servatur */
        n = similitudo_optima(chorda_ex_literis("xar", piscina),
            candidati, IV, fructus, II);
        CREDO_AEQUALIS_I32 (n, II);
        CREDO_AEQUALIS_I32 (fructus[0].index, I);
        CREDO_AEQUALIS_I32 (fructus[1].index, ZEPHYRUM);
        CREDO_VERUM (fructus[0].punctum >= fructus[1].punctum);

        /* nulla congruentia -> zephyrum */
        n = similitudo_optima(chorda_ex_literis("zzz", piscina),
            candidati, IV, fructus, II);
        CREDO_AEQUALIS_I32 (n, ZEPHYRUM);
    }

    /* ========================================================
     * Compendium
     * ======================================================== */

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
