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

static vacuum scr(constans character* tag, StmlNodus* n)
{
    chorda w = stml_scribere(n, pp, FALSUM);
    printf("%s\tscriptum=[%.*s]\tattributa=%d\n", tag, (int)w.mensura, (char*)w.datum,
        n->attributa ? (int)xar_numerus(n->attributa) : 0);
}

int main(void)
{
    StmlResultus r; StmlNodus* a; chorda w; b32 ret;
    pp = piscina_generare_dynamicum("sonda", 65536);
    it = internamentum_creare(pp);

    /* Pars A: labels="" parsed, then addere */
    r = stml_legere_ex_literis("<a labels=\"\"/>", pp, it);
    a = r.elementum_radix;
    scr("A_PARSED", a);
    ret = stml_titulum_addere(a, pp, it, "foo");
    printf("A_ADDERE ret=%d habet(foo)=%d numerus=%d\n", (int)ret, (int)stml_titulum_habet(a, "foo"), (int)stml_titulos_numerus(a));
    scr("A_POST", a);
    ret = stml_titulum_addere(a, pp, it, "foo");
    printf("A_ADDERE_ITERUM ret=%d\n", (int)ret);
    scr("A_POST2", a);

    /* Pars B: remove last label, cycle write/read twice */
    r = stml_legere_ex_literis("<a labels=\"x\"/>", pp, it);
    a = r.elementum_radix;
    ret = stml_titulum_removere(a, pp, it, "x");
    printf("B_REMOVERE ret=%d\n", (int)ret);
    w = stml_scribere(a, pp, FALSUM);
    printf("B_W1=[%.*s]\n", (int)w.mensura, (char*)w.datum);
    r = stml_legere(w, pp, it);
    w = stml_scribere(r.radix, pp, FALSUM);
    printf("B_W2=[%.*s]\n", (int)w.mensura, (char*)w.datum);
    r = stml_legere(w, pp, it);
    printf("B_R3 habet(true)=%d numerus=%d\n", (int)stml_titulum_habet(r.elementum_radix, "true"), (int)stml_titulos_numerus(r.elementum_radix));
    redde 0;
}
