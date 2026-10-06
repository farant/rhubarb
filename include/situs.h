/* situs.h - Praedicata geometrica EXACTA: orientatio, sectio, contactus
 *
 * Decisiones geometricae (supra/infra, sectio, coplanaritas, motus
 * trianguli) exacte super coordinatas rationales (fractio): signum
 * determinantis, numquam fluitantes. Casus DEGENERES (collineares,
 * coplanares, contactus in extremo, proiectio non generica) NOMINANTUR
 * et numquam tacite in partem unam cadunt: vocans decernit (e.g.
 * directionem proiectionis mutat, motum refutat).
 *
 * MEMORIA: piscina est alveus temporarius. Praedicata quae signum aut
 * genus reddunt piscinam relinquunt ut acceperunt (notare ...
 * reficere), ergo in ansis calidis (O(n^2) paria segmentorum) memoria
 * non crescit.
 * Puncta ipsa in memoria vocantis manent.
 *
 * USUS:
 *   Punctum a = situs_punctum(0, 0, 0);  ...
 *   si (situs_triangulum_segmentum(a, c, b, p, q, piscina)
 *           != SITUS_DISIUNCTA) refutare motum;
 *
 * Vide lib/situs.worklog.md.
 */
/* <aedilis corpus="lib/situs.c"/> */
#ifndef SITUS_H
#define SITUS_H

#include "latina.h"
#include "piscina.h"
#include "fractio.h"

/* puncta: coordinatae rationales; integra (cancelli) per
 * denominatorem 1 */
nomen structura {
    Fractio x;
    Fractio y;
} PunctumPlani;

nomen structura {
    Fractio x;
    Fractio y;
    Fractio z;
} Punctum;

/* constructores e coordinatis integris (casus frequentissimus) */
PunctumPlani
situs_punctum_plani (
    s64 x,
    s64 y);

Punctum
situs_punctum (
    s64 x,
    s64 y,
    s64 z);

b32
situs_puncta_aequalia (
    Punctum a,
    Punctum b);


/* ==================================================
 * Orientatio
 * ================================================== */

/* signum (b - a) x (c - a): +1 sinistrorsum, 0 collineares, -1
 * dextrorsum */
s32
situs_orientatio_plana (
    PunctumPlani  a,
    PunctumPlani  b,
    PunctumPlani  c,
         Piscina* piscina);

/* signum det[b - a, c - a, d - a] (productum triplex): +1 si d ex parte
 * normalis (b - a) x (c - a), 0 coplanares, -1 aliter */
s32
situs_orientatio (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  d,
     Piscina* piscina);

/* valor ipse determinantis (sexies volumen tetraedri signatum): TESTIS
 * signi, verificabilis sine hac bibliotheca. Hic piscina retinetur
 * (effectus in ea vivit). */
Fractio
situs_volumen_sexies (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  d,
     Piscina* piscina);


/* ==================================================
 * Contactus
 * ================================================== */

nomen enumeratio {
    SITUS_DISIUNCTA  = ZEPHYRUM,   /* nullum punctum commune */
    SITUS_SECANT     = I,          /* interiora in puncto uno */
    SITUS_TANGUNT    = II          /* contactus degener: extremum in
                                    * altero, collineares superpositae,
                                    * coplanaritas cum contactu */
} SitusContactus;

/* segmenta [a,b] et [c,d] in plano (segmentum nullum, a == b, ut
 * punctum tractatur) */
SitusContactus
situs_segmenta_plana (
    PunctumPlani  a,
    PunctumPlani  b,
    PunctumPlani  c,
    PunctumPlani  d,
         Piscina* piscina);

/* segmenta in spatio: polygonum se ipsum secat? Non coplanaria numquam
 * se tangunt. */
SitusContactus
situs_segmenta (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  d,
     Piscina* piscina);

/* triangulum clausum [t0,t1,t2] et segmentum [p,q]. SECANT: segmentum
 * interius trianguli proprie transfigit; TANGUNT: contactus in margine,
 * vertice, extremo segmenti, aut coplanaritas cum contactu (triangulum
 * degener - collineare - semper TANGUNT aut DISIUNCTA). Motus trianguli
 * (AB -> AC + CB) licitus est sse DISIUNCTA pro omni segmento
 * alieno. */
SitusContactus
situs_triangulum_segmentum (
     Punctum  t0,
     Punctum  t1,
     Punctum  t2,
     Punctum  p,
     Punctum  q,
     Piscina* piscina);


/* ==================================================
 * Transitus: supra et infra
 * ================================================== */

/* Proiectio parallela secundum v (vector non nullus; spectator in
 * directione +v, ergo punctum maioris v.P propius). SECANT:
 * proiectiones [a,b] et [c,d] proprie se secant; *superius = 0 si
 * [a,b] propius, 1 si [c,d]; *signum = signum transitus, regula
 * dextrae: signum det[o, u, v] (o supra, u infra, segmentis orientatis
 * a->b, c->d). TANGUNT: proiectio non generica (contactus in
 * proiectione non proprius) aut segmenta in spatio se secant - vocans
 * v mutat aut polygonum refutat. Exitus solum si SECANT scribuntur.
 * Sine coordinatis proiectis: omnia per producta triplicia cum v. */
SitusContactus
situs_transitus (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  d,
     Punctum  v,
     Piscina* piscina,
         s32* superius,
         s32* signum);

#endif /* SITUS_H */
