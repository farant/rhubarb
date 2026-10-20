#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define DIR_T "build/sonda/filum_tmp_legere"

static void scribe_n(const char* via, const char* buf, size_t n)
{
    FILE* f = fopen(via, "wb"); fwrite(buf, 1, n, f); fclose(f);
}

static void pendet(int s) { (void)s; write(1, "  -> PENDET (alarm 2s)\n", 23); _exit(7); }

/* run fn in child with 2s alarm */
static void in_filio(const char* titulus, int quod)
{
    pid_t pid;
    int st;
    fflush(stdout);
    pid = fork();
    if (pid == 0)
    {
        Piscina* p = piscina_generare_dynamicum("filius", 4096);
        signal(SIGALRM, pendet);
        alarm(2);
        printf("%s:\n", titulus); fflush(stdout);
        if (quod == 0) printf("  -> redde=%d\n", (int)filum_existit(DIR_T "/fifo"));
        if (quod == 1) printf("  -> redde=%lu\n", (unsigned long)filum_mensura(DIR_T "/fifo"));
        if (quod == 2) { chorda c = filum_legere_totum(DIR_T "/fifo", p); printf("  -> mensura=%u\n", (unsigned)c.mensura); }
        if (quod == 3) { FilumLector* l = filum_lector_aperire(DIR_T "/fifo", p); printf("  -> lector=%p\n", (void*)l); }
        if (quod == 4) { FilumStatus s; printf("  -> redde=%d\n", (int)filum_status(DIR_T "/fifo", &s)); }
        fflush(stdout);
        _exit(0);
    }
    waitpid(pid, &st, 0);
}

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    chorda c;
    const char* e;
    system("rm -rf " DIR_T);
    mkdir(DIR_T, 0755);

    /* 1: empty file */
    scribe_n(DIR_T "/vacuum.txt", "", 0);
    c = filum_legere_totum(DIR_T "/vacuum.txt", p);
    e = filum_error_recens();
    printf("casus1 filum vacuum: mensura=%u datum=%p error=%s\n", (unsigned)c.mensura, (void*)c.datum, e ? e : "NIHIL");

    /* 2: missing file */
    c = filum_legere_totum(DIR_T "/absens.txt", p);
    e = filum_error_recens();
    printf("casus2 absens: mensura=%u error=%s\n", (unsigned)c.mensura, e ? e : "NIHIL");

    /* 3: directory */
    mkdir(DIR_T "/dir", 0755);
    c = filum_legere_totum(DIR_T "/dir", p);
    e = filum_error_recens();
    printf("casus3 directorium legere_totum: mensura=%u error=%s\n", (unsigned)c.mensura, e ? e : "NIHIL");
    printf("casus3b filum_existit(dir)=%d filum_mensura(dir)=%lu\n", (int)filum_existit(DIR_T "/dir"), (unsigned long)filum_mensura(DIR_T "/dir"));

    /* 4: embedded NUL */
    scribe_n(DIR_T "/nul.bin", "ab\0cd", 5);
    c = filum_legere_totum(DIR_T "/nul.bin", p);
    printf("casus4 NUL interior: mensura=%u byte4=%c\n", (unsigned)c.mensura, c.mensura == 5 ? c.datum[4] : '?');

    /* 5: /dev/zero, /dev/null */
    c = filum_legere_totum("/dev/null", p);
    e = filum_error_recens();
    printf("casus5 /dev/null: mensura=%u error=%s\n", (unsigned)c.mensura, e ? e : "NIHIL");

    /* 6: dangling symlink */
    symlink("nusquam", DIR_T "/pendens");
    printf("casus6 nexus pendens: existit=%d mensura=%lu dir_existit=%d\n",
        (int)filum_existit(DIR_T "/pendens"), (unsigned long)filum_mensura(DIR_T "/pendens"),
        (int)filum_directorium_existit(DIR_T "/pendens"));

    /* 7: empty string via */
    c = filum_legere_totum("", p);
    printf("casus7 via vacua: mensura=%u existit=%d\n", (unsigned)c.mensura, (int)filum_existit(""));

    /* 8: FIFO with no writer */
    mkfifo(DIR_T "/fifo", 0644);
    in_filio("casus8a filum_existit(FIFO)", 0);
    in_filio("casus8b filum_mensura(FIFO)", 1);
    in_filio("casus8c filum_legere_totum(FIFO)", 2);
    in_filio("casus8d filum_lector_aperire(FIFO)", 3);
    in_filio("casus8e filum_status(FIFO)", 4);

    system("rm -rf " DIR_T);
    piscina_destruere(p);
    return 0;
}
