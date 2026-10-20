#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include <stdio.h>
#include <string.h>

static Piscina* pp;
static InternamentumChorda* it;

static vacuum ostendere(constans character* tag, StmlNodus* a)
{
    chorda* v = stml_attributum_capere(a, "v");
    printf("%s\thabet=%d\tcapere=%s mensura=%d\n", tag, (int)stml_attributum_habet(a, "v"),
        v ? "non-NIHIL" : "NIHIL", v ? (int)v->mensura : -1);
}

int main(void)
{
    StmlResultus r, r2; chorda w, w2; StmlNodus* a; StmlNodus* e;
    pp = piscina_generare_dynamicum("sonda", 65536);
    it = internamentum_creare(pp);
    r = stml_legere_ex_literis("<a v=\"\" w=\"x\"/>", pp, it);
    w = stml_scribere(r.radix, pp, FALSUM);
    printf("PARSED status=%d scriptum=[%.*s]\n", (int)r.status, (int)w.mensura, (char*)w.datum);
    ostendere("PARSED", r.elementum_radix);
    r2 = stml_legere(w, pp, it);
    w2 = stml_scribere(r2.radix, pp, FALSUM);
    printf("RELECTUM status=%d scriptum=[%.*s]\n", (int)r2.status, (int)w2.mensura, (char*)w2.datum);
    a = r2.elementum_radix;
    {
        chorda* v = stml_attributum_capere(a, "v");
        printf("RELECTUM v=[%.*s]\n", v ? (int)v->mensura : 4, v ? (char*)v->datum : "NIHIL");
    }
    e = stml_elementum_creare(pp, it, "a");
    printf("ADDERE ret=%d\n", (int)stml_attributum_addere(e, pp, it, "v", ""));
    ostendere("ADDERE", e);
    w = stml_scribere(e, pp, FALSUM);
    printf("ADDERE scriptum=[%.*s]\n", (int)w.mensura, (char*)w.datum);
    redde 0;
}
