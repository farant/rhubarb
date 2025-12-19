#ifndef SPUTNIK_SYNTAXIS_H
#define SPUTNIK_SYNTAXIS_H

#include "latina.h"
#include "piscina.h"
#include "fenestra.h"

/* ==================================================
 * SPUTNIK SYNTAXIS - Syntax Reference Widget
 *
 * Widget static pro visualizare syntaxis linguae Sputnik.
 * Monstrat verba clausa, literae speciales, REPO, et
 * methodi entitatis.
 *
 * PHILOSOPHIA:
 * - Read-only viewer, non interactivus
 * - Focus in elementis Sputnik-specificis
 * - Non monstrat syntaxis JavaScript-similis
 *
 * EXEMPLUM:
 *   SputnikSyntaxis* syn = sputnik_syntaxis_creare(piscina);
 *   sputnik_syntaxis_reddere(syn, tabula, x, y, lat, alt, scala, focused);
 *
 * ================================================== */


/* ==================================================
 * Typi
 * ================================================== */

/* SputnikSyntaxis - Widget state */
nomen structura {
    Piscina* piscina;
} SputnikSyntaxis;


/* ==================================================
 * Creatio
 * ================================================== */

/* Creare syntax viewer
 *
 * piscina: piscina memoriae
 *
 * Redde: SputnikSyntaxis* si successus, NIHIL si error
 */
SputnikSyntaxis*
sputnik_syntaxis_creare(
    Piscina* piscina);


/* ==================================================
 * Reddere
 * ================================================== */

/* Reddere syntax viewer
 *
 * syntaxis: syntax viewer
 * tabula: tabula pixelorum
 * x, y: positio in tabula
 * latitudo, altitudo: dimensiones widget
 * scala: factor scalae fontis
 * focused: an widget habet focus
 */
vacuum
sputnik_syntaxis_reddere(
    SputnikSyntaxis* syntaxis,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused);


/* ==================================================
 * Eventus
 * ================================================== */

/* Tractare eventum
 *
 * Widget est read-only, ergo semper redde FALSUM.
 *
 * syntaxis: syntax viewer
 * eventus: eventus tractandus
 *
 * Redde: VERUM si eventus tractatus, FALSUM alioquin
 */
b32
sputnik_syntaxis_tractare_eventum(
    SputnikSyntaxis*  syntaxis,
    constans Eventus* eventus);


#endif /* SPUTNIK_SYNTAXIS_H */
