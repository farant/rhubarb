#ifndef ARBOR_INDEX_H
#define ARBOR_INDEX_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "arbor_syntaxis.h"
#include "arbor_typus.h"

/* ==================================================
 * ARBOR INDEX - Project-Wide Symbol Index
 *
 * Cross-file symbol resolution.
 * Go-to-definition, find-references.
 *
 * PHILOSOPHIA:
 * - Aedificat indicem symbolorum ex multis filis
 * - Tractat nexum (externus/internus/nullus)
 * - Supportat quaestiones: invenire, referentiae, definitio
 *
 * EXEMPLUM:
 *   ArborProiectum* proj = arbor_index_creare(piscina, intern);
 *   arbor_index_addere_filum(proj, "src/main.c");
 *   arbor_index_addere_filum(proj, "src/utils.c");
 *   arbor_index_aedificare(proj);
 *   ArborSymbolum* sym = arbor_index_invenire(proj, nomen_func);
 * ================================================== */

/* ==================================================
 * Types
 * ================================================== */

/* Filum in proiecto */
nomen structura {
    chorda          via;            /* Via relativa */
    chorda          fons;           /* Fons contentum */
    ArborNodus*     radix;          /* AST parsita */
    i64             tempus_mutatum; /* Tempus mutationis */
} ArborFilum;

/* Genera symbolorum */
nomen enumeratio {
    ARBOR_SYMBOLUM_FUNCTIO,
    ARBOR_SYMBOLUM_VARIABILIS,
    ARBOR_SYMBOLUM_TYPUS,
    ARBOR_SYMBOLUM_MACRO,
    ARBOR_SYMBOLUM_ENUM_VALOR
} ArborSymbolumGenus;

/* Nexus (linkage) */
nomen enumeratio {
    ARBOR_NEXUS_EXTERNUS,     /* extern */
    ARBOR_NEXUS_INTERNUS,     /* static */
    ARBOR_NEXUS_NULLUS        /* local/parameter */
} ArborNexus;

/* Referentia situs */
nomen structura {
    ArborFilum*         filum;
    i32                 linea;
    i32                 columna;
    ArborNodus*         nodus;
} ArborIndexReferentia;

/* Symbolum introitus in indice (differens ab ArborSymbolum in arbor_typus.h) */
nomen structura {
    chorda*                titulus;       /* Nomen symboli (non 'nomen' - macro!) */
    ArborSymbolumGenus     genus;
    ArborNexus             nexus;
    ArborFilum*            filum;
    ArborNodus*            declaratio;
    ArborNodus*            definitio;     /* Potest differre a declaratione */
    Xar*                   referentiae;   /* Xar de ArborIndexReferentia */
} ArborIndexSymbolum;

/* Status proiecti (opacus) */
nomen structura ArborProiectum ArborProiectum;

/* ==================================================
 * API
 * ================================================== */

/* Creare proiectum */
ArborProiectum* arbor_index_creare(
    Piscina*              piscina,
    InternamentumChorda*  intern);

/* Addere filum ad proiectum */
vacuum arbor_index_addere_filum(
    ArborProiectum*       proj,
    constans character*   via);

/* Addere folder (cum glob pattern) */
vacuum arbor_index_addere_folder(
    ArborProiectum*       proj,
    constans character*   via,
    constans character*   pattern);  /* e.g., "*.c" */

/* Addere via include */
vacuum arbor_index_addere_via_include(
    ArborProiectum*       proj,
    constans character*   via);

/* Aedificare/reaedificare indicem */
vacuum arbor_index_aedificare(ArborProiectum* proj);

/* Quaestio: invenire symbolum per nomen */
ArborIndexSymbolum* arbor_index_invenire(
    ArborProiectum*       proj,
    chorda*               titulus);

/* Quaestio: invenire omnes referentias */
Xar* arbor_index_referentiae(
    ArborProiectum*       proj,
    chorda*               titulus);

/* Quaestio: ire ad definitionem ad locum */
ArborNodus* arbor_index_definitio_ad_punctum(
    ArborProiectum*       proj,
    constans character*   via,
    i32                   linea,
    i32                   columna);

/* Quaestio: invenire symbolum ad locum */
ArborIndexSymbolum* arbor_index_symbolum_ad_punctum(
    ArborProiectum*       proj,
    constans character*   via,
    i32                   linea,
    i32                   columna);

/* Obtinere nomen genus symboli (pro debugging) */
constans character* arbor_symbolum_genus_nomen(ArborSymbolumGenus genus);

/* Obtinere nomen nexus (pro debugging) */
constans character* arbor_nexus_nomen(ArborNexus nexus);

#endif /* ARBOR_INDEX_H */
