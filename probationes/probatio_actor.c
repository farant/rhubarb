/* probatio_actor.c - Probationes Actoris (Nuntius et Capsa) */
#include "latina.h"
#include "actor.h"
#include "piscina.h"
#include "internamentum.h"
#include "credo.h"
#include <stdio.h>


    /* ==================================================
     * Probare nuntius_creare
     * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    si (intern == NIHIL)
    {
        redde;
    }
    {
        Nuntius* nuntius;
         chorda  mittens_chorda;
         chorda* mittens_id;

        /* Creare mittens_id */
        mittens_chorda  = chorda_ex_literis("actor-123", piscina);
        mittens_id      = chorda_internare(intern, mittens_chorda);
        CREDO_NON_NIHIL(mittens_id);

        /* Creare nuntium cum mittente */
        nuntius = nuntius_creare(piscina, intern, mittens_id, "click");
        CREDO_NON_NIHIL(nuntius);
        CREDO_NON_NIHIL(nuntius->genus);
        CREDO_VERUM(nuntius->mittens_id == mittens_id);
        CREDO_NIHIL(nuntius->datum);  /* Datum allocatur pigre */
        CREDO_VERUM(nuntius->tempus_creatus >= 0.0);

        /* Creare nuntium sine mittente (systema) */
        nuntius = nuntius_creare(piscina, intern, NIHIL, "tick");
        CREDO_NON_NIHIL(nuntius);
        CREDO_NIHIL(nuntius->mittens_id);
        CREDO_NON_NIHIL(nuntius->genus);

        /* Creare nuntium cum argumentis invalidis */
        nuntius = nuntius_creare(NIHIL, intern, NIHIL, "test");
        CREDO_NIHIL(nuntius);

        nuntius = nuntius_creare(piscina, NIHIL, NIHIL, "test");
        CREDO_NIHIL(nuntius);

        nuntius = nuntius_creare(piscina, intern, NIHIL, NIHIL);
        CREDO_NIHIL(nuntius);
    }
}


    /* ==================================================
     * Probare nuntius_datum_ponere et capere
     * ================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    si (intern == NIHIL)
    {
        redde;
    }
    {
        Nuntius* nuntius;
         chorda  valor_chorda;
         chorda* valor_internatum;
         chorda* resultus;

        nuntius = nuntius_creare(piscina, intern, NIHIL, "update");
        CREDO_NON_NIHIL(nuntius);
        CREDO_NIHIL(nuntius->datum);  /* Nondum allocatum */

        /* Ponere valorem - hoc allocat datum */
        valor_chorda      = chorda_ex_literis("mundus", piscina);
        valor_internatum  = chorda_internare(intern, valor_chorda);
        CREDO_VERUM(nuntius_datum_ponere(nuntius, "salutatio",
            valor_internatum));
        CREDO_NON_NIHIL(nuntius->datum);  /* Nunc allocatum */

        /* Capere valorem */
        resultus = nuntius_datum_capere(nuntius, "salutatio");
        CREDO_NON_NIHIL(resultus);
        CREDO_VERUM(resultus == valor_internatum);  /* Pointer aequalis */

        /* Capere clavem non existentem */
        resultus = nuntius_datum_capere(nuntius, "non_existens");
        CREDO_NIHIL(resultus);
    }
}


    /* ==================================================
     * Probare nuntius_datum_ponere_literis
     * ================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    si (intern == NIHIL)
    {
        redde;
    }
    {
        Nuntius* nuntius;
         chorda* resultus;
         chorda  speratus;

        nuntius = nuntius_creare(piscina, intern, NIHIL, "config");
        CREDO_NON_NIHIL(nuntius);

        CREDO_VERUM(nuntius_datum_ponere_literis(nuntius, "titulus",
            "Salve Munde"));
        CREDO_VERUM(nuntius_datum_ponere_literis(nuntius, "descriptio",
            "Probatio"));

        resultus = nuntius_datum_capere(nuntius, "titulus");
        CREDO_NON_NIHIL(resultus);
        speratus = chorda_ex_literis("Salve Munde", piscina);
        CREDO_CHORDA_AEQUALIS(*resultus, speratus);

        resultus = nuntius_datum_capere(nuntius, "descriptio");
        CREDO_NON_NIHIL(resultus);
        speratus = chorda_ex_literis("Probatio", piscina);
        CREDO_CHORDA_AEQUALIS(*resultus, speratus);
    }
}


    /* ==================================================
     * Probare nuntius_datum_ponere_s32 et capere_s32
     * ================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    si (intern == NIHIL)
    {
        redde;
    }
    {
        Nuntius* nuntius;
            s32  valor;
            b32  successus;

        nuntius = nuntius_creare(piscina, intern, NIHIL, "counter");
        CREDO_NON_NIHIL(nuntius);

        /* Ponere valorem positivum */
        CREDO_VERUM(nuntius_datum_ponere_s32(nuntius, "numerus", XLII));

        successus = nuntius_datum_capere_s32(nuntius, "numerus",
            &valor);
        CREDO_VERUM(successus);
        CREDO_AEQUALIS_S32(valor, XLII);

        /* Ponere valorem negativum */
        CREDO_VERUM(nuntius_datum_ponere_s32(nuntius, "negativus", -C));

        successus = nuntius_datum_capere_s32(nuntius, "negativus",
            &valor);
        CREDO_VERUM(successus);
        CREDO_AEQUALIS_S32(valor, -C);

        /* Ponere zephyrum */
        CREDO_VERUM(nuntius_datum_ponere_s32(nuntius, "zephyrus",
            ZEPHYRUM));

        successus = nuntius_datum_capere_s32(nuntius, "zephyrus",
            &valor);
        CREDO_VERUM(successus);
        CREDO_AEQUALIS_S32(valor, ZEPHYRUM);

        /* Capere clavem non existentem */
        successus = nuntius_datum_capere_s32(nuntius, "non_existens",
            &valor);
        CREDO_FALSUM(successus);
    }
}


    /* ==================================================
     * Probare nuntius_datum_capere_s64
     * ================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    si (intern == NIHIL)
    {
        redde;
    }
    {
        Nuntius* nuntius;
            s64  valor;
            b32  successus;

        nuntius = nuntius_creare(piscina, intern, NIHIL, "timestamp");
        CREDO_NON_NIHIL(nuntius);

        /* Ponere valorem magnum */
        CREDO_VERUM(nuntius_datum_ponere_literis(nuntius, "tempus",
            "1234567890123"));

        successus = nuntius_datum_capere_s64(nuntius, "tempus", &valor);
        CREDO_VERUM(successus);
        CREDO_AEQUALIS_S64(valor, 1234567890123LL);

        /* Valorem negativum */
        CREDO_VERUM(nuntius_datum_ponere_literis(nuntius, "negativum",
            "-9876543210"));

        successus = nuntius_datum_capere_s64(nuntius, "negativum",
            &valor);
        CREDO_VERUM(successus);
        CREDO_AEQUALIS_S64(valor, -9876543210LL);
    }
}


    /* ==================================================
     * Probare nuntius_datum_capere_f64
     * ================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    si (intern == NIHIL)
    {
        redde;
    }
    {
        Nuntius* nuntius;
            f64  valor;
            b32  successus;

        nuntius = nuntius_creare(piscina, intern, NIHIL, "metrics");
        CREDO_NON_NIHIL(nuntius);

        CREDO_VERUM(nuntius_datum_ponere_literis(nuntius, "ratio",
            "3.14159"));

        successus = nuntius_datum_capere_f64(nuntius, "ratio", &valor);
        CREDO_VERUM(successus);
        CREDO_VERUM(valor > 3.14 && valor < 3.15);

        /* Valorem negativum */
        CREDO_VERUM(nuntius_datum_ponere_literis(nuntius, "temperatura",
            "-273.15"));

        successus = nuntius_datum_capere_f64(nuntius, "temperatura",
            &valor);
        CREDO_VERUM(successus);
        CREDO_VERUM(valor < -273.0 && valor > -274.0);
    }
}


    /* ==================================================
     * Probare nuntius_datum_capere_b32
     * ================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    si (intern == NIHIL)
    {
        redde;
    }
    {
        Nuntius* nuntius;
            b32  valor;
            b32  successus;

        nuntius = nuntius_creare(piscina, intern, NIHIL, "flags");
        CREDO_NON_NIHIL(nuntius);

        /* Verum */
        CREDO_VERUM(nuntius_datum_ponere_literis(nuntius, "activum",
            "verum"));
        successus = nuntius_datum_capere_b32(nuntius, "activum",
            &valor);
        CREDO_VERUM(successus);
        CREDO_VERUM(valor);

        CREDO_VERUM(nuntius_datum_ponere_literis(nuntius, "activum2",
            "true"));
        successus = nuntius_datum_capere_b32(nuntius, "activum2",
            &valor);
        CREDO_VERUM(successus);
        CREDO_VERUM(valor);

        CREDO_VERUM(nuntius_datum_ponere_literis(nuntius, "activum3",
            "1"));
        successus = nuntius_datum_capere_b32(nuntius, "activum3",
            &valor);
        CREDO_VERUM(successus);
        CREDO_VERUM(valor);

        /* Falsum */
        CREDO_VERUM(nuntius_datum_ponere_literis(nuntius, "inactivum",
            "falsum"));
        successus = nuntius_datum_capere_b32(nuntius, "inactivum",
            &valor);
        CREDO_VERUM(successus);
        CREDO_FALSUM(valor);

        CREDO_VERUM(nuntius_datum_ponere_literis(nuntius, "inactivum2",
            "false"));
        successus = nuntius_datum_capere_b32(nuntius, "inactivum2",
            &valor);
        CREDO_VERUM(successus);
        CREDO_FALSUM(valor);

        CREDO_VERUM(nuntius_datum_ponere_literis(nuntius, "inactivum3",
            "0"));
        successus = nuntius_datum_capere_b32(nuntius, "inactivum3",
            &valor);
        CREDO_VERUM(successus);
        CREDO_FALSUM(valor);

        /* Valor invalidus */
        CREDO_VERUM(nuntius_datum_ponere_literis(nuntius, "invalidum",
            "maybe"));
        successus = nuntius_datum_capere_b32(nuntius, "invalidum",
            &valor);
        CREDO_FALSUM(successus);
    }
}


    /* ==================================================
     * Probare nuntius_datum_habet
     * ================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    si (intern == NIHIL)
    {
        redde;
    }
    {
        Nuntius* nuntius;

        nuntius = nuntius_creare(piscina, intern, NIHIL, "test");
        CREDO_NON_NIHIL(nuntius);

        /* Sine datum - omnes claves non existunt */
        CREDO_FALSUM(nuntius_datum_habet(nuntius, "clavis"));

        /* Addere clavem */
        CREDO_VERUM(nuntius_datum_ponere_literis(nuntius, "clavis",
            "valor"));

        CREDO_VERUM(nuntius_datum_habet(nuntius, "clavis"));
        CREDO_FALSUM(nuntius_datum_habet(nuntius, "alia_clavis"));
    }
}


    /* ==================================================
     * Probare capsa_creare
     * ================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    si (intern == NIHIL)
    {
        redde;
    }
    {
         Capsa* capsa;
        chorda  possessor_chorda;
        chorda* possessor_id;

        possessor_chorda  = chorda_ex_literis("widget-456", piscina);
        possessor_id      = chorda_internare(intern, possessor_chorda);
        CREDO_NON_NIHIL(possessor_id);

        capsa = capsa_creare(piscina, possessor_id);
        CREDO_NON_NIHIL(capsa);
        CREDO_VERUM(capsa->possessor_id == possessor_id);
        CREDO_NON_NIHIL(capsa->nuntii);
        CREDO_AEQUALIS_I32(capsa_numerus(capsa), ZEPHYRUM);
        CREDO_VERUM(capsa_vacua_est(capsa));

        /* Argumenta invalida */
        capsa = capsa_creare(NIHIL, possessor_id);
        CREDO_NIHIL(capsa);

        capsa = capsa_creare(piscina, NIHIL);
        CREDO_NIHIL(capsa);
    }
}


    /* ==================================================
     * Probare capsa_addere et capsa_tollere (FIFO)
     * ================================================== */

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    si (intern == NIHIL)
    {
        redde;
    }
    {
          Capsa* capsa;
         chorda  possessor_chorda;
         chorda* possessor_id;
        Nuntius* n1;
        Nuntius* n2;
        Nuntius* n3;
        Nuntius* resultus;

        possessor_chorda  = chorda_ex_literis("fifo-test", piscina);
        possessor_id      = chorda_internare(intern, possessor_chorda);
        capsa             = capsa_creare(piscina, possessor_id);
        CREDO_NON_NIHIL(capsa);

        /* Creare tres nuntios */
        n1 = nuntius_creare(piscina, intern, NIHIL, "primus");
        n2 = nuntius_creare(piscina, intern, NIHIL, "secundus");
        n3 = nuntius_creare(piscina, intern, NIHIL, "tertius");
        CREDO_NON_NIHIL(n1);
        CREDO_NON_NIHIL(n2);
        CREDO_NON_NIHIL(n3);

        /* Addere in ordine */
        CREDO_VERUM(capsa_addere(capsa, n1));
        CREDO_AEQUALIS_I32(capsa_numerus(capsa), I);
        CREDO_FALSUM(capsa_vacua_est(capsa));

        CREDO_VERUM(capsa_addere(capsa, n2));
        CREDO_AEQUALIS_I32(capsa_numerus(capsa), II);

        CREDO_VERUM(capsa_addere(capsa, n3));
        CREDO_AEQUALIS_I32(capsa_numerus(capsa), III);

        /* Tollere in ordine FIFO */
        resultus = capsa_tollere(capsa);
        CREDO_VERUM(resultus == n1);
        CREDO_AEQUALIS_I32(capsa_numerus(capsa), II);

        resultus = capsa_tollere(capsa);
        CREDO_VERUM(resultus == n2);
        CREDO_AEQUALIS_I32(capsa_numerus(capsa), I);

        resultus = capsa_tollere(capsa);
        CREDO_VERUM(resultus == n3);
        CREDO_AEQUALIS_I32(capsa_numerus(capsa), ZEPHYRUM);
        CREDO_VERUM(capsa_vacua_est(capsa));

        /* Tollere ex capsa vacua */
        resultus = capsa_tollere(capsa);
        CREDO_NIHIL(resultus);
    }
}


    /* ==================================================
     * Probare capsa_inspicere
     * ================================================== */

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    si (intern == NIHIL)
    {
        redde;
    }
    {
          Capsa* capsa;
         chorda  possessor_chorda;
         chorda* possessor_id;
        Nuntius* n1;
        Nuntius* n2;
        Nuntius* resultus;

        possessor_chorda  = chorda_ex_literis("peek-test", piscina);
        possessor_id      = chorda_internare(intern, possessor_chorda);
        capsa             = capsa_creare(piscina, possessor_id);
        CREDO_NON_NIHIL(capsa);

        /* Inspicere capsam vacuam */
        resultus = capsa_inspicere(capsa);
        CREDO_NIHIL(resultus);

        /* Addere et inspicere */
        n1 = nuntius_creare(piscina, intern, NIHIL, "primus");
        n2 = nuntius_creare(piscina, intern, NIHIL, "secundus");

        CREDO_VERUM(capsa_addere(capsa, n1));
        CREDO_VERUM(capsa_addere(capsa, n2));

        /* Inspicere non removet */
        resultus = capsa_inspicere(capsa);
        CREDO_VERUM(resultus == n1);
        CREDO_AEQUALIS_I32(capsa_numerus(capsa), II);

        /* Iterum inspicere - idem resultus */
        resultus = capsa_inspicere(capsa);
        CREDO_VERUM(resultus == n1);
        CREDO_AEQUALIS_I32(capsa_numerus(capsa), II);

        /* Post tollere, inspicere dat secundum */
        capsa_tollere(capsa);
        resultus = capsa_inspicere(capsa);
        CREDO_VERUM(resultus == n2);
    }
}


    /* ==================================================
     * Probare capsa_vacare
     * ================================================== */

interior vacuum
_sectio_decima_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    si (intern == NIHIL)
    {
        redde;
    }
    {
          Capsa* capsa;
         chorda  possessor_chorda;
         chorda* possessor_id;
        Nuntius* n1;
        Nuntius* n2;

        possessor_chorda  = chorda_ex_literis("clear-test", piscina);
        possessor_id      = chorda_internare(intern, possessor_chorda);
        capsa             = capsa_creare(piscina, possessor_id);
        CREDO_NON_NIHIL(capsa);

        n1 = nuntius_creare(piscina, intern, NIHIL, "primus");
        n2 = nuntius_creare(piscina, intern, NIHIL, "secundus");

        CREDO_VERUM(capsa_addere(capsa, n1));
        CREDO_VERUM(capsa_addere(capsa, n2));
        CREDO_AEQUALIS_I32(capsa_numerus(capsa), II);

        capsa_vacare(capsa);

        CREDO_AEQUALIS_I32(capsa_numerus(capsa), ZEPHYRUM);
        CREDO_VERUM(capsa_vacua_est(capsa));
        CREDO_NIHIL(capsa_inspicere(capsa));
    }
}


    /* ==================================================
     * Probare nuntius cum multis clavibus
     * ================================================== */

interior vacuum
_sectio_decima_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    si (intern == NIHIL)
    {
        redde;
    }
    {
          Nuntius* nuntius;
              s32  i;
              s32  valor;
              b32  successus;
        character  clavis_buffer[XXXII];

        nuntius = nuntius_creare(piscina, intern, NIHIL, "bulk");
        CREDO_NON_NIHIL(nuntius);

        /* Addere multas claves */
        per (i = ZEPHYRUM; i < C; i++)
        {
            sprintf(clavis_buffer, "clavis_%d", (integer)i);
            CREDO_VERUM(nuntius_datum_ponere_s32(nuntius, clavis_buffer,
                i * X));
        }

        /* Verificare omnes */
        per (i = ZEPHYRUM; i < C; i++)
        {
            sprintf(clavis_buffer, "clavis_%d", (integer)i);
            successus = nuntius_datum_capere_s32(nuntius, clavis_buffer,
                &valor);
            CREDO_VERUM(successus);
            CREDO_AEQUALIS_S32(valor, i * X);
        }
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "nuntius_creare", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nuntius_datum", _sectio_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nuntius_datum_ponere_literis", _sectio_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "nuntius_datum_s32", _sectio_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nuntius_datum_s64", _sectio_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nuntius_datum_f64", _sectio_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nuntius_datum_b32", _sectio_septima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "nuntius_datum_habet", _sectio_octava, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "capsa_creare", _sectio_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "capsa FIFO", _sectio_decima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "capsa_inspicere", _sectio_decima_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "capsa_vacare", _sectio_decima_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "nuntius_multae_claves", _sectio_decima_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("actor", SECTIONES);
}
