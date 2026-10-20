#include "latina.h"
#include "sigillum.h"
#include <stdio.h>
#include <string.h>
int main(void)
{
    Sigillum s; char out[80]; int idx, ok = 1;
    for (idx = 0; idx < 32; idx++) s.octeti[idx] = (i8)(idx < 16 ? idx : 0xF0 + (idx - 16) % 16);
    memset(out, 'X', sizeof out);
    sigillum_hex(&s, out);
    printf("hex=%s\n", out);
    printf("strlen=%d out[64]=%d out[65]=%c\n", (int)strlen(out), (int)out[64], out[65]);
    for (idx = 0; idx < 64; idx++) if (!((out[idx] >= '0' && out[idx] <= '9') || (out[idx] >= 'a' && out[idx] <= 'f'))) ok = 0;
    printf("lowercase_ok=%d\n", ok);
    { Sigillum z; memset(&z, 0, sizeof z); sigillum_hex(&z, out); printf("zeros=%s\n", out); }
    { Sigillum f; memset(&f, 0xff, sizeof f); sigillum_hex(&f, out); printf("ff=%s\n", out); }
    return 0;
}
