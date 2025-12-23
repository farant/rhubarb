#ifndef WIDGET_H
#define WIDGET_H

#include "latina.h"
#include "fenestra.h"
#include "piscina.h"
#include "internamentum.h"
#include "entitas_repositorium.h"
#include "registrum_commandi.h"
#include "registrum_widget.h"

/* Constantae pro detectione duplex click */
#define DUPLEX_CLICK_TEMPUS     0.3     /* 300ms */
#define DUPLEX_CLICK_DISTANTIA  XII     /* 12 pixels (~2 characteres) */


/* ==================================================
 * CONTEXTUS WIDGET - Servitia Communicata
 *
 * Contextus continet omnia servitia quae widgets
 * requirunt. Omnes widgets utuntur eodem contextu
 * ut consistentia servetur.
 *
 * ================================================== */

/* Callback pro commutare widget in schirmata
 *
 * schirmata_datum: datum opacum schirmata
 * widget_titulus: nomen widget (e.g., "biblia", "librarium")
 * argumentum: argumentum optionale (e.g., "Genesis 3", search query)
 */
nomen vacuum (*FunctioCommutareWidget)(
    vacuum*             schirmata_datum,
    constans character* widget_titulus,
    constans character* argumentum);

nomen structura {
    Piscina*             piscina;
    InternamentumChorda* intern;
    EntitasRepositorium* repo;          /* NIHIL si non requiritur */
    RegistrumCommandi*   reg_commandi;  /* NIHIL si non requiritur */
    RegistrumWidget*     reg_widget;    /* NIHIL si non requiritur */

    /* Pro mode switching - widgets vocant hunc callback ad commutare */
    FunctioCommutareWidget commutare_widget;  /* NIHIL si non requiritur */
    vacuum*                schirmata_datum;   /* Datum opacum pro callback */
} ContextusWidget;

/* Creare contextum widget
 *
 * piscina: piscina pro allocatione
 * intern: internamentum chordarum
 * repo: repositorium entitatum (potest esse NIHIL)
 * reg_commandi: registrum commandi (potest esse NIHIL)
 * reg_widget: registrum widget factories (potest esse NIHIL)
 * commutare_widget: callback pro mode switching (potest esse NIHIL)
 * schirmata_datum: datum opacum pro callback (potest esse NIHIL)
 *
 * Redde: ContextusWidget* si successus, NIHIL si error
 */
ContextusWidget*
contextus_widget_creare(
    Piscina*               piscina,
    InternamentumChorda*   intern,
    EntitasRepositorium*   repo,
    RegistrumCommandi*     reg_commandi,
    RegistrumWidget*       reg_widget,
    FunctioCommutareWidget commutare_widget,
    vacuum*                schirmata_datum);


/* ==================================================
 * WIDGET - Sistema Compositionis Widgetorum
 *
 * Generic widget system pro compositione UI widgets
 * cum focus management et event routing.
 *
 * PHILOSOPHIA:
 * - Widgets sunt polymorphici (vtable pattern)
 * - Manager tractat focus et event routing
 * - Widgets decidunt si consumunt eventus
 * - Click-to-focus automaticus
 *
 * EXEMPLUM:
 *   ManagerWidget* manager = manager_widget_creare(piscina);
 *   manager_widget_registrare(manager, pagina_widget, 0, 0, 71, 60);
 *   manager_widget_tractare_eventum(manager, &eventus);
 *   manager_widget_reddere(manager, tabula, 1);
 *
 * ================================================== */


/* ==================================================
 * Widget Vtable - Function Pointers
 * ================================================== */

/* Forward declaration */
structura Widget;

/* Reddere widget
 *
 * widget: widget ad reddendum
 * tabula: tabula pixelorum
 * x, y: positio in characteribus
 * latitudo, altitudo: dimensiones in characteribus
 * scala: factor scalae fontis
 * focused: VERUM si widget habet focus
 */
nomen vacuum (*FunctioReddere)(
    structura Widget* widget,
    TabulaPixelorum* tabula,
    i32 x,
    i32 y,
    i32 latitudo,
    i32 altitudo,
    i32 scala,
    b32 focused);

/* Tractare eventum
 *
 * widget: widget
 * eventus: eventus ad tractandum
 *
 * Redde: VERUM si eventus consumptus, FALSUM si non
 */
nomen b32 (*FunctioTractareEventum)(
    structura Widget* widget,
    constans Eventus* eventus);


/* ==================================================
 * Widget Structura
 * ================================================== */

nomen structura Widget {
    vacuum*               datum;              /* Widget data (Pagina*, NavigatorEntitatum*, etc) */
    FunctioReddere        reddere;            /* Rendering function */
    FunctioTractareEventum tractare_eventum;  /* Event handler */

    /* Layout */
    i32 x;                /* Positio X in characteribus */
    i32 y;                /* Positio Y in characteribus */
    i32 latitudo;         /* Latitudo in characteribus */
    i32 altitudo;         /* Altitudo in characteribus */
} Widget;


/* ==================================================
 * Widget Manager
 * ================================================== */

nomen structura ManagerWidget {
    Piscina* piscina;
    Widget   widgets[XVI];      /* Array widgetorum registratorum */
    s32      numerus_widgetorum;
    s32      focus_index;       /* Index widget cum focus (-1 si nullus) */

    /* Detectio duplex click */
    f64      tempus_ultimus_click;
    i32      ultimus_click_x;   /* In pixelis */
    i32      ultimus_click_y;
} ManagerWidget;


/* ==================================================
 * Manager - Creatio
 * ================================================== */

/* Creare manager widgetorum
 *
 * piscina: piscina pro allocare manager
 *
 * Redde: manager novus, vel NIHIL si fractura
 */
ManagerWidget*
manager_widget_creare(
    Piscina* piscina);


/* ==================================================
 * Manager - Registratio Widgetorum
 * ================================================== */

/* Registrare widget cum manager
 *
 * manager: manager
 * datum: datum widget (e.g., Pagina*, NavigatorEntitatum*)
 * reddere: functio reddendi
 * tractare_eventum: functio tractandi eventum
 * x, y: positio in characteribus
 * latitudo, altitudo: dimensiones in characteribus
 *
 * Redde: index widget registrati, vel -1 si capacitas plena
 */
s32
manager_widget_registrare(
    ManagerWidget*         manager,
    vacuum*                datum,
    FunctioReddere         reddere,
    FunctioTractareEventum tractare_eventum,
    i32                    x,
    i32                    y,
    i32                    latitudo,
    i32                    altitudo);


/* ==================================================
 * Manager - Focus Management
 * ================================================== */

/* Ponere focus ad widget per index
 *
 * manager: manager
 * index: index widget
 */
vacuum
manager_widget_ponere_focus(
    ManagerWidget* manager,
    s32            index);

/* Movere focus ad widget proximum (cyclus)
 *
 * manager: manager
 */
vacuum
manager_widget_focus_proximum(
    ManagerWidget* manager);

/* Movere focus ad widget praecedentem (cyclus)
 *
 * manager: manager
 */
vacuum
manager_widget_focus_praecedens(
    ManagerWidget* manager);

/* Ponere focus basatus in coordinatis muris (click-to-focus)
 *
 * manager: manager
 * x, y: coordinatae characterum
 *
 * Redde: VERUM si widget inventus ad coordinatas
 */
b32
manager_widget_focus_ad_punctum(
    ManagerWidget* manager,
    i32            x,
    i32            y);


/* ==================================================
 * Manager - Event Handling
 * ================================================== */

/* Tractare eventum
 *
 * Routes eventum ad widget cum focus.
 * Si widget non consumit eventum, tractat TAB pro focus cycling.
 *
 * manager: manager
 * eventus: eventus ad tractandum
 *
 * Redde: VERUM si eventus tractatus
 */
b32
manager_widget_tractare_eventum(
    ManagerWidget*    manager,
    constans Eventus* eventus);


/* ==================================================
 * Manager - Rendering
 * ================================================== */

/* Reddere omnes widgets
 *
 * manager: manager
 * tabula: tabula pixelorum
 * scala: factor scalae fontis
 */
vacuum
manager_widget_reddere(
    ManagerWidget*   manager,
    TabulaPixelorum* tabula,
    i32              scala);

#endif /* WIDGET_H */
