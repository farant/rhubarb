#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "base64.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    chorda ch, enc;
    Base64Fructus fr;
    i8 copia[16];
    static const i8 vacuum_lit[1] = { 0 };

    /* vacua decodificatio, datum non-NIHIL */
    ch.datum = (i8*)piscina_allocare(p, 1); ch.mensura = 0;
    fr = base64_decodificare(ch, p);
    printf("dec \"\" (datum non-NIHIL): datum=%s mensura=%u  [RFC: \"\" -> \"\" successus]\n",
        fr.datum ? "non-NIHIL" : "NIHIL", (unsigned)fr.mensura);
    /* vacua codificatio */
    enc = base64_codificare(vacuum_lit, 0, p);
    printf("enc \"\": datum=%s mensura=%u\n", enc.datum ? "non-NIHIL" : "NIHIL", (unsigned)enc.mensura);
    fr = base64_decodificare(enc, p);
    printf("dec(enc(\"\")): datum=%s -> round trip %s\n", fr.datum ? "non-NIHIL" : "NIHIL",
        fr.datum ? "successus" : "ERROR per contractum");
    enc = base64_codificare(NIHIL, 0, p);
    printf("enc NIHIL,0: datum=%s mensura=%u\n", enc.datum ? "non-NIHIL" : "NIHIL", (unsigned)enc.mensura);

    /* alias: ingressus in eadem piscina; ingressus immutatus */
    ch.datum = (i8*)piscina_allocare(p, 12); memcpy(ch.datum, "Zm9vYmFyYg==", 12); ch.mensura = 12;
    memcpy(copia, ch.datum, 12);
    fr = base64_decodificare(ch, p);
    printf("alias: %s mensura=%u '%.*s' ingressus %s\n", fr.datum ? "ACC" : "REJ", (unsigned)fr.mensura,
        (int)fr.mensura, (const char*)fr.datum, memcmp(copia, ch.datum, 12) == 0 ? "immutatus" : "MUTATUS");
    /* chorda subrange: mensura 4 ex "Zm9vXXXX" (non-NUL-terminata) */
    ch.mensura = 4;
    fr = base64_decodificare(ch, p);
    printf("subrange 4: %s '%.*s'\n", fr.datum ? "ACC" : "REJ", (int)fr.mensura, (const char*)fr.datum);
    piscina_destruere(p);
    return 0;
}
