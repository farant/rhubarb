/* briar_registrum.c - Vide briar_registrum.h. Series LOCORUM plana;
 * genus quodque fenestram suam per loci_offset + loci_numerus nominat.
 * Probatio congruentiam enumerationum cum hac tabula per TITULOS
 * asserit. */

#include "briar_registrum.h"
#include "materia_nodus.h"

hic_manens constans MateriaTabLocus LOCI_BRIAR[] = {
    /* documentum (0..3) */
    { "interpres",  (s32)MATERIA_LOCUS_NODUS },
    { "partes",     (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "finis",      (s32)MATERIA_LOCUS_TOKEN },
    { "vitium",     (s32)MATERIA_LOCUS_INDEX },

    /* interpres (4) */
    { "tok",        (s32)MATERIA_LOCUS_TOKEN },

    /* prosa (5) */
    { "tok",        (s32)MATERIA_LOCUS_TOKEN },

    /* regio (6..10) */
    { "apertum",    (s32)MATERIA_LOCUS_TOKEN },
    { "titulus",    (s32)MATERIA_LOCUS_TOKEN },
    { "contentum",  (s32)MATERIA_LOCUS_TOKEN },
    { "clausum",    (s32)MATERIA_LOCUS_TOKEN },
    { "vitium",     (s32)MATERIA_LOCUS_INDEX },

    /* elementum (11..13) */
    { "tok",        (s32)MATERIA_LOCUS_TOKEN },
    { "titulus",    (s32)MATERIA_LOCUS_TOKEN },
    { "vitium",     (s32)MATERIA_LOCUS_INDEX }
};

hic_manens constans MateriaTabGenus GENERA_BRIAR_NODI[] = {
    /* titulus        offset   numerus */
    { "documentum",   (i32)0,  (i32)4 },
    { "interpres",    (i32)4,  (i32)1 },
    { "prosa",        (i32)5,  (i32)1 },
    { "regio",        (i32)6,  (i32)5 },
    { "elementum",    (i32)11, (i32)3 }
};

constans MateriaRegistrumCoctum BRIAR_REGISTRUM = {
    GENERA_BRIAR_NODI,
    (i32)(magnitudo(GENERA_BRIAR_NODI)
        / magnitudo(GENERA_BRIAR_NODI[0])),
    LOCI_BRIAR,
    (i32)(magnitudo(LOCI_BRIAR) / magnitudo(LOCI_BRIAR[0]))
};
