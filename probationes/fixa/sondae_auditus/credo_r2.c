#include "latina.h"
#include "credo.h"
#include "piscina.h"
#include <stdio.h>

interior vacuum
probare_bene (CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM(VERUM);
}

interior constans CredoSectio sectiones_sondae[] = {
    { "bene", probare_bene, NIHIL, NIHIL, "rete", NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    s32 r;
    r = credo_suitam_currere("sonda_r2", sectiones_sondae);
    imprimere("REDDITUM %d\n", r);
    redde ZEPHYRUM;
}
