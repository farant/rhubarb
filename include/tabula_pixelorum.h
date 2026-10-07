#ifndef TABULA_PIXELORUM_H
#define TABULA_PIXELORUM_H

/* <aedilis nexus="purus"/> - clausura sine regula nexus (nulla
 * framework; eventus A1b: bin/aedilis --nexus-purus) */

/* <aedilis corpus="lib/fenestra_textus.c"/> */
/* tabula_pixelorum.h - Tabula pixelorum PURA: typus, pixela, colores,
 * textus (fons_6x8) et mensura eius. Sine fenestra, sine Cocoa.
 *
 * Ex fenestra.h divisum (tessellatio T4, 2026-10-01): aedilis per CAPUT
 * nectit - fenestra.h includere fenestra_macos.m + -framework Cocoa
 * trahit (regula variantium), quodcumque vocatur. Rasterizatores
 * (delineare, delineare_mandata) hoc caput solum includunt, ergo
 * programmata terminalis (tessellatio, saltuarius) Cocoa non nectunt.
 * fenestra.h hoc includit: vocantes eius nihil mutatum vident.
 * Implementatio: lib/fenestra_textus.c (nomen servatum: silex et briar
 * clausuram fenestra.h -> fenestra_textus.c asserunt).
 */

#include "latina.h"
#include "chorda.h"
#include "imago_typus.h"


/* ==================================================
 * Typi - Tabula Pixelorum
 * ================================================== */

/* Tabula pixelorum */
nomen structura {
    i32* pixela;              /* Forma RGBA8888 */
    i32  latitudo;            /* Latitudo tabulae in pixelis */
    i32  altitudo;            /* Altitudo tabulae */
    i32  fenestra_latitudo;   /* Latitudo fenestrae actualis */
    i32  fenestra_altitudo;   /* Altitudo fenestrae actualis */
    f32  scala;               /* Factor scalae pro reddendo */
    i32  capacitas;           /* pixela allocata (013 B3b; 0 =
                               * ignota) - ad_fenestram eam reddit */
} TabulaPixelorum;


/* ==================================================
 * Tabula Pixelorum - Manipulatio
 * ================================================== */

/* Vacare tabulam pixelorum
 *
 * Implet tabulam cum colore dato
 *
 * tabula: tabula pixelorum
 * color: color (RGBA8888)
 */
vacuum
tabula_pixelorum_vacare (
    TabulaPixelorum* tabula,
                i32  color);

/* Ponere pixelum
 *
 * tabula: tabula pixelorum
 * x: coordinata x
 * y: coordinata y
 * color: color (RGBA8888)
 */
vacuum
tabula_pixelorum_ponere_pixelum (
    TabulaPixelorum* tabula,
                i32  x,
                i32  y,
                i32  color);

/* Obtinere pixelum
 *
 * tabula: tabula pixelorum
 * x: coordinata x
 * y: coordinata y
 *
 * Reddit: color (RGBA8888)
 */
i32
tabula_pixelorum_obtinere_pixelum (
    TabulaPixelorum* tabula,
                i32  x,
                i32  y);


/* ==================================================
 * Macros - Colores
 * ================================================== */

/* Creare colorem RGBA */
#define RGBA(r, g, b, a) (((i32)(a) << XXIV) | ((i32)(b) << XVI) | ((i32)(g) << VIII) | (i32)(r))

/* Creare colorem RGB (alpha = 255) */
#define RGB(r, g, b) RGBA(r, g, b, CCLV)


/* ==================================================
 * Reddere Textus
 * ================================================== */

/* Pingere characterem
 *
 * tabula: tabula pixelorum
 * x: coordinata x
 * y: coordinata y
 * c: character pingendus
 * color: color textus (RGBA8888)
 */
vacuum
tabula_pixelorum_pingere_characterem (
    TabulaPixelorum* tabula,
                i32  x,
                i32  y,
          character  c,
                i32  color);

/* Cellula textus: progressus characteris et gradus lineae quibus
 * tabula_pixelorum_pingere_chordam utitur (fons_6x8: VI x VIII; scala
 * I). Fons solus veritatem fert - hic declaratur, ne numeri VI et VIII
 * alibi repetantur (tessellatio T2).
 */
vacuum
tabula_pixelorum_cellula_textus (
    i32* latitudo,
    i32* altitudo);

/* Pingere chordam
 *
 * tabula: tabula pixelorum
 * x: coordinata x
 * y: coordinata y
 * chorda: chorda pingenda
 * color: color textus (RGBA8888)
 */
vacuum
tabula_pixelorum_pingere_chordam (
    TabulaPixelorum* tabula,
                i32  x,
                i32  y,
             chorda  textus,
                i32  color);

/* Pingere characterem scalatum
 *
 * tabula: tabula pixelorum
 * x: coordinata x
 * y: coordinata y
 * c: character pingendus
 * color: color textus (RGBA8888)
 * scala: factor scalae (1 = normalis, 2 = duplex, etc.)
 */
vacuum
tabula_pixelorum_pingere_characterem_scalatum (
    TabulaPixelorum* tabula,
                i32  x,
                i32  y,
          character  c,
                i32  color,
                i32  scala);

/* Pingere chordam scalatam
 *
 * tabula: tabula pixelorum
 * x: coordinata x
 * y: coordinata y
 * chorda: chorda pingenda
 * color: color textus (RGBA8888)
 * scala: factor scalae (1 = normalis, 2 = duplex, etc.)
 */
vacuum
tabula_pixelorum_pingere_chordam_scalatam (
    TabulaPixelorum* tabula,
                i32  x,
                i32  y,
             chorda  textus,
                i32  color,
                i32  scala);

/* Pingere chordam praecisum
 *
 * tabula: tabula pixelorum
 * x: coordinata x
 * y: coordinata y
 * chorda: chorda pingenda
 * color: color textus (RGBA8888)
 * praecisio_x: x rectanguli praecisionis
 * praecisio_y: y rectanguli praecisionis
 * praecisio_latitudo: latitudo rectanguli praecisionis
 * praecisio_altitudo: altitudo rectanguli praecisionis
 */
vacuum
tabula_pixelorum_pingere_chordam_praecisum (
    TabulaPixelorum* tabula,
                i32  x,
                i32  y,
             chorda  textus,
                i32  color,
                i32  praecisio_x,
                i32  praecisio_y,
                i32  praecisio_latitudo,
                i32  praecisio_altitudo);


/* ==================================================
 * Mensura Textus
 * ================================================== */

/* Obtinere latitudinem chordae
 *
 * chorda: chorda mensuranda
 * scala: factor scalae
 *
 * Reddit: latitudo in pixelis
 */
i32
fons_latitudo_chordae (
             chorda textus,
                i32 scala);

/* Obtinere altitudinem chordae
 *
 * scala: factor scalae
 *
 * Reddit: altitudo in pixelis
 */
i32
fons_altitudo_chordae (
    i32 scala);

/* Tabulam ad fenestram novam aptare (013 B3b): SCALA SERVATA -
 * latitudo/altitudo = fenestra / scala (pavimentum, >= 1), ut fenestra
 * maior PLUS tabulae ostendat, numquam imaginem distendat. Pixela ex
 * piscina nova solum si capacitas excedit (minor: memoria eadem).
 * fenestra_*: puncta fenestrae (contentum). FALSUM si argumentum
 * NIHIL aut memoria deficit - tabula tunc intacta. Contentum non
 * servatur (quadrum proximum totam pingit). */
b32
tabula_pixelorum_ad_fenestram (
    TabulaPixelorum* tabula,
            Piscina* piscina,
                i32  fenestra_latitudo,
                i32  fenestra_altitudo);

/* Pixela tabulae in imaginem RGBA (octeti R, G, B, A per
 * translationes - ordo octetorum machinae non refert): latitudo x
 * altitudo TABULAE ipsius (non fenestrae, non scala). Pixela ex
 * piscina copiantur. FALSUM si argumentum NIHIL aut memoria deficit
 * (imago tunc intacta). Imagines quadrorum (screenshots) - vide
 * ludus_fenestra_imaginem_scribere. */
b32
tabula_pixelorum_in_imaginem (
    constans TabulaPixelorum* tabula,
                     Piscina* piscina,
                       Imago* imago);

#endif /* TABULA_PIXELORUM_H */
