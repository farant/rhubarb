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
#include <limits.h>

#define DIR_T "build/sonda/filum_tmp_arbor"

static void scribe(const char* via, const char* s)
{
    FILE* f = fopen(via, "wb"); fputs(s, f); fclose(f);
}

int main(void)
{
    b32 r;
    const char* e;
    char cwd[4096];
    char abs_t[4096];
    int k;
    system("chmod -R u+rwx " DIR_T " 2>/dev/null; rm -rf " DIR_T);
    mkdir(DIR_T, 0755);
    getcwd(cwd, sizeof cwd);
    snprintf(abs_t, sizeof abs_t, "%s/%s", cwd, DIR_T);

    /* 1: 'nexus/' trailing slash to a dir with a precious file */
    mkdir(DIR_T "/aliena", 0755);
    scribe(DIR_T "/aliena/testis.txt", "testis");
    mkdir(DIR_T "/arbor", 0755);
    symlink("../aliena", DIR_T "/arbor/nexus");
    r = filum_arborem_delere(DIR_T "/arbor/nexus/");
    e = filum_error_recens();
    printf("casus1 'nexus/': redde=%d error=%s testis_post=%d nexus_lstat=%d aliena_dir=%d\n",
        (int)r, e ? e : "NIHIL",
        (int)filum_existit(DIR_T "/aliena/testis.txt"),
        (int)(access(DIR_T "/arbor/nexus", F_OK) == 0 || 1),
        (int)filum_directorium_existit(DIR_T "/aliena"));

    /* 2: via itself is a symlink with trailing slash, whole */
    mkdir(DIR_T "/aliena2", 0755);
    scribe(DIR_T "/aliena2/testis.txt", "testis");
    symlink("aliena2", DIR_T "/nexus2");
    r = filum_arborem_delere(DIR_T "/nexus2/.");
    e = filum_error_recens();
    printf("casus2 'nexus2/.': redde=%d error=%s testis_post=%d\n",
        (int)r, e ? e : "NIHIL", (int)filum_existit(DIR_T "/aliena2/testis.txt"));

    /* 3: '.' relative to cwd */
    mkdir(DIR_T "/punctum", 0755);
    scribe(DIR_T "/punctum/x.txt", "x");
    mkdir(DIR_T "/punctum/sub", 0755);
    scribe(DIR_T "/punctum/sub/y.txt", "y");
    chdir(DIR_T "/punctum");
    r = filum_arborem_delere(".");
    e = filum_error_recens();
    printf("casus3 '.': redde=%d error=%s x_post=%d sub_post=%d\n",
        (int)r, e ? e : "NIHIL", (int)filum_existit("x.txt"), (int)filum_directorium_existit("sub"));
    chdir(cwd);

    /* 4: deep nesting beyond PATH_MAX (macOS 1024) */
    chdir(DIR_T);
    mkdir("profundum", 0755);
    chdir("profundum");
    for (k = 0; k < 600; k++) { mkdir("a", 0755); chdir("a"); }
    scribe("folium.txt", "f");
    chdir(abs_t);
    r = filum_arborem_delere("profundum");
    e = filum_error_recens();
    printf("casus4 profundum 600 (via ~%d bytes, PATH_MAX=%d): redde=%d error=%s radix_post=%d\n",
        10 + 600*2, (int)PATH_MAX, (int)r, e ? e : "NIHIL", (int)filum_directorium_existit("profundum"));
    chdir(cwd);

    /* 5: permission denied mid-tree */
    mkdir(DIR_T "/clausa", 0755);
    mkdir(DIR_T "/clausa/sub", 0755);
    scribe(DIR_T "/clausa/sub/z.txt", "z");
    scribe(DIR_T "/clausa/w.txt", "w");
    chmod(DIR_T "/clausa/sub", 0500);
    r = filum_arborem_delere(DIR_T "/clausa");
    e = filum_error_recens();
    printf("casus5 permissio negata: redde=%d error=%s\n", (int)r, e ? e : "NIHIL");
    chmod(DIR_T "/clausa/sub", 0755);

    system("cd " DIR_T " && chmod -R u+rwx . ; cd ..; rm -rf filum_tmp_arbor");
    return 0;
}
