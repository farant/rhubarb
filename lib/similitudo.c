/* similitudo.c - congruentia subsequentiae nebulosa (vide .h) */
#include "similitudo.h"

/* pondera punctationis (exemplar fzf v1; numeri integri) */
#define PRAEMIUM_LIMITIS     XVI   /* congruentia ad initium verbi */
#define PRAEMIUM_CONTINUI    VIII  /* continua cum congruentia priore */
#define PRAEMIUM_CASUS       I     /* casus exactus litterae */
#define POENA_HIATUS_APERTI  III   /* hiatus inter congruentias */
#define POENA_HIATUS_LONGI   I     /* per litteram ultra primam */

interior character
_minusculum (character c)
{
    si (c >= 'A' && c <= 'Z')
    {
        redde (character)(c + ('a' - 'A'));
    }
    redde c;
}

interior b32
_est_maiuscula (character c)
{
    redde (c >= 'A' && c <= 'Z') ? VERUM : FALSUM;
}

interior b32
_est_minuscula (character c)
{
    redde (c >= 'a' && c <= 'z') ? VERUM : FALSUM;
}

/* initium verbi? positio 0; post separatorem; transitus
 * minusculae -> MAIUSCULAE (CamelCase: MedullaLineae) */
interior b32
_limes_verbi (chorda candidatus, i32 positio)
{
    character prior;
    character praesens;

    si (positio == ZEPHYRUM) redde VERUM;
    prior = (character)candidatus.datum[positio - I];
    praesens = (character)candidatus.datum[positio];
    si (prior == '_' || prior == '-' || prior == '.'
        || prior == '/' || prior == ':' || prior == ' ')
    {
        redde VERUM;
    }
    si (_est_minuscula(prior) && _est_maiuscula(praesens))
    {
        redde VERUM;
    }
    redde FALSUM;
}

/* ambulatio avara communis (punctare + decurtata - corpus UNUM ne
 * punctationes umquam divergant). Punctum cumulativum ad
 * consumptionem k == punctare(praefixum_k) EXACTE: ambulatio
 * deterministica, litterae candidati post consumptionem numquam
 * punctant. Exitus: consumpta (quot litterae quaestionis
 * congruerunt), punctum_finale (post omnes), punctum_optimum =
 * maximum trans consumptiones >= pavimento (validum si
 * optimum_est). */
interior vacuum
_ambulare (chorda quaestio, chorda candidatus, i32 pavimentum,
    i32* consumpta, s32* punctum_finale, s32* punctum_optimum,
    b32* optimum_est)
{
    s32 punctum = ZEPHYRUM;
    s32 optimum = ZEPHYRUM;
    b32 ullum = FALSUM;
    i32 qi = ZEPHYRUM;
    i32 ci;
    i32 prior = ZEPHYRUM;
    b32 prima_facta = FALSUM;

    per (ci = ZEPHYRUM;
         ci < candidatus.mensura && qi < quaestio.mensura; ci++)
    {
        character lc = (character)candidatus.datum[ci];
        character lq = (character)quaestio.datum[qi];

        si (_minusculum(lc) != _minusculum(lq)) perge;

        /* poena hiatus ANTE hanc congruentiam */
        si (prima_facta && ci > prior + I)
        {
            i32 hiatus = ci - prior - I;

            punctum -= POENA_HIATUS_APERTI
                + (s32)(hiatus - I) * POENA_HIATUS_LONGI;
        }
        si (_limes_verbi(candidatus, ci))
        {
            punctum += PRAEMIUM_LIMITIS;
        }
        si (prima_facta && ci == prior + I)
        {
            punctum += PRAEMIUM_CONTINUI;
        }
        si (lc == lq)
        {
            punctum += PRAEMIUM_CASUS;
        }
        prior = ci;
        prima_facta = VERUM;
        qi++;
        si (qi >= pavimentum && (!ullum || punctum > optimum))
        {
            optimum = punctum;
            ullum = VERUM;
        }
    }
    *consumpta = qi;
    *punctum_finale = punctum;
    *punctum_optimum = optimum;
    *optimum_est = ullum;
}

b32
similitudo_punctare (
    chorda  quaestio,
    chorda  candidatus,
       s32* punctum_exitus)
{
    i32 consumpta;
    s32 finale;
    s32 optimum;
    b32 optimum_est;

    si (punctum_exitus == NIHIL) redde FALSUM;
    si (quaestio.mensura == ZEPHYRUM)
    {
        *punctum_exitus = ZEPHYRUM;
        redde VERUM;
    }
    si (candidatus.mensura == ZEPHYRUM) redde FALSUM;

    _ambulare(quaestio, candidatus, quaestio.mensura, &consumpta,
        &finale, &optimum, &optimum_est);
    si (consumpta != quaestio.mensura) redde FALSUM;
    *punctum_exitus = finale;
    redde VERUM;
}

/* a melior quam fructus f? (punctum desc, tum brevior, tum index) */
interior b32
_melior (s32 punctum, i32 index, constans chorda* candidati,
    constans SimilitudoFructus* f)
{
    si (punctum != f->punctum) redde (punctum > f->punctum);
    si (candidati[index].mensura != candidati[f->index].mensura)
    {
        redde (candidati[index].mensura
            < candidati[f->index].mensura);
    }
    redde (index < f->index);
}

/* inserere in fructus ordinatos (a fine; ultimus cadit si plenum);
 * scripta novum redditur */
interior i32
_inserere (SimilitudoFructus* fructus, i32 scripta, i32 maxima,
    constans chorda* candidati, i32 k, s32 punctum)
{
    i32 loco = scripta;

    dum (loco > ZEPHYRUM
        && _melior(punctum, k, candidati, &fructus[loco - I]))
    {
        loco--;
    }
    si (loco >= maxima)
    {
        redde scripta;
    }
    {
        i32 m = (scripta < maxima) ? scripta : maxima - I;

        dum (m > loco)
        {
            fructus[m] = fructus[m - I];
            m--;
        }
    }
    fructus[loco].index = k;
    fructus[loco].punctum = punctum;
    redde (scripta < maxima) ? scripta + I : scripta;
}

i32
similitudo_optima (
    chorda             quaestio,
    constans chorda*   candidati,
    i32                numerus_candidatorum,
    SimilitudoFructus* fructus,
    i32                maxima)
{
    i32 scripta = ZEPHYRUM;
    i32 k;

    si (fructus == NIHIL || candidati == NIHIL
        || maxima == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }

    per (k = ZEPHYRUM; k < numerus_candidatorum; k++)
    {
        s32 punctum;

        si (!similitudo_punctare(quaestio, candidati[k], &punctum))
        {
            perge;
        }
        scripta = _inserere(fructus, scripta, maxima, candidati,
            k, punctum);
    }
    redde scripta;
}

i32
similitudo_optima_decurtata (chorda quaestio,
    constans chorda* candidati, i32 numerus_candidatorum,
    SimilitudoFructus* fructus, i32 maxima)
{
    i32 scripta = ZEPHYRUM;
    i32 k;

    si (fructus == NIHIL || candidati == NIHIL
        || maxima == ZEPHYRUM || quaestio.mensura < III)
    {
        redde ZEPHYRUM;
    }

    per (k = ZEPHYRUM; k < numerus_candidatorum; k++)
    {
        i32 consumpta;
        s32 finale;
        s32 optimum;
        b32 optimum_est;

        si (candidati[k].mensura == ZEPHYRUM)
        {
            perge;
        }
        _ambulare(quaestio, candidati[k], III, &consumpta,
            &finale, &optimum, &optimum_est);
        si (!optimum_est)
        {
            perge;
        }
        scripta = _inserere(fructus, scripta, maxima, candidati,
            k, optimum);
    }
    redde scripta;
}
