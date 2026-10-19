/* sonda: scriptor - textus non XML, reales, integri, dies, data */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "plist.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>

static Piscina* p;
static InternamentumChorda* intern;

static void proba(const char* t, PlistValor* v)
{
    PlistStatus st = PLIST_SUCCESSUS;
    chorda out = plist_scribere(v, &st, p);
    char via[256]; FILE* f;
    PlistResultus r;
    sprintf(via, "build/sonda/plist_rt/w_%s.plist", t);
    f = fopen(via, "wb"); if (f) { fwrite(out.datum, 1, out.mensura, f); fclose(f); }
    printf("%s\tscribere=%d\tmensura=%u\t", t, (int)st, (unsigned)out.mensura);
    if (st == PLIST_SUCCESSUS) {
        r = plist_legere(out, p, intern);
        printf("legere=%d\taequalis=%d", (int)r.status, r.radix ? (int)plist_aequalis(v, r.radix) : -1);
        if (r.radix && r.radix->genus == PLIST_DICTIO) {
            PlistValor* x = plist_dictio_capere(r.radix, "v");
            if (x && x->genus == PLIST_REALIS) printf("\treal_lectum=%.17g", x->realis);
            if (x && x->genus == PLIST_INTEGER) printf("\tint_lectum=%lld", (long long)x->integrum);
        }
    }
    printf("\n");
}

static PlistValor* involve(PlistValor* v)
{
    PlistValor* d = plist_dictio_creare(p);
    plist_dictio_ponere(d, chorda_ex_literis("v", p), v, p);
    return d;
}

static PlistValor* str_n(const char* s, unsigned n)
{
    chorda c; c.datum = (i8*)piscina_allocare(p, n ? n : 1); memcpy(c.datum, s, n); c.mensura = n;
    return involve(plist_chordam_creare(c, p));
}

int main(void)
{
    DiesHora dh;
    p = piscina_generare_dynamicum("sonda", 65536);
    intern = internamentum_creare(p);

    proba("utf8_malum_ff", str_n("a\xff" "b", 3));
    proba("utf8_trunc", str_n("a\xc3", 2));
    proba("utf8_overlong", str_n("\xc0\xaf", 2));
    proba("utf8_surrogate", str_n("\xed\xa0\x80", 3));
    proba("u_fffe", str_n("\xef\xbf\xbe", 3));
    proba("u_ffff", str_n("\xef\xbf\xbf", 3));
    proba("del_7f", str_n("a\x7f" "b", 3));
    proba("c1_85", str_n("\xc2\x85", 2));
    proba("nul_val", str_n("a\0b", 3));
    {   /* NUL in clave */
        PlistValor* d = plist_dictio_creare(p); chorda k; k.datum = (i8*)"k\0z"; k.mensura = 3;
        plist_dictio_ponere(d, k, plist_integrum_creare(1, p), p); proba("nul_clavis", d); }
    {   /* C0 profunde in clave */
        PlistValor* d = plist_dictio_creare(p); PlistValor* l = plist_lista_creare(p); PlistValor* d2 = plist_dictio_creare(p);
        plist_dictio_ponere(d2, chorda_ex_literis("a\x1b", p), plist_integrum_creare(1, p), p);
        plist_lista_addere(l, d2, p); plist_dictio_ponere(d, chorda_ex_literis("x", p), l, p); proba("c0_clavis_profunda", d); }
    {   PlistValor* d = plist_dictio_creare(p);
        plist_dictio_ponere(d, chorda_ex_literis("a\nb\tc\rd", p), plist_integrum_creare(1, p), p); proba("clavis_lf_tab_cr", d); }

    proba("real_nan", involve(plist_realem_creare(NAN, p)));
    proba("real_inf", involve(plist_realem_creare(INFINITY, p)));
    proba("real_ninf", involve(plist_realem_creare(-INFINITY, p)));
    proba("real_negzero", involve(plist_realem_creare(-0.0, p)));
    proba("real_0.1", involve(plist_realem_creare(0.1, p)));
    proba("real_1e-310", involve(plist_realem_creare(1e-310, p)));
    proba("real_dblmax", involve(plist_realem_creare(DBL_MAX, p)));
    proba("real_dblmin_sub", involve(plist_realem_creare(4.9406564584124654e-324, p)));
    proba("real_1e22", involve(plist_realem_creare(1e22, p)));
    proba("int_s64min", involve(plist_integrum_creare((s64)(-9223372036854775807LL - 1), p)));
    proba("int_s64max", involve(plist_integrum_creare((s64)9223372036854775807LL, p)));
    proba("verum_2", involve(plist_veritatem_creare(2, p)));
    proba("data_vacua", involve(plist_data_creare(chorda_ex_literis("", p), p)));
    { chorda n; n.datum = NIHIL; n.mensura = 0; proba("data_nihil", involve(plist_data_creare(n, p)));
      proba("chorda_nihil", involve(plist_chordam_creare(n, p))); }
    proba("data_1", involve(plist_data_creare(chorda_ex_literis("\xff", p), p)));

    memset(&dh, 0, sizeof dh);
    dh.dies.annus = 2026; dh.dies.mensis = 13; dh.dies.dies = 1; proba("dies_mensis13", involve(plist_diem_creare(dh, p)));
    dh.dies.annus = 10000; dh.dies.mensis = 1; proba("dies_annus10000", involve(plist_diem_creare(dh, p)));
    dh.dies.annus = -5; proba("dies_annus_neg", involve(plist_diem_creare(dh, p)));
    dh.dies.annus = 999; proba("dies_annus999", involve(plist_diem_creare(dh, p)));
    dh.dies.annus = 0; proba("dies_annus0", involve(plist_diem_creare(dh, p)));
    dh.dies.annus = 2026; dh.hora.secundum = 60; proba("dies_sec60", involve(plist_diem_creare(dh, p)));
    {   PlistValor* v = plist_integrum_creare(1, p); v->genus = (PlistGenus)99; proba("genus99", involve(v)); }
    {   PlistValor* d = plist_dictio_creare(p); PlistPar* par;
        plist_dictio_ponere(d, chorda_ex_literis("k", p), plist_integrum_creare(1, p), p);
        par = (PlistPar*)xar_obtinere(d->liberi, 0); par->valor = NIHIL; proba("valor_nihil", d); }
    {   PlistStatus st = PLIST_SUCCESSUS; chorda o = plist_scribere(NIHIL, &st, p);
        printf("radix_nihil\tscribere=%d\tmensura=%u\n", (int)st, (unsigned)o.mensura);
        o = plist_scribere(involve(plist_integrum_creare(1, p)), NIHIL, p);
        printf("status_nihil\tmensura=%u\n", (unsigned)o.mensura); }
    return 0;
}
