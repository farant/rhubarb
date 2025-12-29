/* probatio_libri.c - Tests for embedded book library */

#include "../include/latina.h"
#include "../include/piscina.h"
#include "../include/capsula.h"
#include "../include/stml.h"
#include "../include/internamentum.h"
#include "../book_assets/capsula_libri.h"

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

    capsula = capsula_aperire(&capsula_libri, piscina);

    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula_aperire rediit NIHIL\n");
        redde FALSUM;
    }

    /* Expectamus 11 fila (10 libri + librarium.stml) */
    si (capsula_numerus(capsula) != XI)
    {
        printf("FALSUM - expectavit 11 fila, obtinuit %d\n",
               capsula_numerus(capsula));
        redde FALSUM;
    }

    printf("VERUM\n");
    redde VERUM;
}


interior i32
proba_librarium_stml(Piscina* piscina)
{
    Capsula*       capsula;
    CapsulaFructus res;

    printf("  proba_librarium_stml: ");

    capsula = capsula_aperire(&capsula_libri, piscina);

    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula_aperire rediit NIHIL\n");
        redde FALSUM;
    }

    res = capsula_legere(capsula, "librarium.stml", piscina);

    si (res.status != CAPSULA_OK)
    {
        printf("FALSUM - capsula_legere status = %d\n", res.status);
        redde FALSUM;
    }

    si (res.datum.mensura < C)
    {
        printf("FALSUM - librarium.stml nimis parvum: %d bytes\n",
               (i32)res.datum.mensura);
        redde FALSUM;
    }

    /* Verify it starts with <librarium> */
    si (memcmp(res.datum.datum, "<librarium>", XI) != 0)
    {
        printf("FALSUM - non incipit cum <librarium>\n");
        redde FALSUM;
    }

    printf("VERUM (%d bytes)\n", (i32)res.datum.mensura);
    redde VERUM;
}


interior i32
proba_legere_librum(Piscina* piscina)
{
    Capsula*       capsula;
    CapsulaFructus res;

    printf("  proba_legere_librum: ");

    capsula = capsula_aperire(&capsula_libri, piscina);

    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula_aperire rediit NIHIL\n");
        redde FALSUM;
    }

    /* Try reading a book by Gutenberg ID */
    res = capsula_legere(capsula, "14286.txt", piscina);

    si (res.status != CAPSULA_OK)
    {
        printf("FALSUM - capsula_legere status = %d\n", res.status);
        redde FALSUM;
    }

    si (res.datum.mensura < M)
    {
        printf("FALSUM - liber nimis parvum: %d bytes\n",
               (i32)res.datum.mensura);
        redde FALSUM;
    }

    printf("VERUM (%d bytes)\n", (i32)res.datum.mensura);
    redde VERUM;
}


interior i32
proba_parse_embedded_stml(Piscina* piscina)
{
    Capsula*             capsula;
    CapsulaFructus       res;
    InternamentumChorda* intern;
    StmlResultus         stml_res;
    Xar*                 libri;

    printf("  proba_parse_embedded_stml: ");

    capsula = capsula_aperire(&capsula_libri, piscina);

    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula_aperire rediit NIHIL\n");
        redde FALSUM;
    }

    res = capsula_legere(capsula, "librarium.stml", piscina);

    si (res.status != CAPSULA_OK)
    {
        printf("FALSUM - capsula_legere status = %d\n", res.status);
        redde FALSUM;
    }

    /* Parse the STML */
    intern = internamentum_creare(piscina);
    stml_res = stml_legere(res.datum, piscina, intern);

    si (!stml_res.successus)
    {
        printf("FALSUM - stml_legere fallita\n");
        redde FALSUM;
    }

    /* Find all liber elements */
    libri = stml_invenire_omnes_liberos(stml_res.elementum_radix, "liber", piscina);

    si (xar_numerus(libri) != X)
    {
        printf("FALSUM - expectavit 10 libri, obtinuit %d\n",
               xar_numerus(libri));
        redde FALSUM;
    }

    printf("VERUM (%d libri)\n", xar_numerus(libri));
    redde VERUM;
}


interior i32
proba_via_paths_updated(Piscina* piscina)
{
    Capsula*             capsula;
    CapsulaFructus       res;
    InternamentumChorda* intern;
    StmlResultus         stml_res;
    Xar*                 libri;
    StmlNodus*           nodus_liber;
    StmlNodus*           nodus_via;
    chorda               via;

    printf("  proba_via_paths_updated: ");

    capsula = capsula_aperire(&capsula_libri, piscina);
    res = capsula_legere(capsula, "librarium.stml", piscina);

    si (res.status != CAPSULA_OK)
    {
        printf("FALSUM - capsula_legere status = %d\n", res.status);
        redde FALSUM;
    }

    intern = internamentum_creare(piscina);
    stml_res = stml_legere(res.datum, piscina, intern);

    si (!stml_res.successus)
    {
        printf("FALSUM - stml_legere fallita\n");
        redde FALSUM;
    }

    libri = stml_invenire_omnes_liberos(stml_res.elementum_radix, "liber", piscina);

    si (xar_numerus(libri) == 0)
    {
        printf("FALSUM - nulla libri\n");
        redde FALSUM;
    }

    /* Check first book's via tag */
    nodus_liber = *(StmlNodus**)xar_obtinere(libri, 0);
    nodus_via = stml_invenire_liberum(nodus_liber, "via");

    si (nodus_via == NIHIL)
    {
        printf("FALSUM - via non inventa\n");
        redde FALSUM;
    }

    via = stml_textus_internus(nodus_via, piscina);

    /* Via should be "{numerus}.txt" format, not "../gutenberg-mirror/..." */
    si (via.mensura < IV || via.datum[via.mensura - I] != 't' ||
        via.datum[via.mensura - II] != 'x' || via.datum[via.mensura - III] != 't')
    {
        printf("FALSUM - via non finit cum '.txt': %.*s\n",
               (i32)via.mensura, via.datum);
        redde FALSUM;
    }

    /* Should NOT contain ".." */
    {
        i32 j;
        per (j = 0; j < (i32)via.mensura - I; j++)
        {
            si (via.datum[j] == '.' && via.datum[j + I] == '.')
            {
                printf("FALSUM - via continet '..': %.*s\n",
                       (i32)via.mensura, via.datum);
                redde FALSUM;
            }
        }
    }

    printf("VERUM (via = %.*s)\n", (i32)via.mensura, via.datum);
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

    piscina = piscina_generare_dynamicum("probatio_libri", VIII * M);

    printf("\nProbationes Libri (Embedded Books)\n");
    printf("===================================\n\n");

    successus = 0;
    fallitae = 0;

    si (proba_aperire(piscina)) successus++; alioquin fallitae++;
    si (proba_librarium_stml(piscina)) successus++; alioquin fallitae++;
    si (proba_legere_librum(piscina)) successus++; alioquin fallitae++;
    si (proba_parse_embedded_stml(piscina)) successus++; alioquin fallitae++;
    si (proba_via_paths_updated(piscina)) successus++; alioquin fallitae++;

    printf("\n");
    printf("Summa: %d successus, %d fallitae\n", successus, fallitae);

    piscina_destruere(piscina);

    redde fallitae > 0 ? I : 0;
}
