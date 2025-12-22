/* paginarium.h - "Codex Paginationis Textuum"
 *
 * Bibliotheca pro paginatione textuum cum intelligentia paragraphorum
 *
 * PHILOSOPHIA:
 * - Textus Gutenberg habent lineas fixas (~70-80 characteres)
 * - Prosa debet re-involvi ad latitudinem widget
 * - Versus (poesis, indices) debent preservare structuram linearum
 * - Modus versus detegatur per paragraphum, non per paginam
 *
 * HEURISTICA:
 * - Paragraph break = 2+ newlines consecutivae -> una linea vacua
 * - Versus detectio: si 80%+ primarum 5 linearum < 80% latitudinis
 * - Prosa: singulum \n -> spatium, re-involvi ad marginem
 *
 * USUS:
 *   PaginariumConfig config = paginarium_config_defectus();
 *   config.latitudo = 80;
 *   config.altitudo = 25;
 *   PaginariumResultus res = paginarium_paginare(textus, config, piscina);
 *   Pagina* pag = paginarium_pagina_obtinere(&res, 0);
 *   chorda linea_reddita = paginarium_linea_reddere(textus, linea, piscina);
 */

#ifndef PAGINARIUM_H
#define PAGINARIUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

/* ========================================================================
 * CONSTANTAE
 * ======================================================================== */

#define PAGINARIUM_LIMINA_VERSUS_DEFECTUS     LXXX  /* 80% */
#define PAGINARIUM_LINEAE_PROBARE_DEFECTUS    V     /* 5 lineae */
#define PAGINARIUM_LATITUDO_DEFECTUS          LXXX  /* 80 characteres */
#define PAGINARIUM_ALTITUDO_DEFECTUS          XXV   /* 25 lineae */

/* ========================================================================
 * TYPI
 * ======================================================================== */

/* PaginariumConfig - Configuratio paginatoris
 */
nomen structura PaginariumConfig {
    i32 latitudo;              /* Characteres per lineam */
    i32 altitudo;              /* Lineae per paginam */
    i32 limina_versus;         /* Limina percentualis pro detectione versus (0-100) */
    i32 lineae_versus_probare; /* Quot lineas probare pro detectione versus */
} PaginariumConfig;

/* PaginariumLinea - Una linea reddita
 */
nomen structura PaginariumLinea {
    i32 initium;       /* Offset byte in textu fonte */
    i32 finis;         /* Offset byte finis (exclusivus) */
    b32 est_vacua;     /* Verum si linea vacua (paragraph break) */
    b32 modus_versus;  /* Verum si linea in modo versus reddita */
} PaginariumLinea;

/* PaginariumPagina - Una pagina cum lineis
 */
nomen structura PaginariumPagina {
    Xar* lineae;       /* Array de PaginariumLinea */
    i32  initium;      /* Offset byte initii paginae */
    i32  finis;        /* Offset byte finis paginae */
} PaginariumPagina;

/* PaginariumResultus - Resultus paginationis
 */
nomen structura PaginariumResultus {
    Xar* paginae;           /* Array de PaginariumPagina */
    i32  numerus_paginarum; /* Numerus paginarum totalis */
} PaginariumResultus;

/* ========================================================================
 * FUNCTIONES - CONFIGURATIO
 * ======================================================================== */

/* Obtinere configurationem defectus
 */
PaginariumConfig
paginarium_config_defectus(vacuum);

/* ========================================================================
 * FUNCTIONES - PAGINATIO
 * ======================================================================== */

/* Paginare textum integrum
 * "Dividere textum in paginas"
 *
 * textus:  Textus fonte
 * config:  Configuratio paginatoris
 * piscina: Arena pro allocationibus
 *
 * Redde: Resultus cum paginis
 */
PaginariumResultus
paginarium_paginare(
    chorda           textus,
    PaginariumConfig config,
    Piscina*         piscina);

/* ========================================================================
 * FUNCTIONES - ACCESSUS
 * ======================================================================== */

/* Obtinere paginam specificam (0-indexed)
 *
 * resultus: Resultus paginationis
 * index:    Index paginae
 *
 * Redde: Pointer ad paginam, vel NIHIL si index invalidus
 */
PaginariumPagina*
paginarium_pagina_obtinere(
    PaginariumResultus* resultus,
    i32                 index);

/* Obtinere lineam specificam de pagina (0-indexed)
 *
 * pagina: Pagina
 * index:  Index lineae
 *
 * Redde: Pointer ad lineam, vel NIHIL si index invalidus
 */
PaginariumLinea*
paginarium_linea_obtinere(
    PaginariumPagina* pagina,
    i32               index);

/* Numerus linearum in pagina
 */
i32
paginarium_pagina_numerus_linearum(
    PaginariumPagina* pagina);

/* ========================================================================
 * FUNCTIONES - REDDITIO
 * ======================================================================== */

/* Reddere lineam ad chordam
 * "Transformare lineam fontem ad textum reddendum"
 *
 * In modo prosa: \n singulum -> spatium
 * In modo versus: preservare ut est
 * Linea vacua: redde chordam vacuam
 *
 * textus:  Textus fonte originalis
 * linea:   Linea paginata
 * piscina: Arena pro allocatio chordae
 *
 * Redde: Chorda allocata cum textu reddendo
 */
chorda
paginarium_linea_reddere(
    chorda           textus,
    PaginariumLinea* linea,
    Piscina*         piscina);

#endif /* PAGINARIUM_H */
