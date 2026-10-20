#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "base64.h"
#include <stdio.h>
#include <string.h>

static void casus_unus(const char* s, unsigned n, Piscina* p)
{
    chorda ch;
    Base64Fructus fr;
    i32 lon;
    ch.datum = (i8*)piscina_allocare(p, n ? n : 1);
    memcpy(ch.datum, s, n);
    ch.mensura = n;
    lon = base64_longitudo_decodificata(ch);
    fr = base64_decodificare(ch, p);
    printf("in=\"%.*s\" n=%u longdec=%u (0x%08X) decode=%s mensura=%u\n",
        (int)n, s, n, (unsigned)lon, (unsigned)lon,
        fr.datum ? "OK" : "NIHIL", (unsigned)fr.mensura);
}

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    chorda vac;
    casus_unus("=", 1, p);
    casus_unus("==", 2, p);
    casus_unus("A=", 2, p);
    casus_unus("Q==", 3, p);
    casus_unus("Zg", 2, p);
    casus_unus("Z", 1, p);
    casus_unus("Zm8", 3, p);
    casus_unus("Zm9vY", 5, p);
    casus_unus("Zg==", 4, p);
    casus_unus("Zm8=", 4, p);
    casus_unus("Zm9vYg", 6, p);
    casus_unus("Zg=A", 4, p);
    casus_unus("====", 4, p);
    casus_unus("Zg==Zg==", 8, p);
    vac.datum = (i8*)"x"; vac.mensura = 0;
    printf("vacua datum!=NIHIL longdec=%u\n", (unsigned)base64_longitudo_decodificata(vac));
    vac.datum = NIHIL; vac.mensura = 0;
    printf("vacua datum==NIHIL longdec=%u\n", (unsigned)base64_longitudo_decodificata(vac));
    piscina_destruere(p);
    return 0;
}
