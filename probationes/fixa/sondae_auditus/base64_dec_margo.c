#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "base64.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Ingressus in malloc EXACTAE magnitudinis (ASan videt quemvis octetum
 * ultra mensuram) - nulla lectio ultra mensuram. */
static void casus_unus(const char* s, Piscina* p)
{
    unsigned n = (unsigned)strlen(s);
    i8* m = (i8*)malloc(n);
    chorda ch; Base64Fructus fr;
    memcpy(m, s, n);
    ch.datum = m; ch.mensura = n;
    fr = base64_decodificare(ch, p);
    printf("%-10s n=%u longdec=%u dec=%s mensura=%u\n", s, n,
        (unsigned)base64_longitudo_decodificata(ch), fr.datum ? "ACC" : "REJ", (unsigned)fr.mensura);
    free(m);
}

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    chorda nul;
    casus_unus("=", p); casus_unus("A", p); casus_unus("Zg", p); casus_unus("Zm8", p);
    casus_unus("Zm9vY", p); casus_unus("Zm9vYm", p); casus_unus("Zm9vYmE", p);
    casus_unus("Zm9v", p); casus_unus("Zm9vYg==", p);
    nul.datum = NIHIL; nul.mensura = 8;
    printf("longdec(NIHIL, 8)=%u\n", (unsigned)base64_longitudo_decodificata(nul));
    piscina_destruere(p);
    return 0;
}
