/*
 * importatio_visus.c - Import preview widget implementation
 */

#include "importatio_visus.h"
#include "imago_opus.h"
#include "thema.h"
#include "color.h"
#include "tempus.h"
#include "chorda_aedificator.h"
#include <stdio.h>

/* ============================================================
 * Functiones Internae
 * ============================================================ */

hic_manens vacuum
_generare_titulum_defaltum(ImportatioVisus* visus)
{
    ChordaAedificator* aed;
    f64 nunc;

    aed = chorda_aedificator_creare(visus->piscina, LXIV);
    si (aed == NIHIL)
    {
        visus->titulus = chorda_ex_literis("imago", visus->piscina);
        redde;
    }

    nunc = tempus_nunc();
    chorda_aedificator_appendere_literis(aed, "imago_");
    chorda_aedificator_appendere_s32(aed, (s32)nunc);

    visus->titulus = chorda_aedificator_finire(aed);
}

hic_manens vacuum
_praeparare_imaginem(ImportatioVisus* visus)
{
    s32 src_w, src_h;
    s32 canvas_w, canvas_h;
    f32 base_scale;        /* Scale to fit source in canvas at zoom=1.0 */
    f32 display_scale;     /* Actual scale = base_scale * zoom */
    s32 crop_w, crop_h;    /* Visible region in source pixels */
    s32 crop_x, crop_y;
    s32 output_w, output_h;
    memoriae_index mensura;

    si (!visus->habet_imaginem)
    {
        redde;
    }

    src_w = (s32)visus->imago_originalis.latitudo;
    src_h = (s32)visus->imago_originalis.altitudo;
    canvas_w = (s32)visus->canvas_latitudo;
    canvas_h = (s32)visus->canvas_altitudo;

    si (canvas_w < 1) canvas_w = 1;
    si (canvas_h < 1) canvas_h = 1;

    /* Compute base scale: at zoom=1.0, entire source fits in canvas */
    {
        f32 scale_x = (f32)canvas_w / (f32)src_w;
        f32 scale_y = (f32)canvas_h / (f32)src_h;
        base_scale = (scale_x < scale_y) ? scale_x : scale_y;
    }

    /* Display scale = base * zoom (zoom magnifies the image) */
    display_scale = base_scale * visus->zoom;

    /* Crop region = what part of source is visible at this scale
     * visible_size = canvas_size / display_scale */
    crop_w = (s32)((f32)canvas_w / display_scale);
    crop_h = (s32)((f32)canvas_h / display_scale);

    /* Clamp crop to source size */
    si (crop_w > src_w) crop_w = src_w;
    si (crop_h > src_h) crop_h = src_h;
    si (crop_w < 1) crop_w = 1;
    si (crop_h < 1) crop_h = 1;

    /* Compute crop origin from center position */
    crop_x = (s32)visus->crop_center_x - (crop_w / II);
    crop_y = (s32)visus->crop_center_y - (crop_h / II);

    /* Clamp crop to source bounds */
    si (crop_x < 0) crop_x = 0;
    si (crop_y < 0) crop_y = 0;
    si (crop_x + crop_w > src_w) crop_x = src_w - crop_w;
    si (crop_y + crop_h > src_h) crop_y = src_h - crop_h;

    /* Update crop center to reflect clamping */
    visus->crop_center_x = (i32)(crop_x + (crop_w / II));
    visus->crop_center_y = (i32)(crop_y + (crop_h / II));

    /* Output size = crop scaled by display_scale, clamped to canvas */
    output_w = (s32)((f32)crop_w * display_scale);
    output_h = (s32)((f32)crop_h * display_scale);
    si (output_w > canvas_w) output_w = canvas_w;
    si (output_h > canvas_h) output_h = canvas_h;
    si (output_w < 1) output_w = 1;
    si (output_h < 1) output_h = 1;

    /* Extract crop and scale to output size */
    visus->imago_scalata = imago_extrahere_et_scalare(
        &visus->imago_originalis,
        (i32)crop_x, (i32)crop_y, (i32)crop_w, (i32)crop_h,
        (i32)output_w,
        (i32)output_h,
        IMAGO_SCALA_PROXIMUS,
        visus->piscina);

    si (visus->imago_scalata.pixela != NIHIL)
    {
        visus->preview_latitudo = (i32)visus->imago_scalata.latitudo;
        visus->preview_altitudo = (i32)visus->imago_scalata.altitudo;

        /* Allocare buffer pro RGBA dithered */
        mensura = (memoriae_index)(visus->preview_latitudo *
                                   visus->preview_altitudo * IV);
        visus->rgba_dithered = (i8*)piscina_allocare(visus->piscina, mensura);
        visus->necesse_redithering = VERUM;
    }
}

hic_manens vacuum
_executare_dithering_internum(ImportatioVisus* visus)
{
    si (!visus->habet_imaginem ||
        visus->imago_scalata.pixela == NIHIL ||
        visus->rgba_dithered == NIHIL)
    {
        redde;
    }

    /* Use scratch piscina pro dithering */

    si (visus->modus_coloris)
    {
        visus->dithering_fructus = dithering_atkinson_colorum(
            visus->imago_scalata.pixela,
            visus->imago_scalata.latitudo,
            visus->imago_scalata.altitudo,
            visus->colores_activi,
            visus->piscina_scratch);
    }
    alioquin
    {
        visus->dithering_fructus = dithering_atkinson_griseum(
            visus->imago_scalata.pixela,
            visus->imago_scalata.latitudo,
            visus->imago_scalata.altitudo,
            visus->colores_activi,
            visus->puncta,
            visus->piscina_scratch);
    }

    si (visus->dithering_fructus.successus)
    {
        dithering_indices_ad_rgba(
            visus->dithering_fructus.indices,
            visus->rgba_dithered,
            visus->preview_latitudo,
            visus->preview_altitudo);
    }

    visus->necesse_redithering = FALSUM;
}

/* ============================================================
 * Creatio
 * ============================================================ */

ImportatioVisus*
importatio_visus_creare(Piscina* piscina)
{
    ImportatioVisus* visus;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }

    visus = piscina_allocare(piscina, magnitudo(ImportatioVisus));
    si (visus == NIHIL)
    {
        redde NIHIL;
    }

    visus->piscina = piscina;
    visus->piscina_scratch = piscina_generare_dynamicum("importatio_scratch", M * X);
    visus->habet_imaginem = FALSUM;
    visus->activus = FALSUM;

    /* Initiare canvas dimensions (will be set properly in reddere) */
    visus->canvas_latitudo = 0;
    visus->canvas_altitudo = 0;

    /* Initiare dithering parameters */
    dithering_praeparare_omnes(visus->colores_activi);
    visus->puncta[0] = LXIV;
    visus->puncta[1] = CXXVIII;
    visus->puncta[2] = CXCII;
    visus->modus_coloris = VERUM;

    /* Initiare dithering fructus */
    visus->dithering_fructus.successus = FALSUM;
    visus->dithering_fructus.indices = NIHIL;
    visus->rgba_dithered = NIHIL;

    /* Initiare UI state */
    visus->titulus.datum = NIHIL;
    visus->titulus.mensura = ZEPHYRUM;
    visus->titulus_cursor = ZEPHYRUM;
    visus->titulus_focused = FALSUM;

    /* Initiare view state */
    visus->zoom = 1.0f;
    visus->crop_center_x = 0;
    visus->crop_center_y = 0;
    visus->dragging = FALSUM;
    visus->drag_start_x = 0;
    visus->drag_start_y = 0;
    visus->drag_start_crop_x = 0;
    visus->drag_start_crop_y = 0;

    /* Initiare debounce state */
    visus->tempus_ultima_mutatio = 0.0;
    visus->zoom_pendens = 1.0f;
    visus->crop_center_x_pendens = 0;
    visus->crop_center_y_pendens = 0;

    redde visus;
}

/* ============================================================
 * Sessio
 * ============================================================ */

vacuum
importatio_visus_initiare_sessionem(
    ImportatioVisus* visus,
    constans Imago*  imago)
{
    si (visus == NIHIL || imago == NIHIL)
    {
        redde;
    }

    /* Copiare imaginem */
    visus->imago_originalis = *imago;
    visus->habet_imaginem = VERUM;
    visus->activus = VERUM;

    /* Reset parameters ad defaults */
    dithering_praeparare_omnes(visus->colores_activi);
    visus->puncta[0] = LXIV;
    visus->puncta[1] = CXXVIII;
    visus->puncta[2] = CXCII;
    visus->modus_coloris = VERUM;

    /* Generare titulum defaltum */
    _generare_titulum_defaltum(visus);
    visus->titulus_cursor = (s32)visus->titulus.mensura;

    /* Reset view state - center on source image */
    visus->zoom = 1.0f;
    visus->crop_center_x = (i32)imago->latitudo / II;
    visus->crop_center_y = (i32)imago->altitudo / II;
    visus->dragging = FALSUM;

    /* Reset debounce - sync pending to current */
    visus->zoom_pendens = visus->zoom;
    visus->crop_center_x_pendens = visus->crop_center_x;
    visus->crop_center_y_pendens = visus->crop_center_y;
    visus->tempus_ultima_mutatio = 0.0;

    /* Praeparare et dithering */
    _praeparare_imaginem(visus);
    _executare_dithering_internum(visus);

    fprintf(stderr, "Importatio sessio initiata: %d x %d\n",
        visus->preview_latitudo, visus->preview_altitudo);
}

vacuum
importatio_visus_terminare_sessionem(ImportatioVisus* visus)
{
    si (visus == NIHIL)
    {
        redde;
    }

    visus->activus = FALSUM;
    visus->habet_imaginem = FALSUM;

    fprintf(stderr, "Importatio sessio terminata\n");
}

b32
importatio_visus_est_activus(ImportatioVisus* visus)
{
    si (visus == NIHIL)
    {
        redde FALSUM;
    }

    redde visus->activus;
}

/* ============================================================
 * Dithering Control
 * ============================================================ */

vacuum
importatio_visus_applicare_preset(
    ImportatioVisus* visus,
    s32              preset)
{
    si (visus == NIHIL)
    {
        redde;
    }

    commutatio (preset)
    {
        casus 0:
            dithering_praeparare_omnes(visus->colores_activi);
            frange;
        casus 1:
            dithering_praeparare_griseum(visus->colores_activi);
            frange;
        casus 2:
            dithering_praeparare_calidus(visus->colores_activi);
            frange;
        casus 3:
            dithering_praeparare_frigidus(visus->colores_activi);
            frange;
        casus 4:
            dithering_praeparare_monochromaticus(visus->colores_activi);
            frange;
        ordinarius:
            frange;
    }

    visus->necesse_redithering = VERUM;
}

vacuum
importatio_visus_toggle_color(
    ImportatioVisus* visus,
    s32              index)
{
    si (visus == NIHIL || index < 0 || index >= XVI)
    {
        redde;
    }

    visus->colores_activi[index] = !visus->colores_activi[index];
    visus->necesse_redithering = VERUM;
}

vacuum
importatio_visus_ponere_punctum(
    ImportatioVisus* visus,
    s32              index,
    i32              valor)
{
    si (visus == NIHIL || index < 0 || index >= III)
    {
        redde;
    }

    visus->puncta[index] = valor;
    visus->necesse_redithering = VERUM;
}

vacuum
importatio_visus_ponere_modus(
    ImportatioVisus* visus,
    b32              modus_coloris)
{
    si (visus == NIHIL)
    {
        redde;
    }

    visus->modus_coloris = modus_coloris;
    visus->necesse_redithering = VERUM;
}

vacuum
importatio_visus_executare_dithering(ImportatioVisus* visus)
{
    si (visus == NIHIL)
    {
        redde;
    }

    visus->necesse_redithering = VERUM;
}

vacuum
importatio_visus_ponere_zoom(
    ImportatioVisus* visus,
    f32              zoom)
{
    si (visus == NIHIL)
    {
        redde;
    }

    /* Clamp zoom to valid range
     * zoom = 1.0 → entire source visible
     * zoom = 8.0 → 1/8 of source visible (more detail) */
    si (zoom < 1.0f)
    {
        zoom = 1.0f;
    }
    si (zoom > 8.0f)
    {
        zoom = 8.0f;
    }

    si (visus->zoom != zoom)
    {
        /* Apply immediately - no debounce */
        visus->zoom = zoom;
        visus->zoom_pendens = zoom;
        visus->necesse_redithering = VERUM;
    }
}

vacuum
importatio_visus_reset_view(ImportatioVisus* visus)
{
    si (visus == NIHIL)
    {
        redde;
    }

    visus->zoom = 1.0f;
    /* Reset crop center to center of source image */
    si (visus->habet_imaginem)
    {
        visus->crop_center_x = (i32)visus->imago_originalis.latitudo / II;
        visus->crop_center_y = (i32)visus->imago_originalis.altitudo / II;
    }
    visus->dragging = FALSUM;

    /* Sync pending values - immediate reset */
    visus->zoom_pendens = visus->zoom;
    visus->crop_center_x_pendens = visus->crop_center_x;
    visus->crop_center_y_pendens = visus->crop_center_y;
    visus->tempus_ultima_mutatio = 0.0;

    /* Immediate re-dither pro reset */
    visus->necesse_redithering = VERUM;
}

/* ============================================================
 * Reddere
 * ============================================================ */

vacuum
importatio_visus_reddere(
    ImportatioVisus* visus,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused)
{
    i32 char_w, char_h;
    i32 px, py, pw, ph;
    i32 dest_x, dest_y;
    i32 img_x, img_y;
    Color color_text;
    chorda label;

    (vacuum)focused;

    si (visus == NIHIL || tabula == NIHIL)
    {
        redde;
    }

    char_w = VI * scala;  /* 6 pixels per character width */
    char_h = VIII * scala; /* 8 pixels per character height */

    /* Update canvas dimensions from widget size (with small border) */
    {
        i32 padding_top = 1;
        i32 padding_bottom = 2;
        i32 new_canvas_w = latitudo * char_w;
        i32 new_canvas_h = altitudo * char_h - padding_top - padding_bottom;

        /* If canvas size changed, need to re-prepare */
        si (new_canvas_w != (i32)visus->canvas_latitudo ||
            new_canvas_h != (i32)visus->canvas_altitudo)
        {
            visus->canvas_latitudo = (i32)new_canvas_w;
            visus->canvas_altitudo = (i32)new_canvas_h;
            visus->necesse_redithering = VERUM;
        }
    }

    /* Praeparare et executare dithering si necesse */
    si (visus->necesse_redithering && visus->activus)
    {
        _praeparare_imaginem(visus);
        _executare_dithering_internum(visus);
    }
    color_text = thema_color(COLOR_TEXT);

    /* Convertere ad pixela */
    px = x * char_w;
    py = y * char_h;
    pw = latitudo * char_w;
    ph = altitudo * char_h;

    /* Si non activus, monstrare message */
    si (!visus->activus)
    {
        label = chorda_ex_literis("Nulla importatio activa", visus->piscina);
        tabula_pixelorum_pingere_chordam_scalatam(
            tabula,
            px + (pw / II) - ((i32)label.mensura * char_w / II),
            py + (ph / II),
            label,
            color_ad_pixelum(color_text),
            scala);
        redde;
    }

    /* Delineare imaginem dithered */
    si (visus->dithering_fructus.successus && visus->rgba_dithered != NIHIL)
    {
        /* Center image in panel */
        dest_x = px + (pw - visus->preview_latitudo) / II;
        dest_y = py + (ph - visus->preview_altitudo) / II;

        /* Delineare imaginem */
        per (img_y = 0; img_y < visus->preview_altitudo; img_y++)
        {
            per (img_x = 0; img_x < visus->preview_latitudo; img_x++)
            {
                i32 src_idx, dx, dy;
                insignatus character r, g, b;

                src_idx = (img_y * visus->preview_latitudo + img_x) * IV;

                dx = dest_x + img_x;
                dy = dest_y + img_y;

                /* Clip to panel bounds */
                si (dx >= px && dx < px + pw &&
                    dy >= py && dy < py + ph &&
                    dx >= 0 && dx < (i32)tabula->latitudo &&
                    dy >= 0 && dy < (i32)tabula->altitudo)
                {
                    i32 dest_idx_pixel;
                    dest_idx_pixel = dy * (i32)tabula->latitudo + dx;
                    r = (insignatus character)visus->rgba_dithered[src_idx + 0];
                    g = (insignatus character)visus->rgba_dithered[src_idx + 1];
                    b = (insignatus character)visus->rgba_dithered[src_idx + 2];
                    tabula->pixela[dest_idx_pixel] = (i32)RGB(r, g, b);
                }
            }
        }
    }
}

/* ============================================================
 * Eventus
 * ============================================================ */

b32
importatio_visus_tractare_eventum(
    ImportatioVisus*  visus,
    constans Eventus* eventus)
{
    si (visus == NIHIL || eventus == NIHIL)
    {
        redde FALSUM;
    }

    si (!visus->activus)
    {
        redde FALSUM;
    }

    /* Tractare mouse drag pro panning crop center */
    si (eventus->genus == EVENTUS_MUS_DEPRESSUS)
    {
        visus->dragging = VERUM;
        visus->drag_start_x = eventus->datum.mus.x;
        visus->drag_start_y = eventus->datum.mus.y;
        /* Use pending values as starting point */
        visus->drag_start_crop_x = visus->crop_center_x_pendens;
        visus->drag_start_crop_y = visus->crop_center_y_pendens;
        redde VERUM;
    }

    si (eventus->genus == EVENTUS_MUS_LIBERATUS)
    {
        si (visus->dragging)
        {
            visus->dragging = FALSUM;
            /* Re-dither iam factum in MUS_MOTUS */
        }
        redde VERUM;
    }

    si (eventus->genus == EVENTUS_MUS_MOTUS && visus->dragging)
    {
        s32 delta_x, delta_y;       /* Signed - can be negative */
        s32 src_w, src_h;
        s32 crop_w, crop_h;
        s32 new_center_x, new_center_y;  /* Signed for clamping logic */

        /* Compute how much mouse moved in pixels */
        delta_x = (s32)eventus->datum.mus.x - (s32)visus->drag_start_x;
        delta_y = (s32)eventus->datum.mus.y - (s32)visus->drag_start_y;

        /* Get source and crop dimensions (use pending zoom) */
        src_w = (s32)visus->imago_originalis.latitudo;
        src_h = (s32)visus->imago_originalis.altitudo;
        crop_w = (s32)((f32)src_w / visus->zoom_pendens);
        crop_h = (s32)((f32)src_h / visus->zoom_pendens);

        /* Convert pixel delta to source delta (inverse of scale) */
        /* Dragging right should move crop left in source */
        new_center_x = (s32)visus->drag_start_crop_x -
            (delta_x * crop_w) / (s32)visus->preview_latitudo;
        new_center_y = (s32)visus->drag_start_crop_y -
            (delta_y * crop_h) / (s32)visus->preview_altitudo;

        /* Clamp crop center to valid range */
        si (new_center_x < crop_w / II)
            new_center_x = crop_w / II;
        si (new_center_y < crop_h / II)
            new_center_y = crop_h / II;
        si (new_center_x > src_w - crop_w / II)
            new_center_x = src_w - crop_w / II;
        si (new_center_y > src_h - crop_h / II)
            new_center_y = src_h - crop_h / II;

        /* Apply immediately - no debounce (values are clamped positive) */
        visus->crop_center_x = (i32)new_center_x;
        visus->crop_center_y = (i32)new_center_y;
        visus->crop_center_x_pendens = (i32)new_center_x;
        visus->crop_center_y_pendens = (i32)new_center_y;
        visus->necesse_redithering = VERUM;

        redde VERUM;
    }

    /* Tractare scroll pro zoom */
    si (eventus->genus == EVENTUS_MUS_ROTULA)
    {
        f32 zoom_delta;
        zoom_delta = eventus->datum.rotula.delta_y * 0.5f;
        importatio_visus_ponere_zoom(visus, visus->zoom + zoom_delta);
        redde VERUM;
    }

    redde FALSUM;
}

/* ============================================================
 * Fructus
 * ============================================================ */

b32
importatio_visus_obtinere_fructum(
    ImportatioVisus* visus,
    constans i8**    indices,
    i32*             latitudo,
    i32*             altitudo,
    chorda*          titulus)
{
    si (visus == NIHIL || !visus->activus || !visus->dithering_fructus.successus)
    {
        redde FALSUM;
    }

    si (indices)
    {
        *indices = visus->dithering_fructus.indices;
    }
    si (latitudo)
    {
        *latitudo = visus->preview_latitudo;
    }
    si (altitudo)
    {
        *altitudo = visus->preview_altitudo;
    }
    si (titulus)
    {
        *titulus = visus->titulus;
    }

    redde VERUM;
}
