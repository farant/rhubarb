/* cum_functione.c - fixum Mach-O cum functione exportata
 * (_calculare_summa), duabus interioribus (_addere, _multiplicare) et
 * symbolo externo indefinito (_printf: __stubs, __got, __cstring).
 * Legitur a probatio_macho, probatio_sectio, probatio_symbola.
 * Binarium generatur per probationes/fixa/macho/generare.sh (quaestio
 * …QY4). */
#include "latina.h"
#include <stdio.h>

integer
calculare_summa (
    integer n);

interior integer
addere (
    integer a,
    integer b)
{
    redde a + b;
}

interior integer
multiplicare (
    integer a,
    integer b)
{
    redde a * b;
}

/* summa quadratorum I..n */
integer
calculare_summa (
    integer n)
{
    integer summa = ZEPHYRUM;
    integer i;

    per (i = I; i <= n; i++)
    {
        summa = addere(summa, multiplicare(i, i));
    }
    redde summa;
}

integer
principale (vacuum)
{
    imprimere("%d\n", calculare_summa(X));
    redde ZEPHYRUM;
}
