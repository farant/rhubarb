/* mcp_servus_canalis.h - MCP super canalem localem (canalis-spec II.2)
 *
 * SERVUS (mcp_canalis_praebere): McpServus applicationis super canalem
 * (canalis.h) in reactore - lineae tabellarii ut stdio (initialize
 * primum), responsum unum per lineam in caudam scriptionis connexionis
 * (EAGAIN -> interest scribendi, canalis P2), responsa dilata (par qui
 * abiit per generationem agnoscitur, responsum omittitur). Persona
 * FIXA omnium vocationum: { usor "administratio", cliens "canalis",
 * scopi "administratio" } - tabula administrationis instrumenta scopo
 * "administratio" signat, ergo in /mcp (persona OAuth) numquam
 * currunt. VIII connexiones summum (nona clauditur); linea ultra
 * MCP_CANALIS_LINEA_MAXIMA connexionem claudit; linea ultima sine '\n'
 * ante finem omittitur. Nulla mora otii (sessio Claude Code diu
 * quiescit; solus uid noster loca implere potest). Corpora numquam in
 * actis; par recusatus lineam unam stderr ('canalis: par recusatus').
 *
 * NOTA: McpServus 'initiatum' servo toti pertinet, non connexioni -
 * post initialize primum connexiones ceterae non coguntur (clientes
 * nostri semper initiant; canalis T2).
 *
 * CLIENTES (obstruentes): pons stdio <-> canalis (Claude Code per ssh:
 * exemplar -administrare) et vocatio una (exemplar -invitare).
 * Nihil quod vehunt in actis scribunt.
 */
#ifndef MCP_SERVUS_CANALIS_H
#define MCP_SERVUS_CANALIS_H

/* <aedilis corpus="lib/mcp_servus_canalis.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"
#include "reactor.h"
#include "mcp_servus.h"

#define MCP_CANALIS_CONNEXIONES   VIII
#define MCP_CANALIS_LINEA_MAXIMA  (MXXIV * MXXIV)   /* I MiB */
#define MCP_CANALIS_MORA_VOCATIONIS_MS  (XXX * M)   /* vocare: XXX s */

nomen structura McpCanalis McpCanalis;

/* servus super canalem in via (canalis_audire - PRAECONDITIO eius:
 * sera scriptoris unius tenetur). NIHIL + *error si canalis deficit.
 * Post mcp_canalis_claudere servus responsa dilata non iam tradit -
 * vocans canalem ante reactorem claudit. */
McpCanalis*
mcp_canalis_praebere (
              McpServus* servus,
                Reactor* reactor,
     constans character* via,
                 chorda* error,
                Piscina* piscina);

/* connexiones claudit, fossam e reactore removet, viam delet */
vacuum
mcp_canalis_claudere (
    McpCanalis* canalis);

/* pons stdio <-> canalis (obstruens; poll super fossas crudas, numquam
 * FILE*): EOF intrae -> SHUT_WR, responsa usque ad EOF socketi
 * transmittuntur. EOF socketi (servus mortuus) -> statim finis.
 * 0 finis ordinatus | I scriptura fracta | II nemo audit */
s32
mcp_canalis_pons (
    constans character* via,
               integer  intra,
               integer  extra);

/* vocatio una: initialize, notifications/initialized, tools/call
 * 'instrumentum' cum 'argumenta' (NIHIL = {}); MCP_CANALIS_MORA_
 * VOCATIONIS_MS summum. textus = result.content[0].text (aut causa).
 * 0 bene | I isError (instrumentum cucurrit et defecit) | II nemo
 * audit | III protocollum aut tempus */
s32
mcp_canalis_vocare (
     constans character* via,
     constans character* instrumentum,
              JsonValor* argumenta,
                 chorda* textus,
                Piscina* piscina);

#endif /* MCP_SERVUS_CANALIS_H */
