/* probatio_briar_dialectus.c - charta dialecti (briar -dialectus):
 * partes derivatae ex textu latina.h (fixura parva et latina.h VIVA)
 * et vexillis datis; laquei C89 manu scripti.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "credo.h"
#include "briar_dialectus.h"
#include <stdio.h>
#include <string.h>

#define VEXILLA_PLANA \
    "-std=c89 -pedantic -Wall -Werror -Wsign-conversion"
#define VEXILLA_VITREA \
    "-std=c89 -Werror -fbracket-depth=512 | -O2 -DSQLITE_X"

interior constans character* FIXURA =
    "#ifndef LATINA_H\n"
    "#define LATINA_H\n"
    "\n"
    "#include <stddef.h>\n"
    "\n"
    "#define character     char\n"
    "#define integer         int\n"
    "\n"
    "#define signatus         signed\n"
    "#define insignatus  unsigned\n"
    "\n"
    "#define si                    if\n"
    "#define redde                return\n"
    "\n"
    "#define magnitudo     sizeof\n"
    "\n"
    "#define ZEPHYRUM    0\n"
    "#define I                     1\n"
    "#define II                    2\n"
    "#define III                    3\n"
    "#define IV                    4\n"
    "#define V                        5\n"
    "#define VI                    6\n"
    "#define VII                    7\n"
    "#define VIII                8\n"
    "#define IX                    9\n"
    "#define X                        10\n"
    "#define M                        1000\n"
    "\n"
    "nomen insignatus integer       i32;\n"
    "nomen signatus integer         s32;\n"
    "nomen size_t                                 memoriae_index;\n"
    "\n"
    "#endif /* LATINA_H */\n";

/* VERUM si linea aliqua textus et a et b continet (b post a) */
interior b32
_linea_habet (
                 chorda  textus,
     constans character* a,
     constans character* b)
{
    i32 initium = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k <= (i32)textus.mensura; k++)
    {
        si (k == (i32)textus.mensura || textus.datum[k] == '\n')
        {
                     character  linea[CCLVI];
                           i32  longitudo = k - initium;
            constans character* pa;

            si (longitudo >= (i32)CCLVI)
            {
                longitudo = CCLV;
            }
            memcpy(linea, textus.datum + initium, (size_t)longitudo);
            linea[longitudo]  = '\0';
            pa                = strstr(linea, a);
            si (pa != NIHIL && strstr(pa + strlen(a), b) != NIHIL)
            {
                redde VERUM;
            }
            initium = k + I;
        }
    }
    redde FALSUM;
}

/* longitudo lineae longissimae (octeti) */
interior i32
_linea_longissima (
    chorda textus)
{
    i32 maxima   = ZEPHYRUM;
    i32 initium  = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < (i32)textus.mensura; k++)
    {
        si (textus.datum[k] == '\n')
        {
            si (k - initium > maxima)
            {
                maxima = k - initium;
            }
            initium = k + I;
        }
    }
    redde maxima;
}

s32
principale (vacuum)
{
     Piscina* pn;
         b32  praeteritus;
      chorda  charta;
      chorda  latina;

    pn = piscina_generare_dynamicum("probatio_briar_dialectus",
        262144);
    si (!pn)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(pn);

    imprimere("\n--- Probans chartam ex fixura ---\n");
    charta = briar_dialectus_charta(chorda_ex_literis(FIXURA, pn),
        VEXILLA_PLANA, VEXILLA_VITREA, pn);
    CREDO_VERUM (charta.mensura > ZEPHYRUM);
    /* typi integrorum ex 'nomen', verba latina in C versa */
    CREDO_VERUM (_linea_habet(charta, "i32", "= unsigned int"));
    CREDO_VERUM (_linea_habet(charta, "s32", "= signed int"));
    CREDO_VERUM (_linea_habet(charta, "memoriae_index", "= size_t"));
    /* verba per greges: titulus ex verbo primo gregis */
    CREDO_VERUM (_linea_habet(charta, "typi:", "character=char"));
    CREDO_VERUM (_linea_habet(charta, "character=char", "integer=int"));
    CREDO_VERUM (_linea_habet(charta, "imperium:", "si=if"));
    CREDO_VERUM (_linea_habet(charta, "si=if", "redde=return"));
    CREDO_VERUM (_linea_habet(charta, "cetera:", "magnitudo=sizeof"));
    /* numeri contracti: primi et ultimus cum numero, non omnes */
    CREDO_VERUM (_linea_habet(charta, "numeri:", "ZEPHYRUM=0"));
    CREDO_VERUM (_linea_habet(charta, "numeri:", "M=1000"));
    CREDO_VERUM (_linea_habet(charta, "numeri:", "(12)"));
    CREDO_FALSUM (_linea_habet(charta, "V=5", ""));
    /* custos capitis non verbum */
    CREDO_FALSUM (_linea_habet(charta, "LATINA_H", "="));
    /* vexilla ut data */
    CREDO_VERUM (_linea_habet(charta, "plana:", "-std=c89"));
    CREDO_VERUM (_linea_habet(charta, "-Wall", "-Werror"));
    /* vitrea = plana + differentia; post '|' (fontes venditi) nihil */
    CREDO_VERUM (_linea_habet(charta, "vitrea:", "plana +"));
    CREDO_VERUM (_linea_habet(charta, "plana +",
        "-fbracket-depth=512"));
    CREDO_FALSUM (_linea_habet(charta, "vitrea:", "-std=c89"));
    CREDO_FALSUM (_linea_habet(charta, "-O2", ""));
    CREDO_FALSUM (_linea_habet(charta, "SQLITE", ""));
    /* laquei manu: idioma ordinis argumentorum, principale */
    CREDO_VERUM (_linea_habet(charta, "argumenta[I]", "= via;"));
    CREDO_VERUM (_linea_habet(charta, "NUMQUAM", "i32"));
    CREDO_VERUM (_linea_habet(charta, "%.*s", "mensura"));
    /* charta in terminali legitur: lineae <= LXXII */
    CREDO_VERUM (_linea_longissima(charta) <= (i32)LXXII);

    imprimere("\n--- Probans chartam ex latina.h VIVA ---\n");
    latina = filum_legere_totum("include/latina.h", pn);
    CREDO_VERUM (latina.mensura > ZEPHYRUM);
    charta = briar_dialectus_charta(latina, VEXILLA_PLANA,
        VEXILLA_VITREA, pn);
    CREDO_VERUM (_linea_habet(charta, "i8", "= unsigned char"));
    CREDO_VERUM (_linea_habet(charta, "i32", "= unsigned int"));
    CREDO_VERUM (_linea_habet(charta, "s32", "= signed int"));
    CREDO_VERUM (_linea_habet(charta, "i64", "= unsigned long long"));
    CREDO_VERUM (_linea_habet(charta, "b32", "= int"));
    CREDO_VERUM (_linea_habet(charta, "nomen=typedef", ""));
    CREDO_VERUM (_linea_habet(charta, "casus=case", ""));
    CREDO_VERUM (_linea_habet(charta, "numeri:", "ZEPHYRUM=0"));
    CREDO_VERUM (_linea_longissima(charta) <= (i32)LXXII);

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(pn);
    redde praeteritus ? ZEPHYRUM : I;
}
