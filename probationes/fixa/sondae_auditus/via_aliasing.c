#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "via.h"
#include <stdio.h>
#include <string.h>

static void scr(const char* t, chorda c, chorda in)
{
    unsigned int k;
    printf("%s -> '", t);
    for (k = 0; k < c.mensura; k++) putchar(c.datum[k]);
    printf("' alias=%d\n", (c.datum >= in.datum && c.datum < in.datum + in.mensura));
}
int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    chorda in = chorda_ex_literis("/usr/lib/foo.txt", p);
    chorda d = via_directorium(in, p);
    chorda n = via_nomen(in, p);
    chorda r = via_nomen_radix(in, p);
    chorda e = via_extensio(in, p);
    chorda in2 = chorda_ex_literis("bar", p);
    chorda nn = via_normalizare(in2, p);
    chorda empty; chorda ne;
    empty.datum = NIHIL; empty.mensura = 0;
    scr("directorium", d, in); scr("nomen", n, in); scr("nomen_radix", r, in); scr("extensio", e, in);
    scr("normalizare(bar)", nn, in2);
    memset(in.datum, 'Q', in.mensura);
    memset(in2.datum, 'Q', in2.mensura);
    printf("--- after overwriting the input buffers with 'Q' ---\n");
    scr("directorium", d, in); scr("nomen", n, in); scr("nomen_radix", r, in); scr("extensio", e, in);
    scr("normalizare(bar)", nn, in2);
    ne = via_normalizare(empty, p);
    printf("normalizare(NIHIL,0) datum=%p mensura=%u\n", (void*)ne.datum, (unsigned)ne.mensura);
    piscina_destruere(p);
    return 0;
}
