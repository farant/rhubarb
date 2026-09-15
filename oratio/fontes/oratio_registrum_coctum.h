/* oratio_registrum_coctum.h
 *
 * Registrum generum COCTUM grammaticae 'oratio' - GENERATUM, NE MANU
 * EDITES. Fons: oratio/grammatica/oratio.registrum.stml
 * (materia/coquere.sh). Genera XXVI, loci CXCI.
 */

#ifndef ORATIO_REGISTRUM_COCTUM_H
#define ORATIO_REGISTRUM_COCTUM_H

#include "latina.h"
#include "materia_registrum.h"

nomen enumeratio {
    ORATIO_GENUS_DOCUMENTUM = 0,
    ORATIO_GENUS_PARAGRAPHUS,
    ORATIO_GENUS_SENTENTIA,
    ORATIO_GENUS_VOCABULUM,
    ORATIO_GENUS_INTERPUNCTIO,
    ORATIO_GENUS_NUMERUS,
    ORATIO_GENUS_ANALYSIS_SUBSTANTIVI,
    ORATIO_GENUS_ANALYSIS_NOMINIS_PROPRII,
    ORATIO_GENUS_ANALYSIS_VERBI,
    ORATIO_GENUS_ANALYSIS_AUXILIARIS,
    ORATIO_GENUS_ANALYSIS_ADIECTIVI,
    ORATIO_GENUS_ANALYSIS_ADVERBII,
    ORATIO_GENUS_ANALYSIS_PRONOMINIS,
    ORATIO_GENUS_ANALYSIS_DETERMINANTIS,
    ORATIO_GENUS_ANALYSIS_ADPOSITIONIS,
    ORATIO_GENUS_ANALYSIS_NUMERALIS,
    ORATIO_GENUS_ANALYSIS_CONIUNCTIONIS_COORDINANTIS,
    ORATIO_GENUS_ANALYSIS_CONIUNCTIONIS_SUBORDINANTIS,
    ORATIO_GENUS_ANALYSIS_PARTICULAE,
    ORATIO_GENUS_ANALYSIS_INTERIECTIONIS,
    ORATIO_GENUS_ANALYSIS_SYMBOLI,
    ORATIO_GENUS_ANALYSIS_INTERPUNCTIONIS,
    ORATIO_GENUS_ANALYSIS_IGNOTI,
    /* T19d (2026-09-07): dependens exspectatus lectionis (relatio,
     * condiciones, impletio); APPENSA post analyses, ergo analyses
     * [PRIMUM, ULTIMUM) contiguae manent */
    ORATIO_GENUS_UMBRA,
    /* T20a (2026-09-08, decisiones XLIII-XLIV): capsa sudoku inter
     * vicinitatem et sententiam; nodus in lista clausulae
     * sententiae, umbras e schemate ferens; APPENSA post umbram,
     * extra intervallum analysium */
    ORATIO_GENUS_CLAUSULA,
    /* T32 b (2026-09-09): petitio umbrae cedens (socius, auctor,
     * causa, fides, victor); T33 socius REFERENTIA */
    ORATIO_GENUS_ALTERNA,

    ORATIO_GENUS_NUMERUS_GENERUM
} OratioGenus;

externus constans MateriaRegistrumCoctum ORATIO_REGISTRUM;

#endif /* ORATIO_REGISTRUM_COCTUM_H */
