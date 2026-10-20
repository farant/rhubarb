#include "iter_directoria_communia.inc"
/* r3: callback reddit 7 -> directorium_ambulare reddit? */
s32 principale(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    s32 r;
    _fructus_reddendus = 7;
    r = directorium_ambulare("build/sonda/iter_directoria_fix", NIHIL, _monstrare, NIHIL, p);
    imprimere("callback 7 -> reddit=%d (caput: 0 vel -1)\n", (int)r);
    _fructus_reddendus = -1;
    r = directorium_ambulare("build/sonda/iter_directoria_fix", NIHIL, _monstrare, NIHIL, p);
    imprimere("callback -1 -> reddit=%d\n", (int)r);
    _fructus_reddendus = 0;
    r = directorium_ambulare("build/sonda/non_existit_xyz", NIHIL, _monstrare, NIHIL, p);
    imprimere("via absens -> reddit=%d\n", (int)r);
    piscina_destruere(p);
    redde 0;
}
