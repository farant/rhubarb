/* sonda: limites >= 32768 -> s32 superfluit in max * SCALE_FP_ONE? */
#include "latina.h"
#include "imago.h"
#include "imago_opus.h"
#include "piscina.h"
#include <stdio.h>

integer
principale (vacuum)
{
    Piscina* piscina = piscina_generare_dynamicum("sonda", 1 << 20);
    Imago f, o;

    f = imago_creare(32768, 1, 9, 9, 9, 255, piscina);
    o = imago_scalare_ad_limites(&f, 32768, 1, IMAGO_SCALA_PROXIMUS, piscina);
    imprimere("ad_limites 32768x1 -> 32768x1: %ux%u pixela=%s\n",
        o.latitudo, o.altitudo, o.pixela ? "sic" : "NIHIL");
    o = imago_extrahere_et_scalare(&f, 0, 0, 32768, 1, 32768, 1,
        IMAGO_SCALA_PROXIMUS, piscina);
    imprimere("extrahere 32768x1 -> 32768x1: %ux%u pixela=%s\n",
        o.latitudo, o.altitudo, o.pixela ? "sic" : "NIHIL");

    piscina_destruere(piscina);
    redde 0;
}
