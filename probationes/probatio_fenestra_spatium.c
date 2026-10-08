/* probatio_fenestra_spatium.c - spatium utile schirmi (vicus-latera
 * S2c)
 *
 * fenestra_spatium_utile: rectangulum contenti fenestrae titulatae quae
 * aream visibilem implet. Magnitudo exacta a schirmo pendet; probatio
 * solum verisimilitudinem iudicat (area positiva, saltem DCXL x CD,
 * origo non sub schirmo) et argumenta NIHIL recusari. Fenestra nulla
 * aperitur. */
#include "latina.h"
#include "piscina.h"
#include "fenestra.h"
#include "credo.h"
#include <stdio.h>

s32 principale (vacuum)
{
    Piscina* piscina;
        i32  x;
        i32  y;
        i32  latitudo;
        i32  altitudo;

    piscina = piscina_generare_dynamicum("probatio_fenestra_spatium",
        M * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- I: spatium utile schirmi principalis ---\n");
    x         = ZEPHYRUM;
    y         = ZEPHYRUM;
    latitudo  = ZEPHYRUM;
    altitudo  = ZEPHYRUM;
    CREDO_VERUM(fenestra_spatium_utile(&x, &y, &latitudo, &altitudo));
    imprimere("  spatium utile: %u,%u %ux%u puncta\n", (unsigned)x,
        (unsigned)y, (unsigned)latitudo, (unsigned)altitudo);
    CREDO_VERUM(latitudo >= DCXL);
    CREDO_VERUM(altitudo >= CD);
    /* origo in schirmo (i32 insignatum: negativum = ingens) */
    CREDO_VERUM(x < XX * M);
    CREDO_VERUM(y < XX * M);

    imprimere("\n--- II: argumenta NIHIL ---\n");
    CREDO_FALSUM(fenestra_spatium_utile(NIHIL, &y, &latitudo,
        &altitudo));
    CREDO_FALSUM(fenestra_spatium_utile(&x, &y, &latitudo, NIHIL));

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
