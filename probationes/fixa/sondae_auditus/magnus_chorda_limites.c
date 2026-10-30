/* sonda: magnus_ex_chorda limites (vacua, '-', '+', spatia, NUL
 * interius, non-ASCII, zephyra praefixa, limites s64) et
 * magnus_ad_chordam (frusta interiora nulla). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "magnus.h"
#include <stdio.h>
#include <string.h>

/* Magnus XVI octetorum (budget B1 D1, 2026-10-29): membra != NIHIL ->
 * parvus = signum x longitudo */
static int canon(Magnus m)
{
    int signum;
    long long longitudo;
    if (m.membra == NULL) return 1;
    if (m.parvus == 0) return 0;
    signum = m.parvus < 0 ? -1 : 1;
    longitudo = m.parvus < 0 ? -m.parvus : m.parvus;
    if (longitudo < 2) return 0;
    if (m.membra[longitudo - 1] == 0) return 0;
    if (longitudo == 2) {
        unsigned int top = m.membra[1];
        if (signum > 0 && top < 0x80000000U) return 0;
        if (signum < 0 && (top < 0x80000000U ||
            (top == 0x80000000U && m.membra[0] == 0))) return 0;
    }
    return 1;
}

static void casus_(Piscina* p, const char* tag, const char* buf, int len)
{
    Magnus m;
    b32 ok;
    m.parvus = 4242; m.membra = NULL;
    ok = magnus_ex_chorda(chorda_ex_buffer((i8*)buf, (i32)len), p, &m);
    if (ok) {
        chorda c = magnus_ad_chordam(m, p);
        printf("%s ok=1 val=%.*s canon=%d inline=%d\n", tag, (int)c.mensura,
            (const char*)c.datum, canon(m), m.membra == NULL);
    } else {
        printf("%s ok=0 untouched=%d\n", tag,
            m.parvus == 4242 && m.membra == NULL);
    }
}

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    static char big[4000];
    int k;

    casus_(p, "empty", "", 0);
    casus_(p, "minus", "-", 1);
    casus_(p, "minus0", "-0", 2);
    casus_(p, "minus000", "-000", 4);
    casus_(p, "plus5", "+5", 2);
    casus_(p, "space1", " 1", 2);
    casus_(p, "1space", "1 ", 2);
    casus_(p, "minusminus", "--1", 3);
    casus_(p, "nul_inside", "12\0" "3", 4);
    casus_(p, "nul_trailing", "12\0", 3);
    casus_(p, "utf8_fullwidth1", "\xef\xbc\x91", 3);
    casus_(p, "arabic1", "\xd9\xa1", 2);
    casus_(p, "high_byte", "1\x80", 2);
    casus_(p, "colon", "1:", 2);
    casus_(p, "slash", "/1", 2);
    casus_(p, "s64max", "9223372036854775807", 19);
    casus_(p, "2^63", "9223372036854775808", 19);
    casus_(p, "s64min", "-9223372036854775808", 20);
    casus_(p, "-2^63-1", "-9223372036854775809", 20);
    casus_(p, "2^64-1", "18446744073709551615", 20);
    casus_(p, "2^64", "18446744073709551616", 20);
    casus_(p, "s64min_lz", "-00009223372036854775808", 24);
    /* centum zephyra + 1 */
    for (k = 0; k < 300; k++) big[k] = '0';
    big[300] = '1';
    casus_(p, "zeros300_1", big, 301);
    big[0] = '-';
    casus_(p, "minus_zeros299_1", big, 301);
    for (k = 0; k < 3000; k++) big[k] = '0';
    casus_(p, "zeros3000", big, 3000);
    big[0] = '-';
    casus_(p, "minus_zeros2999", big, 3000);
    /* 2^63 cum zephyris */
    memcpy(big, "000000000000000000000009223372036854775808", 42);
    casus_(p, "lz_2^63", big, 42);
    /* datum NIHIL cum mensura > 0 */
    {
        Magnus m; b32 ok;
        m.parvus = 4242; m.membra = NULL;
        ok = magnus_ex_chorda(chorda_ex_buffer(NULL, 5), p, &m);
        printf("nulldatum ok=%d untouched=%d\n", (int)ok, m.parvus == 4242);
    }
    /* ad_chordam: frusta nulla interiora */
    {
        const char* tests[] = { "1000000000", "1000000000000000000",
            "1000000000000000000000000000", "1000000001",
            "18446744073709551616", "-1000000000000000000000000000",
            "100000000000000000000000000000000000001",
            "-9223372036854775808", "9223372036854775807", "0", "-1",
            "-9223372036854775809", "4294967296", "-4294967296",
            "999999999999999999999999999", NULL };
        for (k = 0; tests[k]; k++) {
            Magnus m;
            chorda c;
            magnus_ex_chorda(chorda_ex_literis(tests[k], p), p, &m);
            c = magnus_ad_chordam(m, p);
            printf("ad %s -> %.*s %s\n", tests[k], (int)c.mensura,
                (const char*)c.datum,
                ((int)c.mensura == (int)strlen(tests[k]) &&
                 memcmp(c.datum, tests[k], c.mensura) == 0) ? "OK" : "DIFF");
        }
    }
    /* ad_chordam ex s64 directo */
    {
        s64 vals[5];
        vals[0] = (s64)(-0x7FFFFFFFFFFFFFFFLL - 1); vals[1] = 0x7FFFFFFFFFFFFFFFLL;
        vals[2] = 0; vals[3] = -1; vals[4] = 1000000000LL * 1000000000LL;
        for (k = 0; k < 5; k++) {
            chorda c = magnus_ad_chordam(magnus_ex_s64(vals[k]), p);
            printf("ex_s64 %lld -> %.*s\n", (long long)vals[k], (int)c.mensura,
                (const char*)c.datum);
        }
    }
    piscina_destruere(p);
    return 0;
}
