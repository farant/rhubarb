/* sonda flatura r4: flatura_maxima_magnitudo_compressa non est
 * maxima - nivellus fixus et datum vacuum superant */
#include "latina.h"
#include "piscina.h"
#include "flatura.h"
#include <stdio.h>

integer
principale (vacuum)
{
    Piscina*       piscina;
    FlaturaFructus fr;
    i8*            orig;
    i32            n;
    i32            k;
    i32            semen;

    piscina = piscina_generare_dynamicum("sonda", 65536);
    n = 1000;
    orig = (i8*)piscina_allocare(piscina, (memoriae_index)n);
    semen = 7;
    per (k = 0; k < n; k++)
    {
        semen = semen * 1103515245u + 12345u;
        orig[k] = (i8)(144 + ((semen >> 16) % 112));
    }
    fr = flatura_deflare(orig, n, FLATURA_COMPRESSIO_ORDINARIA, piscina);
    imprimere("raw deflare n=%d -> %d, limes=%d\n", (int)n, (int)fr.mensura,
        (int)flatura_maxima_magnitudo_compressa(n));
    fr = flatura_gzip_deflare(orig, n, FLATURA_COMPRESSIO_OPTIMA, piscina);
    imprimere("gzip deflare n=%d -> %d, limes=%d\n", (int)n, (int)fr.mensura,
        (int)flatura_maxima_magnitudo_compressa(n));
    fr = flatura_gzip_deflare(orig, 0, FLATURA_COMPRESSIO_NULLA, piscina);
    imprimere("gzip deflare n=0 -> %d, limes=%d\n", (int)fr.mensura,
        (int)flatura_maxima_magnitudo_compressa(0));
    fr = flatura_gzip_deflare(orig, n, FLATURA_COMPRESSIO_NULLA, piscina);
    imprimere("gzip NULLA n=%d -> %d, limes=%d\n", (int)n, (int)fr.mensura,
        (int)flatura_maxima_magnitudo_compressa(n));
    piscina_destruere(piscina);
    redde 0;
}
