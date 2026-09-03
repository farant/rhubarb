/* md_arbor.h - Parsator markdown super materiam (md-arbor-spec par. III-IV)
 *
 * A3: bloci FOLIA - paragraphi, capitula ATX et setext, divisiones,
 * saepta (saepta et indentata), lineae vacuae, bloci html opaci,
 * praefatio YAML. Continentia (citationes, listae) A4, tabulae et
 * definitiones A5, arbor inlinea A7 - sed FORMA una ab hinc:
 * paragraphus = praefixa + nudus + inlinea (+ finis), cuius liberi in A3
 * sunt textus crudus per lineam et fractura-mollis per terminatorem;
 * A7 intra eandem formam refinit.
 *
 * LEX OCTETORUM (spec par. III): omnis octetus in loco UNO; quaeque
 * linea a bloco folio uno possidetur; marcae continentium praefixa
 * lineae sunt; nulla trivia. Ergo materia_scribere_nodum fontem
 * octetim reddit QUAECUMQUE structura agnita sit - porta corporis
 * super omnes plagulas currit iam in A3, etsi listae adhuc paragraphi
 * sunt.
 *
 * Fons NON copiatur: lexemata in eum spectant (fons vivat quamdiu
 * arbor vivit - lectio natalis css B9).
 */

#ifndef MD_ARBOR_H
#define MD_ARBOR_H

#include "latina.h"
#include "piscina.h"
#include "materia_nodus.h"

/* Documentum parsare. Nodum generis MD_GENUS_DOCUMENTUM reddit; NIHIL
 * solum si memoria deficit (textus quilibet markdown validus est -
 * parsator numquam recusat). Patres per materia_arbor_patres_figere
 * iam fixi. */
MateriaNodus*
md_arbor_parsare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura);

#endif /* MD_ARBOR_H */
