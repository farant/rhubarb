/* runae.c - Nucleus Unicode (vide runae.h)
 *
 * Proprietates ex tabulis generatis (lib/runae_tabulae.c, tools/
 * runae_generare.sh): octetus unus per runam (latitudo, classis
 * rupturae, basis variationis - include/runae_tabulae.h). Quaestio: duo
 * lectiones ordinum. Rupturae graphematum: UAX #29 15.1 per classes et
 * statum parvum a vocante possessum; latitudo graphematis: Ghostty
 * graphemeWidth (lib/runae.phase-log.md U4).
 */

#include "runae.h"
#include "runae_tabulae.h"
#include "utf8.h"

#define RUNA_MAXIMA 0x10FFFF

/* Status rupturae (RunaeRuptura.status): series quae in PRIORE finitur */
#define STATUS_RI_IMPAR       0x01   /* indicatores regionum impares */
#define STATUS_EMOJI          0x02   /* Extended_Pictographic Extend* */
#define STATUS_EMOJI_IUNCTOR  0x04   /* ... Extend* ZWJ (prior = ZWJ) */
#define STATUS_CONSONANS      0x08   /* InCB Consonant [Extend Linker]* */
#define STATUS_CONIUNCTOR     0x10   /* ... cum Linker viso */

interior b32
_valida (
    s32 runa)
{
    redde (b32)(runa >= ZEPHYRUM && runa <= RUNA_MAXIMA);
}

/* Octetus tabulae runae VALIDAE */
interior i32
_valor (
    s32 runa)
{
    i32 truncus = (i32)RUNAE_GRADUS_PRIMUS[(i32)runa >> VIII];

    redde (i32)RUNAE_GRADUS_SECUNDUS[truncus * RUNAE_TRUNCUS
        + ((i32)runa & 0xFF)];
}

/* Classis rupturae; runa invalida = REGIMEN (utrimque rumpit) */
interior i32
_classis (
    s32 runa)
{
    si (!_valida(runa))
    {
        redde RUNAE_CLASSIS_REGIMEN;
    }
    redde (_valor(runa) & RUNAE_CLASSIS_MASCULA)
        >> RUNAE_CLASSIS_POSITIO;
}

/* GCB Extend (quattuor species) */
interior b32
_extensio (
    i32 classis)
{
    redde (b32)(   classis == RUNAE_CLASSIS_EXTENSIO
                || classis == RUNAE_CLASSIS_EXTENSIO_INCB
                || classis == RUNAE_CLASSIS_CONIUNCTOR
                || classis == RUNAE_CLASSIS_MODIFICATOR);
}

/* uucode wcwidth_zero_in_grapheme, derivatum: latitudo 0, aut Prepend,
 * aut modificator emoji (lib/runae.phase-log.md U4) */
interior b32
_nulla_in_graphemate (
    s32 runa)
{
    i32 classis = _classis(runa);

    redde (b32)(   runae_latitudo(runa) == ZEPHYRUM
                || classis == RUNAE_CLASSIS_PRAEPOSITUM
                || classis == RUNAE_CLASSIS_MODIFICATOR);
}

i32
runae_latitudo (
    s32 runa)
{
    si (!_valida(runa))
    {
        redde I;   /* invalida: U+FFFD pingitur */
    }
    redde (i32)(_valor(runa) & RUNAE_LATITUDO_MASCULA);
}

vacuum
runae_rupturam_initiare (
    RunaeRuptura* ruptura)
{
    ruptura->status = ZEPHYRUM;
}

b32
runae_rumpitur (
             s32  prior,
             s32  runa,
    RunaeRuptura* ruptura)
{
    i32 p = _classis(prior);
    i32 c = _classis(runa);
    i32 s = ruptura->status;

    /* status: series quae in priore finitur (prior semel accipitur) */
    si (p == RUNAE_CLASSIS_REGIONIS)
    {
        s ^= STATUS_RI_IMPAR;
    }
    alioquin
    {
        s &= ~(i32)STATUS_RI_IMPAR;
    }
    si (p == RUNAE_CLASSIS_PICTOGRAPHUM)
    {
        s = (s | STATUS_EMOJI) & ~(i32)STATUS_EMOJI_IUNCTOR;
    }
    alioquin si ((s & STATUS_EMOJI) && _extensio(p))
    {
        s &= ~(i32)STATUS_EMOJI_IUNCTOR;
    }
    alioquin si ((s & STATUS_EMOJI) && p == RUNAE_CLASSIS_IUNCTOR)
    {
        s = (s & ~(i32)STATUS_EMOJI) | STATUS_EMOJI_IUNCTOR;
    }
    alioquin
    {
        s &= ~(i32)(STATUS_EMOJI | STATUS_EMOJI_IUNCTOR);
    }
    si (p == RUNAE_CLASSIS_CONSONANS)
    {
        s = (s | STATUS_CONSONANS) & ~(i32)STATUS_CONIUNCTOR;
    }
    alioquin si (   (s & STATUS_CONSONANS)
                 && (p == RUNAE_CLASSIS_EXTENSIO_INCB
                     || p == RUNAE_CLASSIS_IUNCTOR))
    {
        /* InCB Extend: series manet */
    }
    alioquin si (   (s & STATUS_CONSONANS)
                 && p == RUNAE_CLASSIS_CONIUNCTOR)
    {
        s |= STATUS_CONIUNCTOR;
    }
    alioquin
    {
        s &= ~(i32)(STATUS_CONSONANS | STATUS_CONIUNCTOR);
    }
    ruptura->status = s;

    /* GB3: CR x LF */
    si (p == RUNAE_CLASSIS_CR && c == RUNAE_CLASSIS_LF)
    {
        redde FALSUM;
    }
    /* GB4, GB5: regimina utrimque rumpunt */
    si (   p == RUNAE_CLASSIS_CR || p == RUNAE_CLASSIS_LF
        || p == RUNAE_CLASSIS_REGIMEN || c == RUNAE_CLASSIS_CR
        || c == RUNAE_CLASSIS_LF || c == RUNAE_CLASSIS_REGIMEN)
    {
        redde VERUM;
    }
    /* GB6-GB8: syllabae Hangul */
    si (   p == RUNAE_CLASSIS_SYLLABA_INITIALIS
        && (   c == RUNAE_CLASSIS_SYLLABA_INITIALIS
            || c == RUNAE_CLASSIS_SYLLABA_MEDIA
            || c == RUNAE_CLASSIS_SYLLABA_APERTA
            || c == RUNAE_CLASSIS_SYLLABA_CLAUSA))
    {
        redde FALSUM;
    }
    si (   (p == RUNAE_CLASSIS_SYLLABA_APERTA
            || p == RUNAE_CLASSIS_SYLLABA_MEDIA)
        && (c == RUNAE_CLASSIS_SYLLABA_MEDIA
            || c == RUNAE_CLASSIS_SYLLABA_FINALIS))
    {
        redde FALSUM;
    }
    si (   (p == RUNAE_CLASSIS_SYLLABA_CLAUSA
            || p == RUNAE_CLASSIS_SYLLABA_FINALIS)
        && c == RUNAE_CLASSIS_SYLLABA_FINALIS)
    {
        redde FALSUM;
    }
    /* GB9, GB9a, GB9b */
    si (   _extensio(c) || c == RUNAE_CLASSIS_IUNCTOR
        || c == RUNAE_CLASSIS_SPATIANS
        || p == RUNAE_CLASSIS_PRAEPOSITUM)
    {
        redde FALSUM;
    }
    /* GB9c: consonans [extend linker]* linker [extend linker]* x
     * consonans */
    si (   c == RUNAE_CLASSIS_CONSONANS && (s & STATUS_CONSONANS)
        && (s & STATUS_CONIUNCTOR))
    {
        redde FALSUM;
    }
    /* GB11: pictographum Extend* ZWJ x pictographum */
    si (c == RUNAE_CLASSIS_PICTOGRAPHUM && (s & STATUS_EMOJI_IUNCTOR))
    {
        redde FALSUM;
    }
    /* GB12, GB13: indicatores regionum per paria */
    si (   p == RUNAE_CLASSIS_REGIONIS && c == RUNAE_CLASSIS_REGIONIS
        && (s & STATUS_RI_IMPAR))
    {
        redde FALSUM;
    }
    redde VERUM;   /* GB999 */
}

constans i8*
runae_graphema_ex_politica (
      constans i8* initium,
      constans i8* finis,
    RunaePolitica  politica,
              i32* latitudo)
{
        constans i8* cursor = initium;

             s32 prior;
             s32 ultima;   /* Ghostty 'prev': runa ultima cum effectu */
             i32 lat;
    RunaeRuptura ruptura;


    *latitudo = ZEPHYRUM;
    si (initium >= finis)
    {
        redde initium;
    }
    prior = utf8_decodere(&cursor, finis);
    si (prior < ZEPHYRUM)
    {
        *latitudo = I;   /* series invalida: graphema suum (U+FFFD) */
        redde cursor;
    }
    lat     = runae_latitudo(prior);
    ultima  = prior;
    runae_rupturam_initiare(&ruptura);
    dum (cursor < finis)
    {
         constans i8* post = cursor;
                 s32  runa = utf8_decodere(&post, finis);

        si (runa < ZEPHYRUM || runae_rumpitur(prior, runa, &ruptura))
        {
            frange;   /* invalida aut limes: graphema finitur */
        }
        si (   politica        == RUNAE_POLITICA_SIMPLEX
            && _classis(prior) == RUNAE_CLASSIS_IUNCTOR
            && _classis(runa)  == RUNAE_CLASSIS_PICTOGRAPHUM)
        {
            frange;   /* SIMPLEX: ZWJ pictographa non iungit (GB11 non) */
        }
        si (runa == 0xFE0F || runa == 0xFE0E)
        {
            /* VS16/VS15 solum post basim variationis; aliter nullus
             * effectus, ultima manet (Ghostty .ignore) */
            si (_valor(ultima) & RUNAE_BASIS_VARIATIONIS)
            {
                lat     = (runa == 0xFE0F) ? II : I;
                ultima  = runa;
            }
        }
                alioquin si (!_nulla_in_graphemate(runa))
        {
            lat     = II;   /* runa latitudinem conferens */
            ultima  = runa;
        }
        alioquin
        {
            ultima = runa;
        }
        prior   = runa;
        cursor  = post;
    }
    *latitudo = lat;
    redde cursor;
}

constans i8*
runae_graphema_proximum (
    constans i8* initium,
    constans i8* finis,
            i32* latitudo)
{
    redde runae_graphema_ex_politica(initium, finis,
        RUNAE_POLITICA_GRAPHEMATUM, latitudo);
}
