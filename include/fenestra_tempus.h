#ifndef FENESTRA_TEMPUS_H
#define FENESTRA_TEMPUS_H

/* <aedilis nexus="purus"/> - clausura sine regula nexus (nulla
 * framework; eventus A1b: bin/aedilis --nexus-purus) */

/* fenestra_tempus.h - Horologium platformae: pulsus, frequentia, ms,
 * dormire. Ex fenestra.h divisum (eventus A3a, 2026-10-01):
 * implementatio mach/POSIX pura (lib/fenestra_tempus_macos.c, regula
 * variantium), sine Cocoa. Olim lib/tempus.c fenestra_macos.m TOTAM per
 * annotationem obiecti nectebat ut horologium legeret - et obiectum
 * annotatum non ambulatur, ergo dependentiae novae fenestrae ibi
 * deerant. fenestra.h hoc includit: vocantes nihil mutatum vident.
 */

#include "latina.h"


/* ==================================================
 * Functiones Temporis Platformae
 * ================================================== */

/* Obtinere pulsus temporis ad altam praecisionem
 *
 * Reddit: numerus pulsuum ex tempore arbitrario
 */
i64
fenestra_tempus_obtinere_pulsus (
    vacuum);

/* Obtinere frequentiam horologii
 *
 * Reddit: pulsus per secundum
 */
f64
fenestra_tempus_obtinere_frequentiam (
    vacuum);

/* Tempus currens in millisecundis - pulsus * M / frequentia. Sedes
 * UNICA horologii pro eventibus (ludus: tempus est datum in eventu).
 *
 * Reddit: millisecundae ex tempore arbitrario (eodem ac pulsus)
 */
s64
fenestra_tempus_ms (
    vacuum);

/* Dormire pro microsecundis datis
 *
 * microsecundae: numerus microsecundarum dormire
 */
vacuum
fenestra_dormire (
    i32 microsecundae);

#endif /* FENESTRA_TEMPUS_H */
