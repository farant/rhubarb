#ifndef LAYOUT_H
#define LAYOUT_H

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "tabula_dispersa.h"
#include "widget.h"
#include "pagina.h"
#include "navigator_entitatum.h"
#include "entitas_repositorium.h"
#include "registrum_commandi.h"

/* ==================================================
 * LAYOUT - Systema Declarativum pro Widgets
 *
 * Parset STML et creat widgets, registrat cum ManagerWidget.
 * Reddit LayoutDom cum manager et tabula lookup per id.
 *
 * EXEMPLUM:
 *   LayoutDom* dom = layout_creare(piscina, intern,
 *       "<layout>"
 *       "  <pagina id='editor' x=0 y=0 latitudo=71 altitudo=60/>"
 *       "  <navigator id='nav' x=71 y=0 latitudo=71 altitudo=60/>"
 *       "</layout>",
 *       repositorium);
 *
 *   manager_widget_tractare_eventum(dom->manager, &eventus);
 *   manager_widget_reddere(dom->manager, tabula, scala);
 *
 *   Pagina* editor = layout_obtinere_pagina(dom, "editor");
 *
 * ================================================== */


/* ==================================================
 * Typi
 * ================================================== */

/* Genus widget (pro lookup typato) */
nomen enumeratio {
    LAYOUT_WIDGET_PAGINA = I,
    LAYOUT_WIDGET_NAVIGATOR = II
} LayoutGenusWidget;

/* Introitus in tabula lookup */
nomen structura {
    vacuum*           datum;   /* Pagina*, NavigatorEntitatum*, etc */
    LayoutGenusWidget genus;   /* Genus widget */
} LayoutWidgetIntroitus;

/* DOM layout - continet manager et tabula lookup */
nomen structura {
    ManagerWidget*       manager;
    TabulaDispersa*      widgets;  /* id (chorda*) -> LayoutWidgetIntroitus* */
    Piscina*             piscina;
    InternamentumChorda* intern;
    RegistrumCommandi*   reg_commandi;  /* Registrum commandi pro tag clicks */
} LayoutDom;


/* ==================================================
 * Creatio
 * ================================================== */

/* Creare layout ex STML
 *
 * Parset STML, creat widgets, registrat cum manager.
 * Tags supportati: <pagina>, <navigator>
 *
 * piscina: piscina pro allocatione
 * intern: internamentum chordarum
 * stml: chorda STML cum <layout> radice
 * repositorium: usatum pro <navigator> widgets (potest esse NIHIL si nullus navigator)
 *
 * Redde: LayoutDom* si successus, NIHIL si error
 */
LayoutDom*
layout_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  stml,
    EntitasRepositorium* repositorium);


/* ==================================================
 * Accessores
 * ================================================== */

/* Obtinere widget per id (genericum)
 *
 * dom: layout dom
 * id: identificator widget
 *
 * Redde: vacuum* ad widget, vel NIHIL si non inventum
 */
vacuum*
layout_obtinere(
    LayoutDom*          dom,
    constans character* id);

/* Obtinere Pagina per id
 *
 * dom: layout dom
 * id: identificator widget
 *
 * Redde: Pagina* si inventum et genus correctum, NIHIL aliter
 */
Pagina*
layout_obtinere_pagina(
    LayoutDom*          dom,
    constans character* id);

/* Obtinere NavigatorEntitatum per id
 *
 * dom: layout dom
 * id: identificator widget
 *
 * Redde: NavigatorEntitatum* si inventum et genus correctum, NIHIL aliter
 */
NavigatorEntitatum*
layout_obtinere_navigator(
    LayoutDom*          dom,
    constans character* id);


/* ==================================================
 * Registrum Commandi
 * ================================================== */

/* Ponere registrum commandi
 *
 * Omnes paginae in layout usabunt hoc registrum
 * pro executando commands ex tag clicks.
 *
 * dom: layout dom
 * reg_commandi: registrum commandi
 */
vacuum
layout_ponere_reg_commandi(
    LayoutDom*         dom,
    RegistrumCommandi* reg_commandi);

#endif /* LAYOUT_H */
