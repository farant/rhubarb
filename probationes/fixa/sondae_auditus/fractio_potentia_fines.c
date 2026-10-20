#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "magnus.h"
#include "fractio.h"
#include <stdio.h>
#include <string.h>

static Piscina* pp;
static Fractio P(const char* s) { Fractio f = fractio_ex_s64(0); (void)fractio_ex_chorda(chorda_ex_literis(s, pp), pp, &f); return f; }
static void run(const char* b, s32 e)
{
    Fractio f = fractio_ex_s64(4242);
    b32 r = fractio_potentia(P(b), e, pp, &f);
    chorda t = fractio_ad_chordam(f, pp);
    Magnus d = fractio_denominator(f);
    if (t.mensura > 80) printf("(%s)^%d ret=%d len=%u densign=%d\n", b, (int)e, r, (unsigned)t.mensura, magnus_signum(d));
    else printf("(%s)^%d ret=%d val=%.*s densign=%d\n", b, (int)e, r, (int)t.mensura, (const char*)t.datum, magnus_signum(d));
}
int main(void)
{
    s32 smin = (s32)0x80000000U;
    pp = piscina_generare_dynamicum("sonda", 65536);
    run("-2/3", -3); run("-2/3", -2); run("-2/3", -1); run("-2/3", 3); run("-2/3", 0);
    run("0", -1); run("0", 0); run("0", 5); run("0", smin);
    run("1", smin); run("-1", smin); run("-1", smin + 1); run("-1", 0x7FFFFFFF);
    run("-1/2", -1); run("-3", -3); run("-9223372036854775808", -1); run("-9223372036854775808", -2);
    run("2", 300); run("-3/2", -101); run("7/9", 64);
    piscina_destruere(pp);
    return 0;
}
