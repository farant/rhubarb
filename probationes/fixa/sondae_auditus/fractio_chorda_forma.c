#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "magnus.h"
#include "fractio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Piscina* pp;

static void experire(const char* tag, const char* buf, unsigned len)
{
    /* exact-size heap copy so ASan sees any overread past mensura */
    char* h = (char*)malloc(len ? len : 1);
    Fractio f = fractio_ex_s64(4242);
    chorda c;
    b32 r;
    if (len) memcpy(h, buf, len);
    c.datum = (i8*)h; c.mensura = len;
    r = fractio_ex_chorda(c, pp, &f);
    {
        chorda t = fractio_ad_chordam(f, pp);
        Magnus n = fractio_numerator(f), d = fractio_denominator(f);
        int canon = magnus_signum(d) > 0 &&
            (magnus_signum(n) == 0 ? magnus_aequalis(d, magnus_ex_s64(1))
             : magnus_aequalis(magnus_divisor_communis(n, d, pp), magnus_ex_s64(1)));
        printf("%-14s ret=%d val=%.*s canon=%d\n", tag, r, (int)t.mensura, (const char*)t.datum, canon);
    }
    free(h);
}
#define S(x) experire("[" x "]", x, (unsigned)strlen(x))

int main(void)
{
    pp = piscina_generare_dynamicum("sonda", 65536);
    S(""); S("-"); S("-/2"); S("--1"); S("+1"); S("1/-2"); S("-1/-2"); S("-0"); S("-0/5");
    S("/"); S("1/"); S("/2"); S("1//2"); S("1/2/3"); S("1/2/");
    S("1/0"); S("0/0"); S("5/000"); S("-0/0");
    S(" 1"); S("1 "); S("1/ 2"); S("1\t"); S("1\n"); S("12a"); S("0x10"); S("1e3"); S("1.5"); S("1/2 ");
    S("007"); S("007/010"); S("0/0005"); S("00"); S("-007/014"); S("6/4"); S("-6/4");
    S("9223372036854775808"); S("-9223372036854775809/3");
    experire("[1\\0/2]", "1\0/2", 4);
    experire("[12\\0]", "12\0", 3);
    experire("[\\0]", "\0", 1);
    experire("[1/2\\0]", "1/2\0", 4);
    experire("[1/2 len3 of 1/23]", "1/23", 3);
    experire("[12 len1]", "12", 1);
    experire("[NIHIL,0]", NULL, 0);
    { chorda c; Fractio f = fractio_ex_s64(4242); c.datum = NULL; c.mensura = 0;
      printf("NIHIL datum mensura0 ret=%d untouched=%d\n", fractio_ex_chorda(c, pp, &f), fractio_aequalis(f, fractio_ex_s64(4242))); }
    { chorda c; Fractio f = fractio_ex_s64(4242); c.datum = NULL; c.mensura = 5;
      printf("NIHIL datum mensura5 ret=%d untouched=%d\n", fractio_ex_chorda(c, pp, &f), fractio_aequalis(f, fractio_ex_s64(4242))); }
    /* very long */
    {
        unsigned n = 5000, k;
        char* b = (char*)malloc(2 * n + 2);
        Fractio f = fractio_ex_s64(0);
        chorda c, t;
        /* (10^5000 - 1) / (10^5000 - 1)*... : numerator 6 repeated, denominator 3 repeated -> 2 */
        for (k = 0; k < n; k++) b[k] = '6';
        b[n] = '/';
        for (k = 0; k < n; k++) b[n + 1 + k] = '3';
        c.datum = (i8*)b; c.mensura = 2 * n + 1;
        printf("666../333.. ret=%d ", fractio_ex_chorda(c, pp, &f));
        t = fractio_ad_chordam(f, pp);
        printf("val=%.*s\n", (int)t.mensura, (const char*)t.datum);
        /* numerator 1 followed by 4999 zeros over 2 followed by 4999 zeros -> 1/2 */
        b[0] = '1'; for (k = 1; k < n; k++) b[k] = '0';
        b[n] = '/'; b[n + 1] = '2'; for (k = 1; k < n; k++) b[n + 1 + k] = '0';
        printf("10^4999/(2*10^4999) ret=%d ", fractio_ex_chorda(c, pp, &f));
        t = fractio_ad_chordam(f, pp);
        printf("val=%.*s\n", (int)t.mensura, (const char*)t.datum);
        /* round-trip of a long odd numerator */
        b[0] = '-'; for (k = 1; k < n; k++) b[k] = (char)('0' + (k * 7 + 3) % 10); b[n - 1] = '7';
        c.mensura = n;
        printf("long int ret=%d ", fractio_ex_chorda(c, pp, &f));
        t = fractio_ad_chordam(f, pp);
        printf("rt_equal=%d len=%u\n", t.mensura == n && memcmp(t.datum, b, n) == 0, (unsigned)t.mensura);
        free(b);
    }
    piscina_destruere(pp);
    return 0;
}
