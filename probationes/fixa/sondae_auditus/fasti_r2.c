#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "fasti.h"
#include <stdio.h>
#include <string.h>

/* r2: fasti_formare_diem cum Dies (a) invalida mensis 13, (b) VALIDA
 * annus -5000 (JDN negativus -> dies hebdomadis negativus). argv[1]
 * eligit casum. */
int main(int argc, char** argv)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    const char* casum = argc > 1 ? argv[1] : "b";
    Dies d;
    chorda c;
    if (casum[0] == 'a') {
        d = fasti_dies(2024, 13, 1);
        c = fasti_formare_diem(d, FASTI_FORMA_LATINA_BREVIS, p);
    } else {
        d = fasti_dies(-5000, 3, 2);
        printf("valida=%d jdn=%lld dh=%d\n", (int)fasti_dies_valida(d),
            (long long)fasti_ad_iulianum(d), (int)fasti_dies_hebdomadis(d));
        fflush(stdout);
        c = fasti_formare_diem(d, FASTI_FORMA_LITURGICA, p);
    }
    printf("forma: %.*s\n", (int)c.mensura, (const char*)c.datum);
    return 0;
}
