/* md_registrum.c - Vocabularium nodorum markdown
 *
 * Vide md_registrum.h. Series LOCORUM plana; quodque genus fenestram
 * suam per loci_offset + loci_numerus nominat. Probatio congruentiam
 * enumerationis cum hac tabula per TITULOS asserit et offsets
 * CONTIGUOS - permutatio et lapsus capiuntur, non absorbentur.
 */

#include "md_registrum.h"
#include "materia_nodus.h"

hic_manens constans MateriaTabLocus LOCI_MD[] = {
    /* documentum (0..2) */
    { "praefatio",   (s32)MATERIA_LOCUS_NODUS },
    { "bloci",       (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "finis",       (s32)MATERIA_LOCUS_TOKEN },

    /* praefatio (3..5) */
    { "apertum",     (s32)MATERIA_LOCUS_NODUS },
    { "lineae",      (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "clausum",     (s32)MATERIA_LOCUS_NODUS },

    /* linea (6..8) */
    { "praefixa",    (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "contentum",   (s32)MATERIA_LOCUS_LISTA_TOKEN },   /* scissa ad "</md-" */
    { "finis",       (s32)MATERIA_LOCUS_TOKEN },

    /* linea-vacua (9..10) */
    { "praefixa",    (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "finis",       (s32)MATERIA_LOCUS_TOKEN },

    /* paragraphus (11..14) */
    { "praefixa",    (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "nudus",       (s32)MATERIA_LOCUS_INDEX },
    { "inlinea",     (s32)MATERIA_LOCUS_NODUS },
    { "finis",       (s32)MATERIA_LOCUS_TOKEN },

    /* capitulum (15..21) */
    { "praefixa",    (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "gradus",      (s32)MATERIA_LOCUS_INDEX },
    { "marca",       (s32)MATERIA_LOCUS_TOKEN },
    { "inlinea",     (s32)MATERIA_LOCUS_NODUS },
    { "clausum",     (s32)MATERIA_LOCUS_TOKEN },
    { "finis",       (s32)MATERIA_LOCUS_TOKEN },
    { "subductio",   (s32)MATERIA_LOCUS_NODUS },

    /* saeptum (22..26) */
    { "forma",       (s32)MATERIA_LOCUS_INDEX },
    { "apertum",     (s32)MATERIA_LOCUS_NODUS },
    { "lingua",      (s32)MATERIA_LOCUS_TOKEN },
    { "lineae",      (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "clausum",     (s32)MATERIA_LOCUS_NODUS },

    /* limes (27..30) */
    { "praefixa",    (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "signum",      (s32)MATERIA_LOCUS_TOKEN },
    { "info",        (s32)MATERIA_LOCUS_TOKEN },
    { "finis",       (s32)MATERIA_LOCUS_TOKEN },

    /* citatio (31) */
    { "bloci",       (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* lista (32..35) */
    { "genus",       (s32)MATERIA_LOCUS_INDEX },
    { "initium",     (s32)MATERIA_LOCUS_INDEX },
    { "laxa",        (s32)MATERIA_LOCUS_INDEX },
    { "elementa",    (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* elementum (36..37) */
    { "officium",    (s32)MATERIA_LOCUS_INDEX },
    { "bloci",       (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* divisio (38) */
    { "linea",       (s32)MATERIA_LOCUS_NODUS },

    /* tabula (39..41) */
    { "caput",       (s32)MATERIA_LOCUS_NODUS },
    { "separator",   (s32)MATERIA_LOCUS_NODUS },
    { "ordines",     (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* ordo (42..45) */
    { "praefixa",    (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "cellae",      (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "clausum",     (s32)MATERIA_LOCUS_TOKEN },
    { "finis",       (s32)MATERIA_LOCUS_TOKEN },

    /* cella (46..48) */
    { "ordinatio",   (s32)MATERIA_LOCUS_INDEX },
    { "apertum",     (s32)MATERIA_LOCUS_TOKEN },
    { "inlinea",     (s32)MATERIA_LOCUS_NODUS },

    /* html (49) */
    { "lineae",      (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* definitio-nexus (50..53) */
    { "lineae",      (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "titulus",     (s32)MATERIA_LOCUS_TOKEN },
    { "url",         (s32)MATERIA_LOCUS_TOKEN },
    { "descriptio",  (s32)MATERIA_LOCUS_TOKEN },

    /* inlinea (54) */
    { "liberi",      (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* textus (55..56) */
    { "crudum",      (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "valor",       (s32)MATERIA_LOCUS_TOKEN },

    /* emphasis (57..59) */
    { "apertum",     (s32)MATERIA_LOCUS_TOKEN },
    { "liberi",      (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "clausum",     (s32)MATERIA_LOCUS_TOKEN },

    /* fortis (60..62) */
    { "apertum",     (s32)MATERIA_LOCUS_TOKEN },
    { "liberi",      (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "clausum",     (s32)MATERIA_LOCUS_TOKEN },

    /* deletio (63..65) */
    { "apertum",     (s32)MATERIA_LOCUS_TOKEN },
    { "liberi",      (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "clausum",     (s32)MATERIA_LOCUS_TOKEN },

    /* verbatim (66..69) */
    { "apertum",     (s32)MATERIA_LOCUS_TOKEN },
    { "crudum",      (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "valor",       (s32)MATERIA_LOCUS_TOKEN },
    { "clausum",     (s32)MATERIA_LOCUS_TOKEN },

    /* nexus (70..75) */
    { "forma",       (s32)MATERIA_LOCUS_INDEX },
    { "apertum",     (s32)MATERIA_LOCUS_TOKEN },
    { "liberi",      (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "cauda",       (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "url",         (s32)MATERIA_LOCUS_TOKEN },
    { "descriptio",  (s32)MATERIA_LOCUS_TOKEN },

    /* imago (76..81) */
    { "forma",       (s32)MATERIA_LOCUS_INDEX },
    { "apertum",     (s32)MATERIA_LOCUS_TOKEN },
    { "liberi",      (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "cauda",       (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "url",         (s32)MATERIA_LOCUS_TOKEN },
    { "descriptio",  (s32)MATERIA_LOCUS_TOKEN },

    /* fractura-mollis (82..83) */
    { "finis",       (s32)MATERIA_LOCUS_TOKEN },
    { "praefixa",    (s32)MATERIA_LOCUS_LISTA_TOKEN },

    /* fractura-dura (84..86) */
    { "signum",      (s32)MATERIA_LOCUS_TOKEN },
    { "finis",       (s32)MATERIA_LOCUS_TOKEN },
    { "praefixa",    (s32)MATERIA_LOCUS_LISTA_TOKEN },

    /* html-inlineum (87) */
    { "crudum",      (s32)MATERIA_LOCUS_LISTA_TOKEN }
};

hic_manens constans MateriaTabGenus GENERA_MD_NODI[] = {
    /* titulus              offset   numerus */
    { "documentum",        (i32)0,  (i32)3 },
    { "praefatio",         (i32)3,  (i32)3 },
    { "linea",             (i32)6,  (i32)3 },
    { "linea-vacua",       (i32)9,  (i32)2 },
    { "paragraphus",       (i32)11, (i32)4 },
    { "capitulum",         (i32)15, (i32)7 },
    { "saeptum",           (i32)22, (i32)5 },
    { "limes",             (i32)27, (i32)4 },
    { "citatio",           (i32)31, (i32)1 },
    { "lista",             (i32)32, (i32)4 },
    { "elementum",         (i32)36, (i32)2 },
    { "divisio",           (i32)38, (i32)1 },
    { "tabula",            (i32)39, (i32)3 },
    { "ordo",              (i32)42, (i32)4 },
    { "cella",             (i32)46, (i32)3 },
    { "html",              (i32)49, (i32)1 },
    { "definitio-nexus",   (i32)50, (i32)4 },
    { "inlinea",           (i32)54, (i32)1 },
    { "textus",            (i32)55, (i32)2 },
    { "emphasis",          (i32)57, (i32)3 },
    { "fortis",            (i32)60, (i32)3 },
    { "deletio",           (i32)63, (i32)3 },
    { "verbatim",          (i32)66, (i32)4 },
    { "nexus",             (i32)70, (i32)6 },
    { "imago",             (i32)76, (i32)6 },
    { "fractura-mollis",   (i32)82, (i32)2 },
    { "fractura-dura",     (i32)84, (i32)3 },
    { "html-inlineum",     (i32)87, (i32)1 }
};

constans MateriaRegistrumCoctum MD_REGISTRUM = {
    GENERA_MD_NODI,
    (i32)(magnitudo(GENERA_MD_NODI) / magnitudo(GENERA_MD_NODI[0])),
    LOCI_MD,
    (i32)(magnitudo(LOCI_MD) / magnitudo(LOCI_MD[0]))
};
