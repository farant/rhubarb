/* energia_posix.c - suggestus sine lectione pilae: nulla pila (vide
 * energia.h). Variant aedilis post macos. */

#include "energia.h"

#include <string.h>

b32
energia_status (
    EnergiaStatus* status)
{
    si (!status)
    {
        redde FALSUM;
    }
    memset(status, ZEPHYRUM, magnitudo(*status));
    redde FALSUM;
}
