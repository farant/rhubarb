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
     * PROBARE: optima_decurtata - cauda substituta sanata
     * (fusio trans gradus: macra competitoria in piscina!)
     * ======================================================== */
    {
        chorda candidati[V];
        SimilitudoFructus fructus[V];
        i32 n;

        imprimere("\n--- Probans optima_decurtata ---\n");

        candidati[0] = chorda_ex_literis(
            "legatus_currere", piscina);
        candidati[1] = chorda_ex_literis(
            "machinula_currere", piscina);
        candidati[2] = chorda_ex_literis(
            "piscina_vacare", piscina);
        candidati[3] = chorda_ex_literis(
            "LEGATUS_RELATIONES_MAXIMAE", piscina);
        candidati[4] = chorda_ex_literis(
            "LEGATUS_PLAGULAE_VOCANTIUM_MAXIMAE", piscina);

        /* casus natalis (excussio 2026-07-30): praefixum verum,
         * verbum falsum - subsequentia tota necatur. Fusio trans
         * gradus: macra ad 'legatus_t' (9) congruunt sed sordide
         * (casus discors, hiatus), legatus_currere ad 'legatus_'
         * (8) MUNDE - punctum optimum trans praefixa functionem
         * veram primam ponit. Puncta manu computata ex ponderibus:
         * currere 17+7*9=80; macro praefixum 73 (cauda 't' pretio
         * -6 se ipsa demittit: 67@9 < 73@8). */
        n = similitudo_optima(
            chorda_ex_literis("legatus_tractare", piscina),
            candidati, V, fructus, V);
        CREDO_AEQUALIS_I32 (n, ZEPHYRUM);
        n = similitudo_optima_decurtata(
            chorda_ex_literis("legatus_tractare", piscina),
            candidati, V, fructus, V);
        CREDO_AEQUALIS_I32 (n, III);
        CREDO_AEQUALIS_I32 (fructus[0].index, ZEPHYRUM);
        CREDO_VERUM (fructus[0].punctum == 80);
        /* macra paria (73) - brevius primum (_melior) */
        CREDO_AEQUALIS_I32 (fructus[1].index, III);
        CREDO_VERUM (fructus[1].punctum == 73);
        CREDO_AEQUALIS_I32 (fructus[2].index, IV);
        /* machinula_currere: 'leg' numquam congruit (consumpta II
         * sub pavimento) - exclusa, non sordide inclusa */

        /* congruentia plena = optima ipsa (nulla decurtatio) */
        n = similitudo_optima_decurtata(
            chorda_ex_literis("piscina_vacare", piscina),
            candidati, V, fructus, V);
        CREDO_VERUM (n >= I);
        CREDO_AEQUALIS_I32 (fructus[0].index, II);

        /* pavimentum III: quaestio brevior numquam temptatur */
        n = similitudo_optima_decurtata(
            chorda_ex_literis("zz", piscina),
            candidati, V, fructus, V);
        CREDO_AEQUALIS_I32 (n, ZEPHYRUM);

        /* nihil simile omnino: nullum praefixum >= III congruit,
         * zephyrum honestum */
        n = similitudo_optima_decurtata(
            chorda_ex_literis("qqqqqq", piscina),
            candidati, V, fructus, V);
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
