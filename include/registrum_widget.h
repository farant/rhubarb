#ifndef REGISTRUM_WIDGET_H
#define REGISTRUM_WIDGET_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"

/* ==================================================
 * REGISTRUM WIDGET - Factory Registry pro Widgets
 *
 * Registrum mappat nomina tags STML ad functiones factory
 * quae widgets creant. Hoc permittit widgets pluggable
 * sine modificare layout.c.
 *
 * EXEMPLUM:
 *   RegistrumWidget* reg = registrum_widget_creare(piscina);
 *   registrum_widget_registrare(reg, "pagina", pagina_factory);
 *   registrum_widget_registrare(reg, "navigator", navigator_factory);
 *
 *   FunctioWidgetFactory f = registrum_widget_invenire(reg, titulus);
 *   si (f) f(dom, nodus);
 *
 * ================================================== */

/* Maximus numerus widget types */
#define REGISTRUM_WIDGET_MAXIMUS  XXXII  /* 32 */


/* ==================================================
 * Typi
 * ================================================== */

/* Functio factory pro widgets
 *
 * dom: vacuum* ad LayoutDom (pro registrare widget cum manager)
 * nodus: vacuum* ad StmlNodus cum attributa
 *
 * Redde: VERUM si successus, FALSUM si error
 *
 * Nota: Usare vacuum* pro evitare circular dependencies.
 * Factory debet castare ad LayoutDom* et StmlNodus*.
 */
nomen b32 (*FunctioWidgetFactory)(
    vacuum* dom,
    vacuum* nodus);

/* Introitus in registrum */
nomen structura {
    chorda*              titulus;   /* Tag name e.g. "pagina" */
    FunctioWidgetFactory factory;   /* Factory function */
} RegistrumWidgetIntroitus;

/* Registrum widget */
nomen structura {
    Piscina*                  piscina;
    InternamentumChorda*      intern;
    RegistrumWidgetIntroitus  introitus[REGISTRUM_WIDGET_MAXIMUS];
    s32                       numerus;
} RegistrumWidget;


/* ==================================================
 * Creatio
 * ================================================== */

/* Creare registrum widget
 *
 * piscina: piscina pro allocatione
 * intern: internamentum pro chordae
 *
 * Redde: RegistrumWidget* si successus, NIHIL si error
 */
RegistrumWidget*
registrum_widget_creare(
    Piscina*             piscina,
    InternamentumChorda* intern);


/* ==================================================
 * Registratio
 * ================================================== */

/* Registrare widget factory
 *
 * reg: registrum widget
 * titulus: nomen tag (e.g. "pagina", "navigator")
 * factory: functio quae creat widget
 *
 * Redde: VERUM si successus, FALSUM si plenum vel error
 */
b32
registrum_widget_registrare(
    RegistrumWidget*     reg,
    constans character*  titulus,
    FunctioWidgetFactory factory);


/* ==================================================
 * Lookup
 * ================================================== */

/* Invenire factory pro tag
 *
 * reg: registrum widget
 * titulus: nomen tag (chorda*)
 *
 * Redde: FunctioWidgetFactory si inventum, NIHIL si non
 */
FunctioWidgetFactory
registrum_widget_invenire(
    RegistrumWidget* reg,
    chorda*          titulus);

/* Invenire factory pro tag (ex literis)
 *
 * reg: registrum widget
 * titulus: nomen tag (C string)
 *
 * Redde: FunctioWidgetFactory si inventum, NIHIL si non
 */
FunctioWidgetFactory
registrum_widget_invenire_literis(
    RegistrumWidget*    reg,
    constans character* titulus);

#endif /* REGISTRUM_WIDGET_H */
