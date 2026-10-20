/* sonda: duplicate keys, empty keys, capere_chorda bounds, intern table growth */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Piscina* pisc;

static void lege_(const char* t, const char* s)
{
    JsonResultus r = json_legere_literis(s, pisc);
    printf("%-18s succ=%d L%u C%u [%.*s]\n", t, (int)r.successus, (unsigned)r.linea, (unsigned)r.columna,
        (int)r.error.mensura, r.error.datum ? (const char*)r.error.datum : "");
}

int main(void)
{
    JsonResultus r;
    i32 n0, n1; int i; char buf[64];
    chorda k; i8* exact;
    JsonObjectumIterator it; chorda ck; JsonValor* cv;
    pisc = piscina_generare_dynamicum("sonda", 65536);
    lege_("dup_plain", "{\"a\":1,\"a\":2}");
    lege_("dup_escape", "{\"a\":1,\"\\u0061\":2}");
    lege_("dup_slash", "{\"/\":1,\"\\/\":2}");
    lege_("dup_empty", "{\"\":1,\"\":2}");
    lege_("dup_nul", "{\"\\u0000\":1,\"\\u0000\":2}");
    lege_("nul_vs_empty", "{\"\\u0000\":1,\"\":2}");
    lege_("nested_ok", "{\"a\":{\"a\":1}}");
    lege_("sibling_ok", "[{\"a\":1},{\"a\":2}]");
    lege_("dup_late", "{\"a\":1,\"b\":2,\"c\":3,\"b\":4}");

    /* key with embedded NUL: capere vs capere_chorda */
    r = json_legere_literis("{\"a\\u0000b\":1,\"a\":2}", pisc);
    k.datum = (i8*)"a\0b"; k.mensura = 3;
    printf("nulkey capere(\"a\")=%lld capere_chorda(a\\0b)=%lld\n",
        (long long)json_ad_integer(json_objectum_capere(r.radix, "a")),
        (long long)json_ad_integer(json_objectum_capere_chorda(r.radix, k)));
    { chorda w = json_scribere(r.radix, pisc); printf("nulkey scribere=%.*s (mensura %u)\n", (int)w.mensura, (const char*)w.datum, (unsigned)w.mensura); }

    /* capere_chorda with key in exact malloc buffer (no NUL) */
    r = json_legere_literis("{\"clavis\":7,\"\":8}", pisc);
    exact = (i8*)malloc(6); memcpy(exact, "clavis", 6);
    k.datum = exact; k.mensura = 6;
    printf("exact capere_chorda=%lld\n", (long long)json_ad_integer(json_objectum_capere_chorda(r.radix, k)));
    k.mensura = 3;
    printf("prefix 'cla' capere_chorda=%s\n", json_objectum_capere_chorda(r.radix, k) ? "found" : "NIHIL");
    free(exact);
    k.datum = NULL; k.mensura = 0;
    printf("empty {NIHIL,0} capere_chorda=%lld capere(\"\")=%lld habet(\"\")=%d habet(NIHIL)=%d\n",
        (long long)json_ad_integer(json_objectum_capere_chorda(r.radix, k)),
        (long long)json_ad_integer(json_objectum_capere(r.radix, "")),
        (int)json_objectum_habet(r.radix, ""), (int)json_objectum_habet(r.radix, NULL));
    printf("capere_integer(NIHIL clavis)=%lld\n", (long long)json_capere_integer(r.radix, NULL, -5));
    it = json_objectum_iterator(r.radix);
    while (json_objectum_iterator_proxima(&it, &ck, &cv))
        printf("iter key mensura=%u datum=%s val=%lld\n", (unsigned)ck.mensura, ck.datum ? "ptr" : "NIHIL", (long long)json_ad_integer(cv));
    printf("iter after end=%d\n", (int)json_objectum_iterator_proxima(&it, &ck, &cv));

    /* intern growth: failed lookups of absent keys */
    n0 = internamentum_numerus(internamentum_globale());
    for (i = 0; i < 5000; i++) { sprintf(buf, "absens_%d", i); (void)json_objectum_capere(r.radix, buf); }
    n1 = internamentum_numerus(internamentum_globale());
    printf("intern after 5000 absent capere: +%u\n", (unsigned)(n1 - n0));
    n0 = n1;
    for (i = 0; i < 5000; i++) { sprintf(buf, "absens_v_%d", i); (void)json_via(r.radix, buf); }
    n1 = internamentum_numerus(internamentum_globale());
    printf("intern after 5000 absent json_via: +%u\n", (unsigned)(n1 - n0));
    n0 = n1;
    for (i = 0; i < 5000; i++) { sprintf(buf, "{\"unica_%d\":1}", i); (void)json_legere_literis(buf, pisc); piscina_vacare(pisc); }
    n1 = internamentum_numerus(internamentum_globale());
    printf("intern after 5000 parses unique keys (arena emptied): +%u\n", (unsigned)(n1 - n0));
    n0 = n1;
    for (i = 0; i < 5000; i++) { sprintf(buf, "{\"unica_x_%d\":1}", i); r = json_legere_literis(buf, pisc); (void)r; }
    /* failed parses intern too? */
    n0 = internamentum_numerus(internamentum_globale());
    for (i = 0; i < 5000; i++) { sprintf(buf, "{\"fallax_%d\" 1}", i); (void)json_legere_literis(buf, pisc); }
    n1 = internamentum_numerus(internamentum_globale());
    printf("intern after 5000 FAILED parses: +%u\n", (unsigned)(n1 - n0));
    return 0;
}
