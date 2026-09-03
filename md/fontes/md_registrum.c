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
    { "contentum",   (s32)MATERIA_LOCUS_TOKEN },
    { "finis",       (s32)MATERIA_LOCUS_TOKEN },

    /* linea-vacua (9..10) */
    { "praefixa",    (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "finis",       (s32)MATERIA_LOCUS_TOKEN },

    /* paragraphus (11..13) */
    { "praefixa",    (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "nudus",       (s32)MATERIA_LOCUS_INDEX },
    { "inlinea",     (s32)MATERIA_LOCUS_NODUS },

    /* capitulum (14..20) */
    { "praefixa",    (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "gradus",      (s32)MATERIA_LOCUS_INDEX },
    { "marca",       (s32)MATERIA_LOCUS_TOKEN },
    { "inlinea",     (s32)MATERIA_LOCUS_NODUS },
    { "clausum",     (s32)MATERIA_LOCUS_TOKEN },
    { "finis",       (s32)MATERIA_LOCUS_TOKEN },
    { "subductio",   (s32)MATERIA_LOCUS_NODUS },

    /* saeptum (21..25) */
    { "forma",       (s32)MATERIA_LOCUS_INDEX },
    { "apertum",     (s32)MATERIA_LOCUS_NODUS },
    { "lingua",      (s32)MATERIA_LOCUS_TOKEN },
    { "lineae",      (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "clausum",     (s32)MATERIA_LOCUS_NODUS },

    /* limes (26..29) */
    { "praefixa",    (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "signum",      (s32)MATERIA_LOCUS_TOKEN },
    { "info",        (s32)MATERIA_LOCUS_TOKEN },
    { "finis",       (s32)MATERIA_LOCUS_TOKEN },

    /* citatio (30) */
    { "bloci",       (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* lista (31..34) */
    { "genus",       (s32)MATERIA_LOCUS_INDEX },
    { "initium",     (s32)MATERIA_LOCUS_INDEX },
    { "laxa",        (s32)MATERIA_LOCUS_INDEX },
    { "elementa",    (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* elementum (35..36) */
    { "officium",    (s32)MATERIA_LOCUS_INDEX },
    { "bloci",       (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* divisio (37) */
    { "linea",       (s32)MATERIA_LOCUS_NODUS },

    /* tabula (38..40) */
    { "caput",       (s32)MATERIA_LOCUS_NODUS },
    { "separator",   (s32)MATERIA_LOCUS_NODUS },
    { "ordines",     (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* ordo (41..44) */
    { "praefixa",    (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "cellae",      (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "clausum",     (s32)MATERIA_LOCUS_TOKEN },
    { "finis",       (s32)MATERIA_LOCUS_TOKEN },

    /* cella (45..47) */
    { "ordinatio",   (s32)MATERIA_LOCUS_INDEX },
    { "apertum",     (s32)MATERIA_LOCUS_TOKEN },
    { "inlinea",     (s32)MATERIA_LOCUS_NODUS },

    /* html (48) */
    { "lineae",      (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* definitio-nexus (49..52) */
    { "lineae",      (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "titulus",     (s32)MATERIA_LOCUS_TOKEN },
    { "url",         (s32)MATERIA_LOCUS_TOKEN },
    { "descriptio",  (s32)MATERIA_LOCUS_TOKEN },

    /* inlinea (53) */
    { "liberi",      (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* textus (54..55) */
    { "crudum",      (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "valor",       (s32)MATERIA_LOCUS_TOKEN },

    /* emphasis (56..58) */
    { "apertum",     (s32)MATERIA_LOCUS_TOKEN },
    { "liberi",      (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "clausum",     (s32)MATERIA_LOCUS_TOKEN },

    /* fortis (59..61) */
    { "apertum",     (s32)MATERIA_LOCUS_TOKEN },
    { "liberi",      (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "clausum",     (s32)MATERIA_LOCUS_TOKEN },

    /* deletio (62..64) */
    { "apertum",     (s32)MATERIA_LOCUS_TOKEN },
    { "liberi",      (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "clausum",     (s32)MATERIA_LOCUS_TOKEN },

    /* verbatim (65..68) */
    { "apertum",     (s32)MATERIA_LOCUS_TOKEN },
    { "crudum",      (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "valor",       (s32)MATERIA_LOCUS_TOKEN },
    { "clausum",     (s32)MATERIA_LOCUS_TOKEN },

    /* nexus (69..74) */
    { "forma",       (s32)MATERIA_LOCUS_INDEX },
    { "apertum",     (s32)MATERIA_LOCUS_TOKEN },
    { "liberi",      (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "cauda",       (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "url",         (s32)MATERIA_LOCUS_TOKEN },
    { "descriptio",  (s32)MATERIA_LOCUS_TOKEN },

    /* imago (75..80) */
    { "forma",       (s32)MATERIA_LOCUS_INDEX },
    { "apertum",     (s32)MATERIA_LOCUS_TOKEN },
    { "liberi",      (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "cauda",       (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "url",         (s32)MATERIA_LOCUS_TOKEN },
    { "descriptio",  (s32)MATERIA_LOCUS_TOKEN },

    /* fractura-mollis (81..82) */
    { "finis",       (s32)MATERIA_LOCUS_TOKEN },
    { "praefixa",    (s32)MATERIA_LOCUS_LISTA_TOKEN },

    /* fractura-dura (83..85) */
    { "signum",      (s32)MATERIA_LOCUS_TOKEN },
    { "finis",       (s32)MATERIA_LOCUS_TOKEN },
    { "praefixa",    (s32)MATERIA_LOCUS_LISTA_TOKEN },

    /* html-inlineum (86) */
    { "crudum",      (s32)MATERIA_LOCUS_LISTA_TOKEN }
};

hic_manens constans MateriaTabGenus GENERA_MD_NODI[] = {
    /* titulus              offset   numerus */
    { "documentum",        (i32)0,  (i32)3 },
    { "praefatio",         (i32)3,  (i32)3 },
    { "linea",             (i32)6,  (i32)3 },
    { "linea-vacua",       (i32)9,  (i32)2 },
    { "paragraphus",       (i32)11, (i32)3 },
    { "capitulum",         (i32)14, (i32)7 },
    { "saeptum",           (i32)21, (i32)5 },
    { "limes",             (i32)26, (i32)4 },
    { "citatio",           (i32)30, (i32)1 },
    { "lista",             (i32)31, (i32)4 },
    { "elementum",         (i32)35, (i32)2 },
    { "divisio",           (i32)37, (i32)1 },
    { "tabula",            (i32)38, (i32)3 },
    { "ordo",              (i32)41, (i32)4 },
    { "cella",             (i32)45, (i32)3 },
    { "html",              (i32)48, (i32)1 },
    { "definitio-nexus",   (i32)49, (i32)4 },
    { "inlinea",           (i32)53, (i32)1 },
    { "textus",            (i32)54, (i32)2 },
    { "emphasis",          (i32)56, (i32)3 },
    { "fortis",            (i32)59, (i32)3 },
    { "deletio",           (i32)62, (i32)3 },
    { "verbatim",          (i32)65, (i32)4 },
    { "nexus",             (i32)69, (i32)6 },
    { "imago",             (i32)75, (i32)6 },
    { "fractura-mollis",   (i32)81, (i32)2 },
    { "fractura-dura",     (i32)83, (i32)3 },
    { "html-inlineum",     (i32)86, (i32)1 }
};

constans MateriaRegistrumCoctum MD_REGISTRUM = {
    GENERA_MD_NODI,
    (i32)(magnitudo(GENERA_MD_NODI) / magnitudo(GENERA_MD_NODI[0])),
    LOCI_MD,
    (i32)(magnitudo(LOCI_MD) / magnitudo(LOCI_MD[0]))
};
