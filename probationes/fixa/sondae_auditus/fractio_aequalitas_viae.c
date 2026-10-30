#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "magnus.h"
#include "fractio.h"
#include <stdio.h>

static Piscina* pp;
/* Magnus XVI octetorum (budget B1 D1, 2026-10-29): membra != NIHIL ->
 * parvus = signum x longitudo; inline -> signum/longitudo 0 ut olim */
static int SIGNUM_M(Magnus m) { return m.membra ? (m.parvus < 0 ? -1 : 1) : 0; }
static unsigned LONGITUDO_M(Magnus m) { return m.membra ? (unsigned)(m.parvus < 0 ? -m.parvus : m.parvus) : 0U; }
static Fractio P(const char* s) { Fractio f = fractio_ex_s64(4242); if (!fractio_ex_chorda(chorda_ex_literis(s, pp), pp, &f)) printf("PARSE FAIL %s\n", s); return f; }
static void raw(const char* tag, Fractio f)
{
    Magnus n = fractio_numerator(f), d = fractio_denominator(f);
    chorda t = fractio_ad_chordam(f, pp);
    printf("%-22s %-8.*s num{parvus=%lld signum=%d long=%u membra=%s} den{parvus=%lld signum=%d long=%u membra=%s}\n", tag,
        (int)t.mensura, (const char*)t.datum,
        (long long)n.parvus, SIGNUM_M(n), LONGITUDO_M(n), n.membra ? "P" : "0",
        (long long)d.parvus, SIGNUM_M(d), LONGITUDO_M(d), d.membra ? "P" : "0");
}
static void prm(const char* tag, Magnus m)
{
    chorda t = magnus_ad_chordam(m, pp);
    printf("%-22s %.*s (parvus=%lld membra=%s)\n", tag, (int)t.mensura, (const char*)t.datum, (long long)m.parvus, m.membra ? "P" : "0");
}
int main(void)
{
    Fractio z0, a, b, c, w;
    pp = piscina_generare_dynamicum("sonda", 65536);
    z0 = fractio_ex_s64(0);
    raw("ex_s64(0)", z0);
    raw("nega(0)", fractio_nega(z0, pp));
    raw("parse -0", P("-0"));
    raw("parse -0/7", P("-0/7"));
    raw("0 * -5/3", fractio_multiplica(z0, P("-5/3"), pp));
    raw("-5/3 * 0", fractio_multiplica(P("-5/3"), z0, pp));
    raw("-5/3 - -5/3", fractio_subtrahe(P("-5/3"), P("-5/3"), pp));
    raw("big - big", fractio_subtrahe(P("-123456789012345678901234567/5"), P("-123456789012345678901234567/5"), pp));
    raw("big + -big", fractio_adde(P("123456789012345678901234567/35"), P("-123456789012345678901234567/35"), pp));
    { Fractio q; (void)fractio_divide(z0, P("-7/2"), pp, &q); raw("0 / -7/2", q); }
    { Fractio q; (void)fractio_ex_s64_s64(0, -9, pp, &q); raw("ex_s64_s64(0,-9)", q); }
    { Fractio q; (void)fractio_ex_magnis(magnus_ex_s64(0), magnus_nega(magnus_potentia(magnus_ex_s64(3), 80, pp), pp), pp, &q); raw("0/-(3^80)", q); }
    { Fractio q; (void)fractio_potentia(z0, 3, pp, &q); raw("0^3", q); }
    a = P("2/4"); b = P("1/2");
    (void)fractio_ex_s64_s64(3, 6, pp, &c);
    w = fractio_multiplica(P("3"), P("1/6"), pp);
    printf("aeq 2/4~1/2=%d 1/2~3/6=%d 1/2~3*1/6=%d cmp=%d\n", fractio_aequalis(a, b), fractio_aequalis(b, c), fractio_aequalis(b, w), fractio_compara(b, w, pp));
    raw("2/3+4/3 (=2)", fractio_adde(P("2/3"), P("4/3"), pp));
    printf("est_integra(2/3+4/3)=%d est_integra(0)=%d est_integra(-3)=%d\n", fractio_est_integra(fractio_adde(P("2/3"), P("4/3"), pp)), fractio_est_integra(z0), fractio_est_integra(P("-3")));
    /* 2^64 built two ways */
    raw("2^64 parse", P("18446744073709551616"));
    raw("2^32*2^32", fractio_multiplica(P("4294967296"), P("4294967296"), pp));
    printf("aeq=%d\n", fractio_aequalis(P("18446744073709551616"), fractio_multiplica(P("4294967296"), P("4294967296"), pp)));
    raw("2^64/2^64 back to 1", fractio_multiplica(P("18446744073709551616"), P("1/18446744073709551616"), pp));
    raw("(2^64+1)-2^64", fractio_subtrahe(P("18446744073709551617"), P("18446744073709551616"), pp));
    raw("2^63 - 1", fractio_subtrahe(P("9223372036854775808"), P("1"), pp));
    raw("-2^63 via big", fractio_nega(P("9223372036854775808"), pp));
    raw("abs(-2^63) - 1", fractio_subtrahe(fractio_absolutum(P("-9223372036854775808"), pp), P("1"), pp));
    /* rounding corners */
    prm("tectum(-1/2)", fractio_tectum(P("-1/2"), pp));
    prm("rotunda(-1/2)", fractio_rotunda(P("-1/2"), pp));
    prm("rotunda(-49/100)", fractio_rotunda(P("-49/100"), pp));
    prm("rotunda(49/100)", fractio_rotunda(P("49/100"), pp));
    prm("rotunda(51/100)", fractio_rotunda(P("51/100"), pp));
    prm("rotunda(-51/100)", fractio_rotunda(P("-51/100"), pp));
    prm("tectum(-1/3)", fractio_tectum(P("-1/3"), pp));
    prm("pavimentum(1/3)", fractio_pavimentum(P("1/3"), pp));
    prm("tectum(100000000000000000000001/100000000000000000000000)", fractio_tectum(P("100000000000000000000001/100000000000000000000000"), pp));
    prm("rotunda(-(2^100+1)/2)", fractio_rotunda(P("-1267650600228229401496703205377/2"), pp));
    prm("rotunda((2^100+1)/2)", fractio_rotunda(P("1267650600228229401496703205377/2"), pp));
    prm("rotunda((2^100+3)/2)", fractio_rotunda(P("1267650600228229401496703205379/2"), pp));
    prm("rotunda(-(2^100+3)/2)", fractio_rotunda(P("-1267650600228229401496703205379/2"), pp));
    piscina_destruere(pp);
    return 0;
}
