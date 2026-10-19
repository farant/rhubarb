#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include <stdio.h>
int main(void) {
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    const char* s16 = "0::1::2::3::4::5::6::7::8::9::10::11::12::13::14::15::16";
    const char* s40 = "0::1::2::3::4::5::6::7::8::9::10::11::12::13::14::15::16::17::18::19::20::21::22::23::24::25::26::27::28::29::30::31::32::33::34::35::36";
    chorda_fissio_fructus f;
    chorda d = chorda_ex_literis("::", p), r;
    i32 k; b32 ok = VERUM;
    setvbuf(stdout, NIHIL, _IONBF, 0);
    f = chorda_fissio_chorda(chorda_ex_literis(s16, p), d, p);
    for (k = 0; k < f.numerus; k++) { s32 v; ok = ok && chorda_ut_s32(f.elementa[k], &v) && v == (s32)k; }
    printf("fissio_chorda 17 fields numerus=%u ok=%d (expect 17,1)\n", f.numerus, ok);
    ok = VERUM;
    f = chorda_fissio_chorda(chorda_ex_literis(s40, p), d, p);
    for (k = 0; k < f.numerus; k++) { s32 v; ok = ok && chorda_ut_s32(f.elementa[k], &v) && v == (s32)k; }
    printf("fissio_chorda 37 fields numerus=%u ok=%d (expect 37,1)\n", f.numerus, ok);
    r = chorda_serpens(chorda_ex_literis("aBcDeFgHiJkLmNoPqRsTuVwXyZaBcDeFgHiJkL", p), p);
    printf("serpens 20 words -> \"%.*s\" len=%u\n", (int)r.mensura, (char*)r.datum, r.mensura);
    return 0;
}
