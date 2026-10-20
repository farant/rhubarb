#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tabula_dispersa.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    TabulaDispersa* t = tabula_dispersa_creare_chorda(p, 16);
    static char keys[200000][12];
    long n;
    chorda c;
    /* churn: at most ONE live key at any moment */
    for (n = 0; n < 200000; n++) {
        sprintf(keys[n], "k%ld", n);
        c.datum = (i8*)keys[n]; c.mensura = (i32)strlen(keys[n]);
        tabula_dispersa_inserere(t, c, (void*)1);
        tabula_dispersa_delere(t, c);
        if (n == 99 || n == 999 || n == 9999 || n == 99999 || n == 199999)
            printf("post %ld insert+delete: numerus=%u numerus_deletorum=%u capacitas=%u (bytes sloti=%lu)\n",
                n + 1, t->numerus, t->numerus_deletorum, t->capacitas,
                (unsigned long)t->capacitas * sizeof(Slotus));
    }
    return 0;
}
