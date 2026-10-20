#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include <stdio.h>
#include <string.h>

static Piscina* pp;
static InternamentumChorda* it;

static void mon(const char* tag, const char* s)
{
    StmlResultus r;
    chorda in, w, v;
    StmlNodus* e;
    in.datum = (i8*)s; in.mensura = (i32)strlen(s);
    r = stml_legere(in, pp, it);
    e = r.elementum_radix;
    w = r.radix ? stml_scribere(r.radix, pp, FALSUM) : in;
    v = e ? stml_textus_valor(e, pp) : in;
    printf("%s\tin=[%s]\tstatus=%d\tsucc=%d\tradix_tit=[%.*s]\tvalor=[%.*s]\tnliberi_doc=%u\tscriptum=[%.*s]\n",
        tag, s, (int)r.status, (int)r.successus,
        (e && e->titulus) ? (int)e->titulus->mensura : 0,
        (e && e->titulus) ? (char*)e->titulus->datum : "",
        e ? (int)v.mensura : 0, e ? (char*)v.datum : "",
        r.radix ? stml_numerus_liberorum(r.radix) : 0,
        (int)w.mensura, (char*)w.datum);
}

int main(void)
{
    StmlNodus* t; StmlNodus* x; chorda w; StmlResultus r; chorda v;
    pp = piscina_generare_dynamicum("sonda", 65536);
    it = internamentum_creare(pp);

    /* raw content BEGINNING with a prefix-match of the terminator */
    mon("PRAEFIXUM_INITIO", "<T!></Tx></T>");
    mon("PRAEFIXUM_INITIO_LONGUS", "<code!></code-x>y</code>");
    mon("PRAEFIXUM_MEDIO_CONTROL", "<T!>a</Tx></T>");
    mon("PRAEFIXUM_INITIO_ESC", "<T!><\\/Tx></T>");
    mon("TERMINATOR_INITIO", "<T!></T>");
    mon("EOF_POST_T", "<T!></T");

    /* writer side: hand-built raw element whose value starts with '</Tx' */
    t = stml_elementum_crudum_creare(pp, it, "T");
    x = stml_textum_creare(pp, it, "</Tx>");
    stml_liberum_addere(t, x);
    w = stml_scribere(t, pp, FALSUM);
    r = stml_legere(w, pp, it);
    v = r.elementum_radix ? stml_textus_valor(r.elementum_radix, pp) : w;
    printf("SCRIPTOR_PRAEFIXUM\tscriptum=[%.*s]\trelectum_status=%d\tvalor=[%.*s]\texpect=[</Tx>]\n",
        (int)w.mensura, (char*)w.datum, (int)r.status,
        r.elementum_radix ? (int)v.mensura : 0, r.elementum_radix ? (char*)v.datum : "");
    return 0;
}
