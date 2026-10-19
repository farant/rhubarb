/* utf8: random exact-size heap buffers - ASan watches every read; counts disagreements */
#include "latina.h"
#include "utf8.h"
#include <stdio.h>
#include <stdlib.h>

static unsigned long semen = 777;
static unsigned long alea(void) { semen = semen * 1103515245UL + 12345UL; return (semen >> 16) & 0x7FFF; }

int main(void)
{
    int iter, j; long dis_np = 0, bad_ret = 0, stuck = 0;
    for (iter = 0; iter < 200000; iter++) {
        int n = 1 + (int)(alea() % 12); i8* b = (i8*)malloc((size_t)n); const i8* p; int calls = 0, steps = 0; s32 cnt;
        for (j = 0; j < n; j++) { unsigned long c = alea() % 5; b[j] = (i8)(c == 0 ? 0x80 + alea() % 0x40 : c == 1 ? 0xC0 + alea() % 0x40 : alea() & 0xFF); }
        p = b; while (p < b + n) { const i8* o = p; s32 r = utf8_decodere(&p, b + n); if (p <= o || p > b + n) { stuck++; break; } if (r < -1 || r > 0x10FFFF) bad_ret++; calls++; }
        p = b; while (p < b + n) { const i8* q = utf8_proxima_runa(p, b + n); if (q <= p || q > b + n) { stuck++; break; } p = q; steps++; }
        p = b + n; while (p > b) { const i8* q = utf8_prior_runa(p, b); if (q >= p || q < b) { stuck++; break; } p = q; }
        cnt = utf8_numerare_runas(b, n);
        if (cnt != calls) bad_ret++;
        if (cnt != steps) dis_np++;
        free(b);
    }
    printf("200000 random buffers: bad_ret_or_numerare!=decodere_calls=%ld  numerare!=proxima_steps=%ld  stuck/overrun=%ld\n", bad_ret, dis_np, stuck);
    return 0;
}
