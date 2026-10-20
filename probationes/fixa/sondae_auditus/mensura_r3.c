/* sonda: mensura_ex_linea valorem '1.5abc' et momentum '1000xyz' accipit */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "mensura.h"
#include <stdio.h>

integer
principale (vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    Mensura m;
    b32 r;

    r = mensura_ex_linea(chorda_ex_literis("s\ti\t\tt\t1.5abc\tsecunda\t1000xyz\n", p), &m, p);
    imprimere("'1.5abc'/'1000xyz': redde=%d valor=%f momentum=%ld\n", (integer)r, m.valor, (long)m.momentum);
    r = mensura_ex_linea(chorda_ex_literis("s\ti\t\tt\tabc\tsecunda\t1000\n", p), &m, p);
    imprimere("'abc': redde=%d\n", (integer)r);
    r = mensura_ex_linea(chorda_ex_literis("s\ti\t\tt\t\tsecunda\t1000\n", p), &m, p);
    imprimere("'': redde=%d\n", (integer)r);
    piscina_destruere(p);
    redde 0;
}
