/* ==================================================
 * CALENDARIO VISUS
 * Widget pro calendarium liturgicum mensile
 *
 * Monstra grillam mensis cum coloribus liturgicis
 * et detalia diei selecti.
 * ================================================== */

#include "calendario_visus.h"
#include "color.h"
#include "registrum_commandi.h"
#include "delineare.h"
#include <string.h>
#include <stdio.h>


/* ==================================================
 * Constantae
 * ================================================== */

/* Nomina dierum hebdomadae (abbreviata) */
interior constans character* NOMINA_DIERUM[] = {
    "Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"
};

/* Nomina mensium */
interior constans character* NOMINA_MENSIUM[] = {
    "",
    "January", "February", "March", "April",
    "May", "June", "July", "August",
    "September", "October", "November", "December"
};


/* ==================================================
 * Colores Liturgici ad RGB
 * ================================================== */

interior i32
_color_liturgicus_ad_pixelum(ColorLiturgicus color)
{
    Color c;

    commutatio (color) {
        casus COLLIT_ALBUS:
            c = color_ex_rgb((i8)255, (i8)255, (i8)255);
            frange;
        casus COLLIT_RUBER:
            c = color_ex_rgb((i8)220, (i8)50, (i8)50);
            frange;
        casus COLLIT_VIRIDIS:
            c = color_ex_rgb((i8)50, (i8)180, (i8)80);
            frange;
        casus COLLIT_VIOLACEUS:
            c = color_ex_rgb((i8)140, (i8)80, (i8)180);
            frange;
        casus COLLIT_ROSACEUS:
            c = color_ex_rgb((i8)240, (i8)150, (i8)180);
            frange;
        casus COLLIT_NIGER:
            c = color_ex_rgb((i8)100, (i8)100, (i8)100);
            frange;
        ordinarius:
            c = color_ex_rgb((i8)200, (i8)200, (i8)200);
    }

    redde color_ad_pixelum(c);
}


/* ==================================================
 * Functiones Auxiliares
 * ================================================== */

/*
 * _computare_mensis
 *
 * Computare proprietates mensis currentis.
 */
interior vacuum
_computare_mensis(CalendarioVisus* visus)
{
    Dies prima;

    /* Prima dies mensis */
    prima = fasti_dies(visus->annus_visus, visus->mensis_visus, I);

    /* Dies hebdomadae primae diei (0=Dominica) */
    visus->primus_dies_hebdomadae = fasti_dies_hebdomadis(prima);

    /* Numerus dierum in mense */
    visus->numerus_dierum = fasti_dies_in_mense(
        visus->mensis_visus,
        visus->annus_visus
    );

    /* Verificare dies selectus */
    si (visus->dies_selectus > visus->numerus_dierum) {
        visus->dies_selectus = visus->numerus_dierum;
    }
    si (visus->dies_selectus < I) {
        visus->dies_selectus = I;
    }
}


/*
 * _navigare_mensis
 *
 * Navigare ad mensis proximum vel priorem.
 * delta: +1 = proximus, -1 = prior
 */
interior vacuum
_navigare_mensis(CalendarioVisus* visus, s32 delta)
{
    visus->mensis_visus += delta;

    si (visus->mensis_visus > XII) {
        visus->mensis_visus = I;
        visus->annus_visus++;
    } alioquin si (visus->mensis_visus < I) {
        visus->mensis_visus = XII;
        visus->annus_visus--;
    }

    _computare_mensis(visus);
}


/*
 * _navigare_annus
 *
 * Navigare ad annum proximum vel priorem.
 */
interior vacuum
_navigare_annus(CalendarioVisus* visus, s32 delta)
{
    visus->annus_visus += delta;
    _computare_mensis(visus);
}


/*
 * _navigare_dies
 *
 * Navigare ad diem proximam vel priorem.
 */
interior vacuum
_navigare_dies(CalendarioVisus* visus, s32 delta)
{
    visus->dies_selectus += delta;

    si (visus->dies_selectus > visus->numerus_dierum) {
        /* Ad mensem proximum */
        _navigare_mensis(visus, I);
        visus->dies_selectus = I;
    } alioquin si (visus->dies_selectus < I) {
        /* Ad mensem priorem */
        _navigare_mensis(visus, -I);
        visus->dies_selectus = visus->numerus_dierum;
    }
}


/* ==================================================
 * Reddere - Detalia Diei
 * ================================================== */

interior vacuum
_reddere_details(
    CalendarioVisus* visus,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala)
{
    Dies dies_sel;
    InformatioDiei* info;
    Piscina* piscina_temp;
    character buffer[C];
    s32 longitudo;
    i32 linea;
    i32 col;
    i32 pixel_x;
    i32 pixel_y;
    i32 char_width;
    i32 char_height;
    i32 color_text;
    i32 color_dim;
    Color c_text;
    Color c_dim;

    char_width = VI * scala;
    char_height = VIII * scala;
    c_text = color_ex_rgb((i8)220, (i8)220, (i8)220);
    c_dim = color_ex_rgb((i8)140, (i8)140, (i8)140);
    color_text = color_ad_pixelum(c_text);
    color_dim = color_ad_pixelum(c_dim);

    /* Creare piscina temporaria pro informatione diei */
    piscina_temp = piscina_generare_dynamicum("calendario_temp", M * LXIV);
    si (piscina_temp == NIHIL) {
        redde;
    }

    /* Obtinere informationes diei selecti */
    dies_sel = fasti_dies(visus->annus_visus, visus->mensis_visus, visus->dies_selectus);
    info = calendarium_obtinere_diem(visus->cal, dies_sel, piscina_temp);

    linea = ZEPHYRUM;

    /* Linea 1: Mensis et Annus */
    longitudo = snprintf(buffer, C, "%s %d",
        NOMINA_MENSIUM[visus->mensis_visus],
        visus->annus_visus);

    pixel_y = y * char_height;
    per (col = ZEPHYRUM; col < (i32)longitudo && col < latitudo; col++) {
        pixel_x = (x + col) * char_width;
        tabula_pixelorum_pingere_characterem_scalatum(
            tabula, pixel_x, pixel_y,
            buffer[col], color_text, scala
        );
    }
    linea++;

    /* Linea 2: Numerus diei */
    longitudo = snprintf(buffer, C, "%d %s %d",
        visus->dies_selectus,
        NOMINA_MENSIUM[visus->mensis_visus],
        visus->annus_visus);

    pixel_y = (y + linea) * char_height;
    per (col = ZEPHYRUM; col < (i32)longitudo && col < latitudo; col++) {
        pixel_x = (x + col) * char_width;
        tabula_pixelorum_pingere_characterem_scalatum(
            tabula, pixel_x, pixel_y,
            buffer[col], color_dim, scala
        );
    }
    linea++;

    si (info != NIHIL) {
        /* Linea 3: Titulus diei (celebratio) */
        si (info->titulus_diei.mensura > ZEPHYRUM && info->titulus_diei.datum != NIHIL) {
            i32 max_chars = latitudo - II;
            i32 chars_to_copy = info->titulus_diei.mensura;
            si (chars_to_copy > max_chars) {
                chars_to_copy = max_chars;
            }

            pixel_y = (y + linea) * char_height;
            per (col = ZEPHYRUM; col < chars_to_copy; col++) {
                pixel_x = (x + col) * char_width;
                tabula_pixelorum_pingere_characterem_scalatum(
                    tabula, pixel_x, pixel_y,
                    (character)info->titulus_diei.datum[col], color_text, scala
                );
            }
            linea++;
        }

        /* Linea 4: Tempus liturgicum */
        {
            chorda nomen_temp = calendarium_nomen_temporis(
                info->tempus_info.tempus, FALSUM, piscina_temp);

            si (nomen_temp.mensura > ZEPHYRUM && nomen_temp.datum != NIHIL) {
                pixel_y = (y + linea) * char_height;
                per (col = ZEPHYRUM; col < nomen_temp.mensura && col < latitudo; col++) {
                    pixel_x = (x + col) * char_width;
                    tabula_pixelorum_pingere_characterem_scalatum(
                        tabula, pixel_x, pixel_y,
                        (character)nomen_temp.datum[col], color_dim, scala
                    );
                }
                linea++;
            }
        }

        /* Linea 5: Cyclus lectionum */
        {
            constans character* cyclus_lit;
            commutatio (info->cyclus.cyclus_dominicalis) {
                casus CYCLUS_A: cyclus_lit = "Year A"; frange;
                casus CYCLUS_B: cyclus_lit = "Year B"; frange;
                casus CYCLUS_C: cyclus_lit = "Year C"; frange;
                ordinarius: cyclus_lit = ""; frange;
            }

            longitudo = snprintf(buffer, C, "Lectionary: %s, Week %d",
                cyclus_lit,
                info->cyclus.hebdomada_psalterii);

            pixel_y = (y + linea) * char_height;
            per (col = ZEPHYRUM; col < (i32)longitudo && col < latitudo; col++) {
                pixel_x = (x + col) * char_width;
                tabula_pixelorum_pingere_characterem_scalatum(
                    tabula, pixel_x, pixel_y,
                    buffer[col], color_dim, scala
                );
            }
            linea++;
        }

        /* Linea 6: Celebrationes sanctorales */
        si (info->numerus_celebrationum > I && info->celebrationes != NIHIL) {
            i32 idx;
            per (idx = I; idx < (i32)info->numerus_celebrationum && linea < altitudo; idx++) {
                Celebratio* celeb = &info->celebrationes[idx];
                si (celeb->titulus.mensura > ZEPHYRUM && celeb->titulus.datum != NIHIL) {
                    i32 max_chars = latitudo - II;
                    i32 chars_to_copy = celeb->titulus.mensura;
                    si (chars_to_copy > max_chars) {
                        chars_to_copy = max_chars;
                    }

                    pixel_y = (y + linea) * char_height;
                    per (col = ZEPHYRUM; col < chars_to_copy; col++) {
                        pixel_x = (x + col) * char_width;
                        tabula_pixelorum_pingere_characterem_scalatum(
                            tabula, pixel_x, pixel_y,
                            (character)celeb->titulus.datum[col], color_dim, scala
                        );
                    }
                    linea++;
                }
            }
        }
    }

    piscina_destruere(piscina_temp);
}


/* ==================================================
 * Reddere - Grilla Mensis
 * ================================================== */

interior vacuum
_reddere_grid(
    CalendarioVisus* visus,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused)
{
    s32 char_width;
    s32 char_height;
    s32 cell_width;
    s32 cell_height;
    s32 cell_pixel_width;
    s32 cell_pixel_height;
    i32 pixel_x;
    i32 pixel_y;
    s32 col_idx;
    s32 row_idx;
    s32 dies;
    s32 dies_pos;
    character buffer[VIII];
    s32 longitudo;
    s32 i;
    i32 color_header;
    i32 color_dies;
    i32 color_selected_bg;
    Color c;
    Color color_grid;
    Piscina* piscina_temp;
    ContextusDelineandi* ctx_delin;

    char_width = VI * (s32)scala;
    char_height = VIII * (s32)scala;

    /* Latitudo cellae = latitudo / 7 */
    cell_width = (s32)latitudo / VII;
    cell_height = ((s32)altitudo - I) / VII;  /* -1 pro header, /7 pro 6 rows + header */

    si (cell_height < I) cell_height = I;

    /* Dimensiones cellae in pixelibus */
    cell_pixel_width = cell_width * char_width;
    cell_pixel_height = cell_height * char_height;

    c = color_ex_rgb((i8)100, (i8)100, (i8)100);
    color_header = color_ad_pixelum(c);

    c = color_ex_rgb((i8)60, (i8)60, (i8)80);
    color_selected_bg = color_ad_pixelum(c);

    /* Color pro grilla */
    color_grid = color_ex_rgb((i8)50, (i8)50, (i8)60);

    /* Reddere header (Su Mo Tu We Th Fr Sa) */
    per (i = ZEPHYRUM; i < VII; i++) {
        pixel_x = (i32)(((s32)x + i * cell_width) * char_width);
        pixel_y = (i32)((s32)y * char_height);

        tabula_pixelorum_pingere_characterem_scalatum(
            tabula, pixel_x, pixel_y,
            NOMINA_DIERUM[i][0], color_header, scala
        );
        tabula_pixelorum_pingere_characterem_scalatum(
            tabula, pixel_x + (i32)char_width, pixel_y,
            NOMINA_DIERUM[i][1], color_header, scala
        );
    }

    /* Creare piscina temporaria */
    piscina_temp = piscina_generare_dynamicum("cal_grid_temp", M * XVI);
    si (piscina_temp == NIHIL) {
        redde;
    }

    /* Creare contextum delineandi */
    ctx_delin = delineare_creare_contextum(piscina_temp, tabula);

    /* Obtinere diem hodiernam pro comparatione */
    {
        Dies hodie = fasti_dies_hodie();

        /* Reddere dies mensis */
        per (dies = I; dies <= visus->numerus_dierum; dies++) {
            Dies dies_obj;
            ColorLiturgicus lit_color;
            b32 est_hodie;

            /* Computare positio in grilla */
            dies_pos = dies + visus->primus_dies_hebdomadae - I;
            col_idx = dies_pos % VII;
            row_idx = dies_pos / VII;

            /* Obtinere colorem liturgicum */
            dies_obj = fasti_dies(visus->annus_visus, visus->mensis_visus, dies);
            lit_color = calendarium_color_temporis(visus->cal, dies_obj);
            color_dies = _color_liturgicus_ad_pixelum(lit_color);

            /* Verificare si est dies hodierna */
            est_hodie = (dies_obj.annus == hodie.annus &&
                         dies_obj.mensis == hodie.mensis &&
                         dies_obj.dies == hodie.dies);

            /* Positio in characteribus */
            pixel_x = (i32)(((s32)x + col_idx * cell_width) * char_width);
            pixel_y = (i32)(((s32)y + I + row_idx * cell_height) * char_height);

            /* Delineare rectangulum circa cellam */
            si (ctx_delin != NIHIL) {
                delineare_rectangulum(
                    ctx_delin,
                    pixel_x,
                    pixel_y,
                    (i32)cell_pixel_width,
                    (i32)cell_pixel_height,
                    color_grid
                );
            }

            /* Si est dies hodierna, pingere background cum textura */
            si (est_hodie && ctx_delin != NIHIL) {
                Color color_bg_gray;
                Color color_fg_gold;

                /* Colores pro die hodierna ex palette thema */
                color_fg_gold = color_ex_palette(XI);   /* 11: Bright yellow-gold */
                color_bg_gray = color_ex_palette(IV);   /* 4: Light gray (warm) */

                /* Primo: implere cum colore griseo (background) */
                delineare_ponere_modum(ctx_delin, MODUS_SOLIDUS);
                delineare_rectangulum_plenum(
                    ctx_delin,
                    pixel_x + I,
                    pixel_y + I,
                    (i32)cell_pixel_width - II,
                    (i32)cell_pixel_height - II,
                    color_bg_gray
                );

                /* Secundo: superponere exemplar cum colore aureo */
                delineare_ponere_modum(ctx_delin, MODUS_EXEMPLAR);
                delineare_ponere_exemplar_internum(ctx_delin, EXEMPLAR_PUNCTA_DUPLICIA);
                delineare_rectangulum_plenum(
                    ctx_delin,
                    pixel_x + I,
                    pixel_y + I,
                    (i32)cell_pixel_width - II,
                    (i32)cell_pixel_height - II,
                    color_fg_gold
                );

                /* Restituere modum solidum */
                delineare_ponere_modum(ctx_delin, MODUS_SOLIDUS);
            }
            /* Si dies selectus et focused (et non hodie), pingere background */
            alioquin si (dies == visus->dies_selectus && focused) {
                s32 bg_x;
                s32 bg_y;
                per (bg_y = ZEPHYRUM; bg_y < char_height; bg_y++) {
                    per (bg_x = ZEPHYRUM; bg_x < cell_width * char_width; bg_x++) {
                        tabula_pixelorum_ponere_pixelum(
                            tabula,
                            pixel_x + (i32)bg_x,
                            pixel_y + (i32)bg_y,
                            color_selected_bg
                        );
                    }
                }
            }

        /* Formare numerum diei */
        longitudo = snprintf(buffer, VIII, "%2d", dies);

        /* Pingere characteres (cum offset 5px) */
        {
            i32 text_x = pixel_x + V;
            i32 text_y = pixel_y + V;

            per (i = ZEPHYRUM; i < longitudo; i++) {
                tabula_pixelorum_pingere_characterem_scalatum(
                    tabula,
                    text_x + (i32)(i * char_width),
                    text_y,
                    buffer[i],
                    color_dies,
                    scala
                );
            }

            /* Si dies selectus, addere indicatorem */
            si (dies == visus->dies_selectus) {
                /* Addere brackets circa diem */
                si (focused) {
                    tabula_pixelorum_pingere_characterem_scalatum(
                        tabula,
                        text_x - (i32)char_width,
                        text_y,
                        '[',
                        color_dies,
                        scala
                    );
                    tabula_pixelorum_pingere_characterem_scalatum(
                        tabula,
                        text_x + (i32)(longitudo * char_width),
                        text_y,
                        ']',
                        color_dies,
                        scala
                    );
                }
            }
        }
    }
    }  /* Finis block hodie */

    piscina_destruere(piscina_temp);
}


/* ==================================================
 * Creatio
 * ================================================== */

CalendarioVisus*
calendario_visus_creare(Piscina* piscina)
{
    CalendarioVisus* visus;
    Dies hodie;

    visus = (CalendarioVisus*)piscina_allocare(piscina, magnitudo(CalendarioVisus));

    si (visus == NIHIL) {
        redde NIHIL;
    }

    visus->piscina = piscina;

    /* Creare calendarium liturgicum */
    visus->cal = calendarium_creare(piscina);
    si (visus->cal == NIHIL) {
        redde NIHIL;
    }

    /* Initiare ad diem hodiernum */
    hodie = fasti_dies_hodie();
    visus->annus_visus = hodie.annus;
    visus->mensis_visus = hodie.mensis;
    visus->dies_selectus = hodie.dies;

    _computare_mensis(visus);

    redde visus;
}


/* ==================================================
 * Reddere
 * ================================================== */

vacuum
calendario_visus_reddere(
    CalendarioVisus* visus,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused)
{
    i32 altitudo_details;
    i32 altitudo_grid;

    si (visus == NIHIL || tabula == NIHIL) {
        redde;
    }

    /* Salvare dimensiones pro click detection */
    visus->widget_x = x;
    visus->widget_y = y;
    visus->latitudo = latitudo;
    visus->altitudo = altitudo;
    visus->scala = scala;

    /* Dividere altitudinem: 1/3 details, 2/3 grid */
    altitudo_details = altitudo / III;
    altitudo_grid = altitudo - altitudo_details;

    /* Reddere pars superior: detalia */
    _reddere_details(visus, tabula, x, y, latitudo, altitudo_details, scala);

    /* Reddere pars inferior: grilla */
    _reddere_grid(visus, tabula, x, y + altitudo_details, latitudo, altitudo_grid, scala, focused);
}


/* ==================================================
 * Eventus
 * ================================================== */

b32
calendario_visus_tractare_eventum(
    CalendarioVisus*  visus,
    constans Eventus* eventus)
{
    si (visus == NIHIL || eventus == NIHIL) {
        redde FALSUM;
    }

    si (eventus->genus == EVENTUS_CLAVIS_DEPRESSUS) {
        i32 clavis = eventus->datum.clavis.clavis;

        commutatio (clavis) {
            /* h = sinistra (dies prior) */
            casus 'h':
                _navigare_dies(visus, -I);
                redde VERUM;

            /* l = dextra (dies proximus) */
            casus 'l':
                _navigare_dies(visus, I);
                redde VERUM;

            /* k = sursum (hebdomada prior) */
            casus 'k':
                _navigare_dies(visus, -VII);
                redde VERUM;

            /* j = deorsum (hebdomada proxima) */
            casus 'j':
                _navigare_dies(visus, VII);
                redde VERUM;

            /* H = mensis prior */
            casus 'H':
                _navigare_mensis(visus, -I);
                redde VERUM;

            /* L = mensis proximus */
            casus 'L':
                _navigare_mensis(visus, I);
                redde VERUM;

            /* K = annus prior */
            casus 'K':
                _navigare_annus(visus, -I);
                redde VERUM;

            /* J = annus proximus */
            casus 'J':
                _navigare_annus(visus, I);
                redde VERUM;

            /* t = hodie */
            casus 't':
                calendario_visus_hodie(visus);
                redde VERUM;

            ordinarius:
                frange;
        }
    }

    redde FALSUM;
}


/* ==================================================
 * Navigatio
 * ================================================== */

vacuum
calendario_visus_navigare_ad(CalendarioVisus* visus, Dies dies)
{
    si (visus == NIHIL) {
        redde;
    }

    visus->annus_visus = dies.annus;
    visus->mensis_visus = dies.mensis;
    visus->dies_selectus = dies.dies;

    _computare_mensis(visus);
}

vacuum
calendario_visus_hodie(CalendarioVisus* visus)
{
    Dies hodie;

    si (visus == NIHIL) {
        redde;
    }

    hodie = fasti_dies_hodie();
    calendario_visus_navigare_ad(visus, hodie);
}


/* ==================================================
 * Lifecycle
 * ================================================== */

/* Command handler pro :calendario */
interior b32
_calendario_command_handler(ContextusCommandi* ctx)
{
    ContextusWidget* widget_ctx;

    widget_ctx = (ContextusWidget*)ctx->datum_registratus;

    widget_ctx->commutare_widget(
        widget_ctx->schirmata_datum,
        "calendario",
        NIHIL);

    redde VERUM;
}

vacuum
calendario_visus_init(ContextusWidget* ctx)
{
    si (!ctx || !ctx->reg_commandi)
    {
        redde;
    }

    registrum_commandi_registrare(
        ctx->reg_commandi,
        "calendario",
        _calendario_command_handler,
        ctx);

    registrum_commandi_registrare(
        ctx->reg_commandi,
        "cal",
        _calendario_command_handler,
        ctx);
}
