/* tabellarius.h - Cursor JSON-RPC (LEGATUS chunk A)
 *
 * Tabellarius epistulas fert: stratum vecturae JSON-RPC 2.0 PURUM,
 * sine ulla scientia LSP. Duo strata separata et composibilia:
 *
 *   EPISTULA (framing): capita "Content-Length: N" + \r\n\r\n +
 *   corpus, super FILE* (vectura LSP). Servus MCP futurus idem
 *   involucrum cum framing alio (lineis) uteretur - ideo strata
 *   numquam coniungenda.
 *
 *   NUNTIUS (envelope): petitio / nuntiatio / responsum super
 *   lib/json. Id VERBATIM refertur (JsonValor* - integer AUT
 *   chorda; conversio corrumpit). Fasces (tabulata JSON-RPC 2.0)
 *   PRAVA - LSP eis numquam utitur.
 *
 * Omnes allocationes ex piscina vocantis; exemplar naturale =
 * piscina per epistulam, vacata post tractationem.
 */

#ifndef TABELLARIUS_H
#define TABELLARIUS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"

#include <stdio.h>

/* Codices errorum JSON-RPC 2.0 (+ LSP -32002) */
#define TABELLARIUS_ERROR_ANALYSIS           (-32700L)
#define TABELLARIUS_ERROR_PETITIO_INVALIDA   (-32600L)
#define TABELLARIUS_ERROR_METHODUS_IGNOTA    (-32601L)
#define TABELLARIUS_ERROR_PARAMETRA_INVALIDA (-32602L)
#define TABELLARIUS_ERROR_INTERNUS           (-32603L)
#define TABELLARIUS_ERROR_NONDUM_INITIATUM   (-32002L)

/* Tectum corporis (contra mensuras corruptas/hostiles) */
#define TABELLARIUS_CORPUS_MAXIMUM (67108864UL)

/* ==================================================
 * EPISTULA (framing)
 * ================================================== */

/* Epistulam unam legere: capita (lineae \r\n, ordine quolibet,
 * "content-length" sine casu) usque ad lineam vacuam, deinde
 * corpus exacte N octetorum (NUL appenso in piscinam). Chorda
 * vacua + *fluxus_finitus = fluxus finitus AUT corruptus (EOF,
 * caput sine mensura, linea nimis longa, corpus breve, tectum
 * excessum) - vocans exit, non recuperat (desync irrecuperabilis).
 */
chorda
tabellarius_epistulam_legere (
    FILE*    fluxus,
    Piscina* piscina,
    b32*     fluxus_finitus);

/* Epistulam scribere: caput + corpus + fflush (puritas stdout =
 * vocantis; haec functio solum protocollum scribit). */
vacuum
tabellarius_epistulam_scribere (
    FILE*  fluxus,
    chorda corpus);

/* ==================================================
 * NUNTIUS (envelope)
 * ================================================== */

nomen enumeratio {
    TABELLARIUS_PRAVUM = 0,   /* analysis fracta / non objectum /
                               * fascis / neque methodus neque
                               * responsum */
    TABELLARIUS_PETITIO,      /* methodus + id */
    TABELLARIUS_NUNTIATIO,    /* methodus sine id */
    TABELLARIUS_RESPONSUM     /* id + (result|error) sine methodo */
} TabellariusGenus;

nomen structura {
    TabellariusGenus genus;
    JsonValor*       radix;    /* objectum totum; NIHIL si analysis
                                * fracta */
    JsonValor*       id;       /* VERBATIM; NIHIL = absens */
    chorda           methodus; /* vacua si absens */
    JsonValor*       params;   /* NIHIL = absens */
} TabellariusNuntius;

/* Corpus in nuntium analysare. Tolerans: campus "jsonrpc" non
 * imponitur (LSP semper "2.0" mittit; structura sola iudicatur). */
TabellariusNuntius
tabellarius_nuntium_legere (
    chorda   corpus,
    Piscina* piscina);

/* ==================================================
 * FABRI (corpora responsi/erroris/nuntiationis)
 * ================================================== */

/* {"jsonrpc":"2.0","id":<id>,"result":<resultatum>}
 * id NIHIL -> null; resultatum NIHIL -> null */
chorda
tabellarius_responsum (
    Piscina*   piscina,
    JsonValor* id,
    JsonValor* resultatum);

/* {"jsonrpc":"2.0","id":<id>,"error":{"code":N,"message":"..."}} */
chorda
tabellarius_errorem (
    Piscina*            piscina,
    JsonValor*          id,
    longus              codex,
    constans character* nuntius);

/* {"jsonrpc":"2.0","method":"...","params":<params>}
 * params NIHIL -> campus omissus */
chorda
tabellarius_nuntiationem (
    Piscina*            piscina,
    constans character* methodus,
    JsonValor*          params);

#endif /* TABELLARIUS_H */
