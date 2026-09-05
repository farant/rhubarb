/* gamma.c - AMALGAMA a briar genitum ex briar/probationes/fixa/amalgama/gamma.thistle: plagula UNA, sola compilanda (effugium):
 * clang -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings -Wno-long-long gamma.c -o gamma
 * Ordo: capita clausurae ordine dependentiae (postulata_posix.h primum), caput regionum,
 * fontes bibliothecarum (statica per plagulam renominata: #define ante, #undef post),
 * regiones, principale. Inclusiones locales lineis vacuis substitutae (numeri linearum
 * servati); '#line 1' plagulam originalem nominat. */
#line 1 "include/postulata_posix.h"
/* postulata_posix.h - FIXUM amalgamae: gemellus include/postulata_posix.h
 * (caput PRIMUM plagulae POSIX-utentis; amalgama id primum ponit) */

#ifndef POSTULATA_POSIX_H
#define POSTULATA_POSIX_H

#define _DEFAULT_SOURCE 1

#endif /* POSTULATA_POSIX_H */
#line 1 "include/latina.h"
/* latina.h - FIXUM amalgamae: latina.h DECURTATUM (id solum quod
 * gamma.thistle et alpha/beta adhibent) ne aurum gamma.c CDXXII lineas
 * ferat. Fabrica ficta: briar/probationes/fixa/amalgama/fabrica. */

#ifndef LATINA_H
#define LATINA_H

#define character   char
#define integer     int
#define vacuum      void
#define constans    const
#define staticus    static
#define interior    static
#define hic_manens  static
#define si          if
#define alioquin    else
#define per         for
#define dum         while
#define redde       return
#define structura   struct
#define nomen       typedef
#define principale  main
#define imprimere   printf
#define ZEPHYRUM    0
#define I           1
#define II          2

nomen unsigned int  i32;
nomen signed int    s32;
nomen int           b32;

#endif /* LATINA_H */
#line 1 "include/alpha.h"
/* alpha.h - FIXUM amalgamae: bibliotheca ficta prima */

#ifndef ALPHA_H
#define ALPHA_H



/* numerus characterum non albarum */
i32
alpha_numerare (
    constans character* textus);

#endif /* ALPHA_H */
#line 1 "include/beta.h"
/* beta.h - FIXUM amalgamae: bibliotheca ficta secunda (alpha includit:
 * ordo dependentiae alpha.h ante beta.h) */

#ifndef BETA_H
#define BETA_H



/* alpha_numerare duplicatum */
i32
beta_duplicare (
    constans character* textus);

#endif /* BETA_H */
#line 1 "include/gamma_regiones.h"
/* gamma_regiones.h - a briar genitum ex briar/probationes/fixa/amalgama/gamma.thistle: directivae, typi, prototypi regionum */
#ifndef GAMMA_REGIONES_H
#define GAMMA_REGIONES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#endif /* GAMMA_REGIONES_H */
/* lib/alpha.c: statica per plagulam renominata */
#define _est_spatium _est_spatium_alpha
#line 1 "lib/alpha.c"
/* alpha.c - FIXUM amalgamae: staticum '_est_spatium' cum beta.c
 * COLLIDIT (renominatio per plagulam probatur); macro ALPHA_MAXIMUM
 * post plagulam #undef */



#include <string.h>

#define ALPHA_MAXIMUM 64

interior b32
_est_spatium (
    character c)
{
    redde (b32)(c == ' ' || c == '\t');
}

i32
alpha_numerare (
    constans character* textus)
{
    i32 n = ZEPHYRUM;
    i32 i;
    i32 m = (i32)strlen(textus);

    si (m > ALPHA_MAXIMUM)
    {
        m = ALPHA_MAXIMUM;
    }
    per (i = ZEPHYRUM; i < m; i++)
    {
        si (!_est_spatium(textus[i]))
        {
            n = n + I;
        }
    }
    redde n;
}
#undef ALPHA_MAXIMUM
#undef _est_spatium
/* lib/beta.c: statica per plagulam renominata */
#define Cella Cella_beta
#define _cella _cella_beta
#define _est_spatium _est_spatium_beta
#line 1 "lib/beta.c"
/* beta.c - FIXUM amalgamae: staticum '_est_spatium' ALTERUM (typo alio),
 * variabile scopi plagulae, typedef localis - omnia renominanda */



nomen structura {
    i32 factor;
} Cella;

hic_manens Cella _cella = { II };

interior b32
_est_spatium (
    constans character* p)
{
    redde (b32)(*p == ' ');
}

i32
beta_duplicare (
    constans character* textus)
{
    i32 spatia = ZEPHYRUM;
    constans character* p;

    per (p = textus; *p != '\0'; p++)
    {
        si (_est_spatium(p))
        {
            spatia = spatia + I;
        }
    }
    redde (alpha_numerare(textus) + spatia - spatia) * _cella.factor;
}
#undef Cella
#undef _cella
#undef _est_spatium
#line 1 "fontes/gamma_regiones.c"
/* gamma_regiones.c - a briar genitum ex briar/probationes/fixa/amalgama/gamma.thistle: obiecta et definitiones regionum */


#line 1 "fontes/gamma.c"
/* gamma.c - a briar genitum ex briar/probationes/fixa/amalgama/gamma.thistle: principale */


#line 8 "briar/probationes/fixa/amalgama/gamma.thistle"
s32
principale (vacuum)
{
    imprimere("%d\n", (integer)beta_duplicare("ab cd"));
    redde ZEPHYRUM;
}
