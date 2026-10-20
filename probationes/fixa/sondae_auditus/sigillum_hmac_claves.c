#include "latina.h"
#include "sigillum.h"
#include <stdio.h>
/* key lengths 0..200 x msg lengths at corners; same-buffer aliasing */
int main(void)
{
    static unsigned char kb[300], mb[300];
    static const int ml[] = {0,1,55,56,57,63,64,65,119,120,127,128,200};
    char hex[SIGILLUM_HEX_MENSURA];
    int kl, mi, idx;
    for (idx = 0; idx < 300; idx++) { kb[idx] = (unsigned char)(idx * 89 + 3); mb[idx] = (unsigned char)(255 - idx * 7); }
    kb[0] = 0; mb[5] = 0;
    for (kl = 0; kl <= 200; kl++)
        for (mi = 0; mi < (int)(sizeof ml / sizeof ml[0]); mi++) {
            Sigillum s = sigillum_hmac(kb, (memoriae_index)kl, mb, (memoriae_index)ml[mi]);
            sigillum_hex(&s, hex);
            printf("H %d %d %s\n", kl, ml[mi], hex);
        }
    for (kl = 0; kl <= 130; kl += 13) {
        Sigillum s = sigillum_hmac(kb, (memoriae_index)kl, kb, (memoriae_index)kl);
        sigillum_hex(&s, hex); printf("A %d %s\n", kl, hex);
    }
    return 0;
}
