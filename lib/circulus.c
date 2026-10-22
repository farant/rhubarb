/* circulus.c - anulus SPSC (vide include/circulus.h)
 *
 * Scriptor: caudam LEGIT (atomus_legere - spatium liberum?), locum
 * implet, caput PUBLICAT (atomus_publicare - post copiam). Lector
 * speculariter: caput legit, locum copiat, caudam publicat. Index
 * proprius sine atomo legitur (solus eum scribit). Vide
 * lib/circulus.worklog.md.
 */
#include "circulus.h"
#include "atomus.h"
#include <string.h>

b32
circulus_parare (
    Circulus* c,
      vacuum* memoria,
         i32  capacitas,
         i32  mensura)
{
    /* potentia II: unum bitum solum */
    si (memoria == NIHIL || mensura == ZEPHYRUM || capacitas == ZEPHYRUM
        || (capacitas & (capacitas - I)) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    c->loci       = (i8*)memoria;
    c->capacitas  = capacitas;
    c->mensura    = mensura;
    c->caput      = ZEPHYRUM;
    c->cauda      = ZEPHYRUM;
    redde VERUM;
}

b32
circulus_scribere (
             Circulus* c,
      constans vacuum* elementum)
{
    i32 caput = c->caput;
    i32 cauda = atomus_legere(&c->cauda);

    si (caput - cauda >= c->capacitas)
    {
        redde FALSUM;
    }
    memcpy(c->loci + (caput & (c->capacitas - I)) * c->mensura,
        elementum,
           (memoriae_index)c->mensura);
    atomus_publicare(&c->caput, caput + I);
    redde VERUM;
}

b32
circulus_legere (
    Circulus* c,
      vacuum* elementum)
{
    i32 cauda = c->cauda;
    i32 caput = atomus_legere(&c->caput);

    si (caput == cauda)
    {
        redde FALSUM;
    }
    memcpy(elementum, c->loci + (cauda & (c->capacitas
        - I)) * c->mensura,
           (memoriae_index)c->mensura);
    atomus_publicare(&c->cauda, cauda + I);
    redde VERUM;
}

i32
circulus_numerus (
    constans Circulus* c)
{
    redde atomus_legere(&c->caput) - atomus_legere(&c->cauda);
}
