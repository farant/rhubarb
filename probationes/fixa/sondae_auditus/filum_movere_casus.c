#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define DIR_T "build/sonda/filum_tmp_movere"
static const char* er(void) { const char* e = filum_error_recens(); return e ? e : "NIHIL"; }
static void scribe(const char* via, const char* s) { FILE* f = fopen(via, "wb"); fputs(s, f); fclose(f); }

int main(void)
{
    b32 r;
    system("rm -rf " DIR_T);
    mkdir(DIR_T, 0755);
    scribe(DIR_T "/a", "AAAA");
    r = filum_movere(DIR_T "/a", DIR_T "/a");
    printf("mv1 idem: r=%d mensura=%lu\n", (int)r, (unsigned long)filum_mensura(DIR_T "/a"));
    scribe(DIR_T "/b", "B");
    r = filum_movere(DIR_T "/a", DIR_T "/b");
    printf("mv2 destinatio existit: r=%d mensura_b=%lu (superscripta)\n", (int)r, (unsigned long)filum_mensura(DIR_T "/b"));
    mkdir(DIR_T "/d", 0755);
    r = filum_movere(DIR_T "/d", DIR_T "/d/sub");
    printf("mv3 dir in subdir suum: r=%d error=%s\n", (int)r, er());
    r = filum_movere(DIR_T "/absens", DIR_T "/c");
    printf("mv4 absens: r=%d error=%s\n", (int)r, er());
    r = filum_directorium_creare(DIR_T "/d");
    printf("dc1 creare existens: r=%d error=%s\n", (int)r, er());
    r = filum_directorium_creare(DIR_T "/nonest/sub");
    printf("dc2 parens absens: r=%d error=%s\n", (int)r, er());
    scribe(DIR_T "/d/f", "f");
    r = filum_delere(DIR_T "/d");
    printf("del1 dir non vacuum: r=%d error=%s\n", (int)r, er());
    r = filum_arborem_delere(DIR_T "/absens");
    printf("ad1 absens: r=%d\n", (int)r);
    system("rm -rf " DIR_T);
    return 0;
}
