#include "paginatio.h"
#include "numerus_romanus.h"


/* ====================================================================
 * Vide paginatio.h de ratione.
 * ==================================================================== */

interior b32
_album (
    i8 c)
{
    redde (c == ' ' || c == '\t') ? VERUM : FALSUM;
}

Paginatio
paginatio_legere (
    chorda s)
{
    Paginatio p;
          i32 initium = ZEPHYRUM;
          i32 finis;
          i32 i;
          i32 valor = ZEPHYRUM;
       chorda nucleus;

    p.genus = PAGINATIO_NULLA;
    p.valor = ZEPHYRUM;

    si (s.datum == NIHIL || s.mensura == ZEPHYRUM)
    {
        redde p;
    }
    finis = s.mensura;
    dum (initium < finis && _album(s.datum[initium])) initium++;
    dum (finis > initium && _album(s.datum[finis - I])) finis--;
    si (initium >= finis)
    {
        redde p;
    }
    nucleus.datum    = s.datum + initium;
    nucleus.mensura  = finis - initium;

    /* Arabica: cifrae SOLAE. Signum '+'/'-' non accipitur - pagina
     * negativa non est, et '+12' mendum est, non comitas. */
    si (   nucleus.datum[ZEPHYRUM] >= '0'
        && nucleus.datum[ZEPHYRUM] <= '9')
    {
        per (i = ZEPHYRUM; i < nucleus.mensura; i++)
        {
            si (nucleus.datum[i] < '0' || nucleus.datum[i] > '9')
            {
                redde p;   /* '12a' RESPUITUR */
            }
            /* tectum: ne cursus cifrarum immanis circumvolvat */
            si (valor > (PAGINATIO_BANDA / X))
            {
                redde p;
            }
            valor = valor * X + (i32)(nucleus.datum[i] - '0');
        }
        si (valor == ZEPHYRUM)
        {
            redde p;   /* pagina nulla in libro non est */
        }
        p.genus = PAGINATIO_ARABICA;
        p.valor = valor;
        redde p;
    }

    si (numerus_romanus_legere(nucleus, &valor))
    {
        p.genus = PAGINATIO_ROMANA;
        p.valor = valor;
    }
    redde p;
}

i64
paginatio_clavis (
    Paginatio p)
{
    si (p.genus == PAGINATIO_ROMANA)
    {
        redde (i64)p.valor;
    }
    si (p.genus == PAGINATIO_ARABICA)
    {
        redde (i64)PAGINATIO_BANDA + (i64)p.valor;
    }
    /* nulla ULTIMA: banda duplex supra omnes veras */
    redde (i64)PAGINATIO_BANDA * (i64)II;
}

i64
paginatio_clavis_chordae (
    chorda s)
{
    redde paginatio_clavis(paginatio_legere(s));
}
