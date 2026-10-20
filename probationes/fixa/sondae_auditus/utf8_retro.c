/* utf8: prior_runa vs proxima_runa boundaries; well-formed fuzz round trip; random-byte walk */
#include "latina.h"
#include "utf8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void ambulare(const char* titulus, const unsigned char* s, int n)
{
    unsigned char* b = (unsigned char*)malloc((size_t)n);
    const i8* p; char fw[256]; char bw[256]; int lf = 0, lb = 0, k = 0;
    memcpy(b, s, (size_t)n);
    fw[0] = bw[0] = 0;
    p = b; lf += sprintf(fw + lf, "%ld", (long)(p - b));
    while (p < b + n && k++ < 50) { p = utf8_proxima_runa(p, b + n); lf += sprintf(fw + lf, ",%ld", (long)(p - b)); }
    p = b + n; k = 0; lb += sprintf(bw + lb, "%ld", (long)(p - b));
    while (p > b && k++ < 50) { p = utf8_prior_runa(p, b); lb += sprintf(bw + lb, ",%ld", (long)(p - b)); }
    printf("%-16s proxima: %-18s prior: %s\n", titulus, fw, bw);
    free(b);
}
#define CASE(t, ...) do { static const unsigned char a_[] = { __VA_ARGS__ }; ambulare(t, a_, (int)sizeof(a_)); } while (0)

static unsigned long semen = 12345;
static unsigned long alea(void) { semen = semen * 1103515245UL + 12345UL; return (semen >> 16) & 0x7FFF; }

int main(void)
{
    int iter, j, mism_wf = 0, mism_rand = 0, stuck = 0;
    CASE("A_80", 0x41, 0x80);
    CASE("C3_A9_80", 0xC3, 0xA9, 0x80);
    CASE("80x5", 0x80, 0x80, 0x80, 0x80, 0x80);
    CASE("A_F0_9F_98_80", 0x41, 0xF0, 0x9F, 0x98, 0x80);
    CASE("C3_41", 0xC3, 0x41);
    CASE("E2_82_AC_80_80", 0xE2, 0x82, 0xAC, 0x80, 0x80);
    /* well-formed fuzz: prior(proxima(p)) == p at every boundary */
    for (iter = 0; iter < 20000; iter++) {
        i8 buf[64]; i8 enc[4]; int n = 0, nb = 0; const i8* bnd[64]; const i8* p;
        while (n < 56) { s32 r; s32 m; unsigned long c = alea() % 4;
            r = (s32)(c == 0 ? alea() % 0x80 : c == 1 ? 0x80 + alea() % 0x780 : c == 2 ? 0x800 + alea() % 0xF800 : 0x10000 + ((alea() << 5) ^ alea()) % 0x100000);
            m = utf8_codere(r, enc); if (!m) continue; memcpy(buf + n, enc, (size_t)m); n += m; }
        p = buf; bnd[nb++] = p;
        while (p < buf + n) { p = utf8_proxima_runa(p, buf + n); bnd[nb++] = p; }
        for (j = 1; j < nb; j++) if (utf8_prior_runa(bnd[j], buf) != bnd[j - 1]) mism_wf++;
    }
    /* random bytes: does prior hit the same boundaries as proxima (reversed)? progress? */
    for (iter = 0; iter < 20000; iter++) {
        i8 buf[16]; int n = 1 + (int)(alea() % 16), nb = 0; const i8* bnd[40]; const i8* p;
        for (j = 0; j < n; j++) buf[j] = (i8)(alea() & 0xFF);
        p = buf; bnd[nb++] = p;
        while (p < buf + n) { const i8* q = utf8_proxima_runa(p, buf + n); if (q <= p) { stuck++; break; } p = q; bnd[nb++] = p; }
        for (j = 1; j < nb; j++) if (utf8_prior_runa(bnd[j], buf) != bnd[j - 1]) { mism_rand++; break; }
        p = buf + n; j = 0; while (p > buf && j++ < 40) { const i8* q = utf8_prior_runa(p, buf); if (q >= p) { stuck++; break; } p = q; }
    }
    printf("wellformed prior(proxima) mismatches=%d  random strings with prior/proxima mismatch=%d/20000  stuck=%d\n",
        mism_wf, mism_rand, stuck);
    return 0;
}
