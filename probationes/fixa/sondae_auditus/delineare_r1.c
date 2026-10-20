#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "delineare.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

static void pendet(int s) { (void)s; write(1, "  -> PENDET (alarm 3s)\n", 23); _exit(7); }

static ContextusDelineandi* ctx_facere(Piscina* p, TabulaPixelorum* t)
{
    t->latitudo = 40; t->altitudo = 40;
    t->pixela = (i32*)piscina_allocare(p, 40 * 40 * sizeof(i32));
    { int k; for (k = 0; k < 1600; k++) t->pixela[k] = 0; }
    return delineare_creare_contextum(p, t);
}

static void in_filio(const char* titulus, int quod)
{
    pid_t pid; int st;
    fflush(stdout);
    pid = fork();
    if (pid == 0)
    {
        Piscina* p = piscina_generare_dynamicum("sonda", 65536);
        TabulaPixelorum t;
        ContextusDelineandi* c = ctx_facere(p, &t);
        Color col = color_ex_rgb(255, 255, 255);
        signal(SIGALRM, pendet);
        alarm(3);
        printf("%s:\n", titulus); fflush(stdout);
        if (quod == 0) delineare_circulum_plenum(c, 0, 10, 1, col);
        if (quod == 1) delineare_rectangulum(c, 0, 0, 5, 1, col);
        if (quod == 2) delineare_ellipsin_plenam(c, 0, 10, 2, 2, col);
        if (quod == 3) delineare_circulum_plenum(c, 20, 20, 5, col);
        if (quod == 4) delineare_lineam_horizontalem(c, (i32)-2, 3, 5, col);
        printf("  -> redditum\n"); fflush(stdout);
        _exit(0);
    }
    waitpid(pid, &st, 0);
}

int main(void)
{
    in_filio("circulum_plenum(ctx, 0,10, r=1) [centrum in margine]", 0);
    in_filio("rectangulum(ctx, 0,0, 5x1) [altitudo 1]", 1);
    in_filio("ellipsin_plenam(ctx, 0,10, 2,2)", 2);
    in_filio("circulum_plenum(ctx, 20,20, r=5) [control]", 3);
    in_filio("lineam_horizontalem(ctx, (i32)-2, 3, 5) [~4e9 iter]", 4);
    return 0;
}
