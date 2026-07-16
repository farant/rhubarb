/* legatus (principale) - involucrum stdio machinae legati
 * (vide instrumenta/legatus.h; probationes machinam ipsam agunt)
 *
 * STDOUT PURUS PROTOCOLLO - nihil hic imprimitur. */

#include "latina.h"
#include "legatus.h"

#include <string.h>

integer
principale (integer argc, character** argv)
{
    LegatusConfiguratio cfg;
    integer i;

    memset(&cfg, ZEPHYRUM, magnitudo(LegatusConfiguratio));
    si (argc > ZEPHYRUM)
    {
        /* vigilia sui: binarium residens fontibus posterius
         * cognoscit (excubitor chunk 3) */
        cfg.binarium_via = argv[ZEPHYRUM];
    }
    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-mcp") == ZEPHYRUM)
        {
            cfg.modus_mcp = VERUM;
        }
        alioquin si (strcmp(argv[i], "-radix") == ZEPHYRUM
            && i + I < argc)
        {
            cfg.radix = argv[i + I];
            i++;
        }
        alioquin si (strcmp(argv[i], "-signum") == ZEPHYRUM
            && i + I < argc)
        {
            cfg.signum = argv[i + I];
            i++;
        }
        alioquin si (strcmp(argv[i], "-manifestum") == ZEPHYRUM
            && i + I < argc)
        {
            cfg.via_manifesti = argv[i + I];
            i++;
        }
    }
    redde (integer)legatus_currere(stdin, stdout, &cfg);
}
