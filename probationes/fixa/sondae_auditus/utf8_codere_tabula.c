/* utf8: codere edge values + written-bytes extent; longitudo_byte / est_continuatio over all 256 */
#include "latina.h"
#include "utf8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int main(void)
{
    static const s32 vals[] = { -1, INT_MIN, INT_MAX, 0x110000, 0xD7FF, 0xD800, 0xDBFF, 0xDC00, 0xDFFF, 0xE000, 0, 0x7F, 0x80, 0x7FF, 0x800, 0xFFFF, 0x10000, 0x10FFFF, 0x7FFFFFF };
    int i, k; unsigned b;
    int lb_bad = 0, ec_bad = 0;
    for (i = 0; i < (int)(sizeof(vals) / sizeof(vals[0])); i++) {
        i8 buf[8]; s32 n; int touched = 0;
        memset(buf, 0xAA, sizeof(buf));
        n = utf8_codere(vals[i], buf);
        for (k = 0; k < 8; k++) if (buf[k] != 0xAA) touched = k + 1;
        printf("codere(%#x) n=%d highest_byte_touched=%d bytes=", (unsigned)vals[i], n, touched);
        for (k = 0; k < n; k++) printf("%02X", buf[k]);
        printf("\n");
    }
    /* exact-size heap buffer of n bytes: ASan catches writes beyond n */
    { s32 r; for (r = 0; r <= 0x10FFFF; r += 0x101) { i8 tmp[4]; s32 n = utf8_codere(r, tmp); if (n) { i8* h = (i8*)malloc((size_t)n); (void)utf8_codere(r, h); free(h); } } }
    printf("lb:");
    for (b = 0; b < 256; b++) {
        s32 e = b < 0x80 ? 1 : b < 0xC0 ? 0 : b < 0xE0 ? 2 : b < 0xF0 ? 3 : b < 0xF8 ? 4 : 0;
        s32 got = utf8_longitudo_byte((i8)b);
        if (got != e) lb_bad++;
        if (b == 0xC0 || b == 0xC1 || (b >= 0xF5 && b <= 0xF7)) printf(" %02X->%d", b, got);
        if (utf8_est_continuatio((i8)b) != ((b & 0xC0) == 0x80 ? 1 : 0)) ec_bad++;
    }
    printf("\nlongitudo_byte table mismatches=%d est_continuatio mismatches=%d\n", lb_bad, ec_bad);
    return 0;
}
