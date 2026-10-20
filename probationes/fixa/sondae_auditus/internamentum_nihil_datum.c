#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include <stdio.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* in = internamentum_creare(p);
    chorda mala; chorda bona; chorda* r;
    mala.datum = NIHIL; mala.mensura = 5;
    bona = chorda_ex_literis("x", p);
    /* NIHIL intern on every entry point */
    printf("internare(NIHIL,x)=%p\n", (void*)chorda_internare(NIHIL, bona));
    printf("ex_literis(NIHIL,\"x\")=%p\n", (void*)chorda_internare_ex_literis(NIHIL, "x"));
    printf("ex_literis(in,NIHIL)=%p\n", (void*)chorda_internare_ex_literis(in, NIHIL));
    printf("est_internata(NIHIL,x)=%d\n", (int)chorda_est_internata(NIHIL, bona));
    printf("numerus(NIHIL)=%u\n", internamentum_numerus(NIHIL));
    internamentum_vacare(NIHIL); printf("vacare(NIHIL) ok\n");
    printf("creare(NIHIL)=%p\n", (void*)internamentum_creare(NIHIL));
    r = chorda_internare(in, mala);
    printf("internare(datum NIHIL, mensura 5)=%p numerus=%u\n", (void*)r, internamentum_numerus(in));
    fflush(stdout);
    printf("est_internata(datum NIHIL, mensura 5) -> ...\n"); fflush(stdout);
    printf("  = %d\n", (int)chorda_est_internata(in, mala));
    return 0;
}
