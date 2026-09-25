/* silva_scribere.h - Emissio arborum ad octetos (Phase 5)
 *
 * Claudit catenam lexema -> arbor -> octeti. Duo ingressus, duo
 * contractus (INTENTIO Phase 5 + simulatio ⑥):
 *
 *   - silva_scribere_nodum / _valorem: AMBULATIO PURE STRUCTURALIS
 *     subarborum - loci ordine layout, numquam positione dependens
 *     (nulla ordinatio per byte_offset, nulla lectio vicinorum).
 *     Haec puritas est quae subarbores intactas post transformationes
 *     octetim exactas servat: subarbor mota octetos suos ubicumque
 *     emittit. NB directivae consumptae subarboribus NON pertinent
 *     (res plagulae sunt, non nodorum) - eas fontem infra reinserit.
 *
 *   - silva_scribere_fontem: reconstructio STRATI 0 unius plagulae ex
 *     parsura. Tres fontes octetorum (partitio exacta - invariatum
 *     triviorum eam gratis dat, sim ⑥ C3): lexemata arboris (strati 0
 *     per limitem expansionis), lineae directivae consumptae
 *     (parsura->directivae, per fontem filtratae), laminae ramorum
 *     non sumptorum (arbor regionum). Ordinatio per offset - licita
 *     HIC SOLUM (res strati fluxus, non arboris).
 *
 * LIMES EXPANSIONIS (sim ⑥ C4): lexema cuius origo non FONS est non
 * se emittit - invocatio strati 0 emittitur (radix catenae), semel
 * per instantiam (dedup per intervalla octetorum). Invocationes
 * functio-similes strati 0 laminas extentorum in expansione memoratas
 * emittunt (C5) - argumenta cum triviis suis redeunt. PASTA/CHORDA/
 * API in catena = fractura clara (deferral nominatum). Sine contextu
 * expansionis (expansio NIHIL) lexema expansum = fractura clara.
 *
 * Ambulatio grammaticae-ignara est (exemplar commissionis): forma
 * ambigui ex registro cocto PER NOMEN invenitur; AMBIGUUS solam
 * interpretationem CANONICAM emittit. Nodi ERROR casu speciali NON
 * egent (locus unicus lista-token ambulatione generica emittitur).
 * Genus ignotum aut forma corrupta = fractura clara (successus
 * FALSUM + causa + sedes), numquam praetermissio tacita.
 *
 * SUPER MATERIAM (silva-migratio T9, 2026-09-25): ambulatio et
 * emissio sunt materia_scribere; hic manent API silvae et quod C89
 * PROPRIUM est - unci (radix per catenam originis, extentum
 * invocationis per identitatem et continentiam, valor cum
 * scissuris) et collectio reinserendorum plagulae totius (lineae
 * directivae, rami non sumpti, invocationes vacuae, EOF). Una
 * differentia nota: registrum SINE genere 'ambiguus' iam non
 * recusatur (materia: lingua sine ambiguitate); registra silvae
 * id semper ferunt.
 */

#ifndef SILVA_SCRIBERE_H
#define SILVA_SCRIBERE_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_registrum.h"
#include "silva_expandere.h"
#include "silva_parsare.h"
#include "materia_scribere.h"


/* ==================================================
 * Fructus scripturae
 * ================================================== */

/* successus, textus (octeti; vacua in fractura), causa (NIHIL si
 * bene), sedes (nodus fracturae; NIHIL licet) - campi idem */
nomen MateriaScriptura SilvaScriptura;


/* ==================================================
 * API
 * ================================================== */

/* Valor quilibet (radix commissionis est LISTA plerumque).
 * expansio: contextus limitis expansionis (extenta) - NIHIL licet,
 * tunc lexema expansum quodlibet = fractura clara. */
SilvaScriptura
silva_scribere_valorem (
                          Piscina* piscina,
                       SilvaValor  valor,
    constans SilvaRegistrumCoctum* tabularium,
           constans SilvaExpansio* expansio);

/* Subarbor una - ingressus transformationum */
SilvaScriptura
silva_scribere_nodum (
                          Piscina* piscina,
              constans SilvaNodus* nodus,
    constans SilvaRegistrumCoctum* tabularium,
           constans SilvaExpansio* expansio);

/* Fons integer ex parsura: arbor + lineae directivae + rami non
 * sumpti + trivia caudae (EOF - pro plagula inclusa ex includenda
 * accersitur, sim ⑥ C6). fons_index: cuius plagulae octeti petuntur;
 * -1 = quaelibet (fluxus praelexati sine includendo). */
SilvaScriptura
silva_scribere_fontem (
                          Piscina* piscina,
            constans SilvaParsura* parsura,
    constans SilvaRegistrumCoctum* tabularium,
                              s32  fons_index);

#endif /* SILVA_SCRIBERE_H */
