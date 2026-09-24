/* briar_bibliotheca.h - documentatio rudis corporis (briar
 * -bibliothecae, -bibliotheca <nomen>): descriptio bibliothecae ex
 * linea prima capitis sui. PURUM - lectio corporis in tools/briar.c
 * (silex_fons_enumerare / silex_fons_legere).
 */
/* <aedilis corpus="briar/fontes/briar_bibliotheca.c"/> */
#ifndef BRIAR_BIBLIOTHECA_H
#define BRIAR_BIBLIOTHECA_H

#include "latina.h"
#include "chorda.h"

/* Descriptio ex linea PRIMA capitis secundum conventionem domus:
 * '/' '*' ' ' <titulus> (e.g. "sors.h") deinde '-', '—' aut ':'
 * deinde descriptio usque ad finem lineae (clausura commentarii et
 * spatia finalia abscissa). Chorda vacua si caput conventionem non
 * sequitur (e.g. custos '#ifndef' primus) - quae ipsa debita
 * documentationis est. Reddita chorda textum ipsum spectat (nulla
 * copia). */
chorda
briar_bibliotheca_descriptio (
    chorda textus,
    chorda titulus);

#endif /* BRIAR_BIBLIOTHECA_H */
