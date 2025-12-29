/* capsula_caudae.h - Tail-Appended Asset Embedding Library
 *
 * Bibliotheca pro embedding files in caudae binarii (binary tail).
 * Similis capsula.h sed assets sunt appended post compilationem.
 *
 * Nomen ex Latina "cauda" (tail) - assets vivunt in cauda binarii.
 *
 * Differentia a capsula:
 *   - capsula: assets compiled into binary as static data
 *   - capsula_caudae: assets appended to binary post-compilation
 *
 * Vantages:
 *   - Nullum limitem compilatoris (can embed gigabytes)
 *   - Eadem format binaria interna (header, TOC, compressed data)
 *   - API similis capsula pro facile switching
 *
 * Binary layout:
 *   ┌─────────────────────────────┐
 *   │ Original executable         │
 *   ├─────────────────────────────┤ ← asset_offset
 *   │ CAPS header (12 bytes)      │
 *   │ TOC entries (20 bytes each) │
 *   │ String table                │
 *   │ Compressed file data        │
 *   ├─────────────────────────────┤
 *   │ Footer (16 bytes)           │ ← offset(8) + magic "CAPSCLDA"(8)
 *   └─────────────────────────────┘
 *
 * USUS:
 *   CapsulaCaudae* cap = capsula_caudae_aperire(argv[0], piscina);
 *   si (cap == NIHIL) { ... error ... }
 *   CapsulaFructus res = capsula_caudae_legere(cap, "librarium.stml", piscina);
 *   si (res.status == CAPSULA_OK) { ... use res.datum ... }
 */

#ifndef CAPSULA_CAUDAE_H
#define CAPSULA_CAUDAE_H

#include "latina.h"
#include <stdio.h>   /* FILUM (FILE) - must be after latina.h */
#include "chorda.h"
#include "piscina.h"
#include "capsula.h"  /* Pro CapsulaStatus, CapsulaFructus, CapsulaIndexum */


/* ========================================================================
 * CONSTANTIAE
 * ======================================================================== */

/* Magic footer: "CAPSCLDA" (CAPSula CauDA) */
#define CAPSULA_CAUDAE_MAGICA_0  0x53504143  /* "CAPS" */
#define CAPSULA_CAUDAE_MAGICA_1  0x4144434C  /* "LCDA" (reversed for LE) */

#define CAPSULA_CAUDAE_FOOTER_MENSURA  16  /* 8 bytes offset + 8 bytes magic */


/* ========================================================================
 * TYPI - RUNTIME STRUCTURES
 * ======================================================================== */

/* Opened tail capsule (runtime state) */
nomen structura {
    FILUM*           filum;            /* Open file handle to binary */
    i64              asset_offset;     /* Where assets start in file */
    i32              numerus_filorum;  /* Number of files */
    CapsulaIndexum*  index;            /* Parsed TOC array */
    Piscina*         piscina;          /* Arena used for parsing */
} CapsulaCaudae;


/* Iterator for walking all files */
nomen structura {
    CapsulaCaudae* capsula;
    i32            positus;            /* Current position (1-indexed after proximus) */
} CapsulaCaudaeIter;


/* ========================================================================
 * FUNCTIONES - APERIRE / CLAUDERE
 * ======================================================================== */

/* Aperire capsulam ex cauda binarii
 *
 * via_binarii: Path to binary (typically argv[0])
 * piscina:     Arena for allocating parsed index
 *
 * Redde: CapsulaCaudae pointer, vel NIHIL si error
 *
 * Nota: Call capsula_caudae_claudere() when done to close file handle
 */
CapsulaCaudae*
capsula_caudae_aperire(
    constans character* via_binarii,
    Piscina*            piscina);


/* Claudere capsulam et file handle
 *
 * capsula: Opened capsule to close
 *
 * Nota: Index memoria vivit in piscina, hoc solum claudit file
 */
vacuum
capsula_caudae_claudere(CapsulaCaudae* capsula);


/* ========================================================================
 * FUNCTIONES - LEGERE
 * ======================================================================== */

/* Legere file per path
 *
 * capsula: Opened tail capsule
 * via:     File path (C string, null-terminated)
 * piscina: Arena for decompressed data
 *
 * Redde: CapsulaFructus with status and data
 *
 * Nota: Uses same CapsulaFructus type as capsula.h for API compatibility
 */
CapsulaFructus
capsula_caudae_legere(
    CapsulaCaudae*      capsula,
    constans character* via,
    Piscina*            piscina);


/* Legere file per chorda path */
CapsulaFructus
capsula_caudae_legere_chorda(
    CapsulaCaudae* capsula,
    chorda         via,
    Piscina*       piscina);


/* ========================================================================
 * FUNCTIONES - QUAERERE
 * ======================================================================== */

/* Verificare si file existit
 *
 * capsula: Opened tail capsule
 * via:     File path to check
 *
 * Redde: VERUM si existit
 */
b32
capsula_caudae_habet(
    CapsulaCaudae*      capsula,
    constans character* via);


/* Invenire index entry per path (vel NIHIL si non inventum) */
CapsulaIndexum*
capsula_caudae_invenire(
    CapsulaCaudae*      capsula,
    constans character* via);


/* ========================================================================
 * FUNCTIONES - ITERATIO
 * ======================================================================== */

/* Obtinere numerum filorum */
i32
capsula_caudae_numerus(CapsulaCaudae* capsula);


/* Obtinere index entry per position (0-indexed) */
CapsulaIndexum*
capsula_caudae_indexum(CapsulaCaudae* capsula, i32 index);


/* Creare iterator */
CapsulaCaudaeIter
capsula_caudae_iter(CapsulaCaudae* capsula);


/* Avanzare ad proximum file
 *
 * Redde: VERUM si est proximum, FALSUM si finis
 *
 * Post call, access current entry via:
 *   CapsulaIndexum* curr = capsula_caudae_indexum(iter->capsula, iter->positus - 1);
 */
b32
capsula_caudae_iter_proximus(CapsulaCaudaeIter* iter);


/* ========================================================================
 * FUNCTIONES - UTILITATES
 * ======================================================================== */

/* Verificare si binary habet assets in cauda
 *
 * via_binarii: Path to binary to check
 *
 * Redde: VERUM si habet valid footer
 */
b32
capsula_caudae_habet_assets(constans character* via_binarii);


#endif /* CAPSULA_CAUDAE_H */
