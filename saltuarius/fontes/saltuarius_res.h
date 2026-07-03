/* saltuarius_res.h - Status app-latus (Phase A)
 *
 * PURUS: viae per bibliothecam via aedificantur (logica chordarum,
 * non discus). Possidet: modum, radicem, viam currentem, vexillum
 * ansae, nuntium transitorium.
 */

#ifndef SALTUARIUS_RES_H
#define SALTUARIUS_RES_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

nomen enumeratio {
    SALT_MODUS_COLUMNAE = 0,
    SALT_MODUS_FONS          /* Phase B */
} SaltuariusModus;

nomen structura {
    Piscina*        piscina;
    SaltuariusModus modus;
    chorda          radix;        /* initium, non carcer */
    chorda          via_currens;  /* directorium visum */
    b32             currens;      /* ansa eventuum viva */
    chorda          nuntius;      /* nuntius transitorius */
    s32             nuntius_aetas; /* quadra reliqua; 0 = nullus */
} SaltuariusRes;

/* Creare statum; via_currens incipit ad radicem */
SaltuariusRes*
saltuarius_res_creare (Piscina* piscina,
    constans character* radix);

/* Nuntium transitorium ponere (quadra = quot quadris vivit) */
vacuum
saltuarius_res_nuntiare (SaltuariusRes* res,
    constans character* textus, s32 quadra);

/* Aetatem nuntii decrescere (per quadrum); VERUM si visibilis */
b32
saltuarius_res_nuntius_senescere (SaltuariusRes* res);

/* In subdirectorium intrare (via_currens := currens/titulus) */
vacuum
saltuarius_res_intrare (SaltuariusRes* res, chorda titulus);

/* Ad parentem ascendere (supra radicem licet - radix initium,
 * non carcer; "/" fixum manet per via_directorium) */
vacuum
saltuarius_res_ascendere (SaltuariusRes* res);

#endif /* SALTUARIUS_RES_H */
