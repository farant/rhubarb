/* lapifex_generare.h - Generator Grammaticae GLR
 *
 * Legit grammaticam ex STML, computat FIRST coniuncta,
 * fundamentum pro generatore tabularum GLR.
 *
 * Usus:
 *   LapifexGrammatica* g = lapifex_grammaticam_legere(piscina, intern, stml);
 *   lapifex_first_computare(g);
 *   lapifex_first_imprimere(g);
 */

#ifndef LAPIFEX_GENERARE_H
#define LAPIFEX_GENERARE_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"

/* ================================================
 * Symbolum - Terminale vel Non-Terminale
 * ================================================ */

nomen structura {
    chorda*  titulus;          /* Symbolum appellatio (internatum) */
    chorda*  genus;            /* Pro terminalibus: genus lexematis */
    s32      index;            /* Index in tabula symbolorum */
    b32      est_terminale;    /* VERUM si terminale */
    Xar*     first;            /* FIRST coniunctum: Xar de s32 (indices terminalium) */
    b32      habet_epsilon;    /* VERUM si epsilon in FIRST */
} LapifexSymbolum;

/* ================================================
 * Productio - Regula A -> alpha
 * ================================================ */

nomen structura {
    s32      sinistrum;        /* Index symboli non-terminalis (LHS) */
    Xar*     dextrum;          /* Xar de s32: indices symbolorum (RHS) */
    s32      index;            /* Index productionis */
} LapifexProductio;

/* ================================================
 * Grammatica - Collectio plena
 * ================================================ */

nomen structura {
    Xar*                     symbola;           /* Xar de LapifexSymbolum */
    Xar*                     productiones;      /* Xar de LapifexProductio */
    s32                      initium_index;     /* Index symboli initialis */
    i32                      numerus_terminalium;
    i32                      numerus_non_terminalium;
    Piscina*                 piscina;
    InternamentumChorda*     intern;
} LapifexGrammatica;

/* ================================================
 * Legere Grammaticam ex STML
 * ================================================ */

/* Legere grammaticam ex literis STML
 * Redde: LapifexGrammatica* vel NIHIL si error
 */
LapifexGrammatica*
lapifex_grammaticam_legere(
    Piscina*                 piscina,
    InternamentumChorda*     intern,
    constans character*      stml_fons);

/* ================================================
 * FIRST Coniuncta
 * ================================================ */

/* Computare FIRST coniuncta pro omnibus symbolis
 * Redde: VERUM si successus
 */
b32
lapifex_first_computare(
    LapifexGrammatica*  grammatica);

/* Obtinere FIRST coniunctum pro symbolo
 * Redde: Xar* de i32 (indices terminalium)
 */
Xar*
lapifex_first_obtinere(
    LapifexGrammatica*  grammatica,
    i32                 symbolum_index);

/* ================================================
 * Depuratio - Imprimere
 * ================================================ */

/* Imprimere grammaticam ad stdout */
vacuum
lapifex_grammaticam_imprimere(
    LapifexGrammatica*  grammatica);

/* Imprimere FIRST coniuncta ad stdout */
vacuum
lapifex_first_imprimere(
    LapifexGrammatica*  grammatica);

#endif /* LAPIFEX_GENERARE_H */
