/* sonda: vita effectuum - magnus_transcribe post destructionem fontis,
 * divisor communis per piscinas alternas (effectus et testes post
 * clausuram legendi; ASan videt si in piscinam alternam monstrant),
 * magnus_divide cum quotiens == residuum, apex alternarum. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "magnus.h"
#include <stdio.h>

static void pr(const char* tag, Magnus m, Piscina* p)
{
    chorda c = magnus_ad_chordam(m, p);
    printf("%s=%.*s\n", tag, (int)c.mensura, (const char*)c.datum);
}

static Magnus fib(int n, Piscina* p)
{
    Magnus a = magnus_ex_s64(0), b = magnus_ex_s64(1), t;
    int k;
    for (k = 0; k < n; k++) { t = magnus_adde(a, b, p); a = b; b = t; }
    return a;
}

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    Piscina* src = piscina_generare_dynamicum("fons", 4096);
    Magnus a, b, g, u, v, copy, k10;
    memoriae_index ante;

    /* transcribe: fons destruitur */
    a = magnus_potentia(magnus_ex_s64(7), 300, src);
    copy = magnus_transcribe(a, p);
    printf("shares=%d\n", copy.membra == a.membra);
    piscina_destruere(src);
    pr("tr7^300", copy, p);
    /* inline */
    copy = magnus_transcribe(magnus_ex_s64(-5), p);
    printf("tr_inline membra=%p val=%lld\n", (void*)copy.membra, (long long)copy.parvus);

    /* gcd Fibonacci F2000, F2001 (multi gradus, ambo magni) */
    a = fib(2001, p); b = fib(2000, p);
    ante = piscina_summa_usus(p);
    g = magnus_divisor_communis(a, b, p);
    printf("gcd_fib usus_delta=%lu apex=%lu\n", (unsigned long)(piscina_summa_usus(p) - ante),
        (unsigned long)magnus_apex_alternarum());
    pr("gcd_fib", g, p);
    ante = piscina_summa_usus(p);
    g = magnus_divisor_communis_testatus(a, b, p, &u, &v);
    printf("gcdt_fib usus_delta=%lu apex=%lu\n", (unsigned long)(piscina_summa_usus(p) - ante),
        (unsigned long)magnus_apex_alternarum());
    pr("gcdt_fib_g", g, p); pr("gcdt_fib_u", u, p); pr("gcdt_fib_v", v, p);

    /* gcd magnorum cum divisore communi magno: a = F2001 * 10^60, b = F2000 * 10^60 */
    k10 = magnus_potentia(magnus_ex_s64(10), 60, p);
    a = magnus_multiplica(fib(2001, p), k10, p);
    b = magnus_multiplica(fib(2000, p), magnus_nega(k10, p), p);
    g = magnus_divisor_communis(a, b, p);
    {
        Magnus g2 = magnus_divisor_communis(magnus_potentia(magnus_ex_s64(3), 500, p),
            magnus_potentia(magnus_ex_s64(6), 400, p), p);   /* secundus, piscinas alternas reficit */
        pr("gcd_big_g", g, p);
        pr("gcd_second", g2, p);
    }
    g = magnus_divisor_communis_testatus(a, b, p, &u, &v);
    {
        Magnus g2 = magnus_divisor_communis_testatus(magnus_potentia(magnus_ex_s64(3), 500, p),
            magnus_potentia(magnus_ex_s64(6), 400, p), p, NULL, NULL);
        pr("gcdt_big_g", g, p); pr("gcdt_big_u", u, p); pr("gcdt_big_v", v, p);
        pr("gcdt_second", g2, p);
    }
    /* unus magnus, alter parvus: testatus per alternas */
    a = fib(3000, p);
    g = magnus_divisor_communis_testatus(a, magnus_ex_s64(-7), p, &u, &v);
    printf("apex_one_big=%lu\n", (unsigned long)magnus_apex_alternarum());
    pr("one_big_g", g, p); pr("one_big_u", u, p); pr("one_big_v", v, p);
    /* a magnus, b 0 per alternas */
    g = magnus_divisor_communis_testatus(magnus_nega(a, p), magnus_ex_s64(0), p, &u, &v);
    pr("big0_g", g, p); pr("big0_u", u, p); pr("big0_v", v, p);
    g = magnus_divisor_communis(magnus_nega(a, p), magnus_nega(a, p), p);
    printf("apex_aa=%lu\n", (unsigned long)magnus_apex_alternarum());
    pr("aa_g", g, p);
    /* apex: parvus post magnum */
    g = magnus_divisor_communis(magnus_ex_s64(12), magnus_ex_s64(18), p);
    printf("apex_after_small=%lu\n", (unsigned long)magnus_apex_alternarum());

    /* quotiens == residuum */
    {
        Magnus same;
        b32 ok = magnus_divide(fib(300, p), magnus_ex_s64(-1000003), p, &same, &same);
        printf("same_ok=%d\n", (int)ok);
        pr("same", same, p);
        ok = magnus_divide(magnus_ex_s64(-17), magnus_ex_s64(5), p, &same, &same);
        pr("same_small", same, p);
    }
    pr("fib300", fib(300, p), p);
    pr("fib2000", fib(2000, p), p);
    pr("fib2001", fib(2001, p), p);
    pr("fib3000", fib(3000, p), p);
    piscina_destruere(p);
    return 0;
}
