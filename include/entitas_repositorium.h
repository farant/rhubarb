#ifndef ENTITAS_REPOSITORIUM_H
#define ENTITAS_REPOSITORIUM_H

#include "latina.h"
#include "entitas.h"
#include "chorda.h"
#include "xar.h"
#include "persistentia.h"

/* ==================================================
 * EntitasRepositorium - Repository Interface (Abstract)
 *
 * Pattern: Repository Interface
 * Consumers depend on this interface,
 * not on concrete implementation (GraphusEntitatum, GraphusPersistens)
 * ================================================== */

/* EntitasRepositorium - Interface abstracta pro repositorio entitatum
 *
 * Continet:
 * - datum: Index opacus ad implementationem concretam
 * - functiones: Indices functionum implementationis
 */
nomen structura EntitasRepositorium {
	vacuum* datum;  /* Implementatio concreta (opaca) */

	/* ==================================================
	 * Lectio (Read)
	 * ================================================== */

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

	/* Capere relationem per ID
	 *
	 * Redde: Relatio* si inventum, NIHIL si non inventum
	 */
	Relatio* (*capere_relatio)(
		vacuum* datum,
		chorda* relatio_id);

	/* Capere omnes relationes AD entitatem (back-references)
	 * Invenit relationes ubi destinatio_id == entitas_id
	 *
	 * Redde: Xar de Relatio*, vel NIHIL si nullae
	 */
	Xar* (*capere_relationes_ad)(
		vacuum* datum,
		chorda* entitas_id);

	/* ==================================================
	 * Scriptio (Write)
	 * ================================================== */

	/* Creare novam entitatem cum UUIDv7 auto-generato
	 *
	 * Redde: Entitas* si successus, NIHIL si error
	 */
	Entitas* (*entitas_creare)(
		vacuum*             datum,
		constans character* genus);

	/* Scaffoldare entitatem - creare si non existit, aliter redde existentem
	 * Utitur UUIDv5 deterministicum ex genere et titulo
	 * Idempotens: potest vocare multiple sine effectu
	 *
	 * genus:   Genus entitatis (e.g., "Page", "Folder")
	 * titulus: Titulus unicus intra genus (e.g., "root", "home")
	 *
	 * Redde: Entitas* (nova vel existens), NIHIL si error
	 */
	Entitas* (*entitas_scaffoldare)(
		vacuum*             datum,
		constans character* genus,
		constans character* titulus);

	/* Delere entitatem per ID
	 *
	 * Redde: VERUM si successus, FALSUM si error
	 */
	b32 (*entitas_delere)(
		vacuum* datum,
		chorda* id);

	/* Ponere proprietatem (addere vel superscribere)
	 *
	 * Redde: VERUM si successus, FALSUM si error
	 */
	b32 (*proprietas_ponere)(
		vacuum*             datum,
		Entitas*            entitas,
		constans character* clavis,
		constans character* valor);

	/* Delere proprietatem
	 *
	 * Redde: VERUM si successus, FALSUM si error
	 */
	b32 (*proprietas_delere)(
		vacuum*             datum,
		Entitas*            entitas,
		constans character* clavis);

	/* Addere relationem cum UUIDv7 auto-generato
	 *
	 * Redde: Relatio* si successus, NIHIL si error
	 */
	Relatio* (*relatio_addere)(
		vacuum*             datum,
		Entitas*            entitas,
		constans character* genus,
		chorda*             destinatio_id);

	/* Delere relationem per ID
	 *
	 * Redde: VERUM si successus, FALSUM si error
	 */
	b32 (*relatio_delere)(
		vacuum* datum,
		chorda* relatio_id);

	/* Addere notam ad entitatem
	 *
	 * Redde: VERUM si successus, FALSUM si error
	 */
	b32 (*nota_addere)(
		vacuum*             datum,
		Entitas*            entitas,
		constans character* nota);

	/* Delere notam ab entitate
	 *
	 * Redde: VERUM si successus, FALSUM si error
	 */
	b32 (*nota_delere)(
		vacuum*             datum,
		Entitas*            entitas,
		constans character* nota);

	/* ==================================================
	 * Lectio Eventorum (Event Log Read Access)
	 * ================================================== */

	/* Legere omnes eventus
	 * Redde: Xar de Eventum*, vel NIHIL si error
	 */
	Xar* (*legere_omnes_eventus)(
		vacuum* datum);

	/* Numerus eventorum in log
	 * Redde: numerus eventorum
	 */
	i32 (*numerus_eventorum)(
		vacuum* datum);

	/* Legere eventus pro entitate specifica
	 * Redde: Xar de Eventum*, vel NIHIL si nullae
	 */
	Xar* (*legere_eventus_entitatis)(
		vacuum* datum,
		chorda* entitas_id);

	/* Legere eventus post indicem (pro sync incrementali)
	 * Redde: Xar de Eventum*, vel NIHIL si nullae
	 */
	Xar* (*legere_eventus_post_indicem)(
		vacuum* datum,
		i32     index);

	/* Legere N eventus recentes
	 * Redde: Xar de Eventum*, vel NIHIL si nullae
	 */
	Xar* (*legere_eventus_recentes)(
		vacuum* datum,
		i32     numerus);

	/* Legere eventus pro genere entitatis (e.g., "Page", "Document")
	 * Redde: Xar de Eventum*, vel NIHIL si nullae
	 */
	Xar* (*legere_eventus_generis_entitatis)(
		vacuum*             datum,
		constans character* genus_entitatis);

	/* Legere eventus per typum eventi (e.g., EVENTUS_CREARE_ENTITAS)
	 * Redde: Xar de Eventum*, vel NIHIL si nullae
	 */
	Xar* (*legere_eventus_typi)(
		vacuum*      datum,
		EventusGenus typus);

} EntitasRepositorium;


/* ==================================================
 * Factory Function (Unified Implementation)
 * ================================================== */

/* Creare repositorium cum persistentia backend
 *
 * piscina: piscina pro allocationibus
 * persistentia: backend (memoria vel nuntium)
 *
 * On creation, replays events from persistentia to rebuild in-memory state.
 * All write operations emit events to persistentia AND update in-memory state.
 *
 * Redde: EntitasRepositorium* si successus, NIHIL si error
 */
EntitasRepositorium*
entitas_repositorium_creare(
    Piscina*      piscina,
    Persistentia* persistentia);

#endif /* ENTITAS_REPOSITORIUM_H */
