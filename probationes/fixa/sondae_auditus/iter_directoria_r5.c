#include "iter_directoria_communia.inc"
/* r5: mensura pro directoriis (caput: 0) */
s32 principale(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    DirectoriumIterator* it = directorium_iterator_aperire("build/sonda/iter_directoria_fix", p);
    DirectoriumIntroitus* e;
    dum ((e = directorium_iterator_proximum(it)) != NIHIL)
    {
        si (e->genus == INTROITUS_DIRECTORIUM)
            imprimere("  DIRECTORIUM %.*s mensura=%lu\n", (int)e->titulus.mensura,
                (constans character*)e->titulus.datum, (unsigned long)e->mensura);
    }
    directorium_iterator_claudere(it);
    piscina_destruere(p);
    redde 0;
}
