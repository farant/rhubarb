#include "latina.h"
#include "sigillum.h"
#include <stdio.h>
/* state after finire; finire twice */
int main(void)
{
    SigillumContextus c;
    Sigillum s1, s2;
    char h1[SIGILLUM_HEX_MENSURA], h2[SIGILLUM_HEX_MENSURA];
    sigillum_incipere(&c);
    sigillum_addere(&c, "abc", 3);
    s1 = sigillum_finire(&c);
    sigillum_hex(&s1, h1);
    printf("post finire: massa_mensura=%u totalis=%llu hex=%s\n", (unsigned)c.massa_mensura, (unsigned long long)c.totalis, h1);
    fflush(stdout);
    s2 = sigillum_finire(&c);
    sigillum_hex(&s2, h2);
    printf("finire bis: massa_mensura=%u hex=%s aequale=%d\n", (unsigned)c.massa_mensura, h2, (int)sigillum_aequale(&s1,&s2));
    return 0;
}
