#include "numerus_romanus.h"
#include "chorda_aedificator.h"


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


/* ==================================================
 * SCRIBERE ET EXPRIMERE
 * ================================================== */

#define ROMANUS_MAXIMUS      (MMM + CM + XC + IX)   /* MMMCMXCIX */
#define INT_MAXIMUS_VALOR    ((i64)0x7FFFFFFF)

interior constans i32 VALORES_GREGUM[XIII] = {
    M, CM, D, CD, C, XC, L, XL, X, IX, V, IV, I
};
interior constans character* LITTERAE_GREGUM[XIII] = {
    "M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV",
    "I"
};

chorda
numerus_romanus_scribere (
         i32  n,
    Piscina* piscina)
{
    ChordaAedificator* aed;
                  i32  k;

    si (n == ZEPHYRUM || n > ROMANUS_MAXIMUS || piscina == NIHIL)
    {
        chorda vacua;

        vacua.datum    = NIHIL;
        vacua.mensura  = ZEPHYRUM;
        redde vacua;
    }
    aed = chorda_aedificator_creare(piscina, XVI);
    per (k = ZEPHYRUM; k < XIII; k++)
    {
        dum (n >= VALORES_GREGUM[k])
        {
            chorda_aedificator_appendere_literis(aed,
                LITTERAE_GREGUM[k]);
            n -= VALORES_GREGUM[k];
        }
    }
    redde chorda_aedificator_finire(aed);
}

/* Terminum 'r * M * M ...' (potentia milium) appendere */
interior vacuum
_terminum_appendere (
    ChordaAedificator* aed,
                  i32  r,
                  i32  potentia,
              Piscina* piscina)
{
    i64 valor = (i64)r;
    i32 k;

    per (k = ZEPHYRUM; k < potentia; k++)
    {
        valor *= (i64)M;
    }
    si (valor > INT_MAXIMUS_VALOR)
    {
        chorda_aedificator_appendere_literis(aed, "(i64)");
    }
    chorda_aedificator_appendere_chorda(aed,
        numerus_romanus_scribere(r, piscina));
    per (k = ZEPHYRUM; k < potentia; k++)
    {
        chorda_aedificator_appendere_literis(aed, " * M");
    }
}

chorda
numerus_romanus_exprimere (
         i64  n,
         b32* compositum,
    Piscina* piscina)
{
    ChordaAedificator* aed;

    si (compositum != NIHIL)
    {
        *compositum = FALSUM;
    }
    si (piscina == NIHIL)
    {
        chorda vacua;

        vacua.datum    = NIHIL;
        vacua.mensura  = ZEPHYRUM;
        redde vacua;
    }
    si (n == ZEPHYRUM)
    {
        redde chorda_ex_literis("ZEPHYRUM", piscina);
    }
    si (n <= (i64)ROMANUS_MAXIMUS)
    {
        redde numerus_romanus_scribere((i32)n, piscina);
    }
    si (compositum != NIHIL)
    {
        *compositum = VERUM;
    }
    aed = chorda_aedificator_creare(piscina, LXIV);

    /* familia binaria: multiplum MXXIV, non milium rotundum */
    si (n % (i64)MXXIV == ZEPHYRUM && n % (i64)M != ZEPHYRUM)
    {
           b32 summa;
        chorda factor = numerus_romanus_exprimere(n / (i64)MXXIV,
                                                         NIHIL,
                                                         piscina);

        /* 'IV * M + D' ut factor parentheses poscit; productum non */
        summa = chorda_continet(factor,
                                chorda_ex_literis("+", piscina));
        si (   n > INT_MAXIMUS_VALOR
            && !chorda_incipit(factor,
                               chorda_ex_literis("(i64)", piscina)))
        {
            chorda_aedificator_appendere_literis(aed, "(i64)");
        }
        si (summa)
        {
            chorda_aedificator_appendere_literis(aed, "(");
        }
        chorda_aedificator_appendere_chorda(aed, factor);
        si (summa)
        {
            chorda_aedificator_appendere_literis(aed, ")");
        }
        chorda_aedificator_appendere_literis(aed, " * MXXIV");
        redde chorda_aedificator_finire(aed);
    }

    /* familia milium (vinculum): greges ab imo colliguntur, ab alto
     * scribuntur. Grex supremus < MMMM totus stat ('MD * M', non
     * 'M * M + D * M'). */
    {
        i32 residua[XXIV];
        i32 potentiae[XXIV];
        i32 numerus   = ZEPHYRUM;
        i32 potentia  = ZEPHYRUM;
        s32 k;   /* s32: ad -1 descendit (i32 involveretur) */

        dum (n > ZEPHYRUM && numerus < XXIV)
        {
            si (n <= (i64)ROMANUS_MAXIMUS)
            {
                residua[numerus]    = (i32)n;
                potentiae[numerus]  = potentia;
                numerus++;
                frange;
            }
            si (n % (i64)M != ZEPHYRUM)
            {
                residua[numerus]    = (i32)(n % (i64)M);
                potentiae[numerus]  = potentia;
                numerus++;
            }
            n /= (i64)M;
            potentia++;
        }
        per (k = (s32)numerus - I; k >= ZEPHYRUM; k--)
        {
            _terminum_appendere(aed, residua[k], potentiae[k], piscina);
            si (k > ZEPHYRUM)
            {
                chorda_aedificator_appendere_literis(aed, " + ");
            }
        }
    }
    redde chorda_aedificator_finire(aed);
}
