/* html_registrum_coctum.c
 *
 * Registrum generum COCTUM grammaticae 'html' - GENERATUM, NE MANU
 * EDITES. Fons: html/grammatica/html.registrum.stml
 * (materia/coquere.sh). Series LOCORUM plana; quodque genus fenestram
 * suam per loci_offset + loci_numerus nominat. Genera X, loci XVIII.
 */

#include "html_registrum_coctum.h"
#include "materia_nodus.h"

hic_manens constans MateriaTabLocus LOCI_COCTI[] = {
    /* documentum (0..1) */
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "cauda", (s32)MATERIA_LOCUS_TOKEN },

    /* doctype (2..2) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* elementum (3..8) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "attributa", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_finis", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_clausura_finis", (s32)MATERIA_LOCUS_TOKEN },

    /* attributum (9..11) */
    { "tok_nomen", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_aequale", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_valor", (s32)MATERIA_LOCUS_TOKEN },

    /* textus (12..12) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* referentia (13..13) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* textus-crudus (14..14) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* commentarium (15..15) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* cdata (16..16) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* elementum-malum (17..17) */
    { "tokens", (s32)MATERIA_LOCUS_LISTA_TOKEN },
};

hic_manens constans MateriaTabGenus GENERA_COCTA[] = {
    /* titulus, offset, numerus */
    { "documentum", (i32)0, (i32)2 },
    { "doctype", (i32)2, (i32)1 },
    { "elementum", (i32)3, (i32)6 },
    { "attributum", (i32)9, (i32)3 },
    { "textus", (i32)12, (i32)1 },
    { "referentia", (i32)13, (i32)1 },
    { "textus-crudus", (i32)14, (i32)1 },
    { "commentarium", (i32)15, (i32)1 },
    { "cdata", (i32)16, (i32)1 },
    { "elementum-malum", (i32)17, (i32)1 },
};

constans MateriaRegistrumCoctum HTML_REGISTRUM = {
    GENERA_COCTA,
    (i32)(magnitudo(GENERA_COCTA) / magnitudo(GENERA_COCTA[0])),
    LOCI_COCTI,
    (i32)(magnitudo(LOCI_COCTI) / magnitudo(LOCI_COCTI[0]))
};
