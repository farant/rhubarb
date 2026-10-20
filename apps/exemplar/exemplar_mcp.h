/* exemplar_mcp.h - viae MCP et OAuth exemplaris (internum)
 *
 * Corpus: exemplar_mcp.c. Sonda (mcp-remotus T1): viae inventionis
 * (RFC 9728, RFC 8414), POST /mcp -> 401 cum WWW-Authenticate,
 * /oauth/register -> 503 dum admissio deest; acta petitionum redacta
 * si configuratio.sonda.
 */
#ifndef EXEMPLAR_MCP_H
#define EXEMPLAR_MCP_H

#include "exemplar_internum.h"

/* viae MCP + OAuth in hospitio exemplaris */
vacuum
exemplar_mcp_praebere (
    Exemplar* e);

#endif /* EXEMPLAR_MCP_H */
