/* alpha.c - FIXUM amalgamae: staticum '_est_spatium' cum beta.c
 * COLLIDIT (renominatio per plagulam probatur); macro ALPHA_MAXIMUM
 * post plagulam #undef */

#include "postulata_posix.h"
#include "alpha.h"
#include <string.h>

#define ALPHA_MAXIMUM 64

interior b32
_est_spatium (
    character c)
{
    redde (b32)(c == ' ' || c == '\t');
}

i32
alpha_numerare (
    constans character* textus)
{
    i32 n = ZEPHYRUM;
    i32 i;
    i32 m = (i32)strlen(textus);

    si (m > ALPHA_MAXIMUM)
    {
        m = ALPHA_MAXIMUM;
    }
    per (i = ZEPHYRUM; i < m; i++)
    {
        si (!_est_spatium(textus[i]))
        {
            n = n + I;
        }
    }
    redde n;
}
