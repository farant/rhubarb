/* md_inlinea.h - Arbor inlinea (md-arbor-spec par. III-IV, A7)
 *
 * Post parsuram blocorum quisque nodus 'inlinea' (paragraphus,
 * capitulum, cella) liberos suos crudos (textus per lineam, fractura
 * mollis per terminatorem) in arborem inlineam SUBSTITUIT: textus cum
 * valore decocto (solum cum differt), emphasis/fortis/deletio per
 * algorithmum cursuum delimitatorum (CommonMark par. 6.2, GFM '~'),
 * verbatim (code span, valor normalizatus), nexus/imago (inlinei,
 * referentes ex tabula definitionum, autonexus '<...>' et GFM nudi),
 * fracturae durae (spatia finalia II+ aut '\' ante terminatorem),
 * html inlineum.
 *
 * LEX OCTETORUM: arbor inlinea octetos suos POSSIDET. Contentum
 * linearum in copia contigua ('\n' inter lineas) parsatur cum tabula
 * segmentorum ad fontem; lexema numquam lineam transit. Fractura
 * intra locum LISTA_TOKEN (verbatim.crudum, nexus.cauda, html) =
 * lexemata cruda (terminator + praefixa lineae sequentis); intra
 * locum LISTA_NODUS (liberi) = nodus fractura-mollis/dura.
 *
 * LACUNAE NOMINATAE (oraculum html B3 per sectionem ostendet): classes
 * Unicode interpunctionis/albi in regulis flanking = ASCII; descriptio
 * nexus inlinei lineam non transit; titulus referens lineam non
 * transit; entia nominata ~CL.
 */

#ifndef MD_INLINEA_H
#define MD_INLINEA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "materia_nodus.h"
#include "md_lexema.h"

nomen structura {
               Piscina* piscina;
    constans character* fons;
             MdFabrica* fabrica;
    /* definitio per clavem normalizatam; NIHIL si nulla */
          MateriaNodus* (*definitio_quaerere)(vacuum* datum,
              chorda clavis);
                vacuum* datum;
} MdInlineaContextus;

/* Liberos nodi 'inlinea' in arborem inlineam substituere. Liberi
 * ingredientes: textus (crudum = lexema lineae unum) et fractura-mollis
 * alternantes. FALSUM = memoria deficit. */
b32
md_inlinea_construere (
    MdInlineaContextus* c,
          MateriaNodus* inlinea);

#endif /* MD_INLINEA_H */
