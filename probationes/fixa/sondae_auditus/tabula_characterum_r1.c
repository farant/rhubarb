#include "latina.h"
#include "piscina.h"
#include "tabula_characterum.h"
#include <stdio.h>

interior vacuum
monstrare (TabulaCharacterum* t, i32 linea)
{
    i32 c;
    imprimere("  linea %u:", (insignatus integer)linea);
    per (c = ZEPHYRUM; c < t->latitudo; c++)
    {
        imprimere(" %02x", (insignatus integer)(insignatus character)tabula_cellula(t, linea, c));
    }
    imprimere("\n");
}

integer
principale (vacuum)
{
    Piscina* p;
    TabulaCharacterum t;
    b32 r;
    b32 r2;

    p = piscina_generare_dynamicum("sonda", 65536);

    /* latitudo 4, altitudo 2: [a b \t 0x01] / vacua */
    tabula_ex_literis_cum_dimensionibus(&t, p, 4, 2, "ab\t");
    imprimere("ante:\n");
    monstrare(&t, 0);
    monstrare(&t, 1);
    r = tabula_inserere_characterem(&t, 0, 0, 'x');
    imprimere("inserere 'x' ad 0 -> %d\n", (integer)r);
    monstrare(&t, 0);
    monstrare(&t, 1);
    imprimere("cellula[0][3]=='\\t' %d, est_tab_continuatio(0,3)? n/a (columna ultima)\n",
        (integer)(tabula_cellula(&t, 0, 3) == '\t'));
    imprimere("tabula_aequalis_literis(\"xab\\t\") -> %d\n", (integer)tabula_aequalis_literis(&t, "xab\t"));

    /* idem per tabula_inserere_spatium */
    tabula_ex_literis_cum_dimensionibus(&t, p, 4, 2, "ab\t");
    r2 = tabula_inserere_spatium(&t, 0, 0, 1);
    imprimere("inserere_spatium(0,0,1) -> %d\n", (integer)r2);
    monstrare(&t, 0);
    monstrare(&t, 1);

    /* tabula_inserere_tab ipsa recusat tab in columna ultima */
    tabula_ex_literis_cum_dimensionibus(&t, p, 4, 2, "");
    imprimere("inserere_tab ad columnam 3 (ultimam) -> %d\n", (integer)tabula_inserere_tab(&t, 0, 3));

    piscina_destruere(p);
    redde ZEPHYRUM;
}
