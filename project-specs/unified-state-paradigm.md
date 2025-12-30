# Unified State Paradigm

Date: 2025-12-29
Status: Draft
Related: actor.md, brainstorm in unified-state-paradigm-brainstorm.md

## Summary

Refactor widget and entity state management into a unified paradigm where:
- Everything is a node in a graph database
- Every node is an actor that can send/receive messages
- Every node has event-sourced history
- Double-buffered frame model enables deterministic replay and parallelism
- Immediate durability with intelligent debouncing

This is "Plan 9, but everything is a node" instead of "everything is a file."

## Core Principles

### 1. Everything Is A Node

Every piece of state in the system is a node in the graph database:
- Widgets are nodes
- Screens (schirmata) are nodes
- Domain entities are nodes
- Even relationships may be nodes (for mutation ownership)

Nodes have:
- UUID/address for message targeting
- Genus (type within taxonomy)
- Tags (runtime classification)
- Event-sourced mutation history
- Relationships (named, directional, with backlinking)

### 2. Double-Buffered Actor Model

Each frame tick:
1. All actors have **read access** to complete previous frame state (global)
2. Each actor has **write access** only to its own next frame state
3. Messages between actors processed in deterministic order within frame
4. Frame completes, buffers swap

Benefits:
- Deterministic replay (same inputs = same outputs)
- Massive parallelism potential (actors are independent)
- Clear ownership (only you can write your state)

### 3. Immediate Durability with WAL Debouncing

State changes persist immediately, but high-frequency changes (keystrokes, frame-by-frame game state) use a write-ahead log:
1. Change written to WAL immediately (crash safe)
2. Debounce timer aggregates rapid changes
3. Single consolidated event written to main event log
4. If app quits before debounce timeout, WAL recovers pending changes

### 4. Moderate Realist Taxonomy

Not nominalist (arbitrary names) but moderate realist (real genera):
- Core genera defined upfront, extended carefully
- Application-specific genera added per project
- Every node is an instance of some species within the taxonomy
- Intents themselves are typed within intent taxonomy

### 5. Intent Tracking (Final Cause)

Every mutation traceable to explicit intent:
- "Why does this code/data exist?"
- Intents form DAG/graph (sub-intents, related intents)
- UI prompts for intent at key moments
- Code annotations link to intent IDs
- Event metadata carries intent provenance

## State Classification

### Persistent State
- Saved on every mutation (with debouncing)
- Restored on app reopen
- Examples: which page is open, user preferences, document content

### Ephemeral State
- Lives only for current frame or session
- Not persisted
- Examples: animations, hover states, computed view models

### Derivable State
- Can be recomputed from events/other state
- May be cached but not authoritative
- Deterministic (integer math only, no floats for cross-machine consistency)

## Event Types

1. **CRUD Events** - Automatic, already implemented for entities
2. **Domain Events** - User-defined, semantic ("UserCompletedCheckout")
3. **Messages** - Inter-actor communication
4. **UI Events** - User input from outside environment

All event types must support:
- Replay (for deterministic restore)
- Streaming (for observers on other machines)

## Retention Policies

Per-genus or per-instance configuration:

| Policy | Description | Use Case |
|--------|-------------|----------|
| Forever | Keep all events indefinitely | Audit trails, undo history |
| Snapshot + N | Keep snapshot + last N events | Game entity frame state |
| TTL | Discard events older than duration | Logs, metrics |
| Manual | "Close books" compacts to snapshot on demand | Storage management |

## Memory Architecture

### Per-Frame Arena
- Passed as context to actors
- Wiped/reset each frame
- For ephemeral allocations, query results, view models

### Global Arenas
- String interning (append-only, cross-session stable IDs)
- Persist offsets/relative pointers to disk
- Stable references across sessions

### Lazy Paging
- Not all entities in RAM at once
- Load on query or message receipt
- Opaque to actor code (just query what you need)

## API Sketch

### Reading Previous Frame State

```c
/* Query-style access to global state */
nomen structura interrogatio_eventus {
    /* ... */
} interrogatio_eventus;

/* Returns results with pointers into frame arena */
interrogatio_eventus* quaere_entia(
    contextus_teli* contextus,   /* frame context */
    genus_id genus,               /* filter by genus */
    /* ... other filters */
);

/* Or direct by ID */
vacuum* lege_entis(contextus_teli* contextus, uuid_t id);
```

### Writing Next Frame State

```c
/* Copy-on-write: actor receives copy of current state */
/* Mutate via setters, conceptually pure function */

vacuum scribe_campum(
    contextus_teli* contextus,
    uuid_t self_id,
    campus_id campus,
    vacuum* valor
);
```

### Message Passing

```c
/* Tagged union message type */
nomen structura nuntius {
    nuntius_genus genus;
    uuid_t mittens;      /* sender */
    uuid_t recipiens;    /* recipient (or NIHIL for broadcast) */
    unio {
        /* genus-specific payloads, or just references to global state */
    } corpus;
} nuntius;

/* Point-to-point */
vacuum mitte_nuntium(contextus_teli* ctx, uuid_t recipiens, nuntius* n);

/* Broadcast to query match */
vacuum diffunde_nuntium(contextus_teli* ctx, genus_id genus, nuntius* n);
```

## Widget/Schirmata Model

### Current Problems
1. Manual save calls (easy to forget)
2. Widget singletons hydrate to screen state (complex, buggy)
3. Inconsistent persistence across widget types
4. Same widget on different screens not properly isolated

### New Model
- Each **screen** (schirmata instance) is its own actor
- Each **widget instance** on a screen is its own actor
- Widget actor has:
  - Persistent state (which page open, scroll position, etc.)
  - Ephemeral state (animations, hover, computed view model)
- View model built via query/signal system (Sputnik + STML)

### Migration Path
1. Define widget genera with persistent/ephemeral field annotations
2. Convert widget singletons to actor-per-screen-instance
3. Remove manual save calls - mutations auto-persist
4. Add debouncing for high-frequency state (text input, scroll)

## First Milestone

Convert one widget type end-to-end:
1. Define widget genus with field annotations
2. Implement per-screen-instance actors
3. Auto-persist on mutation with debouncing
4. Verify state restores correctly on app reopen
5. Verify different screens have isolated widget state

Then iterate through remaining 6-7 widgets + schirmata.

## Open Questions

1. Exact mechanism for persistent vs ephemeral field declaration (annotations vs separate structs?)
2. Message processing order within frame (entity ID order? topological? explicit priority?)
3. Copy-on-write vs full double buffer - performance profiling needed
4. Query language design for complex view model projections
5. Relationship ownership (source actor, target actor, or relationship-as-actor?)

## Dependencies

- piscina.h (arena allocation)
- nuntium.h (serialization)
- Entity repository (graphus_persistens.c?)
- Existing actor/message infrastructure

## Next Steps

1. Explore existing codebase together to understand current implementation
2. Design field annotation/separation mechanism
3. Prototype single widget conversion
4. Measure persistence correctness and performance
5. Iterate on API based on ergonomics
