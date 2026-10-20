#include "iter_directoria_communia.inc"
/* r1: profunditas_max = -1 ("sine recursione") -> visitat summum gradum? */
s32 principale(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    DirectoriumFiltrum f = directorium_filtrum_omnia();
    s32 r;
    f.includere_occultos = VERUM;
    f.profunditas_max = -1;
    imprimere("profunditas_max=-1:\n");
    r = directorium_ambulare("build/sonda/iter_directoria_fix", &f, _monstrare, NIHIL, p);
    imprimere("reddit=%d visi=%d\n", (int)r, (int)_numerus_visorum);
    _numerus_visorum = 0;
    f.profunditas_max = I;
    imprimere("profunditas_max=1 (comparatio):\n");
    r = directorium_ambulare("build/sonda/iter_directoria_fix", &f, _monstrare, NIHIL, p);
    imprimere("reddit=%d visi=%d\n", (int)r, (int)_numerus_visorum);
    piscina_destruere(p);
    redde 0;
}
