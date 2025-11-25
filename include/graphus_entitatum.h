#ifndef GRAPHUS_ENTITATUM_H
#define GRAPHUS_ENTITATUM_H

#include "latina.h"
#include "entitas.h"
#include "entitas_repositorium.h"
#include "tabula_dispersa.h"
#include "internamentum.h"
#include "piscina.h"

/* ==================================================
 * GraphusEntitatum - Repositorium Entitatum Concretum
 *
 * Implementatio concreta EntitasRepositorium
 * Entitates in tabula dispersa (hash table) conservat
 * ================================================== */

nomen structura {
    TabulaDispersa*      entitates;  /* id -> Entitas* */
    TabulaDispersa*      relationes; /* relatio_id -> Relatio* (index secundarius) */
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
 * Additio Relationum (Index)
 * ================================================== */

/* Registrare relationem in indice graphi
 * Vocatur post entitas_relatio_addere pro indexatione
 *
 * Redde: VERUM si successus, FALSUM si fractura
 */
b32
graphus_entitatum_registrare_relatio(
    GraphusEntitatum* graphus,
    Relatio*          relatio);

/* Deregistrare relationem ex indice graphi
 * Vocatur ante entitas_relatio_delere
 *
 * Redde: VERUM si successus, FALSUM si non inventum
 */
b32
graphus_entitatum_deregistrare_relatio(
    GraphusEntitatum* graphus,
    chorda*           relatio_id);


/* ==================================================
 * Repositorium Interface
 * ================================================== */

/* Creare EntitasRepositorium ex GraphusEntitatum
 * Reddit interfacem abstractam ad hunc graphum
 *
 * Redde: EntitasRepositorium*, vel NIHIL si fractura
 */
EntitasRepositorium*
graphus_entitatum_repositorium_creare(
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
