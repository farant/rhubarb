#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include <stdio.h>
#include <string.h>

static Piscina* pp;
static InternamentumChorda* it;

static void esc(chorda c) { i32 i; for (i = 0; i < c.mensura; i++) { char ch = (char)c.datum[i]; if (ch == '\n') printf("\\n"); else if (ch == '\t') printf("\\t"); else putchar(ch); } }

static void mon(const char* tag, const char* s)
{
    chorda in, w, p1, p2, v0, v1; StmlResultus r, r1, r2;
    in.datum = (i8*)(size_t)s; in.mensura = (i32)strlen(s);
    r = stml_legere(in, pp, it);
    w = stml_scribere(r.radix, pp, FALSUM);
    p1 = stml_scribere(r.radix, pp, VERUM);
    r1 = stml_legere(p1, pp, it);
    p2 = stml_scribere(r1.radix, pp, VERUM);
    r2 = stml_legere(p2, pp, it);
    v0 = stml_textus_valor(r.radix, pp); v1 = stml_textus_valor(r1.radix, pp);
    printf("%s\tstatus=%d/%d/%d\terr=[%.*s]\tfidelis=%d\tP_fixum=%d\tvalor_idem=%d\tP1=[", tag, (int)r.status, (int)r1.status, (int)r2.status,
        (int)r.error.mensura, r.error.datum ? (char*)r.error.datum : "",
        (int)(w.mensura == in.mensura && memcmp(w.datum, in.datum, in.mensura) == 0),
        (int)(p1.mensura == p2.mensura && memcmp(p1.datum, p2.datum, p1.mensura) == 0),
        (int)(v0.mensura == v1.mensura && memcmp(v0.datum, v1.datum, v0.mensura) == 0));
    esc(p1); printf("]\tvalor0=["); esc(v0); printf("]\n");
}

int main(void)
{
    pp = piscina_generare_dynamicum("sonda", 1 << 18);
    it = internamentum_creare(pp);
    mon("BASIS", "<r>\n  <m\\>\n    one\n      two\n  </m>\n</r>");
    mon("LINEA_SPATIO_SOLA", "<m\\>\n  a\n   \n  b\n</m>");
    mon("TAB_SPATIUM_MIXTA", "<m\\>\n\ta\n  b\n</m>");
    mon("LINEA_TAGI", "<m\\>first\n  second\n</m>");
    mon("LIBERI_INLINE", "<m\\>\n  a <b/>\n</m>");
    mon("CAPTURA", "<r><m\\ (> x</r>");
    mon("ORDO_INVERSUS", "<m\\!>x</m>");
    mon("CRUDUS_ML", "<m!\\>\n  <a> & </m\n</m>");
    mon("ENTIA_ML", "<m\\>\n  a &amp; b\n</m>");
    mon("VACUUM", "<m\\>\n</m>");
    return 0;
}
