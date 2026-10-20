#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "url.h"
#include <stdio.h>

static void scribe(const char* tit, chorda r)
{
    i32 k;
    printf("%s|%s|%u|", tit, r.datum ? "ptr" : "NIHIL", (unsigned)r.mensura);
    for (k = 0; k < r.mensura; k++) putchar(r.datum[k]);
    putchar('\n');
}

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    i8 buf[256];
    i8 one[1];
    chorda in, r;
    int b;
    char t[32];
    for (b = 0; b < 256; b++) {
        one[0] = (i8)b;
        in.datum = one; in.mensura = 1;
        r = url_codificare(in, p);
        sprintf(t, "B%02X", b);
        scribe(t, r);
    }
    for (b = 0; b < 256; b++) buf[b] = (i8)b;
    in.datum = buf; in.mensura = 256;
    r = url_codificare(in, p);
    scribe("ALL", r);
    /* embedded NUL */
    in.datum = (i8*)"a\0b\0"; in.mensura = 4;
    r = url_codificare(in, p);
    scribe("NUL", r);
    /* empty with datum */
    in.datum = (i8*)"x"; in.mensura = 0;
    r = url_codificare(in, p);
    scribe("EMPTY_PTR", r);
    in.datum = NIHIL; in.mensura = 0;
    r = url_codificare(in, p);
    scribe("EMPTY_NIL", r);
    /* aliasing: all-unreserved */
    in.datum = (i8*)"abc"; in.mensura = 3;
    r = url_codificare(in, p);
    printf("ALIAS|%s\n", r.datum == in.datum ? "same" : "copy");
    /* NIHIL piscina */
    r = url_codificare(in, NIHIL);
    scribe("NOPISC", r);
    return 0;
}
