/* punctum.c - a briar genitum ex briar/probationes/fixa/thistle/punctum.thistle: principale */
#include "latina.h"
#include "punctum_regiones.h"
#line 25 "briar/probationes/fixa/thistle/punctum.thistle"
s32
principale (vacuum)
{
    Punctum p;

    p.x = I;
    p.y = II;
    imprimere("summa %d\n", (integer)punctum_summa(&p));
    redde (punctum_summa(&p) <= PUNCTUM_MAXIMUM) ? ZEPHYRUM : I;
}
