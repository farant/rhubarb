/* css_registrum.c - Vocabularium nodorum CSS
 *
 * Vide css_registrum.h. Series LOCORUM plana est; quodque genus
 * fenestram suam per loci_offset + loci_numerus nominat. Probatio
 * congruentiam enumerationis cum hac tabula asserit per TITULOS -
 * permutatio ergo capitur, non tacite absorbetur.
 */

#include "css_registrum.h"
#include "materia_nodus.h"

hic_manens constans MateriaTabLocus LOCI_CSS[] = {
    /* plagula (0..1) */
    { "regulae",       (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "cauda",         (s32)MATERIA_LOCUS_TOKEN },

    /* regula-qualificata (2..3) */
    { "praeludium",    (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "corpus",        (s32)MATERIA_LOCUS_NODUS },

    /* regula-apud (4..7) */
    { "tok_nomen",     (s32)MATERIA_LOCUS_TOKEN },
    { "praeludium",    (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "corpus",        (s32)MATERIA_LOCUS_NODUS },
    { "tok_terminator",(s32)MATERIA_LOCUS_TOKEN },

    /* declaratio (8..12) */
    { "tok_nomen",     (s32)MATERIA_LOCUS_TOKEN },
    { "tok_colon",     (s32)MATERIA_LOCUS_TOKEN },
    { "valor",         (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "praevalentia",  (s32)MATERIA_LOCUS_NODUS },
    { "tok_terminator",(s32)MATERIA_LOCUS_TOKEN },

    /* praevalentia (13..14) */
    { "tok_clamor",    (s32)MATERIA_LOCUS_TOKEN },
    { "tok_verbum",    (s32)MATERIA_LOCUS_TOKEN },

    /* lexema-servatum (15) */
    { "tok",           (s32)MATERIA_LOCUS_TOKEN },

    /* functio (16..18) */
    { "tok_nomen",     (s32)MATERIA_LOCUS_TOKEN },
    { "argumenta",     (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausum",   (s32)MATERIA_LOCUS_TOKEN },

    /* saeptum (19..21) */
    { "tok_apertum",   (s32)MATERIA_LOCUS_TOKEN },
    { "contentum",     (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausum",   (s32)MATERIA_LOCUS_TOKEN },

    /* regula-mala (22) */
    { "tokens",        (s32)MATERIA_LOCUS_LISTA_TOKEN },

    /* declaratio-mala (23) */
    { "tokens",        (s32)MATERIA_LOCUS_LISTA_TOKEN }
};

hic_manens constans MateriaTabGenus GENERA_CSS_NODI[] = {
    /* titulus                offset  numerus */
    { "plagula",              (i32)0,  (i32)2 },
    { "regula-qualificata",   (i32)2,  (i32)2 },
    { "regula-apud",          (i32)4,  (i32)4 },
    { "declaratio",           (i32)8,  (i32)5 },
    { "praevalentia",         (i32)13, (i32)2 },
    { "lexema-servatum",      (i32)15, (i32)1 },
    { "functio",              (i32)16, (i32)3 },
    { "saeptum",              (i32)19, (i32)3 },
    { "regula-mala",          (i32)22, (i32)1 },
    { "declaratio-mala",      (i32)23, (i32)1 }
};

constans MateriaRegistrumCoctum CSS_REGISTRUM = {
    GENERA_CSS_NODI,
    (i32)(magnitudo(GENERA_CSS_NODI) / magnitudo(GENERA_CSS_NODI[0])),
    LOCI_CSS,
    (i32)(magnitudo(LOCI_CSS) / magnitudo(LOCI_CSS[0]))
};
