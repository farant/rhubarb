/* utf8: per case - decodere ret/advance, proxima_runa advance, numerare count,
   number of decodere calls, number of proxima steps (exact malloc buffers) */
#include "latina.h"
#include "utf8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void casus_probare(const char* titulus, const unsigned char* s, int n)
{
    unsigned char* buf = (unsigned char*)malloc((size_t)(n ? n : 1));
    const i8* p; const i8* fin; s32 r; int calls = 0, steps = 0; long adv_d, adv_p;
    memcpy(buf, s, (size_t)n);
    fin = buf + n;
    p = buf; r = utf8_decodere(&p, fin); adv_d = (long)(p - buf);
    adv_p = (long)(utf8_proxima_runa(buf, fin) - buf);
    p = buf; while (p < fin && calls < 100) { (void)utf8_decodere(&p, fin); calls++; }
    p = buf; while (p < fin && steps < 100) { p = utf8_proxima_runa(p, fin); steps++; }
    printf("%-22s dec=%-8d dec_adv=%ld prox_adv=%ld numerare=%d dec_calls=%d prox_steps=%d\n",
        titulus, r, adv_d, adv_p, utf8_numerare_runas(buf, n), calls, steps);
    free(buf);
}
#define CASE(t, ...) do { static const unsigned char a_[] = { __VA_ARGS__ }; casus_probare(t, a_, (int)sizeof(a_)); } while (0)

int main(void)
{
    CASE("C3_41", 0xC3, 0x41);
    CASE("E2_28_A1", 0xE2, 0x28, 0xA1);
    CASE("F0_9F_41_41", 0xF0, 0x9F, 0x41, 0x41);
    CASE("C0_80", 0xC0, 0x80);
    CASE("C0_AF", 0xC0, 0xAF);
    CASE("C1_BF", 0xC1, 0xBF);
    CASE("E0_80_80", 0xE0, 0x80, 0x80);
    CASE("E0_9F_BF", 0xE0, 0x9F, 0xBF);
    CASE("F0_80_80_80", 0xF0, 0x80, 0x80, 0x80);
    CASE("F0_8F_BF_BF", 0xF0, 0x8F, 0xBF, 0xBF);
    CASE("ED_A0_80", 0xED, 0xA0, 0x80);
    CASE("ED_BF_BF", 0xED, 0xBF, 0xBF);
    CASE("F4_90_80_80", 0xF4, 0x90, 0x80, 0x80);
    CASE("F5_80_80_80", 0xF5, 0x80, 0x80, 0x80);
    CASE("F7_BF_BF_BF", 0xF7, 0xBF, 0xBF, 0xBF);
    CASE("F8_80_80_80_80", 0xF8, 0x80, 0x80, 0x80, 0x80);
    CASE("FE", 0xFE);
    CASE("FF", 0xFF);
    CASE("80", 0x80);
    CASE("BF", 0xBF);
    CASE("E2_82_41", 0xE2, 0x82, 0x41);
    CASE("F0_9F_98", 0xF0, 0x9F, 0x98);
    CASE("E2", 0xE2);
    CASE("00", 0x00);
    CASE("41_00_42", 0x41, 0x00, 0x42);
    CASE("7F", 0x7F);
    CASE("C2_80", 0xC2, 0x80);
    CASE("DF_BF", 0xDF, 0xBF);
    CASE("E0_A0_80", 0xE0, 0xA0, 0x80);
    CASE("EF_BF_BE", 0xEF, 0xBF, 0xBE);
    CASE("EF_BF_BF", 0xEF, 0xBF, 0xBF);
    CASE("F0_90_80_80", 0xF0, 0x90, 0x80, 0x80);
    CASE("F4_8F_BF_BF", 0xF4, 0x8F, 0xBF, 0xBF);
    return 0;
}
