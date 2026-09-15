/* html_adaptare.h - HtmlLexema -> MateriaToken, et ligatio triviorum
 *
 * DUAE RES, CONSULTO SEPARATAE (exemplar css_adaptare.h). Adaptatio
 * lexemata VERTIT unum pro uno; ligatio trivia in significantia
 * DISTRIBUIT. INVARIATUM: lexema materiae UNUM pro lexemate HTML UNO,
 * semper - 'dominus unus' AEDIFICATUR, non asseritur.
 *
 * TRIVIA INTRA TAG SOLA (spec par. XI.5). Lexator html_lexema SPATIA
 * et DELIM inter '<titulus' et '>' solum emittit; extra tags omnis
 * octetus TEXTUS est - contentum, nodus (H6). Ergo ligator inter
 * tags NUMQUAM consulitur; intra tag trivia pendentia significanti
 * proximo (titulus attributi, '=', valor, '>') ligantur. Cauda
 * documenti (lexema FINIS) trivia tagi ad EOF scissi accipit - via
 * UNA qua trivia ad finem perveniunt.
 *
 * REGULA DIVISIONIS eadem ac css (portata ex silva_lexema.c:950-1000):
 * divisor est primum trivium muneris SPATIUM quod terminatorem lineae
 * continet; illud et sequentia SEQUENTI ut 'ante', priora PRIORI ut
 * 'post'; sine linea nova omnia priori; sine priore omnia sequenti.
 * 'Sine priore' in HTML per constructionem non evenit (omne trivium
 * apertionem tagi sequitur) - ligator eum ramum tamen fert et
 * probatio eum directe exercet.
 */

#ifndef HTML_ADAPTARE_H
#define HTML_ADAPTARE_H

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "materia_token.h"
#include "materia_lexicon.h"


/* ==================================================
 * Adaptatio - UNUM PRO UNO
 * ================================================== */

/* Xar de HtmlLexema (per valorem) -> Xar de MateriaToken*.
 *
 * Nihil absorbetur, nihil omittitur, nihil fingitur: index i exitus
 * lexema i ingressus est. Valor in FONTEM spectat, sicut HtmlLexema
 * (fons vivat quamdiu lexemata vivunt). 'forma' caudam frontis
 * nominat; NIHIL redditur solum si memoria deficit. */
Xar*
html_adaptare (
                       Piscina* piscina,
                           Xar* lexemata_html,
    constans MateriaTokenForma* forma);


/* ==================================================
 * Ligator triviorum
 * ================================================== */

/* pendentia = Xar de MateriaToken* (trivia differita); prior =
 * ultimum significans ligatum (NIHIL ante primum). */
nomen structura {
                         Piscina* piscina;
    constans MateriaLexiconRatum* lexicon;
                             Xar* pendentia;
                    MateriaToken* prior;
} HtmlLigator;

vacuum
html_ligator_incipere (
                      HtmlLigator* ligator,
                          Piscina* piscina,
     constans MateriaLexiconRatum* lexicon);

/* Trivium differre. Nihil ligatur donec significans adveniat. */
b32
html_ligator_cumulare (
     HtmlLigator* ligator,
    MateriaToken* trivium);

/* Pendentia inter 'prior' et 'sequens' dividere, regula supra.
 * 'sequens' NIHIL esse non potest - html_lexare lexema FINIS semper
 * ultimum emittit. */
b32
html_ligator_solvere (
     HtmlLigator* ligator,
    MateriaToken* sequens);

#endif /* HTML_ADAPTARE_H */
