/* probatio_energia.c - status pilae (S5), fumus in machina vera
 *
 * Machina sine pila (mensae) FALSUM reddit et adest FALSUM; cum pila
 * VERUM, adest VERUM, centesimae 0-100. Lectio iterata idem (intra
 * horam unam, centesimae vix mutantur) - non mentitur. Valor ipse
 * oculis Frani confertur (lectio IOKit). */
#include "latina.h"
#include "piscina.h"
#include "energia.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    EnergiaStatus s;
              b32 r;

    (vacuum)c;
    memset(&s, (integer)VII, magnitudo(s));
    r = energia_status(&s);
    CREDO_VERUM(r == s.adest);
    si (r)
    {
        imprimere("  pila: %d%%%s\n", (integer)s.centesimae,
            s.oneratur ? " (oneratur)" : "");
        CREDO_VERUM(s.centesimae >= ZEPHYRUM && s.centesimae <= C);
    }
    alioquin
    {
        imprimere("  nulla pila\n");
        CREDO_FALSUM(s.oneratur);
    }
}

/* lectio prima propria (sine assertione): sectio a priore libera */
interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    EnergiaStatus s;
    EnergiaStatus t;
              b32 r;

    (vacuum)c;
    r = energia_status(&s);
    CREDO_VERUM(energia_status(&t) == r);
    si (r)
    {
        CREDO_VERUM(   t.centesimae - s.centesimae <= II
                    && s.centesimae - t.centesimae <= II);
    }
    CREDO_FALSUM(energia_status(NIHIL));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "I: status sanus", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "II: lectio iterata", _sectio_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("energia", SECTIONES);
}
