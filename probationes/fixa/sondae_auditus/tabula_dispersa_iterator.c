#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tabula_dispersa.h"
#include <stdio.h>
#include <string.h>

static char kk[400][8];
static chorda K(int idx) { chorda c; sprintf(kk[idx], "e%d", idx); c.datum=(i8*)kk[idx]; c.mensura=(i32)strlen(kk[idx]); return c; }

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    TabulaDispersa* t;
    TabulaIterator it;
    chorda c; void* v;
    int seen[400]; int idx, n, dup, miss, nz;

    /* empty, only tombstones, proximum after FALSUM */
    t = tabula_dispersa_creare_chorda(p, 16);
    it = tabula_dispersa_iterator_initium(t);
    printf("vacua: %d\n", tabula_dispersa_iterator_proximum(&it, &c, &v));
    for (idx = 0; idx < 5; idx++) tabula_dispersa_inserere(t, K(idx), (void*)1);
    for (idx = 0; idx < 5; idx++) tabula_dispersa_delere(t, K(idx));
    it = tabula_dispersa_iterator_initium(t);
    printf("tumuli soli: %d; iterum %d %d\n", tabula_dispersa_iterator_proximum(&it, &c, &v),
        tabula_dispersa_iterator_proximum(&it, &c, &v), tabula_dispersa_iterator_proximum(&it, NULL, NULL));

    /* delete current during iteration (filter) */
    t = tabula_dispersa_creare_chorda(p, 16);
    for (idx = 0; idx < 10; idx++) tabula_dispersa_inserere(t, K(idx), (void*)(long)idx);
    it = tabula_dispersa_iterator_initium(t); n = 0;
    while (tabula_dispersa_iterator_proximum(&it, &c, &v)) { n++; if ((long)v % 2 == 0) tabula_dispersa_delere(t, c); }
    printf("filtrum delere currentem: visi=%d (exsp 10) reliqui=%u (exsp 5)\n", n, t->numerus);

    /* insert during iteration triggering growth */
    t = tabula_dispersa_creare_chorda(p, 16);
    for (idx = 0; idx < 12; idx++) tabula_dispersa_inserere(t, K(idx), (void*)(long)idx);
    memset(seen, 0, sizeof seen);
    it = tabula_dispersa_iterator_initium(t); n = 0; idx = 100;
    while (tabula_dispersa_iterator_proximum(&it, &c, &v) && n < 1000) {
        n++;
        if ((long)v < 400) seen[(long)v]++;
        if (n == 3) { int j; for (j = 0; j < 4; j++, idx++) tabula_dispersa_inserere(t, K(idx), (void*)(long)idx); printf("  post insertiones in medio: cap=%u\n", t->capacitas); }
    }
    dup = 0; miss = 0;
    for (idx = 0; idx < 12; idx++) { if (seen[idx] > 1) dup++; if (seen[idx] == 0) miss++; }
    printf("inserere in iteratione (crescentia 16->32): visi=%d, originales bis visi=%d, originales numquam visi=%d\n", n, dup, miss);

    /* vacare during iteration */
    t = tabula_dispersa_creare_chorda(p, 16);
    for (idx = 0; idx < 6; idx++) tabula_dispersa_inserere(t, K(idx), (void*)(long)idx);
    it = tabula_dispersa_iterator_initium(t);
    tabula_dispersa_iterator_proximum(&it, &c, &v);
    tabula_dispersa_vacare(t);
    printf("vacare in iteratione: proximum -> %d\n", tabula_dispersa_iterator_proximum(&it, &c, &v));
    /* vacare reuse */
    printf("post vacare: numerus=%u del=%u coll=%u dmax=%u cap=%u continet e0=%d\n", t->numerus, t->numerus_deletorum, t->collisiones_totales, t->distantia_maxima, t->capacitas, tabula_dispersa_continet(t, K(0)));
    tabula_dispersa_inserere(t, K(1), (void*)5); v=0; tabula_dispersa_invenire(t, K(1), &v);
    printf("  reusus: numerus=%u v=%ld\n", t->numerus, (long)v);

    /* large growth: 70000 keys, iterate exactly once each */
    {
        static char big[70000][8]; chorda cc; int bad = 0; static unsigned char sn[70000];
        t = tabula_dispersa_creare_chorda(p, 1);
        for (idx = 0; idx < 70000; idx++) { sprintf(big[idx], "%d", idx); cc.datum=(i8*)big[idx]; cc.mensura=(i32)strlen(big[idx]); tabula_dispersa_inserere(t, cc, (void*)(long)(idx+1)); }
        for (idx = 0; idx < 70000; idx++) { cc.datum=(i8*)big[idx]; cc.mensura=(i32)strlen(big[idx]); v=0; if (!tabula_dispersa_invenire(t, cc, &v) || (long)v != idx+1) bad++; }
        it = tabula_dispersa_iterator_initium(t); n = 0; nz = 0;
        while (tabula_dispersa_iterator_proximum(&it, &cc, &v)) { n++; if (sn[(long)v-1]++) nz++; }
        printf("70000 claves: errores=%d numerus=%u cap=%u iter=%d duplicata=%d hash_ok=%d\n", bad, t->numerus, t->capacitas, n, nz,
            t->sloti[0].status != SLOT_OCCUPATUM || t->sloti[0].hash_cachatum == tabula_friare_fnv1a(t->sloti[0].clavis));
    }
    return 0;
}
