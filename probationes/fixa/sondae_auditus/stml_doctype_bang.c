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
    chorda in, w;
    i32 n, i;
    in.datum = (i8*)(size_t)s; in.mensura = (i32)strlen(s);
    r = stml_legere(in, pp, it);
    w = r.radix ? stml_scribere(r.radix, pp, FALSUM) : in;
    n = r.radix ? stml_numerus_liberorum(r.radix) : 0;
    printf("%s\tin=[%s]\tstatus=%d\tliberi=%u\tgenera=", tag, s, (int)r.status, n);
    per (i = 0; i < n; i++)
    {
        StmlNodus* l = stml_liberum_ad_indicem(r.radix, i);
        printf("%d:[%.*s] ", (int)l->genus,
            l->valor ? (int)l->valor->mensura : (l->titulus ? (int)l->titulus->mensura : 0),
            l->valor ? (char*)l->valor->datum : (l->titulus ? (char*)l->titulus->datum : ""));
    }
    printf("\tscriptum=[%.*s]\tfidelis=%d\n", (int)w.mensura, (char*)w.datum,
        (int)(w.mensura == in.mensura && memcmp(w.datum, in.datum, in.mensura) == 0));
}

int main(void)
{
    pp = piscina_generare_dynamicum("sonda", 65536);
    it = internamentum_creare(pp);
    mon("DOCTYPE_NORMALE", "<!DOCTYPE html><a/>");
    mon("D_BREVIS", "<!D><a/>xyz");
    mon("D_ALIUD", "<!Dummy><a>t</a>");
    mon("DOCTYPE_SINE_SPATIO", "<!DOCTYPEhtml><a/>");
    mon("doctype_minusc", "<!doctype html><a/>");
    mon("CDATA", "<a><![CDATA[x<y]]></a>");
    mon("BANG_NUDUM", "<!><a/>");
    mon("ENTITY", "<!ENTITY x \"y\"><a/>");
    mon("COMMENTUM_TRIA", "<a><!--- x --></a>");
    mon("COMMENTUM_VACUUM", "<a><!----></a>");
    mon("PI_VACUA", "<?x?><a/>");
    mon("PI_NON_CLAUSA", "<?xml");
    return 0;
}
