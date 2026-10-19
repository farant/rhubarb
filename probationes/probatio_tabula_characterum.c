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
probans_initiare(CredoContextus* c)
{
    TabulaCharacterum tabula;

    tabula_initiare(&tabula, c->piscina, TABULA_LATITUDO_DEFALTA,
        TABULA_ALTITUDO_DEFALTA);

    CREDO_VERUM(tabula_est_vacua(&tabula));
    CREDO_FALSUM(tabula_est_plena(&tabula));
}


/* ==================================================
 * Test: Query Functions
 * ================================================== */

hic_manens vacuum
probans_invenire_finem_contenti(CredoContextus* c)
{
    TabulaCharacterum tabula;

    /* Linea vacua */
    tabula_initiare(&tabula, c->piscina, TABULA_LATITUDO_DEFALTA,
        TABULA_ALTITUDO_DEFALTA);
    CREDO_AEQUALIS_S32(tabula_invenire_finem_contenti(&tabula,
        ZEPHYRUM), -I);

    /* Linea cum uno charactere */
    tabula_ex_literis(&tabula, c->piscina, "a");
    CREDO_AEQUALIS_S32(tabula_invenire_finem_contenti(&tabula,
        ZEPHYRUM), ZEPHYRUM);

    /* Linea cum characteribus */
    tabula_ex_literis(&tabula, c->piscina, "hello");
    CREDO_AEQUALIS_S32(tabula_invenire_finem_contenti(&tabula,
        ZEPHYRUM), IV);

    /* Linea cum spatiis post contentum - spatia non sunt contentum */
    tabula_ex_literis(&tabula, c->piscina, "hi   ");
    CREDO_AEQUALIS_S32(tabula_invenire_finem_contenti(&tabula,
        ZEPHYRUM), I);
}

hic_manens vacuum
probans_invenire_initium_contenti(CredoContextus* c)
{
    TabulaCharacterum tabula;

    /* Linea vacua */
    tabula_initiare(&tabula, c->piscina, TABULA_LATITUDO_DEFALTA,
        TABULA_ALTITUDO_DEFALTA);
    CREDO_AEQUALIS_I32(tabula_invenire_initium_contenti(&tabula,
        ZEPHYRUM), tabula.latitudo);

    /* Linea sine indentation */
    tabula_ex_literis(&tabula, c->piscina, "hello");
    CREDO_AEQUALIS_I32(tabula_invenire_initium_contenti(&tabula,
        ZEPHYRUM), ZEPHYRUM);

    /* Linea cum indentation */
    tabula_ex_literis(&tabula, c->piscina, "    hello");
    CREDO_AEQUALIS_I32(tabula_invenire_initium_contenti(&tabula,
        ZEPHYRUM), IV);
}

hic_manens vacuum
probans_invenire_obicem(CredoContextus* c)
{
    TabulaCharacterum tabula;

    /* Linea vacua - nulla obex */
    tabula_initiare(&tabula, c->piscina, TABULA_LATITUDO_DEFALTA,
        TABULA_ALTITUDO_DEFALTA);
    CREDO_AEQUALIS_S32(tabula_invenire_obicem(&tabula, ZEPHYRUM,
        ZEPHYRUM), -I);

    /* Contentum ad initium */
    tabula_ex_literis(&tabula, c->piscina, "hello");
    CREDO_AEQUALIS_S32(tabula_invenire_obicem(&tabula, ZEPHYRUM,
        ZEPHYRUM), ZEPHYRUM);

    /* Spatia ante contentum */
    tabula_ex_literis(&tabula, c->piscina, "ab   hello");
    CREDO_AEQUALIS_S32(tabula_invenire_obicem(&tabula, ZEPHYRUM, II),
        V);

    /* Nulla contentum post columnam */
    tabula_ex_literis(&tabula, c->piscina, "hi");
    CREDO_AEQUALIS_S32(tabula_invenire_obicem(&tabula, ZEPHYRUM, V),
        -I);
}


/* ==================================================
 * Test: Mutation Functions
 * ================================================== */

hic_manens vacuum
probans_trudere_dextram(CredoContextus* c)
{
    TabulaCharacterum tabula;
            character overflow;

    /* Trudere in linea cum spatio */
    tabula_ex_literis(&tabula, c->piscina, "hello");
    overflow = tabula_trudere_dextram(&tabula, ZEPHYRUM, II);
    CREDO_AEQUALIS_I32((i32)overflow, (i32)' ');  /* Grid impleta cum spatiis */
    tabula_asserere(&tabula, c->piscina, "he llo",
        "trudere dextram in hello");

    /* Trudere cum overflow */
    tabula_ex_literis(&tabula, c->piscina, "");
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

    /* Cum dimensionibus parvis (8x2) - overflow facilius testare */
    tabula_ex_literis_cum_dimensionibus(&tabula, c->piscina, VIII, II,
        "abcdefgh");
    CREDO_AEQUALIS_I32(tabula.latitudo, VIII);
    overflow = tabula_trudere_dextram(&tabula, ZEPHYRUM, ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)overflow, (i32)'h');  /* Ultimus character excidit */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM),
        (i32)' ');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, I),
        (i32)'a');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, VII),
        (i32)'g');
}

hic_manens vacuum
probans_inserere_lineam(CredoContextus* c)
{
    TabulaCharacterum tabula;
                  b32 successus;

    tabula_ex_literis(&tabula, c->piscina, "line0\nline1\nline2");

    successus = tabula_inserere_lineam(&tabula, I);
    CREDO_VERUM(successus);
    tabula_asserere(&tabula, c->piscina, "line0\n\nline1\nline2",
        "inserere lineam ad 1");

    /* Cum dimensionibus parvis (10x4) */
    tabula_ex_literis_cum_dimensionibus(&tabula, c->piscina, X, IV,
        "aa\nbb\ncc");
    CREDO_AEQUALIS_I32(tabula.altitudo, IV);
    successus = tabula_inserere_lineam(&tabula, I);
    CREDO_VERUM(successus);
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM),
        (i32)'a');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, ZEPHYRUM),
        (i32)' ');  /* Nova linea vacua */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, II, ZEPHYRUM),
        (i32)'b');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, III, ZEPHYRUM),
        (i32)'c');
}

hic_manens vacuum
probans_delere_lineam(CredoContextus* c)
{
    TabulaCharacterum tabula;

    tabula_ex_literis(&tabula, c->piscina, "line0\nline1\nline2");

    tabula_delere_lineam(&tabula, I);
    tabula_asserere(&tabula, c->piscina, "line0\nline2",
        "delere lineam 1");

    /* Idem cum dimensionibus parvis (8x4) */
    tabula_ex_literis_cum_dimensionibus(&tabula, c->piscina, VIII, IV,
        "aa\nbb\ncc\ndd");
    CREDO_AEQUALIS_I32(tabula.altitudo, IV);

    /* Delere lineam 1 (bb) */
    tabula_delere_lineam(&tabula, I);
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM),
        (i32)'a');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, ZEPHYRUM),
        (i32)'c');  /* cc moved up */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, II, ZEPHYRUM),
        (i32)'d');  /* dd moved up */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, III, ZEPHYRUM),
        (i32)' '); /* ultima linea vacua */
}


/* ==================================================
 * Test: Main Operations
 * ================================================== */

hic_manens vacuum
probans_inserere_characterem_simplex(CredoContextus* c)
{
    TabulaCharacterum tabula;

    /* Inserere in tabula vacua */
    tabula_initiare(&tabula, c->piscina, TABULA_LATITUDO_DEFALTA,
        TABULA_ALTITUDO_DEFALTA);
    tabula_inserere_characterem(&tabula, ZEPHYRUM, ZEPHYRUM, 'H');
    tabula_asserere(&tabula, c->piscina, "H", "inserere H in vacua");

    tabula_inserere_characterem(&tabula, ZEPHYRUM, I, 'i');
    tabula_asserere(&tabula, c->piscina, "Hi", "inserere i post H");

    /* Idem cum dimensionibus parvis (6x2) */
    tabula_initiare(&tabula, c->piscina, VI, II);
    CREDO_AEQUALIS_I32(tabula.latitudo, VI);
    CREDO_AEQUALIS_I32(tabula.altitudo, II);

    tabula_inserere_characterem(&tabula, ZEPHYRUM, ZEPHYRUM, 'A');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM),
        (i32)'A');

    tabula_inserere_characterem(&tabula, ZEPHYRUM, I, 'B');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, I),
        (i32)'B');

    tabula_inserere_characterem(&tabula, I, ZEPHYRUM, 'C');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, ZEPHYRUM),
        (i32)'C');
}

hic_manens vacuum
probans_inserere_characterem_consumere_whitespace (
    CredoContextus* c)
{
    TabulaCharacterum tabula;

    /* Inserere in whitespace cushion - solum pingere (consumere whitespace) */
    tabula_ex_literis(&tabula, c->piscina, "   hello");
    tabula_inserere_characterem(&tabula, ZEPHYRUM, ZEPHYRUM, 'X');
    tabula_asserere(&tabula, c->piscina, "X  hello",
        "inserere X consumit whitespace");

    /* Inserere in medio whitespace cushion */
    tabula_ex_literis(&tabula, c->piscina, "ab   hello");
    tabula_inserere_characterem(&tabula, ZEPHYRUM, II, 'X');
    tabula_asserere(&tabula, c->piscina, "abX  hello",
        "inserere X in whitespace cushion");
}

hic_manens vacuum
probans_inserere_characterem_ad_barrier (
    CredoContextus* c)
{
    TabulaCharacterum tabula;

    /* Inserere ad barrier (immediate ante contentum) - debet trudere */
    tabula_ex_literis(&tabula, c->piscina, "ab hello");  /* 1 space = barrier */
    tabula_inserere_characterem(&tabula, ZEPHYRUM, II, 'X');
    tabula_asserere(&tabula, c->piscina, "abX hello",
        "inserere X ad barrier trudere");

    /* Inserere directe ante contentum (cursor ad ultimum whitespace) */
    tabula_ex_literis(&tabula, c->piscina, "  hello");  /* 2 spaces */
    tabula_inserere_characterem(&tabula, ZEPHYRUM, I, 'X');
    tabula_asserere(&tabula, c->piscina, " X hello",
        "inserere X ad barrier (1 space ante)");
}

hic_manens vacuum
probans_inserere_characterem_post_contentum (
    CredoContextus* c)
{
    TabulaCharacterum tabula;

    /* Inserere post contentum - solum pingere, non trudere */
    tabula_ex_literis(&tabula, c->piscina, "ab");
    tabula_inserere_characterem(&tabula, ZEPHYRUM, V, 'X');
    tabula_asserere(&tabula, c->piscina, "ab   X",
        "inserere X post contentum (solum pingere)");
}

hic_manens vacuum
probans_delere_characterem(CredoContextus* c)
{
    TabulaCharacterum tabula;

    tabula_ex_literis(&tabula, c->piscina, "hello");
    tabula_delere_characterem(&tabula, ZEPHYRUM, II);
    tabula_asserere(&tabula, c->piscina, "helo", "delere 'l' ex hello");

    /* Delere in linea vacua - nihil facit */
    tabula_initiare(&tabula, c->piscina, TABULA_LATITUDO_DEFALTA,
        TABULA_ALTITUDO_DEFALTA);
    tabula_delere_characterem(&tabula, ZEPHYRUM, ZEPHYRUM);
    CREDO_VERUM(tabula_est_vacua(&tabula));
}


/* ==================================================
 * Test: Test Helpers
 * ================================================== */

hic_manens vacuum
probans_ex_literis(CredoContextus* c)
{
    TabulaCharacterum tabula;

    /* Una linea */
    tabula_ex_literis(&tabula, c->piscina, "hello");
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM),
        (i32)'h');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, IV),
        (i32)'o');

    /* Plures lineae */
    tabula_ex_literis(&tabula, c->piscina, "line1\nline2");
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM),
        (i32)'l');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, ZEPHYRUM),
        (i32)'l');

    /* Cum dimensionibus parvis (10x3) */
    tabula_ex_literis_cum_dimensionibus(&tabula, c->piscina, X, III,
        "abc\nde");
    CREDO_AEQUALIS_I32(tabula.latitudo, X);
    CREDO_AEQUALIS_I32(tabula.altitudo, III);
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM),
        (i32)'a');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, II),
        (i32)'c');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, ZEPHYRUM),
        (i32)'d');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, I), (i32)'e');
}

hic_manens vacuum
probans_aequalis_literis(CredoContextus* c)
{
    TabulaCharacterum tabula;

    tabula_ex_literis(&tabula, c->piscina, "hello");
    CREDO_VERUM(tabula_aequalis_literis(&tabula, "hello"));
    CREDO_FALSUM(tabula_aequalis_literis(&tabula, "world"));

    tabula_ex_literis(&tabula, c->piscina, "line1\nline2");
    CREDO_VERUM(tabula_aequalis_literis(&tabula, "line1\nline2"));
}


/* ==================================================
 * Test: Tab Handling
 * ================================================== */

hic_manens vacuum
probans_inserere_tab_simplex(CredoContextus* c)
{
    TabulaCharacterum tabula;
                  b32 successus;

    /* Inserere tab in tabula vacua ad columna 0 */
    tabula_initiare(&tabula, c->piscina, TABULA_LATITUDO_DEFALTA,
        TABULA_ALTITUDO_DEFALTA);
    successus = tabula_inserere_tab(&tabula, ZEPHYRUM, ZEPHYRUM);
    CREDO_VERUM(successus);

    /* Verificare: '\t' ad columna 0, TAB_CONTINUATIO ad columna 1 */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM),
        (i32)'\t');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, I),
        (i32)TAB_CONTINUATIO);

    /* Verificare: spatia post tab */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, II),
        (i32)' ');

    /* Idem cum dimensionibus parvis (6x2) - tab requirit 2 cellulas */
    tabula_initiare(&tabula, c->piscina, VI, II);
    CREDO_AEQUALIS_I32(tabula.latitudo, VI);

    successus = tabula_inserere_tab(&tabula, ZEPHYRUM, ZEPHYRUM);
    CREDO_VERUM(successus);
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM),
        (i32)'\t');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, I),
        (i32)TAB_CONTINUATIO);

    /* Tab ad columna 2 */
    successus = tabula_inserere_tab(&tabula, ZEPHYRUM, II);
    CREDO_VERUM(successus);
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, II),
        (i32)'\t');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, III),
        (i32)TAB_CONTINUATIO);
}

hic_manens vacuum
probans_inserere_tab_ante_contentum(CredoContextus* c)
{
    TabulaCharacterum tabula;
                  b32 successus;

    /* Inserere "hello" deinde tab ad initium - debet trudere */
    tabula_ex_literis(&tabula, c->piscina, "hello");
    successus = tabula_inserere_tab(&tabula, ZEPHYRUM, ZEPHYRUM);
    CREDO_VERUM(successus);

    /* Verificare: '\t', TAB_CONTINUATIO, 'h', 'e', 'l', 'l', 'o' */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM),
        (i32)'\t');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, I),
        (i32)TAB_CONTINUATIO);
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, II),
        (i32)'h');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, III),
        (i32)'e');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, VI),
        (i32)'o');

    /* Idem cum dimensionibus parvis (8x2) - tab trudit "ab" */
    tabula_ex_literis_cum_dimensionibus(&tabula, c->piscina, VIII, II,
        "ab");
    CREDO_AEQUALIS_I32(tabula.latitudo, VIII);

    successus = tabula_inserere_tab(&tabula, ZEPHYRUM, ZEPHYRUM);
    CREDO_VERUM(successus);

    /* Verificare: '\t', TAB_CONTINUATIO, 'a', 'b' */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM),
        (i32)'\t');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, I),
        (i32)TAB_CONTINUATIO);
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, II),
        (i32)'a');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, III),
        (i32)'b');
}

hic_manens vacuum
probans_inserere_tab_in_medio(CredoContextus* c)
{
    TabulaCharacterum tabula;
                  b32 successus;

    /* Inserere "ab" deinde tab deinde "cd" */
    tabula_ex_literis(&tabula, c->piscina, "abcd");
    successus = tabula_inserere_tab(&tabula, ZEPHYRUM, II);
    CREDO_VERUM(successus);

    /* Verificare: 'a', 'b', '\t', TAB_CONTINUATIO, 'c', 'd' */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM),
        (i32)'a');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, I),
        (i32)'b');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, II),
        (i32)'\t');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, III),
        (i32)TAB_CONTINUATIO);
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, IV),
        (i32)'c');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, V),
        (i32)'d');
}

hic_manens vacuum
probans_delere_tab(CredoContextus* c)
{
    TabulaCharacterum tabula;

    /* Creare tab manuali */
    tabula_initiare(&tabula, c->piscina, TABULA_LATITUDO_DEFALTA,
        TABULA_ALTITUDO_DEFALTA);
    tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM) = '\t';
    tabula_cellula(&tabula, ZEPHYRUM, I) = TAB_CONTINUATIO;
    tabula_cellula(&tabula, ZEPHYRUM, II) = 'h';
    tabula_cellula(&tabula, ZEPHYRUM, III) = 'i';

    /* Delere tab ad columna 0 */
    tabula_delere_tab(&tabula, ZEPHYRUM, ZEPHYRUM);

    /* Verificare: "hi" ad initium */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM),
        (i32)'h');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, I),
        (i32)'i');
}

hic_manens vacuum
probans_delere_ad_tab_continuatio(CredoContextus* c)
{
    TabulaCharacterum tabula;

    /* Creare "ab" + tab + "cd" */
    tabula_initiare(&tabula, c->piscina, TABULA_LATITUDO_DEFALTA,
        TABULA_ALTITUDO_DEFALTA);
    tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM) = 'a';
    tabula_cellula(&tabula, ZEPHYRUM, I) = 'b';
    tabula_cellula(&tabula, ZEPHYRUM, II) = '\t';
    tabula_cellula(&tabula, ZEPHYRUM, III) = TAB_CONTINUATIO;
    tabula_cellula(&tabula, ZEPHYRUM, IV) = 'c';
    tabula_cellula(&tabula, ZEPHYRUM, V) = 'd';

    /* Delere ad columna 3 (TAB_CONTINUATIO) - debet delere totum tab */
    tabula_delere_tab(&tabula, ZEPHYRUM, III);

    /* Verificare: "abcd" */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM),
        (i32)'a');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, I),
        (i32)'b');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, II),
        (i32)'c');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, III),
        (i32)'d');
}

hic_manens vacuum
probans_est_tab_continuatio(CredoContextus* c)
{
    TabulaCharacterum tabula;

    tabula_initiare(&tabula, c->piscina, TABULA_LATITUDO_DEFALTA,
        TABULA_ALTITUDO_DEFALTA);
    tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM) = 'a';
    tabula_cellula(&tabula, ZEPHYRUM, I) = '\t';
    tabula_cellula(&tabula, ZEPHYRUM, II) = TAB_CONTINUATIO;
    tabula_cellula(&tabula, ZEPHYRUM, III) = 'b';

    CREDO_FALSUM(tabula_est_tab_continuatio(&tabula, ZEPHYRUM,
        ZEPHYRUM));  /* 'a' */
    CREDO_FALSUM(tabula_est_tab_continuatio(&tabula, ZEPHYRUM, I));         /* '\t' */
    CREDO_VERUM(tabula_est_tab_continuatio(&tabula, ZEPHYRUM, II));         /* TAB_CONTINUATIO */
    CREDO_FALSUM(tabula_est_tab_continuatio(&tabula, ZEPHYRUM, III));       /* 'b' */
}


/* ==================================================
 * Test: Dimensiones Variae
 * ================================================== */

hic_manens vacuum
probans_dimensiones_parvae(CredoContextus* c)
{
    TabulaCharacterum tabula;
                  i32 parva_latitudo = X;   /* 10 */
                  i32 parva_altitudo = V;   /* 5 */

    /* Initiare cum dimensionibus parvis */
    tabula_initiare(&tabula, c->piscina, parva_latitudo,
        parva_altitudo);
    CREDO_AEQUALIS_I32(tabula.latitudo, parva_latitudo);
    CREDO_AEQUALIS_I32(tabula.altitudo, parva_altitudo);
    CREDO_VERUM(tabula_est_vacua(&tabula));

    /* Inserere contentum et verificare limites */
    tabula_inserere_characterem(&tabula, ZEPHYRUM, ZEPHYRUM, 'A');
    tabula_inserere_characterem(&tabula, ZEPHYRUM, parva_latitudo - I,
        'Z');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM),
        (i32)'A');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM,
        parva_latitudo - I), (i32)'Z');

    /* Probare ultimam lineam */
    tabula_inserere_characterem(&tabula, parva_altitudo - I, ZEPHYRUM,
        'X');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, parva_altitudo - I,
        ZEPHYRUM), (i32)'X');
}

hic_manens vacuum
probans_dimensiones_minimae(CredoContextus* c)
{
    TabulaCharacterum tabula;
    i32 minima_latitudo = IV;   /* 4 - minima pro tab (2) + aliquid */
    i32 minima_altitudo = II;   /* 2 */

    /* Initiare cum dimensionibus minimis */
    tabula_initiare(&tabula, c->piscina, minima_latitudo,
        minima_altitudo);
    CREDO_AEQUALIS_I32(tabula.latitudo, minima_latitudo);
    CREDO_AEQUALIS_I32(tabula.altitudo, minima_altitudo);

    /* Inserere tab - requirit 2 cellas */
    tabula_inserere_tab(&tabula, ZEPHYRUM, ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM),
        (i32)'\t');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, I),
        (i32)TAB_CONTINUATIO);

    /* Inserere lineam - probare scroll */
    tabula_inserere_lineam(&tabula, ZEPHYRUM);
    /* Verificare quod inserere lineam non frangit */
    CREDO_AEQUALIS_I32(tabula.altitudo, minima_altitudo);
}

hic_manens vacuum
probans_dimensiones_asymmetricae(CredoContextus* c)
{
    TabulaCharacterum tabula;
                  i32 lata_latitudo    = C;   /* 100 - multa lata */
                  i32 brevis_altitudo  = III; /* 3 - paucae lineae */

    /* Initiare cum latitudine magna et altitudine parva */
    tabula_initiare(&tabula, c->piscina, lata_latitudo,
        brevis_altitudo);
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
    CREDO_AEQUALIS_S32(tabula_invenire_finem_contenti(&tabula,
        ZEPHYRUM), XLIX);

    /* Probare delere lineam cum paucis lineis */
    tabula_inserere_characterem(&tabula, I, ZEPHYRUM, 'A');
    tabula_inserere_characterem(&tabula, II, ZEPHYRUM, 'B');
    tabula_delere_lineam(&tabula, I);
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, ZEPHYRUM),
        (i32)'B');
}

hic_manens vacuum
probans_dimensiones_trudere_ad_limitem(CredoContextus* c)
{
    TabulaCharacterum tabula;
                  i32 latitudo = VIII;   /* 8 */
                  i32 altitudo = II;     /* 2 */
            character overflow;

    tabula_initiare(&tabula, c->piscina, latitudo, altitudo);

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
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM),
        (i32)' ');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, I),
        (i32)'a');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, VII),
        (i32)'g');
}


/* ==================================================
 * Main
 * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    probans_initiare(c);
}

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    probans_invenire_finem_contenti(c);
}

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    probans_invenire_initium_contenti(c);
}

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    probans_invenire_obicem(c);
}

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    probans_trudere_dextram(c);
}

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    probans_inserere_lineam(c);
}

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    probans_delere_lineam(c);
}

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    probans_inserere_characterem_simplex(c);
}

/* <tolera codex="CREDO_SECTIO_SINE_ASSERTIONE" (>in tabula_asserere */
interior vacuum
_sectio_nona (
    CredoContextus* c)
{
    probans_inserere_characterem_consumere_whitespace(c);
}

/* <tolera codex="CREDO_SECTIO_SINE_ASSERTIONE" (>in tabula_asserere */
interior vacuum
_sectio_decima (
    CredoContextus* c)
{
    probans_inserere_characterem_ad_barrier(c);
}

/* <tolera codex="CREDO_SECTIO_SINE_ASSERTIONE" (>in tabula_asserere */
interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
    probans_inserere_characterem_post_contentum(c);
}

interior vacuum
_sectio_decima_secunda (
    CredoContextus* c)
{
    probans_delere_characterem(c);
}

interior vacuum
_sectio_decima_tertia (
    CredoContextus* c)
{
    probans_ex_literis(c);
}

interior vacuum
_sectio_decima_quarta (
    CredoContextus* c)
{
    probans_aequalis_literis(c);
}

interior vacuum
_sectio_decima_quinta (
    CredoContextus* c)
{
    probans_inserere_tab_simplex(c);
}

interior vacuum
_sectio_decima_sexta (
    CredoContextus* c)
{
    probans_inserere_tab_ante_contentum(c);
}

interior vacuum
_sectio_decima_septima (
    CredoContextus* c)
{
    probans_inserere_tab_in_medio(c);
}

interior vacuum
_sectio_decima_octava (
    CredoContextus* c)
{
    probans_delere_tab(c);
}

interior vacuum
_sectio_decima_nona (
    CredoContextus* c)
{
    probans_delere_ad_tab_continuatio(c);
}

interior vacuum
_sectio_vicesima (
    CredoContextus* c)
{
    probans_est_tab_continuatio(c);
}

interior vacuum
_sectio_vicesima_prima (
    CredoContextus* c)
{
    probans_dimensiones_parvae(c);
}

interior vacuum
_sectio_vicesima_secunda (
    CredoContextus* c)
{
    probans_dimensiones_minimae(c);
}

interior vacuum
_sectio_vicesima_tertia (
    CredoContextus* c)
{
    probans_dimensiones_asymmetricae(c);
}

interior vacuum
_sectio_vicesima_quarta (
    CredoContextus* c)
{
    probans_dimensiones_trudere_ad_limitem(c);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "initiare", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "invenire_finem_contenti", _sectio_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "invenire_initium_contenti", _sectio_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "invenire_obicem", _sectio_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "trudere_dextram", _sectio_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "inserere_lineam", _sectio_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "delere_lineam", _sectio_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "inserere_characterem_simplex", _sectio_octava, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "inserere_characterem_consumere_whitespace", _sectio_nona, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "inserere_characterem_ad_barrier", _sectio_decima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "inserere_characterem_post_contentum", _sectio_decima_prima,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "delere_characterem", _sectio_decima_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "ex_literis", _sectio_decima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "aequalis_literis", _sectio_decima_quarta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "inserere_tab_simplex", _sectio_decima_quinta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "inserere_tab_ante_contentum", _sectio_decima_sexta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "inserere_tab_in_medio", _sectio_decima_septima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "delere_tab", _sectio_decima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "delere_ad_tab_continuatio", _sectio_decima_nona, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "est_tab_continuatio", _sectio_vicesima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "dimensiones_parvae", _sectio_vicesima_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "dimensiones_minimae", _sectio_vicesima_secunda, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "dimensiones_asymmetricae", _sectio_vicesima_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "dimensiones_trudere_ad_limitem", _sectio_vicesima_quarta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("tabula_characterum", SECTIONES);
}
