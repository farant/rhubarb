/* toni_psalmorum_infixum.h - textus musica/toni_psalmorum.stml
 * infixus (tabula tonorum psalmorum, include/psalmodia_tabula.h):
 * binaria (auditio -psalmus) sine plagula tabulae vivunt. Corpus
 * lib/toni_psalmorum_infixum.c GENERATUM est per
 * tools/toni_psalmorum_infigere.sh - numquam manu. */
#ifndef TONI_PSALMORUM_INFIXUM_H
#define TONI_PSALMORUM_INFIXUM_H

/* <aedilis corpus="lib/toni_psalmorum_infixum.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

/* copia textus tabulae in piscina */
chorda
toni_psalmorum_infixum_textus (
    Piscina* piscina);

#endif /* TONI_PSALMORUM_INFIXUM_H */
