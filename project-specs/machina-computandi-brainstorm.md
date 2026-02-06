# Machina Computandi — Computational Substrate for Rhubarb

Date: 2026-02-05
Status: Brainstorm
Related: unified-state-paradigm.md, systema-paradigmata.md, actor.md
Influences: Dreams engine (Alex Evans), Anton Mikhailov "Large Arrays of Things", Eskil Steenberg, GraphBLAS, interaction nets (Lafont), HVM (Victor Taelin), TCC (Fabrice Bellard)

---

## I. Vision

Rhubarb's entity graph is not just a database — it is a computational substrate. Programs, data, relationships, and execution state all live as entities in the same graph. The system provides:

1. **Algebraic relationship types** — edges between entities obey declared algebraic laws (semiring structure), enabling sparse matrix operations for efficient graph-wide propagation and queries
2. **Double-buffered state + event sourcing** — runtime execution via immutable-old / mutable-new frame model, with frame diffs recorded as events for persistence, replay, and forking
3. **Actor model with message passing** — entities own their own state, communicate through typed messages, read the whole previous frame for queries
4. **In-process C89 compiler** — programs are compiled within the runtime, operating directly on the entity graph. Source representation lives as entities (source-as-truth, machine code as cache)
5. **Constraint spectrum** — four levels of programmer constraint, from pure declarations down to unconstrained C89, where more constraint earns more automatic optimization, parallelism, and persistence

The synthesis: each technique addresses weaknesses the others have in isolation. Event sourcing alone has unbounded log growth — content-addressed snapshots solve it. Double buffering alone is "uniformly slow" — but event diffs and sparse matrix queries avoid redundant work. Sparse matrices alone are expensive to maintain — but frame-boundary batch updates make maintenance predictable. The in-process compiler alone is a toy — but when programs are entities in an algebraically structured graph, compilation, execution, debugging, and live reload become the same operation.

The goal is not to require programmers to think in terms of semirings and interaction nets. The goal is to make writing actor code, message handlers, and constraint declarations so natural that the algebraic structure is implicit — like how shader programmers write C-like code without thinking about SIMD execution.

---

## II. Existing Infrastructure

What rhubarb already has that this builds on:

### Entity System (`entitas.h`)
- Entities with string-keyed properties, typed parsing (s32, s64, f64, b32, tempus, blobum)
- Directed relationships with IDs (Relatio: origo_id, genus, destinatio_id)
- Tags (notae) for runtime classification
- UUIDv7 identifiers, UUIDv5 for deterministic scaffolding

### Entity Repository (`entitas_repositorium.h`)
- Abstract interface (vtable pattern) for read/write
- Event-sourced CRUD (8 event types: creare, delere, ponere_proprietas, etc.)
- Replay-based initialization
- Two persistence backends: in-memory (Xar) and file (nuntium format)

### Actor System (`actor.h`, `cursus.h`, `tractator.h`)
- Nuntius: message with sender_id, genus, TabulaDispersa payload
- Capsa: per-entity mailbox (lazy allocation)
- CursusNuntiorum: message bus with frame-limited processing
- RegistrumTractatoris: two-level dispatch (entity_type × message_type → handler)

### Memory (`piscina.h`)
- Arena allocator with mark/reset
- No individual frees — lifetime management via arena scoping

### Collections
- TabulaDispersa: open-addressing hash table
- Xar: segmented dynamic array with stable indices (elements never move)
- InternamentumChorda: global string interning

### Serialization (`nuntium.h`, `persistentia.h`)
- Tag-length-value binary format
- Event log serialization/deserialization

### What's Specced But Not Built
- Double-buffered frame model (unified-state-paradigm-v2.md Phase 6)
- Per-frame arena discipline
- Debounce/WAL for high-frequency mutations
- Retention policies and event compaction

---

## III. The Computational Substrate

### III.1 — Algebraic Relationship Types

**Core idea:** Constrain relationship types to obey algebraic laws so the entity graph becomes amenable to sparse matrix operations.

Currently, Relatio is a simple directed edge (origo, genus, destinatio). This proposal adds algebraic metadata to relationship genera:

```
Genus Relationis: "damnum" (damage)
├── algebraic_add: sum          /* multiple damage sources combine by addition */
├── algebraic_mul: product      /* resistance/armor applies multiplicatively */
├── commutativity: true         /* order of damage sources doesn't matter */
├── associativity: true         /* grouping doesn't matter */
├── identity_add: 0.0           /* zero damage is identity for addition */
├── identity_mul: 1.0           /* no armor (1.0) is identity for multiplication */
├── carrier_type: f64           /* values are 64-bit floats */
└── distributive: true          /* mul distributes over add */
```

This metadata declares that "damnum" forms a semiring (f64, +, ×, 0, 1). The system can now:

- Represent all damage relationships as a **sparse matrix** where entry (i,j) = damage from entity i to entity j
- Propagate damage across the graph via **sparse matrix-vector multiply** under the (+, ×) semiring
- Do this in parallel because commutativity guarantees order-independence
- Compute multi-hop effects via matrix powers (e.g., chain reactions)

**Other semirings that arise naturally:**

| Relationship | Add Op | Mul Op | Use Case |
|---|---|---|---|
| Damage/healing | sum | product | Accumulate from sources, apply resistance |
| Transform hierarchy | compose | identity | Scene graph propagation |
| Visibility/reachability | OR | AND | "Can any path see me?" |
| Shortest path / cost | min | + | Cheapest route through graph |
| Bottleneck / capacity | max | min | Maximum flow |
| Type lattice | join (max) | meet (min) | Type checking, usual arithmetic conversions |
| Dataflow (reaching defs) | union | intersection | Compiler analysis |

**Data structure — sparse matrix as flat arrays (CSR format):**

```c
/* Matrix Sparsa — compressed sparse row representation
 * Tres tabulae planae, nihil in acervo
 */
nomen structura {
    i32     numerus_entitatum;    /* rows = columns = entity count */
    i32     numerus_arcuum;       /* number of non-zero entries */
    i32*    index_ordinum;        /* [numerus_entitatum + 1] row offsets */
    i32*    index_columnarum;     /* [numerus_arcuum] column indices */
    vacuum* valores;              /* [numerus_arcuum] edge values, typed by carrier */
    i32     magnitudo_valoris;    /* sizeof carrier type */
} MatrixSparsa;
```

**Generic sparse matrix-vector multiply:**

```c
/* Semiring operators */
nomen structura {
    vacuum (*addere)(vacuum* acc, constans vacuum* val);  /* acc = acc + val */
    vacuum (*multiplicare)(vacuum* out, constans vacuum* a, constans vacuum* b); /* out = a * b */
    vacuum (*identitas_additionis)(vacuum* out);  /* out = 0 (additive identity) */
    i32    magnitudo;  /* sizeof carrier type */
} Semiannulus;  /* semiring */

/* spmv: out[i] = Σ_j ( M[i,j] * in[j] )  under given semiring */
vacuum
matrix_sparsa_multiplicare_vectorem(
    MatrixSparsa*     matrix,
    constans vacuum*  vector_in,     /* [numerus_entitatum] */
    vacuum*           vector_ex,     /* [numerus_entitatum] */
    Semiannulus*      semiannulus);
```

**Fixpoint computation for multi-hop queries:**

```c
/* Iterare donec vector non mutet (transitive closure, dataflow, etc.)
 * Redde: numerum iterationum
 */
i32
matrix_sparsa_punctum_fixum(
    MatrixSparsa*  matrix,
    vacuum*        vector,           /* in-place, converges */
    Semiannulus*   semiannulus,
    i32            maximus_iterationum,
    Piscina*       piscina_teli);    /* scratch space */
```

**Connection to existing code:** The entity graph's relationships (currently stored as Xar of Relatio per entity) would additionally be materialized as MatrixSparsa per relationship genus. The Relatio structures remain the source of truth; the matrices are acceleration structures rebuilt at frame boundaries from the event diff.

### III.2 — Double-Buffered State + Event Sourcing

**The synthesis:** Double buffering is the runtime execution model. Event sourcing is the persistence and history model. They compose at the frame boundary.

```
Within a frame:
┌──────────────────────────────────────────┐
│  old_state (const)  →  actor ticks  →  new_state (write)  │
│  old_graph (const)  →  queries      →  messages (append)  │
│                                                            │
│  All actors run in parallel (no shared mutable state)      │
└──────────────────────────────────────────────────────────────┘

At frame boundary:
┌──────────────────────────────────────────┐
│  diff = new_state - old_state                              │
│  append diff to event log                                  │
│  update relationship matrices from diff                    │
│  swap buffers                                              │
│  reset per-frame arena                                     │
└──────────────────────────────────────────────────────────────┘

Over time:
┌──────────────────────────────────────────┐
│  event log = source of truth                               │
│  periodic content-addressed snapshots                      │
│  retention policies per genus                              │
│  fork = share event log up to fork point, then diverge     │
│  replay = restore snapshot + replay events                 │
└──────────────────────────────────────────────────────────────┘
```

**What each part contributes:**

| Technique | Provides | Weakness Covered By |
|---|---|---|
| Double buffering | Parallelism, determinism, no locks | Event sourcing (persistence, history) |
| Event sourcing | Replay, forking, persistence, undo | Content-addressed snapshots (log growth) |
| Content-addressed snapshots | Deduplication, cheap forking | Double buffering (atomic frame transactions) |
| Sparse matrix queries | Efficient multi-hop graph analysis | Frame-boundary batch updates (maintenance cost) |

**Frame context passed to actors:**

```c
nomen structura ContextusTeli {
    constans vacuum*     status_prior;       /* read-only previous frame */
    vacuum*              status_novus;       /* write-only for own entity */
    Piscina*             piscina_teli;       /* per-frame scratch arena */
    i64                  numerus_teli;       /* frame number */
    CursusNuntiorum*     cursus;             /* for sending messages */
    EntitasRepositorium* repo;               /* for graph queries (reads old state) */
    MatrixSparsa**       matrices;           /* relationship matrices (old frame) */
    InternamentumChorda* intern;
} ContextusTeli;
```

### III.3 — Actor Model: Read Global, Write Self, Message Others

Eskil Steenberg's synthesis of double-buffered state with actor-model message passing:

**Three operations an actor can perform during its tick:**

1. **Read** — query any entity's state from the previous frame. Free, parallel-safe, const.
2. **Write self** — mutate own entity's state in the new frame. Trivially parallel (each entity owns its slot).
3. **Send message** — request another entity to change. Recipient processes the message on its next tick.

**Why messages are necessary:** In the double-buffered model, you can only write your own slot. If entity A (a sword) hits entity B (a goblin), A cannot decrement B's health. A sends B a "damnum: 10" message. B processes it on its tick, writing `new_health = old_health - 10` into its own new-frame slot.

**Entity sovereignty:** No one can change your state — they can only ask. An entity can ignore, modify, cap, or reflect a message. All state-change logic for an entity lives in one place: its tick function / message handlers. This eliminates the scatter of "who can mutate this?" across the codebase.

**Connection to existing code:** The actor.h / cursus.h system already implements message dispatch with frame-limited processing. The new element is making the double-buffered constraint explicit — handlers receive ContextusTeli that enforces read-old / write-new discipline.

### III.4 — The Four-Level Constraint Spectrum

The programmer's input surface, from most constrained (most automatic) to least constrained (most manual):

**Level 1: Pure Declarations**
```
Entity type with fields and taxonomy tags.
System infers: persistence, serialization, validation, default UI, graph relationships.
Programmer writes: schema only.
```

Example — an entity type declaration:
```
Genus: "Ordo" (Order)
├── proprietates:
│   ├── pretium: f64, required
│   ├── status: enum(pendens, confirmatus, cancellatus)
│   └── tempus_creatus: tempus, auto
├── relationes:
│   ├── "continet" → Genus::LinimentumOrdinis (1..N, required)
│   └── "a_cliente" → Genus::Cliens (1..1, required)
├── taxonomia:
│   └── Mercatura::Transactio::Ordo
└── retentio: FOREVER
```

**Level 2: Message Handlers and Constraints**
```
Pure functions over entity state with typed messages.
System handles: parallelism, event sourcing, persistence, dispatch.
Programmer writes: domain logic only.
```

Example:
```c
/* Handler: when Ordo receives "cancellare" message */
vacuum
ordo_tractare_cancellare(ContextusTeli* ctx, Entitas* ego, Nuntius* nuntius)
{
    /* Read own state from previous frame */
    chorda* status = entitas_proprietas_capere(ego, intern("status"));

    /* Guard: can only cancel pending orders */
    si (chorda_aequat(*status, intern("pendens")))
    {
        /* Write own state for next frame */
        scribe_proprietatem(ctx, intern("status"), intern("cancellatus"));

        /* Message other entities */
        Xar* pagamenta = repo_capere_relatae(ctx->repo, ego, intern("pagamentum"));
        per (i32 i = 0; i < xar_numerus(pagamenta); i++)
        {
            Entitas* pag = *(Entitas**)xar_obtinere(pagamenta, i);
            mitte_nuntium(ctx, pag->id, nuntius_creare_simplex(ctx, intern("refundere")));
        }
    }
}
```

**Level 3: Shader-Like C with Entity Graph Access**
```
Arbitrary C89 computation with read/write APIs.
System knows: inputs, outputs. Can schedule, persist.
Programmer writes: algorithms that operate on entities.
```

Example — a compiler pass that operates on AST entities:
```c
/* Type propagation pass: bottom-up through expression tree */
vacuum
propagare_typos(ContextusTeli* ctx, MatrixSparsa* arbor_filius)
{
    /* Use sparse matrix under (max, max) semiring to propagate types up */
    Semiannulus lattice_typorum = { .addere = typus_join, .multiplicare = typus_join, ... };

    matrix_sparsa_punctum_fixum(
        arbor_filius,
        vector_typorum,
        &lattice_typorum,
        X,  /* max 10 iterations */
        ctx->piscina_teli);
}
```

**Level 4: Plain C89 Program**
```
Takes bytes in, produces bytes out. Black box to the system.
System handles: wiring input/output to entity graph via serialization.
Programmer writes: whatever they want.
```

Example:
```c
/* Standard C program — system wires stdin/stdout to entity graph */
integer principale(integer argc, character** argv)
{
    /* Read from stdin (system serialized entities here) */
    /* Do arbitrary computation */
    /* Write to stdout (system deserializes back to entities) */
}
```

**The key insight:** You don't need to rewrite level 4 programs to get benefits. The system wraps them. But as you migrate code upward through the levels — adding type declarations, converting to message handlers, declaring algebraic properties — the system progressively gives you more: automatic persistence, parallelism, incremental recomputation, live reload, debugging introspection.

### III.5 — In-Process C89 Compiler

**Why:** The system needs a way for users to write and execute code without leaving the entity graph. Instead of a scripting language, use C89 — the language rhubarb is already written in.

**Architecture:**

```
Source text (entity) → Lexer → AST (entities) → Type checker → IR (entities) → Code gen → Machine code (entity)
```

Every stage produces entities in the graph. The AST is not a separate data structure — it's entities with relationships (parent-child, type-of, refers-to). This means:

- The debugger is a graph query ("show me AST nodes of type FunctionDecl")
- Type checking is constraint propagation over the type relationship matrix
- Live reload = change source entity → invalidate dependent AST entities → re-reduce affected subgraph
- Machine code is a cached materialization, not the source of truth

**References:**
- TCC (Tiny C Compiler) — existence proof of in-process C compilation, fast, small
- QBE — small SSA-based backend (~10k lines C)
- MIR — lightweight JIT framework in C

**First milestone:** Parse C89 source into AST entities in the graph. Type checking and code generation are later phases. Even just having the AST as queryable entities in the graph is immediately useful for tooling, analysis, and the C parser you mentioned wanting to build.

### III.6 — Interaction Nets and the Lambda Calculus Connection

**Longer-term theoretical foundation.** Interaction nets (Lafont 1990) are a graph rewriting model where:

- Agents (entities) connect through ports (typed relationship edges)
- Computation = local rewrites: two connected agents replaced by new configuration
- Strong confluence: order of rewrites doesn't matter → inherently parallel

**Connection to the architecture:**

- Entities are agents. Relationships are port connections. Message handlers are interaction rules.
- The double-buffered model handles the execution: scan old graph for active pairs, fire non-overlapping interactions in parallel, write results to new frame.
- Strong confluence is the algebraic property that guarantees parallel execution is valid — the same property the semiring constraints encode for relationship types.

**Connection to linear logic:**

- Interaction nets are the computational interpretation of linear logic
- Linear logic has multiplicative connectives (like ×) and additive connectives (like +)
- These are the two operations of a semiring
- Constraining relationships to obey semiring laws = building a linear logic over the entity graph

**Practical implication:** If the C compiler's intermediate representation is an interaction net stored as entities in the graph, then compilation, optimization, and execution are all the same operation — graph rewriting. This is what HVM (Victor Taelin) does for lambda calculus evaluation using flat arrays on GPUs.

**This is not a near-term implementation target.** It's the theoretical end-state that validates the algebraic relationship framework. The near-term work (sparse matrices, double buffering, C compiler) converges toward this whether or not you explicitly implement interaction nets.

### III.7 — Constraint Solving and Prolog-Like Rules

The Regulae Integritatis from systema-paradigmata.md, reimagined through the algebraic lens:

**Datalog-style rules** are fixpoint computations over boolean semirings. "All ancestors of X" = transitive closure = matrix power under (OR, AND). This connects the rule engine directly to the sparse matrix infrastructure.

**Type checking** for the C compiler = constraint propagation over a type lattice. The usual arithmetic conversions are a (max, max) semiring over the type ordering char < short < int < long < float < double.

**Integrity rules** ("every Order must have at least one LineItem") = checking that certain entries in the relationship matrix are non-zero. This is a sparse matrix query.

**Inference rules** ("if Truck model is F150, wheel_count = 4") = conditional matrix entries that get activated when pattern matches.

All of these are instances of the same underlying operation: sparse matrix operations under domain-specific semirings, iterated to fixpoint when multi-hop reasoning is needed.

### III.8 — Taxonomy as Ontology

The moderate realist taxonomy from systema-paradigmata.md gains computational significance in this framework:

**Taxonomy tags determine algebraic properties.** When you declare a relationship genus belongs to a certain place in the taxonomy, the system looks up the algebraic laws that apply. A relationship tagged as `Quantitas::Additiva` automatically gets the (+, ×, 0, 1) semiring. A relationship tagged as `Ordo::Hierarchia` automatically gets tree-structure optimizations.

**Intent tracking becomes typed.** Intents (final causes) are entities in the taxonomy. A mutation event carries not just "what changed" but "why" — linked to an intent entity. The system can query "all changes motivated by intent X" using the relationship matrix.

**The taxonomy is itself in the graph.** Genus definitions, algebraic properties, inference rules — all stored as entities with relationships. The system bootstraps from this self-description. Adding a new genus with new algebraic properties = adding entities to the taxonomy subgraph, after which the sparse matrix infrastructure automatically handles the new relationship type.

---

## IV. The Stack

```
┌─────────────────────────────────────────────────────────────┐
│  Level 4: Application Paradigms                             │
│  (Inbox, Commands, State Machines, Projections, Networking) │
│  → systema-paradigmata.md                                   │
├─────────────────────────────────────────────────────────────┤
│  Level 3: Machina Computandi (this document)                │
│  Algebraic relationships · Double-buffered frames           │
│  Actor execution · Sparse matrix queries                    │
│  In-process C compiler · Constraint solver                  │
│  Program-as-entity · Four-level constraint spectrum         │
├─────────────────────────────────────────────────────────────┤
│  Level 2: Unified State Paradigm                            │
│  Entity-as-actor · Auto-persistence · Event sourcing        │
│  WAL/debounce · Retention policies · Per-frame arenas       │
│  → unified-state-paradigm-v2.md                             │
├─────────────────────────────────────────────────────────────┤
│  Level 1: Core Libraries                                    │
│  piscina · xar · tabula_dispersa · chorda · internamentum   │
│  nuntium · persistentia · entitas · actor · reactor         │
└─────────────────────────────────────────────────────────────┘
```

Level 1 exists. Level 2 is specced with partial implementation. Level 3 is this document. Level 4 is specced in systema-paradigmata.md.

---

## V. What Needs to Be Built

### Near-term (extends existing infrastructure)

**V.1 — MatrixSparsa library**
- CSR sparse matrix as three flat arrays
- Arena-allocated (piscina)
- spmv parameterized by Semiannulus (semiring)
- Fixpoint iteration for multi-hop queries
- Build from entity relationship data
- A few standard semirings: boolean (OR,AND), arithmetic (+,×), tropical (min,+), type lattice (max,max)

New files: `include/matrix_sparsa.h`, `lib/matrix_sparsa.c`
Tests: `probationes/probatio_matrix_sparsa.c`

**V.2 — Double-buffered frame execution**
- TelusBuffer with two state snapshots
- ContextusTeli passed to actor handlers
- Frame boundary: diff → events → matrix update → swap → arena reset
- Enforce read-old / write-self discipline

New files: `include/telum.h`, `lib/telum.c`
Extends: actor.h (handlers receive ContextusTeli), cursus.h (frame-aware dispatch)

**V.3 — Algebraic relationship metadata on Genus**
- Genus entities gain algebraic property declarations
- Relationship genus → semiring mapping
- Automatic MatrixSparsa construction per relationship genus at frame boundary
- Incremental update from frame diff events

Extends: entitas.h (genus properties), entitas_repositorium_impl.c (matrix maintenance)

**V.4 — Frame-boundary event diff**
- Compare old and new state snapshots
- Emit minimal event set to event log
- Feed diff to matrix update logic
- Content-address periodic snapshots for cheap forking/persistence

New files: `include/differentia.h`, `lib/differentia.c`
Extends: persistentia.h (new event batch format)

### Medium-term (new capabilities)

**V.5 — C89 lexer and parser producing entity AST**
- Lex C89 source into token stream
- Parse into AST stored as entities in the graph
- Relationships: parent-child, type-of, refers-to, scope-contains
- Foundation for type checking, analysis, code generation

Could extend existing arbor (parser) infrastructure or be built fresh.
New files: `include/compilator.h`, `lib/compilator_lexema.c`, `lib/compilator_syntaxis.c`

**V.6 — Type checking via matrix operations**
- Type compatibility as sparse matrix under type lattice semiring
- Name resolution via scope-contains relationship matrix
- Reaching definitions / liveness via boolean semiring on CFG matrix
- Proof of concept for "compiler passes as graph algebra"

Extends: compilator, matrix_sparsa

**V.7 — Constraint/rule engine**
- Datalog-style rules as fixpoint computation over boolean matrix
- Integrity rules as matrix queries
- Inference rules as conditional matrix updates
- Connects to Regulae Integritatis in systema-paradigmata.md

New files: `include/regula.h`, `lib/regula.c`

### Longer-term (the full vision)

**V.8 — Machine code generation**
- Backend targeting host architecture (aarch64 for Apple Silicon)
- "Compile C to memory, call it" (TCC-style)
- Machine code as cached entity (invalidated when source changes)
- Live reload via recompilation

**V.9 — Source-as-truth execution model**
- Programs stored as entity graphs, not compiled binaries
- JIT compilation as optimization (materialized view of source graph)
- Edit source entity → automatic invalidation → re-reduce → updated behavior
- Four-level constraint spectrum fully operational

**V.10 — Persistence to disk and S3**
- Content-addressed snapshot chunks (hash as key)
- S3 = content-addressed blob store, natural fit
- rsync-style sync between machines (compare chunk lists, upload delta)
- Event log + snapshots = complete persistence story

**V.11 — Interaction net execution model**
- Programs as interaction nets in the entity graph
- Computation as graph rewriting with strong confluence
- Optimal sharing (lambda calculus via interaction nets)
- Massive parallelism from confluence guarantee

---

## VI. Connections to Existing Specs

### unified-state-paradigm-v2.md
- Phase 6 (Double Buffer) → subsumed by V.2 (TelusBuffer)
- Auto-persistence → frame-boundary event diff (V.4) replaces manual save calls
- Per-frame arena → part of ContextusTeli
- Retention policies → operate on the event log from V.4

### systema-paradigmata.md
- Regulae Integritatis → implemented via matrix queries (V.7)
- Taxonomy → gains algebraic significance (III.8), drives matrix construction
- Projections → graph-to-filetree transformation, using matrix operations for dependency analysis
- State Machines → entities with transition relationships, queryable via matrix ops
- Inbox → tasks generated by constraint violations detected via matrix queries

### actor.md
- Message handlers → Level 2 of constraint spectrum, receive ContextusTeli
- Batch actors → parallelized via double-buffer guarantee
- Actor state machine → hierarchical states as entity relationships
- Genus-defined behavior → algebraic properties inherited from genus taxonomy

---

## VII. Design Principles

1. **Flat arrays, not heap graphs.** Entities in Xar, matrices in CSR arrays, everything arena-allocated. No pointer chasing across the heap. (Anton Mikhailov, Casey Muratori, Sean Barrett)

2. **Indices, not pointers.** Entity refs are generation-counted indices. Matrix entries are (row, col) integer pairs. Stable across frames, serializable, safe against use-after-free. (Anton Mikhailov)

3. **Source is truth, machine code is cache.** The structured representation (AST entities, interaction net) is authoritative. Compiled output is a materialized view that can be regenerated. (Smalltalk image philosophy, but with C89 performance floor)

4. **Constraints earn capabilities.** The more structure you give the system, the more it gives back. Pure declarations get automatic everything. Plain C gets wiring only. (Shader programming model)

5. **Speed-of-light budgeting.** Fixed entity limits decided up front. 100k entities × 8 bytes per relationship entry = matrix fits in L2 cache. Know your numbers. (Mark Cerny via Alex Evans)

6. **Assert-driven invariants.** Algebraic laws are enforced by asserts. If a relationship declared commutative produces different results when operands are reordered, that's a hard crash. (Dreams engine)

7. **Iteration speed above all.** The debugger, visualizer, and live reload are not afterthoughts — they fall out of the architecture because programs and state are inspectable entities in the same graph. (Alex Evans: "iteration count is the only number that matters")

8. **The log cabin.** Build your own, keep it small, understand all of it. No dependencies. Latin naming. Long time horizons. (Eskil Steenberg, CLAUDE.md philosophy)

---

## VIII. Open Questions

1. **Matrix rebuild vs incremental update.** At frame boundary, is it cheaper to rebuild matrices from scratch (simple, brute force, "uniformly slow") or incrementally update from the event diff (complex, but potentially much faster for large graphs with few changes per frame)? Probably start with rebuild and optimize later.

2. **Message delivery timing.** Are messages delivered next frame (one frame latency, simpler) or within the same frame after sender's tick (zero latency, requires ordering)? Eskil does next-frame. Dreams did within-frame for gadgets (topological evaluation). Probably start with next-frame and add within-frame as an opt-in for specific relationship types.

3. **Sparse matrix format.** CSR is good for row-wise access (spmv). COO is good for construction and modification. Do we keep both representations (COO for building during frame, convert to CSR at boundary for queries)? Or use a single format that's adequate for both?

4. **How many matrices?** One per relationship genus. If there are 50 relationship genera, that's 50 sparse matrices. Is there a point where the overhead of maintaining many matrices exceeds the benefit? Can related genera share a matrix with a discriminator?

5. **Entity array layout.** Currently entities are individually allocated from piscina with properties in Xar. For the double-buffer model, entities probably need to be in a contiguous flat array (SOA or AOS) so the "copy old to new" operation is a memcpy. This is a significant refactor of entitas.h. When to do it?

6. **C compiler scope.** Full C89 compliance from the start, or a useful subset (no preprocessor, no complex declarators, etc.) that covers the common cases and expands later? Probably subset — cover function definitions, basic types, control flow, structs, and entity graph API calls first.

7. **Taxonomy bootstrap.** The taxonomy describes algebraic properties of relationship types. But the taxonomy itself is stored as entities with relationships. How does the system bootstrap? Probably a hardcoded "kernel" taxonomy that defines the meta-genera (Genus, Relatio, Semiannulus, etc.) from which everything else is defined.

---

## IX. References

- **Alex Evans & Anton Mikhailov** — Wukash Podcast. Dreams engine architecture: double-buffered state, assert-driven development, content-addressable UGC, crash reproduction, functional reactive game state. Anton's "Large Arrays of Things": flat arrays, generation counters, intrusive lists, nil sentinels, free lists.
- **Eskil Steenberg** — Double-buffered state + actor model with explicit message passing. Parallelizable entity systems. "Dependable C" (integer-only determinism).
- **Casey Muratori** — Handmade approach. Compression-oriented programming. No dependencies. Understand your code.
- **Sean Barrett** — stb single-header libraries. Simplicity as a design value.
- **GraphBLAS** — Graph algorithms as sparse linear algebra over user-defined semirings. SuiteSparse:GraphBLAS (Tim Davis) is the reference implementation.
- **Interaction Nets** — Yves Lafont (1990). Graph rewriting with strong confluence. Computational interpretation of linear logic.
- **HVM** — Victor Taelin. Interaction-net-based massively parallel evaluator. Flat array representation. C/CUDA.
- **TCC** — Fabrice Bellard. Tiny C Compiler. In-process compilation, fast, small. Existence proof for "compile C and run it in memory."
- **QBE** — Quentin Carbonneaux. Small compiler backend (~10k lines). SSA, register allocation, instruction selection.
- **MIR** — Vladimir Makarov. Lightweight JIT compiler framework in C.
- **Datalog / Semi-naive Evaluation** — Fixpoint computation over facts = iterative sparse matrix-vector multiply under boolean semiring.
- **Dreams / LBP Data Pipeline** — Content-addressed storage, rsync-style chunking, Merkle trees with copy-on-write, flattened dependency lists.
