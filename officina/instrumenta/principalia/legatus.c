/* legatus (principale) - involucrum stdio machinae legati
 * (vide instrumenta/legatus.h; probationes machinam ipsam agunt)
 *
 * STDOUT PURUS PROTOCOLLO - nihil hic imprimitur. */

#include "latina.h"
#include "legatus.h"

#include <string.h>

integer
principale (vacuum)
{
    LegatusConfiguratio cfg;

    memset(&cfg, ZEPHYRUM, magnitudo(LegatusConfiguratio));
    redde (integer)legatus_currere(stdin, stdout, &cfg);
}
