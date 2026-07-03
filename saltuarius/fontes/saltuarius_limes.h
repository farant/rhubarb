/* saltuarius_limes.h - Limes disci (Phase A)
 *
 * SOLUS locus (praeter principale) ubi discus tangitur: indices ex
 * iter_directoria, praevisus ex filum. Visus/exemplaria numquam -
 * iniectio est sutura probationum, hic modulus est quod inicitur.
 *
 * MEMORIA FRENATA: duae arenae refectionis (piscina_notare/
 * reficere) - indicis (refecta in navigatione) et praevisus
 * (refecta per selectionem). Sine hoc, omnis motus j/k plagulam
 * in arenam perpetuam legeret - sessio longa sine fine cresceret.
 * Structurae tabulariorum arenam PERSISTENTEM habitant; tabulae
 * introituum in arenis refectis vivunt (implere arenam accipit).
 */

#ifndef SALTUARIUS_LIMES_H
#define SALTUARIUS_LIMES_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "saltuarius_res.h"
#include "saltuarius_tabularium.h"
#include "saltuarius_columnae.h"
#include "saltuarius_liber.h"

/* Praevisus plagularum maximus (introitus maior -> "(filum
 * magnum)" sine lectione) */
#define SALT_PRAEVISUS_MAXIMUS 1048576

/* Introitus directorii maximi per indicem (ultra: praecisio
 * tacita - degradatio, non fractura) */
#define SALT_INTROITUS_MAXIMI 4096

nomen structura {
    Piscina*              indicis;        /* arena indicium */
    PiscinaNotatio        nota_indicis;
    Piscina*              praevisus;      /* arena praevisus */
    PiscinaNotatio        nota_praevisus;
    SaltuariusTabularium* tab_parens;
    SaltuariusTabularium* tab_currens;
    SaltuariusTabularium* tab_praevisus;
} SaltuariusLimes;

/* Creare limitem: arenas refectionis generat; tabularia (ex arena
 * persistenti creata) accipit et possidet functionaliter */
SaltuariusLimes*
saltuarius_limes_creare (Piscina* persistens,
    SaltuariusTabularium* tab_parens,
    SaltuariusTabularium* tab_currens,
    SaltuariusTabularium* tab_praevisus);

/* Arenas destruere (exitus app) */
vacuum
saltuarius_limes_destruere (SaltuariusLimes* limes);

/* Indices ex disco implere pro res->via_currens: currens + parens
 * (NIHIL ad "/"). Selectio parentis = titulus directorii currentis;
 * selectio currentis = selectandum si datum (ascensus), alioquin 0.
 * Columnae aptantur. Reddit FALSUM si directorium currens non
 * legibile (index vacuus fit - degradatio). */
b32
saltuarius_limes_indices (SaltuariusLimes* limes,
    constans SaltuariusRes* res, SaltuariusColumnae* columnae,
    constans chorda* selectandum);

/* Praevisum ex disco implere pro introitu selecto currentis:
 * directorium -> index; filum -> textus (custodiae: magnum,
 * binarium, non legibile); alius -> nuntius. Columnae aptantur. */
vacuum
saltuarius_limes_praevisum (SaltuariusLimes* limes,
    constans SaltuariusRes* res, SaltuariusColumnae* columnae);

/* Librum aperire ex introitu selecto currentis (custodiae eaedem
 * ac praevisus: magnum sine lectione, binarium, non legibile).
 * NIHIL si refusatum - causa in causa_out (literis staticis, pro
 * nuntio). Textus per arenam praevisus transit (liber transcribit
 * in arenas proprias). */
SaltuariusLiber*
saltuarius_limes_librum_aperire (SaltuariusLimes* limes,
    Piscina* persistens, constans SaltuariusNexus* nexus,
    constans SaltuariusRes* res, constans character** causa_out);

#endif /* SALTUARIUS_LIMES_H */
