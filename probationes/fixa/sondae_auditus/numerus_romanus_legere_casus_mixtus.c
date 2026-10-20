/* Exhaustive over the 14-letter alphabet IVXLCDMivxlcdm, length 1..5:
 * every accepted string must be single-case; count lowercase
 * non-canonical accepts (lowercase(scribere(v)) != s). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "numerus_romanus.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    static const char alpha[14] = {'I','V','X','L','C','D','M','i','v','x','l','c','d','m'};
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    char buf[8], low[16];
    int idx[8];
    int len, k;
    unsigned long mixed_acc = 0, low_noncanon = 0, low_acc = 0, total = 0;
    for (len = 1; len <= 5; len++)
    {
        for (k = 0; k < len; k++) idx[k] = 0;
        for (;;)
        {
            chorda s; i32 val = 0; int up = 0, lo = 0;
            for (k = 0; k < len; k++) { buf[k] = alpha[idx[k]]; if (idx[k] < 7) up = 1; else lo = 1; }
            s.datum = (i8*)buf; s.mensura = (i32)len; total++;
            if (numerus_romanus_legere(s, &val))
            {
                if (up && lo) { mixed_acc++; if (mixed_acc < 10) printf("MIXED ACCEPT %.*s\n", len, buf); }
                else if (lo)
                {
                    chorda b = numerus_romanus_scribere(val, p);
                    low_acc++;
                    for (k = 0; k < (int)b.mensura; k++) low[k] = (char)tolower(b.datum[k]);
                    if ((int)b.mensura != len || memcmp(low, buf, (size_t)len) != 0)
                    { low_noncanon++; if (low_noncanon < 6) printf("LOWER NONCANON %.*s -> %u\n", len, buf, val); }
                    piscina_vacare(p);
                }
            }
            for (k = len - 1; k >= 0; k--) { if (++idx[k] < 14) break; idx[k] = 0; }
            if (k < 0) break;
        }
    }
    printf("total %lu mixed_accepted %lu lower_accepted %lu lower_noncanonical %lu\n", total, mixed_acc, low_acc, low_noncanon);
    piscina_destruere(p);
    return 0;
}
