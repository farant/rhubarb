/* probatio_copia_terminalis.c - OSC 52 (eventus B3b-ii): octeti
 * exacti, UTF-8, textus vacuus. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "copia_terminalis.h"
#include "credo.h"
#include <stdio.h>

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    chorda textus;

    textus = copia_terminalis_componere(c->piscina,
        (constans i8*)"hello",
        V);
    CREDO_VERUM (chorda_aequalis_literis(textus,
        "\033]52;c;aGVsbG8=\033\\"));
    /* UTF-8: e acutum (C3 A9) */
    textus = copia_terminalis_componere(c->piscina,
        (constans i8*)"\303\251", II);
    CREDO_VERUM (chorda_aequalis_literis(textus,
        "\033]52;c;w6k=\033\\"));
}

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    chorda textus;

    textus = copia_terminalis_componere(c->piscina, (constans i8*)"",
        ZEPHYRUM);
    CREDO_VERUM (chorda_aequalis_literis(textus, "\033]52;c;\033\\"));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "I. OSC 52: octeti exacti", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "II. textus vacuus: series sine onere", _sectio_secunda, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("copia_terminalis", SECTIONES);
}
