/* exemplar_administratio.h - tabula administrationis exemplaris
 * (internum; canalis-spec II.3)
 *
 * Corpus: exemplar_administratio.c. Servus MCP ALTER (non /mcp):
 * XIV instrumenta, omnia scopo "administratio", per canalem localem
 * <data>/canalis/servus (mcp_servus_canalis) - persona fixa canalis
 * scopum habet, persona OAuth numquam. Quisque tractator
 * transactionem suam aperit et committit aut revolvit.
 *
 *   status, invitare {nota, dies I..XXX}, invitationes_enumerare,
 *   invitationem_revocare {id}, cives_enumerare,
 *   civem_suspendere {nomen}, civem_restituere {nomen},
 *   communio_membrum_addere {spatium, nomen},
 *   communio_apparatum_addere {nomen, spatium, titulus, digestum},
 *   communio_apparatum_revocare {id}, communio_apparatus_enumerare,
 *   communio_epistulam_permittere {epistula, nomen, titulus, spatium},
 *   communio_epistulam_revocare {epistula},
 *   communio_permissos_enumerare (plan-3 T5)
 *   (communio plan-2 T7: digestum tesserae intrat, numquam exit)
 *
 * Suspensio statim secat: status + sessiones (ianua) + concessiones
 * OAuth (admissio), transactione una; restitutio nihil resuscitat.
 * Sepulcrum (DELETUS) numquam mutatur. Nullum instrumentum tesseram
 * accipit aut reddit; codices invitationum tantum ab 'invitare' semel.
 */
#ifndef EXEMPLAR_ADMINISTRATIO_H
#define EXEMPLAR_ADMINISTRATIO_H

#include "exemplar_internum.h"

/* via canalis sub data */
#define EXEMPLAR_VIA_CANALIS "canalis/servus"

/* tabula administrationis; NIHIL si deficit */
McpServus*
exemplar_administrationem_creare (
    Exemplar* e,
     Piscina* piscina);

#endif /* EXEMPLAR_ADMINISTRATIO_H */
