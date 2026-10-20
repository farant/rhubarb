/* sonda: parse time vs number of keys in one object (duplicate scan is linear per key) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(void)
{
    Piscina* pisc = piscina_generare_dynamicum("sonda", 1 << 20);
    int ns[4] = { 10000, 20000, 40000, 80000 }; int q;
    for (q = 0; q < 4; q++) {
        int n = ns[q], i; size_t o = 0; char* doc = (char*)malloc((size_t)n * 24 + 16);
        clock_t t0; double s; JsonResultus r; chorda in;
        doc[o++] = '{';
        for (i = 0; i < n; i++) o += (size_t)sprintf(doc + o, "%s\"q%d\":%d", i ? "," : "", q * 1000000 + i, i);
        doc[o++] = '}';
        in.datum = (i8*)doc; in.mensura = (i32)o;
        t0 = clock();
        r = json_legere(in, pisc);
        s = (double)(clock() - t0) / CLOCKS_PER_SEC;
        printf("keys=%6d bytes=%7lu succ=%d parse=%.3fs (%.1f us/key)\n", n, (unsigned long)o, (int)r.successus, s, s * 1e6 / n);
        free(doc);
        piscina_vacare(pisc);
    }
    return 0;
}
