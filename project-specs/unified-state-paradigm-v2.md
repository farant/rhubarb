# Unified State Paradigm v2 - Implementation Spec

Date: 2025-12-29
Status: Draft
Based on: Codebase exploration + interview brainstorm

## Executive Summary

Refactor the existing widget, entity, and actor systems into a unified paradigm where:
- Every widget instance is an actor with event-sourced state
- Automatic persistence on mutation (no manual save calls)
- Per-screen widget instances replace singleton hydration pattern
- Double-buffered frame model for determinism and parallelism
- WAL + debouncing for high-frequency changes
- Per-frame arenas to fix memory leak issues

---

## Part 1: Current State Analysis

### 1.1 What Exists Today

#### Entity Repository (`entitas_repositorium_impl.c`, 2121 lines)
- **Event-sourced**: All mutations emit events (CREARE, PONERE_PROPRIETAS, ADDERE_RELATIO, etc.)
- **Graph structure**: Entities with properties, relationships, tags, backlinking
- **Persistence backends**: `persistentia_memoria` (in-memory) and `persistentia_nuntium` (file)
- **Replay-based init**: Constructor replays all events to rebuild state
- **Idempotent scaffolding**: UUIDv5 for deterministic IDs

#### Actor System (`actor.c`, `cursus.c`, `tractator.c`)
- **Nuntius**: Message with sender_id, genus, TabulaDispersa payload
- **Capsa**: Per-entity mailbox (lazy allocation)
- **CursusNuntiorum**: Message bus with `cursus_processare(cursus, max_messages)`
- **RegistrumTractatoris**: Two-level hash dispatch (entity_type, message_type) → handler
- **Frame-based processing**: Already supports processing N messages per frame

#### Widget System (`widget.h`, `schirmata.c`)
- **Vtable pattern**: Widget struct with reddere/tractare_eventum function pointers
- **ManagerWidget**: Handles focus, click-to-focus, TAB cycling
- **Factory registry**: Widgets created by tag name via RegistrumWidget

#### Schirmata (`schirmata.c`, 2153 lines)
- **10 screens**: Independent ManagerWidget per screen
- **Singleton hydration**: One widget instance per type, state saved/restored on screen switch
- **Mode flags**: Each screen tracks which widget type is active (modus_biblia_visus, etc.)
- **Per-screen libro status**: SchirmaLibroStatus tracks cursor position, vim mode per screen

#### Memory Management (`piscina.c`)
- **Arena allocator**: Linear allocation, no individual frees
- **Mark/reset pattern**: `piscina_notare()` / `piscina_reficere()` for scoped allocations
- **Problem**: Single global piscina used for everything, never reset = memory leaks
- **String interning**: `InternamentumChorda` with dedicated piscina

### 1.2 Current Pain Points

| Problem | Current Behavior | Files Affected |
|---------|-----------------|----------------|
| **Manual saves** | Must call `libro_salvare_*`, `navigator_salvare_status`, etc. | All widget files |
| **Inconsistent persistence** | Pagina doesn't save, CalendarioVisus doesn't save | pagina.c, calendario_visus.c |
| **Only LibroPaginarum debounces** | Other widgets save synchronously | libro_paginarum.c |
| **Singleton hydration complexity** | Widget state saved/restored on every screen switch | schirmata.c lines 1137-1179 |
| **Memory leaks** | Global piscina never freed | Entire codebase |
| **No per-frame arena** | Temporary allocations accumulate forever | All rendering code |

### 1.3 What Already Works Well

- Event sourcing infrastructure is solid
- Actor/message system is well-designed with frame-based limits
- Entity repository supports graph queries and relationships
- Nuntium serialization is efficient and proven
- String interning reduces memory and enables fast comparisons

---

## Part 2: Target Architecture

### 2.1 Core Principle: Widget Instance = Actor = Entity

Every widget instance becomes a first-class entity in the graph:

```
Before: Schirmata owns singleton BibliaVisus, hydrates per-screen state
After:  Each screen owns BibliaVisus entity actor with its own event history
```

**Entity Structure for Widget:**
```c
Entitas: "BibliaVisus::screen_0"
├─ genus: "BibliaVisus"
├─ proprietates:
│   ├─ in_toc: "false"
│   ├─ liber_currens: "0"
│   ├─ capitulum_currens: "1"
│   └─ index_paginae: "0"
├─ relationes:
│   └─ "pertinet_ad" → "Schirma::0"
└─ notae: ["#widget", "#screen_0"]
```

### 2.2 Automatic Persistence on Mutation

Replace manual save calls with automatic event emission:

```c
/* OLD: Manual save */
biblia_visus->liber_currens = 5;
biblia_visus_salvare_status(visus, repo, entitas_id);  /* Easy to forget! */

/* NEW: Setter emits event automatically */
biblia_visus_ponere_liber(visus, 5);  /* Internally calls repo->proprietas_ponere */
```

**Implementation**: Each widget getter/setter wraps entity repository calls:

```c
vacuum biblia_visus_ponere_liber(BibliaVisus* visus, i32 liber) {
    visus->liber_currens = liber;  /* Update in-memory */

    /* Emit event (repo handles persistence) */
    character valor[XVI];
    snprintf(valor, XVI, "%d", liber);
    visus->ctx->repo->proprietas_ponere(
        visus->ctx->repo->datum,
        visus->entitas,
        "liber_currens",
        valor
    );
}
```

### 2.3 Debouncing with WAL

For high-frequency changes (keystrokes, scroll position):

**New struct: MutatioDebounce**
```c
nomen structura {
    chorda*  entitas_id;
    chorda*  clavis;           /* Property key */
    chorda*  valor_pendente;   /* Pending value */
    f64      tempus_mutationis; /* When last changed */
    b32      in_wal;           /* Written to WAL? */
} MutatioDebounce;

nomen structura {
    Xar*     mutationes;       /* Array of MutatioDebounce */
    Piscina* piscina_wal;      /* WAL arena */
    f64      intervallum_ms;   /* Debounce interval (e.g., 500ms) */
} DebounceManager;
```

**Flow:**
1. Mutation comes in → write to WAL immediately (crash safe)
2. Start/reset debounce timer for that property
3. On timeout → consolidate into single PONERE_PROPRIETAS event
4. On app quit → flush all pending to main event log
5. On crash recovery → replay WAL to recover pending changes

### 2.4 Double-Buffered Frame Model

**New structs:**
```c
nomen structura {
    TabulaDispersa* entitates;      /* id → Entitas* snapshot */
    Piscina*        piscina;        /* Arena for this frame's data */
    i64             numerus_teli;   /* Frame number */
} TelusSnapshot;

nomen structura {
    TelusSnapshot*  telum_currens;   /* Read from this */
    TelusSnapshot*  telum_proximum;  /* Write to this */
    Piscina*        piscina_teli;    /* Per-frame arena (reset each frame) */
} TelusBuffer;
```

**Frame tick:**
```c
vacuum telum_processare(TelusBuffer* buffer, CursusNuntiorum* cursus) {
    /* 1. Swap buffers */
    TelusSnapshot* temp = buffer->telum_currens;
    buffer->telum_currens = buffer->telum_proximum;
    buffer->telum_proximum = temp;

    /* 2. Reset per-frame arena */
    piscina_reficere(buffer->piscina_teli, buffer->notatio_initialis);

    /* 3. Copy current → next as baseline */
    _telum_copiare(buffer->telum_proximum, buffer->telum_currens);

    /* 4. Process messages (writes go to telum_proximum) */
    cursus_processare(cursus, C);  /* Up to 100 messages */
}
```

**Actor read/write:**
```c
/* Read: Access previous frame (immutable) */
Entitas* lege_entitatem(ContextusTeli* ctx, chorda* id) {
    return tabula_dispersa_capere(ctx->telum_currens->entitates, id);
}

/* Write: Modify next frame (own entity only) */
vacuum scribe_proprietatem(ContextusTeli* ctx, chorda* clavis, chorda* valor) {
    ASSERT(ctx->entitas_propria != NIHIL);  /* Can only write own state */
    entitas_proprietas_ponere(ctx->entitas_propria, clavis, valor);
}
```

### 2.5 Per-Screen Widget Instances

**Remove singleton hydration pattern:**

```c
/* OLD (schirmata.c) */
nomen structura Schirmata {
    BibliaVisus* biblia_visus;  /* Single instance, hydrated per screen */
    /* ... */
};

/* NEW */
nomen structura Schirma {
    ManagerWidget* manager;
    TabulaDispersa* widgets;    /* id → Widget* instances for THIS screen */
    /* No mode flags - widgets exist or don't */
};
```

**Widget creation per screen:**
```c
vacuum schirma_creare_widget(Schirma* schirma, constans character* genus, constans character* id) {
    /* Create entity for this widget instance */
    character entitas_id[CXXVIII];
    snprintf(entitas_id, CXXVIII, "%s::screen_%d", genus, schirma->index);

    Entitas* entitas = repo->entitas_scaffoldare(repo->datum, genus, entitas_id);

    /* Create widget, passing entity reference */
    Widget* widget = registrum_widget_creare(reg, genus, entitas);

    tabula_dispersa_inserere(schirma->widgets, id, widget);
}
```

### 2.6 Retention Policies

**New entity property: `__retention_policy`**

```c
nomen enumeratio {
    RETENTION_FOREVER    = 0,  /* Keep all events */
    RETENTION_SNAPSHOT_N = 1,  /* Snapshot + last N events */
    RETENTION_TTL        = 2,  /* Discard older than duration */
    RETENTION_MANUAL     = 3   /* Compact on explicit command */
} RetentionPolicy;
```

**Per-genus defaults (in Genus entity):**
```
Genus::BibliaVisus
├─ __retention_policy: "FOREVER"     /* Keep all navigation history */

Genus::GameEntity
├─ __retention_policy: "SNAPSHOT_N"
├─ __retention_n: "100"              /* Keep last 100 frames */
```

**Compaction process:**
```c
vacuum compactare_entitatem(EntitasRepositorium* repo, chorda* id) {
    Entitas* e = repo->capere_entitatem(repo->datum, id);
    RetentionPolicy policy = _capere_retention_policy(e);

    si (policy == RETENTION_SNAPSHOT_N) {
        i32 n = _capere_retention_n(e);
        Xar* eventus = repo->legere_eventus_entitatis(repo->datum, id);

        si (eventus->numerus > n) {
            /* Create snapshot entity */
            _creare_snapshot(repo, e);
            /* Delete old events */
            _delere_eventus_ante(repo, id, eventus->numerus - n);
        }
    }
}
```

---

## Part 3: Migration Plan

### Phase 1: Per-Frame Arena (Fix Memory Leaks)

**Files to modify:**
- `include/piscina.h` - Add frame arena concept
- `lib/schirmata.c` - Pass frame piscina to widgets
- `lib/widget.c` - Accept frame piscina in render context

**New pattern:**
```c
/* In main loop */
Piscina* piscina_teli = piscina_generare_dynamicum("telum", MMMMXCVI);
PiscinaNotatio notatio = piscina_notare(piscina_teli);

/* Each frame */
piscina_reficere(piscina_teli, notatio);  /* Reset to start */
schirmata_reddere(schirmata, tabula, piscina_teli);
```

**Estimate:** Small change, high impact on memory usage

### Phase 2: Widget Entity Binding

**Files to modify:**
- `include/widget.h` - Add `Entitas* entitas` to Widget struct
- `lib/widget.c` - Pass entity to widget creation
- Each widget header/impl - Add entity reference, convert getters/setters

**New Widget struct:**
```c
nomen structura Widget {
    vacuum*               datum;
    Entitas*              entitas;          /* NEW: Bound entity */
    FunctioReddere        reddere;
    FunctioTractareEventum tractare_eventum;
    i32 x, y, latitudo, altitudo;
} Widget;
```

**Estimate:** Medium effort, touches all widget files

### Phase 3: Auto-Persistence

**Files to modify:**
- Each widget impl - Replace direct field access with setter calls
- `lib/entitas_repositorium_impl.c` - Already handles event emission

**Pattern:**
```c
/* Before */
visus->liber_currens = new_value;
biblia_visus_salvare_status(visus, repo, id);

/* After */
biblia_visus_ponere_liber(visus, new_value);  /* Auto-persists */
```

**Estimate:** Medium effort, many small changes

### Phase 4: Remove Singleton Hydration

**Files to modify:**
- `include/schirmata.h` - Remove singleton widget pointers
- `lib/schirmata.c` - Create widget instances per screen
- Remove all `_salvare_widget_status` / `_carcare_widget_status` calls

**Estimate:** Large effort, significant refactor of schirmata.c

### Phase 5: Debounce Manager

**New files:**
- `include/debounce.h`
- `lib/debounce.c`

**Integration points:**
- Widget setters check if property is high-frequency
- Main loop calls `debounce_processare()` to flush timeouts

**Estimate:** New code, medium effort

### Phase 6: Double Buffer (Optional/Future)

**New files:**
- `include/telum.h` - Frame buffer management
- `lib/telum.c` - Buffer swapping, snapshot copying

**Estimate:** Large effort, architectural change

---

## Part 4: Detailed File Changes

### 4.1 widget.h Changes

```c
/* Line ~135, add entitas field */
nomen structura Widget {
    vacuum*               datum;
    Entitas*              entitas;          /* ADD */
    ContextusTeli*        ctx_teli;         /* ADD: Frame context */
    FunctioReddere        reddere;
    FunctioTractareEventum tractare_eventum;
    i32 x, y, latitudo, altitudo;
} Widget;

/* Line ~37, add frame piscina to context */
nomen structura ContextusWidget {
    Piscina*             piscina;
    Piscina*             piscina_teli;      /* ADD: Per-frame arena */
    InternamentumChorda* intern;
    EntitasRepositorium* repo;
    /* ... rest unchanged ... */
} ContextusWidget;
```

### 4.2 biblia_visus.h Changes (Example Widget)

```c
/* Add entity reference */
nomen structura {
    Piscina* piscina;
    Entitas* entitas;                       /* ADD */
    constans Biblia* biblia;
    /* ... fields unchanged ... */
} BibliaVisus;

/* Add typed setters */
vacuum biblia_visus_ponere_liber(BibliaVisus* visus, i32 liber);
vacuum biblia_visus_ponere_capitulum(BibliaVisus* visus, i32 capitulum);
vacuum biblia_visus_ponere_in_toc(BibliaVisus* visus, b32 in_toc);

/* Remove manual save function */
/* DELETE: biblia_visus_salvare_status */
/* DELETE: biblia_visus_carcare_status */
```

### 4.3 schirmata.h Changes

```c
/* Remove singleton widget pointers */
nomen structura Schirmata {
    ContextusWidget* ctx;
    Schirma schirmae[SCHIRMATA_MAXIMUS];
    i32 index_currens;
    b32 praefixum_activum;
    f64 tempus_praefixum;

    LibroPaginarum* libro;                  /* KEEP: Shared document store */

    /* DELETE all singleton widget pointers:
    ArcCaeli*        arx_caeli;
    ThemaVisus*      thema_visus;
    ... etc ...
    */
} Schirmata;

/* Schirma gets widget instances */
nomen structura Schirma {
    ManagerWidget* manager;
    TabulaDispersa* widgets;                /* ADD: Per-screen widgets */
    b32 initiatus;
    /* DELETE all modus_* flags */
} Schirma;
```

### 4.4 New File: debounce.h

```c
#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "entitas_repositorium.h"

nomen structura MutatioDebounce {
    chorda*  entitas_id;
    chorda*  clavis;
    chorda*  valor_pendente;
    f64      tempus_mutationis;
    b32      scripta_ad_wal;
} MutatioDebounce;

nomen structura DebounceManager {
    Xar*                 mutationes;
    Piscina*             piscina_wal;
    EntitasRepositorium* repo;
    f64                  intervallum_ms;
} DebounceManager;

DebounceManager* debounce_creare(Piscina* piscina, EntitasRepositorium* repo, f64 intervallum_ms);
vacuum debounce_registrare(DebounceManager* dm, chorda* entitas_id, chorda* clavis, chorda* valor);
vacuum debounce_processare(DebounceManager* dm, f64 tempus_currens);
vacuum debounce_effundere(DebounceManager* dm);  /* Flush all pending */

#endif /* DEBOUNCE_H */
```

---

## Part 5: Migration Checklist

### Phase 1: Per-Frame Arena
- [ ] Add `piscina_teli` to ContextusWidget
- [ ] Create frame arena in main loop with mark/reset
- [ ] Pass frame piscina through render chain
- [ ] Audit temporary allocations, move to frame arena
- [ ] Verify memory usage stabilizes

### Phase 2: Widget Entity Binding
- [ ] Add `Entitas*` to Widget struct
- [ ] Modify `registrum_widget_creare` to accept entity
- [ ] Update BibliaVisus to store entity reference
- [ ] Update NavigatorEntitatum to store entity reference
- [ ] Update CalendarioVisus to store entity reference
- [ ] Update remaining widgets

### Phase 3: Auto-Persistence
- [ ] Create typed setters for BibliaVisus
- [ ] Remove `biblia_visus_salvare_status`
- [ ] Create typed setters for NavigatorEntitatum
- [ ] Remove `navigator_entitatum_salvare_status`
- [ ] Repeat for all widgets
- [ ] Verify state persists across app restart

### Phase 4: Remove Singleton Hydration
- [ ] Add `TabulaDispersa* widgets` to Schirma
- [ ] Create widget instances per screen on init
- [ ] Remove singleton pointers from Schirmata
- [ ] Remove mode flags from Schirma
- [ ] Remove `_salvare_widget_status` / `_carcare_widget_status`
- [ ] Update screen switching to just change focus
- [ ] Verify different screens have isolated state

### Phase 5: Debounce Manager
- [ ] Implement debounce.h / debounce.c
- [ ] Integrate with LibroPaginarum (replace existing debounce)
- [ ] Add debounce for scroll position changes
- [ ] Add debounce for text input
- [ ] Verify crash recovery from WAL

### Phase 6: Double Buffer (Future)
- [ ] Design telum.h API
- [ ] Implement buffer swapping
- [ ] Modify actor handlers to use frame context
- [ ] Verify deterministic replay

---

## Part 6: Risk Mitigation

### Risk: Breaking Existing Persistence
**Mitigation:** Keep old save/load functions during migration, verify entity-based persistence works before removing them.

### Risk: Performance Regression
**Mitigation:** Per-frame arena should improve performance. Profile before/after Phase 1.

### Risk: Screen State Isolation Bugs
**Mitigation:** Write tests for multi-screen scenarios before Phase 4. Use probatio framework.

### Risk: WAL Corruption
**Mitigation:** Implement checksums in WAL entries. Test crash recovery explicitly.

---

## Part 7: Success Criteria

1. **No manual save calls** in any widget code
2. **Memory usage stable** over long sessions (no leaks)
3. **Each screen has independent widget state** that persists correctly
4. **App crash recovers** pending changes from WAL
5. **CalendarioVisus state persists** (currently doesn't save at all)
6. **Existing tests pass** after migration

---

## Appendix: Key File Locations

| Component | Header | Implementation | Tests |
|-----------|--------|----------------|-------|
| Widget base | include/widget.h | lib/widget.c | - |
| Schirmata | include/schirmata.h | lib/schirmata.c | - |
| Entity repo | include/entitas_repositorium.h | lib/entitas_repositorium_impl.c | probationes/probatio_entitas_repositorium.c |
| Actor system | include/actor.h | lib/actor.c | probationes/probatio_actor.c |
| Message bus | include/cursus.h | lib/cursus.c | probationes/probatio_actor_integratio.c |
| Arena | include/piscina.h | lib/piscina.c | probationes/probatio_piscina.c |
| BibliaVisus | include/biblia_visus.h | lib/biblia_visus.c | - |
| Navigator | include/navigator_entitatum.h | lib/navigator_entitatum.c | - |
| Calendario | include/calendario_visus.h | lib/calendario_visus.c | - |
