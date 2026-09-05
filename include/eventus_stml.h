/* eventus_stml.h - Eventus[] <-> STML: plagulae replay
 *
 * Forma:
 *   <eventus_index>
 *     <eventus genus="mus_depressus" tempus="1000" x="10" y="20"
 *              botton="0" modificantes="2"/>
 *     <eventus genus="clavis_depressus" tempus="1050" clavis="0"
 *              typus="112" modificantes="0"/>
 *     <eventus genus="mus_rotula" tempus="1100" delta_x="0.000"
 *              delta_y="-1.500"/>
 *     <eventus genus="mutare_magnitudinem" tempus="1200"
 *              latitudo="480" altitudo="300"/>
 *   </eventus_index>
 * typus scribitur ut numerus (codex characteris). tempus (s64)
 * scribitur per f64 sine decimalibus - exactum usque ad 2^53.
 */

#ifndef EVENTUS_STML_H
#define EVENTUS_STML_H

/* <aedilis corpus="lib/eventus_stml.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "fenestra.h"


/* ==================================================
 * TITULI GENERUM
 * ================================================== */

constans character*
eventus_genus_titulus (
    eventus_genus_t genus);

eventus_genus_t
eventus_genus_ex_titulo (
    constans character* titulus);


/* ==================================================
 * SERIALIZATIO
 * ================================================== */

chorda
eventus_scribere_stml (
           constans Xar* eventus,   /* Xar de Eventus */
                Piscina* piscina,
    InternamentumChorda* intern,
                    b32  pulchrum);

Xar*
eventus_legere_stml (
     constans character* cstr,
                Piscina* piscina,
    InternamentumChorda* intern);

#endif /* EVENTUS_STML_H */
