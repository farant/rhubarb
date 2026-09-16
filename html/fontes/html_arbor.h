/* html_arbor.h - Aedificator HTML simplex (html-arbor-spec par. XI)
 *
 * TOTALIS: quaelibet series octetorum arborem generis
 * HTML_GENUS_DOCUMENTUM reddit; NIHIL solum memoria aut tabula
 * nostra deficiente. Recuperatio pars grammaticae est: tag clausurae
 * sine pari 'elementum-malum' fit, lexema quodlibet aliud nodum
 * contenti; nullus exitus 'vitium'.
 *
 * ITERATIVUS: acervus elementorum apertorum Xar est, numquam
 * recursio C - profunditas fontis acervum C NON tangit. Scriptor
 * octetorum et proiectio materiae recursivi manent; porta
 * totalitatis (H8) limitem eorum metitur (01M1FAD8).
 *
 * LOCI ABSENTES SENSUS SUNT (H4): elementum vacuum, clausura
 * implicita, apertum ad EOF, tag ad EOF scissum - omnia loci
 * lexematum NIHIL, numquam lexemata ficta. Signum se-claudens '/>'
 * elementum HTML NON claudit (HTML5: vexillum neglectum praeter
 * elementa vacua et contentum alienum); vacua tabula H4 tractat.
 *
 * FONS NON COPIATUR. Valores lexematum in eum spectant (sicut
 * html_lexema), ergo fons vivat quamdiu arbor vivit.
 *
 * TRIVIA INTRA TAG SOLA (html_adaptare.h): inter tags omne
 * lexema contentum est; ligator intra tag solum agit.
 *
 * SYNTHESIS (O7a, 2026-09-15): elementa quae spec HTML5 fingit -
 * involucra html/head/body (modi 'before html' ... 'after head') et
 * partes tabulae tbody/tr/colgroup - nodi generis elementum SINE
 * lexematibus, loco 'synthesis' annotati (HtmlSynthesis). Lex H4
 * manet: lexemata ficta nulla; emissor octetorum nodum fictum
 * transit, visio cocta eum imprimit.
 */

#ifndef HTML_ARBOR_H
#define HTML_ARBOR_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "materia_nodus.h"
#include "html_alienum.h"

/* Documentum parsare. Nodum generis HTML_GENUS_DOCUMENTUM reddit. */
MateriaNodus*
html_arbor_parsare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura);

/* Fragmentum parsare (html5lib '#document-fragment'): contextus =
 * titulus elementi contextus (litteris neglectis) et spatium eius
 * (svg/math/nullum). Contextus VERTEX SUB ACERVO est: spatia, select,
 * frameset et scopus tabulae ex eo; involucra html/head/body NON
 * finguntur - nisi contextu 'html', quo head et body sub radice
 * finguntur (spec 'before head'). Radix documentum manet. */
MateriaNodus*
html_arbor_parsare_fragmentum (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura,
                chorda  contextus,
           HtmlAlienum  spatium);

/* Titulus elementi ficti ex valore loci 'synthesis' (HtmlSynthesis);
 * NIHIL pro NULLA aut extra tabulam. */
constans character*
html_arbor_synthesis_titulus (
    s32 synthesis);

#endif /* HTML_ARBOR_H */
