#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tabula_dispersa.h"
#include <stdio.h>
#include <string.h>

static char kk[64][8];
static chorda K(int idx) { chorda c; sprintf(kk[idx], "r%02d", idx); c.datum=(i8*)kk[idx]; c.mensura=(i32)strlen(kk[idx]); return c; }

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    TabulaDispersa* t = tabula_dispersa_creare_chorda(p, 16);
    TabulaIterator it; chorda c; void* v;
    int seen[64]; int idx, n, dup, miss;
    i32 cap_ante;
    for (idx = 0; idx < 25; idx++) tabula_dispersa_inserere(t, K(idx), (void*)(long)idx);
    cap_ante = t->capacitas;
    printf("ante: cap=%u numerus=%u\n", t->capacitas, t->numerus);
    /* update ONLY the value of an existing key */
    tabula_dispersa_inserere(t, K(0), (void*)(long)0);
    printf("post renovationem unius clavis: cap=%u (ante %u)\n", t->capacitas, cap_ante);

    /* fresh table, update-all-values-during-iteration */
    t = tabula_dispersa_creare_chorda(p, 16);
    for (idx = 0; idx < 25; idx++) tabula_dispersa_inserere(t, K(idx), (void*)(long)idx);
    memset(seen, 0, sizeof seen);
    it = tabula_dispersa_iterator_initium(t); n = 0;
    while (tabula_dispersa_iterator_proximum(&it, &c, &v) && n < 1000) {
        long val = (long)v;
        n++;
        if (val >= 0 && val < 64) seen[val]++;
        tabula_dispersa_inserere(t, c, (void*)val); /* same value: pure update */
    }
    dup = 0; miss = 0;
    for (idx = 0; idx < 25; idx++) { if (seen[idx] > 1) dup++; if (seen[idx] == 0) miss++; }
    printf("renovare omnes in iteratione: visi=%d (exsp 25) bis=%d numquam=%d cap=%u\n", n, dup, miss, t->capacitas);
    return 0;
}
