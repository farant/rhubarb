/* probatio_internamentum.c - Probationes Internamenti Chordae */
#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "internamentum.h"
#include "credo.h"
#include <stdio.h>


    /* ==================================================
     * Probare internamentum_creare
     * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        InternamentumChorda* intern;

        intern = internamentum_creare(piscina);
        CREDO_NON_NIHIL(intern);
        CREDO_NON_NIHIL(intern->tabula);
        CREDO_NON_NIHIL(intern->piscina);
        CREDO_AEQUALIS_I32(internamentum_numerus(intern), ZEPHYRUM);
    }
}


    /* ==================================================
     * Probare chorda_internare - aequales chordas
     * ================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        InternamentumChorda* intern;
        chorda s1, s2;
        chorda *interned1, *interned2;

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
}


    /* ==================================================
     * Probare chorda_internare - differentes chordas
     * ================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        InternamentumChorda* intern;
        chorda s1, s2, s3;
        chorda *interned1, *interned2, *interned3;

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
}


    /* ==================================================
     * Probare chorda_internare_ex_literis
     * ================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        InternamentumChorda* intern;
        chorda *interned1, *interned2;

        intern = internamentum_creare(piscina);

        interned1 = chorda_internare_ex_literis(intern, "test");
        interned2 = chorda_internare_ex_literis(intern, "test");

        /* Debet redde eundem pointer */
        CREDO_NON_NIHIL(interned1);
        CREDO_VERUM(interned1 == interned2);

        /* Verificare contentum */
        CREDO_AEQUALIS_I32(interned1->mensura, IV);
        CREDO_CHORDA_AEQUALIS(*interned1, chorda_ex_literis("test",
            piscina));
    }
}


    /* ==================================================
     * Probare chorda_est_internata
     * ================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        InternamentumChorda* intern;
        chorda s1, s2;

        intern = internamentum_creare(piscina);

        s1 = chorda_ex_literis("internata", piscina);
        s2 = chorda_ex_literis("non_internata", piscina);

        CREDO_FALSUM(chorda_est_internata(intern, s1));

        chorda_internare(intern, s1);

        CREDO_VERUM(chorda_est_internata(intern, s1));
        CREDO_FALSUM(chorda_est_internata(intern, s2));
    }
}


    /* ==================================================
     * Probare pointer equality pro comparatio
     * ================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        InternamentumChorda* intern;
        chorda *id1, *id2, *id3;

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
}


    /* ==================================================
     * Probare internamentum_vacare
     * ================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        InternamentumChorda* intern;
                     chorda* interned;

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
}


    /* ==================================================
     * Probare multa internationes
     * ================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        InternamentumChorda* intern;
                     chorda* interned[X];
                        i32  i;

        intern = internamentum_creare(piscina);

        /* Internare X chordas differentes */
        interned[ZEPHYRUM] = chorda_internare_ex_literis(intern,
            "zero");
        interned[I] = chorda_internare_ex_literis(intern,
            "unus");
        interned[II] = chorda_internare_ex_literis(intern, "duo");
        interned[III] = chorda_internare_ex_literis(intern,
            "tres");
        interned[IV] = chorda_internare_ex_literis(intern,
            "quattuor");
        interned[V] = chorda_internare_ex_literis(intern,
            "quinque");
        interned[VI] = chorda_internare_ex_literis(intern, "sex");
        interned[VII] = chorda_internare_ex_literis(intern,
            "septem");
        interned[VIII] = chorda_internare_ex_literis(intern,
            "octo");
        interned[IX] = chorda_internare_ex_literis(intern,
            "novem");

        CREDO_AEQUALIS_I32(internamentum_numerus(intern), X);

        /* Verificare omnes sunt non-nihil et uniques */
        per (i = ZEPHYRUM; i < X; i++)
        {
            CREDO_NON_NIHIL(interned[i]);
        }

        /* Internare duplicatos - debet redde eosdem pointers */
        CREDO_VERUM(chorda_internare_ex_literis(intern, "zero")
            == interned[ZEPHYRUM]);
        CREDO_VERUM(chorda_internare_ex_literis(intern, "quinque")
            == interned[V]);

        /* Numerus adhuc X */
        CREDO_AEQUALIS_I32(internamentum_numerus(intern), X);
    }
}


    /* ==================================================
     * Probare chordam VACUAM - CONTRACTUS (2026-09-22): "" NON
     * internatur, NIHIL redditur. actio, insula et stml (titulus
     * '< >' = NIHIL) hoc ut recusationem adhibent; audit json id
     * mutare tentavit et porta radix refutavit. Haec figura
     * contractum tenet ne iterum tacite mutetur.
     * ================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        InternamentumChorda* intern;
                     chorda  vacua_sine_datis;
                     chorda  vacua_cum_datis;

        intern = internamentum_creare(piscina);

        vacua_sine_datis.datum    = NIHIL;
        vacua_sine_datis.mensura  = ZEPHYRUM;
        vacua_cum_datis           = chorda_ex_literis("x", piscina);
        vacua_cum_datis.mensura   = ZEPHYRUM;

        CREDO_NIHIL(chorda_internare(intern, vacua_sine_datis));
        CREDO_NIHIL(chorda_internare(intern, vacua_cum_datis));
        CREDO_NIHIL(chorda_internare_ex_literis(intern, ""));
        CREDO_FALSUM(chorda_est_internata(intern, vacua_cum_datis));
        CREDO_AEQUALIS_I32(internamentum_numerus(intern), ZEPHYRUM);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "internamentum_creare", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "chorda_internare (aequales)", _sectio_secunda, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "chorda_internare (differentes)", _sectio_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "chorda_internare_ex_literis", _sectio_quarta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "chorda_est_internata", _sectio_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "pointer equality", _sectio_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "internamentum_vacare", _sectio_septima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "multa internationes", _sectio_octava, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "chordam vacuam (recusatur)", _sectio_nona, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("internamentum", SECTIONES);
}
