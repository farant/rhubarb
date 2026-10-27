/* pensa.h - INDEX COTIDIANUS (pensa K2; project-specs/pensa-spec.md IV):
 * forma = textus ordinatus in SECTIONES (lineis vacuis separatas), quae
 * quaeque per se dedentatur; in sectione linea summa CUM liberis
 * (lineis magis indentatis) = schedula (recursus) et liberi res eius;
 * linea summa SINE liberis = res cotidiana (numquam schedula). Signa =
 * acta in volumine (genus "pensa"), dies + textus rei. Horologium
 * numquam legitur: vocans dies dat (regula "hodie et praeterita" est
 * kalendarii, K3). */

#ifndef PENSA_H
#define PENSA_H

/* <aedilis corpus="lib/pensa.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "volumen.h"
#include "fasti.h"
#include "recursus.h"

/* grex: schedula et res eius */
nomen structura {
       chorda schedula;    /* phrasis ut scripta; vacua = "every day"
                             * implicitus (lineae sine liberis) */
     Recursus* recursus;     /* NIHIL si schedula non legitur */
       chorda  vitium;      /* nuntius recursus_legere; vacuum si legitur */
          Xar* res;         /* chorda: textus rerum (spatia extrema
                             * dempta) */
} PensaGrex;

nomen structura {
    Xar* greges;            /* PensaGrex, ordine formae */
} PensaForma;

/* textus -> forma (purus); NIHIL tantum si piscina deficit */
PensaForma*
pensa_formam_legere (
      chorda  textus,
     Piscina* piscina);

/* textus formae ex libro scribae "pensa" (scriba_liber_textum_legere);
 * vacuus si liber nondum est */
chorda
pensa_textum_legere (
                Volumen* volumen,
    InternamentumChorda* intern,
                Piscina* piscina);

/* res diei d: gregum quorum schedula d continet, ordine formae, textu
 * iterato semel; greges cum vitio nihil dant. Xar de chorda */
Xar*
pensa_res_diei (
    constans PensaForma* forma,
                   Dies  d,
                Piscina* piscina);

/* signa: acta <pensum actio="factum|infectum" dies="YYYY-MM-DD">textus
 * rei</pensum>; ultimum actum per diem et rem vincit */
nomen structura PensaDocumentum PensaDocumentum;

PensaDocumentum*
pensa_aperire (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen);

/* acta nova (etiam aliorum processuum) prius leguntur */
b32
pensa_factum (
    PensaDocumentum* doc,
               Dies  d,
             chorda  res);

/* actum solum si status mutatur; FALSUM si scriptura deficit aut res
 * vacua */
b32
pensa_ponere (
    PensaDocumentum* doc,
               Dies  d,
             chorda  res,
                b32  factum);

#endif /* PENSA_H */
