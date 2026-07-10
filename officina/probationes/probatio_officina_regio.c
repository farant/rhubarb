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

s32 principale (vacuum)
{
    Piscina* piscina;
    b32 praeteritus;

    piscina = piscina_generare_dynamicum("probatio_regio", 1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ========================================================
     * PROBARE: vita + basis fixa + fines
     * ======================================================== */
    {
        Regio* regio;
        i8* basis;

        imprimere("\n--- Probans vitam + basim fixam ---\n");
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

    /* ========================================================
     * PROBARE: globalia - cumulus cum ordinatione
     * ======================================================== */
    {
        Regio* regio = regio_generare(piscina);
        i8* a;
        i8* b;
        i8* c;

        imprimere("\n--- Probans globalia ---\n");
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
        c = (i8*)regio_globalia_allocare(regio, I, XVI);
        CREDO_NON_NIHIL (c);
        CREDO_VERUM ((memoriae_index)(vacuum*)c % XVI == ZEPHYRUM);

        /* usus crescit */
        CREDO_VERUM (regio_globalia_usus(regio) >= XVI + I);

        /* scribere/legere per aream */
        memset(a, 0x5A, III);
        CREDO_AEQUALIS_I32 ((i32)a[ZEPHYRUM], 0x5A);

        regio_destruere(regio);
    }

    /* ========================================================
     * PROBARE: stiva - fines intra regionem
     * ======================================================== */
    {
        Regio* regio = regio_generare(piscina);
        i8* stiva;

        imprimere("\n--- Probans stivam ---\n");
        stiva = (i8*)regio_stiva_initium(regio);
        CREDO_NON_NIHIL (stiva);
        CREDO_VERUM (regio_continet(regio, stiva));
        CREDO_VERUM (regio_continet(regio,
            stiva + regio_stiva_magnitudo_octetorum(regio) - I));
        CREDO_VERUM (regio_stiva_magnitudo_octetorum(regio)
            >= 1073741824UL);

        regio_destruere(regio);
    }

    /* ========================================================
     * PROBARE: acervus - ordinatio + reusus LIFO + scissio
     * ======================================================== */
    {
        Regio* regio = regio_generare(piscina);
        i8* a;
        i8* b;
        i8* c;
        i8* iterum;

        imprimere("\n--- Probans acervum ---\n");

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
        c = (i8*)regio_allocare(regio, 1000);
        CREDO_NON_NIHIL (c);
        CREDO_VERUM (regio_liberare(regio, c));
        iterum = (i8*)regio_allocare(regio, 100);
        CREDO_AEQUALIS_PTR ((vacuum*)iterum, (vacuum*)c);
        /* residuum scissum in indice libero: allocatio parva sequens
         * intra locellum veterem cadit */
        iterum = (i8*)regio_allocare(regio, 100);
        CREDO_VERUM (iterum > c && iterum < c + 1000);

        regio_destruere(regio);
    }

    /* ========================================================
     * PROBARE: liberare - validitas
     * ======================================================== */
    {
        Regio* regio = regio_generare(piscina);
        i8* a;
        i8 alienus[64];

        imprimere("\n--- Probans validitatem liberationis ---\n");

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

    /* ========================================================
     * PROBARE: custodia - venenum
     * ======================================================== */
    {
        Regio* regio = regio_generare(piscina);
        i8* a;

        imprimere("\n--- Probans custodiam (venenum) ---\n");
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

    /* ========================================================
     * PROBARE: reallocare
     * ======================================================== */
    {
        Regio* regio = regio_generare(piscina);
        i8* a;
        i8* b;

        imprimere("\n--- Probans reallocare ---\n");

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

    /* ========================================================
     * PROBARE: census
     * ======================================================== */
    {
        Regio* regio = regio_generare(piscina);
        i8* a;
        i8* b;

        imprimere("\n--- Probans censum ---\n");

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

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
