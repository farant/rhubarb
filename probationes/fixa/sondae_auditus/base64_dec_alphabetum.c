#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "base64.h"
#include <stdio.h>
#include <string.h>

/* Pro omni octeto v (0..255) et positione pos (0..3) in "QUJD" (=ABC):
 * acceptum aut rejectum; effusio pro oraculo python. */
int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    unsigned v, pos, k;
    i8 buf[8];
    chorda ch;
    Base64Fructus fr;
    const char* spec[] = { "Zm9v\r\nYmFy", "Zm9v YmFy", "Zm9v\nYmFy", "Zm9vYmF\n",
                           "Zm-_", "Zm9v-_8=", NULL };
    int t;
    for (pos = 0; pos < 4; pos++)
        for (v = 0; v < 256; v++)
        {
            memcpy(buf, "QUJD", 4);
            buf[pos] = (i8)v;
            ch.datum = buf; ch.mensura = 4;
            fr = base64_decodificare(ch, p);
            printf("%u %u ", pos, v);
            if (!fr.datum) printf("REJ\n");
            else { printf("ACC:"); for (k = 0; k < fr.mensura; k++) printf("%02x", fr.datum[k]); printf("\n"); }
        }
    for (t = 0; spec[t]; t++)
    {
        unsigned n = (unsigned)strlen(spec[t]);
        memcpy(buf, spec[t], n > 8 ? 8 : n);
        ch.datum = (i8*)piscina_allocare(p, n); memcpy(ch.datum, spec[t], n); ch.mensura = n;
        fr = base64_decodificare(ch, p);
        printf("SPEC %u %s\n", t, fr.datum ? "ACC" : "REJ");
    }
    piscina_destruere(p);
    return 0;
}
