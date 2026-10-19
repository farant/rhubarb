#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "fasti.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

/* r1: fasti_ad_iso(fasti_nunc()) (vocatio herbarii/vatis) contra
 * fasti_ad_iso(fasti_ex_unix(time)) (UTC verum). */
int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    time_t t0 = time(NULL);
    chorda a = fasti_ad_iso(fasti_nunc(), p);
    chorda b = fasti_ad_iso(fasti_ex_unix((MomentumSec)t0), p);
    printf("vocatio (fasti_nunc): %.*s\n", (int)a.mensura, (const char*)a.datum);
    printf("UTC verum (ex_unix):  %.*s\n", (int)b.mensura, (const char*)b.datum);
    printf("aequales (minutis): %s\n",
        (a.mensura == b.mensura && a.mensura >= 16 &&
         memcmp(a.datum, b.datum, 16) == 0) ? "ita" : "NON");
    return 0;
}
