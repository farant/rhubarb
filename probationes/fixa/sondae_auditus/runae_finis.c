/* runae_finis: series truncatae ad finem blocci malloc exacti */
#include "latina.h"
#include "runae.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void probare(const char* titulus, const char* b, int n)
{
    i8* buf = (i8*)malloc((size_t)n);
    const i8* fin;
    const i8* r;
    i32 lat, col;
    unsigned w;
    memcpy(buf, b, (size_t)n);
    fin = buf + n;
    r = runae_graphema_proximum(buf, fin, &lat);
    printf("%-14s graphema: cons %d lat %u", titulus, (int)(r - buf), (unsigned)lat);
    r = runae_graphema_ex_politica(buf, fin, RUNAE_POLITICA_SIMPLEX, &lat);
    printf(" | simplex cons %d lat %u", (int)(r - buf), (unsigned)lat);
    r = runae_unitas_proxima(buf, fin, RUNAE_POLITICA_GRAPHEMATUM, &lat);
    printf(" | unitas cons %d lat %u", (int)(r - buf), (unsigned)lat);
    w = (unsigned)runae_latitudo_textus(buf, fin, RUNAE_POLITICA_GRAPHEMATUM);
    printf(" | textus %u", w);
    r = runae_columnam_quaerere(buf, fin, RUNAE_POLITICA_GRAPHEMATUM, 0xFFFFFFFFu, &col);
    printf(" | col_max off %d col %u\n", (int)(r - buf), (unsigned)col);
    free(buf);
}

int main(void)
{
    probare("E2 82", "\xE2\x82", 2);
    probare("E2", "\xE2", 1);
    probare("F0", "\xF0", 1);
    probare("F0 9F", "\xF0\x9F", 2);
    probare("F0 9F 98", "\xF0\x9F\x98", 3);
    probare("65 CC", "\x65\xCC", 2);
    probare("65 E2 83", "\x65\xE2\x83", 3);
    probare("C2", "\xC2", 1);
    probare("smile ok", "\xF0\x9F\x98\x80", 4);
    probare("a smile cut", "a\xF0\x9F\x98", 4);
    probare("ZWJ cut", "\xF0\x9F\x91\xA8\xE2\x80\x8D\xF0\x9F\x91", 10);
    probare("ZWJ end", "\xF0\x9F\x91\xA8\xE2\x80\x8D", 7);
    probare("RI cut", "\xF0\x9F\x87\xA6\xF0\x9F\x87", 7);
    probare("ka virama cut", "\xE0\xA4\x95\xE0\xA5\x8D\xE0\xA4", 8);
    probare("CR", "\r", 1);
    probare("e VS16 cut", "#\xEF\xB8", 3);
    return 0;
}
