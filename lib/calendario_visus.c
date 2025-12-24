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
#include "chorda_aedificator.h"
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
 * Colores Liturgici ex Palette Thema
 * ================================================== */

interior i32
_color_liturgicus_ad_pixelum(ColorLiturgicus color)
{
    commutatio (color) {
        casus COLLIT_ALBUS:
            redde color_ad_pixelum(color_ex_palette(V));     /* #5: White */
        casus COLLIT_RUBER:
            redde color_ad_pixelum(color_ex_palette(IX));    /* #9: Red */
        casus COLLIT_VIRIDIS:
            redde color_ad_pixelum(color_ex_palette(XII));   /* #12: Green */
        casus COLLIT_VIOLACEUS:
            redde color_ad_pixelum(color_ex_palette(II));    /* #2: Purple */
        casus COLLIT_ROSACEUS:
            redde color_ad_pixelum(color_ex_palette(VIII));  /* #8: Pink */
        casus COLLIT_NIGER:
            redde color_ad_pixelum(color_ex_palette(III));   /* #3: Gray */
        ordinarius:
            redde color_ad_pixelum(color_ex_palette(IV));    /* #4: Light gray */
    }
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
    i32 linea_pixel;
    i32 col;
    i32 pixel_x;
    i32 pixel_y;
    i32 char_width;
    i32 char_height;
    i32 char_width_large;
    i32 char_height_large;
    i32 scala_large;
    i32 color_text;
    i32 color_feast;
    i32 color_lectionary;
    i32 color_tempus;
    i32 offset_x;
    i32 offset_y;

    /* Scala normalis et magna (2x) */
    scala_large = scala * II;
    char_width = VI * scala;
    char_height = VIII * scala;
    char_width_large = VI * scala_large;
    char_height_large = VIII * scala_large;

    /* Offset pro textu (25 pixels dextrorsum et deorsum) */
    offset_x = x * char_width + XXV;
    offset_y = y * char_height + XXV;

    color_text = color_ad_pixelum(color_ex_palette(V));  /* #5: White */

    /* Colores specifici ex palette */
    color_feast = color_ad_pixelum(color_ex_palette(XI));      /* #11: Yellow */
    color_lectionary = color_ad_pixelum(color_ex_palette(VI)); /* #6 */

    /* Creare piscina temporaria pro informatione diei */
    piscina_temp = piscina_generare_dynamicum("calendario_temp", M * LXIV);
    si (piscina_temp == NIHIL) {
        redde;
    }

    /* Obtinere informationes diei selecti */
    dies_sel = fasti_dies(visus->annus_visus, visus->mensis_visus, visus->dies_selectus);
    info = calendarium_obtinere_diem(visus->cal, dies_sel, piscina_temp);

    linea_pixel = offset_y;

    /* Linea 1: Data completa Anglice (2x, white) */
    /* e.g. "Wednesday, December 25th 2025" */
    {
        ChordaAedificator* aed;
        chorda chorda_date;

        aed = chorda_aedificator_creare(piscina_temp, CXXVIII);
        si (aed != NIHIL) {
            fasti_scribere_diem(aed, dies_sel, FASTI_FORMA_ANGLICA_LONGA);
            chorda_date = chorda_aedificator_spectare(aed);

            pixel_y = linea_pixel;
            per (col = ZEPHYRUM; col < (i32)chorda_date.mensura && col < latitudo; col++) {
                pixel_x = offset_x + col * char_width_large;
                tabula_pixelorum_pingere_characterem_scalatum(
                    tabula, pixel_x, pixel_y,
                    (character)chorda_date.datum[col], color_text, scala_large
                );
            }
        }
    }
    linea_pixel += char_height_large;

    /* Linea 3: Hebdomada anni (Week N) - white */
    {
        s32 hebdomada = fasti_hebdomada_anni(dies_sel);
        longitudo = snprintf(buffer, C, "(Week %d)", hebdomada);

        pixel_y = linea_pixel;
        per (col = ZEPHYRUM; col < (i32)longitudo && col < latitudo; col++) {
            pixel_x = offset_x + col * char_width;
            tabula_pixelorum_pingere_characterem_scalatum(
                tabula, pixel_x, pixel_y,
                buffer[col], color_text, scala
            );
        }
        linea_pixel += char_height;
        linea_pixel += char_height / II;  /* Extra spatium post hebdomadam */
    }

    si (info != NIHIL) {
        /* Obtinere colorem liturgicum pro tempus */
        color_tempus = _color_liturgicus_ad_pixelum(info->color_diei);

        /* Linea 4: Tempus liturgicum - liturgical color */
        {
            chorda nomen_temp = calendarium_nomen_temporis(
                info->tempus_info.tempus, FALSUM, piscina_temp);

            si (nomen_temp.mensura > ZEPHYRUM && nomen_temp.datum != NIHIL) {
                pixel_y = linea_pixel;
                per (col = ZEPHYRUM; col < nomen_temp.mensura && col < latitudo; col++) {
                    pixel_x = offset_x + col * char_width;
                    tabula_pixelorum_pingere_characterem_scalatum(
                        tabula, pixel_x, pixel_y,
                        (character)nomen_temp.datum[col], color_tempus, scala
                    );
                }
                linea_pixel += char_height;
            }
        }

        /* Linea 5: Cyclus lectionum - color #6 */
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

            pixel_y = linea_pixel;
            per (col = ZEPHYRUM; col < (i32)longitudo && col < latitudo; col++) {
                pixel_x = offset_x + col * char_width;
                tabula_pixelorum_pingere_characterem_scalatum(
                    tabula, pixel_x, pixel_y,
                    buffer[col], color_lectionary, scala
                );
            }
            linea_pixel += char_height;
        }

        /* Linea speciale: Si est Pascha, monstrare "Easter" - yellow #11 */
        {
            Dies pascha = fasti_computus(visus->annus_visus);
            si (dies_sel.mensis == pascha.mensis && dies_sel.dies == pascha.dies) {
                constans character* easter_text = "Easter";
                i32 easter_len = VI;  /* "Easter" = 6 chars */

                pixel_y = linea_pixel;
                per (col = ZEPHYRUM; col < easter_len; col++) {
                    pixel_x = offset_x + col * char_width;
                    tabula_pixelorum_pingere_characterem_scalatum(
                        tabula, pixel_x, pixel_y,
                        easter_text[col], color_feast, scala
                    );
                }
                linea_pixel += char_height;
            }
        }

        /* Linea speciale: Si est Feria VI, monstrare "Good Friday" - yellow #11 */
        {
            Dies pascha = fasti_computus(visus->annus_visus);
            Dies feria_vi = fasti_addere_dies(pascha, -II);
            si (dies_sel.mensis == feria_vi.mensis && dies_sel.dies == feria_vi.dies) {
                constans character* friday_text = "Good Friday";
                i32 friday_len = XI;  /* "Good Friday" = 11 chars */

                pixel_y = linea_pixel;
                per (col = ZEPHYRUM; col < friday_len; col++) {
                    pixel_x = offset_x + col * char_width;
                    tabula_pixelorum_pingere_characterem_scalatum(
                        tabula, pixel_x, pixel_y,
                        friday_text[col], color_feast, scala
                    );
                }
                linea_pixel += char_height;
            }
        }

        /* Linea speciale: Si est Feria IV Cinerum, monstrare "Ash Wednesday" - yellow #11 */
        {
            Dies pascha = fasti_computus(visus->annus_visus);
            Dies feria_iv_cinerum = fasti_addere_dies(pascha, -XLVI);
            si (dies_sel.mensis == feria_iv_cinerum.mensis && dies_sel.dies == feria_iv_cinerum.dies) {
                constans character* ash_text = "Ash Wednesday";
                i32 ash_len = XIII;  /* "Ash Wednesday" = 13 chars */

                pixel_y = linea_pixel;
                per (col = ZEPHYRUM; col < ash_len; col++) {
                    pixel_x = offset_x + col * char_width;
                    tabula_pixelorum_pingere_characterem_scalatum(
                        tabula, pixel_x, pixel_y,
                        ash_text[col], color_feast, scala
                    );
                }
                linea_pixel += char_height;
            }
        }

        /* Linea speciale: Si est Pentecoste, monstrare "Pentecost" - yellow #11 */
        {
            Dies pascha = fasti_computus(visus->annus_visus);
            Dies pentecoste = fasti_addere_dies(pascha, XLIX);
            si (dies_sel.mensis == pentecoste.mensis && dies_sel.dies == pentecoste.dies) {
                constans character* pent_text = "Pentecost";
                i32 pent_len = IX;  /* "Pentecost" = 9 chars */

                pixel_y = linea_pixel;
                per (col = ZEPHYRUM; col < pent_len; col++) {
                    pixel_x = offset_x + col * char_width;
                    tabula_pixelorum_pingere_characterem_scalatum(
                        tabula, pixel_x, pixel_y,
                        pent_text[col], color_feast, scala
                    );
                }
                linea_pixel += char_height;
            }
        }

        /* Linea speciale: Si est Dominica Palmarum, monstrare "Palm Sunday" - yellow #11 */
        {
            Dies pascha = fasti_computus(visus->annus_visus);
            Dies dominica_palmarum = fasti_addere_dies(pascha, -VII);
            si (dies_sel.mensis == dominica_palmarum.mensis && dies_sel.dies == dominica_palmarum.dies) {
                constans character* palm_text = "Palm Sunday";
                i32 palm_len = XI;  /* "Palm Sunday" = 11 chars */

                pixel_y = linea_pixel;
                per (col = ZEPHYRUM; col < palm_len; col++) {
                    pixel_x = offset_x + col * char_width;
                    tabula_pixelorum_pingere_characterem_scalatum(
                        tabula, pixel_x, pixel_y,
                        palm_text[col], color_feast, scala
                    );
                }
                linea_pixel += char_height;
            }
        }

        /* Linea 6: Celebrationes sanctorales - yellow #11 */
        si (info->numerus_celebrationum > I && info->celebrationes != NIHIL) {
            i32 idx;
            i32 max_pixel_y = altitudo * char_height;
            per (idx = I; idx < (i32)info->numerus_celebrationum && linea_pixel < max_pixel_y; idx++) {
                Celebratio* celeb = &info->celebrationes[idx];
                si (celeb->titulus.mensura > ZEPHYRUM && celeb->titulus.datum != NIHIL) {
                    i32 max_chars = latitudo - II;
                    i32 chars_to_copy = celeb->titulus.mensura;
                    si (chars_to_copy > max_chars) {
                        chars_to_copy = max_chars;
                    }

                    pixel_y = linea_pixel;
                    per (col = ZEPHYRUM; col < chars_to_copy; col++) {
                        pixel_x = offset_x + col * char_width;
                        tabula_pixelorum_pingere_characterem_scalatum(
                            tabula, pixel_x, pixel_y,
                            (character)celeb->titulus.datum[col], color_feast, scala
                        );
                    }
                    linea_pixel += char_height;
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
    s32 cell_height;
    s32 cell_pixel_width;
    s32 cell_pixel_height;
    s32 padding_left;
    s32 padding_right;
    s32 padding_header;
    s32 latitudo_pixel_effectiva;
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
    Color color_grid;
    Piscina* piscina_temp;
    ContextusDelineandi* ctx_delin;

    char_width = VI * (s32)scala;
    char_height = VIII * (s32)scala;

    /* Padding pro grilla */
    padding_left = X;    /* 10 pixels sinister */
    padding_right = V;   /* 5 pixels dexter */
    padding_header = V;  /* 5 pixels inter header et grillam */

    /* Latitudo cellae in pixelibus (minus padding sinister et dexter) / 7 */
    latitudo_pixel_effectiva = ((s32)latitudo * char_width) - padding_left - padding_right;
    cell_pixel_width = latitudo_pixel_effectiva / VII;
    cell_height = ((s32)altitudo - I) / VII;  /* -1 pro header, /7 pro 6 rows + header */

    si (cell_height < I) cell_height = I;

    /* Altitudo cellae in pixelibus */
    cell_pixel_height = cell_height * char_height;

    color_header = color_ad_pixelum(color_ex_palette(III));  /* #3: Gray */

    /* Color pro grilla */
    color_grid = color_ex_palette(II);  /* #2: Purple */

    /* Reddere titulum "December 2025" supra grillam, centratum */
    {
        character titulus[XXXII];
        i32 titulus_longitudo;
        i32 titulus_latitudo_pixels;
        i32 grid_total_width;
        i32 titulus_x;
        i32 titulus_y;
        i32 color_titulus;
        i32 char_width_large;
        i32 scala_large;
        i32 idx;

        scala_large = (i32)scala * II;
        char_width_large = VI * scala_large;

        titulus_longitudo = (i32)snprintf(titulus, XXXII, "%s %d",
            NOMINA_MENSIUM[visus->mensis_visus],
            visus->annus_visus);

        titulus_latitudo_pixels = titulus_longitudo * char_width_large;
        grid_total_width = (i32)(VII * cell_pixel_width);

        /* Centrare titulum supra grillam */
        titulus_x = (i32)((s32)x * char_width + padding_left) +
                    (grid_total_width - titulus_latitudo_pixels) / II;
        titulus_y = (i32)((s32)y * char_height);

        color_titulus = color_ad_pixelum(color_ex_palette(V));  /* #5: White */

        per (idx = ZEPHYRUM; idx < titulus_longitudo; idx++) {
            tabula_pixelorum_pingere_characterem_scalatum(
                tabula,
                titulus_x + idx * char_width_large,
                titulus_y,
                titulus[idx],
                color_titulus,
                scala_large
            );
        }
    }

    /* Reddere header (Su Mo Tu We Th Fr Sa) - offset 25px deorsum */
    per (i = ZEPHYRUM; i < VII; i++) {
        pixel_x = (i32)((s32)x * char_width + padding_left + i * cell_pixel_width);
        pixel_y = (i32)((s32)y * char_height) + XLV;

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

    /* Delineare grillam "shrink-wrap" circa dies solum */
    si (ctx_delin != NIHIL) {
        i32 grid_x0;
        i32 grid_y0;
        s32 first_col;
        s32 last_pos;
        s32 last_row;
        s32 last_col;
        s32 row;
        s32 col;

        grid_x0 = (i32)((s32)x * char_width + padding_left);
        grid_y0 = (i32)(((s32)y + I) * char_height + padding_header) + XLV;

        first_col = visus->primus_dies_hebdomadae;
        last_pos = visus->numerus_dierum + first_col - I;
        last_row = last_pos / VII;
        last_col = last_pos % VII;

        /* Lineae horizontales - per ordinem */
        per (row = ZEPHYRUM; row <= last_row; row++) {
            s32 row_start_col;
            s32 row_end_col;
            i32 lx0;
            i32 lx1;
            i32 ly;

            /* Determinare columnas pro hoc ordine */
            row_start_col = (row == ZEPHYRUM) ? first_col : ZEPHYRUM;
            row_end_col = (row == last_row) ? last_col : VI;

            lx0 = grid_x0 + (i32)(row_start_col * cell_pixel_width);
            lx1 = grid_x0 + (i32)((row_end_col + I) * cell_pixel_width);

            /* Linea superior ordinis */
            ly = grid_y0 + (i32)(row * cell_pixel_height);
            delineare_lineam_horizontalem(ctx_delin, lx0, lx1, ly, color_grid);

            /* Linea inferior ordinis (solum pro ultimo ordine) */
            si (row == last_row) {
                ly = grid_y0 + (i32)((row + I) * cell_pixel_height);
                delineare_lineam_horizontalem(ctx_delin, lx0, lx1, ly, color_grid);
            }
        }

        /* Gradus sinister: si primus dies non in columna 0 */
        si (first_col > ZEPHYRUM && last_row > ZEPHYRUM) {
            i32 lx0 = grid_x0;
            i32 lx1 = grid_x0 + (i32)(first_col * cell_pixel_width);
            i32 ly = grid_y0 + (i32)(I * cell_pixel_height);
            delineare_lineam_horizontalem(ctx_delin, lx0, lx1, ly, color_grid);
        }

        /* Gradus dexter: si ultimus dies non in columna 6 */
        si (last_col < VI && last_row > ZEPHYRUM) {
            i32 lx0 = grid_x0 + (i32)((last_col + I) * cell_pixel_width);
            i32 lx1 = grid_x0 + (i32)(VII * cell_pixel_width);
            i32 ly = grid_y0 + (i32)(last_row * cell_pixel_height);
            delineare_lineam_horizontalem(ctx_delin, lx0, lx1, ly, color_grid);
        }

        /* Lineae verticales - per columnam */
        per (col = ZEPHYRUM; col <= VII; col++) {
            s32 start_row;
            s32 end_row;
            i32 lx;
            i32 ly0;
            i32 ly1;

            /* Determinare ordines pro hac columna */
            /* Columna c habet dies in:
             *   - ordine 0 si c >= first_col
             *   - ordinibus 1 ad last_row-1 semper
             *   - ordine last_row si c <= last_col
             */
            si (col == ZEPHYRUM) {
                /* Margo sinister columnae 0 */
                start_row = (first_col == ZEPHYRUM) ? ZEPHYRUM : I;
                end_row = (ZEPHYRUM <= last_col) ? last_row : last_row - I;
            } alioquin si (col < first_col) {
                /* Columnae 1 ad first_col-1: non habent dies in ordine 0 */
                start_row = I;
                end_row = ((col - I) <= last_col) ? last_row : last_row - I;
            } alioquin si (col <= VI) {
                /* Columnae first_col+1 ad 6: habent dies in ordine 0 */
                start_row = ZEPHYRUM;
                end_row = ((col - I) <= last_col) ? last_row : last_row - I;
            } alioquin {
                /* Margo dexter (col == 7) */
                start_row = ZEPHYRUM;
                end_row = (VI <= last_col) ? last_row : last_row - I;
            }

            si (start_row <= end_row && end_row >= ZEPHYRUM) {
                lx = grid_x0 + (i32)(col * cell_pixel_width);
                ly0 = grid_y0 + (i32)(start_row * cell_pixel_height);
                ly1 = grid_y0 + (i32)((end_row + I) * cell_pixel_height);
                delineare_lineam_verticalem(ctx_delin, lx, ly0, ly1, color_grid);
            }
        }
    }

    /* Obtinere diem hodiernam et Pascha pro comparatione */
    {
        Dies hodie = fasti_dies_hodie();
        Dies pascha = fasti_computus(visus->annus_visus);
        Dies feria_vi = fasti_addere_dies(pascha, -II);  /* Good Friday = Pascha - 2 */
        Dies feria_iv_cinerum = fasti_addere_dies(pascha, -XLVI);  /* Ash Wednesday = Pascha - 46 */
        Dies pentecoste = fasti_addere_dies(pascha, XLIX);  /* Pentecost = Pascha + 49 */
        Dies dominica_palmarum = fasti_addere_dies(pascha, -VII);  /* Palm Sunday = Pascha - 7 */

        /* Reddere dies mensis */
        per (dies = I; dies <= visus->numerus_dierum; dies++) {
            Dies dies_obj;
            ColorLiturgicus lit_color;
            constans SanctoraleDatum* sanctorale_data;
            s32 num_sanctorale;
            b32 est_hodie;
            b32 ante_hodie;
            b32 est_dies_obligationis;
            b32 est_pascha;
            b32 est_feria_vi;
            b32 est_feria_iv_cinerum;
            b32 est_pentecoste;
            b32 est_dominica_palmarum;
            s32 sanct_idx;

            /* Computare positio in grilla */
            dies_pos = dies + visus->primus_dies_hebdomadae - I;
            col_idx = dies_pos % VII;
            row_idx = dies_pos / VII;

            /* Obtinere colorem liturgicum */
            dies_obj = fasti_dies(visus->annus_visus, visus->mensis_visus, dies);
            lit_color = calendarium_color_temporis(visus->cal, dies_obj);
            color_dies = _color_liturgicus_ad_pixelum(lit_color);

            /* Verificare si est dies obligationis */
            est_dies_obligationis = FALSUM;
            sanctorale_data = sanctorale_obtinere(visus->mensis_visus, dies, &num_sanctorale);
            si (sanctorale_data != NIHIL) {
                per (sanct_idx = ZEPHYRUM; sanct_idx < num_sanctorale; sanct_idx++) {
                    si (sanctorale_data[sanct_idx].dies_obligationis) {
                        est_dies_obligationis = VERUM;
                        frange;
                    }
                }
            }

            /* Si dies obligationis, numerus semper albus */
            si (est_dies_obligationis) {
                color_dies = color_ad_pixelum(color_ex_palette(V));  /* #5: Albus */
            }

            /* Verificare si est Pascha (Easter Sunday) */
            est_pascha = (dies_obj.annus == pascha.annus &&
                          dies_obj.mensis == pascha.mensis &&
                          dies_obj.dies == pascha.dies);

            /* Si est Pascha, numerus semper albus */
            si (est_pascha) {
                color_dies = color_ad_pixelum(color_ex_palette(V));  /* #5: Albus */
            }

            /* Verificare si est Feria VI in Passione (Good Friday) */
            est_feria_vi = (dies_obj.annus == feria_vi.annus &&
                            dies_obj.mensis == feria_vi.mensis &&
                            dies_obj.dies == feria_vi.dies);

            /* Si est Feria VI, numerus semper albus */
            si (est_feria_vi) {
                color_dies = color_ad_pixelum(color_ex_palette(V));  /* #5: Albus */
            }

            /* Verificare si est Feria IV Cinerum (Ash Wednesday) */
            est_feria_iv_cinerum = (dies_obj.annus == feria_iv_cinerum.annus &&
                                    dies_obj.mensis == feria_iv_cinerum.mensis &&
                                    dies_obj.dies == feria_iv_cinerum.dies);

            /* Si est Feria IV Cinerum, numerus semper albus */
            si (est_feria_iv_cinerum) {
                color_dies = color_ad_pixelum(color_ex_palette(V));  /* #5: Albus */
            }

            /* Verificare si est Pentecoste (Pentecost) */
            est_pentecoste = (dies_obj.annus == pentecoste.annus &&
                              dies_obj.mensis == pentecoste.mensis &&
                              dies_obj.dies == pentecoste.dies);

            /* Si est Pentecoste, numerus semper albus */
            si (est_pentecoste) {
                color_dies = color_ad_pixelum(color_ex_palette(V));  /* #5: Albus */
            }

            /* Verificare si est Dominica Palmarum (Palm Sunday) */
            est_dominica_palmarum = (dies_obj.annus == dominica_palmarum.annus &&
                                     dies_obj.mensis == dominica_palmarum.mensis &&
                                     dies_obj.dies == dominica_palmarum.dies);

            /* Si est Dominica Palmarum, numerus semper albus */
            si (est_dominica_palmarum) {
                color_dies = color_ad_pixelum(color_ex_palette(V));  /* #5: Albus */
            }

            /* Verificare si est dies hodierna vel ante */
            est_hodie = (dies_obj.annus == hodie.annus &&
                         dies_obj.mensis == hodie.mensis &&
                         dies_obj.dies == hodie.dies);
            ante_hodie = fasti_ante(dies_obj, hodie);

            /* Positio in pixelibus */
            pixel_x = (i32)((s32)x * char_width + padding_left + col_idx * cell_pixel_width);
            pixel_y = (i32)(((s32)y + I + row_idx * cell_height) * char_height + padding_header) + XLV;

            /* Si dies selectus et focused, pingere background selectus (prioritas maxima) */
            si (dies == visus->dies_selectus && focused && ctx_delin != NIHIL) {
                Color color_sel_bg;
                Color color_sel_fg;

                color_sel_bg = color_ex_palette(VII);   /* #7: Background */
                color_sel_fg = color_ex_palette(XIII);  /* #13: Foreground */

                /* Primo: implere cum colore background */
                delineare_ponere_modum(ctx_delin, MODUS_SOLIDUS);
                delineare_rectangulum_plenum(
                    ctx_delin,
                    pixel_x + I,
                    pixel_y + I,
                    (i32)cell_pixel_width - II,
                    (i32)cell_pixel_height - II,
                    color_sel_bg
                );

                /* Secundo: superponere exemplar cum colore foreground */
                delineare_ponere_modum(ctx_delin, MODUS_EXEMPLAR);
                delineare_ponere_exemplar_internum(ctx_delin, EXEMPLAR_PUNCTA_REGULARIA);
                delineare_ponere_offset_exemplaris(ctx_delin, pixel_x + I, pixel_y + I);
                delineare_rectangulum_plenum(
                    ctx_delin,
                    pixel_x + I,
                    pixel_y + I,
                    (i32)cell_pixel_width - II,
                    (i32)cell_pixel_height - II,
                    color_sel_fg
                );

                /* Restituere modum solidum */
                delineare_ponere_modum(ctx_delin, MODUS_SOLIDUS);
            }
            /* Si est dies hodierna, pingere background cum textura */
            alioquin si (est_hodie && ctx_delin != NIHIL) {
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
                delineare_ponere_offset_exemplaris(ctx_delin, pixel_x + I, pixel_y + I);
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
            /* Si dies obligationis (holy day), pingere background cum textura */
            alioquin si (est_dies_obligationis && ctx_delin != NIHIL) {
                Color color_oblig_bg;
                Color color_oblig_fg;

                color_oblig_bg = color_ex_palette(II);    /* #2: Purple background */
                color_oblig_fg = color_ex_palette(VIII);  /* #8: Pink foreground */

                /* Primo: implere cum colore background */
                delineare_ponere_modum(ctx_delin, MODUS_SOLIDUS);
                delineare_rectangulum_plenum(
                    ctx_delin,
                    pixel_x + I,
                    pixel_y + I,
                    (i32)cell_pixel_width - II,
                    (i32)cell_pixel_height - II,
                    color_oblig_bg
                );

                /* Secundo: superponere exemplar 22 cum colore foreground */
                delineare_ponere_modum(ctx_delin, MODUS_EXEMPLAR);
                delineare_ponere_exemplar_internum(ctx_delin, EXEMPLAR_PUNCTA_DUPLICIA);
                delineare_ponere_offset_exemplaris(ctx_delin, pixel_x + I, pixel_y + I);
                delineare_rectangulum_plenum(
                    ctx_delin,
                    pixel_x + I,
                    pixel_y + I,
                    (i32)cell_pixel_width - II,
                    (i32)cell_pixel_height - II,
                    color_oblig_fg
                );

                /* Restituere modum solidum */
                delineare_ponere_modum(ctx_delin, MODUS_SOLIDUS);
            }
            /* Si est Pascha (Easter Sunday), pingere background speciale */
            alioquin si (est_pascha && ctx_delin != NIHIL) {
                Color color_pascha_bg;
                Color color_pascha_fg;

                color_pascha_bg = color_ex_palette(X);    /* #10: Background */
                color_pascha_fg = color_ex_palette(XI);   /* #11: Foreground */

                /* Primo: implere cum colore background */
                delineare_ponere_modum(ctx_delin, MODUS_SOLIDUS);
                delineare_rectangulum_plenum(
                    ctx_delin,
                    pixel_x + I,
                    pixel_y + I,
                    (i32)cell_pixel_width - II,
                    (i32)cell_pixel_height - II,
                    color_pascha_bg
                );

                /* Secundo: superponere exemplar 31 (LINEA_RHOMBUS) */
                delineare_ponere_modum(ctx_delin, MODUS_EXEMPLAR);
                delineare_ponere_exemplar_internum(ctx_delin, EXEMPLAR_LINEA_RHOMBUS);
                delineare_ponere_offset_exemplaris(ctx_delin, pixel_x + I, pixel_y + I);
                delineare_rectangulum_plenum(
                    ctx_delin,
                    pixel_x + I,
                    pixel_y + I,
                    (i32)cell_pixel_width - II,
                    (i32)cell_pixel_height - II,
                    color_pascha_fg
                );

                /* Restituere modum solidum */
                delineare_ponere_modum(ctx_delin, MODUS_SOLIDUS);
            }
            /* Si est Feria VI in Passione (Good Friday), pingere background speciale */
            alioquin si (est_feria_vi && ctx_delin != NIHIL) {
                Color color_feria_bg;
                Color color_feria_fg;

                color_feria_bg = color_ex_palette(VI);    /* #6: Background */
                color_feria_fg = color_ex_palette(VIII);  /* #8: Foreground */

                /* Primo: implere cum colore background */
                delineare_ponere_modum(ctx_delin, MODUS_SOLIDUS);
                delineare_rectangulum_plenum(
                    ctx_delin,
                    pixel_x + I,
                    pixel_y + I,
                    (i32)cell_pixel_width - II,
                    (i32)cell_pixel_height - II,
                    color_feria_bg
                );

                /* Secundo: superponere exemplar 20 (RARISSIMUS) */
                delineare_ponere_modum(ctx_delin, MODUS_EXEMPLAR);
                delineare_ponere_exemplar_internum(ctx_delin, EXEMPLAR_RARISSIMUS);
                delineare_ponere_offset_exemplaris(ctx_delin, pixel_x + I, pixel_y + I);
                delineare_rectangulum_plenum(
                    ctx_delin,
                    pixel_x + I,
                    pixel_y + I,
                    (i32)cell_pixel_width - II,
                    (i32)cell_pixel_height - II,
                    color_feria_fg
                );

                /* Tertio: pingere crucem Latinam flavam in centro (2px) */
                {
                    Color color_crux;
                    i32 center_x;
                    i32 cell_w;
                    i32 cell_h;
                    i32 crux_h;       /* Altitudo totalis crucis */
                    i32 crux_semi_l;  /* Semi-latitudo crucis (horizontalis) */
                    i32 crux_top;     /* Y superior crucis */
                    i32 crux_bottom;  /* Y inferior crucis */
                    i32 crux_horiz_y; /* Y lineae horizontalis (30% ab summo) */

                    color_crux = color_ex_palette(XI);  /* #11: Flavus (yellow) */
                    cell_w = (i32)cell_pixel_width - II;
                    cell_h = (i32)cell_pixel_height - II;
                    center_x = pixel_x + I + cell_w / II;
                    crux_h = cell_h * II / III;  /* 2/3 altitudinis cellae */
                    crux_top = pixel_y + I + (cell_h - crux_h) / II;
                    crux_bottom = crux_top + crux_h;
                    crux_horiz_y = crux_top + crux_h * III / X;  /* 30% ab summo */
                    crux_semi_l = cell_w / IX + II;  /* 1/9 latitudinis + 2px */

                    delineare_ponere_modum(ctx_delin, MODUS_SOLIDUS);
                    /* Linea verticalis (2px lata) */
                    delineare_lineam_verticalem(ctx_delin,
                        center_x, crux_top, crux_bottom, color_crux);
                    delineare_lineam_verticalem(ctx_delin,
                        center_x + I, crux_top, crux_bottom, color_crux);
                    /* Linea horizontalis (2px alta) */
                    delineare_lineam_horizontalem(ctx_delin,
                        center_x - crux_semi_l, center_x + crux_semi_l,
                        crux_horiz_y, color_crux);
                    delineare_lineam_horizontalem(ctx_delin,
                        center_x - crux_semi_l, center_x + crux_semi_l,
                        crux_horiz_y + I, color_crux);
                }

                /* Restituere modum solidum */
                delineare_ponere_modum(ctx_delin, MODUS_SOLIDUS);
            }
            /* Si est Feria IV Cinerum (Ash Wednesday), pingere background speciale */
            alioquin si (est_feria_iv_cinerum && ctx_delin != NIHIL) {
                Color color_cinerum_bg;
                Color color_cinerum_fg;

                color_cinerum_bg = color_ex_palette(II);   /* #2: Background */
                color_cinerum_fg = color_ex_palette(IX);   /* #9: Foreground */

                /* Primo: implere cum colore background */
                delineare_ponere_modum(ctx_delin, MODUS_SOLIDUS);
                delineare_rectangulum_plenum(
                    ctx_delin,
                    pixel_x + I,
                    pixel_y + I,
                    (i32)cell_pixel_width - II,
                    (i32)cell_pixel_height - II,
                    color_cinerum_bg
                );

                /* Secundo: superponere exemplar 8 (DISPERSUS) */
                delineare_ponere_modum(ctx_delin, MODUS_EXEMPLAR);
                delineare_ponere_exemplar_internum(ctx_delin, EXEMPLAR_DISPERSUS);
                delineare_ponere_offset_exemplaris(ctx_delin, pixel_x + I, pixel_y + I);
                delineare_rectangulum_plenum(
                    ctx_delin,
                    pixel_x + I,
                    pixel_y + I,
                    (i32)cell_pixel_width - II,
                    (i32)cell_pixel_height - II,
                    color_cinerum_fg
                );

                /* Restituere modum solidum */
                delineare_ponere_modum(ctx_delin, MODUS_SOLIDUS);
            }
            /* Si est Pentecoste (Pentecost), pingere background speciale */
            alioquin si (est_pentecoste && ctx_delin != NIHIL) {
                Color color_pent_bg;
                Color color_pent_fg;

                color_pent_bg = color_ex_palette(VIII);  /* #8: Background */
                color_pent_fg = color_ex_palette(X);     /* #10: Foreground */

                /* Primo: implere cum colore background */
                delineare_ponere_modum(ctx_delin, MODUS_SOLIDUS);
                delineare_rectangulum_plenum(
                    ctx_delin,
                    pixel_x + I,
                    pixel_y + I,
                    (i32)cell_pixel_width - II,
                    (i32)cell_pixel_height - II,
                    color_pent_bg
                );

                /* Secundo: superponere exemplar 35 (MIXTUS_GRAVIS) */
                delineare_ponere_modum(ctx_delin, MODUS_EXEMPLAR);
                delineare_ponere_exemplar_internum(ctx_delin, EXEMPLAR_MIXTUS_GRAVIS);
                delineare_ponere_offset_exemplaris(ctx_delin, pixel_x + I, pixel_y + I);
                delineare_rectangulum_plenum(
                    ctx_delin,
                    pixel_x + I,
                    pixel_y + I,
                    (i32)cell_pixel_width - II,
                    (i32)cell_pixel_height - II,
                    color_pent_fg
                );

                /* Restituere modum solidum */
                delineare_ponere_modum(ctx_delin, MODUS_SOLIDUS);
            }
            /* Si est Dominica Palmarum (Palm Sunday), pingere background speciale */
            alioquin si (est_dominica_palmarum && ctx_delin != NIHIL) {
                Color color_palm_bg;
                Color color_palm_fg;

                color_palm_bg = color_ex_palette(XII);   /* #12: Background */
                color_palm_fg = color_ex_palette(XIII);  /* #13: Foreground */

                /* Primo: implere cum colore background */
                delineare_ponere_modum(ctx_delin, MODUS_SOLIDUS);
                delineare_rectangulum_plenum(
                    ctx_delin,
                    pixel_x + I,
                    pixel_y + I,
                    (i32)cell_pixel_width - II,
                    (i32)cell_pixel_height - II,
                    color_palm_bg
                );

                /* Secundo: superponere exemplar 16 (MIXTUS) */
                delineare_ponere_modum(ctx_delin, MODUS_EXEMPLAR);
                delineare_ponere_exemplar_internum(ctx_delin, EXEMPLAR_MIXTUS);
                delineare_ponere_offset_exemplaris(ctx_delin, pixel_x + I, pixel_y + I);
                delineare_rectangulum_plenum(
                    ctx_delin,
                    pixel_x + I,
                    pixel_y + I,
                    (i32)cell_pixel_width - II,
                    (i32)cell_pixel_height - II,
                    color_palm_fg
                );

                /* Restituere modum solidum */
                delineare_ponere_modum(ctx_delin, MODUS_SOLIDUS);
            }
            /* Si dies ante hodie, pingere background griseum */
            alioquin si (ante_hodie && ctx_delin != NIHIL) {
                Color color_past;
                color_past = color_ex_palette(III);  /* 3: Medium gray */
                delineare_rectangulum_plenum(
                    ctx_delin,
                    pixel_x + I,
                    pixel_y + I,
                    (i32)cell_pixel_width - II,
                    (i32)cell_pixel_height - II,
                    color_past
                );
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

    /* Tractare click muris */
    si (eventus->genus == EVENTUS_MUS_DEPRESSUS &&
        eventus->datum.mus.botton == MUS_SINISTER) {
        s32 mus_x = (s32)eventus->datum.mus.x;
        s32 mus_y = (s32)eventus->datum.mus.y;
        s32 char_width = VI * (s32)visus->scala;
        s32 char_height = VIII * (s32)visus->scala;
        s32 padding_left = X;    /* 10 pixels - debet esse idem ac in _reddere_grid */
        s32 padding_right = V;   /* 5 pixels */
        s32 padding_header = V;  /* 5 pixels */
        s32 altitudo_details = (s32)visus->altitudo / III;
        s32 altitudo_grid = (s32)visus->altitudo - altitudo_details;
        s32 grid_y_start = ((s32)visus->widget_y + altitudo_details) * char_height + char_height + padding_header + XLV;
        s32 grid_x_start = (s32)visus->widget_x * char_width + padding_left;
        s32 latitudo_pixel_effectiva = ((s32)visus->latitudo * char_width) - padding_left - padding_right;
        s32 cell_pixel_width = latitudo_pixel_effectiva / VII;
        s32 cell_height = (altitudo_grid - I) / VII;
        s32 cell_pixel_height = cell_height * char_height;
        s32 rel_x;
        s32 rel_y;
        s32 col_clicked;
        s32 row_clicked;
        s32 dies_pos;
        s32 dies_clicked;

        /* Verificare si click est in area grillae */
        si (mus_y >= grid_y_start && mus_x >= grid_x_start) {
            /* Positio relativa in grilla (post header) */
            rel_x = mus_x - grid_x_start;
            rel_y = mus_y - grid_y_start; /* grid_y_start iam includit header */

            si (rel_y >= ZEPHYRUM && cell_pixel_width > ZEPHYRUM && cell_pixel_height > ZEPHYRUM) {
                col_clicked = rel_x / cell_pixel_width;
                row_clicked = rel_y / cell_pixel_height;

                /* Computare dies ex positione */
                dies_pos = row_clicked * VII + col_clicked;
                dies_clicked = dies_pos - visus->primus_dies_hebdomadae + I;

                /* Verificare si dies est validus */
                si (dies_clicked >= I && dies_clicked <= visus->numerus_dierum) {
                    visus->dies_selectus = dies_clicked;
                    redde VERUM;
                }
            }
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
