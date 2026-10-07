/* clonatio_macos.c - clonatio per clonefile(2) (vide clonatio.h) */

#include "postulata_posix.h"
#include "clonatio.h"
#include <sys/clonefile.h>

b32
clonatio_facere (
    constans character* fons,
    constans character* destinatio)
{
    si (fons == NIHIL || destinatio == NIHIL)
    {
        redde FALSUM;
    }
    /* vexilla 0: nexus symbolici sequuntur ut copia; possessor et
     * permissiones clonantur */
    redde clonefile(fons, destinatio, 0) == 0;
}
