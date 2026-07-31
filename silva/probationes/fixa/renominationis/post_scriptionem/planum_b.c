/* fixura renominationis B: staticum HOMONYMUM (entitas ALIA) +
 * definitio externae */
#include "planum_caput.h"

int res_communis = 0;

static int adiutor(int n);

static int
adiutor(int n)
{
    return n + res_communis;
}

int
alia_functio(void)
{
    return adiutor(2) + publica(3);
}
