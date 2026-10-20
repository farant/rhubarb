#include "latina.h"
#include "piscina.h"
#include "dithering.h"
#include <stdio.h>

s32 principale(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    b32 colores[XVI];
    i32 puncta[III];
    s32 lat = 256, alt = 8, x, y, k;
    i8* rgba;
    s32 numeri[XVI];
    DitheringFructus f;
    s32 valores[IV];
    s32 v;

    puncta[0] = 64; puncta[1] = 128; puncta[2] = 192;
    dithering_praeparare_griseum(colores);

    /* gradiens 0..255 in R=G=B */
    rgba = (i8*)piscina_allocare(p, (memoriae_index)(lat * alt * 4));
    per (y = 0; y < alt; y++) per (x = 0; x < lat; x++)
    {
        s32 o = (y * lat + x) * 4;
        rgba[o] = rgba[o+1] = rgba[o+2] = (i8)x; rgba[o+3] = (i8)255;
    }
    f = dithering_atkinson_griseum(rgba, (i32)lat, (i32)alt, colores, puncta, p);
    per (k = 0; k < XVI; k++) numeri[k] = 0;
    per (k = 0; k < lat * alt; k++) numeri[f.indices[k]]++;
    imprimere("gradiens successus=%d\n", (int)f.successus);
    per (k = 0; k < XVI; k++) si (colores[k]) imprimere("  color %d: %d pixela\n", (int)k, (int)numeri[k]);

    /* imagines uniformes: color 3 (lum 185) et color 4 (lum 217) ipsi */
    valores[0] = 3; valores[1] = 4; valores[2] = 15; valores[3] = 5;
    per (v = 0; v < IV; v++)
    {
        s32 c = valores[v];
        s32 summa = 0;
        s32 lumin, n = 64 * 64;
        i8* r2 = (i8*)piscina_allocare(p, (memoriae_index)(n * 4));
        per (k = 0; k < n; k++)
        {
            r2[k*4] = AQUINAS_PALETTE[c][0]; r2[k*4+1] = AQUINAS_PALETTE[c][1];
            r2[k*4+2] = AQUINAS_PALETTE[c][2]; r2[k*4+3] = (i8)255;
        }
        lumin = (77 * (s32)AQUINAS_PALETTE[c][0] + 150 * (s32)AQUINAS_PALETTE[c][1] + 28 * (s32)AQUINAS_PALETTE[c][2]) >> 8;
        f = dithering_atkinson_griseum(r2, 64, 64, colores, puncta, p);
        per (k = 0; k < XVI; k++) numeri[k] = 0;
        per (k = 0; k < n; k++)
        {
            s32 ci = f.indices[k];
            numeri[ci]++;
            summa += (77 * (s32)AQUINAS_PALETTE[ci][0] + 150 * (s32)AQUINAS_PALETTE[ci][1] + 28 * (s32)AQUINAS_PALETTE[ci][2]) >> 8;
        }
        imprimere("uniformis color %d (lum %d): media lum output %d;", (int)c, (int)lumin, (int)(summa / n));
        per (k = 0; k < XVI; k++) si (numeri[k]) imprimere(" c%d=%d", (int)k, (int)numeri[k]);
        imprimere("\n");
    }
    redde 0;
}
