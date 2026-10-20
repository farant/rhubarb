/* oceanus.h - cliens DNS DigitalOcean (API v2, registra dominii)
 *
 * GET/POST/PUT/DELETE /v2/domains/{dominium}/records[/{id}], JSON per
 * json.h, paginae per links.pages.next (solum si sub basi - clavis ad
 * hospitem alienum numquam mittitur). Vocationes OBSTANT
 * (http_exsequi): contextus instrumenti (bin/oceanus,
 * praeparare_hospitem.sh), non servi.
 *
 * CLAVIS: ~/.rhubarb/digitalocean.clavis (aut via data), modus 0600
 * strictus, praefixum 'dop_v1_' (sanitas, non securitas). Lectio
 * CRUDA (numquam lectiones), capsa in piscina, deleta in
 * oceanus_claudere. Clavis numquam in errore, effusione aut actis -
 * errores API nuntium API solum ferunt.
 *
 * assecurare: registrum (genus, titulus) unum: abest -> creatur;
 * data/ttl differunt -> mutatur; aequale -> nihil scribitur (IDEM).
 * Plura congruentia -> ERROR (ambiguum, nihil tangitur). */
#ifndef OCEANUS_H
#define OCEANUS_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"

nomen structura {
       i64 id;
    chorda genus;      /* "A", "AAAA", "CNAME", ... */
    chorda titulus;    /* "exemplar" ("@" = dominium ipsum) */
    chorda data;       /* "165.227.210.190" */
       i32 ttl;        /* secundae */
} OceanusRegistrum;

nomen structura Oceanus Oceanus;

nomen enumeratio {
    OCEANUS_IDEM = 0,
    OCEANUS_CREATUM,
    OCEANUS_MUTATUM,
    OCEANUS_ERROR
} OceanusExitus;

/* via_clavis NIHIL = ~/.rhubarb/digitalocean.clavis; basis NIHIL =
 * https://api.digitalocean.com (probationes: http://127.0.0.1:N).
 * NIHIL si clavis legi nequit, modus latior quam 0600, aut praefixum
 * deest (causa ad stderr, clavis numquam). */
Oceanus*
oceanus_creare (
    constans character* via_clavis,
    constans character* basis,
               Piscina* piscina);

/* registra omnia dominii (paginae omnes) in exitus (Xar de
 * OceanusRegistrum, a vocante creatum) */
b32
oceanus_registra (
               Oceanus* oceanus,
    constans character* dominium,
                   Xar* exitus);

b32
oceanus_registrum_creare (
                      Oceanus* oceanus,
           constans character* dominium,
    constans OceanusRegistrum* scheda,
                          i64* id);

b32
oceanus_registrum_mutare (
                      Oceanus* oceanus,
           constans character* dominium,
                          i64  id,
    constans OceanusRegistrum* scheda);

b32
oceanus_registrum_delere (
               Oceanus* oceanus,
    constans character* dominium,
                   i64  id);

OceanusExitus
oceanus_assecurare (
               Oceanus* oceanus,
    constans character* dominium,
    constans character* genus,
    constans character* titulus,
    constans character* data,
                   i32  ttl);

/* causa ultima (numquam clavem continet); "" si nulla */
constans character*
oceanus_error (
    Oceanus* oceanus);

/* clavis in memoria deletur */
vacuum
oceanus_claudere (
    Oceanus* oceanus);

#endif /* OCEANUS_H */
