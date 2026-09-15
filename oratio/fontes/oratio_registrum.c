/* oratio_registrum.c - Vocabularium nodorum orationis
 *
 * Vide oratio_registrum.h. Tabulae (LOCI, GENERA, ORATIO_REGISTRUM)
 * in oratio_registrum_coctum.c GENERATAE ex oratio.registrum.stml
 * (2026-09-15); hic tituli enumerationum et functiones classium.
 */

#include "oratio_registrum.h"
#include "materia_nodus.h"
#include <string.h>


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
