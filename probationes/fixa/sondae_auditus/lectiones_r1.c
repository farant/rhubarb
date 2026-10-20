#include "latina.h"
#include "lectiones.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

integer
principale (vacuum)
{
    structura stat st;
    integer r;
    FILE* f;

    setenv("FABRICA_LECTIONES", "build/sonda/lectiones_r1.liber", 1);
    remove("build/sonda/lectiones_r1.liber");

    errno = 0;
    r = stat(NIHIL, &st);
    imprimere("stat crudum(NIHIL) = %d errno=%d\n", r, errno);
    errno = 0;
    r = lectiones_stat(NIHIL, &st);
    imprimere("lectiones_stat(NIHIL) = %d errno=%d\n", r, errno);

    errno = 0;
    f = fopen(NIHIL, "r");
    imprimere("fopen crudum(NIHIL,r) = %p errno=%d\n", (vacuum*)f, errno);
    errno = 0;
    f = lectiones_fopen(NIHIL, "r");
    imprimere("lectiones_fopen(NIHIL,r) = %p errno=%d\n", (vacuum*)f, errno);
    fflush(stdout);
    redde 0;
}
