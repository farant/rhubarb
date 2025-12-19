#include "libro_paginarum.h"
#include "coloratio.h"
#include "delineare.h"
#include "thema.h"
#include "tempus.h"
#include "vim.h"
#include <stdio.h>

/* ==================================================
 * Utilitas Interna
 * ================================================== */

/* Allocare et initiare nova pagina */
hic_manens Pagina*
_allocare_paginam(
    LibroPaginarum* libro,
    i32 index)
{
    Pagina* pagina;
    character id_buffer[XXXII];
    chorda* identificator;

    /* Allocare Pagina ex piscina */
    pagina = (Pagina*)piscina_allocare(libro->ctx->piscina, magnitudo(Pagina));
    si (pagina == NIHIL)
    {
        redde NIHIL;
    }

    /* Creare identificator "pagina:N" */
    sprintf(id_buffer, "pagina:%d", index);
    identificator = chorda_internare_ex_literis(libro->ctx->intern, id_buffer);

    /* Initiare pagina */
    pagina_initiare(pagina, libro->ctx->piscina, identificator);

    /* Connectere clipboard et creare undo acervum */
    pagina_connectere_vim_contextu(pagina, libro->ctx->piscina, &libro->clipboard);

    /* Ponere reg_commandi in coloratio si habemus */
    si (libro->ctx->reg_commandi != NIHIL && pagina->coloratio != NIHIL)
    {
        coloratio_ponere_registrum(pagina->coloratio, libro->ctx->reg_commandi);
    }

    redde pagina;
}


/* Recordare pagina currens in historia */
hic_manens vacuum
_recordare_in_historia(
    LibroPaginarum* libro)
{
    si (libro->historia_numerus < LIBRO_HISTORIA_MAGNITUDO)
    {
        libro->historia[libro->historia_numerus] = libro->index_currens;
        libro->historia_numerus++;
    }
    alioquin
    {
        /* Historia plena - shift et addere */
        i32 i;
        per (i = ZEPHYRUM; i < LIBRO_HISTORIA_MAGNITUDO - I; i++)
        {
            libro->historia[i] = libro->historia[i + I];
        }
        libro->historia[LIBRO_HISTORIA_MAGNITUDO - I] = libro->index_currens;
    }
}


/* ==================================================
 * Creatio
 * ================================================== */

LibroPaginarum*
libro_creare(
    ContextusWidget* ctx)
{
    LibroPaginarum* libro;
    i32 i;

    si (ctx == NIHIL || ctx->piscina == NIHIL || ctx->intern == NIHIL)
    {
        redde NIHIL;
    }

    /* Allocare libro */
    libro = (LibroPaginarum*)piscina_allocare(ctx->piscina, magnitudo(LibroPaginarum));
    si (libro == NIHIL)
    {
        redde NIHIL;
    }

    /* Initiare campos */
    libro->ctx = ctx;
    libro->numerus_paginarum = ZEPHYRUM;
    libro->index_currens = ZEPHYRUM;
    libro->historia_numerus = ZEPHYRUM;
    libro->est_immundus = FALSUM;
    libro->tempus_ultimae_mutationis = 0.0;

    /* Initiare clipboard communicatum */
    vim_clipboard_initiare(&libro->clipboard);

    /* Vacare arrays */
    per (i = ZEPHYRUM; i < LIBRO_MAXIMUS_PAGINARUM; i++)
    {
        libro->paginae[i] = NIHIL;
        libro->nomina[i] = NIHIL;
        libro->entitas_ids[i] = NIHIL;
    }

    per (i = ZEPHYRUM; i < LIBRO_HISTORIA_MAGNITUDO; i++)
    {
        libro->historia[i] = ZEPHYRUM;
    }

    /* Creare primam paginam */
    libro->paginae[ZEPHYRUM] = _allocare_paginam(libro, ZEPHYRUM);
    si (libro->paginae[ZEPHYRUM] == NIHIL)
    {
        redde NIHIL;
    }
    libro->numerus_paginarum = I;

    redde libro;
}


/* ==================================================
 * Navigatio
 * ================================================== */

vacuum
libro_navigare_ad(
    LibroPaginarum* libro,
    s32             index)
{
    si (libro == NIHIL)
    {
        redde;
    }

    /* Verificare limites */
    si (index < ZEPHYRUM)
    {
        index = ZEPHYRUM;
    }
    si (index >= (s32)LIBRO_MAXIMUS_PAGINARUM)
    {
        index = (s32)LIBRO_MAXIMUS_PAGINARUM - I;
    }

    /* Si iam in hac pagina, nihil facere */
    si ((i32)index == libro->index_currens)
    {
        redde;
    }

    /* Auto-save pagina currens ante navigare */
    si (libro->ctx->repo != NIHIL)
    {
        libro_salvare_paginam(libro);
    }

    /* Recordare pagina currens in historia */
    _recordare_in_historia(libro);

    /* Creare paginas si necesse */
    dum (index >= (s32)libro->numerus_paginarum)
    {
        i32 nova_index;
        i32 prior_index;

        nova_index = libro->numerus_paginarum;
        libro->paginae[nova_index] = _allocare_paginam(libro, nova_index);

        si (libro->paginae[nova_index] == NIHIL)
        {
            /* Error - non possumus creare paginam */
            redde;
        }

        libro->numerus_paginarum++;

        /* Salvare novam paginam ad repository */
        si (libro->ctx->repo != NIHIL)
        {
            prior_index = libro->index_currens;
            libro->index_currens = nova_index;
            libro_salvare_paginam(libro);
            libro->index_currens = prior_index;
        }
    }

    /* Navigare */
    libro->index_currens = (i32)index;
}


vacuum
libro_pagina_proxima(
    LibroPaginarum* libro)
{
    si (libro == NIHIL)
    {
        redde;
    }

    libro_navigare_ad(libro, (s32)libro->index_currens + I);
}


vacuum
libro_pagina_prior(
    LibroPaginarum* libro)
{
    si (libro == NIHIL)
    {
        redde;
    }

    si (libro->index_currens > ZEPHYRUM)
    {
        libro_navigare_ad(libro, (s32)libro->index_currens - I);
    }
}


vacuum
libro_retro(
    LibroPaginarum* libro)
{
    si (libro == NIHIL)
    {
        redde;
    }

    si (libro->historia_numerus > ZEPHYRUM)
    {
        i32 prior_index;

        libro->historia_numerus--;
        prior_index = libro->historia[libro->historia_numerus];

        /* Navigare sine recordare (iam in historia) */
        si (prior_index >= ZEPHYRUM && prior_index < libro->numerus_paginarum)
        {
            libro->index_currens = prior_index;
        }
    }
}


b32
libro_navigare_ad_nomen(
    LibroPaginarum*     libro,
    constans character* nomen_)
{
    i32 i;
    chorda* nomen_chorda;

    si (libro == NIHIL || nomen_ == NIHIL)
    {
        redde FALSUM;
    }

    /* Internare nomen pro comparatione */
    nomen_chorda = chorda_internare_ex_literis(libro->ctx->intern, nomen_);

    /* Quaerere paginam cum nomine */
    per (i = ZEPHYRUM; i < libro->numerus_paginarum; i++)
    {
        si (libro->nomina[i] != NIHIL &&
            libro->nomina[i]->datum == nomen_chorda->datum)  /* Pointer comparison (interned) */
        {
            libro_navigare_ad(libro, (s32)i);
            redde VERUM;
        }
    }

    redde FALSUM;
}


/* ==================================================
 * Accessus
 * ================================================== */

Pagina*
libro_pagina_currens(
    LibroPaginarum* libro)
{
    si (libro == NIHIL)
    {
        redde NIHIL;
    }

    si (libro->index_currens < ZEPHYRUM ||
        libro->index_currens >= libro->numerus_paginarum)
    {
        redde NIHIL;
    }

    redde libro->paginae[libro->index_currens];
}


i32
libro_index_currens(
    LibroPaginarum* libro)
{
    si (libro == NIHIL)
    {
        redde ZEPHYRUM;
    }

    redde libro->index_currens;
}


i32
libro_numerus_paginarum(
    LibroPaginarum* libro)
{
    si (libro == NIHIL)
    {
        redde ZEPHYRUM;
    }

    redde libro->numerus_paginarum;
}


/* ==================================================
 * Gestio Paginarum
 * ================================================== */

s32
libro_pagina_nova(
    LibroPaginarum*     libro,
    constans character* nomen_)
{
    s32 nova_index;

    si (libro == NIHIL)
    {
        redde -I;
    }

    si (libro->numerus_paginarum >= LIBRO_MAXIMUS_PAGINARUM)
    {
        redde -I;  /* Liber plenus */
    }

    nova_index = (s32)libro->numerus_paginarum;

    /* Allocare paginam */
    libro->paginae[nova_index] = _allocare_paginam(libro, (i32)nova_index);
    si (libro->paginae[nova_index] == NIHIL)
    {
        redde -I;
    }

    /* Ponere nomen si datum */
    si (nomen_ != NIHIL)
    {
        libro->nomina[nova_index] = chorda_internare_ex_literis(libro->ctx->intern, nomen_);
    }

    libro->numerus_paginarum++;

    redde nova_index;
}


vacuum
libro_pagina_nominare(
    LibroPaginarum*     libro,
    i32                 index,
    constans character* nomen_)
{
    si (libro == NIHIL)
    {
        redde;
    }

    si (index < ZEPHYRUM || index >= libro->numerus_paginarum)
    {
        redde;
    }

    si (nomen_ == NIHIL)
    {
        libro->nomina[index] = NIHIL;
    }
    alioquin
    {
        libro->nomina[index] = chorda_internare_ex_literis(libro->ctx->intern, nomen_);
    }

    /* Salvare paginam ad repository */
    si (libro->ctx->repo != NIHIL)
    {
        i32 prior_index;
        prior_index = libro->index_currens;
        libro->index_currens = index;
        libro_salvare_paginam(libro);
        libro->index_currens = prior_index;
    }
}


chorda*
libro_pagina_nomen(
    LibroPaginarum* libro,
    i32             index)
{
    si (libro == NIHIL)
    {
        redde NIHIL;
    }

    si (index < ZEPHYRUM || index >= libro->numerus_paginarum)
    {
        redde NIHIL;
    }

    redde libro->nomina[index];
}


/* ==================================================
 * Eventus
 * ================================================== */

b32
libro_tractare_eventum(
    LibroPaginarum* libro,
    constans Eventus* eventus)
{
    Pagina* pagina;
    b32 resultum;

    si (libro == NIHIL || eventus == NIHIL)
    {
        redde VERUM;
    }

    pagina = libro_pagina_currens(libro);
    si (pagina == NIHIL)
    {
        redde VERUM;
    }

    /* Delegare ad pagina currens */
    resultum = pagina_tractare_eventum(pagina, eventus);

    /* Marcare immundum si tabula mutata (insert mode keys vel normal mode operations ut dd, p, u) */
    si (eventus->genus == EVENTUS_CLAVIS_DEPRESSUS && pagina->vim.mutatus)
    {
        libro_marcare_immundum(libro);
    }

    redde resultum;
}


/* ==================================================
 * Reddere
 * ================================================== */

vacuum
libro_reddere(
    Piscina*         piscina,
    TabulaPixelorum* tabula_pixelorum,
    LibroPaginarum*  libro,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused)
{
    Pagina* pagina;
    ContextusDelineandi* ctx;
    i32 character_latitudo;
    i32 character_altitudo;
    i32 box_x0, box_y0, box_x1, box_y1;
    i32 textus_latitudo;
    i32 textus_altitudo;
    Color color_border;
    Color color_border_inner;
    chorda titulo;
    chorda modo_textus;
    chorda pagina_indicium;
    constans character* modo_str;
    character indicium_buffer[XXXII];

    si (libro == NIHIL || tabula_pixelorum == NIHIL || piscina == NIHIL)
    {
        redde;
    }

    pagina = libro_pagina_currens(libro);
    si (pagina == NIHIL)
    {
        redde;
    }

    character_latitudo = VI * scala;
    character_altitudo = VIII * scala;

    /* Computare dimensiones areae textus (minus border) */
    textus_latitudo = latitudo - II;
    textus_altitudo = altitudo - II;

    /* Computare coordinatas pixelorum pro box */
    box_x0 = x * character_latitudo + II;
    box_y0 = y * character_altitudo + II;
    box_x1 = (x + latitudo) * character_latitudo - I - II;
    box_y1 = (y + altitudo) * character_altitudo - I - II;

    color_border = thema_color(COLOR_BORDER);
    color_border_inner = focused ? thema_color(COLOR_BORDER_ACTIVE) : color_border;

    /* Creare contextum delineandi */
    ctx = delineare_creare_contextum(piscina, tabula_pixelorum);

    /* Computare titulo - nomen paginae vel "Pagina N/M" */
    si (libro->nomina[libro->index_currens] != NIHIL)
    {
        sprintf(indicium_buffer, "%.*s %d/%d",
                (int)libro->nomina[libro->index_currens]->mensura,
                libro->nomina[libro->index_currens]->datum,
                libro->index_currens + I,
                libro->numerus_paginarum);
        titulo = chorda_ex_literis(indicium_buffer, piscina);
    }
    alioquin
    {
        sprintf(indicium_buffer, "Pagina %d/%d",
                libro->index_currens + I,
                libro->numerus_paginarum);
        titulo = chorda_ex_literis(indicium_buffer, piscina);
    }

    /* Modo textus */
    modo_str = vim_nomen_modi(pagina->vim.modo);
    modo_textus = chorda_ex_literis(modo_str, piscina);

    /* Pagina indicium pro angulo dextro */
    sprintf(indicium_buffer, "%d/%d",
            libro->index_currens + I,
            libro->numerus_paginarum);
    pagina_indicium = chorda_ex_literis(indicium_buffer, piscina);

    {
        i32 titulo_pixel_width;
        i32 modo_pixel_width;
        i32 indicium_pixel_width;
        i32 titulo_x, titulo_y;
        i32 modo_x, modo_y;
        i32 indicium_x, indicium_y;
        i32 gap_padding;

        gap_padding = character_latitudo;

        titulo_pixel_width = titulo.mensura * character_latitudo;
        modo_pixel_width = modo_textus.mensura * character_latitudo;
        indicium_pixel_width = pagina_indicium.mensura * character_latitudo;

        /* Center titulo on top line */
        titulo_x = box_x0 + ((box_x1 - box_x0) - titulo_pixel_width) / II;
        titulo_y = box_y0 - (character_altitudo / II) + III;

        /* Center modo on bottom line */
        modo_x = box_x0 + ((box_x1 - box_x0) - modo_pixel_width) / II;
        modo_y = box_y1 - (character_altitudo / II) + II;

        /* Pagina indicium in angulo dextro superiore */
        indicium_x = box_x1 - indicium_pixel_width - gap_padding;
        indicium_y = titulo_y;

        /* Pingere Double Border */

        /* Top line - left segment */
        delineare_lineam_horizontalem(ctx, box_x0, titulo_x - gap_padding, box_y0, color_border);
        delineare_lineam_horizontalem(ctx, box_x0, titulo_x - gap_padding, box_y0 + II, color_border_inner);

        /* Top line - right segment */
        delineare_lineam_horizontalem(ctx, titulo_x + titulo_pixel_width + gap_padding, box_x1, box_y0, color_border);
        delineare_lineam_horizontalem(ctx, titulo_x + titulo_pixel_width + gap_padding, box_x1, box_y0 + II, color_border_inner);

        /* Bottom line - left segment */
        delineare_lineam_horizontalem(ctx, box_x0, modo_x - gap_padding, box_y1, color_border);
        delineare_lineam_horizontalem(ctx, box_x0, modo_x - gap_padding, box_y1 - II, color_border_inner);

        /* Bottom line - right segment */
        delineare_lineam_horizontalem(ctx, modo_x + modo_pixel_width + gap_padding, box_x1, box_y1, color_border);
        delineare_lineam_horizontalem(ctx, modo_x + modo_pixel_width + gap_padding, box_x1, box_y1 - II, color_border_inner);

        /* Left line */
        delineare_lineam_verticalem(ctx, box_x0, box_y0, box_y1, color_border);
        delineare_lineam_verticalem(ctx, box_x0 + II, box_y0, box_y1, color_border_inner);

        /* Right line */
        delineare_lineam_verticalem(ctx, box_x1, box_y0, box_y1, color_border);
        delineare_lineam_verticalem(ctx, box_x1 - II, box_y0, box_y1, color_border_inner);

        /* Pingere Textum */
        tabula_pixelorum_pingere_chordam(tabula_pixelorum, titulo_x, titulo_y, titulo, color_ad_pixelum(color_border));
        tabula_pixelorum_pingere_chordam(tabula_pixelorum, modo_x, modo_y, modo_textus,
                                          (pagina->vim.modo == MODO_VIM_INSERERE) ?
                                              color_ad_pixelum(thema_color(COLOR_STATUS_INSERT)) :
                                              color_ad_pixelum(thema_color(COLOR_STATUS_NORMAL)));

        /* Pingere pagina indicium si nomen est monstratum (non duplicare) */
        si (libro->nomina[libro->index_currens] != NIHIL)
        {
            tabula_pixelorum_pingere_chordam(tabula_pixelorum, indicium_x, indicium_y, pagina_indicium,
                                              color_ad_pixelum(color_border));
        }
    }

    /* Reddere Textum Paginae (interior) */
    pagina_reddere(tabula_pixelorum, pagina, x + I, y + I, textus_latitudo, textus_altitudo, scala);
}


/* ==================================================
 * Serialization
 * ================================================== */

/* Serializa TabulaCharacterum ad chordam
 * Format: lineae separatae per '\n', trailing spaces removentur
 *
 * Redde: chorda* allocata ex piscina, vel NIHIL si error
 */
hic_manens chorda*
_serializa_tabula(
    Piscina*           piscina,
    TabulaCharacterum* tabula)
{
    i8* buffer;
    i32 buffer_capacitas;
    i32 buffer_longitudo;
    s32 linea;
    s32 columna;
    s32 finis_contenti;
    s32 altitudo_s;
    chorda* resultum;

    si (piscina == NIHIL || tabula == NIHIL)
    {
        redde NIHIL;
    }

    altitudo_s = (s32)tabula->altitudo;

    /* Allocare buffer maximum possibile */
    buffer_capacitas = tabula->latitudo * tabula->altitudo + tabula->altitudo + I;
    buffer = (i8*)piscina_allocare(piscina, (i32)buffer_capacitas);
    si (buffer == NIHIL)
    {
        redde NIHIL;
    }

    buffer_longitudo = ZEPHYRUM;

    /* Serializa linea per lineam */
    per (linea = ZEPHYRUM; linea < altitudo_s; linea++)
    {
        /* Invenire finem contenti in linea */
        finis_contenti = tabula_invenire_finem_contenti(tabula, (i32)linea);

        /* Scribere characteres usque ad finem contenti */
        per (columna = ZEPHYRUM; columna <= finis_contenti; columna++)
        {
            character c;

            c = tabula_cellula(tabula, (i32)linea, (i32)columna);

            /* Convertere TAB_CONTINUATIO ad spatium */
            si (c == TAB_CONTINUATIO)
            {
                c = ' ';
            }
            /* Convertere '\0' ad spatium (si ante finem contenti) */
            alioquin si (c == '\0')
            {
                c = ' ';
            }

            buffer[buffer_longitudo] = (i8)c;
            buffer_longitudo++;
        }

        /* Addere newline (nisi ultima linea vacua) */
        si (finis_contenti >= ZEPHYRUM || linea < altitudo_s - I)
        {
            /* Non addere newline post ultimam lineam vacuam */
            si (linea < altitudo_s - I)
            {
                buffer[buffer_longitudo] = '\n';
                buffer_longitudo++;
            }
            alioquin si (finis_contenti >= ZEPHYRUM)
            {
                /* Ultima linea habet contentum - addere newline */
                buffer[buffer_longitudo] = '\n';
                buffer_longitudo++;
            }
        }
    }

    /* Creare chorda ex buffer */
    resultum = (chorda*)piscina_allocare(piscina, magnitudo(chorda));
    si (resultum == NIHIL)
    {
        redde NIHIL;
    }

    resultum->datum = buffer;
    resultum->mensura = (i32)buffer_longitudo;

    redde resultum;
}


/* Deserializa chordam ad TabulaCharacterum
 * Format: lineae separatae per '\n'
 */
hic_manens vacuum
_deserializa_tabula(
    TabulaCharacterum* tabula,
    chorda*            contentum)
{
    i32 i;
    s32 linea;
    s32 columna;
    s32 altitudo_s;
    s32 latitudo_s;

    si (tabula == NIHIL || contentum == NIHIL)
    {
        redde;
    }

    altitudo_s = (s32)tabula->altitudo;
    latitudo_s = (s32)tabula->latitudo;

    /* Vacare tabulam */
    per (i = ZEPHYRUM; i < tabula->latitudo * tabula->altitudo; i++)
    {
        tabula->cellulae[i] = '\0';
    }

    linea = ZEPHYRUM;
    columna = ZEPHYRUM;

    per (i = ZEPHYRUM; i < (i32)contentum->mensura; i++)
    {
        character c;

        c = (character)contentum->datum[i];

        si (c == '\n')
        {
            linea++;
            columna = ZEPHYRUM;

            si (linea >= altitudo_s)
            {
                frange;
            }
        }
        alioquin
        {
            si (columna < latitudo_s && linea < altitudo_s)
            {
                tabula_cellula(tabula, (i32)linea, (i32)columna) = c;
                columna++;
            }
        }
    }
}


/* Generare titulum scaffold pro pagina
 * Semper usare "pagina-N" pro stabili identitate
 */
hic_manens vacuum
_generare_scaffold_titulum(
    character* buffer,
    i32        index)
{
    sprintf(buffer, "pagina-%d", index);
}


b32
libro_salvare_paginam(
    LibroPaginarum* libro)
{
    Entitas* entitas;
    Pagina* pagina;
    chorda* contentum;
    character scaffold_titulus[XXXII];
    character ordo_buffer[XVI];
    i32 idx;

    si (libro == NIHIL || libro->ctx->repo == NIHIL)
    {
        redde FALSUM;
    }

    idx = libro->index_currens;

    pagina = libro_pagina_currens(libro);
    si (pagina == NIHIL)
    {
        redde FALSUM;
    }

    /* Si iam habemus entity ID, usare capere_entitatem */
    si (libro->entitas_ids[idx] != NIHIL)
    {
        entitas = libro->ctx->repo->capere_entitatem(
            libro->ctx->repo->datum,
            libro->entitas_ids[idx]);
    }
    alioquin
    {
        /* Prima vice: scaffoldare et memorare ID */
        _generare_scaffold_titulum(scaffold_titulus, idx);

        entitas = libro->ctx->repo->entitas_scaffoldare(
            libro->ctx->repo->datum,
            "LibroPagina",
            scaffold_titulus);

        si (entitas != NIHIL)
        {
            /* Memorare entity ID */
            libro->entitas_ids[idx] = entitas->id;
            /* Addere nota pro quaerere in libro_carcare */
            libro->ctx->repo->nota_addere(libro->ctx->repo->datum, entitas, "#LibroPagina");
        }
    }

    si (entitas == NIHIL)
    {
        redde FALSUM;
    }

    /* Serializa contentum */
    contentum = _serializa_tabula(libro->ctx->piscina, &pagina->tabula);
    si (contentum == NIHIL)
    {
        redde FALSUM;
    }

    /* Ponere proprietas "ordo" */
    sprintf(ordo_buffer, "%d", idx);
    libro->ctx->repo->proprietas_ponere(
        libro->ctx->repo->datum,
        entitas,
        "ordo",
        ordo_buffer);

    /* Ponere proprietas "name" et "titulus" */
    si (libro->nomina[idx] != NIHIL)
    {
        /* Usare nomen custom */
        character* titulus_buffer;
        chorda* nomen_chorda;

        nomen_chorda = libro->nomina[idx];
        titulus_buffer = (character*)piscina_allocare(
            libro->ctx->piscina,
            (i32)nomen_chorda->mensura + I);

        si (titulus_buffer != NIHIL)
        {
            i32 j;
            per (j = ZEPHYRUM; j < (i32)nomen_chorda->mensura; j++)
            {
                titulus_buffer[j] = (character)nomen_chorda->datum[j];
            }
            titulus_buffer[nomen_chorda->mensura] = '\0';

            libro->ctx->repo->proprietas_ponere(
                libro->ctx->repo->datum,
                entitas,
                "name",
                titulus_buffer);

            libro->ctx->repo->proprietas_ponere(
                libro->ctx->repo->datum,
                entitas,
                "titulus",
                titulus_buffer);
        }
    }
    alioquin
    {
        /* Usare "pagina-N" si nullum nomen */
        _generare_scaffold_titulum(scaffold_titulus, idx);
        libro->ctx->repo->proprietas_ponere(
            libro->ctx->repo->datum,
            entitas,
            "name",
            scaffold_titulus);
    }

    /* Ponere proprietas "contentum" */
    {
        character* contentum_buffer;

        contentum_buffer = (character*)piscina_allocare(
            libro->ctx->piscina,
            (i32)contentum->mensura + I);

        si (contentum_buffer != NIHIL)
        {
            i32 j;
            per (j = ZEPHYRUM; j < (i32)contentum->mensura; j++)
            {
                contentum_buffer[j] = (character)contentum->datum[j];
            }
            contentum_buffer[contentum->mensura] = '\0';

            libro->ctx->repo->proprietas_ponere(
                libro->ctx->repo->datum,
                entitas,
                "contentum",
                contentum_buffer);
        }
    }

    redde VERUM;
}


b32
libro_salvare_omnes(
    LibroPaginarum* libro)
{
    i32 i;
    i32 prior_index;
    b32 successus;

    si (libro == NIHIL || libro->ctx->repo == NIHIL)
    {
        redde FALSUM;
    }

    prior_index = libro->index_currens;
    successus = VERUM;

    per (i = ZEPHYRUM; i < libro->numerus_paginarum; i++)
    {
        libro->index_currens = i;

        si (!libro_salvare_paginam(libro))
        {
            successus = FALSUM;
        }
    }

    libro->index_currens = prior_index;

    redde successus;
}


b32
libro_carcare(
    LibroPaginarum* libro)
{
    Xar* entitates;
    i32 i;
    i32 num_entitates;
    s32 maximus_ordo;

    si (libro == NIHIL || libro->ctx->repo == NIHIL)
    {
        redde FALSUM;
    }

    /* Quaerere omnes LibroPagina entitates */
    entitates = libro->ctx->repo->quaerere_cum_praefixo_notae(
        libro->ctx->repo->datum,
        "#LibroPagina");

    /* Si nullae entitates, nihil carcare */
    si (entitates == NIHIL || xar_numerus(entitates) == ZEPHYRUM)
    {
        redde VERUM;
    }

    num_entitates = xar_numerus(entitates);

    /* Invenire maximum ordo */
    maximus_ordo = -I;
    per (i = ZEPHYRUM; i < num_entitates; i++)
    {
        Entitas** entitas_ptr;
        Entitas* entitas;
        chorda* ordo_valor;
        s32 ordo;

        entitas_ptr = (Entitas**)xar_obtinere(entitates, i);
        si (entitas_ptr == NIHIL)
        {
            perge;
        }
        entitas = *entitas_ptr;
        si (entitas == NIHIL)
        {
            perge;
        }

        ordo_valor = entitas_proprietas_capere(
            entitas,
            chorda_internare_ex_literis(libro->ctx->intern, "ordo"));

        si (ordo_valor != NIHIL)
        {
            /* Convertere chorda ad integer */
            ordo = ZEPHYRUM;
            {
                i32 j;
                per (j = ZEPHYRUM; j < (i32)ordo_valor->mensura; j++)
                {
                    character c;
                    c = (character)ordo_valor->datum[j];
                    si (c >= '0' && c <= '9')
                    {
                        ordo = ordo * X + (s32)(c - '0');
                    }
                }
            }

            si (ordo > maximus_ordo)
            {
                maximus_ordo = ordo;
            }
        }
    }

    /* Creare paginas si necesse */
    dum ((s32)libro->numerus_paginarum <= maximus_ordo)
    {
        i32 nova_index;

        nova_index = libro->numerus_paginarum;
        libro->paginae[nova_index] = _allocare_paginam(libro, nova_index);

        si (libro->paginae[nova_index] == NIHIL)
        {
            redde FALSUM;
        }

        libro->numerus_paginarum++;
    }

    /* Carcare contentum in paginas */
    per (i = ZEPHYRUM; i < num_entitates; i++)
    {
        Entitas** entitas_ptr;
        Entitas* entitas;
        chorda* ordo_valor;
        chorda* titulus_valor;
        chorda* contentum_valor;
        s32 ordo;

        entitas_ptr = (Entitas**)xar_obtinere(entitates, i);
        si (entitas_ptr == NIHIL)
        {
            perge;
        }
        entitas = *entitas_ptr;
        si (entitas == NIHIL)
        {
            perge;
        }

        /* Obtinere ordo */
        ordo_valor = entitas_proprietas_capere(
            entitas,
            chorda_internare_ex_literis(libro->ctx->intern, "ordo"));

        si (ordo_valor == NIHIL)
        {
            perge;
        }

        /* Convertere ad integer */
        ordo = ZEPHYRUM;
        {
            i32 j;
            per (j = ZEPHYRUM; j < (i32)ordo_valor->mensura; j++)
            {
                character c;
                c = (character)ordo_valor->datum[j];
                si (c >= '0' && c <= '9')
                {
                    ordo = ordo * X + (s32)(c - '0');
                }
            }
        }

        si (ordo < ZEPHYRUM || ordo >= (s32)libro->numerus_paginarum)
        {
            perge;
        }

        /* Memorare entity ID pro futurae salvationes */
        libro->entitas_ids[ordo] = entitas->id;

        /* Carcare titulus */
        titulus_valor = entitas_proprietas_capere(
            entitas,
            chorda_internare_ex_literis(libro->ctx->intern, "titulus"));

        si (titulus_valor != NIHIL)
        {
            libro->nomina[ordo] = titulus_valor;
        }

        /* Carcare contentum */
        contentum_valor = entitas_proprietas_capere(
            entitas,
            chorda_internare_ex_literis(libro->ctx->intern, "contentum"));

        si (contentum_valor != NIHIL && libro->paginae[ordo] != NIHIL)
        {
            _deserializa_tabula(&libro->paginae[ordo]->tabula, contentum_valor);
        }
    }

    redde VERUM;
}


/* ==================================================
 * Debounced Save
 * ================================================== */

vacuum
libro_marcare_immundum(
    LibroPaginarum* libro)
{
    si (libro == NIHIL)
    {
        redde;
    }

    libro->est_immundus = VERUM;
    libro->tempus_ultimae_mutationis = tempus_nunc();
}


vacuum
libro_salvare_si_immundum(
    LibroPaginarum* libro)
{
    f64 tempus_currens;
    f64 intervallum;

    si (libro == NIHIL || !libro->est_immundus || libro->ctx->repo == NIHIL)
    {
        redde;
    }

    tempus_currens = tempus_nunc();
    intervallum = tempus_currens - libro->tempus_ultimae_mutationis;

    /* 2 secundae debounce */
    si (intervallum >= 2.0)
    {
        libro_salvare_paginam(libro);
        libro->est_immundus = FALSUM;
    }
}
