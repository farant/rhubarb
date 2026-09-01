/* css_arbor.h - Parsator CSS, descensu recursivo (CSS Syntax L3)
 *
 * TOTALIS: quaelibet series octetorum arborem reddit. Nihil umquam
 * cadit - algorithmus L3 ita DEFINITUR, et recuperatio pars
 * grammaticae est, non tractatio erroris. Ergo nullus 'vitium'
 * exitus: NIHIL solum si memoria aut tabula nostra deficit.
 * CLAMOR PORTATUS (2026-09-01): probatio_css_totalitas eum
 * mensurat - octeti fortuiti, corpus mutatum/truncatum,
 * nidificatio. Ante portam clamor mendax erat ('a{b:(}' NIHIL
 * dabat, 01M1FACW). EXCEPTIO NOTA: nidificatio ultra ~XX milia
 * acervum C exhaurit (01M1FAD8) - pinna in porta, remedium debitum.
 *
 * FONS NON COPIATUR. Valores lexematum in eum spectant (sicut
 * css_lexema), ergo fons vivat quamdiu arbor vivit. Piscina
 * arborem tenet; fontem NON tenet.
 *
 * REGIMINA SPATIORUM (spec par. VI, D7). Parsator regimen tenet,
 * non adaptator - vide css_adaptare.h pro ratione:
 *   STRUCTURALE  inter regulas: spatia trivia sunt
 *   CONTENTUM    intra praeludia et valores: spatia nodi
 *                'lexema-servatum' fiunt
 * Commentaria trivia sunt UTROQUE regimine.
 */

#ifndef CSS_ARBOR_H
#define CSS_ARBOR_H

#include "latina.h"
#include "piscina.h"
#include "materia_nodus.h"

/* Plagulam parsare. Nodum generis CSS_GENUS_PLAGULA reddit. */
MateriaNodus*
css_arbor_parsare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura);

#endif /* CSS_ARBOR_H */
