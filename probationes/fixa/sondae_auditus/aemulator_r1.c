#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "aemulator.h"
#include <stdio.h>
#include <string.h>

interior vacuum
lineam_monstrare(Aemulator* a, constans character* titulus)
{
    AemulatorCellula c;
    i32 x;
    imprimere("%s:", titulus);
    per (x = 0; x < 6; x++)
    {
        aemulator_cellula(a, x, 0, &c);
        imprimere(" [");
        {
            i32 k;
            per (k = 0; k < c.graphema.mensura; k++)
                imprimere("%02X", (insignatus integer)(c.graphema.datum[k] & 0xFF));
        }
        imprimere("]");
    }
    imprimere("\n");
}

interior Aemulator*
novum(Piscina* p)
{
    AemulatorConfiguratio cfg;
    aemulator_configuratio_initiare(&cfg);
    redde aemulator_creare(p, &cfg);
}

integer principale(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    Aemulator* a;
    AemulatorCellula c;
    i32 y;

    /* claim 1: una vocatio */
    a = novum(p);
    aemulator_scribere(a, (constans i8*)"\xF0" "ABC", 4);
    lineam_monstrare(a, "una   F0 A B C");
    /* scissa: F0 | ABC */
    a = novum(p);
    aemulator_scribere(a, (constans i8*)"\xF0", 1);
    aemulator_scribere(a, (constans i8*)"ABC", 3);
    lineam_monstrare(a, "scissa F0|ABC ");
    /* scissa brevis: F0 | A | BC */
    a = novum(p);
    aemulator_scribere(a, (constans i8*)"\xF0", 1);
    aemulator_scribere(a, (constans i8*)"A", 1);
    aemulator_scribere(a, (constans i8*)"BC", 2);
    lineam_monstrare(a, "scissa F0|A|BC");
    /* valid split control: E2 82 | AC */
    a = novum(p);
    aemulator_scribere(a, (constans i8*)"\xE2\x82", 2);
    aemulator_scribere(a, (constans i8*)"\xAC" "Z", 2);
    lineam_monstrare(a, "euro scissa   ");

    /* claim 2: scripta, deinde EL */
    a = novum(p);
    aemulator_scribere(a, (constans i8*)"AB\r\x1b[K", 6);
    aemulator_cellula(a, 0, 0, &c);
    imprimere("post EL: mensura %u (scripta erat)\n", (insignatus integer)c.graphema.mensura);

    /* claim 3: DECSTBM 1..5 (altitudo 24), LF in imo regionis */
    a = novum(p);
    aemulator_scribere(a, (constans i8*)"\x1b[1;5r", 6);
    imprimere("historia ante: %u\n", (insignatus integer)aemulator_historia(a));
    aemulator_scribere(a, (constans i8*)"\x1b[5;1HX\n\n\n", 10);
    imprimere("historia post LF in regione 1..5: %u\n", (insignatus integer)aemulator_historia(a));
    aemulator_scribere(a, (constans i8*)"\x1b[2S", 4);
    imprimere("historia post SU 2 in regione: %u\n", (insignatus integer)aemulator_historia(a));
    (vacuum)y;
    redde 0;
}
