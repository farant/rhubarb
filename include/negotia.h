/* negotia.h - negotia (incepta) et facultates eorum, in volumine
 * vici (project-specs/negotia.md; plan negotia-plan.md T1).
 * VERITAS = acta trium generum (JSON), quodque IMAGO PLENA rei:
 *   "negotium"           {id, titulus, descriptio}
 *   "negotii-facultas"   {id, negotium, titulus, status, corpus}
 *   "negotii-colloquium" {negotium, sessio}
 * Status praesens = actum ultimum per id. Omnis mutatio servata =
 * actum unum = REVISIO una (numquam deletur); mutatio statui
 * praesenti par nihil scribit. Acta OMNIUM replicarum leguntur (res
 * communes, non status localis derivatus).
 * Index in memoria ex cursore: aperire omnia legit, scripturae post
 * appendendum ex cursore renovant, negotia_renovare acta ALIORUM
 * PROCESSUUM per volumen_seq_ultimum invenit (vicus pulsu suo
 * vocat - nihil aliud in arbore scripturas alienas videt). */

#ifndef NEGOTIA_H
#define NEGOTIA_H

/* <aedilis corpus="lib/negotia.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "volumen.h"
#include "moneta.h"

nomen structura Negotia Negotia;

nomen enumeratio {
    NEGOTIA_PROPOSITA = 0,   /* "planned" */
    NEGOTIA_INCHOATA,        /* "partial" */
    NEGOTIA_PERFECTA         /* "shipped" */
} NegotiaStatus;

nomen structura {
    chorda id;               /* ULID */
    chorda titulus;
    chorda descriptio;
       s64 seq;              /* actum ultimum ipsius */
       s64 seq_activitatis;  /* actum ultimum de eo (facultates,
                              * colloquia comprehensa) */
} Negotium;

nomen structura {
    chorda id;               /* ULID */
    chorda negotium;         /* id negotii */
    chorda titulus;
       s32 status;           /* NegotiaStatus */
    chorda corpus;           /* textus liber */
       s64 seq;              /* actum huius revisionis */
    chorda momentum;         /* ISO 8601, ex actu */
} NegotiiFacultas;

/* fons monetae NIHIL = moneta_ulid (probationes: moneta_numerator);
 * NIHIL si volumen legi non potest */
Negotia*
negotia_aperire (
       Volumen* volumen,
    MonetaFons  fons,
        vacuum* fontis_ctx,
       Piscina* piscina);

/* acta nova (etiam aliorum processuum) post cursorem legit; VERUM si
 * qua lecta sunt */
b32
negotia_renovare (
    Negotia* n);

i32
negotia_numerus (
    constans Negotia* n);

/* ordine activitatis (seq_activitatis decrescente); NIHIL extra
 * ambitum */
constans Negotium*
negotia_index (
    constans Negotia* n,
                 i32  k);

/* id plenum aut suffixum unicum; NIHIL si ignotum aut ambiguum
 * (*ambiguum_out VERUM si plura congruunt; ambiguum_out NIHIL licet) */
constans Negotium*
negotia_negotium_invenire (
    constans Negotia* n,
              chorda  id,
                 b32* ambiguum_out);

/* reddit id; chorda vacua si refutatum (titulus vacuus) */
chorda
negotia_negotium_creare (
    Negotia* n,
     chorda  titulus,
     chorda  descriptio);

/* imago plena; par statui praesenti: VERUM, nihil scriptum */
b32
negotia_negotium_mutare (
    Negotia* n,
     chorda  id,
     chorda  titulus,
     chorda  descriptio);

/* NegotiiFacultas (copiae status praesentis), ordine creationis */
Xar*
negotia_facultates (
    constans Negotia* n,
              chorda  negotium,
             Piscina* piscina);

/* ut negotia_negotium_invenire */
constans NegotiiFacultas*
negotia_facultatem_invenire (
    constans Negotia* n,
              chorda  id,
                 b32* ambiguum_out);

/* chorda vacua si refutatum (titulus vacuus, negotium ignotum,
 * status extra NegotiaStatus) */
chorda
negotia_facultatem_creare (
    Negotia* n,
     chorda  negotium,
     chorda  titulus,
        s32  status,
     chorda  corpus);

b32
negotia_facultatem_mutare (
    Negotia* n,
     chorda  id,
     chorda  titulus,
        s32  status,
     chorda  corpus);

/* NegotiiFacultas (copiae), vetustissima prima; NIHIL si ignota */
Xar*
negotia_revisiones (
    constans Negotia* n,
              chorda  facultas,
             Piscina* piscina);

/* nexus negotium -> sessio Claude; idempotens (iterum: VERUM, nihil
 * scriptum); FALSUM si negotium ignotum aut sessio vacua */
b32
negotia_colloquium_adiungere (
    Negotia* n,
     chorda  negotium,
     chorda  sessio);

/* chorda (sessiones), ordine adiunctionis */
Xar*
negotia_colloquia (
    constans Negotia* n,
              chorda  negotium,
             Piscina* piscina);

/* "planned" / "partial" / "shipped"; "?" extra ambitum */
constans character*
negotia_status_titulus (
    s32 status);

/* proposita|inchoata|perfecta aut planned|partial|shipped */
b32
negotia_status_legere (
    chorda  verbum,
       s32* status_out);

#endif /* NEGOTIA_H */
