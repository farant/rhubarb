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

#define DIR_T "build/sonda/filum_tmp_status"

static const char* er(void) { const char* e = filum_error_recens(); return e ? e : "NIHIL"; }
static unsigned modus(const char* v) { struct stat st; if (stat(v, &st)) return 0xDEAD; return (unsigned)(st.st_mode & 07777); }

int main(void)
{
    FilumStatus s;
    b32 r;
    system("chmod -R u+rwx " DIR_T " 2>/dev/null; rm -rf " DIR_T);
    mkdir(DIR_T, 0755);

    /* status of a root-owned file the caller cannot write */
    r = filum_status("/usr/bin/true", &s);
    printf("st1 /usr/bin/true (root, 0755): r=%d potest_scribere=%d access(W_OK)=%d\n",
        (int)r, (int)s.potest_scribere, (int)(access("/usr/bin/true", W_OK) == 0));
    r = filum_status("/", &s);
    printf("st2 / : potest_scribere=%d access(W_OK)=%d est_dir=%d\n", (int)s.potest_scribere, (int)(access("/", W_OK) == 0), (int)s.est_directorium);

    /* status_out on failure */
    memset(&s, 0x5A, sizeof s);
    r = filum_status(DIR_T "/absens", &s);
    printf("st3 absens: r=%d status_out intactum=%d\n", (int)r, (int)(s.est_filum == (b32)0x5A5A5A5A));

    /* symlink to dir: stat follows */
    mkdir(DIR_T "/scopus", 0755);
    symlink("scopus", DIR_T "/nexus");
    r = filum_status(DIR_T "/nexus", &s);
    printf("st4 nexus->dir: r=%d est_dir=%d\n", (int)r, (int)s.est_directorium);

    /* directorium_creare_cum_modo through a symlink: chmods target */
    r = filum_directorium_creare_cum_modo(DIR_T "/nexus", 0700);
    printf("cm1 cum_modo(nexus->scopus, 0700): r=%d modus_scopi=%o (fuit 755)\n", (int)r, modus(DIR_T "/scopus"));
    /* fresh creation, umask */
    umask(022);
    r = filum_directorium_creare_cum_modo(DIR_T "/novum", 0777);
    printf("cm2 cum_modo(novum,0777) umask 022: r=%d modus=%o\n", (int)r, modus(DIR_T "/novum"));
    /* on a file */
    { FILE* f = fopen(DIR_T "/planum", "wb"); fclose(f); }
    r = filum_directorium_creare_cum_modo(DIR_T "/planum", 0700);
    printf("cm3 cum_modo(filum): r=%d error=%s\n", (int)r, er());
    /* negative mode */
    r = filum_directorium_creare_cum_modo(DIR_T "/neg", -1);
    printf("cm4 cum_modo(neg, -1): r=%d modus=%o\n", (int)r, modus(DIR_T "/neg"));
    r = filum_directorium_creare_cum_modo(DIR_T "/magnus", 010755);
    printf("cm5 cum_modo(magnus, 010755): r=%d modus=%o\n", (int)r, modus(DIR_T "/magnus"));
    r = filum_modum_ponere(DIR_T "/planum", -1);
    printf("mp1 modum_ponere(filum,-1): r=%d modus=%o\n", (int)r, modus(DIR_T "/planum"));
    r = filum_modum_ponere(DIR_T "/absens", 0644);
    printf("mp2 modum_ponere(absens): r=%d error=%s\n", (int)r, er());

    /* si_necesse */
    r = filum_directorium_creare_si_necesse(DIR_T "/planum");
    printf("sn1 si_necesse(filum): r=%d error=%s\n", (int)r, er());
    r = filum_directorium_creare_si_necesse(DIR_T "/nexus");
    printf("sn2 si_necesse(nexus->dir): r=%d\n", (int)r);
    r = filum_directorium_creare(DIR_T "/slash/");
    printf("dc1 creare('slash/'): r=%d dir=%d\n", (int)r, (int)filum_directorium_existit(DIR_T "/slash"));
    printf("de1 directorium_existit(nexus)=%d (filum)=%d\n", (int)filum_directorium_existit(DIR_T "/nexus"), (int)filum_directorium_existit(DIR_T "/planum"));

    /* filum_delere on empty dir, non-empty dir, symlink */
    mkdir(DIR_T "/vacuum_dir", 0755);
    printf("del1 delere(dir vacuum): r=%d\n", (int)filum_delere(DIR_T "/vacuum_dir"));
    printf("del2 delere(dir plenum scopus? no) nexus: r=%d scopus_post=%d\n", (int)filum_delere(DIR_T "/nexus"), (int)filum_directorium_existit(DIR_T "/scopus"));

    system("chmod -R u+rwx " DIR_T " 2>/dev/null; rm -rf " DIR_T);
    return 0;
}
