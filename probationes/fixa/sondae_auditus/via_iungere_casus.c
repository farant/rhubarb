#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "via.h"
#include <stdio.h>
#include <string.h>

static void scr(chorda c)
{
    unsigned int k;
    if (!c.datum) { printf("<NIHIL m=%u>", (unsigned)c.mensura); return; }
    printf("'");
    for (k = 0; k < c.mensura; k++) putchar(c.datum[k]);
    printf("'");
}
static Piscina* p;
static void iu(const char* a, const char* b, const char* c, int n)
{
    chorda t[3];
    t[0] = chorda_ex_literis(a, p);
    t[1] = chorda_ex_literis(b ? b : "", p);
    t[2] = chorda_ex_literis(c ? c : "", p);
    printf("{%s|%s|%s} n=%d -> ", a, b ? b : "", c ? c : "", n);
    scr(via_iungere(t, (i32)n, p)); printf("\n");
}
int main(void)
{
    chorda t1[1];
    chorda r;
    p = piscina_generare_dynamicum("sonda", 65536);
    printf("n=0 partes valid -> "); t1[0] = chorda_ex_literis("x", p); scr(via_iungere(t1, 0, p)); printf("\n");
    printf("n=0 partes NIHIL -> "); scr(via_iungere(NIHIL, 0, p)); printf("\n");
    printf("n=3 partes NIHIL -> "); scr(via_iungere(NIHIL, 3, p)); printf("\n");
    iu("", "a", NULL, 2);
    iu("a", "", "b", 3);
    iu("a", "", NULL, 2);
    iu("", "", NULL, 2);
    iu("/", "usr", NULL, 2);
    iu("foo/", "bar", NULL, 2);
    iu("foo", "/bar", NULL, 2);
    iu("foo/", "/bar", NULL, 2);
    iu("a", "/b", NULL, 2);
    iu("/", "/", NULL, 2);
    iu("a", ".", "..", 3);
    iu("a//", "b", NULL, 2);
    /* single part copied? */
    r = via_iungere(t1, 1, p);
    printf("n=1 'x' -> "); scr(r); printf(" alias=%d\n", r.datum == t1[0].datum);
    t1[0].datum[0] = 'Z';
    printf("after mutating input, result -> "); scr(r); printf("\n");
    piscina_destruere(p);
    return 0;
}
