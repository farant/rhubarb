/* scribere: print every n in 0..4001 plus extremes; NUL-termination and
 * empty forms; NIHIL piscina. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "numerus_romanus.h"
#include <stdio.h>
#include <string.h>

static void show(i32 n, chorda c)
{
    printf("S %u %.*s|len=%u datum=%s\n", n, (int)c.mensura, c.datum ? (char*)c.datum : "",
           c.mensura, c.datum ? "ptr" : "NIHIL");
}

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    i32 n;
    i32 zero = 0;
    chorda c;
    for (n = 0; n <= 4001; n++) { show(n, numerus_romanus_scribere(n, p)); if (n % 100 == 0) piscina_vacare(p); }
    show(4096, numerus_romanus_scribere(4096, p));
    show(0xFFFFFFFFu, numerus_romanus_scribere(0xFFFFFFFFu, p));
    show(zero - 1, numerus_romanus_scribere(zero - 1, p));
    show(0x80000000u, numerus_romanus_scribere(0x80000000u, p));
    c = numerus_romanus_scribere(106, NIHIL);
    printf("NIHILPISC len=%u datum=%s\n", c.mensura, c.datum ? "ptr" : "NIHIL");
    /* NUL termination: byte after the 15-char output */
    piscina_vacare(p);
    c = numerus_romanus_scribere(3888, p);
    printf("NULTERM 3888 len=%u nextbyte=%d (capacity 16 so byte 15 is inside buffer)\n", c.mensura, (int)c.datum[c.mensura]);
    c = numerus_romanus_scribere(1, p);
    printf("NULTERM 1 len=%u nextbyte=%d\n", c.mensura, (int)c.datum[c.mensura]);
    piscina_destruere(p);
    return 0;
}
