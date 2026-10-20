/* runae_cesu: CESU-8 et praefixa truncata sub graphemate */
#include "latina.h"
#include "runae.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static void ambula(const char* tit, const char* b, int n)
{
    i8* buf = (i8*)malloc((size_t)n);
    const i8* cur; const i8* fin; i32 lat; int summa = 0, cellae = 0;
    memcpy(buf, b, (size_t)n); cur = buf; fin = buf + n;
    printf("%-22s", tit);
    while (cur < fin) { const i8* nx = runae_graphema_proximum(cur, fin, &lat);
        printf(" [%d:%u]", (int)(nx - cur), (unsigned)lat); summa += (int)lat; cellae++; cur = nx; }
    printf("  graphema %d  textus %u\n", summa, (unsigned)runae_latitudo_textus(buf, fin, RUNAE_POLITICA_GRAPHEMATUM));
    free(buf);
}
int main(void)
{
    ambula("CESU ED A0 BD ED B8 80", "\xED\xA0\xBD\xED\xB8\x80", 6);
    ambula("E9 41", "\xE9\x41", 2);
    ambula("x C0 AF y", "x\xC0\xAFy", 4);
    ambula("E2 82 41", "\xE2\x82\x41", 3);
    ambula("E2 82 E2 82 AC", "\xE2\x82\xE2\x82\xAC", 5);
    return 0;
}
