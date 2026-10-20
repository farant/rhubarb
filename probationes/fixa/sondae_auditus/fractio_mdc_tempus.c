#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "magnus.h"
#include "fractio.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv)
{
    unsigned n = argc > 1 ? (unsigned)atoi(argv[1]) : 2000, k;
    Piscina* p = piscina_generare_dynamicum("sonda", 1 << 20);
    char* b = (char*)malloc(2 * n + 1);
    Fractio f = fractio_ex_s64(0), g;
    chorda c;
    clock_t t0;
    srand(5);
    for (k = 0; k < 2 * n + 1; k++) b[k] = (char)('0' + rand() % 10);
    b[0] = '1'; b[n] = '/'; b[n + 1] = '1'; b[2 * n] = '7';
    c.datum = (i8*)b; c.mensura = 2 * n + 1;
    t0 = clock();
    printf("ret=%d ", fractio_ex_chorda(c, p, &f));
    printf("parse %u-digit/%u-digit: %.2fs\n", n, n - 1, (double)(clock() - t0) / CLOCKS_PER_SEC);
    t0 = clock();
    g = fractio_adde(f, f, p);
    g = fractio_multiplica(g, f, p);
    printf("adde+multiplica: %.2fs sign=%d\n", (double)(clock() - t0) / CLOCKS_PER_SEC, fractio_signum(g));
    piscina_destruere(p);
    free(b);
    return 0;
}
