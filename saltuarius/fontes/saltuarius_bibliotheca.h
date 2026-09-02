/* saltuarius_bibliotheca.h - Cache librorum LRU (Phase B)
 *
 * N=8 (spec-v2 par 10.1, revisio Q10 mensuris pulsa): libri
 * recentissimi primi; evictio = liber_destruere (arenae ambae).
 * Phase B libri lexati leves sunt; Phase C parsuras graves in
 * eadem forma commutabit - structura NUNC nata ut C intra formam
 * existentem crescat.
 */

#ifndef SALTUARIUS_BIBLIOTHECA_H
#define SALTUARIUS_BIBLIOTHECA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "saltuarius_liber.h"

#define SALT_LIBRI_MAXIMI 8

nomen structura {
    SaltuariusLiber* libri[SALT_LIBRI_MAXIMI];  /* [0] recentissimus */
                i32  numerus;
} SaltuariusBibliotheca;

vacuum
saltuarius_bibliotheca_parare (
    SaltuariusBibliotheca* bibliotheca);

/* Librum per viam quaerere; si inventus, ad frontem motus et
 * redditus, alioquin NIHIL */
SaltuariusLiber*
saltuarius_bibliotheca_quaerere (
    SaltuariusBibliotheca* bibliotheca,
                   chorda  via);

/* Librum condere (frons); si plena, vetustissimus destruitur */
vacuum
saltuarius_bibliotheca_condere (
    SaltuariusBibliotheca* bibliotheca,
          SaltuariusLiber* liber);

/* Omnes destruere (exitus) */
vacuum
saltuarius_bibliotheca_vacare (
    SaltuariusBibliotheca* bibliotheca);

#endif /* SALTUARIUS_BIBLIOTHECA_H */
