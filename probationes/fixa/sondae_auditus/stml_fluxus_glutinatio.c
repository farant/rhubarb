#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    Piscina* pp = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* it = internamentum_creare(pp);
    const char* docs[] = { "<p>one <b>two</b>\n  three</p>", "<p>a<!--c-->\n b</p>", "<p><b>x</b>\n<i>y</i> z</p>", NIHIL };
    int k;
    for (k = 0; docs[k]; k++)
    {
        chorda in, p, f0, f1, t0, t1; StmlResultus r0, r1;
        in.datum = (i8*)(size_t)docs[k]; in.mensura = (i32)strlen(docs[k]);
        r0 = stml_legere(in, pp, it);
        p = stml_scribere(r0.radix, pp, VERUM);
        r1 = stml_legere(p, pp, it);
        f0 = stml_textus_fluxus(r0.elementum_radix, pp);
        f1 = stml_textus_fluxus(r1.elementum_radix, pp);
        t0 = stml_textus_normalizatus(r0.elementum_radix, pp);
        t1 = stml_textus_normalizatus(r1.elementum_radix, pp);
        printf("DOC%d\tpulcher=[%.*s]\tfluxus_ante=[%.*s]\tfluxus_post=[%.*s]\tnorm_ante=[%.*s]\tnorm_post=[%.*s]\n", k, (int)p.mensura, (char*)p.datum,
            (int)f0.mensura, (char*)f0.datum, (int)f1.mensura, (char*)f1.datum, (int)t0.mensura, (char*)t0.datum, (int)t1.mensura, (char*)t1.datum);
    }
    return 0;
}
