/* probatio_fenestra_tempus.c - Eventus.tempus et fenestra_tempus_ms
 *
 * Sine fenestra: campus tempus in Eventu et sedes unica horologii.
 * Stampa in impellere_eventum per cursum vivum (T12) exercetur. */
#include "latina.h"
#include "piscina.h"
#include "fenestra.h"
#include "credo.h"
#include <stdio.h>

s32 principale (vacuum)
{
    Piscina* piscina;
        s64  a;
        s64  b;
    Eventus  e;

    piscina = piscina_generare_dynamicum("probatio_fenestra_tempus",
        IV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- Probans fenestra_tempus_ms ---\n");
    a = fenestra_tempus_ms();
    b = fenestra_tempus_ms();
    CREDO_VERUM (a >= ZEPHYRUM);
    CREDO_VERUM (b >= a);

    imprimere("\n--- Probans campum Eventus.tempus ---\n");
    e.genus   = EVENTUS_NIHIL;
    e.tempus  = CCC;
    CREDO_VERUM (e.tempus == CCC);

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
