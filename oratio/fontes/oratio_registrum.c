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

    /* interpunctio (13..14) */
    { "signum",      (s32)MATERIA_LOCUS_TOKEN },
    { "cauda",       (s32)MATERIA_LOCUS_LISTA_TOKEN },

        /* numerus (15..16) */
    { "crudum",      (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "cauda",       (s32)MATERIA_LOCUS_LISTA_TOKEN },

    /* gradus III (T11): analysis-* APPENSA - loci communes V (lemma
     * lingua fons nativum sensus) deinde accidentia classis (INDEX) */
    /* analysis-substantivi (17..25) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "casus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioCasus */
    { "numerus",        (s32)MATERIA_LOCUS_INDEX },   /* OratioNumerusGrammaticus */
    { "genus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioGenusGrammaticum */
    { "declinatio",     (s32)MATERIA_LOCUS_INDEX },   /* declinatio I-V (WORDS) */

    /* analysis-nominis-proprii (26..34) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "casus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioCasus */
    { "numerus",        (s32)MATERIA_LOCUS_INDEX },   /* OratioNumerusGrammaticus */
    { "genus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioGenusGrammaticum */
    { "declinatio",     (s32)MATERIA_LOCUS_INDEX },   /* declinatio I-V (WORDS) */

    /* analysis-verbi (35..48) */
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

    /* analysis-auxiliaris (49..62) */
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

    /* analysis-adiectivi (63..72) */
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

    /* analysis-adverbii (73..78) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "gradus",         (s32)MATERIA_LOCUS_INDEX },   /* OratioGradus */

    /* analysis-pronominis (79..87) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "casus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioCasus */
    { "numerus",        (s32)MATERIA_LOCUS_INDEX },   /* OratioNumerusGrammaticus */
    { "genus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioGenusGrammaticum */
    { "persona",        (s32)MATERIA_LOCUS_INDEX },   /* OratioPersona */

    /* analysis-determinantis (88..95) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "casus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioCasus */
    { "numerus",        (s32)MATERIA_LOCUS_INDEX },   /* OratioNumerusGrammaticus */
    { "genus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioGenusGrammaticum */

    /* analysis-adpositionis (96..101) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "casus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioCasus */

    /* analysis-numeralis (102..110) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "casus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioCasus */
    { "numerus",        (s32)MATERIA_LOCUS_INDEX },   /* OratioNumerusGrammaticus */
    { "genus",          (s32)MATERIA_LOCUS_INDEX },   /* OratioGenusGrammaticum */
    { "species",        (s32)MATERIA_LOCUS_INDEX },   /* OratioSpeciesNumeralis */

    /* analysis-coniunctionis-coordinantis (111..115) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */

    /* analysis-coniunctionis-subordinantis (116..120) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */

    /* analysis-particulae (121..125) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */

    /* analysis-interiectionis (126..130) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */

    /* analysis-symboli (131..135) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */

    /* analysis-interpunctionis (136..140) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */

    /* analysis-ignoti (141..145) */
    { "lemma",          (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "lingua",         (s32)MATERIA_LOCUS_INDEX },   /* OratioLingua */
    { "fons",           (s32)MATERIA_LOCUS_INDEX },   /* OratioFonsAnalysis */
    { "nativum",        (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
    { "sensus",         (s32)MATERIA_LOCUS_TOKEN },   /* DERIVATUM */
};

hic_manens constans MateriaTabGenus GENERA_ORATIONIS[] = {
    /* titulus              offset   numerus */
    { "documentum",        (i32)0,  (i32)3 },
    { "paragraphus",       (i32)3,  (i32)4 },
    { "sententia",         (i32)7,  (i32)1 },
    { "vocabulum",         (i32)8,  (i32)5 },
        { "interpunctio",      (i32)13, (i32)2 },
    { "numerus",           (i32)15, (i32)2 },

    /* gradus III (T11): unum per classem universalem, ordine UD */
    { "analysis-substantivi",                    (i32)17, (i32)9 },
    { "analysis-nominis-proprii",                (i32)26, (i32)9 },
    { "analysis-verbi",                          (i32)35, (i32)14 },
    { "analysis-auxiliaris",                     (i32)49, (i32)14 },
    { "analysis-adiectivi",                      (i32)63, (i32)10 },
    { "analysis-adverbii",                       (i32)73, (i32)6 },
    { "analysis-pronominis",                     (i32)79, (i32)9 },
    { "analysis-determinantis",                  (i32)88, (i32)8 },
    { "analysis-adpositionis",                   (i32)96, (i32)6 },
    { "analysis-numeralis",                      (i32)102, (i32)9 },
    { "analysis-coniunctionis-coordinantis",     (i32)111, (i32)5 },
    { "analysis-coniunctionis-subordinantis",    (i32)116, (i32)5 },
    { "analysis-particulae",                     (i32)121, (i32)5 },
    { "analysis-interiectionis",                 (i32)126, (i32)5 },
    { "analysis-symboli",                        (i32)131, (i32)5 },
    { "analysis-interpunctionis",                (i32)136, (i32)5 },
    { "analysis-ignoti",                         (i32)141, (i32)5 }
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
constans character* constans ORATIO_TITULI_FONTIUM_ANALYSIS[] = {
    "vocabularium-la", "vocabularium-en", "glossarium"
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
        || (i32)genus >= (i32)ORATIO_GENUS_NUMERUS_GENERUM)
    {
        redde ORATIO_CLASSIS_NUMERUS_CLASSIUM;
    }
    redde (OratioClassis)((i32)genus
        - (i32)ORATIO_GENUS_ANALYSIS_PRIMUM);
}
