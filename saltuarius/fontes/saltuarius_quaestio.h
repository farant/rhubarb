/* saltuarius_quaestio.h - Quaestio incrementalis '/' (Phase D, D2)
 *
 * PRIMUS modus INITUS TEXTUS app: dum activa, principale claves
 * praeterit et eventa CRUDA huc tradit (runae appenduntur ut
 * UTF-8, RETRORSUM runam INTEGRAM delet, REDITUS committit, FUGA
 * cancellat cursorem restituens).
 *
 * Semantica (exemplar vim wrapscan): litteralis, non-regex, super
 * textum strati OSTENSI (quaere quod vides - fiat colloquii);
 * origo ad '/' notatur, quisque ictus AB ORIGINE re-quaerit (non
 * a cursore movente - alioquin scriptio prosiliret), inventum
 * cursorem movet, nihil -> cursor ad originem + insigne. n/N
 * post commissionem a cursore stricte ultra/citra, cum volutione
 * (volvit -> principale "(finis)" nuntiat).
 *
 * Buffera FIXA (256): quaestiones natura breves, arena non opus.
 * Collatio pastae: runae sicut imber adveniunt - acceptum,
 * nominatum (notae bracketed a tessera absorptae).
 */

#ifndef SALTUARIUS_QUAESTIO_H
#define SALTUARIUS_QUAESTIO_H

#include "latina.h"
#include "tessera.h"
#include "saltuarius_liber.h"

#define SALT_QUAESTIO_MAXIMA 256

nomen enumeratio {
    SALT_QUAESTIO_PERGIT = 0,   /* adhuc scribens */
    SALT_QUAESTIO_COMMISSA,     /* Reditus - commissum servatum */
    SALT_QUAESTIO_CANCELLATA    /* Fuga - cursor restitutus */
} SaltuariusQuaestioExitus;

nomen structura {
    b32 activa;
    i8  litterae[SALT_QUAESTIO_MAXIMA];   /* in flumine */
    i32 mensura;
    i8  commissum[SALT_QUAESTIO_MAXIMA];  /* memoria n/N */
    i32 commissum_mensura;
    b32 nihil_inventum;    /* insigne lineae echo */
    /* imago originis (FUGA totam restituit) */
    s32 origo_offset;
    s32 origo_linea;
    s32 origo_columna;
    s32 origo_vol_x;
    s32 origo_vol_y;
} SaltuariusQuaestio;

vacuum
saltuarius_quaestio_parare (SaltuariusQuaestio* quaestio);

/* Modum initus aperire: originem notat, litteras purgat */
vacuum
saltuarius_quaestio_aperire (SaltuariusQuaestio* quaestio,
    constans SaltuariusLiber* liber);

/* Eventum crudum tradere (solum dum activa). Cursor libri per
 * inventionem movetur; FUGA restituit. */
SaltuariusQuaestioExitus
saltuarius_quaestio_tradere (SaltuariusQuaestio* quaestio,
    SaltuariusLiber* liber, constans TesseraEventum* eventum);

/* n/N: commissum a cursore stricte ultra (directio > 0) / citra.
 * VERUM si inventum (cursor motus); volvit_out VERUM si quaestio
 * circum finem voluta. FALSUM: nihil inventum aut commissum
 * vacuum (vocator distinguat per commissum_mensura). */
b32
saltuarius_quaestio_proximum (constans SaltuariusQuaestio* quaestio,
    SaltuariusLiber* liber, s32 directio, b32* volvit_out);

#endif /* SALTUARIUS_QUAESTIO_H */
