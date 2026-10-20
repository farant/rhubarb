#include "latina.h"
#include "sigillum.h"
#include <stdio.h>
int main(void)
{
    Sigillum a = sigillum_computare("abc", 3), b = a, c;
    int idx, bit, wrong = 0;
    printf("idem=%d\n", (int)sigillum_aequale(&a, &b));
    printf("se=%d\n", (int)sigillum_aequale(&a, &a));
    for (idx = 0; idx < 32; idx++) for (bit = 0; bit < 8; bit++) {
        c = a; c.octeti[idx] ^= (i8)(1u << bit);
        if (sigillum_aequale(&a, &c) != FALSUM || sigillum_aequale(&c, &a) != FALSUM) wrong++;
    }
    printf("bitflip_wrong=%d (of 256)\n", wrong);
    c = a; c.octeti[31] ^= 0x80; printf("ultimus_diff=%d\n", (int)sigillum_aequale(&a, &c));
    c = a; c.octeti[0] ^= 0x01; printf("primus_diff=%d\n", (int)sigillum_aequale(&a, &c));
    printf("a_NIHIL=%d b_NIHIL=%d ambo_NIHIL=%d\n", (int)sigillum_aequale(NIHIL, &a), (int)sigillum_aequale(&a, NIHIL), (int)sigillum_aequale(NIHIL, NIHIL));
    printf("aequale_redde_exacte_1=%d\n", sigillum_aequale(&a, &b) == 1);
    return 0;
}
