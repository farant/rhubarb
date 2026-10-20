#include "latina.h"
#include "credo.h"
#include "piscina.h"
#include <stdio.h>

interior vacuum
probare_aequalis (CredoContextus* c)
{
    i32 magnum;
    (vacuum)c;
    magnum = (i32)3000000000UL;
    CREDO_AEQUALIS_I32(magnum, (i32)1);
    CREDO_INAEQUALITAS_I32(magnum, magnum);
    CREDO_MAIOR_AUT_AEQUALIS_I32((i32)1, magnum);
}

interior constans CredoSectio sectiones_sondae[] = {
    { "aequalis", probare_aequalis, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    s32 r;
    r = credo_suitam_currere("sonda_r1", sectiones_sondae);
    imprimere("REDDITUM %d\n", r);
    redde ZEPHYRUM;
}
