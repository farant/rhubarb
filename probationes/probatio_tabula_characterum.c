/* probatio_tabula_characterum.c - Probationes TabulaCharacterum */
#include "latina.h"
#include "tabula_characterum.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>

/* ==================================================
 * Test: Initiatio
 * ================================================== */

hic_manens vacuum
probans_initiare(vacuum)
{
    TabulaCharacterum tabula;

    tabula_initiare(&tabula);

    CREDO_VERUM(tabula_est_vacua(&tabula));
    CREDO_FALSUM(tabula_est_plena(&tabula));
}


/* ==================================================
 * Test: Query Functions
 * ================================================== */

hic_manens vacuum
probans_invenire_finem_contenti(vacuum)
{
    TabulaCharacterum tabula;

    /* Linea vacua */
    tabula_initiare(&tabula);
    CREDO_AEQUALIS_S32(tabula_invenire_finem_contenti(&tabula, ZEPHYRUM), -I);

    /* Linea cum uno charactere */
    tabula_ex_literis(&tabula, "a");
    CREDO_AEQUALIS_S32(tabula_invenire_finem_contenti(&tabula, ZEPHYRUM), ZEPHYRUM);

    /* Linea cum characteribus */
    tabula_ex_literis(&tabula, "hello");
    CREDO_AEQUALIS_S32(tabula_invenire_finem_contenti(&tabula, ZEPHYRUM), IV);

    /* Linea cum spatiis post contentum - spatia non sunt contentum */
    tabula_ex_literis(&tabula, "hi   ");
    CREDO_AEQUALIS_S32(tabula_invenire_finem_contenti(&tabula, ZEPHYRUM), I);
}

hic_manens vacuum
probans_invenire_initium_contenti(vacuum)
{
    TabulaCharacterum tabula;

    /* Linea vacua */
    tabula_initiare(&tabula);
    CREDO_AEQUALIS_I32(tabula_invenire_initium_contenti(&tabula, ZEPHYRUM), TABULA_LATITUDO);

    /* Linea sine indentation */
    tabula_ex_literis(&tabula, "hello");
    CREDO_AEQUALIS_I32(tabula_invenire_initium_contenti(&tabula, ZEPHYRUM), ZEPHYRUM);

    /* Linea cum indentation */
    tabula_ex_literis(&tabula, "    hello");
    CREDO_AEQUALIS_I32(tabula_invenire_initium_contenti(&tabula, ZEPHYRUM), IV);
}

hic_manens vacuum
probans_invenire_obicem(vacuum)
{
    TabulaCharacterum tabula;

    /* Linea vacua - nulla obex */
    tabula_initiare(&tabula);
    CREDO_AEQUALIS_S32(tabula_invenire_obicem(&tabula, ZEPHYRUM, ZEPHYRUM), -I);

    /* Contentum ad initium */
    tabula_ex_literis(&tabula, "hello");
    CREDO_AEQUALIS_S32(tabula_invenire_obicem(&tabula, ZEPHYRUM, ZEPHYRUM), ZEPHYRUM);

    /* Spatia ante contentum */
    tabula_ex_literis(&tabula, "ab   hello");
    CREDO_AEQUALIS_S32(tabula_invenire_obicem(&tabula, ZEPHYRUM, II), V);

    /* Nulla contentum post columnam */
    tabula_ex_literis(&tabula, "hi");
    CREDO_AEQUALIS_S32(tabula_invenire_obicem(&tabula, ZEPHYRUM, V), -I);
}


/* ==================================================
 * Test: Mutation Functions
 * ================================================== */

hic_manens vacuum
probans_trudere_dextram(vacuum)
{
    TabulaCharacterum tabula;
    character overflow;

    /* Trudere in linea cum spatio */
    tabula_ex_literis(&tabula, "hello");
    overflow = tabula_trudere_dextram(&tabula, ZEPHYRUM, II);
    CREDO_AEQUALIS_I32((i32)overflow, (i32)' ');  /* Grid impleta cum spatiis */
    tabula_asserere(&tabula, "he llo", "trudere dextram in hello");

    /* Trudere cum overflow */
    tabula_ex_literis(&tabula, "");
    /* Implere lineam */
    {
        i32 i;
        per (i = ZEPHYRUM; i < TABULA_LATITUDO; i++)
        {
            tabula.cellulae[ZEPHYRUM][i] = 'x';
        }
    }
    overflow = tabula_trudere_dextram(&tabula, ZEPHYRUM, ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)overflow, (i32)'x');
}

hic_manens vacuum
probans_inserere_lineam(vacuum)
{
    TabulaCharacterum tabula;
    b32 successus;

    tabula_ex_literis(&tabula, "line0\nline1\nline2");

    successus = tabula_inserere_lineam(&tabula, I);
    CREDO_VERUM(successus);
    tabula_asserere(&tabula, "line0\n\nline1\nline2", "inserere lineam ad 1");
}

hic_manens vacuum
probans_delere_lineam(vacuum)
{
    TabulaCharacterum tabula;

    tabula_ex_literis(&tabula, "line0\nline1\nline2");

    tabula_delere_lineam(&tabula, I);
    tabula_asserere(&tabula, "line0\nline2", "delere lineam 1");
}


/* ==================================================
 * Test: Main Operations
 * ================================================== */

hic_manens vacuum
probans_inserere_characterem_simplex(vacuum)
{
    TabulaCharacterum tabula;

    /* Inserere in tabula vacua */
    tabula_initiare(&tabula);
    tabula_inserere_characterem(&tabula, ZEPHYRUM, ZEPHYRUM, 'H');
    tabula_asserere(&tabula, "H", "inserere H in vacua");

    tabula_inserere_characterem(&tabula, ZEPHYRUM, I, 'i');
    tabula_asserere(&tabula, "Hi", "inserere i post H");
}

hic_manens vacuum
probans_inserere_characterem_cum_push(vacuum)
{
    TabulaCharacterum tabula;

    /* Inserere ante contentum - debet trudere */
    tabula_ex_literis(&tabula, "   hello");
    tabula_inserere_characterem(&tabula, ZEPHYRUM, ZEPHYRUM, 'X');
    tabula_asserere(&tabula, "X   hello", "inserere X ante spatia et hello");

    /* Inserere in medio spatii */
    tabula_ex_literis(&tabula, "ab   hello");
    tabula_inserere_characterem(&tabula, ZEPHYRUM, II, 'X');
    tabula_asserere(&tabula, "abX   hello", "inserere X in barrier");
}

hic_manens vacuum
probans_inserere_characterem_post_contentum(vacuum)
{
    TabulaCharacterum tabula;

    /* Inserere post contentum - solum pingere, non trudere */
    tabula_ex_literis(&tabula, "ab");
    tabula_inserere_characterem(&tabula, ZEPHYRUM, V, 'X');
    tabula_asserere(&tabula, "ab   X", "inserere X post contentum (solum pingere)");
}

hic_manens vacuum
probans_delere_characterem(vacuum)
{
    TabulaCharacterum tabula;

    tabula_ex_literis(&tabula, "hello");
    tabula_delere_characterem(&tabula, ZEPHYRUM, II);
    tabula_asserere(&tabula, "helo", "delere 'l' ex hello");

    /* Delere in linea vacua - nihil facit */
    tabula_initiare(&tabula);
    tabula_delere_characterem(&tabula, ZEPHYRUM, ZEPHYRUM);
    CREDO_VERUM(tabula_est_vacua(&tabula));
}


/* ==================================================
 * Test: Test Helpers
 * ================================================== */

hic_manens vacuum
probans_ex_literis(vacuum)
{
    TabulaCharacterum tabula;

    /* Una linea */
    tabula_ex_literis(&tabula, "hello");
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[ZEPHYRUM][ZEPHYRUM], (i32)'h');
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[ZEPHYRUM][IV], (i32)'o');

    /* Plures lineae */
    tabula_ex_literis(&tabula, "line1\nline2");
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[ZEPHYRUM][ZEPHYRUM], (i32)'l');
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[I][ZEPHYRUM], (i32)'l');
}

hic_manens vacuum
probans_aequalis_literis(vacuum)
{
    TabulaCharacterum tabula;

    tabula_ex_literis(&tabula, "hello");
    CREDO_VERUM(tabula_aequalis_literis(&tabula, "hello"));
    CREDO_FALSUM(tabula_aequalis_literis(&tabula, "world"));

    tabula_ex_literis(&tabula, "line1\nline2");
    CREDO_VERUM(tabula_aequalis_literis(&tabula, "line1\nline2"));
}


/* ==================================================
 * Main
 * ================================================== */

integer
principale(vacuum)
{
    Piscina* piscina;
    b32      praeteritus;

    /* Aperire credo et piscina */
    piscina = piscina_generare_dynamicum("test_tabula_characterum", 4096);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    printf("--- Probans initiare ---\n");
    probans_initiare();

    printf("--- Probans invenire_finem_contenti ---\n");
    probans_invenire_finem_contenti();

    printf("--- Probans invenire_initium_contenti ---\n");
    probans_invenire_initium_contenti();

    printf("--- Probans invenire_obicem ---\n");
    probans_invenire_obicem();

    printf("--- Probans trudere_dextram ---\n");
    probans_trudere_dextram();

    printf("--- Probans inserere_lineam ---\n");
    probans_inserere_lineam();

    printf("--- Probans delere_lineam ---\n");
    probans_delere_lineam();

    printf("--- Probans inserere_characterem_simplex ---\n");
    probans_inserere_characterem_simplex();

    printf("--- Probans inserere_characterem_cum_push ---\n");
    probans_inserere_characterem_cum_push();

    printf("--- Probans inserere_characterem_post_contentum ---\n");
    probans_inserere_characterem_post_contentum();

    printf("--- Probans delere_characterem ---\n");
    probans_delere_characterem();

    printf("--- Probans ex_literis ---\n");
    probans_ex_literis();

    printf("--- Probans aequalis_literis ---\n");
    probans_aequalis_literis();

    /* Compendium */
    printf("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    /* Destructio */
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
