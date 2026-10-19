#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "iter_directoria.h"
#include <stdio.h>

hic_manens constans character* _genus_titulus(IntroitusGenus g)
{
    commutatio (g)
    {
        casus INTROITUS_FILUM: redde "FILUM";
        casus INTROITUS_DIRECTORIUM: redde "DIRECTORIUM";
        casus INTROITUS_ALIUS: redde "ALIUS";
        ordinarius: redde "IGNOTUS";
    }
}

hic_manens s32 _numerus_visorum = 0;
hic_manens s32 _fructus_reddendus = 0;

hic_manens s32
_monstrare(chorda via_plena, constans DirectoriumIntroitus* e, vacuum* ctx)
{
    (vacuum)ctx;
    _numerus_visorum++;
    imprimere("  %-12s mensura=%lu  %.*s\n", _genus_titulus(e->genus),
        (unsigned long)e->mensura, (int)via_plena.mensura,
        (constans character*)via_plena.datum);
    redde _fructus_reddendus;
}
