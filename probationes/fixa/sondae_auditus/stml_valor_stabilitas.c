#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include <stdio.h>
#include <string.h>

static Piscina* pp;
static InternamentumChorda* it;

static int eq(chorda a, chorda b) { return a.mensura == b.mensura && (a.mensura == 0 || memcmp(a.datum, b.datum, a.mensura) == 0); }

static void pr(const char* k, chorda c)
{
    i32 i; printf("%s=[", k);
    for (i = 0; i < c.mensura; i++) { char ch = (char)c.datum[i]; if (ch == '\n') printf("\\n"); else putchar(ch); }
    printf("]");
}

static void st(const char* tag, const char* s)
{
    chorda in, p1, p2, v0, v1, f0, f1; StmlResultus r0, r1, r2;
    in.datum = (i8*)(size_t)s; in.mensura = (i32)strlen(s);
    r0 = stml_legere(in, pp, it);
    p1 = stml_scribere(r0.radix, pp, VERUM);
    r1 = stml_legere(p1, pp, it);
    p2 = stml_scribere(r1.radix, pp, VERUM);
    r2 = stml_legere(p2, pp, it);
    v0 = stml_textus_valor(r0.radix, pp);
    v1 = stml_textus_valor(r1.radix, pp);
    f0 = stml_textus_fluxus(r0.radix, pp);
    f1 = stml_textus_fluxus(r1.radix, pp);
    printf("%s\tst=%d/%d/%d\tvalor_stabilis=%d\tfluxus_stabilis=%d\tP_punctum_fixum=%d\t", tag, (int)r0.status, (int)r1.status, (int)r2.status,
        eq(v0, v1), eq(f0, f1), eq(p1, p2));
    if (!eq(v0, v1)) { pr("v0", v0); printf(" "); pr("v1", v1); printf(" "); }
    if (!eq(p1, p2)) { pr("P1", p1); printf(" "); pr("P2", p2); }
    else pr("P1", p1);
    printf("\n");
}

int main(void)
{
    pp = piscina_generare_dynamicum("sonda", 65536);
    it = internamentum_creare(pp);
    st("PROSA_LINEAE", "<p>\n  alpha\n  beta\n</p>");
    st("PROSA_INLINE_NL", "<p>alpha\n   beta</p>");
    st("SPATIA_DUO", "<p>a  b</p>");
    st("MIXTUM", "<p>one <b>two</b>\n  three</p>");
    st("LONGA", "<p>Lorem ipsum dolor sit amet consectetur adipiscing elit sed do eiusmod tempor incididunt ut labore et dolore magna aliqua</p>");
    st("MARGO_ALBUS", "<p> x </p>");
    st("ALBUS_SOLUS", "<r><sep>   </sep><a/></r>");
    st("COMMENTUM", "<p>a<!--c-->\n b</p>");
    st("CAPT", "<r><a(> x\n y</r>");
    st("TAB", "<p>a\tb</p>");
    st("ENTIA", "<p>a &amp; b &lt;c&gt; \"q\" &apos;</p>");
    return 0;
}
