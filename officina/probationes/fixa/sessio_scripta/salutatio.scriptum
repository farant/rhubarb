#!/usr/bin/env colloquium
#include "piscina.h"
#include "chorda.h"

integer principale(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("scriptum", 4096);
    chorda salutatio = chorda_ex_literis("ave ex scripto", p);
    FILE* pl;

    printf("%.*s (%d octeti)\n", (int)salutatio.mensura,
        (constans character*)salutatio.datum,
        (int)salutatio.mensura);

    pl = fopen("officina/build/salutatio_scripti.txt", "w");
    si (pl)
    {
        fprintf(pl, "scriptum scripsit\n");
        fclose(pl);
    }
    pl = fopen("officina/build/salutatio_scripti.txt", "r");
    si (pl)
    {
        character linea[64];

        si (fgets(linea, 64, pl))
        {
            printf("relectum: %s", linea);
        }
        fclose(pl);
    }
    redde 0;
}
