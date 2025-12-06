#ifndef SCHIRMATA_H
#define SCHIRMATA_H

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "widget.h"
#include "libro_paginarum.h"
#include "navigator_entitatum.h"
#include "entitas_repositorium.h"
#include "registrum_commandi.h"

/* ==================================================
 * SCHIRMATA - Systema Schirmarum (Screens)
 *
 * Multiplices schirmae cum commutatio per ctrl-a prefix.
 * Similis ad tmux: ctrl-a + n/p/1-9 ad commutare.
 *
 * PHILOSOPHIA:
 * - X schirmae (10), omnes cum eodem layout
 * - Libro et repositorium communicata inter schirmas
 * - Status visus (pagina currens, cursor) salvatur per schirmam
 * - Tab bar in fundo fenestrae
 *
 * EXEMPLUM:
 *   Schirmata* s = schirmata_creare(piscina, intern, libro, repo, reg);
 *   schirmata_tractare_eventum(s, &eventus);
 *   schirmata_reddere(s, tabula, scala);
 *
 * ================================================== */


/* ==================================================
 * Constantae
 * ================================================== */

#define SCHIRMATA_MAXIMUS  X  /* 10 schirmae */


/* ==================================================
 * Typi
 * ================================================== */

/* Status salvatus pro libro widget in schirma */
nomen structura {
    i32 index_paginae;      /* Quae pagina aperta */
    i32 cursor_linea;       /* Positio cursoris */
    i32 cursor_columna;
    ModoVim modo;           /* Modus vim (normalis, inserere, visualis) */
} SchirmaLibroStatus;

/* Singula schirma */
nomen structura {
    ManagerWidget* manager;           /* Widgets pro hac schirma */
    SchirmaLibroStatus libro_status;  /* Status salvatus libro */
    b32 initiatus;                    /* An haec schirma initiata */
} Schirma;

/* Controller schirmarum */
nomen structura {
    Piscina* piscina;
    InternamentumChorda* intern;

    Schirma schirmae[SCHIRMATA_MAXIMUS];
    i32 index_currens;        /* Schirma currens (0-9) */
    b32 praefixum_activum;    /* ctrl-a depressus, expectans commandum */
    f64 tempus_praefixum;     /* Tempus quando ctrl-a depressus */

    /* Res communicatae inter omnes schirmas */
    LibroPaginarum* libro;
    EntitasRepositorium* repo;
    RegistrumCommandi* reg_commandi;
} Schirmata;


/* ==================================================
 * Creatio
 * ================================================== */

/* Creare controller schirmarum
 *
 * Initializat omnes X schirmas cum layout defalta.
 *
 * piscina: piscina pro allocatione
 * intern: internamentum chordarum
 * libro: libro paginarum (communicatus)
 * repo: repositorium entitatum (communicatus)
 * reg_commandi: registrum commandi
 *
 * Redde: Schirmata* si successus, NIHIL si error
 */
Schirmata*
schirmata_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    LibroPaginarum*      libro,
    EntitasRepositorium* repo,
    RegistrumCommandi*   reg_commandi);


/* ==================================================
 * Commutatio Schirmarum
 * ================================================== */

/* Commutare ad schirmam per index
 *
 * Salvat status currentis, commutat, restituit status novae.
 *
 * schirmata: controller
 * index: index schirmae (0-9)
 */
vacuum
schirmata_commutare_ad(
    Schirmata* schirmata,
    i32        index);

/* Commutare ad schirmam proximam (cyclus)
 *
 * schirmata: controller
 */
vacuum
schirmata_proxima(
    Schirmata* schirmata);

/* Commutare ad schirmam priorem (cyclus)
 *
 * schirmata: controller
 */
vacuum
schirmata_prior(
    Schirmata* schirmata);


/* ==================================================
 * Eventus
 * ================================================== */

/* Tractare eventum
 *
 * Intercipiat ctrl-a prefix et commanda schirmae.
 * Aliter delegat ad manager schirmae currentis.
 *
 * schirmata: controller
 * eventus: eventus tractandus
 *
 * Redde: VERUM si eventus tractatus
 */
b32
schirmata_tractare_eventum(
    Schirmata*        schirmata,
    constans Eventus* eventus);


/* ==================================================
 * Reddere
 * ================================================== */

/* Reddere schirmam currentem et tab bar
 *
 * schirmata: controller
 * tabula: tabula pixelorum
 * scala: factor scalae fontis
 */
vacuum
schirmata_reddere(
    Schirmata*       schirmata,
    TabulaPixelorum* tabula,
    i32              scala);


/* ==================================================
 * Interrogationes
 * ================================================== */

/* Obtinere index schirmae currentis
 *
 * schirmata: controller
 *
 * Redde: index (0-9)
 */
i32
schirmata_index_currens(
    Schirmata* schirmata);

/* Obtinere manager schirmae currentis
 *
 * schirmata: controller
 *
 * Redde: ManagerWidget* schirmae currentis
 */
ManagerWidget*
schirmata_manager_currens(
    Schirmata* schirmata);


#endif /* SCHIRMATA_H */
