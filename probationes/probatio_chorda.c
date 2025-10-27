/* tests/test_chorda.c */
#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>

s32 principale (vacuum) {
    Piscina* piscina;
        b32  praeteritus;


    /* Aperire credo et piscina */
    piscina = piscina_generare_dynamicum("test_chorda", 4096);
    si (!piscina) 
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ================================================== 
     * Probare chorda_ex_literis
     * ================================================== */

    {
        chorda s1, s2, s3;

        imprimere("\n--- Probans chorda_ex_literis ---\n");

        s1 = chorda_ex_literis("salve", piscina);
        CREDO_AEQUALIS_S32((s32) s1.mensura, V);

        s2 = chorda_ex_literis("", piscina);
        CREDO_AEQUALIS_S32((s32) s2.mensura, ZEPHYRUM);

        s3 = chorda_ex_literis(NIHIL, piscina);
        CREDO_AEQUALIS_S32((s32) s3.mensura, ZEPHYRUM);
    }


    /* ================================================= 
     * Probare chorda_aequalis
     * ================================================== */

    {
        chorda a, b, c;

        imprimere("\n--- Probans chorda_aequalis ---\n");
        
        a = chorda_ex_literis("probatio", piscina);
        b = chorda_ex_literis("probatio", piscina);
        c = chorda_ex_literis("altera", piscina);

        CREDO_VERUM  (chorda_aequalis(a, b));
        CREDO_FALSUM (chorda_aequalis(a, c));
    }


    /* ================================================= 
     * Probare chorda_transcribere
     * ================================================== */

    {
        chorda originalis, transcriptus;

        imprimere("\n--- Probans chorda_transcribere ---\n");

        originalis   = chorda_ex_literis("transcriptio", piscina);
        transcriptus = chorda_transcribere(originalis, piscina);

        CREDO_AEQUALIS_I32 (transcriptus.mensura, originalis.mensura);
        CREDO_VERUM        (chorda_aequalis(transcriptus, originalis));
    }


    /* ================================================= 
     * Probare chorda_continet
     * ================================================== */

    {
        chorda fenum, acus_inventa, acus_non_inventa;

        imprimere("\n--- Probans chorda_continet ---\n");

        fenum            = chorda_ex_literis("salve munde", piscina);
        acus_inventa     = chorda_ex_literis("munde",       piscina);
        acus_non_inventa = chorda_ex_literis("xyz",         piscina);

        CREDO_VERUM  (chorda_continet(fenum, acus_inventa));
        CREDO_FALSUM (chorda_continet(fenum, acus_non_inventa));
    }


    /* ================================================= 
     * Probare chorda_incipit
     * ================================================== */

    {
        chorda ch, praefixum, non_praefixum;

        imprimere("\n--- Probans chorda_incipit ---\n");

        ch            = chorda_ex_literis("praefixum_suffixum", piscina);
        praefixum     = chorda_ex_literis("praefixum",          piscina);
        non_praefixum = chorda_ex_literis("suffixum",           piscina);

        CREDO_VERUM  (chorda_incipit(ch, praefixum));
        CREDO_FALSUM (chorda_incipit(ch, non_praefixum));
    }
 
    /* ================================================= 
     * Probare chorda_terminatur
     * ================================================== */

    {
        chorda ch2, suffixum, non_suffixum;

        imprimere("\n--- Probans chorda_terminatur ---\n");

        ch2          = chorda_ex_literis("praefixum_suffixum", piscina);
        suffixum     = chorda_ex_literis("suffixum",           piscina);
        non_suffixum = chorda_ex_literis("praefixum",          piscina);

        CREDO_VERUM  (chorda_terminatur(ch2, suffixum));
        CREDO_FALSUM (chorda_terminatur(ch2, non_suffixum));
    }


    /* ================================================= 
     * Probare chorda_terminatur
     * ================================================== */

    {
        chorda plenus, sectio, speratus;

        imprimere("\n--- Probans chorda_sectio ---\n");

        plenus = chorda_ex_literis("abcdef", piscina);
        sectio = chorda_sectio(plenus, II, V);

        CREDO_AEQUALIS_I32 (sectio.mensura, III);

        speratus = chorda_ex_literis("cde", piscina);
        CREDO_VERUM (chorda_aequalis(sectio, speratus));
    }

    /* ================================================= 
     * Compendium
     * ================================================== */

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    /* Destructio */
    credo_claudere();
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
