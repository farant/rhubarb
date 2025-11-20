/* probatio_internamentum.c - Probationes Internamenti Chordae */
#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "internamentum.h"
#include "credo.h"
#include <stdio.h>

s32 principale (vacuum)
{
    Piscina* piscina;
        b32  praeteritus;

    /* Aperire piscinam et credonem */
    piscina = piscina_generare_dynamicum("probatio_internamentum", CDLVI);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ==================================================
     * Probare internamentum_creare
     * ================================================== */

    {
        InternamentumChorda* intern;

        imprimere("\n--- Probans internamentum_creare ---\n");

        intern = internamentum_creare(piscina);
        CREDO_NON_NIHIL(intern);
        CREDO_NON_NIHIL(intern->tabula);
        CREDO_NON_NIHIL(intern->piscina);
        CREDO_AEQUALIS_I32(internamentum_numerus(intern), ZEPHYRUM);
    }

    /* ==================================================
     * Probare chorda_internare - aequales chordas
     * ================================================== */

    {
        InternamentumChorda* intern;
        chorda s1, s2;
        chorda *interned1, *interned2;

        imprimere("\n--- Probans chorda_internare (aequales) ---\n");

        intern = internamentum_creare(piscina);

        s1 = chorda_ex_literis("salve", piscina);
        s2 = chorda_ex_literis("salve", piscina);

        interned1 = chorda_internare(intern, s1);
        interned2 = chorda_internare(intern, s2);

        /* Debet redde eundem pointer */
        CREDO_NON_NIHIL(interned1);
        CREDO_NON_NIHIL(interned2);
        CREDO_VERUM(interned1 == interned2);  /* Pointer equality! */

        /* Numerus debet esse I (una chorda unica) */
        CREDO_AEQUALIS_I32(internamentum_numerus(intern), I);
    }

    /* ==================================================
     * Probare chorda_internare - differentes chordas
     * ================================================== */

    {
        InternamentumChorda* intern;
        chorda s1, s2, s3;
        chorda *interned1, *interned2, *interned3;

        imprimere("\n--- Probans chorda_internare (differentes) ---\n");

        intern = internamentum_creare(piscina);

        s1 = chorda_ex_literis("alice", piscina);
        s2 = chorda_ex_literis("bob", piscina);
        s3 = chorda_ex_literis("charlie", piscina);

        interned1 = chorda_internare(intern, s1);
        interned2 = chorda_internare(intern, s2);
        interned3 = chorda_internare(intern, s3);

        /* Omnes debent esse differentes pointers */
        CREDO_NON_NIHIL(interned1);
        CREDO_NON_NIHIL(interned2);
        CREDO_NON_NIHIL(interned3);
        CREDO_FALSUM(interned1 == interned2);
        CREDO_FALSUM(interned2 == interned3);
        CREDO_FALSUM(interned1 == interned3);

        /* Numerus debet esse III */
        CREDO_AEQUALIS_I32(internamentum_numerus(intern), III);
    }

    /* ==================================================
     * Probare chorda_internare_ex_literis
     * ================================================== */

    {
        InternamentumChorda* intern;
        chorda *interned1, *interned2;

        imprimere("\n--- Probans chorda_internare_ex_literis ---\n");

        intern = internamentum_creare(piscina);

        interned1 = chorda_internare_ex_literis(intern, "test");
        interned2 = chorda_internare_ex_literis(intern, "test");

        /* Debet redde eundem pointer */
        CREDO_NON_NIHIL(interned1);
        CREDO_VERUM(interned1 == interned2);

        /* Verificare contentum */
        CREDO_AEQUALIS_I32(interned1->mensura, IV);
        CREDO_CHORDA_AEQUALIS(*interned1, chorda_ex_literis("test", piscina));
    }

    /* ==================================================
     * Probare chorda_est_internata
     * ================================================== */

    {
        InternamentumChorda* intern;
        chorda s1, s2;

        imprimere("\n--- Probans chorda_est_internata ---\n");

        intern = internamentum_creare(piscina);

        s1 = chorda_ex_literis("internata", piscina);
        s2 = chorda_ex_literis("non_internata", piscina);

        CREDO_FALSUM(chorda_est_internata(intern, s1));

        chorda_internare(intern, s1);

        CREDO_VERUM(chorda_est_internata(intern, s1));
        CREDO_FALSUM(chorda_est_internata(intern, s2));
    }

    /* ==================================================
     * Probare pointer equality pro comparatio
     * ================================================== */

    {
        InternamentumChorda* intern;
        chorda *id1, *id2, *id3;

        imprimere("\n--- Probans pointer equality ---\n");

        intern = internamentum_creare(piscina);

        /* Simulare IDs entitatum */
        id1 = chorda_internare_ex_literis(intern, "entity:page:1");
        id2 = chorda_internare_ex_literis(intern, "entity:page:1");
        id3 = chorda_internare_ex_literis(intern, "entity:page:2");

        /* id1 et id2 debent esse idem pointer */
        CREDO_VERUM(id1 == id2);

        /* id1 et id3 debent esse differentes */
        CREDO_FALSUM(id1 == id3);

        /* Comparatio pointer est multo celerior quam chorda_aequalis */
    }

    /* ==================================================
     * Probare internamentum_vacare
     * ================================================== */

    {
        InternamentumChorda* intern;
        chorda *interned;

        imprimere("\n--- Probans internamentum_vacare ---\n");

        intern = internamentum_creare(piscina);

        chorda_internare_ex_literis(intern, "test1");
        chorda_internare_ex_literis(intern, "test2");
        chorda_internare_ex_literis(intern, "test3");

        CREDO_AEQUALIS_I32(internamentum_numerus(intern), III);

        internamentum_vacare(intern);

        CREDO_AEQUALIS_I32(internamentum_numerus(intern), ZEPHYRUM);

        /* Post vacare, possumus internare novas chordas */
        interned = chorda_internare_ex_literis(intern, "nova");
        CREDO_NON_NIHIL(interned);
        CREDO_AEQUALIS_I32(internamentum_numerus(intern), I);
    }

    /* ==================================================
     * Probare multa internationes
     * ================================================== */

    {
        InternamentumChorda* intern;
        chorda *interned[X];
        i32 i;

        imprimere("\n--- Probans multa internationes ---\n");

        intern = internamentum_creare(piscina);

        /* Internare X chordas differentes */
        interned[ZEPHYRUM] = chorda_internare_ex_literis(intern, "zero");
        interned[I]        = chorda_internare_ex_literis(intern, "unus");
        interned[II]       = chorda_internare_ex_literis(intern, "duo");
        interned[III]      = chorda_internare_ex_literis(intern, "tres");
        interned[IV]       = chorda_internare_ex_literis(intern, "quattuor");
        interned[V]        = chorda_internare_ex_literis(intern, "quinque");
        interned[VI]       = chorda_internare_ex_literis(intern, "sex");
        interned[VII]      = chorda_internare_ex_literis(intern, "septem");
        interned[VIII]     = chorda_internare_ex_literis(intern, "octo");
        interned[IX]       = chorda_internare_ex_literis(intern, "novem");

        CREDO_AEQUALIS_I32(internamentum_numerus(intern), X);

        /* Verificare omnes sunt non-nihil et uniques */
        per (i = ZEPHYRUM; i < X; i++)
        {
            CREDO_NON_NIHIL(interned[i]);
        }

        /* Internare duplicatos - debet redde eosdem pointers */
        CREDO_VERUM(chorda_internare_ex_literis(intern, "zero") == interned[ZEPHYRUM]);
        CREDO_VERUM(chorda_internare_ex_literis(intern, "quinque") == interned[V]);

        /* Numerus adhuc X */
        CREDO_AEQUALIS_I32(internamentum_numerus(intern), X);
    }

    /* ==================================================
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
