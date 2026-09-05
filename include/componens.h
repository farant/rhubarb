/* componens.h - Arbor componentium: nodus LOGICUS (mores, non formae)
 *
 * Duae arbores numquam fusae (ludus-brainstorm.md XV par. 1): haec
 * fert id, actio, partes, fines, praedicatum, focus, liberos - NULLAM
 * formam. Pingere formas ex hac derivat; destinatio/focus/captura
 * HANC solam ambulant. Componens FINES POSSIDET; pingere AD eos
 * pingit.
 *
 * Chordae (id, actio, titulus) INTERNANTUR per internamentum nodi -
 * vocator literas quaslibet dat, nodus copiam canonicam tenet.
 *
 * USUS:
 *   Componens* r = componens_creare(piscina, intern, "radix", PARTES_NULLUM);
 *   componens_ponere_fines(r, fines);
 *   componens_addere_liberum(r, liberum);
 *   chorda s = componens_scribere_stml(r, piscina, intern, VERUM);
 */

#ifndef COMPONENS_H
#define COMPONENS_H

/* <aedilis corpus="lib/componens.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "mandatum.h"   /* Punctum, Fines */


/* ====================================================================
 * TYPI
 * ================================================================== */

nomen enumeratio {
    PARTES_NULLUM = ZEPHYRUM,
    PARTES_BOTTONE,
    PARTES_CAMPUS,
    PARTES_OPTIO,
    PARTES_INDEX,
    PARTES_ITEM,
    PARTES_DIALOGUS,
    PARTES_TITULUS,
    PARTES_IMAGO,
    PARTES_TABULA,
    PARTES_NUMERUS
} Partes;

/* Praedicatum ictus, escalans: rectangulum (fines) -> polygonum ->
 * masca -> PROPRIUS (componens punctum locale ipse resolvit). */
nomen enumeratio {
    PRAEDICATUM_RECTANGULUM = ZEPHYRUM,
    PRAEDICATUM_POLYGONUM,
    PRAEDICATUM_MASCA,
    PRAEDICATUM_PROPRIUS,
    PRAEDICATUM_NUMERUS
} Praedicatum;

nomen structura Componens {
                  chorda  id;                 /* internata; vacua = anonymus */
                  chorda  actio;              /* nomen actionis; vacua = nulla */
                  chorda  titulus;              /* titulus accessibilis (label) */
                  Partes  partes;
                   Fines  fines;              /* in spatio PARENTIS */
             Praedicatum  praedicatum;
                 Punctum* puncta;             /* polygonum */
                     i32  numerus_punctorum;
                     b32  focusabilis;
                     b32  sectio;             /* praecidere liberos ad fines */
                 Punctum  translatio;         /* liberis applicata */
                     i32  scala;              /* >= I */
                     Xar* liberi;             /* Xar de Componens* */
    structura Componens*  parens;
    InternamentumChorda* intern;             /* pro actio/titulus internandis */
} Componens;


/* ====================================================================
 * AEDIFICATIO
 * ================================================================== */

Componens*
componens_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* id,
                 Partes  partes);

vacuum
componens_addere_liberum (
    Componens* parens,
    Componens* liberum);

vacuum
componens_ponere_fines (
    Componens* c,
        Fines  fines);
vacuum
componens_ponere_actio (
             Componens* c,
    constans character* actio);
vacuum
componens_ponere_titulum (
             Componens* c,
    constans character* titulus);
vacuum
componens_ponere_praedicatum (
      Componens* c,
    Praedicatum  p);
vacuum
componens_ponere_focusabilis (
    Componens* c,
          b32  focusabilis);
vacuum
componens_ponere_sectio (
    Componens* c,
          b32  sectio);

vacuum
componens_ponere_transformatio (
     Componens* c,
           i32  translatio_x,
           i32  translatio_y,
           i32  scala);

i32
componens_numerus_liberorum (
    constans Componens* c);

Componens*
componens_liberum (
    constans Componens* c,
                   i32  index);

Componens*
componens_invenire_per_id (
     Componens* radix,
        chorda  id);


/* ====================================================================
 * SERIALIZATIO ET COMPARATIO
 * ================================================================== */

chorda
componens_scribere_stml (
              Componens* radix,
                Piscina* piscina,
    InternamentumChorda* intern,
                    b32  pulchrum);

Componens*
componens_legere_stml (
     constans character* cstr,
                Piscina* piscina,
    InternamentumChorda* intern);

b32
componens_aequalis (
    constans Componens* a,
    constans Componens* b);

constans character*
partes_titulus (
    Partes p);

Partes
partes_ex_titulo (
    constans character* titulus);


#endif /* COMPONENS_H */
