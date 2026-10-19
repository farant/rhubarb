/* Is scribere's / exprimere's result NUL-terminated after the arena has
 * been dirtied? (house chorda convention: not promised) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "numerus_romanus.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 4096);
    chorda c;
    char* junk = (char*)piscina_allocare(p, 4000);
    memset(junk, 'Z', 4000);
    piscina_vacare(p);
    c = numerus_romanus_scribere(3888, p);
    printf("scribere 3888 len=%u next=%d '%c'\n", c.mensura, (int)c.datum[c.mensura], c.datum[c.mensura]);
    piscina_vacare(p);
    c = numerus_romanus_scribere(1, p);
    printf("scribere 1 len=%u next=%d '%c' strlen=%lu\n", c.mensura, (int)c.datum[c.mensura], c.datum[c.mensura], (unsigned long)strlen((char*)c.datum));
    piscina_vacare(p);
    c = numerus_romanus_exprimere(5280, NIHIL, p);
    printf("exprimere 5280 len=%u next=%d\n", c.mensura, (int)c.datum[c.mensura]);
    piscina_destruere(p);
    return 0;
}
