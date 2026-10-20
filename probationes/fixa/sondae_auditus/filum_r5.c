#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    chorda lib;
    int r;
    remove("build/sonda/tmp_r/r5_liber.txt");
    filum_scribere_literis("build/sonda/tmp_r/r5_fons.txt", "contentum fontis\n");
    remove("build/sonda/tmp_r/r5_dest.txt");
    setenv("FABRICA_LECTIONES", "build/sonda/tmp_r/r5_liber.txt", 1);
    r = filum_copiare("build/sonda/tmp_r/r5_fons.txt", "build/sonda/tmp_r/r5_dest.txt");
    printf("copiare successus redde=%d\n", r);
    r = filum_copiare("build/sonda/tmp_r/r5_absens.txt", "build/sonda/tmp_r/r5_dest2.txt");
    printf("copiare fons absens redde=%d\n", r);
    (void)filum_legere_totum("build/sonda/tmp_r/r5_fons.txt", p);
    unsetenv("FABRICA_LECTIONES");
    lib = filum_legere_totum("build/sonda/tmp_r/r5_liber.txt", p);
    printf("--- liber lectionum ---\n%.*s--- finis ---\n", (int)lib.mensura, lib.datum ? (const char*)lib.datum : "");
    return 0;
}
