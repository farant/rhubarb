#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "base64.h"
#include <stdio.h>
#include <string.h>

static void casus_unus(const char* s, Piscina* p)
{
    chorda ch;
    Base64Fructus fr;
    unsigned n = (unsigned)strlen(s), k;
    ch.datum = (i8*)piscina_allocare(p, n);
    memcpy(ch.datum, s, n);
    ch.mensura = n;
    fr = base64_decodificare(ch, p);
    printf("%s\t", s);
    if (!fr.datum) { printf("REJ\n"); return; }
    printf("ACC:");
    for (k = 0; k < fr.mensura; k++) printf("%02x", fr.datum[k]);
    printf("\n");
}

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    const char* casus_omnes[] = {
        "Zg==Zg==", "Zg==Zm9v", "Zm8=Zm9v", "Zg=A", "AA=A", "AAAAA=AA",
        "Z===", "====", "Zg=", "Zg===", "Zm9v=", "Zm9v====",
        "Zh==", "Zm9=", "Zm8=", "Zg==", "Zm9vYg==", "Zm9vYmE=", "Zm9vYmFy",
        "=AAA", "A=AA", "AA==AAAA", "Zm9vYg=", NULL };
    int t;
    for (t = 0; casus_omnes[t]; t++) casus_unus(casus_omnes[t], p);
    piscina_destruere(p);
    return 0;
}
