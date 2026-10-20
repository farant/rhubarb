/* runae_exhaustivum: omnes series 1-2 octetorum, et 3-4 octetorum ex
 * alphabeto selecto; progressus, limites, consensus summae */
#include "latina.h"
#include "runae.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static long fr = 0, probati = 0;
static void proba(const unsigned char* src, int n)
{
    i8* buf = (i8*)malloc((size_t)n);
    const i8* fin = buf + n; const i8* cur; i32 lat, summa = 0, col;
    int pol;
    memcpy(buf, src, (size_t)n);
    probati++;
    for (pol = 0; pol < 2; pol++) {
        summa = 0;
        for (cur = buf; cur < fin; ) {
            const i8* nx = runae_graphema_ex_politica(cur, fin, (RunaePolitica)pol, &lat);
            if (nx <= cur || nx > fin || lat > 2) { if (fr++ < 10) printf("FRACTA graphema n=%d %02X\n", n, src[0]); break; }
            cur = nx;
        }
        for (cur = buf; cur < fin; ) {
            const i8* nx = runae_unitas_proxima(cur, fin, (RunaePolitica)pol, &lat);
            if (nx <= cur || nx > fin || lat > 2) { if (fr++ < 10) printf("FRACTA unitas n=%d %02X\n", n, src[0]); break; }
            if (lat > 0) {
                const i8* r = runae_columnam_quaerere(buf, fin, (RunaePolitica)pol, summa + lat - 1, &col);
                if (r != cur || col != summa) { if (fr++ < 10) printf("FRACTA columna n=%d\n", n); }
            }
            summa += lat; cur = nx;
        }
        if (runae_latitudo_textus(buf, fin, (RunaePolitica)pol) != summa) { if (fr++ < 10) printf("FRACTA summa\n"); }
    }
    free(buf);
}

int main(void)
{
    static const unsigned char A[] = {0x00,0x0D,0x0A,0x20,0x41,0x7F,0x80,0x81,0x8D,0x9F,0xA0,0xA4,0xB8,0xBF,
        0xC0,0xC1,0xC2,0xCC,0xD8,0xDF,0xE0,0xE1,0xE2,0xE4,0xEA,0xED,0xEF,0xF0,0xF3,0xF4,0xF5,0xF8,0xFE,0xFF,0x8F,0x90,0x9E,0x87};
    int na = (int)sizeof(A), a, b, c, d;
    unsigned char s[4];
    for (a = 0; a < 256; a++) { s[0] = (unsigned char)a; proba(s, 1);
        for (b = 0; b < 256; b++) { s[1] = (unsigned char)b; proba(s, 2); } }
    for (a = 0; a < na; a++) for (b = 0; b < na; b++) for (c = 0; c < na; c++) {
        s[0]=A[a]; s[1]=A[b]; s[2]=A[c]; proba(s, 3);
        for (d = 0; d < na; d++) { s[3]=A[d]; proba(s, 4); } }
    printf("probati %ld fracturae %ld\n", probati, fr);
    return 0;
}
