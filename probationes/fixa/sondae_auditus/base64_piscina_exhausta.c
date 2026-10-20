#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "base64.h"
#include <stdio.h>
#include <string.h>

/* argv[1] = "dec" aut "enc": piscina certae magnitudinis (64 octeti)
 * minor quam effusio; contractus decodificationis: datum = NIHIL si error. */
int main(int argc, char** argv)
{
    Piscina* magna = piscina_generare_dynamicum("sonda_magna", 65536);
    Piscina* parva = piscina_generare_certae_magnitudinis("sonda_parva", 64);
    static i8 buf[4096];
    chorda ch;
    unsigned k;
    for (k = 0; k < sizeof buf; k++) buf[k] = 'A';
    if (argc > 1 && strcmp(argv[1], "enc") == 0)
    {
        printf("enc 3000 octeti in piscinam 64...\n"); fflush(stdout);
        ch = base64_codificare(buf, 3000, parva);
        printf("redit datum=%s mensura=%u\n", ch.datum ? "non-NIHIL" : "NIHIL", (unsigned)ch.mensura);
    }
    else
    {
        Base64Fructus fr;
        ch.datum = buf; ch.mensura = 4096;
        printf("dec 4096 characteres in piscinam 64...\n"); fflush(stdout);
        fr = base64_decodificare(ch, parva);
        printf("redit datum=%s mensura=%u\n", fr.datum ? "non-NIHIL" : "NIHIL", (unsigned)fr.mensura);
    }
    piscina_destruere(parva);
    piscina_destruere(magna);
    return 0;
}
