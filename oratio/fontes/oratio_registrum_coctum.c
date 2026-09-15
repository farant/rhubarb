/* oratio_registrum_coctum.c
 *
 * Registrum generum COCTUM grammaticae 'oratio' - GENERATUM, NE MANU
 * EDITES. Fons: oratio/grammatica/oratio.registrum.stml
 * (materia/coquere.sh). Series LOCORUM plana; quodque genus fenestram
 * suam per loci_offset + loci_numerus nominat. Genera XXVI, loci CXCI.
 */

#include "oratio_registrum_coctum.h"
#include "materia_nodus.h"

hic_manens constans MateriaTabLocus LOCI_COCTI[] = {
    /* documentum (0..2) */
    { "praefixa", (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "paragraphi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "finis", (s32)MATERIA_LOCUS_TOKEN },

    /* paragraphus (3..6) */
    { "praefixa", (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "sententiae", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "cauda", (s32)MATERIA_LOCUS_LISTA_TOKEN },
    /* OratioForma (T6b) */
    { "forma", (s32)MATERIA_LOCUS_INDEX },

    /* sententia (7..8) */
    { "elementa", (s32)MATERIA_LOCUS_LISTA_NODUS },
    /* T20a: clausulae sententiae */
    { "clausulae", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* vocabulum (9..18) */
    { "partes", (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "cauda", (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "analyses", (s32)MATERIA_LOCUS_LISTA_NODUS },
    /* DERIVATUM */
    { "classes", (s32)MATERIA_LOCUS_TOKEN },
    /* DERIVATUM */
    { "linguae", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioDecisio (T19g) */
    { "decisio", (s32)MATERIA_LOCUS_INDEX },
    /* DERIVATUM (T19g) */
    { "auctor", (s32)MATERIA_LOCUS_TOKEN },
    /* T20a: clausula intra sententiam */
    { "clausula", (s32)MATERIA_LOCUS_INDEX },
    /* OratioClausulaCausa (T20a) */
    { "clausula-causa", (s32)MATERIA_LOCUS_INDEX },
    /* OratioHabitus (T38 c) */
    { "habitus", (s32)MATERIA_LOCUS_INDEX },

    /* interpunctio (19..22) */
    { "signum", (s32)MATERIA_LOCUS_TOKEN },
    { "cauda", (s32)MATERIA_LOCUS_LISTA_TOKEN },
    /* T20a */
    { "clausula", (s32)MATERIA_LOCUS_INDEX },
    /* OratioClausulaCausa (T20a) */
    { "clausula-causa", (s32)MATERIA_LOCUS_INDEX },

    /* numerus (23..26) */
    { "crudum", (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "cauda", (s32)MATERIA_LOCUS_LISTA_TOKEN },
    /* T20a */
    { "clausula", (s32)MATERIA_LOCUS_INDEX },
    /* OratioClausulaCausa (T20a) */
    { "clausula-causa", (s32)MATERIA_LOCUS_INDEX },

    /* analysis-substantivi (27..36) */
    /* DERIVATUM */
    { "lemma", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioLingua */
    { "lingua", (s32)MATERIA_LOCUS_INDEX },
    /* OratioFonsAnalysis */
    { "fons", (s32)MATERIA_LOCUS_INDEX },
    /* DERIVATUM */
    { "nativum", (s32)MATERIA_LOCUS_TOKEN },
    /* DERIVATUM */
    { "sensus", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioCasus */
    { "casus", (s32)MATERIA_LOCUS_INDEX },
    /* OratioNumerusGrammaticus */
    { "numerus", (s32)MATERIA_LOCUS_INDEX },
    /* OratioGenusGrammaticum */
    { "genus", (s32)MATERIA_LOCUS_INDEX },
    /* declinatio I-V (WORDS) */
    { "declinatio", (s32)MATERIA_LOCUS_INDEX },
    /* T19d: umbrae lectionis */
    { "umbrae", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* analysis-nominis-proprii (37..46) */
    /* DERIVATUM */
    { "lemma", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioLingua */
    { "lingua", (s32)MATERIA_LOCUS_INDEX },
    /* OratioFonsAnalysis */
    { "fons", (s32)MATERIA_LOCUS_INDEX },
    /* DERIVATUM */
    { "nativum", (s32)MATERIA_LOCUS_TOKEN },
    /* DERIVATUM */
    { "sensus", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioCasus */
    { "casus", (s32)MATERIA_LOCUS_INDEX },
    /* OratioNumerusGrammaticus */
    { "numerus", (s32)MATERIA_LOCUS_INDEX },
    /* OratioGenusGrammaticum */
    { "genus", (s32)MATERIA_LOCUS_INDEX },
    /* declinatio I-V (WORDS) */
    { "declinatio", (s32)MATERIA_LOCUS_INDEX },
    /* T19d: umbrae lectionis */
    { "umbrae", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* analysis-verbi (47..61) */
    /* DERIVATUM */
    { "lemma", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioLingua */
    { "lingua", (s32)MATERIA_LOCUS_INDEX },
    /* OratioFonsAnalysis */
    { "fons", (s32)MATERIA_LOCUS_INDEX },
    /* DERIVATUM */
    { "nativum", (s32)MATERIA_LOCUS_TOKEN },
    /* DERIVATUM */
    { "sensus", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioPersona */
    { "persona", (s32)MATERIA_LOCUS_INDEX },
    /* OratioNumerusGrammaticus */
    { "numerus", (s32)MATERIA_LOCUS_INDEX },
    /* OratioTempus */
    { "tempus", (s32)MATERIA_LOCUS_INDEX },
    /* OratioModus */
    { "modus", (s32)MATERIA_LOCUS_INDEX },
    /* OratioVox */
    { "vox", (s32)MATERIA_LOCUS_INDEX },
    /* OratioFormaVerbi */
    { "forma-verbi", (s32)MATERIA_LOCUS_INDEX },
    /* coniugatio I-IV (WORDS) */
    { "coniugatio", (s32)MATERIA_LOCUS_INDEX },
    /* OratioCasus */
    { "casus", (s32)MATERIA_LOCUS_INDEX },
    /* OratioGenusGrammaticum */
    { "genus", (s32)MATERIA_LOCUS_INDEX },
    /* T19d: umbrae lectionis */
    { "umbrae", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* analysis-auxiliaris (62..76) */
    /* DERIVATUM */
    { "lemma", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioLingua */
    { "lingua", (s32)MATERIA_LOCUS_INDEX },
    /* OratioFonsAnalysis */
    { "fons", (s32)MATERIA_LOCUS_INDEX },
    /* DERIVATUM */
    { "nativum", (s32)MATERIA_LOCUS_TOKEN },
    /* DERIVATUM */
    { "sensus", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioPersona */
    { "persona", (s32)MATERIA_LOCUS_INDEX },
    /* OratioNumerusGrammaticus */
    { "numerus", (s32)MATERIA_LOCUS_INDEX },
    /* OratioTempus */
    { "tempus", (s32)MATERIA_LOCUS_INDEX },
    /* OratioModus */
    { "modus", (s32)MATERIA_LOCUS_INDEX },
    /* OratioVox */
    { "vox", (s32)MATERIA_LOCUS_INDEX },
    /* OratioFormaVerbi */
    { "forma-verbi", (s32)MATERIA_LOCUS_INDEX },
    /* coniugatio I-IV (WORDS) */
    { "coniugatio", (s32)MATERIA_LOCUS_INDEX },
    /* OratioCasus */
    { "casus", (s32)MATERIA_LOCUS_INDEX },
    /* OratioGenusGrammaticum */
    { "genus", (s32)MATERIA_LOCUS_INDEX },
    /* T19d: umbrae lectionis */
    { "umbrae", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* analysis-adiectivi (77..87) */
    /* DERIVATUM */
    { "lemma", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioLingua */
    { "lingua", (s32)MATERIA_LOCUS_INDEX },
    /* OratioFonsAnalysis */
    { "fons", (s32)MATERIA_LOCUS_INDEX },
    /* DERIVATUM */
    { "nativum", (s32)MATERIA_LOCUS_TOKEN },
    /* DERIVATUM */
    { "sensus", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioCasus */
    { "casus", (s32)MATERIA_LOCUS_INDEX },
    /* OratioNumerusGrammaticus */
    { "numerus", (s32)MATERIA_LOCUS_INDEX },
    /* OratioGenusGrammaticum */
    { "genus", (s32)MATERIA_LOCUS_INDEX },
    /* OratioGradus */
    { "gradus", (s32)MATERIA_LOCUS_INDEX },
    /* declinatio I-V (WORDS) */
    { "declinatio", (s32)MATERIA_LOCUS_INDEX },
    /* T19d: umbrae lectionis */
    { "umbrae", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* analysis-adverbii (88..94) */
    /* DERIVATUM */
    { "lemma", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioLingua */
    { "lingua", (s32)MATERIA_LOCUS_INDEX },
    /* OratioFonsAnalysis */
    { "fons", (s32)MATERIA_LOCUS_INDEX },
    /* DERIVATUM */
    { "nativum", (s32)MATERIA_LOCUS_TOKEN },
    /* DERIVATUM */
    { "sensus", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioGradus */
    { "gradus", (s32)MATERIA_LOCUS_INDEX },
    /* T19d: umbrae lectionis */
    { "umbrae", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* analysis-pronominis (95..104) */
    /* DERIVATUM */
    { "lemma", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioLingua */
    { "lingua", (s32)MATERIA_LOCUS_INDEX },
    /* OratioFonsAnalysis */
    { "fons", (s32)MATERIA_LOCUS_INDEX },
    /* DERIVATUM */
    { "nativum", (s32)MATERIA_LOCUS_TOKEN },
    /* DERIVATUM */
    { "sensus", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioCasus */
    { "casus", (s32)MATERIA_LOCUS_INDEX },
    /* OratioNumerusGrammaticus */
    { "numerus", (s32)MATERIA_LOCUS_INDEX },
    /* OratioGenusGrammaticum */
    { "genus", (s32)MATERIA_LOCUS_INDEX },
    /* OratioPersona */
    { "persona", (s32)MATERIA_LOCUS_INDEX },
    /* T19d: umbrae lectionis */
    { "umbrae", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* analysis-determinantis (105..113) */
    /* DERIVATUM */
    { "lemma", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioLingua */
    { "lingua", (s32)MATERIA_LOCUS_INDEX },
    /* OratioFonsAnalysis */
    { "fons", (s32)MATERIA_LOCUS_INDEX },
    /* DERIVATUM */
    { "nativum", (s32)MATERIA_LOCUS_TOKEN },
    /* DERIVATUM */
    { "sensus", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioCasus */
    { "casus", (s32)MATERIA_LOCUS_INDEX },
    /* OratioNumerusGrammaticus */
    { "numerus", (s32)MATERIA_LOCUS_INDEX },
    /* OratioGenusGrammaticum */
    { "genus", (s32)MATERIA_LOCUS_INDEX },
    /* T19d: umbrae lectionis */
    { "umbrae", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* analysis-adpositionis (114..120) */
    /* DERIVATUM */
    { "lemma", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioLingua */
    { "lingua", (s32)MATERIA_LOCUS_INDEX },
    /* OratioFonsAnalysis */
    { "fons", (s32)MATERIA_LOCUS_INDEX },
    /* DERIVATUM */
    { "nativum", (s32)MATERIA_LOCUS_TOKEN },
    /* DERIVATUM */
    { "sensus", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioCasus */
    { "casus", (s32)MATERIA_LOCUS_INDEX },
    /* T19d: umbrae lectionis */
    { "umbrae", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* analysis-numeralis (121..130) */
    /* DERIVATUM */
    { "lemma", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioLingua */
    { "lingua", (s32)MATERIA_LOCUS_INDEX },
    /* OratioFonsAnalysis */
    { "fons", (s32)MATERIA_LOCUS_INDEX },
    /* DERIVATUM */
    { "nativum", (s32)MATERIA_LOCUS_TOKEN },
    /* DERIVATUM */
    { "sensus", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioCasus */
    { "casus", (s32)MATERIA_LOCUS_INDEX },
    /* OratioNumerusGrammaticus */
    { "numerus", (s32)MATERIA_LOCUS_INDEX },
    /* OratioGenusGrammaticum */
    { "genus", (s32)MATERIA_LOCUS_INDEX },
    /* OratioSpeciesNumeralis */
    { "species", (s32)MATERIA_LOCUS_INDEX },
    /* T19d: umbrae lectionis */
    { "umbrae", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* analysis-coniunctionis-coordinantis (131..136) */
    /* DERIVATUM */
    { "lemma", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioLingua */
    { "lingua", (s32)MATERIA_LOCUS_INDEX },
    /* OratioFonsAnalysis */
    { "fons", (s32)MATERIA_LOCUS_INDEX },
    /* DERIVATUM */
    { "nativum", (s32)MATERIA_LOCUS_TOKEN },
    /* DERIVATUM */
    { "sensus", (s32)MATERIA_LOCUS_TOKEN },
    /* T19d: umbrae lectionis */
    { "umbrae", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* analysis-coniunctionis-subordinantis (137..142) */
    /* DERIVATUM */
    { "lemma", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioLingua */
    { "lingua", (s32)MATERIA_LOCUS_INDEX },
    /* OratioFonsAnalysis */
    { "fons", (s32)MATERIA_LOCUS_INDEX },
    /* DERIVATUM */
    { "nativum", (s32)MATERIA_LOCUS_TOKEN },
    /* DERIVATUM */
    { "sensus", (s32)MATERIA_LOCUS_TOKEN },
    /* T19d: umbrae lectionis */
    { "umbrae", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* analysis-particulae (143..148) */
    /* DERIVATUM */
    { "lemma", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioLingua */
    { "lingua", (s32)MATERIA_LOCUS_INDEX },
    /* OratioFonsAnalysis */
    { "fons", (s32)MATERIA_LOCUS_INDEX },
    /* DERIVATUM */
    { "nativum", (s32)MATERIA_LOCUS_TOKEN },
    /* DERIVATUM */
    { "sensus", (s32)MATERIA_LOCUS_TOKEN },
    /* T19d: umbrae lectionis */
    { "umbrae", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* analysis-interiectionis (149..154) */
    /* DERIVATUM */
    { "lemma", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioLingua */
    { "lingua", (s32)MATERIA_LOCUS_INDEX },
    /* OratioFonsAnalysis */
    { "fons", (s32)MATERIA_LOCUS_INDEX },
    /* DERIVATUM */
    { "nativum", (s32)MATERIA_LOCUS_TOKEN },
    /* DERIVATUM */
    { "sensus", (s32)MATERIA_LOCUS_TOKEN },
    /* T19d: umbrae lectionis */
    { "umbrae", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* analysis-symboli (155..160) */
    /* DERIVATUM */
    { "lemma", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioLingua */
    { "lingua", (s32)MATERIA_LOCUS_INDEX },
    /* OratioFonsAnalysis */
    { "fons", (s32)MATERIA_LOCUS_INDEX },
    /* DERIVATUM */
    { "nativum", (s32)MATERIA_LOCUS_TOKEN },
    /* DERIVATUM */
    { "sensus", (s32)MATERIA_LOCUS_TOKEN },
    /* T19d: umbrae lectionis */
    { "umbrae", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* analysis-interpunctionis (161..166) */
    /* DERIVATUM */
    { "lemma", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioLingua */
    { "lingua", (s32)MATERIA_LOCUS_INDEX },
    /* OratioFonsAnalysis */
    { "fons", (s32)MATERIA_LOCUS_INDEX },
    /* DERIVATUM */
    { "nativum", (s32)MATERIA_LOCUS_TOKEN },
    /* DERIVATUM */
    { "sensus", (s32)MATERIA_LOCUS_TOKEN },
    /* T19d: umbrae lectionis */
    { "umbrae", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* analysis-ignoti (167..172) */
    /* DERIVATUM */
    { "lemma", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioLingua */
    { "lingua", (s32)MATERIA_LOCUS_INDEX },
    /* OratioFonsAnalysis */
    { "fons", (s32)MATERIA_LOCUS_INDEX },
    /* DERIVATUM */
    { "nativum", (s32)MATERIA_LOCUS_TOKEN },
    /* DERIVATUM */
    { "sensus", (s32)MATERIA_LOCUS_TOKEN },
    /* T19d: umbrae lectionis */
    { "umbrae", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* umbra (173..181) */
    /* OratioRelatio */
    { "relatio", (s32)MATERIA_LOCUS_INDEX },
    /* OratioClassis exspectata */
    { "classis", (s32)MATERIA_LOCUS_INDEX },
    /* OratioCasus */
    { "casus", (s32)MATERIA_LOCUS_INDEX },
    /* OratioNumerusGrammaticus */
    { "numerus", (s32)MATERIA_LOCUS_INDEX },
    /* OratioGenusGrammaticum */
    { "genus", (s32)MATERIA_LOCUS_INDEX },
    /* nodus analysis implentis (T33) */
    { "impletio", (s32)MATERIA_LOCUS_REFERENTIA },
    /* OratioFormaVerbi exspectata (T20a) */
    { "forma-verbi", (s32)MATERIA_LOCUS_INDEX },
    /* DERIVATUM: titulus regulae implentis (T32 a) */
    { "auctor", (s32)MATERIA_LOCUS_TOKEN },
    /* T32 b: petitiones cedentes (nodi alterna) */
    { "alternae", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* clausula (182..185) */
    /* ordinalis elementi seminis (T20a) */
    { "semen", (s32)MATERIA_LOCUS_INDEX },
    /* OratioSpeciesClausulae */
    { "species", (s32)MATERIA_LOCUS_INDEX },
    /* clausula continens */
    { "pater", (s32)MATERIA_LOCUS_INDEX },
    /* verbum-finitum, subiectum */
    { "umbrae", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* alterna (186..190) */
    /* nodus analysis socii (T33) */
    { "socius", (s32)MATERIA_LOCUS_REFERENTIA },
    /* DERIVATUM: regula petens */
    { "auctor", (s32)MATERIA_LOCUS_TOKEN },
    /* OratioAlternaCausa */
    { "causa", (s32)MATERIA_LOCUS_INDEX },
    /* permille fiduciae */
    { "fides", (s32)MATERIA_LOCUS_INDEX },
    /* DERIVATUM: regula victrix */
    { "victor", (s32)MATERIA_LOCUS_TOKEN },
};

hic_manens constans MateriaTabGenus GENERA_COCTA[] = {
    /* titulus, offset, numerus */
    { "documentum", (i32)0, (i32)3 },
    { "paragraphus", (i32)3, (i32)4 },
    { "sententia", (i32)7, (i32)2 },
    { "vocabulum", (i32)9, (i32)10 },
    { "interpunctio", (i32)19, (i32)4 },
    { "numerus", (i32)23, (i32)4 },
    { "analysis-substantivi", (i32)27, (i32)10 },
    { "analysis-nominis-proprii", (i32)37, (i32)10 },
    { "analysis-verbi", (i32)47, (i32)15 },
    { "analysis-auxiliaris", (i32)62, (i32)15 },
    { "analysis-adiectivi", (i32)77, (i32)11 },
    { "analysis-adverbii", (i32)88, (i32)7 },
    { "analysis-pronominis", (i32)95, (i32)10 },
    { "analysis-determinantis", (i32)105, (i32)9 },
    { "analysis-adpositionis", (i32)114, (i32)7 },
    { "analysis-numeralis", (i32)121, (i32)10 },
    { "analysis-coniunctionis-coordinantis", (i32)131, (i32)6 },
    { "analysis-coniunctionis-subordinantis", (i32)137, (i32)6 },
    { "analysis-particulae", (i32)143, (i32)6 },
    { "analysis-interiectionis", (i32)149, (i32)6 },
    { "analysis-symboli", (i32)155, (i32)6 },
    { "analysis-interpunctionis", (i32)161, (i32)6 },
    { "analysis-ignoti", (i32)167, (i32)6 },
    { "umbra", (i32)173, (i32)9 },
    { "clausula", (i32)182, (i32)4 },
    { "alterna", (i32)186, (i32)5 },
};

constans MateriaRegistrumCoctum ORATIO_REGISTRUM = {
    GENERA_COCTA,
    (i32)(magnitudo(GENERA_COCTA) / magnitudo(GENERA_COCTA[0])),
    LOCI_COCTI,
    (i32)(magnitudo(LOCI_COCTI) / magnitudo(LOCI_COCTI[0]))
};
