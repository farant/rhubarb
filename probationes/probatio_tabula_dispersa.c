/* probatio_tabula_dispersa.c - Tests for Tabula Dispersa */
#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "tabula_dispersa.h"
#include "credo.h"
#include <stdio.h>

s32 principale (vacuum)
{
    Piscina* piscina;
        b32  praeteritus;

    /* Initialize piscina and credo */
    piscina = piscina_generare_dynamicum("probatio_tabula_dispersa", CDLVI);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    
    /* =================================================
     * Probare tabula_dispersa_creare
     * ================================================= */

    {
        TabulaDispersa* tabula;

        imprimere("\n--- Probans tabula_dispersa_creare ---\n");

        tabula = tabula_dispersa_creare_chorda(piscina, CXXVIII);
        CREDO_NON_NIHIL(tabula);
        CREDO_AEQUALIS_I32((i32)tabula->numerus, ZEPHYRUM);
        CREDO_AEQUALIS_I32((i32)tabula->capacitas, CXXVIII);
    }

    /* =================================================
     * Probare tabula_dispersa_inserere et invenire
     * ================================================= */

    { 
        TabulaDispersa* tabula;
                chorda  clavis1, clavis2, clavis_nonexistens;
                   i32  valor1 = XLII;
                   i32  valor2 = XCIX;
                vacuum* valor_recuperatus;

        imprimere("\n--- Probans tabula_dispersa_inserere et invenire ---\n");

        tabula = tabula_dispersa_creare_chorda(piscina, CXXVIII);
        CREDO_NON_NIHIL(tabula);

        /* Insert first entry */
        clavis1 = chorda_ex_literis("alice", piscina);
        CREDO_VERUM(tabula_dispersa_inserere(
                        tabula, 
                        clavis1, 
                        (vacuum*)(longus)valor1));

        /* Insert second entry */
        clavis2 = chorda_ex_literis("bob", piscina);
        CREDO_VERUM(tabula_dispersa_inserere(
                        tabula, 
                        clavis2, 
                        (vacuum*)(longus)valor2));

        /* Lookup both */
        CREDO_VERUM(tabula_dispersa_invenire(tabula, clavis1, &valor_recuperatus));
        CREDO_AEQUALIS_I32((i32)(longus)valor_recuperatus, valor1);

        CREDO_VERUM(tabula_dispersa_invenire(tabula, clavis2, &valor_recuperatus));
        CREDO_AEQUALIS_I32((i32)(longus)valor_recuperatus, valor2);

        /* Lookup nonexistent */
        clavis_nonexistens = chorda_ex_literis("charlie", piscina);
        CREDO_FALSUM(tabula_dispersa_invenire(
                        tabula, 
                        clavis_nonexistens, 
                        &valor_recuperatus));
    }


    /* =================================================
     * Probare tabula_dispersa_continet
     * ================================================= */

    {
        TabulaDispersa* tabula;
                chorda  clavis;

        imprimere("\n--- Probans tabula_dispersa_continet ---\n");

        tabula = tabula_dispersa_creare_chorda(piscina, CXXVIII);
        clavis = chorda_ex_literis("test", piscina);

        CREDO_FALSUM(tabula_dispersa_continet(tabula, clavis));

        tabula_dispersa_inserere(tabula, clavis, (vacuum*)(longus)CXXIII);
        CREDO_VERUM(tabula_dispersa_continet(tabula, clavis));
    }

    /* =================================================
     * Probare tabula_dispersa_inserere_update
     * ================================================= */

    {
        TabulaDispersa* tabula;
                chorda  clavis;
                   i32  valor1 = 10;
                   i32  valor2 = 20;
                vacuum* valor_recuperatus;

        imprimere("\n--- Probans tabula_dispersa_inserere (update) ---\n");

        tabula = tabula_dispersa_creare_chorda(piscina, CXXVIII);
        clavis = chorda_ex_literis("key", piscina);

        /* Insert */
        CREDO_VERUM(tabula_dispersa_inserere(tabula, clavis, (vacuum*)(longus)valor1));
        CREDO_AEQUALIS_I32((i32)tabula->numerus, I);

        /* Update same key */
        CREDO_VERUM(tabula_dispersa_inserere(tabula, clavis, (vacuum*)(longus)valor2));
        CREDO_AEQUALIS_I32((i32)tabula->numerus, I);

        /* Verify updated value */
        CREDO_VERUM(tabula_dispersa_invenire(tabula, clavis, &valor_recuperatus));
        CREDO_AEQUALIS_I32((i32)(longus)valor_recuperatus, valor2);
    }

    /* =================================================
     * Probare tabula_dispersa_delere
     * ================================================= */

    {
        TabulaDispersa* tabula;
                chorda  clavis1, clavis2, clavis_nonexistens;
                vacuum* valor_recuperatus;

        imprimere("\n--- Probans tabula_dispersa_delere ---\n");

        tabula = tabula_dispersa_creare_chorda(piscina, CXXVIII);
        clavis1 = chorda_ex_literis("alice", piscina);
        clavis2 = chorda_ex_literis("bob", piscina);

        /* Insert both */
        tabula_dispersa_inserere(tabula, clavis1, (vacuum*)(longus)I);
        tabula_dispersa_inserere(tabula, clavis2, (vacuum*)(longus)II);
        CREDO_AEQUALIS_I32((i32)tabula->numerus, II);

        /* Delete first */
        CREDO_VERUM(tabula_dispersa_delere(tabula, clavis1));
        CREDO_AEQUALIS_I32((i32)tabula->numerus, I);

        /* Verify deleted */
        CREDO_FALSUM(tabula_dispersa_invenire(tabula, clavis1, &valor_recuperatus));

        /* Second still there */
        CREDO_VERUM(tabula_dispersa_invenire(tabula, clavis2, &valor_recuperatus));

        /* Delete nonexistent */
        clavis_nonexistens = chorda_ex_literis("charlie", piscina);
        CREDO_FALSUM(tabula_dispersa_delere(tabula, clavis_nonexistens));
        CREDO_AEQUALIS_I32((i32)tabula->numerus, I);
    }


    /* =================================================
     * Probare tabula_dispersa_vacare
     * ================================================= */

    {
        TabulaDispersa* tabula;
                chorda  clavis, clavis2;

        imprimere("\n--- Probans tabula_dispersa_vacare ---\n");

        tabula = tabula_dispersa_creare_chorda(piscina, CXXVIII);
        clavis = chorda_ex_literis("test", piscina);

        tabula_dispersa_inserere(tabula, clavis, (vacuum*)(longus)XLII);
        CREDO_AEQUALIS_I32((i32)tabula->numerus, I);

        /* Clear */
        tabula_dispersa_vacare(tabula);
        CREDO_AEQUALIS_I32((i32)tabula->numerus, ZEPHYRUM);

        /* Reuse table */
        clavis2 = chorda_ex_literis("newkey", piscina);
        CREDO_VERUM(tabula_dispersa_inserere(tabula, clavis2, (vacuum*)(longus)XCIX));
        CREDO_AEQUALIS_I32((i32)tabula->numerus, I);
    }

    /* =================================================
     * Probare tabula_dispersa_crescere
     * ================================================= */

    {
        TabulaDispersa* tabula;
                   i32  capacitas_initialis;
                   i32  i;
             character  buffer[VI];

        imprimere("\n--- Probans tabula_dispersa_crescere ---\n");

        tabula = tabula_dispersa_creare_chorda(piscina, XVI);
        capacitas_initialis = tabula->capacitas;

        /* Insert until growth trigger */
        per (i = ZEPHYRUM; i < XIV; i++)
        {
            chorda clavis;

            snprintf(buffer, VI, "k%d", i);
            clavis = chorda_ex_literis(buffer, piscina);
            tabula_dispersa_inserere(tabula, clavis, (vacuum*)(longus)i);
        }

        /* Should have grown */
        CREDO_VERUM(tabula->capacitas > capacitas_initialis);

        /* All entries still accessible */
        per (i = ZEPHYRUM; i < XII; i++)
        {
            chorda  clavis;
            vacuum *valor;

            snprintf(buffer, VI, "k%d", i);
            clavis = chorda_ex_literis(buffer, piscina);
            CREDO_VERUM(tabula_dispersa_invenire(tabula, clavis, &valor));
            CREDO_AEQUALIS_I32((i32)(longus)valor, i);
        }
    }


    /* =================================================
     * Probare tabula_dispersa_iterator
     * ================================================= */

    {
        TabulaDispersa* tabula;
                chorda  clavis1, clavis2, clavis3;
        TabulaIterator  iter;
                chorda  clavis_iter;
                vacuum* valor_iter;
                   i32  count;

        imprimere("\n--- Probans tabula_dispersa_iterator ---\n");

        tabula = tabula_dispersa_creare_chorda(piscina, CXXVIII);

        clavis1 = chorda_ex_literis("first", piscina);
        clavis2 = chorda_ex_literis("second", piscina);
        clavis3 = chorda_ex_literis("third", piscina);

        tabula_dispersa_inserere(tabula, clavis1, (vacuum*)(longus)I);
        tabula_dispersa_inserere(tabula, clavis2, (vacuum*)(longus)II);
        tabula_dispersa_inserere(tabula, clavis3, (vacuum*)(longus)III);

        /* Iterate and count */
        iter  = tabula_dispersa_iterator_initium(tabula);
        count = ZEPHYRUM;

        dum (tabula_dispersa_iterator_proximum(&iter, &clavis_iter, &valor_iter))
        {
            count++;
        }

        CREDO_AEQUALIS_I32(count, III);
    }


    /* =================================================
     * Probare tabula_dispersa_factor_oneris
     * ================================================= */

    {
        TabulaDispersa* tabula;
                chorda  clavis;
                   f32  factor;
                   i32  i;
             character  buffer[VI];

        imprimere("\n--- Probans tabula_dispersa_factor_oneris ---\n");

        tabula = tabula_dispersa_creare_chorda(piscina, CCLVI);

        /* Empty table */
        factor = tabula_dispersa_factor_oneris(tabula);
        CREDO_VERUM(factor >= 0.0f && factor <= 0.01f);

        /* Add some entries */
        per (i = ZEPHYRUM; i < LXIV; i++)
        {
            snprintf(buffer, VI, "k%d", i);
            clavis = chorda_ex_literis(buffer, piscina);
            tabula_dispersa_inserere(tabula, clavis, (vacuum*)(longus)i);
        }

        factor = tabula_dispersa_factor_oneris(tabula);
        CREDO_VERUM(factor > 0.2f && factor < 0.3f);
    }


    /* =================================================
     * Probare tabula_dispersa_numerus
     * ================================================= */

    {
        TabulaDispersa* tabula;
                chorda  clavis;

        imprimere("\n--- Probans tabula_dispersa_numerus ---\n");

        tabula = tabula_dispersa_creare_chorda(piscina, CXXVIII);

        CREDO_AEQUALIS_I32((i32)tabula_dispersa_numerus(tabula), ZEPHYRUM);

        clavis = chorda_ex_literis("one", piscina);
        tabula_dispersa_inserere(tabula, clavis, (vacuum*)(longus)I);
        CREDO_AEQUALIS_I32((i32)tabula_dispersa_numerus(tabula), I);

        tabula_dispersa_delere(tabula, clavis);
        CREDO_AEQUALIS_I32((i32)tabula_dispersa_numerus(tabula), ZEPHYRUM);
    }

    /* =================================================
     * Probare collisiones tracking
     * ================================================= */

    {
        TabulaDispersa* tabula;
                   i32  i;
             character  buffer[VI];
                   i32  collisiones_ante;


        imprimere("\n--- Probans collisiones tracking ---\n");

        tabula = tabula_dispersa_creare_chorda(piscina, XVI);

        collisiones_ante = tabula->collisiones_totales;
        CREDO_AEQUALIS_I32((i32)collisiones_ante, ZEPHYRUM);

        /* Insert many items to force collisions */
        per (i = ZEPHYRUM; i < XXXII; i++) 
        {
            chorda clavis;

            snprintf(buffer, VI, "k%d", i);
            clavis = chorda_ex_literis(buffer, piscina);
            tabula_dispersa_inserere(tabula, clavis, (vacuum*)(longus)i);
        }

        /* Some collisiones should have occurred */
        CREDO_VERUM(tabula->collisiones_totales >= ZEPHYRUM);
    }


    /* =================================================
     * Probare NULL key handling
     * ================================================= */

    {
        TabulaDispersa* tabula;
                chorda  clavis_vacua;

        imprimere("\n--- Probans NULL key handling ---\n");

        tabula = tabula_dispersa_creare_chorda(piscina, CXXVIII);
        clavis_vacua.mensura = ZEPHYRUM;
        clavis_vacua.datum = NIHIL;

        /* Empty key should not insert */
        CREDO_FALSUM(tabula_dispersa_inserere(
                        tabula, clavis_vacua, (vacuum*)(longus)I));

        CREDO_FALSUM(tabula_dispersa_invenire(
                        tabula, clavis_vacua, NIHIL));
    }


    /* =================================================
     * Probare multiple hash functions
     * ================================================= */

    {
        TabulaDispersa* tabula1;
        TabulaDispersa* tabula2;
                chorda  clavis;
                   i32  i;
             character  buffer[VI];

        imprimere("\n--- Probans multiple hash functions ---\n");

        /* Create tables with different hash functions */
        tabula1 = tabula_dispersa_creare(piscina, CXXVIII, 
                                         tabula_friare_fnv1a,
                                         (TabulaComparatio)chorda_comparare);
        CREDO_NON_NIHIL(tabula1);

        tabula2 = tabula_dispersa_creare(piscina, CXXVIII,
                                         tabula_friare_multiplicatio,
                                         (TabulaComparatio)chorda_comparare);
        CREDO_NON_NIHIL(tabula2);

        /* Both should work */
        per (i = ZEPHYRUM; i < X; i++)
        {
            snprintf(buffer, VI, "k%d", i);
            clavis = chorda_ex_literis(buffer, piscina);

            CREDO_VERUM(tabula_dispersa_inserere(tabula1, clavis, (vacuum*)(longus)i));
            CREDO_VERUM(tabula_dispersa_inserere(tabula2, clavis, (vacuum*)(longus)i));
        }

        /* Verify lookups work */
        per (i = ZEPHYRUM; i < X; i++)
        {
            vacuum* valor1 = NIHIL;
            vacuum* valor2 = NIHIL;

            snprintf(buffer, VI, "k%d", i);
            clavis = chorda_ex_literis(buffer, piscina);

            CREDO_VERUM(tabula_dispersa_invenire(tabula1, clavis, &valor1));
            CREDO_VERUM(tabula_dispersa_invenire(tabula2, clavis, &valor2));

            CREDO_AEQUALIS_I32((i32)(longus)valor1, i);
            CREDO_AEQUALIS_I32((i32)(longus)valor2, i);
        }
    }


    /* =================================================
     * Compendium
     * ================================================= */

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
