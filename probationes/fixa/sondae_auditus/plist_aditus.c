/* sonda: aditus - ad_indicem, capere, ponere, addere, numerus, aequalis */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "plist.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* intern = internamentum_creare(p);
    PlistValor* l = plist_lista_creare(p);
    PlistValor* d = plist_dictio_creare(p);
    PlistValor* s = plist_chordam_creare(chorda_ex_literis("x", p), p);
    PlistValor* nan1 = plist_realem_creare(NAN, p);
    char buf[8];
    chorda kb;

    printf("ad_indicem(vacua,0)=%p\n", (void*)plist_ad_indicem(l, 0));
    plist_lista_addere(l, s, p);
    printf("ad_indicem(l,1)=%p ad_indicem(l,0xFFFFFFFF)=%p ad_indicem(l,0)==s:%d\n",
        (void*)plist_ad_indicem(l, 1), (void*)plist_ad_indicem(l, 0xFFFFFFFFu), plist_ad_indicem(l, 0) == s);
    plist_dictio_ponere(d, chorda_ex_literis("abc", p), s, p);
    printf("ad_indicem(dict,0)=%p\n", (void*)plist_ad_indicem(d, 0));
    printf("lista_addere(dict)=%d numerus(dict)=%u\n", (int)plist_lista_addere(d, s, p), (unsigned)plist_numerus(d));
    printf("ponere(lista)=%d ponere(scalar)=%d ponere(NIHIL val)=%d\n",
        (int)plist_dictio_ponere(l, chorda_ex_literis("k", p), s, p),
        (int)plist_dictio_ponere(s, chorda_ex_literis("k", p), s, p),
        (int)plist_dictio_ponere(d, chorda_ex_literis("k", p), NIHIL, p));
    printf("capere ab=%p abcd=%p abc==s:%d \"\"=%p NIHIL=%p capere(lista)=%p\n",
        (void*)plist_dictio_capere(d, "ab"), (void*)plist_dictio_capere(d, "abcd"),
        plist_dictio_capere(d, "abc") == s, (void*)plist_dictio_capere(d, ""),
        (void*)plist_dictio_capere(d, NIHIL), (void*)plist_dictio_capere(l, "abc"));
    /* clavis vacua */
    printf("ponere(\"\")=%d capere(\"\")==s:%d\n",
        (int)plist_dictio_ponere(d, chorda_ex_literis("", p), s, p), plist_dictio_capere(d, "") == s);
    /* aliasing: clavis ex buffer vocantis */
    strcpy(buf, "alias"); kb.datum = (i8*)buf; kb.mensura = 5;
    plist_dictio_ponere(d, kb, s, p);
    strcpy(buf, "ZZZZZ");
    printf("aliasing: capere(alias)=%p capere(ZZZZZ)=%p\n", (void*)plist_dictio_capere(d, "alias"), (void*)plist_dictio_capere(d, "ZZZZZ"));
    printf("numerus(scalar)=%u numerus(NIHIL)=%u\n", (unsigned)plist_numerus(s), (unsigned)plist_numerus(NIHIL));
    printf("aequalis(nan,nan same ptr)=%d\n", (int)plist_aequalis(nan1, nan1));
    printf("aequalis(-0,+0)=%d\n", (int)plist_aequalis(plist_realem_creare(-0.0, p), plist_realem_creare(0.0, p)));
    printf("aequalis(NIHIL,NIHIL)=%d aequalis(1,1.0)=%d\n", (int)plist_aequalis(NIHIL, NIHIL),
        (int)plist_aequalis(plist_integrum_creare(1, p), plist_realem_creare(1.0, p)));
    printf("aequalis(verum 2, verum 1)=%d veritas_stored=%u\n",
        (int)plist_aequalis(plist_veritatem_creare(2, p), plist_veritatem_creare(1, p)), (unsigned)plist_veritatem_creare(2, p)->veritas);
    /* NaN in dict: fasciculum - scribere, legere, aequalis */
    {
        PlistValor* dd = plist_dictio_creare(p); PlistStatus st; chorda o; PlistResultus r;
        plist_dictio_ponere(dd, chorda_ex_literis("n", p), plist_realem_creare(NAN, p), p);
        o = plist_scribere(dd, &st, p); r = plist_legere(o, p, intern);
        printf("nan roundtrip: scribere=%d legere=%d aequalis(self)=%d aequalis(lectum)=%d\n",
            (int)st, (int)r.status, (int)plist_aequalis(dd, dd), (int)plist_aequalis(dd, r.radix));
    }
    /* duplicates via legere then ponere */
    {
        const char* t = "<plist version=\"1.0\"><dict><key>a</key><integer>1</integer><key>b</key><integer>2</integer><key>a</key><integer>3</integer></dict></plist>";
        PlistResultus r = plist_legere(chorda_ex_literis(t, p), p, intern);
        PlistPar* p0 = (PlistPar*)xar_obtinere(r.radix->liberi, 0);
        printf("dup legere: numerus=%u primum=%.*s=%lld\n", (unsigned)plist_numerus(r.radix),
            (int)p0->clavis.mensura, (const char*)p0->clavis.datum, (long long)p0->valor->integrum);
    }
    return 0;
}
