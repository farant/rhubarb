/* xml_registrum_coctum.c - GENERATUM, NE MANU EDITES
 *
 * Registrum generum COCTUM grammaticae 'xml'. Fons:
 * xml/grammatica/xml.registrum.stml (materia/coquere.sh). Series
 * LOCORUM plana; quodque genus fenestram suam per loci_offset +
 * loci_numerus nominat. Genera XV, loci XXXIII.
 */

#include "xml_registrum_coctum.h"
#include "materia_nodus.h"

hic_manens constans MateriaTabLocus LOCI_COCTI[] = {
    /* documentum (0..1) */
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "cauda", (s32)MATERIA_LOCUS_TOKEN },

    /* signum-ordinis (2..2) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* declaratio-xml (3..5) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "attributa", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },

    /* doctype (6..10) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_titulus", (s32)MATERIA_LOCUS_TOKEN },
    { "id_externum", (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "pars_interna", (s32)MATERIA_LOCUS_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },

    /* pars-interna (11..13) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },

    /* declaratio (14..16) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "partes", (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },

    /* referentia-parametri (17..17) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* elementum (18..23) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "attributa", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_finis", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_clausura_finis", (s32)MATERIA_LOCUS_TOKEN },

    /* attributum (24..26) */
    { "tok_titulus", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_aequale", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_valor", (s32)MATERIA_LOCUS_TOKEN },

    /* textus (27..27) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* referentia (28..28) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* cdata (29..29) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* commentarium (30..30) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* instructio (31..31) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* malum (32..32) */
    { "tokens", (s32)MATERIA_LOCUS_LISTA_TOKEN },
};

hic_manens constans MateriaTabGenus GENERA_COCTA[] = {
    /* titulus, offset, numerus */
    { "documentum", (i32)0, (i32)2 },
    { "signum-ordinis", (i32)2, (i32)1 },
    { "declaratio-xml", (i32)3, (i32)3 },
    { "doctype", (i32)6, (i32)5 },
    { "pars-interna", (i32)11, (i32)3 },
    { "declaratio", (i32)14, (i32)3 },
    { "referentia-parametri", (i32)17, (i32)1 },
    { "elementum", (i32)18, (i32)6 },
    { "attributum", (i32)24, (i32)3 },
    { "textus", (i32)27, (i32)1 },
    { "referentia", (i32)28, (i32)1 },
    { "cdata", (i32)29, (i32)1 },
    { "commentarium", (i32)30, (i32)1 },
    { "instructio", (i32)31, (i32)1 },
    { "malum", (i32)32, (i32)1 },
};

constans MateriaRegistrumCoctum XML_REGISTRUM = {
    GENERA_COCTA,
    (i32)(magnitudo(GENERA_COCTA) / magnitudo(GENERA_COCTA[0])),
    LOCI_COCTI,
    (i32)(magnitudo(LOCI_COCTI) / magnitudo(LOCI_COCTI[0]))
};

hic_manens constans MateriaTabDiagnosticum DIAGNOSTICA_COCTA[] = {
    /* genus, locus, species, gravitas, codex, causa */
    { (s32)XML_GENUS_DECLARATIO_XML, (s32)2,
      (s32)MATERIA_DIAGNOSTICUM_ABSENTIA,
      (s32)MATERIA_GRAVITAS_ERRATUM,
      "declaratio-xml/tok_clausura",
      "'?&gt;' deest" },
    { (s32)XML_GENUS_DOCTYPE, (s32)1,
      (s32)MATERIA_DIAGNOSTICUM_ABSENTIA,
      (s32)MATERIA_GRAVITAS_ERRATUM,
      "doctype/tok_titulus",
      "titulus typi deest" },
    { (s32)XML_GENUS_DOCTYPE, (s32)4,
      (s32)MATERIA_DIAGNOSTICUM_ABSENTIA,
      (s32)MATERIA_GRAVITAS_ERRATUM,
      "doctype/tok_clausura",
      "'&gt;' deest" },
    { (s32)XML_GENUS_PARS_INTERNA, (s32)2,
      (s32)MATERIA_DIAGNOSTICUM_ABSENTIA,
      (s32)MATERIA_GRAVITAS_ERRATUM,
      "pars-interna/tok_clausura",
      "']' deest" },
    { (s32)XML_GENUS_DECLARATIO, (s32)2,
      (s32)MATERIA_DIAGNOSTICUM_ABSENTIA,
      (s32)MATERIA_GRAVITAS_ERRATUM,
      "declaratio/tok_clausura",
      "'&gt;' deest" },
    { (s32)XML_GENUS_ELEMENTUM, (s32)2,
      (s32)MATERIA_DIAGNOSTICUM_ABSENTIA,
      (s32)MATERIA_GRAVITAS_ERRATUM,
      "elementum/tok_finis",
      "'&gt;' deest" },
    { (s32)XML_GENUS_ATTRIBUTUM, (s32)1,
      (s32)MATERIA_DIAGNOSTICUM_ABSENTIA,
      (s32)MATERIA_GRAVITAS_ERRATUM,
      "attributum/tok_aequale",
      "'=' deest" },
    { (s32)XML_GENUS_ATTRIBUTUM, (s32)2,
      (s32)MATERIA_DIAGNOSTICUM_ABSENTIA,
      (s32)MATERIA_GRAVITAS_ERRATUM,
      "attributum/tok_valor",
      "valor deest" },
    { (s32)XML_GENUS_MALUM, (s32)-1,
      (s32)MATERIA_DIAGNOSTICUM_GENUS,
      (s32)MATERIA_GRAVITAS_ERRATUM,
      "malum",
      "lexemata quae grammatica ponere non potuit" },
};

hic_manens constans s32 INANIA_COCTA[] = {
    (s32)XML_GENUS_MALUM,
};

constans MateriaDiagnosticaCocta XML_DIAGNOSTICA = {
    DIAGNOSTICA_COCTA,
    (i32)(magnitudo(DIAGNOSTICA_COCTA) /
        magnitudo(DIAGNOSTICA_COCTA[0])),
    INANIA_COCTA,
    (i32)(magnitudo(INANIA_COCTA) /
        magnitudo(INANIA_COCTA[0]))
};
