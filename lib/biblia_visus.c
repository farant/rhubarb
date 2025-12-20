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
    visus->latitudo_characterum = ZEPHYRUM;
    visus->altitudo_linearum = ZEPHYRUM;
    visus->scala = I;
    visus->widget_x = ZEPHYRUM;
    visus->widget_y = ZEPHYRUM;

    redde visus;
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

    per (versus_idx = visus->versus_initium; versus_idx < versus_in_capitulo; versus_idx++)
    {
        chorda versus;
        character versus_buffer[DXII];  /* 512 chars max */
        i32 versus_longitudo;
        i32 lineae_versus;
        i32 pos;
        i32 linea_start;

        versus = biblia_versus(visus->biblia,
            visus->liber_currens, visus->capitulum_currens, versus_idx + I);

        si (versus.mensura == ZEPHYRUM)
        {
            perge;
        }

        /* Format: "N text..." */
        sprintf(versus_buffer, "%d ", versus_idx + I);
        versus_longitudo = (i32)strlen(versus_buffer);

        /* Copy verse text, truncate if needed */
        {
            i32 copy_len;

            copy_len = versus.mensura;
            si (copy_len > DX - versus_longitudo)  /* 510 - prefix */
            {
                copy_len = DX - versus_longitudo;
            }
            memcpy(versus_buffer + versus_longitudo, versus.datum, (size_t)copy_len);
            versus_longitudo += copy_len;
            versus_buffer[versus_longitudo] = '\0';
        }

        /* Calculate lines needed for this verse */
        lineae_versus = (versus_longitudo + chars_disponibiles - I) / chars_disponibiles;

        /* Check if verse fits */
        si (lineae_usae + lineae_versus > lineae_disponibiles)
        {
            frange;  /* Stop - don't break mid-verse */
        }

        /* Render verse line by line */
        pos = ZEPHYRUM;
        linea_start = linea;
        dum (pos < versus_longitudo && linea < y + altitudo - II)
        {
            character line_buffer[CXXVIII];
            i32 line_len;
            chorda line_chorda;

            line_len = chars_disponibiles;
            si (pos + line_len > versus_longitudo)
            {
                line_len = versus_longitudo - pos;
            }

            memcpy(line_buffer, versus_buffer + pos, (size_t)line_len);
            line_buffer[line_len] = '\0';
            line_chorda = _chorda_ex_cstr(line_buffer);

            tabula_pixelorum_pingere_chordam_scalatam(tabula,
                (x + PADDING) * char_lat, linea * char_alt,
                line_chorda, pixelum_text, scala);

            pos += line_len;
            linea++;
        }

        lineae_usae = linea - linea_start + lineae_usae;
    }

    /* === Footer: Page info === */
    {
        i32 pagina_currens;
        i32 paginae_totales;
        i32 footer_y;

        /* Rough estimate of pages */
        pagina_currens = (visus->versus_initium / X) + I;
        paginae_totales = (versus_in_capitulo / X) + I;
        si (paginae_totales < I) paginae_totales = I;

        footer_y = y + altitudo - I;
        sprintf(nav_buffer, "Page %d/%d", pagina_currens, paginae_totales);
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
    i32 versus_in_capitulo;
    i32 versus_per_pagina;

    versus_in_capitulo = biblia_versus_in_capitulo(visus->biblia,
        visus->liber_currens, visus->capitulum_currens);

    /* Rough estimate: 10 verses per page */
    versus_per_pagina = X;

    visus->versus_initium += versus_per_pagina;

    si (visus->versus_initium >= versus_in_capitulo)
    {
        /* Go to next chapter */
        i32 capitula_in_libro;

        capitula_in_libro = biblia_capitula_in_libro(visus->biblia, visus->liber_currens);

        si (visus->capitulum_currens < capitula_in_libro)
        {
            visus->capitulum_currens++;
            visus->versus_initium = ZEPHYRUM;
        }
        alioquin
        {
            /* Stay at end */
            visus->versus_initium = versus_in_capitulo - versus_per_pagina;
            si (visus->versus_initium < ZEPHYRUM)
            {
                visus->versus_initium = ZEPHYRUM;
            }
        }
    }
}

hic_manens vacuum
_biblia_visus_pagina_prior(
    BibliaVisus* visus)
{
    i32 versus_per_pagina;

    versus_per_pagina = X;

    visus->versus_initium -= versus_per_pagina;

    si (visus->versus_initium < ZEPHYRUM)
    {
        /* Go to previous chapter */
        si (visus->capitulum_currens > I)
        {
            visus->capitulum_currens--;
            /* Go to last page of previous chapter */
            {
                i32 versus_in_cap;

                versus_in_cap = biblia_versus_in_capitulo(visus->biblia,
                    visus->liber_currens, visus->capitulum_currens);
                visus->versus_initium = ((versus_in_cap - I) / versus_per_pagina) * versus_per_pagina;
                si (visus->versus_initium < ZEPHYRUM)
                {
                    visus->versus_initium = ZEPHYRUM;
                }
            }
        }
        alioquin
        {
            visus->versus_initium = ZEPHYRUM;
        }
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
        visus->versus_initium = ZEPHYRUM;
    }
    alioquin si (visus->liber_currens < LIBRI_NUMERUS - I)
    {
        /* Go to next book */
        visus->liber_currens++;
        visus->capitulum_currens = I;
        visus->versus_initium = ZEPHYRUM;
    }
}

hic_manens vacuum
_biblia_visus_capitulum_priorem(
    BibliaVisus* visus)
{
    si (visus->capitulum_currens > I)
    {
        visus->capitulum_currens--;
        visus->versus_initium = ZEPHYRUM;
    }
    alioquin si (visus->liber_currens > ZEPHYRUM)
    {
        /* Go to previous book, last chapter */
        visus->liber_currens--;
        visus->capitulum_currens = biblia_capitula_in_libro(visus->biblia, visus->liber_currens);
        visus->versus_initium = ZEPHYRUM;
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
                visus->versus_initium = ZEPHYRUM;
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
                visus->versus_initium = ZEPHYRUM;
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
