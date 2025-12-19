/* probatio_libro.c - Probationes LibroPaginarum */
#include "latina.h"
#include "libro_paginarum.h"
#include "piscina.h"
#include "internamentum.h"
#include "widget.h"
#include "credo.h"
#include <stdio.h>

/* Piscina globalis pro probationes */
hic_manens Piscina* g_piscina = NIHIL;
hic_manens InternamentumChorda* g_intern = NIHIL;
hic_manens ContextusWidget* g_ctx = NIHIL;


/* ==================================================
 * Test: Creatio
 * ================================================== */

hic_manens vacuum
probans_libro_creare(vacuum)
{
    LibroPaginarum* libro;

    libro = libro_creare(g_ctx);

    CREDO_NON_NIHIL(libro);
    CREDO_AEQUALIS_I32(libro_numerus_paginarum(libro), I);
    CREDO_AEQUALIS_I32(libro_index_currens(libro), ZEPHYRUM);
    CREDO_NON_NIHIL(libro_pagina_currens(libro));
}


/* ==================================================
 * Test: Navigatio
 * ================================================== */

hic_manens vacuum
probans_libro_navigare_ad(vacuum)
{
    LibroPaginarum* libro;

    libro = libro_creare(g_ctx);
    CREDO_NON_NIHIL(libro);

    /* Navigare ad paginam 2 (creat paginas 1 et 2) */
    libro_navigare_ad(libro, II);

    CREDO_AEQUALIS_I32(libro_index_currens(libro), II);
    CREDO_AEQUALIS_I32(libro_numerus_paginarum(libro), III);
    CREDO_NON_NIHIL(libro_pagina_currens(libro));
}

hic_manens vacuum
probans_libro_pagina_proxima(vacuum)
{
    LibroPaginarum* libro;

    libro = libro_creare(g_ctx);
    CREDO_NON_NIHIL(libro);

    /* pagina_proxima creat novam paginam */
    libro_pagina_proxima(libro);

    CREDO_AEQUALIS_I32(libro_index_currens(libro), I);
    CREDO_AEQUALIS_I32(libro_numerus_paginarum(libro), II);
}

hic_manens vacuum
probans_libro_pagina_prior(vacuum)
{
    LibroPaginarum* libro;

    libro = libro_creare(g_ctx);
    CREDO_NON_NIHIL(libro);

    /* Navigare ad paginam 2 */
    libro_navigare_ad(libro, II);
    CREDO_AEQUALIS_I32(libro_index_currens(libro), II);

    /* pagina_prior */
    libro_pagina_prior(libro);

    CREDO_AEQUALIS_I32(libro_index_currens(libro), I);
}

hic_manens vacuum
probans_libro_pagina_prior_ad_initium(vacuum)
{
    LibroPaginarum* libro;

    libro = libro_creare(g_ctx);
    CREDO_NON_NIHIL(libro);

    /* pagina_prior ad initium - non debet movere */
    libro_pagina_prior(libro);

    CREDO_AEQUALIS_I32(libro_index_currens(libro), ZEPHYRUM);
}


/* ==================================================
 * Test: Historia
 * ================================================== */

hic_manens vacuum
probans_libro_historia(vacuum)
{
    LibroPaginarum* libro;

    libro = libro_creare(g_ctx);
    CREDO_NON_NIHIL(libro);

    /* Navigare: 0 -> 2 -> 5 */
    libro_navigare_ad(libro, II);
    libro_navigare_ad(libro, V);

    CREDO_AEQUALIS_I32(libro_index_currens(libro), V);

    /* Retro: 5 -> 2 */
    libro_retro(libro);
    CREDO_AEQUALIS_I32(libro_index_currens(libro), II);

    /* Retro: 2 -> 0 */
    libro_retro(libro);
    CREDO_AEQUALIS_I32(libro_index_currens(libro), ZEPHYRUM);

    /* Retro ad initium - non debet movere */
    libro_retro(libro);
    CREDO_AEQUALIS_I32(libro_index_currens(libro), ZEPHYRUM);
}


/* ==================================================
 * Test: Nomina
 * ================================================== */

hic_manens vacuum
probans_libro_nominare_pagina(vacuum)
{
    LibroPaginarum* libro;
    chorda* titulus;

    libro = libro_creare(g_ctx);
    CREDO_NON_NIHIL(libro);

    /* Nominare paginam 0 */
    libro_pagina_nominare(libro, ZEPHYRUM, "Index");

    titulus = libro_pagina_nomen(libro, ZEPHYRUM);
    CREDO_NON_NIHIL(titulus);
}

hic_manens vacuum
probans_libro_navigare_ad_nomen(vacuum)
{
    LibroPaginarum* libro;

    libro = libro_creare(g_ctx);
    CREDO_NON_NIHIL(libro);

    /* Creare paginas cum nominibus */
    libro_pagina_nova(libro, "Alpha");
    libro_pagina_nova(libro, "Beta");
    libro_pagina_nova(libro, "Gamma");

    /* Navigare ad nomen */
    CREDO_VERUM(libro_navigare_ad_nomen(libro, "Beta"));
    CREDO_AEQUALIS_I32(libro_index_currens(libro), II);  /* Beta est pagina 2 (0=prima, 1=Alpha, 2=Beta) */

    /* Navigare ad nomen non existens */
    CREDO_FALSUM(libro_navigare_ad_nomen(libro, "Delta"));
}


/* ==================================================
 * Test: Pagina Nova
 * ================================================== */

hic_manens vacuum
probans_libro_pagina_nova(vacuum)
{
    LibroPaginarum* libro;
    s32 index;

    libro = libro_creare(g_ctx);
    CREDO_NON_NIHIL(libro);

    /* Creare novam paginam */
    index = libro_pagina_nova(libro, "Nova Pagina");

    CREDO_AEQUALIS_I32((i32)index, I);
    CREDO_AEQUALIS_I32(libro_numerus_paginarum(libro), II);
    CREDO_NON_NIHIL(libro_pagina_nomen(libro, I));
}


/* ==================================================
 * Test: Pagina Currens
 * ================================================== */

hic_manens vacuum
probans_libro_pagina_currens(vacuum)
{
    LibroPaginarum* libro;
    Pagina* pagina;

    libro = libro_creare(g_ctx);
    CREDO_NON_NIHIL(libro);

    pagina = libro_pagina_currens(libro);
    CREDO_NON_NIHIL(pagina);

    /* Verificare pagina habet identificator */
    CREDO_NON_NIHIL(pagina->identificator);
}


/* ==================================================
 * Test: Limites
 * ================================================== */

hic_manens vacuum
probans_libro_limites_index(vacuum)
{
    LibroPaginarum* libro;

    libro = libro_creare(g_ctx);
    CREDO_NON_NIHIL(libro);

    /* Index negativus */
    libro_navigare_ad(libro, -V);
    CREDO_AEQUALIS_I32(libro_index_currens(libro), ZEPHYRUM);

    /* Index maior quam maximum */
    libro_navigare_ad(libro, LIBRO_MAXIMUS_PAGINARUM + X);
    CREDO_AEQUALIS_I32(libro_index_currens(libro), LIBRO_MAXIMUS_PAGINARUM - I);
}


/* ==================================================
 * Main
 * ================================================== */

integer
principale(vacuum)
{
    Piscina* piscina;
    b32 praeteritus;

    piscina = piscina_generare_dynamicum("test_libro", M * II);  /* 2MB */
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    g_piscina = piscina;
    g_intern = internamentum_creare(piscina);
    g_ctx = contextus_widget_creare(piscina, g_intern, NIHIL, NIHIL);

    credo_aperire(piscina);

    printf("--- Probans libro_creare ---\n");
    probans_libro_creare();

    printf("--- Probans libro_navigare_ad ---\n");
    probans_libro_navigare_ad();

    printf("--- Probans libro_pagina_proxima ---\n");
    probans_libro_pagina_proxima();

    printf("--- Probans libro_pagina_prior ---\n");
    probans_libro_pagina_prior();

    printf("--- Probans libro_pagina_prior_ad_initium ---\n");
    probans_libro_pagina_prior_ad_initium();

    printf("--- Probans libro_historia ---\n");
    probans_libro_historia();

    printf("--- Probans libro_nominare_pagina ---\n");
    probans_libro_nominare_pagina();

    printf("--- Probans libro_navigare_ad_nomen ---\n");
    probans_libro_navigare_ad_nomen();

    printf("--- Probans libro_pagina_nova ---\n");
    probans_libro_pagina_nova();

    printf("--- Probans libro_pagina_currens ---\n");
    probans_libro_pagina_currens();

    printf("--- Probans libro_limites_index ---\n");
    probans_libro_limites_index();

    printf("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

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
