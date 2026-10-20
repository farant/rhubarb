#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "fasti.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    chorda c = fasti_formare_diem(fasti_dies(2025, 12, 25), FASTI_FORMA_ANGLICA_LONGA, p);
    chorda e = fasti_formare_diem(fasti_dies(-1, 6, 1), FASTI_FORMA_ANGLICA_LONGA, p);
    chorda z = fasti_formare_diem(fasti_dies(0, 6, 1), FASTI_FORMA_ANGLICA_LONGA, p);
    Dies d;
    chorda s;
    printf("2025-12-25: '%.*s'\n", (int)c.mensura, (const char*)c.datum);
    printf("annus -1:   '%.*s'\n", (int)e.mensura, (const char*)e.datum);
    printf("annus 0:    '%.*s'\n", (int)z.mensura, (const char*)z.datum);
    printf("bissextilis(0)=%d valida(0-02-29)=%d\n", (int)fasti_est_bissextilis(0),
        (int)fasti_dies_valida(fasti_dies(0, 2, 29)));
    s.datum = (i8*)"15/03/2024"; s.mensura = 10;
    printf("parsare_diem('15/03/2024')=%d\n", (int)fasti_parsare_diem(s, &d));
    s.datum = (i8*)"03/15/2024";
    printf("parsare_diem('03/15/2024')=%d\n", (int)fasti_parsare_diem(s, &d));
    return 0;
}
