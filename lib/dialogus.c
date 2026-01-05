/*
 * dialogus.c - Modal dialog framework implementation
 */

#include "dialogus.h"
#include "delineare.h"
#include "thema.h"
#include "color.h"

/* ============================================================
 * Constantae
 * ============================================================ */

/* Dialog occupa ~80% panel */
#define DIALOGUS_RATIO_LATITUDO 80
#define DIALOGUS_RATIO_ALTITUDO 80

/* Margines frame */
#define DIALOGUS_MARGIN 1
#define DIALOGUS_TITULUS_ALTITUDO 2

/* ============================================================
 * dialogus_computare_limites
 * ============================================================ */

vacuum
dialogus_computare_limites(
    i32  panel_w, i32 panel_h,
    i32* dialog_x, i32* dialog_y,
    i32* dialog_w, i32* dialog_h)
{
    i32 w, h;

    /* Computare dimensiones (~80% panel) */
    w = (panel_w * DIALOGUS_RATIO_LATITUDO) / C;
    h = (panel_h * DIALOGUS_RATIO_ALTITUDO) / C;

    /* Centrare in panel */
    *dialog_x = (panel_w - w) / II;
    *dialog_y = (panel_h - h) / II;
    *dialog_w = w;
    *dialog_h = h;
}

/* ============================================================
 * dialogus_reddere_frame
 * ============================================================ */

vacuum
dialogus_reddere_frame(
    TabulaPixelorum* tabula,
    i32 x, i32 y,
    i32 latitudo, i32 altitudo,
    chorda* titulus,
    f32 scala,
    i32* content_x,
    i32* content_y,
    i32* content_w,
    i32* content_h)
{
    ContextusDelineandi* ctx;
    Piscina* piscina_temp;
    Color color_bg;
    Color color_border;
    Color color_title;
    i32 char_w, char_h;
    i32 px, py, pw, ph;
    i32 title_y;

    /* Creare piscina temporaria pro contextum */
    piscina_temp = piscina_generare_dynamicum("dialogus_frame", M * IV);
    si (piscina_temp == NIHIL)
    {
        redde;
    }

    /* Obtinere colores ex thema */
    color_bg = thema_color(COLOR_BACKGROUND);
    color_border = thema_color(COLOR_BORDER_ACTIVE);
    color_title = thema_color(COLOR_TEXT);

    /* Computare dimensiones character (6x8 ut elementa) */
    char_w = (i32)(6.0f * scala);
    char_h = (i32)(8.0f * scala);

    /* Convertere ad pixela */
    px = x * char_w;
    py = y * char_h;
    pw = latitudo * char_w;
    ph = altitudo * char_h;

    /* Creare contextum delineandi */
    ctx = delineare_creare_contextum(piscina_temp, tabula);
    si (ctx == NIHIL)
    {
        piscina_destruere(piscina_temp);
        redde;
    }

    /* Delineare background plenum */
    delineare_rectangulum_plenum(ctx, px, py, pw, ph, color_bg);

    /* Delineare border */
    delineare_rectangulum(ctx, px, py, pw, ph, color_border);

    /* Delineare linea sub titulo */
    title_y = py + (DIALOGUS_TITULUS_ALTITUDO * char_h);
    delineare_lineam(ctx, px, title_y, px + pw - I, title_y, color_border);

    /* Delineare titulum si datum */
    si (titulus != NIHIL && titulus->datum != NIHIL)
    {
        i32 title_x = px + char_w;
        i32 title_text_y = py + (char_h / 2);
        tabula_pixelorum_pingere_chordam_scalatam(
            tabula,
            title_x, title_text_y,
            *titulus,
            color_ad_pixelum(color_title),
            (i32)scala);
    }

    /* Computare content area */
    *content_x = x + DIALOGUS_MARGIN;
    *content_y = y + DIALOGUS_TITULUS_ALTITUDO + DIALOGUS_MARGIN;
    *content_w = latitudo - (DIALOGUS_MARGIN * II);
    *content_h = altitudo - DIALOGUS_TITULUS_ALTITUDO - (DIALOGUS_MARGIN * II);

    piscina_destruere(piscina_temp);
}

/* ============================================================
 * dialogus_reddere_overlay
 * ============================================================ */

vacuum
dialogus_reddere_overlay(
    Dialogus*        dialogus,
    TabulaPixelorum* tabula,
    i32 panel_x, i32 panel_y,
    i32 panel_w, i32 panel_h,
    f32 scala)
{
    i32 dialog_x, dialog_y, dialog_w, dialog_h;
    i32 content_x, content_y, content_w, content_h;

    si (dialogus == NIHIL)
    {
        redde;
    }

    /* Computare limites dialogi */
    dialogus_computare_limites(
        panel_w, panel_h,
        &dialog_x, &dialog_y,
        &dialog_w, &dialog_h);

    /* Addere offset panel */
    dialog_x += panel_x;
    dialog_y += panel_y;

    /* Dim background (optionale - simplex overlay) */
    /* Pro nunc, non dim - just draw dialog frame */

    /* Delineare frame */
    dialogus_reddere_frame(
        tabula,
        dialog_x, dialog_y,
        dialog_w, dialog_h,
        &dialogus->titulus,
        scala,
        &content_x, &content_y,
        &content_w, &content_h);

    /* Vocare reddere dialogi pro contentum */
    si (dialogus->reddere != NIHIL)
    {
        dialogus->reddere(
            dialogus,
            tabula,
            content_x, content_y,
            content_w, content_h,
            scala);
    }
}
