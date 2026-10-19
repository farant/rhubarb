/* runae_gbt_ambulatio: runae_graphema_proximum (ambulator, non solum
 * runae_rumpitur) contra GraphemeBreakTest 15.1 */
#include "latina.h"
#include "runae.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "runae_gbt_data.h"

int main(void)
{
    int k, j, mala = 0, mala_u = 0;
    for (k = 0; k < (int)(sizeof(GBT)/sizeof(GBT[0])); k++) {
        i8* buf = (i8*)malloc((size_t)GBT[k].n);
        const i8* cur; i32 lat; int bona = 1;
        memcpy(buf, GBT[k].b, (size_t)GBT[k].n);
        cur = buf;
        for (j = 0; j < GBT[k].nl; j++) {
            const i8* nx = runae_graphema_proximum(cur, buf + GBT[k].n, &lat);
            if ((int)(nx - cur) != GBT[k].l[j]) { bona = 0; break; }
            cur = nx;
        }
        if (!bona || cur != buf + GBT[k].n) { if (mala++ < 10) printf("DISCORDIA linea data %d graphema %d\n", k, j); }
        /* unitas: si nullum C0/DEL, idem quod graphema */
        {
            int c0 = 0, q;
            for (q = 0; q < GBT[k].n; q++) if ((unsigned)buf[q] < 0x20 || buf[q] == 0x7F) c0 = 1;
            if (!c0) {
                cur = buf;
                for (j = 0; j < GBT[k].nl; j++) {
                    const i8* nx = runae_unitas_proxima(cur, buf + GBT[k].n, RUNAE_POLITICA_GRAPHEMATUM, &lat);
                    if ((int)(nx - cur) != GBT[k].l[j]) { if (mala_u++ < 10) printf("DISCORDIA unitas linea %d\n", k); break; }
                    cur = nx;
                }
            }
        }
        free(buf);
    }
    printf("lineae %d discordiae graphema %d unitas %d\n", (int)(sizeof(GBT)/sizeof(GBT[0])), mala, mala_u);
    return 0;
}
