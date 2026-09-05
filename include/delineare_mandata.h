/* delineare_mandata.h - Mandata -> TabulaPixelorum (rasterizator CPU)
 *
 * Nomen sigillatum: nullum novum substantivum - delineare.h extensum.
 * Cursus unus super elementa: coetus impellit scaenam (origo +=
 * fines + translatio; scala *= scala; sectio ∩= fines si sectio) et
 * post magnitudo_arboris - I elementa cadit. Primitiva: schirmum =
 * origo + locale * scala. Colores per thema.h (COLOR_MANDATI_THEMA
 * -> thema_color; INDEX -> palette; RGBA -> pixelum ipsum).
 *
 * IMAGO per fontem resolvitur (campus textus = fons imaginis ->
 * Imago*): mandata pixela numquam ferunt; identitas imaginis est
 * nomen (pictor: sigillum proiectionis). Alpha 0 = transparens
 * (overlay marquee / ictus pendentis).
 *
 * V1: linea crassitudine I; textus non praecisus; scala integra.
 */

#ifndef DELINEARE_MANDATA_H
#define DELINEARE_MANDATA_H

/* <aedilis corpus="lib/delineare_mandata.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "color.h"
#include "fenestra.h"
#include "imago_typus.h"
#include "mandatum.h"


/* ==================================================
 * Tabula nuda et imago
 * ================================================== */

/* Tabula pixelorum SINE fenestra (probationes, bake, specimina):
 * scala I, dimensiones fenestrae = dimensiones. */
TabulaPixelorum*
tabula_pixelorum_creare_nuda (
     Piscina* piscina,
         i32  latitudo,
         i32  altitudo);

/* Eadem memoria, nulla copia: pixelum i32 (ABGR in verbo) iacet
 * R,G,B,A in memoria - forma Imaginis. */
Imago
imago_ex_tabula (
    constans TabulaPixelorum* tabula);

Color
color_ex_mandato (
    ColorMandati color);


/* ==================================================
 * Rasterizatio
 * ================================================== */

nomen constans Imago* (*ImagoFons)(
    chorda provenientia,
    vacuum* ctx);

vacuum
delineare_mandata (
     constans Mandata* m,
      TabulaPixelorum* tabula,
            ImagoFons  fons,
               vacuum* ctx);

#endif /* DELINEARE_MANDATA_H */
