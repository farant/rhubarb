/* officina_medulla_textus.h - Forma textualis medullae (M1a C)
 *
 * QUOD SCRIPTOR EVOMIT, CANON EST (DECISUS interview Q5-Q8 +
 * chunk-C: fluitantia %.17g decimalia; commenta a lectore
 * praetermissa). Vectis M1a: dump(parse(t)) == t byte-idem pro
 * plagulis canonicis; plagulae commentatae vectem idempotentiae
 * habent (parse -> dump -> parse -> dump idem).
 *
 * Sigilla: % registrum, $ symbolum, @ bloccus, "..." causa,
 * nudum immediatum. Lector STRICTUS: forma canonica sola, praeter
 * lineas vacuas et commenta (;) quae praetermittuntur (plagulae
 * manu scriptae). Indentatio: spatia praecedentia praetermissa.
 */

#ifndef OFFICINA_MEDULLA_TEXTUS_H
#define OFFICINA_MEDULLA_TEXTUS_H

#include "officina_medulla.h"

/* nomina signata (vocabularium-medullae.md) - fons unicus et
 * scriptoris et lectoris; NIHIL si extra fines */
constans character* medulla_op_titulus (s32 op);
constans character* medulla_typum_titulus (s32 typus);

chorda medulla_textum_scribere (Piscina* piscina,
    constans MedullaModulus* modulus);

/* NIHIL + linea_erroris (1-basis; NIHIL licet) in errore */
MedullaModulus* medulla_textum_legere (Piscina* piscina,
    chorda textus, i32* linea_erroris);

#endif /* OFFICINA_MEDULLA_TEXTUS_H */
