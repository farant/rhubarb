/* sonda: imago_png_codificare - crudum_mensura = (1 + latitudo*4) *
 * altitudo in i32 sine custodia; 32768 x 32769 -> 163841 (involutum),
 * deinde versus 131073 bytorum per y in telam parvam copiantur */
#include "latina.h"
#include "imago_png.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>

integer
principale (vacuum)
{
    Piscina*    piscina;
    Imago       imago;
    PngFructus  fructus;
    size_t      pixela_mensura;
    i32         versus_m;

    imago.latitudo = (i32)32768;
    imago.altitudo = (i32)32769;
    versus_m = (i32)1 + imago.latitudo * (i32)4;
    imprimere("versus %u, crudum (i32) %u, verum %lu\n",
        (unsigned)versus_m, (unsigned)(versus_m * imago.altitudo),
        (unsigned long)((size_t)versus_m * (size_t)imago.altitudo));

    /* pixela VERA plena: 32768*32769*4 bytes (size_t) */
    pixela_mensura = (size_t)imago.latitudo * (size_t)imago.altitudo * (size_t)4;
    imago.pixela = (i8*)calloc(pixela_mensura, (size_t)1);
    si (imago.pixela == NIHIL) { imprimere("calloc fracta\n"); redde 1; }

    piscina = piscina_generare_dynamicum("sonda", 65536);
    fructus = imago_png_codificare(&imago, piscina);
    imprimere("successus %d mensura %u\n", (int)fructus.successus,
        (unsigned)fructus.mensura);
    redde 0;
}
