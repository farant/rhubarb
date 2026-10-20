#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tabula_dispersa.h"
#include <stdio.h>
#include <string.h>

static char kk[256][12];

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    TabulaDispersa* t; TabulaIterator it; chorda c; void* v;
    int seen[256]; int idx, n, dup, miss, total, casus_mali = 0, pref;
    for (pref = 0; pref < 4; pref++)
    for (total = 13; total < 200; total++) {
        t = tabula_dispersa_creare_chorda(p, 16);
        for (idx = 0; idx < total; idx++) {
            sprintf(kk[idx], "%c%d", "abcd"[pref], idx); c.datum=(i8*)kk[idx]; c.mensura=(i32)strlen(kk[idx]);
            tabula_dispersa_inserere(t, c, (void*)(long)idx);
        }
        memset(seen, 0, sizeof seen);
        it = tabula_dispersa_iterator_initium(t); n = 0;
        while (tabula_dispersa_iterator_proximum(&it, &c, &v) && n < 2000) {
            n++; seen[(long)v]++;
            tabula_dispersa_inserere(t, c, v);
        }
        dup = 0; miss = 0;
        for (idx = 0; idx < total; idx++) { if (seen[idx] > 1) dup++; if (seen[idx] == 0) miss++; }
        if (dup || miss) { casus_mali++; if (casus_mali <= 6) printf("praefixum %c total=%d: visi=%d bis=%d numquam=%d cap=%u\n", "abcd"[pref], total, n, dup, miss, t->capacitas); }
    }
    printf("casus mali: %d\n", casus_mali);
    return 0;
}
