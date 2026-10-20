/* sonda flatura r2: rivus truncatus -> FRACTA_DATUM, numquam
 * INCOMPLETUS (git _zlib_e_sarcina fenestram crescentem in
 * INCOMPLETUS exspectat) */
#include "latina.h"
#include "piscina.h"
#include "flatura.h"
#include <stdio.h>

integer
principale (vacuum)
{
    Piscina*       piscina;
    FlaturaFructus comp;
    FlaturaFructus fr;
    i8*            orig;
    i32            n;
    i32            k;
    i32            semen;

    piscina = piscina_generare_dynamicum("sonda", 65536);
    n = 200000;
    orig = (i8*)piscina_allocare(piscina, (memoriae_index)n);
    semen = 12345;
    per (k = 0; k < n; k++)
    {
        semen = semen * 1103515245u + 12345u;
        orig[k] = (i8)((semen >> 16) & 0xFF);
    }
    comp = flatura_deflare(orig, n, FLATURA_COMPRESSIO_ORDINARIA, piscina);
    imprimere("deflare status=%d mensura=%d\n", (int)comp.status, (int)comp.mensura);
    fr = flatura_inflare(comp.datum, comp.mensura, piscina);
    imprimere("integrum: status=%d mensura=%d\n", (int)fr.status, (int)fr.mensura);
    fr = flatura_inflare(comp.datum, 65536 - 2, piscina);
    imprimere("truncatum ad 65534: status=%d (%s) [INCOMPLETUS=%d]\n",
        (int)fr.status, flatura_status_nuntium(fr.status),
        (int)FLATURA_STATUS_INCOMPLETUS);
    fr = flatura_inflare(comp.datum, 10, piscina);
    imprimere("truncatum ad 10: status=%d (%s)\n",
        (int)fr.status, flatura_status_nuntium(fr.status));
    piscina_destruere(piscina);
    redde 0;
}
