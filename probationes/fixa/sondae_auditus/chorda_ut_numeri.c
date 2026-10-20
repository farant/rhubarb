#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include <stdio.h>
static chorda lit(const char* c, Piscina* p) { return chorda_ex_literis(c, p); }
int main(void) {
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    const char* in_i32[] = {" -18446744073709551615", "\t-18446744069414584321", " -0", "-0", "+5", " 7",
        "4294967295", "4294967296", "18446744073709551615", " -1", "0x10", "", "7 ", NIHIL};
    const char* in_s32[] = {"+", "-", "   ", "- 1", "+-1", "0x10", "007", " 12", "12 ", "12\n",
        "-2147483648", "-2147483649", "99999999999999999999", NIHIL};
    const char* in_s64[] = {"+", "-", " 1", "1 ", "-0", "+0", "-0009223372036854775808", "-0009223372036854775809",
        "000000000000000000009", "9223372036854775807", "09223372036854775807", "9223372036854775808",
        "+9223372036854775807", "00000000000000000000000000", NIHIL};
    const char* in_f64[] = {"inf", "-infinity", "nan", "NAN(123)", "0x1p3", "1e999", "1e-400", ".5", "5.", "1e", "e5", "-",
        " 1.5", "1.5 ", "4.9406564584124654e-324", NIHIL};
    i32 k;
    setvbuf(stdout, NIHIL, _IONBF, 0);
    for (k = 0; in_i32[k]; k++) { i32 v = 777; b32 ok = chorda_ut_i32(lit(in_i32[k], p), &v);
        printf("ut_i32 \"%s\" -> ok=%d v=%u\n", in_i32[k], ok, v); }
    for (k = 0; in_s32[k]; k++) { s32 v = 777; b32 ok = chorda_ut_s32(lit(in_s32[k], p), &v);
        printf("ut_s32 \"%s\" -> ok=%d v=%d\n", in_s32[k], ok, v); }
    for (k = 0; in_s64[k]; k++) { s64 v = 777; b32 ok = chorda_ut_s64(lit(in_s64[k], p), &v);
        printf("ut_s64 \"%s\" -> ok=%d v=%lld\n", in_s64[k], ok, v); }
    for (k = 0; in_f64[k]; k++) { f64 v = 777; b32 ok = chorda_ut_f64(lit(in_f64[k], p), &v);
        printf("ut_f64 \"%s\" -> ok=%d v=%.17g\n", in_f64[k], ok, v); }
    return 0;
}
