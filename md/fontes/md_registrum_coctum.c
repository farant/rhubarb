/* md_registrum_coctum.c
 *
 * Registrum generum COCTUM grammaticae 'md' - GENERATUM, NE MANU
 * EDITES. Fons: md/grammatica/md.registrum.stml (materia/coquere.sh).
 * Series LOCORUM plana; quodque genus fenestram suam per loci_offset +
 * loci_numerus nominat. Genera XXVIII, loci XCIV.
 */

#include "md_registrum_coctum.h"
#include "materia_nodus.h"

hic_manens constans MateriaTabLocus LOCI_COCTI[] = {
    /* documentum (0..2) */
    { "praefatio", (s32)MATERIA_LOCUS_NODUS },
    { "bloci", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "finis", (s32)MATERIA_LOCUS_TOKEN },

    /* praefatio (3..5) */
    { "apertum", (s32)MATERIA_LOCUS_NODUS },
    { "lineae", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "clausum", (s32)MATERIA_LOCUS_NODUS },

    /* linea (6..8) */
    { "praefixa", (s32)MATERIA_LOCUS_LISTA_TOKEN },
    /* scissa ad &quot;&lt;/md-&quot; */
    { "contentum", (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "finis", (s32)MATERIA_LOCUS_TOKEN },

    /* linea-vacua (9..10) */
    { "praefixa", (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "finis", (s32)MATERIA_LOCUS_TOKEN },

    /* paragraphus (11..14) */
    { "praefixa", (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "nudus", (s32)MATERIA_LOCUS_INDEX },
    { "inlinea", (s32)MATERIA_LOCUS_NODUS },
    { "finis", (s32)MATERIA_LOCUS_TOKEN },

    /* capitulum (15..21) */
    { "praefixa", (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "gradus", (s32)MATERIA_LOCUS_INDEX },
    { "marca", (s32)MATERIA_LOCUS_TOKEN },
    { "inlinea", (s32)MATERIA_LOCUS_NODUS },
    { "clausum", (s32)MATERIA_LOCUS_TOKEN },
    { "finis", (s32)MATERIA_LOCUS_TOKEN },
    { "subductio", (s32)MATERIA_LOCUS_NODUS },

    /* saeptum (22..27) */
    { "forma", (s32)MATERIA_LOCUS_INDEX },
    { "apertum", (s32)MATERIA_LOCUS_NODUS },
    /* DERIVATUM: verbum primum info */
    { "lingua", (s32)MATERIA_LOCUS_TOKEN },
    { "lineae", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "clausum", (s32)MATERIA_LOCUS_NODUS },
    /* DERIVATUM: contentum decoctum, lineae + '\n' */
    { "valor", (s32)MATERIA_LOCUS_TOKEN },

    /* limes (28..31) */
    { "praefixa", (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "signum", (s32)MATERIA_LOCUS_TOKEN },
    { "info", (s32)MATERIA_LOCUS_TOKEN },
    { "finis", (s32)MATERIA_LOCUS_TOKEN },

    /* citatio (32..32) */
    { "bloci", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* lista (33..36) */
    { "genus", (s32)MATERIA_LOCUS_INDEX },
    { "initium", (s32)MATERIA_LOCUS_INDEX },
    { "laxa", (s32)MATERIA_LOCUS_INDEX },
    { "elementa", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* elementum (37..38) */
    { "officium", (s32)MATERIA_LOCUS_INDEX },
    { "bloci", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* divisio (39..39) */
    { "linea", (s32)MATERIA_LOCUS_NODUS },

    /* tabula (40..42) */
    { "caput", (s32)MATERIA_LOCUS_NODUS },
    { "separator", (s32)MATERIA_LOCUS_NODUS },
    { "ordines", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* ordo (43..46) */
    { "praefixa", (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "cellae", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "clausum", (s32)MATERIA_LOCUS_TOKEN },
    { "finis", (s32)MATERIA_LOCUS_TOKEN },

    /* cella (47..49) */
    { "ordinatio", (s32)MATERIA_LOCUS_INDEX },
    { "apertum", (s32)MATERIA_LOCUS_TOKEN },
    { "inlinea", (s32)MATERIA_LOCUS_NODUS },

    /* html (50..51) */
    { "lineae", (s32)MATERIA_LOCUS_LISTA_NODUS },
    /* DERIVATUM: lineae verbatim + '\n' (B3.1) */
    { "valor", (s32)MATERIA_LOCUS_TOKEN },

    /* definitio-nexus (52..55) */
    { "lineae", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "titulus", (s32)MATERIA_LOCUS_TOKEN },
    { "url", (s32)MATERIA_LOCUS_TOKEN },
    { "descriptio", (s32)MATERIA_LOCUS_TOKEN },

    /* inlinea (56..56) */
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* textus (57..58) */
    { "crudum", (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "valor", (s32)MATERIA_LOCUS_TOKEN },

    /* emphasis (59..61) */
    { "apertum", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "clausum", (s32)MATERIA_LOCUS_TOKEN },

    /* fortis (62..64) */
    { "apertum", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "clausum", (s32)MATERIA_LOCUS_TOKEN },

    /* deletio (65..67) */
    { "apertum", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "clausum", (s32)MATERIA_LOCUS_TOKEN },

    /* verbatim (68..71) */
    { "apertum", (s32)MATERIA_LOCUS_TOKEN },
    { "crudum", (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "valor", (s32)MATERIA_LOCUS_TOKEN },
    { "clausum", (s32)MATERIA_LOCUS_TOKEN },

    /* nexus (72..77) */
    { "forma", (s32)MATERIA_LOCUS_INDEX },
    { "apertum", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "cauda", (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "url", (s32)MATERIA_LOCUS_TOKEN },
    { "descriptio", (s32)MATERIA_LOCUS_TOKEN },

    /* imago (78..84) */
    { "forma", (s32)MATERIA_LOCUS_INDEX },
    { "apertum", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "cauda", (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "url", (s32)MATERIA_LOCUS_TOKEN },
    { "descriptio", (s32)MATERIA_LOCUS_TOKEN },
    /* DERIVATUM: textus planus liberorum */
    { "alt", (s32)MATERIA_LOCUS_TOKEN },

    /* fractura-mollis (85..87) */
    { "finis", (s32)MATERIA_LOCUS_TOKEN },
    { "praefixa", (s32)MATERIA_LOCUS_LISTA_TOKEN },
    /* DERIVATUM: '\n' redditum (terminator octetos in proiectione
     * non fert) */
    { "valor", (s32)MATERIA_LOCUS_TOKEN },

    /* fractura-dura (88..91) */
    { "signum", (s32)MATERIA_LOCUS_TOKEN },
    { "finis", (s32)MATERIA_LOCUS_TOKEN },
    { "praefixa", (s32)MATERIA_LOCUS_LISTA_TOKEN },
    /* DERIVATUM: '\n' post &lt;br/> */
    { "valor", (s32)MATERIA_LOCUS_TOKEN },

    /* html-inlineum (92..93) */
    { "crudum", (s32)MATERIA_LOCUS_LISTA_TOKEN },
    /* DERIVATUM: octeti cum lineis novis (B3.3, cum lineas plures
     * tenet) */
    { "valor", (s32)MATERIA_LOCUS_TOKEN },
};

hic_manens constans MateriaTabGenus GENERA_COCTA[] = {
    /* titulus, offset, numerus */
    { "documentum", (i32)0, (i32)3 },
    { "praefatio", (i32)3, (i32)3 },
    { "linea", (i32)6, (i32)3 },
    { "linea-vacua", (i32)9, (i32)2 },
    { "paragraphus", (i32)11, (i32)4 },
    { "capitulum", (i32)15, (i32)7 },
    { "saeptum", (i32)22, (i32)6 },
    { "limes", (i32)28, (i32)4 },
    { "citatio", (i32)32, (i32)1 },
    { "lista", (i32)33, (i32)4 },
    { "elementum", (i32)37, (i32)2 },
    { "divisio", (i32)39, (i32)1 },
    { "tabula", (i32)40, (i32)3 },
    { "ordo", (i32)43, (i32)4 },
    { "cella", (i32)47, (i32)3 },
    { "html", (i32)50, (i32)2 },
    { "definitio-nexus", (i32)52, (i32)4 },
    { "inlinea", (i32)56, (i32)1 },
    { "textus", (i32)57, (i32)2 },
    { "emphasis", (i32)59, (i32)3 },
    { "fortis", (i32)62, (i32)3 },
    { "deletio", (i32)65, (i32)3 },
    { "verbatim", (i32)68, (i32)4 },
    { "nexus", (i32)72, (i32)6 },
    { "imago", (i32)78, (i32)7 },
    { "fractura-mollis", (i32)85, (i32)3 },
    { "fractura-dura", (i32)88, (i32)4 },
    { "html-inlineum", (i32)92, (i32)2 },
};

constans MateriaRegistrumCoctum MD_REGISTRUM = {
    GENERA_COCTA,
    (i32)(magnitudo(GENERA_COCTA) / magnitudo(GENERA_COCTA[0])),
    LOCI_COCTI,
    (i32)(magnitudo(LOCI_COCTI) / magnitudo(LOCI_COCTI[0]))
};
