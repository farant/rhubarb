/* punctum_regiones.h - a briar genitum ex briar/probationes/fixa/thistle/punctum.thistle: directivae, typi, prototypi regionum */
#ifndef PUNCTUM_REGIONES_H
#define PUNCTUM_REGIONES_H
#include "latina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#line 8 "briar/probationes/fixa/thistle/punctum.thistle"
#include "chorda.h"
#line 9 "briar/probationes/fixa/thistle/punctum.thistle"
#define PUNCTUM_MAXIMUM 3
#line 11 "briar/probationes/fixa/thistle/punctum.thistle"
nomen structura {
    i32 x;
    i32 y;
} Punctum;
#line 18 "briar/probationes/fixa/thistle/punctum.thistle"
i32
punctum_summa (constans Punctum* p);
#endif /* PUNCTUM_REGIONES_H */
