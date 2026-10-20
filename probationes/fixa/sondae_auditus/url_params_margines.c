#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "url.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void ch(chorda r)
{
    i32 k;
    putchar('[');
    if (!r.datum) printf("N:");
    for (k = 0; k < r.mensura; k++) {
        if (r.datum[k] < 0x20 || r.datum[k] >= 0x7F) printf("\\x%02X", r.datum[k]);
        else putchar(r.datum[k]);
    }
    putchar(']');
}

static const char* qs[] = {
    "", "a", "a=", "=v", "=", "a=b=c", "a&b", "&&", "&a=1", "a=1&", "&",
    "a=%3D&b=%26", "q=a+b", "a=%", "a=%G1", "%=x", "a=1&a=2", "a=1;b=2",
    "?a=1", "a=1#frag", "k=%00v", NULL
};

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    chorda in;
    UrlParams r;
    int idx;
    i32 k;
    for (idx = 0; qs[idx]; idx++) {
        size_t n = strlen(qs[idx]);
        i8* m = (i8*)malloc(n ? n : 1);
        memcpy(m, qs[idx], n);
        in.datum = m; in.mensura = (i32)n;
        r = url_params_parse(in, p);
        printf("Q \"%s\" -> numerus %u :", qs[idx], (unsigned)r.numerus);
        for (k = 0; k < r.numerus; k++) { putchar(' '); ch(r.claves[k]); putchar('='); ch(r.valores[k]); }
        putchar('\n');
        /* aliasing: overwrite input then check result still same */
        memset(m, 'Z', n);
        if (r.numerus > 0 && r.claves[0].mensura > 0 && r.claves[0].datum[0] == 'Z')
            printf("  ALIAS: clavis[0] mutata cum input\n");
        free(m);
    }
    /* embedded NUL in query */
    in.datum = (i8*)"a\0=b&c=\0"; in.mensura = 8;
    r = url_params_parse(in, p);
    printf("Q NUL -> numerus %u :", (unsigned)r.numerus);
    for (k = 0; k < r.numerus; k++) { putchar(' '); ch(r.claves[k]); putchar('='); ch(r.valores[k]); }
    putchar('\n');
    return 0;
}
