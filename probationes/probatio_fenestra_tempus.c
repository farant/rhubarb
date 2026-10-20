/* probatio_fenestra_tempus.c - Eventus.tempus et fenestra_tempus_ms
 *
 * Sine fenestra: campus tempus in Eventu et sedes unica horologii.
 * Stampa in impellere_eventum per cursum vivum (T12) exercetur. */
#include "latina.h"
#include "piscina.h"
#include "fenestra.h"
#include "credo.h"
#include <stdio.h>

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    s64 a;
    s64 b;

    (vacuum)c;
    a = fenestra_tempus_ms();
    b = fenestra_tempus_ms();
    CREDO_VERUM (a >= ZEPHYRUM);
    CREDO_VERUM (b >= a);
}

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Eventus e;

    (vacuum)c;
    e.genus   = EVENTUS_NIHIL;
    e.tempus  = CCC;
    CREDO_VERUM (e.tempus == CCC);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "fenestra_tempus_ms", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "campum Eventus.tempus", _sectio_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("fenestra_tempus", SECTIONES);
}
