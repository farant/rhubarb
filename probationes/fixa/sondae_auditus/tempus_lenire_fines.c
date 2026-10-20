#include "latina.h"
#include "tempus.h"
#include <stdio.h>
#include <math.h>

typedef f32 (*FunLen)(f32);
typedef struct { const char* tit; FunLen in; FunLen out; FunLen io; } Familia;

static void
fines(const char* tit, FunLen f)
{
    f32 a = f(0.0f), b = f(1.0f);
    printf("%-38s f(0)=%.9g f(1)=%.9g %s\n", tit, (double)a, (double)b,
        (a == 0.0f && b == 1.0f) ? "OK" : "NON_EXACTUS");
}

int
main(void)
{
    Familia fam[7];
    int k, j;
    fam[0].tit="quadraticus"; fam[0].in=lenire_quadraticus_ingressus; fam[0].out=lenire_quadraticus_egressus; fam[0].io=lenire_quadraticus_ingressus_egressus;
    fam[1].tit="cubicus"; fam[1].in=lenire_cubicus_ingressus; fam[1].out=lenire_cubicus_egressus; fam[1].io=lenire_cubicus_ingressus_egressus;
    fam[2].tit="sinus"; fam[2].in=lenire_sinus_ingressus; fam[2].out=lenire_sinus_egressus; fam[2].io=lenire_sinus_ingressus_egressus;
    fam[3].tit="exponens"; fam[3].in=lenire_exponens_ingressus; fam[3].out=lenire_exponens_egressus; fam[3].io=lenire_exponens_ingressus_egressus;
    fam[4].tit="retro"; fam[4].in=lenire_retro_ingressus; fam[4].out=lenire_retro_egressus; fam[4].io=lenire_retro_ingressus_egressus;
    fam[5].tit="elasticus"; fam[5].in=lenire_elasticus_ingressus; fam[5].out=lenire_elasticus_egressus; fam[5].io=lenire_elasticus_ingressus_egressus;
    fam[6].tit="resiliens"; fam[6].in=lenire_resiliens_ingressus; fam[6].out=lenire_resiliens_egressus; fam[6].io=lenire_resiliens_ingressus_egressus;

    fines("linearis", lenire_linearis);
    for (k = 0; k < 7; k++) {
        char b1[64];
        sprintf(b1, "%s_ingressus", fam[k].tit); fines(b1, fam[k].in);
        sprintf(b1, "%s_egressus", fam[k].tit); fines(b1, fam[k].out);
        sprintf(b1, "%s_ingressus_egressus", fam[k].tit); fines(b1, fam[k].io);
    }
    /* continuity at 0.5 and symmetry */
    for (k = 0; k < 7; k++) {
        f32 lo = fam[k].io(0.49999997f), mid = fam[k].io(0.5f);
        double maxsym = 0.0, maxio = 0.0;
        for (j = 0; j <= 1000; j++) {
            f32 t = (f32)j / 1000.0f;
            double d1 = fabs((double)fam[k].out(t) - (1.0 - (double)fam[k].in(1.0f - t)));
            double d2 = fabs((double)fam[k].io(t) + (double)fam[k].io(1.0f - t) - 1.0);
            if (d1 > maxsym) maxsym = d1;
            if (d2 > maxio) maxio = d2;
        }
        printf("%-12s io(0.5-)=%.9g io(0.5)=%.9g  max|out(t)-(1-in(1-t))|=%.3g  max|io(t)+io(1-t)-1|=%.3g\n",
            fam[k].tit, (double)lo, (double)mid, maxsym, maxio);
    }
    /* out of domain / NaN */
    {
        f32 ts[6]; int n;
        ts[0] = -0.5f; ts[1] = 1.5f; ts[2] = 2.0f; ts[3] = 100.0f; ts[4] = (f32)NAN; ts[5] = (f32)INFINITY;
        for (n = 0; n < 6; n++) {
            printf("t=%g: res_eg=%.6g res_in=%.6g res_io=%.6g ela_eg=%.6g exp_in=%.6g retro_in=%.6g sin_in=%.6g\n",
                (double)ts[n], (double)lenire_resiliens_egressus(ts[n]), (double)lenire_resiliens_ingressus(ts[n]),
                (double)lenire_resiliens_ingressus_egressus(ts[n]), (double)lenire_elasticus_egressus(ts[n]),
                (double)lenire_exponens_ingressus(ts[n]), (double)lenire_retro_ingressus(ts[n]),
                (double)lenire_sinus_ingressus(ts[n]));
        }
    }
    /* overshoot ranges */
    {
        double mn[3] = {1e9,1e9,1e9}, mx[3] = {-1e9,-1e9,-1e9};
        for (j = 0; j <= 10000; j++) {
            f32 t = (f32)j / 10000.0f; f32 v[3];
            v[0] = lenire_retro_ingressus_egressus(t); v[1] = lenire_elasticus_egressus(t); v[2] = lenire_resiliens_egressus(t);
            for (k = 0; k < 3; k++) { if (v[k] < mn[k]) mn[k] = v[k]; if (v[k] > mx[k]) mx[k] = v[k]; }
        }
        printf("range retro_io [%g,%g] elastic_eg [%g,%g] resiliens_eg [%g,%g]\n", mn[0],mx[0],mn[1],mx[1],mn[2],mx[2]);
    }
    return 0;
}
