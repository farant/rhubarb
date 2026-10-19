#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "tabula_dispersa.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* in = internamentum_creare(p);
    const char* a[4] = {"c693596","c693597","c693594","c693595"};
    const char* b[4] = {"c1170850","c1170851","c1170852","c1170853"};
    int k;
    for (k = 0; k < 4; k++) {
        chorda ca, cb; chorda *pa, *pb, *qb;
        ca.datum = (i8*)a[k]; ca.mensura = (i32)strlen(a[k]);
        cb.datum = (i8*)b[k]; cb.mensura = (i32)strlen(b[k]);
        printf("hash %s=%u %s=%u\n", a[k], tabula_friare_fnv1a(ca), b[k], tabula_friare_fnv1a(cb));
        pa = chorda_internare(in, ca);
        printf("  est(%s) before intern = %d (0)\n", b[k], (int)chorda_est_internata(in, cb));
        qb = chorda_internare_ex_literis(in, b[k]);
        pb = chorda_internare(in, cb);
        printf("  pa!=pb %d  pb==qb %d  pa==internare(a) %d  ex_literis(a)==pa %d\n",
            pa != pb, pb == qb, chorda_internare(in, ca) == pa, chorda_internare_ex_literis(in, a[k]) == pa);
    }
    printf("numerus=%u (8)\n", internamentum_numerus(in));
    piscina_destruere(p);
    return 0;
}
