/* oratio_registrum.c - Vocabularium nodorum orationis
 *
 * Vide oratio_registrum.h. Series LOCORUM plana; quodque genus
 * fenestram suam per loci_offset + loci_numerus nominat. Probatio
 * congruentiam enumerationis cum hac tabula per TITULOS asserit et
 * offsets CONTIGUOS.
 */

#include "oratio_registrum.h"
#include "materia_nodus.h"

hic_manens constans MateriaTabLocus LOCI_ORATIONIS[] = {
    /* documentum (0..2) */
    { "praefixa",    (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "paragraphi",  (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "finis",       (s32)MATERIA_LOCUS_TOKEN },

    /* paragraphus (3..6) */
    { "praefixa",    (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "sententiae",  (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "cauda",       (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "forma",       (s32)MATERIA_LOCUS_INDEX },      /* OratioForma (T6b) */

    /* sententia (7) */
    { "elementa",    (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* vocabulum (8..12) */
    { "partes",      (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "cauda",       (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "analyses",    (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "classes",     (s32)MATERIA_LOCUS_TOKEN },      /* DERIVATUM */
    { "linguae",     (s32)MATERIA_LOCUS_TOKEN },      /* DERIVATUM */

    /* interpunctio (13..14) */
    { "signum",      (s32)MATERIA_LOCUS_TOKEN },
    { "cauda",       (s32)MATERIA_LOCUS_LISTA_TOKEN },

    /* numerus (15..16) */
    { "crudum",      (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "cauda",       (s32)MATERIA_LOCUS_LISTA_TOKEN }
};

hic_manens constans MateriaTabGenus GENERA_ORATIONIS[] = {
    /* titulus              offset   numerus */
    { "documentum",        (i32)0,  (i32)3 },
    { "paragraphus",       (i32)3,  (i32)4 },
    { "sententia",         (i32)7,  (i32)1 },
    { "vocabulum",         (i32)8,  (i32)5 },
    { "interpunctio",      (i32)13, (i32)2 },
    { "numerus",           (i32)15, (i32)2 }
};

constans MateriaRegistrumCoctum ORATIO_REGISTRUM = {
    GENERA_ORATIONIS,
    (i32)(magnitudo(GENERA_ORATIONIS) / magnitudo(GENERA_ORATIONIS[0])),
    LOCI_ORATIONIS,
    (i32)(magnitudo(LOCI_ORATIONIS) / magnitudo(LOCI_ORATIONIS[0]))
};
