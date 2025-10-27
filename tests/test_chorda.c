/* tests/test_chorda.c */
#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>

s32 principale (vacuum) {
    Piscina* piscina;
        b32  praeteritus;


    /* Initialize credo and piscina */
    piscina = piscina_generare_dynamicum("test_chorda", 4096);
    si (!piscina) 
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ================================================== 
     * Test chorda_ex_literis
     * ================================================== */

    {
        chorda s1, s2, s3;

        imprimere("\n--- Testing chorda_ex_literis ---\n");

        s1 = chorda_ex_literis("hello", piscina);
        CREDO_AEQUALIS_S32((s32) s1.mensura, V);

        s2 = chorda_ex_literis("", piscina);
        CREDO_AEQUALIS_S32((s32) s2.mensura, ZEPHYRUM);

        s3 = chorda_ex_literis(NIHIL, piscina);
        CREDO_AEQUALIS_S32((s32) s3.mensura, ZEPHYRUM);
    }


    /* ================================================= 
     * Test chorda_aequalis
     * ================================================== */

    {
        chorda a, b, c;

        imprimere("\n--- Testing chorda_aequalis ---\n");
        
        a = chorda_ex_literis("test", piscina);
        b = chorda_ex_literis("test", piscina);
        c = chorda_ex_literis("other", piscina);

        CREDO_VERUM  (chorda_aequalis(a, b));
        CREDO_FALSUM (chorda_aequalis(a, c));
    }


    /* ================================================= 
     * Test chorda_transcribere
     * ================================================== */

    {
        chorda original, copied;

        imprimere("\n--- Testing chorda_transcribere ---\n");

        original = chorda_ex_literis("transcribe me", piscina);
        copied   = chorda_transcribere(original, piscina);

        CREDO_AEQUALIS_I32 (copied.mensura, original.mensura);
        CREDO_VERUM        (chorda_aequalis(copied, original));
    }


    /* ================================================= 
     * Test chorda_continet
     * ================================================== */

    {
        chorda haystack, needle_found, needle_notfound;

        imprimere("\n--- Testing chorda_continet ---\n");

        haystack        = chorda_ex_literis("hello world", piscina);
        needle_found    = chorda_ex_literis("world",       piscina);
        needle_notfound = chorda_ex_literis("xyz",         piscina);

        CREDO_VERUM  (chorda_continet(haystack, needle_found));
        CREDO_FALSUM (chorda_continet(haystack, needle_notfound));
    }


    /* ================================================= 
     * Test chorda_incipit
     * ================================================== */

    {
        chorda str, prefix, notprefix;

        imprimere("\n--- Testing chorda_incipit ---\n");

        str       = chorda_ex_literis("prefix_suffix", piscina);
        prefix    = chorda_ex_literis("prefix",        piscina);
        notprefix = chorda_ex_literis("suffix",        piscina);

        CREDO_VERUM  (chorda_incipit(str, prefix));
        CREDO_FALSUM (chorda_incipit(str, notprefix));
    }

    /* ================================================= 
     * Test chorda_terminatur
     * ================================================== */

    {
        chorda str2, suffix, notsuffix;

        imprimere("\n--- Testing chorda_terminatur ---\n");

        str2      = chorda_ex_literis("prefix_suffix", piscina);
        suffix    = chorda_ex_literis("suffix",        piscina);
        notsuffix = chorda_ex_literis("prefix",           piscina);

        CREDO_VERUM  (chorda_terminatur(str2, suffix));
        CREDO_FALSUM (chorda_terminatur(str2, notsuffix));
    }


    /* ================================================= 
     * Test chorda_terminatur
     * ================================================== */

    {
        chorda full, slice, expected;

        imprimere("\n--- Testing chorda_sectio ---\n");

        full  = chorda_ex_literis("abcdef", piscina);
        slice = chorda_sectio(full, II, V);

        CREDO_AEQUALIS_I32 (slice.mensura, III);

        expected = chorda_ex_literis("cde", piscina);
        CREDO_VERUM (chorda_aequalis(slice, expected));
    }

    /* ================================================= 
     * Summary
     * ================================================== */

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    /* Cleanup */
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
