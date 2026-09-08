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
    { "praefixa",    (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "paragraphi",  (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "finis",       (s32)MATERIA_LOCUS_TOKEN },

    /* paragraphus (3..6) */
    { "praefixa",    (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "sententiae",  (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "cauda",       (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "forma",       (s32)MATERIA_LOCUS_INDEX },      /* OratioForma (T6b) */

    /* sententia (7) */
    { "elementa",    (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* vocabulum (8..12) */
    { "partes",      (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "cauda",       (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "analyses",    (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "classes",     (s32)MATERIA_LOCUS_TOKEN },      /* DERIVATUM */
    { "linguae",     (s32)MATERIA_LOCUS_TOKEN },      /* DERIVATUM */
    /* T19g (2026-09-08, decretum SUDOKU): decisio vocabuli -
     * quo genere lectio prima facta sit (INDEX OratioDecisio, non
     * scripta = nemo decidit) et auctor eius (titulus regulae) */
    { "decisio",     (s32)MATERIA_LOCUS_INDEX },      /* OratioDecisio */
    { "auctor",      (s32)MATERIA_LOCUS_TOKEN },      /* DERIVATUM */

    /* interpunctio (15..16) */
    { "signum",      (s32)MATERIA_LOCUS_TOKEN },
    { "cauda",       (s32)MATERIA_LOCUS_LISTA_TOKEN },

        /* numerus (17..18) */
    { "crudum",      (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "cauda",       (s32)MATERIA_LOCUS_LISTA_TOKEN },

    /* gradus III (T11): analysis-* APPENSA - loci communes V (lemma
     * lingua fons nativum sensus) deinde accidentia classis (INDEX) */
    /* analysis-substantivi (19..28) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "casus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioCasus */
    { "numerus",        (s32)MATERIA_LOCUS_INDEX },   /* OratioNumerusGrammaticus */
    { "genus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioGenusGrammaticum */
    { "declinatio",     (s32)MATERIA_LOCUS_INDEX },   /* declinatio I-V (WORDS) */
    { "umbrae",         (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-nominis-proprii (29..38) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "casus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioCasus */
    { "numerus",        (s32)MATERIA_LOCUS_INDEX },   /* OratioNumerusGrammaticus */
    { "genus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioGenusGrammaticum */
    { "declinatio",     (s32)MATERIA_LOCUS_INDEX },   /* declinatio I-V (WORDS) */
    { "umbrae",         (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-verbi (39..53) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "persona",        (s32)MATERIA_LOCUS_INDEX },   /* OratioPersona */
    { "numerus",        (s32)MATERIA_LOCUS_INDEX },   /* OratioNumerusGrammaticus */
    { "tempus",         (s32)MATERIA_LOCUS_INDEX },   /* OratioTempus */
    { "modus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioModus */
    { "vox",            (s32)MATERIA_LOCUS_INDEX },   /* OratioVox */
    { "forma-verbi",    (s32)MATERIA_LOCUS_INDEX },   /* OratioFormaVerbi */
    { "coniugatio",     (s32)MATERIA_LOCUS_INDEX },   /* coniugatio I-IV (WORDS) */
    { "casus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioCasus */
    { "genus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioGenusGrammaticum */
    { "umbrae",         (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-auxiliaris (54..68) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "persona",        (s32)MATERIA_LOCUS_INDEX },   /* OratioPersona */
    { "numerus",        (s32)MATERIA_LOCUS_INDEX },   /* OratioNumerusGrammaticus */
    { "tempus",         (s32)MATERIA_LOCUS_INDEX },   /* OratioTempus */
    { "modus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioModus */
    { "vox",            (s32)MATERIA_LOCUS_INDEX },   /* OratioVox */
    { "forma-verbi",    (s32)MATERIA_LOCUS_INDEX },   /* OratioFormaVerbi */
    { "coniugatio",     (s32)MATERIA_LOCUS_INDEX },   /* coniugatio I-IV (WORDS) */
    { "casus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioCasus */
    { "genus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioGenusGrammaticum */
    { "umbrae",         (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-adiectivi (69..79) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "casus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioCasus */
    { "numerus",        (s32)MATERIA_LOCUS_INDEX },   /* OratioNumerusGrammaticus */
    { "genus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioGenusGrammaticum */
    { "gradus",         (s32)MATERIA_LOCUS_INDEX },   /* OratioGradus */
    { "declinatio",     (s32)MATERIA_LOCUS_INDEX },   /* declinatio I-V (WORDS) */
    { "umbrae",         (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-adverbii (80..86) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "gradus",         (s32)MATERIA_LOCUS_INDEX },   /* OratioGradus */
    { "umbrae",         (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-pronominis (87..96) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "casus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioCasus */
    { "numerus",        (s32)MATERIA_LOCUS_INDEX },   /* OratioNumerusGrammaticus */
    { "genus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioGenusGrammaticum */
    { "persona",        (s32)MATERIA_LOCUS_INDEX },   /* OratioPersona */
    { "umbrae",         (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-determinantis (97..105) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "casus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioCasus */
    { "numerus",        (s32)MATERIA_LOCUS_INDEX },   /* OratioNumerusGrammaticus */
    { "genus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioGenusGrammaticum */
    { "umbrae",         (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-adpositionis (106..112) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "casus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioCasus */
    { "umbrae",         (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-numeralis (113..122) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "casus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioCasus */
    { "numerus",        (s32)MATERIA_LOCUS_INDEX },   /* OratioNumerusGrammaticus */
    { "genus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioGenusGrammaticum */
    { "species",        (s32)MATERIA_LOCUS_INDEX },   /* OratioSpeciesNumeralis */
    { "umbrae",         (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-coniunctionis-coordinantis (123..128) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "umbrae",         (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-coniunctionis-subordinantis (129..134) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "umbrae",         (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-particulae (135..140) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "umbrae",         (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-interiectionis (141..146) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "umbrae",         (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-symboli (147..152) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "umbrae",         (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-interpunctionis (153..158) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "umbrae",         (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* analysis-ignoti (159..164) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "umbrae",         (s32)MATERIA_LOCUS_LISTA_NODUS },   /* T19d: umbrae lectionis */

    /* umbra (165..171) - T19d: dependens exspectatus lectionis */
    { "relatio",            (s32)MATERIA_LOCUS_INDEX },   /* OratioRelatio */
    { "classis",            (s32)MATERIA_LOCUS_INDEX },   /* OratioClassis exspectata */
    { "casus",              (s32)MATERIA_LOCUS_INDEX },   /* OratioCasus */
    { "numerus",            (s32)MATERIA_LOCUS_INDEX },   /* OratioNumerusGrammaticus */
    { "genus",              (s32)MATERIA_LOCUS_INDEX },   /* OratioGenusGrammaticum */
    { "impletio-vocabulum", (s32)MATERIA_LOCUS_INDEX },   /* ordinalis vocabuli implentis */
    { "impletio-analysis",  (s32)MATERIA_LOCUS_INDEX },   /* ordinalis analysis implentis */
};

hic_manens constans MateriaTabGenus GENERA_ORATIONIS[] = {
    /* titulus              offset   numerus */
    { "documentum",        (i32)0,  (i32)3 },
    { "paragraphus",       (i32)3,  (i32)4 },
    { "sententia",         (i32)7,  (i32)1 },
    { "vocabulum",         (i32)8,  (i32)7 },
        { "interpunctio",      (i32)15, (i32)2 },
    { "numerus",           (i32)17, (i32)2 },

    /* gradus III (T11): unum per classem universalem, ordine UD */
    { "analysis-substantivi",                     (i32)19, (i32)10 },
    { "analysis-nominis-proprii",                 (i32)29, (i32)10 },
    { "analysis-verbi",                           (i32)39, (i32)15 },
    { "analysis-auxiliaris",                      (i32)54, (i32)15 },
    { "analysis-adiectivi",                       (i32)69, (i32)11 },
    { "analysis-adverbii",                        (i32)80, (i32)7 },
    { "analysis-pronominis",                      (i32)87, (i32)10 },
    { "analysis-determinantis",                   (i32)97, (i32)9 },
    { "analysis-adpositionis",                    (i32)106, (i32)7 },
    { "analysis-numeralis",                       (i32)113, (i32)10 },
    { "analysis-coniunctionis-coordinantis",      (i32)123, (i32)6 },
    { "analysis-coniunctionis-subordinantis",     (i32)129, (i32)6 },
    { "analysis-particulae",                      (i32)135, (i32)6 },
    { "analysis-interiectionis",                  (i32)141, (i32)6 },
    { "analysis-symboli",                         (i32)147, (i32)6 },
    { "analysis-interpunctionis",                 (i32)153, (i32)6 },
    { "analysis-ignoti",                          (i32)159, (i32)6 },

    /* T19d (2026-09-07): UMBRA appensa post analyses */
    { "umbra",                                   (i32)165, (i32)7 }
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
constans character* constans ORATIO_TITULI_RELATIONUM[] = {
    "obiectum", "caput"
};
constans character* constans ORATIO_TITULI_DECISIONUM[] = {
    "praelatio", "impletio", "umbra"
};
constans character* constans ORATIO_TITULI_FONTIUM_ANALYSIS[] = {
    "vocabularium-la", "vocabularium-en", "glossarium", "regula"
};

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
