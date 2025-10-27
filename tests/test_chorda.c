/* tests/test_chorda.c */
#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>

s32 principale (vacuum) {
    Piscina* piscina;
     chorda  s1;
     chorda  s2;
     chorda  s3;
        b32  praeteritus;

     /*
     chorda  a;
     chorda  b;
     chorda  c;
     */

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

    imprimere("\n--- Testing chorda_ex_literis ---\n");

    s1 = chorda_ex_literis("hello", piscina);
    CREDO_AEQUALIS_S32(s1.mensura, V);

    s2 = chorda_ex_literis("", piscina);
    CREDO_AEQUALIS_S32(s2.mensura, ZEPHYRUM);

    s3 = chorda_ex_literis(NIHIL, piscina);
    CREDO_AEQUALIS_S32(s3.mensura, ZEPHYRUM);

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
