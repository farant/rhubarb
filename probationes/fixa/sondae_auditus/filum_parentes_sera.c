#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>

#define DIR_T "build/sonda/filum_tmp_parentes"

static const char* er(void) { const char* e = filum_error_recens(); return e ? e : "NIHIL"; }
static double nunc(void) { struct timeval tv; gettimeofday(&tv, 0); return tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0; }

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    static char via[8192];
    size_t lens[] = {4094, 4095, 4096, 4097};
    int k;
    b32 r;
    system("rm -rf " DIR_T);
    mkdir(DIR_T, 0755);

    for (k = 0; k < 4; k++)
    {
        size_t n = lens[k], i;
        strcpy(via, DIR_T "/");
        for (i = strlen(via); i < n; i++) via[i] = ((i % 50) == 49) ? '/' : 'q';
        via[n] = 0;
        r = filum_directorium_creare_cum_parentibus(via);
        printf("cp%d longitudo=%lu: r=%d error=%s\n", k, (unsigned long)n, (int)r, er());
    }
    r = filum_directorium_creare_cum_parentibus(DIR_T "//a///b//");
    printf("cp5 '//a///b//': r=%d a/b=%d\n", (int)r, (int)filum_directorium_existit(DIR_T "/a/b"));
    r = filum_directorium_creare_cum_parentibus("/");
    printf("cp6 '/': r=%d\n", (int)r);
    r = filum_directorium_creare_cum_parentibus(DIR_T "/x/../y/z");
    printf("cp7 'x/../y/z': r=%d y/z=%d x=%d\n", (int)r, (int)filum_directorium_existit(DIR_T "/y/z"), (int)filum_directorium_existit(DIR_T "/x"));
    { FILE* f = fopen(DIR_T "/planum", "wb"); fclose(f); }
    r = filum_directorium_creare_cum_parentibus(DIR_T "/planum/sub");
    printf("cp8 segmentum medium filum: r=%d error=%s\n", (int)r, er());
    r = filum_directorium_creare_cum_parentibus("");
    printf("cp9 vacua: r=%d error=%s\n", (int)r, er());

    /* sera timings */
    {
        FilumSera* a = filum_seram_capere(DIR_T "/s.sera", 0, p);
        FilumSera* b;
        double t0;
        unsigned moras[] = {0, 1, 5, 100};
        for (k = 0; k < 4; k++)
        {
            t0 = nunc();
            b = filum_seram_capere(DIR_T "/s.sera", moras[k], p);
            printf("se%d mora=%u: sera=%s tempus=%.0f ms\n", k, moras[k], b ? "ok" : "NIHIL", nunc() - t0);
        }
        filum_seram_liberare(a);
        filum_seram_liberare(a);
        filum_seram_liberare(NIHIL);
        b = filum_seram_capere(DIR_T "/s.sera", 0, p);
        printf("se4 post liberare: sera=%s\n", b ? "ok" : "NIHIL");
        filum_seram_liberare(b);
        b = filum_seram_capere(DIR_T, 0, p);
        printf("se5 via directorium: sera=%s\n", b ? "ok" : "NIHIL");
        b = filum_seram_capere(DIR_T "/absens/s.sera", 0, p);
        printf("se6 parens absens: sera=%s\n", b ? "ok" : "NIHIL");
    }
    system("rm -rf " DIR_T);
    piscina_destruere(p);
    return 0;
}
