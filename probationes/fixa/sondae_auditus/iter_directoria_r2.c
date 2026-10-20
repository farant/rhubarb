#include "iter_directoria_communia.inc"
/* r2: filtrum_omnia vs NIHIL - occulta */
s32 principale(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    DirectoriumFiltrum f = directorium_filtrum_omnia();
    s32 r;
    imprimere("filtrum NIHIL:\n");
    r = directorium_ambulare("build/sonda/iter_directoria_fix", NIHIL, _monstrare, NIHIL, p);
    imprimere("reddit=%d visi=%d\n", (int)r, (int)_numerus_visorum);
    _numerus_visorum = 0;
    imprimere("filtrum_omnia():\n");
    r = directorium_ambulare("build/sonda/iter_directoria_fix", &f, _monstrare, NIHIL, p);
    imprimere("reddit=%d visi=%d\n", (int)r, (int)_numerus_visorum);
    piscina_destruere(p);
    redde 0;
}
