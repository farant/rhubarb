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

    memset(&cfg, ZEPHYRUM, magnitudo(LegatusConfiguratio));
    si (argc > ZEPHYRUM)
    {
        /* vigilia sui: binarium residens fontibus posterius
         * cognoscit (excubitor chunk 3) */
        cfg.binarium_via = argv[ZEPHYRUM];
    }
    redde (integer)legatus_currere(stdin, stdout, &cfg);
}
