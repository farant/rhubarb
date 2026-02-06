/* lapifex_lexere.h - Lexer pro Lapifex
 *
 * Tokenizat fontem textualem usans exempla grammaticae.
 * Terminalia cum attributo "exemplum" in STML definiuntur.
 *
 * Exemplum genera:
 *   "+"           - Literale congruens
 *   "$numerus"    - Scanner integralis [0-9]+
 *   "$identificator" - Scanner identificatoris [a-zA-Z_][a-zA-Z0-9_]*
 *
 * Usus:
 *   LapifexLexFructus f = lapifex_lexere(grammatica, fons, longitudo, piscina);
 *   si (f.successus) { ... f.signa, f.numerus ... }
 *
 *   LapifexParsaturaFructus p = lapifex_lexere_et_parsare(
 *       tabula, fons, longitudo, reductio, contextus);
 */

#ifndef LAPIFEX_LEXERE_H
#define LAPIFEX_LEXERE_H

#include "latina.h"
#include "lapifex_generare.h"
#include "lapifex_parsere.h"
#include "piscina.h"

/* ================================================
 * Fructus Lexeris
 * ================================================ */

nomen structura {
    LapifexSignum*  signa;          /* Tabula signorum plana */
    s32             numerus;        /* Numerus signorum */
    b32             successus;
    s32             error_positio;  /* Byte offset erroris */
    s32             error_linea;    /* Linea erroris (1-basis) */
    s32             error_columna;  /* Columna erroris (1-basis) */
} LapifexLexFructus;

/* ================================================
 * Functiones Principales
 * ================================================ */

/* Lexere: tokenizare fontem usans exempla grammaticae
 *
 * grammatica  - Grammatica cum exemplis in terminalibus
 * fons        - Fons textualis
 * longitudo   - Longitudo fontis in bytes
 * piscina     - Piscina pro allocationibus
 *
 * Redde: LapifexLexFructus
 */
LapifexLexFructus
lapifex_lexere(
    LapifexGrammatica*       grammatica,
    constans character*      fons,
    s32                      longitudo,
    Piscina*                 piscina);

/* Commoditas: lexere et parsare in uno passu
 *
 * tabula      - Tabula ACTION/GOTO
 * fons        - Fons textualis
 * longitudo   - Longitudo fontis in bytes
 * reductio    - Callback reductionis
 * contextus   - Contextus opacus
 *
 * Redde: LapifexParsaturaFructus
 */
LapifexParsaturaFructus
lapifex_lexere_et_parsare(
    LapifexTabula*           tabula,
    constans character*      fons,
    s32                      longitudo,
    LapifexReductioCallback  reductio,
    vacuum*                  contextus);

#endif /* LAPIFEX_LEXERE_H */
