#include "numerus_romanus.h"


/* ====================================================================
 * Vide numerus_romanus.h de ratione strictitudinis.
 * ==================================================================== */

/* Valorem characteris reddere; ZEPHYRUM si non Romanus.
 * 'minuscula' per parametrum, non per utrumque casum acceptum: casus
 * MIXTUS ('Xii', 'iV') verbum est aut mendum, non numerus, et
 * acceptus regulam strictam supra dictam vacuam faceret. */
interior i32
_valor (
     i8 c,
    b32 minuscula)
{
    si (minuscula)
    {
        commutatio (c)
        {
            casus 'i': redde I;
            casus 'v': redde V;
            casus 'x': redde X;
            casus 'l': redde L;
            casus 'c': redde C;
            casus 'd': redde D;
            casus 'm': redde M;
            ordinarius: frange;
        }
        redde ZEPHYRUM;
    }
    commutatio (c)
    {
        casus 'I': redde I;
        casus 'V': redde V;
        casus 'X': redde X;
        casus 'L': redde L;
        casus 'C': redde C;
        casus 'D': redde D;
        casus 'M': redde M;
        ordinarius: frange;
    }
    redde ZEPHYRUM;
}

/* coniunctiones subtractivae licitae: IV IX XL XC CD CM */
interior b32
_par_subtractivum (
    i32 minor,
    i32 maior)
{
    si (minor == I && (maior == V || maior == X))    redde VERUM;
    si (minor == X && (maior == L || maior == C))    redde VERUM;
    si (minor == C && (maior == D || maior == M))    redde VERUM;
    redde FALSUM;
}

b32
numerus_romanus_legere (
    chorda  s,
       i32* valor)
{
    i32 i      = ZEPHYRUM;
    i32 summa  = ZEPHYRUM;
    b32 minuscula;
    /* limes: character princeps gregis proximi HOC minor esse debet.
     * Ita 'XXXIX' licet (post XXX limes X est, I princeps minor) sed
     * 'IXX' non (post IX limes I est). */
    i32 limes = M + I;

    si (s.mensura == ZEPHYRUM || s.datum == NIHIL)
    {
        redde FALSUM;
    }
    /* casus ex charactere PRIMO sumitur, deinde per omnes tenetur */
    minuscula = (s.datum[ZEPHYRUM] >= 'a' && s.datum[ZEPHYRUM] <= 'z')
        ? VERUM : FALSUM;

    dum (i < s.mensura)
    {
        i32 v = _valor(s.datum[i], minuscula);
        i32 w;

        si (v == ZEPHYRUM)
        {
            redde FALSUM;   /* non-Romanus aut casus mixtus */
        }

        w = (i + I < s.mensura) ? _valor(s.datum[i + I], minuscula)
                                : ZEPHYRUM;

        si (w > v)
        {
            /* grex subtractivus */
            si (!_par_subtractivum(v, w) || v >= limes)
            {
                redde FALSUM;
            }
            summa  += w - v;
            limes  = v;
            i      += II;
        }
        alioquin
        {
            /* grex additivus: cursus eiusdem characteris */
            i32 numerus = I;

            dum (   i + numerus < s.mensura
                 && s.datum[i + numerus] == s.datum[i])
            {
                numerus++;
            }
            si (v >= limes) redde FALSUM;
            si (numerus > III) redde FALSUM;
            /* V L D bis stare nequeunt (VV = X scribendum) */
            si (numerus > I && (v == V || v == L || v == D))
            {
                redde FALSUM;
            }
            summa  += v * numerus;
            limes  = v;
            i      += numerus;
        }
    }

    si (valor != NIHIL) *valor = summa;
    redde VERUM;
}
