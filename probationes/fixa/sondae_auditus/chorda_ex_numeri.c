#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include <stdio.h>
#include <float.h>
#include <math.h>
int main(void) {
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    i64 b[] = {0ULL, 5ULL, 512ULL, 999ULL, 1000ULL, 1023ULL, 1024ULL, 1536ULL, 999999ULL, 1048575ULL, 1048576ULL,
        1073741824ULL, 2147483648ULL, 1099511627776ULL, 18446744073709551615ULL};
    f64 fv[6];
    s32 s32v[] = {(-2147483647 - 1), -1, 0, 2147483647};
    s64 s64v[] = {(-9223372036854775807LL - 1LL), -1LL, 0LL, 9223372036854775807LL};
    i32 k;
    setvbuf(stdout, NIHIL, _IONBF, 0);
    for (k = 0; k < (i32)(sizeof b / sizeof b[0]); k++) {
        chorda c = chorda_ex_bytes_legibilis(b[k], p);
        printf("bytes %llu -> \"%.*s\"\n", b[k], (int)c.mensura, c.datum ? (char*)c.datum : "");
    }
    fv[0] = NAN; fv[1] = INFINITY; fv[2] = -INFINITY; fv[3] = -0.0; fv[4] = 4.9406564584124654e-324; fv[5] = DBL_MAX;
    for (k = 0; k < 6; k++) {
        chorda e = chorda_ex_f64_exacta(fv[k], p);
        chorda f6 = chorda_ex_f64(fv[k], 6, p);
        f64 back = 12345; b32 ok = chorda_ut_f64(e, &back);
        printf("f64[%u] exacta=\"%.*s\" ex_f64(6)=\"%.*s\"(len%u) roundtrip ok=%d same=%d signbit=%d\n", k,
            (int)e.mensura, e.datum ? (char*)e.datum : "", (int)f6.mensura, f6.datum ? (char*)f6.datum : "", f6.mensura,
            ok, (back == fv[k]) || (back != back && fv[k] != fv[k]), signbit(back) != 0);
    }
    { chorda c = chorda_ex_f64(1.5, 0, p); printf("ex_f64(1.5,0)=\"%.*s\"\n", (int)c.mensura, (char*)c.datum); }
    { chorda c = chorda_ex_f64(1.5, 51, p); printf("ex_f64(1.5,51) len=%u\n", c.mensura); }
    { chorda c = chorda_ex_f64(1.5, 4294967295U, p); printf("ex_f64(1.5,0xFFFFFFFF) len=%u\n", c.mensura); }
    for (k = 0; k < 4; k++) { chorda c = chorda_ex_s32(s32v[k], p); s32 v = 7; b32 ok = chorda_ut_s32(c, &v);
        printf("ex_s32 %d -> \"%.*s\" rt ok=%d eq=%d\n", s32v[k], (int)c.mensura, (char*)c.datum, ok, v == s32v[k]); }
    for (k = 0; k < 4; k++) { chorda c = chorda_ex_s64(s64v[k], p); s64 v = 7; b32 ok = chorda_ut_s64(c, &v);
        printf("ex_s64 %lld -> \"%.*s\" rt ok=%d eq=%d\n", s64v[k], (int)c.mensura, (char*)c.datum, ok, v == s64v[k]); }
    return 0;
}
