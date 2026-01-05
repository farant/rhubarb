/*
 * dialogus_importatio.c - Import dialog implementation
 *
 * Dialog continet solum controllos - preview est in importatio_visus.
 * Utitur elementa pro GUI components.
 */

#include "dialogus_importatio.h"
#include "elementa.h"
#include "thema.h"
#include "color.h"
#include <stdio.h>

/* ============================================================
 * Constantae
 * ============================================================ */

/* Layout in characteribus */
#define LAYOUT_MARGIN       II
#define LAYOUT_NOMEN_Y      II
#define LAYOUT_MODE_Y       V
#define LAYOUT_PRESETS_Y    VIII
#define LAYOUT_PALETTE_Y    XII
#define LAYOUT_SLIDERS_Y    XVIII
#define LAYOUT_ZOOM_Y       XXV
#define LAYOUT_BUTTONS_Y    XXVIII

/* ============================================================
 * Functiones Internae - Forward Declarations
 * ============================================================ */

hic_manens vacuum
_dialogus_importatio_reddere(
    Dialogus*        dialogus,
    TabulaPixelorum* tabula,
    i32 x, i32 y,
    i32 latitudo, i32 altitudo,
    f32 scala);

hic_manens DialogusFructus
_dialogus_importatio_tractare_eventum(
    Dialogus*         dialogus,
    constans Eventus* eventus);

hic_manens vacuum
_dialogus_importatio_destruere(Dialogus* dialogus);

/* ============================================================
 * Reddere
 * ============================================================ */

hic_manens vacuum
_dialogus_importatio_reddere(
    Dialogus*        dialogus,
    TabulaPixelorum* tabula,
    i32 x, i32 y,
    i32 latitudo, i32 altitudo,
    f32 scala)
{
    DialogusImportatio* di;
    ImportatioVisus*    visus;
    i32 char_w, char_h;
    i32 px, py;
    i32 base_x, base_y;  /* Positio base in characteribus */
    Color color_text;
    chorda label;
    chorda preset_labels[V];
    s32 i;

    di = (DialogusImportatio*)dialogus->datum;
    si (di == NIHIL || di->visus == NIHIL)
    {
        redde;
    }

    visus = di->visus;
    /* Character dimensiones: 6 pixels latitudo, 8 pixels altitudo */
    char_w = (i32)(6.0f * scala);
    char_h = (i32)(8.0f * scala);
    color_text = thema_color(COLOR_TEXT);

    /* Base position in character coordinates */
    base_x = x;
    base_y = y;

    /* Convertere ad pixela pro text rendering */
    px = x * char_w;
    py = y * char_h;

    /* ---- Nomen label et input ---- */
    {
        i32 label_px, label_py;
        i32 input_char_x, input_char_y;
        FructusCampusTextus fructus;

        /* Label in pixel coordinates */
        label_px = px + (LAYOUT_MARGIN * char_w);
        label_py = py + (LAYOUT_NOMEN_Y * char_h);

        label = chorda_ex_literis("Nomen:", di->piscina);
        tabula_pixelorum_pingere_chordam_scalatam(
            tabula, label_px, label_py, label,
            color_ad_pixelum(color_text), (i32)scala);

        /* Text input in character coordinates (elementa converts to pixels) */
        input_char_x = base_x + LAYOUT_MARGIN + VIII;
        input_char_y = base_y + LAYOUT_NOMEN_Y;
        fructus = elementa_campus_textus(
            tabula, di->piscina,
            input_char_x, input_char_y,
            XX,  /* latitudo in characteribus */
            &di->titulus,
            di->titulus_cursor,
            di->titulus_focused,
            di->eventus_currens,
            scala);

        si (fructus.mutatum)
        {
            di->titulus = *fructus.textus;
            di->titulus_cursor = fructus.cursor;
        }
        di->titulus_focused = fructus.focused;
    }

    /* ---- Modus indicator ---- */
    {
        i32 mode_px, mode_py;
        mode_px = px + (LAYOUT_MARGIN * char_w);
        mode_py = py + (LAYOUT_MODE_Y * char_h);

        si (visus->modus_coloris)
        {
            label = chorda_ex_literis("Modus: Coloris [C/G]", di->piscina);
        }
        alioquin
        {
            label = chorda_ex_literis("Modus: Griseus [C/G]", di->piscina);
        }
        tabula_pixelorum_pingere_chordam_scalatam(
            tabula, mode_px, mode_py, label,
            color_ad_pixelum(color_text), (i32)scala);
    }

    /* ---- Preset buttons ---- */
    {
        i32 btn_char_x, btn_char_y;
        FructusBottone fructus;

        btn_char_y = base_y + LAYOUT_PRESETS_Y;

        preset_labels[0] = chorda_ex_literis("1:Omnes", di->piscina);
        preset_labels[1] = chorda_ex_literis("2:Gris", di->piscina);
        preset_labels[2] = chorda_ex_literis("3:Calid", di->piscina);
        preset_labels[3] = chorda_ex_literis("4:Frig", di->piscina);
        preset_labels[4] = chorda_ex_literis("5:Mono", di->piscina);

        btn_char_x = base_x + LAYOUT_MARGIN;
        per (i = 0; i < V; i++)
        {
            fructus = elementa_bottone(
                tabula, di->piscina,
                btn_char_x, btn_char_y,
                &preset_labels[i],
                di->eventus_currens,
                scala);

            si (fructus.clicked)
            {
                importatio_visus_applicare_preset(visus, i);
            }

            /* Incrementare per label mensura + spacing (in characteribus) */
            btn_char_x += (i32)(preset_labels[i].mensura + II);
        }
    }

    /* ---- Palette grid ---- */
    {
        i32 palette_char_x, palette_char_y;
        FructusGraticulaColorum fructus;

        palette_char_x = base_x + LAYOUT_MARGIN;
        palette_char_y = base_y + LAYOUT_PALETTE_Y;

        fructus = elementa_graticula_colorum(
            tabula, di->piscina,
            palette_char_x, palette_char_y,
            visus->colores_activi,
            di->eventus_currens,
            scala);

        si (fructus.mutatum && fructus.toggled_index >= 0)
        {
            importatio_visus_toggle_color(visus, fructus.toggled_index);
        }
    }

    /* ---- Sliders (solum in griseum mode) ---- */
    si (!visus->modus_coloris)
    {
        i32 sliders_base_char_y;
        chorda slider_labels[III];
        FructusSlider fructus;

        sliders_base_char_y = base_y + LAYOUT_SLIDERS_Y;

        slider_labels[0] = chorda_ex_literis("Punctum 1:", di->piscina);
        slider_labels[1] = chorda_ex_literis("Punctum 2:", di->piscina);
        slider_labels[2] = chorda_ex_literis("Punctum 3:", di->piscina);

        per (i = 0; i < III; i++)
        {
            i32 row_char_y, row_py;
            i32 label_px;
            i32 slider_char_x;

            row_char_y = sliders_base_char_y + (i32)i * II;
            row_py = py + (LAYOUT_SLIDERS_Y + (i32)i * II) * char_h;
            label_px = px + (LAYOUT_MARGIN * char_w);

            tabula_pixelorum_pingere_chordam_scalatam(
                tabula, label_px, row_py,
                slider_labels[i],
                color_ad_pixelum(color_text), (i32)scala);

            slider_char_x = base_x + LAYOUT_MARGIN + XII;
            fructus = elementa_slider(
                tabula, di->piscina,
                slider_char_x, row_char_y,
                XV,  /* latitudo in characteribus */
                visus->puncta[i],
                0, CCLV,
                di->eventus_currens,
                scala);

            si (fructus.mutatum)
            {
                importatio_visus_ponere_punctum(visus, i, fructus.valor);
            }
        }
    }

    /* ---- Zoom slider ---- */
    {
        i32 zoom_char_y, zoom_label_px, zoom_label_py;
        FructusSlider fructus;
        i32 zoom_valor;
        chorda zoom_label;

        zoom_char_y = base_y + LAYOUT_ZOOM_Y;
        zoom_label_px = px + (LAYOUT_MARGIN * char_w);
        zoom_label_py = py + (LAYOUT_ZOOM_Y * char_h);

        zoom_label = chorda_ex_literis("Zoom:", di->piscina);
        tabula_pixelorum_pingere_chordam_scalatam(
            tabula, zoom_label_px, zoom_label_py,
            zoom_label,
            color_ad_pixelum(color_text), (i32)scala);

        /* Convert zoom 1.0-8.0 to slider 0-100 */
        zoom_valor = (i32)((visus->zoom - 1.0f) * 100.0f / 7.0f);
        si (zoom_valor < 0) zoom_valor = 0;
        si (zoom_valor > C) zoom_valor = (i32)C;

        fructus = elementa_slider(
            tabula, di->piscina,
            base_x + LAYOUT_MARGIN + VIII, zoom_char_y,
            XX,  /* latitudo in characteribus */
            zoom_valor,
            0, C,
            di->eventus_currens,
            scala);

        si (fructus.mutatum)
        {
            f32 new_zoom;
            /* zoom = 1.0 + slider * 7.0 / 100 → range 1.0-8.0 */
            new_zoom = 1.0f + (f32)fructus.valor * 7.0f / 100.0f;
            importatio_visus_ponere_zoom(visus, new_zoom);
        }
    }

    /* ---- Action buttons ---- */
    {
        i32 buttons_char_y;
        chorda btn_abicere, btn_salvare;
        FructusBottone fructus;

        buttons_char_y = base_y + LAYOUT_BUTTONS_Y;

        btn_abicere = chorda_ex_literis("[A]bicere", di->piscina);
        btn_salvare = chorda_ex_literis("[S]alvare", di->piscina);

        fructus = elementa_bottone(
            tabula, di->piscina,
            base_x + LAYOUT_MARGIN, buttons_char_y,
            &btn_abicere,
            di->eventus_currens,
            scala);

        si (fructus.clicked)
        {
            di->abicere_clicked = VERUM;
        }

        fructus = elementa_bottone(
            tabula, di->piscina,
            base_x + LAYOUT_MARGIN + XV, buttons_char_y,
            &btn_salvare,
            di->eventus_currens,
            scala);

        si (fructus.clicked)
        {
            di->salvare_clicked = VERUM;
        }
    }

    /* Clariare eventum post rendering ut non processum iterum */
    di->eventus_currens = NIHIL;

    (vacuum)latitudo;
    (vacuum)altitudo;
}

/* ============================================================
 * Tractare Eventum
 * ============================================================ */

hic_manens DialogusFructus
_dialogus_importatio_tractare_eventum(
    Dialogus*         dialogus,
    constans Eventus* eventus)
{
    DialogusImportatio* di;
    ImportatioVisus*    visus;

    di = (DialogusImportatio*)dialogus->datum;
    si (di == NIHIL || di->visus == NIHIL)
    {
        redde DIALOGUS_CURRENS;
    }

    visus = di->visus;

    /* Verificare button clicks (set in reddere on previous frame) */
    si (di->salvare_clicked)
    {
        di->salvare_clicked = FALSUM;
        visus->titulus = di->titulus;
        redde DIALOGUS_CONFIRMATUS;
    }
    si (di->abicere_clicked)
    {
        di->abicere_clicked = FALSUM;
        redde DIALOGUS_ABORTUS;
    }

    /* Salvare eventus pro elementa in reddere
     * Click/release/key events semper salvare.
     * MUS_MOTUS solum salvare si non overwriting release event
     * (bottone necessitat videre MUS_LIBERATUS pro click). */
    si (eventus->genus == EVENTUS_MUS_DEPRESSUS ||
        eventus->genus == EVENTUS_MUS_LIBERATUS ||
        eventus->genus == EVENTUS_CLAVIS_DEPRESSUS ||
        eventus->genus == EVENTUS_MUS_ROTULA)
    {
        di->eventus_currens = eventus;
        di->eventus_mus_x = eventus->datum.mus.x;
        di->eventus_mus_y = eventus->datum.mus.y;
    }
    alioquin si (eventus->genus == EVENTUS_MUS_MOTUS)
    {
        /* Semper renovare positio muris */
        di->eventus_mus_x = eventus->datum.mus.x;
        di->eventus_mus_y = eventus->datum.mus.y;

        /* Solum salvare si non overwriting release event */
        si (di->eventus_currens == NIHIL ||
            di->eventus_currens->genus != EVENTUS_MUS_LIBERATUS)
        {
            di->eventus_currens = eventus;
        }
    }

    /* Tractare keyboard */
    si (eventus->genus == EVENTUS_CLAVIS_DEPRESSUS)
    {
        s32 clavis;
        clavis = (s32)eventus->datum.clavis.clavis;

        /* Escape = abort */
        si (clavis == CLAVIS_EFFUGIUM)
        {
            redde DIALOGUS_ABORTUS;
        }

        /* Enter = confirm (si non in text field) */
        si (clavis == CLAVIS_REDITUS && !di->titulus_focused)
        {
            /* Copiare titulum ad visus */
            visus->titulus = di->titulus;
            redde DIALOGUS_CONFIRMATUS;
        }

        /* 1-5 = presets (solum si non in text field) */
        si (clavis >= '1' && clavis <= '5' && !di->titulus_focused)
        {
            importatio_visus_applicare_preset(visus, clavis - '1');
        }

        /* C/G = toggle color/grayscale mode (solum si non in text field) */
        si ((clavis == 'c' || clavis == 'C') && !di->titulus_focused)
        {
            importatio_visus_ponere_modus(visus, VERUM);
        }
        alioquin si ((clavis == 'g' || clavis == 'G') && !di->titulus_focused)
        {
            importatio_visus_ponere_modus(visus, FALSUM);
        }

        /* S = salvare */
        si ((clavis == 's' || clavis == 'S') && !di->titulus_focused)
        {
            visus->titulus = di->titulus;
            redde DIALOGUS_CONFIRMATUS;
        }

        /* A = abicere */
        si ((clavis == 'a' || clavis == 'A') && !di->titulus_focused)
        {
            redde DIALOGUS_ABORTUS;
        }
    }

    redde DIALOGUS_CURRENS;
}

/* ============================================================
 * Destruere
 * ============================================================ */

hic_manens vacuum
_dialogus_importatio_destruere(Dialogus* dialogus)
{
    /* Nihil speciale - piscina cleanup handles memory */
    (vacuum)dialogus;
}

/* ============================================================
 * API Publica
 * ============================================================ */

DialogusImportatio*
dialogus_importatio_creare(
    Piscina*         piscina,
    ImportatioVisus* visus)
{
    DialogusImportatio* di;

    si (piscina == NIHIL || visus == NIHIL)
    {
        redde NIHIL;
    }

    di = piscina_allocare(piscina, magnitudo(DialogusImportatio));
    si (di == NIHIL)
    {
        redde NIHIL;
    }

    di->piscina = piscina;
    di->visus = visus;

    /* Copiare titulum ab visus */
    di->titulus = visus->titulus;
    di->titulus_cursor = (s32)di->titulus.mensura;
    di->titulus_focused = FALSUM;
    di->focus_index = 0;
    di->eventus_currens = NIHIL;
    di->salvare_clicked = FALSUM;
    di->abicere_clicked = FALSUM;

    /* Configurare dialogus base */
    di->dialogus.datum = di;
    di->dialogus.piscina = piscina;
    di->dialogus.reddere = _dialogus_importatio_reddere;
    di->dialogus.tractare_eventum = _dialogus_importatio_tractare_eventum;
    di->dialogus.destruere = _dialogus_importatio_destruere;
    di->dialogus.titulus = chorda_ex_literis("Importare Imaginem", piscina);

    fprintf(stderr, "Dialogus importatio creatus\n");

    redde di;
}

Dialogus*
dialogus_importatio_obtinere_dialogum(DialogusImportatio* di)
{
    si (di == NIHIL)
    {
        redde NIHIL;
    }

    redde &di->dialogus;
}

b32
dialogus_importatio_obtinere_fructum(
    DialogusImportatio* di,
    constans i8**       indices,
    i32*                latitudo,
    i32*                altitudo,
    chorda*             titulus)
{
    si (di == NIHIL || di->visus == NIHIL)
    {
        redde FALSUM;
    }

    /* Delegare ad visus */
    redde importatio_visus_obtinere_fructum(
        di->visus, indices, latitudo, altitudo, titulus);
}
