#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    Piscina* pp = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* it = internamentum_creare(pp);
    const char* docs[] = { "<a\0>x</a>", "<!D><a/>b><c/>", "<a\001>x</a>", "<a\xc3\xa9>x</a>", "<\xc3\xa9>x</\xc3\xa9>" };
    i32 lens[] = { 9, 14, 9, 10, 11 };
    int k;
    for (k = 0; k < 5; k++)
    {
        chorda in, w; StmlResultus r; i32 i, n;
        in.datum = (i8*)(size_t)docs[k]; in.mensura = lens[k];
        r = stml_legere(in, pp, it);
        w = stml_scribere(r.radix, pp, FALSUM);
        n = stml_numerus_liberorum(r.radix);
        printf("CASUS%d\tstatus=%d\tliberi_doc=%u\tscriptum_hex=", k, (int)r.status, n);
        for (i = 0; i < w.mensura; i++) printf("%02x", (unsigned)w.datum[i]);
        printf("\tscriptum=[");
        for (i = 0; i < w.mensura; i++) putchar(w.datum[i] >= 32 && w.datum[i] < 127 ? w.datum[i] : '?');
        printf("]\n");
    }
    return 0;
}
