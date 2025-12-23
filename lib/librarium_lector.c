#include "librarium_lector.h"
#include "filum.h"
#include "entitas_repositorium.h"
#include "internamentum.h"
#include <string.h>
#include <stdio.h>

/* ==================================================
 * Constantae
 * ================================================== */

#define PADDING  II  /* 2 characteres padding */


/* ==================================================
 * Creatio
 * ================================================== */

LibrariumLector*
librarium_lector_creare(
    Piscina* piscina)
{
    LibrariumLector* lector;

    si (!piscina)
    {
        redde NIHIL;
    }

    lector = piscina_allocare(piscina, magnitudo(LibrariumLector));
    si (!lector)
    {
        redde NIHIL;
    }

    lector->piscina = piscina;
    lector->textus.datum = NIHIL;
    lector->textus.mensura = 0;
    lector->numerus_buffer[0] = '\0';
    lector->via_buffer[0] = '\0';
    lector->paginarium.paginae = NIHIL;
    lector->paginarium.numerus_paginarum = 0;
    lector->pagina_currens = 0;
    lector->paginae_totales = 0;
    lector->cache_latitudo = 0;
    lector->cache_altitudo = 0;
    lector->est_carcatus = FALSUM;

    redde lector;
}


/* ==================================================
 * Carcare
 * ================================================== */

b32
librarium_lector_carcare(
    LibrariumLector*    lector,
    constans character* via,
    constans character* numerus_libri)
{
    size_t len_via;
    size_t len_numerus;

    si (!lector || !via)
    {
        redde FALSUM;
    }

    /* Copiare via ad buffer */
    len_via = strlen(via);
    si (len_via >= LIBRARIUM_LECTOR_VIA_MAX)
    {
        len_via = LIBRARIUM_LECTOR_VIA_MAX - 1;
    }
    memcpy(lector->via_buffer, via, len_via);
    lector->via_buffer[len_via] = '\0';

    /* Copiare numerus ad buffer */
    si (numerus_libri)
    {
        len_numerus = strlen(numerus_libri);
        si (len_numerus >= LIBRARIUM_LECTOR_NUMERUS_MAX)
        {
            len_numerus = LIBRARIUM_LECTOR_NUMERUS_MAX - 1;
        }
        memcpy(lector->numerus_buffer, numerus_libri, len_numerus);
        lector->numerus_buffer[len_numerus] = '\0';
    }
    alioquin
    {
        lector->numerus_buffer[0] = '\0';
    }

    /* Legere filum */
    lector->textus = filum_legere_totum(lector->via_buffer, lector->piscina);

    si (lector->textus.mensura == 0)
    {
        lector->est_carcatus = FALSUM;
        redde FALSUM;
    }

    lector->est_carcatus = VERUM;
    lector->pagina_currens = 0;

    /* Invalidare cache */
    lector->cache_latitudo = 0;
    lector->cache_altitudo = 0;

    redde VERUM;
}


/* ==================================================
 * Paginatio
 * ================================================== */

vacuum
librarium_lector_paginare(
    LibrariumLector* lector,
    s32              latitudo,
    s32              altitudo)
{
    PaginariumConfig config;
    s32 chars_disponibiles;
    s32 lineae_disponibiles;

    si (!lector)
    {
        redde;
    }

    /* Verificare cache */
    si (lector->cache_latitudo == latitudo &&
        lector->cache_altitudo == altitudo &&
        lector->est_carcatus)
    {
        redde;
    }

    lector->cache_latitudo = latitudo;
    lector->cache_altitudo = altitudo;

    si (lector->textus.mensura == 0)
    {
        lector->paginae_totales = 0;
        redde;
    }

    chars_disponibiles = latitudo - (PADDING * II);
    lineae_disponibiles = altitudo - V;  /* Header et footer */

    si (chars_disponibiles < X)
    {
        chars_disponibiles = X;
    }
    si (lineae_disponibiles < V)
    {
        lineae_disponibiles = V;
    }

    /* Configurare paginarium */
    config = paginarium_config_defectus();
    config.latitudo = (i32)chars_disponibiles;
    config.altitudo = (i32)lineae_disponibiles;

    /* Paginare via paginarium */
    lector->paginarium = paginarium_paginare(
        lector->textus, config, lector->piscina);

    lector->paginae_totales = (s32)lector->paginarium.numerus_paginarum;
}


/* ==================================================
 * Navigatio
 * ================================================== */

vacuum
librarium_lector_pagina_proxima(
    LibrariumLector* lector)
{
    si (!lector)
    {
        redde;
    }

    si (lector->pagina_currens < lector->paginae_totales - 1)
    {
        lector->pagina_currens++;
    }
}

vacuum
librarium_lector_pagina_prior(
    LibrariumLector* lector)
{
    si (!lector)
    {
        redde;
    }

    si (lector->pagina_currens > 0)
    {
        lector->pagina_currens--;
    }
}

vacuum
librarium_lector_pagina_saltare(
    LibrariumLector* lector,
    s32              pagina)
{
    si (!lector)
    {
        redde;
    }

    si (pagina < 0)
    {
        pagina = 0;
    }
    si (pagina >= lector->paginae_totales)
    {
        pagina = lector->paginae_totales - 1;
        si (pagina < 0)
        {
            pagina = 0;
        }
    }

    lector->pagina_currens = pagina;
}


/* ==================================================
 * Accessores
 * ================================================== */

s32
librarium_lector_pagina_currens(
    LibrariumLector* lector)
{
    si (!lector)
    {
        redde 0;
    }
    redde lector->pagina_currens;
}

s32
librarium_lector_paginae_totales(
    LibrariumLector* lector)
{
    si (!lector)
    {
        redde 0;
    }
    redde lector->paginae_totales;
}

PaginariumPagina*
librarium_lector_pagina_obtinere(
    LibrariumLector* lector,
    s32              index)
{
    si (!lector)
    {
        redde NIHIL;
    }
    redde paginarium_pagina_obtinere(&lector->paginarium, (i32)index);
}

chorda
librarium_lector_textus(
    LibrariumLector* lector)
{
    chorda vacua;

    vacua.datum = NIHIL;
    vacua.mensura = 0;

    si (!lector)
    {
        redde vacua;
    }
    redde lector->textus;
}

b32
librarium_lector_habet_textum(
    LibrariumLector* lector)
{
    si (!lector)
    {
        redde FALSUM;
    }
    redde lector->est_carcatus && lector->textus.mensura > 0;
}

constans character*
librarium_lector_numerus(
    LibrariumLector* lector)
{
    si (!lector)
    {
        redde NIHIL;
    }
    redde lector->numerus_buffer;
}


/* ==================================================
 * Persistentia
 * ================================================== */

vacuum
librarium_lector_salvare_progressum(
    LibrariumLector*     lector,
    EntitasRepositorium* repo)
{
    Entitas* entitas;
    character pagina_buffer[XXXII];

    si (!lector || !repo)
    {
        redde;
    }

    si (lector->numerus_buffer[0] == '\0')
    {
        redde;
    }

    /* Scaffoldare entitas */
    entitas = repo->entitas_scaffoldare(
        repo->datum,
        "LibrumProgressus",
        lector->numerus_buffer);

    si (!entitas)
    {
        redde;
    }

    /* Ponere pagina_currens */
    sprintf(pagina_buffer, "%d", (int)lector->pagina_currens);
    repo->proprietas_ponere(
        repo->datum,
        entitas,
        "pagina_currens",
        pagina_buffer);
}

vacuum
librarium_lector_carcare_progressum(
    LibrariumLector*     lector,
    EntitasRepositorium* repo,
    InternamentumChorda* intern)
{
    Entitas* entitas;
    chorda* clavis;
    s32 pagina_salvata = 0;

    si (!lector || !repo || !intern)
    {
        redde;
    }

    si (lector->numerus_buffer[0] == '\0')
    {
        redde;
    }

    /* Invenire entitas (scaffoldare est idempotens) */
    entitas = repo->entitas_scaffoldare(
        repo->datum,
        "LibrumProgressus",
        lector->numerus_buffer);

    si (!entitas)
    {
        redde;
    }

    /* Capere pagina_currens */
    clavis = chorda_internare_ex_literis(intern, "pagina_currens");
    si (clavis && entitas_proprietas_capere_s32(entitas, clavis, &pagina_salvata))
    {
        lector->pagina_currens = pagina_salvata;
    }
}
