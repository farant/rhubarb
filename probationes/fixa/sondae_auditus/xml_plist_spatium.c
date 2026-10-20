#include "latina.h"
#include "plist.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include <stdio.h>
static void casus_probare(const char* lit, Piscina* p, InternamentumChorda* it)
{
    PlistValor* d; PlistValor* s; PlistStatus st; chorda out; PlistResultus r; PlistValor* v;
    d = plist_dictio_creare(p);
    s = plist_chordam_creare(chorda_ex_literis(lit, p), p);
    plist_dictio_ponere(d, chorda_ex_literis("k", p), s, p);
    out = plist_scribere(d, &st, p);
    printf("in=[%s] st=%d out=%.*s", lit, (int)st, (int)out.mensura, (const char*)out.datum);
    r = plist_legere(out, p, it);
    v = r.radix ? plist_dictio_capere(r.radix, "k") : NIHIL;
    printf("relectum status=%d valor=[%.*s] aequalis=%d\n\n", (int)r.status,
           v ? (int)v->textus.mensura : 0, v && v->textus.datum ? (const char*)v->textus.datum : "",
           r.radix ? (int)plist_aequalis(d, r.radix) : -1);
}
int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* it = internamentum_creare(p);
    casus_probare("  indentatum", p, it);
    casus_probare(" ", p, it);
    casus_probare("finis ", p, it);
    return 0;
}
