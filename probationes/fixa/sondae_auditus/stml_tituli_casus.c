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

static StmlNodus* leg(const char* s)
{
    chorda in; StmlResultus r;
    in.datum = (i8*)(size_t)s; in.mensura = (i32)strlen(s);
    r = stml_legere(in, pp, it);
    return r.elementum_radix;
}

static void omnia(const char* tag, StmlNodus* e)
{
    Xar* x = stml_titulos_capere(e, pp);
    chorda w = stml_scribere(e, pp, FALSUM);
    printf("%s\tnumerus=%u\tcapere_n=%u\tscriptum=[%.*s]\n", tag, stml_titulos_numerus(e), xar_numerus(x), (int)w.mensura, (char*)w.datum);
}

int main(void)
{
    StmlNodus* e; b32 r1;
    pp = piscina_generare_dynamicum("sonda", 65536);
    it = internamentum_creare(pp);

    e = leg("<a labels=\"x\ty\"/>");
    omnia("TAB_SEP", e);
    printf("TAB_SEP\thabet_x=%d\thabet_y=%d\n", (int)stml_titulum_habet(e, "x"), (int)stml_titulum_habet(e, "y"));
    r1 = stml_titulum_removere(e, pp, it, "x");
    printf("TAB_SEP_REMOVERE_x\tret=%d\thabet_x_post=%d\n", (int)r1, (int)stml_titulum_habet(e, "x"));

    e = leg("<a labels=\"x\ny\"/>");
    printf("NEWLINE_SEP\tnumerus=%u\thabet_y=%d\n", stml_titulos_numerus(e), (int)stml_titulum_habet(e, "y"));

    e = leg("<a labels=\"\" class=\"foo\"/>");
    printf("LABELS_VACUUM_CLASS\thabet_foo=%d\tnumerus=%u\n", (int)stml_titulum_habet(e, "foo"), stml_titulos_numerus(e));

    e = leg("<a/>");
    r1 = stml_titulum_commutare(e, pp, it, "p q");
    printf("COMMUTARE_SPATIUM\tret=%d\thabet_post=%d\tnumerus=%u\n", (int)r1, (int)stml_titulum_habet(e, "p q"), stml_titulos_numerus(e));

    e = leg("<a/>");
    r1 = stml_titulum_addere(e, pp, it, "");
    printf("ADDERE_VACUUM\tret=%d\thabet_post=%d\n", (int)r1, (int)stml_titulum_habet(e, ""));

    e = leg("<a><@labels=>foo</></a>");
    printf("ATTR_ELEM_LABELS\thabet_foo=%d\n", (int)stml_titulum_habet(e, "foo"));
    r1 = stml_titulum_addere(e, pp, it, "bar");
    printf("ATTR_ELEM_ADDERE_bar\tret=%d\thabet_bar=%d\n", (int)r1, (int)stml_titulum_habet(e, "bar"));

    e = leg("<a labels=\"x x\"/>");
    r1 = stml_titulum_removere(e, pp, it, "x");
    printf("DUPLEX_REMOVERE\tret=%d\thabet_post=%d\n", (int)r1, (int)stml_titulum_habet(e, "x"));

    e = leg("<a labels=\"x\"/>");
    stml_titulum_removere(e, pp, it, "x");
    printf("REMOVERE_ULTIMUM\tinternata=%d\n", (int)chorda_est_internata(it, *stml_attributum_capere(e, "labels")));
    return 0;
}
