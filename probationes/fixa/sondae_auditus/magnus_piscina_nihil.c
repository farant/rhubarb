/* sonda: piscina NIHIL (via celeris et non), exitus NIHIL in
 * magnus_ex_chorda, piscina certae magnitudinis exhausta. Casus quisque
 * in processu filio (fork), ut ruina unius ceteros non tegat. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "magnus.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

static Magnus big_;   /* 2^64 in piscina parentis */
static Magnus smin_;

static void run_case(int which)
{
    Magnus m;
    Piscina* fixa;
    switch (which) {
    case 0: m = magnus_adde(magnus_ex_s64(2), magnus_ex_s64(3), NULL);
        printf("adde_parva_nihil=%lld\n", (long long)m.parvus); break;
    case 1: m = magnus_multiplica(magnus_ex_s64(1000), magnus_ex_s64(1000), NULL);
        printf("mult_parva_nihil=%lld\n", (long long)m.parvus); break;
    case 2: { Magnus q, r; b32 ok = magnus_divide(magnus_ex_s64(7), magnus_ex_s64(2), NULL, &q, &r);
        printf("divide_parva_nihil ok=%d q=%lld r=%lld\n", (int)ok, (long long)q.parvus, (long long)r.parvus); } break;
    case 3: m = magnus_nega(smin_, NULL);
        printf("nega_smin_nihil membra=%p\n", (void*)m.membra); break;
    case 4: m = magnus_adde(big_, magnus_ex_s64(1), NULL);
        printf("adde_magna_nihil membra=%p\n", (void*)m.membra); break;
    case 5: { b32 ok = magnus_ex_chorda(chorda_ex_buffer((i8*)"7", 1), NULL, &m);
        printf("ex_chorda_parva_piscina_nihil ok=%d\n", (int)ok); } break;
    case 6: { Piscina* p = piscina_generare_dynamicum("s", 4096);
        b32 ok = magnus_ex_chorda(chorda_ex_literis("7", p), p, NULL);
        printf("ex_chorda_exitus_nihil ok=%d\n", (int)ok); } break;
    case 7: { chorda c = magnus_ad_chordam(magnus_ex_s64(5), NULL);
        printf("ad_chordam_piscina_nihil mensura=%u\n", (unsigned)c.mensura); } break;
    case 8: m = magnus_divisor_communis(magnus_ex_s64(12), magnus_ex_s64(18), NULL);
        printf("gcd_parva_nihil=%lld\n", (long long)m.parvus); break;
    case 9: m = magnus_potentia(magnus_ex_s64(3), 5, NULL);
        printf("potentia_parva_nihil=%lld\n", (long long)m.parvus); break;
    case 10: /* piscina certae magnitudinis exhausta */
        fixa = piscina_generare_certae_magnitudinis("fixa", 256);
        m = magnus_potentia(magnus_ex_s64(3), 2000, fixa);
        printf("fixa_exhausta membra=%p\n", (void*)m.membra); break;
    case 11: { Magnus u, v;
        m = magnus_divisor_communis_testatus(magnus_ex_s64(12), magnus_ex_s64(18), NULL, &u, &v);
        printf("gcdt_parva_nihil g=%lld u=%lld v=%lld\n", (long long)m.parvus,
            (long long)u.parvus, (long long)v.parvus); } break;
    case 12: { Magnus x;
        m = magnus_divisor_communis_testatus(magnus_ex_s64(12), magnus_ex_s64(18), NULL, NULL, NULL);
        x = m;
        printf("gcdt_uv_nihil g=%lld\n", (long long)x.parvus); } break;
    case 13: m = magnus_absolutum(smin_, NULL);
        printf("abs_smin_nihil membra=%p\n", (void*)m.membra); break;
    }
}

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    int k;
    (void)magnus_ex_chorda(chorda_ex_literis("18446744073709551616", p), p, &big_);
    smin_ = magnus_ex_s64((s64)(-0x7FFFFFFFFFFFFFFFLL - 1));
    for (k = 0; k <= 13; k++) {
        pid_t pid;
        int status = 0;
        fflush(stdout);
        pid = fork();
        if (pid == 0) {
            run_case(k);
            fflush(stdout);
            _exit(0);
        }
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            printf("  case %d: exit %d\n", k, WEXITSTATUS(status));
        else
            printf("  case %d: SIGNAL %d\n", k, WTERMSIG(status));
    }
    piscina_destruere(p);
    return 0;
}
