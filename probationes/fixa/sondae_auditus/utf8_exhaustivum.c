/* utf8: exhaustive invariants of decodere vs codere (canonical self-oracle) */
#include "latina.h"
#include "utf8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static long viol_range = 0, viol_canon = 0, viol_adv = 0, viol_rt = 0, accepted = 0;
static long viol_noadv = 0;

static void check(const unsigned char* s, int n)
{
    unsigned char* buf = (unsigned char*)malloc((size_t)n);
    const i8* p; const i8* fin; s32 r; i8 enc[4]; s32 en; int k; long adv;
    memcpy(buf, s, (size_t)n);
    p = buf; fin = buf + n;
    r = utf8_decodere(&p, fin);
    adv = (long)(p - buf);
    if (adv < 1 || adv > n) { viol_noadv++; }
    if (r != -1 && (r < 0 || r > 0x10FFFF || (r >= 0xD800 && r <= 0xDFFF))) {
        if (viol_range < 5) printf("RANGE: %02x.. -> %x\n", s[0], r);
        viol_range++;
    }
    if (r >= 0) {
        accepted++;
        en = utf8_codere(r, enc);
        if (en != adv) { viol_adv++; }
        else {
            for (k = 0; k < en; k++) if (enc[k] != buf[k]) break;
            if (k != en) { if (viol_canon < 5) printf("NONCANON: lead %02x -> %x\n", s[0], r); viol_canon++; }
        }
    }
    free(buf);
}

int main(void)
{
    unsigned char s[4]; unsigned a, b, c, d; s32 r; i8 enc[4]; s32 n;
    const i8* p;
    /* all 1-byte, 2-byte, 3-byte sequences (exact-size buffers) */
    for (a = 0; a < 256; a++) { s[0] = (unsigned char)a; check(s, 1); }
    for (a = 0; a < 256; a++) for (b = 0; b < 256; b++) { s[0]=(unsigned char)a; s[1]=(unsigned char)b; check(s, 2); }
    for (a = 0xC0; a < 256; a++) for (b = 0; b < 256; b++) for (c = 0; c < 256; c++) {
        s[0]=(unsigned char)a; s[1]=(unsigned char)b; s[2]=(unsigned char)c; check(s, 3); }
    /* 4-byte: leads F0..FF, conts 80..BF plus a few non-conts */
    for (a = 0xF0; a < 256; a++) for (b = 0x7F; b <= 0xC0; b++) for (c = 0x7F; c <= 0xC0; c++) for (d = 0x7F; d <= 0xC0; d++) {
        s[0]=(unsigned char)a; s[1]=(unsigned char)b; s[2]=(unsigned char)c; s[3]=(unsigned char)d; check(s, 4); }
    printf("accepted=%ld range_viol=%ld noncanon=%ld adv_mismatch=%ld no_progress_or_overrun=%ld\n",
        accepted, viol_range, viol_canon, viol_adv, viol_noadv);
    /* round trip every scalar */
    for (r = 0; r <= 0x10FFFF; r++) {
        s32 got;
        if (r >= 0xD800 && r <= 0xDFFF) { if (utf8_codere(r, enc) != 0) viol_rt++; continue; }
        n = utf8_codere(r, enc);
        if (n != (r < 0x80 ? 1 : r < 0x800 ? 2 : r < 0x10000 ? 3 : 4)) { viol_rt++; continue; }
        p = enc; got = utf8_decodere(&p, enc + n);
        if (got != r || p != enc + n) viol_rt++;
    }
    printf("roundtrip_viol=%ld\n", viol_rt);
    return 0;
}
