/* probatio_officina_regio.c - Regio: memoria possessa (M2a)
 *
 * Probat: basim fixam (determinismus), fines (continet), globalia
 * (ordinatio), acervum (XVI-ordinatio, reusus LIFO identicus,
 * scissio, validitas liberationis, venenum custodiae), reallocare
 * (contractio idem monstrator, amplificatio copiat), censum. */

#include "officina_regio.h"
#include "credo.h"

#include <stdio.h>
#include <string.h>


/* ==================================================
 * PROBARE: vita + basis fixa + fines
 * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        Regio* regio;
           i8* basis;

        regio = regio_generare(piscina);
        CREDO_NON_NIHIL (regio);
        basis = (i8*)regio_basis(regio);

        /* basis fixa = determinismus (pinna: valor ipse) */
        CREDO_VERUM ((memoriae_index)(vacuum*)basis
            == (memoriae_index)0x300000000000UL);

        /* fines */
        CREDO_VERUM (regio_continet(regio, basis));
        CREDO_VERUM (regio_continet(regio,
            basis + regio_magnitudo_tota(regio) - I));
        CREDO_FALSUM (regio_continet(regio,
            basis + regio_magnitudo_tota(regio)));
        CREDO_FALSUM (regio_continet(regio, (vacuum*)&piscina));

        regio_destruere(regio);

        /* iterum generare -> EADEM basis (determinismus trans
         * cyclos) */
        regio = regio_generare(piscina);
        CREDO_NON_NIHIL (regio);
        CREDO_AEQUALIS_PTR (regio_basis(regio), (vacuum*)basis);
        regio_destruere(regio);
    }
}


/* ==================================================
 * PROBARE: globalia - cumulus cum ordinatione
 * ================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        Regio* regio = regio_generare(piscina);
           i8* a;
           i8* b;
           i8* tertium;

        CREDO_NON_NIHIL (regio);

        a = (i8*)regio_globalia_allocare(regio, III, I);
        CREDO_NON_NIHIL (a);
        CREDO_AEQUALIS_PTR ((vacuum*)a, regio_basis(regio));

        /* ordinatio VIII: cursor III rotundatur ad VIII */
        b = (i8*)regio_globalia_allocare(regio, VIII, VIII);
        CREDO_NON_NIHIL (b);
        CREDO_VERUM ((memoriae_index)(vacuum*)b % VIII == ZEPHYRUM);
        CREDO_AEQUALIS_PTR ((vacuum*)b, (vacuum*)(a + VIII));

        /* ordinatio XVI */
        tertium = (i8*)regio_globalia_allocare(regio, I, XVI);
        CREDO_NON_NIHIL (tertium);
        CREDO_VERUM ((memoriae_index)(vacuum*)tertium % XVI
            == ZEPHYRUM);

        /* usus crescit */
        CREDO_VERUM (regio_globalia_usus(regio) >= XVI + I);

        /* scribere/legere per aream */
        memset(a, 0x5A, III);
        CREDO_AEQUALIS_I32 ((i32)a[ZEPHYRUM], 0x5A);

        regio_destruere(regio);
    }
}


/* ==================================================
 * PROBARE: stiva - fines intra regionem
 * ================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        Regio* regio = regio_generare(piscina);
           i8* stiva;

        stiva = (i8*)regio_stiva_initium(regio);
        CREDO_NON_NIHIL (stiva);
        CREDO_VERUM (regio_continet(regio, stiva));
        CREDO_VERUM (regio_continet(regio,
            stiva + regio_stiva_magnitudo_octetorum(regio) - I));
        CREDO_VERUM (regio_stiva_magnitudo_octetorum(regio)
            >= 1073741824UL);

        regio_destruere(regio);
    }
}


/* ==================================================
 * PROBARE: acervus - ordinatio + reusus LIFO + scissio
 * ================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        Regio* regio = regio_generare(piscina);
           i8* a;
           i8* b;
           i8* magnum;
           i8* iterum;

        a = (i8*)regio_allocare(regio, 100);
        b = (i8*)regio_allocare(regio, 50);
        CREDO_NON_NIHIL (a);
        CREDO_NON_NIHIL (b);
        CREDO_VERUM ((memoriae_index)(vacuum*)a % XVI == ZEPHYRUM);
        CREDO_VERUM ((memoriae_index)(vacuum*)b % XVI == ZEPHYRUM);
        CREDO_VERUM (a != b);
        CREDO_VERUM (regio_continet(regio, a));

        /* scribere/legere */
        memset(a, 0x42, 100);
        memset(b, 0x24, 50);
        CREDO_AEQUALIS_I32 ((i32)a[99], 0x42);
        CREDO_AEQUALIS_I32 ((i32)b[49], 0x24);

        /* reusus LIFO: liberare b, allocare 50 -> IDEM monstrator
         * (determinismus allocatoris - pinna) */
        CREDO_VERUM (regio_liberare(regio, b));
        iterum = (i8*)regio_allocare(regio, 50);
        CREDO_AEQUALIS_PTR ((vacuum*)iterum, (vacuum*)b);

        /* scissio: liberare locellum magnum, allocare parvum ->
         * basis eadem, residuum reusabile */
        magnum = (i8*)regio_allocare(regio, 1000);
        CREDO_NON_NIHIL (magnum);
        CREDO_VERUM (regio_liberare(regio, magnum));
        iterum = (i8*)regio_allocare(regio, 100);
        CREDO_AEQUALIS_PTR ((vacuum*)iterum, (vacuum*)magnum);
        /* residuum scissum in indice libero: allocatio parva sequens
         * intra locellum veterem cadit */
        iterum = (i8*)regio_allocare(regio, 100);
        CREDO_VERUM (iterum > magnum && iterum < magnum + 1000);

        regio_destruere(regio);
    }
}


/* ==================================================
 * PROBARE: liberare - validitas
 * ================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        Regio* regio = regio_generare(piscina);
           i8* a;
           i8  alienus[64];

        a = (i8*)regio_allocare(regio, 64);
        CREDO_NON_NIHIL (a);

        /* monstrator alienus (extra regionem) */
        CREDO_FALSUM (regio_liberare(regio, alienus + XVI));
        /* NIHIL */
        CREDO_FALSUM (regio_liberare(regio, NIHIL));
        /* medium locelli (non XVI-ordinatum ad caput) */
        CREDO_FALSUM (regio_liberare(regio, a + I));

        /* liberatio recta */
        CREDO_VERUM (regio_liberare(regio, a));
        /* liberatio duplex */
        CREDO_FALSUM (regio_liberare(regio, a));

        regio_destruere(regio);
    }
}


/* ==================================================
 * PROBARE: custodia - venenum
 * ================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        Regio* regio = regio_generare(piscina);
           i8* a;

        CREDO_FALSUM (regio_custodia(regio));
        regio_custodiam_ponere(regio, VERUM);
        CREDO_VERUM (regio_custodia(regio));

        /* venenum allocationis 0xAA */
        a = (i8*)regio_allocare(regio, 32);
        CREDO_AEQUALIS_I32 ((i32)a[ZEPHYRUM], 0xAA);
        CREDO_AEQUALIS_I32 ((i32)a[31], 0xAA);

        /* venenum liberationis 0xDD (legere post liberationem -
         * allocator noster, probatio nostra) */
        CREDO_VERUM (regio_liberare(regio, a));
        CREDO_AEQUALIS_I32 ((i32)a[ZEPHYRUM], 0xDD);

        regio_destruere(regio);
    }
}


/* ==================================================
 * PROBARE: reallocare
 * ================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        Regio* regio = regio_generare(piscina);
           i8* a;
           i8* b;

        a = (i8*)regio_allocare(regio, 64);
        memset(a, 0x77, 64);

        /* contractio: idem monstrator */
        b = (i8*)regio_reallocare(regio, a, 32);
        CREDO_AEQUALIS_PTR ((vacuum*)b, (vacuum*)a);

        /* amplificatio: novus monstrator, contenta copiata */
        b = (i8*)regio_reallocare(regio, a, 256);
        CREDO_NON_NIHIL (b);
        CREDO_VERUM (b != a);
        CREDO_AEQUALIS_I32 ((i32)b[ZEPHYRUM], 0x77);
        CREDO_AEQUALIS_I32 ((i32)b[63], 0x77);

        /* NIHIL -> allocare */
        a = (i8*)regio_reallocare(regio, NIHIL, XVI);
        CREDO_NON_NIHIL (a);

        regio_destruere(regio);
    }
}


/* ==================================================
 * PROBARE: census
 * ================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        Regio* regio = regio_generare(piscina);
           i8* a;
           i8* b;

        a = (i8*)regio_allocare(regio, 100);
        b = (i8*)regio_allocare(regio, 200);
        CREDO_NON_NIHIL (a);
        CREDO_NON_NIHIL (b);
        /* 100->112, 200->208 rotundata */
        CREDO_VERUM (regio_acervus_usus(regio) == 320UL);
        CREDO_VERUM (regio_acervus_apex(regio) == 320UL);
        CREDO_VERUM (regio_numerus_allocationum(regio) == II);

        regio_liberare(regio, a);
        CREDO_VERUM (regio_acervus_usus(regio) == 208UL);
        CREDO_VERUM (regio_acervus_apex(regio) == 320UL);
        CREDO_VERUM (regio_numerus_liberationum(regio) == I);

        regio_destruere(regio);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "vitam + basim fixam", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "globalia", _sectio_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "stivam", _sectio_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "acervum", _sectio_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "validitatem liberationis", _sectio_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "custodiam (venenum)", _sectio_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "reallocare", _sectio_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "censum", _sectio_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("officina_regio", SECTIONES);
}
