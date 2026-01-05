/*
 * elementa.c - Immediate-mode GUI component library
 *
 * Pars Phase II Pinacothecae. Componenta GUI simplicia pro dialogis.
 */

#include "elementa.h"
#include "delineare.h"
#include "thema.h"
#include "color.h"
#include "chorda_aedificator.h"

/* ============================================================
 * Status Staticus - Tractare statum per frames
 * ============================================================ */

hic_manens structura {
    b32 slider_dragging;
    i32 slider_drag_id;
    b32 bottone_pressed;
    i32 bottone_pressed_id;
    i32 mus_x;
    i32 mus_y;
    b32 mus_depressus;  /* Mus button currently down */
} _elementa_status = {FALSUM, 0, FALSUM, 0, 0, 0, FALSUM};

/* ============================================================
 * Functiones Auxiliares
 * ============================================================ */

/* Generare id unicum ex positione */
interior i32
_elementa_generare_id(i32 x, i32 y)
{
    redde (x * 31337) ^ (y * 7919);
}

/* Probare si punctum intra rectangulum */
interior b32
_elementa_punctum_in_recto(i32 px, i32 py, i32 rx, i32 ry, i32 rl, i32 ra)
{
    redde (px >= rx && px < rx + rl && py >= ry && py < ry + ra);
}

/* Renovare positio muris ex eventu */
interior vacuum
_elementa_renovare_mus(constans Eventus* eventus)
{
    si (eventus != NIHIL)
    {
        si (eventus->genus == EVENTUS_MUS_MOTUS ||
            eventus->genus == EVENTUS_MUS_DEPRESSUS ||
            eventus->genus == EVENTUS_MUS_LIBERATUS)
        {
            _elementa_status.mus_x = eventus->datum.mus.x;
            _elementa_status.mus_y = eventus->datum.mus.y;
        }
        si (eventus->genus == EVENTUS_MUS_DEPRESSUS)
        {
            _elementa_status.mus_depressus = VERUM;
        }
        si (eventus->genus == EVENTUS_MUS_LIBERATUS)
        {
            _elementa_status.mus_depressus = FALSUM;
            /* Finis slider drag state - bottone state managed by bottone itself */
            _elementa_status.slider_dragging = FALSUM;
            _elementa_status.slider_drag_id = 0;
            /* NON clariare bottone_pressed hic - bottone facit in suo codice */
        }
    }
}

/* ============================================================
 * Slider
 * ============================================================ */

FructusSlider
elementa_slider(
    TabulaPixelorum* tabula,
    Piscina*         piscina,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              valor,
    i32              valor_min,
    i32              valor_max,
    constans Eventus* eventus,
    f32              scala)
{
    FructusSlider fructus;
    ContextusDelineandi* ctx;
    i32 id;
    i32 px, py;
    i32 track_y;
    i32 track_latitudo;
    i32 thumb_radius;
    i32 thumb_x;
    f32 ratio;
    Color color_track;
    Color color_thumb;
    i32 hit_x, hit_y, hit_lat, hit_alt;

    /* Initiare fructum */
    fructus.valor = valor;
    fructus.mutatum = FALSUM;
    fructus.hover = FALSUM;
    fructus.dragging = FALSUM;

    /* Creare contextum delineandi */
    ctx = delineare_creare_contextum(piscina, tabula);
    si (!ctx)
    {
        redde fructus;
    }

    /* Computare dimensiones in pixelis */
    /* Character = 6x8 pixels */
    px = (i32)((f32)x * 6.0f * scala);
    py = (i32)((f32)y * 8.0f * scala);
    track_latitudo = (i32)((f32)latitudo * 6.0f * scala);
    thumb_radius = (i32)(4.0f * scala);
    track_y = py + (i32)(4.0f * scala);  /* Centro verticali */

    /* ID unicum pro hoc slider */
    id = _elementa_generare_id(x, y);

    /* Computare positio thumb ex valore */
    si (valor_max > valor_min)
    {
        ratio = (f32)(valor - valor_min) / (f32)(valor_max - valor_min);
    }
    alioquin
    {
        ratio = 0.0f;
    }
    thumb_x = px + (i32)(ratio * (f32)(track_latitudo - thumb_radius * 2)) + thumb_radius;

    /* Hit testing area */
    hit_x = px;
    hit_y = py;
    hit_lat = track_latitudo;
    hit_alt = (i32)(8.0f * scala);

    /* Renovare positio muris */
    _elementa_renovare_mus(eventus);

    /* Hover sempre ex positione muris currente */
    fructus.hover = _elementa_punctum_in_recto(_elementa_status.mus_x, _elementa_status.mus_y, hit_x, hit_y, hit_lat, hit_alt);

    /* Tractare eventus */
    si (eventus != NIHIL)
    {
        /* Tractare mouse down - initium drag */
        si (eventus->genus == EVENTUS_MUS_DEPRESSUS && fructus.hover)
        {
            _elementa_status.slider_dragging = VERUM;
            _elementa_status.slider_drag_id = id;
        }

        /* Tractare mouse up - finis drag */
        si (eventus->genus == EVENTUS_MUS_LIBERATUS)
        {
            _elementa_status.slider_dragging = FALSUM;
            _elementa_status.slider_drag_id = 0;
        }
    }

    /* Si trahitur hic slider (et mus adhuc depressus) */
    si (_elementa_status.slider_dragging && _elementa_status.slider_drag_id == id && _elementa_status.mus_depressus)
    {
        s32 valor_novus;
        f32 pos_ratio;
        s32 numerator;   /* MUST be signed - can be negative */
        s32 denominator;

        fructus.dragging = VERUM;

        /* Computare novum valorem ex positione muris */
        numerator = (s32)_elementa_status.mus_x - (s32)px - (s32)thumb_radius;
        denominator = (s32)track_latitudo - (s32)thumb_radius * 2;
        pos_ratio = (f32)numerator / (f32)denominator;
        si (pos_ratio < 0.0f) pos_ratio = 0.0f;
        si (pos_ratio > 1.0f) pos_ratio = 1.0f;

        valor_novus = (s32)valor_min + (s32)(pos_ratio * (f32)((s32)valor_max - (s32)valor_min));

        si (valor_novus != (s32)fructus.valor)
        {
            fructus.valor = (i32)valor_novus;
            fructus.mutatum = VERUM;
        }

        /* Recomputare thumb_x */
        thumb_x = px + (i32)(pos_ratio * (f32)(track_latitudo - thumb_radius * 2)) + thumb_radius;
    }

    /* Obtinere colores */
    color_track = thema_color(COLOR_BORDER);
    color_thumb = thema_color(COLOR_ACCENT_PRIMARY);

    /* Delineare track */
    delineare_lineam_horizontalem(ctx, px, px + track_latitudo, track_y, color_track);

    /* Delineare thumb */
    delineare_circulum_plenum(ctx, thumb_x, track_y, thumb_radius, color_thumb);

    /* Si hover vel dragging, delineare circulum extra */
    si (fructus.hover || fructus.dragging)
    {
        delineare_circulum(ctx, thumb_x, track_y, thumb_radius + 2, color_thumb);
    }

    redde fructus;
}

/* ============================================================
 * Bottone
 * ============================================================ */

FructusBottone
elementa_bottone(
    TabulaPixelorum* tabula,
    Piscina*         piscina,
    i32              x,
    i32              y,
    chorda*          label,
    constans Eventus* eventus,
    f32              scala)
{
    FructusBottone fructus;
    ContextusDelineandi* ctx;
    i32 id;
    i32 px, py;
    i32 padding;
    i32 latitudo, altitudo;
    i32 text_x, text_y;
    i32 offset;
    b32 pressed;
    Color color_bg;
    Color color_border;
    Color color_text;

    /* Initiare fructum */
    fructus.clicked = FALSUM;
    fructus.hover = FALSUM;

    /* Creare contextum delineandi */
    ctx = delineare_creare_contextum(piscina, tabula);
    si (!ctx)
    {
        redde fructus;
    }

    /* ID unicum pro hoc bottone */
    id = _elementa_generare_id(x, y);

    /* Computare dimensiones */
    px = (i32)((f32)x * 6.0f * scala);
    py = (i32)((f32)y * 8.0f * scala);
    padding = (i32)(6.0f * scala);

    /* Latitudo ex longitudine label + padding */
    si (label != NIHIL)
    {
        latitudo = (i32)((f32)label->mensura * 6.0f * scala) + padding * 2;
    }
    alioquin
    {
        latitudo = padding * 4;
    }
    altitudo = (i32)(8.0f * scala) + padding * 2;

    /* Tractare eventus */
    pressed = FALSUM;

    /* Renovare positio muris (sed NON clariare button state - facimus hic) */
    si (eventus != NIHIL)
    {
        si (eventus->genus == EVENTUS_MUS_MOTUS ||
            eventus->genus == EVENTUS_MUS_DEPRESSUS ||
            eventus->genus == EVENTUS_MUS_LIBERATUS)
        {
            _elementa_status.mus_x = eventus->datum.mus.x;
            _elementa_status.mus_y = eventus->datum.mus.y;
        }
    }

    /* Hover sempre ex positione muris currente */
    fructus.hover = _elementa_punctum_in_recto(_elementa_status.mus_x, _elementa_status.mus_y, px, py, latitudo, altitudo);

    si (eventus != NIHIL)
    {
        /* Mouse down - start press */
        si (eventus->genus == EVENTUS_MUS_DEPRESSUS && fructus.hover)
        {
            _elementa_status.bottone_pressed = VERUM;
            _elementa_status.bottone_pressed_id = id;
        }

        /* Mouse up - end press, trigger click if still over button */
        si (eventus->genus == EVENTUS_MUS_LIBERATUS)
        {
            si (_elementa_status.bottone_pressed && _elementa_status.bottone_pressed_id == id)
            {
                /* Hic bottone erat depressus - clariare statum */
                si (fructus.hover)
                {
                    fructus.clicked = VERUM;
                }
                /* Solum clariare si hic bottone erat depressus */
                _elementa_status.bottone_pressed = FALSUM;
                _elementa_status.bottone_pressed_id = 0;
            }
        }
    }

    /* Check if this button is currently pressed */
    si (_elementa_status.bottone_pressed && _elementa_status.bottone_pressed_id == id)
    {
        pressed = VERUM;
    }

    /* Offset pro pressed state (button moves down slightly) */
    offset = pressed ? (i32)(2.0f * scala) : 0;

    /* Obtinere colores */
    si (pressed)
    {
        /* Pressed: darker accent */
        color_bg = thema_color(COLOR_ACCENT_SECONDARY);
        color_border = thema_color(COLOR_BORDER_ACTIVE);
        color_text = thema_color(COLOR_BACKGROUND);
    }
    alioquin si (fructus.hover)
    {
        color_bg = thema_color(COLOR_ACCENT_PRIMARY);
        color_border = thema_color(COLOR_BORDER_ACTIVE);
        color_text = thema_color(COLOR_BACKGROUND);
    }
    alioquin
    {
        color_bg = thema_color(COLOR_BACKGROUND);
        color_border = thema_color(COLOR_BORDER);
        color_text = thema_color(COLOR_TEXT);
    }

    /* Delineare rectangulum rotundum (offset when pressed) */
    delineare_rectangulum_rotundum_plenum(ctx, px, py + offset, latitudo, altitudo, (i32)(2.0f * scala), color_bg);
    delineare_rectangulum_rotundum(ctx, px, py + offset, latitudo, altitudo, (i32)(2.0f * scala), color_border);

    /* Delineare label (offset when pressed) */
    si (label != NIHIL)
    {
        text_x = px + padding;
        text_y = py + padding + offset;
        tabula_pixelorum_pingere_chordam_scalatam(tabula, text_x, text_y, *label, color_ad_pixelum(color_text), (i32)scala);
    }

    redde fructus;
}

/* ============================================================
 * Capsa Optandi (Checkbox)
 * ============================================================ */

FructusCapsaOptandi
elementa_capsa_optandi(
    TabulaPixelorum* tabula,
    Piscina*         piscina,
    i32              x,
    i32              y,
    chorda*          label,
    b32              valor,
    constans Eventus* eventus,
    f32              scala)
{
    FructusCapsaOptandi fructus;
    ContextusDelineandi* ctx;
    i32 px, py;
    i32 box_size;
    i32 hit_latitudo;
    Color color_border;
    Color color_fill;
    Color color_text;

    /* Initiare fructum */
    fructus.valor = valor;
    fructus.mutatum = FALSUM;
    fructus.hover = FALSUM;

    /* Creare contextum delineandi */
    ctx = delineare_creare_contextum(piscina, tabula);
    si (!ctx)
    {
        redde fructus;
    }

    /* Computare dimensiones */
    px = (i32)((f32)x * 6.0f * scala);
    py = (i32)((f32)y * 8.0f * scala);
    box_size = (i32)(8.0f * scala);

    /* Hit area includit label */
    si (label != NIHIL)
    {
        hit_latitudo = box_size + (i32)(4.0f * scala) + (i32)((f32)label->mensura * 6.0f * scala);
    }
    alioquin
    {
        hit_latitudo = box_size;
    }

    /* Renovare positio muris */
    _elementa_renovare_mus(eventus);

    /* Hover sempre ex positione muris currente */
    fructus.hover = _elementa_punctum_in_recto(_elementa_status.mus_x, _elementa_status.mus_y, px, py, hit_latitudo, box_size);

    /* Tractare eventus */
    si (eventus != NIHIL)
    {
        si (eventus->genus == EVENTUS_MUS_DEPRESSUS && fructus.hover)
        {
            fructus.valor = !fructus.valor;
            fructus.mutatum = VERUM;
        }
    }

    /* Obtinere colores */
    color_border = fructus.hover ? thema_color(COLOR_BORDER_ACTIVE) : thema_color(COLOR_BORDER);
    color_fill = thema_color(COLOR_ACCENT_PRIMARY);
    color_text = thema_color(COLOR_TEXT);

    /* Delineare capsam */
    delineare_rectangulum(ctx, px, py, box_size, box_size, color_border);

    /* Si selectum, implere */
    si (fructus.valor)
    {
        i32 margin = (i32)(2.0f * scala);
        delineare_rectangulum_plenum(ctx, px + margin, py + margin,
                                     box_size - margin * 2, box_size - margin * 2,
                                     color_fill);
    }

    /* Delineare label */
    si (label != NIHIL)
    {
        i32 text_x = px + box_size + (i32)(4.0f * scala);
        i32 text_y = py;
        tabula_pixelorum_pingere_chordam_scalatam(tabula, text_x, text_y, *label, color_ad_pixelum(color_text), (i32)scala);
    }

    redde fructus;
}

/* ============================================================
 * Campus Textus (Text Input)
 * ============================================================ */

FructusCampusTextus
elementa_campus_textus(
    TabulaPixelorum* tabula,
    Piscina*         piscina,
    i32              x,
    i32              y,
    i32              latitudo,
    chorda*          textus,
    s32              cursor,
    b32              focused,
    constans Eventus* eventus,
    f32              scala)
{
    FructusCampusTextus fructus;
    ContextusDelineandi* ctx;
    i32 px, py;
    i32 field_latitudo, field_altitudo;
    i32 padding;
    i32 cursor_x;
    Color color_border;
    Color color_bg;
    Color color_text;
    Color color_cursor;

    /* Initiare fructum */
    fructus.textus = textus;
    fructus.cursor = cursor;
    fructus.mutatum = FALSUM;
    fructus.focused = focused;

    /* Creare contextum delineandi */
    ctx = delineare_creare_contextum(piscina, tabula);
    si (!ctx)
    {
        redde fructus;
    }

    /* Computare dimensiones */
    px = (i32)((f32)x * 6.0f * scala);
    py = (i32)((f32)y * 8.0f * scala);
    field_latitudo = (i32)((f32)latitudo * 6.0f * scala);
    field_altitudo = (i32)(14.0f * scala);  /* Altior pro padding maiore */
    padding = (i32)(4.0f * scala);           /* Padding maior */

    /* Renovare positio muris */
    _elementa_renovare_mus(eventus);

    /* Tractare eventus */
    si (eventus != NIHIL)
    {
        /* Click ad focus et cursor ad finem */
        si (eventus->genus == EVENTUS_MUS_DEPRESSUS)
        {
            b32 clicked_inside = _elementa_punctum_in_recto(_elementa_status.mus_x, _elementa_status.mus_y, px, py, field_latitudo, field_altitudo);
            si (clicked_inside && !fructus.focused)
            {
                /* Novum focus - cursor ad finem */
                fructus.focused = VERUM;
                si (textus != NIHIL)
                {
                    fructus.cursor = (s32)textus->mensura;
                }
                alioquin
                {
                    fructus.cursor = 0;
                }
            }
            alioquin si (!clicked_inside)
            {
                fructus.focused = FALSUM;
            }
        }

        /* Tractare clavis si focused */
        si (fructus.focused && eventus->genus == EVENTUS_CLAVIS_DEPRESSUS)
        {
            character typus = eventus->datum.clavis.typus;
            clavis_t clavis = eventus->datum.clavis.clavis;

            /* Backspace */
            si (clavis == CLAVIS_RETRORSUM && fructus.cursor > 0)
            {
                /* Creare novam chordam sine charactere ad cursor-1 */
                si (textus != NIHIL && textus->mensura > 0)
                {
                    ChordaAedificator* aed = chorda_aedificator_creare(piscina, textus->mensura);
                    s32 j;

                    /* Copiere omnes characteres praeter eum ad cursor-1 */
                    per (j = 0; j < (s32)textus->mensura; j++)
                    {
                        si (j != fructus.cursor - 1)
                        {
                            chorda_aedificator_appendere_character(aed, (character)textus->datum[j]);
                        }
                    }

                    fructus.textus = piscina_allocare(piscina, magnitudo(chorda));
                    *fructus.textus = chorda_aedificator_finire(aed);
                    fructus.cursor--;
                    fructus.mutatum = VERUM;
                }
            }
            /* Character normalis */
            alioquin si (typus >= 32 && typus < 127)
            {
                /* Creare novam chordam cum charactere inserto ad cursor */
                ChordaAedificator* aed;
                s32 j;
                i32 old_len = (textus != NIHIL) ? textus->mensura : 0;

                aed = chorda_aedificator_creare(piscina, old_len + 2);

                /* Copiere characteres ante cursor */
                si (textus != NIHIL)
                {
                    per (j = 0; j < fructus.cursor && j < (s32)old_len; j++)
                    {
                        chorda_aedificator_appendere_character(aed, (character)textus->datum[j]);
                    }
                }

                /* Inserere novum characterem */
                chorda_aedificator_appendere_character(aed, typus);

                /* Copiere characteres post cursor */
                si (textus != NIHIL)
                {
                    per (j = fructus.cursor; j < (s32)old_len; j++)
                    {
                        chorda_aedificator_appendere_character(aed, (character)textus->datum[j]);
                    }
                }

                fructus.textus = piscina_allocare(piscina, magnitudo(chorda));
                *fructus.textus = chorda_aedificator_finire(aed);
                fructus.cursor++;
                fructus.mutatum = VERUM;
            }
            /* Sagitta sinistra */
            alioquin si (clavis == CLAVIS_SINISTER && fructus.cursor > 0)
            {
                fructus.cursor--;
            }
            /* Sagitta dextra */
            alioquin si (clavis == CLAVIS_DEXTER && textus != NIHIL && (i32)fructus.cursor < textus->mensura)
            {
                fructus.cursor++;
            }
        }
    }

    /* Obtinere colores */
    color_bg = thema_color(COLOR_BACKGROUND);
    color_border = fructus.focused ? thema_color(COLOR_BORDER_ACTIVE) : thema_color(COLOR_BORDER);
    color_text = thema_color(COLOR_TEXT);
    color_cursor = thema_color(COLOR_CURSOR);

    /* Delineare rectangulum campi */
    delineare_rectangulum_plenum(ctx, px, py, field_latitudo, field_altitudo, color_bg);
    delineare_rectangulum(ctx, px, py, field_latitudo, field_altitudo, color_border);

    /* Computare visibiles characteres et delineare */
    {
        i32 text_y_centered = py + (field_altitudo - (i32)(8.0f * scala)) / 2;
        i32 char_latitudo = (i32)(6.0f * scala);
        i32 visible_latitudo = field_latitudo - padding * 2;
        i32 max_visible_chars = visible_latitudo / char_latitudo;
        s32 scroll_chars = 0;  /* Quot characteres scrolled */

        /* Si cursor ultra aream visibilem, scroll */
        si (fructus.cursor > (s32)max_visible_chars - 1)
        {
            scroll_chars = fructus.cursor - (s32)max_visible_chars + 1;
        }

        /* Delineare tantum characteres visibiles */
        si (textus != NIHIL && textus->mensura > 0)
        {
            s32 start_char = scroll_chars;
            s32 end_char = scroll_chars + (s32)max_visible_chars;
            s32 text_len = (s32)textus->mensura;
            s32 j;

            /* Clamp ad limites textus */
            si (start_char < 0) start_char = 0;
            si (end_char > text_len) end_char = text_len;

            /* Delineare singulos characteres */
            per (j = start_char; j < end_char; j++)
            {
                chorda char_str;
                i32 char_x = px + padding + (i32)(j - scroll_chars) * char_latitudo;

                char_str.datum = &textus->datum[j];
                char_str.mensura = 1;

                tabula_pixelorum_pingere_chordam_scalatam(tabula, char_x, text_y_centered, char_str, color_ad_pixelum(color_text), (i32)scala);
            }
        }

        /* Delineare cursor si focused */
        si (fructus.focused)
        {
            cursor_x = px + padding + (i32)(fructus.cursor - scroll_chars) * char_latitudo;
            delineare_lineam_verticalem(ctx, cursor_x, text_y_centered, text_y_centered + (i32)(8.0f * scala), color_cursor);
        }
    }

    redde fructus;
}

/* ============================================================
 * Graticula Colorum (Palette Grid)
 * ============================================================ */

FructusGraticulaColorum
elementa_graticula_colorum(
    TabulaPixelorum* tabula,
    Piscina*         piscina,
    i32              x,
    i32              y,
    constans b32*    colores,
    constans Eventus* eventus,
    f32              scala)
{
    FructusGraticulaColorum fructus;
    ContextusDelineandi* ctx;
    i32 px, py;
    i32 swatch_size;
    i32 gap;
    i32 i;
    s32 row, col;

    /* Initiare fructum */
    per (i = 0; i < 16; i++)
    {
        fructus.colores[i] = colores[i];
    }
    fructus.mutatum = FALSUM;
    fructus.toggled_index = -1;

    /* Creare contextum delineandi */
    ctx = delineare_creare_contextum(piscina, tabula);
    si (!ctx)
    {
        redde fructus;
    }

    /* Computare dimensiones */
    px = (i32)((f32)x * 6.0f * scala);
    py = (i32)((f32)y * 8.0f * scala);
    swatch_size = (i32)(12.0f * scala);
    gap = (i32)(2.0f * scala);

    /* Renovare positio muris */
    _elementa_renovare_mus(eventus);

    /* Tractare eventus et delineare */
    per (i = 0; i < 16; i++)
    {
        Color color_swatch;
        i32 sx, sy;

        /* Computare positio in graticula 2x8 */
        row = (s32)(i / 8);
        col = (s32)(i % 8);
        sx = px + (i32)col * (swatch_size + gap);
        sy = py + (i32)row * (swatch_size + gap);

        /* Obtinere color ex palette (color_ex_palette scales 6-bit to 8-bit) */
        color_swatch = color_ex_palette(i);

        /* Tractare click */
        si (eventus != NIHIL && eventus->genus == EVENTUS_MUS_DEPRESSUS)
        {
            si (_elementa_punctum_in_recto(_elementa_status.mus_x, _elementa_status.mus_y, sx, sy, swatch_size, swatch_size))
            {
                fructus.colores[i] = !fructus.colores[i];
                fructus.mutatum = VERUM;
                fructus.toggled_index = (s32)i;
            }
        }

        /* Delineare swatch */
        delineare_rectangulum_plenum(ctx, sx, sy, swatch_size, swatch_size, color_swatch);

        /* Si color deselectus, delineare lineas diagonales */
        si (!fructus.colores[i])
        {
            Color color_x;
            /* Pro colore 2 (dark purple), uti colore contrastante (medium gold, index 10) */
            si (i == 2)
            {
                color_x = color_ex_palette(10);
            }
            alioquin
            {
                color_x = thema_color(COLOR_TEXT_DIM);
            }
            delineare_lineam(ctx, sx, sy, sx + swatch_size, sy + swatch_size, color_x);
            delineare_lineam(ctx, sx + swatch_size, sy, sx, sy + swatch_size, color_x);
        }

        /* Delineare margo */
        delineare_rectangulum(ctx, sx, sy, swatch_size, swatch_size, thema_color(COLOR_BORDER));
    }

    redde fructus;
}
