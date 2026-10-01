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
#include "tabula_pixelorum.h"
#include "imago_typus.h"
#include "mandatum.h"
#include "modulus.h"


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

/* Filtrum primitivorum: VERUM = pinge. Coetus SEMPER ambulantur
 * (transformatio et sectio exactae) - filtrum primitiva sola iudicat.
 * tessellatio (T4) eo via pixelorum sola rasterizat. */
nomen b32 (*DelineareFiltrum)(
    constans Mandatum* x,
               vacuum* ctx);

/* delineare_mandata cum filtro; filtrum NIHIL = omnia (idem ac
 * delineare_mandata). */
vacuum
delineare_mandata_selecta (
      constans Mandata* m,
       TabulaPixelorum* tabula,
             ImagoFons  fons,
                vacuum* ctx,
      DelineareFiltrum  filtrum,
                vacuum* filtrum_ctx);


/* ==================================================
 * Modulus scopi nativi (tessellatio D1, D4)
 * ================================================== */

/* Modulus quem hic scopus DECLARAT: cellula = cellula textus fenestrae
 * (tabula_pixelorum_cellula_textus; textus scalam coetus non sequitur,
 * V1), extensio = tabula. Textus ab hoc rasterizatore pictus intra
 * modulus_textum_metiri(modulus, mensor) iacet (probatum). */
Modulus
delineare_mandata_modulus (
    constans TabulaPixelorum* tabula);

/* Mensor huius scopi: MODULUS_MENSOR_FONTIS (fons_6x8 runam quamque
 * una cellula pingit). */
ModulusMensor
delineare_mandata_mensor (
    vacuum);

#endif /* DELINEARE_MANDATA_H */
