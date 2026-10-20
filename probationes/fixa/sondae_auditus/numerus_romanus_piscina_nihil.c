/* exprimere/scribere with NIHIL piscina and compositum NIHIL; then an
 * exhausted fixed-size piscina (expected: house fatal exit(1)). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "numerus_romanus.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    b32 c;
    chorda e;
    i64 ns[5];
    int k;
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    ns[0]=0; ns[1]=106; ns[2]=5280; ns[3]=4096; ns[4]=0xFFFFFFFFFFFFFFFFULL;
    for (k = 0; k < 5; k++)
    {
        c = 7;
        e = numerus_romanus_exprimere(ns[k], &c, NIHIL);
        printf("NIHILpisc n=%llu len=%u datum=%s compositum=%d\n", (unsigned long long)ns[k], e.mensura, e.datum?"ptr":"NIHIL", (int)c);
        e = numerus_romanus_exprimere(ns[k], NIHIL, p);
        printf("NIHILcomp n=%llu %.*s\n", (unsigned long long)ns[k], (int)e.mensura, (char*)e.datum);
    }
    fflush(stdout);
    if (argc > 0)
    {
        Piscina* f = piscina_generare_certae_magnitudinis("sonda_certa", 64);
        int j;
        (void)argv;
        for (j = 0; j < 10; j++)
        {
            c = 7;
            e = numerus_romanus_exprimere(0xFFFFFFFFFFFFFFFFULL, &c, f);
            printf("fixed iter %d len=%u datum=%s c=%d %.*s\n", j, e.mensura, e.datum?"ptr":"NIHIL", (int)c, (int)e.mensura, e.datum?(char*)e.datum:"");
            fflush(stdout);
        }
    }
    piscina_destruere(p);
    return 0;
}
