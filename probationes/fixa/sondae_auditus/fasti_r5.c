#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "fasti.h"
#include <stdio.h>
#include <time.h>

/* r5: epocha Unix. fasti_ex_unix(0) debet 1970-01-01T00:00:00Z. */
int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    DiesHora e;
    chorda c;
    time_t t0 = 0;
    struct tm* g = gmtime(&t0);
    e = fasti_ex_unix((MomentumSec)0);
    c = fasti_ad_iso(e, p);
    printf("fasti_ex_unix(0) = %.*s ; gmtime(0) annus=%d\n", (int)c.mensura,
        (const char*)c.datum, g->tm_year + 1900);
    e.dies = fasti_dies(1970, 1, 1); e.hora = fasti_hora(0, 0, 0);
    printf("fasti_ad_unix(1970-01-01T00:00:00) = %lld (debet 0)\n",
        (long long)fasti_ad_unix(e));
    e.dies = fasti_dies(2024, 3, 15); e.hora = fasti_hora(12, 0, 0);
    printf("fasti_ad_unix(2024-03-15T12:00:00) = %lld (debet 1710504000)\n",
        (long long)fasti_ad_unix(e));
    return 0;
}
