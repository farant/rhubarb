#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include <stdio.h>

static Piscina* pp;
static void show(const char* tag, ChordaAedificator* a, b32 r)
{
    chorda s = chorda_aedificator_spectare(a);
    printf("%s\tr=%d\t[%.*s]\n", tag, (int)r, (int)s.mensura, (char*)s.datum);
}
int main(void)
{
    ChordaAedificator* a;
    pp = piscina_generare_dynamicum("sonda", 65536);
#define T(tag, call) a = chorda_aedificator_creare(pp, 1); show(tag, a, call)
    T("S32_MIN", chorda_aedificator_appendere_s32(a, (s32)(-2147483647 - 1)));
    T("S32_MAX", chorda_aedificator_appendere_s32(a, 2147483647));
    T("S32_0", chorda_aedificator_appendere_s32(a, 0));
    T("S32_-1", chorda_aedificator_appendere_s32(a, -1));
    T("I32_0", chorda_aedificator_appendere_i32(a, 0));
    T("I32_MAX", chorda_aedificator_appendere_i32(a, 4294967295u));
    T("I32_2^31", chorda_aedificator_appendere_i32(a, 2147483648u));
    T("HEX_0", chorda_aedificator_appendere_hex_i32(a, 0));
    T("HEX_FFFFFFFF", chorda_aedificator_appendere_hex_i32(a, 0xFFFFFFFFu));
    T("HEX_10", chorda_aedificator_appendere_hex_i32(a, 0x10));
    T("HEX_DEADBEEF", chorda_aedificator_appendere_hex_i32(a, 0xDEADBEEFu));
    T("REP_0", chorda_aedificator_appendere_repetita(a, 'x', 0));
    T("REP_5", chorda_aedificator_appendere_repetita(a, 'x', 5));
    T("REP_100", chorda_aedificator_appendere_repetita(a, 'y', 100));
    return 0;
}
