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
            s32  x,
            s32  y);

b32
manus_ludus_movere (
     ManusLudus* m,
            s32  x,
            s32  y);

/* Ictus: depressus in puncta[0], motus per cetera, liberatus in
 * ultimo - omnia in spatio locali componentis. n >= I. */
b32
manus_ludus_trahere (
            ManusLudus* m,
    constans character* selector,
      constans Punctum* puncta,
                   i32  n);

/* Clavis pressa et liberata. modificantes = vexilla ut fenestra ea
 * fert (MOD_* + latera, e.g. MOD_SHIFT | MOD_SHIFT_SINISTER). Campi
 * vocabularii (clavis logica, runa, codex, actio) ex typo implentur
 * (eventus A5). */
b32
manus_ludus_clavem (
     ManusLudus* m,
      character  typus,
            i32  modificantes);


/* Textum scribere ut fons verus (fenestra, terminalis): pro quoque
 * charactere imprimibili DEPRESSUS + TEXTUS COMMISSUM (eodem tempore,
 * origo SCRIPTA) + LIBERATUS - consumptor qui ambo legit litteram bis
 * videt, ut in usu. Characteres regiminis (< 0x20, 0x7F): clavis sola
 * (fontes veri textum pro eis non mittunt). Contentum ex piscina
 * manus (notarius id servare potest). */
b32
manus_ludus_scribere (
             ManusLudus* m,
     constans character* textus);


/* ==================================================
 * Iteratio plagulae notatae (eventus A6b; spec D6)
 * ================================================== */

nomen enumeratio {
    MANUS_ITERATIO_CRUDA = ZEPHYRUM,  /* positiones ut notatae */
    MANUS_ITERATIO_SEMANTICA          /* scopus + locale -> positio
                                       * ex arbore PRAESENTI */
} ManusIteratio;

/* Divergentia: eventus muris cuius positio notata NUNC alium
 * componentem tangit quam scopus notatus (vacuus: nullus). */
nomen structura {
       i32 index;
    chorda scopus_notatus;
    chorda scopus_crudus;
} ManusDivergentia;

/* Plagulam notatam (Xar de EventusNotatum) dispensatori tradere.
 * CRUDA: eventus ut notati (destinatio iterum currit). SEMANTICA:
 * eventus muris cum scopo ad componentem eius id in arbore praesenti
 * diriguntur - positio = scopus_x/y per manus_ludus_ad_schirmum;
 * scopus absens -> positio cruda. DIVERGENTIAE in modo utroque
 * nuntiantur (ante traditionem cuiusque eventus): positio cruda contra
 * scopum notatum. Redde numerum divergentiarum; divergentiae (si non
 * NIHIL) Xar de ManusDivergentia impletur. */
i32
manus_ludus_iterare (
       ManusLudus* m,
     constans Xar* notata,
    ManusIteratio  modus,
              Xar* divergentiae);

/* Traditio eventus iterati (eventus B6b): via per quam quisque eventus
 * ad dispensatorem it. NIHIL = directe (dispensator_tractare);
 * transitus terminalis eum in octetos codificat et per rivum
 * decodificat. */
nomen vacuum (*ManusTraditio)(
               vacuum* ctx,
          Dispensator* d,
    constans Eventus* e);

/* Ut manus_ludus_iterare, sed per traditionem datam (NIHIL:
 * directe). */
i32
manus_ludus_iterare_per (
       ManusLudus* m,
     constans Xar* notata,
    ManusIteratio  modus,
              Xar* divergentiae,
    ManusTraditio  traditio,
           vacuum* ctx);

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
