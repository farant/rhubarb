# Actor: Message-Passing System Documentation

Every entity is an actor. Every actor has a mailbox. Actors communicate by sending messages. That's the actor model, and this is the implementation.

The actor library is actually a constellation of five tightly integrated libraries that together form a complete actor system:

1. **actor** (`actor.h`) - Messages (`Nuntius`) and mailboxes (`Capsa`)
2. **tractator** (`tractator.h`) - Handler registry (which function handles which message for which entity type)
3. **cursus** (`cursus.h`) - Message bus (central dispatcher that routes messages to mailboxes and invokes handlers)
4. **tempestivum** (`tempestivum.h`) - Timer entities (periodic message delivery, like a clock tick)
5. **servitium** (`servitium.h`) - Service registry (singleton actors you can look up by name)

These layers build on top of the entity graph system (`entitas.h`, `entitas_repositorium.h`), which provides persistence, properties, relationships, and tags for all entities.

## Philosophy

The design follows the actor model with some specific choices:

- **Every entity is an actor.** You don't opt in. If an entity has a `genus` (type) with registered handlers, it receives and processes messages. If not, messages are no-ops.
- **Actors communicate exclusively through messages.** No shared mutable state between actors.
- **Mailboxes are lazily allocated.** An entity doesn't get a mailbox until it receives its first message. This is efficient for large entity graphs where most entities never receive messages.
- **Message payload is a hash table.** Each message carries a `TabulaDispersa` (hash table) of key-value pairs, where both keys and values are `chorda*` (interned strings). Type conversion helpers are provided for `s32`, `s64`, `f64`, and `b32`.
- **Handlers are dispatched by (entity_genus, message_genus) pair.** A "Clock" entity receiving a "tick" message dispatches to the handler registered for `("Clock", "tick")`.
- **Messages are enqueued, not immediately dispatched.** You call `cursus_processare()` to drain the queue, with a configurable limit per frame.
- **Timers are entities.** A timer is a `System::Tempestivum` entity with properties for interval, target, and message type. The timer administrator queries the entity repository each frame for active timers.

## The Data Flow

Here's how a message moves through the system:

```
1. Something calls cursus_mittere(cursus, sender_id, dest_id, "tick", data)
2. CursusNuntiorum creates a Nuntius and enqueues it in the destination's Capsa
3. (Capsa is lazily created if this is the first message to that entity)
4. Later, cursus_processare(cursus, max) is called (e.g., in the game loop)
5. For each Capsa with pending messages:
   a. Look up the destination Entitas from the repository
   b. Get the entity's genus (e.g., "Clock")
   c. Get the message's genus (e.g., "tick")
   d. Look up the handler in RegistrumTractatoris for ("Clock", "tick")
   e. Build a ContextusTractandi with entity, message, repository, cursus
   f. Call the handler function
6. Handler can read message data, modify entity properties, send new messages
```

---

# Core Library: actor.h

The `actor.h` header defines two fundamental types: `Nuntius` (message) and `Capsa` (mailbox).

## Nuntius (Message)

A message is a typed envelope sent from one actor to another.

```c
nomen structura Nuntius {
    chorda*              mittens_id;       /* Sender ID (NIHIL for system) */
    chorda*              genus;            /* Message type (interned string) */
    TabulaDispersa*      datum;            /* Payload: key-value hash table */
    f64                  tempus_creatus;   /* Creation timestamp */
    Piscina*             piscina;          /* Memory pool */
    InternamentumChorda* intern;           /* String interment pool */
} Nuntius;
```

**Fields:**
- `mittens_id` - The sender's entity ID. `NIHIL` for system-generated messages (timers, etc.).
- `genus` - The message type as an interned string (e.g., `"tick"`, `"click"`, `"open"`). Interning means pointer comparison works for type checking.
- `datum` - The payload. This is a `TabulaDispersa` (hash table) mapping `chorda` keys to `chorda*` values. Lazily allocated -- `NIHIL` until the first key-value pair is set.
- `tempus_creatus` - Timestamp from `tempus_nunc()` recording when the message was created.
- `piscina` / `intern` - Internal references for allocating payload data.

### Creating Messages

```c
Nuntius* nuntius_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    chorda*              mittens_id,    /* NIHIL for system messages */
    constans character*  genus);        /* Message type as C string */
```

Creates a new message. The `genus` string is automatically interned. Returns `NIHIL` if any required argument (`piscina`, `intern`, `genus`) is missing.

```c
/* System message (no sender) */
Nuntius* tick = nuntius_creare(piscina, intern, NIHIL, "tick");

/* Message from a specific actor */
chorda  sender_str = chorda_ex_literis("actor-123", piscina);
chorda* sender_id  = chorda_internare(intern, sender_str);
Nuntius* click = nuntius_creare(piscina, intern, sender_id, "click");
```

### Setting Payload Data

The payload is a hash table. You set key-value pairs using typed setters:

```c
/* Set a chorda* value directly */
b32 nuntius_datum_ponere(
    Nuntius*            nuntius,
    constans character* clavis,     /* Key as C string */
    chorda*             valor);     /* Value as interned chorda* */

/* Set from C string literals (auto-interns) */
b32 nuntius_datum_ponere_literis(
    Nuntius*            nuntius,
    constans character* clavis,
    constans character* valor);

/* Set s32 value (auto-converts to string) */
b32 nuntius_datum_ponere_s32(
    Nuntius*            nuntius,
    constans character* clavis,
    s32                 valor);
```

All setters return `VERUM` on success, `FALSUM` on failure. The `datum` hash table is lazily allocated on the first `ponere` call.

```c
Nuntius* msg = nuntius_creare(piscina, intern, NIHIL, "update");

/* msg->datum is NIHIL here */

nuntius_datum_ponere_literis(msg, "titulus", "Salve Munde");
nuntius_datum_ponere_s32(msg, "numerus", XLII);

/* msg->datum is now allocated with 2 entries */
```

### Getting Payload Data

```c
/* Get raw chorda* value */
chorda* nuntius_datum_capere(
    Nuntius*            nuntius,
    constans character* clavis);    /* Returns NIHIL if not found */

/* Get typed values (returns VERUM if found and parsed) */
b32 nuntius_datum_capere_s32(Nuntius* nuntius, constans character* clavis, s32* valor);
b32 nuntius_datum_capere_s64(Nuntius* nuntius, constans character* clavis, s64* valor);
b32 nuntius_datum_capere_f64(Nuntius* nuntius, constans character* clavis, f64* valor);
b32 nuntius_datum_capere_b32(Nuntius* nuntius, constans character* clavis, b32* valor);
```

Typed getters parse the string value into the requested type. The `b32` getter accepts `"true"`, `"verum"`, `"1"` for true and `"false"`, `"falsum"`, `"0"` for false. All getters return `FALSUM` if the key is missing or parsing fails.

```c
s32 numerus;
si (nuntius_datum_capere_s32(msg, "numerus", &numerus))
{
    imprimere("Numerus: %d\n", numerus);
}
```

### Checking and Printing

```c
/* Check if a key exists in the payload */
b32 nuntius_datum_habet(Nuntius* nuntius, constans character* clavis);

/* Debug print a message */
vacuum nuntius_imprimere(Nuntius* nuntius);
```

`nuntius_imprimere` prints the sender, type, timestamp, and all key-value pairs. Useful for debugging message flow.

---

## Capsa (Mailbox)

A mailbox is a FIFO queue of messages belonging to an actor.

```c
nomen structura Capsa {
    chorda*  possessor_id;    /* Owner entity ID */
    Xar*     nuntii;          /* Xar of Nuntius* (FIFO queue) */
    Piscina* piscina;         /* Memory pool */
} Capsa;
```

### Creating a Mailbox

```c
Capsa* capsa_creare(Piscina* piscina, chorda* possessor_id);
```

Creates a mailbox for the given entity. Both arguments are required -- returns `NIHIL` if either is missing.

In practice, you rarely create mailboxes directly. The `CursusNuntiorum` (message bus) creates them lazily when an entity first receives a message.

### Mailbox Operations

```c
/* Enqueue a message (append to end) */
b32 capsa_addere(Capsa* capsa, Nuntius* nuntius);

/* Dequeue first message (FIFO - removes and returns) */
Nuntius* capsa_tollere(Capsa* capsa);

/* Peek at first message without removing */
Nuntius* capsa_inspicere(Capsa* capsa);

/* Count pending messages */
i32 capsa_numerus(Capsa* capsa);

/* Check if empty */
b32 capsa_vacua_est(Capsa* capsa);

/* Clear all messages */
vacuum capsa_vacare(Capsa* capsa);
```

The FIFO order is maintained by shifting elements when dequeuing (`capsa_tollere`). This is O(n) per dequeue, which is acceptable for typical mailbox sizes but worth noting if an actor accumulates thousands of pending messages.

```c
/* FIFO ordering */
capsa_addere(capsa, msg1);
capsa_addere(capsa, msg2);
capsa_addere(capsa, msg3);

capsa_tollere(capsa);  /* Returns msg1 */
capsa_tollere(capsa);  /* Returns msg2 */
capsa_tollere(capsa);  /* Returns msg3 */
capsa_tollere(capsa);  /* Returns NIHIL (empty) */
```

---

# Handler Registry: tractator.h

The handler registry maps `(entity_genus, message_genus)` pairs to handler functions. When a "Clock" entity receives a "tick" message, the registry tells the system which function to call.

## ContextusTractandi (Handler Context)

Every handler receives a context struct with everything it needs:

```c
nomen structura ContextusTractandi {
    Entitas*                    entitas;       /* The receiving entity */
    Nuntius*                    nuntius;        /* The message being handled */
    EntitasRepositorium*        repositorium;   /* For querying/modifying entities */
    structura CursusNuntiorum*  cursus;         /* For sending messages to others */
    Piscina*                    piscina;        /* For temporary allocations */
    vacuum*                     datum_custom;   /* Application-specific data */
} ContextusTractandi;
```

This is the handler's window into the entire system. It can:
- Read the message payload (`ctx->nuntius`)
- Inspect and modify the entity's properties (`ctx->entitas`, `ctx->repositorium`)
- Send messages to other entities (`ctx->cursus`)
- Access application-specific state (`ctx->datum_custom`)

## FunctioTractandi (Handler Function Signature)

```c
nomen b32 (*FunctioTractandi)(ContextusTractandi* ctx);
```

All handlers have this signature. Return `VERUM` if the message was handled successfully, `FALSUM` on error.

```c
/* Example handler */
interior b32
clock_tractare_tick(ContextusTractandi* ctx)
{
    /* Read a property */
    chorda* ticks_clavis = chorda_internare_ex_literis(intern, "ticks");
    chorda* current = entitas_proprietas_capere(ctx->entitas, ticks_clavis);

    /* Update the entity */
    ctx->repositorium->proprietas_ponere(
        ctx->repositorium->datum,
        ctx->entitas,
        "ticks",
        "42");

    redde VERUM;
}
```

## RegistrumTractatoris

```c
nomen structura RegistrumTractatoris {
    TabulaDispersa*      genera;       /* genus_entitatis -> (genus_nuntii -> handler) */
    TabulaDispersa*      fallbacks;    /* genus_entitatis -> fallback handler */
    Piscina*             piscina;
    InternamentumChorda* intern;
} RegistrumTractatoris;
```

Internally, this is a two-level hash table. The outer table maps entity genus to an inner table, which maps message genus to handler function.

### Creating the Registry

```c
RegistrumTractatoris* registrum_tractatoris_creare(
    Piscina*             piscina,
    InternamentumChorda* intern);
```

### Registering Handlers

```c
/* Register a specific handler for (entity_type, message_type) */
b32 registrum_tractatoris_registrare(
    RegistrumTractatoris* reg,
    constans character*   genus_entitatis,   /* e.g., "Clock" */
    constans character*   genus_nuntii,      /* e.g., "tick" */
    FunctioTractandi      functio);

/* Register a fallback handler for an entity type */
b32 registrum_tractatoris_ponere_fallback(
    RegistrumTractatoris* reg,
    constans character*   genus_entitatis,
    FunctioTractandi      functio);
```

The fallback handler is called when a message arrives for an entity type that has a fallback registered but no specific handler for that message type. This lets an entity respond to arbitrary messages.

```c
RegistrumTractatoris* reg = registrum_tractatoris_creare(piscina, intern);

/* Clock handles "tick" messages */
registrum_tractatoris_registrare(reg, "Clock", "tick", clock_tractare_tick);

/* Widget handles everything with a fallback */
registrum_tractatoris_ponere_fallback(reg, "Widget", widget_fallback);
```

### Looking Up Handlers

```c
/* Look up by interned chorda* */
FunctioTractandi registrum_tractatoris_invenire(
    RegistrumTractatoris* reg,
    chorda*               genus_entitatis,
    chorda*               genus_nuntii);

/* Look up by C string */
FunctioTractandi registrum_tractatoris_invenire_literis(
    RegistrumTractatoris* reg,
    constans character*   genus_entitatis,
    constans character*   genus_nuntii);

/* Check if a handler exists */
b32 registrum_tractatoris_habet(
    RegistrumTractatoris* reg,
    constans character*   genus_entitatis,
    constans character*   genus_nuntii);
```

Lookup tries the specific handler first, then falls back to the fallback handler. Returns `NIHIL` if neither is registered (in which case the message is a no-op).

### Utility

```c
i32  registrum_tractatoris_numerus(RegistrumTractatoris* reg);
vacuum registrum_tractatoris_imprimere(RegistrumTractatoris* reg);
```

---

# Message Bus: cursus.h

The `CursusNuntiorum` (message bus) is the central dispatcher. It owns all the mailboxes, routes messages, and invokes handlers.

```c
nomen structura CursusNuntiorum {
    TabulaDispersa*       capsae;         /* entity_id -> Capsa* */
    RegistrumTractatoris* tractator;      /* Handler registry */
    EntitasRepositorium*  repositorium;   /* Entity repository */
    InternamentumChorda*  intern;
    Piscina*              piscina;
    vacuum*               datum_custom;   /* Application-specific data */
} CursusNuntiorum;
```

### Creating the Message Bus

```c
CursusNuntiorum* cursus_creare(
    Piscina*              piscina,
    RegistrumTractatoris* tractator,
    EntitasRepositorium*  repositorium,
    InternamentumChorda*  intern);

/* Set application-specific data (passed to handlers via ctx->datum_custom) */
vacuum cursus_ponere_datum(CursusNuntiorum* cursus, vacuum* datum);
```

### Sending Messages

Three ways to send a message:

```c
/* Full send: with sender, destination, type, and optional payload */
b32 cursus_mittere(
    CursusNuntiorum*    cursus,
    chorda*             mittens_id,        /* Sender (can be NIHIL) */
    chorda*             destinatio_id,     /* Destination entity ID */
    constans character* genus_nuntii,      /* Message type */
    TabulaDispersa*     datum);            /* Payload (can be NIHIL) */

/* Send a pre-built message */
b32 cursus_mittere_nuntium(
    CursusNuntiorum* cursus,
    chorda*          destinatio_id,
    Nuntius*         nuntius);

/* Simple send: no payload */
b32 cursus_mittere_simplex(
    CursusNuntiorum*    cursus,
    chorda*             mittens_id,
    chorda*             destinatio_id,
    constans character* genus_nuntii);
```

All sends enqueue the message in the destination's mailbox. They do **not** immediately dispatch. You must call `cursus_processare()` to actually invoke handlers.

```c
/* Send a tick to the clock entity */
cursus_mittere_simplex(cursus, NIHIL, clock->id, "tick");

/* Nothing happens yet -- message is queued */

/* Now process pending messages */
i32 tractati = cursus_processare(cursus, C);  /* Process up to 100 */
```

### Processing Messages

```c
/* Process up to maximus_nuntii pending messages across all mailboxes */
i32 cursus_processare(
    CursusNuntiorum* cursus,
    i32              maximus_nuntii);

/* Process messages for a specific entity only */
i32 cursus_processare_entitatem(
    CursusNuntiorum* cursus,
    chorda*          entitas_id,
    i32              maximus_nuntii);
```

`cursus_processare` iterates through all mailboxes, dequeues messages, looks up handlers, and invokes them. The `maximus_nuntii` limit prevents one frame from spending too long processing messages.

Returns the number of messages actually processed.

### Querying

```c
/* Total pending messages across all mailboxes */
i32 cursus_numerus_pendentium(CursusNuntiorum* cursus);

/* Pending messages for a specific entity */
i32 cursus_numerus_pendentium_entitatis(CursusNuntiorum* cursus, chorda* entitas_id);

/* Check if entity has a mailbox */
b32 cursus_habet_capsam(CursusNuntiorum* cursus, chorda* entitas_id);

/* Get entity's mailbox (NIHIL if none) */
Capsa* cursus_obtinere_capsam(CursusNuntiorum* cursus, chorda* entitas_id);
```

### Utility

```c
vacuum cursus_vacare(CursusNuntiorum* cursus);     /* Clear all mailboxes */
vacuum cursus_imprimere(CursusNuntiorum* cursus);   /* Debug print */
```

---

# Timer System: tempestivum.h

Timers are entities. Not callbacks, not abstract timer IDs -- actual entities in the repository with genus `"System::Tempestivum"` and properties like interval, target, and active state.

This means timers are queryable, pausable, serializable, and debuggable just like any other entity.

## Timer Properties

Each timer entity has these properties (stored as entity properties):

| Property | Key | Type | Description |
|---|---|---|---|
| Destination | `"destinatio_id"` | chorda | Target entity to receive messages |
| Message type | `"genus_nuntii"` | chorda | Message type to send (e.g., "tick") |
| Interval | `"intervallum"` | s64 (ms) | Milliseconds between firings |
| Repeat | `"repetere"` | b32 | Whether timer repeats or fires once |
| Next fire time | `"proximum_tempus"` | s64 (ms) | Next activation time (epoch ms) |
| Active | `"activum"` | b32 | Whether timer is currently active |

## AdministratorTempestivi

```c
nomen structura AdministratorTempestivi {
    EntitasRepositorium* repositorium;
    CursusNuntiorum*     cursus;
    InternamentumChorda* intern;
    Piscina*             piscina;
    chorda*              genus_tempestivi;   /* Cached interned "System::Tempestivum" */
} AdministratorTempestivi;
```

### Creating the Timer Administrator

```c
AdministratorTempestivi* administrator_tempestivi_creare(
    Piscina*             piscina,
    EntitasRepositorium* repositorium,
    CursusNuntiorum*     cursus,
    InternamentumChorda* intern);
```

### Creating Timers

```c
/* Create a timer that fires immediately, then repeats at interval */
Entitas* tempestivum_creare(
    AdministratorTempestivi* admin,
    chorda*                  destinatio_id,      /* Target entity */
    constans character*      genus_nuntii,       /* Message to send */
    s64                      intervallum_ms,     /* Interval in ms */
    b32                      repetere);          /* Repeat? */

/* Create a timer with initial delay before first firing */
Entitas* tempestivum_creare_cum_delay(
    AdministratorTempestivi* admin,
    chorda*                  destinatio_id,
    constans character*      genus_nuntii,
    s64                      intervallum_ms,
    s64                      delay_ms,           /* Delay before first fire */
    b32                      repetere);
```

Both return the timer `Entitas*`, which can be used to pause, resume, or cancel the timer.

```c
/* Create a repeating 10ms tick timer */
Entitas* timer = tempestivum_creare_cum_delay(
    admin,
    clock->id,      /* Send to clock entity */
    "tick",         /* Message type */
    X,              /* 10ms interval */
    X,              /* 10ms initial delay */
    VERUM);         /* Repeat */
```

### Controlling Timers

```c
/* Pause a timer (stops firing, remembers state) */
b32 tempestivum_pausare(AdministratorTempestivi* admin, chorda* tempestivum_id);

/* Resume a paused timer */
b32 tempestivum_resumere(AdministratorTempestivi* admin, chorda* tempestivum_id);

/* Cancel a timer permanently */
b32 tempestivum_annullare(AdministratorTempestivi* admin, chorda* tempestivum_id);
```

Use the timer entity's `id` field:

```c
tempestivum_pausare(admin, timer->id);     /* Paused -- no ticks */
tempestivum_resumere(admin, timer->id);    /* Resumed -- ticks again */
tempestivum_annullare(admin, timer->id);   /* Cancelled -- gone */
```

### Processing Timers

```c
i32 tempestivum_processare(AdministratorTempestivi* admin);
```

Call this every frame. It queries the repository for all `System::Tempestivum` entities, checks if `proximum_tempus <= now`, and sends messages for any that have fired. For repeating timers, it updates `proximum_tempus` to the next interval. For one-shot timers, it deactivates them.

Returns the number of messages sent.

### Querying Timers

```c
/* Count active timers */
i32 tempestivum_numerus_activorum(AdministratorTempestivi* admin);

/* Get all timers targeting a specific entity */
Xar* tempestivum_pro_destinatione(AdministratorTempestivi* admin, chorda* destinatio_id);

/* Check if a specific timer is active */
b32 tempestivum_est_activum(AdministratorTempestivi* admin, chorda* tempestivum_id);

/* Debug print */
vacuum tempestivum_imprimere(AdministratorTempestivi* admin);
```

---

# Service Registry: servitium.h

Services are singleton actors -- entities you can look up by name instead of ID. Think "the clock service", "the logger service", "the input manager".

```c
nomen structura RegistrumServitii {
    TabulaDispersa*      nomina;          /* service_name -> entity_id chorda* */
    EntitasRepositorium* repositorium;
    InternamentumChorda* intern;
    Piscina*             piscina;
} RegistrumServitii;
```

### Creating the Service Registry

```c
RegistrumServitii* registrum_servitii_creare(
    Piscina*             piscina,
    EntitasRepositorium* repositorium,
    InternamentumChorda* intern);
```

### Registering Services

```c
/* Register by entity ID */
b32 servitium_registrare(
    RegistrumServitii*  reg,
    constans character* titulus,        /* Service name (e.g., "clock") */
    chorda*             entitas_id);

/* Register with an entity pointer */
b32 servitium_registrare_entitatem(
    RegistrumServitii*  reg,
    constans character* titulus,
    Entitas*            entitas);

/* Unregister */
b32 servitium_deregistrare(
    RegistrumServitii*  reg,
    constans character* titulus);
```

Registration automatically adds a tag `"#service::name"` to the entity, enabling auto-discovery.

```c
servitium_registrare_entitatem(reg, "clock", clock_entity);
/* clock_entity now has tag "#service::clock" */
```

### Looking Up Services

```c
/* Get entity by service name */
Entitas* servitium_obtinere(RegistrumServitii* reg, constans character* titulus);

/* Get entity ID by service name */
chorda* servitium_obtinere_id(RegistrumServitii* reg, constans character* titulus);

/* Check if a service is registered */
b32 servitium_habet(RegistrumServitii* reg, constans character* titulus);

/* Count registered services */
i32 servitium_numerus(RegistrumServitii* reg);
```

```c
Entitas* clock = servitium_obtinere(reg, "clock");
si (clock)
{
    /* Send it a message */
    cursus_mittere_simplex(cursus, NIHIL, clock->id, "tick");
}
```

### Auto-Discovery

```c
i32 servitium_discoperire(RegistrumServitii* reg);
```

Scans all entities for tags with the `"#service::"` prefix and automatically registers them as services. The service name is extracted from the tag (everything after `"#service::"`).

This is useful when loading entities from persistence -- services tagged before shutdown are automatically re-discovered.

---

# Complete Example: Clock Widget

This example (adapted from `probatio_actor_integratio.c`) shows the full actor system working together:

```c
#include "cursus.h"
#include "tractator.h"
#include "tempestivum.h"
#include "servitium.h"
#include "entitas_repositorium.h"
#include "persistentia.h"

/* Global state for the clock (simplified for example) */
hic_manens i32 g_tick_count = ZEPHYRUM;

/* Handler for Clock::tick messages */
interior b32
clock_tractare_tick(ContextusTractandi* ctx)
{
    character buffer[XXXII];
    g_tick_count++;

    sprintf(buffer, "%d", g_tick_count);
    ctx->repositorium->proprietas_ponere(
        ctx->repositorium->datum,
        ctx->entitas,
        "ticks",
        buffer);

    redde VERUM;
}

/* Game loop frame */
interior vacuum
simulare_quadrum(
    AdministratorTempestivi* admin,
    CursusNuntiorum*         cursus)
{
    tempestivum_processare(admin);       /* Fire due timers */
    cursus_processare(cursus, C);        /* Process up to 100 messages */
}

s32 principale(vacuum)
{
    Piscina*                 piscina;
    InternamentumChorda*     intern;
    Persistentia*            pers;
    EntitasRepositorium*     repo;
    RegistrumTractatoris*    tractator;
    CursusNuntiorum*         cursus;
    AdministratorTempestivi* tempestivum_admin;
    RegistrumServitii*       servitium_reg;
    Entitas*                 clock;
    Entitas*                 timer;

    /* Initialize infrastructure */
    piscina = piscina_generare_dynamicum("actor_demo", MMM * CXXVIII);
    intern  = internamentum_globale();
    pers    = persistentia_memoria_creare(piscina);
    repo    = entitas_repositorium_creare(piscina, pers);

    /* Create handler registry and register handlers */
    tractator = registrum_tractatoris_creare(piscina, intern);
    registrum_tractatoris_registrare(tractator, "Clock", "tick", clock_tractare_tick);

    /* Create message bus */
    cursus = cursus_creare(piscina, tractator, repo, intern);

    /* Create timer administrator */
    tempestivum_admin = administrator_tempestivi_creare(piscina, repo, cursus, intern);

    /* Create service registry */
    servitium_reg = registrum_servitii_creare(piscina, repo, intern);

    /* Create clock entity */
    clock = repo->entitas_scaffoldare(repo->datum, "Clock", "system::clock");
    repo->proprietas_ponere(repo->datum, clock, "ticks", "0");

    /* Register as service */
    servitium_registrare_entitatem(servitium_reg, "clock", clock);

    /* Create 10ms repeating timer */
    timer = tempestivum_creare_cum_delay(
        tempestivum_admin,
        clock->id,
        "tick",
        X,          /* 10ms interval */
        X,          /* 10ms initial delay */
        VERUM);     /* Repeat */

    /* Simulate 3 frames */
    usleep(15000);
    simulare_quadrum(tempestivum_admin, cursus);
    /* g_tick_count == 1 */

    usleep(15000);
    simulare_quadrum(tempestivum_admin, cursus);
    /* g_tick_count == 2 */

    usleep(15000);
    simulare_quadrum(tempestivum_admin, cursus);
    /* g_tick_count == 3 */

    /* Pause timer */
    tempestivum_pausare(tempestivum_admin, timer->id);
    usleep(15000);
    simulare_quadrum(tempestivum_admin, cursus);
    /* g_tick_count still == 3 */

    /* Resume timer */
    tempestivum_resumere(tempestivum_admin, timer->id);
    usleep(15000);
    simulare_quadrum(tempestivum_admin, cursus);
    /* g_tick_count == 4 */

    /* Cancel timer */
    tempestivum_annullare(tempestivum_admin, timer->id);

    /* Look up service */
    Entitas* found = servitium_obtinere(servitium_reg, "clock");
    /* found == clock */

    /* Send a direct message (no timer) */
    cursus_mittere_simplex(cursus, NIHIL, clock->id, "tick");
    cursus_processare(cursus, M);
    /* g_tick_count == 5 */

    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
```

---

# Relationship to Other Libraries

## Dependencies

The actor system depends on these rhubarb libraries:

| Library | Role |
|---|---|
| `piscina.h` | Arena memory allocation for all objects |
| `chorda.h` | String type (non-null-terminated) |
| `internamentum.h` | String interning for efficient comparison |
| `tabula_dispersa.h` | Hash tables for message payloads and registries |
| `xar.h` | Variable-length arrays for mailbox queues |
| `entitas.h` | Entity type (properties, relationships, tags) |
| `entitas_repositorium.h` | Entity persistence and queries |
| `persistentia.h` | Persistence backend (memory or disk) |
| `uuid.h` | UUIDv7 generation for entity IDs |
| `tempus.h` | Timestamps for message creation times |

## Relationship to reactor.h

The **reactor** (`reactor.h`) is a separate, lower-level system for I/O multiplexing (poll-based event loop for file descriptors). It operates independently from the actor system:

- **Reactor** = I/O events (sockets, files, pipes) + system timers
- **Actor system** = application-level message passing between entities

They can work together: a reactor callback could send actor messages via `cursus_mittere` when network data arrives, bridging I/O events into the actor world.

---

# Gotchas and Important Notes

1. **Messages are enqueued, not dispatched.** Calling `cursus_mittere` only puts the message in a mailbox. You must call `cursus_processare()` to actually invoke handlers. If you forget to process, messages accumulate forever.

2. **Mailboxes are lazily allocated.** Don't assume an entity has a mailbox. The first message to an entity creates its mailbox in the `CursusNuntiorum`'s `capsae` hash table.

3. **Message payload is always strings.** Even `s32` values are converted to strings internally and parsed back when retrieved. This simplifies serialization but means type-checking happens at read time, not write time.

4. **Handler dispatch requires matching genus.** If an entity's `genus` (type) doesn't have a registered handler for the incoming message type, and there's no fallback handler, the message is silently dropped. No error, no warning.

5. **Timer processing is per-frame.** You must call `tempestivum_processare()` regularly (typically once per game loop iteration). If you skip frames, timers may fire multiple times on the next process call to catch up, depending on how far behind you are.

6. **chorda is not null-terminated.** The `chorda` type stores length + data without a null terminator. Don't pass `chorda.datum` to standard C string functions like `strcmp` or `printf("%s", ...)`. Use `%.*s` format with `mensura` and `datum`, or use the `chorda_aequalis_literis` helper.

7. **All strings should be interned.** The actor system relies heavily on pointer comparison for interned strings. Always use `chorda_internare` or `chorda_internare_ex_literis` for strings that will be compared (entity IDs, message types, keys).

8. **Capsa dequeue is O(n).** The `capsa_tollere` function shifts all remaining messages forward. For typical use (a few messages per entity per frame), this is fine. For high-throughput scenarios, consider batching.

---

# API Quick Reference

## actor.h

| Function | Description |
|---|---|
| `nuntius_creare(piscina, intern, mittens_id, genus)` | Create a message |
| `nuntius_datum_ponere(nuntius, clavis, valor)` | Set chorda* payload value |
| `nuntius_datum_ponere_literis(nuntius, clavis, valor)` | Set C string payload value |
| `nuntius_datum_ponere_s32(nuntius, clavis, valor)` | Set s32 payload value |
| `nuntius_datum_capere(nuntius, clavis)` | Get chorda* payload value |
| `nuntius_datum_capere_s32(nuntius, clavis, &valor)` | Get s32 payload value |
| `nuntius_datum_capere_s64(nuntius, clavis, &valor)` | Get s64 payload value |
| `nuntius_datum_capere_f64(nuntius, clavis, &valor)` | Get f64 payload value |
| `nuntius_datum_capere_b32(nuntius, clavis, &valor)` | Get b32 payload value |
| `nuntius_datum_habet(nuntius, clavis)` | Check if key exists |
| `nuntius_imprimere(nuntius)` | Debug print message |
| `capsa_creare(piscina, possessor_id)` | Create a mailbox |
| `capsa_addere(capsa, nuntius)` | Enqueue message |
| `capsa_tollere(capsa)` | Dequeue first message (FIFO) |
| `capsa_inspicere(capsa)` | Peek at first message |
| `capsa_numerus(capsa)` | Count pending messages |
| `capsa_vacua_est(capsa)` | Check if empty |
| `capsa_vacare(capsa)` | Clear all messages |

## tractator.h

| Function | Description |
|---|---|
| `registrum_tractatoris_creare(piscina, intern)` | Create handler registry |
| `registrum_tractatoris_registrare(reg, genus_entitatis, genus_nuntii, functio)` | Register handler |
| `registrum_tractatoris_ponere_fallback(reg, genus_entitatis, functio)` | Register fallback handler |
| `registrum_tractatoris_invenire(reg, genus_entitatis, genus_nuntii)` | Look up handler (chorda*) |
| `registrum_tractatoris_invenire_literis(reg, genus_entitatis, genus_nuntii)` | Look up handler (C string) |
| `registrum_tractatoris_habet(reg, genus_entitatis, genus_nuntii)` | Check if handler registered |
| `registrum_tractatoris_numerus(reg)` | Count registered handlers |
| `registrum_tractatoris_imprimere(reg)` | Debug print registry |

## cursus.h

| Function | Description |
|---|---|
| `cursus_creare(piscina, tractator, repositorium, intern)` | Create message bus |
| `cursus_ponere_datum(cursus, datum)` | Set custom data |
| `cursus_mittere(cursus, mittens_id, destinatio_id, genus_nuntii, datum)` | Send message with payload |
| `cursus_mittere_nuntium(cursus, destinatio_id, nuntius)` | Send pre-built message |
| `cursus_mittere_simplex(cursus, mittens_id, destinatio_id, genus_nuntii)` | Send message without payload |
| `cursus_processare(cursus, maximus_nuntii)` | Process pending messages |
| `cursus_processare_entitatem(cursus, entitas_id, maximus_nuntii)` | Process for one entity |
| `cursus_numerus_pendentium(cursus)` | Count total pending |
| `cursus_numerus_pendentium_entitatis(cursus, entitas_id)` | Count pending for entity |
| `cursus_habet_capsam(cursus, entitas_id)` | Check if entity has mailbox |
| `cursus_obtinere_capsam(cursus, entitas_id)` | Get entity's mailbox |
| `cursus_vacare(cursus)` | Clear all mailboxes |
| `cursus_imprimere(cursus)` | Debug print |

## tempestivum.h

| Function | Description |
|---|---|
| `administrator_tempestivi_creare(piscina, repositorium, cursus, intern)` | Create timer admin |
| `tempestivum_creare(admin, destinatio_id, genus_nuntii, intervallum_ms, repetere)` | Create timer |
| `tempestivum_creare_cum_delay(admin, destinatio_id, genus_nuntii, intervallum_ms, delay_ms, repetere)` | Create timer with delay |
| `tempestivum_pausare(admin, tempestivum_id)` | Pause timer |
| `tempestivum_resumere(admin, tempestivum_id)` | Resume timer |
| `tempestivum_annullare(admin, tempestivum_id)` | Cancel timer |
| `tempestivum_processare(admin)` | Process all timers (call per frame) |
| `tempestivum_numerus_activorum(admin)` | Count active timers |
| `tempestivum_pro_destinatione(admin, destinatio_id)` | Get timers for entity |
| `tempestivum_est_activum(admin, tempestivum_id)` | Check if timer active |
| `tempestivum_imprimere(admin)` | Debug print |

## servitium.h

| Function | Description |
|---|---|
| `registrum_servitii_creare(piscina, repositorium, intern)` | Create service registry |
| `servitium_registrare(reg, titulus, entitas_id)` | Register service by ID |
| `servitium_registrare_entitatem(reg, titulus, entitas)` | Register service by entity |
| `servitium_deregistrare(reg, titulus)` | Unregister service |
| `servitium_obtinere(reg, titulus)` | Look up service entity |
| `servitium_obtinere_id(reg, titulus)` | Look up service entity ID |
| `servitium_habet(reg, titulus)` | Check if service registered |
| `servitium_numerus(reg)` | Count services |
| `servitium_discoperire(reg)` | Auto-discover from tags |
| `servitium_imprimere(reg)` | Debug print |

---

That's the actor system. Messages, mailboxes, handlers, a message bus, timers, and services. Five libraries working together to give every entity in your world the ability to communicate through structured messages. Build your handlers, register them, create timers, and call `cursus_processare` every frame. The rest takes care of itself.
