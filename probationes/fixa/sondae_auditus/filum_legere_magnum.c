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

#define VIA "build/sonda/filum_tmp_magnum.bin"

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    chorda c;
    FILE* f;
    unsigned long long quattuor = 4294967296ULL;
    f = fopen(VIA, "wb");
    fseeko(f, (off_t)quattuor, SEEK_SET);
    fwrite("xyz", 1, 3, f);
    fclose(f);
    printf("filum_mensura=%llu (oraculum %llu)\n", (unsigned long long)filum_mensura(VIA), quattuor + 3);
    c = filum_legere_totum(VIA, p);
    printf("legere_totum: mensura=%u (oraculum %llu aut 0 si defectu) error=%s\n",
        (unsigned)c.mensura, quattuor + 3, filum_error_recens() ? filum_error_recens() : "NIHIL");
    if (c.mensura > 0 && c.mensura < 16)
        printf("  bytes: %02x %02x %02x (cauda vera 'xyz' = 78 79 7a)\n",
            (unsigned)c.datum[0], (unsigned)c.datum[1], (unsigned)c.datum[2]);
    remove(VIA);
    piscina_destruere(p);
    return 0;
}
