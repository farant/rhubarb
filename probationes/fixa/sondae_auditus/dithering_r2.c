#include "latina.h"
#include "piscina.h"
#include "dithering.h"
#include <stdio.h>

s32 principale(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    b32 colores[XVI];
    i8 rgba[16];
    DitheringFructus f;
    s32 k;
    per (k = 0; k < 16; k++) rgba[k] = (i8)100;
    dithering_praeparare_monochromaticus(colores);
    imprimere("vocatio cum puncta NIHIL, II colores activi...\n");
    fflush(stdout);
    f = dithering_atkinson_griseum(rgba, 2, 2, colores, NIHIL, p);
    imprimere("redditum successus=%d\n", (int)f.successus);
    redde 0;
}
