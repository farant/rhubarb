/* arbor2_lapifex_adaptator.h - Pons inter Arbor2 Lexer et Lapifex Parser
 *
 * Convertit flumen Arbor2Lexema* in LapifexSignum[] cum indice terminali
 * ex grammatica STML.
 *
 * USUS:
 *   Arbor2LapifexAdaptator* adapt = arbor2_lapifex_adaptator_creare(
 *       piscina, grammatica);
 *   Arbor2LapifexFructus fructus = arbor2_lapifex_convertere(
 *       adapt, lexemata);
 *   si (fructus.successus) {
 *       lapifex_glr_parsare(glr, fructus.signa, fructus.numerus, ...);
 *   }
 *
 * VALOR SEMANTICUS:
 *   LapifexSignum.valor continet (s64)(longus)(vacuum*)lexema
 *   Recuperare: (Arbor2Lexema*)(vacuum*)(longus)signum.valor
 */

#ifndef ARBOR2_LAPIFEX_ADAPTATOR_H
#define ARBOR2_LAPIFEX_ADAPTATOR_H

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "arbor2_lexema.h"
#include "lapifex_generare.h"
#include "lapifex_parsere.h"

/* Numerus maximus generum lexematum (enum Arbor2LexemaGenus) */
#define ARBOR2_LAPIFEX_MAX_GENERA XCV

/* ================================================
 * Fructus Conversionis
 * ================================================ */

nomen structura {
    LapifexSignum*  signa;              /* Tabula signorum convertorum */
    s32             numerus;            /* Numerus signorum */
    b32             successus;          /* VERUM si conversio successit */
    s32             numerus_ignoratorum; /* Numerus trivialium filtratorum */
} Arbor2LapifexFructus;

/* ================================================
 * Adaptator
 * ================================================ */

nomen structura {
    s32                 mappa[ARBOR2_LAPIFEX_MAX_GENERA]; /* genus -> index terminalis */
    Piscina*            piscina;
    LapifexGrammatica*  grammatica;
} Arbor2LapifexAdaptator;

/* ================================================
 * API Publica
 * ================================================ */

/* Creare adaptatorem cum mappa inter genera et terminalia
 * piscina     - Arena allocatoris
 * grammatica  - Grammatica STML (pro quaerendo terminalia)
 * Redde: Arbor2LapifexAdaptator* vel NIHIL si error
 */
Arbor2LapifexAdaptator*
arbor2_lapifex_adaptator_creare(
    Piscina*            piscina,
    LapifexGrammatica*  grammatica);

/* Convertere flumen lexematum in signa lapifex
 * adaptator  - Adaptator creatus
 * lexemata   - Xar de Arbor2Lexema* (ex arbor2_lexema_omnia)
 * Redde: Arbor2LapifexFructus
 */
Arbor2LapifexFructus
arbor2_lapifex_convertere(
    Arbor2LapifexAdaptator*  adaptator,
    Xar*                     lexemata);

/* Confer si genus est trivia (spatia, commentum, etc.)
 * Redde: VERUM si trivia
 */
b32
arbor2_lapifex_est_trivia(
    Arbor2LexemaGenus  genus);

/* Imprimere mappam pro depuratione */
vacuum
arbor2_lapifex_mappam_imprimere(
    Arbor2LapifexAdaptator*  adaptator);

#endif /* ARBOR2_LAPIFEX_ADAPTATOR_H */
