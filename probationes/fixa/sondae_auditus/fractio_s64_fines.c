#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "magnus.h"
#include "fractio.h"
#include <stdio.h>

static Piscina* pp;

static void pr(const char* tag, Fractio f)
{
    chorda t = fractio_ad_chordam(f, pp);
    Magnus n = fractio_numerator(f), d = fractio_denominator(f);
    Magnus g = magnus_divisor_communis(n, d, pp);
    int canon = magnus_signum(d) > 0 &&
        (magnus_signum(n) == 0 ? magnus_aequalis(d, magnus_ex_s64(1)) : magnus_aequalis(g, magnus_ex_s64(1)));
    printf("%s = %.*s canon=%d\n", tag, (int)t.mensura, (const char*)t.datum, canon);
}

static Fractio F(s64 a, s64 b)
{
    Fractio f = fractio_ex_s64(4242);
    if (!fractio_ex_s64_s64(a, b, pp, &f)) printf("  (FALSUM %lld/%lld)\n", (long long)a, (long long)b);
    return f;
}

int main(void)
{
    s64 Smax = (s64)0x7FFFFFFFFFFFFFFFLL;
    s64 Smin = -Smax - 1;
    Fractio f, one = fractio_ex_s64(1);
    pp = piscina_generare_dynamicum("sonda", 65536);

    pr("ex_s64(MIN)", fractio_ex_s64(Smin));
    pr("ex_s64(MAX)", fractio_ex_s64(Smax));
    pr("ex_s64(0)", fractio_ex_s64(0));
    pr("ex_s64(-1)", fractio_ex_s64(-1));
    pr("MIN/-1", F(Smin, -1));
    pr("1/MIN", F(1, Smin));
    pr("MIN/MIN", F(Smin, Smin));
    pr("2/MIN", F(2, Smin));
    pr("-1/MIN", F(-1, Smin));
    pr("MAX/MIN", F(Smax, Smin));
    pr("MIN/MAX", F(Smin, Smax));
    pr("MIN/2", F(Smin, 2));
    pr("MIN/-2", F(Smin, -2));
    pr("0/-5", F(0, -5));
    pr("0/MIN", F(0, Smin));
    pr("5/-10", F(5, -10));
    pr("-5/-10", F(-5, -10));
    f = fractio_ex_s64(7);
    printf("0/0 ret=%d ", fractio_ex_s64_s64(0, 0, pp, &f)); pr("exitus", f);
    printf("MIN/0 ret=%d ", fractio_ex_s64_s64(Smin, 0, pp, &f)); pr("exitus", f);

    pr("nega(MIN)", fractio_nega(fractio_ex_s64(Smin), pp));
    pr("abs(MIN)", fractio_absolutum(fractio_ex_s64(Smin), pp));
    pr("abs(MIN/3)", fractio_absolutum(F(Smin, 3), pp));
    pr("nega(0)", fractio_nega(fractio_ex_s64(0), pp));
    pr("nega(nega(MIN))", fractio_nega(fractio_nega(fractio_ex_s64(Smin), pp), pp));
    printf("aeq nega(nega(MIN)) MIN = %d\n", fractio_aequalis(fractio_nega(fractio_nega(fractio_ex_s64(Smin), pp), pp), fractio_ex_s64(Smin)));
    if (fractio_inversa(F(1, Smin), pp, &f)) pr("inv(1/MIN)", f);
    if (fractio_inversa(fractio_ex_s64(Smin), pp, &f)) pr("inv(MIN)", f);
    if (fractio_inversa(F(Smin, 3), pp, &f)) pr("inv(MIN/3)", f);
    if (fractio_inversa(F(-2, 3), pp, &f)) pr("inv(-2/3)", f);

    pr("MAX+1", fractio_adde(fractio_ex_s64(Smax), one, pp));
    pr("MIN-1", fractio_subtrahe(fractio_ex_s64(Smin), one, pp));
    pr("MIN-MAX", fractio_subtrahe(fractio_ex_s64(Smin), fractio_ex_s64(Smax), pp));
    pr("MAX-MIN", fractio_subtrahe(fractio_ex_s64(Smax), fractio_ex_s64(Smin), pp));
    pr("MIN+MIN", fractio_adde(fractio_ex_s64(Smin), fractio_ex_s64(Smin), pp));
    pr("1/MAX + 1/(MAX-1)", fractio_adde(F(1, Smax), F(1, Smax - 1), pp));
    pr("1/MAX - 1/MAX", fractio_subtrahe(F(1, Smax), F(1, Smax), pp));
    pr("MIN*MIN", fractio_multiplica(fractio_ex_s64(Smin), fractio_ex_s64(Smin), pp));
    pr("MIN*-1", fractio_multiplica(fractio_ex_s64(Smin), fractio_ex_s64(-1), pp));
    pr("(1/MIN)*(1/MIN)", fractio_multiplica(F(1, Smin), F(1, Smin), pp));
    pr("(MIN/MAX)*(MAX/MIN)", fractio_multiplica(F(Smin, Smax), F(Smax, Smin), pp));
    pr("1/MAX + 1/MAX", fractio_adde(F(1, Smax), F(1, Smax), pp));
    pr("MIN/3 - MIN/3", fractio_subtrahe(F(Smin, 3), F(Smin, 3), pp));
    pr("1/6+1/10", fractio_adde(F(1, 6), F(1, 10), pp));
    pr("1/4+1/4", fractio_adde(F(1, 4), F(1, 4), pp));
    pr("2/3*3/2", fractio_multiplica(F(2, 3), F(3, 2), pp));
    pr("0*-5/3", fractio_multiplica(fractio_ex_s64(0), F(-5, 3), pp));
    pr("-2/3*-9/4", fractio_multiplica(F(-2, 3), F(-9, 4), pp));
    if (fractio_divide(F(1, 2), F(-1, 3), pp, &f)) pr("1/2 / -1/3", f);
    if (fractio_divide(F(-1, 2), F(-1, 3), pp, &f)) pr("-1/2 / -1/3", f);
    if (fractio_divide(fractio_ex_s64(0), F(-1, 3), pp, &f)) pr("0 / -1/3", f);
    if (fractio_divide(fractio_ex_s64(Smin), fractio_ex_s64(-1), pp, &f)) pr("MIN / -1", f);
    if (fractio_divide(fractio_ex_s64(1), fractio_ex_s64(Smin), pp, &f)) pr("1 / MIN", f);
    printf("cmp MIN MAX = %d, MAX MIN = %d, 1/MIN 1/MAX = %d\n",
        fractio_compara(fractio_ex_s64(Smin), fractio_ex_s64(Smax), pp),
        fractio_compara(fractio_ex_s64(Smax), fractio_ex_s64(Smin), pp),
        fractio_compara(F(1, Smin), F(1, Smax), pp));
    {
        Magnus m = fractio_pavimentum(F(Smin, 3), pp); chorda t = magnus_ad_chordam(m, pp);
        printf("floor(MIN/3) = %.*s\n", (int)t.mensura, (const char*)t.datum);
        m = fractio_tectum(F(Smin, 3), pp); t = magnus_ad_chordam(m, pp);
        printf("ceil(MIN/3) = %.*s\n", (int)t.mensura, (const char*)t.datum);
        m = fractio_rotunda(F(Smin, 3), pp); t = magnus_ad_chordam(m, pp);
        printf("round(MIN/3) = %.*s\n", (int)t.mensura, (const char*)t.datum);
        m = fractio_rotunda(F(Smax, 2), pp); t = magnus_ad_chordam(m, pp);
        printf("round(MAX/2) = %.*s\n", (int)t.mensura, (const char*)t.datum);
        m = fractio_rotunda(F(Smin + 1, 2), pp); t = magnus_ad_chordam(m, pp);
        printf("round((MIN+1)/2) = %.*s\n", (int)t.mensura, (const char*)t.datum);
        m = fractio_tectum(F(Smax, 2), pp); t = magnus_ad_chordam(m, pp);
        printf("ceil(MAX/2) = %.*s\n", (int)t.mensura, (const char*)t.datum);
    }
    piscina_destruere(pp);
    return 0;
}
