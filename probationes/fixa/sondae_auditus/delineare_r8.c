#include "latina.h"
#include "piscina.h"
#include "delineare.h"
#include <stdio.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 1 << 20);
    TabulaPixelorum a, b, d;
    int k, alg, diff_bayer, diff_atk;
    AlgorithusDithering algs[3];
    TabulaPixelorum* tt[3];
    algs[0] = DITHERING_FLOYD_STEINBERG; algs[1] = DITHERING_BAYER_4X4; algs[2] = DITHERING_ATKINSON;
    tt[0] = &a; tt[1] = &b; tt[2] = &d;
    for (alg = 0; alg < 3; alg++)
    {
        tt[alg]->latitudo = 64; tt[alg]->altitudo = 64;
        tt[alg]->pixela = (i32*)piscina_allocare(p, 64 * 64 * sizeof(i32));
        for (k = 0; k < 4096; k++) tt[alg]->pixela[k] = 0;
        delineare_gradientum_radialem_dithered_cum_palette(delineare_creare_contextum(p, tt[alg]),
            32, 32, 25, color_ex_rgb(0, 0, 0), color_ex_rgb(255, 255, 255), algs[alg], NIHIL, 0);
    }
    diff_bayer = diff_atk = 0;
    for (k = 0; k < 4096; k++) { diff_bayer += a.pixela[k] != b.pixela[k]; diff_atk += d.pixela[k] != b.pixela[k]; }
    printf("radialis cum_palette: FLOYD_STEINBERG vs BAYER_4X4 pixela diversa %d; ATKINSON vs BAYER_4X4 %d\n", diff_bayer, diff_atk);
    return 0;
}
