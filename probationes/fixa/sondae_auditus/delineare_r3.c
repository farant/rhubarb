#include "latina.h"
#include "piscina.h"
#include "delineare.h"
#include <stdio.h>

static TabulaPixelorum t;

static ContextusDelineandi* novum(Piscina* p)
{
    int k;
    t.latitudo = 64; t.altitudo = 8;
    t.pixela = (i32*)piscina_allocare(p, 64 * 8 * sizeof(i32));
    for (k = 0; k < 64 * 8; k++) t.pixela[k] = 0;
    return delineare_creare_contextum(p, &t);
}

static int albi(void)
{
    int k, n = 0;
    for (k = 0; k < 64 * 8; k++) if (color_ex_pixelum(t.pixela[k]).r == 255) n++;
    return n;
}

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    Color album = color_ex_rgb(255, 255, 255), nigrum = color_ex_rgb(0, 0, 0);
    ContextusDelineandi* c;

    c = novum(p);
    delineare_gradientum_linearem_dithered(c, 0, 0, 64, 8, nigrum, album, VERUM);
    printf("linearis nigrum->album: albi %d / 512 (expectatum ~256)\n", albi());
    c = novum(p);
    delineare_gradientum_linearem_dithered(c, 0, 0, 64, 8, album, nigrum, VERUM);
    printf("linearis album->nigrum: albi %d / 512 (expectatum ~256)\n", albi());
    c = novum(p);
    delineare_gradientum_linearem_dithered_cum_palette(c, 0, 0, 64, 8, album, nigrum, VERUM, DITHERING_BAYER_4X4, NIHIL, 0);
    printf("palette Bayer4 album->nigrum: albi %d / 512\n", albi());
    c = novum(p);
    delineare_gradientum_linearem_dithered_cum_palette(c, 0, 0, 64, 8, nigrum, album, VERUM, DITHERING_BAYER_4X4, NIHIL, 0);
    printf("palette Bayer4 nigrum->album: albi %d / 512\n", albi());
    c = novum(p);
    delineare_gradientum_linearem_dithered_cum_palette(c, 0, 0, 64, 8, album, nigrum, VERUM, DITHERING_FLOYD_STEINBERG, NIHIL, 0);
    printf("palette FS album->nigrum: albi %d / 512\n", albi());
    c = novum(p);
    delineare_gradientum_radialem_dithered(c, 32, 4, 3, album, nigrum);
    printf("radialis album->nigrum r3 (29 pixela in disco): albi %d\n", albi());
    c = novum(p);
    delineare_gradientum_radialem_dithered(c, 32, 4, 3, nigrum, album);
    printf("radialis nigrum->album r3: albi %d\n", albi());
    return 0;
}
