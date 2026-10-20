#include "latina.h"
#include "sigillum.h"
#include <stdio.h>
/* addere one byte after finire without incipere */
int main(void)
{
    SigillumContextus c;
    Sigillum s1;
    sigillum_incipere(&c);
    sigillum_addere(&c, "abc", 3);
    s1 = sigillum_finire(&c);
    (void)s1;
    printf("ante addere: massa_mensura=%u\n", (unsigned)c.massa_mensura);
    fflush(stdout);
    sigillum_addere(&c, "Z", 1);
    printf("post addere 'Z' (0x5a): massa_mensura=%u (expected <=63)\n", (unsigned)c.massa_mensura);
    return 0;
}
