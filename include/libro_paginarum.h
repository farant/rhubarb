#ifndef LIBRO_PAGINARUM_H
#define LIBRO_PAGINARUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "pagina.h"
#include "entitas_repositorium.h"
#include "registrum_commandi.h"

/* ==================================================
 * LIBRO PAGINARUM - Multi-Page Editor Widget
 *
 * Collectio paginarum cum navigatio et historia.
 * Inspiratum ab Aquinas page.c, sed cum entitas integration.
 *
 * PHILOSOPHIA:
 * - Array de Pagina* (in memoria)
 * - Navigatio past ultima pagina creat novam
 * - Historia navigationis pro #back
 * - Repository optionalis pro persistentia
 *
 * EXEMPLUM:
 *   LibroPaginarum* libro = libro_creare(piscina, intern);
 *   libro_navigare_ad(libro, II);  // ad paginam 2
 *   Pagina* p = libro_pagina_currens(libro);
 *   pagina_tractare_eventum(p, &eventus);
 *
 * ================================================== */


/* ==================================================
 * Constantae
 * ================================================== */

#define LIBRO_MAXIMUS_PAGINARUM   C      /* 100 paginae */
#define LIBRO_HISTORIA_MAGNITUDO  XXXII  /* 32 entries */


/* ==================================================
 * Typi
 * ================================================== */

/* Liber paginarum - multi-page editor */
nomen structura {
    /* Piscina pro allocationes */
    Piscina* piscina;

    /* Internamentum pro chordae */
    InternamentumChorda* intern;

    /* Repository pro persistentia (NIHIL pro standalone) */
    EntitasRepositorium* repo;

    /* Registrum commandi pro coloratio (NIHIL = colorare omnes $word) */
    RegistrumCommandi* reg_commandi;

    /* Array de Pagina* */
    Pagina* paginae[LIBRO_MAXIMUS_PAGINARUM];

    /* Nomina paginarum (chorda* internatae) */
    chorda* nomina[LIBRO_MAXIMUS_PAGINARUM];

    /* Entity IDs pro paginis (chorda* internatae, NIHIL si nondum creata) */
    chorda* entitas_ids[LIBRO_MAXIMUS_PAGINARUM];

    /* Numerus paginarum */
    i32 numerus_paginarum;

    /* Index paginae currentis */
    i32 index_currens;

    /* Historia navigationis */
    i32 historia[LIBRO_HISTORIA_MAGNITUDO];
    i32 historia_numerus;

    /* Dirty flag pro debounced save */
    b32 est_immundus;
    f64 tempus_ultimae_mutationis;
} LibroPaginarum;


/* ==================================================
 * Creatio
 * ================================================== */

/* Creare libro paginarum
 *
 * Allocat libro et creat primam paginam.
 *
 * piscina: piscina pro allocationes
 * intern:  internamentum pro chordae
 *
 * Redde: LibroPaginarum* si successus, NIHIL si error
 */
LibroPaginarum*
libro_creare(
    Piscina*             piscina,
    InternamentumChorda* intern);


/* Connectere repository (optionalis)
 *
 * libro: libro paginarum
 * repo:  repository pro persistentia
 */
vacuum
libro_connectere_repo(
    LibroPaginarum*      libro,
    EntitasRepositorium* repo);


/* ==================================================
 * Navigatio
 * ================================================== */

/* Navigare ad paginam per index
 *
 * Si index >= numerus_paginarum, creat novas paginas.
 * Recordat paginam currentem in historia.
 *
 * libro: libro paginarum
 * index: index paginae (0-based, negative clamped to 0)
 */
vacuum
libro_navigare_ad(
    LibroPaginarum* libro,
    s32             index);


/* Navigare ad paginam proximam
 *
 * libro: libro paginarum
 */
vacuum
libro_pagina_proxima(
    LibroPaginarum* libro);


/* Navigare ad paginam priorem
 *
 * libro: libro paginarum
 */
vacuum
libro_pagina_prior(
    LibroPaginarum* libro);


/* Navigare retro in historia
 *
 * libro: libro paginarum
 */
vacuum
libro_retro(
    LibroPaginarum* libro);


/* Navigare ad paginam per nomen
 *
 * libro:  libro paginarum
 * nomen_: nomen paginae (null-terminated)
 *
 * Redde: VERUM si inventa et navigata, FALSUM si non inventa
 */
b32
libro_navigare_ad_nomen(
    LibroPaginarum*     libro,
    constans character* nomen_);


/* ==================================================
 * Accessus
 * ================================================== */

/* Obtinere paginam currentem
 *
 * libro: libro paginarum
 *
 * Redde: Pagina* currentis
 */
Pagina*
libro_pagina_currens(
    LibroPaginarum* libro);


/* Obtinere index currens
 *
 * libro: libro paginarum
 *
 * Redde: index paginae currentis (0-based)
 */
i32
libro_index_currens(
    LibroPaginarum* libro);


/* Obtinere numerum paginarum
 *
 * libro: libro paginarum
 *
 * Redde: numerus paginarum
 */
i32
libro_numerus_paginarum(
    LibroPaginarum* libro);


/* ==================================================
 * Gestio Paginarum
 * ================================================== */

/* Creare novam paginam
 *
 * Creat paginam ad finem libri.
 *
 * libro:  libro paginarum
 * nomen_: nomen paginae (NIHIL pro sine nomine)
 *
 * Redde: index novae paginae, vel -1 si error
 */
s32
libro_pagina_nova(
    LibroPaginarum*     libro,
    constans character* nomen_);


/* Nominare paginam
 *
 * libro:  libro paginarum
 * index:  index paginae
 * nomen_: novum nomen (NIHIL pro removere nomen)
 */
vacuum
libro_pagina_nominare(
    LibroPaginarum*     libro,
    i32                 index,
    constans character* nomen_);


/* Obtinere nomen paginae
 *
 * libro: libro paginarum
 * index: index paginae
 *
 * Redde: chorda* nomen (vel NIHIL si sine nomine)
 */
chorda*
libro_pagina_nomen(
    LibroPaginarum* libro,
    i32             index);


/* ==================================================
 * Eventus
 * ================================================== */

/* Tractare eventum
 *
 * Delegat ad pagina currens, sed intercipiat navigationem.
 *
 * libro:   libro paginarum
 * eventus: eventus tractandus
 *
 * Redde: VERUM si tractatus, FALSUM si debet claudere
 */
b32
libro_tractare_eventum(
    LibroPaginarum* libro,
    constans Eventus* eventus);


/* ==================================================
 * Reddere
 * ================================================== */

/* Reddere libro cum margine et indicatore paginae
 *
 * Pingit paginam currentem cum border et "Pagina N/M" in summo.
 *
 * piscina:         piscina pro allocationes temporarias
 * tabula_pixelorum: tabula pixelorum
 * libro:           libro paginarum
 * x, y:            originis (in grid coordinates)
 * latitudo:        latitudo totalis (includens border)
 * altitudo:        altitudo totalis (includens border)
 * scala:           factor scalae fontis
 * focused:         VERUM si habet focus
 */
vacuum
libro_reddere(
    Piscina*         piscina,
    TabulaPixelorum* tabula_pixelorum,
    LibroPaginarum*  libro,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused);


/* ==================================================
 * Serialization (pro persistentia)
 * ================================================== */

/* Salvare paginam currentem ad repository
 *
 * libro: libro paginarum
 *
 * Redde: VERUM si successus
 */
b32
libro_salvare_paginam(
    LibroPaginarum* libro);


/* Salvare omnes paginas ad repository
 *
 * libro: libro paginarum
 *
 * Redde: VERUM si successus
 */
b32
libro_salvare_omnes(
    LibroPaginarum* libro);


/* Carcare paginas ex repository
 *
 * libro: libro paginarum
 *
 * Redde: VERUM si successus
 */
b32
libro_carcare(
    LibroPaginarum* libro);


/* Ponere registrum commandi pro coloratio omnium paginarum
 *
 * libro: libro paginarum
 * reg:   registrum commandi
 */
vacuum
libro_ponere_reg_commandi(
    LibroPaginarum*    libro,
    RegistrumCommandi* reg);


/* ==================================================
 * Debounced Save
 * ================================================== */

/* Marcare libro ut immundum (dirty) - resets debounce timer
 *
 * libro: libro paginarum
 */
vacuum
libro_marcare_immundum(
    LibroPaginarum* libro);


/* Salvare si immundum et debounce tempus praeteritum
 * Vocare in render loop vel event loop
 *
 * libro: libro paginarum
 */
vacuum
libro_salvare_si_immundum(
    LibroPaginarum* libro);


#endif /* LIBRO_PAGINARUM_H */
