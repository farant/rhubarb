#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "tabula_dispersa.h"
#include <stdio.h>

integer
principale (vacuum)
{
    Piscina* p;
    TabulaDispersa* t;
    TabulaDispersa* tf;
    chorda c;
    vacuum* v;
    b32 a, b, d, e;
    integer valor;

    valor = VII;
    p = piscina_generare_dynamicum("sonda", 65536);
    t = tabula_dispersa_creare(p, XVI, tabula_friare_multiplicatio,
                               (TabulaComparatio)chorda_comparare);
    tf = tabula_dispersa_creare_chorda(p, XVI);
    c.datum = (i8*)"alpha";
    c.mensura = V;
    tabula_dispersa_inserere(t, c, &valor);
    tabula_dispersa_inserere(tf, c, &valor);
    v = NIHIL;
    a = tabula_dispersa_invenire(t, c, &v);
    b = tabula_dispersa_invenire_literis(t, "alpha", &v);
    d = tabula_dispersa_continet_literis(t, "alpha");
    e = tabula_dispersa_continet_literis(tf, "alpha");
    imprimere("djb2: invenire=%d invenire_literis=%d continet_literis=%d\n",
              (integer)a, (integer)b, (integer)d);
    imprimere("fnv1a: continet_literis=%d\n", (integer)e);
    piscina_destruere(p);
    redde ZEPHYRUM;
}
