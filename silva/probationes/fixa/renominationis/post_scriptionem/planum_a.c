/* fixura renominationis A: staticum + usus + homonyma omnium
 * generum (membrum, macro, titulus salti) */
#include "planum_caput.h"

#define GEMINUM(x) ((x) + (x))
#define VOCA_ADIUTOREM iuvans(5)
#define NUMQUAM_VOCATUM adiutor(9)

#ifdef PHANTASMA_NON_DEFINITUM
int umbra(void) { return adiutor(7); }
static const char* dictum = "adiutor dicit salve";
#endif

static int iuvans(int n);

static int
iuvans(int n)
{
    return GEMINUM(n) + res_communis;
}

int
publica(int n)
{
    int fructus = iuvans(n);
    struct saccus s;

    s.adiutor = fructus;          /* membrum - manet */
    if (n > 0)
    {
        goto adiutor;             /* titulus salti - manet */
    }
    fructus += VOCA_ADIUTOREM;    /* per expansionem - manualis */
adiutor:
    return s.adiutor + fructus;
}
