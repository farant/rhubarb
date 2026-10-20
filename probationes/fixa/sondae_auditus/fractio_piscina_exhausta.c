#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "magnus.h"
#include "fractio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    int k = argc > 1 ? atoi(argv[1]) : 0;
    Piscina* p = piscina_generare_certae_magnitudinis("certa", 512);
    Fractio f = fractio_ex_s64(4242), h = fractio_ex_s64(3);
    static char buf[3001];
    chorda c;
    memset(buf, '7', 3000); buf[1500] = '/';
    c.datum = (i8*)buf; c.mensura = 3000;
    printf("casus %d\n", k); fflush(stdout);
    if (k == 1) { b32 r = fractio_ex_chorda(c, p, &f); printf("ex_chorda ret=%d untouched=%d\n", r, fractio_aequalis(f, fractio_ex_s64(4242))); }
    if (k == 2) { b32 r = fractio_potentia(h, 100000, p, &f); printf("potentia ret=%d\n", r); }
    if (k == 3) { Fractio a = fractio_ex_s64((s64)0x7FFFFFFFFFFFFFFFLL); int j; for (j = 0; j < 200; j++) a = fractio_multiplica(a, a, p); printf("done\n"); }
    piscina_destruere(p);
    return 0;
}
