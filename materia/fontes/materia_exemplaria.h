/* materia_exemplaria.h - Ordines inventorum ex documento EXPANSO
 * (project-specs/exemplaria-c-spec.md par. II; res 01M2RYR3JJ)
 *
 * Gradus II ('an hic codex dubius sit') per exemplaria STML currit:
 * proiectio clientis + regula -> stml_expandere -> elementa <relatum>.
 * Extractio illa hactenus in Pythone vivebat, per html.parser - id
 * est, parsator ALIENUS in media catena instrumentorum nostrorum, ut
 * lector STML noster (C) circumiretur. Hoc eam in C ponit.
 *
 * ==================================================
 * CLIENTE CAECUM
 * ==================================================
 *
 * Hoc legit 'sedes' et 'octeti', quae SCRIPTOR MATERIAE uniformiter
 * emittit (visio sedium, materia-sedes par. III), et 'lint',
 * 'gravitas', 'causa', 'nota', quae AUCTOR REGULAE scribit. Plagulam,
 * suffixum, grammaticam, registrum clientis NUMQUAM videt. Ergo
 * clientem novum addere hoc caput NON emendat - lex M8, et probatio
 * quae sola refert (specificatio E1).
 *
 * ==================================================
 * DESCENSUS TACET POST CAPTURAM
 * ==================================================
 *
 * Nodus insertus per pontem SUBARBOREM TOTAM fert, et elementum
 * quodque in ea sedem SUAM habet (imperium unum XXIX dedit,
 * mensuratum 2026-09-17). Ergo sedes SUMMA per involucrum capitur et
 * descensus TACET donec subarbor capta exeatur. Sine hac lege ordo
 * unus sedes omnium descendentium ferret.
 *
 * Python idem per acervum notarum et 'gradus_capti' agebat, quia
 * html.parser fluxum SAX dat. Super arbore vera RECURSIO ACERVUS EST
 * et 'noli descendere' REDDE est - ergo ea fulcimenta hic DESUNT
 * consulto, non per oblivionem.
 */

#ifndef MATERIA_EXEMPLARIA_H
#define MATERIA_EXEMPLARIA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "stml.h"
#include "materia_diagnostica.h"


/* ==================================================
 * Compositio documenti
 * ================================================== */

/* Documentum componere: proiectio clientis + TRANSPARENTIA (si regula
 * nullam declarat) + elementa regulae. Arbor NOVA redditur; neutrum
 * argumentum mutatur.
 *
 * TRANSPARENTIA ordinaria per SUBSTRATUM est, non per clientem
 * (mensuratum: tabula tota duas res habet) - 'ante post' pro tagis,
 * 'sedes octeti' pro attributis. Involucra triviae et attributa
 * visionis littera congruentiae fierent aliter.
 *
 * NIHIL si argumentum necessarium abest aut memoria defecit. */
StmlNodus*
materia_exemplaria_componere (
              Piscina* piscina,
            StmlNodus* proiectio,
            StmlNodus* regula,
  InternamentumChorda* intern);


/* ==================================================
 * Extractio ordinum
 * ================================================== */

/* Documentum EXPANSUM -> Xar de MateriaDiagnosticum.
 *
 * Ordo UNUS per LIBERUM elementum ipsius <relatum> (emissio PER una);
 * filii BINI inventa DUO sunt, non inventum unum sedibus binis -
 * electio auctoris regulae, quae ex structura ipsa fluit.
 *
 * Sedes PRIMA primaria est, ceterae relatae. 'nota' involucri
 * AMBIENTIS sedem infra se nominat.
 *
 * codex = 'lint:<lint>' - praefixum SUUM fert, ut codices substrati,
 * ergo pictor grammaticam non praefigit. gravitas ordinaria
 * 'erratum'; causa ordinaria nomen lintris ipsum.
 *
 * FONTEM NON POSCIT: MateriaDiagnosticum segmentum fontis NON
 * servat - pictor eum sede sua secat cum pingit. Signatura prior
 * 'fons'/'mensura' ferebat, ex Pythone translata, ubi Congruentia
 * campum 'textus_fontis' habet; probatio primum scripta id statim
 * ostendit (membrum 'textus' non exstat).
 *
 * Xar vacuum SANUM est (regula nihil invenit); NIHIL = argumentum
 * abest aut memoria defecit. */
Xar*
materia_exemplaria_extrahere (
    Piscina* piscina,
  StmlNodus* expansum);


/* ==================================================
 * Subtractio declarata
 * ==================================================
 *
 * CUR OMNINO: exemplar DEORSUM quaerit, et conditio quaedam SUPRA
 * inventum sedet. Ordo ad inventum radicatus conditionem suam videre
 * NON potest; ordo ad documentum radicatus eam NIMIS LATE videt, et
 * inventa OMNIA necaret. Ergo auctor regulae bracchia DUO scribit et
 * differentiam DECLARAT:
 *
 *     <relatum lint="nt-aequalitas" gravitas="erratum">...</relatum>
 *     <relatum lint="nt-negata" minuit="nt-aequalitas">...</relatum>
 *
 * Bracchium cum 'minuit' AUXILIARE est: ordines suos ex lintre nominata
 * per SEDEM tollit, et ipse numquam emittitur - inventum non est,
 * conditio est. Nomina lintrium AUCTORIS sunt; hoc nullum novit.
 *
 * SEDES clavis est (initium), non textus: inventa duo eiusdem textus
 * in plagula una communia sunt. Documentum unum plagula una est, ergo
 * via clavem non intrat (Python, quae plagulas multas simul tractabat,
 * '(via, initium)' poscebat).
 *
 * CUSTODIA: si bracchium auxiliare subset positivi non est, bracchia
 * sedes DIVERSAS capiunt et differentia nihil significat - NIHIL cum
 * causa nominata, numquam numerus tacite falsus.
 *
 * Xar NOVUM reddit; 'ordines' immotum manet. NIHIL = memoria defecit
 * aut bracchia vaga ('causa' posita si datur). */

Xar*
materia_exemplaria_minuere (
               Piscina*  piscina,
             StmlNodus*  expansum,
          constans Xar*  ordines,
    constans character** causa);      /* NIHIL licet */

#endif /* MATERIA_EXEMPLARIA_H */
