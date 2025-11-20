#ifndef ENTITAS_H
#define ENTITAS_H

#include "latina.h"
#include "chorda.h"
#include "xar.h"
#include "piscina.h"

/* ==================================================
 * Structurae
 * ================================================== */

/* Proprietas - Par clavis-valoris
 * "Property: key-value pair"
 */
nomen structura {
    chorda* clavis;  /* Clavis (internata) */
    chorda* valor;   /* Valor (internatus) */
} Proprietas;

/* Relatio - Arcus directus ad aliam entitatem
 * "Relationship: directed edge to another entity"
 */
nomen structura {
    chorda* genus;         /* Genus relationis (internatum) */
    chorda* destinatio_id; /* ID entitatis destinationis (internatum) */
} Relatio;

/* Entitas - Entitas cum proprietatibus, relationibus, notis
 * "Entity with properties, relationships, tags"
 */
nomen structura {
    chorda* id;             /* ID unicum (internatum) */
    chorda* genus;          /* Nomen generis (internatum) */
    Xar*    proprietates;   /* Xar de Proprietas */
    Xar*    relationes;     /* Xar de Relatio */
    Xar*    notae;          /* Xar de chorda* (notae internatas) */
} Entitas;


/* ==================================================
 * Creatio
 * ================================================== */

/* Creare novam entitatem
 * Omnes chordae debent iam internatae esse
 *
 * Redde: Entitas nova, vel NIHIL si fractura
 */
Entitas*
entitas_creare(
    Piscina* piscina,
    chorda*  id,
    chorda*  genus);


/* ==================================================
 * Proprietates
 * ================================================== */

/* Addere proprietatem (clavis-valor par)
 * Si clavis iam existit, valor renovatur
 *
 * Redde: VERUM si successus, FALSUM si fractura
 */
b32
entitas_proprietas_addere(
    Entitas* entitas,
    chorda*  clavis,
    chorda*  valor);

/* Capere valorem proprietatis per clavis
 * Quaestio linearis per omnes proprietates
 *
 * Redde: Valor si inventum, NIHIL si non inventum
 */
chorda*
entitas_proprietas_capere(
    Entitas* entitas,
    chorda*  clavis);

/* Verificare si entitas proprietatem habet
 *
 * Redde: VERUM si habet, FALSUM si non
 */
b32
entitas_proprietas_habet(
    Entitas* entitas,
    chorda*  clavis);

/* Delere proprietatem per clavis
 * Usus swap-and-pop: proprietas ultima movetur ad locum deletum
 *
 * Redde: VERUM si deletum, FALSUM si non inventum
 */
b32
entitas_proprietas_delere(
    Entitas* entitas,
    chorda*  clavis);


/* ==================================================
 * Relationes
 * ================================================== */

/* Addere relationem ad aliam entitatem
 *
 * Redde: VERUM si successus, FALSUM si fractura
 */
b32
entitas_relatio_addere(
    Entitas* entitas,
    chorda*  genus,
    chorda*  destinatio_id);

/* Capere omnes relationes generis specificati
 * Allocat novum Xar cum relationibus generis
 *
 * Redde: Xar de Relatio*, vel NIHIL si nullae
 */
Xar*
entitas_relationes_generis_capere(
    Entitas* entitas,
    chorda*  genus,
    Piscina* piscina);

/* Delere relationem per indicem
 * Usus swap-and-pop: relatio ultima movetur ad locum deletum
 *
 * Redde: VERUM si deletum, FALSUM si index invalidus
 */
b32
entitas_relatio_delere(
    Entitas* entitas,
    i32      index);


/* ==================================================
 * Notae (Tags)
 * ================================================== */

/* Addere notam ad entitatem
 *
 * Redde: VERUM si successus, FALSUM si fractura
 */
b32
entitas_nota_addere(
    Entitas* entitas,
    chorda*  nota);

/* Verificare si entitas notam habet
 * Usus aequalitas indicis (chordae internatae)
 *
 * Redde: VERUM si habet, FALSUM si non
 */
b32
entitas_nota_habet(
    Entitas* entitas,
    chorda*  nota);

/* Verificare si entitas notam cum praefixo habet
 * e.g., "#project::" matches "#project::active"
 *
 * Redde: VERUM si habet, FALSUM si non
 */
b32
entitas_nota_cum_praefixo_habet(
    Entitas*             entitas,
    constans character*  praefixum);

/* Delere notam ab entitate
 * Usus swap-and-pop: nota ultima movetur ad locum deletum
 *
 * Redde: VERUM si deletum, FALSUM si non inventum
 */
b32
entitas_nota_delere(
    Entitas* entitas,
    chorda*  nota);


/* ==================================================
 * Utilitas
 * ================================================== */

/* Capere numerum proprietatum
 */
i32
entitas_numerus_proprietatum(
    Entitas* entitas);

/* Capere numerum relationum
 */
i32
entitas_numerus_relationum(
    Entitas* entitas);

/* Capere numerum notarum
 */
i32
entitas_numerus_notarum(
    Entitas* entitas);

/* Imprimere entitatem pro depuratione
 */
vacuum
entitas_imprimere(
    Entitas* entitas);

#endif /* ENTITAS_H */
