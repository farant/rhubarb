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

#define DIR_T "build/sonda/filum_tmp_lector"
#define VIA DIR_T "/l.txt"

static void scribe_n(const char* buf, size_t n)
{
    FILE* f = fopen(VIA, "wb"); fwrite(buf, 1, n, f); fclose(f);
}

static void lege_omnes(const char* titulus, Piscina* p)
{
    FilumLector* l = filum_lector_aperire(VIA, p);
    chorda c;
    int n = 0;
    printf("%s:", titulus);
    printf(" finis_ante=%d", (int)filum_lector_finis(l));
    while (filum_lector_lineam_proximam(l, &c))
    {
        printf(" [%u", (unsigned)c.mensura);
        if (c.mensura > 0 && c.mensura < 12) printf(":%.*s", (int)c.mensura, (const char*)c.datum);
        printf("]");
        n++;
    }
    printf(" lineae=%d numerus_versus=%u finis=%d\n", n, (unsigned)filum_lector_numerus_versus(l), (int)filum_lector_finis(l));
    filum_lector_claudere(l);
}

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    static char buf[20000];
    size_t n;
    system("rm -rf " DIR_T);
    mkdir(DIR_T, 0755);

    /* 1: CRLF split at fgets boundary: 4094 'A' + "\r\n" + "B\n" -> oracle 2 lines [4094][1:B] */
    memset(buf, 'A', 4094); memcpy(buf + 4094, "\r\nB\n", 4); n = 4098;
    scribe_n(buf, n);
    lege_omnes("casus1 CRLF trans limitem (oraculum [4094][1:B])", p);

    /* 2: interior CR at position 4095 of a line: 4094 'A' + "\rX\n" -> oracle 1 line of 4096 */
    memset(buf, 'A', 4094); memcpy(buf + 4094, "\rX\n", 3); n = 4097;
    scribe_n(buf, n);
    lege_omnes("casus2 CR interior ad limitem (oraculum [4096])", p);

    /* 2b: same interior CR not at boundary: "a\rX\n" */
    scribe_n("a\rX\n", 4);
    lege_omnes("casus2b CR interior brevis (oraculum [3])", p);

    /* 3: CRLF split at continuation boundary: 4095 + 4094 'A' + "\r\n" + "B\n" */
    memset(buf, 'A', 4095 + 4094); memcpy(buf + 4095 + 4094, "\r\nB\n", 4); n = 4095 + 4094 + 4;
    scribe_n(buf, n);
    lege_omnes("casus3 CRLF trans limitem secundum (oraculum [8189][1:B])", p);

    /* 4: embedded NUL: "ab\0cd\nef\n" -> oracle [5][2:ef] */
    scribe_n("ab\0cd\nef\n", 9);
    lege_omnes("casus4 NUL interior (oraculum [5][2:ef])", p);

    /* 5: NUL in a long line: 10 'A' + NUL + 5000 'B' + "\n" + "Z\n" -> oracle [5011][1:Z] */
    memset(buf, 'A', 10); buf[10] = 0; memset(buf + 11, 'B', 5000); memcpy(buf + 5011, "\nZ\n", 3); n = 5014;
    scribe_n(buf, n);
    lege_omnes("casus5 NUL in linea longa (oraculum [5011][1:Z])", p);

    /* 6: "\r\r\n" trailing CRs */
    scribe_n("abc\r\r\n", 6);
    lege_omnes("casus6 abc CR CR LF", p);

    /* 7: empty file, "\n" only, "\n\n", unterminated */
    scribe_n("", 0);
    lege_omnes("casus7a vacuum", p);
    scribe_n("\n", 1);
    lege_omnes("casus7b LF solum", p);
    scribe_n("\n\n", 2);
    lege_omnes("casus7c LF LF", p);
    scribe_n("x\ny", 3);
    lege_omnes("casus7d sine LF finali", p);

    /* 8: finis after last short line w/o newline vs long line w/o newline */
    {
        FilumLector* l; chorda c;
        scribe_n("x", 1);
        l = filum_lector_aperire(VIA, p);
        filum_lector_lineam_proximam(l, &c);
        printf("casus8a post lineam brevem sine LF: finis=%d\n", (int)filum_lector_finis(l));
        filum_lector_claudere(l);
        memset(buf, 'Q', 5000); scribe_n(buf, 5000);
        l = filum_lector_aperire(VIA, p);
        filum_lector_lineam_proximam(l, &c);
        printf("casus8b post lineam longam sine LF: finis=%d\n", (int)filum_lector_finis(l));
        filum_lector_claudere(l);
    }

    /* 9: 1 MB line */
    {
        static char big[1048577];
        memset(big, 'M', 1048576); big[1048576] = '\n';
        scribe_n(big, 1048577);
        lege_omnes("casus9 linea 1MB (oraculum [1048576])", p);
    }

    system("rm -rf " DIR_T);
    piscina_destruere(p);
    return 0;
}
