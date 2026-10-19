#include "latina.h"
#include "tempus.h"
#include <stdio.h>
#include <math.h>
int main(void)
{
    f32 ts[7] = {0.25f, 0.75f, -0.25f, -0.75f, -0.1f, 0.0f, 0.5f};
    int i;
    for (i = 0; i < 7; i++)
        printf("t=%g f=1 A=1: tri=%.6g saw=%.6g sq=%.6g sin=%.6g\n", (double)ts[i],
            (double)unda_triangulus(ts[i], 1.0f, 1.0f), (double)unda_dens_serrae(ts[i], 1.0f, 1.0f),
            (double)unda_quadratus(ts[i], 1.0f, 1.0f), (double)unda_sinus(ts[i], 1.0f, 1.0f));
    printf("neg freq t=0.25 f=-1: tri=%.6g saw=%.6g sq=%.6g\n", (double)unda_triangulus(0.25f, -1.0f, 1.0f),
        (double)unda_dens_serrae(0.25f, -1.0f, 1.0f), (double)unda_quadratus(0.25f, -1.0f, 1.0f));
    printf("freq 0: tri=%.6g saw=%.6g sq=%.6g sin=%.6g\n", (double)unda_triangulus(5.0f, 0.0f, 1.0f),
        (double)unda_dens_serrae(5.0f, 0.0f, 1.0f), (double)unda_quadratus(5.0f, 0.0f, 1.0f), (double)unda_sinus(5.0f, 0.0f, 1.0f));
    /* tumultus */
    {
        f32 mn = 9, mx = -9; int det = 1, j;
        for (j = -200000; j < 200000; j++) {
            f32 t = (f32)j * 0.0013f;
            f32 v = unda_tumultus(t);
            if (v != unda_tumultus(t)) det = 0;
            if (v < mn) mn = v; if (v > mx) mx = v;
        }
        printf("tumultus det=%d range [%.6g, %.6g]\n", det, (double)mn, (double)mx);
        printf("tumultus nan=%.6g inf=%.6g -inf=%.6g 1e30=%.6g 0=%.6g\n", (double)unda_tumultus((f32)NAN),
            (double)unda_tumultus((f32)INFINITY), (double)unda_tumultus(-(f32)INFINITY), (double)unda_tumultus(1e30f), (double)unda_tumultus(0.0f));
    }
    /* oscillare */
    printf("oscillare(0.5,1)=%g (1.5,1)=%g (-0.5,1)=%g (-1.5,1)=%g (2,1)=%g (1,0)=%g (0.5,-1)=%g\n",
        (double)oscillare(0.5f,1.0f),(double)oscillare(1.5f,1.0f),(double)oscillare(-0.5f,1.0f),(double)oscillare(-1.5f,1.0f),
        (double)oscillare(2.0f,1.0f),(double)oscillare(1.0f,0.0f),(double)oscillare(0.5f,-1.0f));
    /* remappare */
    printf("remappare(5,1,1,0,10)=%g (1,1,1,0,10)=%g (15,0,10,0,1)=%g (0,10,0,0,1)=%g (10,0,10,0,1)=%g\n",
        (double)remappare(5,1,1,0,10),(double)remappare(1,1,1,0,10),(double)remappare(15,0,10,0,1),
        (double)remappare(0,10,0,0,1),(double)remappare(10,0,10,0,1));
    /* gradus_lenis */
    printf("gradus_lenis(1,1,1)=%g (1,1,0.5)=%g (1,1,2)=%g (0,1,-1)=%g (0,1,2)=%g (1,0,0.25)=%g (0,1,nan)=%g\n",
        (double)gradus_lenis(1,1,1),(double)gradus_lenis(1,1,0.5f),(double)gradus_lenis(1,1,2),
        (double)gradus_lenis(0,1,-1),(double)gradus_lenis(0,1,2),(double)gradus_lenis(1,0,0.25f),(double)gradus_lenis(0,1,(f32)NAN));
    /* cohibere */
    printf("cohibere(0.5,1,0)=%g (nan,0,1)=%g (5,nan,1)=%g (-0.0,0,1)=%g (2,0,1)=%g\n",
        (double)cohibere(0.5f,1,0),(double)cohibere((f32)NAN,0,1),(double)cohibere(5,(f32)NAN,1),(double)cohibere(-0.0f,0,1),(double)cohibere(2,0,1));
    /* interpolare t=1 */
    {
        f32 A[4] = {1e8f, 3.0f, -1e7f, 0.1f}; f32 B[4] = {1.0f, 0.1f, 0.3f, 0.7f}; int k;
        for (k = 0; k < 4; k++) {
            f32 r1 = interpolare(A[k], B[k], 1.0f), r0 = interpolare(A[k], B[k], 0.0f);
            printf("interpolare(%.9g,%.9g,1)=%.9g %s ; t=0 -> %.9g %s\n", (double)A[k], (double)B[k], (double)r1,
                r1 == B[k] ? "==b" : "!=b", (double)r0, r0 == A[k] ? "==a" : "!=a");
        }
    }
    /* animare_ad */
    printf("animare_ad(0,10,-1,0.1)=%g (0,10,1,-0.1)=%g (0,10,1000,1)=%g (5,5,1,0.1)=%g (0,10,nan,1)=%g\n",
        (double)animare_ad(0,10,-1,0.1f),(double)animare_ad(0,10,1,-0.1f),(double)animare_ad(0,10,1000,1),
        (double)animare_ad(5,5,1,0.1f),(double)animare_ad(0,10,(f32)NAN,1));
    {
        f32 x = 0; int k;
        for (k = 0; k < 5; k++) x = animare_ad(x, 10, -2.0f, 1.0f);
        printf("animare_ad negative celeritas 5 steps from 0 toward 10: %g\n", (double)x);
    }
    return 0;
}
