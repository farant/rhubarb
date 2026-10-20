#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include "stml_macros.h"
#include <stdio.h>
#include <string.h>

static Piscina* pp;
static InternamentumChorda* it;

static void proba(const char* tag, const char* s)
{
    chorda in, w; StmlResultus r; StmlExpansioResultus e;
    in.datum = (i8*)(size_t)s; in.mensura = (i32)strlen(s);
    r = stml_legere(in, pp, it);
    printf("%s\tin=[%s]\tlegere=%d\n", tag, s, (int)r.successus);
    fflush(stdout);
    if (!r.successus) return;
    e = stml_expandere(r.radix, pp, it);
    printf("  successus=%d vitium=%d loculus=[%.*s]", (int)e.successus, (int)e.vitium,
           (int)e.loculus.mensura, e.loculus.datum ? (char*)e.loculus.datum : "");
    if (e.successus && e.radix_expansa) {
        w = stml_scribere(e.radix_expansa, pp, FALSUM);
        printf(" exitus=[%.*s]", (int)w.mensura, (char*)w.datum);
    }
    printf("\n");
    fflush(stdout);
}

int main(int argc, char** argv)
{
    const char* modus = argc > 1 ? argv[1] : "omnia";
    pp = piscina_generare_dynamicum("sonda", 65536);
    it = internamentum_creare(pp);
    if (!strcmp(modus, "omnia") || !strcmp(modus, "nonvacuus"))
        proba("TEXTUS_NONVACUUS", "<r><#@f p=\"@p\"><a>&@p;</a></#><<#@f p=\"x\">></r>");
    if (!strcmp(modus, "omnia") || !strcmp(modus, "commutatio"))
        proba("COMMUTATIO_EST_VACUUS", "<r><#@f p=\"@p\"><COMMUTATIO de=\"&@p;\"><CASUS est=\"\"><vacuus/></CASUS><ORDINARIUS><alius/></ORDINARIUS></COMMUTATIO></#><<#@f p=\"\">></r>");
    if (!strcmp(modus, "omnia") || !strcmp(modus, "commutatio2"))
        proba("COMMUTATIO_EST_A_VACUUS", "<r><#@f p=\"@p\"><COMMUTATIO de=\"&@p;\"><CASUS est=\"a\"><a/></CASUS><ORDINARIUS><alius/></ORDINARIUS></COMMUTATIO></#><<#@f p=\"\">></r>");
    if (!strcmp(modus, "omnia") || !strcmp(modus, "attr"))
        proba("ATTRIBUTUM_VACUUS", "<r><#@f p=\"@p\"><a x=\"&@p;\"/></#><<#@f p=\"\">></r>");
    if (!strcmp(modus, "omnia") || !strcmp(modus, "textus"))
        proba("TEXTUS_VACUUS", "<r><#@f p=\"@p\"><a>&@p;</a></#><<#@f p=\"\">></r>");
    return 0;
}
