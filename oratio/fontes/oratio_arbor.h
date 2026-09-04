/* oratio_arbor.h - Arbor orationis ex fluxu lexematum (T3, spec par. III)
 *
 * documentum -> paragraphi -> sententiae -> elementa (vocabulum |
 * interpunctio | numerus), omnis octetus in loco uno:
 *
 *  - PARAGRAPHUS: cursus spatii cum lineis novis DUABUS aut pluribus eum
 *    finit. Lex possessoris: spatium post elementum ultimum usque ad
 *    lineam novam primam INCLUSIVE = cauda elementi; residuum usque ad
 *    lineam novam ULTIMAM inclusive = cauda paragraphi; spatia post eam
 *    (indentatio) = praefixa paragraphi sequentis. Intra paragraphum
 *    spatium totum caudae elementi praecedentis. Ante paragraphum
 *    primum = praefixa documenti; post ultimum = cauda eius. Octeti
 *    imperii (SIGNUM) spatii classis sunt.
 *  - VOCABULUM: LITTERAE, deinde iuncturae repetitae: HYPHEN | APOSTROPHUS
 *    | PUNCTUM (sine spatio) + LITTERAE, aut DIGITI (B2, md5). Punctum
 *    finale numquam pars est.
 *  - NUMERUS: DIGITI, deinde (PUNCTUM | ',') DIGITI repetita, deinde
 *    LITTERAE suffixum (3rd, 1980s).
 *  - INTERPUNCTIO: lexema PUNCTUM aut INTERPUNCTIO unum.
 *  - SENTENTIA (v1): finis candidatus = elementum interpunctionis
 *    PUNCTUM, aut '!' '?' (cursus), aut cursus punctorum ('...');
 *    interpunctio claudens sequens (citationes, parentheses) eidem
 *    sententiae adhaeret. Confirmatur si elementum proximum (post
 *    interpunctionem aperientem) littera CAPITALI ASCII incipit, aut
 *    octeto >= 0x80 (capitalis ignota - fissio praefertur), aut numerus
 *    est; paragraphus finiens semper finit. NISI vocabulum ante punctum
 *    abbreviatio est (tabula: Mr Dr St etc e.g i.e cf ... et littera
 *    capitalis UNA = initialis), aut punctum intra vocabulum iacet.
 *    Numquam finis fingitur qui iustificari nequit.
 */

#ifndef ORATIO_ARBOR_H
#define ORATIO_ARBOR_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "materia_nodus.h"

/* NIHIL = memoria deficit (nunquam ob textum). */
MateriaNodus*
oratio_arbor_parsare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura);

/* An vocabulum (sine puncto finali) abbreviatio tabulae sit - litterae
 * ASCII, casus indifferens; littera capitalis una = initialis. */
b32
oratio_abbreviatio_est (
    constans character* datum,
                   i32  mensura);

#endif /* ORATIO_ARBOR_H */
