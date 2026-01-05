/*
 * dialogus.h - Modal dialog framework
 *
 * Systema pro dialogi modales super widgets.
 * Dialogus captat omnes eventus quando activus est.
 */

#ifndef DIALOGUS_H
#define DIALOGUS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "fenestra.h"

/* ============================================================
 * Typi
 * ============================================================ */

/*
 * DialogusFructus - Fructus tractationis eventuum
 */
nomen enumeratio {
    DIALOGUS_CURRENS,      /* Dialogus adhuc activus */
    DIALOGUS_CONFIRMATUS,  /* Usator confirmavit (Salvare) */
    DIALOGUS_ABORTUS       /* Usator abortavit (Abicere/Escape) */
} DialogusFructus;

/*
 * Dialogus - Interfacies generica pro dialogis
 */
nomen structura Dialogus Dialogus;

struct Dialogus {
    vacuum*  datum;       /* Datum specificum dialogi */
    Piscina* piscina;     /* Piscina pro allocationes */

    /*
     * reddere - Delineare contentum dialogi
     *
     * @dialogus:  Dialogus
     * @tabula:    Tabula pixelorum
     * @x, y:      Positio content area (post frame)
     * @latitudo:  Latitudo content area
     * @altitudo:  Altitudo content area
     * @scala:     Scala fontis
     */
    vacuum (*reddere)(
        Dialogus*        dialogus,
        TabulaPixelorum* tabula,
        i32 x, i32 y,
        i32 latitudo, i32 altitudo,
        f32 scala);

    /*
     * tractare_eventum - Tractare eventum
     *
     * @dialogus:  Dialogus
     * @eventus:   Eventus tractandus
     *
     * Redde: DialogusFructus
     */
    DialogusFructus (*tractare_eventum)(
        Dialogus*         dialogus,
        constans Eventus* eventus);

    /*
     * destruere - Cleanup quando dialogus clauditur
     */
    vacuum (*destruere)(Dialogus* dialogus);

    /* Titulus dialogi */
    chorda titulus;
};

/* ============================================================
 * Functiones
 * ============================================================ */

/*
 * dialogus_reddere_frame - Delineare frame dialogi
 *
 * Delineat background, border, et titulum.
 * Redde positio et dimensiones content area.
 *
 * @tabula:    Tabula pixelorum
 * @x, y:      Positio dialogi (in characteribus)
 * @latitudo:  Latitudo dialogi (in characteribus)
 * @altitudo:  Altitudo dialogi (in characteribus)
 * @titulus:   Titulus dialogi (vel NIHIL)
 * @scala:     Scala fontis
 * @content_x: Output: x content area
 * @content_y: Output: y content area
 * @content_w: Output: latitudo content area
 * @content_h: Output: altitudo content area
 */
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
    i32* content_h);

/*
 * dialogus_reddere_overlay - Delineare dialogum cum frame
 *
 * Functio completa: dim background + frame + content
 *
 * @dialogus:     Dialogus
 * @tabula:       Tabula pixelorum
 * @panel_x:      Positio panel (in characteribus)
 * @panel_y:      Positio panel (in characteribus)
 * @panel_w:      Latitudo panel (in characteribus)
 * @panel_h:      Altitudo panel (in characteribus)
 * @scala:        Scala fontis
 */
vacuum
dialogus_reddere_overlay(
    Dialogus*        dialogus,
    TabulaPixelorum* tabula,
    i32 panel_x, i32 panel_y,
    i32 panel_w, i32 panel_h,
    f32 scala);

/*
 * dialogus_computare_limites - Computare positio et dimensiones dialogi
 *
 * Dialogus est ~80% panel, centratum.
 *
 * @panel_w:     Latitudo panel (in characteribus)
 * @panel_h:     Altitudo panel (in characteribus)
 * @dialog_x:    Output: offset x in panel
 * @dialog_y:    Output: offset y in panel
 * @dialog_w:    Output: latitudo dialogi
 * @dialog_h:    Output: altitudo dialogi
 */
vacuum
dialogus_computare_limites(
    i32  panel_w, i32 panel_h,
    i32* dialog_x, i32* dialog_y,
    i32* dialog_w, i32* dialog_h);

#endif /* DIALOGUS_H */
