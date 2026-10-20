/* sonda: number grammar, s64/f64 limits, writer forms vs Python repr */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Piscina* pisc;

static void legere_(const char* s)
{
    JsonResultus r = json_legere_literis(s, pisc);
    char vis[48];
    size_t n = strlen(s);
    if (n > 40) { memcpy(vis, s, 20); strcpy(vis + 20, "..."); sprintf(vis + 23, "(%lu)", (unsigned long)n); }
    else strcpy(vis, s);
    if (!r.successus) {
        printf("LEGE %-34s FAIL L%u C%u [%.*s]\n", vis, (unsigned)r.linea, (unsigned)r.columna,
            (int)r.error.mensura, (const char*)r.error.datum);
        return;
    }
    if (json_est_integer(r.radix))
        printf("LEGE %-34s INT %lld\n", vis, (long long)json_ad_integer(r.radix));
    else if (json_est_fluitans(r.radix)) {
        chorda w = json_scribere(r.radix, pisc);
        printf("LEGE %-34s FLT %a  scr=%.*s\n", vis, json_ad_fluitans(r.radix), (int)w.mensura, (const char*)w.datum);
    } else printf("LEGE %-34s genus %d\n", vis, (int)json_genus(r.radix));
}

static void scribere_(unsigned long long bits)
{
    double d; chorda w; JsonResultus r;
    memcpy(&d, &bits, 8);
    w = json_scribere(json_fluitans_creare(pisc, d), pisc);
    r = json_legere(w, pisc);
    printf("SCRI %016llx %.*s  redit=%d genus=%d aeq=%d\n", bits, (int)w.mensura, (const char*)w.datum,
        (int)r.successus, r.successus ? (int)json_genus(r.radix) : -1,
        r.successus && json_est_fluitans(r.radix) && memcmp(&r.radix->datum.fluitans_valor, &d, 8) == 0);
}

int main(void)
{
    char* longus_; size_t i;
    pisc = piscina_generare_dynamicum("sonda", 65536);
    legere_("-0"); legere_("-0.0"); legere_("0.0"); legere_("1E5"); legere_("1e2"); legere_("1E+2");
    legere_("-01"); legere_("-0000");
    legere_("9223372036854775807"); legere_("9223372036854775808");
    legere_("-9223372036854775808"); legere_("-9223372036854775809");
    legere_("18446744073709551616"); legere_("-18446744073709551616");
    legere_("123456789012345678901234567890");
    legere_("1e400"); legere_("-1e400"); legere_("1e-400"); legere_("-1e-400");
    legere_("4.9e-324"); legere_("2.4e-324"); legere_("2.5e-324");
    legere_("1.7976931348623157e308"); legere_("1.7976931348623158e308"); legere_("1.7976931348623159e308");
    legere_("2.2250738585072011e-308"); legere_("1e99999999999999999999"); legere_("1e-99999999999999999999");
    legere_("0e400");
    /* 1 followed by 400 zeros then .0 */
    longus_ = (char*)malloc(20000);
    longus_[0] = '1'; for (i = 1; i <= 400; i++) longus_[i] = '0'; strcpy(longus_ + 401, ".0");
    legere_(longus_);
    /* 0.000...0001 with 5000 zeros */
    strcpy(longus_, "0."); for (i = 2; i < 5002; i++) longus_[i] = '0'; strcpy(longus_ + 5002, "1");
    legere_(longus_);
    /* 1.000...0001 with 15000 digits */
    strcpy(longus_, "1."); for (i = 2; i < 15002; i++) longus_[i] = '0'; strcpy(longus_ + 15002, "1");
    legere_(longus_);
    /* 25-digit integer > s64 as fraction mantissa */
    legere_("12345678901234567890.5");
    free(longus_);
    /* writer: denormals, extremes, integral floats */
    scribere_(0x0000000000000001ULL); /* 5e-324 */
    scribere_(0x0000000000000002ULL); /* 1e-323 */
    scribere_(0x000000000000000aULL);
    scribere_(0x00000000000003e8ULL);
    scribere_(0x0000000012345678ULL);
    scribere_(0x000fffffffffffffULL); /* largest denormal */
    scribere_(0x0010000000000000ULL); /* DBL_MIN */
    scribere_(0x7fefffffffffffffULL); /* DBL_MAX */
    scribere_(0x8000000000000000ULL); /* -0.0 */
    scribere_(0x4341c37937e08000ULL); /* 1e16 */
    scribere_(0x430c6bf526340000ULL); /* 1e15 */
    scribere_(0x3fb999999999999aULL); /* 0.1 */
    scribere_(0x3fd5555555555555ULL); /* 1/3 */
    scribere_(0x4415af1d78b58c40ULL); /* 1e20 */
    scribere_(0x44b52d02c7e14af6ULL); /* 1e23 */
    scribere_(0x3ff0000000000001ULL); /* 1+eps */
    scribere_(0xc340000000000000ULL); /* -2^53 */
    scribere_(0x7ff8000000000000ULL); /* nan */
    scribere_(0x7ff0000000000000ULL); /* inf */
    return 0;
}
