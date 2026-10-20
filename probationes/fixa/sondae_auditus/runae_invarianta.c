/* runae_invarianta: progressus, limites, consensus ambulationum per
 * textus fortuitos ex signis (runae classium variarum + octeti mali) */
#include "latina.h"
#include "runae.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* const SIGNA[] = {
    "a", "\r", "\n", "\x01", "\x7f", "\x00", "\xC2\x80", "\xC2\x9F",
    "\xCC\x81", "\xE2\x80\x8D", "\xEF\xB8\x8F", "\xEF\xB8\x8E",
    "\xF0\x9F\x91\xA8", "\xF0\x9F\x87\xA6", "\xE0\xA4\x95", "\xE0\xA5\x8D",
    "\xE0\xA4\xBF", "\xE1\x84\x80", "\xE1\x85\xA1", "\xE1\x86\xA8",
    "\xEA\xB0\x80", "\xD8\x80", "\x80", "\xE2", "\xF0", "\xFF", "\xC0",
    "\xE2\x8C\x9A", "#", "\xE2\x80\x8B", "\xF0\x9F\x8F\xBF", "\xE4\xB8\xAD",
    "\xED\xA0\x80", "\xF4\x90\x80\x80", "\xE2\x83\xA3", "\xE1\x85\xA0"
};
#define NSIGNA ((int)(sizeof(SIGNA)/sizeof(SIGNA[0])))

static unsigned long semen = 12345UL;
static unsigned fortuitus(void) { semen = semen * 1103515245UL + 12345UL; return (unsigned)((semen >> 16) & 0x7FFF); }

static int fracturae = 0;
static void fractura(const char* quid, const i8* b, int n)
{
    int k;
    if (fracturae++ < 25) {
        printf("FRACTA %s:", quid);
        for (k = 0; k < n; k++) printf(" %02X", (unsigned)b[k]);
        printf("\n");
    }
}

int main(void)
{
    int iter;
    long probati = 0;
    for (iter = 0; iter < 300000; iter++)
    {
        char tmp[128];
        int n = 0, t, nt = 1 + (int)(fortuitus() % 10);
        i8* buf;
        const i8* fin;
        const i8* cur;
        i32 summa = 0, lat, lat2, col;
        RunaePolitica pol = (fortuitus() & 1) ? RUNAE_POLITICA_SIMPLEX : RUNAE_POLITICA_GRAPHEMATUM;
        for (t = 0; t < nt; t++) {
            const char* s = SIGNA[fortuitus() % NSIGNA];
            int l = (s[0] == 0) ? 1 : (int)strlen(s);
            memcpy(tmp + n, s, (size_t)l); n += l;
        }
        if (fortuitus() % 4 == 0 && n > 1) n -= 1 + (int)(fortuitus() % 2); /* truncatio */
        if (n <= 0) continue;
        buf = (i8*)malloc((size_t)n);
        memcpy(buf, tmp, (size_t)n);
        fin = buf + n;
        probati++;
        /* graphema ambulatio */
        for (cur = buf; cur < fin; ) {
            const i8* nx = runae_graphema_ex_politica(cur, fin, pol, &lat);
            const i8* nx2;
            if (nx <= cur || nx > fin) { fractura("graphema progressus", buf, n); break; }
            if (lat > 2) fractura("graphema latitudo > II", buf, n);
            if (pol == RUNAE_POLITICA_GRAPHEMATUM) {
                nx2 = runae_graphema_proximum(cur, fin, &lat2);
                if (nx2 != nx || lat2 != lat) fractura("proximum != ex_politica", buf, n);
                nx2 = runae_graphema_ex_politica(cur, fin, (RunaePolitica)7, &lat2);
                if (nx2 != nx || lat2 != lat) fractura("politica 7 != GRAPHEMATUM", buf, n);
            }
            cur = nx;
        }
        /* unitas ambulatio + columnae */
        for (cur = buf; cur < fin; ) {
            const i8* nx = runae_unitas_proxima(cur, fin, pol, &lat);
            if (nx <= cur || nx > fin) { fractura("unitas progressus", buf, n); break; }
            if (lat > 2) fractura("unitas latitudo > II", buf, n);
            if (runae_latitudo_textus(buf, cur, pol) != summa) fractura("latitudo_textus praefixi != summa", buf, n);
            if (lat > 0) {
                const i8* r;
                i32 c;
                for (c = summa; c < summa + lat; c++) {
                    r = runae_columnam_quaerere(buf, fin, pol, c, &col);
                    if (r != cur || col != summa) fractura("columnam_quaerere != initium unitatis", buf, n);
                }
            }
            summa += lat;
            cur = nx;
        }
        if (runae_latitudo_textus(buf, fin, pol) != summa) fractura("latitudo_textus != summa unitatum", buf, n);
        {
            const i8* r = runae_columnam_quaerere(buf, fin, pol, summa, &col);
            if (r != fin || col != summa) fractura("columna == tota non finis", buf, n);
            r = runae_columnam_quaerere(buf, fin, pol, 0xFFFFFFFFu, &col);
            if (r != fin || col != summa) fractura("columna 0xFFFFFFFF non finis", buf, n);
        }
        free(buf);
    }
    printf("probati %ld fracturae %d\n", probati, fracturae);
    return 0;
}
