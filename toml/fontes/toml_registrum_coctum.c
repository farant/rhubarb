/* toml_registrum_coctum.c
 *
 * Registrum generum COCTUM grammaticae 'toml' - GENERATUM, NE MANU
 * EDITES. Fons: toml/grammatica/toml.registrum.stml
 * (materia/coquere.sh). Series LOCORUM plana; quodque genus fenestram
 * suam per loci_offset + loci_numerus nominat. Genera XIV, loci XXV.
 */

#include "toml_registrum_coctum.h"
#include "materia_nodus.h"

hic_manens constans MateriaTabLocus LOCI_COCTI[] = {
    /* documentum (0..1) */
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "cauda", (s32)MATERIA_LOCUS_TOKEN },

    /* caput-tabulae (2..4) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "clavis", (s32)MATERIA_LOCUS_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },

    /* caput-seriei (5..7) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "clavis", (s32)MATERIA_LOCUS_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },

    /* par (8..10) */
    { "clavis", (s32)MATERIA_LOCUS_NODUS },
    { "tok_signum", (s32)MATERIA_LOCUS_TOKEN },
    { "valor", (s32)MATERIA_LOCUS_NODUS },

    /* clavis (11..11) */
    { "partes", (s32)MATERIA_LOCUS_LISTA_TOKEN },

    /* chorda (12..12) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* numerus (13..13) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* boolean (14..14) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* tempus (15..15) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* series (16..18) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },

    /* tabula-compacta (19..21) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },

    /* comma (22..22) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* linea (23..23) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* malum (24..24) */
    { "tokens", (s32)MATERIA_LOCUS_LISTA_TOKEN },
};

hic_manens constans MateriaTabGenus GENERA_COCTA[] = {
    /* titulus, offset, numerus */
    { "documentum", (i32)0, (i32)2 },
    { "caput-tabulae", (i32)2, (i32)3 },
    { "caput-seriei", (i32)5, (i32)3 },
    { "par", (i32)8, (i32)3 },
    { "clavis", (i32)11, (i32)1 },
    { "chorda", (i32)12, (i32)1 },
    { "numerus", (i32)13, (i32)1 },
    { "boolean", (i32)14, (i32)1 },
    { "tempus", (i32)15, (i32)1 },
    { "series", (i32)16, (i32)3 },
    { "tabula-compacta", (i32)19, (i32)3 },
    { "comma", (i32)22, (i32)1 },
    { "linea", (i32)23, (i32)1 },
    { "malum", (i32)24, (i32)1 },
};

constans MateriaRegistrumCoctum TOML_REGISTRUM = {
    GENERA_COCTA,
    (i32)(magnitudo(GENERA_COCTA) / magnitudo(GENERA_COCTA[0])),
    LOCI_COCTI,
    (i32)(magnitudo(LOCI_COCTI) / magnitudo(LOCI_COCTI[0]))
};

hic_manens constans MateriaTabDiagnosticum DIAGNOSTICA_COCTA[] = {
    /* genus, locus, species, gravitas, codex, causa */
    { (s32)TOML_GENUS_CAPUT_TABULAE, (s32)1,
      (s32)MATERIA_DIAGNOSTICUM_ABSENTIA,
      (s32)MATERIA_GRAVITAS_ERRATUM,
      "caput-tabulae/clavis",
      "clavis tabulae deest" },
    { (s32)TOML_GENUS_CAPUT_TABULAE, (s32)2,
      (s32)MATERIA_DIAGNOSTICUM_ABSENTIA,
      (s32)MATERIA_GRAVITAS_ERRATUM,
      "caput-tabulae/tok_clausura",
      "']' deest" },
    { (s32)TOML_GENUS_CAPUT_SERIEI, (s32)1,
      (s32)MATERIA_DIAGNOSTICUM_ABSENTIA,
      (s32)MATERIA_GRAVITAS_ERRATUM,
      "caput-seriei/clavis",
      "clavis seriei tabularum deest" },
    { (s32)TOML_GENUS_CAPUT_SERIEI, (s32)2,
      (s32)MATERIA_DIAGNOSTICUM_ABSENTIA,
      (s32)MATERIA_GRAVITAS_ERRATUM,
      "caput-seriei/tok_clausura",
      "']]' deest" },
    { (s32)TOML_GENUS_PAR, (s32)1,
      (s32)MATERIA_DIAGNOSTICUM_ABSENTIA,
      (s32)MATERIA_GRAVITAS_ERRATUM,
      "par/tok_signum",
      "'=' deest" },
    { (s32)TOML_GENUS_PAR, (s32)2,
      (s32)MATERIA_DIAGNOSTICUM_ABSENTIA,
      (s32)MATERIA_GRAVITAS_ERRATUM,
      "par/valor",
      "valor deest" },
    { (s32)TOML_GENUS_SERIES, (s32)2,
      (s32)MATERIA_DIAGNOSTICUM_ABSENTIA,
      (s32)MATERIA_GRAVITAS_ERRATUM,
      "series/tok_clausura",
      "']' deest" },
    { (s32)TOML_GENUS_TABULA_COMPACTA, (s32)2,
      (s32)MATERIA_DIAGNOSTICUM_ABSENTIA,
      (s32)MATERIA_GRAVITAS_ERRATUM,
      "tabula-compacta/tok_clausura",
      "'}' deest" },
    { (s32)TOML_GENUS_MALUM, (s32)-1,
      (s32)MATERIA_DIAGNOSTICUM_GENUS,
      (s32)MATERIA_GRAVITAS_ERRATUM,
      "malum",
      "lexemata quae grammatica ponere non potuit" },
};

hic_manens constans s32 INANIA_COCTA[] = {
    (s32)TOML_GENUS_COMMA,
    (s32)TOML_GENUS_LINEA,
    (s32)TOML_GENUS_MALUM,
};

constans MateriaDiagnosticaCocta TOML_DIAGNOSTICA = {
    DIAGNOSTICA_COCTA,
    (i32)(magnitudo(DIAGNOSTICA_COCTA) /
        magnitudo(DIAGNOSTICA_COCTA[0])),
    INANIA_COCTA,
    (i32)(magnitudo(INANIA_COCTA) /
        magnitudo(INANIA_COCTA[0]))
};
