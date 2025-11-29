#ifndef PERSISTENTIA_H
#define PERSISTENTIA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

/* ==================================================
 * PERSISTENTIA - Event Sourcing Storage Interface
 *
 * Abstractio pro storing et retrieving eventus.
 * Implementationes: memoria (Xar), nuntium (file), sqlite (futura).
 *
 * Usus:
 *   Persistentia* pers = persistentia_memoria_creare(piscina);
 *   (vel: persistentia_nuntium_creare(piscina, "data.log"))
 *   EntitasRepositorium* repo = entitas_repositorium_creare(piscina, pers);
 *
 * ================================================== */


/* ==================================================
 * Serialization Tags (for nuntium format)
 * ================================================== */

/* Header tags (1-9) */
#define EVENTUM_TAG_ID          I
#define EVENTUM_TAG_GENUS       II
#define EVENTUM_TAG_ENTITAS_ID  III

/* Payload tags (10+) - shared across event types */
#define EVENTUM_TAG_ENTITAS_GENUS  X
#define EVENTUM_TAG_CLAVIS         X
#define EVENTUM_TAG_VALOR          XI
#define EVENTUM_TAG_RELATIO_ID     X
#define EVENTUM_TAG_RELATIO_GENUS  XI
#define EVENTUM_TAG_DESTINATIO_ID  XII
#define EVENTUM_TAG_NOTA           X


/* ==================================================
 * Eventum Types
 * ================================================== */

/* Genus eventuum */
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

/* Payload pro creare entitas */
nomen structura {
    chorda* entitas_genus;
} EventumCreare;

/* Payload pro proprietas (ponere vel delere) */
nomen structura {
    chorda* clavis;
    chorda* valor;  /* NIHIL pro delere */
} EventumProprietatis;

/* Payload pro relatio (addere vel delere) */
nomen structura {
    chorda* relatio_id;
    chorda* relatio_genus;     /* NIHIL pro delere */
    chorda* destinatio_id;     /* NIHIL pro delere */
} EventumRelationis;

/* Payload pro nota (addere vel delere) */
nomen structura {
    chorda* nota;
} EventumNotae;

/* Eventum - tagged union */
nomen structura {
    EventusGenus genus;
    chorda*      entitas_id;

    unio {
        EventumCreare       creare;
        EventumProprietatis proprietas;
        EventumRelationis   relatio;
        EventumNotae        nota;
    } datum;
} Eventum;


/* ==================================================
 * Persistentia Interface
 * ================================================== */

/* Forward declaration */
nomen structura Persistentia Persistentia;

/* Functio: scribere eventum */
nomen b32 (*PersistentiaScribereEventum)(
    vacuum*         datum,
    constans Eventum* eventum);

/* Functio: legere omnes eventus pro replay */
nomen Xar* (*PersistentiaLegereEventus)(
    vacuum*  datum,
    Piscina* piscina);

/* Functio: sync ad storage */
nomen b32 (*PersistentiaSync)(
    vacuum* datum);

/* Functio: claudere */
nomen vacuum (*PersistentiaClaudere)(
    vacuum* datum);

/* Interface */
structura Persistentia {
    vacuum* datum;

    PersistentiaScribereEventum scribere_eventum;
    PersistentiaLegereEventus   legere_eventus;
    PersistentiaSync            sync;
    PersistentiaClaudere        claudere;
};


/* ==================================================
 * Factory Functions
 * ================================================== */

/* Creare persistentia in memoria (Xar of Eventum)
 * Eventus non serializantur - manent in memoria.
 * Velox pro tests.
 *
 * piscina: piscina pro allocationibus
 *
 * Reddit: Persistentia* vel NIHIL si error
 */
Persistentia*
persistentia_memoria_creare(
    Piscina* piscina);

/* Creare persistentia cum file (nuntium/protobuf serialization)
 * Eventus serializantur et scribuntur ad file.
 * Pro productione.
 *
 * piscina: piscina pro allocationibus
 * via: path ad log file
 *
 * Reddit: Persistentia* vel NIHIL si error
 */
Persistentia*
persistentia_nuntium_creare(
    Piscina*            piscina,
    constans character* via);

/* Aperire persistentia ex file existente
 * Legit eventus pro replay.
 *
 * piscina: piscina pro allocationibus
 * via: path ad log file existentem
 *
 * Reddit: Persistentia* vel NIHIL si file non existit vel error
 */
Persistentia*
persistentia_nuntium_aperire(
    Piscina*            piscina,
    constans character* via);


/* ==================================================
 * Helper Functions
 * ================================================== */

/* Creare eventum pro creare entitas
 *
 * piscina: piscina pro allocatione
 * entitas_id: id entitatis
 * entitas_genus: genus entitatis
 *
 * Reddit: Eventum* vel NIHIL
 */
Eventum*
eventum_creare_entitas(
    Piscina* piscina,
    chorda*  entitas_id,
    chorda*  entitas_genus);

/* Creare eventum pro delere entitas */
Eventum*
eventum_delere_entitas(
    Piscina* piscina,
    chorda*  entitas_id);

/* Creare eventum pro ponere proprietas */
Eventum*
eventum_ponere_proprietas(
    Piscina* piscina,
    chorda*  entitas_id,
    chorda*  clavis,
    chorda*  valor);

/* Creare eventum pro delere proprietas */
Eventum*
eventum_delere_proprietas(
    Piscina* piscina,
    chorda*  entitas_id,
    chorda*  clavis);

/* Creare eventum pro addere relatio */
Eventum*
eventum_addere_relatio(
    Piscina* piscina,
    chorda*  entitas_id,
    chorda*  relatio_id,
    chorda*  relatio_genus,
    chorda*  destinatio_id);

/* Creare eventum pro delere relatio */
Eventum*
eventum_delere_relatio(
    Piscina* piscina,
    chorda*  entitas_id,
    chorda*  relatio_id);

/* Creare eventum pro addere nota */
Eventum*
eventum_addere_nota(
    Piscina* piscina,
    chorda*  entitas_id,
    chorda*  nota);

/* Creare eventum pro delere nota */
Eventum*
eventum_delere_nota(
    Piscina* piscina,
    chorda*  entitas_id,
    chorda*  nota);

#endif /* PERSISTENTIA_H */
