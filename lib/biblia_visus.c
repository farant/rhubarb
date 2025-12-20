#include "biblia_visus.h"
#include "thema.h"
#include "color.h"
#include "delineare.h"
#include "chorda.h"
#include <string.h>
#include <stdio.h>

/* ==================================================
 * Constantae
 * ================================================== */

#define CHAR_LATITUDO    VI     /* 6 pixels per character */
#define CHAR_ALTITUDO    VIII   /* 8 pixels per character */
#define PADDING          II     /* 2 characters padding */
#define COLUMNA_LATITUDO XXVI   /* 26 chars per book name column */
#define COLUMNAE_NUMERUS III    /* 3 columns in TOC */

/* Indices pro testamentis */
#define LIBER_PRIMUS_NT  LIBER_MATTHAEUS  /* 46 = first NT book */


/* ==================================================
 * Functiones Auxiliares
 * ================================================== */

/* Creare chorda ex C string (view, non allocatio) */
hic_manens chorda
_chorda_ex_cstr(
    constans character* cstr)
{
    chorda fructus;
    union { constans character* c; i8* m; } u;
    u.c = cstr;

    fructus.datum = u.m;
    fructus.mensura = (i32)strlen(cstr);

    redde fructus;
}


/* ==================================================
 * Creatio
 * ================================================== */

BibliaVisus*
biblia_visus_creare(
    Piscina* piscina)
{
    BibliaVisus* visus;

    si (!piscina)
    {
        redde NIHIL;
    }

    visus = (BibliaVisus*)piscina_allocare(piscina, magnitudo(BibliaVisus));
    si (!visus)
    {
        redde NIHIL;
    }

    visus->piscina = piscina;
    visus->biblia = biblia_obtinere_dr();
    visus->in_toc = VERUM;
    visus->liber_currens = ZEPHYRUM;
    visus->capitulum_currens = I;
    visus->versus_initium = ZEPHYRUM;
    visus->versus_in_pagina = ZEPHYRUM;
    visus->paginae_numerus = ZEPHYRUM;
    visus->index_paginae = ZEPHYRUM;
    visus->cache_liber = (i32)(-1);
    visus->cache_capitulum = (i32)(-1);
    visus->cache_latitudo = ZEPHYRUM;
    visus->cache_altitudo = ZEPHYRUM;
    visus->latitudo_characterum = ZEPHYRUM;
    visus->altitudo_linearum = ZEPHYRUM;
    visus->scala = I;
    visus->widget_x = ZEPHYRUM;
    visus->widget_y = ZEPHYRUM;

    redde visus;
}


/* ==================================================
 * Pagination Calculation
 * ================================================== */

/* Calculate page boundaries for current chapter
 *
 * Simulates rendering to determine where each page starts.
 * Must be called when chapter or layout dimensions change.
 */
hic_manens vacuum
_biblia_visus_calculare_paginationem(
    BibliaVisus* visus,
    i32          latitudo,
    i32          altitudo)
{
    i32 versus_in_capitulo;
    i32 chars_disponibiles;
    i32 lineae_disponibiles;
    i32 versus_idx;
    i32 lineae_usae;
    i32 pagina_idx;

    /* Check if recalculation needed */
    si (visus->cache_liber == visus->liber_currens &&
        visus->cache_capitulum == visus->capitulum_currens &&
        visus->cache_latitudo == latitudo &&
        visus->cache_altitudo == altitudo)
    {
        redde;  /* Cache still valid */
    }

    /* Update cache */
    visus->cache_liber = visus->liber_currens;
    visus->cache_capitulum = visus->capitulum_currens;
    visus->cache_latitudo = latitudo;
    visus->cache_altitudo = altitudo;

    versus_in_capitulo = biblia_versus_in_capitulo(visus->biblia,
        visus->liber_currens, visus->capitulum_currens);
    chars_disponibiles = latitudo - (PADDING * II);
    lineae_disponibiles = altitudo - V;  /* header, separator, footer */

    si (lineae_disponibiles < I)
    {
        lineae_disponibiles = I;
    }

    /* First page always starts at verse 0 */
    visus->paginae_limites[ZEPHYRUM] = ZEPHYRUM;
    pagina_idx = I;
    lineae_usae = ZEPHYRUM;
    versus_idx = ZEPHYRUM;

    dum (versus_idx < versus_in_capitulo && pagina_idx < BIBLIA_PAGINAE_MAXIMUS)
    {
        chorda versus;
        i32 versus_longitudo;
        i32 num_longitudo;
        i32 lineae_versus;
        character num_buffer[VIII];

        versus = biblia_versus(visus->biblia,
            visus->liber_currens, visus->capitulum_currens, versus_idx + I);

        si (versus.mensura == ZEPHYRUM)
        {
            versus_idx++;
            perge;
        }

        /* Calculate lines needed for this verse */
        sprintf(num_buffer, "%d ", versus_idx + I);
        num_longitudo = (i32)strlen(num_buffer);
        versus_longitudo = versus.mensura;

        /* Estimate lines: first line has number, rest are full width */
        {
            i32 first_line_chars;
            i32 remaining_chars;

            first_line_chars = chars_disponibiles - num_longitudo;
            si (versus_longitudo <= first_line_chars)
            {
                lineae_versus = I;
            }
            alioquin
            {
                remaining_chars = versus_longitudo - first_line_chars;
                lineae_versus = I + ((remaining_chars + chars_disponibiles - I) / chars_disponibiles);
            }
        }

        /* Check if verse fits on current page */
        si (lineae_usae + lineae_versus > lineae_disponibiles && lineae_usae > ZEPHYRUM)
        {
            /* Start new page at this verse */
            visus->paginae_limites[pagina_idx] = versus_idx;
            pagina_idx++;
            lineae_usae = ZEPHYRUM;
        }

        lineae_usae += lineae_versus;
        versus_idx++;
    }

    visus->paginae_numerus = pagina_idx;
}


/* ==================================================
 * TOC Rendering
 * ================================================== */

hic_manens vacuum
_biblia_visus_reddere_toc(
    BibliaVisus*     visus,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala)
{
    ContextusDelineandi* ctx;
    i32 char_lat;
    i32 char_alt;
    i32 linea;
    i32 col;
    i32 liber_idx;
    i32 pixelum_text;
    i32 pixelum_text_dim;
    i32 pixelum_accent;
    Color color_background;
    chorda titulus;

    (vacuum)latitudo;
    (vacuum)altitudo;

    ctx = delineare_creare_contextum(visus->piscina, tabula);
    si (!ctx)
    {
        redde;
    }

    char_lat = CHAR_LATITUDO * scala;
    char_alt = CHAR_ALTITUDO * scala;

    color_background = thema_color(COLOR_BACKGROUND);
    pixelum_text = color_ad_pixelum(thema_color(COLOR_TEXT));
    pixelum_text_dim = color_ad_pixelum(thema_color(COLOR_TEXT_DIM));
    pixelum_accent = color_ad_pixelum(thema_color(COLOR_ACCENT_PRIMARY));

    /* Pingere fondum */
    delineare_rectangulum_plenum(ctx,
        x * char_lat, y * char_alt,
        latitudo * char_lat, altitudo * char_alt,
        color_background);

    linea = y + I;

    /* === Titulus === */
    titulus = _chorda_ex_cstr("SACRA BIBLIA - Douay-Rheims");
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        titulus, pixelum_accent, scala);
    linea += II;

    /* === Vetus Testamentum === */
    titulus = _chorda_ex_cstr("=== Vetus Testamentum ===");
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        titulus, pixelum_text_dim, scala);
    linea += II;

    /* Libri Veteris Testamenti (0-45) - vertical flow */
    {
        i32 ot_numerus = LIBER_PRIMUS_NT;  /* 46 books */
        i32 libri_per_columna = (ot_numerus + COLUMNAE_NUMERUS - I) / COLUMNAE_NUMERUS;
        i32 linea_initium = linea;

        per (liber_idx = ZEPHYRUM; liber_idx < ot_numerus; liber_idx++)
        {
            constans character* titulis;
            chorda titulis_chorda;
            i32 col_x;
            i32 linea_liber;

            col = liber_idx / libri_per_columna;
            linea_liber = linea_initium + (liber_idx % libri_per_columna);

            titulis = biblia_nomen_libri(visus->biblia, liber_idx);
            si (titulis)
            {
                titulis_chorda = _chorda_ex_cstr(titulis);
                col_x = x + PADDING + (col * COLUMNA_LATITUDO);

                tabula_pixelorum_pingere_chordam_scalatam(tabula,
                    col_x * char_lat, linea_liber * char_alt,
                    titulis_chorda, pixelum_text, scala);
            }
        }

        linea = linea_initium + libri_per_columna;
    }
    linea += II;

    /* === Novum Testamentum === */
    titulus = _chorda_ex_cstr("=== Novum Testamentum ===");
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        titulus, pixelum_text_dim, scala);
    linea += II;

    /* Libri Novi Testamenti (46-72) - vertical flow */
    {
        i32 nt_numerus = LIBRI_NUMERUS - LIBER_PRIMUS_NT;  /* 27 books */
        i32 libri_per_columna = (nt_numerus + COLUMNAE_NUMERUS - I) / COLUMNAE_NUMERUS;
        i32 linea_initium = linea;
        i32 nt_idx;

        per (nt_idx = ZEPHYRUM; nt_idx < nt_numerus; nt_idx++)
        {
            constans character* titulis;
            chorda titulis_chorda;
            i32 col_x;
            i32 linea_liber;

            liber_idx = LIBER_PRIMUS_NT + nt_idx;
            col = nt_idx / libri_per_columna;
            linea_liber = linea_initium + (nt_idx % libri_per_columna);

            titulis = biblia_nomen_libri(visus->biblia, liber_idx);
            si (titulis)
            {
                titulis_chorda = _chorda_ex_cstr(titulis);
                col_x = x + PADDING + (col * COLUMNA_LATITUDO);

                tabula_pixelorum_pingere_chordam_scalatam(tabula,
                    col_x * char_lat, linea_liber * char_alt,
                    titulis_chorda, pixelum_text, scala);
            }
        }

        linea = linea_initium + libri_per_columna;
    }

    /* Instructiones */
    linea += III;
    titulus = _chorda_ex_cstr("Click book name to read. Press Escape to exit.");
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        titulus, pixelum_text_dim, scala);
}


/* ==================================================
 * Reading View Rendering
 * ================================================== */

hic_manens vacuum
_biblia_visus_reddere_lectio(
    BibliaVisus*     visus,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala)
{
    ContextusDelineandi* ctx;
    i32 char_lat;
    i32 char_alt;
    i32 linea;
    i32 pixelum_text;
    i32 pixelum_text_dim;
    i32 pixelum_accent;
    Color color_background;
    constans character* nomen_libri;
    character titulus_buffer[LXIV];
    chorda titulus;
    i32 versus_in_capitulo;
    i32 versus_idx;
    i32 lineae_disponibiles;
    i32 lineae_usae;
    i32 chars_disponibiles;
    character nav_buffer[XXXII];

    ctx = delineare_creare_contextum(visus->piscina, tabula);
    si (!ctx)
    {
        redde;
    }

    char_lat = CHAR_LATITUDO * scala;
    char_alt = CHAR_ALTITUDO * scala;

    color_background = thema_color(COLOR_BACKGROUND);
    pixelum_text = color_ad_pixelum(thema_color(COLOR_TEXT));
    pixelum_text_dim = color_ad_pixelum(thema_color(COLOR_TEXT_DIM));
    pixelum_accent = color_ad_pixelum(thema_color(COLOR_ACCENT_PRIMARY));

    /* Pingere fondum */
    delineare_rectangulum_plenum(ctx,
        x * char_lat, y * char_alt,
        latitudo * char_lat, altitudo * char_alt,
        color_background);

    linea = y + I;
    chars_disponibiles = latitudo - (PADDING * II);

    /* Unused variable warning suppression */
    (vacuum)pixelum_text_dim;

    /* === Header: Book Chapter === */
    nomen_libri = biblia_nomen_libri(visus->biblia, visus->liber_currens);
    si (!nomen_libri)
    {
        nomen_libri = "???";
    }
    sprintf(titulus_buffer, "%s %d", nomen_libri, visus->capitulum_currens);
    titulus = _chorda_ex_cstr(titulus_buffer);
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        titulus, pixelum_accent, scala);

    /* Navigation hints on right side */
    sprintf(nav_buffer, "h/l:ch  j/k:pg  Esc:TOC");
    titulus = _chorda_ex_cstr(nav_buffer);
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + latitudo - XXVI) * char_lat, linea * char_alt,
        titulus, pixelum_text_dim, scala);

    linea += II;

    /* === Separator line === */
    {
        i32 sep_x;
        per (sep_x = ZEPHYRUM; sep_x < chars_disponibiles; sep_x++)
        {
            character sep_char[II];
            chorda sep_chorda;

            sep_char[ZEPHYRUM] = '-';
            sep_char[I] = '\0';
            sep_chorda = _chorda_ex_cstr(sep_char);
            tabula_pixelorum_pingere_chordam_scalatam(tabula,
                (x + PADDING + sep_x) * char_lat, linea * char_alt,
                sep_chorda, pixelum_text_dim, scala);
        }
    }
    linea++;

    /* === Verses === */
    versus_in_capitulo = biblia_versus_in_capitulo(visus->biblia,
        visus->liber_currens, visus->capitulum_currens);

    lineae_disponibiles = altitudo - V;  /* header, separator, footer */
    lineae_usae = ZEPHYRUM;

    /* Dark red pro numeris versuum */
    {
        Color color_ruber_obscurus;
        i32 pixelum_numerus;

        color_ruber_obscurus = color_ex_rgb((i8)139, (i8)0, (i8)0);
        pixelum_numerus = color_ad_pixelum(color_ruber_obscurus);

        per (versus_idx = visus->versus_initium; versus_idx < versus_in_capitulo; versus_idx++)
        {
            chorda versus;
            character versus_buffer[DXII];  /* 512 chars max */
            character num_buffer[VIII];
            i32 versus_longitudo;
            i32 num_longitudo;
            i32 lineae_versus;
            i32 pos;
            i32 linea_start;
            i32 col_currens;

            versus = biblia_versus(visus->biblia,
                visus->liber_currens, visus->capitulum_currens, versus_idx + I);

            si (versus.mensura == ZEPHYRUM)
            {
                perge;
            }

            /* Format number separately */
            sprintf(num_buffer, "%d ", versus_idx + I);
            num_longitudo = (i32)strlen(num_buffer);

            /* Copy verse text only */
            versus_longitudo = versus.mensura;
            si (versus_longitudo > DX)
            {
                versus_longitudo = DX;
            }
            memcpy(versus_buffer, versus.datum, (size_t)versus_longitudo);
            versus_buffer[versus_longitudo] = '\0';

            /* Estimate lines needed (rough) */
            lineae_versus = ((num_longitudo + versus_longitudo) + chars_disponibiles - I) / chars_disponibiles;
            si (lineae_versus < I) lineae_versus = I;

            /* Check if verse fits */
            si (lineae_usae + lineae_versus > lineae_disponibiles)
            {
                frange;  /* Stop - don't break mid-verse */
            }

            /* Render verse with word wrap */
            pos = ZEPHYRUM;
            linea_start = linea;
            col_currens = ZEPHYRUM;

            /* First line: number (dark red) then text start */
            {
                chorda num_chorda;

                num_chorda = _chorda_ex_cstr(num_buffer);
                tabula_pixelorum_pingere_chordam_scalatam(tabula,
                    (x + PADDING) * char_lat, linea * char_alt,
                    num_chorda, pixelum_numerus, scala);
                col_currens = num_longitudo;
            }

            /* Render text with word wrap */
            dum (pos < versus_longitudo && linea < y + altitudo - II)
            {
                character line_buffer[CXXVIII];
                i32 chars_in_line;
                i32 break_pos;
                chorda line_chorda;

                chars_in_line = chars_disponibiles - col_currens;
                si (pos + chars_in_line > versus_longitudo)
                {
                    chars_in_line = versus_longitudo - pos;
                }

                /* Find word boundary - don't break mid-word */
                break_pos = chars_in_line;
                si (pos + chars_in_line < versus_longitudo)
                {
                    /* Look for last space within the line */
                    i32 i;
                    per (i = chars_in_line - I; i > ZEPHYRUM; i--)
                    {
                        si (versus_buffer[pos + i] == ' ')
                        {
                            break_pos = i + I;  /* Include the space */
                            frange;
                        }
                    }
                    /* If no space found, break at limit */
                }

                memcpy(line_buffer, versus_buffer + pos, (size_t)break_pos);
                line_buffer[break_pos] = '\0';
                line_chorda = _chorda_ex_cstr(line_buffer);

                tabula_pixelorum_pingere_chordam_scalatam(tabula,
                    (x + PADDING + col_currens) * char_lat, linea * char_alt,
                    line_chorda, pixelum_text, scala);

                pos += break_pos;

                /* Skip leading space on new line */
                dum (pos < versus_longitudo && versus_buffer[pos] == ' ')
                {
                    pos++;
                }

                linea++;
                col_currens = ZEPHYRUM;  /* Reset column for next lines */
            }

            lineae_usae = linea - linea_start + lineae_usae;
        }

        /* Track actual verses rendered for pagination */
        visus->versus_in_pagina = versus_idx - visus->versus_initium;
    }

    /* === Footer: Page info === */
    {
        i32 footer_y;

        footer_y = y + altitudo - I;
        sprintf(nav_buffer, "Page %d/%d", visus->index_paginae + I, visus->paginae_numerus);
        titulus = _chorda_ex_cstr(nav_buffer);
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + latitudo - XVI) * char_lat, footer_y * char_alt,
            titulus, pixelum_text_dim, scala);
    }
}


/* ==================================================
 * Reddere
 * ================================================== */

vacuum
biblia_visus_reddere(
    BibliaVisus*     visus,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused)
{
    (vacuum)focused;

    si (!visus || !tabula)
    {
        redde;
    }

    /* Cache layout dimensions and widget position */
    visus->latitudo_characterum = latitudo;
    visus->altitudo_linearum = altitudo;
    visus->scala = scala;
    visus->widget_x = x;
    visus->widget_y = y;

    si (visus->in_toc)
    {
        _biblia_visus_reddere_toc(visus, tabula, x, y, latitudo, altitudo, scala);
    }
    alioquin
    {
        /* Calculate pagination if needed */
        _biblia_visus_calculare_paginationem(visus, latitudo, altitudo);

        /* Clamp page index to valid range */
        si (visus->index_paginae >= visus->paginae_numerus)
        {
            visus->index_paginae = visus->paginae_numerus - I;
        }
        si (visus->index_paginae < ZEPHYRUM)
        {
            visus->index_paginae = ZEPHYRUM;
        }

        /* Ensure versus_initium matches current page */
        si (visus->paginae_numerus > ZEPHYRUM)
        {
            visus->versus_initium = visus->paginae_limites[visus->index_paginae];
        }

        _biblia_visus_reddere_lectio(visus, tabula, x, y, latitudo, altitudo, scala);
    }
}


/* ==================================================
 * Navigation Helpers
 * ================================================== */

hic_manens vacuum
_biblia_visus_pagina_proxima(
    BibliaVisus* visus)
{
    si (visus->index_paginae < visus->paginae_numerus - I)
    {
        /* Go to next page in current chapter */
        visus->index_paginae++;
    }
    alioquin
    {
        /* Go to next chapter */
        i32 capitula_in_libro;

        capitula_in_libro = biblia_capitula_in_libro(visus->biblia, visus->liber_currens);

        si (visus->capitulum_currens < capitula_in_libro)
        {
            visus->capitulum_currens++;
            visus->index_paginae = ZEPHYRUM;
            visus->cache_capitulum = (i32)(-1);  /* Force recalculation */
        }
        alioquin si (visus->liber_currens < LIBRI_NUMERUS - I)
        {
            /* Go to next book */
            visus->liber_currens++;
            visus->capitulum_currens = I;
            visus->index_paginae = ZEPHYRUM;
            visus->cache_liber = (i32)(-1);  /* Force recalculation */
        }
        /* else stay at end */
    }
}

hic_manens vacuum
_biblia_visus_pagina_prior(
    BibliaVisus* visus)
{
    si (visus->index_paginae > ZEPHYRUM)
    {
        /* Go to previous page in current chapter */
        visus->index_paginae--;
    }
    alioquin
    {
        /* Go to previous chapter */
        si (visus->capitulum_currens > I)
        {
            visus->capitulum_currens--;
            visus->cache_capitulum = (i32)(-1);  /* Force recalculation */
            /* Will go to last page after pagination recalculates */
            visus->index_paginae = BIBLIA_PAGINAE_MAXIMUS;  /* Will be clamped */
        }
        alioquin si (visus->liber_currens > ZEPHYRUM)
        {
            /* Go to previous book, last chapter */
            visus->liber_currens--;
            visus->capitulum_currens = biblia_capitula_in_libro(visus->biblia, visus->liber_currens);
            visus->cache_liber = (i32)(-1);  /* Force recalculation */
            visus->index_paginae = BIBLIA_PAGINAE_MAXIMUS;  /* Will be clamped */
        }
        /* else stay at beginning */
    }
}

hic_manens vacuum
_biblia_visus_capitulum_proximum(
    BibliaVisus* visus)
{
    i32 capitula_in_libro;

    capitula_in_libro = biblia_capitula_in_libro(visus->biblia, visus->liber_currens);

    si (visus->capitulum_currens < capitula_in_libro)
    {
        visus->capitulum_currens++;
        visus->index_paginae = ZEPHYRUM;
        visus->cache_capitulum = (i32)(-1);  /* Force recalculation */
    }
    alioquin si (visus->liber_currens < LIBRI_NUMERUS - I)
    {
        /* Go to next book */
        visus->liber_currens++;
        visus->capitulum_currens = I;
        visus->index_paginae = ZEPHYRUM;
        visus->cache_liber = (i32)(-1);  /* Force recalculation */
    }
}

hic_manens vacuum
_biblia_visus_capitulum_priorem(
    BibliaVisus* visus)
{
    si (visus->capitulum_currens > I)
    {
        visus->capitulum_currens--;
        visus->index_paginae = ZEPHYRUM;
        visus->cache_capitulum = (i32)(-1);  /* Force recalculation */
    }
    alioquin si (visus->liber_currens > ZEPHYRUM)
    {
        /* Go to previous book, last chapter */
        visus->liber_currens--;
        visus->capitulum_currens = biblia_capitula_in_libro(visus->biblia, visus->liber_currens);
        visus->index_paginae = ZEPHYRUM;
        visus->cache_liber = (i32)(-1);  /* Force recalculation */
    }
}


/* ==================================================
 * Click Detection
 * ================================================== */

hic_manens b32
_biblia_visus_tractare_click_toc(
    BibliaVisus* visus,
    i32          click_x,
    i32          click_y)
{
    i32 char_lat;
    i32 char_alt;
    i32 linea;
    i32 linea_ot_start;
    i32 linea_nt_start;

    char_lat = CHAR_LATITUDO * visus->scala;
    char_alt = CHAR_ALTITUDO * visus->scala;

    /* Convert pixel coords to character coords relative to widget */
    click_x = (click_x / char_lat) - visus->widget_x;
    click_y = (click_y / char_alt) - visus->widget_y;

    /* Calculate where OT books start (after title and header) */
    linea_ot_start = I + II + II;  /* title + spacing + OT header + spacing */

    /* Check OT section - vertical flow */
    {
        i32 ot_numerus = LIBER_PRIMUS_NT;  /* 46 books */
        i32 libri_per_columna = (ot_numerus + COLUMNAE_NUMERUS - I) / COLUMNAE_NUMERUS;
        i32 liber_idx;

        per (liber_idx = ZEPHYRUM; liber_idx < ot_numerus; liber_idx++)
        {
            i32 col;
            i32 linea_liber;
            i32 col_x;
            i32 name_len;
            constans character* titulis;

            col = liber_idx / libri_per_columna;
            linea_liber = linea_ot_start + (liber_idx % libri_per_columna);
            col_x = PADDING + (col * COLUMNA_LATITUDO);

            titulis = biblia_nomen_libri(visus->biblia, liber_idx);
            name_len = titulis ? (i32)strlen(titulis) : ZEPHYRUM;

            si (click_y == linea_liber &&
                click_x >= col_x &&
                click_x < col_x + name_len)
            {
                visus->liber_currens = liber_idx;
                visus->capitulum_currens = I;
                visus->index_paginae = ZEPHYRUM;
                visus->cache_liber = (i32)(-1);  /* Force recalculation */
                visus->in_toc = FALSUM;
                redde VERUM;
            }
        }

        linea = linea_ot_start + libri_per_columna;
    }

    /* Calculate where NT section starts */
    linea_nt_start = linea + II + II;  /* spacing + NT header + spacing */

    /* Check NT section - vertical flow */
    {
        i32 nt_numerus = LIBRI_NUMERUS - LIBER_PRIMUS_NT;  /* 27 books */
        i32 libri_per_columna = (nt_numerus + COLUMNAE_NUMERUS - I) / COLUMNAE_NUMERUS;
        i32 nt_idx;

        per (nt_idx = ZEPHYRUM; nt_idx < nt_numerus; nt_idx++)
        {
            i32 liber_idx;
            i32 col;
            i32 linea_liber;
            i32 col_x;
            i32 name_len;
            constans character* titulis;

            liber_idx = LIBER_PRIMUS_NT + nt_idx;
            col = nt_idx / libri_per_columna;
            linea_liber = linea_nt_start + (nt_idx % libri_per_columna);
            col_x = PADDING + (col * COLUMNA_LATITUDO);

            titulis = biblia_nomen_libri(visus->biblia, liber_idx);
            name_len = titulis ? (i32)strlen(titulis) : ZEPHYRUM;

            si (click_y == linea_liber &&
                click_x >= col_x &&
                click_x < col_x + name_len)
            {
                visus->liber_currens = liber_idx;
                visus->capitulum_currens = I;
                visus->index_paginae = ZEPHYRUM;
                visus->cache_liber = (i32)(-1);  /* Force recalculation */
                visus->in_toc = FALSUM;
                redde VERUM;
            }
        }
    }

    redde FALSUM;
}


/* ==================================================
 * Eventus
 * ================================================== */

b32
biblia_visus_tractare_eventum(
    BibliaVisus*      visus,
    constans Eventus* eventus)
{
    si (!visus || !eventus)
    {
        redde FALSUM;
    }

    /* Handle keyboard */
    si (eventus->genus == EVENTUS_CLAVIS_DEPRESSUS)
    {
        si (visus->in_toc)
        {
            /* In TOC: only Escape exits */
            si (eventus->datum.clavis.clavis == CLAVIS_EFFUGIUM)
            {
                /* Signal to exit widget (handled by schirmata) */
                redde FALSUM;
            }
        }
        alioquin
        {
            /* In reading view - check character keys first */
            commutatio (eventus->datum.clavis.typus)
            {
                casus 'j':
                    _biblia_visus_pagina_proxima(visus);
                    redde VERUM;

                casus 'k':
                    _biblia_visus_pagina_prior(visus);
                    redde VERUM;

                casus 'l':
                    _biblia_visus_capitulum_proximum(visus);
                    redde VERUM;

                casus 'h':
                    _biblia_visus_capitulum_priorem(visus);
                    redde VERUM;

                ordinarius:
                    frange;
            }

            /* Check special keys */
            si (eventus->datum.clavis.clavis == CLAVIS_EFFUGIUM)
            {
                /* Back to TOC */
                visus->in_toc = VERUM;
                redde VERUM;
            }
        }
    }

    /* Handle mouse click */
    si (eventus->genus == EVENTUS_MUS_DEPRESSUS)
    {
        si (visus->in_toc)
        {
            redde _biblia_visus_tractare_click_toc(visus,
                eventus->datum.mus.x,
                eventus->datum.mus.y);
        }
    }

    redde FALSUM;
}


/* ==================================================
 * Navigatio
 * ================================================== */

/* Parse reference: "Book", "Book N", "Book N:V"
 * Extract book name, chapter (default 1), verse (default 0 = no specific verse)
 */
hic_manens b32
_biblia_visus_parse_referentiam(
    constans Biblia* biblia,
    constans character* referentia,
    i32* liber_out,
    i32* capitulum_out,
    i32* versus_out)
{
    character nomen_buffer[LXIV];
    i32 nomen_idx;
    constans character* p;
    i32 liber;
    i32 capitulum;
    i32 versus;
    b32 in_numero_libri;

    si (!biblia || !referentia || !*referentia)
    {
        redde FALSUM;
    }

    /* Initialize defaults */
    capitulum = I;
    versus = ZEPHYRUM;
    nomen_idx = ZEPHYRUM;
    p = referentia;
    in_numero_libri = FALSUM;

    /* Handle numbered books (e.g., "1 Maccabees", "2 Kings") */
    si (*p >= '1' && *p <= '9')
    {
        nomen_buffer[nomen_idx++] = *p++;
        in_numero_libri = VERUM;

        /* Copy space if present */
        si (*p == ' ')
        {
            nomen_buffer[nomen_idx++] = *p++;
        }
    }

    /* Find end of book name:
     * - If we had a book number prefix, look for next digit
     * - Otherwise, look for digit or end of string
     */
    dum (*p && nomen_idx < LX)
    {
        /* Stop at digit that starts chapter reference */
        si (*p >= '0' && *p <= '9')
        {
            /* But don't stop at digits in book names like "1 Kings" */
            si (!in_numero_libri)
            {
                frange;
            }
            in_numero_libri = FALSUM;
        }

        nomen_buffer[nomen_idx++] = *p++;
    }

    /* Trim trailing space from book name */
    dum (nomen_idx > ZEPHYRUM && nomen_buffer[nomen_idx - I] == ' ')
    {
        nomen_idx--;
    }
    nomen_buffer[nomen_idx] = '\0';

    /* Look up book */
    liber = biblia_invenire_librum(biblia, nomen_buffer);
    si (liber < ZEPHYRUM)
    {
        redde FALSUM;
    }

    /* Parse chapter number if present */
    si (*p >= '0' && *p <= '9')
    {
        capitulum = ZEPHYRUM;
        dum (*p >= '0' && *p <= '9')
        {
            capitulum = capitulum * X + (i32)(*p - '0');
            p++;
        }

        /* Clamp to valid range */
        {
            i32 max_cap;

            max_cap = biblia_capitula_in_libro(biblia, liber);
            si (capitulum < I)
            {
                capitulum = I;
            }
            si (capitulum > max_cap)
            {
                capitulum = max_cap;
            }
        }
    }

    /* Parse verse number if : present */
    si (*p == ':')
    {
        p++;
        si (*p >= '0' && *p <= '9')
        {
            versus = ZEPHYRUM;
            dum (*p >= '0' && *p <= '9')
            {
                versus = versus * X + (i32)(*p - '0');
                p++;
            }
        }
    }

    *liber_out = liber;
    *capitulum_out = capitulum;
    *versus_out = versus;

    redde VERUM;
}

b32
biblia_visus_navigare_ad(
    BibliaVisus* visus,
    constans character* referentia)
{
    i32 liber;
    i32 capitulum;
    i32 versus;

    si (!visus || !referentia || !*referentia)
    {
        redde FALSUM;
    }

    si (!_biblia_visus_parse_referentiam(visus->biblia,
            referentia, &liber, &capitulum, &versus))
    {
        redde FALSUM;
    }

    /* Set navigation state */
    visus->liber_currens = liber;
    visus->capitulum_currens = capitulum;
    visus->in_toc = FALSUM;
    visus->cache_liber = (i32)(-1);  /* Force recalculation */
    visus->cache_capitulum = (i32)(-1);

    /* If verse specified, find page containing it */
    si (versus > ZEPHYRUM)
    {
        /* Need to calculate pagination first */
        _biblia_visus_calculare_paginationem(visus,
            visus->latitudo_characterum > ZEPHYRUM ? visus->latitudo_characterum : LXXX,
            visus->altitudo_linearum > ZEPHYRUM ? visus->altitudo_linearum : XL);

        /* Find page containing verse (0-indexed internally) */
        {
            i32 versus_idx;
            i32 pagina;

            versus_idx = versus - I;  /* Convert to 0-indexed */

            per (pagina = visus->paginae_numerus - I; pagina >= ZEPHYRUM; pagina--)
            {
                si (visus->paginae_limites[pagina] <= versus_idx)
                {
                    visus->index_paginae = pagina;
                    frange;
                }
            }
        }
    }
    alioquin
    {
        visus->index_paginae = ZEPHYRUM;
    }

    redde VERUM;
}
