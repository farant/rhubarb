#include "postulata_posix.h"
#include "latina.h"
#include "tempus.h"
#include <stdio.h>
#include <math.h>
int main(void)
{
    StatisticaeQuadri* s; Tempus* g; int i, zeri = 0, inf_media = 0, tent;
    /* ring wrap */
    tempus_initiare();
    for (i = 0; i < 250; i++) tempus_quadrum();
    s = tempus_obtinere_statisticas();
    printf("250 quadra: n=%lu index=%lu (expect 250 %% 120 = 10)\n", (unsigned long)s->numerus_quadrorum, (unsigned long)s->index_temporis_quadri);
    {
        double sum = 0; for (i = 0; i < 120; i++) sum += s->tempora_quadrorum[i];
        printf("media=%g oracle 120/sum=%g min=%g max=%g\n", s->quadra_media_per_secundum, 120.0/sum, s->tempus_minimum_quadri, s->tempus_maximum_quadri);
    }
    /* zero-delta frames */
    for (tent = 0; tent < 20000; tent++) {
        tempus_initiare();
        tempus_quadrum();
        if (tempus_delta() == 0.0) zeri++;
        if (!(s->quadra_media_per_secundum < 1e300)) inf_media++;
    }
    printf("primum quadrum post initiare: delta==0 in %d/20000, media non finita in %d/20000; ultima media=%g currentia=%g min=%g\n",
        zeri, inf_media, s->quadra_media_per_secundum, s->quadra_currentia_per_secundum, s->tempus_minimum_quadri);
    /* non-monotonic via pointer: unsigned wrap */
    tempus_initiare();
    g = tempus_obtinere_horologium();
    g->pulsus_ultimi_quadri += (i64)1000000000000.0;
    tempus_quadrum();
    printf("pulsus_ultimi_quadri in futuro (1e12 pulsus): delta=%g media=%g max=%g\n", tempus_delta(), s->quadra_media_per_secundum, s->tempus_maximum_quadri);
    return 0;
}
