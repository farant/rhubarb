#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    FilumLector* l;
    chorda c;
    b32 r;
    FILE* f = fopen("build/sonda/filum_tmp_lpc.txt", "wb");
    fputs("una\nduae\n", f); fclose(f);
    l = filum_lector_aperire("build/sonda/filum_tmp_lpc.txt", p);
    r = filum_lector_lineam_proximam(l, &c);
    printf("ante claudere: r=%d mensura=%u\n", (int)r, (unsigned)c.mensura);
    filum_lector_claudere(l);
    filum_lector_claudere(l);
    printf("claudere iteratum: superfuit\n");
    printf("NIHIL lector: r=%d finis=%d numerus=%u\n", (int)filum_lector_lineam_proximam(NIHIL, &c), (int)filum_lector_finis(NIHIL), (unsigned)filum_lector_numerus_versus(NIHIL));
    printf("lineam_proximam post claudere (exspectatum FALSUM, ut scriptor 'scriptor invalidus'):\n");
    fflush(stdout);
    remove("build/sonda/filum_tmp_lpc.txt");
    r = filum_lector_lineam_proximam(l, &c);
    printf("  -> r=%d\n", (int)r);
    piscina_destruere(p);
    return 0;
}
