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

#define DIR_T "build/sonda/filum_tmp_copiare"

static void scribe(const char* via, const char* s)
{
    FILE* f = fopen(via, "wb"); fputs(s, f); fclose(f);
}

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    chorda c;
    b32 r;
    const char* e;
    system("rm -rf " DIR_T);
    mkdir(DIR_T, 0755);

    /* 1: same path */
    scribe(DIR_T "/a.txt", "CONTENTUM PRETIOSUM");
    r = filum_copiare(DIR_T "/a.txt", DIR_T "/a.txt");
    c = filum_legere_totum(DIR_T "/a.txt", p);
    printf("casus1 idem via: redde=%d mensura_post=%u (exspectatum 19 aut FALSUM)\n", (int)r, (unsigned)c.mensura);

    /* 2: ./ spelling */
    scribe(DIR_T "/b.txt", "CONTENTUM PRETIOSUM");
    r = filum_copiare(DIR_T "/b.txt", DIR_T "/./b.txt");
    c = filum_legere_totum(DIR_T "/b.txt", p);
    printf("casus2 ./ via: redde=%d mensura_post=%u\n", (int)r, (unsigned)c.mensura);

    /* 3: hardlink destination */
    scribe(DIR_T "/c.txt", "CONTENTUM PRETIOSUM");
    link(DIR_T "/c.txt", DIR_T "/c_link.txt");
    r = filum_copiare(DIR_T "/c.txt", DIR_T "/c_link.txt");
    c = filum_legere_totum(DIR_T "/c.txt", p);
    printf("casus3 hardlink: redde=%d mensura_fontis_post=%u\n", (int)r, (unsigned)c.mensura);

    /* 4: symlink destination pointing to source */
    scribe(DIR_T "/d.txt", "CONTENTUM PRETIOSUM");
    symlink("d.txt", DIR_T "/d_sym.txt");
    r = filum_copiare(DIR_T "/d.txt", DIR_T "/d_sym.txt");
    c = filum_legere_totum(DIR_T "/d.txt", p);
    printf("casus4 symlink: redde=%d mensura_fontis_post=%u\n", (int)r, (unsigned)c.mensura);

    /* 5: source is a directory */
    mkdir(DIR_T "/subdir", 0755);
    scribe(DIR_T "/dest5.txt", "VETUS DESTINATIO");
    r = filum_copiare(DIR_T "/subdir", DIR_T "/dest5.txt");
    e = filum_error_recens();
    c = filum_legere_totum(DIR_T "/dest5.txt", p);
    printf("casus5 fons directorium: redde=%d error=%s mensura_dest_post=%u\n", (int)r, e ? e : "NIHIL", (unsigned)c.mensura);

    /* 6: empty source */
    scribe(DIR_T "/vacuum.txt", "");
    r = filum_copiare(DIR_T "/vacuum.txt", DIR_T "/dest6.txt");
    printf("casus6 fons vacuus: redde=%d dest_existit=%d\n", (int)r, (int)filum_existit(DIR_T "/dest6.txt"));

    /* 7: dest parent missing */
    r = filum_copiare(DIR_T "/c.txt", DIR_T "/nonest/dest7.txt");
    e = filum_error_recens();
    printf("casus7 parens absens: redde=%d error=%s\n", (int)r, e ? e : "NIHIL");

    /* 8: missing source must not truncate destination */
    scribe(DIR_T "/dest8.txt", "SERVANDUM");
    r = filum_copiare(DIR_T "/absens.txt", DIR_T "/dest8.txt");
    c = filum_legere_totum(DIR_T "/dest8.txt", p);
    printf("casus8 fons absens: redde=%d mensura_dest=%u\n", (int)r, (unsigned)c.mensura);

    system("rm -rf " DIR_T);
    piscina_destruere(p);
    return 0;
}
