/*
 * importatio_visus.h - Import preview widget
 *
 * Widget pro visualizare imaginem dithered durante importatione.
 * Continet statum dithering et output pro dialog et preview.
 */

#ifndef IMPORTATIO_VISUS_H
#define IMPORTATIO_VISUS_H

#include "latina.h"
#include "piscina.h"
#include "fenestra.h"
#include "widget.h"
#include "imago.h"
#include "dithering.h"

/* ============================================================
 * Typi
 * ============================================================ */

nomen structura {
    Piscina*     piscina;
    Piscina*     piscina_scratch;  /* Pro dithering output */

    /* Imago originalis */
    Imago        imago_originalis;
    b32          habet_imaginem;

    /* Canvas (widget) dimensions in pixels */
    i32          canvas_latitudo;
    i32          canvas_altitudo;

    /* Imago scalata pro preview */
    Imago        imago_scalata;
    i32          preview_latitudo;
    i32          preview_altitudo;

    /* Dithering parameters */
    b32          colores_activi[XVI];
    i32          puncta[III];
    b32          modus_coloris;

    /* Dithering output */
    DitheringFructus dithering_fructus;
    i8*          rgba_dithered;
    b32          necesse_redithering;

    /* UI state */
    chorda       titulus;
    s32          titulus_cursor;
    b32          titulus_focused;

    /* View/crop state - zoom and pan control source region selection
     * zoom = 1.0 → entire source fits in preview
     * zoom = 2.0 → 1/2 source width/height visible (2x detail)
     * zoom = 4.0 → 1/4 source width/height visible (4x detail)
     * pan_x, pan_y = center of visible region in source pixels */
    f32          zoom;            /* Zoom factor (1.0 - 8.0) */
    i32          crop_center_x;   /* Center of crop region in source pixels */
    i32          crop_center_y;
    b32          dragging;        /* Currently dragging to pan */
    i32          drag_start_x;    /* Drag start mouse position */
    i32          drag_start_y;
    i32          drag_start_crop_x; /* Crop center at drag start */
    i32          drag_start_crop_y;

    /* Debounce pro zoom/pan - differere redithering */
    f64          tempus_ultima_mutatio;  /* Timestamp ultimae mutationis */
    f32          zoom_pendens;           /* Zoom value pendens */
    i32          crop_center_x_pendens;  /* Crop center pendens */
    i32          crop_center_y_pendens;

    /* Status */
    b32          activus;         /* Import in progressu */
} ImportatioVisus;

/* ============================================================
 * Creatio
 * ============================================================ */

/*
 * importatio_visus_creare - Creare import preview widget
 *
 * @piscina: Piscina memoriae
 *
 * Redde: ImportatioVisus* si successus, NIHIL si error
 */
ImportatioVisus*
importatio_visus_creare(Piscina* piscina);

/* ============================================================
 * Sessio
 * ============================================================ */

/*
 * importatio_visus_initiare_sessionem - Initiare nova import sessio
 *
 * @visus: Import preview widget
 * @imago: Imago importanda (copiatur)
 */
vacuum
importatio_visus_initiare_sessionem(
    ImportatioVisus* visus,
    constans Imago*  imago);

/*
 * importatio_visus_terminare_sessionem - Terminare import sessio
 *
 * @visus: Import preview widget
 */
vacuum
importatio_visus_terminare_sessionem(ImportatioVisus* visus);

/*
 * importatio_visus_est_activus - Quaerere an sessio activa
 *
 * @visus: Import preview widget
 *
 * Redde: VERUM si sessio activa
 */
b32
importatio_visus_est_activus(ImportatioVisus* visus);

/* ============================================================
 * Dithering Control
 * ============================================================ */

/*
 * importatio_visus_applicare_preset - Applicare preset palette
 *
 * @visus: Import preview widget
 * @preset: 0=omnes, 1=griseum, 2=calidus, 3=frigidus, 4=mono
 */
vacuum
importatio_visus_applicare_preset(
    ImportatioVisus* visus,
    s32              preset);

/*
 * importatio_visus_toggle_color - Toggle singulum colorem
 *
 * @visus: Import preview widget
 * @index: Index coloris (0-15)
 */
vacuum
importatio_visus_toggle_color(
    ImportatioVisus* visus,
    s32              index);

/*
 * importatio_visus_ponere_punctum - Ponere breakpoint value
 *
 * @visus: Import preview widget
 * @index: Index puncti (0-2)
 * @valor: Valor novus (0-255)
 */
vacuum
importatio_visus_ponere_punctum(
    ImportatioVisus* visus,
    s32              index,
    i32              valor);

/*
 * importatio_visus_ponere_modus - Ponere modus coloris
 *
 * @visus: Import preview widget
 * @modus_coloris: VERUM = color, FALSUM = griseum
 */
vacuum
importatio_visus_ponere_modus(
    ImportatioVisus* visus,
    b32              modus_coloris);

/*
 * importatio_visus_executare_dithering - Force re-dithering
 *
 * @visus: Import preview widget
 */
vacuum
importatio_visus_executare_dithering(ImportatioVisus* visus);

/*
 * importatio_visus_ponere_zoom - Ponere zoom factor
 *
 * @visus: Import preview widget
 * @zoom: Zoom factor (0.25 - 4.0)
 */
vacuum
importatio_visus_ponere_zoom(
    ImportatioVisus* visus,
    f32              zoom);

/*
 * importatio_visus_reset_view - Reset zoom et pan ad defalta
 *
 * @visus: Import preview widget
 */
vacuum
importatio_visus_reset_view(ImportatioVisus* visus);

/* ============================================================
 * Reddere
 * ============================================================ */

/*
 * importatio_visus_reddere - Reddere preview
 *
 * @visus: Import preview widget
 * @tabula: Tabula pixelorum
 * @x, y: Positio in characteribus
 * @latitudo, altitudo: Dimensiones in characteribus
 * @scala: Factor scalae
 * @focused: An widget habet focus
 */
vacuum
importatio_visus_reddere(
    ImportatioVisus* visus,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused);

/* ============================================================
 * Eventus
 * ============================================================ */

/*
 * importatio_visus_tractare_eventum - Tractare eventum
 *
 * @visus: Import preview widget
 * @eventus: Eventus tractandus
 *
 * Redde: VERUM si eventus tractatus
 */
b32
importatio_visus_tractare_eventum(
    ImportatioVisus*  visus,
    constans Eventus* eventus);

/* ============================================================
 * Fructus
 * ============================================================ */

/*
 * importatio_visus_obtinere_fructum - Obtinere dithered datum
 *
 * @visus: Import preview widget
 * @indices: Output: palette indices
 * @latitudo: Output: latitudo
 * @altitudo: Output: altitudo
 * @titulus: Output: titulus
 *
 * Redde: VERUM si datum disponibilis
 */
b32
importatio_visus_obtinere_fructum(
    ImportatioVisus* visus,
    constans i8**    indices,
    i32*             latitudo,
    i32*             altitudo,
    chorda*          titulus);

#endif /* IMPORTATIO_VISUS_H */
