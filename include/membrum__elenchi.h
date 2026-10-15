/* membrum__elenchi.h - MENU HOSPITIS widget (nomenclator-plan U4; ex
 * calendario extractum; genus membrum__ = componens UI compositioni
 * destinatum): menu contextuale ex tabula hospitis (contextus,
 * praeceptum, divisor), super elenchus (menu ipsum) et
 * membrum__praecepta (textus, claves, disponibilitas).
 *
 * Status in ramo HOSPITIS - campi manifesti eius, scriptore actione
 * hospitis ("elenchus" aspectu popover: dimissio quoque claudit):
 *   elenchus         "<contextus> <x> <y>": larva contextus hospitis
 *                    (decimalis) et ancora (spatio radicis hospitis);
 *                    absens = clausum
 *   elenchus_electa  res illuminata; absens = nulla
 * Res: introitus quorum contextus congruit et praeceptum in visu est
 * (aliter omissi; divisor ad proximum transit); requisita non impleta
 * = obscura. Menu apertum eventa omnia accipit: motus illuminat;
 * ictus / Reditus in re habili claudit et praeceptum agit; obscura
 * deglutitur; Esc et ictus extra claudunt; claves ceterae et duplex
 * deglutiuntur.
 */
#ifndef MEMBRUM__ELENCHI_H
#define MEMBRUM__ELENCHI_H

/* <aedilis corpus="lib/membrum__elenchi.c"/> */

#include "latina.h"
#include "piscina.h"
#include "insula.h"
#include "eventus.h"
#include "componens.h"
#include "elenchus.h"
#include "membrum__praecepta.h"

/* introitus tabulae menu hospitis (finis: titulus NIHIL) */
nomen structura {
                    i32  contextus;   /* larva contextuum hospitis */
     constans character* titulus;     /* praeceptum tabulae hospitis */
                    b32  divisor;     /* linea divisoria ante */
} ElenchiIntroitus;

/* hospes: tabulae, visus currens, requisita et contextus praeceptorum
 * (paratus ab hospite pro eventu) */
nomen structura {
    constans ElenchiIntroitus* introitus;
          constans Praeceptum* praecepta;
                          i32  visus;
           PraeceptaRequisita  requisita;
                       vacuum* ctx;
} ElenchiHospes;

/* tabula sana: omnis titulus in praeceptis - montatio aliter recusat */
b32
elenchi_verificare (
    constans ElenchiIntroitus* introitus,
          constans Praeceptum* praecepta);

/* aperire contextu et ancora; electa nulla */
b32
elenchi_aperire (
            InsulaRamus* ramus,
     constans character* scriptor,
                    i32  contextus,
                Punctum  ancora,
                Piscina* piscina);

b32
elenchi_claudere (
            InsulaRamus* ramus,
     constans character* scriptor,
                Piscina* piscina);

/* apertum? res (in piscina), electa et ancora ex statu */
b32
elenchi_legere (
      constans InsulaRamus* ramus,
    constans ElenchiHospes* hospes,
                   Piscina* piscina,
                  Elenchus* e,
                   Punctum* ancora);

/* eventus dum apertum (vide supra); nodus = componens tactus (res
 * "elenchus.<k>", DIALOGUS, alius); VERUM si consumptus */
b32
elenchi_agere (
               InsulaRamus* ramus,
        constans character* scriptor,
    constans ElenchiHospes* hospes,
         constans Elenchus* e,
        constans Componens* nodus,
          constans Eventus* ev,
                   Piscina* piscina);

#endif /* MEMBRUM__ELENCHI_H */
