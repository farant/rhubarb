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

static void st(const char* tag, const char* s)
{
    chorda in; StmlResultus r; Xar* v; i32 i, n;
    in.datum = (i8*)(size_t)s; in.mensura = (i32)strlen(s);
    r = stml_legere(in, pp, it);
    v = stml_strictum(r.radix, pp);
    n = xar_numerus(v);
    printf("%s\tin=[%s]\tstatus=%d\tvitia=%u\t", tag, s, (int)r.status, n);
    per (i = 0; i < n; i++) { StmlStrictumVitium* q = (StmlStrictumVitium*)xar_obtinere(v, i); printf("%d ", (int)q->genus); }
    printf("\n");
}

int main(void)
{
    pp = piscina_generare_dynamicum("sonda", 65536);
    it = internamentum_creare(pp);
    st("RETRO", "<r><a/><) b></r>");
    st("FARCIMEN", "<r><a/><= s =><b/></r>");
    st("FRAG_NUDUM", "<r><>x</></r>");
    st("DUP_INLINE", "<r><a n=\"1\" n=\"2\" n=\"3\"/></r>");
    st("ALBUS_EXTRA", "  \n<r/>\n  ");
    st("TEXTUS_EXTRA_LINEA", "x <r/>");
    st("TITULUS_VACUUS", "<r>< >x</></r>");
    st("PLURES_CUM_COMM", "<r/><!--c--><s/>");
    st("VACUUM_SPATIUM", "   ");
    st("CAPT_NUDA", "<r><(> <a/></r>");
    printf("NUNTIUS_0\t[%s]\n", stml_strictum_nuntius((StmlStrictumGenus)0));
    printf("NUNTIUS_99\t[%s]\n", stml_strictum_nuntius((StmlStrictumGenus)99));
    printf("STRICTUM_NIHIL\t%p\n", (void*)stml_strictum(NIHIL, pp));
    {
        StmlNodus* e = stml_elementum_creare(pp, it, "a");
        stml_attributum_addere(e, pp, it, "k", "1");
        stml_attributum_addere(e, pp, it, "k", "2");
        printf("ADDERE_DUPLEX\tcapere=[%.*s]\tvitia=%u\n", (int)stml_attributum_capere(e, "k")->mensura, (char*)stml_attributum_capere(e, "k")->datum, xar_numerus(stml_strictum(e, pp)));
    }
    return 0;
}
