/* probatio_tabula_characterum.c - Probationes TabulaCharacterum */
#include "latina.h"
#include "tabula_characterum.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>

/* Piscina globalis pro probationes */
hic_manens Piscina* g_piscina = NIHIL;

/* ==================================================
 * Test: Initiatio
 * ================================================== */

hic_manens vacuum
probans_initiare(vacuum)
{
    TabulaCharacterum tabula;

    tabula_initiare(&tabula, g_piscina, TABULA_LATITUDO_DEFALTA, TABULA_ALTITUDO_DEFALTA);

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
    tabula_initiare(&tabula, g_piscina, TABULA_LATITUDO_DEFALTA, TABULA_ALTITUDO_DEFALTA);
    CREDO_AEQUALIS_S32(tabula_invenire_finem_contenti(&tabula, ZEPHYRUM), -I);

    /* Linea cum uno charactere */
    tabula_ex_literis(&tabula, g_piscina, "a");
    CREDO_AEQUALIS_S32(tabula_invenire_finem_contenti(&tabula, ZEPHYRUM), ZEPHYRUM);

    /* Linea cum characteribus */
    tabula_ex_literis(&tabula, g_piscina, "hello");
    CREDO_AEQUALIS_S32(tabula_invenire_finem_contenti(&tabula, ZEPHYRUM), IV);

    /* Linea cum spatiis post contentum - spatia non sunt contentum */
    tabula_ex_literis(&tabula, g_piscina, "hi   ");
    CREDO_AEQUALIS_S32(tabula_invenire_finem_contenti(&tabula, ZEPHYRUM), I);
}

hic_manens vacuum
probans_invenire_initium_contenti(vacuum)
{
    TabulaCharacterum tabula;

    /* Linea vacua */
    tabula_initiare(&tabula, g_piscina, TABULA_LATITUDO_DEFALTA, TABULA_ALTITUDO_DEFALTA);
    CREDO_AEQUALIS_I32(tabula_invenire_initium_contenti(&tabula, ZEPHYRUM), tabula.latitudo);

    /* Linea sine indentation */
    tabula_ex_literis(&tabula, g_piscina, "hello");
    CREDO_AEQUALIS_I32(tabula_invenire_initium_contenti(&tabula, ZEPHYRUM), ZEPHYRUM);

    /* Linea cum indentation */
    tabula_ex_literis(&tabula, g_piscina, "    hello");
    CREDO_AEQUALIS_I32(tabula_invenire_initium_contenti(&tabula, ZEPHYRUM), IV);
}

hic_manens vacuum
probans_invenire_obicem(vacuum)
{
    TabulaCharacterum tabula;

    /* Linea vacua - nulla obex */
    tabula_initiare(&tabula, g_piscina, TABULA_LATITUDO_DEFALTA, TABULA_ALTITUDO_DEFALTA);
    CREDO_AEQUALIS_S32(tabula_invenire_obicem(&tabula, ZEPHYRUM, ZEPHYRUM), -I);

    /* Contentum ad initium */
    tabula_ex_literis(&tabula, g_piscina, "hello");
    CREDO_AEQUALIS_S32(tabula_invenire_obicem(&tabula, ZEPHYRUM, ZEPHYRUM), ZEPHYRUM);

    /* Spatia ante contentum */
    tabula_ex_literis(&tabula, g_piscina, "ab   hello");
    CREDO_AEQUALIS_S32(tabula_invenire_obicem(&tabula, ZEPHYRUM, II), V);

    /* Nulla contentum post columnam */
    tabula_ex_literis(&tabula, g_piscina, "hi");
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
    tabula_ex_literis(&tabula, g_piscina, "hello");
    overflow = tabula_trudere_dextram(&tabula, ZEPHYRUM, II);
    CREDO_AEQUALIS_I32((i32)overflow, (i32)' ');  /* Grid impleta cum spatiis */
    tabula_asserere(&tabula, g_piscina, "he llo", "trudere dextram in hello");

    /* Trudere cum overflow */
    tabula_ex_literis(&tabula, g_piscina, "");
    /* Implere lineam */
    {
        i32 i;
        per (i = ZEPHYRUM; i < tabula.latitudo; i++)
        {
            tabula_cellula(&tabula, ZEPHYRUM, i) = 'x';
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

    tabula_ex_literis(&tabula, g_piscina, "line0\nline1\nline2");

    successus = tabula_inserere_lineam(&tabula, I);
    CREDO_VERUM(successus);
    tabula_asserere(&tabula, g_piscina, "line0\n\nline1\nline2", "inserere lineam ad 1");
}

hic_manens vacuum
probans_delere_lineam(vacuum)
{
    TabulaCharacterum tabula;

    tabula_ex_literis(&tabula, g_piscina, "line0\nline1\nline2");

    tabula_delere_lineam(&tabula, I);
    tabula_asserere(&tabula, g_piscina, "line0\nline2", "delere lineam 1");
}


/* ==================================================
 * Test: Main Operations
 * ================================================== */

hic_manens vacuum
probans_inserere_characterem_simplex(vacuum)
{
    TabulaCharacterum tabula;

    /* Inserere in tabula vacua */
    tabula_initiare(&tabula, g_piscina, TABULA_LATITUDO_DEFALTA, TABULA_ALTITUDO_DEFALTA);
    tabula_inserere_characterem(&tabula, ZEPHYRUM, ZEPHYRUM, 'H');
    tabula_asserere(&tabula, g_piscina, "H", "inserere H in vacua");

    tabula_inserere_characterem(&tabula, ZEPHYRUM, I, 'i');
    tabula_asserere(&tabula, g_piscina, "Hi", "inserere i post H");
}

hic_manens vacuum
probans_inserere_characterem_consumere_whitespace(vacuum)
{
    TabulaCharacterum tabula;

    /* Inserere in whitespace cushion - solum pingere (consumere whitespace) */
    tabula_ex_literis(&tabula, g_piscina, "   hello");
    tabula_inserere_characterem(&tabula, ZEPHYRUM, ZEPHYRUM, 'X');
    tabula_asserere(&tabula, g_piscina, "X  hello", "inserere X consumit whitespace");

    /* Inserere in medio whitespace cushion */
    tabula_ex_literis(&tabula, g_piscina, "ab   hello");
    tabula_inserere_characterem(&tabula, ZEPHYRUM, II, 'X');
    tabula_asserere(&tabula, g_piscina, "abX  hello", "inserere X in whitespace cushion");
}

hic_manens vacuum
probans_inserere_characterem_ad_barrier(vacuum)
{
    TabulaCharacterum tabula;

    /* Inserere ad barrier (immediate ante contentum) - debet trudere */
    tabula_ex_literis(&tabula, g_piscina, "ab hello");  /* 1 space = barrier */
    tabula_inserere_characterem(&tabula, ZEPHYRUM, II, 'X');
    tabula_asserere(&tabula, g_piscina, "abX hello", "inserere X ad barrier trudere");

    /* Inserere directe ante contentum (cursor ad ultimum whitespace) */
    tabula_ex_literis(&tabula, g_piscina, "  hello");  /* 2 spaces */
    tabula_inserere_characterem(&tabula, ZEPHYRUM, I, 'X');
    tabula_asserere(&tabula, g_piscina, " X hello", "inserere X ad barrier (1 space ante)");
}

hic_manens vacuum
probans_inserere_characterem_post_contentum(vacuum)
{
    TabulaCharacterum tabula;

    /* Inserere post contentum - solum pingere, non trudere */
    tabula_ex_literis(&tabula, g_piscina, "ab");
    tabula_inserere_characterem(&tabula, ZEPHYRUM, V, 'X');
    tabula_asserere(&tabula, g_piscina, "ab   X", "inserere X post contentum (solum pingere)");
}

hic_manens vacuum
probans_delere_characterem(vacuum)
{
    TabulaCharacterum tabula;

    tabula_ex_literis(&tabula, g_piscina, "hello");
    tabula_delere_characterem(&tabula, ZEPHYRUM, II);
    tabula_asserere(&tabula, g_piscina, "helo", "delere 'l' ex hello");

    /* Delere in linea vacua - nihil facit */
    tabula_initiare(&tabula, g_piscina, TABULA_LATITUDO_DEFALTA, TABULA_ALTITUDO_DEFALTA);
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
    tabula_ex_literis(&tabula, g_piscina, "hello");
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'h');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, IV), (i32)'o');

    /* Plures lineae */
    tabula_ex_literis(&tabula, g_piscina, "line1\nline2");
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'l');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, ZEPHYRUM), (i32)'l');
}

hic_manens vacuum
probans_aequalis_literis(vacuum)
{
    TabulaCharacterum tabula;

    tabula_ex_literis(&tabula, g_piscina, "hello");
    CREDO_VERUM(tabula_aequalis_literis(&tabula, "hello"));
    CREDO_FALSUM(tabula_aequalis_literis(&tabula, "world"));

    tabula_ex_literis(&tabula, g_piscina, "line1\nline2");
    CREDO_VERUM(tabula_aequalis_literis(&tabula, "line1\nline2"));
}


/* ==================================================
 * Test: Tab Handling
 * ================================================== */

hic_manens vacuum
probans_inserere_tab_simplex(vacuum)
{
    TabulaCharacterum tabula;
    b32 successus;

    /* Inserere tab in tabula vacua ad columna 0 */
    tabula_initiare(&tabula, g_piscina, TABULA_LATITUDO_DEFALTA, TABULA_ALTITUDO_DEFALTA);
    successus = tabula_inserere_tab(&tabula, ZEPHYRUM, ZEPHYRUM);
    CREDO_VERUM(successus);

    /* Verificare: '\t' ad columna 0, TAB_CONTINUATIO ad columna 1 */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'\t');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, I), (i32)TAB_CONTINUATIO);

    /* Verificare: spatia post tab */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, II), (i32)' ');
}

hic_manens vacuum
probans_inserere_tab_ante_contentum(vacuum)
{
    TabulaCharacterum tabula;
    b32 successus;

    /* Inserere "hello" deinde tab ad initium - debet trudere */
    tabula_ex_literis(&tabula, g_piscina, "hello");
    successus = tabula_inserere_tab(&tabula, ZEPHYRUM, ZEPHYRUM);
    CREDO_VERUM(successus);

    /* Verificare: '\t', TAB_CONTINUATIO, 'h', 'e', 'l', 'l', 'o' */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'\t');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, I), (i32)TAB_CONTINUATIO);
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, II), (i32)'h');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, III), (i32)'e');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, VI), (i32)'o');
}

hic_manens vacuum
probans_inserere_tab_in_medio(vacuum)
{
    TabulaCharacterum tabula;
    b32 successus;

    /* Inserere "ab" deinde tab deinde "cd" */
    tabula_ex_literis(&tabula, g_piscina, "abcd");
    successus = tabula_inserere_tab(&tabula, ZEPHYRUM, II);
    CREDO_VERUM(successus);

    /* Verificare: 'a', 'b', '\t', TAB_CONTINUATIO, 'c', 'd' */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'a');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, I), (i32)'b');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, II), (i32)'\t');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, III), (i32)TAB_CONTINUATIO);
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, IV), (i32)'c');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, V), (i32)'d');
}

hic_manens vacuum
probans_delere_tab(vacuum)
{
    TabulaCharacterum tabula;

    /* Creare tab manuali */
    tabula_initiare(&tabula, g_piscina, TABULA_LATITUDO_DEFALTA, TABULA_ALTITUDO_DEFALTA);
    tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM) = '\t';
    tabula_cellula(&tabula, ZEPHYRUM, I) = TAB_CONTINUATIO;
    tabula_cellula(&tabula, ZEPHYRUM, II) = 'h';
    tabula_cellula(&tabula, ZEPHYRUM, III) = 'i';

    /* Delere tab ad columna 0 */
    tabula_delere_tab(&tabula, ZEPHYRUM, ZEPHYRUM);

    /* Verificare: "hi" ad initium */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'h');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, I), (i32)'i');
}

hic_manens vacuum
probans_delere_ad_tab_continuatio(vacuum)
{
    TabulaCharacterum tabula;

    /* Creare "ab" + tab + "cd" */
    tabula_initiare(&tabula, g_piscina, TABULA_LATITUDO_DEFALTA, TABULA_ALTITUDO_DEFALTA);
    tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM) = 'a';
    tabula_cellula(&tabula, ZEPHYRUM, I) = 'b';
    tabula_cellula(&tabula, ZEPHYRUM, II) = '\t';
    tabula_cellula(&tabula, ZEPHYRUM, III) = TAB_CONTINUATIO;
    tabula_cellula(&tabula, ZEPHYRUM, IV) = 'c';
    tabula_cellula(&tabula, ZEPHYRUM, V) = 'd';

    /* Delere ad columna 3 (TAB_CONTINUATIO) - debet delere totum tab */
    tabula_delere_tab(&tabula, ZEPHYRUM, III);

    /* Verificare: "abcd" */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'a');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, I), (i32)'b');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, II), (i32)'c');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, III), (i32)'d');
}

hic_manens vacuum
probans_est_tab_continuatio(vacuum)
{
    TabulaCharacterum tabula;

    tabula_initiare(&tabula, g_piscina, TABULA_LATITUDO_DEFALTA, TABULA_ALTITUDO_DEFALTA);
    tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM) = 'a';
    tabula_cellula(&tabula, ZEPHYRUM, I) = '\t';
    tabula_cellula(&tabula, ZEPHYRUM, II) = TAB_CONTINUATIO;
    tabula_cellula(&tabula, ZEPHYRUM, III) = 'b';

    CREDO_FALSUM(tabula_est_tab_continuatio(&tabula, ZEPHYRUM, ZEPHYRUM));  /* 'a' */
    CREDO_FALSUM(tabula_est_tab_continuatio(&tabula, ZEPHYRUM, I));         /* '\t' */
    CREDO_VERUM(tabula_est_tab_continuatio(&tabula, ZEPHYRUM, II));         /* TAB_CONTINUATIO */
    CREDO_FALSUM(tabula_est_tab_continuatio(&tabula, ZEPHYRUM, III));       /* 'b' */
}


/* ==================================================
 * Test: Dimensiones Variae
 * ================================================== */

hic_manens vacuum
probans_dimensiones_parvae(vacuum)
{
    TabulaCharacterum tabula;
    i32 parva_latitudo = X;   /* 10 */
    i32 parva_altitudo = V;   /* 5 */

    /* Initiare cum dimensionibus parvis */
    tabula_initiare(&tabula, g_piscina, parva_latitudo, parva_altitudo);
    CREDO_AEQUALIS_I32(tabula.latitudo, parva_latitudo);
    CREDO_AEQUALIS_I32(tabula.altitudo, parva_altitudo);
    CREDO_VERUM(tabula_est_vacua(&tabula));

    /* Inserere contentum et verificare limites */
    tabula_inserere_characterem(&tabula, ZEPHYRUM, ZEPHYRUM, 'A');
    tabula_inserere_characterem(&tabula, ZEPHYRUM, parva_latitudo - I, 'Z');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'A');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, parva_latitudo - I), (i32)'Z');

    /* Probare ultimam lineam */
    tabula_inserere_characterem(&tabula, parva_altitudo - I, ZEPHYRUM, 'X');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, parva_altitudo - I, ZEPHYRUM), (i32)'X');
}

hic_manens vacuum
probans_dimensiones_minimae(vacuum)
{
    TabulaCharacterum tabula;
    i32 minima_latitudo = IV;   /* 4 - minima pro tab (2) + aliquid */
    i32 minima_altitudo = II;   /* 2 */

    /* Initiare cum dimensionibus minimis */
    tabula_initiare(&tabula, g_piscina, minima_latitudo, minima_altitudo);
    CREDO_AEQUALIS_I32(tabula.latitudo, minima_latitudo);
    CREDO_AEQUALIS_I32(tabula.altitudo, minima_altitudo);

    /* Inserere tab - requirit 2 cellas */
    tabula_inserere_tab(&tabula, ZEPHYRUM, ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'\t');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, I), (i32)TAB_CONTINUATIO);

    /* Inserere lineam - probare scroll */
    tabula_inserere_lineam(&tabula, ZEPHYRUM);
    /* Verificare quod inserere lineam non frangit */
    CREDO_AEQUALIS_I32(tabula.altitudo, minima_altitudo);
}

hic_manens vacuum
probans_dimensiones_asymmetricae(vacuum)
{
    TabulaCharacterum tabula;
    i32 lata_latitudo = C;   /* 100 - multa lata */
    i32 brevis_altitudo = III; /* 3 - paucae lineae */

    /* Initiare cum latitudine magna et altitudine parva */
    tabula_initiare(&tabula, g_piscina, lata_latitudo, brevis_altitudo);
    CREDO_AEQUALIS_I32(tabula.latitudo, lata_latitudo);
    CREDO_AEQUALIS_I32(tabula.altitudo, brevis_altitudo);

    /* Inserere textum longum */
    {
        i32 i;
        per (i = ZEPHYRUM; i < L; i++)  /* 50 characteres */
        {
            tabula_inserere_characterem(&tabula, ZEPHYRUM, i, 'x');
        }
    }
    CREDO_AEQUALIS_S32(tabula_invenire_finem_contenti(&tabula, ZEPHYRUM), XLIX);

    /* Probare delere lineam cum paucis lineis */
    tabula_inserere_characterem(&tabula, I, ZEPHYRUM, 'A');
    tabula_inserere_characterem(&tabula, II, ZEPHYRUM, 'B');
    tabula_delere_lineam(&tabula, I);
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, ZEPHYRUM), (i32)'B');
}

hic_manens vacuum
probans_dimensiones_trudere_ad_limitem(vacuum)
{
    TabulaCharacterum tabula;
    i32 latitudo = VIII;   /* 8 */
    i32 altitudo = II;     /* 2 */
    character overflow;

    tabula_initiare(&tabula, g_piscina, latitudo, altitudo);

    /* Implere lineam prope finem */
    tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM) = 'a';
    tabula_cellula(&tabula, ZEPHYRUM, I) = 'b';
    tabula_cellula(&tabula, ZEPHYRUM, II) = 'c';
    tabula_cellula(&tabula, ZEPHYRUM, III) = 'd';
    tabula_cellula(&tabula, ZEPHYRUM, IV) = 'e';
    tabula_cellula(&tabula, ZEPHYRUM, V) = 'f';
    tabula_cellula(&tabula, ZEPHYRUM, VI) = 'g';
    tabula_cellula(&tabula, ZEPHYRUM, VII) = 'h';

    /* Trudere - debet habere overflow */
    overflow = tabula_trudere_dextram(&tabula, ZEPHYRUM, ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)overflow, (i32)'h');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)' ');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, I), (i32)'a');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, VII), (i32)'g');
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
    piscina = piscina_generare_dynamicum("test_tabula_characterum", M);  /* 1MB - pro multis tabulis */
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    g_piscina = piscina;
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

    printf("--- Probans inserere_characterem_consumere_whitespace ---\n");
    probans_inserere_characterem_consumere_whitespace();

    printf("--- Probans inserere_characterem_ad_barrier ---\n");
    probans_inserere_characterem_ad_barrier();

    printf("--- Probans inserere_characterem_post_contentum ---\n");
    probans_inserere_characterem_post_contentum();

    printf("--- Probans delere_characterem ---\n");
    probans_delere_characterem();

    printf("--- Probans ex_literis ---\n");
    probans_ex_literis();

    printf("--- Probans aequalis_literis ---\n");
    probans_aequalis_literis();

    printf("--- Probans inserere_tab_simplex ---\n");
    probans_inserere_tab_simplex();

    printf("--- Probans inserere_tab_ante_contentum ---\n");
    probans_inserere_tab_ante_contentum();

    printf("--- Probans inserere_tab_in_medio ---\n");
    probans_inserere_tab_in_medio();

    printf("--- Probans delere_tab ---\n");
    probans_delere_tab();

    printf("--- Probans delere_ad_tab_continuatio ---\n");
    probans_delere_ad_tab_continuatio();

    printf("--- Probans est_tab_continuatio ---\n");
    probans_est_tab_continuatio();

    printf("--- Probans dimensiones_parvae ---\n");
    probans_dimensiones_parvae();

    printf("--- Probans dimensiones_minimae ---\n");
    probans_dimensiones_minimae();

    printf("--- Probans dimensiones_asymmetricae ---\n");
    probans_dimensiones_asymmetricae();

    printf("--- Probans dimensiones_trudere_ad_limitem ---\n");
    probans_dimensiones_trudere_ad_limitem();

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
