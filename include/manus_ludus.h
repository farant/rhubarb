/* manus_ludus.h - manus IN PROCESSU super dispensatorem
 *
 * Transportus nativus: eadem verba ac manus.h (premere, clavem,
 * existit, focus), SYNCHRONA - nulla asynchronia, ergo nulla mora.
 * Selectores super arborem LOGICAM: '#id', '[partes=x]',
 * '[actio=x]', '[titulus=x]'. Tempus manus per exspectare solum
 * procedit (et per gradum post eventum quemque) - horologium
 * nullum. Unificatio sub manus.h ut transportus alter: dilatio
 * nominata.
 */

#ifndef MANUS_LUDUS_H
#define MANUS_LUDUS_H

/* <aedilis corpus="lib/manus_ludus.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "mandatum.h"
#include "componens.h"
#include "dispensator.h"


/* ==================================================
 * Typi
 * ================================================== */

nomen structura {
     Dispensator* d;
             s64  tempus;
             s64  gradus_ms;     /* inter eventus manus */
         Piscina* piscina;
          chorda  causa;
} ManusLudus;


/* ==================================================
 * Vita et selectores
 * ================================================== */

ManusLudus*
manus_ludus_creare (
        Piscina* piscina,
    Dispensator* d);

/* NIHIL si selector malus (causa nominat) aut nihil congruit */
Componens*
manus_ludus_invenire (
            ManusLudus* m,
    constans character* selector);

/* Punctum locale componentis in spatium schirmi: inversio exacta
 * transformationis destinationis (fines, translatio, scala per
 * gradum). */
Punctum
manus_ludus_ad_schirmum (
     ManusLudus* m,
      Componens* c,
        Punctum  locale);


/* ==================================================
 * Actus
 * ================================================== */

b32
manus_ludus_premere (
            ManusLudus* m,
    constans character* selector);

b32
manus_ludus_premere_ad (
     ManusLudus* m,
            i32  x,
            i32  y);

b32
manus_ludus_movere (
     ManusLudus* m,
            i32  x,
            i32  y);

/* Ictus: depressus in puncta[0], motus per cetera, liberatus in
 * ultimo - omnia in spatio locali componentis. n >= I. */
b32
manus_ludus_trahere (
            ManusLudus* m,
    constans character* selector,
      constans Punctum* puncta,
                   i32  n);

b32
manus_ludus_clavem (
     ManusLudus* m,
      character  typus,
            i32  modificantes);

/* Tempus procedit; pulsus dispensatori (sedes quietis). */
vacuum
manus_ludus_exspectare (
     ManusLudus* m,
            s64  ms);


/* ==================================================
 * Lectio
 * ================================================== */

b32
manus_ludus_existit (
            ManusLudus* m,
    constans character* selector);

chorda
manus_ludus_focus (
    ManusLudus* m);

chorda
manus_ludus_causa (
    constans ManusLudus* m);

#define CREDO_MANUS_LUDUS_EXISTIT(m, sel) \
    CREDO_VERUM(manus_ludus_existit((m), (sel)))
#define CREDO_MANUS_LUDUS_ABEST(m, sel) \
    CREDO_FALSUM(manus_ludus_existit((m), (sel)))
#define CREDO_MANUS_LUDUS_FOCUS(m, id) \
    CREDO_CHORDA_AEQUALIS_LITERIS(manus_ludus_focus(m), (id))

#endif /* MANUS_LUDUS_H */
