#ifndef GRAPHUS_PERSISTENS_H
#define GRAPHUS_PERSISTENS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "graphus_entitatum.h"
#include "entitas_repositorium.h"
#include "nuntium.h"
#include "filum.h"

/* ==================================================
 * Eventus Genera
 * ================================================== */

nomen enumeratio {
	EVENTUS_CREARE_ENTITAS    = 1,
	EVENTUS_DELERE_ENTITAS    = 2,
	EVENTUS_PONERE_PROPRIETAS = 3,
	EVENTUS_DELERE_PROPRIETAS = 4,
	EVENTUS_ADDERE_RELATIO    = 5,
	EVENTUS_DELERE_RELATIO    = 6,
	EVENTUS_ADDERE_NOTA       = 7,
	EVENTUS_DELERE_NOTA       = 8
} EventusGenus;

/* ==================================================
 * Eventus Tag Numbers
 * ================================================== */

/* Header tags (1-9) */
#define EVENTUS_TAG_ID          I
#define EVENTUS_TAG_GENUS       II
#define EVENTUS_TAG_ENTITAS_ID  III

/* Payload tags (10+) */
#define EVENTUS_TAG_ENTITAS_GENUS  X
#define EVENTUS_TAG_CLAVIS         X
#define EVENTUS_TAG_VALOR          XI
#define EVENTUS_TAG_RELATIO_ID     X
#define EVENTUS_TAG_RELATIO_GENUS  XI
#define EVENTUS_TAG_DESTINATIO_ID  XII
#define EVENTUS_TAG_NOTA           X

/* ==================================================
 * GraphusPersistens
 * ================================================== */

nomen structura GraphusPersistens GraphusPersistens;

/* Creare novum graphum persistentem
 * Si via_log existit, replay events to rebuild state
 * Si non existit, creare novum log file
 *
 * Redde: GraphusPersistens* si successus, NIHIL si error
 */
GraphusPersistens*
graphus_persistens_creare(
	Piscina*            piscina,
	constans character* via_log);

/* Aperire graphum persistentem existentem (readonly check)
 * Reddit NIHIL si log file non existit */
GraphusPersistens*
graphus_persistens_aperire(
	Piscina*            piscina,
	constans character* via_log);

/* Claudere graphum et sync ad discum */
vacuum
graphus_persistens_claudere(
	GraphusPersistens* gp);

/* Capere EntitasRepositorium interface
 * Usare hoc pro NavigatorEntitatum etc. */
EntitasRepositorium*
graphus_persistens_repositorium(
	GraphusPersistens* gp);

/* Capere GraphusEntitatum subiacentem (pro inspectione)
 * Nota: Non mutare directe! Usare repositorium interface. */
GraphusEntitatum*
graphus_persistens_graphus(
	GraphusPersistens* gp);

/* Sync pending writes ad discum (fflush)
 * Nota: Writes sunt iam synced per default post unumquemque eventum */
b32
graphus_persistens_sync(
	GraphusPersistens* gp);

/* ==================================================
 * Statisticae
 * ================================================== */

/* Capere numerum eventuum in log */
i32
graphus_persistens_numerus_eventuum(
	GraphusPersistens* gp);

/* Capere mensuram log file in bytes */
memoriae_index
graphus_persistens_mensura_log(
	GraphusPersistens* gp);

#endif /* GRAPHUS_PERSISTENS_H */
