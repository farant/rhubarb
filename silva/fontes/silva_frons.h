/* silva_frons.h - FRONS C89 super materiam: arborem silvae in typos
 * materiae vertere et unci C89 scriptoribus materiae praebere
 *
 * Phasis V (project-specs/silva-migratio-plan.md, T6b): e shim
 * (materia/instrumenta/shim_c89.c) promotus. Shim hanc conversionem
 * primum aedificavit ut emissorem materiae contra silvam iudicaret;
 * hic est PRODUCTIO, et shim nunc eam solam agitat.
 *
 * Quid facit:
 *   - lexema, valorem, nodum silvae in MateriaToken/Valor/Nodus
 *     vertit; DATUM FRONTIS C89 (origo, macro, def-site, extentum,
 *     scissurae, standard) in CAUDA lexematis materiae fert;
 *   - consilia scripturae (octeti) et arboris (STML) implet: registrum
 *     C89 conversum, uncus originis (radix, sedes, extentum), unci
 *     frontis (ornatus et lectio originis/scissurarum), lexicon C89.
 *
 * Contextus OPACUS: status conversionis (index lexematum, expansio)
 * per plagulam unam vivit, in piscina vocantis. Unci eum per 'datum'
 * accipiunt - nullus status globalis.
 *
 * Extra amalgama usque ad sigillum (T13; fontes_politica.sh).
 */

#ifndef SILVA_FRONS_H
#define SILVA_FRONS_H

#include "latina.h"
#include "piscina.h"
#include "silva_nodus.h"
#include "silva_expandere.h"
#include "materia_nodus.h"
#include "materia_scribere.h"
#include "materia_arbor.h"

nomen structura SilvaFrons SilvaFrons;

/* Contextus novus pro plagula una. expansio NIHIL licet (fons sine
 * praeprocessore): tum extenta invocationum non quaeruntur. */
SilvaFrons*
silva_frons_creare (
                   Piscina* piscina,
    constans SilvaExpansio* expansio);

/* Conversio. Lexema quodque SEMEL vertitur (index per identitatem):
 * trivia et catena originis lexemata eadem communiter ferunt. */
MateriaValor
silva_frons_valorem_convertere (
     SilvaFrons* frons,
     SilvaValor  valor);

MateriaNodus*
silva_frons_nodum_convertere (
     SilvaFrons* frons,
     SilvaNodus* nodus);

/* Consilium scripturae OCTETORUM: registrum C89, uncus originis,
 * emissio valoris cum laminis (scissurae) reinsertis. */
vacuum
silva_frons_scripturam_parare (
                   SilvaFrons* frons,
    MateriaScripturaConsilium* consilium);

/* Consilium ARBORIS (STML): ut supra + frons C89 (ornatus et lectio)
 * + forma caudae + lexicon C89. FALSUM si lexicon C89 recusatur -
 * causa per silva_frons_causa. */
b32
silva_frons_arborem_parare (
               SilvaFrons* frons,
    MateriaArborConsilium* consilium);

/* Quot lexemata conversa (trivia et origines inclusa). */
i32
silva_frons_lexemata_numerus (
    constans SilvaFrons* frons);

/* Causa fracturae ultimae (NIHIL si nulla) - additum ad API
 * probatum: shim nomen vitii lexici nuntiabat. */
constans character*
silva_frons_causa (
    constans SilvaFrons* frons);

/* Nodus radicis ex valore radicis commissionis (VALOR est: nodus aut
 * lista cuius primus nodus). NIHIL si nullus. Additum ad API
 * probatum: shim et scriptor arboris eo egent. */
SilvaNodus*
silva_frons_nodus_radicis (
    SilvaValor radix);

#endif /* SILVA_FRONS_H */
