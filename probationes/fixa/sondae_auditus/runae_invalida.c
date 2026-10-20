/* runae_invalida: quot octetos series invalida consumit, et latitudines */
#include "latina.h"
#include "runae.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* const TITULI[] = {
    "C0 AF", "C1 BF", "E0 80 AF", "E0 9F BF", "F0 80 80 AF", "F0 8F BF BF",
    "ED A0 80", "ED BF BF", "F4 90 80 80", "F5 80 80 80", "F7 BF BF BF",
    "F8 88 80 80 80", "FC 84 80 80 80 80", "FE", "FF", "80", "BF",
    "E2 41", "E2 82 41", "F0 9F 41", "C2 41", "E0 80 80"
};
static const char* const OCTETI[] = {
    "\xC0\xAF", "\xC1\xBF", "\xE0\x80\xAF", "\xE0\x9F\xBF", "\xF0\x80\x80\xAF", "\xF0\x8F\xBF\xBF",
    "\xED\xA0\x80", "\xED\xBF\xBF", "\xF4\x90\x80\x80", "\xF5\x80\x80\x80", "\xF7\xBF\xBF\xBF",
    "\xF8\x88\x80\x80\x80", "\xFC\x84\x80\x80\x80\x80", "\xFE", "\xFF", "\x80", "\xBF",
    "\xE2\x41", "\xE2\x82\x41", "\xF0\x9F\x41", "\xC2\x41", "\xE0\x80\x80"
};

int main(void)
{
    int k;
    for (k = 0; k < (int)(sizeof(OCTETI) / sizeof(OCTETI[0])); k++)
    {
        int n = (int)strlen(OCTETI[k]);
        i8* buf = (i8*)malloc((size_t)n);
        const i8* fin;
        const i8* cur;
        i32 lat;
        int graphemata = 0, latg = 0;
        memcpy(buf, OCTETI[k], (size_t)n);
        fin = buf + n;
        /* graphema walk */
        cur = buf;
        printf("%-20s graphema:", TITULI[k]);
        while (cur < fin)
        {
            const i8* nx = runae_graphema_proximum(cur, fin, &lat);
            printf(" [%d:%u]", (int)(nx - cur), (unsigned)lat);
            graphemata++; latg += (int)lat;
            cur = nx;
        }
        printf("  sum_graphema %d  latitudo_textus %u\n", latg,
            (unsigned)runae_latitudo_textus(buf, fin, RUNAE_POLITICA_GRAPHEMATUM));
        free(buf);
    }
    return 0;
}
