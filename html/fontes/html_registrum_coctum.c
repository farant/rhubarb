/* html_registrum_coctum.c
 *
 * Registrum generum COCTUM grammaticae 'html' - GENERATUM, NE MANU
 * EDITES. Fons: html/grammatica/html.registrum.stml
 * (materia/coquere.sh). Series LOCORUM plana; quodque genus fenestram
 * suam per loci_offset + loci_numerus nominat. Genera XI, loci XXVI.
 */

#include "html_registrum_coctum.h"
#include "materia_nodus.h"

hic_manens constans MateriaTabLocus LOCI_COCTI[] = {
    /* documentum (0..1) */
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "cauda", (s32)MATERIA_LOCUS_TOKEN },

    /* doctype (2..2) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* elementum (3..12) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "attributa", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_finis", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_clausura_finis", (s32)MATERIA_LOCUS_TOKEN },
    { "synthesis", (s32)MATERIA_LOCUS_INDEX },
    { "sedes", (s32)MATERIA_LOCUS_REFERENTIA },
    { "exemplar", (s32)MATERIA_LOCUS_REFERENTIA },
    { "praecedens", (s32)MATERIA_LOCUS_REFERENTIA },

    /* attributum (13..15) */
    { "tok_nomen", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_aequale", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_valor", (s32)MATERIA_LOCUS_TOKEN },

    /* textus (16..17) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },
    { "sedes", (s32)MATERIA_LOCUS_REFERENTIA },

    /* referentia (18..19) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },
    { "sedes", (s32)MATERIA_LOCUS_REFERENTIA },

    /* textus-crudus (20..20) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* commentarium (21..22) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },
    { "sedes", (s32)MATERIA_LOCUS_REFERENTIA },

    /* cdata (23..23) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* elementum-malum (24..24) */
    { "tokens", (s32)MATERIA_LOCUS_LISTA_TOKEN },

    /* spatium-omissum (25..25) */
    { "tokens", (s32)MATERIA_LOCUS_LISTA_TOKEN },
};

hic_manens constans MateriaTabGenus GENERA_COCTA[] = {
    /* titulus, offset, numerus */
    { "documentum", (i32)0, (i32)2 },
    { "doctype", (i32)2, (i32)1 },
    { "elementum", (i32)3, (i32)10 },
    { "attributum", (i32)13, (i32)3 },
    { "textus", (i32)16, (i32)2 },
    { "referentia", (i32)18, (i32)2 },
    { "textus-crudus", (i32)20, (i32)1 },
    { "commentarium", (i32)21, (i32)2 },
    { "cdata", (i32)23, (i32)1 },
    { "elementum-malum", (i32)24, (i32)1 },
    { "spatium-omissum", (i32)25, (i32)1 },
};

constans MateriaRegistrumCoctum HTML_REGISTRUM = {
    GENERA_COCTA,
    (i32)(magnitudo(GENERA_COCTA) / magnitudo(GENERA_COCTA[0])),
    LOCI_COCTI,
    (i32)(magnitudo(LOCI_COCTI) / magnitudo(LOCI_COCTI[0]))
};

hic_manens constans MateriaTabDiagnosticum DIAGNOSTICA_COCTA[] = {
    /* genus, locus, species, gravitas, codex, causa */
    { (s32)HTML_GENUS_ELEMENTUM_MALUM, (s32)-1,
      (s32)MATERIA_DIAGNOSTICUM_GENUS,
      (s32)MATERIA_GRAVITAS_ERRATUM,
      "elementum-malum",
      "tag clausurae sine pari, aut lexema quod HTML5 neglegit" },
};

constans MateriaDiagnosticaCocta HTML_DIAGNOSTICA = {
    DIAGNOSTICA_COCTA,
    (i32)(magnitudo(DIAGNOSTICA_COCTA) /
        magnitudo(DIAGNOSTICA_COCTA[0])),
    NIHIL,
    (i32)0
};
