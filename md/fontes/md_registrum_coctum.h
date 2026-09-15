/* md_registrum_coctum.h
 *
 * Registrum generum COCTUM grammaticae 'md' - GENERATUM, NE MANU
 * EDITES. Fons: md/grammatica/md.registrum.stml (materia/coquere.sh).
 * Genera XXVIII, loci XCIV.
 */

#ifndef MD_REGISTRUM_COCTUM_H
#define MD_REGISTRUM_COCTUM_H

#include "latina.h"
#include "materia_registrum.h"

nomen enumeratio {
    /* bloci (documentum .. definitio-nexus) */
    MD_GENUS_DOCUMENTUM = 0,
    MD_GENUS_PRAEFATIO,
    MD_GENUS_LINEA,
    MD_GENUS_LINEA_VACUA,
    MD_GENUS_PARAGRAPHUS,
    MD_GENUS_CAPITULUM,
    MD_GENUS_SAEPTUM,
    MD_GENUS_LIMES,
    MD_GENUS_CITATIO,
    MD_GENUS_LISTA,
    MD_GENUS_ELEMENTUM,
    MD_GENUS_DIVISIO,
    MD_GENUS_TABULA,
    MD_GENUS_ORDO,
    MD_GENUS_CELLA,
    /* blocus html opacus */
    MD_GENUS_HTML,
    MD_GENUS_DEFINITIO_NEXUS,
    /* inlinea (inlinea .. html-inlineum): arbor inlinea octetos suos
     * possidet */
    MD_GENUS_INLINEA,
    MD_GENUS_TEXTUS,
    MD_GENUS_EMPHASIS,
    MD_GENUS_FORTIS,
    MD_GENUS_DELETIO,
    /* code span */
    MD_GENUS_VERBATIM,
    MD_GENUS_NEXUS,
    MD_GENUS_IMAGO,
    MD_GENUS_FRACTURA_MOLLIS,
    MD_GENUS_FRACTURA_DURA,
    MD_GENUS_HTML_INLINEUM,

    MD_GENUS_NUMERUS_GENERUM
} MdGenus;

externus constans MateriaRegistrumCoctum MD_REGISTRUM;

#endif /* MD_REGISTRUM_COCTUM_H */
