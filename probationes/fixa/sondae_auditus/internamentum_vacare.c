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
    InternamentumChorda* in2 = internamentum_creare(p);
    InternamentumChorda *g1, *g2;
    chorda *old, *nov, *x2, *gold, *gnew;
    memoriae_index u0, u1; int k; char buf[32];

    internamentum_vacare(in); internamentum_vacare(in);
    printf("vacare twice on empty: numerus=%u\n", internamentum_numerus(in));
    old = chorda_internare_ex_literis(in, "titulus");
    x2 = chorda_internare_ex_literis(in2, "titulus");
    printf("two interns same piscina: distinct=%d in2 numerus=%u est_in2(old)=%d\n",
        old != x2, internamentum_numerus(in2), (int)chorda_est_internata(in2, *old));
    internamentum_vacare(in);
    printf("after vacare: numerus=%u est(titulus)=%d old still readable='%.*s'\n",
        internamentum_numerus(in), (int)chorda_est_internata(in, *old), (int)old->mensura, old->datum);
    nov = chorda_internare_ex_literis(in, "titulus");
    printf("re-intern after vacare: new==old %d (old pointer no longer canonical)\n", nov == old);
    printf("in2 unaffected: numerus=%u est=%d\n", internamentum_numerus(in2), (int)chorda_est_internata(in2, *old));
    /* memory not reclaimed by vacare */
    u0 = piscina_summa_usus(p);
    for (k = 0; k < 50; k++) {
        int j;
        for (j = 0; j < 100; j++) { sprintf(buf, "s%d", j); chorda_internare_ex_literis(in, buf); }
        internamentum_vacare(in);
    }
    u1 = piscina_summa_usus(p);
    printf("50 cycles x 100 same strings + vacare: usus grew by %lu bytes, numerus=%u\n",
        (unsigned long)(u1-u0), internamentum_numerus(in));
    /* global */
    g1 = internamentum_globale(); g2 = internamentum_globale();
    printf("globale same=%d nonNIHIL=%d\n", g1 == g2, g1 != NIHIL);
    gold = chorda_internare_ex_literis(g1, "globalis");
    internamentum_vacare(g1);
    gnew = chorda_internare_ex_literis(internamentum_globale(), "globalis");
    printf("global vacare: old==new %d, globale still same ptr %d\n", gold == gnew, internamentum_globale() == g1);
    piscina_destruere(p);
    return 0;
}
