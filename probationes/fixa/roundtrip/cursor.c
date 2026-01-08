#include "cursor.h"
#include "color.h"
#include "thema.h"

/* ==================================================
 * Cursor Bitmap
 *
 * Sagitta 11×17 pixelorum.
 * 0 = transparens
 * 1 = margo (PALETTE_MEDIUM_GRAY)
 * 2 = plenum (PALETTE_BLACK)
 *
 *    0123456789A
 * 0  1..........
 * 1  11.........
 * 2  121........
 * 3  1221.......
 * 4  12221......
 * 5  122221.....
 * 6  1222221....
 * 7  12222221...
 * 8  122222221..
 * 9  1222222221.
 * 10 1222211111.
 * 11 122121.....
 * 12 121.121....
 * 13 11..121....
 * 14 1....121...
 * 15 .....121...
 * 16 ......11...
 *
 * ================================================== */

#define CURSOR_LATITUDO XI
#define CURSOR_ALTITUDO XVII

hic_manens constans i8 cursor_bitmap[CURSOR_ALTITUDO][CURSOR_LATITUDO] = {
    /* 0  */ {1,0,0,0,0,0,0,0,0,0,0},
    /* 1  */ {1,1,0,0,0,0,0,0,0,0,0},
    /* 2  */ {1,2,1,0,0,0,0,0,0,0,0},
    /* 3  */ {1,2,2,1,0,0,0,0,0,0,0},
    /* 4  */ {1,2,2,2,1,0,0,0,0,0,0},
    /* 5  */ {1,2,2,2,2,1,0,0,0,0,0},
    /* 6  */ {1,2,2,2,2,2,1,0,0,0,0},
    /* 7  */ {1,2,2,2,2,2,2,1,0,0,0},
    /* 8  */ {1,2,2,2,2,2,2,2,1,0,0},
    /* 9  */ {1,2,2,2,2,2,2,2,2,1,0},
    /* 10 */ {1,2,2,2,2,1,1,1,1,1,0},
    /* 11 */ {1,2,2,1,2,1,0,0,0,0,0},
    /* 12 */ {1,2,1,0,1,2,1,0,0,0,0},
    /* 13 */ {1,1,0,0,1,2,1,0,0,0,0},
    /* 14 */ {1,0,0,0,0,1,2,1,0,0,0},
    /* 15 */ {0,0,0,0,0,1,2,1,0,0,0},
    /* 16 */ {0,0,0,0,0,0,1,1,0,0,0}
};


/* ==================================================
 * Reddere
 * ================================================== */

vacuum
cursor_reddere(
    TabulaPixelorum* tabula,
    i32 x,
    i32 y)
{
    i32 color_margo;
    i32 color_plenum;
    i32 dy;
    i32 dx;
    i32 px;
    i32 py;
    i8 valor;

    si (!tabula)
    {
        redde;
    }

    /* Obtinere colores ex palette */
    color_margo = color_ad_pixelum(color_ex_palette(PALETTE_MEDIUM_GRAY));
    color_plenum = color_ad_pixelum(color_ex_palette(PALETTE_MEDIUM_DARK_GRAY));

    /* Reddere bitmap */
    per (dy = ZEPHYRUM; dy < CURSOR_ALTITUDO; dy++)
    {
        per (dx = ZEPHYRUM; dx < CURSOR_LATITUDO; dx++)
        {
            valor = cursor_bitmap[dy][dx];

            si (valor == ZEPHYRUM)
            {
                perge;  /* Transparens */
            }

            px = x + dx;
            py = y + dy;

            /* Verificare limites */
            si (px < ZEPHYRUM || px >= tabula->latitudo ||
                py < ZEPHYRUM || py >= tabula->altitudo)
            {
                perge;
            }

            si (valor == I)
            {
                tabula_pixelorum_ponere_pixelum(tabula, px, py, color_margo);
            }
            alioquin si (valor == II)
            {
                tabula_pixelorum_ponere_pixelum(tabula, px, py, color_plenum);
            }
        }
    }
}
