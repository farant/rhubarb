#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include <stdio.h>

int main(void)
{
    Piscina* p = piscina_generare_certae_magnitudinis("sonda_fixa", 8192);
    InternamentumChorda* in = internamentum_creare(p);
    int k;
    printf("creare: %p numerus=%u reliqua=%lu\n", (void*)in, internamentum_numerus(in),
        (unsigned long)piscina_reliqua_antequam_cresca_alvei(p));
    fflush(stdout);
    for (k = 0; k < 100000; k++) {
        chorda* r = chorda_internare_ex_literis(in, "");
        if (r != NIHIL) { printf("k=%d non NIHIL\n", k); break; }
        if (k % 500 == 0) { printf("k=%d refusals ok, reliqua=%lu\n", k,
            (unsigned long)piscina_reliqua_antequam_cresca_alvei(p)); fflush(stdout); }
    }
    printf("finis k=%d\n", k);
    return 0;
}
