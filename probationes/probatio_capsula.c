/* probatio_capsula.c - Tests for capsula library */

#include "../include/latina.h"
#include "../include/piscina.h"
#include "../include/capsula.h"
#include "capsula_assets.h"

#include <stdio.h>
#include <string.h>


/* ========================================================================
 * PROBATIONES
 * ======================================================================== */

interior i32
proba_aperire(Piscina* piscina)
{
    Capsula* capsula;

    printf("  proba_aperire: ");

    capsula = capsula_aperire(&capsula_assets, piscina);

    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula_aperire rediit NIHIL\n");
        redde FALSUM;
    }

    si (capsula_numerus(capsula) != II)
    {
        printf("FALSUM - expectavit 2 fila, obtinuit %d\n",
               capsula_numerus(capsula));
        redde FALSUM;
    }

    printf("VERUM\n");
    redde VERUM;
}


interior i32
proba_habet(Piscina* piscina)
{
    Capsula* capsula;

    printf("  proba_habet: ");

    capsula = capsula_aperire(&capsula_assets, piscina);

    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula_aperire rediit NIHIL\n");
        redde FALSUM;
    }

    si (!capsula_habet(capsula, "test_assets/hello.txt"))
    {
        printf("FALSUM - non invenit 'test_assets/hello.txt'\n");
        redde FALSUM;
    }

    si (!capsula_habet(capsula, "test_assets/lorem.txt"))
    {
        printf("FALSUM - non invenit 'test_assets/lorem.txt'\n");
        redde FALSUM;
    }

    si (capsula_habet(capsula, "non_existit.txt"))
    {
        printf("FALSUM - invenit 'non_existit.txt' quod non existit\n");
        redde FALSUM;
    }

    printf("VERUM\n");
    redde VERUM;
}


interior i32
proba_legere_hello(Piscina* piscina)
{
    Capsula*       capsula;
    CapsulaFructus res;
    constans character* expected = "Hello, World!\n"
                                   "This is a test file for capsula embedding.\n";

    printf("  proba_legere_hello: ");

    capsula = capsula_aperire(&capsula_assets, piscina);

    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula_aperire rediit NIHIL\n");
        redde FALSUM;
    }

    res = capsula_legere(capsula, "test_assets/hello.txt", piscina);

    si (res.status != CAPSULA_OK)
    {
        printf("FALSUM - capsula_legere status = %d\n", res.status);
        redde FALSUM;
    }

    si (res.datum.mensura != (i32)strlen(expected))
    {
        printf("FALSUM - expectavit %d bytes, obtinuit %d\n",
               (i32)strlen(expected), (i32)res.datum.mensura);
        redde FALSUM;
    }

    si (memcmp(res.datum.datum, expected, res.datum.mensura) != 0)
    {
        printf("FALSUM - contentum non congruit\n");
        printf("    Expectatum: '%s'\n", expected);
        printf("    Obtentum: '%.*s'\n", (i32)res.datum.mensura, res.datum.datum);
        redde FALSUM;
    }

    printf("VERUM\n");
    redde VERUM;
}


interior i32
proba_legere_lorem(Piscina* piscina)
{
    Capsula*       capsula;
    CapsulaFructus res;
    constans character* expected =
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit.\n"
        "Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n"
        "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris.\n";

    printf("  proba_legere_lorem: ");

    capsula = capsula_aperire(&capsula_assets, piscina);

    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula_aperire rediit NIHIL\n");
        redde FALSUM;
    }

    res = capsula_legere(capsula, "test_assets/lorem.txt", piscina);

    si (res.status != CAPSULA_OK)
    {
        printf("FALSUM - capsula_legere status = %d\n", res.status);
        redde FALSUM;
    }

    si (res.datum.mensura != (i32)strlen(expected))
    {
        printf("FALSUM - expectavit %d bytes, obtinuit %d\n",
               (i32)strlen(expected), (i32)res.datum.mensura);
        redde FALSUM;
    }

    si (memcmp(res.datum.datum, expected, res.datum.mensura) != 0)
    {
        printf("FALSUM - contentum non congruit\n");
        redde FALSUM;
    }

    printf("VERUM\n");
    redde VERUM;
}


interior i32
proba_non_inventum(Piscina* piscina)
{
    Capsula*       capsula;
    CapsulaFructus res;

    printf("  proba_non_inventum: ");

    capsula = capsula_aperire(&capsula_assets, piscina);

    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula_aperire rediit NIHIL\n");
        redde FALSUM;
    }

    res = capsula_legere(capsula, "non_existit.txt", piscina);

    si (res.status != CAPSULA_NON_INVENTUM)
    {
        printf("FALSUM - expectavit CAPSULA_NON_INVENTUM, obtinuit %d\n",
               res.status);
        redde FALSUM;
    }

    printf("VERUM\n");
    redde VERUM;
}


interior i32
proba_iter(Piscina* piscina)
{
    Capsula*     capsula;
    CapsulaIter  iter;
    i32          numerus;

    printf("  proba_iter: ");

    capsula = capsula_aperire(&capsula_assets, piscina);

    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula_aperire rediit NIHIL\n");
        redde FALSUM;
    }

    iter = capsula_iter(capsula);
    numerus = 0;

    dum (capsula_iter_proximus(&iter))
    {
        CapsulaIndexum* idx;

        idx = capsula_indexum(capsula, iter.positus - I);
        si (idx != NIHIL)
        {
            numerus++;
        }
    }

    si (numerus != II)
    {
        printf("FALSUM - expectavit 2 iterationes, obtinuit %d\n", numerus);
        redde FALSUM;
    }

    printf("VERUM\n");
    redde VERUM;
}


interior i32
proba_indexum(Piscina* piscina)
{
    Capsula*        capsula;
    CapsulaIndexum* idx;

    printf("  proba_indexum: ");

    capsula = capsula_aperire(&capsula_assets, piscina);

    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula_aperire rediit NIHIL\n");
        redde FALSUM;
    }

    idx = capsula_indexum(capsula, 0);

    si (idx == NIHIL)
    {
        printf("FALSUM - capsula_indexum(0) rediit NIHIL\n");
        redde FALSUM;
    }

    si (idx->via.mensura == 0)
    {
        printf("FALSUM - via vacua\n");
        redde FALSUM;
    }

    si (idx->mensura_cruda == 0)
    {
        printf("FALSUM - mensura_cruda = 0\n");
        redde FALSUM;
    }

    /* Index extra fines */
    idx = capsula_indexum(capsula, 100);

    si (idx != NIHIL)
    {
        printf("FALSUM - capsula_indexum(100) non rediit NIHIL\n");
        redde FALSUM;
    }

    printf("VERUM\n");
    redde VERUM;
}


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

integer
principale(vacuum)
{
    Piscina* piscina;
    i32      successus;
    i32      fallitae;

    piscina = piscina_generare_dynamicum("probatio_capsula", IV * M);

    printf("\nProbationes Capsula\n");
    printf("===================\n\n");

    successus = 0;
    fallitae = 0;

    si (proba_aperire(piscina)) successus++; alioquin fallitae++;
    si (proba_habet(piscina)) successus++; alioquin fallitae++;
    si (proba_legere_hello(piscina)) successus++; alioquin fallitae++;
    si (proba_legere_lorem(piscina)) successus++; alioquin fallitae++;
    si (proba_non_inventum(piscina)) successus++; alioquin fallitae++;
    si (proba_iter(piscina)) successus++; alioquin fallitae++;
    si (proba_indexum(piscina)) successus++; alioquin fallitae++;

    printf("\n");
    printf("Summa: %d successus, %d fallitae\n", successus, fallitae);

    piscina_destruere(piscina);

    redde fallitae > 0 ? I : 0;
}
