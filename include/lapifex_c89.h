/* lapifex_c89.h - Grammatica C89 pro Lapifex GLR
 *
 * Parsationes expressionum C89 usans motorem Lapifex GLR.
 * Producit Arbor2Nodus* AST nodos.
 *
 * Usus:
 *   Arbor2Nodus* nodus = lapifex_c89_expressionem_parsare(
 *       piscina, intern, "2 + 3 * x", 9);
 */

#ifndef LAPIFEX_C89_H
#define LAPIFEX_C89_H

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "arbor2_lexema.h"
#include "arbor2_token.h"
#include "arbor2_glr.h"
#include "arbor2_lapifex_adaptator.h"
#include "lapifex_generare.h"
#include "lapifex_glr.h"
#include "arbor2_expandere.h"

/* ================================================
 * Contextus pro reductione
 *
 * Traditur ut vacuum* contextus ad callback reductionis.
 * ================================================ */

nomen structura {
    Piscina*              piscina;
    InternamentumChorda*  intern;
    chorda*               via_file;
} LapifexC89Contextus;

/* ================================================
 * API Publica
 * ================================================ */

/* Parsare expressionem ex fonte
 * piscina  - Arena allocatoris
 * intern   - Internamentum chordarum
 * fons     - Fons C89 (expressio)
 * mensura  - Longitudo fontis in bytes
 * Redde: Arbor2Nodus* radix AST, vel NIHIL si error
 */
Arbor2Nodus*
lapifex_c89_expressionem_parsare(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    constans character*   fons,
    i32                   mensura);

/* Parsare declarationem ex fonte
 * piscina  - Arena allocatoris
 * intern   - Internamentum chordarum
 * fons     - Fons C89 (declaratio)
 * mensura  - Longitudo fontis in bytes
 * Redde: Arbor2Nodus* radix AST, vel NIHIL si error
 */
Arbor2Nodus*
lapifex_c89_declarationem_parsare(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    constans character*   fons,
    i32                   mensura);

/* Parsare sententiam ex fonte
 * piscina  - Arena allocatoris
 * intern   - Internamentum chordarum
 * fons     - Fons C89 (sententia)
 * mensura  - Longitudo fontis in bytes
 * Redde: Arbor2Nodus* radix AST, vel NIHIL si error
 */
Arbor2Nodus*
lapifex_c89_sententiam_parsare(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    constans character*   fons,
    i32                   mensura);

/* Parsare translationem (translation unit) ex fonte
 * piscina  - Arena allocatoris
 * intern   - Internamentum chordarum
 * fons     - Fons C89 (plures declarationes/definitiones)
 * mensura  - Longitudo fontis in bytes
 * Redde: Arbor2Nodus* radix AST (TRANSLATION_UNIT), vel NIHIL si error
 */
Arbor2Nodus*
lapifex_c89_translationem_parsare(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    constans character*   fons,
    i32                   mensura);

/* Parsare fontem per expansionem (preprocessor → parser pipeline)
 * piscina  - Arena allocatoris
 * intern   - Internamentum chordarum
 * exp      - Contextus expansionis (iam configuratus cum include viis, macris, etc.)
 * fons     - Fons C89 (cum directivis preprocessoris)
 * mensura  - Longitudo fontis in bytes
 * via_file - Via file fontis (pro provenientia)
 * Redde: Arbor2Nodus* radix AST, vel NIHIL si error
 */
Arbor2Nodus*
lapifex_c89_fontem_parsare(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    Arbor2Expansion*      exp,
    constans character*   fons,
    i32                   mensura,
    constans character*   via_file);

/* Praescandere lexemata pro typedef nominibus
 * lexemata       - Xar of Arbor2Lexema* (mutatur in loco)
 * intern         - Internamentum chordarum
 * externa_nomina - Tabula externarum typedef nominum (vel NIHIL)
 * numerus_ext    - Numerus externarum nominum
 */
vacuum
lapifex_c89_typedef_praescandere(
    Xar*                     lexemata,
    InternamentumChorda*     intern,
    constans character* constans* externa_nomina,
    i32                      numerus_ext);

/* Obtinere grammaticam STML expressionum (pro usu externo / probatione) */
constans character*
lapifex_c89_expressio_grammatica(vacuum);

/* Callback reductionis (exposita pro probatione)
 * productio_index  - Index productionis
 * valori           - Tabula valorum ex acervo
 * numerus          - Numerus symbolorum in dextro
 * contextus        - LapifexC89Contextus*
 * Redde: s64 valor (Arbor2Nodus* codificatus)
 */
s64
lapifex_c89_expressio_reductio(
    s32           productio_index,
    constans s64* valori,
    s32           numerus,
    vacuum*       contextus);

#endif /* LAPIFEX_C89_H */
