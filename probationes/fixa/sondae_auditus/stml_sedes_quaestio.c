#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "xar.h"
#include "stml.h"
#include <stdio.h>
#include <string.h>

static Piscina* pp;
static InternamentumChorda* it;
static int discordiae;

/* collect parse extents of elements in post-order */
static void post(StmlNodus* n, Xar* acc)
{
    i32 i, k = stml_numerus_liberorum(n);
    for (i = 0; i < k; i++) post(stml_liberum_ad_indicem(n, i), acc);
    if (n->genus == STML_NODUS_ELEMENTUM) { StmlNodus** s = (StmlNodus**)xar_addere(acc); *s = n; }
}

static void paritas(const char* tag, const char* s, b32 pul)
{
    chorda in, w, w0; StmlResultus r, rr; Xar* sedes; Xar* acc; i32 i, n, m;
    in.datum = (i8*)(size_t)s; in.mensura = (i32)strlen(s);
    r = stml_legere(in, pp, it);
    sedes = xar_creare(pp, (i32)sizeof(StmlSedesNodi));
    w = stml_scribere_sedibus(r.radix, pp, pul, sedes);
    w0 = stml_scribere(r.radix, pp, pul);
    rr = stml_legere(w, pp, it);
    acc = xar_creare(pp, (i32)sizeof(StmlNodus*));
    post(rr.radix, acc);
    n = xar_numerus(sedes); m = xar_numerus(acc);
    discordiae = 0;
    for (i = 0; i < n && i < m; i++)
    {
        StmlSedesNodi* sd = (StmlSedesNodi*)xar_obtinere(sedes, i);
        StmlNodus* q = *(StmlNodus**)xar_obtinere(acc, i);
        if (sd->initium != q->positus_initium || sd->finis != q->positus_finis || w.datum[sd->initium] != '<') { discordiae++;
            printf("  %s DISCORDIA i=%u sedes=[%u,%u) relectum=[%u,%u) titulus=%.*s\n", tag, i, sd->initium, sd->finis, q->positus_initium, q->positus_finis, q->titulus ? (int)q->titulus->mensura : 0, q->titulus ? (char*)q->titulus->datum : ""); }
    }
    printf("%s\tpul=%d\tbytes_idem=%d\tsedes=%u relecta_elementa=%u\tdiscordiae=%d\tscriptum=[%.*s]\n", tag, (int)pul,
        (int)(w.mensura == w0.mensura && memcmp(w.datum, w0.datum, w.mensura) == 0), n, m, discordiae, (int)w.mensura, (char*)w.datum);
}

int main(void)
{
    StmlResultus r; Xar* pre; chorda w; StmlNodus* q; StmlNodus* a;
    pp = piscina_generare_dynamicum("sonda", 1 << 18);
    it = internamentum_creare(pp);
    paritas("SIMPLEX", "<r><a k=\"v\">t</a><b/></r>", FALSUM);
    paritas("CAPTURAE", "<r><a(> <b(> x\n<c/><) d><= f =><e/></r>", FALSUM);
    paritas("CAPTURAE_P", "<r><a(> <b(> x\n<c/><) d><= f =><e/></r>", VERUM);
    paritas("CRUDUS_CAPT", "<r><v! (>linea\n<z/></r>", FALSUM);
    paritas("CRUDUS_CAPT_P", "<r><v! (>linea\n<z/></r>", VERUM);
    paritas("SPINA_P", "<r><a><b><c>texto</c></b></a></r>", VERUM);
    paritas("FRAG_P", "<r><#f (> <b>x</b><% &k;>q</%></r>", VERUM);
    paritas("MULTI_P", "<r><a><b/><c/></a></r>", VERUM);
    paritas("SPINA_PRIMA_TEXTUS_P", "<a>x</a>", VERUM);
    /* pre-populated sedes */
    r = stml_legere_ex_literis("<r><a/></r>", pp, it);
    pre = xar_creare(pp, (i32)sizeof(StmlSedesNodi));
    { StmlSedesNodi* s0 = (StmlSedesNodi*)xar_addere(pre); s0->nodus = NIHIL; s0->initium = 99; s0->finis = 100; }
    w = stml_scribere_sedibus(r.radix, pp, VERUM, pre);
    printf("PRAEPOPULATA\tn=%u\tprimum=%u\n", xar_numerus(pre), ((StmlSedesNodi*)xar_obtinere(pre, 0))->initium);
    (void)w;
    /* queries with synthetic titles */
    r = stml_legere_ex_literis("<r><#f>x</#><% &k;>y</%><@m=>v</><.sp/><b/></r>", pp, it);
    printf("INVENIRE\t#=%p\t%%=%p\t@=%p\t.sp=%p\tsp=%p\tvacuum=%p\n",
        (void*)stml_invenire_liberum(r.elementum_radix, "#"), (void*)stml_invenire_liberum(r.elementum_radix, "%"),
        (void*)stml_invenire_liberum(r.elementum_radix, "@"), (void*)stml_invenire_liberum(r.elementum_radix, ".sp"),
        (void*)stml_invenire_liberum(r.elementum_radix, "sp"), (void*)stml_invenire_liberum(r.elementum_radix, ""));
    printf("CAPERE_r_m\t%p\tstatus=%d\n", (void*)stml_attributum_capere(r.elementum_radix, "m"), (int)r.status);
    /* stale parent */
    r = stml_legere_ex_literis("<r><a/><b/></r>", pp, it);
    a = stml_invenire_liberum(r.elementum_radix, "a");
    q = stml_duplicare(r.elementum_radix, pp, it);
    a->parens = q;  /* stale: not in q's liberi (clone has new children) */
    printf("PARENS_STALUS\tidx=%d\tprox=%p\tprior=%p\tfratres=%u\n", (int)stml_index_inter_fratres(a), (void*)stml_frater_proximus(a), (void*)stml_frater_prior(a), xar_numerus(stml_fratres(a, pp)));
    /* textus_internus entities */
    r = stml_legere_ex_literis("<a>x&amp;y<!--c--></a>", pp, it);
    { chorda t = stml_textus_internus(r.elementum_radix, pp); printf("INTERNUS_ENTIA\t[%.*s]\texpect_header=[x&amp;y]\n", (int)t.mensura, (char*)t.datum); }
    r = stml_legere_ex_literis("<a>x<<sel>>y</a>", pp, it);
    { chorda t = stml_textus_valor(r.elementum_radix, pp); chorda t2 = stml_textus_internus(r.elementum_radix, pp); printf("TRANSCLUSIO_VALOR\tvalor=[%.*s]\tinternus=[%.*s]\n", (int)t.mensura, (char*)t.datum, (int)t2.mensura, (char*)t2.datum); }
    return 0;
}
