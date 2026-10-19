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
#include <sys/wait.h>

#define DIR_T "build/sonda/filum_tmp_exhausta"

static void scribe_n(const char* via, const char* buf, size_t n)
{
    FILE* f = fopen(via, "wb"); fwrite(buf, 1, n, f); fclose(f);
}

static void in_filio(const char* titulus, int quod)
{
    pid_t pid;
    int st;
    fflush(stdout);
    pid = fork();
    if (pid == 0)
    {
        Piscina* p = piscina_generare_certae_magnitudinis("certa", 256);
        printf("%s:\n", titulus); fflush(stdout);
        if (quod == 0) { chorda c = filum_legere_totum(DIR_T "/magnum.txt", p); printf("  -> REDDIT mensura=%u\n", (unsigned)c.mensura); }
        if (quod == 1) { FilumLector* l = filum_lector_aperire(DIR_T "/magnum.txt", p); printf("  -> REDDIT lector=%p\n", (void*)l); }
        if (quod == 2) {
            Piscina* q = piscina_generare_certae_magnitudinis("certa2", 8192);
            FilumLector* l = filum_lector_aperire(DIR_T "/magnum.txt", q);
            chorda c; b32 r;
            printf("  lector=%p\n", (void*)l); fflush(stdout);
            r = filum_lector_lineam_proximam(l, &c);
            printf("  -> REDDIT r=%d mensura=%u\n", (int)r, (unsigned)c.mensura);
        }
        if (quod == 3) {
            /* exhaust p then capture a lock */
            FilumSera* s;
            while (piscina_conari_allocare(p, 8)) {}
            s = filum_seram_capere(DIR_T "/sera", 0, p);
            printf("  -> REDDIT sera=%p\n", (void*)s);
        }
        fflush(stdout);
        _exit(0);
    }
    waitpid(pid, &st, 0);
    printf("  status filii: exited=%d code=%d\n", WIFEXITED(st), WIFEXITED(st) ? WEXITSTATUS(st) : -1);
}

int main(void)
{
    static char buf[10000];
    system("rm -rf " DIR_T);
    mkdir(DIR_T, 0755);
    memset(buf, 'x', sizeof buf);
    scribe_n(DIR_T "/magnum.txt", buf, sizeof buf);
    in_filio("casus1 legere_totum 10000 B in piscina certa 256 (contractus: mensura=0)", 0);
    in_filio("casus2 lector_aperire in piscina certa 256 (contractus: NIHIL)", 1);
    in_filio("casus3 lineam_proximam 10000 B linea in piscina certa 8192 (contractus: FALSUM)", 2);
    in_filio("casus4 seram_capere piscina exhausta (contractus: NIHIL)", 3);
    system("rm -rf " DIR_T);
    return 0;
}
