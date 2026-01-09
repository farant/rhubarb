/* probatio_arbor.c - Probationes Arbor Convenience API */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "arbor.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

s32 principale(vacuum)
{
    b32 praeteritus;
    Piscina* piscina;
    InternamentumChorda* intern;

    imprimere("=== PROBATIO ARBOR CONVENIENCE API ===\n");

    /* Aperire credo et piscina */
    piscina = piscina_generare_dynamicum("probatio_arbor", MMMMXCVI * IV);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }

    intern = internamentum_creare(piscina);
    si (!intern)
    {
        imprimere("FRACTA: internamentum_creare\n");
        redde I;
    }

    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: arbor_optiones_default
     * ======================================================== */

    {
        ArborOptiones opt;

        imprimere("\n--- Probans arbor_optiones_default ---\n");

        opt = arbor_optiones_default();

        CREDO_AEQUALIS_I32(opt.pp_modus, ARBOR_PP_MODUS_HYBRID);
        CREDO_NON_NIHIL(opt.include_viae);
        CREDO_AEQUALIS_I32(opt.include_numerus, I);

        imprimere("...  Default optiones: OK\n");
    }


    /* ========================================================
     * PROBARE: arbor_parsere_fontem - simplex
     * ======================================================== */

    {
        ArborResultus res;
        constans character* fons = "int x;";
        i32 len;

        imprimere("\n--- Probans arbor_parsere_fontem (simplex) ---\n");

        len = (i32)strlen(fons);
        res = arbor_parsere_fontem(fons, len, piscina, intern, NIHIL);

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        CREDO_AEQUALIS_I32(res.radix->genus, ARBOR_NODUS_TRANSLATION_UNIT);
        CREDO_NON_NIHIL(res.lexemata);

        imprimere("...  Declaratio simplex parsita: OK\n");
    }


    /* ========================================================
     * PROBARE: arbor_parsere_fontem - functio
     * ======================================================== */

    {
        ArborResultus res;
        constans character* fons =
            "int summa(int a, int b)\n"
            "{\n"
            "    return a + b;\n"
            "}\n";
        i32 len;

        imprimere("\n--- Probans arbor_parsere_fontem (functio) ---\n");

        len = (i32)strlen(fons);
        res = arbor_parsere_fontem(fons, len, piscina, intern, NIHIL);

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        imprimere("...  Functio parsita: OK\n");
    }


    /* ========================================================
     * PROBARE: arbor_parsere_fontem - error handling
     * ======================================================== */

    {
        ArborResultus res;

        imprimere("\n--- Probans arbor_parsere_fontem (NIHIL input) ---\n");

        res = arbor_parsere_fontem(NIHIL, ZEPHYRUM, piscina, intern, NIHIL);

        CREDO_FALSUM(res.successus);
        CREDO_NIHIL(res.radix);

        imprimere("...  NIHIL input tractatum: OK\n");
    }


    /* ========================================================
     * PROBARE: arbor_parsere_filum - small test file
     * ======================================================== */

    {
        ArborResultus res;
        ArborOptiones opt;

        imprimere("\n--- Probans arbor_parsere_filum ---\n");

        /* Use PRESERVARE mode for speed - avoids include processing */
        opt = arbor_optiones_default();
        opt.pp_modus = ARBOR_PP_MODUS_PRESERVARE;

        res = arbor_parsere_filum(
            "probationes/fixa/roundtrip/minimal.c",
            piscina,
            intern,
            &opt);

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        CREDO_AEQUALIS_I32(res.radix->genus, ARBOR_NODUS_TRANSLATION_UNIT);
        CREDO_NON_NIHIL(res.lexemata);

        imprimere("...  Filum parsitum: OK\n");
    }


    /* ========================================================
     * PROBARE: arbor_parsere_filum - filum non existens
     * ======================================================== */

    {
        ArborResultus res;
        ArborOptiones opt;

        imprimere("\n--- Probans arbor_parsere_filum (non existens) ---\n");

        /* Use PRESERVARE mode */
        opt = arbor_optiones_default();
        opt.pp_modus = ARBOR_PP_MODUS_PRESERVARE;

        res = arbor_parsere_filum("non_existens.c", piscina, intern, &opt);

        CREDO_FALSUM(res.successus);
        CREDO_NIHIL(res.radix);

        imprimere("...  Filum non existens tractatum: OK\n");
    }


    /* ========================================================
     * PROBARE: arbor_parsere_filum - piscina.c with HYBRID mode
     * ======================================================== */

    {
        ArborResultus res;

        imprimere("\n--- Probans arbor_parsere_filum (piscina.c HYBRID) ---\n");

        /* Use default options (HYBRID mode) to test Latin keyword macros */
        res = arbor_parsere_filum("lib/piscina.c", piscina, intern, NIHIL);

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        CREDO_AEQUALIS_I32(res.radix->genus, ARBOR_NODUS_TRANSLATION_UNIT);

        imprimere("...  piscina.c HYBRID parsitum: OK\n");
    }


    /* ========================================================
     * PROBARE: arbor_parsere_filum - piscina.h with HYBRID mode
     * ======================================================== */

    {
        ArborResultus res;

        imprimere("\n--- Probans arbor_parsere_filum (piscina.h HYBRID) ---\n");

        /* Use default options (HYBRID mode) to test Latin keyword macros */
        res = arbor_parsere_filum("include/piscina.h", piscina, intern, NIHIL);

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        CREDO_AEQUALIS_I32(res.radix->genus, ARBOR_NODUS_TRANSLATION_UNIT);

        imprimere("...  piscina.h HYBRID parsitum: OK\n");
    }


    /* ========================================================
     * Compendium
     * ======================================================== */

    imprimere("\n");
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
