/* mcp_servus_http.h - vectura MCP "Streamable HTTP" super hospitium
 *
 * Aera initialize (2025-03-26 / 06-18 / 11-25), sine sessionibus
 * (Mcp-Session-Id numquam datur), sine SSE:
 *
 *   POST via solum (GET/DELETE -> 405, Allow: POST); corpus
 *   application/json, <= I MB (413), Content-Type alienum 415;
 *   fasces 400; nuntiationes 202 sine corpore; responsa JSON una.
 *
 *   MCP-Protocol-Version: absens = 2025-03-26; nostra licet; alia
 *   (e.g. 2026-07-28) -> 400 cum UnsupportedProtocolVersionError
 *   (-32022, data.supported nostra) - signum quo cliens aerae novae
 *   descendit.
 *
 *   Origin praesens: in indice 'origines' esse debet, aliter 403.
 *
 *   OMNIS petitio "Authorization: Bearer" postulat (initialize
 *   quoque: Claude sigillationem solum post 401 incipit); tessera in
 *   quaestione (?access_token=) 400. Tessera per VERIFICATOREM
 *   (functio applicationis, e.g. admissio) cum audientia 'resource'
 *   iudicatur. 401 / 403 capita WWW-Authenticate octetim (RFC 6750,
 *   9728).
 *
 *   PRM (RFC 9728) ad /.well-known/oauth-protected-resource ET
 *   /.well-known/oauth-protected-resource<via>.
 *
 *   ACTA: linea una per petitionem (usor, cliens, instrumentum,
 *   exitus, ms); tessera numquam.
 */

#ifndef MCP_SERVUS_HTTP_H
#define MCP_SERVUS_HTTP_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "hospitium.h"
#include "mcp_servus.h"

/* corpus maximum petitionis */
#define MCP_HTTP_CORPUS_MAXIMUM (MXXIV * MXXIV)

/* error JSON-RPC aerae 2026-07-28 */
#define MCP_ERROR_VERSIO_IGNOTA (-32022L)

/* tessera -> persona. audientia = URL canonica servi (OAuth
 * "resource"): tessera alii servo data refutanda (RFC 8707).
 * Allocationes in p (piscina petitionis). VERUM = persona impleta. */
nomen b32 (*McpVerificator)(vacuum* datum, chorda tessera,
    chorda audientia, McpPersona* exitus, Piscina* p);

/* linea actorum (sine '\n') */
nomen vacuum (*McpActaScriptor)(vacuum* datum, chorda linea);

nomen structura {
              constans character* via;          /* "/mcp" */
              constans character* audientia;     /* "https://h/mcp" */
              constans character* auctoritas;   /* issuer */
              constans character* scopi;        /* spatiis separati */
    constans character* constans*  origines;     /* NIHIL-terminata;
                                                  * NIHIL = nulla
                                                  * Origin licet */
                   McpVerificator  verificator;
                           vacuum* verificatoris_datum;
                  McpActaScriptor  acta;         /* NIHIL = stderr */
                           vacuum* acta_datum;
} McpHttpOptiones;

/* via (POST; GET/DELETE 405) + duae viae PRM. Optiones et chordae
 * transcribuntur in piscinam (vivere debet quamdiu hospitium). */
b32
mcp_servus_http_montare (
                   Hospitium* hospitium,
                   McpServus* servus,
    constans McpHttpOptiones* optiones,
                     Piscina* piscina);

#endif /* MCP_SERVUS_HTTP_H */
