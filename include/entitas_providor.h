#ifndef ENTITAS_PROVIDOR_H
#define ENTITAS_PROVIDOR_H

#include "latina.h"
#include "entitas.h"
#include "chorda.h"
#include "xar.h"

/* ==================================================
 * EntitasProvidor - Repositorium Interface (Abstract)
 *
 * Pattern: Repository Interface
 * Navigator widget depends on this interface,
 * not on concrete implementation (GraphusEntitatum)
 * ================================================== */

/* EntitasProvidor - Interface abstracta pro repositorio entitatum
 *
 * Continet:
 * - datum: Index opacus ad implementationem concretam
 * - functiones: Indices functionum implementationis
 */
nomen structura EntitasProvidor {
    vacuum* datum;  /* Implementatio concreta (opaca) */

    /* Capere entitatem per ID
     * Redde: Entitas si inventum, NIHIL si non inventum
     */
    Entitas* (*capere_entitatem)(
        vacuum* datum,
        chorda* id);

    /* Capere entitates relatas per genus relationis
     * e.g., genus="contains" reddit filios
     *
     * Redde: Xar de Entitas*, vel NIHIL si nullae
     */
    Xar* (*capere_entitates_relatae)(
        vacuum*  datum,
        Entitas* entitas,
        chorda*  genus_relationis);

    /* Quaerere entitates cum nota exacta
     *
     * Redde: Xar de Entitas*, vel NIHIL si nullae
     */
    Xar* (*quaerere_cum_nota)(
        vacuum* datum,
        chorda* nota);

    /* Quaerere entitates cum praefixo notae
     * e.g., "#project::" invenit "#project::alpha", "#project::beta"
     *
     * Redde: Xar de Entitas*, vel NIHIL si nullae
     */
    Xar* (*quaerere_cum_praefixo_notae)(
        vacuum*              datum,
        constans character*  praefixum);

    /* Quaerere textum per IDs et valores proprietatum
     * Quaestio simplicis substring (non fuzzy pro nunc)
     *
     * Redde: Xar de Entitas*, vel NIHIL si nullae
     */
    Xar* (*quaerere_textum)(
        vacuum*              datum,
        constans character*  query);

    /* Capere entitates radices (initium navigationis)
     * Implementatio potest usare notam "#root" vel aliam logicam
     *
     * Redde: Xar de Entitas*, vel NIHIL si nullae
     */
    Xar* (*capere_radices)(
        vacuum* datum);

} EntitasProvidor;

#endif /* ENTITAS_PROVIDOR_H */
