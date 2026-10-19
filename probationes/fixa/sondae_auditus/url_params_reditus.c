#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "url.h"
#include <stdio.h>
#include <string.h>

static void ch(const char* tit, chorda r)
{
    i32 k;
    printf("%s|%s|%u|", tit, r.datum ? "ptr" : "NIHIL", (unsigned)r.mensura);
    for (k = 0; k < r.mensura; k++) putchar(r.datum[k]);
    putchar('\n');
}

static chorda fac_ch(const char* s, i32 n) { chorda c; c.datum = (i8*)(size_t)s; c.mensura = n; return c; }

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    chorda cl[6], va[6], q;
    UrlParams r;
    i32 k;
    int mala = 0;

    cl[0] = fac_ch("a=b", 3);   va[0] = fac_ch("c&d", 3);
    cl[1] = fac_ch("", 0);      va[1] = fac_ch("x", 1);
    cl[2] = fac_ch("k", 1);     va[2] = fac_ch("", 0);
    cl[3] = fac_ch("sp ace+", 7); va[3] = fac_ch("100%#", 5);
    cl[4] = fac_ch("n\0l", 3);  va[4] = fac_ch("\xC3\xA9\xFF", 3);
    cl[5] = fac_ch("k", 1);     va[5] = fac_ch("dup", 3);

    q = url_params_construere(cl, va, 1, p); ch("N1", q);
    q = url_params_construere(cl + 1, va + 1, 1, p); ch("EMPTYKEY", q);
    q = url_params_construere(cl + 2, va + 2, 1, p); ch("EMPTYVAL", q);
    q = url_params_construere(cl, va, 6, p); ch("N6", q);
    r = url_params_parse(q, p);
    printf("parse numerus %u\n", (unsigned)r.numerus);
    for (k = 0; k < r.numerus && k < 6; k++) {
        if (r.claves[k].mensura != cl[k].mensura || (cl[k].mensura && memcmp(r.claves[k].datum, cl[k].datum, cl[k].mensura))) { mala++; printf("clavis %u differt\n", (unsigned)k); }
        if (r.valores[k].mensura != va[k].mensura || (va[k].mensura && memcmp(r.valores[k].datum, va[k].datum, va[k].mensura))) { mala++; printf("valor %u differt\n", (unsigned)k); }
    }
    printf("reditus mala=%d\n", mala);
    q = url_params_construere(cl, va, 0, p); ch("N0", q);
    q = url_params_construere(NIHIL, va, 1, p); ch("NILCLAVES", q);
    /* two empty pairs */
    cl[0] = fac_ch("", 0); va[0] = fac_ch("", 0); cl[1] = cl[0]; va[1] = va[0];
    q = url_params_construere(cl, va, 2, p); ch("TWOEMPTY", q);
    r = url_params_parse(q, p);
    printf("TWOEMPTY parse numerus %u\n", (unsigned)r.numerus);
    return 0;
}
