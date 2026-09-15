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
 */

#ifndef HTML_ARBOR_H
#define HTML_ARBOR_H

#include "latina.h"
#include "piscina.h"
#include "materia_nodus.h"

/* Documentum parsare. Nodum generis HTML_GENUS_DOCUMENTUM reddit. */
MateriaNodus*
html_arbor_parsare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura);

#endif /* HTML_ARBOR_H */
