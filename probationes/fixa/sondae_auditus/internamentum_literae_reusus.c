#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* in = internamentum_creare(p);
    char buf[16]; chorda *r1, *r2, *r3; chorda q;
    strcpy(buf, "alpha"); r1 = chorda_internare_ex_literis(in, buf);
    strcpy(buf, "omega"); r2 = chorda_internare_ex_literis(in, buf);
    printf("r1='%.*s' r2='%.*s' distinct=%d datum_not_caller=%d\n", (int)r1->mensura, r1->datum,
        (int)r2->mensura, r2->datum, r1 != r2, (char*)r1->datum != buf);
    strcpy(buf, "alpha"); r3 = chorda_internare_ex_literis(in, buf);
    printf("alpha again same=%d numerus=%u\n", r3 == r1, internamentum_numerus(in));
    q.datum = (i8*)"alpha"; q.mensura = 5;
    printf("est(alpha)=%d est(alph)=%d\n", (int)chorda_est_internata(in, q), (q.mensura = 4, (int)chorda_est_internata(in, q)));
    /* single byte */
    r1 = chorda_internare_ex_literis(in, "z"); q.datum = (i8*)"z"; q.mensura = 1;
    printf("single byte: nonNIHIL=%d same=%d mensura=%u\n", r1 != NIHIL, chorda_internare(in, q) == r1, r1->mensura);
    piscina_destruere(p);
    return 0;
}
