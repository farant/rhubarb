#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include <stdio.h>
#include <string.h>

static Piscina* pp;
static InternamentumChorda* it;

static void par(const char* tag, const char* s)
{
    chorda in, a, b, pa, pb; StmlResultus r; StmlNodus* d;
    in.datum = (i8*)(size_t)s; in.mensura = (i32)strlen(s);
    r = stml_legere(in, pp, it);
    d = stml_duplicare(r.radix, pp, it);
    a = stml_scribere(r.radix, pp, FALSUM);
    b = stml_scribere(d, pp, FALSUM);
    pa = stml_scribere(r.radix, pp, VERUM);
    pb = stml_scribere(d, pp, VERUM);
    printf("%s\tstatus=%d\tdup_fidelis=%d\tdup_pulcher=%d", tag, (int)r.status,
        (int)(a.mensura == b.mensura && memcmp(a.datum, b.datum, a.mensura) == 0),
        (int)(pa.mensura == pb.mensura && memcmp(pa.datum, pb.datum, pa.mensura) == 0));
    if (!(a.mensura == b.mensura && memcmp(a.datum, b.datum, a.mensura) == 0))
        printf("\torig=[%.*s]\tdup=[%.*s]", (int)a.mensura, (char*)a.datum, (int)b.mensura, (char*)b.datum);
    if (!(pa.mensura == pb.mensura && memcmp(pa.datum, pb.datum, pa.mensura) == 0))
        printf("\tP_orig=[%.*s]\tP_dup=[%.*s]", (int)pa.mensura, (char*)pa.datum, (int)pb.mensura, (char*)pb.datum);
    printf("\n");
}

int main(void)
{
    StmlNodus* e; StmlNodus* d; StmlNodus* s;
    pp = piscina_generare_dynamicum("sonda", 65536);
    it = internamentum_creare(pp);

    /* clone of a freshly created element: can we still mutate it? */
    e = stml_elementum_creare(pp, it, "a");
    d = stml_duplicare(e, pp, it);
    printf("CLONUS_VACUUS\torig_attr=%p orig_liberi=%p\tdup_attr=%p dup_liberi=%p\n", (void*)e->attributa, (void*)e->liberi, (void*)d->attributa, (void*)d->liberi);
    printf("CLONUS_ADDERE\torig_attr_add=%d\tdup_attr_add=%d\torig_liberum_add=%d\tdup_liberum_add=%d\tdup_textum_add=%d\tdup_titulum_add=%d\n",
        (int)stml_attributum_addere(e, pp, it, "k", "v"),
        (int)stml_attributum_addere(d, pp, it, "k", "v"),
        (int)stml_liberum_addere(e, stml_elementum_creare(pp, it, "x")),
        (int)stml_liberum_addere(d, stml_elementum_creare(pp, it, "x")),
        (int)stml_textum_addere(d, pp, it, "t"),
        (int)stml_titulum_addere(d, pp, it, "lab"));
    s = stml_duplicare_superficialiter(e, pp, it);
    printf("SUPERFICIALIS_ADDERE\tliberum_add=%d\n", (int)stml_liberum_addere(s, stml_elementum_creare(pp, it, "y")));

    par("FRAG", "<r><#f k=\"1\">x</#></r>");
    par("FRAG_CAPT", "<r><#f (> <b/></r>");
    par("AUG", "<r><% &k;>x</%></r>");
    par("ATTR_ELEM", "<r><a><@m=>v</></a></r>");
    par("CRUDUS", "<r><c!>a<\\/c>b</c></r>");
    par("MULTILINEA", "<r>\n  <m\\>\n    one\n      two\n  </m>\n</r>");
    par("CRUDUS_CAPT", "<r><v! (>linea cruda\n<z/></r>");
    par("CAPT_RETRO", "<r><a/><) b></r>");
    par("FARCIMEN", "<r><a/><= s =><b/></r>");
    par("ANONYMA", "<r><a>x</></r>");
    par("TRIVIA", "<r  >\n\n  <a   k=\"v\"  />\n</r>\n");
    par("VACUUM_PAR", "<r><a></a></r>");
    par("CLAUSURAE", "<r><a>\n</a></r>");
    return 0;
}
