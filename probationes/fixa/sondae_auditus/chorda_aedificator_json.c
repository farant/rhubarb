#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include <stdio.h>

static Piscina* pp;
static void probe_case(const char* tag, const i8* d, i32 n)
{
    ChordaAedificator* a = chorda_aedificator_creare(pp, 16);
    chorda in, s; b32 r; i32 k;
    in.datum = (i8*)d; in.mensura = n;
    r = chorda_aedificator_appendere_evasus_json(a, in);
    s = chorda_aedificator_spectare(a);
    printf("J\t%s\t%d\t", tag, (int)r);
    for (k = 0; k < n; k++) printf("%02x", (unsigned)(unsigned char)d[k]);
    printf("\t");
    for (k = 0; k < s.mensura; k++) printf("%02x", (unsigned)(unsigned char)s.datum[k]);
    printf("\n");
}

int main(void)
{
    static i8 buf[4096];
    i32 n, k;
    pp = piscina_generare_dynamicum("sonda", 65536);
    /* lengths around the 500-byte stack buffer threshold (n*6 <= 500 -> n <= 83) */
    for (n = 80; n <= 86; n++)
    {
        char tag[32];
        for (k = 0; k < n; k++) buf[k] = (i8)(k % 32);
        sprintf(tag, "ctrl%d", (int)n); probe_case(tag, buf, n);
        for (k = 0; k < n; k++) buf[k] = (i8)"\"\\a\n"[k % 4];
        sprintf(tag, "mix%d", (int)n); probe_case(tag, buf, n);
    }
    for (k = 0; k < 256; k++) buf[k] = (i8)k;
    probe_case("all256", buf, 256);
    probe_case("all256x4", buf, 1024);
    /* utf8: valid é, U+2028, invalid lone continuation, overlong, surrogate */
    probe_case("utf8_e", (const i8*)"\xc3\xa9", 2);
    probe_case("u2028", (const i8*)"\xe2\x80\xa8", 3);
    probe_case("lone_cont", (const i8*)"\x80", 1);
    probe_case("overlong", (const i8*)"\xc0\xaf", 2);
    probe_case("surrogate", (const i8*)"\xed\xa0\x80", 3);
    probe_case("slash", (const i8*)"/", 1);
    probe_case("nul", (const i8*)"a\0b", 3);
    return 0;
}
