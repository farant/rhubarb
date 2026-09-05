/* fragmenta_regiones.c - a briar genitum ex briar/probationes/fixa/thistle/fragmenta.thistle: obiecta et definitiones regionum */
#include "latina.h"
#include "fragmenta_regiones.h"
#line 34 "briar/probationes/fixa/thistle/fragmenta.thistle"
i32
summare (constans i32* numeri, i32 n)
{
    i32 summa = ZEPHYRUM;
    i32 i;

#line 13 "briar/probationes/fixa/thistle/fragmenta.thistle"
    per (i = ZEPHYRUM; i < n; i++)
    {
#line 9 "briar/probationes/fixa/thistle/fragmenta.thistle"
        summa = summa + numeri[i];
#line 16 "briar/probationes/fixa/thistle/fragmenta.thistle"
    }
#line 41 "briar/probationes/fixa/thistle/fragmenta.thistle"
    redde summa;
}
