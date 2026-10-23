/* indagatio_genera.h - GENERA QUAERENDA domus (indagatio S2,
 * project-specs/indagatio.md): res agendae, notae dierum, liber
 * communis, personae cum notis suis, sancti (D5 S4). Declarationes
 * lib/conspectus_<n>.stml (infixae); computare cuiusque functio PURA
 * documentorum et librorum - lectio sola: nihil scribitur, liber
 * absens non creatur.
 *
 *   conspectus  clavis        fontes                       grex
 *   res         id rei        agenda per-rem               Agenda
 *   notae       dies          spatium dies/{clavis}        Day notes
 *   liber       nomen paginae spatium paginae/{clavis}     Notebook
 *   personae    id personae   personae per-rem,            Contacts
 *                             spatium persona/{clavis}
 *   sancti      id sancti     pura (corpus non mutatur)    Saints
 *
 * Ordines (titulus | contextus | semita; corpus):
 *   res       titulus rei | "due Wed 21 Oct 2026 - #domus", "done
 *             Thu 1 Oct 2026", "no date" | calendarium/dies/<dies>?
 *             res_electa=<id> (sine die calendarium/agenda?...);
 *             corpus notae et rubricae
 *   notae     ordo per paginam: "Mon 19 Oct 2026" (paginis pluribus
 *             + " - page 2 of 3") | vacuus | calendarium/dies/<dies>?
 *             notae.pagina=<nomen paginae>
 *   liber     linea prima paginae | "page 3" | scriba/<nomen>
 *   personae  nomen ostensum | epistula prima - telephonum primum |
 *             nomenclator/<id>; corpus nomina altera, rubricae,
 *             epistulae, telephona (et cifrae solae), lineae notarum
 *   sancti    titulus sancti | festum primum - aetas
 *             ("28 Jan · 1225–1274") | sancti/sanctus/<id>; corpus
 *             titulus latinus,
 *             nomina, munera, patrocinia, loci ortus et obitus, socii,
 *             paragraphi
 * Pagina sine lineis nullum ordinem dat; res aut persona deleta
 * nullum.
 */

#ifndef INDAGATIO_GENERA_H
#define INDAGATIO_GENERA_H

/* <aedilis corpus="lib/indagatio_genera.c"/> */

#include "latina.h"
#include "internamentum.h"
#include "xar.h"
#include "volumen.h"
#include "agenda.h"
#include "personae.h"
#include "sancti.h"
#include "conspectus.h"

nomen structura {
       AgendaDocumentum* agenda;    /* NIHIL: 'res' abest */
     PersonaeDocumentum* personae;  /* NIHIL: 'personae' abest */
                Volumen* volumen;   /* libri; NIHIL: 'notae', 'liber'
                                     * et notae personarum absunt */
      InternamentumChorda* intern;
    constans SanctiCorpus* sancti;   /* D5 S4: NIHIL = 'sancti' abest */
} IndagatioFontes;

/* declarationes generum quorum fontes adsunt legit; opera, per-rem et
 * enumerationes ponit; documenta ligat. Fontes a vocante possessi
 * vivant dum registrum; parare vocantis est. */
b32
indagatio_genera_registrare (
    ConspectusRegistrum*  repertorium,
        IndagatioFontes*  fontes,
                    Xar** vitia);

#endif /* INDAGATIO_GENERA_H */
