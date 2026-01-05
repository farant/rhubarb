#include "pagina.h"
#include "chorda.h"
#include "color.h"
#include "tempus.h"
#include "delineare.h"
#include "thema.h"

/* ==================================================
 * Creatio / Initiatio
 * ================================================== */

vacuum
pagina_initiare(
    Pagina* pagina,
    Piscina* piscina,
    chorda* identificator)
{
    /* Initiare cum dimensionibus defaltis */
    pagina_initiare_cum_dimensionibus(
        pagina,
        piscina,
        TABULA_LATITUDO_DEFALTA,
        TABULA_ALTITUDO_DEFALTA,
        identificator);
}

vacuum
pagina_initiare_cum_dimensionibus(
    Pagina* pagina,
    Piscina* piscina,
    i32 latitudo,
    i32 altitudo,
    chorda* identificator)
{
    /* Initiare tabula characterum cum dimensionibus */
    tabula_initiare(&pagina->tabula, piscina, latitudo, altitudo);

    /* Creare coloratio pro syntax highlighting */
    pagina->coloratio = coloratio_creare(piscina, latitudo, altitudo);
    coloratio_ponere_regulas(pagina->coloratio,
        COLORATIO_REGULA_COMMANDA | COLORATIO_REGULA_STML | COLORATIO_REGULA_SPUTNIK);

    /* Initiare vim status */
    pagina->vim = vim_initiare(&pagina->tabula);

    /* Ponere identificator (non copiat - caller possidet) */
    pagina->identificator = identificator;

    /* Initiare cursor blink state */
    pagina->tempus_cursor_ultimus = 0.0;
    pagina->cursor_visibilis = VERUM;
}

vacuum
pagina_vacare(
    Pagina* pagina)
{
    i32 linea;
    i32 columna;

    /* Vacare cellulae - tabula iam allocata est */
    per (linea = ZEPHYRUM; linea < pagina->tabula.altitudo; linea++)
    {
        per (columna = ZEPHYRUM; columna < pagina->tabula.latitudo; columna++)
        {
            tabula_cellula(&pagina->tabula, linea, columna) = ' ';
        }
        pagina->tabula.indentatio[linea] = -I;
    }

    /* Reset vim */
    pagina->vim = vim_initiare(&pagina->tabula);
}


vacuum
pagina_connectere_vim_contextu(
    Pagina* pagina,
    Piscina* piscina,
    VimClipboard* clipboard)
{
    si (pagina == NIHIL || piscina == NIHIL)
    {
        redde;
    }

    /* Connectere clipboard communicatum */
    pagina->vim.clipboard = clipboard;

    /* Creare undo acervum pro hac pagina */
    pagina->vim.undo_acervus = vim_undo_creare(piscina);
}


/* ==================================================
 * Interrogationes
 * ================================================== */

b32
pagina_est_plena(
    constans Pagina* pagina)
{
    redde tabula_est_plena(&pagina->tabula);
}

b32
pagina_est_vacua(
    constans Pagina* pagina)
{
    redde tabula_est_vacua(&pagina->tabula);
}

ModoVim
pagina_obtinere_modum(
    constans Pagina* pagina)
{
    redde pagina->vim.modo;
}


/* ==================================================
 * Utilitas Interna
 * ================================================== */

/* Verificare si linea est in selectione visuali */
hic_manens b32
_est_linea_in_selectio(
    VimStatus* vim,
    i32 linea)
{
    i32 linea_min;
    i32 linea_max;

    si (vim->modo != MODO_VIM_VISUALIS)
    {
        redde FALSUM;
    }

    si (vim->selectio_initium_linea < ZEPHYRUM)
    {
        redde FALSUM;
    }

    linea_min = (vim->selectio_initium_linea < (s32)vim->cursor_linea) ?
        (i32)vim->selectio_initium_linea : vim->cursor_linea;
    linea_max = (vim->selectio_initium_linea > (s32)vim->cursor_linea) ?
        (i32)vim->selectio_initium_linea : vim->cursor_linea;

    redde (linea >= linea_min && linea <= linea_max);
}


/* ==================================================
 * Reddere
 * ================================================== */

vacuum
pagina_reddere(
    TabulaPixelorum* tabula_pixelorum,
    Pagina* pagina,
    i32 x,
    i32 y,
    i32 latitudo,
    i32 altitudo,
    i32 scala)
{
    i32 linea;
    i32 columna;
    i32 character_latitudo;
    i32 character_altitudo;
    i32 pixel_x;
    i32 pixel_y;
    f64 tempus_currens;
    f64 delta_tempus;

    character_latitudo = VI * scala;
    character_altitudo = VIII * scala;

    /* Recomputare coloratio ante reddere */
    si (pagina->coloratio)
    {
        coloratio_computare(pagina->coloratio, &pagina->tabula);
    }

    /* Pingere characteres ex tabula */
    per (linea = ZEPHYRUM; linea < altitudo && linea < pagina->tabula.altitudo; linea++)
    {
        b32 in_selectio;
        i32 linea_pixel_y;

        in_selectio = _est_linea_in_selectio(&pagina->vim, linea);
        linea_pixel_y = (y + linea) * character_altitudo;

        /* Si linea in selectione, pingere background */
        si (in_selectio)
        {
            i32 px;
            i32 py;
            Color sel_bg;

            sel_bg = thema_color(COLOR_SELECTION);

            /* Pingere rectangulum background pro tota linea */
            per (py = ZEPHYRUM; py < character_altitudo; py++)
            {
                per (px = ZEPHYRUM; px < latitudo * character_latitudo; px++)
                {
                    tabula_pixelorum_ponere_pixelum(
                        tabula_pixelorum,
                        x * character_latitudo + px,
                        linea_pixel_y + py,
                        color_ad_pixelum(sel_bg));
                }
            }
        }

        per (columna = ZEPHYRUM; columna < latitudo && columna < pagina->tabula.latitudo; columna++)
        {
            character c;
            i8 color_index;
            Color text_color;

            c = tabula_cellula(&pagina->tabula, linea, columna);

            /* Saltare cellulas vacuas */
            si (c == '\0')
            {
                perge;
            }

            /* Tractare tab ut duo spatia (non pingere, solum saltare)
             * Tab occupat 2 cellulas: '\t' + TAB_CONTINUATIO
             * Ambae saltantur - spatium visuale implicatur per positiones */
            si (c == '\t' || c == TAB_CONTINUATIO)
            {
                perge;
            }

            pixel_x = (x + columna) * character_latitudo;
            pixel_y = linea_pixel_y;

            /* Obtinere colorem ex coloratio */
            color_index = pagina->coloratio ?
                coloratio_obtinere(pagina->coloratio, linea, columna) :
                COLORATIO_DEFALTA;
            text_color = thema_color_ex_indice_colorationis(color_index);

            /* Si in selectione, usare colorem contrastum (white) */
            si (in_selectio)
            {
                text_color = color_ex_palette(PALETTE_WHITE);
            }

            tabula_pixelorum_pingere_characterem(
                tabula_pixelorum,
                pixel_x,
                pixel_y,
                c,
                color_ad_pixelum(text_color));
        }
    }

    /* Update cursor blink state */
    tempus_currens = tempus_nunc();
    delta_tempus = tempus_currens - pagina->tempus_cursor_ultimus;

    /* Blink every 0.5 seconds */
    si (delta_tempus >= 0.5)
    {
        pagina->cursor_visibilis = !pagina->cursor_visibilis;
        pagina->tempus_cursor_ultimus = tempus_currens;
    }

    /* Pingere cursorem */
    si (pagina->cursor_visibilis)
    {
        i32 cursor_pixel_x;
        i32 cursor_pixel_y;
        i32 j;

        cursor_pixel_x = (x + pagina->vim.cursor_columna) * character_latitudo;
        cursor_pixel_y = (y + pagina->vim.cursor_linea) * character_altitudo;

        /* Pingere lineam verticalem pro cursore */
        per (j = ZEPHYRUM; j < character_altitudo; j++)
        {
            tabula_pixelorum_ponere_pixelum(
                tabula_pixelorum,
                cursor_pixel_x,
                cursor_pixel_y + j,
                color_ad_pixelum(thema_color(COLOR_CURSOR)));
        }
    }
}


/* ==================================================
 * Reddere Cum Margine
 * ================================================== */

vacuum
pagina_reddere_cum_margine(
    Piscina* piscina,
    TabulaPixelorum* tabula_pixelorum,
    Pagina* pagina,
    i32 x,
    i32 y,
    i32 latitudo,
    i32 altitudo,
    i32 scala,
    b32 focused)
{
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
    constans character* modo_str;

    character_latitudo = VI * scala;
    character_altitudo = VIII * scala;

    /* Computare dimensiones areae textus (minus border) */
    textus_latitudo = latitudo - II;
    textus_altitudo = altitudo - II;

    /* Computare coordinatas pixelorum pro box (with padding from edges) */
    box_x0 = x * character_latitudo + II;
    box_y0 = y * character_altitudo + II;
    box_x1 = (x + latitudo) * character_latitudo - I - II;
    box_y1 = (y + altitudo) * character_altitudo - I - II;

    color_border = thema_color(COLOR_BORDER);
    color_border_inner = focused ? thema_color(COLOR_BORDER_ACTIVE) : color_border;

    /* Creare contextum delineandi */
    ctx = delineare_creare_contextum(piscina, tabula_pixelorum);

    /* Computare positiones textus */
    titulo = pagina->identificator ? *pagina->identificator : chorda_ex_literis("", piscina);
    modo_str = vim_nomen_modi(pagina->vim.modo);
    modo_textus = chorda_ex_literis(modo_str, piscina);

    {
        i32 titulo_pixel_width;
        i32 modo_pixel_width;
        i32 titulo_x, titulo_y;
        i32 modo_x, modo_y;
        i32 gap_padding;

        gap_padding = character_latitudo;

        titulo_pixel_width = titulo.mensura * character_latitudo;
        modo_pixel_width = modo_textus.mensura * character_latitudo;

        /* Center titulo on top line */
        titulo_x = box_x0 + ((box_x1 - box_x0) - titulo_pixel_width) / II;
        titulo_y = box_y0 - (character_altitudo / II) + III;

        /* Center modo on bottom line */
        modo_x = box_x0 + ((box_x1 - box_x0) - modo_pixel_width) / II;
        modo_y = box_y1 - (character_altitudo / II) + II;

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
    }

    /* Reddere Textum Paginae (interior) */
    pagina_reddere(tabula_pixelorum, pagina, x + I, y + I, textus_latitudo, textus_altitudo, scala);
}


/* ==================================================
 * Tractare Eventus
 * ================================================== */

/* Convertere clavis fenestrae ad clavis vim */
hic_manens s32
convertere_clavem(
    clavis_t clavis,
    character typus)
{
    commutatio (clavis)
    {
        casus CLAVIS_SURSUM:
            redde VIM_CLAVIS_SURSUM;

        casus CLAVIS_DEORSUM:
            redde VIM_CLAVIS_DEORSUM;

        casus CLAVIS_SINISTER:
            redde VIM_CLAVIS_SINISTRAM;

        casus CLAVIS_DEXTER:
            redde VIM_CLAVIS_DEXTRAM;

        casus CLAVIS_RETRORSUM:
            redde VIM_CLAVIS_BACKSPACE;

        casus CLAVIS_DELERE:
            redde VIM_CLAVIS_DELETE;

        casus CLAVIS_REDITUS:
            redde VIM_CLAVIS_ENTER;

        casus CLAVIS_TABULA:
            redde VIM_CLAVIS_TAB;

        casus CLAVIS_EFFUGIUM:
            redde VIM_CLAVIS_ESCAPE;

        casus CLAVIS_DOMUS:
            redde VIM_CLAVIS_HOME;

        casus CLAVIS_FINIS:
            redde VIM_CLAVIS_END;

        ordinarius:
            /* Character typatum */
            si (typus != '\0')
            {
                /* Tractare Enter/Return */
                si (typus == '\n' || typus == '\r')
                {
                    redde VIM_CLAVIS_ENTER;
                }

                /* Tractare Ctrl-[ ut Escape */
                si (typus == XXVII)
                {
                    redde VIM_CLAVIS_ESCAPE;
                }

                redde (s32)typus;
            }

            redde ZEPHYRUM;
    }
}

b32
pagina_tractare_eventum(
    Pagina* pagina,
    constans Eventus* eventus)
{
    s32 clavis_vim;
    f64 tempus;

    si (eventus->genus != EVENTUS_CLAVIS_DEPRESSUS)
    {
        redde VERUM;  /* Ignorare eventus non-clavis */
    }

    /* Reset cursor blink on keypress */
    pagina->cursor_visibilis = VERUM;
    pagina->tempus_cursor_ultimus = tempus_nunc();

    /* Convertere clavem */
    clavis_vim = convertere_clavem(
        eventus->datum.clavis.clavis,
        eventus->datum.clavis.typus);

    si (clavis_vim == ZEPHYRUM)
    {
        redde VERUM;  /* Clavis non recognita */
    }

    /* TAB in modo normali - permittere focus switching */
    si (clavis_vim == VIM_CLAVIS_TAB && pagina->vim.modo == MODO_VIM_NORMALIS)
    {
        redde FALSUM;
    }

    /* Tractare clavem cum vim */
    tempus = tempus_nunc();
    pagina->vim = vim_tractare_clavem_cum_tempore(pagina->vim, clavis_vim, tempus);

    /* Si vim dicit claudere (ESC in normal mode), signalare */
    si (pagina->vim.debet_claudere)
    {
        redde FALSUM;
    }

    redde VERUM;
}


/* ==================================================
 * Functiones Commodi (Convenience Functions)
 * ================================================== */

vacuum
pagina_ponere_cursor(
    Pagina* pagina,
    i32 linea,
    i32 columna)
{
    pagina->vim = vim_ponere_cursor(pagina->vim, linea, columna);
}

vacuum
pagina_inserere_textum(
    Pagina* pagina,
    constans character* textus)
{
    constans character* p;
    ModoVim modo_prior;

    /* Servare modum et intrare in inserere */
    modo_prior = pagina->vim.modo;
    pagina->vim = vim_ponere_modum(pagina->vim, MODO_VIM_INSERERE);

    per (p = textus; *p != '\0'; p++)
    {
        si (*p == '\n')
        {
            /* Inserere novam lineam */
            pagina->vim = vim_tractare_clavem(pagina->vim, VIM_CLAVIS_ENTER);
        }
        alioquin
        {
            /* Inserere characterem normalem */
            pagina->vim = vim_tractare_clavem(pagina->vim, (s32)*p);
        }
    }

    /* Restituere modum priorem */
    pagina->vim = vim_ponere_modum(pagina->vim, modo_prior);
}

vacuum
pagina_inserere_textum_crudus(
    Pagina* pagina,
    constans character* textus)
{
    constans character* p;
    ModoVim modo_prior;

    /* Servare modum et intrare in inserere */
    modo_prior = pagina->vim.modo;
    pagina->vim = vim_ponere_modum(pagina->vim, MODO_VIM_INSERERE);

    /* Disablare auto-indent pro insertion ex STML */
    pagina->vim.sine_auto_indent = VERUM;

    per (p = textus; *p != '\0'; p++)
    {
        si (*p == '\n')
        {
            /* Inserere novam lineam */
            pagina->vim = vim_tractare_clavem(pagina->vim, VIM_CLAVIS_ENTER);
        }
        alioquin
        {
            /* Inserere characterem normalem */
            pagina->vim = vim_tractare_clavem(pagina->vim, (s32)*p);
        }
    }

    /* Restituere modum priorem et auto-indent */
    pagina->vim.sine_auto_indent = FALSUM;
    pagina->vim = vim_ponere_modum(pagina->vim, modo_prior);
}


/* ==================================================
 * Tag Detection
 * ================================================== */

/* Verificare si character est pars verbi */
hic_manens b32
est_character_verbi(
    character c)
{
    redde (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') ||
           (c == '_');
}

/* Verificare si character est pars link (permittit '-' pro nomina paginarum) */
hic_manens b32
est_character_link(
    character c)
{
    redde (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') ||
           (c == '_') || (c == '-') || (c == '.');
}

b32
pagina_obtinere_regio_ad_punctum(
    constans Pagina* pagina,
    i32 linea,
    i32 columna,
    RegioClicca* regio)
{
    s32 col;
    s32 initium_columna;
    s32 finis_columna;
    s32 longitudo;
    character c;
    character tag_char;
    b32 est_link;

    /* Verificare limites */
    si (linea < ZEPHYRUM || linea >= pagina->tabula.altitudo ||
        columna < ZEPHYRUM || columna >= pagina->tabula.latitudo)
    {
        redde FALSUM;
    }

    /* Scandere retro ad invenire $ vel # */
    initium_columna = -I;
    tag_char = '\0';
    per (col = (s32)columna; col >= ZEPHYRUM; col--)
    {
        c = tabula_cellula(&pagina->tabula, linea, (i32)col);

        si (c == '$' || c == '#')
        {
            initium_columna = col;
            tag_char = c;
            frange;
        }

        /* Stop at whitespace or null */
        si (c == ' ' || c == '\t' || c == '\0')
        {
            redde FALSUM;
        }
    }

    si (initium_columna == -I)
    {
        redde FALSUM;
    }

    est_link = (tag_char == '#');

    /* Scandere ad finem tag */
    finis_columna = (s32)initium_columna + I;
    per (col = finis_columna; col < (s32)pagina->tabula.latitudo; col++)
    {
        c = tabula_cellula(&pagina->tabula, linea, (i32)col);

        si (est_link)
        {
            /* Links permittunt '-' */
            si (c == ' ' || c == '\t' || c == '\0' || !est_character_link(c))
            {
                finis_columna = col;
                frange;
            }
        }
        alioquin
        {
            /* Commands permittunt '-' (e.g., $sputnik-syntax) */
            si (c == ' ' || c == '\t' || c == '\0' || (!est_character_verbi(c) && c != '-'))
            {
                finis_columna = col;
                frange;
            }
        }
    }

    /* Extrahere nomen (sine $ vel #) */
    longitudo = finis_columna - ((s32)initium_columna + I);
    si (longitudo >= LXIV)
    {
        longitudo = LXIV - I;
    }

    si (longitudo > ZEPHYRUM)
    {
        /* Pro links, includere # in datum */
        si (est_link)
        {
            regio->datum[0] = '#';
            per (col = ZEPHYRUM; col < longitudo; col++)
            {
                regio->datum[col + I] = tabula_cellula(&pagina->tabula, linea, (i32)((s32)initium_columna + I + col));
            }
            regio->datum[longitudo + I] = '\0';
        }
        alioquin
        {
            per (col = ZEPHYRUM; col < longitudo; col++)
            {
                regio->datum[col] = tabula_cellula(&pagina->tabula, linea, (i32)((s32)initium_columna + I + col));
            }
            regio->datum[longitudo] = '\0';
        }

        regio->initium_linea = linea;
        regio->initium_columna = (i32)initium_columna;
        regio->finis_linea = linea;
        regio->finis_columna = (i32)finis_columna;

        si (est_link)
        {
            /* "link" */
            regio->genus[0] = 'l';
            regio->genus[1] = 'i';
            regio->genus[2] = 'n';
            regio->genus[3] = 'k';
            regio->genus[4] = '\0';
        }
        alioquin
        {
            /* "command" */
            regio->genus[0] = 'c';
            regio->genus[1] = 'o';
            regio->genus[2] = 'm';
            regio->genus[3] = 'm';
            regio->genus[4] = 'a';
            regio->genus[5] = 'n';
            regio->genus[6] = 'd';
            regio->genus[7] = '\0';
        }

        redde VERUM;
    }

    redde FALSUM;
}
