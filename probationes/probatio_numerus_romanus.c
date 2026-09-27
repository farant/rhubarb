/* probatio_numerus_romanus.c - Probationes numerorum Romanorum:
 * legere (strictum), scribere (canonicum, 1-3999), exprimere (omnis n
 * ut expressio C89 in vocabulario latina.h). */
#include "latina.h"
#include "numerus_romanus.h"
#include "piscina.h"
#include "credo.h"
#include "sors.h"
#include <stdio.h>
#include <string.h>


/* ==================================================
 * AESTIMATOR - expressionem exprimere() redditam ut compilator C
 * aestimat, cum semantica int: productum aut summa sine '(i64)' quae
 * 0x7FFFFFFF excedit = involutio (UB) -> *involutio VERUM.
 *
 *   expressio := terminus (" + " terminus)*
 *   terminus  := factor (" * " factor)*
 *   factor    := ["(i64)"] ( "(" expressio ")" | NUMERUS | "ZEPHYRUM" )
 * ================================================== */

#define INT_LIMES ((i64)0x7FFFFFFF)

nomen structura {
                chorda  e;           /* expressio tota */
    constans character* s;
                   i32  i;
                   i32  n;
                   b32  vitium;      /* forma non intellecta */
                   b32  involutio;   /* int excessit sine (i64) */
} Aestimator;

interior i64
_expressio (
    Aestimator* a,
           b32* lata);

interior b32
_sequitur (
             Aestimator* a,
     constans character* t)
{
    i32 l = (i32)strlen(t);

    si (   a->i + l                          <= a->n
        && memcmp(a->s + a->i, t, (size_t)l) == ZEPHYRUM)
    {
        a->i += l;
        redde VERUM;
    }
    redde FALSUM;
}

interior i64
_factor (
    Aestimator* a,
           b32* lata)
{
    i64 v = ZEPHYRUM;

    *lata = _sequitur(a, "(i64)");
    si (_sequitur(a, "("))
    {
        b32 interna;

        v      = _expressio(a, &interna);
        *lata  = *lata || interna;
        si (!_sequitur(a, ")"))
        {
            a->vitium = VERUM;
        }
        redde v;
    }
    si (_sequitur(a, "ZEPHYRUM"))
    {
        redde ZEPHYRUM;
    }
    {
           i32 initium  = a->i;
           i32 valor    = ZEPHYRUM;
        chorda c;

        dum (a->i < a->n && strchr("IVXLCDM", a->s[a->i]) != NIHIL)
        {
            a->i++;
        }
        c = chorda_sectio(a->e, initium, a->i);
        si (!numerus_romanus_legere(c, &valor))
        {
            a->vitium = VERUM;
        }
        redde (i64)valor;
    }
}

interior i64
_terminus (
    Aestimator* a,
           b32* lata)
{
    i64 v = _factor(a, lata);

    dum (_sequitur(a, " * "))
    {
        b32 l2;
        i64 w = _factor(a, &l2);

        *lata  = *lata || l2;
        v      *= w;
        si (!*lata && v > INT_LIMES)
        {
            a->involutio = VERUM;
        }
    }
    redde v;
}

interior i64
_expressio (
    Aestimator* a,
           b32* lata)
{
    i64 v = _terminus(a, lata);

    dum (_sequitur(a, " + "))
    {
        b32 l2;

        v      += _terminus(a, &l2);
        *lata  = *lata || l2;
        si (!*lata && v > INT_LIMES)
        {
            a->involutio = VERUM;
        }
    }
    redde v;
}

/* VERUM si expressio e aestimata == n, forma tota intellecta, nulla
 * involutio int */
interior b32
_aestimare (
    chorda e,
       i64 n)
{
    Aestimator a;
           b32 lata;
           i64 v;

    a.e          = e;
    a.s          = (constans character*)e.datum;
    a.i          = ZEPHYRUM;
    a.n          = (i32)e.mensura;
    a.vitium     = FALSUM;
    a.involutio  = FALSUM;
    si (e.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    v = _expressio(&a, &lata);
    redde !a.vitium && !a.involutio && a.i == a.n && v == n;
}


s32 principale (vacuum)
{
         b32  praeteritus;
     Piscina* piscina;
     Piscina* p;

    piscina = piscina_generare_dynamicum("probatio_numerus_romanus",
                                         MMMMXCVI);
    p = piscina_generare_dynamicum("probatio_numerus_romanus_ansa",
                                   LXIV * MXXIV);
    si (!piscina || !p)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * PROBARE: scribere - canonicum et par cum legere, 1-3999
     * ================================================== */

    {
        i32 n;
        b32 omnes = VERUM;

        imprimere("\n--- Probans scribere (par cum legere) ---\n");

        CREDO_CHORDA_AEQUALIS_LITERIS(numerus_romanus_scribere(CVI, p),
                                      "CVI");
        CREDO_CHORDA_AEQUALIS_LITERIS(numerus_romanus_scribere(XIV, p),
                                      "XIV");
        CREDO_CHORDA_AEQUALIS_LITERIS(numerus_romanus_scribere(
            MMM + CM + XC + IX, p), "MMMCMXCIX");
        CREDO_CHORDA_AEQUALIS_LITERIS(numerus_romanus_scribere(M + IX,
            p),
                                      "MIX");
        /* nullus numerus Romanus: 0, ultra MMMCMXCIX */
        CREDO_CHORDA_VACUA(numerus_romanus_scribere(ZEPHYRUM, p));
        CREDO_CHORDA_VACUA(numerus_romanus_scribere(MMMM, p));

        per (n = I; n <= MMM + CM + XC + IX; n++)
        {
            i32 valor = ZEPHYRUM;

            si (   !numerus_romanus_legere(
                        numerus_romanus_scribere(n, p), &valor)
                || valor != n)
            {
                imprimere("  par fractum: %u\n", n);
                omnes = FALSUM;
                frange;
            }
            si (n % C == ZEPHYRUM)
            {
                piscina_vacare(p);
            }
        }
        CREDO_VERUM(omnes);
        piscina_vacare(p);
    }


    /* ==================================================
     * PROBARE: legere manet strictum (verba Anglica, MMMM)
     * ================================================== */

    {
        imprimere("\n--- Probans legere strictum ---\n");

        CREDO_FALSUM(numerus_romanus_legere(chorda_ex_literis("CIVIC",
            p),
                                            NIHIL));
        CREDO_FALSUM(numerus_romanus_legere(chorda_ex_literis("LIVID",
            p),
                                            NIHIL));
        /* ultra classicum: forma 'IV * M' est, non MMMM */
        CREDO_FALSUM(numerus_romanus_legere(chorda_ex_literis("MMMM",
            p),
                                            NIHIL));
        piscina_vacare(p);
    }


    /* ==================================================
     * PROBARE: exprimere - formae canonicae nominatae
     * ================================================== */

    {
        nomen structura {
                           i64  n;
            constans character* forma;
                           b32  compositum;
        } Exemplum;
        /* valores magni ex numeralibus computantur (i64 C89 sine
         * suffixo litteralis) */
        Exemplum exempla[XVI];
             i32 k;
             i32 numerus = ZEPHYRUM;

        imprimere("\n--- Probans exprimere (formae canonicae) ---\n");

#define EXEMPLUM(v, f, c) exempla[numerus].n = (v); \
    exempla[numerus].forma = (f); \
    exempla[numerus].compositum = (c); numerus++

        EXEMPLUM((i64)ZEPHYRUM, "ZEPHYRUM", FALSUM);
        EXEMPLUM((i64)CVI, "CVI", FALSUM);
        EXEMPLUM((i64)(MMM + CM + XC + IX), "MMMCMXCIX", FALSUM);
        EXEMPLUM((i64)MMMM, "IV * M", VERUM);
        EXEMPLUM((i64)MMMMXCVI, "IV * MXXIV", VERUM);
        EXEMPLUM((i64)(V * M + CCLXXX), "V * M + CCLXXX", VERUM);
        EXEMPLUM((i64)LXIV * MXXIV, "LXIV * MXXIV", VERUM);
        /* milia rotunda milia manent (128000 = CXXV * MXXIV quoque) */
        EXEMPLUM((i64)CXXVIII * M, "CXXVIII * M", VERUM);
        EXEMPLUM((i64)M * M, "M * M", VERUM);
        EXEMPLUM((i64)MXXIV * MXXIV, "MXXIV * MXXIV", VERUM);
        EXEMPLUM((i64)(M + D) * M, "MD * M", VERUM);
        EXEMPLUM((i64)IV * M * M + (i64)D * M, "IV * M * M + D * M",
            VERUM);
        /* int excessum: (i64) ante factorem primum */
        EXEMPLUM((i64)MMMMXCVI * MXXIV * MXXIV,
              "(i64)IV * MXXIV * MXXIV * MXXIV", VERUM);
        EXEMPLUM((i64)V * M * M * M, "(i64)V * M * M * M", VERUM);
        /* summa binaria intra factorem: parentheses */
        EXEMPLUM(((i64)IV * M + DI) * MXXIV, "(IV * M + DI) * MXXIV",
            VERUM);
        /* idem cum milibus rotundis: 4500 * 1024 = 4608000 -> milia */
        EXEMPLUM(((i64)IV * M + D) * MXXIV, "IV * M * M + DCVIII * M",
                 VERUM);
#undef EXEMPLUM

        per (k = ZEPHYRUM; k < numerus; k++)
        {
               b32 c = !exempla[k].compositum;
            chorda e = numerus_romanus_exprimere(exempla[k].n, &c, p);

            si (!chorda_aequalis_literis(e, exempla[k].forma))
            {
                imprimere("  exspectatum '%s', redditum '%.*s'\n",
                          exempla[k].forma, (integer)e.mensura,
                          (constans character*)e.datum);
            }
            CREDO_VERUM(chorda_aequalis_literis(e, exempla[k].forma));
            CREDO_VERUM(c == exempla[k].compositum);
            CREDO_VERUM(_aestimare(e, exempla[k].n));
        }
        piscina_vacare(p);
    }


    /* ==================================================
     * PROBARE: exprimere - proprietas: aestimatio == n, sine
     * involutione int (0-LXX milia omnes; multipla MXXIV et M;
     * seriei pseudo-fortuitae usque ad 2^40)
     * ================================================== */

    {
         i64 n;
         i64 k;
        Sors sors;
         i32 j;
         b32 omnes = VERUM;

        imprimere("\n--- Probans exprimere (aestimatio == n) ---\n");

        per (n = ZEPHYRUM; n <= (i64)LXX * M && omnes; n++)
        {
            si (!_aestimare(numerus_romanus_exprimere(n, NIHIL, p), n))
            {
                imprimere("  fractum: %lu\n", (unsigned long)n);
                omnes = FALSUM;
            }
            si (n % (i64)C == ZEPHYRUM)
            {
                piscina_vacare(p);
            }
        }
        per (k = I; k <= (i64)MMMMXCVI && omnes; k++)
        {
            i64 duo[II];

            duo[ZEPHYRUM]  = k * (i64)MXXIV;
            duo[I]         = k * (i64)M * (i64)M;
            per (j = ZEPHYRUM; j < II; j++)
            {
                si (!_aestimare(numerus_romanus_exprimere(duo[j], NIHIL,
                                                          p), duo[j]))
                {
                    imprimere("  fractum: %lu\n",
                              (unsigned long)duo[j]);
                    omnes = FALSUM;
                }
            }
            si (k % (i64)C == ZEPHYRUM)
            {
                piscina_vacare(p);
            }
        }
        /* series pseudo-fortuita (sors, semen fixum) intra 2^40 */
        sors_seminare(&sors, (i64)MMMMXCVI, (i64)ZEPHYRUM);
        per (j = ZEPHYRUM; j < X * M && omnes; j++)
        {
            n = (((i64)sors_proximum(&sors) << XXXII)
                 | (i64)sors_proximum(&sors))
                & (((i64)I << XL) - (i64)I);
            si (!_aestimare(numerus_romanus_exprimere(n, NIHIL, p), n))
            {
                imprimere("  fractum: %lu\n", (unsigned long)n);
                omnes = FALSUM;
            }
            si (j % C == ZEPHYRUM)
            {
                piscina_vacare(p);
            }
        }
        CREDO_VERUM(omnes);
    }


    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(p);
    piscina_destruere(piscina);

    redde praeteritus ? ZEPHYRUM : I;
}
