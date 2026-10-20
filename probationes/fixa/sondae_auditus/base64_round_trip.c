#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "base64.h"
#include <stdio.h>
#include <string.h>

/* Data: octeti (k*37+11) & 0xFF longitudinis n = 0..300; effusio
 * codificationis pro oraculo; round trip et aequalitas longitudinum. */
int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    static i8 buf[512];
    unsigned n, k, mala = 0;
    chorda enc;
    Base64Fructus fr;
    for (k = 0; k < 512; k++) buf[k] = (i8)((k * 37u + 11u) & 0xFFu);
    for (n = 0; n <= 300; n++)
    {
        enc = base64_codificare(buf, n, p);
        printf("E %u %.*s\n", n, (int)enc.mensura, enc.datum ? (const char*)enc.datum : "");
        if (enc.mensura != base64_longitudo_codificata(n)) { mala++; printf("MALA longcod n=%u\n", n); }
        if (n == 0) { printf("n=0 enc.datum=%s mensura=%u\n", enc.datum ? "non-NIHIL" : "NIHIL", (unsigned)enc.mensura); }
        fr = base64_decodificare(enc, p);
        if (n == 0) { printf("n=0 decode datum=%s\n", fr.datum ? "non-NIHIL" : "NIHIL"); continue; }
        if (!fr.datum || fr.mensura != n || memcmp(fr.datum, buf, n) != 0) { mala++; printf("MALA rt n=%u\n", n); }
        if (base64_longitudo_decodificata(enc) != fr.mensura) { mala++; printf("MALA longdec n=%u\n", n); }
    }
    /* omnes 256 valores */
    for (k = 0; k < 256; k++) buf[k] = (i8)k;
    enc = base64_codificare(buf, 256, p);
    printf("E 256all %.*s\n", (int)enc.mensura, (const char*)enc.datum);
    fr = base64_decodificare(enc, p);
    printf("rt256 %s\n", (fr.datum && fr.mensura == 256 && memcmp(fr.datum, buf, 256) == 0) ? "OK" : "MALA");
    printf("mala %u\n", mala);
    piscina_destruere(p);
    return 0;
}
