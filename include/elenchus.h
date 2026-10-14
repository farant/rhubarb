/* elenchus.h - MENU CONTEXTUALE (agenda-plan M2b): tabella optionum
 * supra widget, forma Mac classica - margo, umbra plana, res electa
 * colore selectionis, claves ad dextrum, res non praesto obscurae,
 * lineae divisoriae.
 *
 * Forma domus (pictor 'palette', dimissio S5c): hospes campum
 * transientem aspectus="popover" in organo suo declarat (positum =
 * menu apertum). elenchus_componere subarborem reddit - nodus
 * "elenchus" (PARTES_DIALOGUS) cum liberis "elenchus.<k>"
 * (PARTES_OPTIO) - quam hospes filium ULTIMUM radicis addit
 * (destinatio liberos ultimum primum tangit). Res habiles actionem
 * hospitis et 'claudit' ferunt: actio successa menu per dimissionem
 * claudit; Esc et ictus extra menu item per dimissionem. Res
 * obscurae actionem sine 'claudit' ferunt: hospes ictum deglutit,
 * menu manet.
 *
 * Status (quae res, quae electa, ubi) hospitis est, in campis
 * insulae; elenchus purus: mensurae, arbor, figura, claves.
 */
#ifndef ELENCHUS_H
#define ELENCHUS_H

/* <aedilis corpus="lib/elenchus.c"/> */

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "componens.h"
#include "mandatum.h"
#include "eventus.h"

/* res una menu */
nomen structura {
    constans character* titulus;   /* id hospitis ("calendarium.
                                    * perficere") */
    constans character* textus;    /* "Complete / reopen item" */
    constans character* clavis;    /* "x" ad dextrum; NIHIL = nulla */
                   b32  habilis;   /* FALSUM: obscura, non eligitur */
                   b32  divisor;   /* linea divisoria ANTE hanc */
} ElenchusRes;

nomen structura {
    constans ElenchusRes* res;
                     i32  numerus;
                     s32  electa;   /* index illuminatus; -1 nulla */
} Elenchus;

/* exitus clavis */
nomen enumeratio {
    ELENCHUS_NIHIL = ZEPHYRUM,   /* clavis non menu (hospes tamen
                                  * deglutit dum apertum) */
    ELENCHUS_MOTUM,              /* electa mutata */
    ELENCHUS_ELECTUM             /* Reditus in re habili */
} ElenchusExitus;

/* fines menu (pixela, spatium hospitis): latitudo ex textu
 * longissimo et clave, altitudo ex rebus et divisoribus; angulus
 * sinister superus ad ancoram, reflexus sinistrorsum / sursum si
 * aream excederet, deinde intra aream coactus (Mac) */
Fines
elenchus_fines (
    constans Elenchus* e,
              Punctum  ancora,
                Fines  area);

/* subarbor ad fines: "elenchus" (PARTES_DIALOGUS) et "elenchus.<k>"
 * (PARTES_OPTIO, actio data; claudit solum si habilis); divisores
 * spatium tantum sunt, non nodi */
Componens*
elenchus_componere (
       constans Elenchus* e,
                   Fines  fines,
      constans character* actio,
                 Piscina* piscina,
     InternamentumChorda* intern);

/* index rei ex nodo ("elenchus.3" -> 3); -1 si nodus alius */
s32
elenchus_index (
    constans Componens* c);

/* <purus/> FiguraFn pro PARTES_DIALOGUS: menu totum (res in liberis
 * non pinguntur seorsum); ctx = constans Elenchus* hospitis, vivus
 * dum pingitur */
vacuum
elenchus_figura (
    constans Componens* c,
               Mandata* md,
                   i32  thema,
                vacuum* ctx);

/* Sursum / Deorsum (et k / j): electa movetur, obscuras saltat,
 * circumit; electa -1 + Deorsum = prima habilis. Reditus: ELECTUM si
 * electa habilis. *electa = index novus. */
ElenchusExitus
elenchus_clavem_agere (
     constans Elenchus* e,
      constans Eventus* ev,
                   s32* electa);

#endif /* ELENCHUS_H */
