/* oratio_registrum.c - Vocabularium nodorum orationis
 *
 * Vide oratio_registrum.h. Series LOCORUM plana; quodque genus
 * fenestram suam per loci_offset + loci_numerus nominat. Probatio
 * congruentiam enumerationis cum hac tabula per TITULOS asserit et
 * offsets CONTIGUOS.
 */

#include "oratio_registrum.h"
#include "materia_nodus.h"
#include <string.h>

hic_manens constans MateriaTabLocus LOCI_ORATIONIS[] = {
    /* documentum (0..2) */
    { "praefixa",            (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "paragraphi",          (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "finis",               (s32)MATERIA_LOCUS_TOKEN },

    /* paragraphus (3..6) */
    { "praefixa",            (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "sententiae",          (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "cauda",               (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "forma",               (s32)MATERIA_LOCUS_INDEX },   /* OratioForma (T6b) */

    /* sententia (7..8) */
    { "elementa",            (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "clausulae",           (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T20a: clausulae sententiae */

    /* vocabulum (9..17) */
    { "partes",              (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "cauda",               (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "analyses",            (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "classes",             (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "linguae",             (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "decisio",             (s32)MATERIA_LOCUS_INDEX },   /* OratioDecisio (T19g) */
    { "auctor",              (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM (T19g) */
    { "clausula",            (s32)MATERIA_LOCUS_INDEX },   /* T20a: clausula intra sententiam */
    { "clausula-causa",      (s32)MATERIA_LOCUS_INDEX },   /* OratioClausulaCausa (T20a) */
    { "habitus",             (s32)MATERIA_LOCUS_INDEX },   /* OratioHabitus (T38 c) */

    /* interpunctio (19..22) */
    { "signum",              (s32)MATERIA_LOCUS_TOKEN },
    { "cauda",               (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "clausula",            (s32)MATERIA_LOCUS_INDEX },   /* T20a */
    { "clausula-causa",      (s32)MATERIA_LOCUS_INDEX },   /* OratioClausulaCausa (T20a) */

    /* numerus (22..25) */
    { "crudum",              (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "cauda",               (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "clausula",            (s32)MATERIA_LOCUS_INDEX },   /* T20a */
    { "clausula-causa",      (s32)MATERIA_LOCUS_INDEX },   /* OratioClausulaCausa (T20a) */

    /* gradus III (T11): analysis-* APPENSA - loci communes V (lemma
     * lingua fons nativum sensus) deinde accidentia classis (INDEX) */
    /* analysis-substantivi (26..35) */
    { "lemma",               (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",              (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",                (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",             (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",              (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "casus",               (s32)MATERIA_LOCUS_INDEX },   /* OratioCasus */
    { "numerus",             (s32)MATERIA_LOCUS_INDEX },   /* OratioNumerusGrammaticus */
    { "genus",               (s32)MATERIA_LOCUS_INDEX },   /* OratioGenusGrammaticum */
    { "declinatio",          (s32)MATERIA_LOCUS_INDEX },   /* declinatio I-V (WORDS) */
    { "umbrae",              (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-nominis-proprii (36..45) */
    { "lemma",               (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",              (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",                (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",             (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",              (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "casus",               (s32)MATERIA_LOCUS_INDEX },   /* OratioCasus */
    { "numerus",             (s32)MATERIA_LOCUS_INDEX },   /* OratioNumerusGrammaticus */
    { "genus",               (s32)MATERIA_LOCUS_INDEX },   /* OratioGenusGrammaticum */
    { "declinatio",          (s32)MATERIA_LOCUS_INDEX },   /* declinatio I-V (WORDS) */
    { "umbrae",              (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-verbi (46..60) */
    { "lemma",               (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",              (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",                (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",             (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",              (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "persona",             (s32)MATERIA_LOCUS_INDEX },   /* OratioPersona */
    { "numerus",             (s32)MATERIA_LOCUS_INDEX },   /* OratioNumerusGrammaticus */
    { "tempus",              (s32)MATERIA_LOCUS_INDEX },   /* OratioTempus */
    { "modus",               (s32)MATERIA_LOCUS_INDEX },   /* OratioModus */
    { "vox",                 (s32)MATERIA_LOCUS_INDEX },   /* OratioVox */
    { "forma-verbi",         (s32)MATERIA_LOCUS_INDEX },   /* OratioFormaVerbi */
    { "coniugatio",          (s32)MATERIA_LOCUS_INDEX },   /* coniugatio I-IV (WORDS) */
    { "casus",               (s32)MATERIA_LOCUS_INDEX },   /* OratioCasus */
    { "genus",               (s32)MATERIA_LOCUS_INDEX },   /* OratioGenusGrammaticum */
    { "umbrae",              (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-auxiliaris (61..75) */
    { "lemma",               (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",              (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",                (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",             (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",              (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "persona",             (s32)MATERIA_LOCUS_INDEX },   /* OratioPersona */
    { "numerus",             (s32)MATERIA_LOCUS_INDEX },   /* OratioNumerusGrammaticus */
    { "tempus",              (s32)MATERIA_LOCUS_INDEX },   /* OratioTempus */
    { "modus",               (s32)MATERIA_LOCUS_INDEX },   /* OratioModus */
    { "vox",                 (s32)MATERIA_LOCUS_INDEX },   /* OratioVox */
    { "forma-verbi",         (s32)MATERIA_LOCUS_INDEX },   /* OratioFormaVerbi */
    { "coniugatio",          (s32)MATERIA_LOCUS_INDEX },   /* coniugatio I-IV (WORDS) */
    { "casus",               (s32)MATERIA_LOCUS_INDEX },   /* OratioCasus */
    { "genus",               (s32)MATERIA_LOCUS_INDEX },   /* OratioGenusGrammaticum */
    { "umbrae",              (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-adiectivi (76..86) */
    { "lemma",               (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",              (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",                (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",             (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",              (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "casus",               (s32)MATERIA_LOCUS_INDEX },   /* OratioCasus */
    { "numerus",             (s32)MATERIA_LOCUS_INDEX },   /* OratioNumerusGrammaticus */
    { "genus",               (s32)MATERIA_LOCUS_INDEX },   /* OratioGenusGrammaticum */
    { "gradus",              (s32)MATERIA_LOCUS_INDEX },   /* OratioGradus */
    { "declinatio",          (s32)MATERIA_LOCUS_INDEX },   /* declinatio I-V (WORDS) */
    { "umbrae",              (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-adverbii (87..93) */
    { "lemma",               (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",              (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",                (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",             (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",              (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "gradus",              (s32)MATERIA_LOCUS_INDEX },   /* OratioGradus */
    { "umbrae",              (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-pronominis (94..103) */
    { "lemma",               (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",              (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",                (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",             (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",              (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "casus",               (s32)MATERIA_LOCUS_INDEX },   /* OratioCasus */
    { "numerus",             (s32)MATERIA_LOCUS_INDEX },   /* OratioNumerusGrammaticus */
    { "genus",               (s32)MATERIA_LOCUS_INDEX },   /* OratioGenusGrammaticum */
    { "persona",             (s32)MATERIA_LOCUS_INDEX },   /* OratioPersona */
    { "umbrae",              (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-determinantis (104..112) */
    { "lemma",               (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",              (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",                (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",             (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",              (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "casus",               (s32)MATERIA_LOCUS_INDEX },   /* OratioCasus */
    { "numerus",             (s32)MATERIA_LOCUS_INDEX },   /* OratioNumerusGrammaticus */
    { "genus",               (s32)MATERIA_LOCUS_INDEX },   /* OratioGenusGrammaticum */
    { "umbrae",              (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-adpositionis (113..119) */
    { "lemma",               (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",              (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",                (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",             (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",              (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "casus",               (s32)MATERIA_LOCUS_INDEX },   /* OratioCasus */
    { "umbrae",              (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-numeralis (120..129) */
    { "lemma",               (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",              (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",                (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",             (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",              (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "casus",               (s32)MATERIA_LOCUS_INDEX },   /* OratioCasus */
    { "numerus",             (s32)MATERIA_LOCUS_INDEX },   /* OratioNumerusGrammaticus */
    { "genus",               (s32)MATERIA_LOCUS_INDEX },   /* OratioGenusGrammaticum */
    { "species",             (s32)MATERIA_LOCUS_INDEX },   /* OratioSpeciesNumeralis */
    { "umbrae",              (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-coniunctionis-coordinantis (130..135) */
    { "lemma",               (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",              (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",                (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",             (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",              (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "umbrae",              (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-coniunctionis-subordinantis (136..141) */
    { "lemma",               (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",              (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",                (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",             (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",              (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "umbrae",              (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-particulae (142..147) */
    { "lemma",               (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",              (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",                (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",             (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",              (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "umbrae",              (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-interiectionis (148..153) */
    { "lemma",               (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",              (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",                (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",             (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",              (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "umbrae",              (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-symboli (154..159) */
    { "lemma",               (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",              (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",                (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",             (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",              (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "umbrae",              (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-interpunctionis (160..165) */
    { "lemma",               (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",              (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",                (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",             (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",              (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "umbrae",              (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-ignoti (166..171) */
    { "lemma",               (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",              (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",                (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",             (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",              (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "umbrae",              (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* umbra - T19d: dependens exspectatus lectionis; forma-verbi T20a */
    /* umbra (172..180) - T32 a: auctor; T32 b: alternae; T33
     * (2026-09-10): impletio REFERENTIA una pro ordinalibus duobus */
    { "relatio",             (s32)MATERIA_LOCUS_INDEX },   /* OratioRelatio */
    { "classis",             (s32)MATERIA_LOCUS_INDEX },   /* OratioClassis exspectata */
    { "casus",               (s32)MATERIA_LOCUS_INDEX },   /* OratioCasus */
    { "numerus",             (s32)MATERIA_LOCUS_INDEX },   /* OratioNumerusGrammaticus */
    { "genus",               (s32)MATERIA_LOCUS_INDEX },   /* OratioGenusGrammaticum */
    { "impletio",            (s32)MATERIA_LOCUS_REFERENTIA },   /* nodus analysis implentis (T33) */
    { "forma-verbi",         (s32)MATERIA_LOCUS_INDEX },   /* OratioFormaVerbi exspectata (T20a) */
    { "auctor",              (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM: titulus regulae implentis (T32 a) */
    { "alternae",            (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T32 b: petitiones cedentes (nodi alterna) */

    /* clausula - T20a (2026-09-08): capsa sudoku, nodus in sententia */
    /* clausula (181..184) */
    { "semen",               (s32)MATERIA_LOCUS_INDEX },   /* ordinalis elementi seminis (T20a) */
    { "species",             (s32)MATERIA_LOCUS_INDEX },   /* OratioSpeciesClausulae */
    { "pater",               (s32)MATERIA_LOCUS_INDEX },   /* clausula continens */
    { "umbrae",              (s32)MATERIA_LOCUS_LISTA_NODUS },   /* verbum-finitum, subiectum */

    /* alterna - T32 b (2026-09-09): petitio umbrae cedens */
    /* alterna (185..189) - T33: socius REFERENTIA pro ordinalibus */
    { "socius",              (s32)MATERIA_LOCUS_REFERENTIA },   /* nodus analysis socii (T33) */
    { "auctor",              (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM: regula petens */
    { "causa",               (s32)MATERIA_LOCUS_INDEX },   /* OratioAlternaCausa */
    { "fides",               (s32)MATERIA_LOCUS_INDEX },   /* permille fiduciae */
    { "victor",              (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM: regula victrix */
};

hic_manens constans MateriaTabGenus GENERA_ORATIONIS[] = {
    /* titulus              offset   numerus */
    { "documentum",                                (i32)0, (i32)3 },
    { "paragraphus",                               (i32)3, (i32)4 },
    { "sententia",                                 (i32)7, (i32)2 },
    /* T38 c (2026-09-15): locus 'habitus' vocabuli appensus (IX -> X;
     * offsets sequentes +I, loci CXC -> CXCI) */
    { "vocabulum",                                 (i32)9, (i32)10 },
    { "interpunctio",                              (i32)19, (i32)4 },
    { "numerus",                                   (i32)23, (i32)4 },

    /* gradus III (T11): unum per classem universalem, ordine UD */
    { "analysis-substantivi",                      (i32)27, (i32)10 },
    { "analysis-nominis-proprii",                  (i32)37, (i32)10 },
    { "analysis-verbi",                            (i32)47, (i32)15 },
    { "analysis-auxiliaris",                       (i32)62, (i32)15 },
    { "analysis-adiectivi",                        (i32)77, (i32)11 },
    { "analysis-adverbii",                         (i32)88, (i32)7 },
    { "analysis-pronominis",                       (i32)95, (i32)10 },
    { "analysis-determinantis",                    (i32)105, (i32)9 },
    { "analysis-adpositionis",                     (i32)114, (i32)7 },
    { "analysis-numeralis",                        (i32)121, (i32)10 },
    { "analysis-coniunctionis-coordinantis",       (i32)131, (i32)6 },
    { "analysis-coniunctionis-subordinantis",      (i32)137, (i32)6 },
    { "analysis-particulae",                       (i32)143, (i32)6 },
    { "analysis-interiectionis",                   (i32)149, (i32)6 },
    { "analysis-symboli",                          (i32)155, (i32)6 },
    { "analysis-interpunctionis",                  (i32)161, (i32)6 },
    { "analysis-ignoti",                           (i32)167, (i32)6 },

    /* T19d (2026-09-07): UMBRA appensa post analyses; T20a (2026-09-08):
     * CLAUSULA appensa post umbram (loci CLXXII -> CLXXXIV); T32 a
     * (2026-09-09): locus 'auctor' umbrae appensus (CLXXXIV -> CLXXXV) */
    { "umbra",                                     (i32)173, (i32)9 },
    { "clausula",                                  (i32)182, (i32)4 },
    /* T32 b (2026-09-09): ALTERNA appensa post clausulam (CLXXXV -> CXCII);
     * T33 (2026-09-10): REFERENTIAE - umbra X -> IX, alterna VI -> V
     * (CXCII -> CXC) */
    { "alterna",                                   (i32)186, (i32)5 },
};

constans MateriaRegistrumCoctum ORATIO_REGISTRUM = {
    GENERA_ORATIONIS,
    (i32)(magnitudo(GENERA_ORATIONIS) / magnitudo(GENERA_ORATIONIS[0])),
    LOCI_ORATIONIS,
    (i32)(magnitudo(LOCI_ORATIONIS) / magnitudo(LOCI_ORATIONIS[0]))
};


/* ==================================================
 * Gradus III (T11): tituli enumerationum et classes
 * ================================================== */

constans character* constans ORATIO_TITULI_CLASSIUM[] = {
    "substantivum", "nomen-proprium", "verbum", "auxiliare",
        "adiectivum", "adverbium", "pronomen", "determinans",
            "adpositio", "numerale", "coniunctio-coordinans",
                "coniunctio-subordinans", "particula", "interiectio",
                    "symbolum", "interpunctio", "ignotum"
};
constans character* constans ORATIO_TITULI_CASUUM[] = {
    "nominativus", "genitivus", "dativus", "accusativus", "ablativus",
        "locativus", "vocativus"
};
constans character* constans ORATIO_TITULI_NUMERORUM[] = {
    "singularis", "pluralis", "dualis"
};
constans character* constans ORATIO_TITULI_GENERUM_GRAMMATICORUM[] = {
    "masculinum", "femininum", "neutrum", "commune"
};
constans character* constans ORATIO_TITULI_PERSONARUM[] = {
    "I", "II", "III"
};
constans character* constans ORATIO_TITULI_TEMPORUM[] = {
    "praesens", "imperfectum", "futurum", "perfectum",
        "plusquamperfectum", "futurum-exactum", "praeteritum"
};
constans character* constans ORATIO_TITULI_MODORUM[] = {
    "indicativus", "subiunctivus", "imperativus", "infinitivus"
};
constans character* constans ORATIO_TITULI_VOCUM[] = {
    "activa", "passiva", "deponens"
};
constans character* constans ORATIO_TITULI_FORMARUM_VERBI[] = {
    "finitum", "infinitivum", "participium", "gerundium", "gerundivum",
        "supinum"
};
constans character* constans ORATIO_TITULI_GRADUUM[] = {
    "positivus", "comparativus", "superlativus"
};
constans character* constans ORATIO_TITULI_SPECIERUM_NUMERALIS[] = {
    "cardinale", "ordinale", "distributivum", "adverbiale"
};
constans character* constans ORATIO_TITULI_LINGUARUM[] = {
    "latina", "anglica"
};
/* T22 b: dialectus documenti (index = valor) */
constans character* constans ORATIO_TITULI_DIALECTORUM[] = {
    "ignotus", "classicus", "medius"
};
constans character* constans ORATIO_TITULI_RELATIONUM[] = {
    "obiectum", "caput", "verbum-finitum", "subiectum", "obiectum-verbi"
};
constans character* constans ORATIO_TITULI_DECISIONUM[] = {
    "praelatio", "impletio", "umbra", "decretum"
};

/* T38 c: habitus cellulae decretoris = tituli reticuli (decisio LVI) */
constans character* constans ORATIO_TITULI_HABITUUM[] = {
    "coactus", "ordinatus", "apertus"
};
/* T20a: species et causae clausularum (index = valor) */
constans character* constans ORATIO_TITULI_SPECIERUM_CLAUSULAE[] = {
    "principalis", "subordinata", "relativa", "coordinata",
        "parenthetica"
};
constans character* constans ORATIO_TITULI_CAUSARUM_CLAUSULAE[] = {
    "semen", "extentum", "clausura", "catena", "unica", "verbum"
};
constans character* constans ORATIO_TITULI_CAUSARUM_ALTERNAE[] = {
    "recusata", "revocata", "praeoccupata", "decreta"
};
constans character* constans ORATIO_TITULI_FONTIUM_ANALYSIS[] = {
    "vocabularium-la", "vocabularium-en", "glossarium", "regula",
    "orthographia"
};

s32
oratio_locus_clausulae (
    OratioGenus genus,
            b32 causa)
{
    commutatio ((i32)genus)
    {
        casus (i32)ORATIO_GENUS_VOCABULUM:
            redde causa ? (s32)ORATIO_VOCABULUM_CLAUSULA_CAUSA
                : (s32)ORATIO_VOCABULUM_CLAUSULA;
        casus (i32)ORATIO_GENUS_INTERPUNCTIO:
            redde causa ? (s32)ORATIO_INTERPUNCTIO_CLAUSULA_CAUSA
                : (s32)ORATIO_INTERPUNCTIO_CLAUSULA;
        casus (i32)ORATIO_GENUS_NUMERUS:
            redde causa ? (s32)ORATIO_NUMERUS_CLAUSULA_CAUSA
                : (s32)ORATIO_NUMERUS_CLAUSULA;
        ordinarius:
            redde (s32)-I;
    }
}

constans character*
oratio_classis_titulus (
    OratioClassis classis)
{
    si ((i32)classis >= (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM)
    {
        redde NIHIL;
    }
    redde ORATIO_TITULI_CLASSIUM[classis];
}

OratioClassis
oratio_classis_ex_titulo (
    constans character* titulus,
                   i32  mensura)
{
    i32 i;

    per (i = ZEPHYRUM; i < (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM; i++)
    {
        constans character* t = ORATIO_TITULI_CLASSIUM[i];

        si (   (i32)strlen(t)                      == mensura
            && memcmp(t, titulus, (size_t)mensura) == ZEPHYRUM)
        {
            redde (OratioClassis)i;
        }
    }
    redde ORATIO_CLASSIS_NUMERUS_CLASSIUM;
}

OratioGenus
oratio_classis_genus (
    OratioClassis classis)
{
    redde (OratioGenus)((i32)ORATIO_GENUS_ANALYSIS_PRIMUM
        + (i32)classis);
}

OratioClassis
oratio_genus_classis (
    OratioGenus genus)
{
    si (   (i32)genus < (i32)ORATIO_GENUS_ANALYSIS_PRIMUM
        || (i32)genus >= (i32)ORATIO_GENUS_ANALYSIS_ULTIMUM)
    {
        redde ORATIO_CLASSIS_NUMERUS_CLASSIUM;   /* umbra quoque */
    }
    redde (OratioClassis)((i32)genus
        - (i32)ORATIO_GENUS_ANALYSIS_PRIMUM);
}
