#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    int modus = argc > 1 ? atoi(argv[1]) : 0;
    if (modus == 0) {
        Piscina* p = piscina_generare_certae_magnitudinis("sonda_parva", 64);
        InternamentumChorda* in;
        printf("creare in 64-byte fixed piscina...\n"); fflush(stdout);
        in = internamentum_creare(p);
        printf("creare = %p\n", (void*)in);
    } else {
        Piscina* p = piscina_generare_certae_magnitudinis("sonda_fixa", 8192);
        InternamentumChorda* in = internamentum_creare(p);
        char buf[32]; int k; chorda* r = NIHIL;
        printf("creare=%p reliqua=%lu\n", (void*)in, (unsigned long)piscina_reliqua_antequam_cresca_alvei(p));
        fflush(stdout);
        for (k = 0; k < 100000; k++) {
            sprintf(buf, "clavis_%d", k);
            r = chorda_internare_ex_literis(in, buf);
            if (!r) { printf("k=%d NIHIL redditum numerus=%u\n", k, internamentum_numerus(in)); break; }
            printf("k=%d ok numerus=%u reliqua=%lu\n", k, internamentum_numerus(in), (unsigned long)piscina_reliqua_antequam_cresca_alvei(p));
            fflush(stdout);
        }
    }
    return 0;
}
