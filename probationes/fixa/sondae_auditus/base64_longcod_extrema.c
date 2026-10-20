#include "latina.h"
#include "base64.h"
#include <stdio.h>

int main(void)
{
    unsigned vals[] = { 0u, 1u, 2u, 3u, 4u, 5u, 6u, 3221225469u, 3221225470u, 3221225471u,
                        3221225472u, 3221225473u, 0xFFFFFFFCu, 0xFFFFFFFDu, 0xFFFFFFFEu, 0xFFFFFFFFu };
    unsigned k;
    for (k = 0; k < sizeof vals / sizeof vals[0]; k++)
        printf("%u %u\n", vals[k], (unsigned)base64_longitudo_codificata((i32)vals[k]));
    return 0;
}
