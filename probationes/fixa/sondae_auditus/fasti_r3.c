#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "fasti.h"
#include <stdio.h>
#include <string.h>

static void tenta(const char* t)
{
    Dies d = fasti_dies(0, 0, 0);
    chorda s;
    b32 r, r2;
    s.datum = (i8*)t; s.mensura = (i32)strlen(t);
    r = fasti_parsare_iso(s, &d);
    r2 = fasti_parsare_diem(s, &d);
    printf("'%s' -> iso=%d diem=%d (%d-%d-%d)\n", t, (int)r, (int)r2,
        (int)d.annus, (int)d.mensis, (int)d.dies);
}

int main(void)
{
    tenta("2024-03-15");
    tenta("2024-03-15xyz");
    tenta("2024-03-155");
    tenta("2024-03-15T10:00:00Z");
    return 0;
}
