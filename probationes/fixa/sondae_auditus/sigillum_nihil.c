#include "latina.h"
#include "sigillum.h"
#include <stdio.h>
int main(void)
{
    char hex[SIGILLUM_HEX_MENSURA];
    Sigillum s; SigillumContextus c;
    s = sigillum_computare(NIHIL, 0); sigillum_hex(&s, hex); printf("computare(NIHIL,0) %s\n", hex);
    s = sigillum_computare("", 0); sigillum_hex(&s, hex); printf("computare(\"\",0) %s\n", hex);
    sigillum_incipere(&c); s = sigillum_finire(&c); sigillum_hex(&s, hex); printf("finire recens %s\n", hex);
    sigillum_incipere(&c); sigillum_addere(&c, NIHIL, 0); sigillum_addere(&c, "abc", 3); sigillum_addere(&c, NIHIL, 0);
    s = sigillum_finire(&c); sigillum_hex(&s, hex); printf("NIHIL0+abc+NIHIL0 %s\n", hex);
    s = sigillum_hmac(NIHIL, 0, NIHIL, 0); sigillum_hex(&s, hex); printf("hmac(NIHIL,0,NIHIL,0) %s\n", hex);
    s = sigillum_hmac(NIHIL, 0, "abc", 3); sigillum_hex(&s, hex); printf("hmac(NIHIL,0,abc) %s\n", hex);
    s = sigillum_hmac("key", 3, NIHIL, 0); sigillum_hex(&s, hex); printf("hmac(key,NIHIL,0) %s\n", hex);
    { unsigned char z[64] = {0}; s = sigillum_hmac(z, 64, "abc", 3); sigillum_hex(&s, hex); printf("hmac(zero64,abc) %s\n", hex);
      s = sigillum_hmac(z, 17, "abc", 3); sigillum_hex(&s, hex); printf("hmac(zero17,abc) %s\n", hex); }
    return 0;
}
