#ifndef CONCHA_H
#define CONCHA_H

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "persistentia.h"
#include "entitas_repositorium.h"
#include "widget.h"
#include "registrum_commandi.h"
#include "registrum_widget.h"
#include "schirmata.h"
#include "libro_paginarum.h"
#include "fenestra.h"

/* ==================================================
 * CONCHA - Widget Shell
 *
 * Concha est testa quae omnem infrastructuram possidet:
 * - Piscina, Internamentum, Persistentia, Repositorium
 * - ContextusWidget cum servitiis
 * - Schirmata (10 screens)
 * - Fenestra et TabulaPixelorum
 * - Event loop
 *
 * EXEMPLUM:
 *   ConchaConfiguratio config;
 *   Concha* concha;
 *
 *   config.titulus = "Mea Applicatio";
 *   config.latitudo = DCCCLIII;
 *   config.altitudo = CDLXXX;
 *   config.via_persistentia = "data/app.log";
 *
 *   concha = concha_creare(&config);
 *   si (concha)
 *   {
 *       concha_registrare_commandi(concha, "rename", cmd_rename, datum);
 *       redde concha_currere(concha);
 *   }
 *
 * ================================================== */


/* ==================================================
 * Configuratio
 * ================================================== */

/* Configuratio pro concha */
nomen structura {
    constans character* titulus;           /* Titulus fenestrae */
    i32                 latitudo;          /* Latitudo fenestrae (pixels) */
    i32                 altitudo;          /* Altitudo fenestrae (pixels) */
    constans character* via_persistentia;  /* Via ad log file (NIHIL = in memoria) */
} ConchaConfiguratio;


/* ==================================================
 * Concha Structure
 * ================================================== */

nomen structura {
    /* Infrastructura */
    Piscina*             piscina;
    InternamentumChorda* intern;
    Persistentia*        persistentia;
    EntitasRepositorium* repositorium;
    ContextusWidget*     ctx;

    /* Registra */
    RegistrumCommandi*   reg_commandi;
    RegistrumWidget*     reg_widget;

    /* UI */
    Schirmata*           schirmata;
    Fenestra*            fenestra;
    TabulaPixelorum*     tabula;

    /* Configuratio */
    constans character*  via_persistentia;
    i32                  latitudo;
    i32                  altitudo;

    /* Status */
    b32                  currens;
} Concha;


/* ==================================================
 * Creatio
 * ================================================== */

/* Creare concha - initiare omnem infrastructuram
 *
 * config: configuratio concha
 *
 * Redde: Concha* si successus, NIHIL si error
 *
 * Nota: Registrat built-in widget types (pagina, libro, navigator)
 *       et built-in commands ($date) automatice.
 */
Concha*
concha_creare(ConchaConfiguratio* config);


/* ==================================================
 * Registratio
 * ================================================== */

/* Registrare widget type (ante concha_currere)
 *
 * concha: concha
 * titulus: nomen tag (e.g. "meus-widget")
 * factory: functio factory
 *
 * Redde: VERUM si successus, FALSUM si error
 */
b32
concha_registrare_widget(
    Concha*              concha,
    constans character*  titulus,
    FunctioWidgetFactory factory);

/* Registrare command (ante concha_currere)
 *
 * concha: concha
 * nomen_cmd: nomen commandi (e.g. "rename")
 * functio: functio commandi
 * datum: datum custom pro functio
 *
 * Redde: VERUM si successus, FALSUM si error
 */
b32
concha_registrare_commandi(
    Concha*             concha,
    constans character* nomen_cmd,
    FunctioCommand      functio,
    vacuum*             datum);


/* ==================================================
 * Layout
 * ================================================== */

/* Ponere layout pro schirma specifica
 *
 * concha: concha
 * schirma_index: index schirmae (0-9)
 * stml: STML layout string
 *
 * Redde: VERUM si successus, FALSUM si error
 */
b32
concha_ponere_layout(
    Concha*             concha,
    i32                 schirma_index,
    constans character* stml);


/* ==================================================
 * Executio
 * ================================================== */

/* Currere event loop - bloquat usque ad exitum
 *
 * concha: concha
 *
 * Redde: 0 si clean exit, 1 si error
 */
s32
concha_currere(Concha* concha);


/* ==================================================
 * Accessores
 * ================================================== */

/* Obtinere libro paginarum (pro commands)
 *
 * concha: concha
 *
 * Redde: LibroPaginarum* ex schirma currens, vel NIHIL
 */
LibroPaginarum*
concha_libro(Concha* concha);

/* Obtinere schirmata (pro commands)
 *
 * concha: concha
 *
 * Redde: Schirmata*
 */
Schirmata*
concha_schirmata(Concha* concha);

/* Obtinere repositorium (pro commands)
 *
 * concha: concha
 *
 * Redde: EntitasRepositorium*
 */
EntitasRepositorium*
concha_repositorium(Concha* concha);

/* Obtinere piscina (pro commands quae allocatione requirunt)
 *
 * concha: concha
 *
 * Redde: Piscina*
 */
Piscina*
concha_piscina(Concha* concha);

/* Obtinere internamentum (pro commands)
 *
 * concha: concha
 *
 * Redde: InternamentumChorda*
 */
InternamentumChorda*
concha_intern(Concha* concha);

#endif /* CONCHA_H */
