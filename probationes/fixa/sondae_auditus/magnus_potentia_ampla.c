/* sonda: magnus_potentia (bases limitum, exponentes ad 70, 10^k ad
 * 10^10000, exponentes ingentes pro 0, 1, -1) et itus reditusque
 * decimalis magnorum. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "magnus.h"
#include <stdio.h>
#include <time.h>

static void pr(const char* tag, long x, long e, Magnus m, Piscina* p)
{
    chorda c = magnus_ad_chordam(m, p);
    Magnus back;
    int rt = magnus_ex_chorda(c, p, &back) && magnus_aequalis(back, m);
    printf("%s %ld %ld %.*s rt=%d\n", tag, x, e, (int)c.mensura,
        (const char*)c.datum, rt);
}

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 1 << 20);
    long bases[] = { -3, -2, -1, 0, 1, 2, 3, 12, 4294967295L, -2147483648L,
                     2147483648L, 2147483647L, -2147483647L, 1000000000L };
    int nb = (int)(sizeof bases / sizeof bases[0]);
    int bi;
    long e;
    clock_t t0;

    for (bi = 0; bi < nb; bi++)
        for (e = 0; e <= 70; e++)
            pr("pot", bases[bi], e, magnus_potentia(magnus_ex_s64(bases[bi]),
                (i32)e, p), p);
    {
        Magnus smin = magnus_ex_s64((s64)(-0x7FFFFFFFFFFFFFFFLL - 1));
        for (e = 0; e <= 5; e++)
            pr("potmin", -1, e, magnus_potentia(smin, (i32)e, p), p);
    }
    {
        long ks[] = { 9, 18, 19, 27, 100, 1000, 10000 };
        int k;
        for (k = 0; k < 7; k++)
            pr("pow10", 10, ks[k], magnus_potentia(magnus_ex_s64(10),
                (i32)ks[k], p), p);
    }
    t0 = clock();
    pr("huge", 0, 4294967295L, magnus_potentia(magnus_ex_s64(0), 0xFFFFFFFFU, p), p);
    pr("huge", 1, 4294967295L, magnus_potentia(magnus_ex_s64(1), 0xFFFFFFFFU, p), p);
    pr("huge", -1, 4294967295L, magnus_potentia(magnus_ex_s64(-1), 0xFFFFFFFFU, p), p);
    pr("huge", -1, 4294967294L, magnus_potentia(magnus_ex_s64(-1), 0xFFFFFFFEU, p), p);
    printf("huge_secs %.3f\n", (double)(clock() - t0) / CLOCKS_PER_SEC);
    /* exponens 'negativus' -1 pro basi 2: 2^(2^32-1) - hic NON currimus */
    piscina_destruere(p);
    return 0;
}
