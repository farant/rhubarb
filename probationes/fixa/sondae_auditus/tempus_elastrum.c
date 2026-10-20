#include "latina.h"
#include "tempus.h"
#include <stdio.h>
static void currere(f32 k, f32 d, f32 dt, int n)
{
    f32 x = 0.0f, v = 0.0f; int i; f32 mx = 0;
    for (i = 0; i < n; i++) { x = elastrum_ad(x, 1.0f, &v, k, d, dt); if (x > mx) mx = x; if (-x > mx) mx = -x; }
    printf("k=%g d=%g dt=%g n=%d -> x=%.6g v=%.6g max|x|=%.6g\n", (double)k, (double)d, (double)dt, n, (double)x, (double)v, (double)mx);
}
int main(void)
{
    currere(100, 10, 1.0f/60.0f, 600);   /* normal 60fps */
    currere(100, 10, 0.25f, 40);          /* 4 fps hitch-y frame rate */
    currere(100, 10, 0.5f, 40);
    currere(10, 30, 0.1f, 40);            /* heavy damping: d*dt=3 -> factor -2 */
    currere(100, 0, 1.0f/60.0f, 6000);    /* undamped */
    currere(100, -1, 1.0f/60.0f, 600);    /* negative damping */
    currere(100, 10, 0.0f, 10);           /* dt 0 */
    printf("velocitas NIHIL -> %g\n", (double)elastrum_ad(3.0f, 1.0f, NIHIL, 100, 10, 0.1f));
    return 0;
}
