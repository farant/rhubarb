/* sonda: NIHIL/wrong-genus accessors, index wraps, iterator, creare(NIHIL) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    Piscina* pisc = piscina_generare_dynamicum("sonda", 65536);
    JsonResultus r; JsonValor *arr, *obj, *s; JsonPar *p0, *p0b; int i;
    JsonObjectumIterator it; chorda ck; JsonValor* cv; int n;
    r = json_legere_literis(NULL, pisc);
    printf("legere_literis(NIHIL): succ=%d err.mensura=%u radix=%s\n", (int)r.successus, (unsigned)r.error.mensura, r.radix ? "ptr" : "NIHIL");
    r = json_legere_literis("", pisc);
    printf("legere_literis(\"\"): succ=%d err=[%.*s]\n", (int)r.successus, (int)r.error.mensura, (const char*)r.error.datum);
    r = json_legere_literis("[1]", NULL);
    printf("legere_literis(piscina NIHIL): succ=%d err.mensura=%u\n", (int)r.successus, (unsigned)r.error.mensura);
    r = json_legere_literis("{\"a\":[]}", pisc);
    arr = json_objectum_capere(r.radix, "a");
    printf("obtinere empty[0]=%s [0xFFFFFFFF]=%s nonarr=%s NIHIL=%s\n", json_tabulatum_obtinere(arr, 0) ? "ptr" : "NIHIL",
        json_tabulatum_obtinere(arr, 0xFFFFFFFFu) ? "ptr" : "NIHIL", json_tabulatum_obtinere(r.radix, 0) ? "ptr" : "NIHIL",
        json_tabulatum_obtinere(NULL, 0) ? "ptr" : "NIHIL");
    printf("numerus NIHIL=%u wrong=%u objnum(arr)=%u\n", (unsigned)json_tabulatum_numerus(NULL), (unsigned)json_tabulatum_numerus(r.radix), (unsigned)json_objectum_numerus(arr));
    printf("par_obtinere(obj,1)=%s (obj,0xFFFFFFFF)=%s (arr,0)=%s\n", json_objectum_par_obtinere(r.radix, 1) ? "ptr" : "NIHIL",
        json_objectum_par_obtinere(r.radix, 0xFFFFFFFFu) ? "ptr" : "NIHIL", json_objectum_par_obtinere(arr, 0) ? "ptr" : "NIHIL");
    printf("ad_* NIHIL: b=%d i=%lld f=%g chorda.datum=%s m=%u\n", json_ad_boolean(NULL), (long long)json_ad_integer(NULL), json_ad_fluitans(NULL),
        json_ad_chorda(NULL).datum ? "ptr" : "NIHIL", (unsigned)json_ad_chorda(NULL).mensura);
    r = json_legere_literis("[5, 1.0, \"\", 1.5, -9223372036854775808]", pisc);
    printf("cross: ad_fluitans(int5)=%g ad_integer(1.0)=%lld ad_chorda(\"\").datum=%s m=%u\n",
        json_ad_fluitans(json_tabulatum_obtinere(r.radix, 0)), (long long)json_ad_integer(json_tabulatum_obtinere(r.radix, 1)),
        json_ad_chorda(json_tabulatum_obtinere(r.radix, 2)).datum ? "ptr" : "NIHIL", (unsigned)json_ad_chorda(json_tabulatum_obtinere(r.radix, 2)).mensura);
    r = json_legere_literis("{\"f\":1.5,\"g\":1.0,\"min\":-9223372036854775808,\"neg\":-1,\"n\":null}", pisc);
    printf("capere_integer: f=%lld g=%lld min=%lld neg(ord 7)=%lld n=%lld\n", (long long)json_capere_integer(r.radix, "f", 7),
        (long long)json_capere_integer(r.radix, "g", 7), (long long)json_capere_integer(r.radix, "min", 7),
        (long long)json_capere_integer(r.radix, "neg", 7), (long long)json_capere_integer(r.radix, "n", 7));
    /* iterator edges */
    it = json_objectum_iterator(NULL);
    printf("iter NIHIL: %d\n", (int)json_objectum_iterator_proxima(&it, &ck, &cv));
    it = json_objectum_iterator(arr);
    printf("iter arr: %d  iter(NIHIL iter): %d\n", (int)json_objectum_iterator_proxima(&it, &ck, &cv), (int)json_objectum_iterator_proxima(NULL, &ck, &cv));
    it = json_objectum_iterator(r.radix);
    printf("iter NIHIL outparams: %d\n", (int)json_objectum_iterator_proxima(&it, NULL, NULL));
    /* mutation during iteration + pointer stability */
    obj = json_objectum_creare(pisc);
    json_objectum_ponere(obj, "k0", json_integer_creare(pisc, 0));
    p0 = json_objectum_par_obtinere(obj, 0);
    it = json_objectum_iterator(obj); n = 0;
    while (json_objectum_iterator_proxima(&it, &ck, &cv)) {
        char b[32];
        if (n < 100) { sprintf(b, "k%d", n + 1); json_objectum_ponere(obj, b, json_integer_creare(pisc, n + 1)); }
        n++;
    }
    p0b = json_objectum_par_obtinere(obj, 0);
    printf("iter with growth visited=%d numerus=%u par0 stable=%d par0 key=%.*s\n", n, (unsigned)json_objectum_numerus(obj), p0 == p0b, (int)p0->clavis->mensura, (const char*)p0->clavis->datum);
    /* write duplicate key through par pointer */
    p0b = json_objectum_par_obtinere(obj, 1);
    p0b->clavis = p0->clavis;
    { chorda w = json_scribere(obj, pisc); r = json_legere(w, pisc); printf("dup via par write: reparse=%d [%.*s]\n", (int)r.successus, (int)r.error.mensura, r.error.datum ? (const char*)r.error.datum : ""); }
    /* creare with NIHIL piscina */
    printf("creare(NIHIL): null=%s bool=%s int=%s flt=%s ch=%s chl=%s arr=%s obj=%s chl(NIHIL)=%s\n",
        json_nullum_creare(NULL) ? "ptr" : "NIHIL", json_boolean_creare(NULL, 1) ? "ptr" : "NIHIL",
        json_integer_creare(NULL, 1) ? "ptr" : "NIHIL", json_fluitans_creare(NULL, 1) ? "ptr" : "NIHIL",
        json_chorda_creare(NULL, ck) ? "ptr" : "NIHIL", json_chorda_creare_literis(NULL, "x") ? "ptr" : "NIHIL",
        json_tabulatum_creare(NULL) ? "ptr" : "NIHIL", json_objectum_creare(NULL) ? "ptr" : "NIHIL",
        json_chorda_creare_literis(pisc, NULL) ? "ptr" : "NIHIL");
    s = json_chorda_creare_literis(pisc, "");
    printf("creare_literis(\"\"): datum=%s m=%u\n", json_ad_chorda(s).datum ? "ptr" : "NIHIL", (unsigned)json_ad_chorda(s).mensura);
    for (i = 0; i < 1; i++) {}
    return 0;
}
