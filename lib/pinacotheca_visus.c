/*
 * pinacotheca_visus.c - Gallery Widget Implementation
 *
 * Modus browser: lista imaginum + preview
 * Modus visus: plena imago centrata
 */

#include "pinacotheca_visus.h"
#include "dithering.h"
#include "thema.h"
#include "color.h"
#include "registrum_commandi.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ============================================================
 * Functiones Internae
 * ============================================================ */

/* Comparator pro sortare alphabetice per titulum */
hic_manens s32
_comparare_imagines(constans vacuum* a, constans vacuum* b)
{
    constans ImagoInfo* ia;
    constans ImagoInfo* ib;

    ia = (constans ImagoInfo*)a;
    ib = (constans ImagoInfo*)b;

    si (ia->titulus.datum == NIHIL && ib->titulus.datum == NIHIL)
    {
        redde 0;
    }
    si (ia->titulus.datum == NIHIL)
    {
        redde 1;
    }
    si (ib->titulus.datum == NIHIL)
    {
        redde -1;
    }

    /* Compare by titulus */
    {
        memoriae_index len_a, len_b, min_len;
        s32 cmp;

        len_a = ia->titulus.mensura;
        len_b = ib->titulus.mensura;
        min_len = len_a < len_b ? len_a : len_b;

        cmp = memcmp(ia->titulus.datum, ib->titulus.datum, min_len);
        si (cmp != 0)
        {
            redde cmp;
        }
        redde (s32)(len_a - len_b);
    }
}

/* Carcare et cache-are imaginem selectam */
hic_manens vacuum
_carcare_imaginem_selectam(PinacothecaVisus* visus)
{
    ImagoInfo* info;
    Entitas* entitas;
    chorda* clavis_datum;
    i32 mensura_blob;

    si (visus->imagines == NIHIL ||
        visus->index_selecta < 0 ||
        (i32)visus->index_selecta >= xar_numerus(visus->imagines))
    {
        visus->datum_cache = NIHIL;
        visus->cache_latitudo = 0;
        visus->cache_altitudo = 0;
        redde;
    }

    info = (ImagoInfo*)xar_obtinere(visus->imagines, (i32)visus->index_selecta);
    si (info == NIHIL)
    {
        redde;
    }

    /* Verificare si iam in cache */
    si (visus->cache_id.datum != NIHIL &&
        chorda_aequalis(visus->cache_id, info->id))
    {
        redde;
    }

    /* Capere entitatem - uti entitatem iam in imagines lista */
    /* Non opus est capere iterum - iam habemus ex quaerere_cum_genere */
    entitas = visus->ctx->repo->capere_entitatem(
        visus->ctx->repo->datum,
        &info->id);

    si (entitas == NIHIL)
    {
        visus->datum_cache = NIHIL;
        redde;
    }

    /* Capere blobum - clavis debet esse internata (pointer comparison!) */
    clavis_datum = chorda_internare_ex_literis(internamentum_globale(), "datum");

    si (entitas_proprietas_capere_blobum(
            entitas,
            clavis_datum,
            &visus->datum_cache,
            &mensura_blob,
            visus->piscina))
    {
        visus->cache_latitudo = info->latitudo;
        visus->cache_altitudo = info->altitudo;
        visus->cache_id = info->id;
    }
    alioquin
    {
        visus->datum_cache = NIHIL;
        visus->cache_latitudo = 0;
        visus->cache_altitudo = 0;
        visus->cache_id.datum = NIHIL;  /* Clear cache_id so we retry */
        visus->cache_id.mensura = 0;
    }
}

/* Reddere lista in browser mode */
hic_manens vacuum
_reddere_lista(
    PinacothecaVisus* visus,
    TabulaPixelorum*  tabula,
    i32               x,
    i32               y,
    i32               latitudo,
    i32               altitudo,
    i32               scala,
    b32               focused)
{
    i32 char_w, char_h;
    i32 px, py;
    i32 linea;
    i32 max_lineae;
    i32 color_text, color_text_dim, color_selecta_fg, color_selecta_bg;
    memoriae_index num_imagines;
    memoriae_index i;

    (vacuum)focused;

    char_w = VI * scala;
    char_h = VIII * scala;
    px = x * char_w;
    py = y * char_h;

    color_text = color_ad_pixelum(thema_color(COLOR_TEXT));
    color_text_dim = color_ad_pixelum(thema_color(COLOR_TEXT_DIM));
    color_selecta_fg = color_ad_pixelum(thema_color(COLOR_BACKGROUND));
    color_selecta_bg = color_ad_pixelum(thema_color(COLOR_TEXT));

    max_lineae = altitudo - II;  /* Reservare spatium pro header */
    si (max_lineae < I)
    {
        redde;
    }

    num_imagines = visus->imagines ? xar_numerus(visus->imagines) : 0;

    /* Header */
    {
        chorda header;
        character buf[LXIV];
        sprintf(buf, "Imagines (%d)", (i32)num_imagines);
        header = chorda_ex_literis(buf, visus->piscina);
        tabula_pixelorum_pingere_chordam_scalatam(
            tabula, px, py, header, color_text_dim, scala);
    }

    /* Lista */
    linea = 0;

    per (i = (memoriae_index)visus->index_paginae;
         i < num_imagines && linea < max_lineae;
         i++, linea++)
    {
        ImagoInfo* info;
        i32 line_y;
        b32 selecta;
        chorda titulus_display;

        info = (ImagoInfo*)xar_obtinere(visus->imagines, (i32)i);
        si (info == NIHIL)
        {
            perge;
        }

        line_y = py + (linea + II) * char_h;
        selecta = ((s32)i == visus->index_selecta);

        /* Truncare titulum si necessarium */
        si (latitudo > III && info->titulus.mensura > latitudo - III)
        {
            titulus_display.datum = info->titulus.datum;
            titulus_display.mensura = latitudo - III;
        }
        alioquin
        {
            titulus_display = info->titulus;
        }

        si (selecta)
        {
            /* Pingere fundum selectionis - rectangulum plenum */
            i32 rect_x, rect_y;
            i32 rect_w, rect_h;

            rect_w = latitudo * char_w;
            rect_h = char_h;

            per (rect_y = 0; rect_y < rect_h; rect_y++)
            {
                per (rect_x = 0; rect_x < rect_w; rect_x++)
                {
                    i32 dest_x, dest_y;
                    dest_x = px + rect_x;
                    dest_y = line_y + rect_y;
                    si (dest_x >= 0 && dest_x < (i32)tabula->latitudo &&
                        dest_y >= 0 && dest_y < (i32)tabula->altitudo)
                    {
                        tabula->pixela[dest_y * (i32)tabula->latitudo + dest_x] = color_selecta_bg;
                    }
                }
            }

            /* Prefix */
            {
                chorda prefix;
                prefix = chorda_ex_literis("> ", visus->piscina);
                tabula_pixelorum_pingere_chordam_scalatam(
                    tabula, px, line_y, prefix, color_selecta_fg, scala);
            }

            /* Titulus */
            tabula_pixelorum_pingere_chordam_scalatam(
                tabula, px + (II * char_w), line_y, titulus_display, color_selecta_fg, scala);
        }
        alioquin
        {
            /* Prefix spatium */
            {
                chorda prefix;
                prefix = chorda_ex_literis("  ", visus->piscina);
                tabula_pixelorum_pingere_chordam_scalatam(
                    tabula, px, line_y, prefix, color_text_dim, scala);
            }

            /* Titulus */
            tabula_pixelorum_pingere_chordam_scalatam(
                tabula, px + (II * char_w), line_y, titulus_display, color_text, scala);
        }
    }
}

/* Reddere preview in browser mode */
hic_manens vacuum
_reddere_preview(
    PinacothecaVisus* visus,
    TabulaPixelorum*  tabula,
    i32               x,
    i32               y,
    i32               latitudo,
    i32               altitudo,
    i32               scala)
{
    i32 char_w, char_h;
    i32 px, py, pw, ph;
    i32 dest_x, dest_y;

    char_w = VI * scala;
    char_h = VIII * scala;
    px = x * char_w;
    py = y * char_h;
    pw = latitudo * char_w;
    ph = altitudo * char_h;

    si (visus->datum_cache == NIHIL ||
        visus->cache_latitudo <= 0 ||
        visus->cache_altitudo <= 0)
    {
        /* Monstrare "Nulla imago" */
        chorda msg;
        msg = chorda_ex_literis("(nulla imago)", visus->piscina);
        tabula_pixelorum_pingere_chordam_scalatam(
            tabula,
            px + (pw - (i32)msg.mensura * char_w) / II,
            py + ph / II,
            msg,
            color_ad_pixelum(thema_color(COLOR_TEXT_DIM)),
            scala);
        redde;
    }

    /* Calculare dimensiones scalatas ad fit preview area */
    {
        i32 scaled_w, scaled_h;
        i32 info_space;
        i32 available_h;
        f32 scale_x, scale_y, scale_factor;

        /* Reservare spatium pro info text */
        info_space = char_h * 2;
        available_h = ph - info_space;
        si (available_h < 1)
        {
            available_h = 1;
        }

        /* Calculare scale factor ad fit (maintain aspect ratio) */
        scale_x = (f32)pw / (f32)visus->cache_latitudo;
        scale_y = (f32)available_h / (f32)visus->cache_altitudo;
        scale_factor = scale_x < scale_y ? scale_x : scale_y;

        /* Non magnificare ultra 1.0 */
        si (scale_factor > 1.0f)
        {
            scale_factor = 1.0f;
        }

        scaled_w = (i32)((f32)visus->cache_latitudo * scale_factor);
        scaled_h = (i32)((f32)visus->cache_altitudo * scale_factor);

        /* Center image */
        dest_x = (i32)((s32)px + ((s32)pw - (s32)scaled_w) / 2);
        dest_y = (i32)((s32)py + ((s32)available_h - (s32)scaled_h) / 2);

        /* Pingere imaginem scalatam (nearest neighbor) */
        {
            i32 out_y, out_x;

            per (out_y = 0; out_y < scaled_h; out_y++)
            {
                per (out_x = 0; out_x < scaled_w; out_x++)
                {
                    i32 src_x, src_y, src_idx;
                    i8 index;
                    i8 r, g, b;
                    i32 dx, dy;

                    /* Nearest neighbor sampling */
                    src_x = (i32)((f32)out_x / scale_factor);
                    src_y = (i32)((f32)out_y / scale_factor);

                    /* Clamp ad source bounds */
                    si (src_x >= visus->cache_latitudo)
                    {
                        src_x = visus->cache_latitudo - 1;
                    }
                    si (src_y >= visus->cache_altitudo)
                    {
                        src_y = visus->cache_altitudo - 1;
                    }

                    src_idx = src_y * visus->cache_latitudo + src_x;
                    index = visus->datum_cache[src_idx];

                    /* Capere colorem ex palette */
                    dithering_color_ex_indice((s32)index, &r, &g, &b);

                    dx = dest_x + out_x;
                    dy = dest_y + out_y;

                    si (dx >= 0 && dx < (i32)tabula->latitudo &&
                        dy >= 0 && dy < (i32)tabula->altitudo)
                    {
                        tabula->pixela[dy * (i32)tabula->latitudo + dx] =
                            (i32)RGB((insignatus character)r,
                                     (insignatus character)g,
                                     (insignatus character)b);
                    }
                }
            }
        }

        /* Info sub imagine */
        {
            ImagoInfo* info;
            chorda info_str;
            character buf[CXXVIII];
            i32 info_y;

            si (visus->index_selecta >= 0 &&
                (i32)visus->index_selecta < xar_numerus(visus->imagines))
            {
                info = (ImagoInfo*)xar_obtinere(visus->imagines, (i32)visus->index_selecta);
                si (info != NIHIL)
                {
                    sprintf(buf, "%.*s (%dx%d)",
                        (i32)info->titulus.mensura, info->titulus.datum,
                        info->latitudo, info->altitudo);
                    info_str = chorda_ex_literis(buf, visus->piscina);
                    info_y = dest_y + scaled_h + char_h / 2;
                    si (info_y < py + ph - char_h)
                    {
                        tabula_pixelorum_pingere_chordam_scalatam(
                            tabula,
                            px + (pw - (i32)info_str.mensura * char_w) / 2,
                            info_y,
                            info_str,
                            color_ad_pixelum(thema_color(COLOR_TEXT_DIM)),
                            scala);
                    }
                }
            }
        }
    }
}

/* Reddere modus visus (plena imago) */
hic_manens vacuum
_reddere_visus(
    PinacothecaVisus* visus,
    TabulaPixelorum*  tabula,
    i32               x,
    i32               y,
    i32               latitudo,
    i32               altitudo,
    i32               scala)
{
    i32 char_w, char_h;
    i32 px, py, pw, ph;
    i32 dest_x, dest_y;

    char_w = VI * scala;
    char_h = VIII * scala;
    px = x * char_w;
    py = y * char_h;
    pw = latitudo * char_w;
    ph = altitudo * char_h;

    si (visus->datum_cache == NIHIL ||
        visus->cache_latitudo <= 0 ||
        visus->cache_altitudo <= 0)
    {
        chorda msg;
        msg = chorda_ex_literis("(nulla imago)", visus->piscina);
        tabula_pixelorum_pingere_chordam_scalatam(
            tabula,
            px + (pw - (i32)msg.mensura * char_w) / II,
            py + ph / II,
            msg,
            color_ad_pixelum(thema_color(COLOR_TEXT_DIM)),
            scala);
        redde;
    }

    /* Scalare imaginem ad fit view area */
    {
        i32 scaled_w, scaled_h;
        f32 scale_x, scale_y, scale_factor;

        /* Calculare scale factor ad fit (maintain aspect ratio) */
        scale_x = (f32)pw / (f32)visus->cache_latitudo;
        scale_y = (f32)ph / (f32)visus->cache_altitudo;
        scale_factor = scale_x < scale_y ? scale_x : scale_y;

        /* Non magnificare ultra 1.0 - monstrare ad dimensiones originales */
        si (scale_factor > 1.0f)
        {
            scale_factor = 1.0f;
        }

        scaled_w = (i32)((f32)visus->cache_latitudo * scale_factor);
        scaled_h = (i32)((f32)visus->cache_altitudo * scale_factor);

        /* Center image */
        dest_x = (i32)((s32)px + ((s32)pw - (s32)scaled_w) / 2);
        dest_y = (i32)((s32)py + ((s32)ph - (s32)scaled_h) / 2);

        /* Pingere imaginem scalatam (nearest neighbor) */
        {
            i32 out_y, out_x;

            per (out_y = 0; out_y < scaled_h; out_y++)
            {
                per (out_x = 0; out_x < scaled_w; out_x++)
                {
                    i32 src_x, src_y, src_idx;
                    i8 index;
                    i8 r, g, b;
                    i32 dx, dy;

                    /* Nearest neighbor sampling */
                    src_x = (i32)((f32)out_x / scale_factor);
                    src_y = (i32)((f32)out_y / scale_factor);

                    /* Clamp ad source bounds */
                    si (src_x >= visus->cache_latitudo)
                    {
                        src_x = visus->cache_latitudo - 1;
                    }
                    si (src_y >= visus->cache_altitudo)
                    {
                        src_y = visus->cache_altitudo - 1;
                    }

                    src_idx = src_y * visus->cache_latitudo + src_x;
                    index = visus->datum_cache[src_idx];

                    dithering_color_ex_indice((s32)index, &r, &g, &b);

                    dx = dest_x + out_x;
                    dy = dest_y + out_y;

                    si (dx >= 0 && dx < (i32)tabula->latitudo &&
                        dy >= 0 && dy < (i32)tabula->altitudo)
                    {
                        tabula->pixela[dy * (i32)tabula->latitudo + dx] =
                            (i32)RGB((insignatus character)r,
                                     (insignatus character)g,
                                     (insignatus character)b);
                    }
                }
            }
        }
    }
}

/* Command handlers */
hic_manens b32
_pinacotheca_command_handler(ContextusCommandi* ctx)
{
    ContextusWidget* widget_ctx;

    widget_ctx = (ContextusWidget*)ctx->datum_registratus;
    si (!widget_ctx || !widget_ctx->commutare_widget)
    {
        redde FALSUM;
    }

    /* Commutare ad pinacotheca */
    widget_ctx->commutare_widget(
        widget_ctx->schirmata_datum,
        "pinacotheca",
        NIHIL);

    redde VERUM;
}

hic_manens b32
_image_command_handler(ContextusCommandi* ctx)
{
    ContextusWidget* widget_ctx;
    character argumentum[CXXVIII];
    i32 idx;
    i32 col;
    character c;

    widget_ctx = (ContextusWidget*)ctx->datum_registratus;
    si (!widget_ctx || !widget_ctx->commutare_widget)
    {
        redde FALSUM;
    }

    /* Extractare argumentum (titulus imaginis) */
    idx = 0;
    per (col = ctx->columna; col < ctx->pagina->tabula.latitudo && idx < CXXVII; col++)
    {
        c = tabula_cellula(&ctx->pagina->tabula, ctx->linea, col);
        si (c == '\0' || c == '\n')
        {
            frange;
        }
        argumentum[idx++] = c;
    }
    argumentum[idx] = '\0';

    /* Commutare ad pinacotheca cum titulus */
    widget_ctx->commutare_widget(
        widget_ctx->schirmata_datum,
        "pinacotheca",
        idx > 0 ? argumentum : NIHIL);

    redde VERUM;
}


/* ============================================================
 * API Publica
 * ============================================================ */

PinacothecaVisus*
pinacotheca_visus_creare(ContextusWidget* ctx)
{
    PinacothecaVisus* visus;

    si (ctx == NIHIL || ctx->piscina == NIHIL)
    {
        redde NIHIL;
    }

    visus = piscina_allocare(ctx->piscina, magnitudo(PinacothecaVisus));
    si (visus == NIHIL)
    {
        redde NIHIL;
    }

    visus->piscina = ctx->piscina;
    visus->ctx = ctx;

    visus->imagines = NIHIL;
    visus->index_selecta = 0;
    visus->index_paginae = 0;
    visus->modus = PINACOTHECA_MODUS_BROWSER;

    visus->datum_cache = NIHIL;
    visus->cache_latitudo = 0;
    visus->cache_altitudo = 0;
    visus->cache_id.datum = NIHIL;
    visus->cache_id.mensura = 0;

    visus->confirmare_delere = FALSUM;
    visus->renominare_activum = FALSUM;
    visus->novum_nomen.datum = NIHIL;
    visus->novum_nomen.mensura = 0;
    visus->novum_nomen_cursor = 0;

    /* Carcare imagines */
    pinacotheca_visus_reficere(visus);

    redde visus;
}

vacuum
pinacotheca_visus_reddere(
    PinacothecaVisus* visus,
    TabulaPixelorum*  tabula,
    i32               x,
    i32               y,
    i32               latitudo,
    i32               altitudo,
    i32               scala,
    b32               focused)
{
    si (visus == NIHIL || tabula == NIHIL)
    {
        redde;
    }

    /* Salvare dimensiones (cast i32 to s32) */
    visus->widget_x = (s32)x;
    visus->widget_y = (s32)y;
    visus->latitudo_characterum = (s32)latitudo;
    visus->altitudo_linearum = (s32)altitudo;
    visus->scala = (s32)scala;

    /* Carcare imaginem si necessarium */
    _carcare_imaginem_selectam(visus);

    si (visus->modus == PINACOTHECA_MODUS_VISUS)
    {
        /* Plena imago */
        _reddere_visus(visus, tabula, x, y, latitudo, altitudo, scala);
    }
    alioquin
    {
        /* Browser: lista + preview */
        i32 lista_lat;

        lista_lat = PINACOTHECA_LATITUDO_LISTAE;
        si (lista_lat > latitudo / II)
        {
            lista_lat = latitudo / II;
        }

        _reddere_lista(visus, tabula, x, y, lista_lat, altitudo, scala, focused);
        _reddere_preview(visus, tabula, x + lista_lat, y, latitudo - lista_lat, altitudo, scala);
    }
}

b32
pinacotheca_visus_tractare_eventum(
    PinacothecaVisus* visus,
    constans Eventus* eventus)
{
    memoriae_index num_imagines;

    si (visus == NIHIL || eventus == NIHIL)
    {
        redde FALSUM;
    }

    num_imagines = visus->imagines ? xar_numerus(visus->imagines) : 0;

    si (eventus->genus == EVENTUS_CLAVIS_DEPRESSUS)
    {
        s32 clavis;
        clavis = (s32)eventus->datum.clavis.clavis;

        /* j = down */
        si (clavis == 'j' || clavis == 'J')
        {
            si (visus->modus == PINACOTHECA_MODUS_BROWSER && num_imagines > 0)
            {
                s32 items_per_page;
                s32 position_in_page;

                /* Calculare items per page (altitudo minus header) */
                items_per_page = visus->altitudo_linearum - II;
                si (items_per_page < I)
                {
                    items_per_page = I;
                }

                position_in_page = visus->index_selecta - visus->index_paginae;

                /* Si ad finem paginae, ire ad proximam paginam */
                si (position_in_page >= items_per_page - I)
                {
                    /* Ire ad proximam paginam si sunt plures imagines */
                    si (visus->index_paginae + items_per_page < (s32)num_imagines)
                    {
                        visus->index_paginae += items_per_page;
                        visus->index_selecta = visus->index_paginae;
                    }
                    /* Alioquin manere ad ultimam imaginem */
                }
                alioquin
                {
                    /* Movere infra in pagina currente */
                    visus->index_selecta++;
                    si ((s32)visus->index_selecta >= (s32)num_imagines)
                    {
                        visus->index_selecta = (s32)(num_imagines - I);
                    }
                }
            }
            redde VERUM;
        }

        /* k = up */
        si (clavis == 'k' || clavis == 'K')
        {
            si (visus->modus == PINACOTHECA_MODUS_BROWSER && num_imagines > 0)
            {
                s32 items_per_page;
                s32 position_in_page;

                /* Calculare items per page (altitudo minus header) */
                items_per_page = visus->altitudo_linearum - II;
                si (items_per_page < I)
                {
                    items_per_page = I;
                }

                position_in_page = visus->index_selecta - visus->index_paginae;

                /* Si ad initium paginae, ire ad priorem paginam */
                si (position_in_page <= 0)
                {
                    /* Ire ad priorem paginam si non in prima */
                    si (visus->index_paginae > 0)
                    {
                        visus->index_paginae -= items_per_page;
                        si (visus->index_paginae < 0)
                        {
                            visus->index_paginae = 0;
                        }
                        /* Selectare ultimum item in pagina priore */
                        visus->index_selecta = visus->index_paginae + items_per_page - I;
                        si ((s32)visus->index_selecta >= (s32)num_imagines)
                        {
                            visus->index_selecta = (s32)(num_imagines - I);
                        }
                    }
                    /* Alioquin manere ad primam imaginem */
                }
                alioquin
                {
                    /* Movere supra in pagina currente */
                    visus->index_selecta--;
                    si (visus->index_selecta < 0)
                    {
                        visus->index_selecta = 0;
                    }
                }
            }
            redde VERUM;
        }

        /* Enter = open in visus mode */
        si (clavis == CLAVIS_REDITUS)
        {
            si (visus->modus == PINACOTHECA_MODUS_BROWSER && num_imagines > 0)
            {
                visus->modus = PINACOTHECA_MODUS_VISUS;
            }
            redde VERUM;
        }

        /* Escape = back */
        si (clavis == CLAVIS_EFFUGIUM)
        {
            si (visus->modus == PINACOTHECA_MODUS_VISUS)
            {
                visus->modus = PINACOTHECA_MODUS_BROWSER;
                redde VERUM;
            }
            /* In browser mode, let schirmata handle escape */
            redde FALSUM;
        }

        /* Ctrl+Shift+Arrows in visus mode: cycle images */
        si (visus->modus == PINACOTHECA_MODUS_VISUS &&
            (eventus->datum.clavis.modificantes & MOD_IMPERIUM) &&
            (eventus->datum.clavis.modificantes & MOD_SHIFT))
        {
            /* Right: proxima imago */
            si (clavis == CLAVIS_DEXTER)
            {
                si (num_imagines > 0)
                {
                    visus->index_selecta++;
                    /* Cycle ad initium */
                    si ((memoriae_index)visus->index_selecta >= num_imagines)
                    {
                        visus->index_selecta = 0;
                    }
                    /* Invalidare cache */
                    visus->datum_cache = NIHIL;
                    visus->cache_id.datum = NIHIL;
                }
                redde VERUM;
            }

            /* Left: prior imago */
            si (clavis == CLAVIS_SINISTER)
            {
                si (num_imagines > 0)
                {
                    visus->index_selecta--;
                    /* Cycle ad finem */
                    si (visus->index_selecta < 0)
                    {
                        visus->index_selecta = (s32)(num_imagines - 1);
                    }
                    /* Invalidare cache */
                    visus->datum_cache = NIHIL;
                    visus->cache_id.datum = NIHIL;
                }
                redde VERUM;
            }
        }

        /* d = delete (with confirmation in next phase) */
        si (clavis == 'd' || clavis == 'D')
        {
            /* TODO: Implement delete with confirmation dialog */
            redde VERUM;
        }

        /* r = refresh */
        si (clavis == 'r' || clavis == 'R')
        {
            pinacotheca_visus_reficere(visus);
            redde VERUM;
        }
    }

    redde FALSUM;
}

vacuum
pinacotheca_visus_reficere(PinacothecaVisus* visus)
{
    Xar* entitates;
    memoriae_index i, num;

    si (visus == NIHIL || visus->ctx == NIHIL || visus->ctx->repo == NIHIL)
    {
        redde;
    }

    /* Query omnes Vultus::Imago */
    entitates = visus->ctx->repo->quaerere_cum_genere(
        visus->ctx->repo->datum,
        "Vultus::Imago");

    si (entitates == NIHIL)
    {
        visus->imagines = NIHIL;
        visus->index_selecta = 0;
        visus->index_paginae = 0;
        redde;
    }

    /* Creare lista ImagoInfo */
    visus->imagines = xar_creare(visus->piscina, magnitudo(ImagoInfo));
    num = xar_numerus(entitates);

    per (i = 0; i < num; i++)
    {
        Entitas* entitas;
        ImagoInfo* info;
        chorda* clavis_titulus;
        chorda* clavis_latitudo;
        chorda* clavis_altitudo;
        chorda* titulus_val;
        s32 lat_val, alt_val;

        entitas = *(Entitas**)xar_obtinere(entitates, (i32)i);
        si (entitas == NIHIL)
        {
            perge;
        }

        info = (ImagoInfo*)xar_addere(visus->imagines);
        si (info == NIHIL)
        {
            perge;
        }

        /* ID */
        si (entitas->id != NIHIL)
        {
            info->id = *entitas->id;
        }
        alioquin
        {
            info->id.datum = NIHIL;
            info->id.mensura = 0;
        }

        /* Titulus - clavis debet esse internata (pointer comparison!) */
        clavis_titulus = chorda_internare_ex_literis(internamentum_globale(), "titulus");
        titulus_val = entitas_proprietas_capere(entitas, clavis_titulus);
        si (titulus_val != NIHIL)
        {
            info->titulus = *titulus_val;
        }
        alioquin
        {
            info->titulus.datum = NIHIL;
            info->titulus.mensura = 0;
        }

        /* Latitudo */
        clavis_latitudo = chorda_internare_ex_literis(internamentum_globale(), "latitudo");
        si (entitas_proprietas_capere_s32(entitas, clavis_latitudo, &lat_val))
        {
            info->latitudo = (i32)lat_val;
        }
        alioquin
        {
            info->latitudo = 0;
        }

        /* Altitudo */
        clavis_altitudo = chorda_internare_ex_literis(internamentum_globale(), "altitudo");
        si (entitas_proprietas_capere_s32(entitas, clavis_altitudo, &alt_val))
        {
            info->altitudo = (i32)alt_val;
        }
        alioquin
        {
            info->altitudo = 0;
        }
    }

    /* Sortare alphabetice */
    si (xar_numerus(visus->imagines) > 1)
    {
        xar_ordinare(visus->imagines, _comparare_imagines);
    }

    /* Reset selectio */
    visus->index_selecta = 0;
    visus->index_paginae = 0;

    /* Invalidare cache */
    visus->datum_cache = NIHIL;
    visus->cache_id.datum = NIHIL;

    fprintf(stderr, "Pinacotheca: %d imagines reficatae\n", (i32)xar_numerus(visus->imagines));
}

vacuum
pinacotheca_visus_navigare_ad(
    PinacothecaVisus*   visus,
    constans character* titulus)
{
    memoriae_index i, num;
    memoriae_index titulus_len;

    si (visus == NIHIL || titulus == NIHIL || visus->imagines == NIHIL)
    {
        redde;
    }

    titulus_len = strlen(titulus);
    num = xar_numerus(visus->imagines);

    per (i = 0; i < num; i++)
    {
        ImagoInfo* info;
        info = (ImagoInfo*)xar_obtinere(visus->imagines, (i32)i);

        si (info != NIHIL &&
            info->titulus.mensura == titulus_len &&
            memcmp(info->titulus.datum, titulus, titulus_len) == 0)
        {
            visus->index_selecta = (s32)i;
            visus->index_paginae = (s32)i;
            visus->modus = PINACOTHECA_MODUS_VISUS;
            redde;
        }
    }
}

vacuum
pinacotheca_visus_init(ContextusWidget* ctx)
{
    si (ctx == NIHIL || ctx->reg_commandi == NIHIL)
    {
        redde;
    }

    /* Register $pinacotheca command */
    registrum_commandi_registrare(
        ctx->reg_commandi,
        "pinacotheca",
        _pinacotheca_command_handler,
        ctx);

    /* Register $image command */
    registrum_commandi_registrare(
        ctx->reg_commandi,
        "image",
        _image_command_handler,
        ctx);

    fprintf(stderr, "Pinacotheca: commandi registrati\n");
}
