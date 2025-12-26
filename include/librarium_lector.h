/* librarium_lector.h - "Lector Librorum"
 *
 * Bibliotheca pro carcare et legere textus librorum
 *
 * PHILOSOPHIA:
 * - Separatio inter catalogum (librarium_visus) et lectione (hic)
 * - Caching paginationis pro efficentia
 * - Persistentia progressus lectionis per librum
 *
 * USUS:
 *   LibrariumLector* lector = librarium_lector_creare(piscina);
 *   librarium_lector_carcare(lector, "/via/ad/librum.txt", "12345");
 *   librarium_lector_paginare(lector, 80, 40);
 *   librarium_lector_pagina_proxima(lector);
 *   librarium_lector_salvare_progressum(lector, repo);
 */

#ifndef LIBRARIUM_LECTOR_H
#define LIBRARIUM_LECTOR_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "paginarium.h"
#include "sententia_paginarium.h"
#include "entitas_repositorium.h"
#include "internamentum.h"


/* ========================================================================
 * CONSTANTAE
 * ======================================================================== */

#define LIBRARIUM_LECTOR_NUMERUS_MAX    XXXII   /* 32 characteres */
#define LIBRARIUM_LECTOR_VIA_MAX        DXII    /* 512 characteres */


/* ========================================================================
 * TYPI
 * ======================================================================== */

/* LibrariumLector - Status lectoris libri
 */
nomen structura LibrariumLector {
    Piscina* piscina;

    /* Textus carcatus */
    chorda textus;
    character numerus_buffer[LIBRARIUM_LECTOR_NUMERUS_MAX];
    character via_buffer[LIBRARIUM_LECTOR_VIA_MAX];

    /* Paginatio (modus paginae) */
    PaginariumResultus paginarium;
    s32 pagina_currens;
    s32 paginae_totales;

    /* Modus sententiae */
    b32 modus_sententiae;
    SententiaPaginariumResultus* sententia_resultus;
    s32 sententia_currens;

    /* Cache */
    s32 cache_latitudo;
    s32 cache_altitudo;
    b32 est_carcatus;
} LibrariumLector;


/* ========================================================================
 * FUNCTIONES - CREATIO
 * ======================================================================== */

/* Creare lector novum
 *
 * piscina: Arena pro allocationibus
 *
 * Redde: Lector novus, vel NIHIL si error
 */
LibrariumLector*
librarium_lector_creare(
    Piscina* piscina);


/* ========================================================================
 * FUNCTIONES - CARCARE
 * ======================================================================== */

/* Carcare textum libri ex file
 *
 * lector:        Lector
 * via:           Via ad filum textus (null-terminated)
 * numerus_libri: ID libri pro persistentia (null-terminated)
 *
 * Redde: VERUM si successus, FALSUM si error
 */
b32
librarium_lector_carcare(
    LibrariumLector*    lector,
    constans character* via,
    constans character* numerus_libri);


/* ========================================================================
 * FUNCTIONES - PAGINATIO
 * ======================================================================== */

/* Paginare textum (cum caching)
 *
 * Recalculat solum si dimensiones mutatae.
 *
 * lector:   Lector cum textu carcato
 * latitudo: Latitudo in characteribus
 * altitudo: Altitudo in lineis
 */
vacuum
librarium_lector_paginare(
    LibrariumLector* lector,
    s32              latitudo,
    s32              altitudo);


/* ========================================================================
 * FUNCTIONES - NAVIGATIO
 * ======================================================================== */

/* Saltare ad paginam proximam */
vacuum
librarium_lector_pagina_proxima(
    LibrariumLector* lector);

/* Saltare ad paginam priorem */
vacuum
librarium_lector_pagina_prior(
    LibrariumLector* lector);

/* Saltare ad paginam specificam
 *
 * lector: Lector
 * pagina: Index paginae (0-indexed, clamped ad limites)
 */
vacuum
librarium_lector_pagina_saltare(
    LibrariumLector* lector,
    s32              pagina);


/* ========================================================================
 * FUNCTIONES - MODUS SENTENTIAE
 * ======================================================================== */

/* Toggle modus sententiae (inter paginae et sententiae) */
vacuum
librarium_lector_toggle_modus_sententiae(
    LibrariumLector* lector);

/* Saltare ad sententiam proximam */
vacuum
librarium_lector_sententia_proxima(
    LibrariumLector* lector);

/* Saltare ad sententiam priorem */
vacuum
librarium_lector_sententia_prior(
    LibrariumLector* lector);

/* Paginare sententias (cum caching)
 *
 * lector:   Lector cum textu carcato
 * latitudo: Latitudo in characteribus
 * altitudo: Altitudo in lineis
 */
vacuum
librarium_lector_sententia_paginare(
    LibrariumLector* lector,
    s32              latitudo,
    s32              altitudo);

/* Verificare si in modo sententiae */
b32
librarium_lector_in_modo_sententiae(
    LibrariumLector* lector);

/* Obtinere sententia currens (0-indexed) */
s32
librarium_lector_sententia_currens(
    LibrariumLector* lector);

/* Obtinere numerus sententiarum totalis */
s32
librarium_lector_sententiae_totales(
    LibrariumLector* lector);

/* Obtinere paginam word-wrapped pro sententia currens
 *
 * Redde: Pointer ad SententiaPagina, vel NIHIL si error
 */
SententiaPagina*
librarium_lector_sententia_pagina_obtinere(
    LibrariumLector* lector);


/* ========================================================================
 * FUNCTIONES - ACCESSORES
 * ======================================================================== */

/* Obtinere pagina currens (0-indexed) */
s32
librarium_lector_pagina_currens(
    LibrariumLector* lector);

/* Obtinere numerus paginarum totalis */
s32
librarium_lector_paginae_totales(
    LibrariumLector* lector);

/* Obtinere paginam specificam
 *
 * Redde: Pointer ad paginam, vel NIHIL si invalidus
 */
PaginariumPagina*
librarium_lector_pagina_obtinere(
    LibrariumLector* lector,
    s32              index);

/* Obtinere textum carcatum */
chorda
librarium_lector_textus(
    LibrariumLector* lector);

/* Verificare si textus carcatus */
b32
librarium_lector_habet_textum(
    LibrariumLector* lector);

/* Obtinere numerus libri currens (ID) */
constans character*
librarium_lector_numerus(
    LibrariumLector* lector);


/* ========================================================================
 * FUNCTIONES - PERSISTENTIA
 * ======================================================================== */

/* Salvare progressum lectionis ad repositorium
 *
 * Salvat pagina_currens ad entitas "LibrumProgressus::{numerus}"
 *
 * lector: Lector
 * repo:   Repositorium entitatum (potest esse NIHIL)
 */
vacuum
librarium_lector_salvare_progressum(
    LibrariumLector*     lector,
    EntitasRepositorium* repo);

/* Carcare progressum lectionis ex repositorio
 *
 * Carcat pagina_currens ex entitas "LibrumProgressus::{numerus}"
 *
 * lector: Lector
 * repo:   Repositorium entitatum (potest esse NIHIL)
 * intern: Internamentum chordarum (pro lookup)
 */
vacuum
librarium_lector_carcare_progressum(
    LibrariumLector*     lector,
    EntitasRepositorium* repo,
    InternamentumChorda* intern);


#endif /* LIBRARIUM_LECTOR_H */
