#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    i8* sb = (i8*)malloc(65536);
    i8* nb = (i8*)malloc(65537);
    chorda s, ant, nov, r;
    memset(sb, 'a', 65536);
    memset(nb, 'b', 65537);
    s.datum = sb; s.mensura = 65536;
    ant.datum = sb; ant.mensura = 1;      /* "a" */
    nov.datum = nb; nov.mensura = 65537;  /* 65537 x 'b' */
    /* true length 65536 * 65537 = 4295032832; i32: 65536 + 65536*65536 = 65536 mod 2^32 */
    printf("case substituere s=65536x'a' antiquum=\"a\" novum=65537x'b'\n");
    fflush(stdout);
    r = chorda_substituere(s, ant, nov, p);
    printf("mensura=%u\n", r.mensura);
    return 0;
}
