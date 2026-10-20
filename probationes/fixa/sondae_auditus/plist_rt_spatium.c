/* sonda: iter reditus chordarum cum spatio / CR; scribit plagulas pro oraculo */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "plist.h"
#include <stdio.h>
#include <string.h>

static void hexa(const char* tag, chorda c)
{
    i32 k;
    printf("%s[%u]=\"", tag, (unsigned)c.mensura);
    for (k = 0; k < c.mensura; k++) {
        unsigned char b = (unsigned char)c.datum[k];
        if (b >= 0x20 && b < 0x7f && b != '"' && b != '\\') printf("%c", b);
        else printf("\\x%02x", b);
    }
    printf("\"");
}

static void scribe_plagulam(const char* titulus, chorda t)
{
    char via[256];
    FILE* f;
    sprintf(via, "build/sonda/plist_rt/%s.plist", titulus);
    f = fopen(via, "wb");
    if (f) { fwrite(t.datum, 1, t.mensura, f); fclose(f); }
}

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* intern = internamentum_creare(p);
    const char* casus_t[] = { "  x", "   ", "\tx", "\nx", "a\rb", "a\r\nb", "x  ", "", "a&b<c>\"d'e]]>" };
    const char* titu[] = { "lead2", "ws3", "leadtab", "leadnl", "cr", "crlf", "trail2", "empty", "meta" };
    int idx;
    for (idx = 0; idx < 9; idx++) {
        PlistStatus st;
        PlistValor* v = plist_chordam_creare(chorda_ex_literis(casus_t[idx], p), p);
        chorda out = plist_scribere(v, &st, p);
        PlistResultus r;
        scribe_plagulam(titu[idx], out);
        r = plist_legere(out, p, intern);
        printf("%s: scribere=%d legere=%d ", titu[idx], (int)st, (int)r.status);
        hexa("in", v->textus); printf(" ");
        if (r.radix) hexa("out", r.radix->textus);
        printf(" aequalis=%d\n", r.radix ? (int)plist_aequalis(v, r.radix) : -1);
    }
    /* claves " a" et "a" */
    {
        PlistStatus st;
        PlistValor* d = plist_dictio_creare(p);
        chorda out;
        PlistResultus r;
        plist_dictio_ponere(d, chorda_ex_literis(" a", p), plist_integrum_creare(1, p), p);
        plist_dictio_ponere(d, chorda_ex_literis("a", p), plist_integrum_creare(2, p), p);
        out = plist_scribere(d, &st, p);
        scribe_plagulam("keyws", out);
        r = plist_legere(out, p, intern);
        printf("keyws: scribere=%d legere=%d numerus_in=%u numerus_out=%u aequalis=%d\n",
               (int)st, (int)r.status, (unsigned)plist_numerus(d),
               r.radix ? (unsigned)plist_numerus(r.radix) : 0u,
               r.radix ? (int)plist_aequalis(d, r.radix) : -1);
        printf("%.*s", (int)out.mensura, (const char*)out.datum);
    }
    return 0;
}
