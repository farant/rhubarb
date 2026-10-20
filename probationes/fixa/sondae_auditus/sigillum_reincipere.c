#include "latina.h"
#include "sigillum.h"
#include <stdio.h>
#include <string.h>
int main(void)
{
    SigillumContextus c; Sigillum s, r = sigillum_computare("hello", 5); int ok;
    memset(&c, 0xA5, sizeof c);
    sigillum_incipere(&c); sigillum_addere(&c, "hello", 5); s = sigillum_finire(&c);
    printf("sordidus_memset ok=%d\n", (int)sigillum_aequale(&s, &r));
    sigillum_incipere(&c); sigillum_addere(&c, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 70);
    sigillum_incipere(&c); sigillum_addere(&c, "hello", 5); s = sigillum_finire(&c);
    ok = sigillum_aequale(&s, &r); printf("medio_reincipere ok=%d\n", ok);
    sigillum_incipere(&c); sigillum_addere(&c, "hello", 5); s = sigillum_finire(&c);
    printf("post_finire_reincipere ok=%d massa=%u totalis=%llu\n", (int)sigillum_aequale(&s, &r), (unsigned)c.massa_mensura, (unsigned long long)c.totalis);
    /* aliasing: datum inside massa */
    {
        SigillumContextus d; Sigillum t, u; unsigned char copy[20];
        sigillum_incipere(&d); sigillum_addere(&d, "0123456789abcdefghij", 20);
        memcpy(copy, d.massa, 20);
        sigillum_addere(&d, d.massa, 20);
        t = sigillum_finire(&d);
        { unsigned char all[40]; memcpy(all, "0123456789abcdefghij", 20); memcpy(all+20, copy, 20); u = sigillum_computare(all, 40); }
        printf("alias_massa ok=%d\n", (int)sigillum_aequale(&t, &u));
    }
    return 0;
}
