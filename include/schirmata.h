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
#include "arx_caeli.h"
#include "thema_visus.h"
#include "sputnik_syntaxis.h"
#include "biblia_visus.h"
#include "librarium_visus.h"
#include "fons_visus.h"
#include "calendario_visus.h"
#include "pinacotheca_visus.h"
#include "dialogus.h"
#include "importatio_visus.h"

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
    /* Modi flags - indicant quod widget activum in dextra parte */
    b32 modus_arx_caeli;              /* VERUM = cards mode */
    b32 modus_thema_visus;            /* VERUM = theme viewer mode */
    b32 modus_fons_visus;             /* VERUM = font viewer mode */
    b32 modus_sputnik_syntaxis;       /* VERUM = syntax reference mode */
    b32 modus_biblia_visus;           /* VERUM = Bible viewer mode */
    b32 modus_librarium;              /* VERUM = library viewer mode */
    b32 modus_calendario_visus;       /* VERUM = calendar viewer mode */
    b32 modus_importatio_visus;       /* VERUM = import preview mode */
    b32 modus_pinacotheca;            /* VERUM = gallery viewer mode */
    /* Import state */
    ImportatioVisus* importatio_visus; /* Import preview (during import) */
    /* Dialogus modalis */
    Dialogus* dialogus;               /* Dialogus activus, vel NIHIL */
    s32       dialogus_panel;         /* Quod panel: 0=sinister, 1=dexter */
} Schirma;

/* Controller schirmarum */
nomen structura {
    ContextusWidget* ctx;

    Schirma schirmae[SCHIRMATA_MAXIMUS];
    i32 index_currens;        /* Schirma currens (0-9) */
    b32 praefixum_activum;    /* ctrl-a depressus, expectans commandum */
    f64 tempus_praefixum;     /* Tempus quando ctrl-a depressus */

    /* Res communicatae inter omnes schirmas */
    LibroPaginarum* libro;

    /* Singleton widget instances (una per typum, non per schirmam) */
    ArcCaeli*        arx_caeli;           /* Cloud castle widget */
    ThemaVisus*      thema_visus;         /* Theme viewer widget */
    SputnikSyntaxis* sputnik_syntaxis;    /* Syntax reference widget */
    BibliaVisus*     biblia_visus;        /* Bible viewer widget */
    LibrariumVisus*  librarium_visus;     /* Library viewer widget */
    FonsVisus*       fons_visus;          /* Font glyph viewer widget */
    CalendarioVisus* calendario_visus;    /* Calendar viewer widget */
    PinacothecaVisus* pinacotheca_visus;  /* Gallery viewer widget */
} Schirmata;


/* ==================================================
 * Creatio
 * ================================================== */

/* Creare controller schirmarum
 *
 * Initializat omnes X schirmas cum layout defalta.
 *
 * ctx: contextus widget cum servitiis communicatis
 * libro: libro paginarum (communicatus)
 *
 * Redde: Schirmata* si successus, NIHIL si error
 */
Schirmata*
schirmata_creare(
    ContextusWidget* ctx,
    LibroPaginarum*  libro);


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

/* Commutare ad modus Arx Caeli (cards)
 *
 * schirmata: controller
 * slug: slug schirmae cardium (NIHIL = root)
 */
vacuum
schirmata_commutare_ad_arx_caeli(
    Schirmata*          schirmata,
    constans character* slug);

/* Commutare ad modus navigator
 *
 * schirmata: controller
 */
vacuum
schirmata_commutare_ad_navigator(
    Schirmata* schirmata);

/* Commutare ad modus thema visus (theme viewer)
 *
 * schirmata: controller
 */
vacuum
schirmata_commutare_ad_thema_visus(
    Schirmata* schirmata);

/* Commutare ad modus sputnik syntaxis (syntax reference)
 *
 * schirmata: controller
 */
vacuum
schirmata_commutare_ad_sputnik_syntaxis(
    Schirmata* schirmata);

/* Commutare ad modus biblia visus (Bible viewer)
 *
 * schirmata: controller
 */
vacuum
schirmata_commutare_ad_biblia_visus(
    Schirmata* schirmata);

/* Commutare ad modus librarium (library viewer)
 *
 * schirmata: controller
 * quaestio: optional search query (NIHIL = none)
 */
vacuum
schirmata_commutare_ad_librarium(
    Schirmata*          schirmata,
    constans character* quaestio);

/* Commutare ad modus fons visus (font glyph viewer)
 *
 * schirmata: controller
 */
vacuum
schirmata_commutare_ad_fons_visus(
    Schirmata* schirmata);

/* Commutare ad modus calendario visus (calendar viewer)
 *
 * schirmata: controller
 */
vacuum
schirmata_commutare_ad_calendario_visus(
    Schirmata* schirmata);

/* Commutare ad modus pinacotheca (gallery viewer)
 *
 * schirmata: controller
 * titulus: optional image title to navigate to (NIHIL = browser)
 */
vacuum
schirmata_commutare_ad_pinacotheca(
    Schirmata*          schirmata,
    constans character* titulus);

/* Initiare import imaginis ex clipboard
 *
 * schirmata: controller
 *
 * Redde: VERUM si importatio initiata
 */
b32
schirmata_initiare_importationem_ex_clipboard(
    Schirmata* schirmata);


/* ==================================================
 * Dialogi
 * ================================================== */

/* Aperire dialogum in panel
 *
 * Dialogus captabit omnes eventus usque ad clauditur.
 *
 * schirmata: controller
 * dialogus: dialogus aperiendus
 * panel: quod panel (0=sinister, 1=dexter)
 */
vacuum
schirmata_aperire_dialogum(
    Schirmata* schirmata,
    Dialogus*  dialogus,
    s32        panel);

/* Claudere dialogum currentem
 *
 * schirmata: controller
 */
vacuum
schirmata_claudere_dialogum(
    Schirmata* schirmata);

/* Quaerere an dialogus activus
 *
 * schirmata: controller
 *
 * Redde: VERUM si dialogus activus
 */
b32
schirmata_habet_dialogum(
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

/* Obtinere libro paginarum (communicatus)
 *
 * schirmata: controller
 *
 * Redde: LibroPaginarum*
 */
LibroPaginarum*
schirmata_libro(
    Schirmata* schirmata);


/* ==================================================
 * Persistentia
 * ================================================== */

/* Salvare omnes schirmas ad repository
 *
 * Salvat status globalis (index_currens) et status per schirmam
 * (modus, libro_status, widget status).
 *
 * schirmata: controller
 */
vacuum
schirmata_salvare_omnes(
    Schirmata* schirmata);

/* Carcare omnes schirmas ex repository
 *
 * Restituit status globalis et per schirmam ex repository.
 * Vocare post schirmata_creare().
 *
 * schirmata: controller
 */
vacuum
schirmata_carcare_omnes(
    Schirmata* schirmata);


#endif /* SCHIRMATA_H */
