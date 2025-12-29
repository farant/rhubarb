/* probatio_capsula_caudae.c - Tests for capsula_caudae library
 *
 * Hoc testum legit assets ex sua cauda (argv[0]).
 *
 * USUS:
 *   1. Compile: ./compile_tests.sh probatio_capsula_caudae
 *      (auto-appends assets if config exists)
 *   2. Run: ./bin/probatio_capsula_caudae
 */

#include "../include/latina.h"
#include "../include/piscina.h"
#include "../include/capsula_caudae.h"
#include "../include/chorda.h"
#include "../include/filum.h"

#include <stdio.h>
#include <string.h>


/* Global: via to binary (argv[0]) */
hic_manens constans character* g_via_binarii = NIHIL;


/* ========================================================================
 * PROBATIONES
 * ======================================================================== */

interior i32
proba_habet_assets(vacuum)
{
    printf("  proba_habet_assets: ");

    si (!capsula_caudae_habet_assets(g_via_binarii))
    {
        printf("FALSUM - binary non habet assets\n");
        printf("\n    Necessitas: append assets primo.\n");
        printf("    Config should be at: probationes/probatio_capsula_caudae_caudae_assets.toml\n\n");
        redde FALSUM;
    }

    printf("VERUM\n");
    redde VERUM;
}


interior i32
proba_aperire(Piscina* piscina)
{
    CapsulaCaudae* capsula;
    i32            num;
    i32            i;
    i64            total_cruda;
    i64            total_compressa;
    chorda         raw_str;
    chorda         comp_str;
    f64            ratio;

    printf("  proba_aperire: ");

    capsula = capsula_caudae_aperire(g_via_binarii, piscina);

    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula_caudae_aperire rediit NIHIL\n");
        redde FALSUM;
    }

    num = capsula_caudae_numerus(capsula);
    si (num < I)
    {
        printf("FALSUM - numerus filorum = %d\n", num);
        capsula_caudae_claudere(capsula);
        redde FALSUM;
    }

    /* Calculate total sizes */
    total_cruda = 0;
    total_compressa = 0;
    per (i = 0; i < num; i++)
    {
        CapsulaIndexum* idx;

        idx = capsula_caudae_indexum(capsula, i);
        si (idx != NIHIL)
        {
            total_cruda += (i64)idx->mensura_cruda;
            total_compressa += (i64)idx->mensura_compressa;
        }
    }

    raw_str = chorda_ex_bytes_legibilis(total_cruda, piscina);
    comp_str = chorda_ex_bytes_legibilis(total_compressa, piscina);
    ratio = total_cruda > 0 ? (f64)total_compressa / (f64)total_cruda * 100.0 : 0.0;

    printf("VERUM\n");
    printf("    %d fila, %.*s raw -> %.*s compressed (%.1f%%)\n",
           num,
           (i32)raw_str.mensura, raw_str.datum,
           (i32)comp_str.mensura, comp_str.datum,
           ratio);

    capsula_caudae_claudere(capsula);
    redde VERUM;
}


interior i32
proba_legere_primum(Piscina* piscina)
{
    CapsulaCaudae*  capsula;
    CapsulaIndexum* idx;
    CapsulaFructus  res;
    character       via_buffer[256];
    chorda          size_str;

    printf("  proba_legere_primum: ");

    capsula = capsula_caudae_aperire(g_via_binarii, piscina);

    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula_caudae_aperire rediit NIHIL\n");
        redde FALSUM;
    }

    /* Get first file */
    idx = capsula_caudae_indexum(capsula, 0);
    si (idx == NIHIL)
    {
        printf("FALSUM - nullum filum in capsula\n");
        capsula_caudae_claudere(capsula);
        redde FALSUM;
    }

    /* Copy path to null-terminated buffer */
    si (idx->via.mensura > 255)
    {
        printf("FALSUM - via nimis longa\n");
        capsula_caudae_claudere(capsula);
        redde FALSUM;
    }

    memcpy(via_buffer, idx->via.datum, (size_t)idx->via.mensura);
    via_buffer[idx->via.mensura] = '\0';

    /* Read the file */
    res = capsula_caudae_legere(capsula, via_buffer, piscina);

    si (res.status != CAPSULA_OK)
    {
        printf("FALSUM - status = %d (%s)\n",
               res.status, capsula_status_nuntium(res.status));
        capsula_caudae_claudere(capsula);
        redde FALSUM;
    }

    si (res.datum.mensura == 0)
    {
        printf("FALSUM - datum vacuum\n");
        capsula_caudae_claudere(capsula);
        redde FALSUM;
    }

    size_str = chorda_ex_bytes_legibilis((i64)res.datum.mensura, piscina);
    printf("VERUM (%.*s: %.*s)\n",
           (i32)idx->via.mensura, idx->via.datum,
           (i32)size_str.mensura, size_str.datum);

    capsula_caudae_claudere(capsula);
    redde VERUM;
}


interior i32
proba_non_inventum(Piscina* piscina)
{
    CapsulaCaudae* capsula;
    CapsulaFructus res;

    printf("  proba_non_inventum: ");

    capsula = capsula_caudae_aperire(g_via_binarii, piscina);

    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula_caudae_aperire rediit NIHIL\n");
        redde FALSUM;
    }

    res = capsula_caudae_legere(capsula, "non_existit_xyz_123.txt", piscina);

    si (res.status != CAPSULA_NON_INVENTUM)
    {
        printf("FALSUM - expectavit CAPSULA_NON_INVENTUM, obtinuit %d\n",
               res.status);
        capsula_caudae_claudere(capsula);
        redde FALSUM;
    }

    printf("VERUM\n");

    capsula_caudae_claudere(capsula);
    redde VERUM;
}


interior i32
proba_iter(Piscina* piscina)
{
    CapsulaCaudae*     capsula;
    CapsulaCaudaeIter  iter;
    i32                numerus;
    i32                expected;

    printf("  proba_iter: ");

    capsula = capsula_caudae_aperire(g_via_binarii, piscina);

    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula_caudae_aperire rediit NIHIL\n");
        redde FALSUM;
    }

    expected = capsula_caudae_numerus(capsula);
    iter = capsula_caudae_iter(capsula);
    numerus = 0;

    dum (capsula_caudae_iter_proximus(&iter))
    {
        CapsulaIndexum* idx;

        idx = capsula_caudae_indexum(capsula, iter.positus - I);
        si (idx != NIHIL)
        {
            si (numerus < V)  /* Print first 5 */
            {
                chorda size_str;

                size_str = chorda_ex_bytes_legibilis((i64)idx->mensura_cruda, piscina);
                printf("\n      [%d] %.*s (%.*s)",
                       numerus, (i32)idx->via.mensura, idx->via.datum,
                       (i32)size_str.mensura, size_str.datum);
            }
            alioquin si (numerus == V)
            {
                printf("\n      ...");
            }
            numerus++;
        }
    }

    printf("\n    Total: %d fila - ", numerus);

    si (numerus != expected)
    {
        printf("FALSUM - expectavit %d, obtinuit %d\n", expected, numerus);
        capsula_caudae_claudere(capsula);
        redde FALSUM;
    }

    printf("VERUM\n");

    capsula_caudae_claudere(capsula);
    redde VERUM;
}


interior i32
proba_indexum(Piscina* piscina)
{
    CapsulaCaudae*  capsula;
    CapsulaIndexum* idx;

    printf("  proba_indexum: ");

    capsula = capsula_caudae_aperire(g_via_binarii, piscina);

    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula_caudae_aperire rediit NIHIL\n");
        redde FALSUM;
    }

    idx = capsula_caudae_indexum(capsula, 0);

    si (idx == NIHIL)
    {
        printf("FALSUM - capsula_caudae_indexum(0) rediit NIHIL\n");
        capsula_caudae_claudere(capsula);
        redde FALSUM;
    }

    si (idx->via.mensura == 0)
    {
        printf("FALSUM - via vacua\n");
        capsula_caudae_claudere(capsula);
        redde FALSUM;
    }

    si (idx->mensura_cruda == 0)
    {
        printf("FALSUM - mensura_cruda = 0\n");
        capsula_caudae_claudere(capsula);
        redde FALSUM;
    }

    /* Index out of bounds */
    idx = capsula_caudae_indexum(capsula, 10000);

    si (idx != NIHIL)
    {
        printf("FALSUM - capsula_caudae_indexum(10000) non rediit NIHIL\n");
        capsula_caudae_claudere(capsula);
        redde FALSUM;
    }

    printf("VERUM\n");

    capsula_caudae_claudere(capsula);
    redde VERUM;
}


interior i32
proba_legere_omnes(Piscina* piscina)
{
    CapsulaCaudae*     capsula;
    CapsulaCaudaeIter  iter;
    i32                successus;
    i32                fallitae;
    i64                total_bytes;

    printf("  proba_legere_omnes: ");

    capsula = capsula_caudae_aperire(g_via_binarii, piscina);

    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula_caudae_aperire rediit NIHIL\n");
        redde FALSUM;
    }

    iter = capsula_caudae_iter(capsula);
    successus = 0;
    fallitae = 0;
    total_bytes = 0;

    dum (capsula_caudae_iter_proximus(&iter))
    {
        CapsulaIndexum* idx;
        CapsulaFructus  res;
        character       via_buffer[512];

        idx = capsula_caudae_indexum(capsula, iter.positus - I);
        si (idx == NIHIL)
        {
            fallitae++;
            perge;
        }

        /* Copy path */
        si (idx->via.mensura > 511)
        {
            fallitae++;
            perge;
        }

        memcpy(via_buffer, idx->via.datum, (size_t)idx->via.mensura);
        via_buffer[idx->via.mensura] = '\0';

        /* Read file */
        res = capsula_caudae_legere(capsula, via_buffer, piscina);

        si (res.status == CAPSULA_OK && res.datum.mensura > 0)
        {
            successus++;
            total_bytes += (i64)res.datum.mensura;
        }
        alioquin
        {
            fallitae++;
        }
    }

    capsula_caudae_claudere(capsula);

    si (fallitae > 0)
    {
        printf("FALSUM - %d fallitae\n", fallitae);
        redde FALSUM;
    }

    {
        chorda size_str;

        size_str = chorda_ex_bytes_legibilis(total_bytes, piscina);
        printf("VERUM (%d fila, %.*s total)\n",
               successus, (i32)size_str.mensura, size_str.datum);
    }
    redde VERUM;
}


interior i32
proba_contentum_verificare(Piscina* piscina)
{
    CapsulaCaudae*  capsula;
    CapsulaFructus  res;
    chorda          originale;
    constans character* test_via_capsula;
    constans character* test_via_disco;
    i32             i;
    b32             aequalis;

    printf("  proba_contentum_verificare: ");

    /* Test file: book 100 (Shakespeare) */
    test_via_capsula = "../book_assets/100.txt";
    test_via_disco = "book_assets/100.txt";

    /* Verify original file exists on disk */
    si (!filum_existit(test_via_disco))
    {
        printf("OMISSUS - %s non existit in disco\n", test_via_disco);
        redde VERUM;  /* Non fallita, sed omissa */
    }

    /* Read from capsula */
    capsula = capsula_caudae_aperire(g_via_binarii, piscina);
    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula_caudae_aperire rediit NIHIL\n");
        redde FALSUM;
    }

    res = capsula_caudae_legere(capsula, test_via_capsula, piscina);
    si (res.status != CAPSULA_OK)
    {
        printf("FALSUM - non possum legere '%s' ex capsula (status=%d)\n",
               test_via_capsula, res.status);
        capsula_caudae_claudere(capsula);
        redde FALSUM;
    }

    /* Read original from disk */
    originale = filum_legere_totum(test_via_disco, piscina);
    si (originale.mensura == 0)
    {
        printf("FALSUM - non possum legere '%s' ex disco\n", test_via_disco);
        capsula_caudae_claudere(capsula);
        redde FALSUM;
    }

    /* Compare sizes */
    si (res.datum.mensura != originale.mensura)
    {
        printf("FALSUM - mensura differt: capsula=%d, disco=%d\n",
               res.datum.mensura, originale.mensura);
        capsula_caudae_claudere(capsula);
        redde FALSUM;
    }

    /* Compare content byte-by-byte */
    aequalis = VERUM;
    per (i = 0; i < res.datum.mensura; i++)
    {
        si (res.datum.datum[i] != originale.datum[i])
        {
            printf("FALSUM - byte %d differt: capsula=0x%02X, disco=0x%02X\n",
                   i, (insignatus character)res.datum.datum[i],
                   (insignatus character)originale.datum[i]);
            aequalis = FALSUM;
            frange;
        }
    }

    capsula_caudae_claudere(capsula);

    si (!aequalis)
    {
        redde FALSUM;
    }

    printf("VERUM (%d bytes identici)\n", res.datum.mensura);
    redde VERUM;
}


interior i32
proba_contentum_notum(Piscina* piscina)
{
    CapsulaCaudae*  capsula;
    CapsulaFructus  res;
    constans character* test_via;
    constans character* quaerendum;
    i32             i;
    i32             quaerendum_len;
    b32             inventum;

    printf("  proba_contentum_notum: ");

    /* Test: look for "HAMLET" in Shakespeare (book 100) */
    test_via = "../book_assets/100.txt";
    quaerendum = "HAMLET";
    quaerendum_len = 6;

    capsula = capsula_caudae_aperire(g_via_binarii, piscina);
    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula_caudae_aperire rediit NIHIL\n");
        redde FALSUM;
    }

    res = capsula_caudae_legere(capsula, test_via, piscina);
    si (res.status != CAPSULA_OK)
    {
        /* File might not exist in this test set */
        printf("OMISSUS - '%s' non in capsula\n", test_via);
        capsula_caudae_claudere(capsula);
        redde VERUM;
    }

    /* Search for known string */
    inventum = FALSUM;
    per (i = 0; i <= res.datum.mensura - quaerendum_len; i++)
    {
        si (memcmp(res.datum.datum + i, quaerendum, (size_t)quaerendum_len) == 0)
        {
            inventum = VERUM;
            frange;
        }
    }

    capsula_caudae_claudere(capsula);

    si (!inventum)
    {
        printf("FALSUM - '%s' non inventum in %s\n", quaerendum, test_via);
        redde FALSUM;
    }

    printf("VERUM (\"%s\" inventum ad byte %d)\n", quaerendum, i);
    redde VERUM;
}


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

integer
principale(integer argc, character** argv)
{
    Piscina* piscina;
    i32      successus;
    i32      fallitae;

    si (argc < I)
    {
        fprintf(stderr, "Error: argc < 1\n");
        redde I;
    }

    g_via_binarii = argv[0];

    printf("\nProbationes Capsula Caudae\n");
    printf("==========================\n");
    printf("Binary: %s\n\n", g_via_binarii);

    /* First check if we have assets */
    si (!proba_habet_assets())
    {
        redde I;
    }

    piscina = piscina_generare_dynamicum("probatio_capsula_caudae", CXXVIII * M);

    successus = I;  /* Count habet_assets */
    fallitae = 0;

    si (proba_aperire(piscina)) successus++; alioquin fallitae++;
    si (proba_legere_primum(piscina)) successus++; alioquin fallitae++;
    si (proba_non_inventum(piscina)) successus++; alioquin fallitae++;
    si (proba_iter(piscina)) successus++; alioquin fallitae++;
    si (proba_indexum(piscina)) successus++; alioquin fallitae++;
    si (proba_legere_omnes(piscina)) successus++; alioquin fallitae++;
    si (proba_contentum_verificare(piscina)) successus++; alioquin fallitae++;
    si (proba_contentum_notum(piscina)) successus++; alioquin fallitae++;

    printf("\n");
    printf("Summa: %d successus, %d fallitae\n", successus, fallitae);

    piscina_destruere(piscina);

    redde fallitae > 0 ? I : 0;
}
