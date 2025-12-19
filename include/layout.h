#ifndef LAYOUT_H
#define LAYOUT_H

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "tabula_dispersa.h"
#include "widget.h"
#include "pagina.h"
#include "libro_paginarum.h"
#include "navigator_entitatum.h"
#include "entitas_repositorium.h"
#include "registrum_commandi.h"
#include "registrum_widget.h"

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
    LAYOUT_WIDGET_NAVIGATOR = II,
    LAYOUT_WIDGET_LIBRO = III
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
    ContextusWidget*     ctx;      /* Contextus communicatus */
} LayoutDom;


/* ==================================================
 * Creatio
 * ================================================== */

/* Creare layout ex STML
 *
 * Parset STML, creat widgets, registrat cum manager.
 * Tags supportati: <pagina>, <navigator>
 *
 * ctx: contextus widget cum servitiis communicatis
 * stml: chorda STML cum <layout> radice
 *
 * Redde: LayoutDom* si successus, NIHIL si error
 */
LayoutDom*
layout_creare(
    ContextusWidget*    ctx,
    constans character* stml);


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

/* Obtinere LibroPaginarum per id
 *
 * dom: layout dom
 * id: identificator widget
 *
 * Redde: LibroPaginarum* si inventum et genus correctum, NIHIL aliter
 */
LibroPaginarum*
layout_obtinere_libro(
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


/* ==================================================
 * Registratio Defalta
 * ================================================== */

/* Registrare built-in widget factories
 *
 * Registrat factories pro: pagina, libro, navigator
 * Vocare ante layout_creare() si usare registrum.
 *
 * reg: registrum widget
 *
 * Redde: VERUM si successus, FALSUM si error
 */
b32
layout_registrare_defalta(
    RegistrumWidget* reg);

#endif /* LAYOUT_H */
