/* sonda: scalare_ad_limites / extrahere_et_scalare - dimensio implens limiti brevior? */
#include "latina.h"
#include "imago.h"
#include "imago_opus.h"
#include "piscina.h"
#include <stdio.h>

integer
principale (vacuum)
{
    Piscina* piscina = piscina_generare_dynamicum("sonda", 1 << 20);
    Imago f3, f1000, f100x50, o;

    f3      = imago_creare(3, 3, 9, 9, 9, 255, piscina);
    f1000   = imago_creare(1000, 1000, 9, 9, 9, 255, piscina);
    f100x50 = imago_creare(300, 100, 9, 9, 9, 255, piscina);

    o = imago_scalare_ad_limites(&f3, 100, 100, IMAGO_SCALA_PROXIMUS, piscina);
    imprimere("ad_limites 3x3 -> 100x100: %ux%u\n", o.latitudo, o.altitudo);
    o = imago_scalare_ad_limites(&f1000, 16, 16, IMAGO_SCALA_AREA, piscina);
    imprimere("ad_limites 1000x1000 -> 16x16: %ux%u\n", o.latitudo, o.altitudo);
    o = imago_scalare_ad_limites(&f100x50, 100, 100, IMAGO_SCALA_BILINEARIS, piscina);
    imprimere("ad_limites 300x100 -> 100x100: %ux%u\n", o.latitudo, o.altitudo);
    o = imago_extrahere_et_scalare(&f1000, 0, 0, 3, 3, 100, 100, IMAGO_SCALA_PROXIMUS, piscina);
    imprimere("extrahere 3x3 -> 100x100: %ux%u\n", o.latitudo, o.altitudo);
    o = imago_extrahere_et_scalare(&f1000, 0, 0, 1000, 1000, 16, 16, IMAGO_SCALA_AREA, piscina);
    imprimere("extrahere 1000x1000 -> 16x16: %ux%u\n", o.latitudo, o.altitudo);

    piscina_destruere(piscina);
    redde 0;
}
