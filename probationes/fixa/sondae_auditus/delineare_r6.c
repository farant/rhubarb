#include "latina.h"
#include "piscina.h"
#include "delineare.h"
#include <stdio.h>

#define ALT 40

static int cohib(int v) { return v < 0 ? 0 : (v > 255 ? 255 : v); }

/* referentia Atkinson pro columna una: cum_duobus = diffundit ad [0,2] */
static void referentia(int g, int cum_duobus, int* exitus)
{
    int err_row[ALT + 2];
    int r, k;
    for (k = 0; k < ALT + 2; k++) err_row[k] = 0;
    for (r = 0; r < ALT; r++)
    {
        int act = cohib(g + err_row[r]);
        int ch = act >= 128 ? 255 : 0;
        int e = act - ch;
        exitus[r] = ch;
        if (r + 1 < ALT) err_row[r + 1] += e / 8;
        if (cum_duobus && r + 2 < ALT) err_row[r + 2] += e / 8;
    }
}

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    TabulaPixelorum t;
    ContextusDelineandi* c;
    int sine[ALT], cum[ALT], bib[ALT];
    int k, g, diff_sine, diff_cum, alb_b, alb_s, alb_c;
    for (g = 60; g <= 200; g += 35)
    {
        Color col = color_ex_rgb((i8)g, (i8)g, (i8)g);
        t.latitudo = 1; t.altitudo = ALT;
        t.pixela = (i32*)piscina_allocare(p, ALT * sizeof(i32));
        for (k = 0; k < ALT; k++) t.pixela[k] = 0;
        c = delineare_creare_contextum(p, &t);
        delineare_gradientum_linearem_dithered_cum_palette(c, 0, 0, 1, ALT, col, col, FALSUM, DITHERING_ATKINSON, NIHIL, 0);
        referentia(g, 0, sine);
        referentia(g, 1, cum);
        diff_sine = diff_cum = alb_b = alb_s = alb_c = 0;
        for (k = 0; k < ALT; k++)
        {
            bib[k] = color_ex_pixelum(t.pixela[k]).r;
            if (bib[k] != sine[k]) diff_sine++;
            if (bib[k] != cum[k]) diff_cum++;
            alb_b += bib[k] == 255; alb_s += sine[k] == 255; alb_c += cum[k] == 255;
        }
        printf("g=%3d: bibliotheca albi %2d | ref sine [0,2] albi %2d (diff %d) | ref cum [0,2] albi %2d (diff %d)\n",
            g, alb_b, alb_s, diff_sine, alb_c, diff_cum);
    }
    return 0;
}
