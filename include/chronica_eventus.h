/* chronica_eventus.h - eventa ex lineis paginae (chronica C1).
 * In pagina scribae (comes aut liber communis) Franus scribit:
 *   <event year="-1446" month="5" day="1" people="Moses; Aaron"
 *          place="Egypt" type="exodus">Moses ducit</event>
 * (aut clausura anonyma "</>"). Tag et attributa ANGLICE manent
 * (spec XIII.1); "event" minusculis solum.
 *   year   - necessarium; integer, negativus = a.C.; 0 invalidus
 *   month  - 1-12, optionale
 *   day    - 1-31, solum cum month
 *   people - nomina ';' divisa, tonsa; vacua omissa
 *   place, type - textus liber, tonsus
 * Attributa ignota neglecta. Eventum quodvis vitiosum (etiam mensis
 * aut dies) TOTUM vitium est, non eventum. Descriptio = textus inter
 * tag et clausuram, spatia (et fines linearum) in spatium unum
 * collapsa, extrema tonsa; '<' et '>' intus manent. Eventum per
 * lineas fluere potest. '<event' novum ante clausuram: prius vitium
 * est, novum legitur. Pura. */

#ifndef CHRONICA_EVENTUS_H
#define CHRONICA_EVENTUS_H

/* <aedilis corpus="lib/chronica_eventus.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

/* positiones: linea = index in Xar linearum (vacuae numerantur),
 * columna = octetus in linea; finis = octetus POST '>' clausurae */
nomen structura {
       s32  annus;          /* negativus = a.C.; numquam 0 */
       s32  mensis;         /* 0 = absens, 1-12 */
       s32  dies;           /* 0 = absens, 1-31 */
    chorda  descriptio;
       Xar* personae;       /* chorda, ordine scripto */
    chorda  locus;          /* place; vacua = absens */
    chorda  genus;          /* type; vacua = absens */
       i32  linea;          /* ubi '<event' incipit */
       i32  columna;
       i32  linea_finis;
       i32  columna_finis;
} ChronicaEventus;

/* eventum malum: ambitus eius (ut supra; non clausum: finis =
 * '<event' proximum aut finis textus) et causa Latina ("annus
 * deest", "annus invalidus", "mensis invalidus", "dies sine mense",
 * "dies invalidus", "non clausum") */
nomen structura {
       i32 linea;
       i32 columna;
       i32 linea_finis;
       i32 columna_finis;
    chorda causa;
} ChronicaVitium;

/* lineae: Xar de chorda, index = numerus lineae (vacuae servatae).
 * eventa (Xar de ChronicaEventus) et vitia (Xar de ChronicaVitium;
 * NIHIL licet) ordine textus ADDUNTUR; omnia in piscina copiata.
 * FALSUM solum si memoria deficit aut argumenta NIHIL. */
b32
chronica_eventa_legere (
    constans Xar* lineae,
         Piscina* piscina,
             Xar* eventa,
             Xar* vitia);

#endif /* CHRONICA_EVENTUS_H */
