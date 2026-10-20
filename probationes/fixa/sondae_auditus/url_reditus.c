#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "url.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    unsigned long semen = 12345UL;
    int iter, mala = 0, n_len, k;
    i8 buf[64];
    chorda in, e, d;
    for (iter = 0; iter < 20000; iter++) {
        semen = semen * 1103515245UL + 12345UL;
        n_len = (int)((semen >> 16) % 40) + 1;
        for (k = 0; k < n_len; k++) {
            semen = semen * 1103515245UL + 12345UL;
            /* bias toward '%', '+', NUL */
            switch ((semen >> 16) % 8) {
            case 0: buf[k] = '%'; break;
            case 1: buf[k] = '+'; break;
            case 2: buf[k] = 0; break;
            default: buf[k] = (i8)((semen >> 20) & 0xFF);
            }
        }
        in.datum = buf; in.mensura = (i32)n_len;
        e = url_codificare(in, p);
        d = url_decodificare(e, p);
        if (d.mensura != in.mensura || memcmp(d.datum, in.datum, in.mensura) != 0) {
            mala++;
            if (mala < 5) printf("MALUM iter %d\n", iter);
        }
        if ((iter % 2000) == 0) piscina_vacare(p);
    }
    printf("reditus 20000 casus, mala=%d\n", mala);
    return 0;
}
