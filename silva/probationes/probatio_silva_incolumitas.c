/* probatio_silva_incolumitas.c - Incolumitas a fragore (fuzzing)
 *
 * Spec-v2 §8.2: "crash-freedom fuzzing from the walking skeleton on...
 * runs with the normal test suite". Buffones DETERMINISTICI (xorshift,
 * semen fixum - reproducibilis, sine horologio) per lexare -> emittere
 * (fidelitas octetorum in QUOLIBET input) et per directivas + expansio
 * positionalis (numquam fragor, tempus terminatum per caps).
 *
 * Circuitus A: octeti mere fortuiti (0..255, NUL inclusi).
 * Circuitus B: characteres C-formes (directivae, quotes, laminae) -
 * praeprocessor profundius exercetur.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_lexema.h"
#include "silva_expandere.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define ITERATIONES 200
#define MENSURA_MAXIMA 512

/* xorshift32 - deterministicum (semen fixum) */
interior i32 semen_globale = 0x5EEDC0DEu;

interior i32
_fortuitum (vacuum)
{
    i32 x = semen_globale;

    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    semen_globale = x;
    redde x;
}

/* Buffonem implere octetis fortuitis */
interior i32
_buffo_crudus (i8* buffo, i32 mensura_maxima)
{
    i32 mensura = _fortuitum() % (mensura_maxima + I);
    i32 i;

    per (i = ZEPHYRUM; i < mensura; i++)
    {
        buffo[i] = (i8)(_fortuitum() & 0xFFu);
    }
    redde mensura;
}

/* Buffonem implere characteribus C-formibus */
interior i32
_buffo_c_formis (i8* buffo, i32 mensura_maxima)
{
    hic_manens constans character MATERIA[] =
        "abcdefgXYZ_0123456789 \t\n\r\\\"'#(){}[];,.*+-/<>=!&|?:";
    i32 numerus_materiae = (i32)(magnitudo(MATERIA) - I);
    i32 mensura = _fortuitum() % (mensura_maxima + I);
    i32 i;

    per (i = ZEPHYRUM; i < mensura; i++)
    {
        buffo[i] = (i8)MATERIA[_fortuitum() % numerus_materiae];
    }
    redde mensura;
}

/* Unum specimen: fidelitas lexatoris + expansio sine fragore.
 * Reddit VERUM si fidelis. */
interior b32
_specimen (constans i8* buffo, i32 mensura)
{
    Piscina* piscina;
    Xar* lexemata;
    chorda emissa;
    b32 fidelis;

    piscina = piscina_generare_dynamicum("incolumitas_specimen", 4194304);
    si (piscina == NIHIL)
    {
        redde FALSUM;
    }

    lexemata = silva_lexare(piscina, (constans character*)buffo, mensura,
        ZEPHYRUM);
    emissa = silva_lexemata_emittere(piscina, lexemata);
    fidelis = (emissa.mensura == mensura
        && (mensura == ZEPHYRUM
            || memcmp(emissa.datum, buffo, (memoriae_index)mensura)
                == ZEPHYRUM)) ? VERUM : FALSUM;

    /* praeprocessor integer: numquam fragor, semper terminat */
    {
        SilvaExpansio* exp;
        Xar* reliqua;
        Xar* expansa;

        exp = silva_expansio_creare(piscina);
        silva_fons_addere(exp, "fuzz.c", FALSUM);
        reliqua = silva_expansio_directivas_processare(exp, lexemata, NIHIL);
        expansa = silva_expansio_expandere_reliqua(exp, reliqua, NIHIL);
        (vacuum)expansa;
    }

    piscina_destruere(piscina);
    redde fidelis;
}

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;
    i8       buffo[MENSURA_MAXIMA];
    i32      i;
    i32      fideles_crudi;
    i32      fideles_c;

    piscina = piscina_generare_dynamicum("probatio_silva_incolumitas",
        262144);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- Probans incolumitatem (fuzzing deterministicum) ---\n");

    /* Circuitus A: octeti crudi */
    fideles_crudi = ZEPHYRUM;
    per (i = ZEPHYRUM; i < ITERATIONES; i++)
    {
        i32 mensura = _buffo_crudus(buffo, MENSURA_MAXIMA);

        si (_specimen(buffo, mensura))
        {
            fideles_crudi++;
        }
    }
    imprimere("  crudi: %d/%d fideles\n", (int)fideles_crudi,
        (int)ITERATIONES);
    CREDO_AEQUALIS_I32 (fideles_crudi, ITERATIONES);

    /* Circuitus B: characteres C-formes */
    fideles_c = ZEPHYRUM;
    per (i = ZEPHYRUM; i < ITERATIONES; i++)
    {
        i32 mensura = _buffo_c_formis(buffo, MENSURA_MAXIMA);

        si (_specimen(buffo, mensura))
        {
            fideles_c++;
        }
    }
    imprimere("  C-formes: %d/%d fideles\n", (int)fideles_c,
        (int)ITERATIONES);
    CREDO_AEQUALIS_I32 (fideles_c, ITERATIONES);

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();

    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
