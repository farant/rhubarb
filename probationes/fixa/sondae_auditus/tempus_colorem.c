#include "latina.h"
#include "tempus.h"
#include <stdio.h>
int main(void)
{
    printf("case1 opaque 0xFF000000->0xFF0000FF t=0.5 => 0x%08lX\n", (unsigned long)interpolare_colorem(0xFF000000u, 0xFF0000FFu, 0.5f));
    printf("case2 alpha 0x7F 0x7F112233->0x7F445566 t=1 => 0x%08lX\n", (unsigned long)interpolare_colorem(0x7F112233u, 0x7F445566u, 1.0f));
    printf("case3 t=0 0x80ABCDEF->0x00000000 => 0x%08lX\n", (unsigned long)interpolare_colorem(0x80ABCDEFu, 0x00000000u, 0.0f));
    printf("case4 t=2 0x00000000->0x000000FF => 0x%08lX (oracle clamp 0xFF or extrap 510 unrepresentable)\n", (unsigned long)interpolare_colorem(0x00000000u, 0x000000FFu, 2.0f));
    printf("case5 t=-1 0x00000000->0x000000FF => 0x%08lX\n", (unsigned long)interpolare_colorem(0x00000000u, 0x000000FFu, -1.0f));
    printf("case6 t=1.0 0x00000000->0x00FFFFFF => 0x%08lX\n", (unsigned long)interpolare_colorem(0x00000000u, 0x00FFFFFFu, 1.0f));
    printf("case7 t=0.999999 0x00->0xFF r => 0x%08lX (round would give FF)\n", (unsigned long)interpolare_colorem(0x00000000u, 0x000000FFu, 0.999999f));
    return 0;
}
