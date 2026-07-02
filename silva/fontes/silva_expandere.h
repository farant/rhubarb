/* silva_expandere.h - Expansor silvae (Phase 2)
 *
 * Chunk A: acta (journal) + directivae + definitiones macro.
 *
 * Decisiones (silva-spec-v2.md §8.1/S10, simulatio ①):
 *   - ACTA pro segmentis: historia status macro = index appendix-solus
 *     eventorum (fons, linea, ±definitio, conditio_id). Tabula viva =
 *     via defalta (default track). macros_ad_lineam = replay actorum
 *     in piscinam vocantis. (TabulaDispersa exemplar non habet - R2.)
 *   - Directiva = lexema CANCELLUM cum initium_lineae VERO: proprietas
 *     LOCALIS (classis defectus arbor2 hic irrepraesentabilis).
 *   - Corpus macro = LAMINA fluxus lexematum fontis: def-site
 *     provenientia gratis (lexemata immutabilia et communia).
 *   - Functio-similis si '(' IMMEDIATE post titulum (adiacentia cruda:
 *     byte_offset paren == byte_offset tituli + longitudo tituli).
 *   - conditio_id reservatum (0 = incondicionale); rami in Chunk D.
 */

#ifndef SILVA_EXPANDERE_H
#define SILVA_EXPANDERE_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "silva_token.h"

/* ==================================================
 * Definitio macro
 * ================================================== */

nomen structura {
    chorda*  titulus;       /* nomen macro */
    b32      est_functio;   /* '(' adiacens post titulum */
    b32      est_variadica; /* ... in parametris (C99 - signatum) */
    Xar*     parametra;     /* Xar de chorda* (tituli parametrorum); NIHIL si nulla */
    Xar*     corpus;        /* Xar de SilvaToken* - lamina fluxus fontis */
    s32      fons_index;    /* -1 si ex API */
    i32      linea_def;     /* 0 si ignota */
    b32      ex_api;
} SilvaMacroDef;


/* ==================================================
 * Acta (journal) - historia status macro
 * ================================================== */

nomen enumeratio {
    SILVA_EVENTUM_DEFINITIO = 0,
    SILVA_EVENTUM_DELETIO
} SilvaEventumGenus;

nomen structura {
    SilvaEventumGenus genus;
    s32               fons_index;
    i32               linea;
    chorda*           titulus;
    SilvaMacroDef*    def;          /* NIHIL pro DELETIO */
    i32               conditio_id;  /* 0 = incondicionale (rami: Chunk D) */
} SilvaEventum;


/* ==================================================
 * Contextus expansionis
 * ================================================== */

nomen structura {
    Piscina*        piscina;
    Xar*            fontes;   /* Xar de SilvaFons (per valorem) */
    TabulaDispersa* macros;   /* status vivus - via defalta */
    Xar*            acta;     /* Xar de SilvaEventum (per valorem) */
} SilvaExpansio;

SilvaExpansio*
silva_expansio_creare (
    Piscina* piscina);

/* Addere fontem; reddit indicem eius */
s32
silva_fons_addere (
    SilvaExpansio*      exp,
    constans character* via,
    b32                 est_syntheticus);


/* ==================================================
 * Processio directivarum (Chunk A: define/undef)
 *
 * Ambulat fluxum attachatum (ex silva_lexare); directivas
 * define/undef recognoscit, tabulam vivam et acta renovat.
 * Reddit fluxum RELIQUUM (lexemata non in directivis consumptis);
 * directivae ipsae in *directivae_out (Xar de Xar de SilvaToken* -
 * una linea logica per introitum) si non NIHIL - pro nodis
 * directivarum postea.
 * ================================================== */

Xar*
silva_expansio_definitiones_colligere (
    SilvaExpansio* exp,
    Xar*           lexemata,
    Xar**          directivae_out);

/* Quaerere definitionem in tabula viva; NIHIL si absens */
SilvaMacroDef*
silva_expansio_quaerere (
    SilvaExpansio* exp,
    chorda         titulus);

/* Status macro ad (fons, linea): replay actorum in piscinam datam.
 * Tabula chorda->SilvaMacroDef*. Eventa ordine appendicis replicantur
 * usque ad primum eventum eiusdem fontis cuius linea >= linea data. */
TabulaDispersa*
silva_expansio_macros_ad_lineam (
    SilvaExpansio* exp,
    Piscina*       piscina,
    s32            fons_index,
    i32            linea);


/* ==================================================
 * Chunk B - Generationes expansionis
 *
 * Generatio una = gradus expansionis C89 plenus (S13):
 * argumenta PLENE prae-expansa intra gradum, substitutio,
 * caecatio extensa (Prosser); RESCAN = generatio proxima.
 * Terminatio per caecationes (finis semanticus, non cap).
 * ================================================== */

/* Una generatio: fluxus novus; *mutatum_out VERUM si quid expansum */
Xar*
silva_expansio_generatio (
    SilvaExpansio* exp,
    Xar*           lexemata,
    b32*           mutatum_out);

/* Expansio ad fixum: generationes iteratae dum mutationes.
 * strata_out (si non NIHIL): Xar de Xar* - exitus CUIUSQUE
 * generationis mutantis, ordine (strata modeli stratorum). */
Xar*
silva_expansio_expandere (
    SilvaExpansio* exp,
    Xar*           lexemata,
    Xar**          strata_out);

#endif /* SILVA_EXPANDERE_H */
