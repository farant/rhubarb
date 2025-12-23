/* probatio_combinado.c - Demo applicatio cum Concha */
#include "concha.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

#define LATITUDO_FENESTRA  DCCCLIII  /* 853 */
#define ALTITUDO_FENESTRA  CDLXXX    /* 480 */

#define VIA_LOG          "data/combinado.log"


/* ==================================================
 * Main
 * ================================================== */

int
main(int argc, char** argv)
{
    Concha*             concha;
    ConchaConfiguratio  config;
    constans character* via_log;
    character           via_temp[CXXVIII];
    b32                 utere_temp;
    i32                 i;

    /* Verificare pro --temp-database vexillo */
    utere_temp = FALSUM;
    per (i = I; i < (i32)argc; i++)
    {
        si (strcmp(argv[i], "--temp-database") == ZEPHYRUM ||
            strcmp(argv[i], "-t") == ZEPHYRUM)
        {
            utere_temp = VERUM;
            frange;
        }
    }

    /* Determinare via ad log file */
    si (utere_temp)
    {
        /* Generare via temporanea unica */
        sprintf(via_temp, "/tmp/rhubarb_%ld.log", (longus)time(NIHIL));
        via_log = via_temp;
        imprimere("Utens database temporanea: %s\n", via_log);
    }
    alioquin si (argc > I && argv[I][ZEPHYRUM] != '-')
    {
        via_log = argv[I];
    }
    alioquin
    {
        via_log = VIA_LOG;
    }

    /* Configurare concha */
    config.titulus = "LibroPaginarum + Navigator Demo";
    config.latitudo = LATITUDO_FENESTRA;
    config.altitudo = ALTITUDO_FENESTRA;
    config.via_persistentia = via_log;

    /* Creare concha */
    concha = concha_creare(&config);
    si (!concha)
    {
        imprimere("Fractura: non potest creare concha\n");
        redde I;
    }

    /* Currere event loop */
    redde concha_currere(concha);
}
