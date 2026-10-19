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
    const char* docs[] = { "<caf\xc3\xa9>x</caf\xc3\xa9>", "<r><a\x01 k=\"v\">x</a></r>", "<r><a+b>x</a+b></r>", "<r><a>t</a junk></r>", NIHIL };
    int k;
    for (k = 0; docs[k]; k++)
    {
        StmlResultus r = stml_legere_ex_literis(docs[k], pp, it);
        chorda w = stml_scribere(r.radix, pp, FALSUM);
        StmlNodus* e = r.elementum_radix;
        printf("CASUS%d\tin=[%s]\tstatus=%d\tsucc=%d\tradix=[%.*s]\tliberi_radicis=%u\tscriptum=[%.*s]\n", k, docs[k], (int)r.status, (int)r.successus,
            e && e->titulus ? (int)e->titulus->mensura : 0, e && e->titulus ? (char*)e->titulus->datum : "", e ? stml_numerus_liberorum(e) : 0, (int)w.mensura, (char*)w.datum);
    }
    return 0;
}
