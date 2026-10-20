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
               s32  index;

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
    CREDO_AEQUALIS_I32(libro_index_currens(libro),
        LIBRO_MAXIMUS_PAGINARUM - I);
}


/* ==================================================
 * Sectiones (credo v2)
 * ================================================== */

/* contextus widget in piscina sectionis propria (olim semel in
 * principale) */
interior vacuum
_parare (
    CredoContextus* c)
{
    g_piscina  = c->piscina;
    g_intern   = internamentum_creare(c->piscina);
    g_ctx     = contextus_widget_creare(c->piscina, g_intern, NIHIL,
        NIHIL, NIHIL, NIHIL, NIHIL);
}

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    (vacuum)c;
    probans_libro_creare();
}

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    (vacuum)c;
    probans_libro_navigare_ad();
}

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    (vacuum)c;
    probans_libro_pagina_proxima();
}

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    (vacuum)c;
    probans_libro_pagina_prior();
}

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    (vacuum)c;
    probans_libro_pagina_prior_ad_initium();
}

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    (vacuum)c;
    probans_libro_historia();
}

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    (vacuum)c;
    probans_libro_nominare_pagina();
}

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    (vacuum)c;
    probans_libro_navigare_ad_nomen();
}

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
    (vacuum)c;
    probans_libro_pagina_nova();
}

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
    (vacuum)c;
    probans_libro_pagina_currens();
}

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
    (vacuum)c;
    probans_libro_limites_index();
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "libro_creare", _sectio_prima, _parare, NIHIL, NIHIL, NIHIL },
    { "libro_navigare_ad", _sectio_secunda, _parare, NIHIL, NIHIL,
        NIHIL },
    { "libro_pagina_proxima", _sectio_tertia, _parare, NIHIL, NIHIL,
        NIHIL },
    { "libro_pagina_prior", _sectio_quarta, _parare, NIHIL, NIHIL,
        NIHIL },
    { "libro_pagina_prior_ad_initium", _sectio_quinta, _parare, NIHIL,
        NIHIL, NIHIL },
    { "libro_historia", _sectio_sexta, _parare, NIHIL, NIHIL, NIHIL },
    { "libro_nominare_pagina", _sectio_septima, _parare, NIHIL, NIHIL,
        NIHIL },
    { "libro_navigare_ad_nomen", _sectio_octava, _parare, NIHIL, NIHIL,
        NIHIL },
    { "libro_pagina_nova", _sectio_nona, _parare, NIHIL, NIHIL, NIHIL },
    { "libro_pagina_currens", _sectio_decima, _parare, NIHIL, NIHIL,
        NIHIL },
    { "libro_limites_index", _sectio_decima_prima, _parare, NIHIL,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("libro", SECTIONES);
}
