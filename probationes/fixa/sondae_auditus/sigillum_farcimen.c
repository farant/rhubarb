#include "latina.h"
#include "sigillum.h"
#include <stdio.h>
/* lengths 0..300, pattern bytes incl NUL and high-bit; one-shot hex per length */
int main(void)
{
    static unsigned char buf[400];
    char hex[SIGILLUM_HEX_MENSURA];
    int idx;
    for (idx = 0; idx < 400; idx++) buf[idx] = (unsigned char)((idx * 37 + 11) & 0xff);
    buf[3] = 0; buf[60] = 0;
    for (idx = 0; idx <= 300; idx++) {
        Sigillum s = sigillum_computare(buf, (memoriae_index)idx);
        sigillum_hex(&s, hex);
        printf("L %d %s\n", idx, hex);
    }
    return 0;
}
