/* Exhaustive: every string over IVXLCDM of length 1..8. A string is
 * canonical iff scribere(legere(s)) == s. Report accepted non-canonical
 * strings and accepted values > 3999. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "numerus_romanus.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    static const char alpha[7] = {'I','V','X','L','C','D','M'};
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    char buf[16];
    int idx[16];
    int len, k;
    unsigned long accepted = 0, noncanon = 0, over = 0, shown = 0, total = 0;
    unsigned int canon_seen = 0;
    for (len = 1; len <= 8; len++)
    {
        for (k = 0; k < len; k++) idx[k] = 0;
        for (;;)
        {
            chorda s; i32 val = 0;
            for (k = 0; k < len; k++) buf[k] = alpha[idx[k]];
            s.datum = (i8*)buf; s.mensura = (i32)len;
            total++;
            if (numerus_romanus_legere(s, &val))
            {
                chorda back = numerus_romanus_scribere(val, p);
                accepted++;
                if (back.mensura != (i32)len || memcmp(back.datum, buf, (size_t)len) != 0)
                {
                    noncanon++;
                    if (val > 3999) over++;
                    if (shown < 60 || val > 3999)
                    {
                        if (shown < 120)
                            printf("NONCANON %.*s -> %u (canonical '%.*s')\n", len, buf, val,
                                   (int)back.mensura, back.datum ? (char*)back.datum : "");
                        shown++;
                    }
                }
                else canon_seen++;
                piscina_vacare(p);
            }
            /* increment */
            for (k = len - 1; k >= 0; k--)
            {
                if (++idx[k] < 7) break;
                idx[k] = 0;
            }
            if (k < 0) break;
        }
    }
    printf("total %lu accepted %lu canonical %u noncanonical %lu over3999 %lu\n",
           total, accepted, canon_seen, noncanon, over);
    piscina_destruere(p);
    return 0;
}
