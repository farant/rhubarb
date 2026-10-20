#include "iter_directoria_communia.inc"
/* r4: symlinks -> genus? (iterator plani) */
s32 principale(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    DirectoriumIterator* it = directorium_iterator_aperire("build/sonda/iter_directoria_fix", p);
    DirectoriumIntroitus* e;
    s32 alii = 0;
    dum ((e = directorium_iterator_proximum(it)) != NIHIL)
    {
        imprimere("  %-12s %.*s\n", _genus_titulus(e->genus), (int)e->titulus.mensura,
            (constans character*)e->titulus.datum);
        si (e->genus == INTROITUS_ALIUS) alii++;
    }
    directorium_iterator_claudere(it);
    imprimere("ALIUS numerus=%d\n", (int)alii);
    piscina_destruere(p);
    redde 0;
}
