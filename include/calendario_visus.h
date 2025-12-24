#ifndef CALENDARIO_VISUS_H
#define CALENDARIO_VISUS_H

#include "latina.h"
#include "piscina.h"
#include "fenestra.h"
#include "widget.h"
#include "calendarium_liturgicum.h"
#include "fasti.h"

/* ==================================================
 * CALENDARIO VISUS - Liturgical Calendar Widget
 *
 * Widget pro monstrare calendarium liturgicum mensile
 * cum detaliis diei selecti.
 *
 * LAYOUT:
 * - Pars superior (1/3): Detalia diei selecti
 *   - Data, nomen festi, tempus liturgicum, cyclus
 * - Pars inferior (2/3): Grilla mensis
 *   - 7 columnae (S M T W T F S)
 *   - 6 ordines pro hebdomadis
 *   - Numeri dierum colorati per colorem liturgicum
 *
 * NAVIGATIO (vim):
 * - h/l: dies prior/proximus
 * - j/k: hebdomada proxima/prior
 * - H/L: mensis prior/proximus
 * - J/K: annus prior/proximus
 *
 * EXEMPLUM:
 *   CalendarioVisus* visus = calendario_visus_creare(piscina);
 *   calendario_visus_reddere(visus, tabula, x, y, lat, alt, scala, focused);
 *
 * ================================================== */


/* ==================================================
 * Typi
 * ================================================== */

/* CalendarioVisus - Widget state */
nomen structura {
    Piscina* piscina;
    CalendariumLiturgicum* cal;

    /* Status visus */
    s32 annus_visus;        /* Annus monstratus */
    s32 mensis_visus;       /* Mensis monstratus (1-12) */
    s32 dies_selectus;      /* Dies selectus (1-31) */

    /* Computata pro mense currenti */
    s32 primus_dies_hebdomadae;  /* Dies hebdomadae pro die 1 (0=Dom) */
    s32 numerus_dierum;          /* Numerus dierum in mense */

    /* Cache dimensionum */
    i32 widget_x;
    i32 widget_y;
    i32 latitudo;
    i32 altitudo;
    i32 scala;
} CalendarioVisus;


/* ==================================================
 * Creatio
 * ================================================== */

/* Creare calendario viewer
 *
 * piscina: piscina memoriae
 *
 * Redde: CalendarioVisus* si successus, NIHIL si error
 *
 * Initializat ad diem hodiernum.
 */
CalendarioVisus*
calendario_visus_creare(
    Piscina* piscina);


/* ==================================================
 * Reddere
 * ================================================== */

/* Reddere calendario viewer
 *
 * visus: calendario viewer
 * tabula: tabula pixelorum
 * x, y: positio in characteribus
 * latitudo, altitudo: dimensiones in characteribus
 * scala: factor scalae fontis
 * focused: an widget habet focus
 */
vacuum
calendario_visus_reddere(
    CalendarioVisus* visus,
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
 * visus: calendario viewer
 * eventus: eventus tractandus
 *
 * Redde: VERUM si eventus tractatus, FALSUM alioquin
 */
b32
calendario_visus_tractare_eventum(
    CalendarioVisus*  visus,
    constans Eventus* eventus);


/* ==================================================
 * Navigatio
 * ================================================== */

/* Navigare ad diem specificum
 *
 * visus: calendario viewer
 * dies: dies ad quem navigare
 */
vacuum
calendario_visus_navigare_ad(
    CalendarioVisus* visus,
    Dies             dies);

/* Navigare ad diem hodiernum
 *
 * visus: calendario viewer
 */
vacuum
calendario_visus_hodie(
    CalendarioVisus* visus);


/* ==================================================
 * Lifecycle
 * ================================================== */

/* Initiare widget - registrare commandi
 *
 * ctx: contextus widget cum reg_commandi
 *
 * Vocatur per registrum_widget_initiare_omnes()
 */
vacuum
calendario_visus_init(
    ContextusWidget* ctx);


#endif /* CALENDARIO_VISUS_H */
