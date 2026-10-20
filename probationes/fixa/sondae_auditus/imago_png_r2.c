/* sonda: imago_png_codificare(imago, NIHIL) -> successus FALSUM et error vacuus,
 * contra caput '@error: causa NOMINATA si !successus (numquam tacet)' */
#include "latina.h"
#include "imago_png.h"
#include <stdio.h>

integer
principale (vacuum)
{
    Imago       imago;
    PngFructus  fructus;
    i8          px[4] = { 1, 2, 3, 4 };

    imago.pixela   = px;
    imago.latitudo = (i32)1;
    imago.altitudo = (i32)1;
    fructus = imago_png_codificare(&imago, NIHIL);
    imprimere("successus %d error.datum %s error.mensura %u\n",
        (int)fructus.successus, fructus.error.datum == NIHIL ? "NIHIL" : "aliquid",
        (unsigned)fructus.error.mensura);
    fructus = imago_png_scribere(&imago, "/dev/null", NIHIL);
    imprimere("scribere: successus %d error.mensura %u\n",
        (int)fructus.successus, (unsigned)fructus.error.mensura);
    redde 0;
}
