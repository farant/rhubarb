/* runae_latitudo_graphematis: regula latitudinis graphematum (Ghostty) */
#include "latina.h"
#include "runae.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { const char* tit; const char* b; int n; int exp; } Cg;
static const Cg CG[] = {
  {"Mn solum 0301", "\xCC\x81", 2, 0},
  {"L V T", "\xE1\x84\x80\xE1\x85\xA1\xE1\x86\xA8", 9, 2},
  {"ka i (Mc)", "\xE0\xA4\x95\xE0\xA4\xBF", 6, 2},
  {"CR LF", "\r\n", 2, 0},
  {"manus+mod", "\xF0\x9F\x91\x8B\xF0\x9F\x8F\xBF", 8, 2},
  {"1 FE0F 20E3", "1\xEF\xB8\x8F\xE2\x83\xA3", 7, 2},
  {"a FE0F", "a\xEF\xB8\x8F", 4, 1},
  {"231A FE0E 093F", "\xE2\x8C\x9A\xEF\xB8\x8E\xE0\xA4\xBF", 9, 2},
  {"231A FE0E 0301", "\xE2\x8C\x9A\xEF\xB8\x8E\xCC\x81", 8, 1},
  {"0600 1", "\xD8\x80" "1", 3, 2},
  {"RI solum", "\xF0\x9F\x87\xA6", 4, 2},
  {"vexillum tag", "\xF0\x9F\x8F\xB4\xF3\xA0\x81\xA7\xF3\xA0\x81\xA2\xF3\xA0\x81\xA5\xF3\xA0\x81\xAE\xF3\xA0\x81\xA7\xF3\xA0\x81\xBF", 28, 2},
  {"NUL", "\x00" "a", 2, 0},
  {"ZWJ Mc", "\xE2\x80\x8D\xE0\xA4\xBF", 6, 2},
  {"0301 093F", "\xCC\x81\xE0\xA4\xBF", 5, 2},
  {"115F 1161", "\xE1\x85\x9F\xE1\x85\xA1", 6, 0},
  {"3164", "\xE3\x85\xA4", 3, 0},
  {"FFA0", "\xEF\xBE\xA0", 3, 0},
  {"mod solum", "\xF0\x9F\x8F\xBB", 4, 2},
  {"a mod", "a\xF0\x9F\x8F\xBB", 5, 1},
  {"2764 FE0F", "\xE2\x9D\xA4\xEF\xB8\x8F", 6, 2},
  {"2764 FE0E", "\xE2\x9D\xA4\xEF\xB8\x8E", 6, 1},
  {"2764 FE0F FE0E", "\xE2\x9D\xA4\xEF\xB8\x8F\xEF\xB8\x8E", 9, 2},
  {"e 0301 FE0F", "e\xCC\x81\xEF\xB8\x8F", 6, 1},
  {"C1 0085", "\xC2\x85", 2, 0},
  {"2028", "\xE2\x80\xA8", 3, 0},
  {"zhong 0301", "\xE4\xB8\xAD\xCC\x81", 5, 2},
  {"zhong FE0E", "\xE4\xB8\xAD\xEF\xB8\x8E", 6, 2},
  {"231A FE0E", "\xE2\x8C\x9A\xEF\xB8\x8E", 6, 1},
  {"231A FE0E FE0F", "\xE2\x8C\x9A\xEF\xB8\x8E\xEF\xB8\x8F", 9, 1},
  {"23 20E3 (keycap sine VS)", "#\xE2\x83\xA3", 4, 1},
  {"a 200D 1F468 (a ZWJ EP)", "a\xE2\x80\x8D\xF0\x9F\x91\xA8", 8, 1},
  {"FEFF a", "\xEF\xBB\xBF" "a", 4, 0},
  {"00AD", "\xC2\xAD", 2, 1},
};

int main(void)
{
    int k;
    for (k = 0; k < (int)(sizeof(CG)/sizeof(CG[0])); k++) {
        i8* buf = (i8*)malloc((size_t)CG[k].n);
        const i8* r; i32 lat, latu; const i8* ru;
        memcpy(buf, CG[k].b, (size_t)CG[k].n);
        r = runae_graphema_proximum(buf, buf + CG[k].n, &lat);
        ru = runae_unitas_proxima(buf, buf + CG[k].n, RUNAE_POLITICA_GRAPHEMATUM, &latu);
        printf("%-26s cons %2d lat %u exp %d %s | unitas cons %2d lat %u\n", CG[k].tit,
            (int)(r - buf), (unsigned)lat, CG[k].exp, ((int)lat == CG[k].exp) ? "ok" : "DISCORDIA",
            (int)(ru - buf), (unsigned)latu);
        free(buf);
    }
    {
        static const long R[] = {0x10FFFF, 0x10FFFE, 0x110000, 0x7FFFFFFF, -1, (long)(-2147483647L-1),
            0x3164, 0xFFA0, 0x115F, 0x1160, 0x2065, 0xE0080, 0xE0FFF, 0xE1000, 0x1F6D8, 0x2630,
            0xFFFE, 0xFFFF, 0xFDD0, 0xF0000, 0x30000, 0x3FFFD, 0x2A6E0, 0x4DC0, 0x1F1FF, 0xFFFD,
            0x00A1, 0x2026, 0x0D4E, 0x20DD, 0xD7B0, 0xD7FF, 0xD7FB, 0xD7FC, 0x0080, 0x009F, 0x2029,
            0x1F3FF, 0xE0000, 0xE01EF, 0xE01F0, 0x31350, 0x323AF, 0x323B0, 0x2FFFF, 0x3FFFF, 0x40000, 0x1FFFD};
        int j;
        for (j = 0; j < (int)(sizeof(R)/sizeof(R[0])); j++)
            printf("latitudo %08lX = %u\n", (unsigned long)R[j] & 0xFFFFFFFFUL, (unsigned)runae_latitudo((s32)R[j]));
    }
    return 0;
}
