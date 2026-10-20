#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "via.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    unsigned long n = 100000UL, k, pos;
    char* s = (char*)malloc(n * 6 + 16);
    chorda c, r; clock_t t0; unsigned long u0;
    /* 1: 10^5 "a/" segments */
    pos = 0; for (k = 0; k < n; k++) { s[pos++] = 'a'; s[pos++] = '/'; }
    c.datum = (i8*)s; c.mensura = (i32)pos;
    u0 = (unsigned long)piscina_summa_usus(p); t0 = clock();
    r = via_normalizare(c, p);
    printf("10^5 'a/': out mensura=%u (expect %lu) ms=%ld arena_bytes=%lu input_bytes=%lu allocs=%lu\n",
        (unsigned)r.mensura, 2 * n - 1, (long)((clock() - t0) * 1000 / CLOCKS_PER_SEC),
        (unsigned long)piscina_summa_usus(p) - u0, pos, (unsigned long)piscina_numerus_allocationum(p));
    /* 2: 10^5 "a/" then 10^5 "../" -> "." */
    pos = 0; for (k = 0; k < n; k++) { s[pos++] = 'a'; s[pos++] = '/'; }
    for (k = 0; k < n; k++) { s[pos++] = '.'; s[pos++] = '.'; s[pos++] = '/'; }
    c.datum = (i8*)s; c.mensura = (i32)pos;
    r = via_normalizare(c, p);
    printf("10^5 a/ + 10^5 ../ -> '%.*s' (expect '.')\n", (int)r.mensura, (const char*)r.datum);
    /* 3: "/" + 10^5 "../" + "x" -> "/x" */
    pos = 0; s[pos++] = '/'; for (k = 0; k < n; k++) { s[pos++] = '.'; s[pos++] = '.'; s[pos++] = '/'; } s[pos++] = 'x';
    c.datum = (i8*)s; c.mensura = (i32)pos;
    r = via_normalizare(c, p);
    printf("/ + 10^5 ../ + x -> '%.*s' (expect '/x')\n", (int)r.mensura, (const char*)r.datum);
    /* 4: 3 ".." then x relative -> python '../../../x' */
    r = via_normalizare(chorda_ex_literis("../../../x", p), p);
    printf("'../../../x' -> '%.*s' (python '../../../x')\n", (int)r.mensura, (const char*)r.datum);
    free(s);
    piscina_destruere(p);
    return 0;
}
