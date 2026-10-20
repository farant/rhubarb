/* sonda: magnus contra oraculum Pythonis - legit lineas "a b n" ex
 * plagula (argv[1]), omnes operationes imprimit cum signo formae
 * canonicae (C/N) et immutabilitatis argumentorum. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "magnus.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int canon(Magnus m)
{
    if (m.membra == NULL) return 1;
    if (m.signum != 1 && m.signum != -1) return 0;
    if (m.longitudo < 2) return 0;
    if (m.membra[m.longitudo - 1] == 0) return 0;
    if (m.longitudo == 2) {
        unsigned int top = m.membra[1];
        if (m.signum > 0 && top < 0x80000000U) return 0;
        if (m.signum < 0 && (top < 0x80000000U ||
            (top == 0x80000000U && m.membra[0] == 0))) return 0;
    }
    return 1;
}

static void pr(const char* tag, Magnus m, Piscina* p)
{
    chorda c = magnus_ad_chordam(m, p);
    printf(" %s=%.*s%s", tag, (int)c.mensura, (const char*)c.datum,
        canon(m) ? "" : "!N");
}

static unsigned long hash_limbs(Magnus m)
{
    unsigned long h = 1469598103UL;
    unsigned int k;
    if (m.membra == NULL) return (unsigned long)m.parvus;
    for (k = 0; k < m.longitudo; k++) h = h * 1099511UL + m.membra[k];
    return h;
}

int main(int argc, char** argv)
{
    FILE* f;
    static char line[200000];
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    long count = 0;

    if (argc < 2) return 2;
    f = fopen(argv[1], "r");
    if (!f) return 2;
    while (fgets(line, (int)sizeof line, f)) {
        char* sa = line;
        char* sb;
        char* sn;
        Magnus a, b, q, r, g, gt, u, v, q2, r2;
        unsigned long n;
        unsigned long ha, hb;
        size_t len_;
        PiscinaNotatio nota = piscina_notare(p);

        len_ = strlen(line);
        if (len_ && line[len_ - 1] == '\n') line[--len_] = 0;
        sb = strchr(sa, ' '); *sb++ = 0;
        sn = strchr(sb, ' '); *sn++ = 0;
        n = strtoul(sn, NULL, 10);
        if (!magnus_ex_chorda(chorda_ex_buffer((i8*)sa, (i32)strlen(sa)), p, &a)
         || !magnus_ex_chorda(chorda_ex_buffer((i8*)sb, (i32)strlen(sb)), p, &b)) {
            printf("PARSEFAIL\n");
            continue;
        }
        ha = hash_limbs(a); hb = hash_limbs(b);
        printf("L%ld", count++);
        pr("a", a, p); pr("b", b, p);
        if (!canon(a) || !canon(b)) printf(" PARSE_NONCANON");
        pr("add", magnus_adde(a, b, p), p);
        pr("sub", magnus_subtrahe(a, b, p), p);
        pr("mul", magnus_multiplica(a, b, p), p);
        pr("aa", magnus_adde(a, a, p), p);
        pr("asa", magnus_subtrahe(a, a, p), p);
        pr("ama", magnus_multiplica(a, a, p), p);
        pr("neg", magnus_nega(a, p), p);
        pr("abs", magnus_absolutum(a, p), p);
        pr("tr", magnus_transcribe(a, p), p);
        {
            s64 x = 12345;
            b32 ok = magnus_ad_s64(a, &x);
            printf(" s64=%d:%lld", (int)ok, ok ? (long long)x : 0LL);
        }
        printf(" cmp=%d eq=%d sg=%d bits=%u rp=%u",
            (int)magnus_compara(a, b), (int)magnus_aequalis(a, b),
            (int)magnus_signum(a), (unsigned)magnus_bitorum(a),
            (unsigned)magnus_residuum_parvum(a, (i32)n));
        q = magnus_ex_s64(777); r = magnus_ex_s64(888);
        if (magnus_divide(a, b, p, &q, &r)) {
            pr("q", q, p); pr("r", r, p);
        } else {
            printf(" div=F q=%lld r=%lld", (long long)q.parvus, (long long)r.parvus);
        }
        /* quotiens et residuum NIHIL */
        (void)magnus_divide(a, b, p, NULL, NULL);
        q2 = magnus_ex_s64(0);
        if (magnus_divide(a, b, p, &q2, NULL)) pr("qo", q2, p);
        r2 = magnus_ex_s64(0);
        if (magnus_divide(a, b, p, NULL, &r2)) pr("ro", r2, p);
        g = magnus_divisor_communis(a, b, p);
        pr("g", g, p);
        printf(" apex=%lu", (unsigned long)magnus_apex_alternarum());
        gt = magnus_divisor_communis_testatus(a, b, p, &u, &v);
        pr("gt", gt, p); pr("u", u, p); pr("v", v, p);
        printf(" apext=%lu", (unsigned long)magnus_apex_alternarum());
        if (hash_limbs(a) != ha || hash_limbs(b) != hb) printf(" MUTATED");
        printf("\n");
        piscina_reficere(p, nota);
    }
    fclose(f);
    piscina_destruere(p);
    return 0;
}
