/* probatio_tabula_dispersa.c - Probationes Tabulae Dispersae */
#include "postulata_posix.h"
#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "tabula_dispersa.h"
#include "credo.h"
#include <stdio.h>


/* =================================================
 * Probare tabula_dispersa_creare
 * ================================================= */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        TabulaDispersa* tabula;

        tabula = tabula_dispersa_creare_chorda(piscina, CXXVIII);
        CREDO_NON_NIHIL(tabula);
        CREDO_AEQUALIS_I32((i32)tabula->numerus, ZEPHYRUM);
        CREDO_AEQUALIS_I32((i32)tabula->capacitas, CXXVIII);
    }
}


/* =================================================
 * Probare tabula_dispersa_inserere et invenire
 * ================================================= */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        TabulaDispersa* tabula;
                chorda  clavis1, clavis2, clavis_nonexistens;
                   i32  valor1 = XLII;
                   i32  valor2 = XCIX;
                vacuum* valor_recuperatus;

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
        CREDO_VERUM(tabula_dispersa_invenire(tabula, clavis1,
            &valor_recuperatus));
        CREDO_AEQUALIS_I32((i32)(longus)valor_recuperatus, valor1);

        CREDO_VERUM(tabula_dispersa_invenire(tabula, clavis2,
            &valor_recuperatus));
        CREDO_AEQUALIS_I32((i32)(longus)valor_recuperatus, valor2);

        /* Quaestio nonexistentis */
        clavis_nonexistens = chorda_ex_literis("charlie", piscina);
        CREDO_FALSUM(tabula_dispersa_invenire(
                        tabula,
                        clavis_nonexistens,
                        &valor_recuperatus));
    }
}


/* =================================================
 * Probare tabula_dispersa_continet
 * ================================================= */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        TabulaDispersa* tabula;
                chorda  clavis;

        tabula = tabula_dispersa_creare_chorda(piscina, CXXVIII);
        clavis = chorda_ex_literis("test", piscina);

        CREDO_FALSUM(tabula_dispersa_continet(tabula, clavis));

        tabula_dispersa_inserere(tabula, clavis,
            (vacuum*)(longus)CXXIII);
        CREDO_VERUM(tabula_dispersa_continet(tabula, clavis));
    }
}


/* =================================================
 * Probare tabula_dispersa_inserere_update
 * ================================================= */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        TabulaDispersa* tabula;
                chorda  clavis;
                   i32  valor1 = 10;
                   i32  valor2 = 20;
                vacuum* valor_recuperatus;

        tabula = tabula_dispersa_creare_chorda(piscina, CXXVIII);
        clavis = chorda_ex_literis("key", piscina);

        /* Insertio */
        CREDO_VERUM(tabula_dispersa_inserere(tabula, clavis,
            (vacuum*)(longus)valor1));
        CREDO_AEQUALIS_I32((i32)tabula->numerus, I);

        /* Renovatio eiusdem clavis */
        CREDO_VERUM(tabula_dispersa_inserere(tabula, clavis,
            (vacuum*)(longus)valor2));
        CREDO_AEQUALIS_I32((i32)tabula->numerus, I);

        /* Verificatio valoris renovati */
        CREDO_VERUM(tabula_dispersa_invenire(tabula, clavis,
            &valor_recuperatus));
        CREDO_AEQUALIS_I32((i32)(longus)valor_recuperatus, valor2);
    }
}


/* =================================================
 * Probare tabula_dispersa_delere
 * ================================================= */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        TabulaDispersa* tabula;
                chorda  clavis1, clavis2, clavis_nonexistens;
                vacuum* valor_recuperatus;

        tabula   = tabula_dispersa_creare_chorda(piscina, CXXVIII);
        clavis1  = chorda_ex_literis("alice", piscina);
        clavis2  = chorda_ex_literis("bob", piscina);

        /* Insertiones amborum */
        tabula_dispersa_inserere(tabula, clavis1, (vacuum*)(longus)I);
        tabula_dispersa_inserere(tabula, clavis2, (vacuum*)(longus)II);
        CREDO_AEQUALIS_I32((i32)tabula->numerus, II);

        /* Deletio primi */
        CREDO_VERUM(tabula_dispersa_delere(tabula, clavis1));
        CREDO_AEQUALIS_I32((i32)tabula->numerus, I);

        /* Verificatio deletionis */
        CREDO_FALSUM(tabula_dispersa_invenire(tabula, clavis1,
            &valor_recuperatus));

        /* Secundus adhuc adest */
        CREDO_VERUM(tabula_dispersa_invenire(tabula, clavis2,
            &valor_recuperatus));

        /* Deletio nonexistentis */
        clavis_nonexistens = chorda_ex_literis("charlie", piscina);
        CREDO_FALSUM(tabula_dispersa_delere(tabula,
            clavis_nonexistens));
        CREDO_AEQUALIS_I32((i32)tabula->numerus, I);
    }
}


/* =================================================
 * Probare tabula_dispersa_vacare
 * ================================================= */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        TabulaDispersa* tabula;
                chorda  clavis, clavis2;

        tabula = tabula_dispersa_creare_chorda(piscina, CXXVIII);
        clavis = chorda_ex_literis("test", piscina);

        tabula_dispersa_inserere(tabula, clavis, (vacuum*)(longus)XLII);
        CREDO_AEQUALIS_I32((i32)tabula->numerus, I);

        /* Vaccare */
        tabula_dispersa_vacare(tabula);
        CREDO_AEQUALIS_I32((i32)tabula->numerus, ZEPHYRUM);

        /* Reusum tabulae */
        clavis2 = chorda_ex_literis("newkey", piscina);
        CREDO_VERUM(tabula_dispersa_inserere(tabula, clavis2,
            (vacuum*)(longus)XCIX));
        CREDO_AEQUALIS_I32((i32)tabula->numerus, I);
    }
}


/* =================================================
 * Probare tabula_dispersa_crescere
 * ================================================= */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        TabulaDispersa* tabula;
                   i32  capacitas_initialis;
                   i32  i;
             character  buffer[VI];

        tabula = tabula_dispersa_creare_chorda(piscina, XVI);
        capacitas_initialis = tabula->capacitas;
        CREDO_AEQUALIS_I32(capacitas_initialis, XVI);

        /* Insertiones antequam crescentia */
        per (i = ZEPHYRUM; i < XIV; i++)
        {
            chorda clavis;

            snprintf(buffer, VI, "k%d", i);
            clavis = chorda_ex_literis(buffer, piscina);
            tabula_dispersa_inserere(tabula, clavis,
                (vacuum*)(longus)i);
        }

        /* Oportet crevisse */
        CREDO_VERUM(tabula->capacitas > capacitas_initialis);

        /* Omnes introitus adhuc accessibiles */
        per (i = ZEPHYRUM; i < XIV; i++)
        {
            chorda  clavis;
            vacuum* valor;

            snprintf(buffer, VI, "k%d", i);
            clavis = chorda_ex_literis(buffer, piscina);
            CREDO_VERUM(tabula_dispersa_invenire(tabula, clavis,
                &valor));
            CREDO_AEQUALIS_I32((i32)(longus)valor, i);
        }
    }
}


/* =================================================
 * Probare tabula_dispersa_iterator
 * ================================================= */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        TabulaDispersa* tabula;
                chorda  clavis1, clavis2, clavis3;
        TabulaIterator  iter;
                chorda  clavis_iter;
                vacuum* valor_iter;
                   i32  count;

        tabula = tabula_dispersa_creare_chorda(piscina, CXXVIII);

        clavis1 = chorda_ex_literis("first", piscina);
        clavis2 = chorda_ex_literis("second", piscina);
        clavis3 = chorda_ex_literis("third", piscina);

        tabula_dispersa_inserere(tabula, clavis1, (vacuum*)(longus)I);
        tabula_dispersa_inserere(tabula, clavis2, (vacuum*)(longus)II);
        tabula_dispersa_inserere(tabula, clavis3, (vacuum*)(longus)III);

        /* Ambulare et computare */
        iter   = tabula_dispersa_iterator_initium(tabula);
        count  = ZEPHYRUM;

        dum (tabula_dispersa_iterator_proximum(&iter, &clavis_iter,
            &valor_iter))
        {
            count++;
        }

        CREDO_AEQUALIS_I32(count, III);
    }
}


/* =================================================
 * Probare tabula_dispersa_factor_oneris
 * ================================================= */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        TabulaDispersa* tabula;
                chorda  clavis;
                   f32  factor;
                   i32  i;
             character  buffer[VI];

        tabula = tabula_dispersa_creare_chorda(piscina, CCLVI);

        /* Tabula vacua */
        factor = tabula_dispersa_factor_oneris(tabula);
        CREDO_VERUM(factor >= 0.0f && factor <= 0.01f);

        /* Addere aliquos introitus */
        per (i = ZEPHYRUM; i < LXIV; i++)
        {
            snprintf(buffer, VI, "k%d", i);
            clavis = chorda_ex_literis(buffer, piscina);
            tabula_dispersa_inserere(tabula, clavis,
                (vacuum*)(longus)i);
        }

        factor = tabula_dispersa_factor_oneris(tabula);
        CREDO_VERUM(factor > 0.2f && factor < 0.3f);
    }
}


/* =================================================
 * Probare tabula_dispersa_numerus
 * ================================================= */

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        TabulaDispersa* tabula;
                chorda  clavis;

        tabula = tabula_dispersa_creare_chorda(piscina, CXXVIII);

        CREDO_AEQUALIS_I32((i32)tabula_dispersa_numerus(tabula),
            ZEPHYRUM);

        clavis = chorda_ex_literis("one", piscina);
        tabula_dispersa_inserere(tabula, clavis, (vacuum*)(longus)I);
        CREDO_AEQUALIS_I32((i32)tabula_dispersa_numerus(tabula), I);

        tabula_dispersa_delere(tabula, clavis);
        CREDO_AEQUALIS_I32((i32)tabula_dispersa_numerus(tabula),
            ZEPHYRUM);
    }
}


/* =================================================
 * Probare collisiones tracking
 * ================================================= */

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        TabulaDispersa* tabula;
                   i32  i;
             character  buffer[VI];
                   i32  collisiones_ante;


        tabula = tabula_dispersa_creare_chorda(piscina, XVI);

        collisiones_ante = tabula->collisiones_totales;
        CREDO_AEQUALIS_I32((i32)collisiones_ante, ZEPHYRUM);

        /* Insertiones multorum rerum ad coactiones collidendas */
        per (i = ZEPHYRUM; i < XXXII; i++)
        {
            chorda clavis;

            snprintf(buffer, VI, "k%d", i);
            clavis = chorda_ex_literis(buffer, piscina);
            tabula_dispersa_inserere(tabula, clavis,
                (vacuum*)(longus)i);
        }

        /* Aliquae collisiones debent accidisse */
        CREDO_VERUM(tabula->collisiones_totales > ZEPHYRUM);
    }
}


/* =================================================
 * Probare NIHIL key handling
 * ================================================= */

interior vacuum
_sectio_decima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        TabulaDispersa* tabula;
                chorda  clavis_vacua;

        tabula = tabula_dispersa_creare_chorda(piscina, CXXVIII);
        clavis_vacua.mensura = ZEPHYRUM;
        clavis_vacua.datum = NIHIL;

        /* Clavis vacua non debet inserere */
        CREDO_FALSUM(tabula_dispersa_inserere(
                        tabula, clavis_vacua, (vacuum*)(longus)I));

        CREDO_FALSUM(tabula_dispersa_invenire(
                        tabula, clavis_vacua, NIHIL));
    }
}


/* =================================================
 * Probans functiones friationis multiplices
 * ================================================= */

interior vacuum
_sectio_decima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        TabulaDispersa* tabula1;
        TabulaDispersa* tabula2;
                chorda  clavis;
                   i32  i;
             character  buffer[VI];

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

            CREDO_VERUM(tabula_dispersa_inserere(tabula1, clavis,
                (vacuum*)(longus)i));
            CREDO_VERUM(tabula_dispersa_inserere(tabula2, clavis,
                (vacuum*)(longus)i));
        }

        /* Verificare quaestiones operantur */
        per (i = ZEPHYRUM; i < X; i++)
        {
            vacuum* valor1 = NIHIL;
            vacuum* valor2 = NIHIL;

            snprintf(buffer, VI, "k%d", i);
            clavis = chorda_ex_literis(buffer, piscina);

            CREDO_VERUM(tabula_dispersa_invenire(tabula1, clavis,
                &valor1));
            CREDO_VERUM(tabula_dispersa_invenire(tabula2, clavis,
                &valor2));

            CREDO_AEQUALIS_I32((i32)(longus)valor1, i);
            CREDO_AEQUALIS_I32((i32)(longus)valor2, i);
        }
    }
}


/* =================================================
 * Probare factor_deletorum et crescentia
 * ================================================= */

interior vacuum
_sectio_decima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        TabulaDispersa* tabula;
                chorda  clavis;
                   i32  i;
             character  buffer[VI];
                   i32  capacitas_initialis;

        tabula = tabula_dispersa_creare_chorda(piscina, LXIV);
        capacitas_initialis = tabula->capacitas;

        /* Implere ad plus quam 25% */
        per (i = ZEPHYRUM; i < XVII; i++)
        {
            snprintf(buffer, VI, "k%d", i);
            clavis = chorda_ex_literis(buffer, piscina);
            tabula_dispersa_inserere(tabula, clavis,
                (vacuum*)(longus)i);
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
}


/* =================================================
 * Probare iterator cum tabula vacua
 * ================================================= */

interior vacuum
_sectio_decima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
         TabulaDispersa* tabula;
         TabulaIterator  iter;
                 chorda  clavis_iter;
                 vacuum* valor_iter;
                    i32  count;

        tabula = tabula_dispersa_creare_chorda(piscina, CXXVIII);

        /* Ambulare tabulam vacuam */
        iter   = tabula_dispersa_iterator_initium(tabula);
        count  = ZEPHYRUM;

        dum (tabula_dispersa_iterator_proximum(&iter, &clavis_iter,
            &valor_iter))
        {
            count++;
        }

        CREDO_AEQUALIS_I32(count, ZEPHYRUM);
    }
}


/* =================================================
 * Probare iterator cum outputis NIHIL
 * ================================================= */

interior vacuum
_sectio_decima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
         TabulaDispersa* tabula;
         TabulaIterator  iter;
                 chorda clavis1, clavis2, clavis3;
                    i32 count;

        tabula = tabula_dispersa_creare_chorda(piscina, CXXVIII);

        clavis1 = chorda_ex_literis("alpha", piscina);
        clavis2 = chorda_ex_literis("beta", piscina);
        clavis3 = chorda_ex_literis("gamma", piscina);

        tabula_dispersa_inserere(tabula, clavis1, (vacuum*)(longus)I);
        tabula_dispersa_inserere(tabula, clavis2, (vacuum*)(longus)II);
        tabula_dispersa_inserere(tabula, clavis3, (vacuum*)(longus)III);

        /* Ambulare cum NIHIL outputis */
        iter   = tabula_dispersa_iterator_initium(tabula);
        count  = ZEPHYRUM;

        dum (tabula_dispersa_iterator_proximum(&iter, NIHIL, NIHIL))
        {
            count++;
        }

        CREDO_AEQUALIS_I32(count, III);
    }
}


/* =================================================
 * Probare deletio post crescentiam
 * ================================================= */

interior vacuum
_sectio_decima_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
         TabulaDispersa* tabula;
                    i32  i;
              character  buffer[VI];
                 chorda  clavis;
                 vacuum* valor;

        tabula = tabula_dispersa_creare_chorda(piscina, XVI);

        /* Insertiones ad coactiones crescentiae */
        per (i = ZEPHYRUM; i < XIV; i++)
        {
            snprintf(buffer, VI, "k%d", i);
            clavis = chorda_ex_literis(buffer, piscina);
            tabula_dispersa_inserere(tabula, clavis,
                (vacuum*)(longus)i);
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
}


/* =================================================
 * Probare distantia_maxima tracking
 * ================================================= */

interior vacuum
_sectio_decima_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
         TabulaDispersa* tabula;
                    i32  i;
              character  buffer[VI];

        /* Tabula parva coactiones longas distantias probationis */
        tabula = tabula_dispersa_creare_chorda(piscina, XVI);

        per (i = ZEPHYRUM; i < XII; i++)
        {
            chorda clavis;

            snprintf(buffer, VI, "k%d", i);
            clavis = chorda_ex_literis(buffer, piscina);
            tabula_dispersa_inserere(tabula, clavis,
                (vacuum*)(longus)i);
        }

        /* Invarians verus (">= 0" in i32 vana erat): distantia
         * probationis capacitatem numquam excedit */
        CREDO_VERUM(tabula->distantia_maxima < XVI);
    }
}


/* =================================================
 * Probare capacitas minima
 * ================================================= */

interior vacuum
_sectio_decima_nona (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        TabulaDispersa* tabula_parva;
        TabulaDispersa* tabula_nulla;

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
}


/* =================================================
 * Probare tabula_dispersa_invenire_literis
 * ================================================= */

interior vacuum
_sectio_vicesima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        TabulaDispersa* tabula;
                vacuum* valor_recuperatus;
                   i32  valor1 = XLII;
                   i32  valor2 = XCIX;

        tabula = tabula_dispersa_creare_chorda(piscina, CXXVIII);

        /* Insertio cum chorda */
        {
            chorda clavis;
            clavis = chorda_ex_literis("test_key", piscina);
            tabula_dispersa_inserere(tabula, clavis,
                (vacuum*)(longus)valor1);
        }

        /* Quaestio cum literis */
        CREDO_VERUM(tabula_dispersa_invenire_literis(tabula, "test_key",
            &valor_recuperatus));
        CREDO_AEQUALIS_I32((i32)(longus)valor_recuperatus, valor1);

        /* Quaestio clavis non existentis */
        CREDO_FALSUM(tabula_dispersa_invenire_literis(tabula,
            "nonexistent", &valor_recuperatus));

        /* Insertio alterius */
        {
            chorda clavis2;
            clavis2 = chorda_ex_literis("another_key", piscina);
            tabula_dispersa_inserere(tabula, clavis2,
                (vacuum*)(longus)valor2);
        }

        /* Verificare ambas claves */
        CREDO_VERUM(tabula_dispersa_invenire_literis(tabula, "test_key",
            &valor_recuperatus));
        CREDO_AEQUALIS_I32((i32)(longus)valor_recuperatus, valor1);

        CREDO_VERUM(tabula_dispersa_invenire_literis(tabula,
            "another_key", &valor_recuperatus));
        CREDO_AEQUALIS_I32((i32)(longus)valor_recuperatus, valor2);
    }
}


/* =================================================
 * Probare tabula_dispersa_continet_literis
 * ================================================= */

interior vacuum
_sectio_vicesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        TabulaDispersa* tabula;
                chorda  clavis;

        tabula = tabula_dispersa_creare_chorda(piscina, CXXVIII);

        /* Verificare clavis non existens */
        CREDO_FALSUM(tabula_dispersa_continet_literis(tabula,
            "missing"));

        /* Inserere */
        clavis = chorda_ex_literis("present", piscina);
        tabula_dispersa_inserere(tabula, clavis,
            (vacuum*)(longus)CXXIII);

        /* Verificare existit */
        CREDO_VERUM(tabula_dispersa_continet_literis(tabula,
            "present"));
        CREDO_FALSUM(tabula_dispersa_continet_literis(tabula,
            "absent"));
    }
}


/* =================================================
 * Probare NIHIL piscina et functiones
 * ================================================= */

interior vacuum
_sectio_vicesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        TabulaDispersa* tabula_nulla;

        /* Creatio cum piscina NIHIL */
        tabula_nulla = tabula_dispersa_creare_chorda(NIHIL, CXXVIII);
        CREDO_NIHIL(tabula_nulla);

        /* Creatio cum friatio NIHIL */
        tabula_nulla = tabula_dispersa_creare(piscina, CXXVIII, NIHIL,
            (TabulaComparatio)chorda_comparare);
        CREDO_NIHIL(tabula_nulla);

        /* Creatio cum comparatio NIHIL */
        tabula_nulla = tabula_dispersa_creare(piscina, CXXVIII,
            tabula_friare_fnv1a, NIHIL);
        CREDO_NIHIL(tabula_nulla);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "tabula_dispersa_creare", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "tabula_dispersa_inserere et invenire", _sectio_secunda, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "tabula_dispersa_continet", _sectio_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "tabula_dispersa_inserere (update)", _sectio_quarta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "tabula_dispersa_delere", _sectio_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "tabula_dispersa_vacare", _sectio_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "tabula_dispersa_crescere", _sectio_septima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "tabula_dispersa_iterator", _sectio_octava, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "tabula_dispersa_factor_oneris", _sectio_nona, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "tabula_dispersa_numerus", _sectio_decima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "collisiones tracking", _sectio_decima_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "NIHIL key handling", _sectio_decima_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "functiones friationis multiplices", _sectio_decima_tertia, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "factor_deletorum et crescentia", _sectio_decima_quarta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "iterator cum tabula vacua", _sectio_decima_quinta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "iterator cum outputis NIHIL", _sectio_decima_sexta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "deletio post crescentiam", _sectio_decima_septima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "distantia_maxima tracking", _sectio_decima_octava, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "capacitas minima", _sectio_decima_nona, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "tabula_dispersa_invenire_literis", _sectio_vicesima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "tabula_dispersa_continet_literis", _sectio_vicesima_prima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "NIHIL piscina", _sectio_vicesima_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("tabula_dispersa", SECTIONES);
}
