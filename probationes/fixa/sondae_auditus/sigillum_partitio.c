#include "latina.h"
#include "sigillum.h"
#include <stdio.h>
#include <stdlib.h>
/* split invariance: many partitions incl zero-length NIHIL adds; check invariants between calls */
static unsigned char buf[1000];
static int bad_inv = 0;
static void inv(SigillumContextus* c, unsigned long tot)
{
    if (c->massa_mensura > 63 || c->totalis != (i64)tot) { bad_inv++; printf("INV massa=%u tot=%llu exp=%lu\n", (unsigned)c->massa_mensura, (unsigned long long)c->totalis, tot); }
}
int main(void)
{
    int idx, n, trial, fails = 0, cases = 0;
    unsigned rs = 12345;
    for (idx = 0; idx < 1000; idx++) buf[idx] = (unsigned char)((idx * 131 + 7) ^ (idx >> 3));
    for (n = 0; n <= 600; n += (n < 200 ? 1 : 17)) {
        Sigillum ref = sigillum_computare(buf, (memoriae_index)n);
        for (trial = 0; trial < 12; trial++) {
            SigillumContextus c; Sigillum s; int pos = 0;
            sigillum_incipere(&c);
            sigillum_addere(&c, NIHIL, 0); inv(&c, 0);
            while (pos < n) {
                int chunk;
                rs = rs * 1103515245u + 12345u;
                switch (trial) {
                case 0: chunk = 1; break;
                case 1: chunk = 63; break;
                case 2: chunk = 64; break;
                case 3: chunk = 65; break;
                case 4: chunk = (pos == 0) ? 7 : 64; break;   /* partial then exact fill */
                case 5: chunk = (pos == 0) ? 5 : 200; break;  /* partial then multi-block */
                case 6: chunk = (pos == 0) ? 57 : 7; break;   /* 57 then crosses */
                default: chunk = (int)((rs >> 16) % 150); break;
                }
                if (chunk > n - pos) chunk = n - pos;
                sigillum_addere(&c, buf + pos, (memoriae_index)chunk);
                pos += chunk;
                inv(&c, (unsigned long)pos);
                if (trial >= 7) { sigillum_addere(&c, NIHIL, 0); inv(&c, (unsigned long)pos); }
            }
            s = sigillum_finire(&c);
            cases++;
            if (!sigillum_aequale(&s, &ref)) { fails++; printf("FAIL n=%d trial=%d\n", n, trial); }
        }
    }
    printf("partitio: casus %d defectus %d invariantia_violata %d\n", cases, fails, bad_inv);
    return 0;
}
