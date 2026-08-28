/* materia_registrum.h - Registrum generum coctum (vocabularium nodorum)
 *
 * Portatum ex silva_registrum.h, quod M4 ex silva_tabulae.h scidit.
 * Illa scissio limitem strati VISIBILEM fecit in grapho inclusionum;
 * hoc caput eundem contractum in materia servat.
 *
 * NIHIL INCLUDIT PRAETER latina.h. Paupertas contractus est: qui
 * vocabularium nodorum legere vult motorem parsandi trahere NON debet.
 */

#ifndef MATERIA_REGISTRUM_H
#define MATERIA_REGISTRUM_H

#include "latina.h"

nomen structura {
    constans character* titulus;     /* nomen loci */
    s32                 species;     /* MateriaLocusSpecies */
} MateriaTabLocus;

nomen structura {
    constans character* titulus;     /* nomen generis */
    i32                 loci_offset; /* in seriem planam locorum */
    i32                 loci_numerus;
} MateriaTabGenus;

nomen structura {
    constans MateriaTabGenus* genera;
    i32                       numerus_generum;
    constans MateriaTabLocus* loci;
    i32                       numerus_locorum;
} MateriaRegistrumCoctum;

#endif /* MATERIA_REGISTRUM_H */
