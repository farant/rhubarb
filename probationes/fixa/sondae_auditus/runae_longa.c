/* runae_longa: graphemata longissima (Zalgo, catena ZWJ, RI longi) */
#include "latina.h"
#include "runae.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void metire(const char* tit, const char* caput, const char* unitas_rep, int reps)
{
    int lc = (int)strlen(caput), lu = (int)strlen(unitas_rep), n = lc + lu * reps, k;
    i8* buf = (i8*)malloc((size_t)n);
    const i8* r; i32 lat; clock_t t0; double s; unsigned tot; int graphemata = 0; const i8* cur;
    memcpy(buf, caput, (size_t)lc);
    for (k = 0; k < reps; k++) memcpy(buf + lc + k * lu, unitas_rep, (size_t)lu);
    t0 = clock();
    r = runae_graphema_proximum(buf, buf + n, &lat);
    tot = (unsigned)runae_latitudo_textus(buf, buf + n, RUNAE_POLITICA_GRAPHEMATUM);
    for (cur = buf; cur < buf + n; graphemata++) { i32 l2; cur = runae_graphema_ex_politica(cur, buf + n, RUNAE_POLITICA_SIMPLEX, &l2); if (l2 > 2) printf("  SIMPLEX lat > II!\n"); }
    s = (double)(clock() - t0) / CLOCKS_PER_SEC;
    printf("%-22s octeti %7d: graphema primum cons %7d lat %u | textus %u | SIMPLEX graphemata %d | %.3fs\n",
        tit, n, (int)(r - buf), (unsigned)lat, tot, graphemata, s);
    free(buf);
}

int main(void)
{
    metire("Zalgo a+0301 x200k", "a", "\xCC\x81", 200000);
    metire("Zalgo a+093F x50k", "a", "\xE0\xA4\xBF", 50000);
    metire("ZWJ catena x20k", "\xF0\x9F\x91\xA8", "\xE2\x80\x8D\xF0\x9F\x91\xA8", 20000);
    metire("RI x10000", "", "\xF0\x9F\x87\xA6", 10000);
    metire("RI x10001", "", "\xF0\x9F\x87\xA6", 10001);
    metire("KA (VR KA) x20k", "\xE0\xA4\x95", "\xE0\xA5\x8D\xE0\xA4\x95", 20000);
    metire("VS16 x50k on 2764", "\xE2\x9D\xA4", "\xEF\xB8\x8F", 50000);
    return 0;
}
