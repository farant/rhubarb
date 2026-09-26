/* silva_frons.h - FRONS C89 super materiam: unci quibus scriptor et
 * lector arboris materiae lexemata SILVAE ipsa tractant (origo,
 * standard, scissurae in cauda silvae), sine conversione.
 *
 * Phasis V: T6b ex shim promotus (conversio), T10b unci super caudam
 * silvae, T10c conversio et shim recesserunt, T13a in amalgamate;
 * T14 sedes efficax = radix strati 0 (diagnostica C89).
 */

#ifndef SILVA_FRONS_H
#define SILVA_FRONS_H

#include "latina.h"
#include "piscina.h"
#include "silva_nodus.h"
#include "silva_expandere.h"
#include "silva_registrum.h"
#include "materia_nodus.h"
#include "materia_scribere.h"
#include "materia_arbor.h"

nomen structura SilvaFrons SilvaFrons;

/* Contextus novus pro plagula una. expansio NIHIL licet (subarbor
 * lecta, fons sine praeprocessore): extenta tunc ex extentis LECTIS
 * solis quaeruntur. */
SilvaFrons*
silva_frons_creare (
                   Piscina* piscina,
    constans SilvaExpansio* expansio);


/* CONSILIUM ARBORIS SUPER LEXEMATA SILVAE (T10b): sine conversione -
 * unci caudam silvae ipsam legunt (origo, standard, scissurae) et
 * lector lexemata forma C89 creat (vera SilvaToken). Extenta lecta
 * in frons vivunt (lectio subarboris sine expansione). FALSUM si
 * lexicon C89 recusatur. */
b32
silva_frons_arborem_silvae_parare (
                       SilvaFrons* frons,
    constans SilvaRegistrumCoctum* tabularium,
               constans character* grammatica,
              InternamentumChorda* intern,
            MateriaArborConsilium* consilium);

/* Extenta LECTA in hanc seriem registrari (Xar de
 * SilvaExtentumInvocationis). Lector parsurae (T10c) expansionem
 * NOVAM reficit et extenta in expansio->extenta ponere debet, ut
 * silva_scribere_fontem ea super parsuram lectam inveniat. */
vacuum
silva_frons_extenta_lecta_ponere (
    SilvaFrons* frons,
           Xar* extenta);

/* Longitudines post lectionem: lexema quodque (trivia, catenae
 * originis, extenta lecta) longitudinem = valor fert, sicut lector
 * silvae vetus. Lector parsurae (T10c) eam super laminas vocat. */
vacuum
silva_frons_longitudines_figere (
    SilvaFrons* frons,
    SilvaValor  valor);


/* Uncus originis C89 frontis (post silva_frons_arborem_silvae_parare):
 * sedes efficax (radix strati 0), radix emissionis, extentum. Pro
 * consumptoribus materiae praeter arborem - materia_diagnostica_
 * derivare (T14). Vita = frontis. */
constans MateriaOrigoUncus*
silva_frons_uncus (
    constans SilvaFrons* frons);

/* Causa fracturae ultimae (NIHIL si nulla) - additum ad API
 * probatum: shim nomen vitii lexici nuntiabat. */
constans character*
silva_frons_causa (
    constans SilvaFrons* frons);


#endif /* SILVA_FRONS_H */
