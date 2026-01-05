/*
 * dialogus_importatio.h - Import dialog for dithering images
 *
 * Dialogus pro importare imagines cum dithering Atkinson.
 * Continet solum controllos - preview est in importatio_visus.
 * Utitur elementa pro controllis GUI.
 */

#ifndef DIALOGUS_IMPORTATIO_H
#define DIALOGUS_IMPORTATIO_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "dialogus.h"
#include "imago.h"
#include "dithering.h"
#include "importatio_visus.h"

/* ============================================================
 * Typi
 * ============================================================ */

nomen structura DialogusImportatio DialogusImportatio;

struct DialogusImportatio {
    Piscina*          piscina;
    Dialogus          dialogus;       /* Base dialog (embedded) */
    ImportatioVisus*  visus;          /* Preview widget (external) */

    /* UI state */
    chorda            titulus;        /* Name field content */
    s32               titulus_cursor;
    b32               titulus_focused;
    s32               focus_index;    /* 0=titulus, 1-5=presets, 6=palette, 7-9=sliders */

    /* Eventus currentis (pro elementa) */
    constans Eventus* eventus_currens;

    /* Mouse position (renovatur per omnes mouse events) */
    i32               eventus_mus_x;
    i32               eventus_mus_y;
};

/* ============================================================
 * Functiones
 * ============================================================ */

/*
 * dialogus_importatio_creare - Creare dialogum importationis
 *
 * @piscina: Piscina pro allocationes
 * @visus:   ImportatioVisus pro preview (iam initiatus cum imagine)
 *
 * Redde: DialogusImportatio* si successus, NIHIL si error
 */
DialogusImportatio*
dialogus_importatio_creare(
    Piscina*         piscina,
    ImportatioVisus* visus);

/*
 * dialogus_importatio_obtinere_dialogum - Obtinere Dialogus* pro schirmata
 *
 * @di: DialogusImportatio
 *
 * Redde: Dialogus* pro schirmata_aperire_dialogum
 */
Dialogus*
dialogus_importatio_obtinere_dialogum(DialogusImportatio* di);

/*
 * dialogus_importatio_obtinere_fructum - Obtinere dithered datum post confirmatio
 *
 * Delegat ad importatio_visus_obtinere_fructum
 *
 * @di:        DialogusImportatio
 * @indices:   Output: palette indices (latitudo * altitudo bytes)
 * @latitudo:  Output: latitudo imaginis
 * @altitudo:  Output: altitudo imaginis
 * @titulus:   Output: titulus imaginis
 *
 * Redde: VERUM si datum disponibilis
 */
b32
dialogus_importatio_obtinere_fructum(
    DialogusImportatio* di,
    constans i8**       indices,
    i32*                latitudo,
    i32*                altitudo,
    chorda*             titulus);

#endif /* DIALOGUS_IMPORTATIO_H */
