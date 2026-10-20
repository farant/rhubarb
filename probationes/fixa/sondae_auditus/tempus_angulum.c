#include "latina.h"
#include "tempus.h"
#include <stdio.h>
#include <math.h>
int main(void)
{
    f32 a[10] = {0.0f, 3.0f, -3.0f, 0.0f, 0.0f, 0.1f, 0.0f, 0.0f, 6.0f, 1.0f};
    f32 b[10] = {1.0f, -3.0f, 3.0f, -4.712389f, 3.14159265f, -6.1831853f, -3.14159265f, 628.3185f, 0.2f, 1.0f};
    int i;
    for (i = 0; i < 10; i++) {
        f32 d = interpolare_angulum(a[i], b[i], 1.0f) - a[i];
        printf("a=%.7g b=%.7g diff(t=1)=%.7g mid(t=0.5)=%.7g\n", (double)a[i], (double)b[i], (double)d,
            (double)interpolare_angulum(a[i], b[i], 0.5f));
    }
    return 0;
}
