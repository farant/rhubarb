#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include <stdio.h>
#include <string.h>

static Piscina* pp;
static InternamentumChorda* it;

static void sig(StmlNodus* n)
{
    i32 i, k, a;
    if (n->attributa) for (a = 0; a < xar_numerus(n->attributa); a++) { StmlAttributum* at = (StmlAttributum*)xar_obtinere(n->attributa, a); printf("@%.*s=%.*s ", (int)at->titulus->mensura, (char*)at->titulus->datum, at->valor ? (int)at->valor->mensura : 0, at->valor ? (char*)at->valor->datum : ""); }
    k = stml_numerus_liberorum(n);
    printf("{");
    per (i = 0; i < k; i++)
    {
        StmlNodus* l = stml_liberum_ad_indicem(n, i);
        if (l->genus == STML_NODUS_ELEMENTUM) { printf("E:%.*s ", l->titulus ? (int)l->titulus->mensura : 0, l->titulus ? (char*)l->titulus->datum : ""); sig(l); }
        else printf("%d:[%.*s]", (int)l->genus, l->valor ? (int)l->valor->mensura : 0, l->valor ? (char*)l->valor->datum : "");
        printf(" ");
    }
    printf("}");
}

static StmlResultus mon(const char* tag, const char* s)
{
    chorda in, w; StmlResultus r;
    in.datum = (i8*)(size_t)s; in.mensura = (i32)strlen(s);
    r = stml_legere(in, pp, it);
    w = stml_scribere(r.radix, pp, FALSUM);
    printf("%s\tin=[%s]\tstatus=%d\tscriptum=[%.*s]\tarbor=", tag, s, (int)r.status, (int)w.mensura, (char*)w.datum);
    if (r.radix) sig(r.radix);
    printf("\n");
    return r;
}

int main(void)
{
    StmlResultus r; chorda* v;
    pp = piscina_generare_dynamicum("sonda", 65536);
    it = internamentum_creare(pp);
    mon("NUDUS_PUNCTUM", "<a href=x.y>t</a>");
    mon("NUDUS_URL", "<a href=http://h/p>t</a>");
    mon("NUDUS_VACUUS", "<a b= c=\"d\"/>");
    mon("QUOTA_NON_CLAUSA", "<a b=\"x>t</a>");
    mon("CLAUSURA_SORDIDA", "<r><a>t</a x></r>");
    mon("CLAUSURA_SINE_GT", "<r><a>t</a</r>");
    mon("CLAUSURA_EOF", "<a>t</a");
    mon("FARCIMEN_SINE", "<r>x <= s y</r>");
    mon("RETRO_SINE_GT", "<r><b/><) z</r>");
    mon("CAPT_NULLUS", "<r><a (></r>");
    mon("CAPT_PAUCI", "<r><a ((><b/></r>");
    mon("RETRO_PRIMUS", "<r><) a></r>");
    mon("FARCIMEN_SOLUS", "<r><= a =></r>");
    mon("CRUDUS_CAPT_EOF", "<r><t! (>abc");
    mon("CRUDUS_MULTI", "<r><t! ((>abc\n<b/></r>");
    mon("ATTR_INTRA_CLAUSURAM", "<a>t</a b=\"c\">");
    mon("TAG_NON_CLAUSUS_ATTR", "<r><a b=\"c\"</r>");
    r = mon("ENTITAS_ATTR_VS_ELEM", "<r><a n=\"x&amp;y\"/><b><@n=>x&amp;y</></b></r>");
    {
        StmlNodus* a = stml_invenire_liberum(r.elementum_radix, "a");
        StmlNodus* b = stml_invenire_liberum(r.elementum_radix, "b");
        chorda* va = stml_attributum_capere(a, "n");
        chorda* vb = stml_attributum_capere(b, "n");
        printf("ENTITAS_CAPERE\tinline=[%.*s]\telementum=[%.*s]\n", (int)va->mensura, (char*)va->datum, (int)vb->mensura, (char*)vb->datum);
    }
    r = mon("SEPULCRUM", "<r><a><@m=/></a></r>");
    printf("SEPULCRUM_HABET\thabet=%d\n", (int)stml_attributum_habet(stml_invenire_liberum(r.elementum_radix, "a"), "m"));
    r = mon("ATTR_ELEM_POST", "<r><a><b/><@m=>v</></a></r>");
    r = mon("ATTR_ELEM_MULTI_TEXT", "<r><a><@m=>v<!--c-->w</></a></r>");
    v = stml_attributum_capere(stml_invenire_liberum(r.elementum_radix, "a"), "m");
    printf("ATTR_ELEM_MULTI_TEXT_CAPERE\t[%.*s]\n", v ? (int)v->mensura : 0, v ? (char*)v->datum : "NIHIL");
    return 0;
}
