/* sonda: writer edges - escapes, NUL in keys, aliasing, corrupt genus, deep built trees, pretty indentatio */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Piscina* pisc;

static void mon_(const char* t, chorda w)
{
    JsonResultus r = json_legere(w, pisc);
    printf("%-16s mensura=%u out=", t, (unsigned)w.mensura);
    fwrite(w.datum ? (const void*)w.datum : (const void*)"", 1, w.mensura > 200 ? 200 : w.mensura, stdout);
    printf("%s  reparse=%d%s%.*s\n", w.mensura > 200 ? "..." : "", (int)r.successus, r.successus ? "" : " err=",
        r.successus ? 0 : (int)r.error.mensura, r.successus ? "" : (const char*)r.error.datum);
}

int main(void)
{
    JsonValor *v, *arr, *obj, *cur, *nx; chorda c; char* buf; int i;
    ChordaAedificator* aed;
    pisc = piscina_generare_dynamicum("sonda", 65536);
    c.datum = (i8*)malloc(8); memcpy(c.datum, "a\0b\x7f/\x80\xff\"", 8); c.mensura = 8;
    mon_("bytes", json_scribere(json_chorda_creare(pisc, c), pisc));
    obj = json_objectum_creare(pisc);
    json_objectum_ponere_chorda(obj, c, json_integer_creare(pisc, 1));
    mon_("bytes_key", json_scribere(obj, pisc));
    /* aliasing: mutate caller buffer after creare */
    buf = (char*)malloc(4); memcpy(buf, "abcd", 4);
    c.datum = (i8*)buf; c.mensura = 4;
    v = json_chorda_creare(pisc, c);
    buf[0] = 'Z';
    mon_("alias_after_mut", json_scribere(v, pisc));
    free(buf);
    /* corrupt genus inside array */
    arr = json_tabulatum_creare(pisc);
    json_tabulatum_addere(arr, json_integer_creare(pisc, 1));
    v = json_integer_creare(pisc, 2); v->genus = (JsonGenus)99;
    json_tabulatum_addere(arr, v);
    mon_("corrupt_genus", json_scribere(arr, pisc));
    printf("json_genus(corrupt)=%d est_*=%d%d%d%d%d%d%d\n", (int)json_genus(v), json_est_nullum(v), json_est_boolean(v),
        json_est_integer(v), json_est_fluitans(v), json_est_chorda(v), json_est_tabulatum(v), json_est_objectum(v));
    /* NIHIL root */
    mon_("nihil_root", json_scribere(NULL, pisc));
    mon_("nihil_root_pul", json_scribere_pulchrum(NULL, pisc));
    /* built depth 300 */
    cur = arr = json_tabulatum_creare(pisc);
    for (i = 1; i < 300; i++) { nx = json_tabulatum_creare(pisc); json_tabulatum_addere(cur, nx); cur = nx; }
    mon_("built_depth300", json_scribere(arr, pisc));
    /* shared subtree DAG */
    v = json_chorda_creare_literis(pisc, "x");
    arr = json_tabulatum_creare(pisc); json_tabulatum_addere(arr, v); json_tabulatum_addere(arr, v);
    mon_("dag", json_scribere(arr, pisc));
    /* addere into wrong genus, ponere into array */
    obj = json_objectum_creare(pisc);
    json_tabulatum_addere(obj, v);
    json_objectum_ponere(arr, "k", v);
    json_objectum_ponere(obj, NULL, v);
    mon_("wrong_genus_obj", json_scribere(obj, pisc));
    mon_("wrong_genus_arr", json_scribere(arr, pisc));
    /* ponere NIHIL valor, then capere/habet */
    json_objectum_ponere(obj, "nil", NULL);
    json_objectum_ponere(obj, "jnull", json_nullum_creare(pisc));
    printf("nil: capere=%s habet=%d  jnull: capere=%s habet=%d est_nullum=%d  absent: capere=%s habet=%d  genus(capere(nil))=%d genus(capere(absent))=%d\n",
        json_objectum_capere(obj, "nil") ? "ptr" : "NIHIL", json_objectum_habet(obj, "nil"),
        json_objectum_capere(obj, "jnull") ? "ptr" : "NIHIL", json_objectum_habet(obj, "jnull"),
        json_est_nullum(json_objectum_capere(obj, "jnull")),
        json_objectum_capere(obj, "zz") ? "ptr" : "NIHIL", json_objectum_habet(obj, "zz"),
        (int)json_genus(json_objectum_capere(obj, "nil")), (int)json_genus(json_objectum_capere(obj, "zz")));
    printf("json_genus(NIHIL)=%d (JSON_NULLUM=%d) json_est_nullum(NIHIL)=%d\n", (int)json_genus(NULL), (int)JSON_NULLUM, (int)json_est_nullum(NULL));
    mon_("nil_member", json_scribere(obj, pisc));
    /* pretty: indentatio meaning */
    obj = json_objectum_creare(pisc);
    json_objectum_ponere(obj, "a", json_integer_creare(pisc, 1));
    aed = chorda_aedificator_creare(pisc, 64);
    chorda_aedificator_appendere_literis(aed, "PRE:");
    json_scribere_ad_aedificator_pulchrum(obj, aed, 2);
    c = chorda_aedificator_finire(aed);
    printf("pulchrum_ind2=[%.*s]\n", (int)c.mensura, (const char*)c.datum);
    mon_("pul_scalar", json_scribere_pulchrum(json_integer_creare(pisc, 3), pisc));
    arr = json_tabulatum_creare(pisc); json_tabulatum_addere(arr, json_objectum_creare(pisc)); json_tabulatum_addere(arr, json_tabulatum_creare(pisc));
    c = json_scribere_pulchrum(arr, pisc);
    printf("pul_empties=[%.*s]\n", (int)c.mensura, (const char*)c.datum);
    /* json_scribere_ad_aedificator(NIHIL aed), pulchrum with indentatio wrap: closing level */
    json_scribere_ad_aedificator(obj, NULL);
    printf("ad_aedificator NIHIL aed: ok\n");
    return 0;
}
