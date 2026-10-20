#include "postulata_posix.h"
#include "latina.h"
#include "tempus.h"
#include "piscina.h"
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

static void casus_probare(const char* tit, f32 v, f32 lo, f32 hi)
{
    pid_t p; int st;
    fflush(stdout);
    p = fork();
    if (p == 0) {
        f32 r;
        alarm(2);
        r = circumvolvere(v, lo, hi);
        printf("  [%s] circumvolvere(%g,%g,%g) = %.9g\n", tit, (double)v, (double)lo, (double)hi, (double)r);
        fflush(stdout);
        _exit(0);
    }
    waitpid(p, &st, 0);
    if (WIFSIGNALED(st)) printf("  [%s] circumvolvere(%g,%g,%g) -> PENDET (signum %d post 2 s)\n", tit, (double)v, (double)lo, (double)hi, WTERMSIG(st));
}

int main(void)
{
    casus_probare("normalis", 7.5f, 0.0f, 2.0f);
    casus_probare("negativus", -0.5f, 0.0f, 1.0f);
    casus_probare("aequalis max", 1.0f, 0.0f, 1.0f);
    casus_probare("min==max, v<min", -1.0f, 0.0f, 0.0f);
    casus_probare("min==max, v==min", 0.0f, 0.0f, 0.0f);
    casus_probare("min>max", 0.5f, 1.0f, 0.0f);
    casus_probare("valor magnus 1e9 in [0,2pi)", 1e9f, 0.0f, 6.2831853f);
    casus_probare("valor 1e8 in [0,1)", 1e8f, 0.0f, 1.0f);
    casus_probare("valor 1e5 in [0,1)", 1e5f, 0.0f, 1.0f);
    casus_probare("+inf", (f32)INFINITY, 0.0f, 1.0f);
    casus_probare("nan", (f32)NAN, 0.0f, 1.0f);
    casus_probare("-1e-8 in [0,1)", -1e-8f, 0.0f, 1.0f);
    return 0;
}
