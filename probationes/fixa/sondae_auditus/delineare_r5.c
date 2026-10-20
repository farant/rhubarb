#include "latina.h"
#include "piscina.h"
#include "delineare.h"
#include <stdio.h>

static TabulaPixelorum t;
static Piscina* p;

static ContextusDelineandi* novum(void)
{
    int k;
    t.latitudo = 64; t.altitudo = 64;
    t.pixela = (i32*)piscina_allocare(p, 64 * 64 * sizeof(i32));
    for (k = 0; k < 64 * 64; k++) t.pixela[k] = 0;
    return delineare_creare_contextum(p, &t);
}

static int picta(void)
{
    int k, n = 0;
    for (k = 0; k < 64 * 64; k++) if (t.pixela[k]) n++;
    return n;
}

int main(void)
{
    Color cin = color_ex_rgb(128, 128, 128);
    p = piscina_generare_dynamicum("sonda", 1 << 20);
    delineare_gradientum_radialem_dithered(novum(), 30, 30, 20, cin, cin);
    printf("radialis (30,30) r20 [control]: picta %d\n", picta());
    delineare_gradientum_radialem_dithered(novum(), 10, 10, 20, cin, cin);
    printf("radialis (10,10) r20 [trans marginem]: picta %d\n", picta());
    delineare_gradientum_radialem_dithered(novum(), 10, 40, 20, cin, cin);
    printf("radialis (10,40) r20 [solum x trans]: picta %d\n", picta());
    delineare_gradientum_radialem_dithered_cum_palette(novum(), 30, 30, 20, cin, cin, DITHERING_BAYER_4X4, NIHIL, 0);
    printf("cum_palette (30,30) r20 [control]: picta %d\n", picta());
    delineare_gradientum_radialem_dithered_cum_palette(novum(), 10, 10, 20, cin, cin, DITHERING_BAYER_4X4, NIHIL, 0);
    printf("cum_palette (10,10) r20: picta %d\n", picta());
    delineare_gradientum_radialem_dithered_cum_palette(novum(), 40, 10, 20, cin, cin, DITHERING_BAYER_4X4, NIHIL, 0);
    printf("cum_palette (40,10) r20 [solum y trans]: picta %d\n", picta());
    return 0;
}
