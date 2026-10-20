/* sonda r1: tabellarius_lineam_legere - tectum post lineam totam bufferatam? */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tabellarius.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

integer
principale (vacuum)
{
    Piscina* piscina;
    FILE* f;
    character* bloc;
    memoriae_index longitudo;
    memoriae_index ante;
    chorda linea;
    b32 finitus;

    piscina = piscina_generare_dynamicum("sonda", 65536);
    f = tmpfile();
    longitudo = (memoriae_index)TABELLARIUS_CORPUS_MAXIMUM + 1;
    bloc = (character*)malloc(longitudo);
    memset(bloc, 'a', longitudo);
    fwrite(bloc, 1, longitudo, f);
    fputs("\n{\"x\":1}\n", f);
    free(bloc);
    rewind(f);

    ante = piscina_summa_usus(piscina);
    linea = tabellarius_lineam_legere(f, piscina, &finitus);
    printf("linea nimis longa: mensura=%u finitus=%d usus_ante=%lu usus_post=%lu apex=%lu\n",
        (unsigned)linea.mensura, (int)finitus, (unsigned long)ante,
        (unsigned long)piscina_summa_usus(piscina),
        (unsigned long)piscina_summa_apex_usus(piscina));
    linea = tabellarius_lineam_legere(f, piscina, &finitus);
    printf("linea sequens: mensura=%u finitus=%d\n", (unsigned)linea.mensura, (int)finitus);
    fclose(f);
    redde 0;
}
