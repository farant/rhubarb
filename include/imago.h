/*
 * imago.h - Platform-agnostic image loading library
 *
 * Abstractio pro caricare imagines ex file vel memoria.
 * Implementatio interna (stb_image) celata est.
 */

#ifndef IMAGO_H
#define IMAGO_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"

/* ============================================================
 * Typi
 * ============================================================ */

/*
 * Imago - Datum imaginis in memoria
 *
 * Pixela sunt in forma RGBA (4 bytes per pixel).
 * Datum allocatum in piscina, non opus est liberare.
 */
nomen structura {
    i8* pixela;       /* RGBA datum (4 bytes per pixel) */
    i32 latitudo;     /* Latitudo in pixelis */
    i32 altitudo;     /* Altitudo in pixelis */
} Imago;

/*
 * ImagoFructus - Fructus operationis caricare
 */
nomen structura {
    b32    successus; /* VERUM si carricatio successit */
    Imago  imago;     /* Imago carricata (si successus) */
    chorda error;     /* Nuntius erroris (si non successus) */
} ImagoFructus;

/* ============================================================
 * Functiones
 * ============================================================ */

/*
 * imago_caricare_ex_file - Caricare imaginem ex file
 *
 * @via:     Via ad file (PNG, JPEG, BMP, TGA, etc.)
 * @piscina: Piscina pro allocare pixela
 *
 * Redde: ImagoFructus cum successus=VERUM si bene, alioquin error
 */
ImagoFructus
imago_caricare_ex_file(constans character* via, Piscina* piscina);

/*
 * imago_caricare_ex_memoria - Caricare imaginem ex buffer memoriae
 *
 * @datum:   Pointer ad datum imaginis
 * @mensura: Mensura datorum in bytes
 * @piscina: Piscina pro allocare pixela
 *
 * Redde: ImagoFructus cum successus=VERUM si bene, alioquin error
 */
ImagoFructus
imago_caricare_ex_memoria(constans i8* datum, i32 mensura, Piscina* piscina);

#endif /* IMAGO_H */
