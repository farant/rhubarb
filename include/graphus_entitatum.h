#ifndef GRAPHUS_ENTITATUM_H
#define GRAPHUS_ENTITATUM_H

#include "latina.h"
#include "entitas.h"
#include "entitas_providor.h"
#include "tabula_dispersa.h"
#include "internamentum.h"
#include "piscina.h"

/* ==================================================
 * GraphusEntitatum - Repositorium Entitatum Concretum
 *
 * Implementatio concreta EntitasProvidor
 * Entitates in tabula dispersa (hash table) conservat
 * ================================================== */

nomen structura {
    TabulaDispersa*      entitates;  /* id -> Entitas* */
    InternamentumChorda* intern;     /* Internatio chordarum */
    Piscina*             piscina;    /* Arena memoriae */
} GraphusEntitatum;


/* ==================================================
 * Creatio
 * ================================================== */

/* Creare novum graphum entitatum
 *
 * Redde: GraphusEntitatum novus, vel NIHIL si fractura
 */
GraphusEntitatum*
graphus_entitatum_creare(
    Piscina* piscina);


/* ==================================================
 * Additio Entitatum
 * ================================================== */

/* Addere entitatem ad graphum
 * Entitas iam debent habere ID internatum
 *
 * Redde: VERUM si successus, FALSUM si fractura vel ID iam existit
 */
b32
graphus_entitatum_addere_entitatem(
    GraphusEntitatum* graphus,
    Entitas*          entitas);


/* ==================================================
 * Providor Interface
 * ================================================== */

/* Creare EntitasProvidor ex GraphusEntitatum
 * Reddit interfacem abstractam ad hunc graphum
 *
 * Redde: EntitasProvidor*, vel NIHIL si fractura
 */
EntitasProvidor*
graphus_entitatum_providor_creare(
    GraphusEntitatum* graphus);


/* ==================================================
 * Utilitas
 * ================================================== */

/* Capere internamentum (pro creatio chordarum internatarum)
 */
InternamentumChorda*
graphus_entitatum_internamentum(
    GraphusEntitatum* graphus);

/* Capere numerum entitatum in grapho
 */
i32
graphus_entitatum_numerus(
    GraphusEntitatum* graphus);

#endif /* GRAPHUS_ENTITATUM_H */
