/* membrum__praecepta.h - TABULA PRAECEPTORUM widget (nomenclator-plan
 * U2; ex calendario extractum; genus membrum__ = componens UI
 * compositioni destinatum): praecepta nominata (titulus unicus,
 * descriptio, clavis), visus in quibus valent (larva hospitis),
 * requisita (larva hospitis, ab hospite iudicata), functio.
 *
 * Usus: claves (praecepta_ad_clavem + praecepta_status), invocatio per
 * nomen (globuli, menus), disponibilitas (menu contextuale, palette
 * M3: ABSENS omittitur, OBSCURUM obscuratur). Tabula hospitis constans
 * (finis: titulus NIHIL). Purus: insulam non novit; ctx = contextus
 * hospitis, functionibus et requisitis traditus.
 *
 * Formae clavium: "x" (character productus, sine Imperio aut Super;
 * "X", "<", "[" item), "Enter", "Esc", "Up", "Down" (sagittae, sine
 * modificatoribus), "C-r" (Imperium + littera).
 */
#ifndef MEMBRUM__PRAECEPTA_H
#define MEMBRUM__PRAECEPTA_H

/* <aedilis corpus="lib/membrum__praecepta.c"/> */

#include "latina.h"
#include "eventus.h"

nomen b32 (*PraeceptumFn)(
    vacuum* ctx);

/* requisita (larva) impleta in contextu hospitis? */
nomen b32 (*PraeceptaRequisita)(
       i32  requisita,
    vacuum* ctx);

nomen structura {
    constans character* titulus;      /* "calendarium.perficere" */
    constans character* descriptio;   /* "Complete / reopen item" */
    constans character* clavis;       /* forma (supra); NIHIL = sine
                                       * clave (mus, menus) */
                    i32 visus;        /* larva: visus in quibus valet */
                    i32 requisita;    /* larva; 0 = nulla */
           PraeceptumFn fn;
} Praeceptum;

nomen enumeratio {
    PRAECEPTUM_ABSENS = ZEPHYRUM,   /* non in visu (omittitur) */
    PRAECEPTUM_OBSCURUM,            /* in visu, requisita non impleta */
    PRAECEPTUM_PRAESTO              /* agi potest */
} PraeceptumStatus;

/* tabula sana: campi positi (descriptio, visus, fn), titulus unicus,
 * clavis semel per visum - montatio aliter recusat */
b32
praecepta_verificare (
    constans Praeceptum* tabula);

/* per titulum; NIHIL si ignotum */
constans Praeceptum*
praecepta_invenire (
    constans Praeceptum* tabula,
     constans character* titulus);

/* forma clavis eventui (EVENTUS_CLAVIS_DEPRESSUS) congruit? */
b32
praecepta_clavis_congruit (
    constans character* forma,
      constans Eventus* ev);

/* praeceptum visus cuius clavis eventui congruit; NIHIL si nullum */
constans Praeceptum*
praecepta_ad_clavem (
    constans Praeceptum* tabula,
                    i32  visus,
       constans Eventus* ev);

/* status in visu; requisita NIHIL = omnia impleta */
PraeceptumStatus
praecepta_status (
    constans Praeceptum* p,
                    i32  visus,
     PraeceptaRequisita  requisita,
                 vacuum* ctx);

/* per titulum: si PRAESTO fn(ctx); FALSUM si ignotum, non praesto aut
 * fn FALSUM */
b32
praecepta_invocare (
    constans Praeceptum* tabula,
     constans character* titulus,
                    i32  visus,
     PraeceptaRequisita  requisita,
                 vacuum* ctx);

#endif /* MEMBRUM__PRAECEPTA_H */
