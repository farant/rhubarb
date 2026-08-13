/*
 * imago_typus.h - Typus imaginis SOLUS (sine implemento)
 *
 * CUR SEPARATIM: typus decodificatorem trahere non debet. Qui
 * imaginem SCRIBIT (imago_png) aut CONFERT (imago_collatio) typo
 * eget, stb_image non eget.
 *
 * MENSURATUM 2026-08-13: cum hic typus in imago.h solo habitaret,
 * vitrea (quae imago_png includit) lib/imago.c ET vendor/stb_image.h
 * in clausuram OMNIS applicationis trahebat - in apps/forum/forum.c
 * visum per `bin/aedilis --partes`.
 *
 * Idem vitium quod limes PNG habebat, gradu inferiore: res quae
 * simul non pertinent uno in capite iunctae. Caput sine plagula
 * implementi nihil trahit - id est totum consilium.
 */

#ifndef IMAGO_TYPUS_H
#define IMAGO_TYPUS_H

#include "latina.h"

/*
 * Imago - Datum imaginis in memoria
 *
 * Pixela in forma RGBA (IV bytes per pixelum), NON praemultiplicata.
 * Datum in piscina allocatum - liberare non opus est.
 */
nomen structura {
    i8* pixela;       /* RGBA datum (IV bytes per pixelum) */
    i32 latitudo;     /* Latitudo in pixelis */
    i32 altitudo;     /* Altitudo in pixelis */
} Imago;

#endif /* IMAGO_TYPUS_H */
