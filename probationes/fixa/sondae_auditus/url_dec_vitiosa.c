#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "url.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void hexa(const char* tit, chorda r)
{
    i32 k;
    printf("%s|%s|%u|", tit, r.datum ? "ptr" : "NIHIL", (unsigned)r.mensura);
    for (k = 0; k < r.mensura; k++) printf("%02X", r.datum[k]);
    putchar('\n');
}

static const char* casus_v[] = {
    "%", "%4", "abc%", "abc%4", "%zz", "%G0", "%-1", "% 1", "%%", "%%41",
    "%2f", "%2F", "%c3%a9", "a+b", "%2B", "%00", "%FF", "%C0%80", "%2541",
    "a", "+", "++", "%4g", "%g4", "%%%", "%0", "100%", "x%2", NULL
};

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    chorda in, r;
    int idx;
    i8 hb[3];
    for (idx = 0; casus_v[idx]; idx++) {
        /* copy into exact-size malloc so ASan sees overread */
        size_t n = strlen(casus_v[idx]);
        i8* m = (i8*)malloc(n ? n : 1);
        memcpy(m, casus_v[idx], n);
        in.datum = m; in.mensura = (i32)n;
        r = url_decodificare(in, p);
        hexa(casus_v[idx], r);
        free(m);
    }
    hb[0] = 0xE9; hb[1] = 0xFF; hb[2] = 0x80;
    in.datum = hb; in.mensura = 3;
    r = url_decodificare(in, p);
    hexa("RAWHIGH", r);
    in.datum = (i8*)"x"; in.mensura = 0;
    r = url_decodificare(in, p);
    hexa("EMPTY", r);
    return 0;
}
