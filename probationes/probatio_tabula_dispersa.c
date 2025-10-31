/* probatio_tabula_dispersa.c - Probationes Tabulae Dispersae */
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

    /* Aperire piscinam et credonem */
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

        /* Insertio primi introitus */
        clavis1 = chorda_ex_literis("alice", piscina);
        CREDO_VERUM(tabula_dispersa_inserere(
                        tabula,
                        clavis1,
                        (vacuum*)(longus)valor1));

        /* Insertio secundi introitus */
        clavis2 = chorda_ex_literis("bob", piscina);
        CREDO_VERUM(tabula_dispersa_inserere(
                        tabula,
                        clavis2,
                        (vacuum*)(longus)valor2));

        /* Quaestiones amborum */
        CREDO_VERUM(tabula_dispersa_invenire(tabula, clavis1, &valor_recuperatus));
        CREDO_AEQUALIS_I32((i32)(longus)valor_recuperatus, valor1);

        CREDO_VERUM(tabula_dispersa_invenire(tabula, clavis2, &valor_recuperatus));
        CREDO_AEQUALIS_I32((i32)(longus)valor_recuperatus, valor2);

        /* Quaestio nonexistentis */
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

        /* Insertio */
        CREDO_VERUM(tabula_dispersa_inserere(tabula, clavis, (vacuum*)(longus)valor1));
        CREDO_AEQUALIS_I32((i32)tabula->numerus, I);

        /* Renovatio eiusdem clavis */
        CREDO_VERUM(tabula_dispersa_inserere(tabula, clavis, (vacuum*)(longus)valor2));
        CREDO_AEQUALIS_I32((i32)tabula->numerus, I);

        /* Verificatio valoris renovati */
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

        /* Insertiones amborum */
        tabula_dispersa_inserere(tabula, clavis1, (vacuum*)(longus)I);
        tabula_dispersa_inserere(tabula, clavis2, (vacuum*)(longus)II);
        CREDO_AEQUALIS_I32((i32)tabula->numerus, II);

        /* Deletio primi */
        CREDO_VERUM(tabula_dispersa_delere(tabula, clavis1));
        CREDO_AEQUALIS_I32((i32)tabula->numerus, I);

        /* Verificatio deletionis */
        CREDO_FALSUM(tabula_dispersa_invenire(tabula, clavis1, &valor_recuperatus));

        /* Secundus adhuc adest */
        CREDO_VERUM(tabula_dispersa_invenire(tabula, clavis2, &valor_recuperatus));

        /* Deletio nonexistentis */
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

        /* Vaccare */
        tabula_dispersa_vacare(tabula);
        CREDO_AEQUALIS_I32((i32)tabula->numerus, ZEPHYRUM);

        /* Reusum tabulae */
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
        CREDO_AEQUALIS_I32(capacitas_initialis, XVI);

        /* Insertiones antequam crescentia */
        per (i = ZEPHYRUM; i < XIV; i++)
        {
            chorda clavis;

            snprintf(buffer, VI, "k%d", i);
            clavis = chorda_ex_literis(buffer, piscina);
            tabula_dispersa_inserere(tabula, clavis, (vacuum*)(longus)i);
        }

        /* Oportet crevisse */
        CREDO_VERUM(tabula->capacitas > capacitas_initialis);

        /* Omnes introitus adhuc accessibiles */
        per (i = ZEPHYRUM; i < XIV; i++)
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

        /* Ambulare et computare */
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

        /* Tabula vacua */
        factor = tabula_dispersa_factor_oneris(tabula);
        CREDO_VERUM(factor >= 0.0f && factor <= 0.01f);

        /* Addere aliquos introitus */
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

        /* Insertiones multorum rerum ad coactiones collidendas */
        per (i = ZEPHYRUM; i < XXXII; i++)
        {
            chorda clavis;

            snprintf(buffer, VI, "k%d", i);
            clavis = chorda_ex_literis(buffer, piscina);
            tabula_dispersa_inserere(tabula, clavis, (vacuum*)(longus)i);
        }

        /* Aliquae collisiones debent accidisse */
        CREDO_VERUM(tabula->collisiones_totales > ZEPHYRUM);
    }


    /* =================================================
     * Probare NIHIL key handling
     * ================================================= */

    {
        TabulaDispersa* tabula;
                chorda  clavis_vacua;

        imprimere("\n--- Probans NIHIL key handling ---\n");

        tabula = tabula_dispersa_creare_chorda(piscina, CXXVIII);
        clavis_vacua.mensura = ZEPHYRUM;
        clavis_vacua.datum = NIHIL;

        /* Clavis vacua non debet inserere */
        CREDO_FALSUM(tabula_dispersa_inserere(
                        tabula, clavis_vacua, (vacuum*)(longus)I));

        CREDO_FALSUM(tabula_dispersa_invenire(
                        tabula, clavis_vacua, NIHIL));
    }


    /* =================================================
     * Probans functiones friationis multiplices
     * ================================================= */

    {
        TabulaDispersa* tabula1;
        TabulaDispersa* tabula2;
                chorda  clavis;
                   i32  i;
             character  buffer[VI];

        imprimere("\n--- Probans functiones friationis multiplices ---\n");

        /* Creare tabulas cum functionibus friationis diversis */
        tabula1 = tabula_dispersa_creare(piscina, CXXVIII,
                                         tabula_friare_fnv1a,
                                         (TabulaComparatio)chorda_comparare);
        CREDO_NON_NIHIL(tabula1);

        tabula2 = tabula_dispersa_creare(piscina, CXXVIII,
                                         tabula_friare_multiplicatio,
                                         (TabulaComparatio)chorda_comparare);
        CREDO_NON_NIHIL(tabula2);

        /* Ambae operari debent */
        per (i = ZEPHYRUM; i < X; i++)
        {
            snprintf(buffer, VI, "k%d", i);
            clavis = chorda_ex_literis(buffer, piscina);

            CREDO_VERUM(tabula_dispersa_inserere(tabula1, clavis, (vacuum*)(longus)i));
            CREDO_VERUM(tabula_dispersa_inserere(tabula2, clavis, (vacuum*)(longus)i));
        }

        /* Verificare quaestiones operantur */
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
     * Probare factor_deletorum et crescentia
     * ================================================= */

    {
        TabulaDispersa* tabula;
               chorda   clavis;
                  i32   i;
            character   buffer[VI];
                  i32   capacitas_initialis;

        imprimere("\n--- Probans factor_deletorum et crescentia ---\n");

        tabula = tabula_dispersa_creare_chorda(piscina, LXIV);
        capacitas_initialis = tabula->capacitas;

        /* Implere ad plus quam 25% */
        per (i = ZEPHYRUM; i < XVII; i++)
        {
            snprintf(buffer, VI, "k%d", i);
            clavis = chorda_ex_literis(buffer, piscina);
            tabula_dispersa_inserere(tabula, clavis, (vacuum*)(longus)i);
        }

        /* Delere omnes */
        per (i = ZEPHYRUM; i < XVII; i++)
        {
            snprintf(buffer, VI, "k%d", i);
            clavis = chorda_ex_literis(buffer, piscina);
            tabula_dispersa_delere(tabula, clavis);
        }

        /* Verificare factorem deletorum altum */
        CREDO_VERUM(tabula_dispersa_factor_deletorum(tabula) > 0.20f);
        CREDO_AEQUALIS_I32((i32)tabula->numerus, ZEPHYRUM);
        CREDO_VERUM(tabula->numerus_deletorum > ZEPHYRUM);

        /* Insertio una magis - oportet crescere propter factorem deletorum */
        clavis = chorda_ex_literis("trigger", piscina);
        tabula_dispersa_inserere(tabula, clavis, (vacuum*)(longus)XCIX);

        /* Capacitas crevisse debet, numerus deletorum refectus */
        CREDO_VERUM(tabula->capacitas > capacitas_initialis);
        CREDO_AEQUALIS_I32((i32)tabula->numerus_deletorum, ZEPHYRUM);
    }


    /* =================================================
     * Probare iterator cum tabula vacua
     * ================================================= */

    {
        TabulaDispersa* tabula;
         TabulaIterator iter;
                 chorda clavis_iter;
                vacuum* valor_iter;
                    i32 count;

        imprimere("\n--- Probans iterator cum tabula vacua ---\n");

        tabula = tabula_dispersa_creare_chorda(piscina, CXXVIII);

        /* Ambulare tabulam vacuam */
        iter = tabula_dispersa_iterator_initium(tabula);
        count = ZEPHYRUM;

        dum (tabula_dispersa_iterator_proximum(&iter, &clavis_iter, &valor_iter))
        {
            count++;
        }

        CREDO_AEQUALIS_I32(count, ZEPHYRUM);
    }


    /* =================================================
     * Probare iterator cum outputis NIHIL
     * ================================================= */

    {
        TabulaDispersa* tabula;
         TabulaIterator iter;
                 chorda clavis1, clavis2, clavis3;
                    i32 count;

        imprimere("\n--- Probans iterator cum outputis NIHIL ---\n");

        tabula = tabula_dispersa_creare_chorda(piscina, CXXVIII);

        clavis1 = chorda_ex_literis("alpha", piscina);
        clavis2 = chorda_ex_literis("beta", piscina);
        clavis3 = chorda_ex_literis("gamma", piscina);

        tabula_dispersa_inserere(tabula, clavis1, (vacuum*)(longus)I);
        tabula_dispersa_inserere(tabula, clavis2, (vacuum*)(longus)II);
        tabula_dispersa_inserere(tabula, clavis3, (vacuum*)(longus)III);

        /* Ambulare cum NIHIL outputis */
        iter = tabula_dispersa_iterator_initium(tabula);
        count = ZEPHYRUM;

        dum (tabula_dispersa_iterator_proximum(&iter, NIHIL, NIHIL))
        {
            count++;
        }

        CREDO_AEQUALIS_I32(count, III);
    }


    /* =================================================
     * Probare deletio post crescentiam
     * ================================================= */

    {
        TabulaDispersa* tabula;
                    i32 i;
              character buffer[VI];
                 chorda clavis;
                vacuum* valor;

        imprimere("\n--- Probans deletio post crescentiam ---\n");

        tabula = tabula_dispersa_creare_chorda(piscina, XVI);

        /* Insertiones ad coactiones crescentiae */
        per (i = ZEPHYRUM; i < XIV; i++)
        {
            snprintf(buffer, VI, "k%d", i);
            clavis = chorda_ex_literis(buffer, piscina);
            tabula_dispersa_inserere(tabula, clavis, (vacuum*)(longus)i);
        }

        /* Deletio post crescentiam */
        snprintf(buffer, VI, "k%d", II);
        clavis = chorda_ex_literis(buffer, piscina);
        CREDO_VERUM(tabula_dispersa_delere(tabula, clavis));
        CREDO_FALSUM(tabula_dispersa_invenire(tabula, clavis, &valor));

        /* Alia deletio */
        snprintf(buffer, VI, "k%d", V);
        clavis = chorda_ex_literis(buffer, piscina);
        CREDO_VERUM(tabula_dispersa_delere(tabula, clavis));
        CREDO_FALSUM(tabula_dispersa_invenire(tabula, clavis, &valor));

        /* Ceterae adhuc praesentes */
        snprintf(buffer, VI, "k%d", ZEPHYRUM);
        clavis = chorda_ex_literis(buffer, piscina);
        CREDO_VERUM(tabula_dispersa_invenire(tabula, clavis, &valor));
    }


    /* =================================================
     * Probare distantia_maxima tracking
     * ================================================= */

    {
        TabulaDispersa* tabula;
                    i32 i;
              character buffer[VI];

        imprimere("\n--- Probans distantia_maxima tracking ---\n");

        /* Tabula parva coactiones longas distantias probationis */
        tabula = tabula_dispersa_creare_chorda(piscina, XVI);

        per (i = ZEPHYRUM; i < XII; i++)
        {
            chorda clavis;

            snprintf(buffer, VI, "k%d", i);
            clavis = chorda_ex_literis(buffer, piscina);
            tabula_dispersa_inserere(tabula, clavis, (vacuum*)(longus)i);
        }

        /* Distantia aliqua probationis habere debet */
        CREDO_VERUM(tabula->distantia_maxima >= ZEPHYRUM);
    }


    /* =================================================
     * Probare capacitas minima
     * ================================================= */

    {
        TabulaDispersa* tabula_parva;
        TabulaDispersa* tabula_nulla;

        imprimere("\n--- Probans capacitas minima ---\n");

        /* Capacitas I petita, XVI obtenta */
        tabula_parva = tabula_dispersa_creare_chorda(piscina, I);
        CREDO_NON_NIHIL(tabula_parva);
        CREDO_AEQUALIS_I32((i32)tabula_parva->capacitas, XVI);

        /* Capacitas V petita, XVI obtenta */
        tabula_nulla = tabula_dispersa_creare_chorda(piscina, V);
        CREDO_NON_NIHIL(tabula_nulla);
        CREDO_AEQUALIS_I32((i32)tabula_nulla->capacitas, XVI);

        /* Capacitas XV petita, XVI obtenta */
        tabula_nulla = tabula_dispersa_creare_chorda(piscina, XV);
        CREDO_NON_NIHIL(tabula_nulla);
        CREDO_AEQUALIS_I32((i32)tabula_nulla->capacitas, XVI);

        /* Capacitas XVI petita, XVI obtenta */
        tabula_nulla = tabula_dispersa_creare_chorda(piscina, XVI);
        CREDO_NON_NIHIL(tabula_nulla);
        CREDO_AEQUALIS_I32((i32)tabula_nulla->capacitas, XVI);

        /* Capacitas XVII petita, XXXII obtenta */
        tabula_nulla = tabula_dispersa_creare_chorda(piscina, XVII);
        CREDO_NON_NIHIL(tabula_nulla);
        CREDO_AEQUALIS_I32((i32)tabula_nulla->capacitas, XXXII);
    }


    /* =================================================
     * Probare NIHIL piscina et functiones
     * ================================================= */

    {
        TabulaDispersa* tabula_nulla;

        imprimere("\n--- Probans NIHIL piscina ---\n");

        /* Creatio cum piscina NIHIL */
        tabula_nulla = tabula_dispersa_creare_chorda(NIHIL, CXXVIII);
        CREDO_NIHIL(tabula_nulla);

        /* Creatio cum friatio NIHIL */
        tabula_nulla = tabula_dispersa_creare(piscina, CXXVIII, NIHIL, (TabulaComparatio)chorda_comparare);
        CREDO_NIHIL(tabula_nulla);

        /* Creatio cum comparatio NIHIL */
        tabula_nulla = tabula_dispersa_creare(piscina, CXXVIII, tabula_friare_fnv1a, NIHIL);
        CREDO_NIHIL(tabula_nulla);
    }


    /* =================================================
     * Compendium
     * ================================================= */

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    /* Purificatio */
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
