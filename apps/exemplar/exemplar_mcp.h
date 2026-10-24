/* exemplar_mcp.h - viae MCP et OAuth exemplaris (internum)
 *
 * Corpus: exemplar_mcp.c. Servus MCP tertius (mcp-remotus T7):
 * instrumenta notarum super mcp_servus, vectura mcp_servus_http cum
 * admissio_verificator, admissio montata cum intratione exemplaris;
 * sonda (T1) per speculatores bibliothecarum.
 */
#ifndef EXEMPLAR_MCP_H
#define EXEMPLAR_MCP_H

#include "exemplar_internum.h"

/* servus MCP + admissio in hospitio exemplaris; FALSUM si deficit */
b32
exemplar_mcp_praebere (
    Exemplar* e);

#endif /* EXEMPLAR_MCP_H */
