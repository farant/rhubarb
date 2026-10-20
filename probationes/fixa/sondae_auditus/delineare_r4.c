#include "latina.h"
#include "piscina.h"
#include "delineare.h"
#include <stdio.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    TabulaPixelorum t;
    ContextusDelineandi* c;
    int k, n;
    t.latitudo = 40; t.altitudo = 40;
    t.pixela = (i32*)piscina_allocare(p, 40 * 40 * sizeof(i32));
    for (k = 0; k < 1600; k++) t.pixela[k] = 0;
    c = delineare_creare_contextum(p, &t);
    delineare_ponere_praecisionem(c, -5, -5, 10, 10);
    printf("praecisio post (-5,-5,10,10): x=%u y=%u lat=%u alt=%u (expectatum 0 0 5 5)\n",
        (unsigned)c->praecisio_x, (unsigned)c->praecisio_y,
        (unsigned)c->praecisio_latitudo, (unsigned)c->praecisio_altitudo);
    delineare_rectangulum_plenum(c, 0, 0, 40, 40, color_ex_rgb(255, 255, 255));
    n = 0;
    for (k = 0; k < 1600; k++) if (t.pixela[k]) n++;
    printf("rectangulum plenum totum: pixela picta %d (expectatum 25)\n", n);
    for (k = 0; k < 1600; k++) t.pixela[k] = 0;
    delineare_ponere_modum(c, MODUS_XOR);
    delineare_rectangulum_plenum(c, 0, 0, 40, 40, color_ex_rgb(255, 255, 255));
    n = 0;
    for (k = 0; k < 1600; k++) if (t.pixela[k]) n++;
    printf("via pixelorum (XOR): pixela picta %d (expectatum 25)\n", n);
    return 0;
}
