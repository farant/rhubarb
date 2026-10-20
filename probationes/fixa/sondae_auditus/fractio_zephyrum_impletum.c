#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "magnus.h"
#include "fractio.h"
#include <stdio.h>
#include <string.h>

static Piscina* pp;
static void pr(const char* tag, Fractio f)
{
    chorda t = fractio_ad_chordam(f, pp);
    printf("%s = %.*s\n", tag, (int)t.mensura, (const char*)t.datum);
}
static void prm(const char* tag, Magnus m)
{
    chorda t = magnus_ad_chordam(m, pp);
    printf("%s = %.*s\n", tag, (int)t.mensura, (const char*)t.datum);
}

int main(void)
{
    Fractio z, h, f = fractio_ex_s64(4242);
    pp = piscina_generare_dynamicum("sonda", 65536);
    memset(&z, 0, sizeof z);
    (void)fractio_ex_s64_s64(1, 2, pp, &h);
    pr("0/0", z);
    printf("signum=%d est_integra=%d aequalis(0/0, 0)=%d compara(0/0,1/2)=%d compara(1/2,0/0)=%d compara(0/0,0/0)=%d\n",
        fractio_signum(z), fractio_est_integra(z), fractio_aequalis(z, fractio_ex_s64(0)),
        fractio_compara(z, h, pp), fractio_compara(h, z, pp), fractio_compara(z, z, pp));
    pr("0/0 + 1/2", fractio_adde(z, h, pp));
    pr("1/2 + 0/0", fractio_adde(h, z, pp));
    pr("0/0 - 1/2", fractio_subtrahe(z, h, pp));
    pr("0/0 * 1/2", fractio_multiplica(z, h, pp));
    pr("1/2 * 0/0", fractio_multiplica(h, z, pp));
    pr("nega(0/0)", fractio_nega(z, pp));
    pr("abs(0/0)", fractio_absolutum(z, pp));
    pr("transcribe(0/0)", fractio_transcribe(z, pp));
    printf("divide(1/2,0/0) ret=%d ", fractio_divide(h, z, pp, &f)); pr("exitus", f);
    printf("divide(0/0,1/2) ret=%d ", fractio_divide(z, h, pp, &f)); pr("exitus", f);
    printf("inversa(0/0) ret=%d ", fractio_inversa(z, pp, &f)); pr("exitus", f);
    printf("potentia(0/0,2) ret=%d ", fractio_potentia(z, 2, pp, &f)); pr("exitus", f);
    printf("potentia(0/0,0) ret=%d ", fractio_potentia(z, 0, pp, &f)); pr("exitus", f);
    f = fractio_ex_s64(4242);
    printf("potentia(0/0,-1) ret=%d ", fractio_potentia(z, -1, pp, &f)); pr("exitus", f);
    prm("pavimentum(0/0)", fractio_pavimentum(z, pp));
    prm("tectum(0/0)", fractio_tectum(z, pp));
    prm("rotunda(0/0)", fractio_rotunda(z, pp));
    /* zero-filled Magnus into ex_magno / ex_magnis */
    {
        Magnus mz; memset(&mz, 0, sizeof mz);
        pr("ex_magno(zero-filled)", fractio_ex_magno(mz));
        f = fractio_ex_s64(4242);
        printf("ex_magnis(1, zero-filled) ret=%d ", fractio_ex_magnis(magnus_ex_s64(1), mz, pp, &f)); pr("exitus", f);
        printf("ex_magnis(zero-filled, -5) ret=%d ", fractio_ex_magnis(mz, magnus_ex_s64(-5), pp, &f)); pr("exitus", f);
    }
    piscina_destruere(pp);
    return 0;
}
