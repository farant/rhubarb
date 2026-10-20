#include "postulata_posix.h"
#include "latina.h"
#include "tempus.h"
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>
static void casus_probare(const char* tit, f64 fps, int sleep_ante_us)
{
    pid_t p; int st;
    fflush(stdout); fflush(stderr);
    p = fork();
    if (p == 0) {
        f64 a, b;
        alarm(3);
        tempus_initiare();
        tempus_ponere_quadra_destinata(fps);
        if (sleep_ante_us) usleep((unsigned)sleep_ante_us);
        a = tempus_nunc();
        tempus_exspectare_quadrum();
        b = tempus_nunc();
        printf("  [%s] fps=%g exspectavit %.6f s\n", tit, fps, b - a);
        fflush(stdout);
        _exit(0);
    }
    waitpid(p, &st, 0);
    if (WIFSIGNALED(st)) printf("  [%s] fps=%g -> PENDET (signum %d post 3 s)\n", tit, fps, WTERMSIG(st));
}
int main(void)
{
    casus_probare("60 fps", 60.0, 0);
    casus_probare("fps 0", 0.0, 0);
    casus_probare("fps negativus", -30.0, 0);
    casus_probare("fps nan", NAN, 0);
    casus_probare("fps inf", INFINITY, 0);
    casus_probare("fps 60, quadrum iam excessit (50ms)", 60.0, 50000);
    casus_probare("fps 1e-10 (periodus 1e10 s)", 1e-10, 0);
    return 0;
}
