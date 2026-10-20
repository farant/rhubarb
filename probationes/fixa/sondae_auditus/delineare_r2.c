#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "delineare.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

static void pendet(int s) { (void)s; write(1, "  -> PENDET (alarm 3s)\n", 23); _exit(7); }

static void in_filio(const char* titulus, int quod)
{
    pid_t pid; int st;
    fflush(stdout);
    pid = fork();
    if (pid == 0)
    {
        Piscina* p = piscina_generare_dynamicum("sonda", 65536);
        TabulaPixelorum t;
        ContextusDelineandi* c;
        Color col = color_ex_rgb(255, 255, 255);
        int k, n = 0;
        t.latitudo = 40; t.altitudo = 40;
        t.pixela = (i32*)piscina_allocare(p, 40 * 40 * sizeof(i32));
        for (k = 0; k < 1600; k++) t.pixela[k] = 0;
        c = delineare_creare_contextum(p, &t);
        signal(SIGALRM, pendet);
        alarm(3);
        printf("%s:\n", titulus); fflush(stdout);
        if (quod == 0) delineare_lineam(c, (i32)-5, 0, 34, 0, col);
        if (quod == 1) delineare_lineam(c, 34, 0, (i32)-5, 0, col);
        if (quod == 2) delineare_lineam(c, 5, 0, 34, 0, col);
        if (quod == 3) delineare_triangulum(c, (i32)-3, 2, 10, 2, 5, 9, col);
        for (k = 0; k < 40; k++) if (t.pixela[k]) n++;
        printf("  -> redditum, pixela in ordine 0: %d\n", n); fflush(stdout);
        _exit(0);
    }
    waitpid(pid, &st, 0);
}

int main(void)
{
    in_filio("lineam((i32)-5,0 -> 34,0)", 0);
    in_filio("lineam(34,0 -> (i32)-5,0) [reversa]", 1);
    in_filio("lineam(5,0 -> 34,0) [control]", 2);
    in_filio("triangulum((i32)-3,2, 10,2, 5,9)", 3);
    return 0;
}
