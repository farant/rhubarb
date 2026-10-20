/* runae_simplex: politica SIMPLEX - unitates et latitudines */
#include "latina.h"
#include "runae.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void ambula(const char* tit, const char* b, RunaePolitica pol)
{
    int n = (int)strlen(b);
    i8* buf = (i8*)malloc((size_t)n);
    const i8* cur; i32 lat;
    memcpy(buf, b, (size_t)n);
    printf("%-34s %s:", tit, pol == RUNAE_POLITICA_SIMPLEX ? "SIMPLEX" : "GRAPHEM");
    for (cur = buf; cur < buf + n; ) {
        const i8* nx = runae_graphema_ex_politica(cur, buf + n, pol, &lat);
        printf(" [%d:%u]", (int)(nx - cur), (unsigned)lat);
        cur = nx;
    }
    printf(" | textus %u\n", (unsigned)runae_latitudo_textus(buf, buf + n, pol));
    free(buf);
}

int main(void)
{
    static const char* const T[][2] = {
        {"vexillum US (RI RI)", "\xF0\x9F\x87\xBA\xF0\x9F\x87\xB8"},
        {"manus + tonus", "\xF0\x9F\x91\x8B\xF0\x9F\x8F\xBD"},
        {"medicus (man ZWJ 2695 FE0F)", "\xF0\x9F\x91\xA8\xE2\x80\x8D\xE2\x9A\x95\xEF\xB8\x8F"},
        {"familia + tonus", "\xF0\x9F\x91\xA8\xF0\x9F\x8F\xBB\xE2\x80\x8D\xF0\x9F\x91\xA9\xF0\x9F\x8F\xBF"},
        {"vexillum iris (1F3F3 FE0F ZWJ 1F308)", "\xF0\x9F\x8F\xB3\xEF\xB8\x8F\xE2\x80\x8D\xF0\x9F\x8C\x88"},
        {"cor ignis (2764 FE0F ZWJ 1F525)", "\xE2\x9D\xA4\xEF\xB8\x8F\xE2\x80\x8D\xF0\x9F\x94\xA5"},
        {"keycap #", "#\xEF\xB8\x8F\xE2\x83\xA3"},
        {"ka i", "\xE0\xA4\x95\xE0\xA4\xBF"},
        {"ka virama ta", "\xE0\xA4\x95\xE0\xA5\x8D\xE0\xA4\xA4"},
        {"EP ZWJ (finis)", "\xF0\x9F\x91\xA8\xE2\x80\x8D"},
        {"EP ZWJ ZWJ EP", "\xF0\x9F\x91\xA8\xE2\x80\x8D\xE2\x80\x8D\xF0\x9F\x91\xA8"},
        {"a ZWJ EP", "a\xE2\x80\x8D\xF0\x9F\x91\xA8"},
        {"EP ZWJ 0301 EP", "\xF0\x9F\x91\xA8\xE2\x80\x8D\xCC\x81\xF0\x9F\x91\xA8"},
        {"vexillum tag", "\xF0\x9F\x8F\xB4\xF3\xA0\x81\xA7\xF3\xA0\x81\xA2\xF3\xA0\x81\xA5\xF3\xA0\x81\xAE\xF3\xA0\x81\xA7\xF3\xA0\x81\xBF"},
    };
    int k;
    for (k = 0; k < (int)(sizeof(T)/sizeof(T[0])); k++) {
        ambula(T[k][0], T[k][1], RUNAE_POLITICA_GRAPHEMATUM);
        ambula(T[k][0], T[k][1], RUNAE_POLITICA_SIMPLEX);
    }
    return 0;
}
